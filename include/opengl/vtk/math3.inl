
#define DEGTORAD (M_PI/180.)
#define DEGTORAD64  (M_PI/180.)

void Rotate2(float2 v, const float2 v1, f64 degrees0, const float2 center)
{
  const f64 degrees = degrees0 * DEGTORAD64;
  const f64 cs = cos(degrees);
  const f64 sn = sin(degrees);
  float2 t;

  if (center) {
    VSUB2(t, v1, center);
    VSET2(v, (f32)(t[0] * cs - t[1] * sn), (f32)(t[0] * sn + t[1] * cs));
    VSUB2(v, v, center);
  }
  else {
    VSET2V(t, v1);
    VSET2(v, (f32)(t[0] * cs - t[1] * sn), (f32)(t[0] * sn + t[1] * cs));
  }
}

bool RotateAxis4(float16 ret, const float3 axis, double theta)
{
  double den = SNorm3(axis);
  if (den != 0.0) {
    den = 1. / sqrt(den);
    double sa = sin(theta);
    double ca = cos(theta);
    double u = axis[0]*den;
    double v = axis[1]*den;
    double w = axis[2]*den;
    
    ret[0*4+0] = ca + (u * u) * (1 - ca);
    ret[0*4+1] = u * v * (1 - ca) + w * sa;
    ret[0*4+2] = u * w * (1 - ca) - v * sa;
    ret[0*4+3] = 0;
    
    ret[1*4+0] = u * v * (1 - ca) - w * sa;
    ret[1*4+1] = ca + v * v * (1 - ca);
    ret[1*4+2] = w * v * (1 - ca) + u * sa;
    ret[1*4+3] = 0;
    
    ret[2*4+0] = u * w * (1 - ca) + v * sa;
    ret[2*4+1] = v * w * (1 - ca) - u * sa;
    ret[2*4+2] = ca + w * w * (1 - ca);
    ret[2*4+3] = 0;
    
    ret[3*4+0] = 0;
    ret[3*4+1] = 0;
    ret[3*4+2] = 0;
    ret[3*4+3] = 1;
    return true;
  }
  return false;
}
bool RotateArbitraryLine(float16 ret, const float3 v1, const float3 v2, double theta)
{
  float3 axis;
  axis[0] = v2[0] - v1[0];
  axis[1] = v2[1] - v1[1];
  axis[2] = v2[2] - v1[2];
  double den = SNorm3(axis);
  if (den != 0.0) {
    den = 1. / sqrt(den);
    double u = axis[0]*den;
    double v = axis[1]*den;
    double w = axis[2]*den;
    double a = v1[0];
    double b = v1[1];
    double c = v1[2];
    
    double uu = u * u;
    double uv = u * v;
    double uw = u * w;
    double vv = v * v;
    double vw = v * w;
    double ww = w * w;
    double au = a * u;
    double av = a * v;
    double aw = a * w;
    double bu = b * u;
    double bv = b * v;
    double bw = b * w;
    double cu = c * u;
    double cv = c * v;
    double cw = c * w;
    
    double ca = cos(theta);
    double sa = sin(theta);
    
    ret[0*4+0] = uu + (vv + ww) * ca;
    ret[0*4+1] = uv * (1 - ca) + w * sa;
    ret[0*4+2] = uw * (1 - ca) - v * sa;
    ret[0*4+3] = 0;
    
    ret[1*4+0] = uv * (1 - ca) - w * sa;
    ret[1*4+1] = vv + (uu + ww) * ca;
    ret[1*4+2] = vw * (1 - ca) + u * sa;
    ret[1*4+3] = 0;
    
    ret[2*4+0] = uw * (1 - ca) + v * sa;
    ret[2*4+1] = vw * (1 - ca) - u * sa;
    ret[2*4+2] = ww + (uu + vv) * ca;
    ret[2*4+3] = 0;
    
    ret[3*4+0] = (a * (vv + ww) - u * (bv + cw)) * (1 - ca) + (bw - cv) * sa;
    ret[3*4+1] = (b * (uu + ww) - v * (au + cw)) * (1 - ca) + (cu - aw) * sa;
    ret[3*4+2] = (c * (uu + vv) - w * (au + bv)) * (1 - ca) + (av - bu) * sa;
    ret[3*4+3] = 1;
    return true;
  }
  return false;
}
#if 0
double[,] Rotate3(double angle, float3 u)
{
  double norm = Normalize(u);
  double[,] rotatinMatrix = new double[3,3];
  double sa = sin(angle);
  double ca = cos(angle);
  
  u[0] = u[0] / norm;
  u[1] = u[1] / norm;
  u[2] = u[2] / norm;
  
  rotatinMatrix[0, 0] = ca + u[0] * u[0] * (1 - ca);
  rotatinMatrix[0, 0] = u[0] * u[1] * (1 - ca - u[2] * sa);
  rotatinMatrix[0, 0] = u[1] * sa + u[0] * u[2] * (1 - ca);
  
  rotatinMatrix[0, 0] = u[2] * sa + u[0] * u[1] * (1 - ca);
  rotatinMatrix[0, 0] = ca + u[1] * u[1] * (1 - ca);
  rotatinMatrix[0, 0] = -u[0] * sa + u[1] * u[2] * (1 - ca);
  
  rotatinMatrix[0, 0] = -u[1] * sa + u[0] * u[2] * (1 - ca);
  rotatinMatrix[0, 0] = u[0] * sa + u[1] * u[2] * (1 - ca);
  rotatinMatrix[0, 0] = ca + u[2] * u[2] * (1 - ca);
  
  return rotatinMatrix;
}
#endif


