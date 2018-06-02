
#include <float.h>

#include "geo.h"
#define T_ABS fabs
#define DEFAULT(x)

#define WYKOBI_DOUBLE_PRECISION

#ifdef WYKOBI_SINGLE_PRECISION
typedef float Float;
static const Float Infinity = FLT_MAX;
#endif

#ifdef WYKOBI_DOUBLE_PRECISION
typedef double Float;
static const Float Infinity = DBL_MAX;
#endif

#ifdef WYKOBI_EXTENDED_PRECISION
typedef long double Float;
static const Float Infinity = DBL_MAX;
#endif

static const Float InfinityT = DBL_MAX;

/************[ Epsilon constants ]*************/
static const Float Epsilon_High = 1.0E-16;
static const Float Epsilon_Medium = 1.0E-10;
static const Float Epsilon_Low = 1.0E-07;
static const Float Epsilon = 1.0E-10;
static const double fuzzy = 1.0E-07;

/********[ Random resolution constants ]********/
#define RANDOM_RESOLUTION_INT 1000000000
static const double RANDOM_RESOLUTION_FLT = RANDOM_RESOLUTION_INT * 1.0;

/********[ ]********/
//#define PI ( 3.141592653589793238462643383279500)
#define PI2 ( 6.283185307179586476925286766559000)
#define PIDiv180 ( 0.017453292519943295769236907684886)
#define _180DivPI (57.295779513082320876798154814105000)

CC_INLINE Float sqr(Float val)
{
  return val * val;
}

CC_INLINE double approx_sin(double angle)
{
  double result, asqr, final_sign = (1.0);

  if ((angle <= (180.0)) && (angle > 90.0)) {
    angle = (180.0) - angle;
    final_sign = (1.0);
  }
  else if ((angle <= (270.0)) && (angle > (180.0))) {
    angle = angle - (180.0);
    final_sign = (-1.0);
  }
  else if ((angle <= (360.0)) && (angle > (270.0))) {
    angle = (360.0) - angle;
    final_sign = (-1.0);
  }

  angle *= (PI / 180.0);
  asqr = angle * angle;
  result = (-2.39e-08);
  result *= asqr;
  result += (2.7526e-06);
  result *= asqr;
  result -= (1.98409e-04);
  result *= asqr;
  result += (8.3333315e-03);
  result *= asqr;
  result -= (1.666666664e-01);
  result *= asqr;
  result += (1.0);
  result *= angle;
  return result * final_sign;
}

CC_INLINE double approx_cos(double angle)
{
  double result, asqr, final_sign = (1.0);

  if ((angle <= (180.0)) && (angle > 90.0)) {
    angle = (180.0) - angle;
    final_sign = (-1.0);
  }
  else if ((angle <= (270.0)) && (angle > (180.0))) {
    angle = angle - (180.0);
    final_sign = (-1.0);
  }
  else if ((angle <= (360.0)) && (angle > (270.0))) {
    angle = (360.0) - angle;
    final_sign = (1.0);
  }

  angle *= (PI / 180.0);
  asqr = angle * angle;
  result = (-2.605e-07);
  result *= asqr;
  result += (2.47609e-05);
  result *= asqr;
  result -= (1.3888397e-03);
  result *= asqr;
  result += (4.16666418e-02);
  result *= asqr;
  result -= (4.999999963e-01);
  result *= asqr;
  result += (1.0);
  return result * final_sign;
}

CC_INLINE Float approx_tan(Float angle)
{
  Float result, asqr, final_sign = (1.0);

  if ((angle <= (180.0)) && (angle > 90.0)) {
    angle = (180.0) - angle;
    final_sign = (-1.0);
  }
  else if ((angle <= (270.0)) && (angle > (180.0))) {
    angle = angle - (180.0);
    final_sign = (1.0);
  }
  else if ((angle <= (360.0)) && (angle > (270.0))) {
    angle = (360.0) - angle;
    final_sign = (-1.0);
  }

  angle *= (PI / 180.0);
  asqr = angle * angle;
  result = (9.5168091e-03);
  result *= asqr;
  result += (2.900525e-03);
  result *= asqr;
  result += (2.45650893e-02);
  result *= asqr;
  result += (5.33740603e-02);
  result *= asqr;
  result += (1.333923995e-01);
  result *= asqr;
  result += (3.333314036e-01);
  result *= asqr;
  result += (1.0);
  result *= angle;
  return result * final_sign;
}

