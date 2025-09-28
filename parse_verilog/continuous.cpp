#include "continuous.h"
#include <parse/default/symbol.h>
#include <parse/default/instance.h>

namespace parse_verilog {

continuous::continuous() {
	debug_name = "verilog_continuous";
}

continuous::continuous(tokenizer &tokens, void *data) {
	debug_name = "verilog_continuous";
	parse(tokens, data);
}

continuous::~continuous() {
}

void continuous::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("assign");
	tokens.expect("force");
	tokens.expect("deassign");
	tokens.expect("release");

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		string type = tokens.next();
		force = (type == "force" or type == "release");
		if (type == "assign" or type == "force") {
			tokens.increment(true);
			tokens.expect<assignment_statement>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				assign.parse(tokens, data);
			}
		} else {
			tokens.increment(true);
			tokens.expect<expression>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				deassign.level = assignment_statement::lvalueLevel;
				deassign.parse(tokens, data);
			}
		}
	}

	tokens.syntax_end(this);
}

bool continuous::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("assign", i) or tokens.is_next("force", i)
		or tokens.is_next("deassign", i) or tokens.is_next("release", i);
}

void continuous::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<continuous>()) {
		setup_expressions();
		tokens.register_syntax<continuous>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		
		// Register components
		expression::register_syntax(tokens);
		assignment_statement::register_syntax(tokens);
	}
}

string continuous::to_string(string tab) const {
	if (!valid) {
		return "";
	}

	if (assign.valid) {
		return (force ? "force " : "assign ") + assign.to_string(tab);
	} else if (deassign.valid) {
		return (force ? "release " : "deassign ") + deassign.to_string(tab);
	}
	return "";
}

parse::syntax *continuous::clone() const {
	return new continuous(*this);
}

} // namespace parse_verilog 
