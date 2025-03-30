#include "port.h"
#include "identifier.h"
#include <parse/default/symbol.h>
#include <parse/default/number.h>

namespace parse_verilog {

port::port() {
    debug_name = "port";
    name = "";
    direction = NONE;
    type = IMPLICIT;
    is_vector = false;
}

port::port(tokenizer &tokens, void *data) {
    debug_name = "port";
    name = "";
    direction = NONE;
    type = IMPLICIT;
    is_vector = false;
    parse(tokens, data);
}

port::~port() {
}

void port::parse(tokenizer &tokens, void *data) {
    tokens.syntax_start(this);

    // First check for direction
    if (tokens.is_next("input")) {
        tokens.next();
        direction = INPUT;
    } else if (tokens.is_next("output")) {
        tokens.next();
        direction = OUTPUT;
    } else if (tokens.is_next("inout")) {
        tokens.next();
        direction = INOUT;
    }

    // Then check for data type
    if (tokens.is_next("wire")) {
        tokens.next();
        type = WIRE;
    } else if (tokens.is_next("reg")) {
        tokens.next();
        type = REG;
    } else if (tokens.is_next("integer")) {
        tokens.next();
        type = INTEGER;
    }

    // Check for vector dimensions before the name
    // This handles case like: input [7:0] data
    if (tokens.is_next("[")) {
        is_vector = true;
        tokens.next();

        // Parse MSB expression
        tokens.increment(true);
        tokens.expect<parse_expression::expression>();

        if (tokens.decrement(__FILE__, __LINE__, data)) {
            msb = std::make_shared<parse_expression::expression>(tokens, 0, data);
        }

        // Expect colon
        tokens.increment(true);
        tokens.expect(":");

        if (tokens.decrement(__FILE__, __LINE__, data)) {
            tokens.next();
        }

        // Parse LSB expression
        tokens.increment(true);
        tokens.expect<parse_expression::expression>();

        if (tokens.decrement(__FILE__, __LINE__, data)) {
            lsb = std::make_shared<parse_expression::expression>(tokens, 0, data);
        }

        // Expect closing bracket
        tokens.increment(true);
        tokens.expect("]");

        if (tokens.decrement(__FILE__, __LINE__, data)) {
            tokens.next();
        }
    }

    // Parse identifier
    tokens.increment(true);
    tokens.expect<identifier>();

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        name = identifier(tokens, data).name;
    }

    // Check for vector dimensions after the name
    // This handles cases like: input data [7:0]
    if (!is_vector && tokens.is_next("[")) {
        is_vector = true;
        tokens.next();

        // Parse MSB expression
        tokens.increment(true);
        tokens.expect<parse_expression::expression>();
        tokens.expect<parse::number>();  // Also allow simple numbers

        if (tokens.decrement(__FILE__, __LINE__, data)) {
            if (tokens.found<parse_expression::expression>()) {
                msb = std::make_shared<parse_expression::expression>(tokens, 0, data);
            } else if (tokens.found<parse::number>()) {
                // Create an expression from a number
                std::string num = tokens.next();
                // We'll handle this better later
                msb = std::make_shared<parse_expression::expression>();
                msb->valid = true;
            }
        }

        // Expect colon
        tokens.increment(true);
        tokens.expect(":");

        if (tokens.decrement(__FILE__, __LINE__, data)) {
            tokens.next();
        }

        // Parse LSB expression
        tokens.increment(true);
        tokens.expect<parse_expression::expression>();
        tokens.expect<parse::number>();  // Also allow simple numbers

        if (tokens.decrement(__FILE__, __LINE__, data)) {
            if (tokens.found<parse_expression::expression>()) {
                lsb = std::make_shared<parse_expression::expression>(tokens, 0, data);
            } else if (tokens.found<parse::number>()) {
                // Create an expression from a number
                std::string num = tokens.next();
                // We'll handle this better later
                lsb = std::make_shared<parse_expression::expression>();
                lsb->valid = true;
            }
        }

        // Expect closing bracket
        tokens.increment(true);
        tokens.expect("]");

        if (tokens.decrement(__FILE__, __LINE__, data)) {
            tokens.next();
        }
    }

    tokens.syntax_end(this);
}

bool port::is_next(tokenizer &tokens, int i, void *data) {
    // Ports can start with a direction qualifier, a type, or directly with an identifier
    // Also allow a port to start with a '[' for vector dimensions before the name
    return tokens.is_next("input", i) || 
           tokens.is_next("output", i) || 
           tokens.is_next("inout", i) ||
           tokens.is_next("wire", i) || 
           tokens.is_next("reg", i) || 
           tokens.is_next("integer", i) ||
           tokens.is_next("[", i) ||
           identifier::is_next(tokens, i, data);
}

void port::register_syntax(tokenizer &tokens) {
    if (!tokens.syntax_registered<port>()) {
        tokens.register_syntax<port>();
        tokens.register_token<parse::symbol>();
        tokens.register_token<parse::number>();
        identifier::register_syntax(tokens);
        parse_expression::expression::register_syntax(tokens);
    }
}

std::string port::to_string(std::string tab) const {
    if (!valid) return tab + "invalid port";

    std::string result = tab;

    // Direction
    switch (direction) {
        case INPUT: result += "input "; break;
        case OUTPUT: result += "output "; break;
        case INOUT: result += "inout "; break;
        default: break;
    }

    // Type
    switch (type) {
        case WIRE: result += "wire "; break;
        case REG: result += "reg "; break;
        case INTEGER: result += "integer "; break;
        default: break;
    }

    // Name and vector dimensions
    result += name;
    if (is_vector && msb && lsb) {
        result += "[" + msb->to_string() + ":" + lsb->to_string() + "]";
    }

    return result;
}

parse::syntax *port::clone() const {
    return new port(*this);
}

} 