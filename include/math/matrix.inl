#include <float.h>
//CvComplex_mul(c, a, b)
//#define CvComplex_add mul(c,a, b)      (c.re+=a.re*b.re - a.im*b.im, c.im+=a.re*b.im + a.im*b.re )
#define arrtype double
#define MAX_ITERS  30
#define icvGivens1( n, x, y, c, s )  { int _i; \
    arrtype* _x = (x), *_y = (y), t0, t1, t3, t4;   \
    for( _i = 0; _i < n; _i++ ) {      \
      t0 = _x[_i], t1 = _y[_i];        \
      MUL(t3, t0, c);                  \
      MUL(t4, t1, s);                  \
      ADD(_x[_i], t3, t4);             \
      MUL(t3, t0, s);                  \
      MUL(t4, t1, c);                  \
      SUB(_y[_i], t4, t3);             \
    }                                  \
  }
/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
#define icvMatrAXPY1( m, n, x, dx, a, y, dy ) { int _i, _j; \
    const arrtype* _a = a; \
    const arrtype* _x = x; \
    arrtype t0, t1, t3, t4; \
    for( _i = 0; _i < m; _i++, _x += dx, y += dy ) { \
      arrtype s = _a[_i]; \
      for( _j = 0; _j < n; _j++ ) { \
        MUL(t3, s, _x[_j]); \
        ADD(_y[_j], _y[_j], t3); \
      } \
    } \
  }
