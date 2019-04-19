# qt如何调用.so文件 - xqhrs232的专栏 - CSDN博客
2017年02月24日 14:35:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4333
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://bbs.csdn.net/topics/280013448](http://bbs.csdn.net/topics/280013448)
相关文章
1、新手提问qt调用动态库so文件----[http://bbs.csdn.net/topics/391542908?page=1](http://bbs.csdn.net/topics/391542908?page=1)
在.pro文件里加上：
LIBS += -Lyourlibpath -lyourlibname
比如，假如你的库名为/usr/lib/libtt/libtt.so，则应该加：
LIBS += -L/usr/lib/libtt -ltt
在.cpp文件里加的extern "C",然后编译

//=======================================================================================
备注::  举例如下
1>LIBS          = $(SUBLIBS)  -L/usr/local/Trolltech/Qt-4.7.2/lib -lQtGui -L/usr/local/Trolltech/Qt-4.7.2/lib -L/usr/X11R6/lib -lQtCore -lpthread 
2>LIBS += -L/usr/lib -ltest_xxx
