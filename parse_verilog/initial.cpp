#include "initial.h"

namespace parse_verilog {

initial::initial() {
    debug_name = "initial";
}

initial::~initial() {
}

void initial::parse(tokenizer &tokens, void *data) {
    tokens.syntax_start(this);
    // Base implementation does nothing
    tokens.syntax_end(this);
}

bool initial::is_next(tokenizer &tokens, int i, void *data) {
    // For now, just check for a few basic module items
    // We'll expand this as we implement more types
    return tokens.is_next("initial", i);
}

void initial::register_syntax(tokenizer &tokens) {
    if (!tokens.syntax_registered<initial>()) {
        tokens.register_syntax<initial>();
        
        // Register specific module item types
        // We'll add these as we implement them
    }
}

std::string initial::to_string(std::string tab) const {
    return tab + "initial";
}

parse::syntax *initial::clone() const {
    return new initial(*this);
}

} 
