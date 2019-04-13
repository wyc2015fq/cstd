
# SystemParametersInfo函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 13:28:16[seven-soft](https://me.csdn.net/softn)阅读数：226标签：[工作																](https://so.csdn.net/so/search/s.do?q=工作&t=blog)[path																](https://so.csdn.net/so/search/s.do?q=path&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[旅游																](https://so.csdn.net/so/search/s.do?q=旅游&t=blog)[照片																](https://so.csdn.net/so/search/s.do?q=照片&t=blog)[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=照片&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=旅游&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=旅游&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=path&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=工作&t=blog)
面对华丽的Windows桌面，工作的心情或许好很多，但是久了总会失去兴趣，总想定期地更新桌面的图片。软件开发人员又面对这样的需求了，需要怎么样去做呢？努力去找API函数吧。到目前为止，还有很多变桌面图片的软件，并且还能很挣钱的。其实设置桌面图片的需求，在目前数码相片处理软件也有现实的需求，比如当你去旅游回来后，想把照片当作桌面图片，就可以在处理图片时就设置为桌面图片。这样就需要使用函数SystemParametersInfo来完成这项工作了，当然这个函数还有很多其它功能，比如获取桌面工作区的大小。
函数SystemParametersInfo声明如下：
WINUSERAPI
BOOL
WINAPI
SystemParametersInfoA(
__in UINT uiAction,
__in UINT uiParam,
__inout_opt PVOID pvParam,
__in UINT fWinIni);
WINUSERAPI
BOOL
WINAPI
SystemParametersInfoW(
__in UINT uiAction,
__in UINT uiParam,
__inout_opt PVOID pvParam,
__in UINT fWinIni);
\#ifdef UNICODE
\#define SystemParametersInfo SystemParametersInfoW
\#else
\#define SystemParametersInfo SystemParametersInfoA
\#endif // !UNICODE
**uiAction**是作不同的操作参数。
**uiParam**是设置的参数。
**pvParam**是设置或返回的参数。
**fWinIni**是设置的参数。
调用函数的例子如下：
\#001 //
\#002  //获取系统配置信息。
\#003  //
\#004  void GetSystemParam(void)
\#005  {
\#006         //获取桌面墙纸的路径。
\#007         //SPI_GETDESKWALLPAPER
\#008         TCHAR chPath[MAX_PATH];
**\#009        if (SystemParametersInfo(SPI_GETDESKWALLPAPER,MAX_PATH,chPath,0))**
\#010         {
\#011               //
\#012               OutputDebugString(chPath);
\#013               OutputDebugString(_T("/r/n"));
\#014         }
\#015
\#016         //获取工作区的大小。
\#017         //SPI_GETWORKAREA
\#018         RECT rcWorkArea;
**\#019        if (SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0))**
\#020         {
\#021               //
\#022               const int nBufSize = 256;
\#023               TCHAR chBuf[nBufSize];
\#024
\#025               wsprintf(chBuf,_T("%u,%u,%u,%u"),rcWorkArea.left,rcWorkArea.top,
\#026                    rcWorkArea.right,rcWorkArea.bottom);
\#027
\#028               OutputDebugString(chBuf);
\#029               OutputDebugString(_T("/r/n"));
\#030         }
\#031
\#032  }


