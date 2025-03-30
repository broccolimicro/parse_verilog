#pragma once

#include <parse/syntax.h>
#include <parse_expression/expression.h>
#include <parse_verilog/block_statement.h>

#include <string>

using namespace std;

namespace parse_verilog {

struct for_loop : parse::syntax {
    for_loop();
    for_loop(tokenizer &tokens, void *data = nullptr);
    ~for_loop();

    // Initial assignment (e.g., i = 0)
    string init_var;
    parse_expression::expression init_expr;

    // Condition (e.g., i < 10)
    parse_expression::expression condition;

    // Increment (e.g., i = i + 1)
    string incr_var;
    parse_expression::expression incr_expr;

    // Loop body
    block_statement body;

    void parse(tokenizer &tokens, void *data = nullptr);
    static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
    static void register_syntax(tokenizer &tokens);

    string to_string(string tab = "") const;
    parse::syntax *clone() const;
};

} // namespace parse_verilog 