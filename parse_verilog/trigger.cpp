#include "trigger.h"

#include <parse/default/symbol.h>
#include <parse/default/instance.h>

#include "assignment_statement.h"
#include "if_statement.h"
#include "loop_statement.h"
#include "block_statement.h"

namespace parse_verilog {

trigger::trigger() {
	debug_name = "trigger";
}

trigger::trigger(tokenizer &tokens, void *data) {
	debug_name = "trigger";
	parse(tokens, data);
}

trigger::~trigger() {
}

void trigger::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect<assignment_statement>();
	tokens.expect<if_statement>();
	tokens.expect<loop_statement>();
	tokens.expect<block_statement>();

	// Parse "trigger" keyword
	tokens.increment(true);
	tokens.expect("always");
	tokens.expect("initial");

	bool always = false;

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		always = (tokens.next() == "always");
	}

	if (always) {
		tokens.increment(true);
		tokens.expect(")");

		tokens.increment(true);
		tokens.expect<expression>();
		tokens.expect("*");

		// Expect opening parenthesis
		tokens.increment(true);
		tokens.expect("(");

		// Parse event control (e.g., @(posedge clk))
		tokens.increment(true);
		tokens.expect("@");

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next(); // Consume "@"
		}
			
		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next(); // Consume "("
		}

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			if (tokens.found("*")) {
				tokens.next();
				condition.valid = true;
			} else {
				condition.parse(tokens, data);
			}
		}

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next(); // Consume ")"
		}
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		body.parse(tokens, data);
	}

	tokens.syntax_end(this);
}

bool trigger::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("always", i) or tokens.is_next("initial", i);
}

void trigger::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<trigger>()) {
		tokens.register_syntax<trigger>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		expression::register_syntax(tokens);
		assignment_statement::register_syntax(tokens);
		if_statement::register_syntax(tokens);
		loop_statement::register_syntax(tokens);
		block_statement::register_syntax(tokens);
	}
}

std::string trigger::to_string(std::string tab) const {
	string result;
	if (!valid) {
		return result;
	}

	if (!condition.valid) {
		result += "initial ";
	} else {
		result += "always @(" + condition.to_string(tab) + ") ";
	}

	result += body.to_string(tab);
	
	return result;
}

parse::syntax *trigger::clone() const {
	return new trigger(*this);
}

} // namespace parse_verilog 
