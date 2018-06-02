
#ifndef __MINIGLUT_H__
#define __MINIGLUT_H__

#ifndef GLUT_API_VERSION  /* allow this to be overriden */
#define GLUT_API_VERSION    3
#endif

/* Display mode bit masks. */
#define GLUT_RGB        0
#define GLUT_RGBA     GLUT_RGB
#define GLUT_INDEX      1
#define GLUT_SINGLE     0
#define GLUT_DOUBLE     2
#define GLUT_ACCUM      4
#define GLUT_ALPHA      8
#define GLUT_DEPTH      16
#define GLUT_STENCIL      32
#if (GLUT_API_VERSION >= 2)
#define GLUT_MULTISAMPLE    128
#define GLUT_STEREO     256
#endif
#if (GLUT_API_VERSION >= 3)
#define GLUT_LUMINANCE      512
#endif

/* Mouse buttons. */
#define GLUT_LEFT_BUTTON    0
#define GLUT_MIDDLE_BUTTON    1
#define GLUT_RIGHT_BUTTON   2

/* Mouse button  state. */
#define GLUT_DOWN     0
#define GLUT_UP       1

#if (GLUT_API_VERSION >= 2)
/* function keys */
#define GLUT_KEY_F1     1
#define GLUT_KEY_F2     2
#define GLUT_KEY_F3     3
#define GLUT_KEY_F4     4
#define GLUT_KEY_F5     5
#define GLUT_KEY_F6     6
#define GLUT_KEY_F7     7
#define GLUT_KEY_F8     8
#define GLUT_KEY_F9     9
#define GLUT_KEY_F10      10
#define GLUT_KEY_F11      11
#define GLUT_KEY_F12      12
/* directional keys */
#define GLUT_KEY_LEFT     100
#define GLUT_KEY_UP     101
#define GLUT_KEY_RIGHT      102
#define GLUT_KEY_DOWN     103
#define GLUT_KEY_PAGE_UP    104
#define GLUT_KEY_PAGE_DOWN    105
#define GLUT_KEY_HOME     106
#define GLUT_KEY_END      107
#define GLUT_KEY_INSERT     108
#define GLUT_KEY_ESCAPE     109
#endif

/* Entry/exit  state. */
#define GLUT_LEFT     0
#define GLUT_ENTERED      1

/* Menu usage  state. */
#define GLUT_MENU_NOT_IN_USE    0
#define GLUT_MENU_IN_USE    1

/* Visibility  state. */
#define GLUT_NOT_VISIBLE    0
#define GLUT_VISIBLE      1

/* Window status  state. */
#define GLUT_HIDDEN     0
#define GLUT_FULLY_RETAINED   1
#define GLUT_PARTIALLY_RETAINED   2
#define GLUT_FULLY_COVERED    3

/* Color index component selection values. */
#define GLUT_RED      0
#define GLUT_GREEN      1
#define GLUT_BLUE     2


#include "glut_w32.inl"

#endif // __MYGLUT_H__
