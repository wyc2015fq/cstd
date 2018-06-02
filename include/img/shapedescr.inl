
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "macro.h"

#define CC_NO_ERR (0)
#define CC_BADFACTOR_ERR 1
#define CC_OK 0
#define CC_NOTDEFINED_ERR 0
#define CC_NULLPTR_ERR 0
#define CC_OUTOFMEM_ERR 0
#define CC_BADSIZE_ERR 0

/* calculates length of a curve (e.g. contour perimeter) */
real
cvArcLength(int count, FPOINT* array, int is_closed)
{
  real perimeter = 0;

  int i, j = 0;
  const int N = 16;

  for (i = 0; i < count; i++) {
    real dx, dy;

    {
      FPOINT* pt = (FPOINT*) array + i;
      FPOINT* prev_pt = (FPOINT*) array + (i - 1) % count;

      dx = pt->x - prev_pt->x;
      dy = pt->y - prev_pt->y;
    }

    perimeter += SQRT(dx * dx + dy * dy);
  }

  return perimeter;
}

int
icvIntersectLines(double x1, double dx1, double y1, double dy1,
    double x2, double dx2, double y2, double dy2, double* t2);

float icvDistanceL2_32f(FPOINT pt1, FPOINT pt2)
{
  float dx = pt2.x - pt1.x;
  float dy = pt2.y - pt1.y;

  return SQRT(dx * dx + dy * dy);
}

int
icvFindCircle(FPOINT pt0, FPOINT pt1,
    FPOINT pt2, FPOINT* center, real* radius)
{
  real x1 = (pt0.x + pt1.x) * 0.5f;
  real dy1 = pt0.x - pt1.x;
  real x2 = (pt1.x + pt2.x) * 0.5f;
  real dy2 = pt1.x - pt2.x;
  real y1 = (pt0.y + pt1.y) * 0.5f;
  real dx1 = pt1.y - pt0.y;
  real y2 = (pt1.y + pt2.y) * 0.5f;
  real dx2 = pt2.y - pt1.y;
  double t = 0;

  int result = CC_OK;

  if (icvIntersectLines(x1, dx1, y1, dy1, x2, dx2, y2, dy2, &t) >= 0) {
    center->x = (real)(x2 + dx2 * t);
    center->y = (real)(y2 + dy2 * t);
    *radius = (real) icvDistanceL2_32f(*center, pt0);
  }
  else {
    center->x = center->y = 0.f;
    radius = 0;
    result = CC_NOTDEFINED_ERR;
  }

  return result;
}


real icvIsPtInCircle(FPOINT pt, FPOINT center, real radius)
{
  real dx = pt.x - center.x;
  real dy = pt.y - center.y;
  return (real) radius * radius - dx * dx - dy * dy;
}

FPOINT fPOINT(double x, double y);

