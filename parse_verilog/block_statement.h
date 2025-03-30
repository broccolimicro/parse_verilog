#pragma once

#include <parse/syntax.h>

#include <vector>
#include <string>
#include <memory>

using namespace std;

namespace parse_verilog {

struct block_statement : parse::syntax {
    block_statement();
    block_statement(tokenizer &tokens, void *data = nullptr);
    ~block_statement();

    vector<shared_ptr<parse::syntax> > sub;

    void parse(tokenizer &tokens, void *data = nullptr);
    static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);

    string to_string(string tab = "") const;
    parse::syntax *clone() const;
};

} // namespace parse_verilog 
