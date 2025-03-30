#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_expression/expression.h>

#include <string>

using namespace std;

namespace parse_verilog {

struct trigger : parse::syntax {
    trigger();
    trigger(tokenizer &tokens, void *data = nullptr);
    ~trigger();

    parse_expression::expression condition;
    block_statement body;

    void parse(tokenizer &tokens, void *data = nullptr);
    static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
    static void register_syntax(tokenizer &tokens);

    string to_string(string tab = "") const;
    parse::syntax *clone() const;
};

} 
