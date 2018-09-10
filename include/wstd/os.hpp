

#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#define OS_WINDOWS
#elif  defined(__linux__)
#define OS_LINUX
#include <sys/time.h>
#endif
