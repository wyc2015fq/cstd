
# HeapAlloc函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:25:48[seven-soft](https://me.csdn.net/softn)阅读数：189标签：[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=测试&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



前面已经介绍两个分配内存的函数，一个全局的内存分配，一个是私有的内存分配。在进程私有的内存里分配里，又有两种分配情况，一种上基于栈式的内存分配，另一种是基于堆内存的分配。在c++里使用堆内存分配是使用HeapAlloc函数来实现的，也就是实现new操作符分配内存时会调这个函数。
函数HeapAlloc声明如下：
WINBASEAPI
__bcount(dwBytes)
LPVOID
WINAPI
HeapAlloc(
__in HANDLE hHeap,
__in DWORD dwFlags,
__in SIZE_T dwBytes
);
hHeap是进程堆内存开始位置。
dwFlags是分配堆内存的标志。
dwBytes是分配堆内存的大小。
调用函数的例子如下：
\#001  //
\#002  //分配堆内存。
\#003  //
\#004  void MemHeap(void)
\#005  {
\#006         //
\#007         const int nHeapSize = 1024;
\#008         PBYTE pNewHeap = (PBYTE) ::HeapAlloc(GetProcessHeap(), 0, nHeapSize);
\#009
\#010         if (pNewHeap)
\#011         {
\#012               //测试分配堆内存。
\#013               ZeroMemory(pNewHeap,nHeapSize);
\#014               memcpy(pNewHeap,_T("分配堆内存成功/r/n"),
\#015                    sizeof(_T("分配堆内存成功/r/n")));
\#016               OutputDebugString((LPWSTR)pNewHeap);
\#017
\#018               //释放内存
\#019               BOOL bRes = ::HeapFree(GetProcessHeap(), 0, pNewHeap);
\#020               if (bRes != TRUE)
\#021               {
\#022                    OutputDebugString(_T("释放内存出错/r/n"));
\#023               }
\#024         }
\#025
\#026  }


