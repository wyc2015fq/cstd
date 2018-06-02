#include "cstd.h"
#define MSGSTR_DEFDEF(MSGSTRDEF) \
  MSGSTRDEF(MSG_NOP) \
  MSGSTRDEF(MSG_TIMER) \
  MSGSTRDEF(MSG_LBUTTONDOWN) \
  MSGSTRDEF(MSG_LBUTTONUP) \
  MSGSTRDEF(MSG_RBUTTONDOWN) \
  MSGSTRDEF(MSG_RBUTTONUP) \
  MSGSTRDEF(MSG_MBUTTONDOWN) \
  MSGSTRDEF(MSG_MBUTTONUP) \
  MSGSTRDEF(MSG_LBUTTONDBLCLK) \
  MSGSTRDEF(MSG_RBUTTONDBLCLK) \
  MSGSTRDEF(MSG_MBUTTONDBLCLK) \
  MSGSTRDEF(MSG_MOUSEWHEEL) \
  MSGSTRDEF(MSG_MOUSEWHEELUP) \
  MSGSTRDEF(MSG_MOUSEWHEELDOWN) \
  MSGSTRDEF(MSG_MOUSEMOVE) \
  MSGSTRDEF(MSG_KEYDOWN) \
  MSGSTRDEF(MSG_KEYUP) \
  MSGSTRDEF(MSG_SIZE) \
  MSGSTRDEF(MSG_CHAR) \
  MSGSTRDEF(MSG_CLOSE)
const char* msg2str(int msg)
{
#define MSGSTRDEF(x) case x: return #x;
  switch (msg) {
    MSGSTR_DEFDEF(MSGSTRDEF)
#undef MSGSTRDEF
  }
  return "unkown";
}
typedef struct sys_event_t {
  int msg;
  int last_key;
  int keyctrl;
  int keyshift;
  int keyalt;
  int tick;
  int x, y;
  int w, h;
  float wheelDelta;
  int keydown[512];
} sys_event_t;
sys_event_t* sys_waitevent(int delay);
#include "obj.inl"
void rotatef(float angle, float x, float y, float z);
int new_win(const char* windowName, int winX, int winY, int winWidth, int winHeight);
void gcVertexPointer(int size, GCTYPE type, int stride, const void* pointer);
void gcDrawArrays(GcMode mode, int first, int count);
void gcDrawElements(GcMode mode, int count, GCTYPE type, const void* indices);
void display(void) ;
int test_DrawArrays()
{
  const int stride = 2 * sizeof(float);
  float triangles[][2] = { {0, 0}, {0, 1}, {1, 1}, {1, 1}, {1, 0}, {0, 0} };
  gcVertexPointer(2, GC_FLOAT, stride, triangles);
  gcDrawArrays(GC_TRIANGLES, 0, sizeof(triangles) / stride);
#if 0
  float triangleStrip[][2] = { {0, 1}, {0, 0}, {1, 1}, {1, 0} };
  gcVertexPointer(2, GC_FLOAT, stride, triangleStrip);
  gcDrawArrays(GC_TRIANGLE_STRIP, 0, sizeof(triangleStrip) / stride);
  float triangleFan[][2] = { {0, 0}, {0, 1}, {1, 1}, {1, 0} };
  gcVertexPointer(2, GC_FLOAT, stride, triangleFan);
  gcDrawArrays(GC_TRIANGLE_FAN, 0, sizeof(triangleFan) / stride);
#endif
  return 0;
}
int drawbox()
{
  float vVertices[] = {
    0.0f, 0.0, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0, 0.0f,
    0.0f, 1.0f, 0.0f,
  };
  int vIds[] = {
    0, 1, 2,
    3, 4, 5
  };
  gcVertexPointer(3, GC_FLOAT, 0, vVertices);
  gcDrawElements(GC_TRIANGLES, 3, GC_INT32, vIds);
  return 0;
}
#include "gl2_win32.inl"
#include "test_draw.inl"
#include "nehe/Lesson48.inl"
#include "nehe/test_cube.inl"
#include "ogldev/pipeline.inl"
#if 0
#include "ogldev/tutorial02.inl"
#include "ogldev/tutorial03.inl"
#include "ogldev/tutorial04.inl"
#include "ogldev/tutorial05.inl"
#include "ogldev/tutorial06.inl"
#include "ogldev/tutorial07.inl"
#include "ogldev/tutorial08.inl"
#include "ogldev/tutorial09.inl"
#include "ogldev/tutorial10.inl"
#include "ogldev/tutorial11.inl"
#include "ogldev/tutorial12.inl"
#include "ogldev/tutorial13.inl"
#include "ogldev/tutorial14.inl"
#include "ogldev/tutorial15.inl"
#include "ogldev/tutorial16.inl"
#include "ogldev/tutorial17.inl"
#include "ogldev/tutorial18.inl"
#include "ogldev/tutorial19.inl"
#endif
#include "ogldev/tutorial20.inl"
#include "test_raycast1.inl"
#include "panoramic.inl"
#include "vtk/test_vtk.inl"
#include "vtk/test_Quaternion.inl"
//#include "ogldev/tutorial11.inl"
//#include "vtk/VolumeRayCast_OpenGL2.inl"
int test_3d()
{
  //return test_QShortestArc();
  sys_event_t* event;
  new_win("asdf", 0, 0, 800, 600);
  //float aa = 0;
  for (; event = sys_waitevent(20);) {
    //rotatef(aa, 0, 1, 1);
    //display();
    //draw3dbox();
    //test_cube(event);
    tutorial20(event);
    //test_raycast1(event);
    //drawbox();
    //test_DrawArrays();
    //test_DrawArrays();
    //aa = event->tick * 0.05;
    printf("%5d %-20s %3d %3d\n", event->tick, msg2str(event->msg), event->x, event->y);
  }
  return 0;
}
//#include "sys/sys_win32.inl"

