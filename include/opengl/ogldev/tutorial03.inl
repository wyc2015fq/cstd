static void tutorial03(sys_event_t* event)
{
  float Vertices[] = { -1.0f, -1.0f, 0.0f, //
      1.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f
                     };
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

