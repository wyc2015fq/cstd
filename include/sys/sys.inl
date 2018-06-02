
#include "cstd.h"
#include "parallel.inl"

#if 1
#ifdef _WIN32
#include "sys/sys_win32.inl"
#elif defined __linux__
#include "sys/sys_linux.inl"
#else
#error unkown system
#endif
#else
#include "sys/sys_null.inl"
#endif


//#include "sys/threadpool.inl"
//#include "parallel_win32.inl"