CC_INLINE Float clamp(Float value, Float low_end, Float high_end)
{
  if (value < low_end) {
    return low_end;
  }

  if (value > high_end) {
    return high_end;
  }

  return value;
}

/************[ Trigonometry Tables ]************/

enum { trigonometry_TableSize = 360 };
static double _sin_table[trigonometry_TableSize];
static double _cos_table[trigonometry_TableSize];
static double _tan_table[trigonometry_TableSize];

int trigonometry_tables_init()
{
  static int trigonometry_tables_inited = 0;

  if (!trigonometry_tables_inited) {
    int i;
    trigonometry_tables_inited = 1;

    for (i = 0; i < trigonometry_TableSize; ++i) {
      _sin_table[i] = sin((1.0 * i) * PIDiv180);
      _cos_table[i] = cos((1.0 * i) * PIDiv180);
      _tan_table[i] = tan((1.0 * i) * PIDiv180);
    }
  }

  return 0;
}

double tables_sin(int angle)
{
  trigonometry_tables_init();
  return _sin_table[angle];
}
double tables_cos(int angle)
{
  trigonometry_tables_init();
  return _cos_table[angle];
}
double tables_tan(int angle)
{
  trigonometry_tables_init();
  return _tan_table[angle];
}

CC_INLINE double epsilon_double()
{
  return (double)(Epsilon_Medium);
}
CC_INLINE float epsilon_float()
{
  return (float)(Epsilon_Low);
}

#define T double

static const char VERSION_INFORMATION[] = "Wykobi Version 0.0.4";
static const char AUTHOR_INFORMATION[] = "Arash Partow";
static const char EPOCH_VERSION[] = "21744C5A:44A4153E:EB32F784";

/************[ Geometric Entity ]*************/

enum geometric_type {
  ePoint2D,
  ePoint3D,
  eSegment2D,
  eSegment3D,
  eRectangle,
  eBox,
  eLine2D,
  eLine3D,
  eTriangle2D,
  eTriangle3D,
  eQuadix2D,
  eQuadix3D,
  eRay2D,
  eRay3D,
  eCircle,
  eSphere
};

/**************[ Vertex type ]***************/

enum BezierType {
  eQuadraticBezier = 2,
  eCubicBezier = 3
};

enum eInclusion {
  eFully,
  ePartially,
  eOutside,
  eUnknown
};

enum eTriangleType {
  etEquilateral,
  etIsosceles,
  etRight,
  etScalene,
  etObtuse,
  etUnknown
};

/**********[ Orientation constants ]**********/
const int RightHandSide = -1;
const int LeftHandSide = +1;
const int Clockwise = -1;
const int CounterClockwise = +1;
const int CollinearOrientation = 0;
const int AboveOrientation = +1;
const int BelowOrientation = -1;
const int CoplanarOrientation = 0;
const int PointInside = +1;
const int PointOutside = -1;
const int Cocircular = 0;
const int Cospherical = 0;

/********[ Clipping Codes ]********/
const int CLIP_BOTTOM = 1;
const int CLIP_TOP = 2;
const int CLIP_LEFT = 4;
const int CLIP_RIGHT = 8;

#include "bool.inl"

//平面两点间距离
CC_INLINE double d2_distance(const double x1, const double y1, const double x2, const double y2)
{
  double dx = (x1 - x2);
  double dy = (y1 - y2);
  return sqrt(dx * dx + dy * dy);
}

CC_INLINE double d3_distance(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2)
{
  double dx = (x1 - x2);
  double dy = (y1 - y2);
  double dz = (z1 - z2);
  return sqrt(dx * dx + dy * dy + dz * dz);
}

CC_INLINE T pt2_distance(const DPOINT2 point1, const DPOINT2 point2)
{
  return d2_distance(point1.x, point1.y, point2.x, point2.y);
}

