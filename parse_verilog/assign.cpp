#include "assign.h"
#include <parse/default/symbol.h>
#include <parse/default/instance.h>

namespace parse_verilog {

assign::assign() {
	debug_name = "assign";
}

assign::assign(tokenizer &tokens, void *data) {
	debug_name = "assign";
	parse(tokens, data);
}

assign::~assign() {
}

void assign::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	// Parse semicolon
	tokens.increment(true);
	tokens.expect(";");

	// Parse right-hand side expression
	tokens.increment(true);
	tokens.expect<expression>();

	// Parse equals sign
	tokens.increment(true);
	tokens.expect("=");

	// Parse left-hand side (target)
	tokens.increment(true);
	tokens.expect<parse_ucs::variable_name>();

	// Parse "assign" keyword
	tokens.increment(true);
	tokens.expect("assign");

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		name.parse(tokens, data);
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		expr.parse(tokens, data);
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	tokens.syntax_end(this);
}

bool assign::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("assign", i);
}

void assign::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<assign>()) {
		tokens.register_syntax<assign>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		
		// Register components
		parse_ucs::variable_name::register_syntax(tokens);
		expression::register_syntax(tokens);
	}
}

string assign::to_string(string tab) const {
	if (!valid) {
		return "";
	}
	
	return "assign " + name.to_string(tab) + " = " + expr.to_string(tab) + ";";
}

parse::syntax *assign::clone() const {
	return new assign(*this);
}

} // namespace parse_verilog 
