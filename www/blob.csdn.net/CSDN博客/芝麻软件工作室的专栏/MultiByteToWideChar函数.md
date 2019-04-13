
# MultiByteToWideChar函数 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月24日 08:37:52[seven-soft](https://me.csdn.net/softn)阅读数：698


在软件的使用过程中，经常碰到乱码的情况，那么乱码的原因是什么呢？其实很多情况下，就是字符集的不同，也就是说同一个数字代表不同意思。在Windows里，目前主要有ANSI和UNICODE的方式。如果在UNICODE的方式里直接显示ANSI的字符串是出现乱码的，同样在ANSI函数里也不能显示UNICODE的字符串，而是要进行相互转换才能显示正确的字符串。在NT以后的操作系统里，Windows底层函数已经全部改为UNICODE的方式，如果还是使用ANSI的话，要比UNICODE函数慢一些，系统底层会从ANSI的方式转换为UNICODE方式，然后再显示出来。从ANSI转换为UNICODE的字符串，就可以使用函数MultiByteToWideChar来实现。
函数MultiByteToWideChar声明如下：
WINBASEAPI
int
WINAPI
MultiByteToWideChar(
__in UINT     CodePage,
__in DWORD    dwFlags,
__in LPCSTR   lpMultiByteStr,
__in int      cbMultiByte,
__out_ecount_opt(cchWideChar) LPWSTR lpWideCharStr,
__in int      cchWideChar);
**CodePage**是代码表。
**dwFlags**是转换标志。
**lpMultiByteStr**是输入ANSI字符串。
**cbMultiByte**是输入ANSI的字符串长度。
**lpWideCharStr**是输出UNICODE字符串。
**cchWideChar**是输出UNICODE字符串的缓冲区大小。
调用函数的例子如下：
\#001 //
\#002  //字符串转换。
\#003  //
\#004  void MultiToWide(void)
\#005  {
\#006         //ANSI的字符串。
\#007         std::string strANSI("测试字符串转换/r/n");
\#008
\#009         //
\#010         const int nBufSize = 512;
\#011         TCHAR chBuf[nBufSize];
\#012         ZeroMemory(chBuf,nBufSize);
\#013
\#014         //转换为UNICODE的字符串。
**\#015        if (MultiByteToWideChar(CP_ACP,0,strANSI.c_str(),strANSI.length(),**
**\#016              chBuf,nBufSize) > 0)**
\#017         {
\#018               //
\#019               OutputDebugString(chBuf);
\#020         }
\#021
\#022  }

