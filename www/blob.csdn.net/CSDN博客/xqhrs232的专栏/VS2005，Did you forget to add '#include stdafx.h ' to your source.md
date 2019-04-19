# VS2005，Did you forget to add '#include "stdafx.h"' to your source? - xqhrs232的专栏 - CSDN博客
2013年11月30日 12:10:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1611
原文地址::[http://blog.csdn.net/y_yao/article/details/4516745](http://blog.csdn.net/y_yao/article/details/4516745)
unexpected end of file while looking for precompiled header. Did you forget to add '#include "stdafx.h"' to your source?
项目--->>...属性--->>c/c++--->>预编译头--->>不使用预编译头，即可
//=============================================================================================
备注::
1》VS2005下没有基于MFC的静态LIB的模板，只能基于WIN32来创建基于MFC的静态库工程----创建一个空的静态库工程把基于MFC的东西加进去，
加进的东西可以基于MFC来做一些动作
2》创建WIN32工程也可以选择是否增加对MFC的支持！！！
