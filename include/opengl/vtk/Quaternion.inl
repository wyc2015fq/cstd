//
// Quaternion 的定义
// 四元数一般定义如下：
// q=w+xi+yj+zk
// 其中 w,x,y,z是实数。同时，有:
// i*i=-1
// j*j=-1
// k*k=-1
// 四元数也可以表示为：
// q=[w,v]
// 其中v=(x,y,z)是矢量，w是标量，虽然v是矢量，但不能简单的理解为3D空间的矢量，它是4维空间中的的矢量，也是非常不容易想像的。
// 通俗的讲，一个四元数（Quaternion）描述了一个旋转轴和一个旋转角度。这个旋转轴和这个角度可以通过 Quaternion::ToAngleAxis转换得到。
// 当然也可以随意指定一个角度一个旋转轴来构造一个Quaternion。这个角度是相对于单位四元数而言的，也可以说是相对于物体的初始方向而言的。
// 当用一个四元数乘以一个向量时，实际上就是让该向量围绕着这个四元数所描述的旋转轴，转动这个四元数所描述的角度而得到的向量。
// 四元组的优点1
// 有多种方式可表示旋转，如 axis/angle、欧拉角(Euler angles)、矩阵(matrix)、四元组等。 相对于其它方法，四元组有其本身的优点：
// 四元数不会有欧拉角存在的 gimbal lock 问题
// 四元数由4个数组成，旋转矩阵需要9个数
// 两个四元数之间更容易插值
// 四元数、矩阵在多次运算后会积攒误差，需要分别对其做规范化(normalize)和正交化(orthogonalize)，对四元数规范化更容易
// 与旋转矩阵类似，两个四元组相乘可表示两次旋转
#define QUAT_W 0
#define QUAT_X 1
#define QUAT_Y 2
#define QUAT_Z 3
#define GETQUAT(Data) double w = Data[QUAT_W], x = Data[QUAT_X], y = Data[QUAT_Y], z = Data[QUAT_Z]
#define GETQUAT2(Data) double Data##w = Data[QUAT_W], Data##x = Data[QUAT_X], Data##y = Data[QUAT_Y], Data##z = Data[QUAT_Z]
#define SETXYZW(ret, x, y, z, w) (ret[QUAT_W] = w, ret[QUAT_X] = x, ret[QUAT_Y] = y, ret[QUAT_Z] = z)
void QCopy(float4 Data, const float4 quat)
{
  COPY4(Data, quat);
}
void QSet(float4 Data, double w, double x, double y, double z)
{
  SETXYZW(Data, x, y, z, w);
}
double QNorm(const float4 Data)
{
  return Norm4(Data);
}
void QIdentity(float4 Data)
{
  QSet(Data, 1.0, 0.0, 0.0, 0.0);
}
double QNormalize(float4 Data)
{
  return Normalize4(Data);
}
void QConjugate(const float4 Data, float4 ret)
{
  GETQUAT(Data);
  QSet(ret, w, -x, -y, -z);
}
void QConjugate(float4 Data)
{
  Data[QUAT_X] = -Data[QUAT_X];
  Data[QUAT_Y] = -Data[QUAT_Y];
  Data[QUAT_Z] = -Data[QUAT_Z];
}
bool QInvert(const float4 Data, float4 ret)
{
  double squareNorm = SNorm4(Data);
  if (squareNorm != 0.0) {
    GETQUAT(Data);
    double s = 1. / squareNorm;
    QSet(ret, w * s, -x * s, -y * s, -z * s);
    return true;
  }
  return false;
}
bool QInvert(float4 Data)
{
  return QInvert(Data, Data);
}
double QGetRotationAngleAndAxis(const float4 Data, float axis[3])
{
  GETQUAT(Data);
  double f = sqrt(x * x + y * y + z * z);
  if (f != 0.0) {
    axis[0] = x / f;
    axis[1] = y / f;
    axis[2] = z / f;
  }
  else {
    w = 1.0;
    axis[0] = 0.0;
    axis[1] = 0.0;
    axis[2] = 0.0;
  }
  // atan2() provides a more accurate angle result than acos()
  return 2.0 * atan2(f, w);
}
void QSetRotationAngleAndAxis(float4 Data, double angle, double x, double y, double z)
{
  double axisNorm = x * x + y * y + z * z;
  if (axisNorm != 0.0) {
    double f = sin(0.5 * angle) / axisNorm;
    QSet(Data, cos(0.5 * angle), x * f, y * f, z * f);
  }
  else {
    // set the quaternion for "no rotation"
    QSet(Data, 1.0, 0.0, 0.0, 0.0);
  }
}
void QSetRotationAngleAndAxis(float4 Data, double angle, float axis[3])
{
  QSetRotationAngleAndAxis(Data, angle, axis[0], axis[1], axis[2]);
}
// Multiplying q1 with q2 applies the rotation q2 to q1
void QMulAdd1(const float4 Data, const float4 q, const float4 a, float4 ret)
{
  GETQUAT(Data);
  GETQUAT2(q);
  // the constructor takes its arguments as (x, y, z, w)
  SETXYZW(ret,
      w * qx + x * qw + y * qz - z * qy,
      w * qy + y * qw + z * qx - x * qz,
      w * qz + z * qw + x * qy - y * qx,
      w * qw - x * qx - y * qy - z * qz);
}
void QMulAdd(const float4 Data, const float4 q, const float4 a, float4 ret)
{
  GETQUAT(Data);
  GETQUAT2(q);
  double ww = w * qw;
  double wx = w * qx;
  double wy = w * qy;
  double wz = w * qz;
  double xw = x * qw;
  double xx = x * qx;
  double xy = x * qy;
  double xz = x * qz;
  double yw = y * qw;
  double yx = y * qx;
  double yy = y * qy;
  double yz = y * qz;
  double zw = z * qw;
  double zx = z * qx;
  double zy = z * qy;
  double zz = z * qz;
  if (a) {
    GETQUAT2(a);
    QSet(ret,
        aw + ww - xx - yy - zz,
        ax + wx + xw + yz - zy,
        ay + wy - xz + yw + zx,
        az + wz + xy - yx + zw);
  }
  else {
    QSet(ret,
        ww - xx - yy - zz,
        wx + xw + yz - zy,
        wy - xz + yw + zx,
        wz + xy - yx + zw);
  }
}
void QMul(const float4 Data, const float4 q, float4 ret)
{
  QMulAdd(Data, q, NULL, ret);
}
void QDiv(const float4 Data, const float4 q, float4 ret)
{
  float4 inverseQuaternion;
  QInvert(Data, inverseQuaternion);
  QMul(Data, inverseQuaternion, ret);
}
// Multiplying a quaternion q with a vector v applies the q-rotation to v
void QRotatingVector(const float4 Data, const float3 v1, float3 v2)
{
  float3 vn;
  COPY3(vn, v1);
  Normalize3(vn);
  float4 vecQuat, resQuat, cData, ret;
  SETXYZW(vecQuat, vn[0], vn[1], vn[2], 0.0f);
  QConjugate(Data, cData);
  QMul(vecQuat, cData, resQuat);
  QMul(Data, resQuat, ret);
  v2[0] = ret[QUAT_X];
  v2[1] = ret[QUAT_Y];
  v2[2] = ret[QUAT_Z];
}
// Convert from Euler Angles
void QFromEuler(float4 Data, double pitch, double yaw, double roll)
{
  const float PIOVER180 = 0.0174532925f;
  // Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
  // and multiply those together.
  // the calculation below does the same, just shorter
  double p = pitch * PIOVER180 / 2.0;
  double y = yaw * PIOVER180 / 2.0;
  double r = roll * PIOVER180 / 2.0;
  double sinp = sin(p);
  double siny = sin(y);
  double sinr = sin(r);
  double cosp = cos(p);
  double cosy = cos(y);
  double cosr = cos(r);
  SETXYZW(Data,
      sinr * cosp * cosy - cosr * sinp * siny,
      cosr * sinp * cosy + sinr * cosp * siny,
      cosr * cosp * siny - sinr * sinp * cosy,
      cosr * cosp * cosy + sinr * sinp * siny);
  QNormalize(Data);
}
// Convert to Matrix
void QToMatrix16(const float4 Data, float16 ret)
{
  GETQUAT(Data);
  float x2 = x * x;
  float y2 = y * y;
  float z2 = z * z;
  float xy = x * y;
  float xz = x * z;
  float yz = y * z;
  float wx = w * x;
  float wy = w * y;
  float wz = w * z;
  // This calculation would be a lot more complicated for non-unit length quaternions
  // Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
  // OpenGL
  SET16(ret,
      1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
      2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
      2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
}
void QToMatrix9(const float4 Data, float9 A)
{
  GETQUAT(Data);
  double ww = w * w;
  double wx = w * x;
  double wy = w * y;
  double wz = w * z;
  double xx = x * x;
  double yy = y * y;
  double zz = z * z;
  double xy = x * y;
  double xz = x * z;
  double yz = y * z;
  double rr = xx + yy + zz;
  // normalization factor, just in case quaternion was not normalized
  double f;
  if (ww + rr == 0.0) { //means the quaternion is (0, 0, 0, 0)
    A[0 * 3 + 0] = 0.0, A[1 * 3 + 0] = 0.0, A[2 * 3 + 0] = 0.0,
        A[0 * 3 + 1] = 0.0, A[1 * 3 + 1] = 0.0, A[2 * 3 + 1] = 0.0,
            A[0 * 3 + 2] = 0.0, A[1 * 3 + 2] = 0.0, A[2 * 3 + 2] = 0.0;
    return;
  }
  f = 1.0 / (ww + rr);
  double s = (ww - rr) * f;
  f *= 2.0;
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
void QFromMatrix3x3(float4 Data, const float9 A)
{
  float n[16];
  // on-diagonal elements
  n[0 * 4 + 0] = A[0 * 3 + 0] + A[1 * 3 + 1] + A[2 * 3 + 2];
  n[1 * 4 + 1] = A[0 * 3 + 0] - A[1 * 3 + 1] - A[2 * 3 + 2];
  n[2 * 4 + 2] = -A[0 * 3 + 0] + A[1 * 3 + 1] - A[2 * 3 + 2];
  n[3 * 4 + 3] = -A[0 * 3 + 0] - A[1 * 3 + 1] + A[2 * 3 + 2];
  // off-diagonal elements
  n[0 * 4 + 1] = n[1 * 4 + 0] = A[2 * 3 + 1] - A[1 * 3 + 2];
  n[0 * 4 + 2] = n[2 * 4 + 0] = A[0 * 3 + 2] - A[2 * 3 + 0];
  n[0 * 4 + 3] = n[3 * 4 + 0] = A[1 * 3 + 0] - A[0 * 3 + 1];
  n[1 * 4 + 2] = n[2 * 4 + 1] = A[1 * 3 + 0] + A[0 * 3 + 1];
  n[1 * 4 + 3] = n[3 * 4 + 1] = A[0 * 3 + 2] + A[2 * 3 + 0];
  n[2 * 4 + 3] = n[3 * 4 + 2] = A[2 * 3 + 1] + A[1 * 3 + 2];
  float16 eigenvectors;
  float4 eigenvalues;
  // convert into format that JacobiN can use,
  // then use Jacobi to find eigenvalues and eigenvectors
  JacobiN(n, 4, 4, eigenvalues, eigenvectors, 4);
  // the first eigenvector is the one we want
  for (int i = 0; i < 4; ++i) {
    Data[i] = eigenvectors[i * 4 + 0];
  }
}
void QSlerp(const float4 Data, double t, const float4 q1, float4 ret)
{
  float axis0[3], axis1[3], cross[3];
  QGetRotationAngleAndAxis(Data, axis0);
  QGetRotationAngleAndAxis(q1, axis1);
  double dot = Dot(axis0, axis1);
  Cross(axis0, axis1, cross);
  double f = Norm3(cross);
  // To avoid division by zero, perform a linear interpolation (LERP), if our
  // quarternions are nearly in the same direction, otherwise resort
  // to spherical linear interpolation. In the limiting case (for small
  // angles), SLERP is equivalent to LERP.
  double t1 = 1.0 - t;
  double t2 = t;
  if (f > 1e-6) {
    double theta = atan2(f, dot);
    t1 = sin((1.0 - t) * theta) / sin(theta);
    t2 = sin(t * theta) / sin(theta);
  }
  ret[0] = Data[0] * t1 + q1[0] * t2;
  ret[1] = Data[1] * t1 + q1[1] * t2;
  ret[2] = Data[2] * t1 + q1[2] * t2;
  ret[3] = Data[3] * t1 + q1[3] * t2;
}
void QToUnitLog(const float4 Data, float4 ret)
{
  float3 axis;
  double angle = 0.5 * QGetRotationAngleAndAxis(Data, axis);
  QSet(ret, 0.0, angle * axis[0], angle * axis[1], angle * axis[2]);
}
void QToUnitExp(const float4 Data, float4 ret)
{
  double x = Data[1];
  double y = Data[2];
  double z = Data[3];
  double angle = sqrt(x * x + y * y + z * z);
  double sinAngle = sin(angle);
  double cosAngle = cos(angle);
  if (angle != 0.0) {
    x /= angle;
    y /= angle;
    z /= angle;
  }
  QSet(ret, cosAngle, sinAngle * x, sinAngle * y, sinAngle * z);
}
void QInnerPoint(const float4 Data, const float4 q1, const float4 q2, float4 ret)
{
  float4 qInv, qL, qR, qLLog, qRLog, qSum, qExp;
  QInvert(q1, qInv);
  QMul(qInv, q2, qL);
  QMul(qInv, Data, qR);
  QToUnitLog(qL, qLLog);
  QToUnitLog(qR, qRLog);
  Add4(qLLog, qRLog, qSum);
  qSum[0] /= -4.0;
  qSum[1] /= -4.0;
  qSum[2] /= -4.0;
  QToUnitExp(qSum, qExp);
  QMul(q1, qExp, ret);
}
void QNormalizeWithAngleInDegrees(float4 Data)
{
  QNormalize(Data);
  Data[0] = DegreesFromRadians(Data[0]);
}
void QNormalizedWithAngleInDegrees2(const float4 Data, float4 unitVTK)
{
  QCopy(unitVTK, Data);
  QNormalizeWithAngleInDegrees(unitVTK);
}
void QShortestArc(const float3 v1, const float3 v2, float4 q)
{
  // input vectors NOT unit
  Cross(v1, v2, q + 1);
  q[0] = Dot(v1, v2);
  // reducing to half angle
  q[0] += Norm4(q); // 4 multiplication instead of 6 and more numerical stable
  // handling close to 180 degree case
  //... code skipped
  QNormalize(q);
}
//Quaternion q;
//vector a = crossproduct(v1, v2)
//q.xyz = a;
//q.w = sqrt((v1.Length ^ 2) * (v2.Length ^ 2)) + dotproduct(v1, v2)
void QShortestArc1(const float3 v1, const float3 v2, float4 q)
{
  Cross(v1, v2, q + 1);
  q[0] = sqrt(SNorm3(v1) * SNorm3(v2)) + Dot(v1, v2);
  QNormalize(q);
}
// 平面变换
void QPlaneTrans(const float3 v1, const float3 d1, const float3 v2, const float3 d2, float4 q)
{
  float4 q1, s1, q2, s2;
  QShortestArc(v1, v2, q1);
  QRotatingVector(q1, d1, s1);
  Cross(v1, s1, s1);
  Cross(v2, d2, s2);
  QShortestArc(s1, s2, q2);
  QMul(q1, q2, q);
}

