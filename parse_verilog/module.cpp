#include "module.h"
#include "port.h"
#include "parameter.h"
#include "module_item.h"
#include "identifier.h"

#include <parse/default/symbol.h>
#include <parse/default/instance.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_verilog {

module_def::module_def() {
	debug_name = "module";
	name = "";
}

module_def::module_def(tokenizer &tokens, void *data) {
	debug_name = "module";
	name = "";
	parse(tokens, data);
}

module_def::~module_def() {
}

void module_def::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("endmodule");

	tokens.increment(false):
	tokens.expect<module_item>();

	tokens.increment(true);
	tokens.expect(";");

	tokens.increment(true);
	tokens.expect(")");

	tokens.increment(true);
	tokens.expect("(");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect("module");

	// "module"
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	// "mymodule"
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		name = tokens.next();
	}

	// "(port, port, ..., port"
	while (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(false);
		tokens.expect(",");

		tokens.increment(true):
		tokens.expect<port>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			ports.push_back(port(tokens, data));
		}
	}
	
	// ")"
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}
	
	// Parse semicolon after port list
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}
	
	// Parse module items
	while (tokens.decrement(__FILE__, __LINE__, data)) {
		if (tokens.found<always>()) {
			items.push_back(statement(always(tokens, data)));
		} else if (tokens.found<initial>()) {
			items.push_back(statement(initial(tokens, data)));
		} else if (tokens.found<assign>()) {
			items.push_back(statement(assign(tokens, data)));
		}

		tokens.increment(false);
		tokens.expect<always>();
		tokens.expect<initial>();
		tokens.expect<assign>();
	}
	
	// Parse endmodule
	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	tokens.syntax_end(this);
}

bool module_def::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("module", i);
}

void module_def::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<module>()) {
		tokens.register_syntax<module>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		
		// Register components
		port::register_syntax(tokens);
		parameter::register_syntax(tokens);
		identifier::register_syntax(tokens);
		module_item::register_syntax(tokens);
	}
}

std::string module_def::to_string(std::string tab) const {
	if (!valid) return tab + "invalid module";
	
	std::string result = tab + "module " + name;
	
	// Port list
	result += " (";
	for (size_t i = 0; i < ports.size(); i++) {
		if (i > 0) result += ", ";
		result += ports[i]->to_string("");
	}
	result += ");\n";
	
	// Module items
	for (const auto& item : items) {
		result += item->to_string(tab + "  ") + "\n";
	}
	
	result += tab + "endmodule";
	return result;
}

parse::syntax *module_def::clone() const {
	return new module(*this);
}

statement::statement(always always_stmt) {
	this->always_stmt = always_stmt;
}

statement::statement(initial initial_stmt) {
	this->initial_stmt = initial_stmt;
}

statement::statement(assign assign_stmt) {
	this->assign_stmt = assign_stmt;
}

} 
