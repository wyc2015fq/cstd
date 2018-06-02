#ifndef _GLUT_INL_
#define _GLUT_INL_
#include "glu.inl"
#ifndef GLUT_API_VERSION /* allow this to be overriden */
#define GLUT_API_VERSION 3
#endif
/* Display mode bit masks. */
#define GLUT_RGB 0
#define GLUT_RGBA GLUT_RGB
#define GLUT_INDEX 1
#define GLUT_SINGLE 0
#define GLUT_DOUBLE 2
#define GLUT_ACCUM 4
#define GLUT_ALPHA 8
#define GLUT_DEPTH 16
#define GLUT_STENCIL 32
#define GLUT_MULTISAMPLE 128
#define GLUT_STEREO 256
#define GLUT_LUMINANCE 512
/* Mouse buttons. */
#define GLUT_LEFT_BUTTON 0
#define GLUT_MIDDLE_BUTTON 1
#define GLUT_RIGHT_BUTTON 2
/* Mouse button state. */
#define GLUT_DOWN 0
#define GLUT_UP 1
/* function keys */
#define GLUT_KEY_F1 1
#define GLUT_KEY_F2 2
#define GLUT_KEY_F3 3
#define GLUT_KEY_F4 4
#define GLUT_KEY_F5 5
#define GLUT_KEY_F6 6
#define GLUT_KEY_F7 7
#define GLUT_KEY_F8 8
#define GLUT_KEY_F9 9
#define GLUT_KEY_F10 10
#define GLUT_KEY_F11 11
#define GLUT_KEY_F12 12
/* directional keys */
#define GLUT_KEY_LEFT 100
#define GLUT_KEY_UP 101
#define GLUT_KEY_RIGHT 102
#define GLUT_KEY_DOWN 103
#define GLUT_KEY_PAGE_UP 104
#define GLUT_KEY_PAGE_DOWN 105
#define GLUT_KEY_HOME 106
#define GLUT_KEY_END 107
#define GLUT_KEY_INSERT 108
#define GLUT_KEY_ESCAPE 109
/* Entry/exit state. */
#define GLUT_LEFT 0
#define GLUT_ENTERED 1
/* Menu usage state. */
#define GLUT_MENU_NOT_IN_USE 0
#define GLUT_MENU_IN_USE 1
/* Visibility state. */
#define GLUT_NOT_VISIBLE 0
#define GLUT_VISIBLE 1
/* Window status state. */
#define GLUT_HIDDEN 0
#define GLUT_FULLY_RETAINED 1
#define GLUT_PARTIALLY_RETAINED 2
#define GLUT_FULLY_COVERED 3
/* Color index component selection values. */
#define GLUT_RED 0
#define GLUT_GREEN 1
#define GLUT_BLUE 2
#include "glut_w32.inl"
/* GLUT initialization sub-API. */
void glutInit(int* argcp, char** argv);
void glutInitDisplayMode(unsigned int mode);
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
void glutInitDisplayString(const char* string);
#endif
void glutInitWindowPosition(int x, int y);
void glutInitWindowSize(int width, int height);
/* GLUT window sub-API. */
int glutCreateWindow(const char* title);
int glutCreateSubWindow(int win, int x, int y, int width, int height);
void glutDestroyWindow(int win);
void glutPostRedisplay(void);
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 11)
void glutPostWindowRedisplay(int win);
#endif
void glutSwapBuffers(void);
int glutGetWindow(void);
void glutSetWindow(int win);
void glutSetWindowTitle(const char* title);
void glutSetIconTitle(const char* title);
void glutPositionWindow(int x, int y);
void glutReshapeWindow(int width, int height);
void glutPopWindow(void);
void glutPushWindow(void);
void glutIconifyWindow(void);
void glutShowWindow(void);
void glutHideWindow(void);
#if (GLUT_API_VERSION >= 3)
void glutFullScreen(void);
void glutSetCursor(int cursor);
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
void glutWarpPointer(int x, int y);
#endif
/* GLUT overlay sub-API. */
void glutEstablishOverlay(void);
void glutRemoveOverlay(void);
void glutUseLayer(GLenum layer);
void glutPostOverlayRedisplay(void);
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 11)
void glutPostWindowOverlayRedisplay(int win);
#endif
void glutShowOverlay(void);
void glutHideOverlay(void);
#endif
/* GLUT menu sub-API. */
int glutCreateMenu(void (GLUTCALLBACK* func)(int));
#if defined(_WIN32) && !defined(GLUT_DISABLE_ATEXIT_HACK)
int __glutCreateMenuWithExit(void (GLUTCALLBACK* func)(int), void (__cdecl* exitfunc)(int));
#ifndef GLUT_BUILDING_LIB
static int glutCreateMenu_ATEXIT_HACK(void (GLUTCALLBACK* func)(int))
{
  return __glutCreateMenuWithExit(func, exit);
}
#define glutCreateMenu glutCreateMenu_ATEXIT_HACK
#endif
#endif
void glutDestroyMenu(int menu);
int glutGetMenu(void);
void glutSetMenu(int menu);
void glutAddMenuEntry(const char* label, int value);
void glutAddSubMenu(const char* label, int submenu);
void glutChangeToMenuEntry(int item, const char* label, int value);
void glutChangeToSubMenu(int item, const char* label, int submenu);
void glutRemoveMenuItem(int item);
void glutAttachMenu(int button);
void glutDetachMenu(int button);
/* GLUT window callback sub-API. */
void glutReshapeFunc(void (GLUTCALLBACK* func)(int width, int height));
void glutMouseFunc(void (GLUTCALLBACK* func)(int button, int state, int x, int y));
void glutMotionFunc(void (GLUTCALLBACK* func)(int x, int y));
void glutPassiveMotionFunc(void (GLUTCALLBACK* func)(int x, int y));
void glutEntryFunc(void (GLUTCALLBACK* func)(int state));
void glutVisibilityFunc(void (GLUTCALLBACK* func)(int state));
void glutIdleFunc(void (GLUTCALLBACK* func)(void));
void glutTimerFunc(unsigned int millis, void (GLUTCALLBACK* func)(int value), int value);
void glutMenuStateFunc(void (GLUTCALLBACK* func)(int state));
#if (GLUT_API_VERSION >= 2)
void glutSpecialFunc(void (GLUTCALLBACK* func)(int key, int x, int y));
void glutSpaceballMotionFunc(void (GLUTCALLBACK* func)(int x, int y, int z));
void glutSpaceballRotateFunc(void (GLUTCALLBACK* func)(int x, int y, int z));
void glutSpaceballButtonFunc(void (GLUTCALLBACK* func)(int button, int state));
void glutButtonBoxFunc(void (GLUTCALLBACK* func)(int button, int state));
void glutDialsFunc(void (GLUTCALLBACK* func)(int dial, int value));
void glutTabletMotionFunc(void (GLUTCALLBACK* func)(int x, int y));
void glutTabletButtonFunc(void (GLUTCALLBACK* func)(int button, int state, int x, int y));
#if (GLUT_API_VERSION >= 3)
void glutMenuStatusFunc(void (GLUTCALLBACK* func)(int status, int x, int y));
void glutOverlayDisplayFunc(void (GLUTCALLBACK* func)(void));
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
void glutWindowStatusFunc(void (GLUTCALLBACK* func)(int state));
#endif
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
void glutKeyboardUpFunc(void (GLUTCALLBACK* func)(unsigned char key, int x, int y));
void glutSpecialUpFunc(void (GLUTCALLBACK* func)(int key, int x, int y));
void glutJoystickFunc(void (GLUTCALLBACK* func)(unsigned int buttonMask, int x, int y, int z), int pollInterval);
#endif
#endif
#endif
/* GLUT color index sub-API. */
void glutSetColor(int, GLfloat red, GLfloat green, GLfloat blue);
GLfloat glutGetColor(int ndx, int component);
void glutCopyColormap(int win);
/* GLUT state retrieval sub-API. */
int glutGet(GLenum type);
int glutDeviceGet(GLenum type);
#if (GLUT_API_VERSION >= 2)
/* GLUT extension support sub-API */
int glutExtensionSupported(const char* name);
#endif
#if (GLUT_API_VERSION >= 3)
int glutGetModifiers(void);
int glutLayerGet(GLenum type);
#endif
/* GLUT font sub-API */
void glutBitmapCharacter(void* font, int character);
int glutBitmapWidth(void* font, int character);
void glutStrokeCharacter(void* font, int character);
int glutStrokeWidth(void* font, int character);
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
int glutBitmapLength(void* font, const unsigned char* string);
int glutStrokeLength(void* font, const unsigned char* string);
#endif
/* GLUT pre-built models sub-API */
void glutWireSphere(GLdouble radius, GLint slices, GLint stacks);
//球心坐标为（x，y，z），球的半径为radius，M，N分别表示球体的横纵向被分成多少份
void drawSphere(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat radius, GLint M, GLint N)
{
  float step_z = PI / M;
  float step_xy = 2 * PI / N;
  float x[4], y[4], z[4];
  float angle_z = 0.0;
  float angle_xy = 0.0;
  int i = 0, j = 0, k;
  glBegin(GL_QUADS);
  for (i = 0; i < M; i++) {
    angle_z = i * step_z;
    for (j = 0; j < N; j++) {
      angle_xy = j * step_xy;
      x[0] = radius * sin(angle_z) * cos(angle_xy);
      y[0] = radius * sin(angle_z) * sin(angle_xy);
      z[0] = radius * cos(angle_z);
      x[1] = radius * sin(angle_z + step_z) * cos(angle_xy);
      y[1] = radius * sin(angle_z + step_z) * sin(angle_xy);
      z[1] = radius * cos(angle_z + step_z);
      x[2] = radius * sin(angle_z + step_z) * cos(angle_xy + step_xy);
      y[2] = radius * sin(angle_z + step_z) * sin(angle_xy + step_xy);
      z[2] = radius * cos(angle_z + step_z);
      x[3] = radius * sin(angle_z) * cos(angle_xy + step_xy);
      y[3] = radius * sin(angle_z) * sin(angle_xy + step_xy);
      z[3] = radius * cos(angle_z);
      for (k = 0; k < 4; k++) {
        glNormal3f(x[k], y[k], z[k]);
        glVertex3f(xx + x[k], yy + y[k], zz + z[k]);
      }
    }
  }
  glEnd();
}
void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks)
{
  drawSphere(0, 0, 0, radius, slices, stacks);
}
void glutWireCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
void glutSolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
void glutWireCube(GLdouble size);
void glutSolidCube(GLdouble size);
void glutWireTorus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings);
void glutSolidTorus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings);
void glutWireDodecahedron(void);
void glutSolidDodecahedron(void);
void glutWireTeapot(GLdouble size);
void glutSolidTeapot(GLdouble size);
void glutWireOctahedron(void);
void glutSolidOctahedron(void);
void glutWireTetrahedron(void);
void glutSolidTetrahedron(void);
void glutWireIcosahedron(void);
void glutSolidIcosahedron(void);
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
/* GLUT video resize sub-API. */
int glutVideoResizeGet(GLenum param);
void glutSetupVideoResizing(void);
void glutStopVideoResizing(void);
void glutVideoResize(int x, int y, int width, int height);
void glutVideoPan(int x, int y, int width, int height);
/* GLUT debugging sub-API. */
void glutReportErrors(void);
#endif
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
/* GLUT device control sub-API. */
/* glutSetKeyRepeat modes. */
#define GLUT_KEY_REPEAT_OFF 0
#define GLUT_KEY_REPEAT_ON 1
#define GLUT_KEY_REPEAT_DEFAULT 2
/* Joystick button masks. */
#define GLUT_JOYSTICK_BUTTON_A 1
#define GLUT_JOYSTICK_BUTTON_B 2
#define GLUT_JOYSTICK_BUTTON_C 4
#define GLUT_JOYSTICK_BUTTON_D 8
void glutIgnoreKeyRepeat(int ignore);
void glutSetKeyRepeat(int repeatMode);
void glutForceJoystickFunc(void);
/* GLUT game mode sub-API. */
/* glutGameModeGet. */
#define GLUT_GAME_MODE_ACTIVE ((GLenum) 0)
#define GLUT_GAME_MODE_POSSIBLE ((GLenum) 1)
#define GLUT_GAME_MODE_WIDTH ((GLenum) 2)
#define GLUT_GAME_MODE_HEIGHT ((GLenum) 3)
#define GLUT_GAME_MODE_PIXEL_DEPTH ((GLenum) 4)
#define GLUT_GAME_MODE_REFRESH_RATE ((GLenum) 5)
#define GLUT_GAME_MODE_DISPLAY_CHANGED ((GLenum) 6)
void glutGameModeString(const char* string);
int glutEnterGameMode(void);
void glutLeaveGameMode(void);
int glutGameModeGet(GLenum mode);
#endif
#endif // _GLUT_INL_

