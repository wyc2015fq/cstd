#include <math.h>
#define VTK_SMALL_NUMBER 1.0e-12
bool IsNan(double x)
{
  return IsNaN_f64(x);
}
bool IsInf(double x)
{
  return IsInfinite_f64(x);
}
/**
 * Rounds a float to the nearest integer.
 */
static int Round(float f)
{
  return (int)(f + (f >= 0.0 ? 0.5 : -0.5));
}
static int Round(double f)
{
  return (int)(f + (f >= 0.0 ? 0.5 : -0.5));
}
/**
 * Multiplies a 3-vector by a scalar (float version).
 * This modifies the input 3-vector.
 */
static void MultiplyScalar(float a[3], float s)
{
  for (int i = 0; i < 3; ++i) {
    a[i] *= s;
  }
}
/**
 * Multiplies a 2-vector by a scalar (float version).
 * This modifies the input 2-vector.
 */
static void MultiplyScalar2D(float a[2], float s)
{
  for (int i = 0; i < 2; ++i) {
    a[i] *= s;
  }
}
/**
 * Multiplies a 3-vector by a scalar (double version).
 * This modifies the input 3-vector.
 */
static void MultiplyScalar(double a[3], double s)
{
  for (int i = 0; i < 3; ++i) {
    a[i] *= s;
  }
}
/**
 * Multiplies a 2-vector by a scalar (double version).
 * This modifies the input 2-vector.
 */
static void MultiplyScalar2D(double a[2], double s)
{
  for (int i = 0; i < 2; ++i) {
    a[i] *= s;
  }
}
/**
 * Dot product of two 3-vectors (float version).
 */
static float Dot(const float a[3], const float b[3])
{
  return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
};
/**
 * Dot product of two 3-vectors (double-precision version).
 */
static double Dot(const double a[3], const double b[3])
{
  return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
};
/**
 * Outer product of two 3-vectors (float version).
 */
static void Outer(const float a[3], const float b[3], float C[9])
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      C[i * 3 + j] = a[i] * b[j];
    }
}
/**
 * Outer product of two 3-vectors (double-precision version).
 */
static void Outer(const double a[3], const double b[3], double C[9])
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      C[i * 9 + j] = a[i] * b[j];
    }
}
/**
 * Dot product of two 2-vectors.
 */
static float Dot2D(const float x[2], const float y[2])
{
  return (x[0] * y[0] + x[1] * y[1]);
}
/**
 * Dot product of two 2-vectors. (double-precision version).
 */
static double Dot2D(const double x[2], const double y[2])
{
  return (x[0] * y[0] + x[1] * y[1]);
}
/**
 * Outer product of two 2-vectors (float version).
 */
static void Outer2D(const float2 x, const float2 y, float9 A)
{
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      A[i * 2 + j] = x[i] * y[j];
    }
  }
}
/**
 * Outer product of two 2-vectors (float version).
 */
static void Outer2D(const double x[2], const double y[2], double A[4])
{
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      A[i * 2 + j] = x[i] * y[j];
    }
  }
}
/**
 * Compute determinant of 2x2 matrix. Two columns of matrix are input.
 */
static float Determinant2x2(const float c1[2], const float c2[2])
{
  return (c1[0] * c2[1] - c2[0] * c1[1]);
};
//@{
/**
 * Calculate the determinant of a 2x2 matrix: | a b | | c d |
 */
static double Determinant2x2(double a, double b, double c, double d)
{
  return (a * d - b * c);
};
static double Determinant2x2(const double c1[2], const double c2[2])
{
  return (c1[0] * c2[1] - c2[0] * c1[1]);
};
//@}
//@{
/**
 * Convert color in RGB format (Red, Green, Blue) to HSV format
 * (Hue, Saturation, Value). The input color is not modified.
 * The input RGB must be float values in the range [0,1].
 * The output ranges are hue [0, 1], saturation [0, 1], and
 * value [0, 1].
 */
void RGBToHSV(double r, double g, double b,
    float* h, float* s, float* v)
{
  double onethird = 1.0 / 3.0;
  double onesixth = 1.0 / 6.0;
  double twothird = 2.0 / 3.0;
  double cmax, cmin;
  cmax = r;
  cmin = r;
  if (g > cmax) {
    cmax = g;
  }
  else if (g < cmin) {
    cmin = g;
  }
  if (b > cmax) {
    cmax = b;
  }
  else if (b < cmin) {
    cmin = b;
  }
  *v = cmax;
  if (*v > 0.0) {
    *s = (cmax - cmin) / cmax;
  }
  else {
    *s = 0.0;
  }
  if (*s > 0) {
    if (r == cmax) {
      *h = onesixth * (g - b) / (cmax - cmin);
    }
    else if (g == cmax) {
      *h = onethird + onesixth * (b - r) / (cmax - cmin);
    }
    else {
      *h = twothird + onesixth * (r - g) / (cmax - cmin);
    }
    if (*h < 0.0) {
      *h += 1.0;
    }
  }
  else {
    *h = 0.0;
  }
}
float* RGBToHSV(double r, double g, double b)
{
  static float hsv[3];
  RGBToHSV(r, g, b, hsv, hsv + 1, hsv + 2);
  return hsv;
}
float* RGBToHSV(const double rgb[3])
{
  return RGBToHSV(rgb[0], rgb[1], rgb[2]);
}
void HSVToRGB(double h, double s, double v,
    float* r, float* g, float* b)
{
  const double onethird = 1.0 / 3.0;
  const double onesixth = 1.0 / 6.0;
  const double twothird = 2.0 / 3.0;
  const double fivesixth = 5.0 / 6.0;
  // compute RGB from HSV
  if (h > onesixth && h <= onethird) { // green/red
    *g = 1.0;
    *r = (onethird - h) / onesixth;
    *b = 0.0;
  }
  else if (h > onethird && h <= 0.5) { // green/blue
    *g = 1.0;
    *b = (h - onethird) / onesixth;
    *r = 0.0;
  }
  else if (h > 0.5 && h <= twothird) { // blue/green
    *b = 1.0;
    *g = (twothird - h) / onesixth;
    *r = 0.0;
  }
  else if (h > twothird && h <= fivesixth) { // blue/red
    *b = 1.0;
    *r = (h - twothird) / onesixth;
    *g = 0.0;
  }
  else if (h > fivesixth && h <= 1.0) { // red/blue
    *r = 1.0;
    *b = (1.0 - h) / onesixth;
    *g = 0.0;
  }
  else { // red/green
    *r = 1.0;
    *g = h / onesixth;
    *b = 0.0;
  }
  // add Saturation to the equation.
  *r = (s * *r + (1.0 - s));
  *g = (s * *g + (1.0 - s));
  *b = (s * *b + (1.0 - s));
  *r *= v;
  *g *= v;
  *b *= v;
}
void HSVToRGB(float h, float s, float v, float* r, float* g, float* b)
{
  float dr, dg, db;
  HSVToRGB(h, s, v, &dr, &dg, &db);
  *r = (float)(dr);
  *g = (float)(dg);
  *b = (float)(db);
}
float* HSVToRGB(double h, double s, double v)
{
  static float rgb[3];
  HSVToRGB(h, s, v, rgb, rgb + 1, rgb + 2);
  return rgb;
}
float* HSVToRGB(const float hsv[3])
{
  return HSVToRGB(hsv[0], hsv[1], hsv[2]);
}
void LabToXYZ(double L, double a, double b, float* x, float* y, float* z)
{
  //LAB to XYZ
  double var_Y = (L + 16) / 116;
  double var_X = a / 500 + var_Y;
  double var_Z = var_Y - b / 200;
  if (pow(var_Y, 3) > 0.008856) {
    var_Y = pow(var_Y, 3);
  }
  else {
    var_Y = (var_Y - 16.0 / 116.0) / 7.787;
  }
  if (pow(var_X, 3) > 0.008856) {
    var_X = pow(var_X, 3);
  }
  else {
    var_X = (var_X - 16.0 / 116.0) / 7.787;
  }
  if (pow(var_Z, 3) > 0.008856) {
    var_Z = pow(var_Z, 3);
  }
  else {
    var_Z = (var_Z - 16.0 / 116.0) / 7.787;
  }
  const double ref_X = 0.9505;
  const double ref_Y = 1.000;
  const double ref_Z = 1.089;
  *x = ref_X * var_X; //ref_X = 0.9505 Observer= 2 deg Illuminant= D65
  *y = ref_Y * var_Y; //ref_Y = 1.000
  *z = ref_Z * var_Z; //ref_Z = 1.089
}
float* LabToXYZ(const float lab[3])
{
  static float xyz[3];
  LabToXYZ(lab[0], lab[1], lab[2], xyz + 0, xyz + 1, xyz + 2);
  return xyz;
}
void XYZToLab(double x, double y, double z, float* L, float* a, float* b)
{
  const double ref_X = 0.9505;
  const double ref_Y = 1.000;
  const double ref_Z = 1.089;
  double var_X = x / ref_X; //ref_X = 0.9505 Observer= 2 deg, Illuminant= D65
  double var_Y = y / ref_Y; //ref_Y = 1.000
  double var_Z = z / ref_Z; //ref_Z = 1.089
  if (var_X > 0.008856) {
    var_X = pow(var_X, 1.0 / 3.0);
  }
  else {
    var_X = (7.787 * var_X) + (16.0 / 116.0);
  }
  if (var_Y > 0.008856) {
    var_Y = pow(var_Y, 1.0 / 3.0);
  }
  else {
    var_Y = (7.787 * var_Y) + (16.0 / 116.0);
  }
  if (var_Z > 0.008856) {
    var_Z = pow(var_Z, 1.0 / 3.0);
  }
  else {
    var_Z = (7.787 * var_Z) + (16.0 / 116.0);
  }
  *L = (116 * var_Y) - 16;
  *a = 500 * (var_X - var_Y);
  *b = 200 * (var_Y - var_Z);
}
float* XYZToLab(const float xyz[3])
{
  static float lab[3];
  XYZToLab(xyz[0], xyz[1], xyz[2], lab + 0, lab + 1, lab + 2);
  return lab;
}
void XYZToRGB(double x, double y, double z,
    float* r, float* g, float* b)
{
  //double ref_X = 0.9505; //Observer = 2 deg Illuminant = D65
  //double ref_Y = 1.000;
  //double ref_Z = 1.089;
  *r = x * 3.2406 + y * -1.5372 + z * -0.4986;
  *g = x * -0.9689 + y * 1.8758 + z * 0.0415;
  *b = x * 0.0557 + y * -0.2040 + z * 1.0570;
  // The following performs a "gamma correction" specified by the sRGB color
  // space. sRGB is defined by a canonical definition of a display monitor and
  // has been standardized by the International Electrotechnical Commission (IEC
  // 61966-2-1). The nonlinearity of the correction is designed to make the
  // colors more perceptually uniform. This color space has been adopted by
  // several applications including Adobe Photoshop and Microsoft Windows color
  // management. OpenGL is agnostic on its RGB color space, but it is reasonable
  // to assume it is close to this one.
  if (*r > 0.0031308) {
    *r = 1.055 * (pow(*r, (1 / 2.4))) - 0.055;
  }
  else {
    *r = 12.92 * (*r);
  }
  if (*g > 0.0031308) {
    *g = 1.055 * (pow(*g , (1 / 2.4))) - 0.055;
  }
  else {
    *g = 12.92 * (*g);
  }
  if (*b > 0.0031308) {
    *b = 1.055 * (pow(*b, (1 / 2.4))) - 0.055;
  }
  else {
    *b = 12.92 * (*b);
  }
  // Clip colors. ideally we would do something that is perceptually closest
  // (since we can see colors outside of the display gamut), but this seems to
  // work well enough.
  double maxVal = *r;
  if (maxVal < *g) {
    maxVal = *g;
  }
  if (maxVal < *b) {
    maxVal = *b;
  }
  if (maxVal > 1.0) {
    *r /= maxVal;
    *g /= maxVal;
    *b /= maxVal;
  }
  if (*r < 0) {
    *r = 0;
  }
  if (*g < 0) {
    *g = 0;
  }
  if (*b < 0) {
    *b = 0;
  }
}
float* XYZToRGB(const float xyz[3])
{
  static float rgb[3];
  XYZToRGB(xyz[0], xyz[1], xyz[2], rgb + 0, rgb + 1, rgb + 2);
  return rgb;
}
void RGBToXYZ(double r, double g, double b,
    float* x, float* y, float* z)
{
  // The following performs a "gamma correction" specified by the sRGB color
  // space. sRGB is defined by a canonical definition of a display monitor and
  // has been standardized by the International Electrotechnical Commission (IEC
  // 61966-2-1). The nonlinearity of the correction is designed to make the
  // colors more perceptually uniform. This color space has been adopted by
  // several applications including Adobe Photoshop and Microsoft Windows color
  // management. OpenGL is agnostic on its RGB color space, but it is reasonable
  // to assume it is close to this one.
  if (r > 0.04045) {
    r = pow((r + 0.055) / 1.055, 2.4);
  }
  else {
    r = r / 12.92;
  }
  if (g > 0.04045) {
    g = pow((g + 0.055) / 1.055, 2.4);
  }
  else {
    g = g / 12.92;
  }
  if (b > 0.04045) {
    b = pow((b + 0.055) / 1.055, 2.4);
  }
  else {
    b = b / 12.92;
  }
  //Observer. = 2 deg, Illuminant = D65
  *x = r * 0.4124 + g * 0.3576 + b * 0.1805;
  *y = r * 0.2126 + g * 0.7152 + b * 0.0722;
  *z = r * 0.0193 + g * 0.1192 + b * 0.9505;
}
float* RGBToXYZ(const float rgb[3])
{
  static float xyz[3];
  RGBToXYZ(rgb[0], rgb[1], rgb[2], xyz + 0, xyz + 1, xyz + 2);
  return xyz;
}
void RGBToLab(double red, double green, double blue,
    float* L, float* a, float* b)
{
  float x, y, z;
  RGBToXYZ(red, green, blue, &x, &y, &z);
  XYZToLab(x, y, z, L, a, b);
}
void LabToRGB(double L, double a, double b,
    float* red, float* green, float* blue)
{
  float x, y, z;
  LabToXYZ(L, a, b, &x, &y, &z);
  XYZToRGB(x, y, z, red, green, blue);
}
float* LabToRGB(const float lab[3])
{
  return XYZToRGB(LabToXYZ(lab));
}
float* RGBToLab(const float rgb[3])
{
  return XYZToLab(RGBToXYZ(rgb));
}
void ClampValues(float* values,
    int nb_values,
    const float range[2])
{
  if (!values || nb_values <= 0 || !range) {
    return;
  }
  const float* values_end = values + nb_values;
  while (values < values_end) {
    if (*values < range[0]) {
      *values = range[0];
    }
    else if (*values > range[1]) {
      *values = range[1];
    }
    values++;
  }
}
void ClampValues(const float* values,
    int nb_values,
    const double range[2],
    float* clamped_values)
{
  if (!values || nb_values <= 0 || !range || !clamped_values) {
    return;
  }
  const float* values_end = values + nb_values;
  while (values < values_end) {
    if (*values < range[0]) {
      *clamped_values = range[0];
    }
    else if (*values > range[1]) {
      *clamped_values = range[1];
    }
    else {
      *clamped_values = *values;
    }
    values++;
    clamped_values++;
  }
}
static void RGBToHSV(const float rgb[3], float hsv[3])
{
  RGBToHSV(rgb[0], rgb[1], rgb[2], hsv, hsv + 1, hsv + 2);
}
//@}
//@{
/**
 * Convert color in HSV format (Hue, Saturation, Value) to RGB
 * format (Red, Green, Blue). The input color is not modified.
 * The input 'hsv' must be float values in the range [0,1].
 * The elements of each component of the output 'rgb' are in
 * the range [0, 1].
 */
