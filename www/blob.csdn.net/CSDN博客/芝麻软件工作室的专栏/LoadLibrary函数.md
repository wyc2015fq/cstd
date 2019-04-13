
# LoadLibrary函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:21:51[seven-soft](https://me.csdn.net/softn)阅读数：246标签：[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[产品																](https://so.csdn.net/so/search/s.do?q=产品&t=blog)[任务																](https://so.csdn.net/so/search/s.do?q=任务&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=产品&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=windows&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=windows&t=blog)


随着软件规模的扩大，要求的功能也是越来越多，开发人员的参与也是越来越多。因此软件的功能划分，就成为了现代软件工程的重大任务，还有软件开发的并行性也越来越重要。为了解决这些问题，大家都会看到IT硬件发展非常迅速，功能也越来越复杂，但硬件中发展明显提高在于采用IC的方式来实现复杂的功能，也就是把大部份功能集成到一起，只要给出一些引脚就可以实现产品了。而软件中有没有相同于硬件中的IC呢？我想是应有的，就是动态连接库了。在Windows这座大厦里，很多基石就是动态连接库构成的。一个动态连接库就封装了特别复杂的功能，使用者不必关心它是怎么样实现的。当然，这样也可以让不同的开发者同时开发产品，提高软件开发的速度。要使用动态连接库里的函数，就需要使用LoadLibrary函数来加载动态连接库，使用函数GetProcAddress来获取功能函数的地址。
函数LoadLibrary声明如下：
WINBASEAPI
__out
HMODULE
WINAPI
LoadLibraryA(
__in LPCSTR lpLibFileName
);
WINBASEAPI
__out
HMODULE
WINAPI
LoadLibraryW(
__in LPCWSTR lpLibFileName
);
\#ifdef UNICODE
\#define LoadLibrary LoadLibraryW
\#else
\#define LoadLibrary LoadLibraryA
\#endif // !UNICODE
**lpLibFileName**是动态连接库的名称。
调用函数的例子如下：
\#001 //加载动态连接库。
\#002  //
\#003  void TestLoadDLL(void)
\#004  {
\#005         //加载动态连接库。
**\#006        HMODULE hDllLib = LoadLibrary(_T("Kernel32.dll"));**
\#007         if (hDllLib)
\#008         {
\#009               //获取动态连接库里的函数地址。
\#010**FARPROC fpFun = GetProcAddress(hDllLib,"GetVersion");**
\#011
\#012               //调用函数运行。
\#013               DWORD dwVersion = (*fpFun)();
\#014
\#015               //获取WINDOWS的版本。
\#016               DWORD dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
\#017               DWORD dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
\#018
\#019               //显示。
\#020               const int nBufSize = 512;
\#021               TCHAR chBuf[nBufSize];
\#022               ZeroMemory(chBuf,nBufSize);
\#023
\#024               wsprintf(chBuf,_T("显示版本：%d,%d/r/n"),
\#025                    dwWindowsMajorVersion,dwWindowsMinorVersion);
\#026               OutputDebugString(chBuf);
\#027
\#028               //释放动态连接库。
\#029               FreeLibrary(hDllLib);
\#030         }
\#031
\#032  }


