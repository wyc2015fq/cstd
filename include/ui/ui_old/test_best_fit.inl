
#include "cv/svd.inl"
#include "cv/matrix.inl"
#include "vcgen.inl"
#include "best_fit.inl"


int test_best_fit(img_t* im)
{
  enum {nn = 100};
  DPOINT2 pt[nn];
  double aa[10] = {0};
  //vcgen_rand_line(nn, pt, 10, 10, 100, 200, 1);
  //BestFit(nn, pt, FitT_Line, aa, 0, 0);
  vcgen_rand_circle(nn, pt, 100, 100, 100, 10);
  BestFit(nn, pt, FitT_Circle, aa, 0, 0);
  return 0;
}
