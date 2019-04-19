# warning C4005: 'QT_LARGEFILE_SUPPORT'  解决办法 - baidu_33570760的博客 - CSDN博客
2017年06月05日 12:02:46[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1182
# [warning C4005: 'QT_LARGEFILE_SUPPORT' : macro redefinition](http://www.itnose.net/detail/6639240.html)
2016-09-07 10:24
1>c:\qt\4.8.6_2008\include\qtcore\../../src/corelib/global/qconfig.h(41) : warning C4005: 'QT_LARGEFILE_SUPPORT' : macro redefinition
1> command-line arguments : see previous definition of 'QT_LARGEFILE_SUPPORT'
在VS2008中也出现了，要想办法去掉。
后续：找到qt\src\corelib\global\qconfig.h文件（双击编译错误就可以找到），找到这行：
#define QT_LARGEFILE_SUPPORT 64
替换成：
#ifndef QT_LARGEFILE_SUPPORT
#define QT_LARGEFILE_SUPPORT 64
#endif