#define M4OPT(m, m1, opt, m2) \
  (m[0] = m1[0] opt m2[0], \
      m[1] = m1[1] opt m2[1], \
      m[2] = m1[2] opt m2[2], \
      m[3] = m1[3] opt m2[3], \
      m[4] = m1[4] opt m2[4], \
      m[5] = m1[5] opt m2[5], \
      m[6] = m1[6] opt m2[6], \
      m[7] = m1[7] opt m2[7], \
      m[8] = m1[8] opt m2[8], \
      m[9] = m1[9] opt m2[9], \
      m[10] = m1[10] opt m2[10], \
      m[11] = m1[11] opt m2[11], \
      m[12] = m1[12] opt m2[12], \
      m[13] = m1[13] opt m2[13], \
      m[14] = m1[14] opt m2[14], \
      m[15] = m1[15] opt m2[15])

CC_INLINE void Scale216(float16 m, const float16 m1, double scalar)
{
  m[0] = m1[0] * scalar;
  m[1] = m1[1] * scalar;
  m[2] = m1[2] * scalar;
  m[3] = m1[3] * scalar;
  m[4] = m1[4] * scalar;
  m[5] = m1[5] * scalar;
  m[6] = m1[6] * scalar;
  m[7] = m1[7] * scalar;
  m[8] = m1[8] * scalar;
  m[9] = m1[9] * scalar;
  m[10] = m1[10] * scalar;
  m[11] = m1[11] * scalar;
  m[12] = m1[12] * scalar;
  m[13] = m1[13] * scalar;
  m[14] = m1[14] * scalar;
  m[15] = m1[15] * scalar;
}

