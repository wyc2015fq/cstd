#ifdef WIN32
//#define WINVER 0x0500
//#define _WIN32_WINNT 0x0500 // for Mouse Wheel support
#include "window_w32.h"
#endif // WIN32
#if (defined __linux || defined __linux__)
#include "window_gtk.h"
#endif // __linux__

