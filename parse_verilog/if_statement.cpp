#include "if_statement.h"
#include "block_statement.h"
#include "statement.h"

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
		tokens.expect<block>();
		tokens.expect<assignment_statement>();
		tokens.expect<if_statement>();

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
			tokens.expect<parse_expression::expression>();

			// Expect opening parenthesis
			tokens.increment(true);
			tokens.expect("(");

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();
			}
			
			if (tokens.decrement(__FILE__, __LINE__, data)) {
				condition.push_back(parse_expression::expression(tokens, 0, data));
			}
			
			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();
			}
		}
		
		if (tokens.decrement(__FILE__, __LINE__, data)) {
			body.push_back(body_statement());
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

string if_statement::to_string(string tab) const {
	string result;
	if (!valid) {
		return result;
	}
	
	for (int i = 0; i < (int)body.size(); i++) {
		if (i != 0) {
			result += "else ";
		}
		if (i < (int)condition.size()) {
			result += "if (" + condition[i].to_string(tab) + ")";
		}
		result += body[i].to_string(tab+"\t");
	}

	return result;
}

parse::syntax *if_statement::clone() const {
	return if_statement(*this);
}

} // namespace parse_verilog 
