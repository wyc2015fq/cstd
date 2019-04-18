# 向C语言程序传递命令行参数 - 小灰笔记 - CSDN博客





2017年03月24日 00:56:27[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3352








       虽然很多时候写C语言main函数的时候都选择把形参写为空，也就是void，但是这并不是唯一的写法。C语言实际上支持向主函数传递两个参数，此时的函数原型如下：

       int main(int  argc, char **argv);

       其中，第一个参数用于代表参数的个数；第二个参数则是用于向程序传递参数的列表，通常是一组值。写代码如下：

#include"stdio.h"



int main(int argc,char **argv)

{

       while(*++argv != NULL)

       {

              printf("%s\n",*argv);

       }

       return 0;

}

       通过上面的程序，可以实现输出向主函数传递的参数列表。之所以在判断的时候进行了指针的自加是为了避开第一个参数，参数列表的第一个参数其实是程序的名字。

       程序编译执行如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_09>gcc exp_09.c



E:\WorkSpace\01_编程语言\01_C语言\exp_09>a hello c lang world

hello

c

lang

world

       回过头来再多看一点东西，看一下第一个参数的信息。修改代码直接打印出整个参数列表，代码修改如下：

#include"stdio.h"



int main(int argc,char **argv)

{

       while(*argv != NULL)

       {

              printf("%s\n",*argv);

              argv++;

       }

       return 0;

}



       编译执行如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_09>gcc exp_09.c



E:\WorkSpace\01_编程语言\01_C语言\exp_09>a hello c lang world

a

hello

c

lang

world

       通过以上结果可以看出，本次程序的执行多输出了一个信息，也就是第一行的a。其实，由于采用了gcc默认的编译方式，生成的可执行文件名称就是a。通过程序的行为推测，这个参数的获取应该是通过操作系统接口获知了当前的文件名称。接下来不做编译，直接把a修改一下名字继续以下测试：

E:\WorkSpace\01_编程语言\01_C语言\exp_09>copy a.exe test.exe

已复制         1 个文件。



E:\WorkSpace\01_编程语言\01_C语言\exp_09>test hello c lang world

test

hello

c

lang

world

       通过执行结果可以看出，其实程序名称的获取信息并不是来源于编译阶段而是来自于执行阶段。



