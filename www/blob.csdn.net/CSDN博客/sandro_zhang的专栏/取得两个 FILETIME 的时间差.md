# 取得两个 FILETIME 的时间差 - sandro_zhang的专栏 - CSDN博客
2011年12月28日 16:59:15[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：4644
如果为SYSTEMTIME，可以通过SystemTimeToFileTime先转到FILETIME
```cpp
#define NOMINMAX 
#include <algorithm> 
#include <windows.h> 
 
SYSTEMTIME systemtime_difference(SYSTEMTIME const& st1, SYSTEMTIME const& st2) 
{ 
    FILETIME ft1; 
    SystemTimeToFileTime(&st1, &ft1); 
    ULARGE_INTEGER const u1 = { ft1.dwLowDateTime, ft1.dwHighDateTime }; 
 
    FILETIME ft2; 
    SystemTimeToFileTime(&st2, &ft2); 
    ULARGE_INTEGER const u2 = { ft2.dwLowDateTime, ft2.dwHighDateTime }; 
 
    ULARGE_INTEGER u3; 
    u3.QuadPart = std::max(u1.QuadPart, u2.QuadPart) - std::min(u1.QuadPart, u2.QuadPart); 
    FILETIME const ft3 = { u3.LowPart, u3.HighPart }; 
 
    SYSTEMTIME st3; 
    FileTimeToSystemTime(&ft3, &st3); 
    // FILETIME's starting point is 1601-01-01 
    st3.wYear -= 1601; 
    st3.wMonth -= 1; 
    st3.wDay -= 1; 
    st3.wDayOfWeek = 0; // useless for this purpose, make it always 0 so it's always ignored 
    return st3; 
} 
 
int main() 
{ 
    SYSTEMTIME const st1 = { 2008, 10, 3, 15, 6, 5, 31, 12 }; 
    SYSTEMTIME const st2 = { 2009, 12, 0, 27, 10, 7, 59, 56 }; 
    SYSTEMTIME st3 = systemtime_difference(st1, st2); 
 
    return 0; 
}
```
感谢：[http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/fe975619-5e19-45e0-82cd-797ff7745712](http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/fe975619-5e19-45e0-82cd-797ff7745712)
