#include "parser.h"
#include "lexer.h"
#include "arithm.h"
#include "error.h"
#include "table.h"
#include "driver.h"
#include <iostream>

std::istream* Driver::input = nullptr;
int Error::no_of_errors;
int Driver::rowNum;

void Driver::skip()
{
  while (*input) {
    char ch;
    input->get(ch);
    switch (ch) {
    case '\n':
    case ';':
      return;
    }
  }
}

int Driver::calculator(int argc, char* argv[]) 
{
  using namespace Error;
  using std::cin;
  using std::cout;
  using Lexer::curr_tok;
  using Lexer::get_token;
  //  using Driver::input;

//   std::istream* input;
//  int Error::no_of_errors=0;

  rowNum = 1;

  switch (argc) {
  case 1:
    input = &cin;
    cout << "Для выхода \":\"\n";
    break;
  case 2:
    input = new std::istringstream(argv[1]);
    break;
  default:
    std::cerr << "Количество параметров превышает допустимое\n";
    return 1;
  }

  table["pi"] = 3.141592;
  table["e"] = 2.7182818;

  while(*input) {
    try {
      cout << rowNum << " > ";
      get_token();
      if (curr_tok == Lexer::END)   { break; }
      if (curr_tok == Lexer::PRINT) { continue; }
      
      cout << Parser::expr(false) << '\n';
      if (curr_tok != Lexer::END) {
	rowNum++;
      }
    }

    catch (Error::Zero_divide) {
      std::cerr << "Попытка деления на 0 в строке " << rowNum << std::endl;
      if (curr_tok != Lexer::PRINT) {
	skip();
      }
      //  rowNum++; //для неинтерактивного взаимодействия
    }
    catch (Error::Syntax_error e) {
      std::cerr << "Синтаксическая ошибка в строке " << rowNum << " : " << e.p << "\n";
      if (curr_tok != Lexer::PRINT) {
	skip();
      }
      // rowNum++; //для неинтерактивного взаимодействия
    }
    catch (Error::Limit_error er) {
      std::cerr<<"результат выражения вне диапазаона\n";
      if (curr_tok != Lexer::PRINT) {
	skip();
      }
      // rowNum++; //для неинтерактивного взаимодействия
    }
  }
  

  if (input != &std::cin) {
    delete input;
    input = nullptr;
  }

  // func.erase
  //   for (int i=0;i<func.spis_param.size();i++)
  //     delete[] func.spis_param[i];
  //   func.spis_param.resize(0);

  return no_of_errors;
}
