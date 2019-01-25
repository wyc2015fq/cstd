
#ifndef _STD_MATRIX23_C_H_
#define _STD_MATRIX23_C_H_

#include "algo.h"
#include "math_c.h"

#define ERR printf
#define FIXME printf
#define TENSION_CONST (0.3)

#define MAX_ARC_PTS (13)
#define MAX_DASHLEN (16) /* this is a limitation of gdi */


#if 0
typedef Status(*gdip_format_string_callback)(const WCHAR *string, INT index, INT length, const ttffont_t *f,
  const FRECT *rect, const StringFormat *format,
  INT lineno, const FRECT *bounds, INT *underlined_indexes,
  INT underlined_index_count, void *user_data);
#endif

typedef int(*get_glyph_f)(uint code, const float* matrix, void* userdata);


CC_INLINE INT gdip_round(float x)
{
  return (INT)floorf(x + 0.5);
}

CC_INLINE INT ceilr(float x)
{
  return (INT)ceilf(x);
}


/* Like atan2, but puts angle in correct quadrant if dx is 0. */
CC_INLINE float gdiplus_atan2(float dy, float dx)
{
  if ((dx == 0.0) && (dy != 0.0))
    return dy > 0.0 ? M_PI_2 : -M_PI_2;

  return atan2(dy, dx);
}

/* Multiplies two matrices of the form
*
* idx:0 idx:1     0
* idx:2 idx:3     0
* idx:4 idx:5     1
*
* and puts the output in out.
* */
static void matrix_multiply(const float * left, const float * right, float * out)
{
  float temp[6];
  int i, odd;

  for (i = 0; i < 6; i++) {
    odd = i % 2;
    temp[i] = left[i - odd] * right[odd] + left[i - odd + 1] * right[odd + 2] + (i >= 4 ? right[odd + 4] : 0.0);
  }

  memcpy(out, temp, 6 * sizeof(float));
}

static float M23Det(const float *matrix)
{
  return matrix[0] * matrix[3] - matrix[1] * matrix[2];
}

static Status M23Set2(double m11, double m12, double m21, double m22,
  double dx, double dy, float *matrix)
{
  float* mat = matrix;
  TRACE("(%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %p)\n", m11, m12, m21, m22, dx, dy, matrix);

  /* first row */
  mat[0] = m11;
  mat[1] = m12;
  /* second row */
  mat[2] = m21;
  mat[3] = m22;
  /* third row */
  mat[4] = dx;
  mat[5] = dy;

  return Ok;
}

static Status M23Set3(const FRECT *rect, const FPOINT *pt, float *matrix)
{
  float m11, m12, m21, m22, dx, dy;
  float Width = RCW(rect);
  float Height = RCH(rect);
  TRACE("(%p, %p, %p)\n", rect, pt, matrix);

  if (!matrix || !pt)
    return InvalidParameter;

  m11 = (pt[1].x - pt[0].x) / Width;
  m21 = (pt[2].x - pt[0].x) / Height;
  dx = pt[0].x - m11 * rect->l - m21 * rect->t;
  m12 = (pt[1].y - pt[0].y) / Width;
  m22 = (pt[2].y - pt[0].y) / Height;
  dy = pt[0].y - m12 * rect->l - m22 * rect->t;

  return M23Set2(m11, m12, m21, m22, dx, dy, matrix);
}

static Status M23Set3I(const IRECT *rect, const IPOINT *pt, float *matrix)
{
  FRECT rectF;
  FPOINT ptF[3];
  int i;

  TRACE("(%p, %p, %p)\n", rect, pt, matrix);

  rectF.l = (float)rect->l;
  rectF.t = (float)rect->t;
  rectF.r = (float)rect->r;
  rectF.b = (float)rect->b;

  for (i = 0; i < 3; i++) {
    ptF[i].x = (float)pt[i].x;
    ptF[i].y = (float)pt[i].y;
  }
  return M23Set3(&rectF, ptF, matrix);
}
static Status M23Copy(const float *matrix, float *clone)
{
  TRACE("(%p, %p)\n", matrix, clone);

  if (!matrix || !clone)
    return InvalidParameter;

  MEMCPY(clone, matrix, 6);
  return Ok;
}
// Create identity matrix
static Status M23Set(float *matrix)
{
  float* mat = matrix;
  TRACE("(%p)\n", matrix);

  if (!matrix)
    return InvalidParameter;

  mat[0] = 1.0;
  mat[1] = 0.0;
  mat[2] = 0.0;
  mat[3] = 1.0;
  mat[4] = 0.0;
  mat[5] = 0.0;

  return Ok;
}

