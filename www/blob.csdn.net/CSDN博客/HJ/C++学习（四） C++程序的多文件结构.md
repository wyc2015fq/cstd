# C++学习（四） C++程序的多文件结构 - HJ - CSDN博客
2017年11月28日 14:00:43[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：3878
所属专栏：[C++学习](https://blog.csdn.net/column/details/18257.html)
C++语言支持面向对象程序设计，主要体现在类的定义和应用上。一般情况下，一个模块由规范说明和实现两部分组成。规范说明部分描述一个模块与其他模块的接口，而实现部分则是模块的实现细节。模块中的说明部分作为一个单独的文件存放起来，这个文件被称为头文件，其扩展名为“.h”；而实现部分可能由多个扩展名为“.cpp”的文件组成。一般一个较大的程序可以分为三种文件来保存。
（1）类的定义
将不同类的定义分别作为一个头文件来保存（主文件名一般为类名），成员函数一般采用外联定义方式。若是内联函数，则其原型和定义一般归入头文件。
（2）类的实现
不同类的实现部分分别作为一个文件（.cpp文件），用来保存类中成员函数的定义。
（3）类的使用
类的使用放在一个单独的.cpp文件中，该文件使用#include编译预处理命令包含类定义的头文件，在main()函数中使用不同的类。
模块化是信息隐蔽的重要思想，信息隐蔽对开发大的程序非常有用，可以在极大程度上保证程序的质量。类的用户只需了解类的外部接口，而无须了解类的内部实现。类的用户只能应用类的外部接口，不能修改类的内部结构。
```
//tdate.h这个头文件只存放有关Tdate类的定义说明
#ifndef Tdate.H           //用来避免重复定义 
#define Tdate.H          //不是类的一部分 
class Tdate{
public:
    void set(int,int,int);         //成员你函数原型 
    int isLeapYear();
    void print();
private:
    int month;
    int day;
    int year;
};                      //Tdate类定义的结束 
#endif                  //Tdate_H
```
```cpp
//tdate.cpp 类Tdate的实现部分 
#include<iostream>     
#include"tdate.h"      //包含用户自定义的头文件，该文件中提供了Tdate类的定义 
using namespace std
void Tdate::set(int m,int d,int y)
{
    month = m;
    day = d;
    year = y;
} 
int Tdate::isLeapYear()
{
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}
void Tdate::print()
{
    cout<<month<<"/"<<day<<"/"<<year<<endl;
}
```
说明：
头文件tdate.h中前两行的作用是，如果一个程序系统中的多个文件均包含Tdate类，则在编译时可以避免Tdate类中标识符的重复定义。类定义前的这些行可使编译器跳过文件中最后一行之前的所有行。
除了第一次之外，以后编译器每遇到以下的编译预处理命令
```
#include"tdate.h"
```
则以#ifndef（如果没有定义）开始的命令行测试标识符Tdate_H是否已经定义。如果没有定义，则第二行定义标识符Tdate_H，且它的值为NULL，然后编译器处理文件tdate.h中的其余行。如果以后再一次包含了tdate.h文件，则编译器要处理的第一行
```
#ifndef Tdate_H
```
确定了标识符Tdate_H已经定义，则命令行#endif之前的所有行都被跳过，而不进行编译，因此避免了类中标识符的重复定义。名字Tdate_H没有任何特殊的意义，只是在类名的末尾加了_H。
