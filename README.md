# SimpScript

SimpScript is a simple programming language with both conventional and human-like syntax. It aims to make programming more intuitive by allowing expressions to be written in a natural language style.

## Features

- Basic operations: arithmetic, logical, comparison, and assignment
- Support for arrays and strings
- Custom keywords for I/O:
  - `show` for printing
  - `shownl` for printing with a newline
  - `nextl` for newline
  - `ask` for input
- Dual syntax support:
  - Conventional: `if a > b`
  - Human-like: `if a greater than b`

## Implementation Status

This is a work-in-progress implementation. Currently implemented features:

- [x] Lexer for tokenizing source code
- [x] Parser for creating abstract syntax trees
- [x] Interpreter for executing SimpScript programs
- [x] Variables and assignment
- [x] Basic expressions and operators
- [x] String concatenation 
- [x] Basic I/O functions
- [x] Conditionals (if statements)
- [x] Simple loops (while loops)
- [x] Functions
- [x] Arrays

Known limitations:
- Some complex nesting of expressions may not work correctly
- Error handling could be improved
- Multi-line programs with complex formatting may have parsing issues

## File Extension

SimpScript files use the `.simp` extension.

## Example

```simp
# This is a SimpScript example

# Variable declarations
number = 10
name = "SimpScript"

# Output with both conventional and human-like syntax
show "Hello, " + name
shownl "The number is: " + number

# Conditional statements
if number greater than 5
    shownl "Number is greater than 5"
endif

# Same condition with conventional syntax
if number > 5
    shownl "Number is greater than 5"
endif

# Getting user input
shownl "Enter your name:"
userName = ask

# Arrays
numbers = [1, 2, 3, 4, 5]
shownl "The third number is: " + numbers[2]  # Zero-indexed
```

## Building and Running

### Using Make (Recommended)

```bash
make
bin/simpscript examples/simple.simp
```

### Using CMake

1. Build using CMake:
```bash
mkdir build
cd build
cmake ..
make
```

2. Run the interpreter:
```bash
./simpscript path/to/file.simp
```

## Debug Mode

To see the tokens being processed:

```bash
bin/simpscript examples/simple.simp --debug
```

## Interactive Mode (REPL)

To use the interactive REPL:

```bash
bin/simpscript
``` 