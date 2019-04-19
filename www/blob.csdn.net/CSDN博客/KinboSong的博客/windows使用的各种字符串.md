# windows使用的各种字符串 - KinboSong的博客 - CSDN博客
2017年04月10日 22:16:19[songjinbo3](https://me.csdn.net/KinboSong)阅读数：314
关键字：LPSTR、LPCSTR、LPWSTR、LPCWSTR、LPTSTR、LPCTSTR、CString 、CStringW、char、TCHAR、wchar_t、WCHAR
（1）char和wchar_t是c++的数据类型，wchar_t是用unicdoe进行编码；而vs编译器默认用ansi编码，所以用vs调试存储汉字的wchar_t时会出现乱码
（2）string是STL中的模板类
（3）CString是MFC（Microsoft Fundamental Class）（MFC目录：vs安装目录\VC\atlmfc）中的模板类，不能跨平台，只能在windows下使用
（4）LPSTR、LPCSTR、LPWSTR、LPCWSTR、LPTSTR、LPCTSTR、TCHAR、WCHAR是windows kits（目录为：C:\Program Files (x86)\Windows Kits）中用命令typedef重定义的类名，每一种类型和编码集、char/wchar_t有关，关系如下：
![](https://img-blog.csdn.net/20170410221603225?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（5）string到char *的转换
str.c_str();
（6）string到CString
由于char *可以直接转换到CString,所以先通过c_str()将string转换成char *
CString cstr = str.c_str()
（7）CString、string、char *之间的相互装换
参考：[http://blog.csdn.net/sszgg2006/article/details/8667945](http://blog.csdn.net/sszgg2006/article/details/8667945)
[http://blog.csdn.net/flydream0/article/details/8543525/](http://blog.csdn.net/flydream0/article/details/8543525/) （unicode字符集下CString对char *的转换）
（8）L“<字符串>” 
“<字符串>”是const char *型的，L“<字符串>”是将const char *型转化为const wchar_t *型（unicode编码）
在ansi编码下，_T("<字符串>")  = “<字符串>”
在unicode编码下，_T("<字符串>")  = L“<字符串>”
注意："L"后面只能加字符串常量，不能加变量。
（9）char *和LPWSTR之间的转换
[http://blog.csdn.net/lanshuihaiyu/article/details/8537192](http://blog.csdn.net/lanshuihaiyu/article/details/8537192)
