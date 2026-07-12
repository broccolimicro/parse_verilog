#pragma once

#include <parse/syntax.h>
#include <parse/tokenizer.h>

#include <vector>
#include <string>
#include <memory>

using namespace std;

namespace parse_verilog {

struct block_statement : parse::syntax {
    block_statement();
    block_statement(tokenizer &tokens, std::any data=std::any());
    ~block_statement();

    vector<shared_ptr<parse::syntax> > sub;

    void parse(tokenizer &tokens, std::any data=std::any());
    static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
		static void register_syntax(tokenizer &tokens);

    string to_string(string tab = "") const;
    string to_string(string tab, bool bookend) const;
    parse::syntax *clone() const;
};

} // namespace parse_verilog 