static int
icvFindEnslosingCicle4pts_32f(FPOINT* pts, FPOINT* _center, real* _radius)
{
  int shuffles[ 4 ][ 4 ] = { {0, 1, 2, 3}, {0, 1, 3, 2}, {2, 3, 0, 1}, {2, 3, 1, 0} };

  int idxs[ 4 ] = { 0, 1, 2, 3 };
  int i, j, k = 1, mi = 0;
  real max_dist = 0;
  FPOINT center;
  FPOINT min_center;
  real radius, min_radius = FLT_MAX;
  FPOINT res_pts[ 4 ];

  center = min_center = pts[ 0 ];
  radius = 1.f;

  for (i = 0; i < 4; i++)
    for (j = i + 1; j < 4; j++) {
      real dist = icvDistanceL2_32f(pts[ i ], pts[ j ]);

      if (max_dist < dist) {
        max_dist = dist;
        idxs[ 0 ] = i;
        idxs[ 1 ] = j;
      }
    }

  if (max_dist == 0) {
    goto function_exit;
  }

  k = 2;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < k; j++)
      if (i == idxs[ j ]) {
        break;
      }

    if (j == k) {
      idxs[ k++ ] = i;
    }
  }

  center = fPOINT((pts[ idxs[ 0 ] ].x + pts[ idxs[ 1 ] ].x) * 0.5f,
      (pts[ idxs[ 0 ] ].y + pts[ idxs[ 1 ] ].y) * 0.5f);
  radius = (real)(icvDistanceL2_32f(pts[ idxs[ 0 ] ], center) * 1.03);

  if (radius < 1.f) {
    radius = 1.f;
  }

  if (icvIsPtInCircle(pts[ idxs[ 2 ] ], center, radius) >= 0 &&
      icvIsPtInCircle(pts[ idxs[ 3 ] ], center, radius) >= 0) {
    k = 2; //rand()%2+2;
  }
  else {
    mi = -1;

    for (i = 0; i < 4; i++) {
      if (icvFindCircle(pts[ shuffles[ i ][ 0 ] ], pts[ shuffles[ i ][ 1 ] ],
          pts[ shuffles[ i ][ 2 ] ], &center, &radius) >= 0) {
        radius *= 1.03f;

        if (radius < 2.f) {
          radius = 2.f;
        }

        if (icvIsPtInCircle(pts[ shuffles[ i ][ 3 ] ], center, radius) >= 0 &&
            min_radius > radius) {
          min_radius = radius;
          min_center = center;
          mi = i;
        }
      }
    }

    assert(mi >= 0);

    if (mi < 0) {
      mi = 0;
    }

    k = 3;
    center = min_center;
    radius = min_radius;

    for (i = 0; i < 4; i++) {
      idxs[ i ] = shuffles[ mi ][ i ];
    }
  }

function_exit:

  *_center = center;
  *_radius = radius;

  /* reorder output points */
  for (i = 0; i < 4; i++) {
    res_pts[ i ] = pts[ idxs[ i ] ];
  }

  for (i = 0; i < 4; i++) {
    pts[ i ] = res_pts[ i ];
    assert(icvIsPtInCircle(pts[ i ], center, radius) >= 0);
  }

  return k;
}


int
cvMinEnclosingCircle(int count, const FPOINT* array, FPOINT* _center, real* _radius)
{
  const int max_iters = 100;
  const real eps = FLT_EPSILON * 2;
  FPOINT center = { 0, 0 };
  real radius = 0;
  int result = 0;
  int i, k;
  FPOINT pts[ 8 ];

  if (_center) {
    _center->x = _center->y = 0.f;
  }

  if (_radius) {
    * _radius = 0;
  }

  {
    FPOINT* pt_left, *pt_right, *pt_top, *pt_bottom;
    FPOINT pt;
    pt_left = pt_right = pt_top = pt_bottom = (FPOINT*)(array);

    for (i = 1; i < count; i++) {
      FPOINT* pt_ptr = (FPOINT*) array + i;
      pt = *pt_ptr;

      if (pt.x < pt_left->x) {
        pt_left = pt_ptr;
      }

      if (pt.x > pt_right->x) {
        pt_right = pt_ptr;
      }

      if (pt.y < pt_top->y) {
        pt_top = pt_ptr;
      }

      if (pt.y > pt_bottom->y) {
        pt_bottom = pt_ptr;
      }
    }

    pts[ 0 ] = *pt_left;
    pts[ 1 ] = *pt_right;
    pts[ 2 ] = *pt_top;
    pts[ 3 ] = *pt_bottom;
  }

  for (k = 0; k < max_iters; k++) {
    real min_delta = 0, delta;
    FPOINT ptfl;

    icvFindEnslosingCicle4pts_32f(pts, &center, &radius);

    for (i = 0; i < count; i++) {
      ptfl = *(FPOINT*)(array + i);

      delta = icvIsPtInCircle(ptfl, center, radius);

      if (delta < min_delta) {
        min_delta = delta;
        pts[ 3 ] = ptfl;
      }
    }

    result = min_delta >= 0;

    if (result) {
      break;
    }
  }

  if (!result) {
    radius = 0.f;

    for (i = 0; i < count; i++) {
      FPOINT ptfl;
      real t, dx, dy;

      ptfl = *(FPOINT*)(array + i);

      dx = center.x - ptfl.x;
      dy = center.y - ptfl.y;
      t = dx * dx + dy * dy;
      radius = MAX(radius, t);
    }

    radius = (real)(sqrt(radius) * (1 + eps));
    result = 1;
  }



  *_center = center;
  *_radius = radius;

  return result;
}


