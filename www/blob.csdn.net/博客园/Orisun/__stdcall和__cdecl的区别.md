# __stdcall和__cdecl的区别 - Orisun - 博客园







# [__stdcall和__cdecl的区别](https://www.cnblogs.com/zhangchaoyang/articles/2725189.html)





__stdcall和__cdecl是两种函数名字修饰。(注意是连续的两个下划线)

## Windows上

windows上不管是C还是C++，默认使用的都是__stdcall方式。

不论__stdcall还是__cdecl函数参数都是从可向左入栈的，并且由调用者完成入栈操作。对于__stdcall方式被调用者自身在函数返回前清空堆栈；而__cdecl则由调用者维护内存堆栈，所以调用者函数生成的汇编代码比前一种方式长。

由__cdecl约定的函数只能被C/C++调用。

Windows上使用dumpbin工具查看函数名字修饰。

### C语言

__stdcall方式：_FuncName@sizeofParameters

例如：int __stdcall test(int a,double b)编译之后完整的函数名为_test@12

__cdecl方式：_FuncName

例如：int __stdcall test(int a,double b)编译之后完整的函数名为_test

由于C++允许重载函数，所以函数的名字修饰就不能像C这么简单，C++中的函数名字修饰应该包含返回类型，各参数类型等信息，如果是类成员函数，还应该包含类名、访问级别、是否为const函数等等信息。

### C++语言

不管__cdecl，__fastcall还是__stdcall调用方式，函数修饰都是以一个“?”开始，后面紧跟函数的名字，再后面是参数表的开始标识和按照参数类型代号拼出的参数表。对于__stdcall方式，参数表的开始标识是“@@YG”，对于__cdecl方式则是“@@YA”。

X--void    
D--char    
E--unsigned char    
F--short    
H--int    
I--unsigned int    
J--long    
K--unsigned long（DWORD） 
M--float    
N--double    
_N--bool
U--struct
PA--指针

PB--const类型的指针

如果相同类型的指针连续出现，以“0”代替，一个“0”代表一次重复;

U表示结构类型，通常后跟结构体的类型名，用“@@”表示结构类型名的结束;

函数参数表的第一项实际上是表示函数的返回值类型；

参数表后以“@Z”标识整个名字的结束，如果该函数无参数，则以“Z”标识结束。

举例：

int Function1 (char *var1,unsigned long);其函数修饰名为“?Function1@@YGHPADK@Z”
void Function2();其函数修饰名则为“?Function2@@YGXXZ” 

### C++中调用由C编译器生成的函数

extern "C" {

 long func(int a);

char* strcat(char*,const char*);

}

extern "C" {

#include<string.h>

}

要想在编译阶段就知道使用的编译器类型，可以使用：

#ifdef __cplusplus
cout<<"C++";
#else
printf("C");
#endif


通常应该这样声明头文件：

#ifdef _cplusplus
extern "C" {
#endif

long MakeFun(long lFun);

#ifdef _cplusplus
}
#endif


## Linux上

Linux上使用__stdcall和__cdecl的方式比较麻烦一些。

int __attribute__((cdecl)) test();


Linux上使用nm工具查看函数名字修饰。

__stdcall和__cdecl没有区别，有区别的是编程语言。

### C++语言

char test();  -----    _Z4testv            _Z表示C++，4代表函数名有4个字节，test是函数名，v代表参数为空

double func(unsigned int a,double *b,char c);   -----      _Z4funcjPdc     j代表int，Pd代表double型指针，c代表char

### C语言

只是简单一个函数名，没有其他修饰信息。

char test();  -----    test

double func(unsigned int a,double *b,char c);   -----      func



附：

Linux上的反汇编工具：objdump  -x  exefile

查看二进制文件：hexdump -C biFile

编辑二进制文件：hexedit biFile












