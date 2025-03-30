#include "factory.h"
#include "module.h"
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>
#include <parse/default/line_comment.h>
#include <parse/default/block_comment.h>

namespace parse_verilog {

parse::syntax *produce(tokenizer &tokens, void *data) {
    return new module(tokens, data);
}

void expect(tokenizer &tokens) {
    tokens.expect<module>();
}

void register_syntax(tokenizer &tokens) {
    // Register basic tokens
    tokens.register_token<parse::white_space>(false);
    tokens.register_token<parse::new_line>(true);
    tokens.register_token<parse::line_comment>(false);
    tokens.register_token<parse::block_comment>(false);
    
    // Register verilog syntax
    module::register_syntax(tokens);
}

} 