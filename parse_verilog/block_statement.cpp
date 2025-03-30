#include <parse/default/symbol.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

#include "block_statement.h"
#include "if_statement.h"
#include "assignment_statement.h"

namespace parse_verilog {

block_statement::block_statement() {
	debug_name = "block_statement";
}

block_statement::block_statement(tokenizer &tokens, void *data) {
	debug_name = "block_statement";
	parse(tokens, data);
}

block_statement::~block_statement() {
}

void block_statement::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);
	
	tokens.increment(true);
	tokens.expect("end");

	tokens.increment(false);
	tokens.expect("begin");
	
	bool one = true;
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
		one = false;
	}

	tokens.increment(one);
	tokens.expect<if_statement>();
	tokens.expect<assignment_statement>();
	tokens.expect<block_statement>();
	
	while (tokens.decrement(__FILE__, __LINE__, data)) {
		if (tokens.found<if_statement>()) {
			sub.push_back(shared_ptr<parse::syntax>(new if_statement(tokens, data)));
		} else if (tokens.found<assignment_statement>()) {
			sub.push_back(shared_ptr<parse::syntax>(new assignment_statement(tokens, data)));
		} else if (tokens.found<block_statement>()) {
			sub.push_back(shared_ptr<parse::syntax>(new block_statement(tokens, data)));
		}

		if (not one) {
			tokens.increment(false);
			tokens.expect<block_statement>();
			tokens.expect<if_statement>();
			tokens.expect<assignment_statement>();
		} else {
			break;
		}
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}
	
	tokens.syntax_end(this);
}

bool block_statement::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("begin", i);
}

void block_statement::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<block_statement>()) {
		tokens.register_syntax<block_statement>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(false);
		
		// Register components
		if_statement::register_syntax(tokens);
		assignment_statement::register_syntax(tokens);
	}
}


std::string block_statement::to_string(std::string tab) const {
	string result;

	if (!valid) {
		return result;
	}

	if (sub.size() != 1u) {
		result += "begin\n";
	}

	for (int i = 0; i < (int)sub.size(); i++) {
		result += tab + sub[i]->to_string(tab+"\t") + "\n";
	}

	if (sub.size() != 1u) {
		result += tab + "end";
	}
	
	return result;
}

parse::syntax *block_statement::clone() const {
	block_statement *result = new block_statement();
	result->valid = valid;
	for (int i = 0; i < (int)sub.size(); i++) {
		result->sub.push_back(shared_ptr<parse::syntax>(sub[i]->clone()));
	}
	return result;
}

} // namespace parse_verilog 
