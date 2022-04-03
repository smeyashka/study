#include <string>

namespace Lexer {
  enum Token_value {
    NAME,
    NUMBER,
    FUNC,
    END = ':',
    PLUS = '+',
    MINUS = '-',
    MUL = '*',
    DIV = '/',
    PRINT = ';',
    ASSIGN = '=',
    LP = '(',
    RP = ')',
    LF = '{',
    RF = '}',
    LIST = ','
  };

  extern Token_value curr_tok;

  typedef struct {
    double number_value;
    std::string string_value;
  } Simvols;

  extern Simvols simvol;

  Token_value get_token();
}
