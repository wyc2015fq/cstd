#ifndef _CSTD_H_
#define _CSTD_H_

#ifdef _WINDOWS
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4305)
#pragma warning(disable : 4244)
#pragma warning(disable : 4127) // conditional expression is constant
#pragma warning(disable : 4189) // local variable is initialized but not referenced
#pragma warning(disable : 4310) // cast truncates constant value
#pragma warning(disable : 4100) // unreferenced formal parameter
#pragma warning(disable : 4245) 
#pragma warning(disable : 4068) // unknown pragma
#endif

#include "cetc.h"
#include "ctyp.h"
#include "csys.h"
#include "calg.h"
#include "cmem.h"
#include "cgeo.h"
#include "cstr.h"
#include "wstr.h"
#include "cimg.h"
#include "cimg2.h"
#include "cgui.h"
//#define C_DBG
#include "cdbg.h"

#undef CC_CDECL
#undef CC_STDCALL

//extern "C" {}
#endif // _CSTD_H_

