//#include "VolumeRayCast_OpenGL2.inl"
#include <Windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include <gl/glut.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glut32.lib")
const char* getGLErrorInfo(int errorId)
{
  switch (errorId) {
  case GL_INVALID_ENUM:
    return ("GL Invalid Enum\n");
  case GL_INVALID_VALUE:
    return ("GL Invalid Value\n");
  case GL_INVALID_OPERATION:
    return ("GL Invalid Operation\n");
  case GL_OUT_OF_MEMORY:
    return ("GL Out Of Memory\n");
    //case GL_INVALID_FRAMEBUFFER_OPERATION:
    // return ("GL Invalid FrameBuffer Operation\n");
  case GL_STACK_OVERFLOW:
    return ("GL Stack Overflow\n");
  case GL_STACK_UNDERFLOW:
    return ("GL Stack Underflow\n");
    //case GL_TABLE_TOO_LARGE:
    // return ("GL Table Too Large\n");
  };
  return "GL Undefined Error";
}
int test_vtk2(int argc, char** argv)
{
  ///BuildShader(1);
  return 0;
}

