# Linux下C编程入门 - 工作笔记 - CSDN博客





2012年02月01日 15:50:15[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5564标签：[linux																[编程																[c																[emacs																[编译器																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
个人分类：[基础编程](https://blog.csdn.net/App_12062011/article/category/952065)





                
 这里向大家介绍一下在Linux/UNIX 的机器上，进行 C/C++ 编程的一些入门级知识。


· 所需具备的背景知识

· 开发所需的基本环境

· 获得帮助的途径

· 通过一个实例了解基本步骤



Prerequisite 先决条件：

在Linux上编写 C 程序，至少要熟悉以下两方面的基础知识：


1． C语言的编程基础，至少要知道一些基本的语法，控制流程等编程常识。

对常用的标准 C 函数库有常识性的了解。


2． 对Linux/UNIX 的操作有常识性的了解，掌握常用的shell 命令，如 ls, cat, cp, mkdir …etc.



Environment 所需环境:


1． Linux/ Unix 的操作系统，也可以使用windows下的cygwin。


我们这里讨论的都是通过shell命令行进行操作的。那如果进入了图形界面的Linux 怎么办呢？只要打开一个终端命令，就和命令行环境完全一样了（打开开始菜单可以找到终端命令）。


2． 必备的开发工具：


1) 输入程序需要一个编辑器。常用的有 vi , emacs. 在命令行上输入 vi, emacs, … 就可进入编辑环境

关于 vi 

关于 EMACS 


2) C语言的编译器。常用的是GNU的c语言编译器 gcc(编译 C 程序), g++(编译 C ++程序)。

关于 gcc / g++ 

关于 makefile >>>> 用于简化编译过程

这里有一片入门文章 Linux下C语言编程基础知识，可以先看一下


3) 调试程序的常用工具：gdb.

关于 gdb



Get help获得帮助：


关于 Linux 的文档是非常丰富的。最快捷，方便，全面的资料就在你的机器里，不要浪费。


在命令行上输入shell命令 man 或者 info： 

$man gcc >>>> 这个命令可以获得GNU 的C语言编译器的文档。当然，他们是英文的。

关于 man

关于 info


网络上的资源也很多，多得以至于不知道什么才是自己最需要的。

关于如何获得有价值的信息


看一下 loveunix 上的相关资源 >>>>


无双补充的一些Linux下入门编程资料 >>>>




Basic steps 基本步骤：


1． 输入源代码

2． 编译，链接，运行

3． 调试程序


我们从最基本的 hello world 程序开始，实际操作一下：


1． 输入源代码

引用 


$ emacs hello.c >>>> 进入emacs 编辑器环境


#include >>> 如果你看不懂这个函数，就去好好的看c语言的书

int main()

{

printf(“Hello World.\n”); >>>> Emacs环境下，按下 Tab 键，有自动缩进功能

exit(0);

} 



完成输入后，按住CTRL 键，按下 x, 再按下 c , 最后松开 CTRL。 >>>> 程序保留并退出emacs 环境。 


2． 编译，链接，运行


引用 

$ gcc –o hello hello.c

$ ./hello >>>> ./ 指明了所执行程序的路径

Hello World. 

$ 


一个linux平台上的c程序开发已经完成咯


3． 调试


如果要使用 gdb 调试程序，那么在上一步编译的时候，记得加上 –g 选项



引用 

$ gcc –g –o hello hello.c

$ gdb hello >>>> 进入 gdb 调试环境 




4. CVS版本控制软件 —— 协同工作和保留版本的工具

关于 CVS](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=emacs&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)




