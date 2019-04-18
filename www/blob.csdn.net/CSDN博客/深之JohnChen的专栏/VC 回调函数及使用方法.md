# VC 回调函数及使用方法 - 深之JohnChen的专栏 - CSDN博客

2010年11月11日 21:03:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2863


                                            VC 回调函数及使用方法 

回调函数说白了就是事件响应程序，Windows的每个消息可以理解为一个事件，事件的响应代码要由用户自己来定义。用户定义了事件响应的代码，但还要Windows知道这段代码的位置（要不然Windows就不知道如何去调用，这也没有用），于是用户需要将回调函数的指针告诉Windows，最典型的例子是在窗口类的结构(WNDCLASS)中给lpfnWndProc分量赋回调函数指针值。
回调函数的参数格式是由回调函数的调用者（一般是Windows）来定义的，而回调函数的实现者必须遵循这种格式。Windows程序是以事件驱动模型为基础的，这就必然要用到回调函数这种机制。
要透彻了解回调函数，多看看SDK Samples。而MFC中的消息映射机制已经将窗口消息响应的回调函数隐藏起来了，这也符合C++的编程思想，回调函数终究是一种全局函数，它不能在类中实现，而消息映射机制的目的是使消息响应的代码最终封装在窗口类（CWnd类的子类）中。

如果有时间，不妨看看MESSAGE_MAP宏，消息映射是回调函数，只是这种回调函数的用法不同而已。普通的回调函数是要你提供地址，传进某个函数，由它去调用；而消息映射函数，却是由你定义函数，由MESSAGE_MAP宏去取得地址，并实现它的调用。

回调函数是一个程序员不能显式调用的函数；通过将回调函数的地址传给调用者从而实现调用。要实现回调，必须首先定义函数指针。尽管定义的语法有点不可思议，但如果你熟悉函数声明的一般方法，便会发现函数指针的声明与函数声明非常类似。

**CODE:**

#include "stdafx.h"
#include "stdio.h"

void (*fun1)();
void A()
{
 printf("fun1/n");
}

void caller(void(*fun1)())
{
 printf("fun1 start/n");
 fun1();
 printf("fun1 end/n");
}

bool func2(int * i)
{
 printf("From func2() = %d, Hello World!/n", (*i)++);
 return true;
}

void caller2(bool func2(int *),int *j)
{
 func2(j);
}

int main(int argc, char* argv[])
{
 printf("From main(), Hello World!/n");
 printf("/n");

 caller(A);

 int i = 0; 
 for (int j = 0; j < 10; j++)
 {
  caller2(func2, &i); //* 传递函数地址到调用者 */
 }

 getchar();

 return 0;
}

