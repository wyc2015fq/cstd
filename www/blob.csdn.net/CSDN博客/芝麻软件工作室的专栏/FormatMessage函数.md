
# FormatMessage函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:23:33[seven-soft](https://me.csdn.net/softn)阅读数：190标签：[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[list																](https://so.csdn.net/so/search/s.do?q=list&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[buffer																](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[工作																](https://so.csdn.net/so/search/s.do?q=工作&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=system&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=list&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=api&t=blog)
**在开发软件的过程里，经常要做的工作就是调试程序，许多问题的出现，不但是逻辑的问题，还有可能是对API的不熟悉，或者某种条件下调用API会出错的。那么这些出错的原因是什么呢？通常只获取到错误码，也就是通过函数GetLastError得到。当然可以根据这个错误码去查找MSDN就可以知道出错的原因，但有时在客户那里并没有MSDN，那么就需要把调用API函数出错的信息显示出来，或者写到LOG里去。这时就需要调用函数****FormatMessage****把出错码详细原因显示出来。**
函数FormatMessage声明如下：
WINBASEAPI
DWORD
WINAPI
FormatMessageA(
DWORD dwFlags,
LPCVOID lpSource,
DWORD dwMessageId,
DWORD dwLanguageId,
LPSTR lpBuffer,
DWORD nSize,
va_list *Arguments
);
WINBASEAPI
DWORD
WINAPI
FormatMessageW(
DWORD dwFlags,
LPCVOID lpSource,
DWORD dwMessageId,
DWORD dwLanguageId,
LPWSTR lpBuffer,
DWORD nSize,
va_list *Arguments
);
\#ifdef UNICODE
\#define FormatMessage FormatMessageW
\#else
\#define FormatMessage FormatMessageA
\#endif // !UNICODE
调用函数的例子如下：
\#001 //系统错误信息提示。
\#002  //
\#003  void TestErrorInfo(void)
\#004  {
\#005         //进行出错。
\#006         if (!CreateDirectory(_T("c://"),0))
\#007         {
\#008               TCHAR szBuf[128];
\#009               LPVOID lpMsgBuf;
\#010               DWORD dw = GetLastError();
\#011
**\#012              FormatMessage(**
**\#013                   FORMAT_MESSAGE_ALLOCATE_BUFFER |**
**\#014                   FORMAT_MESSAGE_FROM_SYSTEM,**
**\#015                   NULL,**
**\#016                   dw,**
**\#017                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),**
**\#018                   (LPTSTR) &lpMsgBuf,**
**\#019                   0, NULL );**
\#020
\#021                wsprintf(szBuf,
\#022                    _T("%s出错信息(出错码＝%d): %s"),
\#023                    _T("CreateDirectory"), dw, lpMsgBuf);
\#024
\#025               LocalFree(lpMsgBuf);
\#026
\#027               //输出提示。
\#028               OutputDebugString(szBuf);
\#029         }
\#030
\#031  }
调用后输出下面的提示信息：
**CreateDirectory****出错信息 (出错码＝5): 拒绝访问。**


