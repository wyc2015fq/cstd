#include "cfile.h"
GLubyte* tff;
// init the 1 dimentional texture for transfer function
GLuint initTFF1DTex(const char* filename)
{
  // read in the user defined data of transfer function
  const int MAX_CNT = 10000;
  tff = (GLubyte*)calloc(MAX_CNT, sizeof(GLubyte));
  int bytecnt = loadfile(filename, tff, MAX_CNT, 0);
  if (bytecnt > 0) {
    printf("bytecnt %d\n", bytecnt);
  }
  else {
    printf("%s read failed \n", filename);
  }
  GLuint tff1DTex;
  glGenTextures(1, &tff1DTex);
  glBindTexture(GL_TEXTURE_1D, tff1DTex);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, tff);
  //free(tff);
  return tff1DTex;
}
// init the 2D texture for render backface 'bf' stands for backface
GLuint initFace2DTex(GLuint bfTexWidth, GLuint bfTexHeight)
{
  GLuint backFace2DTex;
  glGenTextures(1, &backFace2DTex);
  glBindTexture(GL_TEXTURE_2D, backFace2DTex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bfTexWidth, bfTexHeight, 0, GL_RGBA, GL_FLOAT, NULL);
  return backFace2DTex;
}
// init 3D texture to store the volume data used fo ray casting
GLuint initVol3DTex(const char* filename, GLuint w, GLuint h, GLuint d)
{
  GLuint g_volTexObj = 0;
  FILE* fp;
  size_t size = w * h * d;
  GLubyte* data = (GLubyte*)malloc(size * sizeof(GLubyte)); // 8bit
  if (!(fp = fopen(filename, "rb"))) {
    printf("Error: opening .raw file failed\n");
    exit(EXIT_FAILURE);
  }
  else {
    printf("OK: open .raw file successed\n");
  }
  if (fread(data, sizeof(char), size, fp) != size) {
    printf("Error: read .raw file failed\n");
    exit(1);
  }
  else {
    printf("OK: read .raw file successed\n");
  }
  fclose(fp);
  if (0) {
    int pos = 10;
    cvNamedWindow("asdf", 0);
    cvCreateTrackbar("pos", "asdf", &pos, d - 1, NULL);
    for (;;) {
      cvShowImagePal("asdf", 256, 256, data + pos * 256 * 256, 256, 1, PF_8bppIndexed, NULL);
      cvWaitKey(10);
    }
  }
  glGenTextures(1, &g_volTexObj);
  // bind 3D texture target
  glBindTexture(GL_TEXTURE_3D, g_volTexObj);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
  // pixel transfer happens here from client to OpenGL server
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
  free(data);
  printf("volume texture created\n");
  return g_volTexObj;
}
static void test_raycast1(sys_event_t* event)
{
  static int inited = 0;
  static GLuint gWorldLocation;
  static GLuint in_inverseTextureDatasetMatrixLoc;
  static GLuint in_cameraPosLoc;
  static GLuint in_inverseVolumeMatrixLoc;
  GLuint g_volTexObj = 0;
  GLuint g_tffTexObj = 0;
  bool in_cellFlag = false;
  if (!inited) {
    inited = 1;
    int xx = 0;
    const char* vs = "\
 #version 330\n\
 layout (location = 0) in vec3 in_vertexPos;\
 uniform mat4 gWorld;\
 uniform mat4 in_inverseTextureDatasetMatrix;\
 out vec3 ip_vertexPos;\
 out vec3 ip_textureCoords;\
 out mat4 ip_inverseTextureDataAdjusted;\
 void main()\
 {\
 gl_Position = gWorld * vec4(in_vertexPos, 1.0);\
 ip_textureCoords = in_vertexPos;\
 ip_inverseTextureDataAdjusted = in_inverseTextureDatasetMatrix;\
 ip_vertexPos = in_vertexPos;\
 }";
    const char* fs = "\
 #version 330\n\
 out vec4 g_fragColor;\
 uniform vec3 in_cameraPos;\
 uniform mat4 in_inverseVolumeMatrix;\
 varying vec3 ip_textureCoords;\
 varying vec3 ip_vertexPos;\
 varying mat4 ip_inverseTextureDataAdjusted;\
 void main()\
 {\
 vec4 g_eyePosObj = (in_inverseVolumeMatrix * vec4(in_cameraPos, 1.0));\
 g_fragColor = g_eyePosObj;\
 }";
    GLuint ShaderProgram = CompileShaders(vs, fs);
    g_tffTexObj = initTFF1DTex("D:/code/C/体绘制/Volume_Rendering_Using_GLSL-master/tff.dat");
    g_volTexObj = initVol3DTex("D:/code/C/体绘制/Volume_Rendering_Using_GLSL-master/head256.raw", 256, 256, 225);
    GLint volumeLoc = glGetUniformLocation(ShaderProgram, "VolumeTex");
    if (volumeLoc >= 0) {
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_3D, g_volTexObj);
      glUniform1i(volumeLoc, 2);
    }
    else {
      printf("VolumeTex is not bind to the uniform\n");
    }
    GLint transferFuncLoc = glGetUniformLocation(ShaderProgram, "TransferFunc");
    if (transferFuncLoc >= 0) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_1D, g_tffTexObj);
      glUniform1i(transferFuncLoc, 0);
    }
    else {
      printf("TransferFunc is not bind to the uniform\n");
    }
    in_inverseTextureDatasetMatrixLoc = glGetUniformLocation(ShaderProgram, "in_inverseTextureDatasetMatrix");
    in_cameraPosLoc = glGetUniformLocation(ShaderProgram, "in_cameraPos");
    in_inverseVolumeMatrixLoc = glGetUniformLocation(ShaderProgram, "in_inverseVolumeMatrix");
  }
  switch (event->msg) {
  case MSG_SIZE: {
    int width = event->w, height = event->h;
    glViewport(0, 0, (GLsizei)(width), (GLsizei)(height)); // Reset The Current Viewport
  }
  break;
  }
  float Scale = event->tick * 0.001;
  float Vertices[] = {
    -0.5, -0.5, -0.5,
    -0.5, -0.5, 0.5,
    -0.5, 0.5, -0.5,
    -0.5, 0.5, 0.5,
    0.5, -0.5, -0.5,
    0.5, -0.5, 0.5,
    0.5, 0.5, -0.5,
    0.5, 0.5, 0.5
  };
  int i;
  unsigned int Indices[] = {
    1, 5, 7,
    7, 3, 1,
    0, 2, 6,
    6, 4, 0,
    0, 1, 3,
    3, 2, 0,
    7, 5, 4,
    4, 6, 7,
    2, 3, 7,
    7, 6, 2,
    1, 0, 4,
    4, 5, 1
  };
  PersProjInfo gPersProjInfo;
  gPersProjInfo.FOV = 60.0f;
  gPersProjInfo.Height = event->h;
  gPersProjInfo.Width = event->w;
  gPersProjInfo.zNear = 1.0f;
  gPersProjInfo.zFar = 100.0f;
  float44 m_WVPtransformation;
  float44 m_Wtransformation;
  float44 m_invWtransformation;
  float3 campos = {0.0f, 0.0f, -3.0f};
  GetWorldTrans(m_Wtransformation, 0.0f, 0.0f, 3.0f, 0.0f, Scale, 0.0f, 1, 1, 1);
  Inverse44fv(m_invWtransformation, m_Wtransformation);
  GetWVPTrans(m_WVPtransformation, 0.0f, 0.0f, 3.0f, 0.0f, Scale, 0.0f, 1, 1, 1,
      campos[0], campos[1], campos[2], 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
      &gPersProjInfo);
  float44 in_inverseTextureDatasetMatrix;
  Inverse44fv(in_inverseTextureDatasetMatrix, m_WVPtransformation);
  glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, m_WVPtransformation[0]);
  glUniformMatrix4fv(in_inverseTextureDatasetMatrixLoc, 1, GL_TRUE, in_inverseTextureDatasetMatrix[0]);
  glUniformMatrix4fv(in_inverseVolumeMatrixLoc, 1, GL_TRUE, m_invWtransformation[0]);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_INDEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, Vertices);
  glDrawElements(GL_TRIANGLES, countof(Indices), GL_UNSIGNED_INT, Indices);
  glDisableClientState(GL_INDEX_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

