
static void tutorial15(sys_event_t* event)
{
  static int inited = 0;
  static GLuint gWorldLocation;
  static IPOINT m_mousePos;
  static float m_AngleH = 270;
  static float m_AngleV = 0;
  if (!inited) {
    inited = 1;
    const char* vs = "\
 #version 330\n\
 layout (location = 0) in vec3 Position;\
 uniform mat4 gWorld;\
 out vec4 Color;\
 void main()\
 {\
 gl_Position = gWorld * vec4(Position, 1.0);\
 Color = vec4(clamp(Position, 0.0, 1.0), 1.0);\
 }";
    const char* fs = "\
 #version 330\n\
 in vec4 Color;\
 out vec4 FragColor;\
 void main()\
 {\
 FragColor = Color;\
 }";
    GLuint ShaderProgram = CompileShaders(vs, fs);
    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);
    m_mousePos.x = event->w / 2;
    m_mousePos.y = event->h / 2;
  }
  static vec3 campos(0, 0, 0);
  static vec3 target(0.0f, 0.0f, 1.0f);
  static vec3 up(0.0f, 1.0f, 0.0f);
  switch (event->msg) {
  case MSG_SIZE: {
    int width = event->w, height = event->h;
    glViewport(0, 0, (GLsizei)(width), (GLsizei)(height)); // Reset The Current Viewport
  }
  break;
  }
  {
    int x = event->x, y = event->y;
    const int DeltaX = x - m_mousePos.x;
    const int DeltaY = y - m_mousePos.y;
    m_mousePos.x = x;
    m_mousePos.y = y;
    m_AngleH += (float)DeltaX / 20.0f;
    m_AngleV += (float)DeltaY / 20.0f;
    Camera_Update(m_AngleH, m_AngleV, target, up);
  }
  Camera_OnKeyboard(campos, target, up, event);
  float Scale = event->tick * 0.01;
  //p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
  //p.WorldPos(sinf(Scale), 0.0f, 0.0f);
  //p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);
  PersProjInfo gPersProjInfo;
  gPersProjInfo.FOV = 60.0f;
  gPersProjInfo.Height = event->h;
  gPersProjInfo.Width = event->w;
  gPersProjInfo.zNear = 1.0f;
  gPersProjInfo.zFar = 100.0f;
  float44 m_WVPtransformation;
  GetWVPTrans(m_WVPtransformation, 0.0f, 0.0f, 3.0f, 0.0f, Scale, 0.0f, 1, 1, 1,
      campos.x, campos.y, campos.z, target.x, target.y, target.z, up.x, up.y, up.z,
      &gPersProjInfo);
  glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, m_WVPtransformation[0]);
  float Vertices[] = {
    -1.0f, -1.0f, 0.5773f,
    0.0f, -1.0f, -1.15475f,
    1.0f, -1.0f, 0.5773f,
    0.0f, 1.0f, 0.0f
  };
  unsigned int Indices[] = { 0, 3, 1,
      1, 3, 2,
      2, 3, 0,
      0, 1, 2
                           };
  glClear(GL_COLOR_BUFFER_BIT);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_INDEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, Vertices);
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, Indices);
  glDisableClientState(GL_INDEX_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

