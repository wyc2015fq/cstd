
static void tutorial11(sys_event_t* event)
{
  static int inited = 0;
  static GLuint gWorldLocation;
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
  }
  switch (event->msg) {
  case MSG_SIZE: {
    int width = event->w, height = event->h;
    glViewport(0, 0, (GLsizei)(width), (GLsizei)(height)); // Reset The Current Viewport
  }
  break;
  }
  float Scale = event->tick * 0.001;
  //p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
  //p.WorldPos(sinf(Scale), 0.0f, 0.0f);
  //p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);
  float44 m_Wtransformation;
  float tt = sinf(Scale);
  float ss = sinf(Scale * 0.1f);
  float rr = tt * 90.0f;
  GetWorldTrans(m_Wtransformation, tt, 0.0f, 0.0f, rr, rr, rr, ss, ss, ss);
  Print44fv(m_Wtransformation);
  glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, m_Wtransformation[0]);
  float Vertices[] = {
    -1.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 0.0f,
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

