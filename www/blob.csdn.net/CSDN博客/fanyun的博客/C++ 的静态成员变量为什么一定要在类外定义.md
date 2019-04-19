# C++ 的静态成员变量为什么一定要在类外定义 - fanyun的博客 - CSDN博客
2016年01月28日 14:35:21[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1190
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
       在C++里面，我们在类里声明静态成员变量，那么一定要在类外定义。初始化是赋一个初始值，而定义是分配内存。静态成员变量在类中仅仅是声明，没有定义，所以要在类的外面定义，实际上是给静态成员变量分配内存。
```cpp
//Tstatic.cpp 
#include <stdio.h> 
class A { 
    public: 
        static int a; //声明但未定义
 }; 
int main() { 
    printf("%d", A::a);
    return 0;
}
```
      编译以上代码会出现“对‘A::a’未定义的引用”错误。这是因为静态成员变量a未定义，也就是还没有分配内存，显然是不可以访问的。
     再看如下例子：
```cpp
//Tstatic.cpp 
#include <stdio.h> 
class A { 
    public: 
        static int a; //声明但未定义
 }; 
int A::a = 3; //定义了静态成员变量，同时初始化。也可以写"int A:a;"，即不给初值，同样可以通过编译
int main() { 
    printf("%d", A::a);
    return 0;
}
```
     这样就编译通过了，因为给在类外对静态成员变量a定义赋值，所以就可以访问了。（备注：在类外面  这样子写：int A::a ； 也可以表示声明了，这样就表示我们已经定义了。）
```cpp
#pragma once
typedef struct MyStruct
{
int name;
int year;
}infoList;
class statics
{
public:
statics();
static int m_nums;
static infoList  m_lists;
};
//cpp文件的声明
#include"stdafx.h"
#include"statics.h"
#include<iostream>
int statics::m_nums; //定义 
infoList statics::m_lists; //定义 这样子可以有些成员我们不知道怎么初始化，或者说必须在用的过程才赋值。可以使用这种方法
statics::statics()
{
std::cout<<"static class ";
}
```
