
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "xtypes.h"
#include "eigen.h"

#define CC_NO_ERR (0)
#define CC_BADFACTOR_ERR 1
#define CC_OK 0

/* Defines for Distance Transform */
#define CC_DIST_USER    -1  /* User defined distance */
#define CC_DIST_L1      1   /* distance = |x1-x2| + |y1-y2| */
#define CC_DIST_L2      2   /* the simple euclidean distance */
#define CC_DIST_C       3   /* distance = max(|x1-x2|,|y1-y2|) */
#define CC_DIST_L12     4   /* L1-L2 metric: distance = 2(SQRT(1+x*x/2) - 1)) */
#define CC_DIST_FAIR    5   /* distance = c^2(|x|/c-log(1+|x|/c)), c = 1.3998 */
#define CC_DIST_WELSCH  6   /* distance = c^2/2(1-exp(-(x/c)^2)), c = 2.9846 */
#define CC_DIST_HUBER   7   /* distance = |x|<c ? x^2/2 : c(|x|-c/2), c=1.345 */

int icvFitLine2D_wods(FPOINT* points, int _count, real* weights, real* line)
{
  double x = 0, y = 0, x2 = 0, y2 = 0, xy = 0, w = 0;
  double dx2, dy2, dxy;
  int i;
  int count = _count;
  real t;

  /* Calculating the average of x and y... */

  if (weights == 0) {
    for (i = 0; i < count; i += 1) {
      x += points[ i ].x;
      y += points[ i ].y;
      x2 += points[ i ].x * points[ i ].x;
      y2 += points[ i ].y * points[ i ].y;
      xy += points[ i ].x * points[ i ].y;
    }

    w = (real) count;
  }
  else {
    for (i = 0; i < count; i += 1) {
      x += weights[ i ] * points[ i ].x;
      y += weights[ i ] * points[ i ].y;
      x2 += weights[ i ] * points[ i ].x * points[ i ].x;
      y2 += weights[ i ] * points[ i ].y * points[ i ].y;
      xy += weights[ i ] * points[ i ].x * points[ i ].y;
      w += weights[ i ];
    }
  }

  x /= w;
  y /= w;
  x2 /= w;
  y2 /= w;
  xy /= w;

  dx2 = x2 - x * x;
  dy2 = y2 - y * y;
  dxy = xy - x * y;

  t = (real) atan2(2 * dxy, dx2 - dy2) / 2;
  line[ 0 ] = (real) cos(t);
  line[ 1 ] = (real) sin(t);

  line[ 2 ] = (real) x;
  line[ 3 ] = (real) y;

  return CC_NO_ERR;
}

