#pragma once

#include <parse_verilog/number.h>
#include <parse_expression/expression.h>
#include <parse_expression/assignment.h>
#include <parse_expression/wrapper.h>
#include <parse/wrapper.h>
#include "number.h"

namespace parse_verilog {

parse_expression::config makeExprConfig();

struct config {
	static std::shared_ptr<parse_expression::config> cfg;
	
	string debug_name;

	config();
	~config();
};

using constant = parse::wrapper<number>;
using literal = parse::wrapper<parse::instance>;
using label = parse::wrapper<parse::instance>;

using rvalue = parse_expression::rvalue<config>;
using lvalue = parse_expression::lvalue<config>;

}
