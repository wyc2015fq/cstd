# GCC 介绍+ Ubuntu10.10使用 - 三少GG - CSDN博客
2010年04月27日 20:59:00[三少GG](https://me.csdn.net/scut1135)阅读数：1594标签：[gcc																[ubuntu																[编译器																[汇编																[compiler																[filenames](https://so.csdn.net/so/search/s.do?q=filenames&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
1.
在终端键入**which gcc**就可以知道gcc的位置了 
2.可以执行
**gcc --version**
的命令来查看版本，输出如下：
gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
Copyright (C) 2010 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
3. 查看帮助
**man gcc   向下翻页，退出时按 :q**
**gcc --help**
一般ubuntu自带gcc版本基本够用，不用安装。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## [Ubuntu下gcc安装及使用](http://www.cnblogs.com/zero1665/archive/2009/11/03/1595510.html)
编译则使用gcc命令。要往下学习首先就得熟悉gcc命令的用法。
gcc命令提供了非常多的命令选项，但并不是所有都要熟悉，初学时掌握几个常用的就可以了，到后面再慢慢学习其它选项，免得因选项太多而打击了学习的信心。
一. 常用编译命令选项
假设源程序文件名为test.c。
1. 无选项编译链接
用法：#gcc test.c
作用：将test.c预处理、汇编、编译并链接形成可执行文件。这里未指定输出文件，默认输出为a.out。
2. 选项 -o
用法：#gcc test.c -o test
作用：将test.c预处理、汇编、编译并链接形成可执行文件test。-o选项用来指定输出文件的文件名。
3. 选项 -E
用法：#gcc -E test.c -o test.i
作用：将test.c预处理输出test.i文件。
4. 选项 -S
用法：#gcc -S test.i 
作用：将预处理输出文件test.i汇编成test.s文件。
5. 选项 -c
用法：#gcc -c test.s
作用：将汇编输出文件test.s编译输出test.o文件。
6. 无选项链接
用法：#gcc test.o -o test
作用：将编译输出文件test.o链接成最终可执行文件test。
7. 选项-O
用法：#gcc -O1 test.c -o test
作用：使用编译优化级别1编译程序。级别为1~3，级别越大优化效果越好，但编译时间越长。
二. 多源文件的编译方法
如果有多个源文件，基本上有两种编译方法：
[假设有两个源文件为test.c和testfun.c]
1. 多个文件一起编译
用法：#gcc testfun.c test.c -o test
作用：将testfun.c和test.c分别编译后链接成test可执行文件。
2. 分别编译各个源文件，之后对编译后输出的目标文件链接。
用法：
#gcc -c testfun.c //将testfun.c编译成testfun.o
#gcc -c test.c //将test.c编译成test.o
#gcc -o testfun.o test.o -o test //将testfun.o和test.o链接成test
以上两种方法相比较，第一中方法编译时需要所有文件重新编译，而第二种方法可以只重新编译修改的文件，未修改的文件不用重新编译。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
【介绍】
GCC是一个用于linux系统下编程的编译器。
## linux下的编译器
　　GCC=GNU Compiler Collection，是由Stallman所开发的linux下的编译器，可以编译的语言包括：C, C++,[Objective-C](view/459423.htm),[Fortran](view/36402.htm),[Java](view/29.htm),Ada等。不过，现在GCC也有了许多Win32下的移植版本。这要感谢Internet上众多程序员的共同努力。
　　Win32 下的 GCC 详细可察看词条:[GCC for Win32](view/409262.htm)
### 发展历史
　　　GCC是GNU公社的一个项目。是一个用于编程开发的自由编译器。最初，GCC只是一个C语言编译器，它是GNU C Compiler 的英文缩写。随着众多自由开发者的加入和GCC自身的发展，**如今的GCC已经是一个包含众多语言的编译器了。其中包括 C,C++,Ada,Object C和Java等。所以，GCC也由原来的GNU C Compiler变为GNU Compiler Collection。也就是 GNU编译器家族的意思。当然，如今的GCC借助于它的特性，具有了**[**交叉编译器**](view/1795991.htm)**的功能，即在一个平台下编译另一个平台的代码。**
　　直到现在，GCC的历史仍然在继续，它的传奇仍然被人所传颂。而且更重要的是GCC完全是一款免费的自由软件，加之其强大的功能所以深受广大用户的喜爱。
基本规则
　　　gcc所遵循的部分约定规则：
　　.c为后缀的文件，C语言源代码文件；
　　.a为后缀的文件，是由目标文件构成的档案库文件；
　　.C，.cc或.cxx 为后缀的文件，是C++源代码文件；
　　.h为后缀的文件，是程序所包含的头文件；
　　.i 为后缀的文件，是已经预处理过的C源代码文件；
　　.ii为后缀的文件，是已经预处理过的C++源代码文件；
　　.m为后缀的文件，是Objective-C源代码文件；
　　.o为后缀的文件，是编译后的目标文件；
　　.s为后缀的文件，是汇编语言源代码文件；
　　.S为后缀的文件，是经过预编译的汇编语言源代码文件。
### 执行过程
　　        虽然我们称Gcc是C语言的编译器，但使用gcc由C语言源代码文件生成可执行文件的过程不仅仅是编译的过程，而是要经历四个相互关联的步骤∶[预处理](view/1168039.htm)(也称[预编译](view/176610.htm)，Preprocessing)、[编译](view/69568.htm)(Compilation)、[汇编](view/33704.htm)(Assembly)和[链接](view/147669.htm)(Linking)。
　　命令gcc首先调用cpp进行预处理，在预处理过程中，对源代码文件中的文件包含(include)、预编译语句(如[宏](view/57.htm)定义define等)进行分析。接着调用cc1进行编译，这个阶段根据输入文件生成以.o为后缀的目标文件。汇编过程是针对汇编语言的步骤，调用as进行工作，一般来讲，.S为后缀的汇编语言源代码文件和汇编、.s为后缀的汇编语言文件经过预编译和汇编之后都生成以.o为后缀的目标文件。当**所有的目标文件都生成之后，gcc就调用ld来完成最后的关键性工作，这个阶段就是连接。在连接阶段，所有的目标文件被安排在可执行程序中的恰当的位置，同时，该程序所调用到的库函数也从各自所在的档案库中连到合适的地方。**
### 基本用法
　　　在使用Gcc编译器的时候，我们必须给出一系列必要的调用参数和文件名称。Gcc编译器的调用参数大约有100多个，其中多数参数我们可能根本就用不到，这里只介绍其中最基本、最常用的参数。
Gcc最基本的用法是∶gcc [options] [filenames]
　　其中options就是编译器所需要的参数，filenames给出相关的文件名称。
**-c，只编译，不连接成为可执行文件，编译器只是由输入的.c等源代码文件生成.o为后缀的目标文件，通常用于编译不包含主程序的子程序文件。**
　　-o output_filename，确定输出文件的名称为output_filename，同时这个名称不能和源文件同名。如果不给出这个选项，gcc就给出预设的可执行文件a.out。
　　-g，产生符号调试工具(GNU的gdb)所必要的符号资讯，要想对源代码进行调试，我们就必须加入这个选项。
　　-O，对程序进行优化编译、连接，采用这个选项，整个源代码会在编译、连接过程中进行优化处理，这样产生的可执行文件的执行效率可以提高，但是，编译、连接的速度就相应地要慢一些。
　　-O2，比-O更好的优化编译、连接，当然整个编译、连接过程会更慢。
　　-Idirname，将dirname所指出的目录加入到程序头文件目录列表中，是在预编译过程中使用的参数。C程序中的头文件包含两种情况∶
　　A)#include <myinc.h>
　　B)#include “myinc.h”
　　其中，A类使用尖括号(< >)，B类使用双引号(“ ”)。对于A类，预处理程序cpp在系统预设包含文件目录(如/usr/include)中搜寻相应的文件，而B类，预处理程序在目标文件的文件夹内搜索相应文件。
### GCC执行过程示例
**示例代码**a.c：
　　#include <stdio.h>
　　int main()
　　{
　　printf("hello/n");
　　}
**预编译过程**：
　　这个过程处理宏定义和include，并做语法检查。
　　可以看到预编译后，代码从5行扩展到了910行。
　　gcc -E a.c -o a.i
　　cat a.c | wc -l
　　5
　　cat a.i | wc -l
　　910
**编译过程**：
　　这个阶段，生成汇编代码。
　　gcc -S a.i -o a.s
　　cat a.s | wc -l
　　59
**汇编过程**：
　　这个阶段，生成目标代码。
　　此过程生成ELF格式的目标代码。
　　as a.s -o a.o
　　file a.o
　　a.o: ELF 64-bit LSB relocatable, AMD x86-64, version 1 (SYSV), not stripped
**链接过程**：
　　链接过程。生成可执行代码。链接分为两种，一种是[静态链接](view/945739.htm)，另外一种是[动态链接](view/945740.htm)。使用静态链接的好处是，依赖的动态链接库较少，对动态链接库的版本不会很敏感，具有较好的兼容性；缺点是生成的程序比较大。使用动态链接的好处是，生成的程序比较小，占用较少的内存。
　　gcc a.o -o a
　　程序运行：
　　./a
　　hello
### GCC编译简单例子
　　　编写如下代码：
　　#include <stdio.h>
　　int main()
　　{
　　printf("hello,world!/n");
　　}
　　执行情况如下：
　　wenjun@ubuntu:~/temp/01$ gcc hello.c -o hello
　　wenjun@ubuntu:~/temp/01$ ls
　　hello  hello.c
　　wenjun@ubuntu:~/temp/01$ ./hello
　　hello,world!

