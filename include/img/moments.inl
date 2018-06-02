
#ifndef _MOMENTS_H_
#define _MOMENTS_H_

#include <float.h>

#define  CC_NONZERO(a)      ((a) != 0)
#define  CC_NONZERO_FLT(a)  (((a)+(a)) != 0)

#define ICVCOMPLETEMOMENTSTATE(moments)                                       \
  do {                                                                          \
    double cx = 0, cy = 0;                                                      \
    double mu20, mu11, mu02;                                                    \
    moments.inv_sqrt_m00 = 0;                                                   \
    if( fabs(moments.m00) > DBL_EPSILON ) {                                     \
      double inv_m00 = 1. / moments.m00;                                        \
      cx = moments.m10 * inv_m00;                                               \
      cy = moments.m01 * inv_m00;                                               \
      moments.inv_sqrt_m00 = sqrt( fabs(inv_m00) );                             \
    }                                                                           \
    /* mu20 = m20 - m10*cx */                                                   \
    mu20 = moments.m20 - moments.m10 * cx;                                      \
    /* mu11 = m11 - m10*cy */                                                   \
    mu11 = moments.m11 - moments.m10 * cy;                                      \
    /* mu02 = m02 - m01*cy */                                                   \
    mu02 = moments.m02 - moments.m01 * cy;                                      \
    \
    moments.mu20 = mu20;                                                        \
    moments.mu11 = mu11;                                                        \
    moments.mu02 = mu02;                                                        \
    \
    /* mu30 = m30 - cx*(3*mu20 + cx*m10) */                                     \
    moments.mu30 = moments.m30 - cx * (3 * mu20 + cx * moments.m10);            \
    mu11 += mu11;                                                               \
    /* mu21 = m21 - cx*(2*mu11 + cx*m01) - cy*mu20 */                           \
    moments.mu21 = moments.m21 - cx * (mu11 + cx * moments.m01) - cy * mu20;    \
    /* mu12 = m12 - cy*(2*mu11 + cy*m10) - cx*mu02 */                           \
    moments.mu12 = moments.m12 - cy * (mu11 + cy * moments.m10) - cx * mu02;    \
    /* mu03 = m03 - cy*(3*mu02 + cy*m01) */                                     \
    moments.mu03 = moments.m03 - cy * (3 * mu02 + cy * moments.m01);            \
  } while(0)

/****************************************************************************************\
*                                   Spatial Moments                                      *
\****************************************************************************************/

#define _MOMENTS_OP(__op__, momtype, height, width, img, step, ai, moments)   \
  do {                                                                          \
    int x, y, sx_init = (width & -4) * (width & -4), sy = 0;                    \
    int a0, a1, cn = (ai);                                                      \
    momtype mom[10];                                                            \
    memset( mom, 0, 10 * sizeof(mom[0]));                                       \
    memset( &moments, 0, sizeof(moments));                                      \
    for( y = 0, a1 = 0; y < height; sy += 2 * y + 1, y++, a1 += (step)) {       \
      momtype x0 = 0, x1 = 0, x2 = 0, x3 = 0;                                   \
      int sx = sx_init;                                                         \
      for( x = 0, a0 = a1; x < width - 3; x += 4, a0 += (cn)*4) {               \
        momtype p0 = __op__(*(img+a0)), p1 = __op__(*(img+a0 + cn));            \
        momtype p2 = __op__(*(img+a0 + 2*cn)), p3 = __op__(*(img+a0 + 3*cn));   \
        momtype t = p1, a, b, c;                                                \
        p0 += p1 + p2 + p3;        /* p0 + p1 + p2 + p3 */                      \
        p1 += 2 * p2 + 3 * p3;     /* p1 + p2*2 + p3*3  */                      \
        p2 = p1 + 2 * p2 + 6 * p3; /* p1 + p2*4 + p3*9  */                      \
        p3 = 2 * p2 - t + 9 * p3;  /* p1 + p2*8 + p3*27 */                      \
        a = x * p0 + p1; /* x*p0 + (x+1)*p1 + (x+2)*p2 + (x+3)*p3 */            \
        b = x * p1 + p2; /* (x+1)*p1 + 2*(x+2)*p2 + 3*(x+3)*p3 */               \
        c = x * p2 + p3; /* (x+1)*p1 + 4*(x+2)*p2 + 9*(x+3)*p3 */               \
        x0 += p0;                                                               \
        x1 += a;                                                                \
        a = a * x + b;                                                          \
        /*(x^2)*p0+((x+1)^2)*p1+((x+2)^2)*p2+((x+3)^2)*p3 */                    \
        x2 += a;                                                                \
        /*x3 += (x^3)*p0+((x+1)^3)*p1 + ((x+2)^3)*p2+((x+3)^3)*p3 */            \
        x3 += ((momtype)(a + b)) * x + c;                                       \
      }                                                                         \
      /* process the rest */                                                    \
      for( ; x < width; sx += 2 * x + 1, x++, a0 += cn) {                       \
        momtype p = __op__(img[a0]);                                            \
        momtype xp = x * p;                                                     \
        x0 += p;                                                                \
        x1 += xp;                                                               \
        x2 += sx * p;                                                           \
        x3 += ((momtype)sx) * xp;                                               \
      }                                                                         \
      {                                                                         \
        momtype py = y * x0;                                                    \
        mom[9] += ((momtype)py) * sy;  /* m03 */                                \
        mom[8] += ((momtype)x1) * sy;  /* m12 */                                \
        mom[7] += ((momtype)x2) * y;   /* m21 */                                \
        mom[6] += x3;                  /* m30 */                                \
        mom[5] += x0 * sy;             /* m02 */                                \
        mom[4] += x1 * y;              /* m11 */                                \
        mom[3] += x2;                  /* m20 */                                \
        mom[2] += py;                  /* m01 */                                \
        mom[1] += x1;                  /* m10 */                                \
        mom[0] += x0;                  /* m00 */                                \
      }                                                                         \
    }                                                                           \
    moments.m03 = mom[9];   /* m03 */                                           \
    moments.m12 = mom[8];   /* m12 */                                           \
    moments.m21 = mom[7];   /* m21 */                                           \
    moments.m30 = mom[6];   /* m30 */                                           \
    moments.m02 = mom[5];   /* m02 */                                           \
    moments.m11 = mom[4];   /* m11 */                                           \
    moments.m20 = mom[3];   /* m20 */                                           \
    moments.m01 = mom[2];   /* m01 */                                           \
    moments.m10 = mom[1];   /* m10 */                                           \
    moments.m00 = mom[0];   /* m00 */                                           \
  } while(0)

#define _MOMENTS(momtype, height, width, img, step, ai, moments)              \
  _MOMENTS_OP(CC_NONZERO, momtype, height, width, img, step, ai, moments);    \
  ICVCOMPLETEMOMENTSTATE(moments)

#endif // _MOMENTS_H_
