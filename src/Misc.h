
#ifndef MISC_H
#define MISC_H

#include <newmat/newmat.h>

#include <stdexcept>

inline void crossproduct_body(NEWMAT::Real* a, NEWMAT::Real* b, NEWMAT::Real* c)
{
   c[0] = a[1] * b[2] - a[2] * b[1];
   c[1] = a[2] * b[0] - a[0] * b[2];
   c[2] = a[0] * b[1] - a[1] * b[0];
}


static NEWMAT::Matrix crossproduct(const NEWMAT::Matrix& A, const NEWMAT::Matrix& B)
{
   int ac = A.Ncols(); int ar = A.Nrows();
   int bc = B.Ncols(); int br = B.Nrows();
   NEWMAT::Real* a = A.Store(); NEWMAT::Real* b = B.Store();
   if (ac == 3)
   {
      if (bc != 3 || ar != 1 || br != 1)
      {
          throw std::invalid_argument("crossproduct: incompatible dimensions");
      }

      NEWMAT::RowVector C(3);
      NEWMAT::Real* c = C.Store();

      crossproduct_body(a, b, c);
      return (NEWMAT::Matrix&)C;
   }
   else
   {
      if (ac != 1 || bc != 1 || ar != 3 || br != 3)
      {
          throw std::invalid_argument("crossproduct: incompatible dimensions");
      }

      NEWMAT::ColumnVector C(3);
      NEWMAT::Real* c = C.Store();
      crossproduct_body(a, b, c);
      return (NEWMAT::Matrix&)C;
   }
}




#endif // MISC_H
