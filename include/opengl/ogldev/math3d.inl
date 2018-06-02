
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))
void mul3fv1f(float3 v, float s)
{
  v[0] *= s;
  v[1] *= s;
  v[2] *= s;
}
void add3fv3fv(float3 v, float3 a, float3 b)
{
  v[0] = a[0] + b[0];
  v[1] = a[1] + b[1];
  v[2] = a[2] + b[2];
}
void sub3fv3fv(float3 v, float3 a, float3 b)
{
  v[0] = a[0] - b[0];
  v[1] = a[1] - b[1];
  v[2] = a[2] - b[2];
}
void Cross3fv(float3 c, const float3 a, const float3 v)
{
  float x = a[1] * v[2] - a[2] * v[1];
  float y = a[2] * v[0] - a[0] * v[2];
  float z = a[0] * v[1] - a[1] * v[0];
  c[0] = x;
  c[1] = y;
  c[2] = z;
}
void Normalize3fv(float3 v, const float3 a)
{
  const float Length = sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
  v[0] = a[0] / Length;
  v[1] = a[1] / Length;
  v[2] = a[2] / Length;
}
void setf4(float4 a, float _x, float _y, float _z, float _w)
{
  a[0] = _x;
  a[1] = _y;
  a[2] = _z;
  a[3] = _w;
}
void Normalize4fv(float4 v, const float4 a)
{
  float Length = sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3]);
  v[0] = a[0] / Length;
  v[1] = a[1] / Length;
  v[2] = a[2] / Length;
  v[3] = a[3] / Length;
}
void Conjugate4fv(float4 ret, const float4 a)
{
  setf4(ret, -a[0], -a[1], -a[2], a[3]);
}
void mul4fv4fv(float4 ret, const float4 l, const float4 r)
{
  const float w = (l[3] * r[3]) - (l[0] * r[0]) - (l[1] * r[1]) - (l[2] * r[2]);
  const float x = (l[0] * r[3]) + (l[3] * r[0]) + (l[1] * r[2]) - (l[2] * r[1]);
  const float y = (l[1] * r[3]) + (l[3] * r[1]) + (l[2] * r[0]) - (l[0] * r[2]);
  const float z = (l[2] * r[3]) + (l[3] * r[2]) + (l[0] * r[1]) - (l[1] * r[0]);
  setf4(ret, x, y, z, w);
}
void mul4fv3fv(float4 ret, const float4 q, const float3 v)
{
  const float w = - (q[0] * v[0]) - (q[1] * v[1]) - (q[2] * v[2]);
  const float x = (q[3] * v[0]) + (q[1] * v[2]) - (q[2] * v[1]);
  const float y = (q[3] * v[1]) + (q[2] * v[0]) - (q[0] * v[2]);
  const float z = (q[3] * v[2]) + (q[0] * v[1]) - (q[1] * v[0]);
  setf4(ret, x, y, z, w);
}
void ToDegrees4f(float3 f, float x, float y, float z, float w)
{
  f[0] = atan2(x * z + y * w, x * w - y * z);
  f[1] = acos(-x * x - y * y - z * z - w * w);
  f[2] = atan2(x * z - y * w, x * w + y * z);
  f[0] = ToDegree(f[0]);
  f[1] = ToDegree(f[1]);
  f[2] = ToDegree(f[2]);
}
void ToDegrees4fv(float3 f, float3 v)
{
  ToDegrees4f(f, v[0], v[1], v[2], v[3]);
}
void Rotate(float3 ret, const float3 v, float Angle, const float3 Axe)
{
  const float SinHalfAngle = sinf(ToRadian(Angle / 2));
  const float CosHalfAngle = cosf(ToRadian(Angle / 2));
  const float Rx = Axe[0] * SinHalfAngle;
  const float Ry = Axe[1] * SinHalfAngle;
  const float Rz = Axe[2] * SinHalfAngle;
  const float Rw = CosHalfAngle;
  float4 ConjugateQ, W, RotationQ = {Rx, Ry, Rz, Rw};
  Conjugate4fv(ConjugateQ, RotationQ);
  // ConjugateQ.Normalize();
  // float4 W = RotationQ * v * ConjugateQ;
  mul4fv3fv(W, RotationQ, v);
  mul4fv4fv(W, W, ConjugateQ);
  ret[0] = W[0];
  ret[1] = W[1];
  ret[2] = W[2];
}
void set44f(float44 m, float a00, float a01, float a02, float a03,
    float a10, float a11, float a12, float a13,
    float a20, float a21, float a22, float a23,
    float a30, float a31, float a32, float a33)
{
  m[0][0] = a00, m[0][1] = a01, m[0][2] = a02, m[0][3] = a03;
  m[1][0] = a10, m[1][1] = a11, m[1][2] = a12, m[1][3] = a13;
  m[2][0] = a20, m[2][1] = a21, m[2][2] = a22, m[2][3] = a23;
  m[3][0] = a30, m[3][1] = a31, m[3][2] = a32, m[3][3] = a33;
}
void set44fv(float44 m, const float44 a)
{
  memcpy(m, a, sizeof(float44));
}
void set33fv(float33 m, const float33 a)
{
  memcpy(m, a, sizeof(float33));
}
void SetZero(float44 m)
{
  memset(m, 0, sizeof(float44));
}
void mul33fv33fvImpl(float33 c, const float33 a, const float33 b)
{
  int i, j;
  for (i = 0 ; i < 3 ; i++) {
    for (j = 0 ; j < 3 ; j++) {
      c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j];
    }
  }
}
void mul33fv33fv(float33 Ret, const float33 Left, const float33 Right)
{
  if (Ret == Left || Ret == Right) {
    float33 tmp;
    mul33fv33fvImpl(tmp, Left, Right);
    set33fv(Ret, tmp);
  }
  else {
    mul33fv33fvImpl(Ret, Left, Right);
  }
}
void mul33fv3f(float3 r, const float33 m, float x, float y, float z)
{
  r[0] = m[0][0] * x + m[0][1] * y + m[0][2] * z;
  r[1] = m[1][0] * x + m[1][1] * y + m[1][2] * z;
  r[2] = m[2][0] * x + m[2][1] * y + m[2][2] * z;
}
void mul33fv3fv(float3 r, const float33 m, const float3 v)
{
  mul33fv3f(r, m, v[0], v[1], v[2]);
}

