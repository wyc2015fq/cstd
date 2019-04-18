# Linux下C语言程序简单的编写和调试 - nosmatch的专栏 - CSDN博客
2011年07月26日 22:50:53[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：3459
1.建一个目录
2.写代码，建一个hello.c文件
代码：
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- #include "stdio.h" 
- main()  
- {  
- printf("Hello Linux.\n")  
- }  
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- #include "stdio.h"     
- main()    
- {    
- printf("Hello Linux.\n")    
- }  
#include "stdio.h"   main()  {  printf("Hello Linux.\n")  }
记得最后一定要换行
3.编译
gcc -o hello hello.c
使用gcc 编译器就会为我们生成一个hello的可执行文件
4.输出
执行./hello就可以看到程序的输出 
结果了.
是不是很简单？
命令行中 gcc表示我们是用gcc来编译我们的源程序,-o 选项表示我们要求编译 
器给我们输出的可执行文件名为hello 而hello.c是我们的源程序文件. 
gcc编译器有许多选项,一般来说我们只要知道其中的几个就够了. -o选项我们已经知道 
了,表示我们要求输出的可执行文件名. -c选项表示我们只要求编译器输出目标代码,而 
不必要输出可执行文件. -g选项表示我们要求编译器在编译的时候提供我们以后对程序 
进行调试的信息. 
知道了这三个选项,我们就可以编译我们自己所写的简单的源程序了,如果你想要知道更 
多的选项,可以查看gcc的帮助文档,那里有着许多对其它选项的详细说明. 
下面我们来演示怎样用GDB来调试一个求0+1+2+3+…+99的程序：
/* Filename:hello.c */
#include "stdio.h"
main()
{
int i, sum; 
sum = 0;
for (i = 0; i < 100; i++)
{
sum + = i;
}
printf("the sum of 1+2+...+ is %d", sum);
}
执行如下命令编译hello.c（加-g选项产生debug信息）：
#gcc –g –o hello sum.c
在命令行上键入gdb hello并按回车键就可以开始调试sum了，再运行run命令执行sum
#gdb hello
![](http://hi.csdn.net/attachment/201107/22/0_1311328021h6Hz.gif)
list命令用于列出源代码，对上述程序两次运行list，将出现如上画面（源代码被标行号）。
根据列出的源程序，如果我们将断点设置在第5行，只需在gdb 命令行提示符下键入如下命令设置断点：(gdb) break 5
设置断点的另一种语法是 break <function>，它在进入指定函数（function）时停住。
　　相反的，clear用于清除所有的已定义的断点，clear <function>清除设置在函数上的断点， clear <linenum>则清除设置在指定行上的断点。
　　watch命令：
　　watch命令用于观查变量或表达式的值，我们观查sum变量只需要运行watch sum
watch <expr>为表达式（变量）expr设置一个观察点，一量表达式值有变化时，程序会停止执行。
　　要观查当前设置的watch，可以使用info watchpoints命令。
　　next、step命令：
　　　next、step用于单步执行，在执行的过程中，被watch变量的变化情况将实时呈现(分别显示Old value和New value)
next、step命令的区别在于step遇到函数调用，会跳转到到该函数定义的开始行去执行，而next则不进入到函数内部，它把函数调用语句当作一条普通语句执行。
**5.Make**
　　make是所有想在Linux系统上编程的用户必须掌握的工具，对于任何稍具规模的程序，我们都会使用到make，几乎可以说不使用make的程序不具备任何实用价值。
　　在此，我们有必要解释编译和连接的区别。编译器使用源码文件来产生某种形式的目标文件(object files)，在编译过程中，外部的符号参考并没有被解释或替换（即外部全局变量和函数并没有被找到）。因此，在编译阶段所报的错误一般都是语法错误。而连接器则用于连接目标文件和程序包，生成一个可执行程序。在连接阶段，一个目标文件中对别的文件中的符号的参考被解释，如果有符号不能找到，会报告连接错误。
　　编译和连接的一般步骤是：第一阶段把源文件一个一个的编译成目标文件，第二阶段把所有的目标文件加上需要的程序包连接成一个可执行文件。这样的过程很痛苦，我们需要使用大量的gcc命令。
　　而make则使我们从大量源文件的编译和连接工作中解放出来，综合为一步完成。GNU Make的主要工作是读进一个文本文件，称为makefile。这个文件记录了哪些文件（目的文件，目的文件不一定是最后的可执行程序，它可以是任何一种文件）由哪些文件（依靠文件）产生，用什么命令来产生。Make依靠此makefile中的信息检查磁盘上的文件，如果目的文件的创建或修改时间比它的一个依靠文件旧的话，make就执行相应的命令，以便更新目的文件。
　　假设我们写下如下的三个文件，add.h用于声明add函数，add.c提供两个整数相加的函数体，而main.c中调用add函数：
/* filename:add.h */
extern int add(int i, int j);
/* filename:add.c */
int add(int i, int j)
{
return i + j;
}
/* filename:main.c */
#include "add.h"
main()
{
int a, b;
a = 2;
b = 3;
printf("the sum of a+b is %d", add(a + b));
}
　　怎样为上述三个文件产生makefile呢？如下：
test : main.o add.o
gcc main.o add.o -o test
main.o : main.c add.h
gcc -c main.c -o main.o
add.o : add.c add.h
gcc -c add.c -o add.o
　　上述makefile利用add.c和add.h文件执行gcc -c add.c -o add.o命令产生add.o目标代码，利用main.c和add.h文件执行gcc -c main.c -o main.o命令产生main.o目标代码，最后利用main.o和add.o文件（两个模块的目标代码）执行gcc main.o add.o -o test命令产生可执行文件test。
　　我们可在makefile中加入变量，另外。环境变量在make过程中也被解释成make的变量。这些变量是大小写敏感的，一般使用大写字母。Make变量可以做很多事情，例如：
　　i) 存储一个文件名列表； 
　　ii) 存储可执行文件名； 
　　iii) 存储编译器选项。 
　　要定义一个变量，只需要在一行的开始写下这个变量的名字，后面跟一个=号，再跟变量的值。引用变量的方法是写一个$符号，后面跟（变量名）。我们把前面的 makefile 利用变量重写一遍（并假设使用-Wall -O –g编译选项）：
OBJS = main.o add.o 
CC = gcc 
CFLAGS = -Wall -O -g
test : $(OBJS)
$(CC) $(OBJS) -o test
main.o : main.c add.h
$(CC) $(CFLAGS) -c main.c -o main.o 
add.o : add.c add.h 
$(CC) $(CFLAGS) -c add.c -o add.o
　　makefile 中还可定义清除（clean）目标，可用来清除编译过程中产生的中间文件，例如在上述makefile文件中添加下列代码： 
clean: 
rm -f *.o
　　运行make clean时，将执行rm -f *.o命令，删除所有编译过程中产生的中间文件。
　　不管怎么说，自己动手编写makefile仍然是很复杂和烦琐的，而且很容易出错。因此，GNU也为我们提供了Automake和Autoconf来辅助快速自动产生makefile，读者可以参阅相关资料。
**7.小结**
　　本章主要阐述了Linux程序的编写、编译、调试方法及make，实际上就是引导读者学习怎样在Linux下编程，为后续章节做好准备。
      原文地址： [http://blog.csdn.net/21aspnet/article/details/1536929](http://blog.csdn.net/21aspnet/article/details/1536929)
