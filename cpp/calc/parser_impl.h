#include "parser.h"
#include "lexer.h"
#include <map>
#include <vector>
#include <list>
#include <cmath>
#include <iostream>

namespace Funcser {
  using namespace std;

  struct func_type { //тип функци, вводимой пользователем
    string func_name;
    string func_body;
    map<string, double> func_param;
    vector<char*> param_list;
  };
  
  typedef list<func_type>::iterator LI;
  list<func_type> func;
  LI func_value = func.end();

  LI find_func(const string& name);//возвращает итератор функции name из списка func
}

namespace Parser {
  typedef double (*PF)(double); // указатель на функции, реализованные в программе
  PF pfunc[4] = { &sqrt, &sin, &log };
  
  double prim(bool get);
  double prim_name();
  double prim_func();
  double term(bool get);
  double expr(bool get);
  double trig_func(PF func); //возвращает результат выпоснения функции func

  using Lexer::get_token;
  using Lexer::curr_tok;
}


