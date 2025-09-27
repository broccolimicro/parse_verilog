#pragma once

#include <parse_verilog/number.h>
#include <parse_expression/expression.h>
#include <parse_expression/assignment.h>
#include <parse_expression/composition.h>

namespace parse_verilog {

static const size_t expr_group = 2;

using argument=parse_expression::argument_t<expr_group, parse_verilog::number>;
using expression=parse_expression::expression_t<expr_group, parse_verilog::number>;
using assignment=parse_expression::assignment_t<expr_group, parse_verilog::number>;
using composition=parse_expression::composition_t<expr_group, parse_verilog::number>;

void setup_expressions();

}