/* area of a whole sequence */
int
icvContourArea(int lpt, const FPOINT* array, real* area)
{
  if (lpt) {
    real a00 = 0, xi_1, yi_1;

    {
      xi_1 = ((FPOINT*)(array)) ->x;
      yi_1 = ((FPOINT*)(array)) ->y;
    }

    while (lpt-- > 0) {
      real dxy, xi, yi;

      xi = ((FPOINT*)(array + lpt)) ->x;
      yi = ((FPOINT*)(array + lpt)) ->y;

      dxy = xi_1 * yi - xi * yi_1;
      a00 += dxy;
      xi_1 = xi;
      yi_1 = yi;
    }

    *area = a00 * 0.5f;
  }
  else {
    *area = 0.f;
  }

  return CC_OK;
}


/****************************************************************************************\

 copy data from one buffer to other buffer

\****************************************************************************************/

int
icvMemCopy(real** buf1, real** buf2, real** buf3, int* b_max)
{
  int bb;

  if (*buf1 == NULL && *buf2 == NULL || *buf3 == NULL) {
    return CC_NULLPTR_ERR;
  }

  bb = *b_max;

  if (*buf2 == NULL) {
    *b_max = 2 * (*b_max);
    *buf2 = (real*) cvAlloc((*b_max) * sizeof(real));

    if (*buf2 == NULL) {
      return CC_OUTOFMEM_ERR;
    }

    memcpy(*buf2, *buf3, bb * sizeof(real));

    *buf3 = *buf2;
    pfree(*buf1);
    *buf1 = NULL;
  }
  else {
    *b_max = 2 * (*b_max);
    *buf1 = (real*) cvAlloc((*b_max) * sizeof(real));

    if (*buf1 == NULL) {
      return CC_OUTOFMEM_ERR;
    }

    memcpy(*buf1, *buf3, bb * sizeof(real));

    *buf3 = *buf1;
    pfree(*buf2);
    *buf2 = NULL;
  }

  return CC_OK;
}


