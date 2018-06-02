
#ifndef _EIGEN_H_
#define _EIGEN_H_

#include <math.h>
#include <float.h>

#define JACOBI_EIGENS(_N, _A, _V, _E, real)                                   \
  do {                                                                          \
    /*-------- init --------*/                                                  \
    int i, j, k, p, q, ind, iters = 0;                                          \
    real *A1 = _A, *V1 = _V, *A2 = _A, *V2 = _V;                                \
    real Amax = 0.0, anorm = 0.0, ax, eps = DBL_EPSILON;                        \
    /*-------- Prepare --------*/                                               \
    for( i = 0; i < (_N); i++, V1 += (_N), A1 += (_N) ) {                       \
      for( j = 0; j < i; j++ ) {                                                \
        real Am = A1[j];                                                        \
        anorm += Am * Am;                                                       \
      }                                                                         \
      for( j = 0; j < (_N); j++ )                                               \
        V1[j] = 0.0f;                                                           \
      V1[i] = 1.0f;                                                             \
    }                                                                           \
    anorm = (real)sqrt( anorm + anorm );                                        \
    ax = anorm * eps / (_N);                                                    \
    Amax = anorm;                                                               \
    while( Amax > ax && iters++ < 100 ) {                                       \
      Amax /= (_N);                                                             \
      do {                     /* while (ind) */                                \
        ind = 0;                                                                \
        A1 = _A;                                                                \
        V1 = _V;                                                                \
        for( p = 0; p < (_N) - 1; p++, A1 += (_N), V1 += (_N) ) {               \
          A2 = _A + (_N) * (p + 1);                                             \
          V2 = _V + (_N) * (p + 1);                                             \
          for( q = p + 1; q < (_N); q++, A2 += (_N), V2 += (_N) ) {             \
            real x, y, c, s, c2, s2, a;                                         \
            real *A3, Apq, App, Aqq, App2, Aqq2, Aip, Aiq, Vpi, Vqi;            \
            if( fabs( A1[q] ) < Amax )                                          \
              continue;                                                         \
            Apq = A1[q];                                                        \
            ind = 1;                                                            \
            /*---- Calculation of rotation angle's sine & cosine ----*/         \
            App = A1[p];                                                        \
            Aqq = A2[q];                                                        \
            y = 5.0e-1f * (App - Aqq);                                          \
            x = -Apq / (real)sqrt( Apq * Apq + (real)y * y );                   \
            if( y < 0.0 )                                                       \
              x = -x;                                                           \
            s = x / (real)sqrt(2.f * (1.f + (real)sqrt(1.f - (real)x*x)));      \
            s2 = s * s;                                                         \
            c = (real)sqrt( 1.0 - s2 );                                         \
            c2 = c * c;                                                         \
            a = 2.0f * Apq * c * s;                                             \
            /*---- Apq annulation ----*/                                        \
            A3 = _A;                                                            \
            for( i = 0; i < p; i++, A3 += (_N) ) {                              \
              Aip = A3[p];                                                      \
              Aiq = A3[q];                                                      \
              Vpi = V1[i];                                                      \
              Vqi = V2[i];                                                      \
              A3[p] = Aip * c - Aiq * s;                                        \
              A3[q] = Aiq * c + Aip * s;                                        \
              V1[i] = Vpi * c - Vqi * s;                                        \
              V2[i] = Vqi * c + Vpi * s;                                        \
            }                                                                   \
            for( ; i < q; i++, A3 += (_N) ) {                                   \
              Aip = A1[i];                                                      \
              Aiq = A3[q];                                                      \
              Vpi = V1[i];                                                      \
              Vqi = V2[i];                                                      \
              A1[i] = Aip * c - Aiq * s;                                        \
              A3[q] = Aiq * c + Aip * s;                                        \
              V1[i] = Vpi * c - Vqi * s;                                        \
              V2[i] = Vqi * c + Vpi * s;                                        \
            }                                                                   \
            for( ; i < (_N); i++ ) {                                            \
              Aip = A1[i];                                                      \
              Aiq = A2[i];                                                      \
              Vpi = V1[i];                                                      \
              Vqi = V2[i];                                                      \
              A1[i] = Aip * c - Aiq * s;                                        \
              A2[i] = Aiq * c + Aip * s;                                        \
              V1[i] = Vpi * c - Vqi * s;                                        \
              V2[i] = Vqi * c + Vpi * s;                                        \
            }                                                                   \
            App2 = App * c2 + Aqq * s2 - a;                                     \
            Aqq2 = App * s2 + Aqq * c2 + a;                                     \
            A1[p] = App2;                                                       \
            A2[q] = Aqq2;                                                       \
            A1[q] = A2[p] = 0.0;                                                \
          }                 /*q */                                              \
        }                   /*p */                                              \
      } while( ind );                                                           \
    }                           /* while ( Amax > ax ) */                       \
    \
    for( i = 0, k = 0; i < (_N); i++, k += (_N) + 1 )                           \
      _E[i] = _A[k];                                                            \
    /* -------- ordering -------- */                                            \
    for( i = 0; i < (_N); i++ ) {                                               \
      int m = i;                                                                \
      real Em = fabs( _E[i] );                                                  \
      for( j = i + 1; j < (_N); j++ ) {                                         \
        real Ej = fabs( _E[j] );                                                \
        m = (Em < Ej) ? j : m;                                                  \
        Em = (Em < Ej) ? Ej : Em;                                               \
      }                                                                         \
      if( m != i ) {                                                            \
        int l;                                                                  \
        real b = _E[i];                                                         \
        _E[i] = _E[m];                                                          \
        _E[m] = b;                                                              \
        for( j = 0, k = i * (_N), l = m * (_N); j < (_N); j++, k++, l++ ) {     \
          b = _V[k];                                                            \
          _V[k] = _V[l];                                                        \
          _V[l] = b;                                                            \
        }                                                                       \
      }                                                                         \
    }                                                                           \
  } while(0)

#endif // _EIGEN_H_