static void HSVToRGB(const float hsv[3], float rgb[3])
{
  HSVToRGB(hsv[0], hsv[1], hsv[2], rgb, rgb + 1, rgb + 2);
}
//@}
//@{
/**
 * Convert color from the CIE-L*ab system to CIE XYZ.
 */
static void LabToXYZ(const float lab[3], float xyz[3])
{
  LabToXYZ(lab[0], lab[1], lab[2], xyz + 0, xyz + 1, xyz + 2);
}
//@}
//@{
/**
 * Convert Color from the CIE XYZ system to CIE-L*ab.
 */
static void XYZToLab(const float xyz[3], float lab[3])
{
  XYZToLab(xyz[0], xyz[1], xyz[2], lab + 0, lab + 1, lab + 2);
}
//@}
//@{
/**
 * Convert color from the CIE XYZ system to RGB.
 */
static void XYZToRGB(const float xyz[3], float rgb[3])
{
  XYZToRGB(xyz[0], xyz[1], xyz[2], rgb + 0, rgb + 1, rgb + 2);
}
//@}
//@{
/**
 * Convert color from the RGB system to CIE XYZ.
 */
static void RGBToXYZ(const float rgb[3], float xyz[3])
{
  RGBToXYZ(rgb[0], rgb[1], rgb[2], xyz + 0, xyz + 1, xyz + 2);
}
//@}
//@{
/**
 * Convert color from the RGB system to CIE-L*ab.
 * The input RGB must be values in the range [0,1].
 * The output ranges of 'L' is [0, 100]. The output
 * range of 'a' and 'b' are approximately [-110, 110].
 */
static void RGBToLab(const float rgb[3], float lab[3])
{
  RGBToLab(rgb[0], rgb[1], rgb[2], lab + 0, lab + 1, lab + 2);
}
//@}
//@{
/**
 * Convert color from the CIE-L*ab system to RGB.
 */
static void LabToRGB(const float lab[3], float rgb[3])
{
  LabToRGB(lab[0], lab[1], lab[2], rgb + 0, rgb + 1, rgb + 2);
}
//@}
//@{
/**
 * Set the bounds to an uninitialized state
 */
static void UninitializeBounds(double bounds[6])
{
  bounds[0] = 1.0;
  bounds[1] = -1.0;
  bounds[2] = 1.0;
  bounds[3] = -1.0;
  bounds[4] = 1.0;
  bounds[5] = -1.0;
}
//@}
//@{
/**
 * Are the bounds initialized?
 */
static BOOL AreBoundsInitialized(double bounds[6])
{
  if (bounds[1] - bounds[0] < 0.0) {
    return 0;
  }
  return 1;
}
//@}
inline float RadiansFromDegrees(float x)
{
  return x * 0.017453292f;
}
inline double RadiansFromDegrees(double x)
{
  return x * 0.017453292519943295;
}
inline float DegreesFromRadians(float x)
{
  return x * 57.2957795131f;
}
inline double DegreesFromRadians(double x)
{
  return x * 57.29577951308232;
}
inline bool IsPowerOfTwo(uint64 x)
{
  return ((x != 0) & ((x & (x - 1)) == 0));
}
// Credit goes to Peter Hart and William Lewis on comp.lang.python 1997
inline int NearestPowerOfTwo(int x)
{
  unsigned int z = ((x > 0) ? x - 1 : 0);
  z |= z >> 1;
  z |= z >> 2;
  z |= z >> 4;
  z |= z >> 8;
  z |= z >> 16;
  return (int)(z + 1);
}
// Modify the trunc() operation provided by (int)() to get floor(),
// Note that in C++ conditions evaluate to values of 1 or 0 (true or false).
inline int Floor(double x)
{
  int i = (int)(x);
  return i - (i > x);
}
// Modify the trunc() operation provided by (int)() to get ceil(),
// Note that in C++ conditions evaluate to values of 1 or 0 (true or false).
inline int Ceil(double x)
{
  int i = (int)(x);
  return i + (i < x);
}
template<class T>
inline T Min(const T& a, const T& b)
{
  return (a < b ? a : b);
}
template<class T>
inline T Max(const T& a, const T& b)
{
  return (a > b ? a : b);
}
void Add3(const float3 a, const float3 b, float3 c)
{
  c[0] = a[0] + b[0];
  c[1] = a[1] + b[1];
  c[2] = a[2] + b[2];
}
void Add4(const float3 a, const float3 b, float3 c)
{
  c[0] = a[0] + b[0];
  c[1] = a[1] + b[1];
  c[2] = a[2] + b[2];
  c[3] = a[3] + b[3];
}
void Scale2(const float2 a, double b, float2 c)
{
  c[0] = a[0] * b;
  c[1] = a[1] * b;
}
void Scale3(const float3 a, double b, float3 c)
{
  c[0] = a[0] * b;
  c[1] = a[1] * b;
  c[2] = a[2] * b;
}
void Scale4(const float4 a, double b, float4 c)
{
  c[0] = a[0] * b;
  c[1] = a[1] * b;
  c[2] = a[2] * b;
  c[3] = a[3] * b;
}
double SNorm2(const float2 x)
{
  return (x[0] * x[0] + x[1] * x[1]);
}
double SNorm3(const float3 x)
{
  return (x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
}
double SNorm4(const float4 x)
{
  return (x[0] * x[0] + x[1] * x[1] + x[2] * x[2] + x[3] * x[3]);
}
static double Norm2(const float2 x)
{
  return sqrt(x[0] * x[0] + x[1] * x[1]);
}
double Norm3(const float3 x)
{
  return sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
}
double Norm4(const float4 x)
{
  return sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2] + x[3] * x[3]);
}
inline bool Normalize2(const float2 v, float2 ret)
{
  double den = SNorm2(v);
  if (den != 0.0) {
    den = 1. / sqrt(den);
    ret[0] = v[0] * den;
    ret[1] = v[1] * den;
    return true;
  }
  return false;
}
inline bool Normalize3(float3 v, float2 ret)
{
  double den = SNorm3(v);
  if (den != 0.0) {
    den = 1. / sqrt(den);
    ret[0] = v[0] * den;
    ret[1] = v[1] * den;
    ret[2] = v[2] * den;
    return true;
  }
  return false;
}
inline double Dist2(const float2 a, const float2 b)
{
  double d0 = a[0] - b[0];
  double d1 = a[1] - b[1];
  return sqrt(d0 * d0 + d1 * d1);
}
inline double Dist3(const float3 a, const float2 b)
{
  double d0 = a[0] - b[0];
  double d1 = a[1] - b[1];
  double d2 = a[2] - b[2];
  return sqrt(d0 * d0 + d1 * d1 + d2 * d2);
}
inline double Dist4(const float4 a, const float4 b)
{
  double d0 = a[0] - b[0];
  double d1 = a[1] - b[1];
  double d2 = a[2] - b[2];
  double d3 = a[3] - b[3];
  return sqrt(d0 * d0 + d1 * d1 + d2 * d2 + d3 * d3);
}
inline bool Normalize4(float4 v, float2 ret)
{
  double den = SNorm4(v);
  if (den != 0.0) {
    den = 1. / sqrt(den);
    ret[0] = v[0] * den;
    ret[1] = v[1] * den;
    ret[2] = v[2] * den;
    ret[3] = v[3] * den;
    return true;
  }
  return false;
}
bool Normalize2(float2 v)
{
  return Normalize2(v, v);
}
bool Normalize3(float3 v)
{
  return Normalize3(v, v);
}
bool Normalize4(float4 v)
{
  return Normalize4(v, v);
}
inline float Determinant9(const float3 c1, const float3 c2, const float3 c3)
{
  return c1[0] * c2[1] * c3[2] + c2[0] * c3[1] * c1[2] + c3[0] * c1[1] * c2[2] -
      c1[0] * c3[1] * c2[2] - c2[0] * c1[1] * c3[2] - c3[0] * c2[1] * c1[2];
}
inline double Determinant9(const double c1[3], const double c2[3], const double c3[3])
{
  return c1[0] * c2[1] * c3[2] + c2[0] * c3[1] * c1[2] + c3[0] * c1[1] * c2[2] -
      c1[0] * c3[1] * c2[2] - c2[0] * c1[1] * c3[2] - c3[0] * c2[1] * c1[2];
}
inline double Determinant9(double a1, double a2, double a3,
    double b1, double b2, double b3,
    double c1, double c2, double c3)
{
  return (a1 * Determinant2x2(b2, b3, c2, c3)
      - b1 * Determinant2x2(a2, a3, c2, c3)
      + c1 * Determinant2x2(a2, a3, b2, b3));
}
inline float Distance2BetweenPoints(const float p1[3],
    const float p2[3])
{
  return ((p1[0] - p2[0]) * (p1[0] - p2[0])
      + (p1[1] - p2[1]) * (p1[1] - p2[1])
      + (p1[2] - p2[2]) * (p1[2] - p2[2]));
}
// Cross product of two 3-vectors. Result (a x b) is stored in c[3].
inline void Cross(const float a[3], const float b[3], float c[3])
{
  float Cx = a[1] * b[2] - a[2] * b[1];
  float Cy = a[2] * b[0] - a[0] * b[2];
  float Cz = a[0] * b[1] - a[1] * b[0];
  c[0] = Cx, c[1] = Cy, c[2] = Cz;
}
inline double Determinant9(const float A[9])
{
  return A[0 * 3 + 0] * A[1 * 3 + 1] * A[4] + A[1 * 3 + 0] * A[2 * 3 + 1] * A[0 * 3 + 2] +
      A[2 * 3 + 0] * A[0 * 3 + 1] * A[1 * 3 + 2] - A[0 * 3 + 0] * A[2 * 3 + 1] * A[1 * 3 + 2] -
      A[1 * 3 + 0] * A[0 * 3 + 1] * A[4] - A[2 * 3 + 0] * A[1 * 3 + 1] * A[0 * 3 + 2];
}
float Determinant9a(const float9 elements)
{
  return Determinant9(
      elements[0], elements[1], elements[2],
      elements[3], elements[4], elements[5],
      elements[6], elements[7], elements[8]);
}
template<class T>
inline T ClampValue(const T& value, const T& min, const T& max)
{
  assert("pre: valid_range" && min <= max);
  if (value < min) {
    return min;
  }
  if (value > max) {
    return max;
  }
  return value;
}
inline void ClampValue(float* value, const double range[2])
{
  if (value && range) {
    assert("pre: valid_range" && range[0] <= range[1]);
    if (*value < range[0]) {
      *value = range[0];
    }
    else if (*value > range[1]) {
      *value = range[1];
    }
  }
}
inline void ClampValue(
    double value, const double range[2], float* clamped_value)
{
  if (range && clamped_value) {
    assert("pre: valid_range" && range[0] <= range[1]);
    if (value < range[0]) {
      *clamped_value = range[0];
    }
    else if (value > range[1]) {
      *clamped_value = range[1];
    }
    else {
      *clamped_value = value;
    }
  }
}
// ---------------------------------------------------------------------------
inline double ClampAndNormalizeValue(double value, const double range[2])
{
  assert("pre: valid_range" && range[0] <= range[1]);
  double result;
  if (range[0] == range[1]) {
    result = 0.0;
  }
  else {
    // clamp
    if (value < range[0]) {
      result = range[0];
    }
    else {
      if (value > range[1]) {
        result = range[1];
      }
      else {
        result = value;
      }
    }
    // normalize
    result = (result - range[0]) / (range[1] - range[0]);
  }
  assert("post: valid_result" && result >= 0.0 && result <= 1.0);
  return result;
}
//
// Some useful macros and functions
//
// Return the lowest value "i" for which 2^i >= x
int CeilLog2(uint64 x)
{
  static const uint64 t[6] = {
    0xffffffff00000000ul,
    0x00000000ffff0000ul,
    0x000000000000ff00ul,
    0x00000000000000f0ul,
    0x000000000000000cul,
    0x0000000000000002ul
  };
  int j = 32;
  // if x is not a power of two, add 1 to final answer
  // (this is the "ceil" part of the computation)
  int y = (((x & (x - 1)) == 0) ? 0 : 1);
  // loop through the table (this unrolls nicely)
  for (int i = 0; i < 6; i++) {
    int k = (((x & t[i]) == 0) ? 0 : j);
    y += k;
    x >>= k;
    j >>= 1;
  }
  return y;
}
int64 Factorial(int N)
{
  static int64 MemoizeFactorial[24] = {0};
  if (N > 20) {
    printf("Factorial(%d) would overflow.", N);
    return _I64_MAX;
  }
  if (N == 0) {
    return 1;
  }
  if (MemoizeFactorial[N] != 0) {
    return MemoizeFactorial[N];
  }
  int64 r = Factorial(N - 1) * N;
  MemoizeFactorial[N] = r;
  return r;
}
// The number of combinations of n objects from a pool of m objects (m>n).
//
int64 Binomial(int m, int n)
{
  double r = 1;
  for (int i = 1; i <= n; ++i) {
    r *= (double)(m - i + 1) / i;
  }
  return static_cast<int64>(r);
}
// Start iterating over "m choose n" objects.
// This function returns an array of n integers, each from 0 to m-1.
// These integers represent the n items chosen from the set [0,m[.
//
int* BeginCombination(int m, int n)
{
  if (m < n) {
    return 0;
  }
  int* r = new int[ n ];
  for (int i = 0; i < n; ++i) {
    r[i] = i;
  }
  return r;
}
// Given \a m, \a n, and a valid \a combination of \a n integers in
// the range [0,m[, this function alters the integers into the next
// combination in a sequence of all combinations of \a n items from
// a pool of \a m.
// If the \a combination is the last item in the sequence on input,
// then \a combination is unaltered and 0 is returned.
// Otherwise, 1 is returned and \a combination is updated.
//
int NextCombination(int m, int n, int* r)
{
  int status = 0;
  for (int i = n - 1; i >= 0; --i) {
    if (r[i] < m - n + i) {
      int j = r[i] + 1;
      while (i < n) {
        r[i++] = j++;
      }
      status = 1;
      break;
    }
  }
  return status;
}

