
#if GLLIB == SYSGL
#if _MSC_VER < 1300
#include <../../VC98/Include/GL/gl.h>
#else
#include <C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include\gl\GL.h>
#endif
#elif GLLIB == MYGL
#include "mygl/gl.inl"
#elif GLLIB == MINIGL
#include "minigl/gl.h"
#endif
