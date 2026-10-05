#!/usr/bin/env python3
"""Bundle an LCOV genhtml directory into one self-contained HTML file."""

from __future__ import annotations

import argparse
import base64
import html
import json
import mimetypes
import re
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import quote, unquote, urlsplit


def _data_uri(path: Path) -> str:
    mime_type = mimetypes.guess_type(path.name)[0] or "application/octet-stream"
    encoded = base64.b64encode(path.read_bytes()).decode("ascii")
    return f"data:{mime_type};base64,{encoded}"


def _inline_css(css: str, report_dir: Path, css_path: Path) -> str:
    url_pattern = re.compile(r"url\(\s*(['\"]?)(.*?)\1\s*\)", re.IGNORECASE)

    def replace_url(match: re.Match[str]) -> str:
        value = match.group(2).strip()
        parsed = urlsplit(value)
        if parsed.scheme or parsed.netloc or value.startswith("#"):
            return match.group(0)

        asset_path = (css_path.parent / unquote(parsed.path)).resolve()
        try:
            asset_path.relative_to(report_dir)
        except ValueError as exc:
            raise ValueError(f"CSS asset escapes report directory: {value}") from exc
        if not asset_path.is_file():
            raise FileNotFoundError(f"CSS asset not found: {asset_path}")
        return f"url({_data_uri(asset_path)})"

    return url_pattern.sub(replace_url, css)


class _PageBundler(HTMLParser):
    def __init__(self, report_dir: Path, page_path: Path, pages: set[str]) -> None:
        super().__init__(convert_charrefs=False)
        self.report_dir = report_dir
        self.page_path = page_path
        self.pages = pages
        self.output: list[str] = []
        self.skip_tag: str | None = None
        self.injected_navigation = False

    def _attributes(self, attrs: list[tuple[str, str | None]]) -> str:
        rendered = []
        for name, value in attrs:
            if value is None:
                rendered.append(f" {name}")
            else:
                rendered.append(f' {name}="{html.escape(value, quote=True)}"')
        return "".join(rendered)

    def _local_path(self, value: str) -> Path | None:
        parsed = urlsplit(html.unescape(value))
        if parsed.scheme or parsed.netloc or value.startswith("//"):
            return None
        path = (self.page_path.parent / unquote(parsed.path)).resolve()
        try:
            path.relative_to(self.report_dir)
        except ValueError as exc:
            raise ValueError(f"Report asset escapes report directory: {value}") from exc
        return path

    def _navigation_href(self, href: str) -> str | None:
        parsed = urlsplit(html.unescape(href))
        if parsed.scheme or parsed.netloc or not parsed.path.lower().endswith(".html"):
            return None
        target = self._local_path(href)
        if target is None or not target.is_file():
            return None
        page_key = target.relative_to(self.report_dir).as_posix()
        if page_key not in self.pages:
            return None
        route = f"#report/{quote(page_key, safe='/')}"
        if parsed.fragment:
            route += f"#{quote(unquote(parsed.fragment), safe='')}"
        return route

    def handle_decl(self, decl: str) -> None:
        self.output.append(f"<!{decl}>")

    def handle_comment(self, data: str) -> None:
        self.output.append(f"<!--{data}-->")

    def handle_entityref(self, name: str) -> None:
        self.output.append(f"&{name};")

    def handle_charref(self, name: str) -> None:
        self.output.append(f"&#{name};")

    def handle_data(self, data: str) -> None:
        if self.skip_tag is None:
            self.output.append(data)

    def handle_starttag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        if self.skip_tag is not None:
            return

        attributes = dict(attrs)
        if tag.lower() == "link" and (attributes.get("rel") or "").lower() == "stylesheet":
            href = attributes.get("href")
            css_path = self._local_path(href) if href else None
            if css_path is not None:
                if not css_path.is_file():
                    raise FileNotFoundError(f"Stylesheet not found: {css_path}")
                css = _inline_css(css_path.read_text(encoding="utf-8"), self.report_dir, css_path)
                self.output.append(f"<style>{css}</style>")
                return

        if tag.lower() == "script" and attributes.get("src"):
            script_path = self._local_path(attributes["src"])
            if script_path is not None:
                if not script_path.is_file():
                    raise FileNotFoundError(f"Script not found: {script_path}")
                retained = [(name, value) for name, value in attrs if name.lower() != "src"]
                self.output.append(f"<script{self._attributes(retained)}>")
                self.output.append(script_path.read_text(encoding="utf-8"))
                self.output.append("</script>")
                self.skip_tag = "script"
                return

        rewritten = []
        for name, value in attrs:
            if value is not None and name.lower() == "href":
                destination = self._navigation_href(value)
                if destination is not None:
                    value = destination
            elif value is not None and name.lower() in {"src", "background"}:
                asset_path = self._local_path(value)
                if asset_path is not None and asset_path.is_file():
                    value = _data_uri(asset_path)
            rewritten.append((name, value))

        self.output.append(f"<{tag}{self._attributes(rewritten)}>")

    def handle_startendtag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        self.handle_starttag(tag, attrs)
        if self.skip_tag is None:
            self.output.append(f"</{tag}>")

    def handle_endtag(self, tag: str) -> None:
        if self.skip_tag is not None:
            if tag.lower() == self.skip_tag:
                self.skip_tag = None
            return

        if tag.lower() == "body" and not self.injected_navigation:
            self.output.append(_PAGE_NAVIGATION)
            self.injected_navigation = True
        self.output.append(f"</{tag}>")

    def bundled_html(self) -> str:
        if not self.injected_navigation:
            self.output.append(_PAGE_NAVIGATION)
        return "".join(self.output)


