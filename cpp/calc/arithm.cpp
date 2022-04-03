#include "arithm.h"
#include "error.h"
#include <limits>
#include <cmath>

using std::numeric_limits;

void Arithm::plus(double *slag1, double slag2)
{
  if (*slag1 > 0 && slag2 < 0) {
    minus(slag1, -slag2);
  }
  else if (*slag1 < 0 && slag2 > 0) {
    *slag1+=slag2;
  }
  else {
    if (numeric_limits<double>::max() - fabs(*slag1) > fabs(slag2)) {
      *slag1 += slag2;
    }
    else throw Error::Limit_error('+');
  }
}

void Arithm::minus(double *slag1, double slag2)
{
  if (*slag1 > 0 && slag2 < 0) {
    plus(slag1,-slag2);
  }
  else if (*slag1 < 0 && slag2 > 0) {
    plus(slag1,-slag2);
  }
  else {
    if (-numeric_limits<double>::max() + fabs(slag2) < fabs(*slag1)) {
      *slag1 -= slag2;
    }
    else throw Error::Limit_error('-');
  }
}


void Arithm::multiply(double *slag1, double slag2)
{
  if (*slag1 == 0 || slag2 == 0) {
    *slag1 = 0;
    return;
  }
  if ( (std::numeric_limits<double>::max() / *slag1 > slag2) &&
       (std::numeric_limits<double>::min() / *slag1 < slag2 )) {
    *slag1 *= slag2;
  }
  else throw Error::Limit_error('*');
}

void Arithm::divide(double *slag1, double slag2)
{
  if (*slag1 == 0) return;
    if (slag2 == 0) throw Error::Zero_divide();
    
  if ( (std::numeric_limits<double>::min() * slag2 < *slag1) &&
       (std::numeric_limits<double>::max() * slag2 > *slag1)) {
    *slag1 /= slag2;
  }
  else throw Error::Limit_error('/');
}
