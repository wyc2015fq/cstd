
/// <summary>
///   Encapsulates a 3-by-3 general transformation matrix that represents
///   a (possibly) non-linear transform. 
/// </summary>
/// <remarks>
/// <para>
///   Linear transformations are not the only ones that can be represented by
///   matrices. Using homogeneous coordinates, both affine transformations and
///   perspective projections on R^n can be represented as linear transformations
///   on R^n+1 (that is, n+1-dimensional real projective space).</para>
/// <para>
///   The general transformation matrix has 8 degrees of freedom, as the last element is just a scale parameter.</para>  
/// </remarks>
/// 

enum {MatrixH_NN=8};
/// <summary>
///   Creates a new projective matrix.
/// </summary>
int MatrixH_init(double* this_elements)
{
  double elements[MatrixH_NN] = { 1, 0, 0, 0, 1, 0, 0, 0 };
    // Start as the identity matrix
    MEMCPY(this_elements, elements, MatrixH_NN);
    return 0;
}

/// <summary>
///   Creates a new projective matrix.
/// </summary>
 int MatrixH_set8(double* elements, double m11, double m12, double m13,
               double m21, double m22, double m23,
               double m31, double m32)
{
    elements[0] = m11; elements[1] = m12; elements[2] = m13;
    elements[3] = m21; elements[4] = m22; elements[5] = m23;
    elements[6] = m31; elements[7] = m32;
    return 0;
}

/// <summary>
///   Creates a new projective matrix.
/// </summary>
 int MatrixH_set9(double* elements, double m11, double m12, double m13,
   double m21, double m22, double m23,
   double m31, double m32, double m33)
{
   int i;
    MatrixH_set8(elements, m11, m12, m13, m21, m22, m23, m31, m32);
    for (i = 0; i < 8; i++) {
        elements[i] /= m33;
    }
    return 0;
}

/// <summary>
///   Creates a new projective matrix.
/// </summary>
 int MatrixH_setm(double* elements, const double* H)
{
   int i;
   for (i = 0; i < 8; i++) {
     elements[i] = (double)(H[i] / H[8]);
   }
   return 0;
}

/// <summary>
///   Gets the offset x
/// </summary>
 double MatrixH_OffsetX(const double* this_elements)
{
    return this_elements[2];
}

/// <summary>
///   Gets the offset y
/// </summary>
 double MatrixH_OffsetY(const double* this_elements)
{
    return this_elements[5];
}

/// <summary>
///   Gets whether this matrix is invertible.
/// </summary>
 bool MatrixH_IsInvertible(const double* elements)
{
    
    
        double det = elements[0] * (elements[4] - elements[5] * elements[7])
            - elements[1] * (elements[3] - elements[5] * elements[6])
            + elements[2] * (elements[3] * elements[7] - elements[4] * elements[6]);

        return det > 0;
    
}

/// <summary>
///   Gets whether this is an Affine transformation matrix.
/// </summary>
 bool MatrixH_IsAffine(const double* elements)
{
    return (elements[6] == 0 && elements[7] == 0);
}

/// <summary>
///   Gets whether this is the identity transformation.
 /// </summary>
 bool IsIdentity(const double* elements)
{
        return
            elements[0] == 1 && elements[1] == 0 && elements[2] == 0 &&
            elements[3] == 0 && elements[4] == 1 && elements[5] == 0 &&
            elements[6] == 0 && elements[7] == 0;
    
}

/// <summary>
///   Resets this matrix to be the identity.
 /// </summary>
 bool MatrixH_Reset(double* elements)
{
    elements[0] = 1; elements[1] = 0; elements[2] = 0;
    elements[3] = 0; elements[4] = 1; elements[5] = 0;
    elements[6] = 0; elements[7] = 0;
    return 0;
}

