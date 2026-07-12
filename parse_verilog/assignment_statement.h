#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_verilog/expression.h>

#include <string>

using namespace std;

namespace parse_verilog {

struct assignment_statement : parse::syntax {
	assignment_statement();
	assignment_statement(tokenizer &tokens, std::any data=std::any());
	~assignment_statement();

	// The left side (target) of the assignment_statementment
	expression lvalue;

	bool blocking;
	
	// The right side (expression) of the assignment_statementment
	expression expr;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

} 
