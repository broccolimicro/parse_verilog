#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

namespace parse_verilog {

parse::syntax *produce(tokenizer &tokens, void *data=nullptr);
void expect(tokenizer &tokens);
void register_syntax(tokenizer &tokens);

} 