void icvGivens(int n, arrtype* x, arrtype* y, arrtype c, arrtype s)
{
  int _i;
  arrtype* _x = (x), * _y = (y), t0, t1;
  for (_i = 0; _i < n; _i++) {
    t0 = _x[_i], t1 = _y[_i];
    _x[_i] = t0 * c + t1 * s;
    _y[_i] = -t0 * s + t1 * c;
  }
}
static int icvMatrAXPY(int m, int n, const arrtype* x, int dx, const arrtype* a, arrtype* y, int dy)
{
  int i, j;
  for (i = 0; i < m; i++, x += dx, y += dy) {
    arrtype s = a[i];
    for (j = 0; j < n; j++) {
      y[j] += s * x[j];
    }
  }
  return 0;
}
static int icvMatrAXPY3(int m, int n, const arrtype* x, int l, arrtype* y, arrtype h)
{
  int i, j;
  for (i = 1; i < m; i++) {
    arrtype s = 0;
    y += l;
    for (j = 0; j < n; j++) {
      s += x[j] * y[j];
    }
    s *= h;
    y[-1] = s * x[-1];
    for (j = 0; j < n; j++) {
      y[j] += s * x[j];
    }
  }
  return 0;
}
/* accurate hypotenuse calculation */
static arrtype pythag(arrtype a, arrtype b)
{
  a = fabs(a);
  b = fabs(b);
  if (a > b) {
    b /= a;
    a *= sqrt(1. + b * b);
  }
  else if (b != 0) {
    a /= b;
    a = b * sqrt(1. + a * a);
  }
  return a;
}
static int iSVD(arrtype* a, int lda, int m, int n, arrtype* w, arrtype* uT, int lduT, int nu, arrtype* vT, int ldvT, arrtype* buffer)
{
  arrtype* e, *temp, *w1, *e1, *hv, *a1, *u0 = uT, *v0 = vT, *hv0;
  arrtype ku0 = 0, kv0 = 0, anorm = 0, scale, h;
  int i, j, k, l;
  int nm, m1, n1;
  int nv = n;
  int iters = 0;
  hv0 = (arrtype*)malloc((m + 2) * sizeof(hv0[0])) + 1;
  e = buffer;
  w1 = w;
  e1 = e + 1;
  nm = n;
  temp = buffer + nm;
  memset(w, 0, nm * sizeof(w[0]));
  memset(e, 0, nm * sizeof(e[0]));
  m1 = m;
  n1 = n;
  /* transform a to bi-diagonal form */
  for (;;) {
    int update_u, update_v;
    if (m1 == 0) {
      break;
    }
    scale = h = 0;
    update_u = uT && (m1 > (m - nu));
    hv = update_u ? uT : hv0;
    for (j = 0, a1 = a; j < m1; j++, a1 += lda) {
      arrtype t = a1[0];
      scale += fabs(hv[j] = t);
    }
    if (scale != 0) {
      arrtype f = 1. / scale, g, s = 0;
      for (j = 0; j < m1; j++) {
        arrtype t = (hv[j] *= f);
        s += t * t;
      }
      g = sqrt(s);
      f = hv[0];
      if (f >= 0) {
        g = -g;
      }
      hv[0] = f - g;
      h = 1. / (f * g - s);
      memset(temp, 0, n1 * sizeof(temp[0]));
      /* calc temp[0:n-i] = a[i:m,i:n]'*hv[0:m-i] */
      icvMatrAXPY(m1, n1 - 1, a + 1, lda, hv, temp + 1, 0);
      for (k = 1; k < n1; k++) {
        temp[k] *= h;
      }
      /* modify a: a[i:m,i:n] = a[i:m,i:n] + hv[0:m-i]*temp[0:n-i]' */
      icvMatrAXPY(m1, n1 - 1, temp + 1, 0, hv, a + 1, lda);
      *w1 = g * scale;
    }
    w1++;
    /* store -2/(hv'*hv) */
    if (update_u) {
      if (m1 == m) {
        ku0 = h;
      }
      else {
        hv[-1] = h;
      }
    }
    a++;
    n1--;
    if (vT) {
      vT += ldvT + 1;
    }
    if (n1 == 0) {
      break;
    }
    scale = h = 0;
    update_v = vT && n1 > n - nv;
    hv = update_v ? vT : hv0;
    for (j = 0; j < n1; j++) {
      arrtype t = a[j];
      scale += fabs(hv[j] = t);
    }
    if (scale != 0) {
      arrtype f = 1. / scale, g, s = 0;
      for (j = 0; j < n1; j++) {
        arrtype t = (hv[j] *= f);
        s += t * t;
      }
      g = sqrt(s);
      f = hv[0];
      if (f >= 0) {
        g = -g;
      }
      hv[0] = f - g;
      h = 1. / (f * g - s);
      hv[-1] = 0.;
      /* update a[i:m:i+1:n] = a[i:m,i+1:n] + (a[i:m,i+1:n]*hv[0:m-i])*... */
      icvMatrAXPY3(m1, n1, hv, lda, a, h);
      *e1 = g * scale;
    }
    e1++;
    /* store -2/(hv'*hv) */
    if (update_v) {
      if (n1 == n) {
        kv0 = h;
      }
      else {
        hv[-1] = h;
      }
    }
    a += lda;
    m1--;
    if (uT) {
      uT += lduT + 1;
    }
  }
  m1 -= m1 != 0;
  n1 -= n1 != 0;
  /* accumulate left transformations */
  if (uT) {
    m1 = m - m1;
    uT = u0 + m1 * lduT;
    for (i = m1; i < nu; i++, uT += lduT) {
      memset(uT + m1, 0, (m - m1) * sizeof(uT[0]));
      uT[i] = 1.;
    }
    for (i = m1 - 1; i >= 0; i--) {
      arrtype s;
      int lh = nu - i;
      l = m - i;
      hv = u0 + (lduT + 1) * i;
      h = i == 0 ? ku0 : hv[-1];
      //assert( h <= 0 );
      if (h != 0) {
        uT = hv;
        icvMatrAXPY3(lh, l - 1, hv + 1, lduT, uT + 1, h);
        s = hv[0] * h;
        for (k = 0; k < l; k++) {
          hv[k] *= s;
        }
        hv[0] += 1;
      }
      else {
        for (j = 1; j < l; j++) {
          hv[j] = 0;
        }
        for (j = 1; j < lh; j++) {
          hv[j * lduT] = 0;
        }
        hv[0] = 1;
      }
    }
    uT = u0;
  }
  /* accumulate right transformations */
  if (vT) {
    n1 = n - n1;
    vT = v0 + n1 * ldvT;
    for (i = n1; i < nv; i++, vT += ldvT) {
      memset(vT + n1, 0, (n - n1) * sizeof(vT[0]));
      vT[i] = 1.;
    }
    for (i = n1 - 1; i >= 0; i--) {
      arrtype s;
      int lh = nv - i;
      l = n - i;
      hv = v0 + (ldvT + 1) * i;
      h = i == 0 ? kv0 : hv[-1];
      //ASSERT( h <= 0. );
      if (h != 0) {
        vT = hv;
        icvMatrAXPY3(lh, l - 1, hv + 1, ldvT, vT + 1, h);
        s = hv[0] * h;
        for (k = 0; k < l; k++) {
          hv[k] *= s;
        }
        hv[0] += 1;
      }
      else {
        for (j = 1; j < l; j++) {
          hv[j] = 0;
        }
        for (j = 1; j < lh; j++) {
          hv[j * ldvT] = 0;
        }
        hv[0] = 1;
      }
    }
    vT = v0;
  }
  for (i = 0; i < nm; i++) {
    arrtype tnorm = fabs(w[i]);
    tnorm += fabs(e[i]);
    if (anorm < tnorm) {
      anorm = tnorm;
    }
  }
  anorm *= DBL_EPSILON;
  /* diagonalization of the bidiagonal form */
  for (k = nm - 1; k >= 0; k--) {
    arrtype z = 0;
    iters = 0;
    for (;;) {              /* do iterations */
      arrtype c, s, f, g, x, y;
      int flag = 0;
      /* test for splitting */
      for (l = k; l >= 0; l--) {
        if (fabs(e[l]) <= anorm) {
          flag = 1;
          break;
        }
        assert(l > 0);
        if (fabs(w[l - 1]) <= anorm) {
          break;
        }
      }
      if (!flag) {
        c = 0;
        s = 1;
        for (i = l; i <= k; i++) {
          f = s * e[i];
          e[i] *= c;
          if (anorm + fabs(f) == anorm) {
            break;
          }
          g = w[i];
          h = pythag(f, g);
          w[i] = h;
          c = g / h;
          s = -f / h;
          if (uT) {
            icvGivens(m, uT + lduT * (l - 1), uT + lduT * i, c, s);
          }
        }
      }
      z = w[k];
      if (l == k || iters++ == MAX_ITERS) {
        break;
      }
      /* shift from bottom 2x2 minor */
      x = w[l];
      y = w[k - 1];
      g = e[k - 1];
      h = e[k];
      f = 0.5 * (((g + z) / h) * ((g - z) / y) + y / h - h / y);
      g = pythag(f, 1);
      if (f < 0) {
        g = -g;
      }
      f = x - (z / x) * z + (h / x) * (y / (f + g) - h);
      /* next QR transformation */
      c = s = 1;
      for (i = l + 1; i <= k; i++) {
        g = e[i];
        y = w[i];
        h = s * g;
        g *= c;
        z = pythag(f, h);
        e[i - 1] = z;
        c = f / z;
        s = h / z;
        f = x * c + g * s;
        g = -x * s + g * c;
        h = y * s;
        y *= c;
        if (vT) {
          icvGivens(n, vT + ldvT * (i - 1), vT + ldvT * i, c, s);
        }
        z = pythag(f, h);
        w[i - 1] = z;
        /* rotation can be arbitrary if z == 0 */
        if (z != 0) {
          c = f / z;
          s = h / z;
        }
        f = c * g + s * y;
        x = -s * g + c * y;
        if (uT) {
          icvGivens(m, uT + lduT * (i - 1), uT + lduT * i, c, s);
        }
      }
      e[l] = 0;
      e[k] = f;
      w[k] = x;
    }                       /* end of iteration loop */
    if (iters > MAX_ITERS) {
      break;
    }
    if (z < 0) {
      w[k] = -z;
      if (vT) {
        for (j = 0; j < n; j++) {
          vT[j + k * ldvT] = -vT[j + k * ldvT];
        }
      }
    }
  }                           /* end of diagonalization loop */
  /* sort singular values and corresponding values */
  for (i = 0; i < nm; i++) {
    k = i;
    for (j = i + 1; j < nm; j++) {
      if (w[k] < w[j]) {
        k = j;
      }
    }
    if (k != i) {
      arrtype t;
      CC_SWAP(w[i], w[k], t);
      if (vT) {
        for (j = 0; j < n; j++) {
          CC_SWAP(vT[j + ldvT * k], vT[j + ldvT * i], t);
        }
      }
      if (uT) {
        for (j = 0; j < m; j++) {
          CC_SWAP(uT[j + lduT * k], uT[j + lduT * i], t);
        }
      }
    }
  }
  free(hv0 - 1);
  return 0;
}
#undef arrtype