CC_INLINE void Mul16(float16 m, const float16 m1, const float16 m2)
{
  m[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
  m[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
  m[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
  m[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
  m[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
  m[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
  m[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
  m[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
  m[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
  m[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
  m[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
  m[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
  m[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
  m[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
  m[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
  m[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
}

CC_INLINE void M4_getTranslation3v(const float* m, float* v)
{
  VSET3(v, m[12], m[13], m[14]);
}
CC_INLINE void M4_setTranslation3v(float* m, const float* translation)
{
  m[12] = translation[0];
  m[13] = translation[1];
  m[14] = translation[2];
}

CC_INLINE void setInverseTranslation(float* m, const float* translation)
{
  m[12] = -translation[0];
  m[13] = -translation[1];
  m[14] = -translation[2];
}

CC_INLINE void M4_setScale(float* m, const float* scale)
{
  m[0] = scale[0];
  m[5] = scale[1];
  m[10] = scale[2];
  return ;
}
//! Returns the absolute values of the scales of the matrix.
/**
Note that this returns the absolute (positive) values unless only scale is set.
Unfortunately it does not appear to be possible to extract any original negative
values. The best that we could do would be to arbitrarily make one scale
negative if one or three of them were negative.
FIXME - return the original values.
*/
#define ISZERO_F32(x)  ((x)==0)
CC_INLINE void GetScale16(const float* m, float* scale)
{
  // See http://www.robertblum.com/articles/2005/02/14/decomposing-matrices
  // Deal with the 0 rotation case first
  // Prior to Irrlicht 1.6, we always returned this value.
  if (ISZERO_F32(m[1]) && ISZERO_F32(m[2]) &&
      ISZERO_F32(m[4]) && ISZERO_F32(m[6]) &&
      ISZERO_F32(m[8]) && ISZERO_F32(m[9])) {
    VSET3(scale, m[0], m[5], m[10]);
  }

  // We have to do the full calculation.
  VSET3(scale, sqrtf(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]),
      sqrtf(m[4] * m[4] + m[5] * m[5] + m[6] * m[6]),
      sqrtf(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]));
}

CC_INLINE void RotationRadians16(float16 m, const float* rotation)
{
  const f64 cr = cos(rotation[0]);
  const f64 sr = sin(rotation[0]);
  const f64 cp = cos(rotation[1]);
  const f64 sp = sin(rotation[1]);
  const f64 cy = cos(rotation[2]);
  const f64 sy = sin(rotation[2]);
  m[0] = (f32)(cp * cy);
  m[1] = (f32)(cp * sy);
  m[2] = (f32)(-sp);
  {
    const f64 srsp = sr * sp;
    const f64 crsp = cr * sp;
    m[4] = (f32)(srsp * cy - cr * sy);
    m[5] = (f32)(srsp * sy + cr * cy);
    m[6] = (f32)(sr * cp);
    m[8] = (f32)(crsp * cy + sr * sy);
    m[9] = (f32)(crsp * sy - sr * cy);
    m[10] = (f32)(cr * cp);
  }
}
//! Sets matrix to rotation matrix of inverse angles given as parameters
CC_INLINE void M4_setInverseRotationRadians(float* m, const float* rotation)
{
  f64 srsp, crsp;
  f64 cr = cos(rotation[0]);
  f64 sr = sin(rotation[0]);
  f64 cp = cos(rotation[1]);
  f64 sp = sin(rotation[1]);
  f64 cy = cos(rotation[2]);
  f64 sy = sin(rotation[2]);
  m[0] = (f32)(cp * cy);
  m[4] = (f32)(cp * sy);
  m[8] = (f32)(-sp);
  srsp = sr * sp;
  crsp = cr * sp;
  m[1] = (f32)(srsp * cy - cr * sy);
  m[5] = (f32)(srsp * sy + cr * cy);
  m[9] = (f32)(sr * cp);
  m[2] = (f32)(crsp * cy + sr * sy);
  m[6] = (f32)(crsp * sy - sr * cy);
  m[10] = (f32)(cr * cp);
}
CC_INLINE void M4_setRotationDegrees(float* m, const float* rotation)
{
  float3 v;
  VSCALE3(v, rotation, DEGTORAD);
  RotationRadians16(m, v);
}
CC_INLINE void M4_setInverseRotationDegrees(float* m, const float* rotation)
{
  float3 v;
  VSCALE3(v, rotation, DEGTORAD);
  M4_setInverseRotationRadians(m, v);
}
#if 0

//! Returns a rotation that is equivalent to that set by setRotationDegrees().
/** This code was sent in by Chev.  Note that it does not necessarily return
the *same* Euler angles as those set by setRotationDegrees(), but the rotation will
be equivalent, i.e. will have the same result when used to rotate a vector or node. */
#define reciprocalf(x)   (1.f/(x))
CC_INLINE void M4_getRotationDegrees(const float* m, float* v)
{
  const float* mat = m;
  float3 scale;
  M4_getScale(m, scale);

  // we need to check for negative scale on to axes, which would bring up wrong results
  if (scale[1] < 0 && scale[2] < 0) {
    scale[1] = -scale[1];
    scale[2] = -scale[2];
  }
  else if (scale[0] < 0 && scale[2] < 0) {
    scale[0] = -scale[0];
    scale[2] = -scale[2];
  }
  else if (scale[0] < 0 && scale[1] < 0) {
    scale[0] = -scale[0];
    scale[1] = -scale[1];
  }

  {
    double invScale[3] = {reciprocalf(scale[0]), reciprocalf(scale[1]), reciprocalf(scale[2])};
    f64 Y = -asin(BOUND(mat[2] * invScale[0], -1.0, 1.0));
    const f64 C = cos(Y);
    f64 rotx, roty, X, Z;
    Y *= RADTODEG64;

    if (!ISZERO_F32(C)) {
      const f64 invC = reciprocalf(C);
      rotx = mat[10] * invC * invScale[2];
      roty = mat[6] * invC * invScale[1];
      X = atan2(roty, rotx) * RADTODEG64;
      rotx = mat[0] * invC * invScale[0];
      roty = mat[1] * invC * invScale[0];
      Z = atan2(roty, rotx) * RADTODEG64;
    }
    else {
      X = 0.0;
      rotx = mat[5] * invScale[1];
      roty = -mat[4] * invScale[1];
      Z = atan2(roty, rotx) * RADTODEG64;
    }

    // fix values that get below zero
    if (X < 0.0) {
      X += 360.0;
    }

    if (Y < 0.0) {
      Y += 360.0;
    }

    if (Z < 0.0) {
      Z += 360.0;
    }

    VSET3(v, (f32)X, (f32)Y, (f32)Z);
  }
}
//! Sets matrix to rotation matrix defined by axis and angle, assuming LH rotation
CC_INLINE void M4_setRotationAxisRadians(float* m, const f32 angle, const float* axis)
{
  const f64 c = cos(angle);
  const f64 s = sin(angle);
  const f64 t = 1.0 - c;
  const f64 tx  = t * axis[0];
  const f64 ty  = t * axis[1];
  const f64 tz  = t * axis[2];
  const f64 sx  = s * axis[0];
  const f64 sy  = s * axis[1];
  const f64 sz  = s * axis[2];
  m[0] = (f32)(tx * axis[0] + c);
  m[1] = (f32)(tx * axis[1] + sz);
  m[2] = (f32)(tx * axis[2] - sy);
  m[4] = (f32)(ty * axis[0] - sz);
  m[5] = (f32)(ty * axis[1] + c);
  m[6] = (f32)(ty * axis[2] + sx);
  m[8]  = (f32)(tz * axis[0] + sy);
  m[9]  = (f32)(tz * axis[1] - sx);
  m[10] = (f32)(tz * axis[2] + c);
}

CC_INLINE void M4_makeIdentity(float* m)
{
  memset(m, 0, 16 * sizeof(f32));
  m[0] = m[5] = m[10] = m[15] = (f32)1;
  return ;
}
/*
  check identity with epsilon
  solve floating range problems..
*/

CC_INLINE bool M4_isIdentity(const float* m)
{
  if (!ISZERO_F32(m[12]) || !ISZERO_F32(m[13]) || !ISZERO_F32(m[14]) || !EQUALS_F32(1.f, m[15])) {
    return false;
  }

  if (!ISZERO_F32(m[ 1]) || !ISZERO_F32(m[ 2]) || !ISZERO_F32(m[ 3]) || !EQUALS_F32(1.f, m[ 0])) {
    return false;
  }

  if (!ISZERO_F32(m[ 4]) || !ISZERO_F32(m[ 6]) || !ISZERO_F32(m[ 7]) || !EQUALS_F32(1.f, m[ 5])) {
    return false;
  }

  if (!ISZERO_F32(m[ 8]) || !ISZERO_F32(m[ 9]) || !ISZERO_F32(m[11]) || !EQUALS_F32(1.f, m[10])) {
    return false;
  }

  return true;
}
/* Check orthogonality of matrix. */
CC_INLINE bool M4_isOrthogonal(const float* m)
{
  f32 dp = m[0] * m[4 ] + m[1] * m[5 ] + m[2 ] * m[6 ] + m[3 ] * m[7 ];

  if (!ISZERO_F32(dp)) {
    return false;
  }

  dp = m[0] * m[8 ] + m[1] * m[9 ] + m[2 ] * m[10] + m[3 ] * m[11];

  if (!ISZERO_F32(dp)) {
    return false;
  }

  dp = m[0] * m[12] + m[1] * m[13] + m[2 ] * m[14] + m[3 ] * m[15];

  if (!ISZERO_F32(dp)) {
    return false;
  }

  dp = m[4] * m[8 ] + m[5] * m[9 ] + m[6 ] * m[10] + m[7 ] * m[11];

  if (!ISZERO_F32(dp)) {
    return false;
  }

  dp = m[4] * m[12] + m[5] * m[13] + m[6 ] * m[14] + m[7 ] * m[15];

  if (!ISZERO_F32(dp)) {
    return false;
  }

  dp = m[8] * m[12] + m[9] * m[13] + m[10] * m[14] + m[11] * m[15];
  return (ISZERO_F32(dp));
}
/*
  doesn't solve floating range problems..
  but takes care on +/- 0 on translation because we are changing it..
  reducing floating point branches
  but it needs the floats in memory..
*/
CC_INLINE bool M4_isIdentity_integer_base(const float* m)
{
  if (IR(m[0]) != F32_VALUE_1) {
    return false;
  }

  if (IR(m[1]) != 0) {
    return false;
  }

  if (IR(m[2]) != 0) {
    return false;
  }

  if (IR(m[3]) != 0) {
    return false;
  }

  if (IR(m[4]) != 0) {
    return false;
  }

  if (IR(m[5]) != F32_VALUE_1) {
    return false;
  }

  if (IR(m[6]) != 0) {
    return false;
  }

  if (IR(m[7]) != 0) {
    return false;
  }

  if (IR(m[8]) != 0) {
    return false;
  }

  if (IR(m[9]) != 0) {
    return false;
  }

  if (IR(m[10]) != F32_VALUE_1) {
    return false;
  }

  if (IR(m[11]) != 0) {
    return false;
  }

  if (IR(m[12]) != 0) {
    return false;
  }

  if (IR(m[13]) != 0) {
    return false;
  }

  if (IR(m[13]) != 0) {
    return false;
  }

  if (IR(m[15]) != F32_VALUE_1) {
    return false;
  }

  return true;
}

//! An alternate transform vector method, writing into a second vector
CC_INLINE void M4_rotateV3(const float* m, float* out, const float* in)
{
  float3 v;
  VSET3V(v, in);
  out[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8];
  out[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9];
  out[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10];
}
CC_INLINE void M4_inverseRotateV3(const float* m, float3 out, const float3 in)
{
  float3 v;
  VSET3V(v, in);
  out[0] = v[0] * m[0] + v[1] * m[1] + v[2] * m[2];
  out[1] = v[0] * m[4] + v[1] * m[5] + v[2] * m[6];
  out[2] = v[0] * m[8] + v[1] * m[9] + v[2] * m[10];
}

CC_INLINE void M4_transformV3(const float* m, float3 out, const float3 in)
{
  float3 v;
  VSET3V(v, in);
  out[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + m[12];
  out[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + m[13];
  out[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + m[14];
}

CC_INLINE void M4_transformV4(const float* m, float4 out, const float3 in)
{
  float3 v;
  VSET3V(v, in);
  out[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + m[12];
  out[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + m[13];
  out[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + m[14];
  out[3] = v[0] * m[3] + v[1] * m[7] + v[2] * m[11] + m[15];
}

double recalculateD(float3 plane, const float3 MPoint)
{
  return -VDOT3(MPoint, plane);
}
void getMemberPoint(float3 plane, float3 MPoint)
{
  VSCALE3(MPoint, plane, -plane[3]);
}
//! Multiplies this matrix by a 1x4 matrix

CC_INLINE void M4_multiplyWith1x4Matrix(const float* m, f32* out, const f32* in)
{
  /*
  0  1  2  3
  4  5  6  7
  8  9  10 11
  12 13 14 15
  */
  f32 v[4];
  VSET4V(v, in);
  out[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12] * v[3];
  out[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13] * v[3];
  out[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
  out[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
}

CC_INLINE void M4_inverseTranslateVect(const float* m, float* out, const float* in)
{
  out[0] = in[0] - m[12];
  out[1] = in[1] - m[13];
  out[2] = in[2] - m[14];
}

CC_INLINE void M4_translateVect(const float* m, float* out, const float* in)
{
  out[0] = in[0] + m[12];
  out[1] = in[1] + m[13];
  out[2] = in[2] + m[14];
}

CC_INLINE bool M4_getInverse(const float* _m, float* _out)
{
  /// Calculates the inverse of this Matrix
  /// The inverse is calculated using Cramers rule.
  /// If no inverse exists then 'false' is returned.
  f32 d;
  float16 _m1;

  if (_m == _out) {
    MEMCPY(_m1, _m, 16);
    _m = _m1;
  }

#define m(i, j)   _m[(i)*4+j]
#define out(i, j)   _out[(i)*4+j]
  d = (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
      (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
      (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
      (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
      (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
      (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0));

  if (ISZERO_F32(d)) {
    return false;
  }

  d = reciprocalf(d);
  out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) + m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) + m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
  out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) + m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) + m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
  out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) + m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) + m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
  out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) + m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) + m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
  out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) + m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) + m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
  out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) + m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) + m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
  out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) + m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) + m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
  out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) + m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) + m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
  out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) + m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) + m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
  out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) + m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) + m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
  out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) + m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) + m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
  out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) + m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) + m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
  out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) + m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) + m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
  out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) + m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) + m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
  out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) + m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) + m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
  out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) + m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) + m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));
