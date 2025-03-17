# SimpScript Language Support for VS Code

This extension provides syntax highlighting and basic language features for the SimpScript programming language (.simp files).

## Features

- Syntax highlighting for SimpScript files
- Code folding
- Bracket matching
- Auto-closing pairs
- Two themes: SimpScript Dark and SimpScript Light

## Installation

### From VSIX File

1. Download the latest `.vsix` file from the [releases page](https://github.com/the-shashwat-singh/SimpScript/releases)
2. Open VS Code
3. Go to the Extensions view (Ctrl+Shift+X)
4. Click on the "..." menu in the top right corner
5. Select "Install from VSIX..."
6. Choose the downloaded `.vsix` file

### Manual Installation

1. Copy this folder to your VS Code extensions folder:
   - Windows: `%USERPROFILE%\.vscode\extensions`
   - macOS/Linux: `~/.vscode/extensions`
2. Restart VS Code

## Usage

1. Open any file with a `.simp` extension
2. The syntax highlighting will be applied automatically
3. To change theme:
   - Press Ctrl+K Ctrl+T (or Cmd+K Ctrl+T on macOS)
   - Select "SimpScript Dark" or "SimpScript Light"

## Example

```simp
# This is a SimpScript example with syntax highlighting

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

# Functions
function greet(person)
    shownl "Hello, " + person
endfunction

greet("World")
```

## License

This extension is released under the MIT License. See the LICENSE file for details. 