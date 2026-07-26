#include "expression.h"
#include <parse_expression/precedence.h>
#include <parse_expression/literal.h>
#include <parse/wrapper.h>
#include "number.h"

namespace parse_verilog {

std::shared_ptr<parse_expression::config> config::cfg = 
	std::make_shared<parse_expression::config>(parse_verilog::makeExprConfig());

config::config() {
	debug_name = "verilog";
}

config::~config() {
}

parse_expression::config makeExprConfig() {
	parse_expression::config cfg;
	int CONSTANT = cfg.push<parse::wrapper<number> >("constant");
	int LITERAL = cfg.push<parse::wrapper<parse::instance> >("literal");

	cfg.base = {LITERAL, CONSTANT};

	using operation_set=parse_expression::operation_set;

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "or", "");
	cfg.order.push_back("", "", ",", "");

	cfg.order.push(operation_set::TERNARY);
	cfg.order.push_back("", "?", ":", "");

	cfg.order.push(operation_set::UNARY);
	cfg.order.push_back("posedge", "", "", "");
	cfg.order.push_back("negedge", "", "", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "||", "");
	
	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "&&", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "|", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "^", "");
	cfg.order.push_back("", "", "~^", "");
	cfg.order.push_back("", "", "^~", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "&", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "==", "");
	cfg.order.push_back("", "", "!=", "");
	cfg.order.push_back("", "", "===", "");
	cfg.order.push_back("", "", "!==", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "<", "");
	cfg.order.push_back("", "", ">", "");
	cfg.order.push_back("", "", "<=", "");
	cfg.order.push_back("", "", ">=", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "<<", "");
	cfg.order.push_back("", "", ">>", "");
	cfg.order.push_back("", "", "<<<", "");
	cfg.order.push_back("", "", ">>>", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "+", "");
	cfg.order.push_back("", "", "-", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "*", "");
	cfg.order.push_back("", "", "/", "");
	cfg.order.push_back("", "", "%", "");

	cfg.order.push(operation_set::BINARY);
	cfg.order.push_back("", "", "**", "");

	cfg.order.push(operation_set::UNARY);
	cfg.order.push_back("+", "", "", "");
	cfg.order.push_back("-", "", "", "");
	cfg.order.push_back("!", "", "", "");
	cfg.order.push_back("~", "", "", "");
	cfg.order.push_back("&", "", "", "");
	cfg.order.push_back("~&", "", "", "");
	cfg.order.push_back("|", "", "", "");
	cfg.order.push_back("~|", "", "", "");
	cfg.order.push_back("^", "", "", "");
	cfg.order.push_back("~^", "", "", "");
	cfg.order.push_back("^~", "", "", "");

	cfg.set_lvalue();

	cfg.order.push(operation_set::MODIFIER);
	cfg.order.push_back("$", "(", ",", ")");
	cfg.order.push_back("", ".", "", "");
	cfg.order.push_back("", "[", ":", "]");
	cfg.order.push_back("", "'(", "", ")");
	
	cfg.order.push(operation_set::GROUP);
	cfg.order.push_back("'{", "", "", "}");

	return cfg;
}

}
