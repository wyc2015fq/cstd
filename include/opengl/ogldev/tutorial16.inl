
static void tutorial16(sys_event_t* event)
{
  static int inited = 0;
  static GLuint gWVPLocation;
  static GLuint gSampler;
  static IPOINT m_mousePos;
  static float m_AngleH = 270;
  static float m_AngleV = 0;
  static GLuint VBO = 0;
  static GLuint IBO = 0;
  static GLuint m_textureObj;
  if (!inited) {
    inited = 1;
    const char* vs = "\
 #version 330\n\
 layout (location = 0) in vec3 Position;\
 layout (location = 1) in vec2 TexCoord;\
 uniform mat4 gWVP;\
 out vec2 TexCoord0;\
 void main()\
 {\
 gl_Position = gWVP * vec4(Position, 1.0);\
 TexCoord0 = TexCoord;\
 }";
    const char* fs = "\
 #version 330\n\
 in vec2 TexCoord0;\
 out vec4 FragColor;\
 uniform sampler2D gSampler;\
 void main()\
 {\
 FragColor = texture2D(gSampler, TexCoord0.xy);\
 }";
    GLuint ShaderProgram = CompileShaders(vs, fs);
    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    assert(gWVPLocation != 0xFFFFFFFF);
    gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
    assert(gSampler != 0xFFFFFFFF);
    m_mousePos.x = event->w / 2;
    m_mousePos.y = event->h / 2;
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    float Vertices[4 * 5] = {
      -1.0f, -1.0f, 0.5773f, 0.0f, 0.0f,
      0.0f, -1.0f, -1.15475f, 0.5f, 0.0f,
      1.0f, -1.0f, 0.5773f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.5f, 1.0f
    };
    unsigned int Indices[] = { 0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
                             };
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    glUniform1i(gSampler, 0);
    m_textureObj = Texture_Load(GL_TEXTURE_2D, "D:/code/C/opengl/ogldev-source/Content/test.png");
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
  glClear(GL_COLOR_BUFFER_BIT);
  float44 m_WVPtransformation;
  GetWVPTrans(m_WVPtransformation, 0.0f, 0.0f, 3.0f, 0.0f, Scale, 0.0f, 1, 1, 1,
      campos.x, campos.y, campos.z, target.x, target.y, target.z, up.x, up.y, up.z,
      &gPersProjInfo);
  glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, m_WVPtransformation[0]);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const GLvoid*)12);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_textureObj);
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

