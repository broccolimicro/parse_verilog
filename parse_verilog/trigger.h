#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

#include <parse_verilog/block_statement.h>
#include <parse_verilog/expression.h>

#include <string>

using namespace std;

namespace parse_verilog {

struct trigger : parse::syntax {
    trigger();
    trigger(tokenizer &tokens, std::any data=std::any());
    ~trigger();

    expression condition;
    block_statement body;

    void parse(tokenizer &tokens, std::any data=std::any());
    static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
    static void register_syntax(tokenizer &tokens);

    string to_string(string tab = "") const;
    parse::syntax *clone() const;
};

} 
