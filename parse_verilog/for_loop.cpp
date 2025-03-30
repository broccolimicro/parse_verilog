#include "for_loop.h"
#include <parse/default/symbol.h>
#include <parse/default/instance.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_verilog {

for_loop::for_loop() {
    debug_name = "for_loop";
    init_var = "";
    incr_var = "";
}

for_loop::for_loop(tokenizer &tokens, void *data) {
    debug_name = "for_loop";
    init_var = "";
    incr_var = "";
    parse(tokens, data);
}

for_loop::~for_loop() {
}

void for_loop::parse(tokenizer &tokens, void *data) {
    tokens.syntax_start(this);

    // Push tokens in reverse order of their appearance in source code
    
    // Parse body (last in source)
    tokens.increment(true);
    tokens.expect<block_statement>();
    
    // Parse closing parenthesis
    tokens.increment(true);
    tokens.expect(")");
    
    // Parse increment expression
    tokens.increment(true);
    tokens.expect<parse_expression::expression>();
    
    // Parse increment variable
    tokens.increment(true);
    tokens.expect<parse::instance>();
    
    // Parse equals sign in increment
    tokens.increment(true);
    tokens.expect("=");
    
    // Parse second semicolon
    tokens.increment(true);
    tokens.expect(";");
    
    // Parse condition
    tokens.increment(true);
    tokens.expect<parse_expression::expression>();
    
    // Parse first semicolon
    tokens.increment(true);
    tokens.expect(";");

    // Parse initialization expression
    tokens.increment(true);
    tokens.expect<parse_expression::expression>();
    
    // Parse initialization variable
    tokens.increment(true);
    tokens.expect<parse::instance>();
    
    // Parse equals sign in initialization
    tokens.increment(true);
    tokens.expect("=");
    
    // Parse opening parenthesis
    tokens.increment(true);
    tokens.expect("(");

    // Parse "for" keyword (first in source)
    tokens.increment(true);
    tokens.expect("for");

    // Process tokens in correct order (as they appear in source)
    if (tokens.decrement(__FILE__, __LINE__, data)) {
        tokens.next(); // for
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        tokens.next(); // (
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        tokens.next(); // =
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        init_var = tokens.next(); // variable being initialized
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        init_expr.parse(tokens, data);
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        tokens.next(); // ;
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        condition.parse(tokens, data);
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        tokens.next(); // ;
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        tokens.next(); // =
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        incr_var = tokens.next(); // variable being incremented
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        incr_expr.parse(tokens, data);
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        tokens.next(); // )
    }

    if (tokens.decrement(__FILE__, __LINE__, data)) {
        body.parse(tokens, data);
    }

    tokens.syntax_end(this);
}

bool for_loop::is_next(tokenizer &tokens, int i, void *data) {
    return tokens.is_next("for", i);
}

void for_loop::register_syntax(tokenizer &tokens) {
    if (!tokens.syntax_registered<for_loop>()) {
        tokens.register_syntax<for_loop>();
        tokens.register_token<parse::symbol>();
        tokens.register_token<parse::instance>();
        tokens.register_token<parse::white_space>(false);
        tokens.register_token<parse::new_line>(false);
        
        // Register components
        parse_expression::expression::register_syntax(tokens);
        block_statement::register_syntax(tokens);
    }
}

string for_loop::to_string(string tab) const {
    if (!valid) {
        return "";
    }
    
    string result = "for (";
    result += init_var + " = " + init_expr.to_string(tab) + "; ";
    result += condition.to_string(tab) + "; ";
    result += incr_var + " = " + incr_expr.to_string(tab) + ")\n";
    result += tab + body.to_string(tab);
    
    return result;
}

parse::syntax *for_loop::clone() const {
    for_loop *result = new for_loop();
    result->init_var = init_var;
    result->init_expr = init_expr;
    result->condition = condition;
    result->incr_var = incr_var;
    result->incr_expr = incr_expr;
    result->body = body;
    return result;
}

} // namespace parse_verilog 