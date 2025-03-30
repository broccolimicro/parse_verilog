#pragma once

#include <parse/syntax.h>
#include <parse_verilog/expression.h>
#include <parse_verilog/assignment_statement.h>
#include <parse_verilog/block_statement.h>

#include <string>

using namespace std;

namespace parse_verilog {

struct loop_statement : parse::syntax {
    loop_statement();
    loop_statement(tokenizer &tokens, void *data = nullptr);
    ~loop_statement();

		assignment_statement init;
		expression cond;
		assignment_statement step;
    block_statement body;

    void parse(tokenizer &tokens, void *data = nullptr);
    static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
    static void register_syntax(tokenizer &tokens);

    string to_string(string tab = "") const;
    parse::syntax *clone() const;
};

} // namespace parse_verilog 
