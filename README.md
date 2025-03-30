# parse_verilog

A parser for a subset of the Verilog hardware description language.

## Overview

The `parse_verilog` library provides a parser for a simplified subset of the Verilog hardware description language. It's designed to work with the broader parsing infrastructure used in the broccoli/loom toolchain.

## Supported Verilog Features

This parser supports the following Verilog features:

- Module definitions with port declarations
- Parameter definitions
- Data type declarations:
  - Wire declarations
  - Register declarations
  - Integer declarations
- Continuous assignments (assign statements)
- Procedural blocks:
  - `always` blocks with event control
  - `initial` blocks
- Control structures:
  - If-else statements
  - Case statements
  - For loops
- Expressions with proper operator precedence
- Basic comments (line and block)

## Usage

To use the parser:

```cpp
#include <parse_verilog/parse_verilog.h>

// Parse a Verilog file
tokenizer tokens;
parse_verilog::register_syntax(tokens);

// Load a file
tokens.load("your_file.v");

// Parse the file
parse_verilog::module* result = (parse_verilog::module*)parse_verilog::produce(tokens);

// Use the parsed result
if (result->valid) {
    // Access parsed data
    // ...
}
```

## Building

To build the library:

```bash
make
```

To run tests:

```bash
make tests
```

## Dependencies

- parse
- parse_expression
- parse_ucs
- common

## License

This code is subject to the terms of the project license.