#include "loop_statement.h"
#include <parse/default/symbol.h>
#include <parse/default/instance.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_verilog {

loop_statement::loop_statement() {
	debug_name = "verilog_loop_statement";
}

loop_statement::loop_statement(tokenizer &tokens, void *data) {
	debug_name = "verilog_loop_statement";
	parse(tokens, data);
}

loop_statement::~loop_statement() {
}

void loop_statement::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	// Push tokens in reverse order of their appearance in source code
	
	// Parse body (last in source)
	tokens.increment(true);
	tokens.expect<block_statement>();
	
	// Parse closing parenthesis
	tokens.increment(true);
	tokens.expect(")");
	
	// Parse increment expression
	tokens.increment(false);
	tokens.expect<assignment_statement>();
	
	// Parse second semicolon
	tokens.increment(true);
	tokens.expect(";");
	
	// Parse condition
	tokens.increment(false);
	tokens.expect<expression>();
	
	// Parse first semicolon
	tokens.increment(true);
	tokens.expect(";");

	// Parse initialization expression
	tokens.increment(false);
	tokens.expect<assignment_statement>();
	
	// Parse opening parenthesis
	tokens.increment(true);
	tokens.expect("(");

	// Parse "for" keyword (first in source)
	tokens.increment(true);
	tokens.expect("for");

	// Process tokens in correct order (as they appear in source)
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next(); // for
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next(); // (
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		init.parse(tokens, data);
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next(); // ;
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		cond.parse(tokens, data);
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next(); // ;
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		step.parse(tokens, data);
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next(); // )
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		body.parse(tokens, data);
	}

	tokens.syntax_end(this);
}

bool loop_statement::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("for", i);
}

void loop_statement::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<loop_statement>()) {
		setup_expressions();
		tokens.register_syntax<loop_statement>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(false);
		
		// Register components
		assignment_statement::register_syntax(tokens);
		expression::register_syntax(tokens);
		block_statement::register_syntax(tokens);
	}
}

string loop_statement::to_string(string tab) const {
	if (!valid) {
		return "";
	}
	
	string result = "for (" + init.to_string(tab) + ";" + cond.to_string(tab) + "; " + step.to_string(tab) + ")";

	if (body.sub.size() == 1u) {
		result += "\n" + tab + "\t" + body.to_string(tab+"\t");
	} else {
		result += " " + body.to_string(tab);
	}
	return result;
}

parse::syntax *loop_statement::clone() const {
	return new loop_statement(*this);
}

} // namespace parse_verilog 
