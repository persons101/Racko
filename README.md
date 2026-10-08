# C++/C# game replicating the Milton-Bradley game Racko

## Single-file coverage report

With `coverage.info`, `lcov`, `genhtml`, and Python available on `PATH`, run:

```sh
make coverageSingleHTML
```

This generates `html_coverage/report.html` as a self-contained report with
in-page navigation across the LCOV HTML pages. The default `*msys64*` exclusion
removes MinGW compiler headers from the report; override it with
`COVERAGE_EXCLUDE` when using a different toolchain. The intermediate report
directory, input tracefile, and final HTML path can be changed with
`COVERAGE_HTML_DIR`, `COVERAGE_INFO`, and `COVERAGE_SINGLE_HTML`.