int icvFitLine3D_wods(CvPoint3D32f* points, int count, real* weights, real* line)
{
  int i;
  real w0 = 0;
  real x0 = 0, y0 = 0, z0 = 0;
  real x2 = 0, y2 = 0, z2 = 0, xy = 0, yz = 0, xz = 0;
  real dx2, dy2, dz2, dxy, dxz, dyz;
  real* v;
  real n;
  real det[ 9 ], evc[ 9 ], evl[ 3 ];

  memset(evl, 0, 3 * sizeof(evl[ 0 ]));
  memset(evc, 0, 9 * sizeof(evl[ 0 ]));

  if (weights) {
    for (i = 0; i < count; i++) {
      real x = points[ i ].x;
      real y = points[ i ].y;
      real z = points[ i ].z;
      real w = weights[ i ];


      x2 += x * x * w;
      xy += x * y * w;
      xz += x * z * w;
      y2 += y * y * w;
      yz += y * z * w;
      z2 += z * z * w;
      x0 += x * w;
      y0 += y * w;
      z0 += z * w;
      w0 += w;
    }
  }
  else {
    for (i = 0; i < count; i++) {
      real x = points[ i ].x;
      real y = points[ i ].y;
      real z = points[ i ].z;

      x2 += x * x;
      xy += x * y;
      xz += x * z;
      y2 += y * y;
      yz += y * z;
      z2 += z * z;
      x0 += x;
      y0 += y;
      z0 += z;
    }

    w0 = (real) count;
  }

  x2 /= w0;
  xy /= w0;
  xz /= w0;
  y2 /= w0;
  yz /= w0;
  z2 /= w0;

  x0 /= w0;
  y0 /= w0;
  z0 /= w0;

  dx2 = x2 - x0 * x0;
  dxy = xy - x0 * y0;
  dxz = xz - x0 * z0;
  dy2 = y2 - y0 * y0;
  dyz = yz - y0 * z0;
  dz2 = z2 - z0 * z0;

  det[ 0 ] = dz2 + dy2;
  det[ 1 ] = -dxy;
  det[ 2 ] = -dxz;
  det[ 3 ] = det[ 1 ];
  det[ 4 ] = dx2 + dz2;
  det[ 5 ] = -dyz;
  det[ 6 ] = det[ 2 ];
  det[ 7 ] = det[ 5 ];
  det[ 8 ] = dy2 + dx2;

  /* Searching for a eigenvector of det corresponding to the minimal eigenvalue */
#if 1

  {
    //CvMat _det = cvMat( 3, 3, CC_32F, det );
    //CvMat _evc = cvMat( 3, 3, CC_32F, evc );
    //CvMat _evl = cvMat( 3, 1, CC_32F, evl );
    JACOBI_EIGENS(3, det, evc, evl, real);
    //cvEigenVV( &_det, &_evc, &_evl, 0 );
    i = evl[ 0 ] < evl[ 1 ] ? (evl[ 0 ] < evl[ 2 ] ? 0 : 2) : (evl[ 1 ] < evl[ 2 ] ? 1 : 2);
  }
#else
  {
    CvMat _det = cvMat(3, 3, CC_32F, det);
    CvMat _evc = cvMat(3, 3, CC_32F, evc);
    CvMat _evl = cvMat(1, 3, CC_32F, evl);

    cvSVD(&_det, &_evl, &_evc, 0, CC_SVD_MODIFY_A + CC_SVD_U_T);
  }
  i = 2;
#endif

  v = &evc[ i * 3 ];
  n = (real) SQRT((double) v[ 0 ] * v[ 0 ] + (double) v[ 1 ] * v[ 1 ] + (double) v[ 2 ] * v[ 2 ]);
  n = (real) MAX(n, EPS);
  line[ 0 ] = v[ 0 ] / n;
  line[ 1 ] = v[ 1 ] / n;
  line[ 2 ] = v[ 2 ] / n;
  line[ 3 ] = x0;
  line[ 4 ] = y0;
  line[ 5 ] = z0;

  return CC_NO_ERR;
}

double
icvCalcDist2D(FPOINT* points, int count, real* _line, real* dist)
{
  int j;
  real px = _line[ 2 ], py = _line[ 3 ];
  real nx = _line[ 1 ], ny = -_line[ 0 ];
  double sum_dist = 0.;

  for (j = 0; j < count; j++) {
    real x, y;

    x = points[ j ].x - px;
    y = points[ j ].y - py;

    dist[ j ] = (real) FABS(nx * x + ny * y);
    sum_dist += dist[ j ];
  }

  return sum_dist;
}

double
icvCalcDist3D(CvPoint3D32f* points, int count, real* _line, real* dist)
{
  int j;
  real px = _line[ 3 ], py = _line[ 4 ], pz = _line[ 5 ];
  real vx = _line[ 0 ], vy = _line[ 1 ], vz = _line[ 2 ];
  double sum_dist = 0.;

  for (j = 0; j < count; j++) {
    real x, y, z;
    double p1, p2, p3;

    x = points[ j ].x - px;
    y = points[ j ].y - py;
    z = points[ j ].z - pz;

    p1 = vy * z - vz * y;
    p2 = vz * x - vx * z;
    p3 = vx * y - vy * x;

    dist[ j ] = (real) SQRT(p1 * p1 + p2 * p2 + p3 * p3);
    sum_dist += dist[ j ];
  }

  return sum_dist;
}

void
icvWeightL1(real* d, int count, real* w)
{
  int i;

  for (i = 0; i < count; i++) {
    double t = FABS((double) d[ i ]);
    w[ i ] = (real)(1. / MAX(t, EPS));
  }
}

void
icvWeightL12(real* d, int count, real* w)
{
  int i;

  for (i = 0; i < count; i++) {
    w[ i ] = 1.0f / (real) SQRT(1 + (double)(d[ i ] * d[ i ] * 0.5));
  }
}


void
icvWeightHuber(real* d, int count, real* w, real _c)
{
  int i;
  const real c = _c <= 0 ? 1.345f : _c;

  for (i = 0; i < count; i++) {
    if (d[ i ] < c) {
      w[ i ] = 1.0f;
    }
    else {
      w[ i ] = c / d[ i ];
    }
  }
}


