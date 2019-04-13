
# CreateFileMapping和MapViewOfFile函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:33:50[seven-soft](https://me.csdn.net/softn)阅读数：210标签：[attributes																](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)[access																](https://so.csdn.net/so/search/s.do?q=access&t=blog)[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[图形																](https://so.csdn.net/so/search/s.do?q=图形&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=图形&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=file&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=file&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=access&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=access&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)

在开发软件过程里，也经常碰到进程间共享数据的需求。比如A进程创建计算数据，B进程进行显示数据的图形。这样的开发方式可以把一个大程序分开成独立的小程序，提高软件的成功率，也可以更加适合团队一起开发，加快软件的开发速度。下面就来使用文件映射的方式进行共享数据。先要使用函数CreateFileMapping来创建一个想共享的文件数据句柄，然后使用MapViewOfFile来获取共享的内存地址，然后使用OpenFileMapping函数在另一个进程里打开共享文件的名称，这样就可以实现不同的进程共享数据。
函数CreateFileMapping、MapViewOfFile声明如下：
WINBASEAPI
__out
HANDLE
WINAPI
CreateFileMappingA(
__in     HANDLE hFile,
__in_opt LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
__in     DWORD flProtect,
__in     DWORD dwMaximumSizeHigh,
__in     DWORD dwMaximumSizeLow,
__in_opt LPCSTR lpName
);
WINBASEAPI
__out
HANDLE
WINAPI
CreateFileMappingW(
__in     HANDLE hFile,
__in_opt LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
__in     DWORD flProtect,
__in     DWORD dwMaximumSizeHigh,
__in     DWORD dwMaximumSizeLow,
__in_opt LPCWSTR lpName
);
\#ifdef UNICODE
\#define CreateFileMapping CreateFileMappingW
\#else
\#define CreateFileMapping CreateFileMappingA
\#endif // !UNICODE
WINBASEAPI
__out
LPVOID
WINAPI
MapViewOfFile(
__in HANDLE hFileMappingObject,
__in DWORD dwDesiredAccess,
__in DWORD dwFileOffsetHigh,
__in DWORD dwFileOffsetLow,
__in SIZE_T dwNumberOfBytesToMap
);
**hFile**是创建共享文件的句柄。
**lpFileMappingAttributes**是文件共享的属性。
**flProtect**是当文件映射时读写文件的属性。
**dwMaximumSizeHigh**是文件共享的大小高位字节。
**dwMaximumSizeLow**是文件共享的大小低位字节。
**lpName**是共享文件对象名称。
**hFileMappingObject**是共享文件对象。
**dwDesiredAccess**是文件共享属性。
**dwFileOffsetHigh**是文件共享区的偏移地址。
**dwFileOffsetLow**是文件共享区的偏移地址。
**dwNumberOfBytesToMap**是共享数据长度。
调用函数的例子如下：
\#001  //文件共享。
\#002  //
\#003  void FileMapping(void)
\#004  {
\#005         //打开共享的文件对象。
**\#006         m_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,**
**\#007              FALSE,_T("TestFileMap"));**
\#008         if (m_hMapFile)
\#009         {
\#010               //显示共享的文件数据。
**\#011              LPTSTR lpMapAddr = (LPTSTR)MapViewOfFile(m_hMapFile,FILE_MAP_ALL_ACCESS,**
**\#012                   0,0,0);**
\#013               OutputDebugString(lpMapAddr);
\#014         }
\#015         else
\#016         {
\#017               //创建共享文件。
**\#018               m_hMapFile = CreateFileMapping( (HANDLE)0xFFFFFFFF,NULL,**
**\#019                   PAGE_READWRITE,0,1024,_T("TestFileMap"));**
\#020
\#021               //拷贝数据到共享文件里。
\#022               LPTSTR lpMapAddr = (LPTSTR)MapViewOfFile(m_hMapFile,FILE_MAP_ALL_ACCESS,
\#023                    0,0,0);
\#024               std::wstring strTest(_T("TestFileMap"));
\#025               wcscpy(lpMapAddr,strTest.c_str());
\#026
\#027               FlushViewOfFile(lpMapAddr,strTest.length()+1);
\#028         }
\#029  }


