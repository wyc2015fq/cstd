# 花3分钟了解下C/C++中的函数可变参简单实现 - xcw0754 - 博客园
# [花3分钟了解下C/C++中的函数可变参简单实现](https://www.cnblogs.com/xcw0754/p/4985064.html)
**1.可变参函数的原理**
　　C/C++函数的参数是存放在栈区的，并且参数的入栈是从参数的右边开始，即最后一个参数先入栈，而第一个参数最后才入栈，所以，根据栈的后进先出性质，函数总能找到第一个参数。所以，可变参函数的实现必须能够从已知参数中获取到函数所需要参数的个数，否则怎么知道传了几个参数呢。
　　例如printf函数，第一个参数就是一个格式串，而后面所需要的参数个数能够从格式串中推得。
**2.可变参函数的设计**
　　标准头文件<stdarg.h>提供了一套对可变参函数的实现机制，所以编写可变参函数需要包含该头文件。
#include<stdarg.h>
　　C语言的头文件<stdarg.h>提供了一个数据类型va_list和三个宏（va_start、va_arg和va_end），得用它们来实现可变参。va_list是一般是一个char指针(即字符串指针)，用来指向可变参的。
　　来看看这四个玩意一般是怎么定义的。             
```
typedef char *  va_list; //就是个指针呀
```
```
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )  // 这个有什么用你得自己看，我管不住你了。可以不看的。
 
#define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )   // 将ap指向v之后的位置
```
```
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) ) // 取出ap的值，并将ap指向下一个位置。t就是类型，可以是int,double等等
```
```
#define va_end(ap)      ( ap = (va_list)0 )　　// 置空
```
**3.可变长参数函数的实现**
　　先上例子，hello world！
　　实例1:：实现求和函数，参数个数可变
```
1 int sum(int n, ...)
 2 {
 3     va_list arg_ptr;
 4     va_start(arg_ptr, n); 
 5     int nRes = 0;
 6     for(int i=0; i < n; ++i)
 7     {
 8         int temp = va_arg(arg_ptr, int);  // 逐个参数取出来
 9         nRes += temp;　　// 求和
10     }
11     va_end(arg_ptr);  // 养成习惯
12     return nRes;
13 }
```
　　实例2：实现printf的功能
```
1 void myPrintf(const char *strFormat, ...)
 2 {
 3     if(NULL==strFormat)   return;
 4     va_list arg_ptr;
 5     va_start(arg_ptr, strFormat);
 6     char strInfo[1000] = {0};       // 小心别溢出？
 7     vsprintf(strInfo, strFormat, arg_ptr);      // 按格式打印到strInfo，功能类似sprintf
 8     fputs(strInfo, stdout);  // 输出
 9     va_end(arg_ptr);
10 }
```
 完整例子，G++编过测过了。
```
1 #include <iostream>
 2 #include <stdarg.h>
 3 #include <stdio.h>
 4 
 5 int sum(int n, ...)
 6 {
 7     va_list arg_ptr;
 8     int i = 0, nRes = 0;
 9     va_start(arg_ptr, n);
10     for(; i < n; ++i)
11     {
12         int temp = va_arg(arg_ptr, int);
13         nRes += temp;
14     }
15     va_end(arg_ptr);
16     return nRes;
17 }
18 
19 void myPrintf(const char *strFormat, ...)
20 {
21     if(NULL==strFormat)   return;
22     va_list arg_ptr;
23     va_start(arg_ptr, strFormat);
24     char strInfo[1000] = {0};       // 小心别溢出？
25     vsprintf(strInfo, strFormat, arg_ptr);      // 按格式打印到strInfo，功能类似sprintf
26     fputs(strInfo, stdout);
27     va_end(arg_ptr);
28 }
29 
30 int main()
31 {
32     myPrintf("%s %d\n","the result is ", sum(4, 45, 15, 45, 100)); 
33     return 0;
34 }
example
```
(1) va_list的定义即例子见 http://www.cplusplus.com/reference/cstdarg/va_list/ 
(2) 各个平台的实现可能是不一样的，如va_end的实现，故最好先了解你所用的平台。
(3) 实际使用时还有更多的东西要考虑，不仅仅像上面那么简单。

