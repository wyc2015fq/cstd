
#ifndef _LIBCAP_INL_
#define _LIBCAP_INL_

//#define WINVER
//#include <windows.h>
//#include "cstd.h"
//#include "cap.h"
//#include "img\color.inl"
//#include "img/color.inl"
#define CAPE_OPEN_DEF(name, impl)  int name(cap_t* s, int idx, int width, int height) {return impl(s, idx, width, height);}
#ifdef _WIN32
#undef CC_CDECL
#undef CC_STDCALL
#include "capdshow.inl"
#include "capvfw.inl"
#include "capvfwavi.inl"
#ifdef __cplusplus
//#include "EroadVideo.inl"
#else
//CAPE_OPEN_DEF(caperoad_open, capdshow_open)
#endif
//CAPE_OPEN_DEF(capv4l_open, capdshow_open)
#endif

#ifndef _MBCS
#endif
#if _MSC_VER > 1400
//#include "capopenni.inl"
#endif
#ifdef __linux__
#include "capv4l.inl"
CAPE_OPEN_DEF(caperoad_open, capv4l_open)
CAPE_OPEN_DEF(capdshow_open, capv4l_open)
CAPE_OPEN_DEF(capopenni_open, capv4l_open)
CAPE_OPEN_DEF(capvfw_open, capv4l_open)
#endif

#endif // _LIBCAP_INL_
