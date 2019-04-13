
# GetDiskFreeSpaceEx函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:29:52[seven-soft](https://me.csdn.net/softn)阅读数：709标签：[integer																](https://so.csdn.net/so/search/s.do?q=integer&t=blog)[磁盘																](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)[电话																](https://so.csdn.net/so/search/s.do?q=电话&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=电话&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=integer&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=integer&t=blog)

有一次客户给我打来了投诉电话，说我的软件太不好用了，导致他们丢失了很多数据。后来我仔细地查看LOG，分析出来的原因，其实是很简单的，就是磁盘的空间不够了。我给客户说他们的电脑磁盘空间不够了导致出错的问题，但客户反问我为什么不提示磁盘空间不足。是啊，为什么不提示磁盘的空间不足呢？为了解决这个需求，就需要使用到这个函数GetDiskFreeSpaceEx。
函数GetDiskFreeSpaceEx声明如下：
WINBASEAPI
BOOL
WINAPI
GetDiskFreeSpaceExA(
__in_opt LPCSTR lpDirectoryName,
__out_opt PULARGE_INTEGER lpFreeBytesAvailableToCaller,
__out_opt PULARGE_INTEGER lpTotalNumberOfBytes,
__out_opt PULARGE_INTEGER lpTotalNumberOfFreeBytes
);
WINBASEAPI
BOOL
WINAPI
GetDiskFreeSpaceExW(
__in_opt LPCWSTR lpDirectoryName,
__out_opt PULARGE_INTEGER lpFreeBytesAvailableToCaller,
__out_opt PULARGE_INTEGER lpTotalNumberOfBytes,
__out_opt PULARGE_INTEGER lpTotalNumberOfFreeBytes
);
\#ifdef UNICODE
\#define GetDiskFreeSpaceEx GetDiskFreeSpaceExW
\#else
\#define GetDiskFreeSpaceEx GetDiskFreeSpaceExA
\#endif // !UNICODE
lpDirectoryName是驱动器的名称。
lpFreeBytesAvailableToCaller是用户可用的磁盘空间。
lpTotalNumberOfBytes是磁盘总共的空间。
lpTotalNumberOfFreeBytes是磁盘空闲的空间。以上都是字节为单位。
调用函数的例子如下：
\#001  //获取磁盘剩余空间。
\#002  //
\#003  void DiskFree(void)
\#004  {
\#005         //
\#006         ULARGE_INTEGER nFreeBytesAvailable;
\#007         ULARGE_INTEGER nTotalNumberOfBytes;
\#008         ULARGE_INTEGER nTotalNumberOfFreeBytes;
\#009         //
\#010         if (GetDiskFreeSpaceEx(_T("C:"),
\#011               &nFreeBytesAvailable,
\#012               &nTotalNumberOfBytes,
\#013               &nTotalNumberOfFreeBytes))
\#014         {
\#015               TCHAR chBuf[256];
\#016               wsprintf(chBuf,_T("Av=%I64d,Total=%I64d,Free=%I64d/r/n"),
\#017                    nFreeBytesAvailable,
\#018                    nTotalNumberOfBytes,
\#019                    nTotalNumberOfFreeBytes);
\#020               OutputDebugString(chBuf);
\#021         }
\#022  }


