#include "highgui/roiSelector.cpp"
#include "highgui/window.cpp"


#ifdef _WIN32
#include "highgui/window_w32.cpp"
#else

#if 0
CV_IMPL int cvInitSystem(int, char**) { return 0; }
void cvSetModeWindow_W32(const char* name, double prop_value) {}
void cvSetModeWindow_GTK(const char* name, double prop_value) {}
void cvSetModeWindow_CARBON(const char* name, double prop_value) {};
void cvSetModeWindow_COCOA(const char* name, double prop_value) {}
void cvSetModeWindow_WinRT(const char* name, double prop_value) {}

CvRect cvGetWindowRect_W32(const char* name) { return CvRect(); }
CvRect cvGetWindowRect_GTK(const char* name) { return CvRect(); }
CvRect cvGetWindowRect_CARBON(const char* name) { return CvRect(); }
CvRect cvGetWindowRect_COCOA(const char* name) { return CvRect(); }

double cvGetModeWindow_W32(const char* name) { return 0.0; }
double cvGetModeWindow_GTK(const char* name) { return 0.0; }
double cvGetModeWindow_CARBON(const char* name) { return 0.0; }
double cvGetModeWindow_COCOA(const char* name) { return 0.0; }
double cvGetModeWindow_WinRT(const char* name) { return 0.0; }

double cvGetPropWindowAutoSize_W32(const char* name) { return 0.0; }
double cvGetPropWindowAutoSize_GTK(const char* name) { return 0.0; }

double cvGetRatioWindow_W32(const char* name) { return 0.0; }
double cvGetRatioWindow_GTK(const char* name) { return 0.0; }

double cvGetOpenGlProp_W32(const char* name) { return 0.0; }
double cvGetOpenGlProp_GTK(const char* name) { return 0.0; }

#endif
#endif


#if 0
#include "highgui/src/window_carbon.cpp"
#include "highgui/src/window_gtk.cpp"
#include "highgui/src/window_QT.cpp"
#include "highgui/src/window_winrt.cpp"
#include "highgui/src/window_winrt_bridge.cpp"
#endif

