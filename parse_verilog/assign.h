#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

namespace parse_verilog {

// Base class for all module items
struct assign : parse::syntax {
    assign();
    virtual ~assign();

    virtual void parse(tokenizer &tokens, void *data = nullptr);
    static bool is_next(tokenizer &tokens, int i = 1, void *data = nullptr);
    static void register_syntax(tokenizer &tokens);

    virtual std::string to_string(std::string tab = "") const override;
    virtual parse::syntax *clone() const override;
};

} 
