#include "math3d.inl"
void GetWorldTrans(float44 m_Wtransformation, float x, float y, float z,
    float RotateX, float RotateY, float RotateZ,
    float ScaleX, float ScaleY, float ScaleZ)
{
  InitTranslationTransform(m_Wtransformation, x, y, z);
  RotateTransform(m_Wtransformation, RotateX, RotateY, RotateZ);
  ScaleTransform3f(m_Wtransformation, ScaleX, ScaleY, ScaleZ);
}
void GetWPTrans(float44 m_WPtransformation, float x, float y, float z,
    float RotateX, float RotateY, float RotateZ,
    float ScaleX, float ScaleY, float ScaleZ, const PersProjInfo* m_persProjInfo)
{
  float44 PersProjTrans, m_Wtransformation;
  GetWorldTrans(m_Wtransformation, x, y, z, RotateX, RotateY, RotateZ, ScaleX, ScaleY, ScaleZ);
  InitPersProjTransform(PersProjTrans, m_persProjInfo);
  mul44fv44fv(m_WPtransformation, PersProjTrans, m_Wtransformation);
}
void GetViewTrans(float44 m_Vtransformation, float CameraPosX, float CameraPosY, float CameraPosZ,
    float TargetX, float TargetY, float TargetZ, float UpX, float UpY, float UpZ)
{
  float44 CameraTranslationTrans, CameraRotateTrans;
  InitTranslationTransform(CameraTranslationTrans, -CameraPosX, -CameraPosY, -CameraPosZ);
  InitCameraTransform3f3f(CameraRotateTrans, TargetX, TargetY, TargetZ, UpX, UpY, UpZ);
  mul44fv44fv(m_Vtransformation, CameraRotateTrans, CameraTranslationTrans);
}
void GetVPTrans(float44 m_VPtransformation, float CameraPosX, float CameraPosY, float CameraPosZ,
    float TargetX, float TargetY, float TargetZ, float UpX, float UpY, float UpZ, const PersProjInfo* m_persProjInfo)
{
  float44 m_Vtransformation, m_ProjTransformation;
  GetViewTrans(m_Vtransformation, CameraPosX, CameraPosY, CameraPosZ, TargetX, TargetY, TargetZ, UpX, UpY, UpZ);
  InitPersProjTransform(m_ProjTransformation, m_persProjInfo);
  mul44fv44fv(m_VPtransformation, m_ProjTransformation, m_Vtransformation);
}
void GetWVPTrans(float44 m_WVPtransformation, float x, float y, float z,
    float RotateX, float RotateY, float RotateZ,
    float ScaleX, float ScaleY, float ScaleZ,
    float CameraPosX, float CameraPosY, float CameraPosZ,
    float TargetX, float TargetY, float TargetZ, float UpX, float UpY, float UpZ, const PersProjInfo* m_persProjInfo)
{
  float44 m_VPtransformation, m_Wtransformation;
  GetWorldTrans(m_Wtransformation, x, y, z, RotateX, RotateY, RotateZ, ScaleX, ScaleY, ScaleZ);
  GetVPTrans(m_VPtransformation, CameraPosX, CameraPosY, CameraPosZ, TargetX, TargetY, TargetZ, UpX, UpY, UpZ, m_persProjInfo);
  mul44fv44fv(m_WVPtransformation, m_VPtransformation, m_Wtransformation);
}

