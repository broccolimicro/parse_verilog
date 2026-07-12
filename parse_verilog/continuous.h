#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_verilog/assignment_statement.h>

#include <string>

using namespace std;

namespace parse_verilog {

struct continuous : parse::syntax {
	continuous();
	continuous(tokenizer &tokens, std::any data=std::any());
	~continuous();

	bool force;
	expression deassign;
	assignment_statement assign;
	
	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

} 
