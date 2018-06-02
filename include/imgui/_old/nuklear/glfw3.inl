
#ifdef _WIN32
#undef WINVER
#undef _WIN32_WINNT
#undef _WIN32_IE
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0501
#endif

#include <gl/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glew32.lib")
#include "gl/GLFW/glfw3.h"
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
//#include <GLFW/glfw3native.h>
#include "gl/GLFW/glfw3native.h"
#endif
#include "gl/glfw/libglfw3.inl"
//#include "GLFW/glfw3.h"