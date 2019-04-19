# WinCE下的帮助文档 - xqhrs232的专栏 - CSDN博客
2013年11月21日 16:43:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：930
原文地址::[http://blog.csdn.net/xjcwzp/article/details/6582132](http://blog.csdn.net/xjcwzp/article/details/6582132)
Windows CE 帮助系统由标准的 HTML (.htm) 文件和图形组成。HTML 帮助 (.chm) 文件不能在 Windows CE 上运行
Htm文件可参照WinCE系统自带的帮助文件
打开方式：
PROCESS_INFORMATION p;
CreateProcess(_T("peghelp.exe"),
           _T("\\Windows\\ctpnl.htm"),
           NULL,
           NULL,
           FALSE,
           0,
           NULL,
           NULL,
           NULL,
           &p);
CloseHandle(p.hProcess);
CloseHandle(p.hThread);
