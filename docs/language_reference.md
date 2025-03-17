# SimpScript Language Reference

SimpScript is a simple programming language with both conventional and human-like syntax. It's designed to be intuitive and easy to learn for beginners.

## File Extension

SimpScript files use the `.simp` extension.

## Comments

Comments start with the `#` character and continue to the end of the line:

```simp
# This is a comment
```

## Variables

Variables are dynamically typed and don't require declaration. They're created when first assigned a value:

```simp
name = "John"      # String
age = 30           # Integer
height = 1.75      # Float
isStudent = true   # Boolean
```

## Data Types

SimpScript supports the following data types:

- **Integer**: Whole numbers like `42`, `-17`, `0`
- **Float**: Decimal numbers like `3.14`, `-0.5`, `1.0`
- **String**: Text enclosed in double quotes like `"Hello, World!"`
- **Boolean**: `true` or `false`
- **Array**: Collection of values like `[1, 2, 3]` or `["apple", "banana", "cherry"]`
- **Nil**: Represents the absence of a value

## Operators

### Arithmetic Operators

- Addition: `+`
- Subtraction: `-`
- Multiplication: `*`
- Division: `/`
- Modulo (remainder): `%`

### Assignment Operators

- Simple assignment: `=`
- Addition assignment: `+=`
- Subtraction assignment: `-=`
- Multiplication assignment: `*=`
- Division assignment: `/=`

### Comparison Operators

SimpScript supports both conventional and human-like comparison operators:

| Conventional | Human-like     | Description       |
|--------------|----------------|-------------------|
| `==`         | `equals`       | Equal to          |
| `!=`         | `isnt`         | Not equal to      |
| `>`          | `greater than` | Greater than      |
| `<`          | `less than`    | Less than         |
| `>=`         | `at least`     | Greater or equal  |
| `<=`         | `at most`      | Less than or equal|

Examples:

```simp
# Conventional syntax
if a > b
    shownl "a is greater than b"
endif

# Human-like syntax
if a greater than b
    shownl "a is greater than b"
endif
```

### Logical Operators

- `and` - Logical AND
- `or` - Logical OR
- `not` - Logical NOT

## Control Structures

### If Statements

```simp
if condition
    # statements
endif

# With else clause
if condition
    # statements
else
    # statements
endif
```

### While Loops

```simp
while condition
    # statements
endwhile
```

### For Loops

```simp
for initialization; condition; increment
    # statements
endfor
```

Example:

```simp
for i = 0; i < 10; i = i + 1
    shownl i
endfor
```

## Functions

### Function Definition

```simp
function name(parameter1, parameter2, ...)
    # statements
    return value  # optional
endfunction
```

### Function Call

```simp
result = name(argument1, argument2, ...)
```

## Input and Output

### Output

- `show` - Print to standard output without a newline
- `shownl` - Print to standard output with a newline
- `nextl` - Insert a newline

```simp
show "Hello"       # Prints "Hello" without a newline
shownl "World!"    # Prints "World!" with a newline
shownl "Line 1" + nextl + "Line 2"  # Prints two lines
```

### Input

- `ask` - Read a line from standard input

```simp
name = ask         # Reads a line and stores it in 'name'
```

## Arrays

### Array Creation

```simp
numbers = [1, 2, 3, 4, 5]
names = ["Alice", "Bob", "Charlie"]
```

### Array Access

Arrays are zero-indexed:

```simp
first = numbers[0]     # Gets the first element (1)
second = names[1]      # Gets the second element ("Bob")
```

### Array Modification

```simp
numbers[2] = 30        # Changes the third element to 30
```

### Array Methods

- `size()` - Returns the number of elements in the array

```simp
count = numbers.size()  # Returns 5 for [1, 2, 3, 4, 5]
```

## String Operations

### String Concatenation

```simp
firstName = "John"
lastName = "Doe"
fullName = firstName + " " + lastName  # "John Doe"
```

### String Methods

- `length()` - Returns the number of characters in the string
- `charCode()` - Returns the ASCII code of a character
- `fromCharCode(code)` - Returns a character from an ASCII code

```simp
len = name.length()       # Returns the length of the string
code = "A".charCode()     # Returns 65
char = char.fromCharCode(65)  # Returns "A"
```

## Examples

### Hello World

```simp
shownl "Hello, World!"
```

### Simple Calculator

```simp
shownl "Enter the first number:"
a = ask
shownl "Enter the second number:"
b = ask

shownl "Sum: " + (a + b)
shownl "Difference: " + (a - b)
shownl "Product: " + (a * b)
shownl "Quotient: " + (a / b)
```

### Counting from 1 to 10

```simp
for i = 1; i <= 10; i = i + 1
    shownl i
endfor
```

### Function Example

```simp
function factorial(n)
    if n <= 1
        return 1
    endif
    return n * factorial(n - 1)
endfunction

shownl "Factorial of 5 is: " + factorial(5)
``` 