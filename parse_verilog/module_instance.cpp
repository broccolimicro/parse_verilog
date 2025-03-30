#include "module_instance.h"
#include <parse/default/symbol.h>
#include <parse/default/instance.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_verilog {

// Port connection implementation
port_connection::port_connection() {
	port_name = "";
}

port_connection::~port_connection() {
}

string port_connection::to_string(string tab) const {
	if (port_name.empty()) {
		// Positional connection
		return expr.to_string(tab);
	} else {
		// Named connection
		return "." + port_name + "(" + expr.to_string(tab) + ")";
	}
}

// Module instance implementation
module_instance::module_instance() {
	debug_name = "module_instance";
	module_type = "";
	instance_name = "";
}

module_instance::module_instance(tokenizer &tokens, void *data) {
	debug_name = "module_instance";
	module_type = "";
	instance_name = "";
	parse(tokens, data);
}

module_instance::~module_instance() {
}

void module_instance::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	// Parse semicolon (last in source)
	tokens.increment(true);
	tokens.expect(";");

	// Parse closing parenthesis
	tokens.increment(true);
	tokens.expect(")");

	// Parse opening parenthesis for port list
	tokens.increment(true);
	tokens.expect("(");

	// Parse instance name (identifier)
	tokens.increment(true);
	tokens.expect<parse::instance>();

	// Parse module type (identifier) (first in source)
	tokens.increment(true);
	tokens.expect<parse::instance>();

	// Process tokens in correct order (as they appear in source)
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		module_type = tokens.next(); // module type
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		instance_name = tokens.next(); // instance name
	}

	// Parse all port connections
	bool first = true;
	while (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next(); // ( then ,
		
		tokens.increment(false);
		tokens.expect(",");
	
		tokens.increment(not first);
		tokens.expect(".");  // Named connection
		tokens.expect<expression>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			port_connection conn;
			if (tokens.found(".")) {
				tokens.next(); // .
			
				tokens.increment(true);
				tokens.expect(")");
		
				tokens.increment(true);
				tokens.expect<expression>();

				tokens.increment(true);
				tokens.expect("(");

				tokens.increment(true);
				tokens.expect<parse::instance>();

				if (tokens.decrement(__FILE__, __LINE__, data)) {
					conn.port_name = tokens.next(); // port name
				}
				
				if (tokens.decrement(__FILE__, __LINE__, data)) {
					tokens.next(); // (
				}
				
				if (tokens.decrement(__FILE__, __LINE__, data)) {
					conn.expr.parse(tokens, data);
				}
				
				if (tokens.decrement(__FILE__, __LINE__, data)) {
					tokens.next(); // )
				}
			} else {
				conn.expr.parse(tokens, data);
			}
		
			connections.push_back(conn);
		}
		
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next(); // )
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next(); // ;
	}

	tokens.syntax_end(this);
}

bool module_instance::is_next(tokenizer &tokens, int i, void *data) {
	// Check if token is an identifier and next token is also an identifier
	return tokens.is_next<parse::instance>(i) and tokens.is_next<parse::instance>(i+1) and tokens.is_next("(", i+2);
}

void module_instance::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<module_instance>()) {
		tokens.register_syntax<module_instance>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(false);
		
		// Register components
		expression::register_syntax(tokens);
	}
}

string module_instance::to_string(string tab) const {
	if (!valid) {
		return "";
	}
	
	string result = module_type + " " + instance_name + "(";
	
	for (size_t i = 0; i < connections.size(); i++) {
		if (i > 0) {
			result += ", ";
		}
		result += connections[i].to_string(tab);
	}
	
	result += ");";
	
	return result;
}

parse::syntax *module_instance::clone() const {
	return new module_instance(*this);
}

} // namespace parse_verilog 
