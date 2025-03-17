# Building SimpScript

This document explains how to build and use the SimpScript interpreter.

## Prerequisites

To build SimpScript, you need:

- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.10 or higher
- Make or equivalent build system

## Building from Source

1. Clone the repository:

```bash
git clone https://github.com/the-shashwat-singh/SimpScript.git
cd SimpScript
```

2. Create a build directory and run CMake:

```bash
mkdir build
cd build
cmake ..
```

3. Build the project:

```bash
make
```

This will create an executable called `simpscript` in the build directory.

## Running SimpScript

### Running a Script File

To run a SimpScript file with the `.simp` extension:

```bash
./simpscript path/to/your_script.simp
```

### Using the Interactive Mode (REPL)

To start the interactive mode (Read-Eval-Print Loop):

```bash
./simpscript
```

In interactive mode, you can type SimpScript code and see the results immediately. Type `exit` to quit.

## Examples

Try running the examples included in the `examples` directory:

```bash
./simpscript ../examples/hello.simp
./simpscript ../examples/string_arrays.simp
./simpscript ../examples/math_logic.simp
```

## Troubleshooting

If you encounter build errors:

1. Make sure you have the correct compiler version
2. Check that CMake is installed and in your PATH
3. Try removing the build directory and rebuilding from scratch

## License

SimpScript is released under the MIT License. See the LICENSE file for details. 