/// <summary>
///   Returns the inverse matrix, if this matrix is invertible.
/// </summary>
int MatrixH_Inverse(const double* this_elements, double* elements)
{
    //    m = 1 / [a(ei-fh) - b(di-fg) + c(dh-eg)]
    // 
    //                  (ei-fh)   (ch-bi)   (bf-ce)
    //  inv(A) =  m  x  (fg-di)   (ai-cg)   (cd-af)
    //                  (dh-eg)   (bg-ah)   (ae-bd)
    //

    double a = this_elements[0], b = this_elements[1], c = this_elements[2];
    double d = this_elements[3], e = this_elements[4], f = this_elements[5];
    double g = this_elements[6], h = this_elements[7];

    double m = 1.f / (a * (e - f * h) - b * (d - f * g) + c * (d * h - e * g));
    double na = m * (e - f * h);
    double nb = m * (c * h - b);
    double nc = m * (b * f - c * e);
    double nd = m * (f * g - d);
    double ne = m * (a - c * g);
    double nf = m * (c * d - a * f);
    double ng = m * (d * h - e * g);
    double nh = m * (b * g - a * h);
    double nj = m * (a * e - b * d);

    return MatrixH_set9(elements, na, nb, nc, nd, ne, nf, ng, nh, nj);
}

PointH MatrixH_TransformPointH(const double* elements, PointH p)
{
  PointH r;
  double x=p.x, y=p.y, w=p.w;
  r.x = elements[0] * x + elements[1] * y + elements[2] * w;
  r.y = elements[3] * x + elements[4] * y + elements[5] * w;
  r.w = elements[6] * x + elements[7] * y + w;
  return r;
}

/// <summary>
///   Transforms the given points using this transformation matrix.
/// </summary>
int MatrixH_TransformPointHs(const double* elements, const PointH* p, PointH* r, int n)
{
    int j;
    for (j = 0; j < n; j++)
    {
        double x=p[j].x, y=p[j].y, w=p[j].w;
        r[j].x = elements[0] * x + elements[1] * y + elements[2] * w;
        r[j].y = elements[3] * x + elements[4] * y + elements[5] * w;
        r[j].w = elements[6] * x + elements[7] * y + w;
    }

    return 0;
}

DPOINT MatrixH_TransformPoint(const double* elements, DPOINT p)
{
  DPOINT r;
  double x=p.x, y=p.y;
  double w = elements[6] * x + elements[7] * y + 1.f;
  r.x = ((elements[0] * x + elements[1] * y + elements[2]) / w);
  r.y = ((elements[3] * x + elements[4] * y + elements[5]) / w);
  return r;
}

/// <summary>
///   Transforms the given points using this transformation matrix.
/// </summary>
 int MatrixH_TransformPoints(const double* elements, const DPOINT* p, DPOINT* r, int n)
{
   int j;
    for (j = 0; j < n; j++)
    {
        double x=p[j].x, y=p[j].y;
        double w = elements[6] * x + elements[7] * y + 1.f;
        r[j].x = ((elements[0] * x + elements[1] * y + elements[2]) / w);
        r[j].y = ((elements[3] * x + elements[4] * y + elements[5]) / w);
    }

    return 0;
}

/// <summary>
///   Multiplies this matrix, returning a new matrix as result.
/// </summary>
 int MatrixH_Multiply(const double* elements0, const double* elements1, double* elements2)
{
    double na = elements0[0] * elements1[0] + elements0[1] * elements1[3] + elements0[2] * elements1[6];
    double nb = elements0[0] * elements1[1] + elements0[1] * elements1[4] + elements0[2] * elements1[7];
    double nc = elements0[0] * elements1[2] + elements0[1] * elements1[5] + elements0[2];

    double nd = elements0[3] * elements1[0] + elements0[4] * elements1[3] + elements0[5] * elements1[6];
    double ne = elements0[3] * elements1[1] + elements0[4] * elements1[4] + elements0[5] * elements1[7];
    double nf = elements0[3] * elements1[2] + elements0[4] * elements1[5] + elements0[5];

    double ng = elements0[6] * elements1[0] + elements0[7] * elements1[3] + elements1[6];
    double nh = elements0[6] * elements1[1] + elements0[7] * elements1[4] + elements1[7];
    double ni = elements0[6] * elements1[2] + elements0[7] * elements1[5] + 1.f;

    return MatrixH_set9(elements2, na, nb, nc, nd, ne, nf, ng, nh, ni);
}

/// <summary>
///   Equality
/// </summary>
 static bool MatrixH_equ(const double* a, const double* b)
{
     int i;
    for (i = 0; i < 8; i++)
        if (a[i] != b[i])
            return false;

    return true;
}

/// <summary>
///   Inequality
/// </summary>
 static bool MatrixH_neq(const double* a, const double* b)
 {
     int i;
    for (i = 0; i < 8; i++)
        if (a[i] == b[i])
            return true;

    return false;
}