/* area of a contour sector */
int icvContourSecArea(int total, const FPOINT* contour, real* area)
{
  FPOINT pt;                 /*  pointer to points   */
  FPOINT pt_s, pt_e;         /*  first and last points  */

  int p_max = 2, p_ind;
  int lpt = total, flag, i;
  real a00;                 /* unnormalized moments m00    */
  real xi, yi, xi_1, yi_1, x0, y0, dxy, sk, sk1, t;
  real x_s, y_s, nx, ny, dx, dy, du, dv;
  real eps = 1.e-5f;
  real* p_are1, *p_are2, *p_are;

  assert(contour != NULL);

  // lpt = cvSliceLength( slice, contour );
  /*if( n2 >= n1 )
      lpt = n2 - n1 + 1;
  else
      lpt = contour->total - n1 + n2 + 1;*/

  if (lpt > 2) {
    a00 = x0 = y0 = xi_1 = yi_1 = 0;
    sk1 = 0;
    flag = 0;
    dxy = 0;
    p_are1 = (real*) cvAlloc(p_max * sizeof(real));

    if (p_are1 == NULL) {
      return CC_OUTOFMEM_ERR;
    }

    p_are = p_are1;
    p_are2 = NULL;

    //cvStartReadSeq( contour, &reader, 0 );
    //cvSetSeqReaderPos( &reader, slice.start_index );
    //CC_READ_SEQ_ELEM( pt_s, reader );
    pt_s = contour[ 0 ];
    p_ind = 0;
    pt_e = contour[ lpt - 1 ];
    //cvSetSeqReaderPos( &reader, slice.end_index );
    //CC_READ_SEQ_ELEM( pt_e, reader );

    /*    normal coefficients    */
    nx = pt_s.y - pt_e.y;
    ny = pt_e.x - pt_s.x;
    //cvSetSeqReaderPos( &reader, slice.start_index );

    while (lpt-- > 0) {
      pt = contour[ lpt ];
      //CC_READ_SEQ_ELEM( pt, reader );

      if (flag == 0) {
        xi_1 = (real) pt.x;
        yi_1 = (real) pt.y;
        x0 = xi_1;
        y0 = yi_1;
        sk1 = 0;
        flag = 1;
      }
      else {
        xi = (real) pt.x;
        yi = (real) pt.y;

        /****************   edges intersection examination   **************************/
        sk = nx * (xi - pt_s.x) + ny * (yi - pt_s.y);

        if (FABS(sk) < eps && lpt > 0 || sk * sk1 < -eps) {
          if (FABS(sk) < eps) {
            dxy = xi_1 * yi - xi * yi_1;
            a00 = a00 + dxy;
            dxy = xi * y0 - x0 * yi;
            a00 = a00 + dxy;

            if (p_ind >= p_max) {
              icvMemCopy(&p_are1, &p_are2, &p_are, &p_max);
            }

            p_are[ p_ind ] = a00 / 2.f;
            p_ind++;
            a00 = 0;
            sk1 = 0;
            x0 = xi;
            y0 = yi;
            dxy = 0;
          }
          else {
            /*  define intersection point    */
            dv = yi - yi_1;
            du = xi - xi_1;
            dx = ny;
            dy = -nx;

            if (FABS(du) > eps)
              t = ((yi_1 - pt_s.y) * du + dv * (pt_s.x - xi_1)) /
                  (du * dy - dx * dv);
            else {
              t = (xi_1 - pt_s.x) / dx;
            }

            if (t > eps && t < 1 - eps) {
              x_s = pt_s.x + t * dx;
              y_s = pt_s.y + t * dy;
              dxy = xi_1 * y_s - x_s * yi_1;
              a00 += dxy;
              dxy = x_s * y0 - x0 * y_s;
              a00 += dxy;

              if (p_ind >= p_max) {
                icvMemCopy(&p_are1, &p_are2, &p_are, &p_max);
              }

              p_are[ p_ind ] = a00 / 2.f;
              p_ind++;

              a00 = 0;
              sk1 = 0;
              x0 = x_s;
              y0 = y_s;
              dxy = x_s * yi - xi * y_s;
            }
          }
        }
        else {
          dxy = xi_1 * yi - xi * yi_1;
        }

        a00 += dxy;
        xi_1 = xi;
        yi_1 = yi;
        sk1 = sk;

      }
    }

    xi = x0;
    yi = y0;
    dxy = xi_1 * yi - xi * yi_1;

    a00 += dxy;

    if (p_ind >= p_max) {
      icvMemCopy(&p_are1, &p_are2, &p_are, &p_max);
    }

    p_are[ p_ind ] = a00 / 2.f;
    p_ind++;

    /*     common area calculation    */
    *area = 0;

    for (i = 0; i < p_ind; i++) {
      (*area) += FABS(p_are[ i ]);
    }

    if (p_are1 != NULL) {
      pfree(p_are1);
    }
    else if (p_are2 != NULL) {
      pfree(p_are2);
    }

    return CC_OK;
  }
  else {
    return CC_BADSIZE_ERR;
  }
}

