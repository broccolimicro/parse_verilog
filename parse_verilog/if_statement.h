#pragma once

#include <parse/syntax.h>

#include <parse_verilog/expression.h>
#include <parse_verilog/block_statement.h>

#include <vector>
#include <memory>
#include <string>

using namespace std;

namespace parse_verilog {

struct if_statement : parse::syntax {
	if_statement();
	if_statement(tokenizer &tokens, std::any data=std::any());
	~if_statement();

	vector<rvalue> condition;
	vector<block_statement> body;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

} // namespace parse_verilog 
