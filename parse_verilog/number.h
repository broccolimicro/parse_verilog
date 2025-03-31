#pragma once

#include <parse/parse.h>

namespace parse_verilog
{
struct number
{
	number();
	~number();

	string debug_name;

	static token consume(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
};
}
