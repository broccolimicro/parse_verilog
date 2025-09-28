#include "expression.h"
#include <parse_expression/precedence.h>
#include <parse_verilog/assignment_statement.h>

namespace parse_verilog {

using parse_expression::precedence_set;
using parse_expression::operation_set;

void setup_expressions() {
	if (expression::precedence.empty()) {
		precedence_set result;
		result.push(operation_set::BINARY);
		result.push_back("", "", "or", "");
		result.push_back("", "", ",", "");

		result.push(operation_set::UNARY);
		result.push_back("posedge", "", "", "");
		result.push_back("negedge", "", "", "");

		result.push(operation_set::BINARY);
		result.push_back("", "", "||", "");
		
		result.push(operation_set::BINARY);
		result.push_back("", "", "&&", "");

		result.push(operation_set::BINARY);
		result.push_back("", "", "|", "");

		result.push(operation_set::BINARY);
		result.push_back("", "", "^", "");
		result.push_back("", "", "~^", "");
		result.push_back("", "", "^~", "");

		result.push(operation_set::BINARY);
		result.push_back("", "", "&", "");

		result.push(operation_set::BINARY);
		result.push_back("", "", "==", "");
		result.push_back("", "", "!=", "");
		result.push_back("", "", "===", "");
		result.push_back("", "", "!==", "");

		result.push(operation_set::BINARY);
		result.push_back("", "", "<", "");
		result.push_back("", "", ">", "");
		result.push_back("", "", "<=", "");
		result.push_back("", "", ">=", "");

		result.push(operation_set::BINARY);
		result.push_back("", "", "<<", "");
		result.push_back("", "", ">>", "");
		result.push_back("", "", "<<<", "");
		result.push_back("", "", ">>>", "");

		result.push(operation_set::BINARY);
		result.push_back("", "", "+", "");
		result.push_back("", "", "-", "");

		result.push(operation_set::BINARY);
		result.push_back("", "", "*", "");
		result.push_back("", "", "/", "");
		result.push_back("", "", "%", "");

		result.push(operation_set::BINARY);
		result.push_back("", "", "**", "");

		result.push(operation_set::UNARY);
		result.push_back("+", "", "", "");
		result.push_back("-", "", "", "");
		result.push_back("!", "", "", "");
		result.push_back("~", "", "", "");
		result.push_back("&", "", "", "");
		result.push_back("~&", "", "", "");
		result.push_back("|", "", "", "");
		result.push_back("~|", "", "", "");
		result.push_back("^", "", "", "");
		result.push_back("~^", "", "", "");
		result.push_back("^~", "", "", "");

		result.push(operation_set::MODIFIER);
		result.push_back("$", "(", ",", ")");
		result.push_back("", ".", "", "");
		result.push_back("", "[", ":", "]");
		
		result.push(operation_set::MODIFIER);
		result.push_back("", "::", "", "");
		
		expression::register_precedence(result);
		size_t lvalueLevel = result.size()-2;
		assignment::lvalueLevel = lvalueLevel;
		assignment_statement::lvalueLevel = lvalueLevel;
	}
}

}
