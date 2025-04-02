#include <parse/default/symbol.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

#include "assignment_statement.h"
#include "if_statement.h"
#include "loop_statement.h"
#include "block_statement.h"

namespace parse_verilog {

if_statement::if_statement() {
	debug_name = "if_statement";
}

if_statement::if_statement(tokenizer &tokens, void *data) {
	debug_name = "if_statement";
	parse(tokens, data);
}

if_statement::~if_statement() {
}

void if_statement::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	bool first = true;
	bool last = false;

	do {
		if (not first) {
			tokens.next();
		}

		tokens.increment(true);
		tokens.expect<assignment_statement>();
		tokens.expect<if_statement>();
		tokens.expect<loop_statement>();
		tokens.expect<block_statement>();

		tokens.increment(first);
		tokens.expect("if");
		first = false;
		
		last = true;
		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
			last = false;

			// Expect closing parenthesis
			tokens.increment(true);
			tokens.expect(")");

			// Parse condition
			tokens.increment(true);
			tokens.expect<expression>();

			// Expect opening parenthesis
			tokens.increment(true);
			tokens.expect("(");

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();
			}
			
			if (tokens.decrement(__FILE__, __LINE__, data)) {
				condition.push_back(expression(tokens, 0, data));
			}
			
			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();
			}
		}
		
		if (tokens.decrement(__FILE__, __LINE__, data)) {
			body.push_back(block_statement());
			body.back().parse(tokens, data);
		}

		if (not last) {
			tokens.increment(false);
			tokens.expect("else");
		}
	} while (not last and tokens.decrement(__FILE__, __LINE__, data));
	
	tokens.syntax_end(this);
}

bool if_statement::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("if", i);
}

void if_statement::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<if_statement>()) {
		tokens.register_syntax<if_statement>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(false);
		
		// Register components
		assignment_statement::register_syntax(tokens);
		if_statement::register_syntax(tokens);
		loop_statement::register_syntax(tokens);
		block_statement::register_syntax(tokens);
	}
}

string if_statement::to_string(string tab) const {
	string result;
	if (!valid) {
		return result;
	}
	
	for (int i = 0; i < (int)body.size(); i++) {
		if (i != 0) {
			if (body[i-1].sub.size() == 1u) {
				result += tab;
			} else {
				result += " ";
			}
			result += "else";
		}
		if (i < (int)condition.size()) {
			if (i != 0) {
				result += " ";
			}
			result += "if (" + condition[i].to_string(tab) + ")";
		}

		if (body[i].sub.size() == 1u) {
			result += "\n" + tab + "\t" + body[i].to_string(tab+"\t");
		} else {
			result += " " + body[i].to_string(tab);
		}
	}

	return result;
}

parse::syntax *if_statement::clone() const {
	return new if_statement(*this);
}

} // namespace parse_verilog 
