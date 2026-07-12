#include "declaration.h"
#include <parse/default/symbol.h>
#include <parse/default/instance.h>

namespace parse_verilog {

declaration::declaration() {
	debug_name = "verilog_declaration";
	name = "";
	input = false;
	output = false;
	type = "wire";
}

declaration::declaration(tokenizer &tokens, std::any data) {
	debug_name = "verilog_declaration";
	name = "";
	input = false;
	output = false;
	type = "wire";
	parse(tokens, data);
}

declaration::~declaration() {
}

void declaration::parse(tokenizer &tokens, std::any data) {
	tokens.syntax_start(this);

	tokens.increment(false);
	tokens.expect("[");

	// Parse identifier
	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(false);
	tokens.expect("[");

	tokens.increment(false);
	tokens.expect("wire");
	tokens.expect("reg");
	tokens.expect("integer");

	tokens.increment(false);
	tokens.expect("input");
	tokens.expect("output");
	tokens.expect("inout");

	if (tokens.decrement(__FILE__, __LINE__)) {
		string dir = tokens.next();
		input = (dir == "input" or dir == "inout");
		output = (dir == "output" or dir == "inout");
	}

	type = "wire";
	if (tokens.decrement(__FILE__, __LINE__)) {
		type = tokens.next();
	}

	// Check for vector dimensions before the name
	// This handles case like: input [7:0] data
	if (tokens.decrement(__FILE__, __LINE__)) {
		tokens.next();

		// Expect closing bracket
		tokens.increment(true);
		tokens.expect("]");

		// Parse LSB expression
		tokens.increment(true);
		tokens.expect<rvalue>();

		// Expect colon
		tokens.increment(true);
		tokens.expect(":");

		// Parse MSB expression
		tokens.increment(true);
		tokens.expect<rvalue>();

		if (tokens.decrement(__FILE__, __LINE__)) {
			msb.parse(tokens);
		}

		if (tokens.decrement(__FILE__, __LINE__)) {
			tokens.next();
		}

		if (tokens.decrement(__FILE__, __LINE__)) {
			lsb.parse(tokens);
		}

		if (tokens.decrement(__FILE__, __LINE__)) {
			tokens.next();
		}
	}

	if (tokens.decrement(__FILE__, __LINE__)) {
		name = tokens.next();
	}

	// Check for vector dimensions after the name
	// This handles cases like: input data [7:0]
	while (tokens.decrement(__FILE__, __LINE__)) {
		tokens.next();

		tokens.increment(false);
		tokens.expect("[");

		// Expect closing bracket
		tokens.increment(true);
		tokens.expect("]");

		// Parse LSB expression
		tokens.increment(true);
		tokens.expect<rvalue>();

		// Expect colon
		tokens.increment(true);
		tokens.expect(":");

		// Parse MSB expression
		tokens.increment(true);
		tokens.expect<rvalue>();

		size.push_back(array<rvalue, 2>());

		if (tokens.decrement(__FILE__, __LINE__)) {
			size.back()[0].parse(tokens);
		}

		if (tokens.decrement(__FILE__, __LINE__)) {
			tokens.next();
		}

		if (tokens.decrement(__FILE__, __LINE__)) {
			size.back()[1].parse(tokens);
		}

		if (tokens.decrement(__FILE__, __LINE__)) {
			tokens.next();
		}
	}

	tokens.syntax_end(this);
}

bool declaration::is_next(tokenizer &tokens, int i, std::any data) {
	return tokens.is_next("input", i) or
		tokens.is_next("output", i) or
		tokens.is_next("inout", i) or
		tokens.is_next("wire", i) or
		tokens.is_next("reg", i) or
		tokens.is_next("integer", i);
}

void declaration::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<declaration>()) {
		tokens.register_syntax<declaration>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		rvalue::register_syntax(tokens);
	}
}

std::string declaration::to_string(std::string tab) const {
	string result;

	if (!valid) {
		return result;
	}

	// Direction
	if (input and output) {
		result += "inout ";
	} else if (input) {
		result += "input ";
	} else if (output) {
		result += "output ";
	}

	result += type + " ";

	if (msb.valid and lsb.valid) {
		result += "[" + msb.to_string(tab) + ":" + lsb.to_string(tab) + "] ";
	}

	// Name and vector dimensions
	result += name;
	for (int i = 0; i < (int)size.size(); i++) {
		result += "[" + size[i][0].to_string(tab) + ":" + size[i][1].to_string(tab) + "]";
	}

	return result;
}

parse::syntax *declaration::clone() const {
	return new declaration(*this);
}

} 
