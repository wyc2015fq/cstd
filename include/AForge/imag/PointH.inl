
/// <summary>
///   Represents an ordered pair of real x- and y-coordinates and scalar w that defines
///   a point in a two-dimensional plane using homogeneous coordinates.
/// </summary>
/// 
/// <remarks>
/// <para>
///   In mathematics, homogeneous coordinates are a system of coordinates used in
///   projective geometry much as Cartesian coordinates are used in Euclidean geometry.</para>
/// <para>
///   They have the advantage that the coordinates of a point, even those at infinity,
///   can be represented using finite coordinates. Often formulas involving homogeneous
///   coordinates are simpler and more symmetric than their Cartesian counterparts.</para>
/// <para>
///   Homogeneous coordinates have a range of applications, including computer graphics,
///   where they allow affine transformations and, in general, projective transformations
///   to be easily represented by a matrix.</para>
///   
/// <para>
///   References: 
///   <list type="bullet">
///     <item><description>
///       http://alumnus.caltech.edu/~woody/docs/3dmatrix.html</description></item>
///     <item><description>
///       http://simply3d.wordpress.com/2009/05/29/homogeneous-coordinates/</description></item>
///   </list></para>
/// </remarks>
/// 
typedef struct PointH
{
  double x, y, w;
}PointH;

/// <summary>
///   Returns the empty point.
/// </summary>
static const PointH Empty = { 0, 0, 1};

/// <summary>
///   Creates a new point.
/// </summary>
PointH make_PointH(double x, double y, double w)
{
  PointH p={0};
    p.x = x,            p.y = y,            p.w = w;
    return p;
}

/// <summary>
///   Creates a new point.
/// </summary>
PointH make_PointH1(double x, double y)
{
  PointH p={0};
  p.x = x,            p.y = y,            p.w = 1;
  return p;
}

/// <summary>
///   Transforms a point using a projection matrix.
/// </summary>
PointH PointH_Transform(PointH p, const double* matrix)
{
  double x=p.x, y=p.y, w=p.w;
    p.x = matrix[0*3+0] * x + matrix[0*3+1] * y + matrix[0*3+2] * w;
    p.y = matrix[1*3+0] * x + matrix[1*3+1] * y + matrix[1*3+2] * w;
    p.w = matrix[2*3+0] * x + matrix[2*3+1] * y + matrix[2*3+2] * w;
    return p;
}

/// <summary>
///   Normalizes the point to have unit scale.
/// </summary>
PointH PointH_Normalize(PointH p)
{
  double w=p.w;
  p.x /= w, p.y /= w, p.w = 1;
  return p;
}

/// <summary>
///   Gets whether this point is normalized (w = 1).
/// </summary>
bool PointH_IsNormalized(PointH p)
{
    return p.w == 1.f;
}

/// <summary>
///   Gets whether this point is at infinity (w = 0).
/// </summary>
bool PointH_IsAtInfinity(PointH p)
{
    return p.w == 0.f;
}

/// <summary>
///   Gets whether this point is at the origin.
/// </summary>
bool PointH_IsEmpty(PointH p)
{
    return p.x == 0 && p.y == 0;
}

/// <summary>
///   Multiplication by scalar.
/// </summary>
static PointH PointH_mul(PointH a, double b)
{
    return make_PointH(b * a.x, b * a.y, b * a.w);
}

/// <summary>
///   Subtraction.
/// </summary>
static PointH PointH_sub(PointH a, PointH b)
{
    return make_PointH(a.x - b.x, a.y - b.y, a.w - b.w);
}

/// <summary>
///   Addition.
/// </summary>
static PointH PointH_add(PointH a, PointH b)
{
    return make_PointH(a.x + b.x, a.y + b.y, a.w + b.w);
}

/// <summary>
///   Equality
/// </summary>
static bool PointH_equ(PointH a, PointH b)
{
    return (a.x / a.w == b.x / b.w && a.y / a.w == b.y / b.w);
}

/// <summary>
///   Inequality
/// </summary>
static bool PointH_neq(PointH a, PointH b)
{
    return (a.x / a.w != b.x / b.w || a.y / a.w != b.y / b.w);
}

/// <summary>
///   PointF Conversion
/// </summary>
static FPOINT PointH_toPointF(PointH a)
{
    return fPOINT((double)(a.x / a.w), (double)(a.y / a.w));
}

/// <summary>
///   Converts to a Integer point by computing the ceiling of the point coordinates. 
/// </summary>
static IPOINT PointH_Ceiling(PointH point)
{
    return iPOINT( (int)ceil(point.x / point.w), (int)ceil(point.y / point.w));
}

/// <summary>
///   Converts to a Integer point by rounding the point coordinates. 
/// </summary>
static IPOINT PointH_Round(PointH point)
{
    return iPOINT( (int)round(point.x / point.w), (int)round(point.y / point.w));
}

int truncate(double x) {
  return (int)floor(x);
}

/// <summary>
///   Converts to a Integer point by truncating the point coordinates. 
/// </summary>
static IPOINT PointH_Truncate(PointH point)
{
    return iPOINT( (int)truncate(point.x / point.w), (int)truncate(point.y / point.w));
}