_PAGE_NAVIGATION = """
<script>
window.addEventListener("message", function (event) {
  if (!event.data || event.data.type !== "racko-report-fragment") return;
  const target = document.getElementById(event.data.fragment) ||
    Array.from(document.getElementsByName(event.data.fragment))[0];
  if (target) target.scrollIntoView();
});
document.addEventListener("click", function (event) {
  const link = event.target.closest("a");
  if (!link) return;
  const href = link.getAttribute("href");
  if (href && href.startsWith("#report/")) {
    event.preventDefault();
    parent.postMessage({type: "racko-report-navigation", href: href}, "*");
  }
});
</script>
"""


_VIEWER = """<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Racko coverage report</title>
<style>
html, body { margin: 0; height: 100%; font: 14px sans-serif; }
header { box-sizing: border-box; display: flex; align-items: center; gap: 1em;
  height: 3em; padding: 0 .8em; background: #eee; }
header label { font-weight: bold; }
header select { min-width: 18em; max-width: 75vw; }
iframe { border: 0; width: 100%; height: calc(100% - 3em); }
</style>
</head>
<body>
<header><label for="page">Coverage report</label><select id="page"></select></header>
<iframe id="report" sandbox="allow-scripts"></iframe>
<script>
"use strict";
const pages = __PAGES__;
const pageSelect = document.getElementById("page");
const frame = document.getElementById("report");
const pageNames = Object.keys(pages).sort();
for (const name of pageNames) {
  const option = document.createElement("option");
  option.value = name;
  option.textContent = name;
  pageSelect.appendChild(option);
}
function showPage(name, fragment, updateHistory) {
  if (!Object.prototype.hasOwnProperty.call(pages, name)) return;
  pageSelect.value = name;
  frame.onload = () => {
    if (fragment) {
      frame.contentWindow.postMessage(
        {type: "racko-report-fragment", fragment: fragment}, "*");
    }
  };
  frame.srcdoc = pages[name];
  if (updateHistory) {
    const route = "#report/" + encodeURIComponent(name) +
      (fragment ? "#" + encodeURIComponent(fragment) : "");
    history.pushState(null, "", route);
  }
}
function showRoute() {
  const match = location.hash.match(/^#report\\/([^#]+)(?:#(.*))?$/);
  if (!match) return showPage("index.html", "", false);
  let name;
  let fragment = "";
  try {
    name = decodeURIComponent(match[1]);
    fragment = match[2] ? decodeURIComponent(match[2]) : "";
  } catch (error) {
    return showPage("index.html", "", false);
  }
  showPage(name, fragment, false);
}
pageSelect.addEventListener("change", () => showPage(pageSelect.value, "", true));
window.addEventListener("message", event => {
  if (event.source !== frame.contentWindow ||
      !event.data || event.data.type !== "racko-report-navigation") return;
  const match = event.data.href.match(/^#report\\/([^#]+)(?:#(.*))?$/);
  if (!match) return;
  let name;
  let fragment = "";
  try {
    name = decodeURIComponent(match[1]);
    fragment = match[2] ? decodeURIComponent(match[2]) : "";
  } catch (error) {
    return;
  }
  showPage(name, fragment, true);
});
window.addEventListener("popstate", showRoute);
showRoute();
</script>
</body>
</html>
"""


def bundle_report(report_dir: Path, output: Path) -> int:
    report_dir = report_dir.resolve()
    output = output.resolve()
    if not report_dir.is_dir():
        raise NotADirectoryError(f"LCOV HTML report directory not found: {report_dir}")
    if output.is_relative_to(report_dir):
        raise ValueError("Output HTML must be outside the genhtml report directory")

    html_files = sorted(report_dir.rglob("*.html"))
    if not html_files:
        raise FileNotFoundError(f"No HTML pages found in {report_dir}")

    page_names = {path.relative_to(report_dir).as_posix() for path in html_files}
    if "index.html" not in page_names:
        raise FileNotFoundError(f"Top-level index.html not found in {report_dir}")

    pages: dict[str, str] = {}
    for path in html_files:
        page_name = path.relative_to(report_dir).as_posix()
        parser = _PageBundler(report_dir, path, page_names)
        parser.feed(path.read_text(encoding="utf-8"))
        parser.close()
        pages[page_name] = parser.bundled_html()

    serialized_pages = json.dumps(pages, ensure_ascii=True).replace("<", "\\u003c")
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(_VIEWER.replace("__PAGES__", serialized_pages), encoding="utf-8")
    return len(pages)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("report_dir", type=Path, help="genhtml output directory")
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("html_coverage/report.html"),
        help="single-file HTML destination (default: html_coverage/report.html)",
    )
    args = parser.parse_args()
    page_count = bundle_report(args.report_dir, args.output)
    print(f"Bundled {page_count} pages into {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
