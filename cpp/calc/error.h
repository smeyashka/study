namespace Error {
  extern int no_of_errors;

   struct Zero_divide {
    Zero_divide() { no_of_errors++; }
  };

  struct Syntax_error {
    const char* p;
    Syntax_error(const char* q) { p = q; no_of_errors++; }
  };

  struct Limit_error {
    char sim;
    Limit_error(char ch) { sim = ch; }
  }; 
}
