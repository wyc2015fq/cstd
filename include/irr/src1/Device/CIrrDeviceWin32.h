// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_IRR_DEVICE_WIN32_H_INCLUDED__
#define __C_IRR_DEVICE_WIN32_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_
//#include "CIrrDeviceStub.h"
//#include "IrrlichtDevice.h"
//#include "IImagePresenter.h"
#define WIN32_LEAN_AND_MEAN
#if !defined(_IRR_XBOX_PLATFORM_)
//#include <windows.h>
//#include <mmsystem.h> // For JOYCAPS
//#include <windowsx.h>
#endif
#if !defined(GET_X_LPARAM)
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif
typedef struct SJoystickWin32Control SJoystickWin32Control;


#include "CIrrDeviceStub.h"
#include "CIrrDeviceStub_impl.h"


  
  
#if 0
#endif
#endif // _IRR_COMPILE_WITH_WINDOWS_DEVICE_
#endif // __C_IRR_DEVICE_WIN32_H_INCLUDED__