#if 0
/* for now this function works bad with singular cases
   You can see in the code, that when some troubles with
   matrices or some variables occur -
   box filled with zero values is returned.
   However in general function works fine.
*/
void
icvFitEllipse_F(CvSeq* points, CvBox2D* box)
{
  real S[ 36 ], C[ 36 ], T[ 36 ];

  int i, j;
  real eigenvalues[ 6 ], eigenvectors[ 36 ];
  real a, b, c, d, e, f;
  real x0, y0, idet, scale, offx = 0, offy = 0;

  int n = points->total;
  int is_real = CC_SEQ_ELTYPE(points) == CC_32FC2;

  CvMat _S = cvMat(6, 6, CC_64F, S), _C = cvMat(6, 6, CC_64F, C), _T = cvMat(6, 6, CC_64F, T);
  CvMat _EIGVECS = cvMat(6, 6, CC_64F, eigenvectors), _EIGVALS = cvMat(6, 1, CC_64F, eigenvalues);

  /* create matrix D of  input points */
  CC_CALL(D = cvCreateMat(n, 6, CC_64F));

  /* shift all points to zero */
  for (i = 0; i < n; i++) {
    {
      offx += ((FPOINT*) reader.ptr) ->x;
      offy += ((FPOINT*) reader.ptr) ->y;
    }
    CC_NEXT_SEQ_ELEM(points->elem_size, reader);
  }

  offx /= n;
  offy /= n;

  // fill matrix rows as (x*x, x*y, y*y, x, y, 1 )
  for (i = 0; i < n; i++) {
    real x, y;
    real* Dptr = D->data.db + i * 6;

    {
      x = ((FPOINT*) reader.ptr) ->x - offx;
      y = ((FPOINT*) reader.ptr) ->y - offy;
    }
    CC_NEXT_SEQ_ELEM(points->elem_size, reader);

    Dptr[ 0 ] = x * x;
    Dptr[ 1 ] = x * y;
    Dptr[ 2 ] = y * y;
    Dptr[ 3 ] = x;
    Dptr[ 4 ] = y;
    Dptr[ 5 ] = 1.;
  }

  // S = D^t*D
  cvMulTransposed(D, &_S, 1);
  cvSVD(&_S, &_EIGVALS, &_EIGVECS, 0, CC_SVD_MODIFY_A + CC_SVD_U_T);

  for (i = 0; i < 6; i++) {
    real a = eigenvalues[ i ];
    a = a < DBL_EPSILON ? 0 : 1. / sqrt(sqrt(a));

    for (j = 0; j < 6; j++) {
      eigenvectors[ i * 6 + j ] *= a;
    }
  }

  // C = Q^-1 = transp(INVEIGV) * INVEIGV
  cvMulTransposed(&_EIGVECS, &_C, 1);

  cvZero(&_S);
  S[ 2 ] = 2.;
  S[ 7 ] = -1.;
  S[ 12 ] = 2.;

  // S = Q^-1*S*Q^-1
  cvMatMul(&_C, &_S, &_T);
  cvMatMul(&_T, &_C, &_S);

  // and find its eigenvalues and vectors too
  //cvSVD( &_S, &_EIGVALS, &_EIGVECS, 0, CC_SVD_MODIFY_A + CC_SVD_U_T );
  cvEigenVV(&_S, &_EIGVECS, &_EIGVALS, 0);

  for (i = 0; i < 3; i++)
    if (eigenvalues[ i ] > 0) {
      break;
    }

  if (i >= 3 /*eigenvalues[0] < DBL_EPSILON*/) {
    box->center.x = box->center.y =
        box->size.width = box->size.height =
            box->angle = 0.f;
    EXIT;
  }

  // now find truthful eigenvector
  _EIGVECS = cvMat(6, 1, CC_64F, eigenvectors + 6 * i);
  _T = cvMat(6, 1, CC_64F, T);
  // Q^-1*eigenvecs[0]
  cvMatMul(&_C, &_EIGVECS, &_T);

  // extract vector components
  a = T[ 0 ];
  b = T[ 1 ];
  c = T[ 2 ];
  d = T[ 3 ];
  e = T[ 4 ];
  f = T[ 5 ];

  ///////////////// extract ellipse axes from above values ////////////////

  /*
     1) find center of ellipse
     it satisfy equation
     | a     b/2 | *  | x0 | +  | d/2 | = |0 |
     | b/2    c  |    | y0 |    | e/2 |   |0 |

   */
  idet = a * c - b * b * 0.25;
  idet = idet > DBL_EPSILON ? 1. / idet : 0;

  // we must normalize (a b c d e f ) to fit (4ac-b^2=1)
  scale = sqrt(0.25 * idet);

  if (scale < DBL_EPSILON) {
    box->center.x = (real) offx;
    box->center.y = (real) offy;
    box->size.width = box->size.height = box->angle = 0.f;
    EXIT;
  }

  a *= scale;
  b *= scale;
  c *= scale;
  d *= scale;
  e *= scale;
  f *= scale;

  x0 = (-d * c + e * b * 0.5) * 2.;
  y0 = (-a * e + d * b * 0.5) * 2.;

  // recover center
  box->center.x = (real)(x0 + offx);
  box->center.y = (real)(y0 + offy);

  // offset ellipse to (x0,y0)
  // new f == F(x0,y0)
  f += a * x0 * x0 + b * x0 * y0 + c * y0 * y0 + d * x0 + e * y0;

  if (FABS(f) < DBL_EPSILON) {
    box->size.width = box->size.height = box->angle = 0.f;
    EXIT;
  }

  scale = -1. / f;
  // normalize to f = 1
  a *= scale;
  b *= scale;
  c *= scale;

  // extract axis of ellipse
  // one more eigenvalue operation
  S[ 0 ] = a;
  S[ 1 ] = S[ 2 ] = b * 0.5;
  S[ 3 ] = c;

  _S = cvMat(2, 2, CC_64F, S);
  _EIGVECS = cvMat(2, 2, CC_64F, eigenvectors);
  _EIGVALS = cvMat(1, 2, CC_64F, eigenvalues);
  cvSVD(&_S, &_EIGVALS, &_EIGVECS, 0, CC_SVD_MODIFY_A + CC_SVD_U_T);

  // exteract axis length from eigenvectors
  box->size.width = (real)(2. / sqrt(eigenvalues[ 0 ]));
  box->size.height = (real)(2. / sqrt(eigenvalues[ 1 ]));

  // calc angle
  box->angle = (real)(180 - atan2(eigenvectors[ 2 ], eigenvectors[ 3 ]) * 180 / CC_PI);

  cvReleaseMat(&D);
}


