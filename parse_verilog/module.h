#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_verilog/declaration.h>

#include <vector>
#include <string>
#include <memory>

using namespace std;

namespace parse_verilog {

struct module_def : parse::syntax {
	module_def();
	module_def(tokenizer &tokens, void *data = nullptr);
	~module_def();

	string name;
	vector<declaration> ports;
	vector<shared_ptr<parse::syntax> > items;

	void parse(tokenizer &tokens, void *data = nullptr);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

} 
