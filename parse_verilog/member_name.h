#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "slice.h"

#include <common/net.h>

namespace parse_verilog
{
struct member_name : parse::syntax
{
	member_name();
	member_name(ucs::Field field);
	member_name(tokenizer &tokens, void *data = NULL);
	~member_name();

	string name;
	vector<slice> slices;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};
}

