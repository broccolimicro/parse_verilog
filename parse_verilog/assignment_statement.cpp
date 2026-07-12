#include "assignment_statement.h"
#include <parse/default/symbol.h>
#include <parse/default/instance.h>

namespace parse_verilog {

assignment_statement::assignment_statement() {
	debug_name = "verilog_assignment_statement";
	blocking = true;
}

assignment_statement::assignment_statement(tokenizer &tokens, std::any data) {
	debug_name = "verilog_assignment_statement";
	blocking = true;
	parse(tokens, data);
}

assignment_statement::~assignment_statement() {
}

void assignment_statement::parse(tokenizer &tokens, std::any data) {
	tokens.syntax_start(this);

	// Parse right-hand side expression
	tokens.increment(true);
	tokens.expect<rvalue>();

	// Parse equals sign
	tokens.increment(true);
	tokens.expect("<=");
	tokens.expect("=");

	// Parse left-hand side (target)
	tokens.increment(true);
	tokens.expect<lvalue>();

	if (tokens.decrement(__FILE__, __LINE__)) {
		left.parse(tokens);
	}

	if (tokens.decrement(__FILE__, __LINE__)) {
		blocking = (tokens.next() == "=");
	}

	if (tokens.decrement(__FILE__, __LINE__)) {
		expr.parse(tokens);
	}

	tokens.syntax_end(this);
}

bool assignment_statement::is_next(tokenizer &tokens, int i, std::any data) {
	return not tokens.is_next("begin", i)
		and not tokens.is_next("end", i)
		and not tokens.is_next("input", i)
		and not tokens.is_next("output", i)
		and not tokens.is_next("inout", i)
		and not tokens.is_next("if", i)
		and not tokens.is_next("always", i)
		and not tokens.is_next("initial", i)
		and not tokens.is_next("assign", i)
		and not tokens.is_next("for", i)
		and not tokens.is_next("module", i)
		and not tokens.is_next("endmodule", i)
		and lvalue::is_next(tokens, i);
	//return tokens.is_next("<=", i+1) or tokens.is_next("=", i+1);
}

void assignment_statement::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<assignment_statement>()) {
		tokens.register_syntax<assignment_statement>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		
		// Register components
		lvalue::register_syntax(tokens);
		rvalue::register_syntax(tokens);
	}
}

string assignment_statement::to_string(string tab) const {
	if (!valid) {
		return "";
	}
	
	return left.to_string(tab) + (blocking ? " = " : " <= ")  + expr.to_string(tab);
}

parse::syntax *assignment_statement::clone() const {
	return new assignment_statement(*this);
}

} // namespace parse_verilog 
