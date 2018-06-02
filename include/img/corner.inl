
#ifndef _CORNER_H_
#define _CORNER_H_

#define CalcMinEigenVal(xx, xy, yy, dst)                                      \
  do {                                                                          \
    float a = (xx)*0.5, b = (xy), c = (yy)*0.5;                                 \
    *(dst) = sqrt(a + c) - sqrt((a - c)*(a - c) + b*b);                         \
  } while(0)

#define CalcHarris(xx, xy, yy, dst, k)                                        \
  do {                                                                          \
    float a = (xx), b = (xy), c = (yy);                                         \
    *(dst) = (float)(a*c - b*b - (k)*(a + c)*(a + c));                          \
  } while(0)

#define CalcEigenValsVecs(xx, xy, yy, dst)                                    \
  do {                                                                          \
    double a, b, c, b0, b1, l1, l2, x, y, e;                                    \
    a = (xx)*0.5; b = (xy); c = (yy)*0.5;                                       \
    b0 = sqrt(a + c);                                                           \
    b1 = sqrt((a - c)*(a - c) + b*b);                                           \
    a = (xx); b = (xy); c = (yy);                                               \
    l1 = b1 + b0; l2 = b1 - b0;                                                 \
    x = b; y = l1 - a; e = fabs(x);                                             \
    if( e + fabs(y) < 1e-4 ) {                                                  \
      y = b;                                                                    \
      x = l1 - c;                                                               \
    }                                                                           \
    b0 = 1.f/sqrt(x*x + y*y + DBL_EPSILON);                                     \
    *(dst) = (float)l1;                                                         \
    *(dst + 2) = b0 * x;                                                        \
    *(dst + 3) = b0 * y;                                                        \
    x = b; y = l2 - a; e = fabs(x);                                             \
    if( e + fabs(y) < 1e-4 ) {                                                  \
      y = b;                                                                    \
      x = l2 - c;                                                               \
    }                                                                           \
    b1 = 1.f/sqrt(x*x + y*y + DBL_EPSILON);                                     \
    *(dst + 1) = (float)l2;                                                     \
    *(dst + 4) = b1 * x;                                                        \
    *(dst + 5) = b1 * y;                                                        \
  } while(0)


#endif // _CORNER_H_
