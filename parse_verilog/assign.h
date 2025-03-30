#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_ucs/variable_name.h>
#include <parse_verilog/expression.h>

#include <string>

using namespace std;

namespace parse_verilog {

struct assign : parse::syntax {
	assign();
	assign(tokenizer &tokens, void *data = nullptr);
	~assign();

	// The left side (target) of the assignment
	parse_ucs::variable_name name;
	
	// The right side (expression) of the assignment
	expression expr;

	void parse(tokenizer &tokens, void *data = nullptr);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

} 
