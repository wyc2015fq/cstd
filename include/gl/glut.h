
#if GLLIB == SYSGL
#include "lib/glut-3.7/include/GL/glut.h"
#pragma comment(lib,"glut32.lib")
#elif GLLIB == MYGL
#include "mygl/glut.inl"
#elif GLLIB == MINIGL
#include "minigl/glut.h"
#endif