struct Vertex {
  vec3 m_pos;
  vec2 m_tex;
  vec3 m_normal;
  Vertex() {}
  Vertex(const vec3& pos, const vec2& tex) {
    m_pos = pos;
    m_tex = tex;
    m_normal = vec3(0.0f, 0.0f, 0.0f);
  }
  Vertex(const vec3& pos, const vec2& tex, const vec3& normal)
  {
    m_pos    = pos;
    m_tex    = tex;
    m_normal = normal;
  }
};
void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
                 Vertex* pVertices, unsigned int VertexCount)
{
  // Accumulate each triangle normal into each of the triangle vertices
  unsigned int i;
  for (i = 0 ; i < VertexCount ; i++) {
    pVertices[i].m_normal = vec3(0.f);
  }
  for (i = 0 ; i < IndexCount ; i += 3) {
    unsigned int Index0 = pIndices[i];
    unsigned int Index1 = pIndices[i + 1];
    unsigned int Index2 = pIndices[i + 2];
    vec3 v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
    vec3 v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
    vec3 Normal = normalize(cross(v1, v2));
    pVertices[Index0].m_normal += Normal;
    pVertices[Index1].m_normal += Normal;
    pVertices[Index2].m_normal += Normal;
  }
  // Normalize all the vertex normals
  for (i = 0 ; i < VertexCount ; i++) {
    pVertices[i].m_normal = normalize(pVertices[i].m_normal);
  }
}
vec3 Rotate(const vec3& v, float Angle, const vec3& Axe)
{
  vec3 ret;
  Rotate(ret.v, v.v, Angle, Axe.v);
  return ret;
}
void Camera_Update(float m_AngleH, float m_AngleV, vec3& m_target, vec3& m_up)
{
  const vec3 Vaxis(0.0f, 1.0f, 0.0f);
  // Rotate the view vector by the horizontal angle around the vertical axis
  vec3 View(1.0f, 0.0f, 0.0f);
  View = Rotate(View, m_AngleH, Vaxis);
  View = normalize(View);
  // Rotate the view vector by the vertical angle around the horizontal axis
  vec3 Haxis = cross(Vaxis, View);
  Haxis = normalize(Haxis);
  View = Rotate(View, m_AngleV, Haxis);
  m_target = View;
  m_target = normalize(m_target);
  m_up = cross(m_target, Haxis);
  m_up = normalize(m_up);
}
static void tutorial02(sys_event_t* event)
{
  float Vertices[] = {0.0f, 0.0f, 0.0f};
  switch (event->msg) {
  case MSG_SIZE: {
    int width = event->w, height = event->h;
    glViewport(0, 0, (GLsizei)(width), (GLsizei)(height)); // Reset The Current Viewport
                 }
    break;
  }
  glClear(GL_COLOR_BUFFER_BIT);
  glVertexPointer(3, GL_FLOAT, 0, Vertices);
  glDrawArrays(GL_POINTS, 0, 1);
}

bool Camera_OnKeyboard(vec3& m_pos, const vec3& m_target, const vec3& m_up, sys_event_t* event)
{
  const static float STEP_SCALE = 1.0f;
  const static float EDGE_STEP = 0.5f;
  const static int MARGIN = 10;
  bool Ret = false;
  if (MSG_KEYDOWN != event->msg) {
    return false;
  }
  switch (event->last_key) {
  case KEY_UP: {
    m_pos += (m_target * STEP_SCALE);
    Ret = true;
               }
    break;
  case KEY_DOWN: {
    m_pos -= (m_target * STEP_SCALE);
    Ret = true;
                 }
    break;
  case KEY_LEFT: {
    m_pos += normalize(cross(m_target, m_up)) * STEP_SCALE;
    Ret = true;
                 }
    break;
  case KEY_RIGHT: {
    m_pos += normalize(cross(m_up, m_target)) * STEP_SCALE;
    Ret = true;
                  }
    break;
  case KEY_PAGEUP:
    m_pos.y += STEP_SCALE;
    break;
  case KEY_PAGEDOWN:
    m_pos.y -= STEP_SCALE;
    break;
  default:
    break;
  }
  return Ret;
}

#include "img/imgio.inl"
GLuint Texture_Load(GLenum m_textureTarget, const char* m_fileName)
{
  img_t im[1] = {0};
  GLuint m_textureObj = -1;
  if (!imread(m_fileName, 4, 1, im)) {
    printf("Error loading texture '%s'", m_fileName);
    return m_textureObj;
  }
  imswap_rb(im);
  glGenTextures(1, &m_textureObj);
  glBindTexture(m_textureTarget, m_textureObj);
  glTexImage2D(m_textureTarget, 0, GL_RGBA, im->w, im->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, im->tt.data);
  glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(m_textureTarget, 0);
  imfree(im);
  return m_textureObj;
}