static Status M23GetElements(const float *matrix, float *out)
{
  TRACE("(%p, %p)\n", matrix, out);

  if (!matrix || !out)
    return InvalidParameter;

  memcpy(out, matrix, 6 * sizeof(*matrix));

  return Ok;
}

static Status M23IsInvertible(const float *matrix, BOOL *result)
{
  TRACE("(%p, %p)\n", matrix, result);

  if (!matrix || !result)
    return InvalidParameter;

  *result = (fabs(M23Det(matrix)) >= 1e-5);

  return Ok;
}
#define M23Invert(matrix)  M23InvertCopy(matrix, matrix)
static Status M23InvertCopy(const float *matrix, float *inv) {
  float det;
  BOOL invertible;
  float _copy[6];
  const float* copy = matrix;

  TRACE("(%p)\n", matrix);

  if (!matrix)
    return InvalidParameter;

  M23IsInvertible(matrix, &invertible);
  if (!invertible)
    return InvalidParameter;

  det = M23Det(matrix);
  if (matrix == inv) {
    M23Copy(matrix, _copy);
    copy = _copy;
  }

  /* store result */
  inv[0] = copy[3] / det;
  inv[1] = -copy[1] / det;
  inv[2] = -copy[2] / det;
  inv[3] = copy[0] / det;
  inv[4] = (copy[2] * copy[5] - copy[3] * copy[4]) / det;
  inv[5] = -(copy[0] * copy[5] - copy[1] * copy[4]) / det;
  return Ok;
}

static Status M23Multiply(float *matrix, const float* matrix2,
  MatrixOrder order)
{
  TRACE("(%p, %p, %d)\n", matrix, matrix2, order);

  if (!matrix || !matrix2)
    return InvalidParameter;

  if (order == MatrixOrderAppend)
    matrix_multiply(matrix, matrix2, matrix);
  else if (order == MatrixOrderPrepend)
    matrix_multiply(matrix2, matrix, matrix);
  else
    return InvalidParameter;

  return Ok;
}

static Status M23Rotate(float *matrix, float angle, MatrixOrder order)
{
  float cos_theta, sin_theta, rotate[6];

  TRACE("(%p, %.2f, %d)\n", matrix, angle, order);

  if (!matrix)
    return InvalidParameter;

  angle = deg2rad(angle);
  cos_theta = cos(angle);
  sin_theta = sin(angle);

  rotate[0] = cos_theta;
  rotate[1] = sin_theta;
  rotate[2] = -sin_theta;
  rotate[3] = cos_theta;
  rotate[4] = 0.0;
  rotate[5] = 0.0;

  if (order == MatrixOrderAppend)
    matrix_multiply(matrix, rotate, matrix);
  else if (order == MatrixOrderPrepend)
    matrix_multiply(rotate, matrix, matrix);
  else
    return InvalidParameter;

  return Ok;
}

static Status M23Scale(float *matrix, float scaleX, float scaleY,
  MatrixOrder order)
{
  float scale[6];

  TRACE("(%p, %.2f, %.2f, %d)\n", matrix, scaleX, scaleY, order);

  if (!matrix)
    return InvalidParameter;

  scale[0] = scaleX;
  scale[1] = 0.0;
  scale[2] = 0.0;
  scale[3] = scaleY;
  scale[4] = 0.0;
  scale[5] = 0.0;

  if (order == MatrixOrderAppend)
    matrix_multiply(matrix, scale, matrix);
  else if (order == MatrixOrderPrepend)
    matrix_multiply(scale, matrix, matrix);
  else
    return InvalidParameter;

  return Ok;
}

static Status M23SetElements(float *matrix, float m11, float m12,
  float m21, float m22, float dx, float dy)
{
  TRACE("(%p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n", matrix, m11, m12,
    m21, m22, dx, dy);

  if (!matrix)
    return InvalidParameter;

  matrix[0] = m11;
  matrix[1] = m12;
  matrix[2] = m21;
  matrix[3] = m22;
  matrix[4] = dx;
  matrix[5] = dy;

  return Ok;
}

static Status M23Shear(float *matrix, float shearX, float shearY, MatrixOrder order)
{
  float shear[6];

  TRACE("(%p, %.2f, %.2f, %d)\n", matrix, shearX, shearY, order);

  if (!matrix)
    return InvalidParameter;

  /* prepare transformation matrix */
  shear[0] = 1.0;
  shear[1] = shearY;
  shear[2] = shearX;
  shear[3] = 1.0;
  shear[4] = 0.0;
  shear[5] = 0.0;

  if (order == MatrixOrderAppend)
    matrix_multiply(matrix, shear, matrix);
  else if (order == MatrixOrderPrepend)
    matrix_multiply(shear, matrix, matrix);
  else
    return InvalidParameter;

  return Ok;
}

