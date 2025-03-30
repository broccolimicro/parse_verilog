#include "factory.h"
#include "module.h"
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>
#include <parse/default/line_comment.h>
#include <parse/default/block_comment.h>

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
