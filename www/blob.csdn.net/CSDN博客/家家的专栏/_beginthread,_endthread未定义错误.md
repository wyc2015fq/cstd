# _beginthread,_endthread未定义错误  - 家家的专栏 - CSDN博客





2010年10月09日 08:59:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1615








_beginthread,_endthread未定义错误 

       win32下建立的console工程，默认是单线程的CRT库，没有定义这两个函数，

        解决方案：改用支持多线程的CRT库



转自：[http://www.cppblog.com/true/archive/2008/06/04/52136.aspx](http://www.cppblog.com/true/archive/2008/06/04/52136.aspx)



