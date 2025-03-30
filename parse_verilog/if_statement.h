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
	if_statement(tokenizer &tokens, void *data = nullptr);
	~if_statement();

	vector<expression> condition;
	vector<block_statement> body;

	void parse(tokenizer &tokens, void *data = nullptr);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

} // namespace parse_verilog 