void mul44fv44fvImpl(float44 c, const float44 a, const float44 b)
{
  int i, j;
  for (i = 0 ; i < 4 ; i++) {
    for (j = 0 ; j < 4 ; j++) {
      c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j] + a[i][3] * b[3][j];
    }
  }
}
void mul44fv44fv(float44 Ret, const float44 Left, const float44 Right)
{
  if (Ret == Left || Ret == Right) {
    float44 tmp;
    mul44fv44fvImpl(tmp, Left, Right);
    set44fv(Ret, tmp);
  }
  else {
    mul44fv44fvImpl(Ret, Left, Right);
  }
}
void mul44fv4f(float4 r, const float44 m, float x, float y, float z, float w)
{
  r[0] = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3] * w;
  r[1] = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3] * w;
  r[2] = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3] * w;
  r[3] = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3] * w;
}
void mul44fv4fv(float4 r, const float44 m, const float4 v)
{
  mul44fv4f(r, m, v[0], v[1], v[2], v[3]);
}
//void MultiplyPoint(const float44 m, const float4 v, float4 r) {  mul44fv4f(r, m, v[0], v[1], v[2], v[3]);}
void Print44fv(const float44 m)
{
  int i;
  for (i = 0 ; i < 4 ; i++) {
    printf("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
  }
}
void InitScaleTransform3f(float44 m, float ScaleX, float ScaleY, float ScaleZ)
{
  m[0][0] = ScaleX, m[0][1] = 0.0f, m[0][2] = 0.0f, m[0][3] = 0.0f;
  m[1][0] = 0.0f, m[1][1] = ScaleY, m[1][2] = 0.0f, m[1][3] = 0.0f;
  m[2][0] = 0.0f, m[2][1] = 0.0f, m[2][2] = ScaleZ, m[2][3] = 0.0f;
  m[3][0] = 0.0f, m[3][1] = 0.0f, m[3][2] = 0.0f, m[3][3] = 1.0f;
}
void ScaleTransform3f(float44 m, float ScaleX, float ScaleY, float ScaleZ)
{
  float44 s;
  InitScaleTransform3f(s, ScaleX, ScaleY, ScaleZ);
  mul44fv44fv(m, m, s);
}
void InitRotateTransform(float44 Ret, float RotateX, float RotateY, float RotateZ)
{
  float44 rx, ry, rz;
  const float x = ToRadian(RotateX);
  const float y = ToRadian(RotateY);
  const float z = ToRadian(RotateZ);
  rx[0][0] = 1.0f, rx[0][1] = 0.0f , rx[0][2] = 0.0f , rx[0][3] = 0.0f;
  rx[1][0] = 0.0f, rx[1][1] = cosf(x), rx[1][2] = -sinf(x), rx[1][3] = 0.0f;
  rx[2][0] = 0.0f, rx[2][1] = sinf(x), rx[2][2] = cosf(x) , rx[2][3] = 0.0f;
  rx[3][0] = 0.0f, rx[3][1] = 0.0f , rx[3][2] = 0.0f , rx[3][3] = 1.0f;
  ry[0][0] = cosf(y), ry[0][1] = 0.0f, ry[0][2] = -sinf(y), ry[0][3] = 0.0f;
  ry[1][0] = 0.0f , ry[1][1] = 1.0f, ry[1][2] = 0.0f , ry[1][3] = 0.0f;
  ry[2][0] = sinf(y), ry[2][1] = 0.0f, ry[2][2] = cosf(y) , ry[2][3] = 0.0f;
  ry[3][0] = 0.0f , ry[3][1] = 0.0f, ry[3][2] = 0.0f , ry[3][3] = 1.0f;
  rz[0][0] = cosf(z), rz[0][1] = -sinf(z), rz[0][2] = 0.0f, rz[0][3] = 0.0f;
  rz[1][0] = sinf(z), rz[1][1] = cosf(z) , rz[1][2] = 0.0f, rz[1][3] = 0.0f;
  rz[2][0] = 0.0f , rz[2][1] = 0.0f , rz[2][2] = 1.0f, rz[2][3] = 0.0f;
  rz[3][0] = 0.0f , rz[3][1] = 0.0f , rz[3][2] = 0.0f, rz[3][3] = 1.0f;
  // this = rz * ry * rx;
  mul44fv44fv(Ret, rz, ry);
  mul44fv44fv(Ret, Ret, rx);
}
void RotateTransform(float44 m, float RotateX, float RotateY, float RotateZ)
{
  float44 s;
  InitRotateTransform(s, RotateX, RotateY, RotateZ);
  mul44fv44fv(m, m, s);
}
void InitRotateTransform2(float44 m, const float4 quat)
{
  float yy2 = 2.0f * quat[1] * quat[1];
  float xy2 = 2.0f * quat[0] * quat[1];
  float xz2 = 2.0f * quat[0] * quat[2];
  float yz2 = 2.0f * quat[1] * quat[2];
  float zz2 = 2.0f * quat[2] * quat[2];
  float wz2 = 2.0f * quat[3] * quat[2];
  float wy2 = 2.0f * quat[3] * quat[1];
  float wx2 = 2.0f * quat[3] * quat[0];
  float xx2 = 2.0f * quat[0] * quat[0];
  m[0][0] = - yy2 - zz2 + 1.0f;
  m[0][1] = xy2 + wz2;
  m[0][2] = xz2 - wy2;
  m[0][3] = 0;
  m[1][0] = xy2 - wz2;
  m[1][1] = - xx2 - zz2 + 1.0f;
  m[1][2] = yz2 + wx2;
  m[1][3] = 0;
  m[2][0] = xz2 + wy2;
  m[2][1] = yz2 - wx2;
  m[2][2] = - xx2 - yy2 + 1.0f;
  m[2][3] = 0.0f;
  m[3][0] = m[3][1] = m[3][2] = 0;
  m[3][3] = 1.0f;
}
void InitTranslationTransform(float44 m, float x, float y, float z)
{
  m[0][0] = 1.0f, m[0][1] = 0.0f, m[0][2] = 0.0f, m[0][3] = x;
  m[1][0] = 0.0f, m[1][1] = 1.0f, m[1][2] = 0.0f, m[1][3] = y;
  m[2][0] = 0.0f, m[2][1] = 0.0f, m[2][2] = 1.0f, m[2][3] = z;
  m[3][0] = 0.0f, m[3][1] = 0.0f, m[3][2] = 0.0f, m[3][3] = 1.0f;
}
void TranslationTransform(float44 m, float x, float y, float z)
{
  float44 s;
  InitTranslationTransform(s, x, y, z);
  mul44fv44fv(m, m, s);
}
void InitCameraTransform(float44 m, const float3 Target, const float3 Up)
{
  float3 N, U, V;
  Normalize3fv(N, Target);
  Cross3fv(U, Up, N);
  Normalize3fv(U, U);
  Cross3fv(V, N, U);
  m[0][0] = U[0], m[0][1] = U[1], m[0][2] = U[2], m[0][3] = 0.0f;
  m[1][0] = V[0], m[1][1] = V[1], m[1][2] = V[2], m[1][3] = 0.0f;
  m[2][0] = N[0], m[2][1] = N[1], m[2][2] = N[2], m[2][3] = 0.0f;
  m[3][0] = 0.0f, m[3][1] = 0.0f, m[3][2] = 0.0f, m[3][3] = 1.0f;
}
void InitCameraTransform3f3f(float44 m, float TargetX, float TargetY, float TargetZ, float UpX, float UpY, float UpZ)
{
  float3 Target = {TargetX, TargetY, TargetZ};
  float3 Up = {UpX, UpY, UpZ};
  InitCameraTransform(m, Target, Up);
}
struct PersProjInfo {
  float FOV;
  float Width;
  float Height;
  float zNear;
  float zFar;
};
struct OrthoProjInfo {
  float r; // right
  float l; // left
  float b; // bottom
  float t; // top
  float n; // z near
  float f; // z far
};
void InitPersProjTransform(float44 m, const struct PersProjInfo* p)
{
  const float ar = p->Width / p->Height;
  const float zRange = p->zNear - p->zFar;
  const float tanHalfFOV = tanf(ToRadian(p->FOV / 2.0f));
  m[0][0] = 1.0f / (tanHalfFOV * ar), m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = 0.0;
  m[1][0] = 0.0f;
  m[1][1] = 1.0f / tanHalfFOV, m[1][2] = 0.0f;
  m[1][3] = 0.0;
  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = (-p->zNear - p->zFar) / zRange , m[2][3] = 2.0f * p->zFar * p->zNear / zRange;
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 1.0f;
  m[3][3] = 0.0;
}
void InitOrthoProjTransform(float44 m, const struct OrthoProjInfo* p)
{
  float l = p->l;
  float r = p->r;
  float b = p->b;
  float t = p->t;
  float n = p->n;
  float f = p->f;
  m[0][0] = 2.0f / (r - l), m[0][1] = 0.0f, m[0][2] = 0.0f, m[0][3] = -(r + l) / (r - l);
  m[1][0] = 0.0f, m[1][1] = 2.0f / (t - b), m[1][2] = 0.0f, m[1][3] = -(t + b) / (t - b);
  m[2][0] = 0.0f, m[2][1] = 0.0f, m[2][2] = 2.0f / (f - n), m[2][3] = -(f + n) / (f - n);
  m[3][0] = 0.0f, m[3][1] = 0.0f, m[3][2] = 0.0f, m[3][3] = 1.0;
}
float Determinant44(const float44 m)
{
  return m[0][0] * m[1][1] * m[2][2] * m[3][3] - m[0][0] * m[1][1] * m[2][3] * m[3][2] + m[0][0] * m[1][2] * m[2][3] * m[3][1] - m[0][0] * m[1][2] * m[2][1] * m[3][3]
      + m[0][0] * m[1][3] * m[2][1] * m[3][2] - m[0][0] * m[1][3] * m[2][2] * m[3][1] - m[0][1] * m[1][2] * m[2][3] * m[3][0] + m[0][1] * m[1][2] * m[2][0] * m[3][3]
      - m[0][1] * m[1][3] * m[2][0] * m[3][2] + m[0][1] * m[1][3] * m[2][2] * m[3][0] - m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][1] * m[1][0] * m[2][3] * m[3][2]
      + m[0][2] * m[1][3] * m[2][0] * m[3][1] - m[0][2] * m[1][3] * m[2][1] * m[3][0] + m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][2] * m[1][0] * m[2][3] * m[3][1]
      + m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][2] * m[1][1] * m[2][0] * m[3][3] - m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][3] * m[1][0] * m[2][2] * m[3][1]
      - m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][3] * m[1][2] * m[2][1] * m[3][0];
}
float Inverse44fvImpl(float44 res, const float44 m)
{
  // Compute the reciprocal determinant
  float invdet;
  float det = Determinant44(m);
  if (det == 0.0f) {
    // Matrix not invertible. Setting all elements to nan is not really
    // correct in a mathematical sense but it is easy to debug for the
    // programmer.
    /*const float nan = std::numeric_limits<float>::quiet_NaN();
    *this = Matrix4f(
    nan,nan,nan,nan,
    nan,nan,nan,nan,
    nan,nan,nan,nan,
    nan,nan,nan,nan);*/
    assert(0);
    return det;
  }
  invdet = 1.0f / det;
  res[0][0] = invdet * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
  res[0][1] = -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[0][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
  res[0][2] = invdet * (m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][1] - m[1][1] * m[3][3]) + m[0][3] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]));
  res[0][3] = -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][1] - m[1][1] * m[2][3]) + m[0][3] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
  res[1][0] = -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
  res[1][1] = invdet * (m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
  res[1][2] = -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][2] - m[1][2] * m[3][0]));
  res[1][3] = invdet * (m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]));
  res[2][0] = invdet * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[1][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res[2][1] = -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[0][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res[2][2] = invdet * (m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) + m[0][1] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
  res[2][3] = -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) + m[0][1] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
  res[3][0] = -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[1][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res[3][1] = invdet * (m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[0][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[0][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res[3][2] = -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) + m[0][1] * (m[1][2] * m[3][0] - m[1][0] * m[3][2]) + m[0][2] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
  res[3][3] = invdet * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
  return det;
}
float Inverse44fv(float44 res, const float44 m)
{
  float det = 0;
  if (res == m) {
    float44 tmp;
    det = Inverse44fvImpl(tmp, m);
    set44fv(res, tmp);
  }
  det = Inverse44fvImpl(res, m);
  return det;
}
float Invert(float44 res, const float44 m) {
  return Inverse44fv(res, m);
}
#define RANDOM rand
float RandomFloat()
{
  float Max = RAND_MAX;
  return ((float)RANDOM() / Max);
}
//////////////////////////////////////
float fract(float x)
{
  return (x - (int)x);
}
union vec2 {
  float2 v;
  struct {
    float x, y;
  };
  vec2() {
    x = y = 0.0f;
  }
  vec2(const float a) {
    x = y = a;
  }
  vec2(const float _x, const float _y) {
    x = _x, y = _y;
  }
  vec2(const float2 _v) {
    v[0] = _v[0];
    v[1] = _v[1];
  }
  vec2& operator *= (float b) {
    v[0] *= b;
    v[1] *= b;
    return *this;
  }
  vec2& operator /= (float b) {
    v[0] /= b;
    v[1] /= b;
    return *this;
  }
  vec2& operator *= (const float2 b) {
    v[0] *= b[0];
    v[1] *= b[1];
    return *this;
  }
  vec2& operator *= (const vec2& b) {
    v[0] *= b.v[0];
    v[1] *= b.v[1];
    return *this;
  }
  vec2& operator += (const vec2& b) {
    v[0] += b.v[0];
    v[1] += b.v[1];
    return *this;
  }
  vec2& operator /= (const vec2& b) {
    v[0] /= b.v[0];
    v[1] /= b.v[1];
    return *this;
  }
  vec2& operator -= (const vec2& b) {
    v[0] -= b.v[0];
    v[1] -= b.v[1];
    return *this;
  }
  vec2& operator -= (float b) {
    v[0] -= b;
    v[1] -= b;
    return *this;
  }
};
union ivec2 {
  int v[2];
  struct {
    int x, y;
  };
  ivec2() {
    x = y = 0;
  }
  ivec2(const int a) {
    x = y = a;
  }
  ivec2(const int _x, const int _y) {
    x = _x, y = _y;
  }
  ivec2(const vec2& _v) {
    x = (int)_v.x;
    y = (int)_v.y;
  }
  ivec2(const int* _v) {
    v[0] = _v[0];
    v[1] = _v[1];
  }
  ivec2& operator *= (int b) {
    v[0] *= b;
    v[1] *= b;
    return *this;
  }
  ivec2& operator *= (const int* b) {
    v[0] *= b[0];
    v[1] *= b[1];
    return *this;
  }
  ivec2& operator *= (const ivec2& b) {
    v[0] *= b.v[0];
    v[1] *= b.v[1];
    return *this;
  }
  ivec2& operator += (const ivec2& b) {
    v[0] += b.v[0];
    v[1] += b.v[1];
    return *this;
  }
  ivec2& operator /= (const ivec2& b) {
    v[0] /= b.v[0];
    v[1] /= b.v[1];
    return *this;
  }
  ivec2& operator -= (const ivec2& b) {
    v[0] -= b.v[0];
    v[1] -= b.v[1];
    return *this;
  }
  ivec2& operator -= (float b) {
    v[0] -= b;
    v[1] -= b;
    return *this;
  }
};
union vec3 {
  float3 v;
  struct {
    float x, y, z;
  };
  struct {
    float r, g, b;
  };
  vec3() {
    x = y = z = 0.f;
  }
  vec3(const float a) {
    x = y = z = a;
  }
  vec3(const float _x, const float _y, const float _z) {
    x = _x, y = _y, z = _z;
  }
  vec3(const float3 _v) {
    v[0] = _v[0];
    v[1] = _v[1];
    v[2] = _v[2];
  }
  vec3& operator *= (float b) {
    v[0] *= b;
    v[1] *= b;
    v[2] *= b;
    return *this;
  }
  vec3& operator += (float b) {
    v[0] += b;
    v[1] += b;
    v[2] += b;
    return *this;
  }
  vec3& operator *= (float2 b) {
    v[0] *= b[0];
    v[1] *= b[1];
    v[2] *= b[2];
    return *this;
  }
  vec3& operator *= (const vec3& b) {
    v[0] *= b.v[0];
    v[1] *= b.v[1];
    v[2] *= b.v[2];
    return *this;
  }
  vec3& operator += (const vec3& b) {
    v[0] += b.v[0];
    v[1] += b.v[1];
    v[2] += b.v[2];
    return *this;
  }
  vec3& operator -= (const vec3& b) {
    v[0] -= b.v[0];
    v[1] -= b.v[1];
    v[2] -= b.v[2];
    return *this;
  }
  vec3& operator /= (const vec3& b) {
    v[0] /= b.v[0];
    v[1] /= b.v[1];
    v[2] /= b.v[2];
    return *this;
  }
};
union vec4 {
  float4 v;
  struct {
    float x, y, z, w;
  };
  struct {
    float r, g, b, a;
  };
  vec4() {
    x = y = z = w = 0.f;
  }
  vec4(const float a) {
    x = y = z = w = a;
  }
  vec4(const float _x, const float _y, const float _z, const float _w) {
    x = _x, y = _y, z = _z, w = _w;
  }
  vec4(const float4 _v) {
    v[0] = _v[0];
    v[1] = _v[1];
    v[2] = _v[2];
    v[3] = _v[3];
  }
  vec3 rgb() const {
    return vec3(r, g, b);
  }
  vec3 rrr() const {
    return vec3(r, r, r);
  }
  vec3 ggg() const {
    return vec3(g, g, g);
  }
  vec3 bbb() const {
    return vec3(b, b, b);
  }
  vec3 aaa() const {
    return vec3(a, a, a);
  }
  vec2 xy() const {
    return vec2(x, y);
  }
  vec4& rgb(const vec3& _v) {
    r = _v.r, g = _v.g, b = _v.b;
    return *this;
  }
  vec3 xyz() const {
    return vec3(x, y, z);
  }
  vec4& operator *= (float b) {
    v[0] *= b;
    v[1] *= b;
    v[2] *= b;
    v[3] *= b;
    return *this;
  }
  vec4& operator *= (float2 b) {
    v[0] *= b[0];
    v[1] *= b[1];
    v[2] *= b[2];
    v[3] *= b[3];
    return *this;
  }
  vec4& operator *= (const vec4& b) {
    v[0] *= b.v[0];
    v[1] *= b.v[1];
    v[2] *= b.v[2];
    v[3] *= b.v[3];
    return *this;
  }
  vec4& operator += (const vec4& b) {
    v[0] += b.v[0];
    v[1] += b.v[1];
    v[2] += b.v[2];
    v[3] += b.v[3];
    return *this;
  }
  vec4& operator -= (const vec4& b) {
    v[0] -= b.v[0];
    v[1] -= b.v[1];
    v[2] -= b.v[2];
    v[3] -= b.v[3];
    return *this;
  }
  vec4& operator /= (const vec4& b) {
    v[0] /= b.v[0];
    v[1] /= b.v[1];
    v[2] /= b.v[2];
    v[3] /= b.v[3];
    return *this;
  }
};
static vec2 operator * (const vec2& a, float b)
{
  return vec2(a) *= b;
}
static vec2 operator / (const vec2& a, float b)
{
  return vec2(a) /= b;
}
static vec2 operator / (const vec2& a, const vec2& b)
{
  return vec2(a) /= b;
}
static vec2 operator + (const vec2& a, const vec2& b)
{
  return vec2(a) += b;
}
static vec2 operator - (const vec2& a, const vec2& b)
{
  return vec2(a) -= b;
}
static vec2 operator - (const vec2& a, float b)
{
  return vec2(a) -= b;
}
static vec3 operator * (const vec3& a, float b)
{
  return vec3(a) *= b;
}
static vec3 operator + (const vec3& a, float b)
{
  return vec3(a) += b;
}
static vec3 operator * (const vec3& a, const vec3& b)
{
  return vec3(a) *= b;
}
static vec3 operator / (const vec3& a, const vec3& b)
{
  return vec3(a) /= b;
}
static vec3 operator + (const vec3& a, const vec3& b)
{
  return vec3(a) += b;
}
static vec3 operator - (const vec3& a, const vec3& b)
{
  return vec3(a) -= b;
}
static vec3 operator - (const vec3& a, float b)
{
  return vec3(a) -= b;
}
static vec4 operator * (const vec4& a, float b)
{
  return vec4(a) *= b;
}
static vec4 operator / (const vec4& a, const vec4& b)
{
  return vec4(a) /= b;
}
static vec4 operator + (const vec4& a, const vec4& b)
{
  return vec4(a) += b;
}
static vec4 operator - (const vec4& a, const vec4& b)
{
  return vec4(a) -= b;
}
static vec4 operator - (const vec4& a, float b)
{
  return vec4(a) -= b;
}
float fwidth(float x)
{
  return (x - (int)x);
}
vec2 fwidth(vec2 v)
{
  return vec2(1, 1);
}
static float clamp(const float a, float _min, float _max)
{
  return BOUND(a, _min, _max);
}
static vec2 clamp(const vec2& a, float _min, float _max)
{
  return vec2(clamp(a.x, _min, _max), clamp(a.y, _min, _max));
}
static vec2 clamp(const vec2& a, const vec2& _min, const vec2& _max)
{
  return vec2(clamp(a.x, _min.x, _max.x), clamp(a.y, _min.y, _max.y));
}
static vec2 fract(const vec2& a)
{
  return vec2(fract(a.x), fract(a.y));
}
static vec2 floor(const vec2& a)
{
  return vec2(floor(a.x), floor(a.y));
}
vec3 cross(const float3 a, const float3 b)
{
  float x = a[1] * b[2] - a[2] * b[1];
  float y = a[2] * b[0] - a[0] * b[2];
  float z = a[0] * b[1] - a[1] * b[0];
  return vec3(x, y, z);
}
vec3 cross(const vec3& a, const vec3& b)
{
  return cross(a.v, b.v);
}
float dot(const float3 a, const float3 b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
float dot(const vec3& a, const vec3& b)
{
  return dot(a.v, b.v);
}
vec3 normalize(const vec3& a)
{
  float invlen = 1. / sqrt(dot(a.v, a.v));
  return a * invlen;
}
vec3 pow(const vec3& a, const vec3& b)
{
  return vec3(pow(a.x, b.x), pow(a.y, b.y), pow(a.z, b.z));
}
float mix(float x, float y, float a)
{
  return x * (1 - a) + y * a;
}
vec3 mix(const vec3& x, const vec3& y, float a)
{
  return vec3(mix(x.x, y.x, a), mix(x.y, y.y, a), mix(x.z, y.z, a));
}
vec4 mix(const vec4& x, const vec4& y, float a)
{
  return vec4(mix(x.x, y.x, a), mix(x.y, y.y, a), mix(x.z, y.z, a), mix(x.w, y.w, a));
}
struct mat3 {
  float33 m;
};
mat3 operator*(const mat3& a, const mat3& b)
{
  mat3 c;
  mul33fv33fvImpl(c.m, a.m, b.m);
  return c;
}
vec3 operator*(const mat3& a, const vec3& b)
{
  vec3 c;
  mul33fv3fv(c.v, a.m, b.v);
  return c;
}
struct mat4 {
  float44 m;
};
mat4 operator*(const mat4& a, const mat4& b)
{
  mat4 c;
  mul44fv44fvImpl(c.m, a.m, b.m);
  return c;
}
vec4 operator*(const mat4& a, const vec4& b)
{
  vec4 c;
  mul44fv4fv(c.v, a.m, b.v);
  return c;
}

//////////////////////////////////////

#define VEC2CPY(dst, src)  (dst[0] = src[0], dst[1] = src[1])

#define VEC3_SUB(a, b, c)   (c[0] = a[0] - b[0], c[1] = a[1] - b[1], c[2] = a[2] - b[2])
#define VEC3_NORM(v)   {double n = 1./NORM3(v); v[0]*=n, v[1]*=n, v[2]*=n;}
#define VEC3CPY(dst, src)  (dst[0] = src[0], dst[1] = src[1], dst[2] = src[2])

enum LightTypeT {
LIGHT_TYPE_HEADLIGHT = 1,
LIGHT_TYPE_CAMERA_LIGHT = 2,
LIGHT_TYPE_SCENE_LIGHT = 3
};

struct Light {
  float4 AmbientColor;
  float4 DiffuseColor;
  float4 TransformedFocalPoint;
  float4 TransformedPosition;
  float3 AttenuationValues;
  double Intensity;
  double Exponent;
  double ConeAngle;
  int Positional;
  int Switch;
  LightTypeT LightType;
};
struct Camera1 {
  float3 Pos;
  float3 Target;
  float3 Up;
  float2 ClippingRange;
};
struct Camera {
  float3 Position;
  float3 FocalPoint;
  float3 ViewUp;
  float3 DirectionOfProjection;
  float3 ScreenBottomLeft;
  float3 ScreenBottomRight;
  float2 ClippingRange;
  double ViewAngle;
  int UseHorizontalViewAngle;
};

struct StereoEye {
  bool LeftEye;
  double EyeAngle;
  double Distance;
};

struct DirectionalLight {
  vec3 Color;
  float AmbientIntensity;
  vec3 Direction;
  float DiffuseIntensity;
};



//----------------------------------------------------------------------------
template<typename T>
void ToFloat(
             const T& in1, const T& in2, float (&out)[2])
{
  out[0] = static_cast<float>(in1);
  out[1] = static_cast<float>(in2);
}

template<typename T>
void ToFloat(
             const T& in1, const T& in2, const T& in3, float (&out)[3])
{
  out[0] = static_cast<float>(in1);
  out[1] = static_cast<float>(in2);
  out[2] = static_cast<float>(in3);
}

//----------------------------------------------------------------------------
template<typename T>
void ToFloat(
             T* in, float* out, int noOfComponents)
{
  for (int i = 0; i < noOfComponents; ++i)
  {
    out[i] = static_cast<float>(in[i]);
  }
}

//----------------------------------------------------------------------------
template<typename T>
void ToFloat(
             T (&in)[3], float (&out)[3])
{
  out[0] = static_cast<float>(in[0]);
  out[1] = static_cast<float>(in[1]);
  out[2] = static_cast<float>(in[2]);
}

//----------------------------------------------------------------------------
template<typename T>
void ToFloat(
             T (&in)[2], float (&out)[2])
{
  out[0] = static_cast<float>(in[0]);
  out[1] = static_cast<float>(in[1]);
}

//----------------------------------------------------------------------------
template<typename T>
void ToFloat(
             T& in, float& out)
{
  out = static_cast<float>(in);
}

//----------------------------------------------------------------------------
template<typename T>
void ToFloat(
             T (&in)[4][2], float (&out)[4][2])
{
  out[0][0] = static_cast<float>(in[0][0]);
  out[0][1] = static_cast<float>(in[0][1]);
  out[1][0] = static_cast<float>(in[1][0]);
  out[1][1] = static_cast<float>(in[1][1]);
  out[2][0] = static_cast<float>(in[2][0]);
  out[2][1] = static_cast<float>(in[2][1]);
  out[3][0] = static_cast<float>(in[3][0]);
  out[3][1] = static_cast<float>(in[3][1]);
}




/////////////////////////////////////////////////////////////////////////////////////////////

typedef float33 Matrix3x3;
typedef float44 Matrix4x4;
