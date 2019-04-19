# 禁止对象拷贝(C++) - walkabc - 博客园
# [禁止对象拷贝(C++)](https://www.cnblogs.com/hitfire/articles/3175822.html)
上一篇文章，说了通过Java的使用方式来使用C++，下面就分析一下如何做。
Java中的对象都是被new出来的，因为Java虚拟机有自动内存回收技术，所以不需要程序员手动delete，在使用C++的时候，我们尽量通过new的方式来创造C++对象，最后通过delete来释放内存。
Java中的=赋值运算符都是把对象的引用传递，而不是C++的拷贝构造函数调用。所以我们要禁止对象拷贝，把=赋值运算符禁止掉。
```
/*
 * NoCopying.h
 *
 *  Created on: 2013-7-7
 *      Author: freshman
 */
#ifndef NOCOPYING_H_
#define NOCOPYING_H_
class NoCopying
{
private:
    NoCopying& operator = (NoCopying&);
    NoCopying( NoCopying&);
};
#endif /* NOCOPYING_H_ */
```
只要继承了这个父类，使用对象拷贝的代码就编译的时候就会报错了。以后在使用=运算符的时候，都是对象指针赋值。如下图：
```
//============================================================================
// Name        : deep.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
using namespace std;
#include "base/NoCopying.h"
int main()
{
    NoCopying *ptr = new NoCopying();
    NoCopying *copyPtr = ptr;
    delete ptr;
    cout<<copyPtr->isDisposed()<<endl;
    return 0;
}
```