// Given a unit vector v1, find two other unit vectors v2 and v3 which
// which form an orthonormal set.
inline void Perpendiculars(const float v1[3], float v2[3], float v3[3], double theta)
{
  int dv1, dv2, dv3;
  double v1sq = v1[0] * v1[0];
  double v2sq = v1[1] * v1[1];
  double v3sq = v1[2] * v1[2];
  double r = sqrt(v1sq + v2sq + v3sq);
  // transpose the vector to avoid divide-by-zero error
  if (v1sq > v2sq && v1sq > v3sq) {
    dv1 = 0;
    dv2 = 1;
    dv3 = 2;
  }
  else if (v2sq > v3sq) {
    dv1 = 1;
    dv2 = 2;
    dv3 = 0;
  }
  else {
    dv1 = 2;
    dv2 = 0;
    dv3 = 1;
  }
  double a = v1[dv1] / r;
  double b = v1[dv2] / r;
  double c = v1[dv3] / r;
  double tmp = sqrt(a * a + c * c);
  if (theta != 0) {
    double sintheta = sin(theta);
    double costheta = cos(theta);
    if (v2) {
      v2[dv1] = (c * costheta - a * b * sintheta) / tmp;
      v2[dv2] = sintheta * tmp;
      v2[dv3] = (-a * costheta - b * c * sintheta) / tmp;
    }
    if (v3) {
      v3[dv1] = (-c * sintheta - a * b * costheta) / tmp;
      v3[dv2] = costheta * tmp;
      v3[dv3] = (a * sintheta - b * c * costheta) / tmp;
    }
  }
  else {
    if (v2) {
      v2[dv1] = c / tmp;
      v2[dv2] = 0;
      v2[dv3] = -a / tmp;
    }
    if (v3) {
      v3[dv1] = -a * b / tmp;
      v3[dv2] = tmp;
      v3[dv3] = -b * c / tmp;
    }
  }
}
// Factor linear equations Ax = b using LU decompostion A = LU where L is
// lower triangular matrix and U is upper triangular matrix. Input is
// square matrix A, integer array of pivot indices index[0->n-1], and size
// of square matrix n. Output factorization LU is in matrix A. If error is
// found, method returns 0.
int LUFactorLinearSystem(float* A, int ldA, int* index, int size)
{
  float scratch[10];
  float* scale = (size < 10 ? scratch : new float[size]);
  int i, j, k;
  int maxI = 0;
  double largest, temp1, temp2, sum;
  //
  // Loop over rows to get implicit scaling information
  //
  for (i = 0; i < size; i++) {
    for (largest = 0.0, j = 0; j < size; j++) {
      if ((temp2 = fabs(A[i * ldA + j])) > largest) {
        largest = temp2;
      }
    }
    if (largest == 0.0) {
      printf("Unable to factor linear system");
      return 0;
    }
    scale[i] = 1.0 / largest;
  }
  //
  // Loop over all columns using Crout's method
  //
  for (j = 0; j < size; j++) {
    for (i = 0; i < j; i++) {
      sum = A[i * ldA + j];
      for (k = 0; k < i; k++) {
        sum -= A[i * ldA + k] * A[k * ldA + j];
      }
      A[i * ldA + j] = sum;
    }
    //
    // Begin search for largest pivot element
    //
    for (largest = 0.0, i = j; i < size; i++) {
      sum = A[i * ldA + j];
      for (k = 0; k < j; k++) {
        sum -= A[i * ldA + k] * A[k * ldA + j];
      }
      A[i * ldA + j] = sum;
      if ((temp1 = scale[i] * fabs(sum)) >= largest) {
        largest = temp1;
        maxI = i;
      }
    }
    //
    // Check for row interchange
    //
    if (j != maxI) {
      for (k = 0; k < size; k++) {
        temp1 = A[maxI * ldA + k];
        A[maxI * ldA + k] = A[j * ldA + k];
        A[j * ldA + k] = temp1;
      }
      scale[maxI] = scale[j];
    }
    //
    // Divide by pivot element and perform elimination
    //
    index[j] = maxI;
    if (fabs(A[j * ldA + j]) <= VTK_SMALL_NUMBER) {
      printf("Unable to factor linear system");
      return 0;
    }
    if (j != (size - 1)) {
      temp1 = 1.0 / A[j * ldA + j];
      for (i = j + 1; i < size; i++) {
        A[i * ldA + j] *= temp1;
      }
    }
  }
  if (size >= 10) {
    delete [] scale;
  }
  return 1;
}
// Solve linear equations Ax = b using LU decompostion A = LU where L is
// lower triangular matrix and U is upper triangular matrix. Input is
// factored matrix A=LU, integer array of pivot indices index[0->n-1],
// load vector x[0->n-1], and size of square matrix n. Note that A=LU and
// index[] are generated from method LUFactorLinearSystem). Also, solution
// vector is written directly over input load vector.
void LUSolveLinearSystem(float* A, int ldA, int* index, float* x, int size)
{
  int i, j, ii, idx;
  double sum;
  //
  // Proceed with forward and backsubstitution for L and U
  // matrices. First, forward substitution.
  //
  for (ii = -1, i = 0; i < size; i++) {
    idx = index[i];
    sum = x[idx];
    x[idx] = x[i];
    if (ii >= 0) {
      for (j = ii; j <= (i - 1); j++) {
        sum -= A[i * ldA + j] * x[j];
      }
    }
    else if (sum != 0.0) {
      ii = i;
    }
    x[i] = sum;
  }
  //
  // Now, back substitution
  //
  for (i = size - 1; i >= 0; i--) {
    sum = x[i];
    for (j = i + 1; j < size; j++) {
      sum -= A[i * ldA + j] * x[j];
    }
    x[i] = sum / A[i * ldA + i];
  }
}
// Solve linear equations Ax = b using Crout's method. Input is square matrix A
// and load vector x. Solution x is written over load vector. The dimension of
// the matrix is specified in size. If error is found, method returns a 0.
int SolveLinearSystem(float* A, int ldA, float* x, int size)
{
  // if we solving something simple, just solve it
  //
  if (size == 2) {
    double det, y[2];
    det = Determinant2x2(A[0 * ldA + 0], A[0 * ldA + 1], A[1 * ldA + 0], A[1 * ldA + 1]);
    static const double eps = 256 * DBL_EPSILON;
    if (fabs(det) < eps) {
      // Unable to solve linear system
      return 0;
    }
    y[0] = (A[1 * ldA + 1] * x[0] - A[0 * ldA + 1] * x[1]) / det;
    y[1] = (-A[1 * ldA + 0] * x[0] + A[0 * ldA + 0] * x[1]) / det;
    x[0] = y[0];
    x[1] = y[1];
    return 1;
  }
  else if (size == 1) {
    if (A[0 * ldA + 0] == 0.0) {
      // Unable to solve linear system
      return 0;
    }
    x[0] /= A[0 * ldA + 0];
    return 1;
  }
  //
  // System of equations is not trivial, use Crout's method
  //
  // Check on allocation of working vectors
  //
  int* index, scratch[10];
  index = (size < 10 ? scratch : new int[size]);
  //
  // Factor and solve matrix
  //
  if (LUFactorLinearSystem(A, ldA, index, size) == 0) {
    return 0;
  }
  LUSolveLinearSystem(A, ldA, index, x, size);
  if (size >= 10) {
    delete [] index;
  }
  return 1;
}
// Factor linear equations Ax = b using LU decompostion A = LU where L is
// lower triangular matrix and U is upper triangular matrix. Input is
// square matrix A, integer array of pivot indices index[0->n-1], and size
// of square matrix n. Output factorization LU is in matrix A. If error is
// found, method returns 0.
//------------------------------------------------------------------
// For thread safe, temporary memory array tmpSize of length size
// must be passed in.
int LUFactorLinearSystem(float* A, int ldA, int* index, int size,
    float* tmpSize)
{
  int i, j, k;
  int maxI = 0;
  double largest, temp1, temp2, sum;
  //
  // Loop over rows to get implicit scaling information
  //
  for (i = 0; i < size; i++) {
    for (largest = 0.0, j = 0; j < size; j++) {
      if ((temp2 = fabs(A[i * ldA + j])) > largest) {
        largest = temp2;
      }
    }
    if (largest == 0.0) {
      printf("Unable to factor linear system");
      return 0;
    }
    tmpSize[i] = 1.0 / largest;
  }
  //
  // Loop over all columns using Crout's method
  //
  for (j = 0; j < size; j++) {
    for (i = 0; i < j; i++) {
      sum = A[i * ldA + j];
      for (k = 0; k < i; k++) {
        sum -= A[i * ldA + k] * A[k * ldA + j];
      }
      A[i * ldA + j] = sum;
    }
    //
    // Begin search for largest pivot element
    //
    for (largest = 0.0, i = j; i < size; i++) {
      sum = A[i * ldA + j];
      for (k = 0; k < j; k++) {
        sum -= A[i * ldA + k] * A[k * ldA + j];
      }
      A[i * ldA + j] = sum;
      if ((temp1 = tmpSize[i] * fabs(sum)) >= largest) {
        largest = temp1;
        maxI = i;
      }
    }
    //
    // Check for row interchange
    //
    if (j != maxI) {
      for (k = 0; k < size; k++) {
        temp1 = A[maxI * ldA + k];
        A[maxI * ldA + k] = A[j * ldA + k];
        A[j * ldA + k] = temp1;
      }
      tmpSize[maxI] = tmpSize[j];
    }
    //
    // Divide by pivot element and perform elimination
    //
    index[j] = maxI;
    if (fabs(A[j * ldA + j]) <= VTK_SMALL_NUMBER) {
      printf("Unable to factor linear system");
      return 0;
    }
    if (j != (size - 1)) {
      temp1 = 1.0 / A[j * ldA + j];
      for (i = j + 1; i < size; i++) {
        A[i * ldA + j] *= temp1;
      }
    }
  }
  return 1;
}
// Invert input square matrix A into matrix AI. Note that A is modified during
// the inversion. The size variable is the dimension of the matrix. Returns 0
// if inverse not computed.
// -----------------------
// For thread safe behavior, temporary arrays tmp1SIze and tmp2Size
// of length size must be passsed in.
int InvertMatrix(float* A, int ldA, float* AI, int ldAI, int size,
    int* tmp1Size, float* tmp2Size)
{
  int i, j;
  //
  // Factor matrix; then begin solving for inverse one column at a time.
  // Note: tmp1Size returned value is used later, tmp2Size is just working
  // memory whose values are not used in LUSolveLinearSystem
  //
  if (LUFactorLinearSystem(A, ldA, tmp1Size, size, tmp2Size) == 0) {
    return 0;
  }
  for (j = 0; j < size; j++) {
    for (i = 0; i < size; i++) {
      tmp2Size[i] = 0.0;
    }
    tmp2Size[j] = 1.0;
    LUSolveLinearSystem(A, ldA, tmp1Size, tmp2Size, size);
    for (i = 0; i < size; i++) {
      AI[i * ldAI + j] = tmp2Size[i];
    }
  }
  return 1;
}
// Invert input square matrix A into matrix AI. Note that A is modified during
// the inversion. The size variable is the dimension of the matrix. Returns 0
// if inverse not computed.
int InvertMatrix(float* A, int ldA, float* AI, int ldAI, int size)
{
  int* index, iScratch[10];
  float* column, dScratch[10];
  // Check on allocation of working vectors
  //
  if (size <= 10) {
    index = iScratch;
    column = dScratch;
  }
  else {
    index = new int[size];
    column = new float[size];
  }
  int retVal = InvertMatrix(A, ldA, AI, ldAI, size, index, column);
  if (size > 10) {
    delete [] index;
    delete [] column;
  }
  return retVal;
}
#undef VTK_SMALL_NUMBER
#define VTK_ROTATE(a,i,j,k,l) g=a[i*lda+j];h=a[k*lda+l];a[i*lda+j]=g-s*(h+g*tau); a[k*lda+l]=h+s*(g-h*tau)
#define VTK_MAX_ROTATIONS 20
//#undef VTK_MAX_ROTATIONS
//#define VTK_MAX_ROTATIONS 50
// Jacobi iteration for the solution of eigenvectors/eigenvalues of a nxn
// real symmetric matrix. Square nxn matrix a; size of matrix in n;
// output eigenvalues in w; and output eigenvectors in v. Resulting
// eigenvalues/vectors are sorted in decreasing order; eigenvectors are
// normalized.
int JacobiN(float* a, int lda, int n, float* w, float* v, int ldv)
{
  int i, j, k, iq, ip, numPos;
  double tresh, theta, tau, t, sm, s, h, g, c, tmp;
  float bspace[4], zspace[4];
  float* b = bspace;
  float* z = zspace;
  // only allocate memory if the matrix is large
  if (n > 4) {
    b = new float[n];
    z = new float[n];
  }
  // initialize
  for (ip = 0; ip < n; ip++) {
    for (iq = 0; iq < n; iq++) {
      v[ip * ldv + iq] = 0.0;
    }
    v[ip * ldv + ip] = 1.0;
  }
  for (ip = 0; ip < n; ip++) {
    b[ip] = w[ip] = a[ip * lda + ip];
    z[ip] = 0.0;
  }
  // begin rotation sequence
  for (i = 0; i < VTK_MAX_ROTATIONS; i++) {
    sm = 0.0;
    for (ip = 0; ip < n - 1; ip++) {
      for (iq = ip + 1; iq < n; iq++) {
        sm += fabs(a[ip * lda + iq]);
      }
    }
    if (sm == 0.0) {
      break;
    }
    if (i < 3) { // first 3 sweeps
      tresh = 0.2 * sm / (n * n);
    }
    else {
      tresh = 0.0;
    }
    for (ip = 0; ip < n - 1; ip++) {
      for (iq = ip + 1; iq < n; iq++) {
        g = 100.0 * fabs(a[ip * lda + iq]);
        // after 4 sweeps
        if (i > 3 && (fabs(w[ip]) + g) == fabs(w[ip])
            && (fabs(w[iq]) + g) == fabs(w[iq])) {
          a[ip * lda + iq] = 0.0;
        }
        else if (fabs(a[ip * lda + iq]) > tresh) {
          h = w[iq] - w[ip];
          if ((fabs(h) + g) == fabs(h)) {
            t = (a[ip * lda + iq]) / h;
          }
          else {
            theta = 0.5 * h / (a[ip * lda + iq]);
            t = 1.0 / (fabs(theta) + sqrt(1.0 + theta * theta));
            if (theta < 0.0) {
              t = -t;
            }
          }
          c = 1.0 / sqrt(1 + t * t);
          s = t * c;
          tau = s / (1.0 + c);
          h = t * a[ip * lda + iq];
          z[ip] -= h;
          z[iq] += h;
          w[ip] -= h;
          w[iq] += h;
          a[ip * lda + iq] = 0.0;
          // ip already shifted left by 1 unit
          for (j = 0; j <= ip - 1; j++) {
            VTK_ROTATE(a, j, ip, j, iq);
          }
          // ip and iq already shifted left by 1 unit
          for (j = ip + 1; j <= iq - 1; j++) {
            VTK_ROTATE(a, ip, j, j, iq);
          }
          // iq already shifted left by 1 unit
          for (j = iq + 1; j < n; j++) {
            VTK_ROTATE(a, ip, j, iq, j);
          }
          for (j = 0; j < n; j++) {
            VTK_ROTATE(v, j, ip, j, iq);
          }
        }
      }
    }
    for (ip = 0; ip < n; ip++) {
      b[ip] += z[ip];
      w[ip] = b[ip];
      z[ip] = 0.0;
    }
  }
  //// this is NEVER called
  if (i >= VTK_MAX_ROTATIONS) {
    printf(
        "Jacobi: Error extracting eigenfunctions");
    return 0;
  }
  // sort eigenfunctions these changes do not affect accuracy
  for (j = 0; j < n - 1; j++) { // boundary incorrect
    k = j;
    tmp = w[k];
    for (i = j + 1; i < n; i++) { // boundary incorrect, shifted already
      if (w[i] >= tmp) { // why exchange if same?
        k = i;
        tmp = w[k];
      }
    }
    if (k != j) {
      w[k] = w[j];
      w[j] = tmp;
      for (i = 0; i < n; i++) {
        tmp = v[i * ldv + j];
        v[i * ldv + j] = v[i * ldv + k];
        v[i * ldv + k] = tmp;
      }
    }
  }
  // insure eigenvector consistency (i.e., Jacobi can compute vectors that
  // are negative of one another (.707,.707,0) and (-.707,-.707,0). This can
  // reek havoc in hyperstreamline/other stuff. We will select the most
  // positive eigenvector.
  int ceil_half_n = (n >> 1) + (n & 1);
  for (j = 0; j < n; j++) {
    for (numPos = 0, i = 0; i < n; i++) {
      if (v[i * ldv + j] >= 0.0) {
        numPos++;
      }
    }
    // if ( numPos < ceil(double(n)/double(2.0)) )
    if (numPos < ceil_half_n) {
      for (i = 0; i < n; i++) {
        v[i * ldv + j] *= -1.0;
      }
    }
  }
  if (n > 4) {
    delete [] b;
    delete [] z;
  }
  return 1;
}
#undef VTK_ROTATE
#undef VTK_MAX_ROTATIONS
// Estimate the condition number of a LU factored matrix. Used to judge the
// accuracy of the solution. The matrix A must have been previously factored
// using the method LUFactorLinearSystem. The condition number is the ratio
// of the infinity matrix norm (i.e., maximum value of matrix component)
// divided by the minimum diagonal value. (This works for triangular matrices
// only: see Conte and de Boor, Elementary Numerical Analysis.)
double EstimateMatrixCondition(float* A, int ldA, int size)
{
  int i;
  int j;
  double min = FLT_MAX, max = (-FLT_MAX);
  // find the maximum value
  for (i = 0; i < size; i++) {
    for (j = i; j < size; j++) {
      if (fabs(A[i * ldA + j]) > max) {
        max = fabs(A[i * ldA + j]);
      }
    }
  }
  // find the minimum diagonal value
  for (i = 0; i < size; i++) {
    if (fabs(A[i * ldA + i]) < min) {
      min = fabs(A[i * ldA + i]);
    }
  }
  if (min == 0.0) {
    return FLT_MAX;
  }
  else {
    return (max / min);
  }
}
// Solves for the least squares best fit matrix for the homogeneous equation X'M' = 0'.
// Uses the method described on pages 40-41 of Computer Vision by
// Forsyth and Ponce, which is that the solution is the eigenvector
// associated with the minimum eigenvalue of T(X)X, where T(X) is the
// transpose of X.
// The inputs and output are transposed matrices.
// Dimensions: X' is numberOfSamples by xOrder,
// M' dimension is xOrder by 1.
// M' should be pre-allocated. All matrices are row major. The resultant
// matrix M' should be pre-multiplied to X' to get 0', or transposed and
// then post multiplied to X to get 0
int SolveHomogeneousLeastSquares(int numberOfSamples, float* xt, int ldxt, int xOrder, float* mt, int ldmt)
{
  // check dimensional consistency
  if (numberOfSamples < xOrder) {
    printf("Insufficient number of samples. Underdetermined.");
    return 0;
  }
  int i, j, k;
  // set up intermediate variables
  // Allocate matrix to hold X times transpose of X
  float* XXt = new float [xOrder * xOrder]; // size x by x
  // Allocate the array of eigenvalues and eigenvectors
  float* eigenvals = new float[xOrder];
  float* eigenvecs = new float [xOrder * xOrder];
  int ldXXt = xOrder;
  int ldeigenvecs = xOrder;
  // Clear the upper triangular region (and btw, allocate the eigenvecs as well)
  for (i = 0; i < xOrder * xOrder; i++) {
    XXt[i * j] = 0.0f;
  }
  // Calculate XXt upper half only, due to symmetry
  for (k = 0; k < numberOfSamples; k++) {
    for (i = 0; i < xOrder; i++) {
      for (j = i; j < xOrder; j++) {
        XXt[i * ldXXt + j] += xt[k * ldxt + i] * xt[k * ldxt + j];
      }
    }
  }
  // now fill in the lower half of the XXt matrix
  for (i = 0; i < xOrder; i++) {
    for (j = 0; j < i; j++) {
      XXt[i * ldXXt + j] = XXt[j * ldXXt + i];
    }
  }
  // Compute the eigenvectors and eigenvalues
  JacobiN(XXt, ldXXt, xOrder, eigenvals, eigenvecs, ldeigenvecs);
  // Smallest eigenval is at the end of the list (xOrder-1), and solution is
  // corresponding eigenvec.
  for (i = 0; i < xOrder; i++) {
    mt[i * ldmt + 0] = eigenvecs[i * ldeigenvecs + xOrder - 1];
  }
  // Clean up:
  delete [] XXt;
  delete [] eigenvecs;
  delete [] eigenvals;
  return 1;
}
#define VTK_SMALL_NUMBER 1.0e-12
// Solves for the least squares best fit matrix for the equation X'M' = Y'.
// Uses pseudoinverse to get the ordinary least squares.
// The inputs and output are transposed matrices.
// Dimensions: X' is numberOfSamples by xOrder,
// Y' is numberOfSamples by yOrder,
// M' dimension is xOrder by yOrder.
// M' should be pre-allocated. All matrices are row major. The resultant
// matrix M' should be pre-multiplied to X' to get Y', or transposed and
// then post multiplied to X to get Y
// By default, this method checks for the homogeneous condition where Y==0, and
// if so, invokes SolveHomogeneousLeastSquares. For better performance when
// the system is known not to be homogeneous, invoke with checkHomogeneous=0.
int SolveLeastSquares(int numberOfSamples, float* xt, int ldxt, int xOrder,
    float* yt, int ldyt, int yOrder, float* mt, int ldmt, int checkHomogeneous)
{
  // check dimensional consistency
  if ((numberOfSamples < xOrder) || (numberOfSamples < yOrder)) {
    printf("Insufficient number of samples. Underdetermined.");
    return 0;
  }
  int i, j, k;
  int someHomogeneous = 0;
  int allHomogeneous = 1;
  float* hmt = NULL;
  int homogRC = 0;
  int* homogenFlags = new int[yOrder];
  int successFlag;
  // Ok, first init some flags check and see if all the systems are homogeneous
  if (checkHomogeneous) {
    // If Y' is zero, it's a homogeneous system and can't be solved via
    // the pseudoinverse method. Detect this case, warn the user, and
    // invoke SolveHomogeneousLeastSquares instead. Note that it doesn't
    // really make much sense for yOrder to be greater than one in this case,
    // since that's just yOrder occurrences of a 0 vector on the RHS, but
    // we allow it anyway. N
    // Initialize homogeneous flags on a per-right-hand-side basis
    for (j = 0; j < yOrder; j++) {
      homogenFlags[j] = 1;
    }
    for (i = 0; i < numberOfSamples; i++) {
      for (j = 0; j < yOrder; j++) {
        if (fabs(yt[i * ldyt + j]) > VTK_SMALL_NUMBER) {
          allHomogeneous = 0;
          homogenFlags[j] = 0;
        }
      }
    }
    // If we've got one system, and it's homogeneous, do it and bail out quickly.
    if (allHomogeneous && yOrder == 1) {
      printf("Detected homogeneous system (Y=0), calling SolveHomogeneousLeastSquares()");
      delete [] homogenFlags;
      return SolveHomogeneousLeastSquares(numberOfSamples, xt, ldxt, xOrder, mt, ldmt);
    }
    // Ok, we've got more than one system of equations.
    // Figure out if we need to calculate the homogeneous equation solution for
    // any of them.
    if (allHomogeneous) {
      someHomogeneous = 1;
    }
    else {
      for (j = 0; j < yOrder; j++) {
        if (homogenFlags[j]) {
          someHomogeneous = 1;
        }
      }
    }
  }
  int ldhmt = 1;
  // If necessary, solve the homogeneous problem
  if (someHomogeneous) {
    // hmt is the homogeneous equation version of mt, the general solution.
    hmt = new float [xOrder];
    ldhmt = 1;
    // Ok, solve the homogeneous problem
    homogRC = SolveHomogeneousLeastSquares(numberOfSamples, xt, ldxt, xOrder, hmt, ldhmt);
  }
  // set up intermediate variables
  float* XXt = new float [xOrder * xOrder]; // size x by x
  float* XXtI = new float [xOrder * xOrder]; // size x by x
  float* XYt = new float [xOrder * xOrder]; // size x by y
  int ldXXt = xOrder;
  int ldXXtI = xOrder;
  int ldXYt = xOrder;
  for (i = 0; i < xOrder; i++) {
    for (j = 0; j < xOrder; j++) {
      XXt[i * ldXXt + j] = 0.0f;
      XXtI[i * ldXXtI + j] = 0.0f;
    }
    for (j = 0; j < yOrder; j++) {
      XYt[i * ldXYt + j] = 0.0;
    }
  }
  // first find the pseudoinverse matrix
  for (k = 0; k < numberOfSamples; k++) {
    for (i = 0; i < xOrder; i++) {
      // first calculate the XXt matrix, only do the upper half (symmetrical)
      for (j = i; j < xOrder; j++) {
        XXt[i * ldXXt + j] += xt[k * ldxt + i] * xt[k * ldxt + j];
      }
      // now calculate the XYt matrix
      for (j = 0; j < yOrder; j++) {
        XYt[i * ldXYt + j] += xt[k * ldxt + i] * yt[k * ldyt + j];
      }
    }
  }
  // now fill in the lower half of the XXt matrix
  for (i = 0; i < xOrder; i++) {
    for (j = 0; j < i; j++) {
      XXt[i * ldXXt + j] = XXt[j * ldXXt + i];
    }
  }
  successFlag = InvertMatrix(XXt, ldXXt, XXtI, ldXXtI, xOrder);
  // next get the inverse of XXt
  if (successFlag) {
    for (i = 0; i < xOrder; i++) {
      for (j = 0; j < yOrder; j++) {
        mt[i * ldmt + j] = 0.0;
        for (k = 0; k < xOrder; k++) {
          mt[i * ldmt + j] += XXtI[i * ldXXtI + k] * XYt[k * ldXYt + j];
        }
      }
    }
  }
  // Fix up any of the solutions that correspond to the homogeneous equation
  // problem.
  if (someHomogeneous) {
    for (j = 0; j < yOrder; j++) {
      if (homogenFlags[j]) {
        // Fix this one
        for (i = 0; i < xOrder; i++) {
          mt[i * ldmt + j] = hmt[i * ldhmt + 0];
        }
      }
    }
    delete [] hmt;
  }
  delete [] XXt;
  delete [] XXtI;
  delete [] XYt;
  delete [] homogenFlags;
  if (someHomogeneous) {
    return homogRC && successFlag;
  }
  else {
    return successFlag;
  }
}
#undef VTK_SMALL_NUMBER
// All of the following methods are for dealing with 3x3 matrices
// helper function, swap two 3-vectors
inline void SwapVectors3(float v1[3], float v2[3])
{
  for (int i = 0; i < 3; i++) {
    float tmp = v1[i];
    v1[i] = v2[i];
    v2[i] = tmp;
  }
}
// Unrolled LU factorization of a 3x3 matrix with pivoting.
inline void LUFactor9(float A[9], int index[3])
{
  int i, maxI;
  double tmp, largest;
  float scale[3];
  // Loop over rows to get implicit scaling information
  for (i = 0; i < 3; i++) {
    largest = fabs(A[i * 3 + 0]);
    if ((tmp = fabs(A[i * 3 + 1])) > largest) {
      largest = tmp;
    }
    if ((tmp = fabs(A[i * 3 + 2])) > largest) {
      largest = tmp;
    }
    scale[i] = (float)(1.0) / largest;
  }
  // Loop over all columns using Crout's method
  // first column
  largest = scale[0] * fabs(A[0 * 3 + 0]);
  maxI = 0;
  if ((tmp = scale[1] * fabs(A[1 * 3 + 0])) >= largest) {
    largest = tmp;
    maxI = 1;
  }
  if ((tmp = scale[2] * fabs(A[2 * 3 + 0])) >= largest) {
    maxI = 2;
  }
  if (maxI != 0) {
    SwapVectors3(A + maxI * 3, A);
    scale[maxI] = scale[0];
  }
  index[0] = maxI;
  A[1 * 3 + 0] /= A[0 * 3 + 0];
  A[2 * 3 + 0] /= A[0 * 3 + 0];
  // second column
  A[1 * 3 + 1] -= A[1 * 3 + 0] * A[0 * 3 + 1];
  A[2 * 3 + 1] -= A[2 * 3 + 0] * A[0 * 3 + 1];
  largest = scale[1] * fabs(A[1 * 3 + 1]);
  maxI = 1;
  if ((tmp = scale[2] * fabs(A[2 * 3 + 1])) >= largest) {
    maxI = 2;
    SwapVectors3(A + 2 * 3, A + 1 * 3);
    scale[2] = scale[1];
  }
  index[1] = maxI;
  A[2 * 3 + 1] /= A[1 * 3 + 1];
  // third column
  A[1 * 3 + 2] -= A[1 * 3 + 0] * A[0 * 3 + 2];
  A[4] -= A[2 * 3 + 0] * A[0 * 3 + 2] + A[2 * 3 + 1] * A[1 * 3 + 2];
  index[2] = 2;
}
// Backsubstitution with an LU-decomposed matrix.
inline void LUSolve9(const float A[9], const int index[3], float x[3])
{
  double sum;
  // forward substitution
  sum = x[index[0]];
  x[index[0]] = x[0];
  x[0] = sum;
  sum = x[index[1]];
  x[index[1]] = x[1];
  x[1] = sum - A[1 * 3 + 0] * x[0];
  sum = x[index[2]];
  x[index[2]] = x[2];
  x[2] = sum - A[2 * 3 + 0] * x[0] - A[2 * 3 + 1] * x[1];
  // back substitution
  x[2] = x[2] / A[4];
  x[1] = (x[1] - A[1 * 3 + 2] * x[2]) / A[1 * 3 + 1];
  x[0] = (x[0] - A[0 * 3 + 1] * x[1] - A[0 * 3 + 2] * x[2]) / A[0 * 3 + 0];
}
// this method solves Ay = x for y
inline void LinearSolve9(const float9 A, const float3 x, float3 y)
{
  double a1 = A[0 * 3 + 0], b1 = A[0 * 3 + 1], c1 = A[0 * 3 + 2];
  double a2 = A[1 * 3 + 0], b2 = A[1 * 3 + 1], c2 = A[1 * 3 + 2];
  double a3 = A[2 * 3 + 0], b3 = A[2 * 3 + 1], c3 = A[4];
  // Compute the adjoint
  double d1 = Determinant2x2(b2, b3, c2, c3);
  double d2 = - Determinant2x2(a2, a3, c2, c3);
  double d3 = Determinant2x2(a2, a3, b2, b3);
  double e1 = - Determinant2x2(b1, b3, c1, c3);
  double e2 = Determinant2x2(a1, a3, c1, c3);
  double e3 = - Determinant2x2(a1, a3, b1, b3);
  double f1 = Determinant2x2(b1, b2, c1, c2);
  double f2 = - Determinant2x2(a1, a2, c1, c2);
  double f3 = Determinant2x2(a1, a2, b1, b2);
  // Compute the determinant
  double det = a1 * d1 + b1 * d2 + c1 * d3;
  // Multiply by the adjoint
  double v1 = d1 * x[0] + e1 * x[1] + f1 * x[2];
  double v2 = d2 * x[0] + e2 * x[1] + f2 * x[2];
  double v3 = d3 * x[0] + e3 * x[1] + f3 * x[2];
  // Divide by the determinant
  y[0] = v1 / det;
  y[1] = v2 / det;
  y[2] = v3 / det;
}
inline void MultiplyVector9(const float A[9], const float v[3], float u[3])
{
  float x = A[0 * 3 + 0] * v[0] + A[0 * 3 + 1] * v[1] + A[0 * 3 + 2] * v[2];
  float y = A[1 * 3 + 0] * v[0] + A[1 * 3 + 1] * v[1] + A[1 * 3 + 2] * v[2];
  float z = A[2 * 3 + 0] * v[0] + A[2 * 3 + 1] * v[1] + A[4] * v[2];
  u[0] = x;
  u[1] = y;
  u[2] = z;
}
inline void MultiplyMatrix9(const float A[9], const float B[9], float C[9])
{
  float D[9];
  for (int i = 0; i < 3; i++) {
    D[0 * 3 + i] = A[0 * 3 + 0] * B[0 * 3 + i] + A[0 * 3 + 1] * B[1 * 3 + i] + A[0 * 3 + 2] * B[2 * 3 + i];
    D[1 * 3 + i] = A[1 * 3 + 0] * B[0 * 3 + i] + A[1 * 3 + 1] * B[1 * 3 + i] + A[1 * 3 + 2] * B[2 * 3 + i];
    D[2 * 3 + i] = A[2 * 3 + 0] * B[0 * 3 + i] + A[2 * 3 + 1] * B[1 * 3 + i] + A[2 * 3 + 2] * B[2 * 3 + i];
  }
  for (int j = 0; j < 3; j++) {
    C[j * 3 + 0] = D[j * 3 + 0];
    C[j * 3 + 1] = D[j * 3 + 1];
    C[j * 3 + 2] = D[j * 3 + 2];
  }
}
void MultiplyMatrix(float* A, int ldA, float* B, int ldB,
    unsigned int rowA, unsigned int colA,
    unsigned int rowB, unsigned int colB,
    float* C, int ldC)
{
  // we need colA == rowB
  if (colA != rowB) {
    printf(
        "Number of columns of A must match number of rows of B.");
  }
  // output matrix is rowA*colB
  // output row
  for (unsigned int i = 0; i < rowA; i++) {
    // output col
    for (unsigned int j = 0; j < colB; j++) {
      C[i * ldC + j] = 0;
      // sum for this point
      for (unsigned int k = 0; k < colA; k++) {
        C[i * ldC + j] += A[i * ldA + k] * B[k * ldB + j];
      }
    }
  }
}
inline void Transpose9(const float A[9], float AT[9])
{
  float tmp;
  tmp = A[1 * 3 + 0];
  AT[1 * 3 + 0] = A[0 * 3 + 1];
  AT[0 * 3 + 1] = tmp;
  tmp = A[2 * 3 + 0];
  AT[2 * 3 + 0] = A[0 * 3 + 2];
  AT[0 * 3 + 2] = tmp;
  tmp = A[2 * 3 + 1];
  AT[2 * 3 + 1] = A[1 * 3 + 2];
  AT[1 * 3 + 2] = tmp;
  AT[0 * 3 + 0] = A[0 * 3 + 0];
  AT[1 * 3 + 1] = A[1 * 3 + 1];
  AT[2 * 3 + 2] = A[2 * 3 + 2];
}
void Transpose33(const float33 A, float33 AT)
{
  Transpose9(*A, *AT);
}
inline void Invert9(const float A[9], float AI[9])
{
  double a1 = A[0 * 3 + 0], b1 = A[0 * 3 + 1], c1 = A[0 * 3 + 2];
  double a2 = A[1 * 3 + 0], b2 = A[1 * 3 + 1], c2 = A[1 * 3 + 2];
  double a3 = A[2 * 3 + 0], b3 = A[2 * 3 + 1], c3 = A[2 * 3 + 2];
  // Compute the adjoint
  double d1 = Determinant2x2(b2, b3, c2, c3);
  double d2 = - Determinant2x2(a2, a3, c2, c3);
  double d3 = Determinant2x2(a2, a3, b2, b3);
  double e1 = - Determinant2x2(b1, b3, c1, c3);
  double e2 = Determinant2x2(a1, a3, c1, c3);
  double e3 = - Determinant2x2(a1, a3, b1, b3);
  double f1 = Determinant2x2(b1, b2, c1, c2);
  double f2 = - Determinant2x2(a1, a2, c1, c2);
  double f3 = Determinant2x2(a1, a2, b1, b2);
  // Divide by the determinant
  double det = a1 * d1 + b1 * d2 + c1 * d3;
  AI[0 * 3 + 0] = d1 / det;
  AI[1 * 3 + 0] = d2 / det;
  AI[2 * 3 + 0] = d3 / det;
  AI[0 * 3 + 1] = e1 / det;
  AI[1 * 3 + 1] = e2 / det;
  AI[2 * 3 + 1] = e3 / det;
  AI[0 * 3 + 2] = f1 / det;
  AI[1 * 3 + 2] = f2 / det;
  AI[2 * 3 + 2] = f3 / det;
}
void Invert33(const float33 A, float33 AI)
{
  Invert9(*A, *AI);
}
void Invert33(float33 A)
{
  Invert9(*A, *A);
}
void Identity9(float elements[9])
{
  elements[0] = elements[4] = elements[8] = 1.0;
  elements[1] = elements[2] = elements[3] = elements[5] =
      elements[6] = elements[7] = 0.0;
}
void Identity33(float33 A)
{
  Identity9(*A);
}
inline void QuaternionToMatrix9(float quat[4], float A[9])
{
  double ww = quat[0] * quat[0];
  double wx = quat[0] * quat[1];
  double wy = quat[0] * quat[2];
  double wz = quat[0] * quat[3];
  double xx = quat[1] * quat[1];
  double yy = quat[2] * quat[2];
  double zz = quat[3] * quat[3];
  double xy = quat[1] * quat[2];
  double xz = quat[1] * quat[3];
  double yz = quat[2] * quat[3];
  double rr = xx + yy + zz;
  // normalization factor, just in case quaternion was not normalized
  double f = 1 / (ww + rr);
  double s = (ww - rr) * f;
  f *= 2;
  A[0 * 3 + 0] = xx * f + s;
  A[1 * 3 + 0] = (xy + wz) * f;
  A[2 * 3 + 0] = (xz - wy) * f;
  A[0 * 3 + 1] = (xy - wz) * f;
  A[1 * 3 + 1] = yy * f + s;
  A[2 * 3 + 1] = (yz + wx) * f;
  A[0 * 3 + 2] = (xz + wy) * f;
  A[1 * 3 + 2] = (yz - wx) * f;
  A[2 * 3 + 2] = zz * f + s;
}
// The solution is based on
// Berthold K. P. Horn (1987),
// "Closed-form solution of absolute orientation using unit quaternions,"
// Journal of the Optical Society of America A, 4:629-642
inline void Matrix9ToQuaternion(const float A[9], float quat[4])
{
  float N[4 * 4];
  // on-diagonal elements
  N[0 * 4 + 0] = A[0 * 4 + 0] + A[1 * 4 + 1] + A[4];
  N[1 * 4 + 1] = A[0 * 4 + 0] - A[1 * 4 + 1] - A[4];
  N[4] = -A[0 * 4 + 0] + A[1 * 4 + 1] - A[4];
  N[9] = -A[0 * 4 + 0] - A[1 * 4 + 1] + A[4];
  // off-diagonal elements
  N[0 * 4 + 1] = N[1 * 4 + 0] = A[2 * 4 + 1] - A[1 * 4 + 2];
  N[0 * 4 + 2] = N[2 * 4 + 0] = A[0 * 4 + 2] - A[2 * 4 + 0];
  N[0 * 4 + 3] = N[3 * 4 + 0] = A[1 * 4 + 0] - A[0 * 4 + 1];
  N[1 * 4 + 2] = N[2 * 4 + 1] = A[1 * 4 + 0] + A[0 * 4 + 1];
  N[1 * 4 + 3] = N[3 * 4 + 1] = A[0 * 4 + 2] + A[2 * 4 + 0];
  N[2 * 4 + 3] = N[3 * 4 + 2] = A[2 * 4 + 1] + A[1 * 4 + 2];
  float eigenvectors[4 * 4], eigenvalues[4];
  // convert into format that JacobiN can use,
  // then use Jacobi to find eigenvalues and eigenvectors
  JacobiN(N, 4, 4, eigenvalues, eigenvectors, 4);
  // the first eigenvector is the one we want
  quat[0] = eigenvectors[0 * 4 + 0];
  quat[1] = eigenvectors[1 * 4 + 0];
  quat[2] = eigenvectors[2 * 4 + 0];
  quat[3] = eigenvectors[3 * 4 + 0];
}
// Multiplying two quaternions
inline void QuaternionMultiplication(const float q1[4], const float q2[4], float q[4])
{
  double ww = q1[0] * q2[0];
  double wx = q1[0] * q2[1];
  double wy = q1[0] * q2[2];
  double wz = q1[0] * q2[3];
  double xw = q1[1] * q2[0];
  double xx = q1[1] * q2[1];
  double xy = q1[1] * q2[2];
  double xz = q1[1] * q2[3];
  double yw = q1[2] * q2[0];
  double yx = q1[2] * q2[1];
  double yy = q1[2] * q2[2];
  double yz = q1[2] * q2[3];
  double zw = q1[3] * q2[0];
  double zx = q1[3] * q2[1];
  double zy = q1[3] * q2[2];
  double zz = q1[3] * q2[3];
  q[0] = ww - xx - yy - zz;
  q[1] = wx + xw + yz - zy;
  q[2] = wy - xz + yw + zx;
  q[3] = wz + xy - yx + zw;
}
// The orthogonalization is done via quaternions in order to avoid
// having to use a singular value decomposition algorithm.
inline void Orthogonalize9(const float A[9], float B[9])
{
  int i;
  // copy the matrix
  for (i = 0; i < 3; i++) {
    B[0 * 3 + i] = A[0 * 3 + i];
    B[1 * 3 + i] = A[1 * 3 + i];
    B[2 * 3 + i] = A[2 * 3 + i];
  }
  // Pivot the matrix to improve accuracy
  float scale[3];
  int index[3];
  double largest;
  // Loop over rows to get implicit scaling information
  for (i = 0; i < 3; i++) {
    double x1 = fabs(B[i * 3 + 0]);
    double x2 = fabs(B[i * 3 + 1]);
    double x3 = fabs(B[i * 3 + 2]);
    largest = (x2 > x1 ? x2 : x1);
    largest = (x3 > largest ? x3 : largest);
    scale[i] = 1;
    if (largest != 0) {
      scale[i] /= largest;
    }
  }
  // first column
  double x1 = fabs(B[0 * 3 + 0]) * scale[0];
  double x2 = fabs(B[1 * 3 + 0]) * scale[1];
  double x3 = fabs(B[2 * 3 + 0]) * scale[2];
  index[0] = 0;
  largest = x1;
  if (x2 >= largest) {
    largest = x2;
    index[0] = 1;
  }
  if (x3 >= largest) {
    index[0] = 2;
  }
  if (index[0] != 0) {
    SwapVectors3(B + index[0] * 3, B);
    scale[index[0]] = scale[0];
  }
  // second column
  double y2 = fabs(B[1 * 3 + 1]) * scale[1];
  double y3 = fabs(B[2 * 3 + 1]) * scale[2];
  index[1] = 1;
  largest = y2;
  if (y3 >= largest) {
    index[1] = 2;
    SwapVectors3(B + 2 * 3, B + 1 * 3);
  }
  // third column
  index[2] = 2;
  // A quaternion can only describe a pure rotation, not
  // a rotation with a flip, therefore the flip must be
  // removed before the matrix is converted to a quaternion.
  int flip = 0;
  if (Determinant9(B) < 0) {
    flip = 1;
    for (i = 0; i < 3; i++) {
      B[0 * 3 + i] = -B[0 * 3 + i];
      B[1 * 3 + i] = -B[1 * 3 + i];
      B[2 * 3 + i] = -B[2 * 3 + i];
    }
  }
  // Do orthogonalization using a quaternion intermediate
  // (this, essentially, does the orthogonalization via
  // diagonalization of an appropriately constructed symmetric
  // 4x4 matrix rather than by doing SVD of the 3x3 matrix)
  float quat[4];
  Matrix9ToQuaternion(B, quat);
  QuaternionToMatrix9(quat, B);
  // Put the flip back into the orthogonalized matrix.
  if (flip) {
    for (i = 0; i < 3; i++) {
      B[0 * 3 + i] = -B[0 * 3 + i];
      B[1 * 3 + i] = -B[1 * 3 + i];
      B[2 * 3 + i] = -B[2 * 3 + i];
    }
  }
  // Undo the pivoting
  if (index[1] != 1) {
    SwapVectors3(B + index[1] * 3, B + 3);
  }
  if (index[0] != 0) {
    SwapVectors3(B + index[0] * 3, B);
  }
}
double Norm(const float* x, int n)
{
  double sum = 0;
  for (int i = 0; i < n; i++) {
    sum += x[i] * x[i];
  }
  return sqrt(sum);
}
double Norm(const double* x, int n)
{
  double sum = 0;
  for (int i = 0; i < n; i++) {
    sum += x[i] * x[i];
  }
  return sqrt(sum);
}
bool ProjectVector(const float a[3], const float b[3], float projection[3])
{
  double bSquared = Dot(b, b);
  if (bSquared == 0) {
    projection[0] = 0;
    projection[1] = 0;
    projection[2] = 0;
    return false;
  }
  double scale = Dot(a, b) / bSquared;
  for (unsigned int i = 0; i < 3; i++) {
    projection[i] = b[i];
  }
  MultiplyScalar(projection, scale);
  return true;
}
bool ProjectVector(const double a[3], const double b[3], double projection[3])
{
  double bSquared = Dot(b, b);
  if (bSquared == 0) {
    projection[0] = 0;
    projection[1] = 0;
    projection[2] = 0;
    return false;
  }
  double scale = Dot(a, b) / bSquared;
  for (unsigned int i = 0; i < 3; i++) {
    projection[i] = b[i];
  }
  MultiplyScalar(projection, scale);
  return true;
}
bool ProjectVector2D(const float a[2], const float b[2], float projection[2])
{
  float bSquared = Dot2D(b, b);
  if (bSquared == 0) {
    projection[0] = 0;
    projection[1] = 0;
    return false;
  }
  float scale = Dot2D(a, b) / bSquared;
  for (unsigned int i = 0; i < 2; i++) {
    projection[i] = b[i];
  }
  MultiplyScalar2D(projection, scale);
  return true;
}
bool ProjectVector2D(const double a[2], const double b[2], double projection[2])
{
  double bSquared = Dot2D(b, b);
  if (bSquared == 0) {
    projection[0] = 0;
    projection[1] = 0;
    return false;
  }
  double scale = Dot2D(a, b) / bSquared;
  for (unsigned int i = 0; i < 2; i++) {
    projection[i] = b[i];
  }
  MultiplyScalar2D(projection, scale);
  return true;
}
// Extract the eigenvalues and eigenvectors from a 3x3 matrix.
// The eigenvectors (the columns of V) will be normalized.
// The eigenvectors are aligned optimally with the x, y, and z
// axes respectively.
inline void Diagonalize9(const float A[9], float w[3], float V[9])
{
  int i, j, k, maxI;
  double tmp, maxVal;
  // do the matrix[9] to **matrix conversion for Jacobi
  float C[9];
  for (i = 0; i < 3; i++) {
    C[i * 3 + 0] = A[i * 3 + 0];
    C[i * 3 + 1] = A[i * 3 + 1];
    C[i * 3 + 2] = A[i * 3 + 2];
  }
  // diagonalize using Jacobi
  JacobiN(C, 3, 3, w, V, 3);
  // if all the eigenvalues are the same, return identity matrix
  if (w[0] == w[1] && w[0] == w[2]) {
    Identity9(V);
    return;
  }
  // transpose temporarily, it makes it easier to sort the eigenvectors
  Transpose9(V, V);
  // if two eigenvalues are the same, re-orthogonalize to optimally line
  // up the eigenvectors with the x, y, and z axes
  for (i = 0; i < 3; i++) {
    if (w[(i + 1) % 3] == w[(i + 2) % 3]) { // two eigenvalues are the same
      // find maximum element of the independent eigenvector
      maxVal = fabs(V[i * 3 + 0]);
      maxI = 0;
      for (j = 1; j < 3; j++) {
        if (maxVal < (tmp = fabs(V[i * 3 + j]))) {
          maxVal = tmp;
          maxI = j;
        }
      }
      // swap the eigenvector into its proper position
      if (maxI != i) {
        tmp = w[maxI];
        w[maxI] = w[i];
        w[i] = tmp;
        SwapVectors3(V + i * 3, V + maxI * 3);
      }
      // maximum element of eigenvector should be positive
      if (V[maxI * 3 + maxI] < 0) {
        V[maxI * 3 + 0] = -V[maxI * 3 + 0];
        V[maxI * 3 + 1] = -V[maxI * 3 + 1];
        V[maxI * 3 + 2] = -V[maxI * 3 + 2];
      }
      // re-orthogonalize the other two eigenvectors
      j = (maxI + 1) % 3;
      k = (maxI + 2) % 3;
      V[j * 3 + 0] = 0.0;
      V[j * 3 + 1] = 0.0;
      V[j * 3 + 2] = 0.0;
      V[j * 3 + j] = 1.0;
      Cross(V + maxI * 3, V + j * 3, V + k * 3);
      Normalize3(V + k * 3);
      Cross(V + k * 3, V + maxI * 3, V + j * 3);
      // transpose vectors back to columns
      Transpose9(V, V);
      return;
    }
  }
  // the three eigenvalues are different, just sort the eigenvectors
  // to align them with the x, y, and z axes
  // find the vector with the largest x element, make that vector
  // the first vector
  maxVal = fabs(V[0 * 3 + 0]);
  maxI = 0;
  for (i = 1; i < 3; i++) {
    if (maxVal < (tmp = fabs(V[i * 3 + 0]))) {
      maxVal = tmp;
      maxI = i;
    }
  }
  // swap eigenvalue and eigenvector
  if (maxI != 0) {
    tmp = w[maxI];
    w[maxI] = w[0];
    w[0] = tmp;
    SwapVectors3(V + maxI * 3, V);
  }
  // do the same for the y element
  if (fabs(V[1 * 3 + 1]) < fabs(V[2 * 3 + 1])) {
    tmp = w[2];
    w[2] = w[1];
    w[1] = tmp;
    SwapVectors3(V + 2 * 3, V + 1 * 3);
  }
  // ensure that the sign of the eigenvectors is correct
  for (i = 0; i < 2; i++) {
    if (V[i * 3 + i] < 0) {
      V[i * 3 + 0] = -V[i * 3 + 0];
      V[i * 3 + 1] = -V[i * 3 + 1];
      V[i * 3 + 2] = -V[i * 3 + 2];
    }
  }
  // set sign of final eigenvector to ensure that determinant is positive
  if (Determinant9(V) < 0) {
    V[2 * 3 + 0] = -V[2 * 3 + 0];
    V[2 * 3 + 1] = -V[2 * 3 + 1];
    V[4] = -V[4];
  }
  // transpose the eigenvectors back again
  Transpose9(V, V);
}
// Perform singular value decomposition on the matrix A:
// A = U * W * VT
// where U and VT are orthogonal W is diagonal (the diagonal elements
// are returned in vector w).
// The matrices U and VT will both have positive determinants.
// The scale factors w are ordered according to how well the
// corresponding eigenvectors (in VT) match the x, y and z axes
// respectively.
//
// The singular value decomposition is used to decompose a linear
// transformation into a rotation, followed by a scale, followed
// by a second rotation. The scale factors w will be negative if
// the determinant of matrix A is negative.
//
// Contributed by David Gobbi (dgobbi@irus.rri.on.ca)
template <class T1, class T2>
inline void SingularValueDecomposition9(const T1 A[9], T2 U[9], T2 w[3], T2 VT[9])
{
  int i;
  T2 B[9];
  // copy so that A can be used for U or VT without risk
  for (i = 0; i < 3; i++) {
    B[0 * 3 + i] = A[0 * 3 + i];
    B[1 * 3 + i] = A[1 * 3 + i];
    B[2 * 3 + i] = A[2 * 3 + i];
  }
  // temporarily flip if determinant is negative
  T2 d = Determinant9(B);
  if (d < 0) {
    for (i = 0; i < 3; i++) {
      B[0 * 3 + i] = -B[0 * 3 + i];
      B[1 * 3 + i] = -B[1 * 3 + i];
      B[2 * 3 + i] = -B[2 * 3 + i];
    }
  }
  // orthogonalize, diagonalize, etc.
  Orthogonalize9(B, U);
  Transpose9(B, B);
  Multiply9(B, U, VT);
  Diagonalize9(VT, w, VT);
  Multiply9(U, VT, U);
  Transpose9(VT, VT);
  // re-create the flip
  if (d < 0) {
    w[0] = -w[0];
    w[1] = -w[1];
    w[2] = -w[2];
  }
  /* paranoia check: recombine to ensure that the SVD is correct
  Transpose9(B, B);
  if (d < 0)
  {
  for (i = 0; i < 3; i++)
  {
  B[0*3+i] = -B[0*3+i];
  B[1*3+i] = -B[1*3+i];
  B[2*3+i] = -B[2*3+i];
  }
  }
  int j;
  T2 maxerr = 0;
  T2 tmp;
  T2 M[9];
  T2 W[9];
  Identity9(W);
  W[0*3+0] = w[0]; W[1*3+1] = w[1]; W[4] = w[2];
  Identity9(M);
  Multiply9(M, U, M);
  Multiply9(M, W, M);
  Multiply9(M, VT, M);
  for (i = 0; i < 3; i++)
  {
  for (j = 0; j < 3; j++)
  {
  if ((tmp = fabs(B[i*3+j] - M[i*3+j])) > maxerr)
  {
  maxerr = tmp;
  }
  }
  }
  printf("SingularValueDecomposition max error = " << maxerr);
  */
}
#define VTK_VOID 0
#define VTK_BIT 1
#define VTK_CHAR 2
#define VTK_UCHAR 3
#define VTK_SHORT 4
#define VTK_USHORT 5
#define VTK_INT 6
#define VTK_UINT 7
#define VTK_LONG 8
#define VTK_ULONG 9
#define VTK_FLOAT 10
#define VTK_DOUBLE 11
#define VTK_ID_TYPE 12
#define VTK_STRING 13
#define VTK_OPAQUE 14
#define VTK_SCHAR 15
#define VTK_LONG_LONG 16
#define VTK_ULONG_LONG 17
#define VTK_INT64 18
#define VTK_UINT64 19
#define VTK_VARIANT 20
#define VTK_OBJECT 21
#define VTK_UNICODE_STRING 22
#define VTK_BIT_MIN 0
#define VTK_BIT_MAX 1
#define LONG_LONG_MIN (~(~0ul >> 1))
#define LONG_LONG_MAX (~0ul >> 1)
#define ULONG_LONG_MAX (~0ul)
#define SHORT_MIN SHRT_MIN
#define SHORT_MAX SHRT_MAX
#define USHORT_MAX USHRT_MAX
int GetScalarTypeFittingRange(double range_min, double range_max, double scale, double shift)
{
  struct TypeRange {
    int Type;
    double Min;
    double Max;
  };
  static const TypeRange FloatTypes[] = {
    { VTK_FLOAT, FLT_MIN, FLT_MAX },
    { VTK_DOUBLE, DBL_MIN, DBL_MAX }
  };
  static const TypeRange IntTypes[] = {
    { VTK_BIT, VTK_BIT_MIN, VTK_BIT_MAX },
    { VTK_CHAR, CHAR_MIN, CHAR_MAX },
    { VTK_SCHAR, SCHAR_MIN, SCHAR_MAX },
    { VTK_UCHAR, 0, UCHAR_MAX },
    { VTK_SHORT, SHRT_MIN, SHRT_MAX },
    { VTK_USHORT, 0, USHRT_MAX },
    { VTK_INT, INT_MIN, INT_MAX },
    { VTK_UINT, 0, UINT_MAX },
    { VTK_LONG, (double)(LONG_MIN), (double)(LONG_MAX) },
    { VTK_ULONG, (double)(0), (double)(ULONG_MAX) },
    { VTK_LONG_LONG, (double)(LONG_LONG_MIN), (double)(LONG_LONG_MAX) },
    { VTK_ULONG_LONG, (double)(0), (double)(ULONG_LONG_MAX) }
  };
  // If the range, scale or shift are decimal number, just browse
  // the decimal types
  double intpart;
  int range_min_is_int = (modf(range_min, &intpart) == 0.0);
  int range_max_is_int = (modf(range_max, &intpart) == 0.0);
  int scale_is_int = (modf(scale, &intpart) == 0.0);
  int shift_is_int = (modf(shift, &intpart) == 0.0);
  range_min = range_min * scale + shift;
  range_max = range_max * scale + shift;
  if (range_min_is_int && range_max_is_int && scale_is_int && shift_is_int) {
    for (unsigned int i = 0; i < sizeof(IntTypes) / sizeof(TypeRange); i++) {
      if (IntTypes[i].Min <= range_min && range_max <= IntTypes[i].Max) {
        return IntTypes[i].Type;
      }
    }
  }
  for (unsigned int i = 0; i < sizeof(FloatTypes) / sizeof(TypeRange); i++) {
    if (FloatTypes[i].Min <= range_min && range_max <= FloatTypes[i].Max) {
      return FloatTypes[i].Type;
    }
  }
  return -1;
}
BOOL ExtentIsWithinOtherExtent(int extent1[6], int extent2[6])
{
  if (!extent1 || !extent2) {
    return 0;
  }
  int i;
  for (i = 0; i < 6; i += 2) {
    if (extent1[i] < extent2[i] || extent1[i] > extent2[i + 1] ||
        extent1[i + 1] < extent2[i] || extent1[i + 1] > extent2[i + 1]) {
      return 0;
    }
  }
  return 1;
}
BOOL BoundsIsWithinOtherBounds(double bounds1[6], double bounds2[6], double delta[3])
{
  if (!bounds1 || !bounds2) {
    return 0;
  }
  for (int i = 0; i < 6; i += 2) {
    if (bounds1[i] + delta[i / 2] < bounds2[i] || bounds1[i] - delta[i / 2] > bounds2[i + 1] ||
        bounds1[i + 1] + delta[i / 2] < bounds2[i] || bounds1[i + 1] - delta[i / 2] > bounds2[i + 1]) {
      return 0;
    }
  }
  return 1;
}
BOOL PointIsWithinBounds(float3 point, float bounds[6], float delta[3])
{
  if (!point || !bounds || !delta) {
    return 0;
  }
  for (int i = 0; i < 3; i++) {
    if (point[i] + delta[i] < bounds[2 * i] || point[i] - delta[i] > bounds[2 * i + 1]) {
      return 0;
    }
  }
  return 1;
}
double AngleBetweenVectors(const float v1[3], const float v2[3])
{
  float cross[3];
  Cross(v1, v2, cross);
  return atan2(Norm3(cross), Dot(v1, v2));
}
double GaussianAmplitude(const double variance, const double distanceFromMean)
{
  return 1. / (sqrt(2.*M_PI * variance)) * exp(-(pow(distanceFromMean, 2)) / (2.*variance));
}
double GaussianAmplitude(const double mean, const double variance, const double position)
{
  double distanceToMean = fabs(mean - position);
  return GaussianAmplitude(variance, distanceToMean);
}
double GaussianWeight(const double variance, const double distanceFromMean)
{
  return exp(-(pow(distanceFromMean, 2)) / (2.*variance));
}
double GaussianWeight(const double mean, const double variance, const double position)
{
  double distanceToMean = fabs(mean - position);
  return GaussianWeight(variance, distanceToMean);
}
double Solve3PointCircle(const float p1[3], const float p2[3], const float p3[3], float center[3])
{
  int i;
  float v21[3], v32[3], v13[3];
  float v12[3], v23[3], v31[3];
  for (i = 0; i < 3; ++i) {
    v21[i] = p1[i] - p2[i];
    v32[i] = p2[i] - p3[i];
    v13[i] = p3[i] - p1[i];
    v12[i] = -v21[i];
    v23[i] = -v32[i];
    v31[i] = -v13[i];
  }
  double norm12 = Norm3(v12);
  double norm23 = Norm3(v23);
  double norm13 = Norm3(v13);
  float crossv21v32[3];
  Cross(v21, v32, crossv21v32);
  double normCross = Norm3(crossv21v32);
  double radius = (norm12 * norm23 * norm13) / (2. * normCross);
  double alpha = ((norm23 * norm23) * Dot(v21, v31)) /
      (2. * normCross * normCross);
  double beta = ((norm13 * norm13) * Dot(v12, v32)) /
      (2. * normCross * normCross);
  double gamma = ((norm12 * norm12) * Dot(v13, v23)) /
      (2. * normCross * normCross);
  for (i = 0; i < 3; ++i) {
    center[i] = alpha * p1[i] + beta * p2[i] + gamma * p3[i];
  }
  return radius;
}
#if 0
int GetAdjustedScalarRange(vtkDataArray* array, int comp, double range[2])
{
  if (!array || comp < 0 || comp >= array->GetNumberOfComponents()) {
    return 0;
  }
  array->GetRange(range, comp);
  switch (array->GetDataType()) {
  case VTK_UCHAR:
    range[0] = (double)(array->GetDataTypeMin());
    range[1] = (double)(array->GetDataTypeMax());
    break;
  case VTK_USHORT:
    range[0] = (double)(array->GetDataTypeMin());
    if (range[1] <= 4095.0) {
      if (range[1] > UCHAR_MAX) {
        range[1] = 4095.0;
      }
    }
    else {
      range[1] = (double)(array->GetDataTypeMax());
    }
    break;
  default:
    assert("check: impossible case." && 0); // reaching this line is a bug.
    break;
  }
  return 1;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////
void Copy9(float dst[9], const float src[9])
{
  COPY9(dst, src);
}
void Copy33(float33 dst, const float33 src)
{
  Copy9(*dst, *src);
}
void Copy16(float16 elements, const float16 newElements)
{
  for (int i = 0; i < 16; ++i) {
    elements[i] = newElements[i];
  }
}
void Copy44(float44 dst, const float44 src)
{
  Copy16(*dst, *src);
}
void Zero9(float elements[9])
{
  for (int i = 0; i < 9; ++i) {
    elements[i] = 0.0;
  }
}
void MultiplyPoint9(const float9 elem, const float in[3], float out[3])
{
  float v1 = in[0];
  float v2 = in[1];
  float v3 = in[2];
  out[0] = v1 * elem[0] + v2 * elem[1] + v3 * elem[2];
  out[1] = v1 * elem[3] + v2 * elem[4] + v3 * elem[5];
  out[2] = v1 * elem[6] + v2 * elem[7] + v3 * elem[8];
}
void Transpose9(float outElements[9])
{
  Transpose9(outElements, outElements);
}
void Transpose33(float33 outElements)
{
  Transpose9(*outElements, *outElements);
}
void PointMultiply9(const float elements[9], const float in[3], float result[3])
{
  float newElements[9];
  Transpose9(elements, newElements);
  MultiplyPoint9(newElements, in, result);
}
// Multiplies matrices a and b and stores the result in c.
void Multiply9(const float a[9], const float b[9], float c[9])
{
  float accum[9];
  for (int i = 0; i < 9; i += 3) {
    for (int k = 0; k < 3; k++) {
      accum[i + k] = a[i + 0] * b[k + 0] +
          a[i + 1] * b[k + 3] +
          a[i + 2] * b[k + 6];
    }
  }
  // Copy to final dest
  for (int j = 0; j < 9; j++) {
    c[j] = accum[j];
  }
}
// Multiplies matrices a and b and stores the result in c.
inline void MultiplyMatrix16(const float16 a, const float16 b, float16 c)
{
  float16 tmp;
  for (int i = 0; i < 16; i += 4) {
    for (int j = 0; j < 4; j++) {
      tmp[i + j] = a[i + 0] * b[j + 0] +
          a[i + 1] * b[j + 4] +
          a[i + 2] * b[j + 8] +
          a[i + 3] * b[j + 12];
    }
  }
  for (int k = 0; k < 16; k++) {
    c[k] = tmp[k];
  }
}
void MultiplyMatrix44(const float44 a, const float44 b, float44 c)
{
  MultiplyMatrix16(*a, *b, *c);
}
void Adjoint9(const float inElements[9], float outElements[9])
{
  //
  // adjoint( original_matrix, inverse_matrix )
  //
  // calculate the adjoint of a 3x3 matrix
  //
  // Let a denote the minor determinant of matrix A obtained by
  // ij
  //
  // deleting the ith row and jth column from A.
  //
  // i+j
  // Let b = (-1) a
  // ij ji
  //
  // The matrix B = (b ) is the adjoint of A
  // ij
  //
  float a1, a2, a3, b1, b2, b3, c1, c2, c3;
  // assign to individual variable names to aid
  // selecting correct values
  a1 = inElements[0];
  b1 = inElements[1];
  c1 = inElements[2];
  a2 = inElements[3];
  b2 = inElements[4];
  c2 = inElements[5];
  a3 = inElements[6];
  b3 = inElements[7];
  c3 = inElements[8];
  // row column labeling reversed since we transpose rows & columns
  outElements[0] = Determinant2x2(b2, b3, c2, c3);
  outElements[3] = - Determinant2x2(a2, a3, c2, c3);
  outElements[6] = Determinant2x2(a2, a3, b2, b3);
  outElements[1] = - Determinant2x2(b1, b3, c1, c3);
  outElements[4] = Determinant2x2(a1, a3, c1, c3);
  outElements[7] = - Determinant2x2(a1, a3, b1, b3);
  outElements[2] = Determinant2x2(b1, b2, c1, c2);
  outElements[5] = - Determinant2x2(a1, a2, c1, c2);
  outElements[8] = Determinant2x2(a1, a2, b1, b2);
}
// Matrix Inversion (adapted from Richard Carling in "Graphics Gems,"
// Academic Press, 1990).
void Invert9_(const float inElements[9], float outElements[9])
{
  float tmp[9];
  if (inElements == outElements) {
    Copy9(tmp, inElements);
    inElements = tmp;
  }
  // inverse( original_matrix, inverse_matrix )
  // calculate the inverse of a 3x3 matrix
  //
  // -1
  // A = ___1__ adjoint A
  // det A
  //
  // calculate the 3x3 determinent
  // if the determinent is zero,
  // then the inverse matrix is not unique.
  float det = Determinant9(inElements);
  if (det == 0.0) {
    return;
  }
  // calculate the adjoint matrix
  Adjoint9(inElements, outElements);
  // scale the adjoint matrix to get the inverse
  for (int i = 0; i < 9; i++) {
    outElements[i] /= det;
  }
}
void print_indent(int indent)
{
  int i;
  for (i = 0; i < indent; ++i) {
    printf(" ");
  }
}
void PrintSelf9(const float* m, int indent)
{
  print_indent(indent);
  printf("Elements:\n");
  for (int i = 0; i < 3; ++i) {
    print_indent(indent);
    for (int j = 0; j < 3; ++j) {
      printf("\t%f", m[i * 3 + j]);
    }
    printf("\n");
  }
}
void Zero16(float16 elements)
{
  for (int i = 0; i < 16; i++) {
    elements[i] = 0.0;
  }
}
void Identity16(float16 elements)
{
  elements[0] = elements[5] = elements[10] = elements[15] = 1.0;
  elements[1] = elements[2] = elements[3] = elements[4] =
      elements[6] = elements[7] = elements[8] = elements[9] =
          elements[11] = elements[12] = elements[13] = elements[14] = 0.0;
}
void MultiplyPoint16(const float16 elem, const float4 in, float4 out)
{
  float v1 = in[0];
  float v2 = in[1];
  float v3 = in[2];
  float v4 = in[3];
  out[0] = v1 * elem[0] + v2 * elem[1] + v3 * elem[2] + v4 * elem[3];
  out[1] = v1 * elem[4] + v2 * elem[5] + v3 * elem[6] + v4 * elem[7];
  out[2] = v1 * elem[8] + v2 * elem[9] + v3 * elem[10] + v4 * elem[11];
  out[3] = v1 * elem[12] + v2 * elem[13] + v3 * elem[14] + v4 * elem[15];
}
void MultiplyPoint(const float44 elem, const float4 in, float4 out) {
  MultiplyPoint16(*elem, in, out);
}
void Adjoint16(const float16 elem, float16 outElem)
{
  //
  // adjoint( original_matrix, inverse_matrix )
  //
  // calculate the adjoint of a 4x4 matrix
  //
  // Let a denote the minor determinant of matrix A obtained by
  // ij
  //
  // deleting the ith row and jth column from A.
  //
  // i+j
  // Let b = (-1) a
  // ij ji
  //
  // The matrix B = (b ) is the adjoint of A
  // ij
  //
  double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;
  // assign to individual variable names to aid
  // selecting correct values
  a1 = elem[0];
  b1 = elem[1];
  c1 = elem[2];
  d1 = elem[3];
  a2 = elem[4];
  b2 = elem[5];
  c2 = elem[6];
  d2 = elem[7];
  a3 = elem[8];
  b3 = elem[9];
  c3 = elem[10];
  d3 = elem[11];
  a4 = elem[12];
  b4 = elem[13];
  c4 = elem[14];
  d4 = elem[15];
  // row column labeling reversed since we transpose rows & columns
  outElem[0] = Determinant9(b2, b3, b4, c2, c3, c4, d2, d3, d4);
  outElem[4] = - Determinant9(a2, a3, a4, c2, c3, c4, d2, d3, d4);
  outElem[8] = Determinant9(a2, a3, a4, b2, b3, b4, d2, d3, d4);
  outElem[12] = - Determinant9(a2, a3, a4, b2, b3, b4, c2, c3, c4);
  outElem[1] = - Determinant9(b1, b3, b4, c1, c3, c4, d1, d3, d4);
  outElem[5] = Determinant9(a1, a3, a4, c1, c3, c4, d1, d3, d4);
  outElem[9] = - Determinant9(a1, a3, a4, b1, b3, b4, d1, d3, d4);
  outElem[13] = Determinant9(a1, a3, a4, b1, b3, b4, c1, c3, c4);
  outElem[2] = Determinant9(b1, b2, b4, c1, c2, c4, d1, d2, d4);
  outElem[6] = - Determinant9(a1, a2, a4, c1, c2, c4, d1, d2, d4);
  outElem[10] = Determinant9(a1, a2, a4, b1, b2, b4, d1, d2, d4);
  outElem[14] = - Determinant9(a1, a2, a4, b1, b2, b4, c1, c2, c4);
  outElem[3] = - Determinant9(b1, b2, b3, c1, c2, c3, d1, d2, d3);
  outElem[7] = Determinant9(a1, a2, a3, c1, c2, c3, d1, d2, d3);
  outElem[11] = - Determinant9(a1, a2, a3, b1, b2, b3, d1, d2, d3);
  outElem[15] = Determinant9(a1, a2, a3, b1, b2, b3, c1, c2, c3);
}
double Determinant16(const float16 elem)
{
  double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;
  // assign to individual variable names to aid selecting
  // correct elements
  a1 = elem[0];
  b1 = elem[1];
  c1 = elem[2];
  d1 = elem[3];
  a2 = elem[4];
  b2 = elem[5];
  c2 = elem[6];
  d2 = elem[7];
  a3 = elem[8];
  b3 = elem[9];
  c3 = elem[10];
  d3 = elem[11];
  a4 = elem[12];
  b4 = elem[13];
  c4 = elem[14];
  d4 = elem[15];
  return a1 * Determinant9(b2, b3, b4, c2, c3, c4, d2, d3, d4)
      - b1 * Determinant9(a2, a3, a4, c2, c3, c4, d2, d3, d4)
      + c1 * Determinant9(a2, a3, a4, b2, b3, b4, d2, d3, d4)
      - d1 * Determinant9(a2, a3, a4, b2, b3, b4, c2, c3, c4);
}
// Matrix Inversion (adapted from Richard Carling in "Graphics Gems,"
// Academic Press, 1990).
void Invert16(const float16 inElements, float16 outElements)
{
  float16 tmp;
  if (inElements == outElements) {
    Copy16(tmp, inElements);
    inElements = tmp;
  }
  // inverse( original_matrix, inverse_matrix )
  // calculate the inverse of a 4x4 matrix
  //
  // -1
  // A = ___1__ adjoint A
  // det A
  //
  // calculate the 4x4 determinent
  // if the determinent is zero,
  // then the inverse matrix is not unique.
  double det = Determinant16(inElements);
  if (det == 0.0) {
    return;
  }
  // calculate the adjoint matrix
  Adjoint16(inElements, outElements);
  // scale the adjoint matrix to get the inverse
  for (int i = 0; i < 16; i++) {
    outElements[i] /= det;
  }
}
// Transpose the matrix and put it into out.
void Transpose16(const float16 src, float16 dst)
{
  int i, j;
  for (i = 0; i < 4; i++) {
    for(j = i; j < 4; j++) {
      float temp = src[i*4+j];
      dst[i*4+j] = src[j*4+i];
      dst[j*4+i] = temp;
    }
  }
}

void Transpose44(const float44 src, float44 dst)
{
  Transpose16(*src, *dst);
}
void Transpose44(float44 dst)
{
  Transpose16(*dst, *dst);
}
void Identity44(float44 m)
{
  m[0][0] = 1.0f, m[0][1] = 0.0f, m[0][2] = 0.0f, m[0][3] = 0.0f;
  m[1][0] = 0.0f, m[1][1] = 1.0f, m[1][2] = 0.0f, m[1][3] = 0.0f;
  m[2][0] = 0.0f, m[2][1] = 0.0f, m[2][2] = 1.0f, m[2][3] = 0.0f;
  m[3][0] = 0.0f, m[3][1] = 0.0f, m[3][2] = 0.0f, m[3][3] = 1.0f;
}

void PrintSelf(const float44 m44, int indent)
{
#if 0
  this->Superclass::PrintSelf(os, indent);
  print_inent(indent << "Elements:\n";
  for (int i = 0; i < 4; i++) {
  os << indent << indent;
  for (int j = 0; j < 4; j++) {
      os << m44[i * 4 + j] << " ";
    }
    os << "\n";
  }
#endif
}
void Adjoint44(float44 in, float44& out)
{
  Adjoint16(*in, *out);
}
double Determinant44(float44 in)
{
  return Determinant16(*in);
}
void Invert44(const float44 in, float44 out)
{
  Invert16(*in, *out);
}
void Invert44(float44 out)
{
  Invert16(*out, *out);
}
void PointMultiply16(const float16 elements, const float in[4], float result[4])
{
  float16 newElements;
  Transpose16(elements, newElements);
  MultiplyPoint16(newElements, in, result);
}
void PointMultiply(const float16 elements, const float in[4], float result[4])
{
  float16 newElements;
  Transpose16(elements, newElements);
  MultiplyPoint16(newElements, in, result);
}
void TransformVector(const float44 m, const float3 in, float3 out)
{
  float x, y, z;
  x = (m[0][0]*in[0] + m[0][1]*in[1] + m[0][2]*in[2]);
  y = (m[1][0]*in[0] + m[1][1]*in[1] + m[1][2]*in[2]);
  z = (m[2][0]*in[0] + m[2][1]*in[1] + m[2][2]*in[2]);
  out[0] = x;
  out[1] = y;
  out[2] = z;
}
void TransformNormal(const float44 mat, const float3 in, float3 out)
{
  // to transform the normal, multiply by the transposed inverse matrix
  float44 matrix;
  Invert44(mat, matrix);
  Transpose44(matrix, matrix);
  TransformVector(matrix,in,out);
  Normalize3(out, out);
}
