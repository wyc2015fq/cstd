# g++多文件编译和简单MakeFile文件写法 - 建建的博客 - CSDN博客
2017年03月13日 14:21:16[纪建](https://me.csdn.net/u013898698)阅读数：114
个人分类：[Linux](https://blog.csdn.net/u013898698/article/category/6754383)
上文([g++](http://blog.csdn.net/richerg85/article/details/17073805)[基本用法](http://blog.csdn.net/richerg85/article/details/17073805))介绍简单的g++编译器的用法，只是针对没有依赖关系的单个文件的操作，当我们有多个文件需要编译的时候，是如何工作的呢？下面以简单的实例进行介绍，然后把实例以MakeFile文件实现，并对MakeFile文件进行简单介绍。
准备工作，下面是需要的简单实例文件及代码:
main.cxx  
Administrator@72cec870236147e /home/liujl/mytest$ cat main.cxx#include <iostream>#include "printf1.hxx"#include "printf2.hxx"Administrator@72cec870236147e /home/liujl/mytest$ g++ -c main.cxxAdministrator@72cec870236147e /home/liujl/mytest$ g++ -c  printf1.cxxAdministrator@72cec870236147e /home/liujl/mytest$ g++ -c printf2.cxx
int main(){ printf1(); printf2();}printf1.hxxAdministrator@72cec870236147e /home/liujl/mytest$ cat printf1.hxx#ifndef _PRINTF_1_H_#define _PRINTF_1_H_void printf1();#endifprintf1.cxxAdministrator@72cec870236147e /home/liujl/mytest$ cat printf1.cxx#include "printf1.hxx"#include <iostream>using namespace std;void printf1(){        cout<<"printf1"<<endl;}printf2.hxxAdministrator@72cec870236147e /home/liujl/mytest$ cat printf2.hxx#ifndef _PRINTF_2_H_#define _PRINTF_2_H_void printf2();#endifprintf2.cxxAdministrator@72cec870236147e /home/liujl/mytest$ cat printf2.cxx#include "printf2.hxx"#include <iostream>using namespace std;void printf2(){        cout<<"printf2"<<endl;}
共计5个文件，3个cxx文件，2个hxx头文件
1、手动多文件编译
①先分别直接汇编（编译）为.o文件
Administrator@72cec870236147e /home/liujl/mytest$ g++ -c main.cxxAdministrator@72cec870236147e /home/liujl/mytest$ g++ -c printf1.cxxAdministrator@72cec870236147e /home/liujl/mytest$ g++ -c printf2.cxx
②链接阶段
如果直接执行
Administrator@72cec870236147e /home/liujl/mytest$ g++ main.cxx -o main/tmp/cc9LFDvP.o:main.cxx:(.text+0xc): undefined reference to `printf1()'/tmp/cc9LFDvP.o:main.cxx:(.text+0x11): undefined reference to `printf2()'collect2: ld 返回 1
出现上边错误，原因是编译器找不到printf1()和printf2()的定义。
所以需要将3个obj文件链接到一个文件上：
Administrator@72cec870236147e /home/liujl/mytest$ g++ main.cxx printf1.cxx printf2.cxx -o mainAdministrator@72cec870236147e /home/liujl/mytest$ ./mainprintf1printf2
并输出结果。
这样就能解决多文件编译问题，但是一般情况下，一个项目下的文件比较多，如果这样输入，比较费劲，所以就需要把编译过程写进一个MakeFile文件中
Administrator@72cec870236147e /home/liujl/mytest$ cat makefilecc=g++exe=mainobj=main.o printf1.o printf2.o$(exe):$(obj)        $(cc) -o $(exe) $(obj)main.o:main.cxx        $(cc) -c main.cxxprintf1.o:printf1.cxx        $(cc) -c printf1.cxxprintf2.o:printf2.cxx        $(cc) -c printf2.cxxclean:        rm -rf *.o main
其中
cc=g++exe=mainobj=main.o printf1.o printf2.o
为变量的定义，$(...)作为引用，可以分析一下，是不是和上文中单个操作效果一样？
执行过程：
Administrator@72cec870236147e /home/liujl/mytest$ makeg++ -c main.cxxg++ -c printf1.cxxg++ -c printf2.cxxg++ -o main main.o printf1.o printf2.o