CC_INLINE T pt3_distance(const DPOINT3 point1, const DPOINT3 point2)
{
  return d3_distance(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
}

CC_INLINE unsigned int d1_quadrant(const double angle)
{
  if ((angle >= (0.0)) && (angle < (90.0))) {
    return 1;
  }
  else if ((angle >= (90.0)) && (angle < (180.0))) {
    return 2;
  }
  else if ((angle >= (180.0)) && (angle < (270.0))) {
    return 3;
  }
  else if ((angle >= (270.0)) && (angle < (360.0))) {
    return 4;
  }
  else if (angle == (360.0)) {
    return 1;
  }
  else {
    return 0;
  }
}

CC_INLINE unsigned int d2_quadrant(const double x, const double y)
{
  if ((x > (0.0)) && (y >= (0.0))) {
    return 1;
  }
  else if ((x <= (0.0)) && (y > (0.0))) {
    return 2;
  }
  else if ((x < (0.0)) && (y <= (0.0))) {
    return 3;
  }
  else if ((x >= (0.0)) && (y < (0.0))) {
    return 4;
  }
  else {
    return 0;
  }

}

CC_INLINE DPOINT2 d2_circumcenter(const double x1, const double y1, const double x2, const double y2, const double x3, const double y3)
{
  DPOINT2 pt;
  double a = x2 - x1;
  double b = y2 - y1;
  double c = x3 - x1;
  double d = y3 - y1;
  double e = a * (x1 + x2) + b * (y1 + y2);
  double f = c * (x1 + x3) + d * (y1 + y3);
  double g = (2.0) * (a * (y3 - y2) - b * (x3 - x2));

  if (d_is_equal(g, (0.0))) {
    pt.x = InfinityT;
    pt.y = InfinityT;
  }
  else {
    pt.x = (d * e - b * f) / g;
    pt.y = (a * f - c * e) / g;
  }

  return pt;
}

CC_INLINE DVECTOR2 v2_add(const DVECTOR2 v1, const DVECTOR2 v2)
{
  DVECTOR2 v;
  v.x = v1.x + v2.x;
  v.y = v1.y + v2.y;
  return v;
}

CC_INLINE DVECTOR3 v3_add(const DVECTOR3 v1, const DVECTOR3 v2)
{
  DVECTOR3 v;
  v.x = v1.x + v2.x;
  v.y = v1.y + v2.y;
  v.z = v1.z + v2.z;
  return v;
}

CC_INLINE DVECTOR3 v3_add3(const DVECTOR3 v1, const DVECTOR3 v2, const DVECTOR3 v3)
{
  DVECTOR3 v;
  v.x = v1.x + v2.x + v3.x;
  v.y = v1.y + v2.y + v3.y;
  v.z = v1.z + v2.z + v3.z;
  return v;
}

CC_INLINE DVECTOR2 v2_sub(const DVECTOR2 v1, const DVECTOR2 v2)
{
  DVECTOR2 v;
  v.x = v1.x - v2.x;
  v.y = v1.y - v2.y;
  return v;
}

CC_INLINE DVECTOR3 v3_sub(const DVECTOR3 v1, const DVECTOR3 v2)
{
  DVECTOR3 v3;
  v3.x = v1.x - v2.x;
  v3.y = v1.y - v2.y;
  v3.z = v1.z - v2.z;
  return v3;
}

CC_INLINE T v2_mul(const DVECTOR2 v1, const DVECTOR2 v2)
{
  return (v1.x * v2.y) - (v1.y - v2.x);
}

CC_INLINE DVECTOR3 v3_mul(const DVECTOR3 v1, const DVECTOR3 v2)
{
  DVECTOR3 v3;
  v3.x = v1.y * v2.z - v1.z * v2.y;
  v3.y = v1.z * v2.x - v1.x * v2.z;
  v3.z = v1.x * v2.y - v1.y * v2.x;
  return v3;
}

CC_INLINE DVECTOR2 v2_perpendicular(const DVECTOR2 v)
{
  DVECTOR2 v2;
  v2.x = v.y, v2.y = -v.x;
  return v2;
}

CC_INLINE DVECTOR3 v3_perpendicular(const DVECTOR3 v)
{
  DVECTOR3 v3;
  v3.x = v.y, v3.y = -v.x, v3.z = v.z;
  return v3;
}

CC_INLINE T v2_dot_product(const DVECTOR2 v1, const DVECTOR2 v2)
{
  return (v1.x * v2.x) + (v1.y * v2.y);
}

CC_INLINE T v3_dot_product(const DVECTOR3 v1, const DVECTOR3 v2)
{
  return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

CC_INLINE T v2_perpendicular_product(const DVECTOR2 v1, const DVECTOR2 v2)
{
  return (v1.x * v2.y) - (v1.y * v2.x);
}

CC_INLINE T v3_triple_product(const DVECTOR3 v1, const DVECTOR3 v2, const DVECTOR3 v3)
{
  return v3_dot_product(v1, v3_mul(v2, v3));
}

CC_INLINE DVECTOR2 v2_scale(const DVECTOR2 v, const T scale)
{
  DVECTOR2 v_;
  v_.x = v.x * scale;
  v_.y = v.y * scale;
  return v_;
}

CC_INLINE DVECTOR3 v3_scale(const DVECTOR3 v, const T scale)
{
  DVECTOR3 v_;
  v_.x = v.x * scale;
  v_.y = v.y * scale;
  v_.z = v.z * scale;
  return v_;
}

CC_INLINE DPOINT2 pt2_scale2(const DPOINT2 point, const T scale)
{
  DPOINT2 pt;
  pt.x = point.x * scale;
  pt.y = point.y * scale;
  return pt;
}

CC_INLINE DPOINT3 pt3_scale2(const DPOINT3 point, const T scale)
{
  DPOINT3 pt;
  pt.x = point.x * scale;
  pt.y = point.y * scale;
  pt.z = point.z * scale;
  return pt;
}

CC_INLINE DPOINT2 pt2_add(const DPOINT2 point, const DVECTOR2 v)
{
  DPOINT2 point_;
  point_.x = point.x + v.x;
  point_.y = point.y + v.y;
  return point_;
}

CC_INLINE DPOINT3 pt3_add(const DPOINT3 point, const DVECTOR3 v)
{
  DPOINT3 point_;
  point_.x = point.x + v.x;
  point_.y = point.y + v.y;
  point_.z = point.z + v.z;
  return point_;
}

CC_INLINE DVECTOR2 pt2_sub(const DPOINT2 p1, const DPOINT2 p2)
{
  DVECTOR2 v_;
  v_.x = p1.x - p2.x;
  v_.y = p1.y - p2.y;
  return v_;
}

CC_INLINE DVECTOR3 pt3_sub(const DPOINT3 p1, const DPOINT3 p2)
{
  DVECTOR3 v_;
  v_.x = p1.x - p2.x;
  v_.y = p1.y - p2.y;
  v_.z = p1.z - p2.z;
  return v_;
}

CC_INLINE DPOINT2 d2_rotate(const T rotation_angle, const T x, const T y)
{
  DPOINT2 pt;
  T sin_val = sin(rotation_angle * (PIDiv180));
  T cos_val = cos(rotation_angle * (PIDiv180));
  pt.x = (x * cos_val) - (y * sin_val);
  pt.y = (y * cos_val) + (x * sin_val);
  return pt;
}

CC_INLINE DPOINT2 d2_rotate_o(const T rotation_angle, const T x, const T y, const T ox, const T oy)
{
  DPOINT2 pt = d2_rotate(rotation_angle, x - ox, y - oy);
  pt.x += ox;
  pt.y += oy;
  return pt;
}

CC_INLINE DPOINT2 pt2_rotate(const T rotation_angle, const DPOINT2 point)
{
  return d2_rotate(rotation_angle, point.x, point.y);
}

CC_INLINE DPOINT2 pt2_rotate_o(const T rotation_angle, const DPOINT2 point, const DPOINT2 opoint)
{
  return d2_rotate_o(rotation_angle, point.x, point.y, opoint.x, opoint.y);
}

CC_INLINE DPOINT2 pt2_translate(const T dx, const T dy, const DPOINT2 point)
{
  DPOINT2 point_;
  point_.x = point.x + dx;
  point_.y = point.y + dy;
  return point_;
}

CC_INLINE int ptn2_translate(const T dx, const T dy, int n, const DPOINT2* point, DPOINT2* out)
{
  int i;

  for (i = 0; i < n; ++i) {
    out[i] = pt2_translate(dx, dy, point[i]);
  }

  return n;
}

CC_INLINE DPOINT3 pt3_translate(const T dx, const T dy, const T dz, const DPOINT3 point)
{
  DPOINT3 point_;
  point_.x = point.x + dx;
  point_.y = point.y + dy;
  point_.z = point.z + dz;
  return point_;
}

CC_INLINE int ptn3_translate(const T dx, const T dy, const T dz, int n, const DPOINT3* point, DPOINT3* out)
{
  int i;

  for (i = 0; i < n; ++i) {
    out[i] = pt3_translate(dx, dy, dz, point[i]);
  }

  return 0;
}

CC_INLINE T d2_lay_distance(const T x1, const T y1, const T x2, const T y2)
{
  T dx = (x2 - x1);
  T dy = (y2 - y1);
  return dx * dx + dy * dy;
}

CC_INLINE T d3_lay_distance(const T x1, const T y1, const T z1, const T x2, const T y2, const T z2)
{
  T dx = (x2 - x1);
  T dy = (y2 - y1);
  T dz = (z2 - z1);
  return dx * dx + dy * dy + dz * dz;
}

CC_INLINE int d2_orientation(const double x1, const double y1,
    const double x2, const double y2,
    const double px, const double py)
{
  double orin = (x2 - x1) * (py - y1) - (px - x1) * (y2 - y1);

  if (orin > (0.0)) {
    return LeftHandSide; /* Orientaion is to the left-hand side */
  }
  else if (orin < (0.0)) {
    return RightHandSide; /* Orientaion is to the right-hand side */
  }
  else {
    return CollinearOrientation; /* Orientaion is neutral aka collinear */
  }
}

CC_INLINE int d3_orientation(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3,
    const T px, const T py, const T pz)
{
  T px1 = x1 - px;
  T px2 = x2 - px;
  T px3 = x3 - px;

  T py1 = y1 - py;
  T py2 = y2 - py;
  T py3 = y3 - py;

  T pz1 = z1 - pz;
  T pz2 = z2 - pz;
  T pz3 = z3 - pz;

  T orin = px1 * (py2 * pz3 - pz2 * py3) +
      px2 * (py3 * pz1 - pz3 * py1) +
      px3 * (py1 * pz2 - pz1 * py2);

  if (orin < (0.0)) {
    return BelowOrientation; /* Orientaion is below plane */
  }
  else if (orin > (0.0)) {
    return AboveOrientation; /* Orientaion is above plane */
  }
  else {
    return CoplanarOrientation; /* Orientaion is coplanar to plane if Result is 0 */
  }
}

CC_INLINE int robust_orientation(const T x1, const T y1,
    const T x2, const T y2,
    const T px, const T py)
{
  T orin = (x2 - x1) * (py - y1) - (px - x1) * (y2 - y1);

  /*
  Calculation Policy:
  if |Orin - Orin`| < Epsilon then Orin` is assumed to be equal to zero.
  Where:
  Orin : is the "real" mathematically precise orientation value, using infinite
  precision arithmetic (hypothetical)
  Orin`: is the calculated imprecise orientation value, using finite precision
  arithmetic
  */
  if (d_is_equal(orin, (0.0))) {
    return CollinearOrientation; /* orientaion is neutral aka collinear */
  }
  else if (orin < (0.0)) {
    return RightHandSide; /* orientaion is to the right-hand side */
  }
  else {
    return LeftHandSide; /* orientaion is to the left-hand side */
  }
}

CC_INLINE int robust_orientation3(const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2,
    const T x3, const T y3, const T z3,
    const T px, const T py, const T pz)
{
  T px1 = x1 - px;
  T px2 = x2 - px;
  T px3 = x3 - px;

  T py1 = y1 - py;
  T py2 = y2 - py;
  T py3 = y3 - py;

  T pz1 = z1 - pz;
  T pz2 = z2 - pz;
  T pz3 = z3 - pz;

  T orin = px1 * (py2 * pz3 - pz2 * py3) +
      px2 * (py3 * pz1 - pz3 * py1) +
      px3 * (py1 * pz2 - pz1 * py2);

  if (d_is_equal(orin, (0.0))) {
    return CoplanarOrientation; /* Orientaion is coplanar to plane if Result is 0 */
  }
  else if (orin < (0.0)) {
    return BelowOrientation; /* Orientaion is below plane */
  }
  else {
    return AboveOrientation; /* Orientaion is above plane */
  }
}

CC_INLINE BOOL d2_point_in_rectangle(const T px, const T py,
    const T x1, const T y1,
    const T x2, const T y2)
{
  return ((x1 <= px) && (px <= x2) && (y1 <= py) && (py <= y2)) ||
      ((x2 <= px) && (px <= x1) && (y2 <= py) && (py <= y1));
}

CC_INLINE BOOL d3_point_in_box(const T px, const T py, const T pz,
    const T x1, const T y1, const T z1,
    const T x2, const T y2, const T z2)
{
  return ((x1 <= px) && (px <= x2) && (y1 <= py) && (py <= y2) && (z1 <= pz) && (pz <= z2)) ||
      ((x2 <= px) && (px <= x1) && (y2 <= py) && (py <= y1) && (z2 <= pz) && (pz <= z1));
}

CC_INLINE BOOL d2_point_in_triangle(const T px, const T py,
    const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3)
{
  int or1 = d2_orientation(x1, y1, x2, y2, px, py);
  int or2 = d2_orientation(x2, y2, x3, y3, px, py);

  if ((or1 * or2) == -1) {
    return false;
  }
  else {
    int or3 = d2_orientation(x3, y3, x1, y1, px, py);

    if ((or1 == or3) || (0 == or3)) {
      return true;
    }
    else if (0 == or1) {
      return ((or2 * or3) >= 0);
    }
    else if (0 == or2) {
      return ((or1 * or3) >= 0);
    }
    else {
      return false;
    }
  }
}

CC_INLINE BOOL d2_point_in_quadix(const T px, const T py,
    const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3,
    const T x4, const T y4)
{
  int or1 = d2_orientation(x1, y1, x2, y2, px, py);
  int or2 = d2_orientation(x2, y2, x3, y3, px, py);
  int or3 = d2_orientation(x3, y3, x4, y4, px, py);
  int or4 = d2_orientation(x4, y4, x1, y1, px, py);

  if ((or1 == or2) && (or2 == or3) && (or3 == or4)) {
    return true;
  }
  else if (0 == or1) {
    return (0 == (or2 * or4));
  }
  else if (0 == or2) {
    return (0 == (or1 * or3));
  }
  else if (0 == or3) {
    return (0 == (or2 * or4));
  }
  else if (0 == or4) {
    return (0 == (or1 * or3));
  }
  else {
    return false;
  }
}

CC_INLINE void d2_calculate_qbezier_coefficients(const DQUADRATIC_BEZIER2 bezier, DPOINT2* a, DPOINT2* b)
{
  b->x = (2.0) * (bezier.p1.x - bezier.p0.x);
  b->y = (2.0) * (bezier.p1.y - bezier.p0.y);
  a->x = bezier.p2.x - bezier.p0.x - b->x;
  a->y = bezier.p2.y - bezier.p0.y - b->y;
}

CC_INLINE void d3_calculate_qbezier_coefficients(const DQUADRATIC_BEZIER3 bezier, DPOINT3* a, DPOINT3* b)
{
  b->x = (2.0) * (bezier.p1.x - bezier.p0.x);
  b->y = (2.0) * (bezier.p1.y - bezier.p0.y);
  b->z = (2.0) * (bezier.p1.z - bezier.p0.z);
  a->x = bezier.p2.x - bezier.p0.x - b->x;
  a->y = bezier.p2.y - bezier.p0.y - b->y;
  a->z = bezier.p2.z - bezier.p0.z - b->z;
}

CC_INLINE void d2_calculate_cbezier_coefficients(const DCUBIC_BEZIER2 bezier, DPOINT2* a, DPOINT2* b, DPOINT2* c)
{
  c->x = (3.0) * (bezier.p1.x - bezier.p0.x);
  c->y = (3.0) * (bezier.p1.y - bezier.p0.y);
  b->x = (3.0) * (bezier.p2.x - bezier.p1.x) - c->x;
  b->y = (3.0) * (bezier.p2.y - bezier.p1.y) - c->y;
  a->x = bezier.p3.x - bezier.p0.x - c->x - b->x;
  a->y = bezier.p3.y - bezier.p0.y - c->y - b->y;
}

CC_INLINE void d3_calculate_cbezier_coefficients(const DCUBIC_BEZIER3 bezier, DPOINT3* a, DPOINT3* b, DPOINT3* c)
{
  c->x = (3.0) * (bezier.p1.x - bezier.p0.x);
  c->y = (3.0) * (bezier.p1.y - bezier.p0.y);
  c->z = (3.0) * (bezier.p1.z - bezier.p0.z);
  b->x = (3.0) * (bezier.p2.x - bezier.p1.x) - c->x;
  b->y = (3.0) * (bezier.p2.y - bezier.p1.y) - c->y;
  b->z = (3.0) * (bezier.p2.z - bezier.p1.z) - c->z;
  a->x = bezier.p3.x - bezier.p0.x - c->x - b->x;
  a->y = bezier.p3.y - bezier.p0.y - c->y - b->y;
  a->z = bezier.p3.z - bezier.p0.z - c->z - b->z;
}

CC_INLINE void pt2_calculate_qbezier_coefficients(const DQUADRATIC_BEZIER2 bezier, DPOINT2* coeffs)
{
  d2_calculate_qbezier_coefficients(bezier, coeffs + 0, coeffs + 1);
}

CC_INLINE void pt3_calculate_qbezier_coefficients(const DQUADRATIC_BEZIER3 bezier,
    DPOINT3* coeffs)
{
  d3_calculate_qbezier_coefficients(bezier, coeffs + 0, coeffs + 1);
}

CC_INLINE void pt2_calculate_cbezier_coefficients(const DCUBIC_BEZIER2 bezier, DPOINT2* coeffs)
{
  d2_calculate_cbezier_coefficients(bezier, coeffs + 0, coeffs + 1, coeffs + 2);
}

CC_INLINE void pt3_calculate_cbezier_coefficients(const DCUBIC_BEZIER3 bezier, DPOINT3* coeffs)
{
  d3_calculate_cbezier_coefficients(bezier, coeffs + 0, coeffs + 1, coeffs + 2);
}

CC_INLINE BOOL d2_point_in_convex_polygon(const T px, const T py, const DPOLYGON2* polygon)
{
  const DPOINT2* p = polygon->p;
  int i, j = 0, n = polygon->n;
  int initial_orientation;

  if (polygon->n < 3) {
    return false;
  }

  initial_orientation = d2_orientation(p[0].x, p[0].y, p[n - 1].x, p[n - 1].y, px, py);

  for (i = 1; i < n; ++i) {
    if (initial_orientation != d2_orientation(p[i].x, p[i].y, p[j].x, p[j].y, px, py)) {
      return false;
    }

    j = i;
  }

  return true;
}

CC_INLINE BOOL d2_point_in_polygon(const T px, const T py, int n, const DPOINT2* polygon)
{
  const DPOINT2* p = polygon;
  int i, j = 0;
  BOOL result = false;

  if (n < 3) {
    return false;
  }

  j = n - 1;

  for (i = 0; i < n; ++i) {
    if (((p[i].y <= py) && (py < p[j].y)) || // an upward crossing
        ((p[j].y <= py) && (py < p[i].y))) { // a downward crossing
      /* compute the edge-ray intersect @ the x-coordinate */
      if (px - p[i].x < ((p[j].x - p[i].x) * (py - p[i].y) / (p[j].y - p[i].y))) {
        result = !result;
      }
    }

    j = i;
  }

  return result;
}

CC_INLINE BOOL d2_intersect(const T x1, const T y1,
    const T x2, const T y2,
    const T x3, const T y3,
    const T x4, const T y4)
{
  T ax = x2 - x1;
  T bx = x3 - x4;

  T ay, by, cx, cy, d, f, e;
  T lowerx, upperx;
  T uppery, lowery;

  if (ax < (0.0)) {
    lowerx = x2;
    upperx = x1;
  }
  else {
    upperx = x2;
    lowerx = x1;
  }

  if (bx > (0.0)) {
    if ((upperx < x4) || (x3 < lowerx)) {
      return false;
    }
  }
  else if ((upperx < x3) || (x4 < lowerx)) {
    return false;
  }

  ay = y2 - y1;
  by = y3 - y4;

  if (ay < (0.0)) {
    lowery = y2;
    uppery = y1;
  }
  else {
    uppery = y2;
    lowery = y1;
  }

  if (by > (0.0)) {
    if ((uppery < y4) || (y3 < lowery)) {
      return false;
    }
  }
  else if ((uppery < y3) || (y4 < lowery)) {
    return false;
  }

  cx = x1 - x3;
  cy = y1 - y3;
  d = (by * cx) - (bx * cy);
  f = (ay * bx) - (ax * by);

  if (f > (0.0)) {
    if ((d < (0.0)) || (d > f)) {
      return false;
    }
  }
  else if ((d > (0.0)) || (d < f)) {
    return false;
  }

  e = (ax * cy) - (ay * cx);

  if (f > (0.0)) {
    if ((e < (0.0)) || (e > f)) {
      return false;
    }
  }
  else if ((e > (0.0)) || (e < f)) {
    return false;
  }

  return true;
}

CC_INLINE BOOL d2_intersect_at(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3, const T x4, const T y4, DPOINT2* pt)
{
  T ax = x2 - x1;
  T bx = x3 - x4;

  T ay, by, cx, cy, d, f, e, ratio;
  T lowerx, upperx;
  T uppery, lowery;

  if (ax < (0.0)) {
    lowerx = x2;
    upperx = x1;
  }
  else {
    upperx = x2;
    lowerx = x1;
  }

  if (bx > (0.0)) {
    if ((upperx < x4) || (x3 < lowerx)) {
      return false;
    }
  }
  else if ((upperx < x3) || (x4 < lowerx)) {
    return false;
  }

  ay = y2 - y1;
  by = y3 - y4;

  if (ay < (0.0)) {
    lowery = y2;
    uppery = y1;
  }
  else {
    uppery = y2;
    lowery = y1;
  }

  if (by > (0.0)) {
    if ((uppery < y4) || (y3 < lowery)) {
      return false;
    }
  }
  else if ((uppery < y3) || (y4 < lowery)) {
    return false;
  }

  cx = x1 - x3;
  cy = y1 - y3;
  d = (by * cx) - (bx * cy);
  f = (ay * bx) - (ax * by);

  if (f > (0.0)) {
    if ((d < (0.0)) || (d > f)) {
      return false;
    }
  }
  else if ((d > (0.0)) || (d < f)) {
    return false;
  }

  e = (ax * cy) - (ay * cx);

  if (f > (0.0)) {
    if ((e < (0.0)) || (e > f)) {
      return false;
    }
  }
  else if ((e > (0.0)) || (e < f)) {
    return false;
  }

  ratio = (ax * -by) - (ay * -bx);

  if (d_not_equal(ratio, (0.0))) {
    ratio = ((cy * -bx) - (cx * -by)) / ratio;
    pt->x = x1 + (ratio * ax);
    pt->y = y1 + (ratio * ay);
  }
  else {
    if (d_is_equal((ax * -cy), (-cx * ay))) {
      pt->x = x3;
      pt->y = y3;
    }
    else {
      pt->x = x4;
      pt->y = y4;
    }
  }

  return true;
}

CC_INLINE BOOL pt2_intersect(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3, const DPOINT2 point4)
{
  return d2_intersect(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, point4.x, point4.y);
}

CC_INLINE BOOL pt2_intersect_at(const DPOINT2 point1, const DPOINT2 point2,
    const DPOINT2 point3, const DPOINT2 point4, DPOINT2* int_point)
{
  return d2_intersect_at(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y,  point4.x, point4.y, int_point);
}

CC_INLINE int pt2_orientation(const DPOINT2 point1, const DPOINT2 point2, const DPOINT2 point3)
{
  return d2_orientation(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

CC_INLINE int pt2_orientation2(const DPOINT2 point1, const DPOINT2 point2, const double px, const double py)
{
  return d2_orientation(point1.x, point1.y, point2.x, point2.y, px, py);
}

CC_INLINE BOOL qu2_convex_quadix(const DQUADIX2 quadix)
{
  int orin = pt2_orientation(quadix.p0, quadix.p2, quadix.p1);

  if (orin != pt2_orientation(quadix.p1, quadix.p3, quadix.p2)) {
    return false;
  }

  if (orin != pt2_orientation(quadix.p2, quadix.p0, quadix.p3)) {
    return false;
  }

  if (orin != pt2_orientation(quadix.p3, quadix.p1, quadix.p0)) {
    return false;
  }

  return true;
}

CC_INLINE BOOL qu3_convex_quadix(const DQUADIX3 quadix)
{
  DVECTOR3 bda, bdc, acd, acb;
  bda = v3_mul(v3_sub(quadix.p3, quadix.p1), v3_sub(quadix.p0, quadix.p1));
  bdc = v3_mul(v3_sub(quadix.p3, quadix.p1), v3_sub(quadix.p2, quadix.p1));

  if (d_greater_than_or_equal(v3_dot_product(bda, bdc), (0.0))) {
    return false;
  }

  acd = v3_mul(v3_sub(quadix.p2, quadix.p0), v3_sub(quadix.p3, quadix.p0));
  acb = v3_mul(v3_sub(quadix.p2, quadix.p0), v3_sub(quadix.p1, quadix.p0));
  return v3_dot_product(acd, acb) < (0.0);
}

