
#include "cstd.h"
#include "img/imgio.inl"
#include "mex.h"
#include "Types.h"
#include "serialize.inl"
#include "token.inl"
#include "scanner.inl"
#include "TREE.inl"
#include "parser.inl"
#include "Array.inl"
#include "scope.inl"
#include "lapack/lapack.inl"
#include "FunctionDef.h"
#include "context.inl"
#include "mymatmul.inl"
#include "interpreter.h"
#include "Operators.h"
//#include "cv/matrix.inl"
//#include "math/matrix.inl"
#include "MatrixMultiply.inl"
#include "IndexArray.inl"
#include "Algorithms.inl"
#include "math.inl"
#include "Class.inl"
#include "interpreter.inl"

int test_freemat()
{
  str_t s[1] = {0};
  tree_t tr[1] = {0};
  _chdir("E:/pub/bin/freemat");
  //return test_mymatmul();
  str_load("test.m", s);
  //ParseExpressionString(s->s, s->l, tr);
  ParseString(s->s, s->l, tr);
  tree_print(tr, 0);
  tree_free(tr);
  str_free(s);
  return 0;
}
