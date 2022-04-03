#include "parser_impl.h"
#include "table.h"
#include "driver.h"
#include "arithm.h"
#include "error.h"

#include <sstream>
#include <cstring>

Funcser::LI Funcser::find_func(const string& name)
{
  cerr << name;
  for (LI it = func.begin(); it != func.end(); ++it) {
    func_type& f = *it; 
    if (name == f.func_name) {
      return it;
    }
  }
  return func.end();
}

double Parser::trig_func(PF func)
{
  if (get_token() != Lexer::LP) throw Error::Syntax_error("Ожидалась (");
  
  double arg = expr(true);
  if (curr_tok != Lexer::RP) throw Error::Syntax_error("Ожидалась )");
  
  get_token();
  return func(arg);
}

double Parser::prim(bool get)
{
  using namespace Funcser;
  using namespace Lexer;
  //  using Driver::input;
  
  //  using Lexer::simvol;

  if (get) {
    get_token();
  }

  switch (curr_tok) {
  case Lexer::NUMBER: {
    double v = simvol.number_value;
    get_token();
    return v;
  }
  case Lexer::NAME:  
    return prim_name();
  case Lexer::FUNC:
    return prim_func();
  case Lexer::MINUS:
    return -prim(true);
  case LP: {
    double e = expr(true);
    if (curr_tok != RP) {
      throw Error::Syntax_error("Ожидалась (");
    }
    get_token();
    return e;
  }
  default:
    //   input->get(ch);
    throw Error::Syntax_error("Ожидалась первичное выражение");
  }
}


double Parser::prim_name()
{
  using namespace Funcser;
  using namespace Lexer;
  using Driver::input;
  
  if (simvol.string_value == "sqrt") return trig_func(pfunc[0]);
  if (simvol.string_value == "sin")  return trig_func(pfunc[1]);
  if (simvol.string_value == "log")  return trig_func(pfunc[2]);
  
  LI f_iter = find_func(simvol.string_value);
  if (f_iter == func.end() ) { //если не имя функции
    double* v;
    if (input == &cin) { //переменная
      v = &table[simvol.string_value];
    } else { // параметр функции
      f_iter = func_value;
      if (f_iter == func.end()) {
	throw Error::Syntax_error("Ожидалась первичное выражение");	
      }
      v = &f_iter->func_param[simvol.string_value];
    }
    if (get_token() == ASSIGN) {
      *v = expr(true);
    }
    return *v;
    
  } else {
    double v = 0;
    if (get_token() != LP) {
      throw Error::Syntax_error("Ожидалась (");
    }
    uint idx = 0;
    while (get_token() != RP && idx < f_iter->param_list.size()) { //присваиваем значения параметрам
      //  get_token();
      if (curr_tok == LIST) {
	continue;
      }
      if (curr_tok != NUMBER) {
	throw Error::Syntax_error("Ожидалось значение параметра");
      }
      f_iter->func_param[f_iter->param_list[idx]] = simvol.number_value;
      idx++;
    }
    
    input = new istringstream(f_iter->func_body);
    
    while (curr_tok != END) {
      get_token();
      if (curr_tok == PRINT || curr_tok == END) {
	continue;
      }
      func_value = f_iter;
      v = expr(false);
    }
    
    delete input;
    input = &cin;
    if (get_token() == ASSIGN) {
      throw Error::Syntax_error("Невозможно присвоить знчение имени функции");
    }
    return v;
  }     
}

double Parser::prim_func()
{
  using namespace Funcser;
  using namespace Lexer;
  using Driver::input;
  char ch = 0;

  LI f_iter = find_func(simvol.string_value);
  if (f_iter != func.end()) {
    cout << "Функция с таким именем уже есть. Она будет удалена" << endl;
    // 	char answer;
    // 	cin>>answer;
    // 	if (answer=='y') 
    func.erase(f_iter);
    // 	else
    // 	  {
    //считать остатки??
    //   return 0;
    // 	}
  }
  
  func_type func_new;
  func_new.func_name = simvol.string_value;
  
  if (get_token() != LP)  {
    throw Error::Syntax_error("Ожидалась (");
  }
  //обнулить func.func_param.clear();
  
  int idx = 0;
  while (get_token() != RP) {
    if (curr_tok == LIST) {
      continue;
    }
    if (curr_tok != NAME) {
      throw Error::Syntax_error("Ожидалось название параметра");//return error("ожидалось название параметра");
    }
    
    func_new.param_list.resize(func_new.param_list.size()+1);
    func_new.param_list[idx] = new char[simvol.string_value.length()];
    strcpy(func_new.param_list[idx], simvol.string_value.c_str());
    idx++;
  }
  
  if (get_token() != LF) {
    for (uint idx = 0; idx < func_new.param_list.size(); idx++) {
      delete[] func_new.param_list[idx];
    }
    func_new.param_list.resize(0);
    throw Error::Syntax_error("Ожидалась {");
  }
  
  //      func_new.func_body="";
  while(input->get(ch) && ch != '}') {
    func_new.func_body.push_back(ch);
  }
  
  input->get(ch);
  func.push_back(func_new);
  
  // вродь значение не копируется, а берется этот адрес => удалаять не надо
  //  for (int i=0;i<func_new.param_list.size();i++)
  // 	delete[] func_new.param_list[i];
  //       func_new.param_list.resize(0);
  
  return 0;
}


		    
double Parser::term(bool get)
{
  double left = prim(get);
  
  for (;;) {
    switch(curr_tok) {
    case Lexer::MUL:
      Arithm::multiply(&left, prim(true));
      break;
    case Lexer::DIV:
      Arithm::divide(&left, prim(true));
    default:
      return left;
    }
  }
}

double Parser::expr(bool get)
{
  double left = term(get);

  for(;;) {
    switch(curr_tok) {
    case Lexer::PLUS:
      Arithm::plus(&left, term(true));
      break;
    case Lexer::MINUS:
      Arithm::minus(&left, term(true));
      break;
    default:
      return left;
    }
  }
}
