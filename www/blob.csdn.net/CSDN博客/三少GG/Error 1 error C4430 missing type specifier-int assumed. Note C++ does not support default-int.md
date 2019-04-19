# Error 1 error C4430: missing type specifier - int assumed. Note: C++ does not support default-int - 三少GG - CSDN博客
2012年10月05日 16:58:52[三少GG](https://me.csdn.net/scut1135)阅读数：2042
## [Error 1 error C4430: missing type specifier - int assumed. Note: C++ does not support default-int](http://www.cnblogs.com/masterfy/archive/2010/05/14/1735063.html)
 以下解决办法转自CSDN：
BaseClasses\ctlutil.h(278) : error C4430: missing type specifier - int assumed. Note: C++ does not support default-int
错误发生在：operator=(LONG); 函数定义中，这是因为在VC6中，如果没有显示的指定返回值类型，编译器将其视为默认整型；但是vs2005不支持默认整型. 这个错误，解决方法如下：
打开project->BaseClasses properties->configuration->C/C++ ->Command Line，增加/wd4430选项。
本文来自CSDN博客，[http://blog.csdn.net/CodeJoker/archive/2009/11/05/4771938.aspx](http://blog.csdn.net/CodeJoker/archive/2009/11/05/4771938.aspx)
我的环境：VS2010按以上办法解决后正常。
