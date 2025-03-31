/*
 * slice.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "slice.h"
#include <parse/default/number.h>
#include <parse/default/symbol.h>

#include "expression.h"

namespace parse_verilog
{
slice::slice()
{
	debug_name = "slice";
}

slice::slice(tokenizer &tokens, void *data)
{
	debug_name = "slice";
	parse(tokens, data);
}

slice::~slice()
{

}

void slice::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("]");

	tokens.increment(false);
	tokens.expect(":");

	tokens.increment(true);
	tokens.expect<expression>();

	tokens.increment(true);
	tokens.expect("[");

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		lower = shared_ptr<parse::syntax>(new expression(tokens, 0, data));
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(true);
		tokens.expect<expression>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			upper = shared_ptr<parse::syntax>(new expression(tokens, 0, data));
		}
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.syntax_end(this);
}

bool slice::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("[", i);
}

void slice::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<slice>())
	{
		tokens.register_syntax<slice>();
		tokens.register_token<parse::number>();
		tokens.register_token<parse::symbol>();

		expression::register_syntax(tokens);
	}
}

string slice::to_string(string tab) const
{
	string result = "[";
	if (lower and lower->valid) {
		result += lower->to_string(tab);
	}

	if (upper and upper->valid) {
		result += ":" + upper->to_string(tab);
	}

	result += "]";
	return result;
}

parse::syntax *slice::clone() const {
	slice *result = new slice();
	result->valid = valid;
	result->lower = shared_ptr<parse::syntax>(lower->clone());
	result->upper = shared_ptr<parse::syntax>(upper->clone());
	return result;
}
}
