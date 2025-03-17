# SimpScript Language Support for Sublime Text

This package provides syntax highlighting and a custom color scheme for the SimpScript programming language (.simp files).

## Features

- Syntax highlighting for SimpScript files
- Custom color scheme optimized for SimpScript

## Installation

### Manual Installation

1. Navigate to your Sublime Text packages directory:
   - Windows: `%APPDATA%\Sublime Text\Packages\`
   - macOS: `~/Library/Application Support/Sublime Text/Packages/`
   - Linux: `~/.config/sublime-text/Packages/`
2. Create a new directory called `SimpScript`
3. Copy both files to this directory:
   - `SimpScript.sublime-syntax`
   - `SimpScript.sublime-color-scheme`
4. Restart Sublime Text

### Package Control (Future)

Once available in Package Control:

1. Open the Command Palette (Ctrl+Shift+P or Cmd+Shift+P on macOS)
2. Type "Package Control: Install Package"
3. Search for "SimpScript" and select it

## Usage

1. Open any file with a `.simp` extension
2. The syntax highlighting will be applied automatically
3. To apply the SimpScript color scheme:
   - Go to Preferences > Select Color Scheme... 
   - Choose "SimpScript"

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

This package is released under the MIT License. See the LICENSE file for details. 