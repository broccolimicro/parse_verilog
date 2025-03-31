#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_verilog/variable_name.h>
#include <parse_verilog/assignment_statement.h>

#include <string>

using namespace std;

namespace parse_verilog {

struct continuous : parse::syntax {
	continuous();
	continuous(tokenizer &tokens, void *data = nullptr);
	~continuous();

	bool force;
	variable_name deassign;
	assignment_statement assign;
	
	void parse(tokenizer &tokens, void *data = nullptr);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

} 
