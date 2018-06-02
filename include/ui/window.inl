#ifndef _WINDOW_INL_
#define _WINDOW_INL_
#include "cstd.h"
#include "str.h"


#ifdef _WIN32
#include "window_win32.inl"
#endif // WIN32
#if (defined __linux || defined __linux__)
#ifdef HAVE_GTK
#include "window_gtk.inl"
#else
#include "window_xcb.inl"
#endif
#endif // __linux__

#endif // _WINDOW_INL_

