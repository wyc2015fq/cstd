#include "cstd.h"
#include "cfile.h"
// check the compilation result
GLboolean compileCheck(GLuint shader)
{
  GLint err;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
  if (GL_FALSE == err) {
    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char* log = (char*)malloc(logLen);
      GLsizei written;
      glGetShaderInfoLog(shader, logLen, &written, log);
      printf("Shader log: %s\n", log);
      free(log);
    }
  }
  return err;
}
GLuint initShaderObj(const GLchar* srcfile, GLenum shaderType)
{
  str_t s[1] = {0};
  // use assert?
  if (!str_load(srcfile, s)) {
    //printf("Error openning file: %s\n", srcfile);
    str_setstr2(s, srcfile);
  }
  // create the shader Object
  GLuint shader = glCreateShader(shaderType);
  if (0 == shader) {
    printf("Error creating vertex shader.\n");
  }
  // cout << shaderCode << endl;
  // cout << endl;
  const GLchar* codeArray[] = { s->s };
  glShaderSource(shader, 1, codeArray, NULL);
  str_free(s);
  // compile the shader
  glCompileShader(shader);
  if (GL_FALSE == compileCheck(shader)) {
    printf("shader compilation failed\n");
  }
  return shader;
}
#define GL_ERROR() checkForOpenGLError(__FILE__, __LINE__)
int checkForOpenGLError(const char* file, int line)
{
  // return 1 if an OpenGL error occured, 0 otherwise.
  GLenum glErr;
  int retCode = 0;
  glErr = glGetError();
  while (glErr != GL_NO_ERROR) {
    printf("glError in file %s @line %d : %s\n", file, line, gluErrorString(glErr));
    retCode = 1;
    exit(EXIT_FAILURE);
  }
  return retCode;
}
GLint checkShaderLinkStatus(GLuint pgmHandle)
{
  GLint status;
  glGetProgramiv(pgmHandle, GL_LINK_STATUS, &status);
  if (GL_FALSE == status) {
    GLint logLen;
    glGetProgramiv(pgmHandle, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      GLchar* log = (GLchar*)malloc(logLen);
      GLsizei written;
      glGetProgramInfoLog(pgmHandle, logLen, &written, log);
      printf("Program log: %s\n", log);
    }
  }
  return status;
}
// link the shader objects using the shader program
int linkShader(GLuint shaderPgm, GLuint newVertHandle, GLuint newFragHandle)
{
  const GLsizei maxCount = 2;
  GLsizei count;
  GLuint shaders[maxCount];
  glGetAttachedShaders(shaderPgm, maxCount, &count, shaders);
  // cout << "get VertHandle: " << shaders[0] << endl;
  // cout << "get FragHandle: " << shaders[1] << endl;
  GL_ERROR();
  for (int i = 0; i < count; i++) {
    glDetachShader(shaderPgm, shaders[i]);
  }
  // Bind index 0 to the shader input variable "VerPos"
  glBindAttribLocation(shaderPgm, 0, "VerPos");
  // Bind index 1 to the shader input variable "VerClr"
  glBindAttribLocation(shaderPgm, 1, "VerClr");
  GL_ERROR();
  glAttachShader(shaderPgm, newVertHandle);
  glAttachShader(shaderPgm, newFragHandle);
  GL_ERROR();
  glLinkProgram(shaderPgm);
  if (GL_FALSE == checkShaderLinkStatus(shaderPgm)) {
    printf("Failed to relink shader program!\n");
    return 0;
  }
  GL_ERROR();
  return 1;
}
// init 3D texture to store the volume data used fo ray casting
// type = GL_UNSIGNED_BYTE
GLuint initVol3DTex(const char* filename, GLuint w, GLuint h, GLuint d, int type)
{
  size_t size = w * h * d;
  GLuint volTexObj;
  buf_t bf[1] = {0};
  if (!buf_load(filename, bf)) {
    bfsetsize(bf, size);
    memcpy(bf->data, filename, size);
  }
  glGenTextures(1, &volTexObj);
  // bind 3D texture target
  glBindTexture(GL_TEXTURE_3D, g_volTexObj);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
  // pixel transfer happens here from client to OpenGL server
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, type, bf->data);
  bffree(bf);
  printf("volume texture created\n");
  return volTexObj;
}

