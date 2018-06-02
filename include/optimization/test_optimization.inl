
#include <stdio.h>
#include <stdlib.h>
#include "golds.inl"


double fun1(double s) {
  return s*s-sin(s);
}

int test_optimization() {
  if (1) {
    double s,phis,E[2];
    int k;
    {
      utime_start(_start_time);
      k = golds(fun1,0,1,1e-4,1e-5, &s, &phis, 0, E);
      printf("%f\n", utime_elapsed(_start_time));
    }
    printf("\n");
  }
  return 0;
}
