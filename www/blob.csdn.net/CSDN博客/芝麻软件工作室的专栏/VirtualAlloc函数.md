
# VirtualAlloc函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:26:31[seven-soft](https://me.csdn.net/softn)阅读数：197标签：[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=windows&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=windows&t=blog)


上一次学习了全局内存的分配，在Windows里内存管理是分为两部份，全局内存是系统管理的内存，因而所有进程都可以访问的内存，而每一个进程又有自己的内存空间，这就是虚拟内存空间了，而虚拟内存的空间比较大，当物理内存不足时，系统会把虚拟内存的数据保存到硬盘里，这样只要硬盘的空间足够大，每个进程就可以使用3G的内存。虚拟内存分配可以作为程序里分配内存的主要方式，比如大量的数据缓冲区，动态分配内存的空间。使用VirtualAlloc函数来分配内存的速度要比全局内存要快。
函数VirtualAlloc声明如下：
WINBASEAPI
__bcount(dwSize)
LPVOID
WINAPI
VirtualAlloc(
__in_opt LPVOID lpAddress,
__in     SIZE_T dwSize,
__in     DWORD flAllocationType,
__in     DWORD flProtect
);
**lpAddress**是指定内存开始的地址。
**dwSize**是分配内存的大小。
**flAllocationType**是分配内存的类型。
**flProtect**是访问这块分配内存的权限。
调用函数的例子如下：
\#001 //
\#002  //分配虚拟内存的分配。
\#003  //
\#004  void MemVirtual(void)
\#005  {
\#006         //
\#007         //分配新内存大小。
\#008         UINT nNewSize = (UINT) ceil(1500 / 1024.0) * 1024;
**\#009        PBYTE pNewBuffer = (PBYTE) VirtualAlloc(NULL,nNewSize,MEM_COMMIT,PAGE_READWRITE);**
\#010         if (pNewBuffer)
\#011         {
\#012               //测试虚拟内存。
\#013               ZeroMemory(pNewBuffer,1500);
\#014               memcpy(pNewBuffer,_T("分配虚拟内存成功/r/n"),
\#015                    sizeof(_T("分配虚拟内存成功/r/n")));
\#016               OutputDebugString((LPWSTR)pNewBuffer);
\#017
\#018               //删除分配的内存。
\#019               VirtualFree(pNewBuffer,0,MEM_RELEASE);
\#020         }
\#021
\#022  }


