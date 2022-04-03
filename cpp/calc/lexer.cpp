#include "lexer.h"
#include "error.h"
#include "driver.h"
#include <iostream>
#include <cctype>

Lexer::Token_value Lexer::curr_tok;
Lexer::Simvols Lexer::simvol;

Lexer::Token_value Lexer::get_token()
{
  using Driver::input;

  char ch = 0;

  do {
    if (!input->get(ch)) {
      return curr_tok = END;
    }
  } while (ch != '\n' && isspace(ch));
  
  switch(ch) {
  case ':':
    return curr_tok = END;
  case ';':
  case '\n':
    return curr_tok = PRINT;
  case '*':
  case '/':
  case '+':
  case '-':
  case '(':
  case ')':
  case '=':
  case '{':
  case '}':
  case ',':
    return curr_tok = Token_value(ch);
  case '0': case '1': case '2': case '3': case '4': 
  case '5': case '6': case '7': case '8': case '9':
  case '.':
    input->putback(ch);
    *input >> simvol.number_value;

    return curr_tok = NUMBER;
  default:
    if (isalpha(ch)) {
      simvol.string_value = ch;
      while(input->get(ch) && isalnum(ch)) {
	simvol.string_value.push_back(ch);
      }
      input->putback(ch);

      if (simvol.string_value == "func") {
	do {
	  input->get(ch);
	} while (ch != '\n' && isspace(ch));

	simvol.string_value = ch;
	while (input->get(ch) && isalnum(ch)) {
	  simvol.string_value.push_back(ch);
	}
	input->putback(ch);

	return curr_tok = FUNC;
      }
      
      return curr_tok=NAME;
    }
    
    throw Error::Syntax_error("Не верная лексема");
  }
}
