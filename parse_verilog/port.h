#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include <parse_expression/expression.h>
#include <string>
#include <memory>

using namespace std;

namespace parse_verilog {

enum data_type {
    IMPLICIT,
    WIRE,
    REG,
    INTEGER
};

struct port : parse::syntax {
    port();
    port(tokenizer &tokens, void *data = nullptr);
    ~port();

    string name;
		bool input;
		bool output;
    data_type type;
    bool is_vector;
    parse_expression::expression msb;
    parse_expression::expression lsb;

    void parse(tokenizer &tokens, void *data = nullptr);
    static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
    static void register_syntax(tokenizer &tokens);

    std::string to_string(std::string tab = "") const override;
    parse::syntax *clone() const override;
};

} 