static Status M23TransformPoints(const float *matrix, FPOINT *pts, INT count)
{
  float x, y;
  INT i;

  TRACE("(%p, %p, %d)\n", matrix, pts, count);

  if (!matrix || !pts || count <= 0)
    return InvalidParameter;

  for (i = 0; i < count; i++)
  {
    x = pts[i].x;
    y = pts[i].y;

    pts[i].x = x * matrix[0] + y * matrix[2] + matrix[4];
    pts[i].y = x * matrix[1] + y * matrix[3] + matrix[5];
  }

  return Ok;
}

static Status M23TransformPointsI(const float *matrix, IPOINT *pts, INT count)
{
  FPOINT *ptsF;
  Status ret;
  INT i;

  TRACE("(%p, %p, %d)\n", matrix, pts, count);

  if (count <= 0)
    return InvalidParameter;

  ptsF = (FPOINT*)malloc(sizeof(FPOINT) * count);
  if (!ptsF)
    return OutOfMemory;

  for (i = 0; i < count; i++) {
    ptsF[i].x = (float)pts[i].x;
    ptsF[i].y = (float)pts[i].y;
  }

  ret = M23TransformPoints(matrix, ptsF, count);

  if (ret == Ok) {
    for (i = 0; i < count; i++) {
      pts[i].x = gdip_round(ptsF[i].x);
      pts[i].y = gdip_round(ptsF[i].y);
    }
  }
  free(ptsF);

  return ret;
}

static Status M23Translate(float *matrix, float offsetX, float offsetY, MatrixOrder order)
{
  float translate[6];

  TRACE("(%p, %.2f, %.2f, %d)\n", matrix, offsetX, offsetY, order);

  if (!matrix)
    return InvalidParameter;

  translate[0] = 1.0;
  translate[1] = 0.0;
  translate[2] = 0.0;
  translate[3] = 1.0;
  translate[4] = offsetX;
  translate[5] = offsetY;

  if (order == MatrixOrderAppend)
    matrix_multiply(matrix, translate, matrix);
  else if (order == MatrixOrderPrepend)
    matrix_multiply(translate, matrix, matrix);
  else
    return InvalidParameter;

  return Ok;
}

static Status M23VectorTransformPoints(float *matrix, FPOINT *pts, INT count)
{
  float x, y;
  INT i;

  TRACE("(%p, %p, %d)\n", matrix, pts, count);

  if (!matrix || !pts || count <= 0)
    return InvalidParameter;

  for (i = 0; i < count; i++)
  {
    x = pts[i].x;
    y = pts[i].y;

    pts[i].x = x * matrix[0] + y * matrix[2];
    pts[i].y = x * matrix[1] + y * matrix[3];
  }

  return Ok;
}

static Status M23VectorTransformPointsI(float *matrix, IPOINT *pts, INT count)
{
  FPOINT *ptsF;
  Status ret;
  INT i;

  TRACE("(%p, %p, %d)\n", matrix, pts, count);

  if (count <= 0)
    return InvalidParameter;

  ptsF = (FPOINT*)malloc(sizeof(FPOINT) * count);
  if (!ptsF)
    return OutOfMemory;

  for (i = 0; i < count; i++) {
    ptsF[i].x = (float)pts[i].x;
    ptsF[i].y = (float)pts[i].y;
  }

  ret = M23VectorTransformPoints(matrix, ptsF, count);
  /* store back */
  if (ret == Ok)
    for (i = 0; i < count; i++) {
      pts[i].x = gdip_round(ptsF[i].x);
      pts[i].y = gdip_round(ptsF[i].y);
    }
  free(ptsF);

  return ret;
}

static Status M23IsEqual(const float *matrix, const float *matrix2,
  BOOL *result)
{
  TRACE("(%p, %p, %p)\n", matrix, matrix2, result);

  if (!matrix || !matrix2 || !result)
    return InvalidParameter;
  /* based on single array member of float */
  *result = (memcmp(matrix, matrix2, sizeof(float)) == 0);

  return Ok;
}

static Status M23IsIdentity(const float *matrix, BOOL *result)
{
  float e[1];
  Status ret;
  BOOL isIdentity;

  TRACE("(%p, %p)\n", matrix, result);

  if (!matrix || !result)
    return InvalidParameter;

  ret = M23Set(e);
  if (ret != Ok) return ret;

  ret = M23IsEqual(matrix, e, &isIdentity);
  if (ret == Ok)
    *result = isIdentity;

  return ret;
}

#endif // _STD_MATRIX23_C_H_
