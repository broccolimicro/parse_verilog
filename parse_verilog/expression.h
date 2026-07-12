#pragma once

#include <parse_verilog/number.h>
#include <parse_expression/expression.h>
#include <parse_expression/assignment.h>

namespace parse_verilog {

parse_expression::config makeExprConfig();

struct tag;
using expression=parse_expression::wrapper<tag>;

}
