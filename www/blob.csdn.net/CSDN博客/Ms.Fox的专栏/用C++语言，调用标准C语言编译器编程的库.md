# 用C++语言，调用标准C语言编译器编程的库 - Ms.Fox的专栏 - CSDN博客
2014年10月17日 14:53:51[lulu-lu](https://me.csdn.net/smbluesky)阅读数：584
当我们调用的函数实现是*.c，而不是*.cpp时，如果不注意，在Visual C++编译器中编译，会报错。
error LNK2019，无法解析的外部符号“**************”（？*******）该符号在函数***中被引用。
**这是在C++中调用C的代码，我们必须对C的代码进行处理，才能给顺利通过。**
有三种方法：
1、修改C代码的头文件，假设其头文件为Max.h，其调用的函数为int Max（int  nA，int nB）;
**C代码的头文件**
   #ifndef_MAX_H_
    #define_MAX_H_
    extern "C" int Max（int  nA，int nB）;
    #endif
2、修改C代码的头文件，并在C++代码中，重新声明一下C函数，在重新声明时添加extern ”C“，
**C代码的头文件**
    #ifndef_MAX_H_
    #define_MAX_H_
    extern int Max（int  nA，int nB）;
    #endif
**C++调用文件中**
extern "C" int Max（int  nA，int nB）;
3、修改C代码的头文件，并在包含C头文件时，添加extern "C".
**C代码的头文件**
#ifndef_MAX_H_
    #define_MAX_H_
    extern int Max（int  nA，int nB）;
    #endif
**C++调用文件中**
extern "C" { #include "Max.h"}
原因：
C和C++语言具有不同的编译和链接方式。
C语言编译器编译函数时不带函数的类型和作用域信息，只包含函数符号名字；
而C++编译器为了实现函数的重载，在编译时会带上函数的类型和作用域信息。
比如 int Func（int A）；
C语言编译器把函数编译成类似_Func的符号，C连接器只要找到这个符号就可以连接成功，实现调用。
而在强调安全的C++语言中，编译器会调查参数类型和作用域信息，上述函数原型会编译成_Z_Func_int符号。
在连接过程中，C++连接器会在函数原型所在模块生成的目标文件中查找_Z_Func_int。
而添加extern ”C“ 的作用是告诉C++编译器查找调用连接符号时采用C语言的方式，让编译器寻找_Func，而不是_Z_Func_int。


