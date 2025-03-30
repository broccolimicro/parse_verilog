#include "always.h"

namespace parse_verilog {

always::always() {
    debug_name = "always";
}

always::~always() {
}

void always::parse(tokenizer &tokens, void *data) {
    tokens.syntax_start(this);
    // Base implementation does nothing
    tokens.syntax_end(this);
}

bool always::is_next(tokenizer &tokens, int i, void *data) {
    // For now, just check for a few basic module items
    // We'll expand this as we implement more types
    return tokens.is_next("always", i);
}

void always::register_syntax(tokenizer &tokens) {
    if (!tokens.syntax_registered<always>()) {
        tokens.register_syntax<always>();
        
        // Register specific module item types
        // We'll add these as we implement them
    }
}

std::string always::to_string(std::string tab) const {
    return tab + "always";
}

parse::syntax *always::clone() const {
    return new always(*this);
}

} 
