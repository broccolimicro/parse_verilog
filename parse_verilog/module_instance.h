#pragma once

#include <parse/syntax.h>
#include <parse_verilog/expression.h>

#include <string>
#include <vector>

using namespace std;

namespace parse_verilog {

struct port_connection {
	port_connection();
	~port_connection();
	
	string port_name;  // Empty for positional connection
	rvalue expr;
	
	string to_string(string tab = "") const;
};

struct module_instance : parse::syntax {
	module_instance();
	module_instance(tokenizer &tokens, std::any data=std::any());
	~module_instance();

	string module_type;  // Type of the module being instantiated
	string instance_name; // Name of this instance
	
	vector<port_connection> connections; // Port connections

	void parse(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

} // namespace parse_verilog 
