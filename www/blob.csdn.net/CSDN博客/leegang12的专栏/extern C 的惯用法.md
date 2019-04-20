# extern "C"的惯用法 - leegang12的专栏 - CSDN博客
2011年05月21日 18:23:00[leegang12](https://me.csdn.net/leegang12)阅读数：511标签：[c																[语言																[编译器																[fortran																[math.h](https://so.csdn.net/so/search/s.do?q=math.h&t=blog)](https://so.csdn.net/so/search/s.do?q=fortran&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[VC++语言](https://blog.csdn.net/leegang12/article/category/824576)
4.extern "C"的惯用法
　　（1）在C++中引用C语言中的函数和变量，在包含C语言头文件（假设为cExample.h）时，需进行下列处理：
extern "C"
{
#include "cExample.h"
}
　　而在C语言的头文件中，对其外部函数只能指定为extern类型，C语言中不支持extern "C"声明，在.c文件中包含了extern "C"时会出现编译语法错误。
　　笔者编写的C++引用C函数例子工程中包含的三个文件的源代码如下：
/* c语言头文件：cExample.h */
#ifndef C_EXAMPLE_H
#define C_EXAMPLE_H
extern int add(int x,int y);
#endif
/* c语言实现文件：cExample.c */
#include "cExample.h"
int add( int x, int y )
{
return x + y;
}
// c++实现文件，调用add：cppFile.cpp
extern "C"  
{
#include "cExample.h"
}
int main(int argc, char* argv[])
{
add(2,3);  
return 0;
}
　　如果C++调用一个C语言编写的.DLL时，当包括.DLL的头文件或声明接口函数时，应加extern "C" {　}。
　　（2）在C中引用C++语言中的函数和变量时，C++的头文件需添加extern "C"，但是在C语言中不能直接引用声明了extern "C"的该头文件，应该仅将C文件中将C++中定义的extern "C"函数声明为extern类型。
　　笔者编写的C引用C++函数例子工程中包含的三个文件的源代码如下：
//C++头文件 cppExample.h
#ifndef CPP_EXAMPLE_H
#define CPP_EXAMPLE_H
extern "C" int add( int x, int y );
#endif
//C++实现文件 cppExample.cpp
#include "cppExample.h"
int add( int x, int y )
{
return x + y;
}
/* C实现文件 cFile.c
/* 这样会编译出错：#include "cExample.h" */
extern int add( int x, int y );
int main( int argc, char* argv[] )
{
add( 2, 3 );  
return 0;
}
链接指示符extern C
如果程序员希望调用其他程序设计语言尤其是C 写的函数那么调用函数时必须
告诉编译器使用不同的要求例如当这样的函数被调用时函数名或参数排列的顺序可能
不同无论是C++函数调用它还是用其他语言写的函数调用它
程序员用链接指示符linkage directive 告诉编译器该函数是用其他的程序设计语言
编写的链接指示符有两种形式既可以是单一语句single statement 形式也可以是复
合语句compound statement 形式
// 单一语句形式的链接指示符
extern "C" void exit(int);
// 复合语句形式的链接指示符
extern "C" {
int printf( const char* ... );
int scanf( const char* ... );
}
// 复合语句形式的链接指示符
extern "C" {
#include <cmath>
}
链接指示符的第一种形式由关键字extern 后跟一个字符串常量以及一个普通的函数
声明构成虽然函数是用另外一种语言编写的但调用它仍然需要类型检查例如编译器
会检查传递给函数exit()的实参的类型是否是int 或者能够隐式地转换成int 型
多个函数声明可以用花括号包含在链接指示符复合语句中这是链接指示符的第二种形
式花招号被用作分割符表示链接指示符应用在哪些声明上在其他意义上该花括号被忽
略所以在花括号中声明的函数名对外是可见的就好像函数是在复合语句外声明的一样
例如在前面的例子中复合语句extern "C"表示函数printf()和scanf()是在C 语言中写的
函数因此这个声明的意义就如同printf()和scanf()是在extern "C"复合语句外面声明的
一样
当复合语句链接指示符的括号中含有#include 时在头文件中的函数声明都被假定是用
链接指示符的程序设计语言所写的在前面的例子中在头文件<cmath>中声明的函数都是C
函数
链接指示符不能出现在函数体中下列代码段将会导致编译错误
int main()
{
// 错误: 链接指示符不能出现在函数内
extern "C" double sqrt( double );
305 第七章函数
double getValue(); //ok
double result = sqrt ( getValue() );
//...
return 0;
}
如果把链接指示符移到函数体外程序编译将无错误
extern "C" double sqrt( double );
int main()
{
double getValue(); //ok
double result = sqrt ( getValue() );
//...
return 0;
}
但是把链接指示符放在头文件中更合适在那里函数声明描述了函数的接口所属
如果我们希望C++函数能够为C 程序所用又该怎么办呢我们也可以使用extern "C"
链接指示符来使C++函数为C 程序可用例如
// 函数calc() 可以被C 程序调用
extern "C" double calc( double dparm ) { /* ... */ }
如果一个函数在同一文件中不只被声明一次则链接指示符可以出现在每个声明中它
也可以只出现在函数的第一次声明中在这种情况下第二个及以后的声明都接受第一个声
明中链接指示符指定的链接规则例如
// ---- myMath.h ----
extern "C" double calc( double );
// ---- myMath.C ----
// 在Math.h 中的calc() 的声明
#include "myMath.h"
// 定义了extern "C" calc() 函数
// calc() 可以从C 程序中被调用
double calc( double dparm ) { // ...
在本节中我们只看到为C 语言提供的链接指示extern "C" extern "C"是惟一被
保证由所有C++实现都支持的每个编译器实现都可以为其环境下常用的语言提供其他链接
指示例如extern "Ada"可以用来声明是用Ada 语言写的函数extern "FORTRAN"用来
声明是用FORTRAN 语言写的函数等等因为其他的链接指示随着具体实现的不同而不同
所以建议读者查看编译器的用户指南以获得其他链接指示符的进一步信息 
