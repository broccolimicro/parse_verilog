#pragma once

#include <parse/parse.h>
#include <parse_expression/expression.h>
#include <string>

using namespace std;

namespace parse_verilog {

struct declaration : parse::syntax {
	declaration();
	declaration(tokenizer &tokens, void *data = nullptr);
	~declaration();

	bool input;
	bool output;

	string type;

	parse_expression::expression msb;
	parse_expression::expression lsb;

	string name;
	
	vector<array<parse_expression::expression, 2> > size;

	void parse(tokenizer &tokens, void *data = nullptr);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
	static void register_syntax(tokenizer &tokens);

	std::string to_string(std::string tab = "") const override;
	parse::syntax *clone() const override;
};

} 
