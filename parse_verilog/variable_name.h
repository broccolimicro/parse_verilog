#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <common/net.h>

#include "member_name.h"

namespace parse_verilog
{

struct variable_name : parse::syntax
{
	variable_name();
	variable_name(ucs::Net net);
	variable_name(tokenizer &tokens, void *data = NULL);
	~variable_name();

	vector<member_name> names;
	string region;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}
