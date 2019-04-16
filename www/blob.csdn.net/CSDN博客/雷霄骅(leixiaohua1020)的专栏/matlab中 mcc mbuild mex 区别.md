# matlab中 mcc/mbuild/mex 区别 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年08月03日 21:44:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：123
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)










mcc的作用是将.m文件编译为c/c++动态链接库文件，使你可以在c/c++程序中使用matlab的一些函数功能。mcc 也可以将.m文件编译为exe可执行文件。





mex的作用是将c/cpp文件编译为.m 文件可以调用的库文件，在Windows操作系统里通常是以mexw32或mexw64为扩展名，让你可以使用c/c++来扩展matlab，即在matlab中调用C/C++编写函数的功能，达到增加新的功能或者加速原有程序的功能



mbuild的作用是将matlab视为一个c/c++编译器，使用mbuild可以将c/c++编译为exe可执行文件，特色在于可以调用matlab的数学库和图形库，但这样需要使用matlab提供的动态连接库才能运行。



由以上可知，通常所说的matlab与c++混合编程所涉及的是mex和mcc两个命令。



[mcc命令选项](http://www.mathworks.com/help/toolbox/compiler/f0-985134.html)



原文地址：[http://blog.csdn.net/zshtang/article/details/6675530](http://blog.csdn.net/zshtang/article/details/6675530)




