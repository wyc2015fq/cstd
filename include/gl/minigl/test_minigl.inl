
#include "cstd.h"
#include <assert.h>
#undef WINGDIAPI
#undef APIENTRY
#define APIENTRY
#define WINGDIAPI
//#define ASSERT assert
//#define BYTE_ORDER  BIG_ENDIAN
#include "ui/window.inl"

#include "gl.inl"
//#include "glx.inl"
#include "glx.h"
#include "glu.inl"

#define KEY_UP     0xe000
#define KEY_DOWN   0xe001
#define KEY_LEFT   0xe002
#define KEY_RIGHT  0xe003
#define KEY_ESCAPE 0xe004

#define None 0


#ifndef M_PI
#  define M_PI 3.14159265
#endif

#include "gl/glut.h"
#if 0
//#include "test_spin.inl"
#include "test_gears.inl"
//#include "test_mech.inl"
#include "test_tri.inl"
#include "test_sq.inl"
#else
#endif
#include "test_w32.inl"


int test_gl()
{
  //test_tri(0, 0);
  ui_loop(0, 0, "spin");
  return(0);
}