void
icvWeightFair(real* d, int count, real* w, real _c)
{
  int i;
  const real c = _c == 0 ? 1 / 1.3998f : 1 / _c;

  for (i = 0; i < count; i++) {
    w[ i ] = 1 / (1 + d[ i ] * c);
  }
}

void
icvWeightWelsch(real* d, int count, real* w, real _c)
{
  int i;
  const real c = _c == 0 ? 1 / 2.9846f : 1 / _c;

  for (i = 0; i < count; i++) {
    w[ i ] = (real) exp(-d[ i ] * d[ i ] * c * c);
  }
}


/* Takes an array of 2D points, type of distance (including user-defined
distance specified by callbacks, fills the array of four reals with line
parameters A, B, C, D, where (A, B) is the normalized direction vector,
(C, D) is the point that belongs to the line. */

int icvFitLine2D(FPOINT* points, int count, int dist,
    real _param, real reps, real aeps, real* line)
{
  void (*calc_weights)(real*, int, real*) = 0;
  void (*calc_weights_param)(real*, int, real*, real) = 0;
  real* w;                   /* weights */
  real* r;                   /* square distances */
  int i, j;
  real _line[ 6 ], _lineprev[ 6 ];
  int first = 1;
  real rdelta = reps != 0 ? reps : 1.0f;
  real adelta = aeps != 0 ? aeps : 0.01f;

  memset(line, 0, 4 * sizeof(line[ 0 ]));

  switch (dist) {
  case CC_DIST_L2:
    return icvFitLine2D_wods(points, count, 0, line);

  case CC_DIST_L1:
    calc_weights = icvWeightL1;
    break;

  case CC_DIST_L12:
    calc_weights = icvWeightL12;
    break;

  case CC_DIST_FAIR:
    calc_weights_param = icvWeightFair;
    break;

  case CC_DIST_WELSCH:
    calc_weights_param = icvWeightWelsch;
    break;

  case CC_DIST_HUBER:
    calc_weights_param = icvWeightHuber;
    break;

    /*case CC_DIST_USER:
        calc_weights = (void ( * )(real *, int, real *)) _PFP.fp;
        break;*/

  default:
    return CC_BADFACTOR_ERR;
  }

  w = (real*) cvAlloc(count * sizeof(real));
  r = (real*) cvAlloc(count * sizeof(real));

  for (i = 0; i < count; i++) {
    w[ i ] = 1.0f;
  }

  icvFitLine2D_wods(points, count, 0, _line);

  for (i = 0; i < 100; i++) {
    double sum_w = 0;

    if (first) {
      first = 0;
    }
    else {
      double t = _line[ 0 ] * _lineprev[ 0 ] + _line[ 1 ] * _lineprev[ 1 ];
      t = MAX(t, -1.);
      t = MIN(t, 1.);

      if (FABS(acos(t)) < adelta) {
        real x, y, d;

        x = (real) FABS(_line[ 2 ] - _lineprev[ 2 ]);
        y = (real) FABS(_line[ 3 ] - _lineprev[ 3 ]);

        d = x > y ? x : y;

        if (d < rdelta) {
          goto _exit_;
        }
      }
    }

    /* calculate distances */
    if (icvCalcDist2D(points, count, _line, r) < FLT_EPSILON * count) {
      break;
    }

    /* calculate weights */
    if (calc_weights) {
      calc_weights(r, count, w);
    }
    else if (calc_weights_param) {
      calc_weights_param(r, count, w, _param);
    }
    else {
      goto _exit_;
    }

    for (j = 0; j < count; j++) {
      sum_w += w[ j ];
    }

    if (FABS(sum_w) > FLT_EPSILON) {
      sum_w = 1. / sum_w;

      for (j = 0; j < count; j++) {
        w[ j ] = (real)(w[ j ] * sum_w);
      }
    }
    else {
      for (j = 0; j < count; j++) {
        w[ j ] = 1.f;
      }
    }

    /* save the line parameters */
    memcpy(_lineprev, _line, 4 * sizeof(real));

    /* Run again... */
    icvFitLine2D_wods(points, count, w, _line);
  }

_exit_:
  memcpy(line, _line, 4 * sizeof(line[ 0 ]));
  pfree(w);
  pfree(r);
  return CC_OK;
}


/* Takes an array of 3D points, type of distance (including user-defined
distance specified by callbacks, fills the array of four reals with line
parameters A, B, C, D, E, F, where (A, B, C) is the normalized direction vector,
(D, E, F) is the point that belongs to the line. */

