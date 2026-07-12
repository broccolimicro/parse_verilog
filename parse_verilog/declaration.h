#pragma once

#include <parse_verilog/expression.h>

#include <parse/parse.h>
#include <string>

using namespace std;

namespace parse_verilog {

struct declaration : parse::syntax {
	declaration();
	declaration(tokenizer &tokens, std::any data=std::any());
	~declaration();

	bool input;
	bool output;

	string type;

	rvalue msb;
	rvalue lsb;

	string name;
	
	vector<array<rvalue, 2> > size;

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	std::string to_string(std::string tab = "") const override;
	parse::syntax *clone() const override;
};

} 
