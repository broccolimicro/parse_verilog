#include "factory.h"
#include "module.h"

namespace parse_verilog {

parse::syntax *produce(tokenizer &tokens, void *data) {
	return new module_def(tokens, data);
}

void expect(tokenizer &tokens) {
	tokens.expect<module_def>();
}

void register_syntax(tokenizer &tokens) {
	module_def::register_syntax(tokens);
}

} 