CvBox2D
cvFitEllipse2(const CvArr* array)
{
  CvBox2D box;
  real* Ad = 0, *bd = 0;

  CC_FUNCNAME("cvFitEllipse2");

  memset(&box, 0, sizeof(box));



  CvContour contour_header;
  CvSeq* ptseq = 0;
  CvSeqBlock block;
  int n;

  if (CC_IS_SEQ(array)) {
    ptseq = (CvSeq*) array;

    if (!CC_IS_SEQ_POINT_SET(ptseq)) {
      CC_ERROR(CC_StsBadArg, "Unsupported sequence type");
    }
  }
  else {
    CC_CALL(ptseq = cvPointSeqFromMat(
        CC_SEQ_KIND_GENERIC, array, &contour_header, &block));
  }

  n = ptseq->total;

  if (n < 5) {
    CC_ERROR(CC_StsBadSize, "Number of points should be >= 6");
  }

#if 1

  icvFitEllipse_F(ptseq, &box);
#else
  /*
  * New fitellipse algorithm, contributed by Dr. Daniel Weiss
  */
  {
    real gfp[ 5 ], rp[ 5 ], t;
    CvMat A, b, x;
    const real min_eps = 1e-6;
    int i, is_real;
    CvSeqReader reader;

    CC_CALL(Ad = (real*) cvAlloc(n * 5 * sizeof(Ad[ 0 ])));
    CC_CALL(bd = (real*) cvAlloc(n * sizeof(bd[ 0 ])));

    // first fit for parameters A - E
    A = cvMat(n, 5, CC_64F, Ad);
    b = cvMat(n, 1, CC_64F, bd);
    x = cvMat(5, 1, CC_64F, gfp);

    cvStartReadSeq(ptseq, &reader);
    is_real = CC_SEQ_ELTYPE(ptseq) == CC_32FC2;

    for (i = 0; i < n; i++) {
      FPOINT p;

      if (is_real) {
        p = *(FPOINT*)(reader.ptr);
      }
      else {
        p.x = (real)((int*) reader.ptr) [ 0 ];
        p.y = (real)((int*) reader.ptr) [ 1 ];
      }

      CC_NEXT_SEQ_ELEM(sizeof(p), reader);

      bd[ i ] = 10000.0; // 1.0?
      Ad[ i * 5 ] = -(real) p.x * p.x;   // A - C signs inverted as proposed by APP
      Ad[ i * 5 + 1 ] = -(real) p.y * p.y;
      Ad[ i * 5 + 2 ] = -(real) p.x * p.y;
      Ad[ i * 5 + 3 ] = p.x;
      Ad[ i * 5 + 4 ] = p.y;
    }

    cvSolve(&A, &b, &x, CC_SVD);

    // now use general-form parameters A - E to find the ellipse center:
    // differentiate general form wrt x/y to get two equations for cx and cy
    A = cvMat(2, 2, CC_64F, Ad);
    b = cvMat(2, 1, CC_64F, bd);
    x = cvMat(2, 1, CC_64F, rp);
    Ad[ 0 ] = 2 * gfp[ 0 ];
    Ad[ 1 ] = Ad[ 2 ] = gfp[ 2 ];
    Ad[ 3 ] = 2 * gfp[ 1 ];
    bd[ 0 ] = gfp[ 3 ];
    bd[ 1 ] = gfp[ 4 ];
    cvSolve(&A, &b, &x, CC_SVD);

    // re-fit for parameters A - C with those center coordinates
    A = cvMat(n, 3, CC_64F, Ad);
    b = cvMat(n, 1, CC_64F, bd);
    x = cvMat(3, 1, CC_64F, gfp);

    for (i = 0; i < n; i++) {
      FPOINT p;

      if (is_real) {
        p = *(FPOINT*)(reader.ptr);
      }
      else {
        p.x = (real)((int*) reader.ptr) [ 0 ];
        p.y = (real)((int*) reader.ptr) [ 1 ];
      }

      CC_NEXT_SEQ_ELEM(sizeof(p), reader);
      bd[ i ] = 1.0;
      Ad[ i * 3 ] = (p.x - rp[ 0 ]) * (p.x - rp[ 0 ]);
      Ad[ i * 3 + 1 ] = (p.y - rp[ 1 ]) * (p.y - rp[ 1 ]);
      Ad[ i * 3 + 2 ] = (p.x - rp[ 0 ]) * (p.y - rp[ 1 ]);
    }

    cvSolve(&A, &b, &x, CC_SVD);

    // store angle and radii
    rp[ 4 ] = -0.5 * atan2(gfp[ 2 ], gfp[ 1 ] - gfp[ 0 ]);   // convert from APP angle usage
    t = sin(-2.0 * rp[ 4 ]);

    if (FABS(t) > FABS(gfp[ 2 ]) * min_eps) {
      t = gfp[ 2 ] / t;
    }
    else {
      t = gfp[ 1 ] - gfp[ 0 ];
    }

    rp[ 2 ] = FABS(gfp[ 0 ] + gfp[ 1 ] - t);

    if (rp[ 2 ] > min_eps) {
      rp[ 2 ] = sqrt(2.0 / rp[ 2 ]);
    }

    rp[ 3 ] = FABS(gfp[ 0 ] + gfp[ 1 ] + t);

    if (rp[ 3 ] > min_eps) {
      rp[ 3 ] = sqrt(2.0 / rp[ 3 ]);
    }

    box.center.x = (real) rp[ 0 ];
    box.center.y = (real) rp[ 1 ];
    box.size.width = (real)(rp[ 2 ] * 2);
    box.size.height = (real)(rp[ 3 ] * 2);

    if (box.size.width > box.size.height) {
      real tmp;
      CC_SWAP(box.size.width, box.size.height, tmp);
      box.angle = (real)(90 + rp[ 4 ] * 180 / CC_PI);
    }

    if (box.angle < -180) {
      box.angle += 360;
    }

    if (box.angle > 360) {
      box.angle -= 360;
    }
  }
#endif


  cvFree(&Ad);
  cvFree(&bd);

  return box;
}


