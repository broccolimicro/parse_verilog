/*
 * expression.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: nbingham
 */

#include "expression.h"
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/instance.h>
#include <parse/default/white_space.h>

#include <cctype>

namespace parse_verilog
{

operation_set::operation_set()
{
	type = binary;
}

operation_set::operation_set(int type)
{
	this->type = type;
}

operation_set::~operation_set()
{

}

vector<operation_set> expression::precedence;

expression::expression()
{
	debug_name = "expression";
	level = 0;
	init();
}

expression::expression(tokenizer &tokens, int level, void *data)
{
	debug_name = "expression";
	this->level = level;
	init();
	parse(tokens, data);
}

expression::~expression()
{
}

void expression::init()
{
	if (precedence.size() == 0)
	{
		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("or");
		precedence.back().symbols.push_back(",");

		precedence.push_back(operation_set(operation_set::left_unary));
		precedence.back().symbols.push_back("posedge");
		precedence.back().symbols.push_back("negedge");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("||");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("&&");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("|");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("^");
		precedence.back().symbols.push_back("~^");
		precedence.back().symbols.push_back("^~");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("&");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("==");
		precedence.back().symbols.push_back("!=");
		precedence.back().symbols.push_back("===");
		precedence.back().symbols.push_back("!==");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("<");
		precedence.back().symbols.push_back("<=");
		precedence.back().symbols.push_back(">");
		precedence.back().symbols.push_back(">=");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back(">>");
		precedence.back().symbols.push_back("<<");
		precedence.back().symbols.push_back(">>>");
		precedence.back().symbols.push_back("<<<");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("+");
		precedence.back().symbols.push_back("-");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("*");
		precedence.back().symbols.push_back("/");
		precedence.back().symbols.push_back("%");

		precedence.push_back(operation_set(operation_set::binary));
		precedence.back().symbols.push_back("**");

		precedence.push_back(operation_set(operation_set::left_unary));
		precedence.back().symbols.push_back("+");
		precedence.back().symbols.push_back("-");
		precedence.back().symbols.push_back("!");
		precedence.back().symbols.push_back("~");
		precedence.back().symbols.push_back("&");
		precedence.back().symbols.push_back("~&");
		precedence.back().symbols.push_back("|");
		precedence.back().symbols.push_back("~|");
		precedence.back().symbols.push_back("^");
		precedence.back().symbols.push_back("~^");
		precedence.back().symbols.push_back("^~");
	}
}

int expression::get_level(string operation)
{
	for (int i = 0; i < (int)precedence.size(); i++)
		for (int j = 0; j < (int)precedence[i].symbols.size(); j++)
			if (precedence[i].symbols[j] == operation)
				return i;

	return precedence.size();
}

bool expression::level_has(int level, string operation)
{
	for (int j = 0; j < (int)precedence[level].symbols.size(); j++)
		if (precedence[level].symbols[j] == operation)
			return true;

	return false;
}

void expression::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	if (precedence[level].type < operation_set::binary)
	{
		tokens.increment(true);
		if (level < (int)precedence.size()-1)
			tokens.expect<expression>();
		else
		{
			tokens.expect<variable_name>();
			tokens.expect<parse::number>();
			tokens.expect("gnd");
			tokens.expect("vdd");
			tokens.expect("(");
		}

		if (precedence[level].type == operation_set::left_unary)
		{
			bool first = true;
			do
			{
				if (first)
					first = false;
				else
					operations.push_back(tokens.next());

				tokens.increment(false);
				for (int i = 0; i < (int)precedence[level].symbols.size(); i++)
					tokens.expect(precedence[level].symbols[i]);
			} while (tokens.decrement(__FILE__, __LINE__, data));
		}

		if (tokens.decrement(__FILE__, __LINE__, &level))
		{
			if (tokens.found<expression>())
				arguments.push_back(argument(expression(tokens, level+1, data)));
			else if (tokens.found<variable_name>())
				arguments.push_back(argument(variable_name(tokens, data)));
			else if (tokens.found<parse::number>())
				arguments.push_back(argument(tokens.next()));
			else if (tokens.found("gnd") or tokens.found("vdd"))
				arguments.push_back(argument(tokens.next()));
			else if (tokens.found("("))
			{
				tokens.next();

				tokens.increment(true);
				tokens.expect(")");

				tokens.increment(true);
				tokens.expect<expression>();

				if (tokens.decrement(__FILE__, __LINE__, NULL))
					arguments.push_back(argument(expression(tokens, 0, data)));

				if (tokens.decrement(__FILE__, __LINE__, data))
					tokens.next();

			}
		}

		if (precedence[level].type == operation_set::right_unary)
		{
			bool first = true;
			do
			{
				if (first)
					first = false;
				else
					operations.push_back(tokens.next());

				tokens.increment(false);
				for (int i = 0; i < (int)precedence[level].symbols.size(); i++)
						tokens.expect(precedence[level].symbols[i]);
			} while (tokens.decrement(__FILE__, __LINE__, data));
		}
	}
	if (precedence[level].type == operation_set::binary)
	{
		bool first = true;
		do
		{
			if (first)
				first = false;
			else
				operations.push_back(tokens.next());

			tokens.increment(false);
			for (int i = 0; i < (int)precedence[level].symbols.size(); i++)
				tokens.expect(precedence[level].symbols[i]);

			tokens.increment(true);
			if (level < (int)precedence.size()-1)
				tokens.expect<expression>();
			else
			{
				tokens.expect<variable_name>();
				tokens.expect<parse::number>();
				tokens.expect("gnd");
				tokens.expect("vdd");
				tokens.expect("(");
			}

			if (tokens.decrement(__FILE__, __LINE__, &level))
			{
				if (tokens.found<expression>())
					arguments.push_back(argument(expression(tokens, level+1, data)));
				else if (tokens.found<variable_name>())
					arguments.push_back(argument(variable_name(tokens, data)));
				else if (tokens.found<parse::number>())
					arguments.push_back(argument(tokens.next()));
				else if (tokens.found("gnd"))
					arguments.push_back(argument(tokens.next()));
				else if (tokens.found("vdd"))
					arguments.push_back(argument(tokens.next()));
				else if (tokens.found("("))
				{
					tokens.next();

					tokens.increment(true);
					tokens.expect(")");

					tokens.increment(true);
					tokens.expect<expression>();

					if (tokens.decrement(__FILE__, __LINE__, NULL))
						arguments.push_back(argument(expression(tokens, 0, data)));

					if (tokens.decrement(__FILE__, __LINE__, data))
						tokens.next();
					
				}
			}

		} while (tokens.decrement(__FILE__, __LINE__, data));
	}