int
icvFitLine3D(CvPoint3D32f* points, int count, int dist,
    real _param, real reps, real aeps, real* line)
{
  void (*calc_weights)(real*, int, real*) = 0;
  void (*calc_weights_param)(real*, int, real*, real) = 0;
  real* w;                   /* weights */
  real* r;                   /* square distances */
  int i, j;
  real _line[ 6 ], _lineprev[ 6 ];
  int first = 1;
  real rdelta = reps != 0 ? reps : 1.0f;
  real adelta = aeps != 0 ? aeps : 0.01f;

  memset(line, 0, 6 * sizeof(line[ 0 ]));

  switch (dist) {
  case CC_DIST_L2:
    return icvFitLine3D_wods(points, count, 0, line);

  case CC_DIST_L1:
    calc_weights = icvWeightL1;
    break;

  case CC_DIST_L12:
    calc_weights = icvWeightL12;
    break;

  case CC_DIST_FAIR:
    calc_weights_param = icvWeightFair;
    break;

  case CC_DIST_WELSCH:
    calc_weights_param = icvWeightWelsch;
    break;

  case CC_DIST_HUBER:
    calc_weights_param = icvWeightHuber;
    break;

    /*case CC_DIST_USER:
        _PFP.p = param;
        calc_weights = (void ( * )(real *, int, real *)) _PFP.fp;
        break;*/

  default:
    return CC_BADFACTOR_ERR;
  }

  w = (real*) cvAlloc(count * sizeof(real));
  r = (real*) cvAlloc(count * sizeof(real));

  for (i = 0; i < count; i++) {
    w[ i ] = 1.0f;
  }

  icvFitLine3D_wods(points, count, 0, _line);

  for (i = 0; i < 100; i++) {
    double sum_w = 0;

    if (first) {
      first = 0;
    }
    else {
      double t = _line[ 0 ] * _lineprev[ 0 ] + _line[ 1 ] * _lineprev[ 1 ] + _line[ 2 ] * _lineprev[ 2 ];
      t = MAX(t, -1.);
      t = MIN(t, 1.);

      if (FABS(acos(t)) < adelta) {
        real x, y, z, ax, ay, az, dx, dy, dz, d;

        x = _line[ 3 ] - _lineprev[ 3 ];
        y = _line[ 4 ] - _lineprev[ 4 ];
        z = _line[ 5 ] - _lineprev[ 5 ];
        ax = _line[ 0 ] - _lineprev[ 0 ];
        ay = _line[ 1 ] - _lineprev[ 1 ];
        az = _line[ 2 ] - _lineprev[ 2 ];
        dx = (real) FABS(y * az - z * ay);
        dy = (real) FABS(z * ax - x * az);
        dz = (real) FABS(x * ay - y * ax);

        d = dx > dy ? (dx > dz ? dx : dz) : (dy > dz ? dy : dz);

        if (d < rdelta) {
          goto _exit_;
        }
      }
    }

    /* calculate distances */
    if (icvCalcDist3D(points, count, _line, r) < FLT_EPSILON * count) {
      break;
    }

    /* calculate weights */
    if (calc_weights) {
      calc_weights(r, count, w);
    }
    else if (calc_weights_param) {
      calc_weights_param(r, count, w, _param);
    }
    else {
      goto _exit_;
    }

    for (j = 0; j < count; j++) {
      sum_w += w[ j ];
    }

    if (FABS(sum_w) > FLT_EPSILON) {
      sum_w = 1. / sum_w;

      for (j = 0; j < count; j++) {
        w[ j ] = (real)(w[ j ] * sum_w);
      }
    }
    else {
      for (j = 0; j < count; j++) {
        w[ j ] = 1.f;
      }
    }

    /* save the line parameters */
    memcpy(_lineprev, _line, 6 * sizeof(real));

    /* Run again... */
    icvFitLine3D_wods(points, count, w, _line);
  }

_exit_:
  // Return...
  memcpy(line, _line, 6 * sizeof(line[ 0 ]));
  pfree(w);
  pfree(r);
  return CC_OK;
}

void cvFitLine(const real* points, int total, int _cn, int dist, double param,
    double reps, double aeps, real* line)
{
  if (dist == CC_DIST_USER) {
    CC_ERROR(CC_StsBadArg, "User-defined distance is not allowed");
  }

  if (_cn == 2) {
    icvFitLine2D((FPOINT*) points, total,
        dist, (real) param, (real) reps, (real) aeps, line);
  }
  else {
    icvFitLine3D((CvPoint3D32f*) points, total,
        dist, (real) param, (real) reps, (real) aeps, line);
  }

}