#undef m
#undef out
  return true;
}
//! Inverts a primitive matrix which only contains a translation and a rotation
//! \param out: where result matrix is written to.

CC_INLINE bool M4_getInversePrimitive(const float* m, float* out)
{
  float16 m1;

  if (m == out) {
    MEMCPY(m1, m, 16);
    m = m1;
  }

  out[0 ] = m[0];
  out[1 ] = m[4];
  out[2 ] = m[8];
  out[3 ] = 0;
  out[4 ] = m[1];
  out[5 ] = m[5];
  out[6 ] = m[9];
  out[7 ] = 0;
  out[8 ] = m[2];
  out[9 ] = m[6];
  out[10] = m[10];
  out[11] = 0;
  out[12] = (f32) - (m[12] * m[0] + m[13] * m[1] + m[14] * m[2]);
  out[13] = (f32) - (m[12] * m[4] + m[13] * m[5] + m[14] * m[6]);
  out[14] = (f32) - (m[12] * m[8] + m[13] * m[9] + m[14] * m[10]);
  out[15] = 1;
  return true;
}

CC_INLINE void M4_setx(float* m, f32 scalar)
{
  int i;

  for (i = 0; i < 16; ++i) {
    m[i] = scalar;
  }

  return ;
}

CC_INLINE bool M4_equals(const float* m, float* m1)
{
  int i;

  for (i = 0; i < 16; ++i) {
    if (m[i] != m1[i]) {
      return false;
    }
  }

  return true;
}
// Builds a right-handed perspective projection matrix based on a field of view
CC_INLINE void M4_buildProjectionMatrixPerspectiveFovRH(float* m, f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
{
  const f64 h = reciprocalf(tan(fieldOfViewRadians * 0.5));
  const f32 w = (f32)(h / aspectRatio);
  _IRR_DEBUG_BREAK_IF(aspectRatio == 0.f); //divide by zero
  _IRR_DEBUG_BREAK_IF(zNear == zFar); //divide by zero
  m[0] = w;
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;
  m[4] = 0;
  m[5] = (f32)h;
  m[6] = 0;
  m[7] = 0;
  m[8] = 0;
  m[9] = 0;
  m[10] = (f32)(zFar / (zNear - zFar)); // DirectX version
  //    m[10] = (f32)(zFar+zNear/(zNear-zFar)); // OpenGL version
  m[11] = -1;
  m[12] = 0;
  m[13] = 0;
  m[14] = (f32)(zNear * zFar / (zNear - zFar)); // DirectX version
  //    m[14] = (f32)(2.0f*zNear*zFar/(zNear-zFar)); // OpenGL version
  m[15] = 0;
  return ;
}
// Builds a left-handed perspective projection matrix based on a field of view

CC_INLINE void M4_buildProjectionMatrixPerspectiveFovLH(float* m, f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
{
  const f64 h = reciprocalf(tan(fieldOfViewRadians * 0.5));
  const f32 w = (f32)(h / aspectRatio);
  _IRR_DEBUG_BREAK_IF(aspectRatio == 0.f); //divide by zero
  _IRR_DEBUG_BREAK_IF(zNear == zFar); //divide by zero
  m[0] = w;
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;
  m[4] = 0;
  m[5] = (f32)h;
  m[6] = 0;
  m[7] = 0;
  m[8] = 0;
  m[9] = 0;
  m[10] = (f32)(zFar / (zFar - zNear));
  m[11] = 1;
  m[12] = 0;
  m[13] = 0;
  m[14] = (f32)(-zNear * zFar / (zFar - zNear));
  m[15] = 0;
  return ;
}
// Builds a left-handed perspective projection matrix based on a field of view, with far plane culling at infinity

CC_INLINE void M4_buildProjectionMatrixPerspectiveFovInfinityLH(float* m, f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 epsilon)
{
  const f64 h = reciprocalf(tan(fieldOfViewRadians * 0.5));
  const f32 w = (f32)(h / aspectRatio);
  _IRR_DEBUG_BREAK_IF(aspectRatio == 0.f); //divide by zero
  m[0] = w;
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;
  m[4] = 0;
  m[5] = (f32)h;
  m[6] = 0;
  m[7] = 0;
  m[8] = 0;
  m[9] = 0;
  m[10] = (f32)(1.f - epsilon);
  m[11] = 1;
  m[12] = 0;
  m[13] = 0;
  m[14] = (f32)(zNear * (epsilon - 1.f));
  m[15] = 0;
  return ;
}
// Builds a left-handed orthogonal projection matrix.

CC_INLINE void M4_buildProjectionMatrixOrthoLH(float* m, f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
{
  _IRR_DEBUG_BREAK_IF(widthOfViewVolume == 0.f); //divide by zero
  _IRR_DEBUG_BREAK_IF(heightOfViewVolume == 0.f); //divide by zero
  _IRR_DEBUG_BREAK_IF(zNear == zFar); //divide by zero
  m[0] = (f32)(2 / widthOfViewVolume);
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;
  m[4] = 0;
  m[5] = (f32)(2 / heightOfViewVolume);
  m[6] = 0;
  m[7] = 0;
  m[8] = 0;
  m[9] = 0;
  m[10] = (f32)(1 / (zFar - zNear));
  m[11] = 0;
  m[12] = 0;
  m[13] = 0;
  m[14] = (f32)(zNear / (zNear - zFar));
  m[15] = 1;
  return ;
}
// Builds a right-handed orthogonal projection matrix.

CC_INLINE void M4_buildProjectionMatrixOrthoRH(float* m, f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
{
  _IRR_DEBUG_BREAK_IF(widthOfViewVolume == 0.f); //divide by zero
  _IRR_DEBUG_BREAK_IF(heightOfViewVolume == 0.f); //divide by zero
  _IRR_DEBUG_BREAK_IF(zNear == zFar); //divide by zero
  m[0] = (f32)(2 / widthOfViewVolume);
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;
  m[4] = 0;
  m[5] = (f32)(2 / heightOfViewVolume);
  m[6] = 0;
  m[7] = 0;
  m[8] = 0;
  m[9] = 0;
  m[10] = (f32)(1 / (zNear - zFar));
  m[11] = 0;
  m[12] = 0;
  m[13] = 0;
  m[14] = (f32)(zNear / (zNear - zFar));
  m[15] = 1;
#if defined ( USE_MATRIX_TEST )

#endif
  return ;
}
// Builds a right-handed perspective projection matrix.

CC_INLINE void M4_buildProjectionMatrixPerspectiveRH(float* m, f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
{
  _IRR_DEBUG_BREAK_IF(widthOfViewVolume == 0.f); //divide by zero
  _IRR_DEBUG_BREAK_IF(heightOfViewVolume == 0.f); //divide by zero
  _IRR_DEBUG_BREAK_IF(zNear == zFar); //divide by zero
  m[0] = (f32)(2 * zNear / widthOfViewVolume);
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;
  m[4] = 0;
  m[5] = (f32)(2 * zNear / heightOfViewVolume);
  m[6] = 0;
  m[7] = 0;
  m[8] = 0;
  m[9] = 0;
  m[10] = (f32)(zFar / (zNear - zFar));
  m[11] = -1;
  m[12] = 0;
  m[13] = 0;
  m[14] = (f32)(zNear * zFar / (zNear - zFar));
  m[15] = 0;
  return ;
}
// Builds a left-handed perspective projection matrix.

CC_INLINE void M4_buildProjectionMatrixPerspectiveLH(float* m, f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
{
  _IRR_DEBUG_BREAK_IF(widthOfViewVolume == 0.f); //divide by zero
  _IRR_DEBUG_BREAK_IF(heightOfViewVolume == 0.f); //divide by zero
  _IRR_DEBUG_BREAK_IF(zNear == zFar); //divide by zero
  m[0] = (f32)(2 * zNear / widthOfViewVolume);
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;
  m[4] = 0;
  m[5] = (f32)(2 * zNear / heightOfViewVolume);
  m[6] = 0;
  m[7] = 0;
  m[8] = 0;
  m[9] = 0;
  m[10] = (f32)(zFar / (zFar - zNear));
  m[11] = 1;
  m[12] = 0;
  m[13] = 0;
  m[14] = (f32)(zNear * zFar / (zNear - zFar));
  m[15] = 0;
  return ;
}
// Builds a matrix that flattens geometry into a plane.
CC_INLINE void M4_buildShadowMatrix(float* m, const float3 light, const float3 plane, f32 point)
{
  float3 normal;
  f32 d;
  V3_norm(normal, plane);
  d = VDOT3(normal, light);
  m[ 0] = (f32)(-normal[0] * light[0] + d);
  m[ 1] = (f32)(-normal[0] * light[1]);
  m[ 2] = (f32)(-normal[0] * light[2]);
  m[ 3] = (f32)(-normal[0] * point);
  m[ 4] = (f32)(-normal[1] * light[0]);
  m[ 5] = (f32)(-normal[1] * light[1] + d);
  m[ 6] = (f32)(-normal[1] * light[2]);
  m[ 7] = (f32)(-normal[1] * point);
  m[ 8] = (f32)(-normal[2] * light[0]);
  m[ 9] = (f32)(-normal[2] * light[1]);
  m[10] = (f32)(-normal[2] * light[2] + d);
  m[11] = (f32)(-normal[2] * point);
  m[12] = (f32)(-plane[3] * light[0]);
  m[13] = (f32)(-plane[3] * light[1]);
  m[14] = (f32)(-plane[3] * light[2]);
  m[15] = (f32)(-plane[3] * point + d);
  return ;
}
// Builds a left-handed look-at matrix.
CC_INLINE void M4_buildCameraLookAtMatrixLH(float* m, const float* position, const float* target, const float* upVector)
{
  float3 zaxis, yaxis, xaxis;
  VSUB3(zaxis, target, position);
  V3_norm(zaxis, zaxis);
  VCROSS3(xaxis, upVector, zaxis);
  V3_norm(xaxis, xaxis);
  VCROSS3(yaxis, zaxis, xaxis);
  m[0] = (f32)xaxis[0];
  m[1] = (f32)yaxis[0];
  m[2] = (f32)zaxis[0];
  m[3] = 0;
  m[4] = (f32)xaxis[1];
  m[5] = (f32)yaxis[1];
  m[6] = (f32)zaxis[1];
  m[7] = 0;
  m[8] = (f32)xaxis[2];
  m[9] = (f32)yaxis[2];
  m[10] = (f32)zaxis[2];
  m[11] = 0;
  m[12] = (f32) - VDOT3(xaxis, position);
  m[13] = (f32) - VDOT3(yaxis, position);
  m[14] = (f32) - VDOT3(zaxis, position);
  m[15] = 1;
  return ;
}
// Builds a right-handed look-at matrix.

CC_INLINE void M4_buildCameraLookAtMatrixRH(float* m, const float* position, const float* target, const float* upVector)
{
  float3 zaxis, yaxis, xaxis;
  VSUB3(zaxis, position, target);
  V3_norm(zaxis, zaxis);
  VCROSS3(xaxis, upVector, zaxis);
  V3_norm(xaxis, xaxis);
  VCROSS3(yaxis, zaxis, xaxis);
  m[0] = (f32)xaxis[0];
  m[1] = (f32)yaxis[0];
  m[2] = (f32)zaxis[0];
  m[3] = 0;
  m[4] = (f32)xaxis[1];
  m[5] = (f32)yaxis[1];
  m[6] = (f32)zaxis[1];
  m[7] = 0;
  m[8] = (f32)xaxis[2];
  m[9] = (f32)yaxis[2];
  m[10] = (f32)zaxis[2];
  m[11] = 0;
  m[12] = (f32) - VDOT3(xaxis, position);
  m[13] = (f32) - VDOT3(yaxis, position);
  m[14] = (f32) - VDOT3(zaxis, position);
  m[15] = 1;
  return ;
}
// creates a new matrix as interpolated matrix from this and the passed one.
CC_INLINE void M4_interpolate(float* mat, const float* m, const float* b, f32 time)
{
  int i;

  for (i = 0; i < 16; i += 4) {
    mat[i + 0] = (f32)(m[i + 0] + (b[i + 0] - m[i + 0]) * time);
    mat[i + 1] = (f32)(m[i + 1] + (b[i + 1] - m[i + 1]) * time);
    mat[i + 2] = (f32)(m[i + 2] + (b[i + 2] - m[i + 2]) * time);
    mat[i + 3] = (f32)(m[i + 3] + (b[i + 3] - m[i + 3]) * time);
  }
}
// returns transposed matrix
CC_INLINE void M4_getTransposed(float* o, const float* m)
{
  float16 m1;

  if (m == o) {
    MEMCPY(m1, m, 16);
    m = m1;
  }

  o[ 0] = m[ 0];
  o[ 1] = m[ 4];
  o[ 2] = m[ 8];
  o[ 3] = m[12];
  o[ 4] = m[ 1];
  o[ 5] = m[ 5];
  o[ 6] = m[ 9];
  o[ 7] = m[13];
  o[ 8] = m[ 2];
  o[ 9] = m[ 6];
  o[10] = m[10];
  o[11] = m[14];
  o[12] = m[ 3];
  o[13] = m[ 7];
  o[14] = m[11];
  o[15] = m[15];
}
// used to scale <-1,-1><1,1> to viewport

CC_INLINE void M4_buildNDCToDCMatrix(float* m, IRECT* viewport, f32 zScale)
{
  float3 scale;
  const f32 scaleX = (RCW(viewport) - 0.75f) * 0.5f;
  const f32 scaleY = -(RCH(viewport) - 0.75f) * 0.5f;
  const f32 dx = -0.5f + ((viewport->l + viewport->r) * 0.5f);
  const f32 dy = -0.5f + ((viewport->t + viewport->b) * 0.5f);
  M4_makeIdentity(m);
  m[12] = (f32)dx;
  m[13] = (f32)dy;
  VSET3(scale, (f32)scaleX, (f32)scaleY, (f32)zScale);
  M4_setScale(m, scale);
  return ;
}
//! Builds a matrix that rotates from one vector to another
/** \param from: vector to rotate from
\param to: vector to rotate to
  http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm
 */

CC_INLINE void M4_buildRotateFromTo(float* m, const float* from, const float* to)
{
  // unit vectors
  float3 f, t, vs, v, vt;
  f32 ca, ca1;
  VSET3V(f, from);
  VSET3V(t, to);
  V3_norm(f, f);
  V3_norm(t, t);
  // axis multiplication by sin
  VCROSS3(vs, t, f);
  // axis of rotation
  VSET3V(v, vs);
  V3_norm(v, v);
  // cosinus angle
  ca = VDOT3(f, t);
  ca1 = 1 - ca;
  VSCALE3(vt, v, ca1);
  m[0] = vt[0] * v[0] + ca;
  m[5] = vt[1] * v[1] + ca;
  m[10] = vt[2] * v[2] + ca;
  vt[0] *= v[1];
  vt[2] *= v[0];
  vt[1] *= v[2];
  m[1] = vt[0] - vs[2];
  m[2] = vt[2] + vs[1];
  m[3] = 0;
  m[4] = vt[0] + vs[2];
  m[6] = vt[1] - vs[0];
  m[7] = 0;
  m[8] = vt[2] - vs[1];
  m[9] = vt[1] + vs[0];
  m[11] = 0;
  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
  return ;
}
//! Builds a combined matrix which translate to a center before rotation and translate afterwards
CC_INLINE void M4_setRotationCenter(float* m, const float* center, const float* translation)
{
  m[12] = -m[0] * center[0] - m[4] * center[1] - m[8] * center[2] + (center[0] - translation[0]);
  m[13] = -m[1] * center[0] - m[5] * center[1] - m[9] * center[2] + (center[1] - translation[1]);
  m[14] = -m[2] * center[0] - m[6] * center[1] - m[10] * center[2] + (center[2] - translation[2]);
  m[15] = (f32) 1.0;
}
//! Builds a matrix which rotates a source vector to a look vector over an arbitrary axis
/** \param camPos: viewer position in world coord
\param center: object position in world-coord, rotation pivot
\param translation: object final translation from center
\param axis: axis to rotate about
\param from: source vector to rotate from
 */

CC_INLINE void M4_buildAxisAlignedBillboard(float* m, const float* camPos, const float* center, const float* translation, const float* axis, const float* from)
{
  // axis of rotation
  float3 up, forward, right, look, vs, vt;
  f32 ca, ca1;
  VSET3V(up, axis);
  V3_norm(up, up);
  VSUB3(forward, camPos, center);
  V3_norm(forward, forward);
  VCROSS3(right, up, forward);
  V3_norm(right, right);
  // correct look vector
  VCROSS3(look, right, up);
  // rotate from to
  // axis multiplication by sin
  VCROSS3(vs, look, from);
  // cosinus angle
  ca = VDOT3(from, look);
  ca1 = (1.f - ca);
  VSCALE3(vt, up, ca1);
  m[0] = (f32)(vt[0] * up[0] + ca);
  m[5] = (f32)(vt[1] * up[1] + ca);
  m[10] = (f32)(vt[2] * up[2] + ca);
  vt[0] *= up[1];
  vt[2] *= up[0];
  vt[1] *= up[2];
  m[1] = (f32)(vt[0] - vs[2]);
  m[2] = (f32)(vt[2] + vs[1]);
  m[3] = 0;
  m[4] = (f32)(vt[0] + vs[2]);
  m[6] = (f32)(vt[1] - vs[0]);
  m[7] = 0;
  m[8] = (f32)(vt[2] - vs[1]);
  m[9] = (f32)(vt[1] + vs[0]);
  m[11] = 0;
  M4_setRotationCenter(m, center, translation);
}
/*!
  Generate texture coordinates as linear functions so that:
    u = Ux*x + Uy*y + Uz*z + Uw
    v = Vx*x + Vy*y + Vz*z + Vw
  The matrix m for this case is:
    Ux  Vx  0  0
    Uy  Vy  0  0
    Uz  Vz  0  0
    Uw  Vw  0  0
*/

CC_INLINE void M4_buildTextureTransform(float* m, f32 rotateRad, const float2 rotatecenter, const float2 translate, const float2 scale)
{
  const f32 c = cosf(rotateRad);
  const f32 s = sinf(rotateRad);
  m[0] = (f32)(c * scale[0]);
  m[1] = (f32)(s * scale[1]);
  m[2] = 0;
  m[3] = 0;
  m[4] = (f32)(-s * scale[0]);
  m[5] = (f32)(c * scale[1]);
  m[6] = 0;
  m[7] = 0;
  m[8] = (f32)(c * scale[0] * rotatecenter[0] + -s * rotatecenter[1] + translate[0]);
  m[9] = (f32)(s * scale[1] * rotatecenter[0] +  c * rotatecenter[1] + translate[1]);
  m[10] = 1;
  m[11] = 0;
  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
  return ;
}
// rotate about z axis, center ( 0.5, 0.5 )
CC_INLINE void M4_setTextureRotationCenter(float* m, f32 rotateRad)
{
  const f32 c = cosf(rotateRad);
  const f32 s = sinf(rotateRad);
  m[0] = (f32)c;
  m[1] = (f32)s;
  m[4] = (f32) - s;
  m[5] = (f32)c;
  m[8] = (f32)(0.5f * (s - c) + 0.5f);
  m[9] = (f32)(-0.5f * (s + c) + 0.5f);
  return ;
}

CC_INLINE void M4_setTextureTranslate(float* m, f32 x, f32 y)
{
  m[8] = (f32)x;
  m[9] = (f32)y;
  return ;
}

CC_INLINE void M4_setTextureTranslateTransposed(float* m, f32 x, f32 y)
{
  m[2] = (f32)x;
  m[6] = (f32)y;
  return ;
}

CC_INLINE void M4_setTextureScale(float* m, f32 sx, f32 sy)
{
  m[0] = (f32)sx;
  m[5] = (f32)sy;
  return ;
}

CC_INLINE void M4_setTextureScaleCenter(float* m, f32 sx, f32 sy)
{
  m[0] = (f32)sx;
  m[5] = (f32)sy;
  m[8] = (f32)(0.5f - 0.5f * sx);
  m[9] = (f32)(0.5f - 0.5f * sy);
  return ;
}
// sets all matrix data members at once

CC_INLINE void M4_setM(float* m, const f32* data)
{
  memcpy(m, data, 16 * sizeof(f32));
  return ;
}
// Multiply by scalar.
//! Transforms a plane by this matrix
CC_INLINE void M4_transformPlane(const float* m, float3 plane)
{
  float3 member;
  float* normal = plane;
  float16 transposedInverse;
  // Transform the plane member point, i.e. rotate, translate and scale it.
  getMemberPoint(plane, member);
  M4_transformV3(m, member, member);
  // Transform the normal by the transposed inverse of the matrix
  M4_getInverse(m, transposedInverse);
  M4_getTransposed(transposedInverse, transposedInverse);
  M4_transformV3(transposedInverse, normal, normal);
  plane[3] = recalculateD(plane, member);
}
//! Transforms a axis aligned bounding box
CC_INLINE void M4_transformBox(const float* m, FAABBOX3* box)
{
  M4_transformV3(m, box->a, box->a);
  M4_transformV3(m, box->b, box->b);
  FAABBOX3_repair(box);
}
//! Transforms a axis aligned bounding box more accurately than transformBox()
CC_INLINE void M4_transformBoxEx(const float* m, FAABBOX3* box)
{
  const f32 Amin[3] = {box->a[0], box->a[1], box->a[2]};
  const f32 Amax[3] = {box->b[0], box->b[1], box->b[2]};
  f32 Bmin[3];
  f32 Bmax[3];
  int i, j;
  Bmin[0] = Bmax[0] = m[12];
  Bmin[1] = Bmax[1] = m[13];
  Bmin[2] = Bmax[2] = m[14];

  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) {
      const f32 a = m[j * 4 + i] * Amin[j];
      const f32 b = m[j * 4 + i] * Amax[j];

      if (a < b) {
        Bmin[i] += a;
        Bmax[i] += b;
      }
      else {
        Bmin[i] += b;
        Bmax[i] += a;
      }
    }
  }

  box->a[0] = Bmin[0];
  box->a[1] = Bmin[1];
  box->a[2] = Bmin[2];
  box->b[0] = Bmax[0];
  box->b[1] = Bmax[1];
  box->b[2] = Bmax[2];
}

#endif