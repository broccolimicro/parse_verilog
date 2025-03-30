#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include <vector>
#include <string>
#include <memory>

namespace parse_verilog {

struct statement;

struct module_def : parse::syntax {
	module_def();
	module_def(tokenizer &tokens, void *data = nullptr);
	~module_def();

	std::string name;
	std::vector<port> ports;
	std::vector<statement> items;

	void parse(tokenizer &tokens, void *data = nullptr);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
	static void register_syntax(tokenizer &tokens);

	std::string to_string(std::string tab = "") const override;
	parse::syntax *clone() const override;
};

struct statement {
	statement(always always_stmt);
	statement(initial initial_stmt);
	statement(assign assign_stmt);
	
	always always_stmt;
	initial initial_stmt;
	assign assign_stmt;
};

} 
