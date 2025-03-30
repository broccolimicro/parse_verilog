#include "module.h"
#include "port.h"
#include "initial.h"
#include "always.h"
#include "assign.h"

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
	tokens.expect<trigger>();
	tokens.expect<assign>();
	tokens.expect<declaration>();

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
		tokens.expect<declaration>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			ports.push_back(declaration(tokens, data));
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
			items.push_back(trigger(tokens, data));
		} else if (tokens.found<assign>()) {
			items.push_back(assign(tokens, data));
		}

		tokens.increment(false);
		tokens.expect<trigger>();
		tokens.expect<assign>();
		tokens.expect<declaration>();
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
		declaration::register_syntax(tokens);
		trigger::register_syntax(tokens);
		assign::register_syntax(tokens);
	}
}

std::string module_def::to_string(std::string tab) const {
	string result;
	if (!valid) {
		return result;
	}
	
	result = "module " + name;
	
	// Port list
	result += " (";
	for (size_t i = 0; i < ports.size(); i++) {
		if (i > 0) result += ", ";
		result += ports[i]->to_string("");
	}
	result += ");\n";
	
	// Module items
	for (auto item = items.begin(); item != items.end(); item++) {
		result += item->to_string(tab + "\t") + "\n";
	}
	
	result += tab + "endmodule";
	return result;
}

parse::syntax *module_def::clone() const {
	module_def *result = new module_def();
	result->name = name;
	result->ports = ports;
	for (int i = 0; i < (int)items.size(); i++) {
		result.items.push_back(items[i]->clone());
	}
	return result;
}

} 
