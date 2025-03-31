#include "number.h"

namespace parse_verilog
{
number::number() {
	debug_name = "number";
}

number::~number() {
}

token number::consume(tokenizer &tokens, void *data) {
	token result;
	result.type = tokens.token_type<number>();
	result.start = tokens.offset+1;

	char character = tokens.peek_char(1);

	enum {
		hexidecimal = 3,
		decimal = 2,
		octal = 1,
		binary = 0,
	} type = decimal;

	if (tokens.peek_char(2) == 'h') {
		type = hexidecimal;
		tokens.next_char();
		tokens.next_char();
	} else if (tokens.peek_char(2) == 'o') {
		type = octal;
		tokens.next_char();
		tokens.next_char();
	} else if (tokens.peek_char(2) == 'b') {
		type = binary;
		tokens.next_char();
		tokens.next_char();
	}

	bool foundBitwidthSpec = false;
	while (character == '_' or
	     (type == hexidecimal and ((character >= '0' and character <= '9') or (character >= 'a' and character <= 'f') or (character >= 'A' and character <= 'F'))) or
		   (type == decimal and (character >= '0' and character <= '9')) or
		   (type == octal and (character >= '0' and character <= '7')) or
		   (type == binary and (character >= '0' and character <= '1')) or
	     (not foundBitwidthSpec and character == '\'')) {
		tokens.next_char();
		character = tokens.peek_char(1);

		if (character == '\'') {
			do {
				tokens.next_char();
				character = tokens.peek_char(1);
			} while (character == 's' or character == 'h' or character == 'b' or character == 'h' or character == 'o');
			foundBitwidthSpec = true;
		}
	}

	if (type == decimal and character == '.' and tokens.peek_char(2) >= '0' and tokens.peek_char(2) <= '9') {
		tokens.next_char();
		character = tokens.peek_char(1);
		while (character >= '0' and character <= '9') {
			tokens.next_char();
			character = tokens.peek_char(1);
		}
	}

	if (type == decimal and (character == 'e' or character == 'E') and (tokens.peek_char(2) == '-' or (tokens.peek_char(2) >= '0' and tokens.peek_char(2) <= '9'))) {
		tokens.next_char();
		character = tokens.peek_char(1);
		if (character == '-') {
			tokens.next_char();
			character = tokens.peek_char(1);
		}

		if (character < '0' or character > '9') {
			tokens.token_error((string)"exponent has no digits", __FILE__, __LINE__);
		}

		while (character >= '0' and character <= '9') {
			tokens.next_char();
			character = tokens.peek_char(1);
		}
	}

	result.end = tokens.offset+1;
	return result;
}

bool number::is_next(tokenizer &tokens, int i, void *data) {
	if (tokens.peek_char(i) == '-')
		i++;

	if (tokens.peek_char(i) == '\'') {
		char c;
		do {
			c = tokens.peek_char(++i);
		} while (c == 's' or c == 'h' or c == 'b' or c == 'h' or c == 'o');
	}

	return (tokens.peek_char(i) >= '0' && tokens.peek_char(i) <= '9');
}

}
