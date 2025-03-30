# parse_verilog

A parser for a subset of the Verilog hardware description language.

## Overview

The `parse_verilog` library provides a recursive-descent parser for a simplified subset of the Verilog hardware description language. It's designed to work with the broader parsing infrastructure used in the broccoli/loom toolchain. The parser converts Verilog text into an Abstract Syntax Tree (AST) representation that can be traversed, analyzed, or transformed.

## Library Structure

The library is organized into several key components:

* **Module Definitions** (`module.h/cpp`): Represents Verilog modules with ports and items
* **Declarations** (`declaration.h/cpp`): Handles variable/port declarations with types and sizes
* **Assignments** (`assign.h/cpp`, `assignment_statement.h/cpp`): Handles both continuous assignments and procedural assignments
* **Control Structures**:
  * `if_statement.h/cpp`: Implements if/else if/else constructs
  * `block_statement.h/cpp`: Represents blocks of statements
  * `trigger.h/cpp`: Handles event-triggered blocks like `always`
  * `for_loop.h/cpp`: Handles for loop constructs
* **Module Instantiation** (`module_instance.h/cpp`): Handles instantiating modules with port connections

Each component inherits from a common `parse::syntax` base class, providing a uniform interface for parsing, cloning, and string conversion operations.

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
- Module instantiation with named and positional port connections
- Expressions with proper operator precedence
- Basic comments (line and block)

## Usage

### Basic Usage

To use the parser:

```cpp
#include <parse_verilog/factory.h>
#include <parse_verilog/module.h>

// Initialize tokenizer
tokenizer tokens;
parse_verilog::register_syntax(tokens);

// Load a file
tokens.load("your_file.v");

// Parse the file
parse_verilog::module_def* result = static_cast<parse_verilog::module_def*>(parse_verilog::produce(tokens));

// Use the parsed result
if (result->valid) {
    // Access module name
    std::cout << "Module name: " << result->name << std::endl;
    
    // Access ports
    for (const auto& port : result->ports) {
        std::cout << "Port: " << port.name << std::endl;
    }
    
    // Access module items
    for (const auto& item : result->items) {
        std::cout << "Item: " << item->to_string() << std::endl;
    }
}

// Clean up
delete result;
```

### Parsing From String

```cpp
#include <parse_verilog/module.h>
#include <sstream>

// Sample Verilog code
std::string verilog_code = 
    "module test(\n"
    "  input clk,\n"
    "  output [7:0] data\n"
    ");\n"
    "  assign data = 8'hAA;\n"
    "endmodule";

// Set up tokenizer
tokenizer tokens;
parse_verilog::module_def::register_syntax(tokens);

// Insert the code
tokens.insert("test_code", verilog_code);

// Parse the module
parse_verilog::module_def module(tokens);

// Use the parsed module
if (module.valid) {
    std::cout << "Parsed module: " << module.to_string() << std::endl;
}
```

### For Loop Example

```cpp
// Parse Verilog code with a for loop
std::string verilog_code = 
    "module counter(\n"
    "  input clk,\n"
    "  output reg [7:0] out\n"
    ");\n"
    "  always @(posedge clk) begin\n"
    "    for (i = 0; i < 8; i = i + 1) begin\n"
    "      out[i] = ~out[i];\n"
    "    end\n"
    "  end\n"
    "endmodule";

tokenizer tokens;
parse_verilog::module_def::register_syntax(tokens);
tokens.insert("test_code", verilog_code);
parse_verilog::module_def module(tokens);
```

### Module Instantiation Example

```cpp
// Parse Verilog code with module instantiation
std::string verilog_code = 
    "module top(\n"
    "  input clk,\n"
    "  output [7:0] data\n"
    ");\n"
    "  counter c1(.clk(clk), .out(data));\n"
    "endmodule";

tokenizer tokens;
parse_verilog::module_def::register_syntax(tokens);
tokens.insert("test_code", verilog_code);
parse_verilog::module_def module(tokens);
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

To generate code coverage reports:

```bash
make coverage
```

## Dependencies

- parse: Core parsing infrastructure
- parse_expression: Expression parsing
- parse_ucs: Unicode support
- common: Common utilities

## License

This code is licensed under the GNU General Public License v3.0.