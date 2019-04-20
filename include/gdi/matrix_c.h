
#include "enums.h"

#define TRACE printf
#define deg2rad(deg) ((deg) * CC_PI / 180.0)
#define rad2deg(rad) ((rad) * 180.0 / CC_PI)

/* Multiplies two matrices of the form
*
* idx:0 idx:1     0
* idx:2 idx:3     0
* idx:4 idx:5     1
*
* and puts the output in out.
* */
static void matrix_multiply(const REAL * left, const REAL * right, REAL * out)
{
	REAL temp[6];
	int i, odd;

	for (i = 0; i < 6; i++) {
		odd = i % 2;
		temp[i] = left[i - odd] * right[odd] + left[i - odd + 1] * right[odd + 2] +
			(i >= 4 ? right[odd + 4] : 0.0);
	}

	memcpy(out, temp, 6 * sizeof(REAL));
}

static REAL matrix_det(const REAL *matrix)
{
	return matrix[0] * matrix[3] - matrix[1] * matrix[2];
}

CError  GdipCreateMatrix2(REAL m11, REAL m12, REAL m21, REAL m22,
	REAL dx, REAL dy, REAL *matrix)
{
	TRACE("(%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %p)\n", m11, m12, m21, m22, dx, dy, matrix);

	if (!matrix)
		return errBadParam;

	/* first row */
	matrix[0] = m11;
	matrix[1] = m12;
	/* second row */
	matrix[2] = m21;
	matrix[3] = m22;
	/* third row */
	matrix[4] = dx;
	matrix[5] = dy;

	return stdOk;
}

CError  GdipCreateMatrix3(const RectF *rect,
	const PointF *pt, REAL *matrix)
{
	REAL m11, m12, m21, m22, dx, dy;
	TRACE("(%p, %p, %p)\n", rect, pt, matrix);

	if (!matrix || !pt)
		return errBadParam;

	m11 = (pt[1].X - pt[0].X) / rect->Width;
	m21 = (pt[2].X - pt[0].X) / rect->Height;
	dx = pt[0].X - m11 * rect->X - m21 * rect->Y;
	m12 = (pt[1].Y - pt[0].Y) / rect->Width;
	m22 = (pt[2].Y - pt[0].Y) / rect->Height;
	dy = pt[0].Y - m12 * rect->X - m22 * rect->Y;

	return GdipCreateMatrix2(m11, m12, m21, m22, dx, dy, matrix);
}

CError  GdipCreateMatrix3I(const Rect *rect, const Point *pt,
	REAL *matrix)
{
	RectF rectF;
	PointF ptF[3];
	int i;

	TRACE("(%p, %p, %p)\n", rect, pt, matrix);

	rectF.X = (REAL)rect->X;
	rectF.Y = (REAL)rect->Y;
	rectF.Width = (REAL)rect->Width;
	rectF.Height = (REAL)rect->Height;

	for (i = 0; i < 3; i++) {
		ptF[i].X = (REAL)pt[i].X;
		ptF[i].Y = (REAL)pt[i].Y;
	}
	return GdipCreateMatrix3(&rectF, ptF, matrix);
}

CError  GdipCopyMatrix(const REAL *matrix, REAL *clone)
{
	TRACE("(%p, %p)\n", matrix, clone);

	if (!matrix || !clone)
		return errBadParam;

	memcpy(clone, matrix, 6*sizeof(REAL));

	return stdOk;
}

CError  GdipCreateMatrix(REAL *matrix)
{
	TRACE("(%p)\n", matrix);

	if (!matrix)
		return errBadParam;

	matrix[0] = 1.0;
	matrix[1] = 0.0;
	matrix[2] = 0.0;
	matrix[3] = 1.0;
	matrix[4] = 0.0;
	matrix[5] = 0.0;

	return stdOk;
}

CError  GdipIsMatrixInvertible(const REAL *matrix, BOOL *result)
{
	TRACE("(%p, %p)\n", matrix, result);

	if (!matrix || !result)
		return errBadParam;

	*result = (fabs(matrix_det(matrix)) >= 1e-5);

	return stdOk;
}

CError  GdipInvertMatrix(REAL *matrix)
{
	REAL copy[6];
	REAL det;
	BOOL invertible;

	TRACE("(%p)\n", matrix);

	if (!matrix)
		return errBadParam;

	GdipIsMatrixInvertible(matrix, &invertible);
	if (!invertible)
		return errBadParam;

	det = matrix_det(matrix);

	GdipCopyMatrix(matrix, copy);
	/* store result */
	matrix[0] = copy[3] / det;
	matrix[1] = -copy[1] / det;
	matrix[2] = -copy[2] / det;
	matrix[3] = copy[0] / det;
	matrix[4] = (copy[2] * copy[5] - copy[3] * copy[4]) / det;
	matrix[5] = -(copy[0] * copy[5] - copy[1] * copy[4]) / det;

	return stdOk;
}

CError  GdipMultiplyMatrix(REAL *matrix, const REAL* matrix2, MatrixOrder order)
{
	TRACE("(%p, %p, %d)\n", matrix, matrix2, order);

	if (!matrix || !matrix2)
		return errBadParam;

	if (order == MatrixOrderAppend)
		matrix_multiply(matrix, matrix2, matrix);
	else if (order == MatrixOrderPrepend)
		matrix_multiply(matrix2, matrix, matrix);
	else
		return errBadParam;

	return stdOk;
}

CError  GdipRotateMatrix(REAL *matrix, REAL angle,
	MatrixOrder order)
{
	REAL cos_theta, sin_theta, rotate[6];

	TRACE("(%p, %.2f, %d)\n", matrix, angle, order);

	if (!matrix)
		return errBadParam;

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
		return errBadParam;

	return stdOk;
}

