static void tutorial04(sys_event_t* event)
{
  float Vertices[] = { -1.0f, -1.0f, 0.0f, //
      1.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f
                     };
  static int inited = 0;
  if (!inited) {
    const char* vs = "\
#version 330\n\
layout (location = 0) in vec3 Position1;\
 void main()\
 {\
 gl_Position = vec4(0.5 * Position1.x, 0.5 * Position1.y, Position1.z, 1.0);\
}";
        const char* fs = "\
#version 330\n\
out vec4 FragColor;\
void main()\
{\
 FragColor = vec4(1.0, 0.0, 0.0, 1.0);\
}";
        CompileShaders(vs, fs);
    inited = 1;
  }
  switch (event->msg) {
  case MSG_SIZE: {
    int width = event->w, height = event->h;
    glViewport(0, 0, (GLsizei)(width), (GLsizei)(height)); // Reset The Current Viewport
  }
  break;
  }
  glClear(GL_COLOR_BUFFER_BIT);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, Vertices);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableClientState(GL_VERTEX_ARRAY);
}

