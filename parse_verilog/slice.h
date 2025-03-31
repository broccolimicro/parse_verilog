#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <memory>

using namespace std;

namespace parse_verilog
{

struct slice : parse::syntax
{
	slice();
	slice(tokenizer &tokens, void *data = NULL);
	~slice();

	shared_ptr<parse::syntax> lower;
	shared_ptr<parse::syntax> upper;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

