#include "assignment_statement.h"
#include <parse/default/symbol.h>
#include <parse/default/instance.h>

namespace parse_verilog {

assignment_statement::assignment_statement() {
	debug_name = "assignment_statement";
}

assignment_statement::assignment_statement(tokenizer &tokens, void *data) {
	debug_name = "assignment_statement";
	parse(tokens, data);
}

assignment_statement::~assignment_statement() {
}

void assignment_statement::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	// Parse semicolon
	tokens.increment(true);
	tokens.expect(";");

	// Parse right-hand side expression
	tokens.increment(true);
	tokens.expect<expression>();

	// Parse equals sign
	tokens.increment(true);
	tokens.expect("<=");
	tokens.expect("=");

	// Parse left-hand side (target)
	tokens.increment(true);
	tokens.expect<parse_ucs::variable_name>();

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

bool assignment_statement::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("<=", i+1) or tokens.is_next("=", i+1);
}

void assignment_statement::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<assignment_statement>()) {
		tokens.register_syntax<assignment_statement>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		
		// Register components
		parse_ucs::variable_name::register_syntax(tokens);
		expression::register_syntax(tokens);
	}
}

string assignment_statement::to_string(string tab) const {
	if (!valid) {
		return "";
	}
	
	return name.to_string(tab) + " <= " + expr.to_string(tab) + ";";
}

parse::syntax *assignment_statement::clone() const {
	return new assignment_statement(*this);
}

} // namespace parse_verilog 
