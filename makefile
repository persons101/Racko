SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests
SCRIPT_DIR = scripts

CXX ?= g++
PYTHON ?= python
LCOV ?= lcov
GENHTML ?= genhtml
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -fdiagnostics-color=always
DEBUG = 
COVERAGEFLAGS = -O0 --coverage
COVERAGE_INFO ?= coverage.info
COVERAGE_HTML_DIR ?= $(BIN_DIR)/coverage_html
COVERAGE_SINGLE_HTML ?= html_coverage/report.html
COVERAGE_EXCLUDE ?= *msys64*

.PHONY: all build buildWithCoverage tests testsWithCoverage coverage coverageSingleHTML clean

# Find all C++ source files
CPP_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
TEST_ROOT_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_SRC_SOURCES := $(wildcard $(TEST_DIR)/$(SRC_DIR)/*.cpp)

CATCH_SOURCE := $(TEST_DIR)/catch_amalgamated.cpp
CATCH_OBJECT := $(TEST_DIR)/$(OBJ_DIR)/catch_amalgamated.o
# Generate corresponding object file names in the object directory
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))

TEST_ROOT_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(TEST_DIR)/$(OBJ_DIR)/%.o,$(TEST_ROOT_SOURCES))
TEST_ROOT_OBJECTS := $(filter-out $(CATCH_OBJECT), $(TEST_ROOT_OBJECTS))
TEST_SRC_OBJECTS := $(patsubst $(TEST_DIR)/$(SRC_DIR)/%.cpp,$(TEST_DIR)/$(OBJ_DIR)/%.o,$(TEST_SRC_SOURCES))

TEST_OBJECTS := $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS))
TEST_OBJECTS += $(TEST_ROOT_OBJECTS) $(TEST_SRC_OBJECTS)


all: build clean

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TEST_DIR)/$(OBJ_DIR):
	mkdir -p $@



# Pattern rule to compile .cpp files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

$(TEST_DIR)/$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_DIR)/$(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

$(TEST_SRC_OBJECTS): $(TEST_DIR)/$(OBJ_DIR)/%.o: $(TEST_DIR)/$(SRC_DIR)/%.cpp | $(TEST_DIR)/$(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

build: $(OBJECTS) | $(BIN_DIR) $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) $^ -o $(BIN_DIR)/Racko.exe

buildWithCoverage: CXXFLAGS += $(COVERAGEFLAGS)
buildWithCoverage: $(OBJECTS) | $(BIN_DIR) $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/RackoCoverage.exe

tests: $(TEST_OBJECTS) $(CATCH_OBJECT)| $(TEST_DIR)/$(OBJ_DIR) $(OBJ_DIR) $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) $(TESTFLAGS) $^ -o $(BIN_DIR)/$@.exe
	./$(BIN_DIR)/$@.exe

testsWithCoverage: CXXFLAGS += $(COVERAGEFLAGS)
testsWithCoverage: $(TEST_OBJECTS) $(CATCH_OBJECT) | $(TEST_DIR)/$(OBJ_DIR) $(OBJ_DIR) $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) $(TESTFLAGS) $^ -o $(BIN_DIR)/$@.exe
	./$(BIN_DIR)/$@.exe

coverage: clean
	$(MAKE) buildWithCoverage
	$(MAKE) testsWithCoverage
	./$(BIN_DIR)/testsWithCoverage.exe

coverageSingleHTML: coverage | $(BIN_DIR)
	$(LCOV) --remove $(COVERAGE_INFO) "$(COVERAGE_EXCLUDE)" --output-file $(BIN_DIR)/coverage-filtered.info
	$(GENHTML) $(BIN_DIR)/coverage-filtered.info --output-directory $(COVERAGE_HTML_DIR)
	$(PYTHON) $(SCRIPT_DIR)/bundle_coverage.py $(COVERAGE_HTML_DIR) --output $(COVERAGE_SINGLE_HTML)
	$(MAKE) cleanHTML

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(TEST_DIR)/$(OBJ_DIR)/*

cleanHTML:
	rm -rf $(SRC_DIR)/*.html
	rm -rf $(TEST_DIR)/*.html