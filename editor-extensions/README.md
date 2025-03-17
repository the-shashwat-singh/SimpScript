# SimpScript Editor Support

This directory contains support for syntax highlighting and theming for the SimpScript language in various text editors and IDEs.

## Available Extensions

### Visual Studio Code

The VS Code extension provides:
- Syntax highlighting for SimpScript files
- Code folding
- Bracket matching
- Auto-closing pairs
- Dark and light themes optimized for SimpScript

[See detailed instructions](./vscode/README.md)

### Sublime Text

The Sublime Text package provides:
- Syntax highlighting for SimpScript files
- Custom color scheme optimized for SimpScript

[See detailed instructions](./sublimetext/README.md)

## Installation

Each editor has its own installation process. Please refer to the README in each directory for detailed instructions.

## Preview

When installed, SimpScript code will appear with proper syntax highlighting:

```simp
# This is a SimpScript example

# Variable declarations and output
name = "World"
shownl "Hello, " + name

# Conditionals with human-like syntax
if name equal to "World"
    shownl "Hello, Earth!"
endif

# Functions
function add(a, b)
    return a + b
endfunction

result = add(5, 10)
shownl "5 + 10 = " + result
```

## Contributing

Feel free to contribute additional editor support or improvements to existing extensions by submitting pull requests.

## License

All editor extensions are released under the MIT License. See the LICENSE file for details. 