CError  GdipScaleMatrix(REAL *matrix, REAL scaleX, REAL scaleY,
	MatrixOrder order)
{
	REAL scale[6];

	TRACE("(%p, %.2f, %.2f, %d)\n", matrix, scaleX, scaleY, order);

	if (!matrix)
		return errBadParam;

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
		return errBadParam;

	return stdOk;
}

CError  GdipSetMatrixElements(REAL *matrix, REAL m11, REAL m12,
	REAL m21, REAL m22, REAL dx, REAL dy)
{
	TRACE("(%p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n", matrix, m11, m12,
		m21, m22, dx, dy);

	if (!matrix)
		return errBadParam;

	matrix[0] = m11;
	matrix[1] = m12;
	matrix[2] = m21;
	matrix[3] = m22;
	matrix[4] = dx;
	matrix[5] = dy;

	return stdOk;
}

CError  GdipShearMatrix(REAL *matrix, REAL shearX, REAL shearY,
	MatrixOrder order)
{
	REAL shear[6];

	TRACE("(%p, %.2f, %.2f, %d)\n", matrix, shearX, shearY, order);

	if (!matrix)
		return errBadParam;

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
		return errBadParam;

	return stdOk;
}

CError  GdipTransformMatrixPoints(const REAL *matrix, PointF *pts,
	INT count)
{
	REAL x, y;
	INT i;

	TRACE("(%p, %p, %d)\n", matrix, pts, count);

	if (!matrix || !pts || count <= 0)
		return errBadParam;

	for (i = 0; i < count; i++)
	{
		x = pts[i].X;
		y = pts[i].Y;

		pts[i].X = x * matrix[0] + y * matrix[2] + matrix[4];
		pts[i].Y = x * matrix[1] + y * matrix[3] + matrix[5];
	}

	return stdOk;
}

CError  GdipTransformMatrixPointsI(const REAL *matrix, Point *pts, INT count)
{
	PointF *ptsF;
	CError ret;
	INT i;

	TRACE("(%p, %p, %d)\n", matrix, pts, count);

	if (count <= 0)
		return errBadParam;

	ptsF = (PointF*)malloc(sizeof(PointF) * count);
	if (!ptsF)
		return OutOfMemory;

	for (i = 0; i < count; i++) {
		ptsF[i].X = (REAL)pts[i].X;
		ptsF[i].Y = (REAL)pts[i].Y;
	}
#define gdip_round(x)  round(x)
	ret = GdipTransformMatrixPoints(matrix, ptsF, count);

	if (ret == stdOk)
		for (i = 0; i < count; i++) {
			pts[i].X = gdip_round(ptsF[i].X);
			pts[i].Y = gdip_round(ptsF[i].Y);
		}
	free(ptsF);

	return ret;
}

CError  GdipTranslateMatrix(REAL *matrix, REAL offsetX,
	REAL offsetY, MatrixOrder order)
{
	REAL translate[6];

	TRACE("(%p, %.2f, %.2f, %d)\n", matrix, offsetX, offsetY, order);

	if (!matrix)
		return errBadParam;

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
		return errBadParam;

	return stdOk;
}

CError  GdipVectorTransformMatrixPoints(const REAL *matrix, PointF *pts, INT count)
{
	REAL x, y;
	INT i;

	TRACE("(%p, %p, %d)\n", matrix, pts, count);

	if (!matrix || !pts || count <= 0)
		return errBadParam;

	for (i = 0; i < count; i++)
	{
		x = pts[i].X;
		y = pts[i].Y;

		pts[i].X = x * matrix[0] + y * matrix[2];
		pts[i].Y = x * matrix[1] + y * matrix[3];
	}

	return stdOk;
}

CError  GdipVectorTransformMatrixPointsI(const REAL *matrix, Point *pts, INT count)
{
	PointF *ptsF;
	CError ret;
	INT i;

	TRACE("(%p, %p, %d)\n", matrix, pts, count);

	if (count <= 0)
		return errBadParam;

	ptsF = (PointF*)malloc(sizeof(PointF) * count);
	if (!ptsF)
		return OutOfMemory;

	for (i = 0; i < count; i++) {
		ptsF[i].X = (REAL)pts[i].X;
		ptsF[i].Y = (REAL)pts[i].Y;
	}

	ret = GdipVectorTransformMatrixPoints(matrix, ptsF, count);
	/* store back */
	if (ret == stdOk)
		for (i = 0; i < count; i++) {
			pts[i].X = gdip_round(ptsF[i].X);
			pts[i].Y = gdip_round(ptsF[i].Y);
		}
	free(ptsF);

	return ret;
}

CError GdipIsMatrixEqual(const REAL *matrix, const REAL *matrix2, BOOL* result)
{
	TRACE("(%p, %p, %p)\n", matrix, matrix2, result);

	if (!matrix || !matrix2 || !result)
		return errBadParam;
	/* based on single array member of REAL */
	*result = (memcmp(matrix, matrix2, sizeof(REAL)) == 0);
	return stdOk;
}

CError GdipIsMatrixIdentity(const REAL *matrix, BOOL *result)
{
	REAL e[6];
	CError ret;
	BOOL isIdentity;

	TRACE("(%p, %p)\n", matrix, result);

	if (!matrix || !result)
		return errBadParam;

	ret = GdipCreateMatrix(e);
	if (ret != stdOk) return ret;

	ret = GdipIsMatrixEqual(matrix, e, &isIdentity);
	if (ret == stdOk)
		*result = isIdentity;

	return stdOk;
}