	tokens.syntax_end(this);
}

bool expression::is_next(tokenizer &tokens, int i, void *data)
{
	int level = -1;
	if (data != NULL)
		level = *(int*)data;

	bool result = tokens.is_next("(", i) || tokens.is_next<parse::number>(i) || variable_name::is_next(tokens, i, data);
	for (int j = level+1; j < (int)precedence.size(); j++)
		if (precedence[j].type == operation_set::left_unary)
			for (int k = 0; k < (int)precedence[j].symbols.size(); k++)
				result = result || tokens.is_next(precedence[j].symbols[k], i);

	return result;
}

void expression::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<expression>())
	{
		tokens.register_syntax<expression>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::number>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		variable_name::register_syntax(tokens);
	}
}

string expression::to_string(string tab) const
{
	return to_string(-1, tab);
}

string expression::to_string(int prev_level, string tab) const
{
	if (!valid || arguments.size() == 0)
		return "gnd";

	string result = "";
	bool paren = prev_level > level && operations.size() > 0;
	if (paren)
		result += "(";

	if (level >= 0 && precedence[level].type == operation_set::left_unary) {
		for (int i = 0; i < (int)operations.size(); i++) {
			result += operations[i];
			if (isalnum(operations[i].back())) {
				result += " ";
			}
		}

		result += arguments[0].to_string(level, tab);
	} else if (level >= 0 && precedence[level].type == operation_set::right_unary) {
		result += arguments[0].to_string(level, tab);

		for (int i = 0; i < (int)operations.size(); i++) {
			if (isalnum(operations[i][0])) {
				result += " ";
			}
			result += operations[i];
		}
	} else {
		for (int i = 0; i < (int)arguments.size() && i-1 < (int)operations.size(); i++) {
			if (i != 0) {
				if (isalnum(operations[i-1][0])) {
					result += " ";
				}
				result += operations[i-1];
				if (isalnum(operations[i-1].back())) {
					result += " ";
				}
			}

			result += arguments[i].to_string(level, tab);
		}
	}

	if (paren)
		result += ")";

	return result;
}

parse::syntax *expression::clone() const
{
	return new expression(*this);
}

argument::argument()
{

}

argument::argument(expression sub)
{
	this->sub = sub;
}

argument::argument(variable_name literal)
{
	this->literal = literal;
}

argument::argument(string constant)
{
	this->constant = constant;
}

argument::~argument()
{

}

string argument::to_string(int level, string tab) const
{
	if (sub.valid)
		return sub.to_string(level, tab);
	else if (literal.valid)
		return literal.to_string(tab);
	else if (constant != "")
		return constant;
	else
		return "vdd";
}

}
