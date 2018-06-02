
/* Multiplies two matrices of the form
 *
 * idx:0 idx:1     0
 * idx:2 idx:3     0
 * idx:4 idx:5     1
 *
 * and puts the output in out.
 * */
static void matrix_multiply(GDIPCONST REAL * left, GDIPCONST REAL * right, REAL * out)
{
    REAL temp[6];
    int i, odd;

    for(i = 0; i < 6; i++){
        odd = i % 2;
        temp[i] = left[i - odd] * right[odd] + left[i - odd + 1] * right[odd + 2] +
                  (i >= 4 ? right[odd + 4] : 0.0);
    }

    memcpy(out, temp, 6 * sizeof(REAL));
}

static REAL matrix_det(GDIPCONST Matrix *matrix)
{
    return matrix->matrix[0]*matrix->matrix[3] - matrix->matrix[1]*matrix->matrix[2];
}

Status  GdipCreateMatrix2(REAL m11, REAL m12, REAL m21, REAL m22,
    REAL dx, REAL dy, Matrix *matrix)
{
  REAL* mat = matrix->matrix;
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

Status  GdipCreateMatrix3(GDIPCONST RectF *rect, GDIPCONST PointF *pt, Matrix *matrix)
{
    REAL m11, m12, m21, m22, dx, dy;
    TRACE("(%p, %p, %p)\n", rect, pt, matrix);

    if(!matrix || !pt)
        return InvalidParameter;

    m11 = (pt[1].X - pt[0].X) / rect->Width;
    m21 = (pt[2].X - pt[0].X) / rect->Height;
    dx = pt[0].X - m11 * rect->X - m21 * rect->Y;
    m12 = (pt[1].Y - pt[0].Y) / rect->Width;
    m22 = (pt[2].Y - pt[0].Y) / rect->Height;
    dy = pt[0].Y - m12 * rect->X - m22 * rect->Y;

    return GdipCreateMatrix2(m11, m12, m21, m22, dx, dy, matrix);
}

Status  GdipCreateMatrix3I(GDIPCONST Rect *rect, GDIPCONST Point *pt,
    Matrix *matrix)
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

Status  GdipCloneMatrix(Matrix *matrix, Matrix *clone)
{
    TRACE("(%p, %p)\n", matrix, clone);

    if(!matrix || !clone)
        return InvalidParameter;

    *clone = *matrix;

    return Ok;
}
// Create identity matrix
Status  GdipCreateMatrix(Matrix *matrix)
{
  REAL* mat = matrix->matrix;
    TRACE("(%p)\n", matrix);

    if(!matrix)
        return InvalidParameter;

    mat[0] = 1.0;
    mat[1] = 0.0;
    mat[2] = 0.0;
    mat[3] = 1.0;
    mat[4] = 0.0;
    mat[5] = 0.0;

    return Ok;
}

Status  GdipDeleteMatrix(Matrix *matrix)
{
    TRACE("(%p)\n", matrix);

    if(!matrix)
        return InvalidParameter;

    return Ok;
}

Status  GdipGetMatrixElements(GDIPCONST Matrix *matrix,
    REAL *out)
{
    TRACE("(%p, %p)\n", matrix, out);

    if(!matrix || !out)
        return InvalidParameter;

    memcpy(out, matrix->matrix, sizeof(matrix->matrix));

    return Ok;
}

Status  GdipIsMatrixInvertible(GDIPCONST Matrix *matrix, BOOL *result)
{
    TRACE("(%p, %p)\n", matrix, result);

    if(!matrix || !result)
        return InvalidParameter;

    *result = (fabs(matrix_det(matrix)) >= 1e-5);

    return Ok;
}

Status  GdipInvertMatrix(Matrix *matrix)
{
  Matrix copy;
  REAL det;
  BOOL invertible;
  
  TRACE("(%p)\n", matrix);
  
  if(!matrix)
    return InvalidParameter;
  
  GdipIsMatrixInvertible(matrix, &invertible);
  if(!invertible)
    return InvalidParameter;
  
  det = matrix_det(matrix);
  
  copy = *matrix;
  /* store result */
  matrix->matrix[0] =   copy.matrix[3] / det;
  matrix->matrix[1] =  -copy.matrix[1] / det;
  matrix->matrix[2] =  -copy.matrix[2] / det;
  matrix->matrix[3] =   copy.matrix[0] / det;
  matrix->matrix[4] =  (copy.matrix[2]*copy.matrix[5]-copy.matrix[3]*copy.matrix[4]) / det;
  matrix->matrix[5] = -(copy.matrix[0]*copy.matrix[5]-copy.matrix[1]*copy.matrix[4]) / det;
  
  return Ok;
}

Status  GdipMultiplyMatrix(Matrix *matrix, GDIPCONST Matrix* matrix2,
    MatrixOrder order)
{
    TRACE("(%p, %p, %d)\n", matrix, matrix2, order);

    if(!matrix || !matrix2)
        return InvalidParameter;

    if(order == MatrixOrderAppend)
        matrix_multiply(matrix->matrix, matrix2->matrix, matrix->matrix);
    else if (order == MatrixOrderPrepend)
        matrix_multiply(matrix2->matrix, matrix->matrix, matrix->matrix);
    else
        return InvalidParameter;

    return Ok;
}

Status  GdipRotateMatrix(Matrix *matrix, REAL angle,
    MatrixOrder order)
{
    REAL cos_theta, sin_theta, rotate[6];

    TRACE("(%p, %.2f, %d)\n", matrix, angle, order);

    if(!matrix)
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

    if(order == MatrixOrderAppend)
        matrix_multiply(matrix->matrix, rotate, matrix->matrix);
    else if (order == MatrixOrderPrepend)
        matrix_multiply(rotate, matrix->matrix, matrix->matrix);
    else
        return InvalidParameter;

    return Ok;
}

Status  GdipScaleMatrix(Matrix *matrix, REAL scaleX, REAL scaleY,
    MatrixOrder order)
{
    REAL scale[6];

    TRACE("(%p, %.2f, %.2f, %d)\n", matrix, scaleX, scaleY, order);

    if(!matrix)
        return InvalidParameter;

    scale[0] = scaleX;
    scale[1] = 0.0;
    scale[2] = 0.0;
    scale[3] = scaleY;
    scale[4] = 0.0;
    scale[5] = 0.0;

    if(order == MatrixOrderAppend)
        matrix_multiply(matrix->matrix, scale, matrix->matrix);
    else if (order == MatrixOrderPrepend)
        matrix_multiply(scale, matrix->matrix, matrix->matrix);
    else
        return InvalidParameter;

    return Ok;
}

Status  GdipSetMatrixElements(Matrix *matrix, REAL m11, REAL m12,
    REAL m21, REAL m22, REAL dx, REAL dy)
{
    TRACE("(%p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n", matrix, m11, m12,
            m21, m22, dx, dy);

    if(!matrix)
        return InvalidParameter;

    matrix->matrix[0] = m11;
    matrix->matrix[1] = m12;
    matrix->matrix[2] = m21;
    matrix->matrix[3] = m22;
    matrix->matrix[4] = dx;
    matrix->matrix[5] = dy;

    return Ok;
}

Status  GdipShearMatrix(Matrix *matrix, REAL shearX, REAL shearY,
    MatrixOrder order)
{
    REAL shear[6];

    TRACE("(%p, %.2f, %.2f, %d)\n", matrix, shearX, shearY, order);

    if(!matrix)
        return InvalidParameter;

    /* prepare transformation matrix */
    shear[0] = 1.0;
    shear[1] = shearY;
    shear[2] = shearX;
    shear[3] = 1.0;
    shear[4] = 0.0;
    shear[5] = 0.0;

    if(order == MatrixOrderAppend)
        matrix_multiply(matrix->matrix, shear, matrix->matrix);
    else if (order == MatrixOrderPrepend)
        matrix_multiply(shear, matrix->matrix, matrix->matrix);
    else
        return InvalidParameter;

    return Ok;
}

Status  GdipTransformMatrixPoints(Matrix *matrix, PointF *pts,
                                              INT count)
{
    REAL x, y;
    INT i;

    TRACE("(%p, %p, %d)\n", matrix, pts, count);

    if(!matrix || !pts || count <= 0)
        return InvalidParameter;

    for(i = 0; i < count; i++)
    {
        x = pts[i].X;
        y = pts[i].Y;

        pts[i].X = x * matrix->matrix[0] + y * matrix->matrix[2] + matrix->matrix[4];
        pts[i].Y = x * matrix->matrix[1] + y * matrix->matrix[3] + matrix->matrix[5];
    }

    return Ok;
}

Status  GdipTransformMatrixPointsI(Matrix *matrix, Point *pts, INT count)
{
    PointF *ptsF;
    Status ret;
    INT i;

    TRACE("(%p, %p, %d)\n", matrix, pts, count);

    if(count <= 0)
        return InvalidParameter;

    ptsF = (PointF*)pmalloc(sizeof(PointF) * count);
    if(!ptsF)
        return OutOfMemory;

    for(i = 0; i < count; i++){
        ptsF[i].X = (REAL)pts[i].X;
        ptsF[i].Y = (REAL)pts[i].Y;
    }

    ret = GdipTransformMatrixPoints(matrix, ptsF, count);

    if(ret == Ok) {
        for(i = 0; i < count; i++) {
            pts[i].X = gdip_round(ptsF[i].X);
            pts[i].Y = gdip_round(ptsF[i].Y);
        }
    }
    pfree(ptsF);

    return ret;
}

Status  GdipTranslateMatrix(Matrix *matrix, REAL offsetX,
    REAL offsetY, MatrixOrder order)
{
    REAL translate[6];

    TRACE("(%p, %.2f, %.2f, %d)\n", matrix, offsetX, offsetY, order);

    if(!matrix)
        return InvalidParameter;

    translate[0] = 1.0;
    translate[1] = 0.0;
    translate[2] = 0.0;
    translate[3] = 1.0;
    translate[4] = offsetX;
    translate[5] = offsetY;

    if(order == MatrixOrderAppend)
        matrix_multiply(matrix->matrix, translate, matrix->matrix);
    else if (order == MatrixOrderPrepend)
        matrix_multiply(translate, matrix->matrix, matrix->matrix);
    else
        return InvalidParameter;

    return Ok;
}

Status  GdipVectorTransformMatrixPoints(Matrix *matrix, PointF *pts, INT count)
{
    REAL x, y;
    INT i;

    TRACE("(%p, %p, %d)\n", matrix, pts, count);

    if(!matrix || !pts || count <= 0)
        return InvalidParameter;

    for(i = 0; i < count; i++)
    {
        x = pts[i].X;
        y = pts[i].Y;

        pts[i].X = x * matrix->matrix[0] + y * matrix->matrix[2];
        pts[i].Y = x * matrix->matrix[1] + y * matrix->matrix[3];
    }

    return Ok;
}

Status  GdipVectorTransformMatrixPointsI(Matrix *matrix, Point *pts, INT count)
{
    PointF *ptsF;
    Status ret;
    INT i;

    TRACE("(%p, %p, %d)\n", matrix, pts, count);

    if(count <= 0)
        return InvalidParameter;

    ptsF = (PointF*)pmalloc(sizeof(PointF) * count);
    if(!ptsF)
        return OutOfMemory;

    for(i = 0; i < count; i++){
        ptsF[i].X = (REAL)pts[i].X;
        ptsF[i].Y = (REAL)pts[i].Y;
    }

    ret = GdipVectorTransformMatrixPoints(matrix, ptsF, count);
    /* store back */
    if(ret == Ok)
        for(i = 0; i < count; i++){
            pts[i].X = gdip_round(ptsF[i].X);
            pts[i].Y = gdip_round(ptsF[i].Y);
        }
    pfree(ptsF);

    return ret;
}

Status  GdipIsMatrixEqual(GDIPCONST Matrix *matrix, GDIPCONST Matrix *matrix2,
    BOOL *result)
{
    TRACE("(%p, %p, %p)\n", matrix, matrix2, result);

    if(!matrix || !matrix2 || !result)
        return InvalidParameter;
    /* based on single array member of Matrix */
    *result = (memcmp(matrix->matrix, matrix2->matrix, sizeof(Matrix)) == 0);

    return Ok;
}

Status  GdipIsMatrixIdentity(GDIPCONST Matrix *matrix, BOOL *result)
{
    Matrix e[1];
    Status ret;
    BOOL isIdentity;

    TRACE("(%p, %p)\n", matrix, result);

    if(!matrix || !result)
        return InvalidParameter;

    ret = GdipCreateMatrix(e);
    if(ret != Ok) return ret;

    ret = GdipIsMatrixEqual(matrix, e, &isIdentity);
    if(ret == Ok)
        *result = isIdentity;

    return ret;
}
