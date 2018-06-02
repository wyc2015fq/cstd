#if 0
class Camera
{
private:
  Quaternion m_orientation;
public:
  void rotate(const Quaternion& q);
  void rotate(const float3& axis, const Radian& angle);
  void roll(const GLfloat angle);
  void yaw(const GLfloat angle);
  void pitch(const GLfloat angle);
};
void Camera::rotate(const Quaternion& q)
{
  // Note the order of the mult, i.e. q comes after
  m_Orientation = q * m_Orientation;
}
void Camera::rotate(const float3& axis, const Radian& angle)
{
  Quaternion q;
  q.FromAngleAxis(angle, axis);
  rotate(q);
}
void Camera::roll(const GLfloat angle)  //in radian
{
  float3 zAxis = m_Orientation * float3::UNIT_Z;
  rotate(zAxis, angleInRadian);
}
void Camera::yaw(const GLfloat angle)  //in degree
{
  float3 yAxis;
  {
    // Rotate around local Y axis
    yAxis = m_Orientation * float3::UNIT_Y;
  }
  rotate(yAxis, angleInRadian);
}
void Camera::pitch(const GLfloat angle)  //in radian
{
  float3 xAxis = m_Orientation * float3::UNIT_X;
  rotate(xAxis, angleInRadian);
}
void Camera::gluLookAt()
{
  GLfloat m[4][4];
  identf(&m[0][0]);
  m_Orientation.createMatrix(&m[0][0]);
  glMultMatrixf(&m[0][0]);
  glTranslatef(-m_eyex, -m_eyey, -m_eyez);
}
用 Quaternion 实现 trackball
用鼠标拖动物体在三维空间里旋转，一般设计一个 trackball，其内部实现也常用四元数。
class TrackBall
{
public:
  TrackBall();
  void push(const QPointF& p);
  void move(const QPointF& p);
  void release(const QPointF& p);
  QQuaternion rotation() const;
private:
  QQuaternion m_rotation;
  QVector3D m_axis;
  float m_angularVelocity;
  QPointF m_lastPos;
};
void TrackBall::move(const QPointF& p)
{
  if (!m_pressed) {
    return;
  }
  QVector3D lastPos3D = QVector3D(m_lastPos.x(), m_lastPos.y(), 0.0f);
  float sqrZ = 1 - QVector3D::dotProduct(lastPos3D, lastPos3D);
  if (sqrZ > 0) {
    lastPos3D.setZ(sqrt(sqrZ));
  }
  else {
    lastPos3D.normalize();
  }
  QVector3D currentPos3D = QVector3D(p.x(), p.y(), 0.0f);
  sqrZ = 1 - QVector3D::dotProduct(currentPos3D, currentPos3D);
  if (sqrZ > 0) {
    currentPos3D.setZ(sqrt(sqrZ));
  }
  else {
    currentPos3D.normalize();
  }
  m_axis = QVector3D::crossProduct(lastPos3D, currentPos3D);
  float angle = 180 / PI * asin(sqrt(QVector3D::dotProduct(m_axis, m_axis)));
  m_axis.normalize();
  m_rotation = QQfromAxisAndAngle(m_axis, angle) * m_rotation;
  m_lastPos = p;
}
#endif