/* Calculates bounding rectagnle of a point set or retrieves already calculated */
CvRect
cvBoundingRect(int tatal, unsigned char* ptr, int step, int update)
{
  CvRect rect = { 0, 0, 0, 0 };
  int xmin = 0, ymin = 0, xmax = -1, ymax = -1, i, j, k;
  int calculate = update;

  if (0) {
    CvSize size = cvGetMatSize(mat);
    xmin = size.width;
    ymin = -1;

    for (i = 0; i < size.height; i++) {
      unsigned char* _ptr = ptr + i * step;
      //unsigned char* ptr = (unsigned char*)cvAlignPtr(_ptr, 4);
      int have_nz = 0, k_min, offset = (int)(0);
      j = 0;
      offset = MIN(offset, size.width);

      for (; j < offset; j++)
        if (_ptr[ j ]) {
          have_nz = 1;
          break;
        }

      if (j < offset) {
        if (j < xmin) {
          xmin = j;
        }

        if (j > xmax) {
          xmax = j;
        }
      }

      if (offset < size.width) {
        xmin -= offset;
        xmax -= offset;
        size.width -= offset;
        j = 0;

        for (; j <= xmin - 4; j += 4)
          if (*((int*)(ptr + j))) {
            break;
          }

        for (; j < xmin; j++)
          if (ptr[ j ]) {
            xmin = j;

            if (j > xmax) {
              xmax = j;
            }

            have_nz = 1;
            break;
          }

        k_min = MAX(j - 1, xmax);
        k = size.width - 1;

        for (; k > k_min && (k & 3) != 3; k--)
          if (ptr[ k ]) {
            break;
          }

        if (k > k_min && (k & 3) == 3) {
          for (; k > k_min + 3; k -= 4)
            if (*((int*)(ptr + k - 3))) {
              break;
            }
        }

        for (; k > k_min; k--)
          if (ptr[ k ]) {
            xmax = k;
            have_nz = 1;
            break;
          }

        if (!have_nz) {
          j &= ~3;

          for (; j <= k - 3; j += 4)
            if (*((int*)(ptr + j))) {
              break;
            }

          for (; j <= k; j++)
            if (ptr[ j ]) {
              have_nz = 1;
              break;
            }
        }

        xmin += offset;
        xmax += offset;
        size.width += offset;
      }

      if (have_nz) {
        if (ymin < 0) {
          ymin = i;
        }

        ymax = i;
      }
    }

    if (xmin >= size.width) {
      xmin = ymin = 0;
    }
  }
  else if (ptseq->total) {
    {
      IPOINT pt;
      suf32_t v;
      /* init values */
      CC_READ_SEQ_ELEM(pt, reader);
      xmin = xmax = CC_TOGGLE_FLT(pt.x);
      ymin = ymax = CC_TOGGLE_FLT(pt.y);

      for (i = 1; i < ptseq->total; i++) {
        CC_READ_SEQ_ELEM(pt, reader);
        pt.x = CC_TOGGLE_FLT(pt.x);
        pt.y = CC_TOGGLE_FLT(pt.y);

        if (xmin > pt.x) {
          xmin = pt.x;
        }

        if (xmax < pt.x) {
          xmax = pt.x;
        }

        if (ymin > pt.y) {
          ymin = pt.y;
        }

        if (ymax < pt.y) {
          ymax = pt.y;
        }
      }

      v.i = CC_TOGGLE_FLT(xmin);
      xmin = cvFloor(v.f);
      v.i = CC_TOGGLE_FLT(ymin);
      ymin = cvFloor(v.f);
      /* because right and bottom sides of
         the bounding rectangle are not inclusive
         (note +1 in width and height calculation below),
         cvFloor is used here instead of cvCeil */
      v.i = CC_TOGGLE_FLT(xmax);
      xmax = cvFloor(v.f);
      v.i = CC_TOGGLE_FLT(ymax);
      ymax = cvFloor(v.f);
    }
  }

  rect.x = xmin;
  rect.y = ymin;
  rect.width = xmax - xmin + 1;
  rect.height = ymax - ymin + 1;

  if (update) {
    ((CvContour*) ptseq) ->rect = rect;
  }



  return rect;
}


#endif
