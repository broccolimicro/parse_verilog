#include "module.h"
#include "declaration.h"
#include "trigger.h"
#include "assign.h"
#include "module_instance.h"

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

	tokens.increment(false);
	tokens.expect<declaration>();
	tokens.expect<assign>();
	tokens.expect<trigger>();
	tokens.expect<module_instance>();

	tokens.increment(true);
	tokens.expect(";");

	tokens.increment(true);
	tokens.expect(")");

	tokens.increment(false);
	tokens.expect<declaration>();

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

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		ports.push_back(declaration(tokens, data));

		tokens.increment(false);
		tokens.expect(",");

		// "(port, port, ..., port"
		while (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();

			tokens.increment(false);
			tokens.expect(",");

			tokens.increment(true);
			tokens.expect<declaration>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				ports.push_back(declaration(tokens, data));
			}
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
		if (tokens.found<declaration>()) {
			items.push_back(shared_ptr<parse::syntax>(new declaration(tokens, data)));

			tokens.increment(true);
			tokens.expect(";");

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();
			}
		} else if (tokens.found<assign>()) {
			items.push_back(shared_ptr<parse::syntax>(new assign(tokens, data)));
		} else if (tokens.found<trigger>()) {
			items.push_back(shared_ptr<parse::syntax>(new trigger(tokens, data)));
		} else if (tokens.found<module_instance>()) {
			items.push_back(shared_ptr<parse::syntax>(new module_instance(tokens, data)));
		} 

		tokens.increment(false);
		tokens.expect<declaration>();
		tokens.expect<assign>();
		tokens.expect<trigger>();
		tokens.expect<module_instance>();
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
	if (!tokens.syntax_registered<module_def>()) {
		tokens.register_syntax<module_def>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(false);
		
		// Register components
		declaration::register_syntax(tokens);
		trigger::register_syntax(tokens);
		assign::register_syntax(tokens);
		module_instance::register_syntax(tokens);
	}
}

std::string module_def::to_string(std::string tab) const {
	string result;
	if (!valid) {
		return result;
	}
	
	result = "module " + name + "(\n";
	// Port list
	for (int i = 0; i < (int)ports.size(); i++) {
		if (i != 0) {
			result += ",\n";
		}
		result += tab + "\t" + ports[i].to_string(tab);
	}
	result += tab + (not ports.empty() ? "\n" : "") + ");\n";
	
	// Module items
	for (int i = 0; i < (int)items.size(); i++) {
		result += tab + "\t" + items[i]->to_string(tab + "\t") + "\n";
	}
	
	result += tab + "endmodule";
	return result;
}

parse::syntax *module_def::clone() const {
	module_def *result = new module_def();
	result->name = name;
	result->ports = ports;
	for (int i = 0; i < (int)items.size(); i++) {
		result->items.push_back(shared_ptr<parse::syntax>(items[i]->clone()));
	}
	return result;
}

} 
