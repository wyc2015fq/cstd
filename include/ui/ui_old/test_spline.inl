
#include "spline.inl"

int test_spline()
{
  /* x[]内不能出现重复数据 */
  double x[] = {
    0, 0.5, 0.75, 1.25, 2.5,  5, 7.5, 10, 15,
    20,  25,   30,   35,  40, 45,  50, 55, 60,
    65,  70,   75,   80,  85, 90,  95, 100
  };
  double y[] = {
    0, 0.6107, 0.7424, 0.9470, 1.3074, 1.7773,    2.1,
    2.3414, 2.6726, 2.8688, 2.9706, 3.0009, 2.9743, 2.9015,
    2.7904, 2.6470, 2.4762, 2.2817, 2.0662, 1.8320, 1.5802,
    1.3116, 1.0263, 0.7239, 0.4033, 0.0630
  };
  int n = sizeof(x) / sizeof(double);
  coefficient* coe;
  int i;
  point p;
  coe = (coefficient*)malloc((n - 1) * sizeof(coefficient));
  p.xCoordinate = x;
  p.yCoordinate = y;
  p.f0 = 0;
  p.fn = 0;
  p.num = n;
  p.con = NOTaKNOT;
  p.coe = coe;
  spline(&p);

  for (i = 0; i < n - 1; i++) {
    printf("%f\t%f\t%f\t%f\n", coe[i].a3, coe[i].b2, coe[i].c1, coe[i].d0);
  }

  free(coe);
  return 0;
}
