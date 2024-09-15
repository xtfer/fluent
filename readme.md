# Fluent Programming Language Compiler

**Fluent** is a high-level, statically-typed programming language designed for simplicity, safety, and expressiveness. This repository contains a basic compiler for Fluent, written in C. The compiler currently translates Fluent code into C code, which can then be compiled with a C compiler like GCC.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Compilation](#compilation)
- [Usage](#usage)
  - [Compiling a Fluent Program](#compiling-a-fluent-program)
  - [Running the Compiled Program](#running-the-compiled-program)
- [Language Syntax](#language-syntax)
  - [Variables and Assignments](#variables-and-assignments)
  - [Functions](#functions)
  - [Control Flow](#control-flow)
  - [Indentation](#indentation)
- [Example](#example)
- [Limitations](#limitations)
- [Future Work](#future-work)
- [Contributing](#contributing)
- [License](#license)

---

## Features

The Fluent compiler currently supports:

- **Variables and Assignments**: Immutable (`let`) and mutable (`var`) variable declarations.
- **Binary Operations**: Arithmetic operations with correct operator precedence.
- **Function Declarations**: Definition of functions without parameters.
- **Control Flow Statements**: `if` statements with `else` clauses, `while` loops.
- **Indentation-Based Blocks**: Uses indentation to define code blocks, similar to Python.

---

## Getting Started

### Prerequisites

- **C Compiler**: GCC or Clang compatible with C99 or later.
- **Make**: For using the provided Makefile.

### Compilation

Clone the repository and navigate to the project directory:

```bash
git clone https://github.com/yourusername/fluent_compiler.git
cd fluent_compiler
```

Compile the compiler using the provided Makefile:

```bash
make
```

This will generate the `fluentc` executable in the project root.

---

## Usage

### Compiling a Fluent Program

To compile a Fluent source file (`.flu` extension) into C code:

```bash
./fluentc path/to/your_program.flu > output.c
```

### Running the Compiled Program

Compile the generated C code:

```bash
gcc -o output output.c
```

Run the executable:

```bash
./output
```

---

## Language Syntax

### Variables and Assignments

- **Immutable Variable Declaration**: `let x = 10`
- **Mutable Variable Declaration**: `var y = 20`
- **Assignment**: `x = x + y`

### Functions

- **Function Declaration**:

  ```
  func main():
      # Function body
  ```

- **Note**: Currently, functions cannot have parameters or return values.

### Control Flow

- **If Statement**:

  ```
  if condition:
      # Then block
  else:
      # Else block
  ```

- **While Loop**:

  ```
  while condition:
      # Loop body
  ```

### Indentation

- Indentation is significant and used to define code blocks.
- Use consistent spaces (e.g., 4 spaces) for indentation.

---

## Example

### Sample Fluent Program (`test.flu`)

```
func main():
    let x = 10
    var y = 20
    if x < y:
        print("x is less than y")
    else:
        print("x is greater than or equal to y")
    x = x + y
    print(x)
```

### Compiling and Running the Example

1. **Compile the Fluent Program to C Code**:

   ```bash
   ./fluentc test.flu > output.c
   ```

2. **Compile the Generated C Code**:

   ```bash
   gcc -o output output.c
   ```

3. **Run the Executable**:

   ```bash
   ./output
   ```

**Expected Output**:

```
x is less than y
30
```

---

## Limitations

- **Function Calls and Parameters**: Function calls and parameter passing are not yet implemented.
- **Standard Library Functions**: The `print` function is not implemented in the code generator. You'll need to modify the generated C code to include `printf` statements.
- **Data Types**: Only integer variables are supported. No support for floats, strings (except as literals in `print`), or other data types.
- **Error Handling**: Limited error messages and handling in the lexer and parser.
- **Semantic Analysis**: No type checking or scope management beyond basic parsing.
- **Standard Library**: No standard library functions are available.

---

## Future Work

- **Function Parameters and Calls**: Implement parsing and code generation for function parameters and function calls.
- **Type System**: Develop a type system with type inference and support for multiple data types.
- **Standard Library**: Create a standard library with common functions like `print`, `input`, etc.
- **Enhanced Error Handling**: Improve error reporting with detailed messages and recovery mechanisms.
- **Semantic Analysis**: Implement a semantic analysis phase for type checking and scope resolution.
- **Optimizations**: Add optimization passes to improve generated code performance.
- **Platform Support**: Ensure compatibility across different operating systems and architectures.

---

## Contributing

Contributions are welcome! Feel free to submit issues, fork the repository, and make pull requests.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

**Note**: This compiler is a work in progress and is intended for educational purposes. It provides a foundation for further development and learning about compiler design.
