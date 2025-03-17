# SimpScript Makefile

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
INCLUDES = -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Output binary
TARGET = $(BIN_DIR)/simpscript

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Build target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

# Clean build files
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run examples
run-hello: $(TARGET)
	@echo "Running hello.simp example:"
	@./$(TARGET) examples/hello.simp

run-string-arrays: $(TARGET)
	@echo "Running string_arrays.simp example:"
	@./$(TARGET) examples/string_arrays.simp

run-math-logic: $(TARGET)
	@echo "Running math_logic.simp example:"
	@./$(TARGET) examples/math_logic.simp

# Run REPL
repl: $(TARGET)
	@echo "Starting SimpScript interactive mode (REPL):"
	@./$(TARGET)

# Build with CMake (alternative build method)
cmake-build:
	@mkdir -p build
	@cd build && cmake .. && make

# Run tests
test: $(TARGET)
	@echo "Running tests..."
	@# Add test commands here

# Help
help:
	@echo "SimpScript Makefile"
	@echo "Available targets:"
	@echo "  all              - Build the SimpScript interpreter"
	@echo "  clean            - Remove build files"
	@echo "  run-hello        - Run the hello.simp example"
	@echo "  run-string-arrays - Run the string_arrays.simp example"
	@echo "  run-math-logic   - Run the math_logic.simp example"
	@echo "  repl             - Start the interactive mode"
	@echo "  cmake-build      - Build using CMake"
	@echo "  test             - Run tests"
	@echo "  help             - Show this help message"

.PHONY: all clean directories run-hello run-string-arrays run-math-logic repl cmake-build test help 