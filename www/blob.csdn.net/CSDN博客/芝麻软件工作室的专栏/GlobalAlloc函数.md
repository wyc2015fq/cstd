
# GlobalAlloc函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:27:15[seven-soft](https://me.csdn.net/softn)阅读数：163标签：[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[byte																](https://so.csdn.net/so/search/s.do?q=byte&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=byte&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=windows&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=windows&t=blog)


在Windows系统里，有一项功能非常实用，就是剪贴板功能，它能够从一个程序里与另一个程序进行数据交换的功能，也就是说两个进程上是可以共享数据。要实现这样的功能，Windows系统在底层上有相应的支持，就是高端地址的内存是系统内存，这样就可以不同的进程进行共享数据了。因此，调用函数GlobalAlloc来分配系统内存，让不同的进程实现共享数据，也就是剪贴板功能，可以在一个进程内分配内存，在另一个进程里访问数据后删除内存。
函数GlobalAlloc声明如下：
HGLOBAL
WINAPI
GlobalAlloc (
__in UINT uFlags,
__in SIZE_T dwBytes
);
**uFlags**是内存标志。
**dwBytes**是分配内存的大小。
调用函数的例子如下：
\#001 //
\#002  //全局内存的分配。
\#003  //
\#004  void MemGlobal(void)
\#005  {
\#006         //分配全局内存。
**\#007        BYTE* pGlobal = (BYTE*)::GlobalAlloc(GMEM_FIXED,1024);**
\#008
\#009         if (!pGlobal)
\#010         {
\#011               return;
\#012         }
\#013         else
\#014         {
\#015               //测试全局内存。
\#016               ZeroMemory(pGlobal,1024);
\#017               memcpy(pGlobal,_T("分配内存成功/r/n"),
\#018                    sizeof(_T("分配内存成功/r/n")));
\#019               OutputDebugString((LPWSTR)pGlobal);
\#020         }
\#021
\#022         //释放全局内存。
\#023         ::GlobalFree((HGLOBAL)pGlobal);
\#024  }


