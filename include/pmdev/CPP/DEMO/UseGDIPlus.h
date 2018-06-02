
#ifndef _USE_GDIPLUS_
#define _USE_GDIPLUS_
                                                                 
#include "comdef.h"
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#include <objbase.h>
#include "GdiPlus.h"
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
#endif

BOOL InitGIDPlus();
BOOL UinitGIDPlus();
BOOL GetImageCLSID(const WCHAR* format, CLSID* pCLSID);

#endif
