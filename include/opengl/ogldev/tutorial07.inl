static void tutorial07(sys_event_t* event)
{
  static int inited = 0;
  static GLuint gWorldLocation;
  float Scale = event->tick * 0.0001;
  float World[] = {
    cosf(Scale), -sinf(Scale), 0.0f, 0.0f,
    sinf(Scale), cosf(Scale), 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
  float Vertices[] = { -1.0f, -1.0f, 0.0f, //
      1.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f
                     };
  if (!inited) {
    const char* vs = "\
 #version 330\n\
 layout (location = 0) in vec3 Position;\
 uniform mat4 gWorld;\
 void main()\
 {\
 gl_Position = gWorld * vec4(Position, 1.0);\
 }";
    const char* fs = "\
 #version 330\n\
 out vec4 FragColor;\
 void main()\
 {\
 FragColor = vec4(1.0, 0.0, 0.0, 1.0);\
 }";
    GLuint ShaderProgram = CompileShaders(vs, fs);
    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);
    inited = 1;
  }
  switch (event->msg) {
  case MSG_SIZE: {
    int width = event->w, height = event->h;
    glViewport(0, 0, (GLsizei)(width), (GLsizei)(height)); // Reset The Current Viewport
  }
  break;
  }
  glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, World);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, Vertices);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableClientState(GL_VERTEX_ARRAY);
}

