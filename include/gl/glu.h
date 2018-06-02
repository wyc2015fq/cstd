
#if GLLIB == SYSGL
#if _MSC_VER < 1300
#include <../../VC98/Include/GL/glu.h>
#else
#include <C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include\gl\glu.h>
#endif
#elif GLLIB == MYGL
#include "mygl/glu.inl"
#elif GLLIB == MINIGL
#include "minigl/glu.h"
#endif
