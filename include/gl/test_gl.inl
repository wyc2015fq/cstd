
#include <windows.h>
#include <gl/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#if 1
//#define gluOrtho2D(left, right, bottom, top) glOrtho(left, right, bottom, top, -1, 1)
//#include "E:\code\c\Graphics\OpenGL\openglex-master\gltools\libgltools.inl"
#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glew32.lib")
#include "mygl/test2.inl"
#else
#include "E:\code\c\Graphics\OpenGL\openglex-master\src\motionblur.cpp"
#include "mygl/test_select.inl"
#include "mygl/test_mygl.inl"
#include "test/double.inl"
#define test_tri main
#include "gl/test_tri.inl"
#include "varray.inl"
#endif

