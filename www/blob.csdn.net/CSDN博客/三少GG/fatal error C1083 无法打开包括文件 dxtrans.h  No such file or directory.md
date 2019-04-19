# fatal error C1083: 无法打开包括文件:"dxtrans.h": No such file or directory - 三少GG - CSDN博客
2012年02月23日 17:18:57[三少GG](https://me.csdn.net/scut1135)阅读数：4635标签：[file																[c																[include																[interface																[windows																[mobile](https://so.csdn.net/so/search/s.do?q=mobile&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
 fatal error C1083: 无法打开包括文件:"dxtrans.h": No such file or directory
[http://blog.waterlin.org/articles/qedit-problem-in-windows-directshow.html](http://blog.waterlin.org/articles/qedit-problem-in-windows-directshow.html)
在使用有关 DirectShow 东西的时候，使用了头文件
#include<qedit.h>
结果，编译的时候提示如下错误：
错误    1       fatal error C1083: 无法打开包括文件:"dxtrans.h": No such file or directory    c:\program files\microsoft sdks\windows\v6.0a\include\qedit.h   498
这个真是奇怪了，为啥微软自己 SDK 里的东西，都会出现找不到头文件的问题呢？
解决办法可以有两种：
(1) 从其它地方把 dxtrans.h 这个文件拷过来，例如从
Program Files\Windows Mobile 5.0 SDK R2\PocketPC\Include\Armv4i\dxtrans.h
Program Files\Windows Mobile 5.0 SDK R2\Smartphone\Include\Armv4i\dxtrans.h
D:\Program Files\Windows Mobile 6 SDK\PocketPC\Include\Armv4i
D:\Program Files\Windows Mobile 6 SDK\Smartphone\Include\Armv4i
里拷 dxtrans.h 这个文件出来。
(2) 或者**在引用 qedit.h 头文件的时候，加上这么几句：**
#pragma include_alias( "dxtrans.h", "qedit.h" )#define__IDxtCompositor_INTERFACE_DEFINED__#define__IDxtAlphaSetter_INTERFACE_DEFINED__#define__IDxtJpeg_INTERFACE_DEFINED__#define__IDxtKey_INTERFACE_DEFINED__#include"Qedit.h"
也可以。
微软的[MSDN上还有对这个问题的讨论](http://social.msdn.microsoft.com/forums/en-US/windowssdk/thread/ed097d2c-3d68-4f48-8448-277eaaf68252/)，看来真是微软自摆乌龙了。
