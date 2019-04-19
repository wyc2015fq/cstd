# C/C++可变参数函数 - yixianfeng41的专栏 - CSDN博客
2017年03月10日 21:29:16[第2梦](https://me.csdn.net/yixianfeng41)阅读数：742
所属专栏：[C++编程](https://blog.csdn.net/column/details/14514.html)
## **一、C可变参数函数**
一般我们编程的时候，函数中形式参数的数目通常是确定的，在调用时要依次给出与形式参数对应的实际参数。但在某些情况下我们希望函数的参数个数可以根据需要确定，因此c语言引入可变参数函数。典型的可变参数函数的例子有printf()、scanf()等.
先看代码：
```
printf(“hello,world!”);其参数个数为1个。
printf(“a=%d,b=%ｓ,c=%c”,a,b,c);其参数个数为4个。
```
如何编写可变参数函数呢？我们首先来看看printf函数原型是如何定义的。 
在linux下，输入man 3 printf，可以看到prinf函数原型如下：
```
SYNOPSIS
#include <stdio.h>
int printf(const char *format, ...);
```
后面的三个点…表示printf参数个数是不定的． 
如何实现可变参数函数？
### **1、编写可变函数**
为了编写可变参数函数，我们通常需要用到
```
void va_start(va_list ap, last);
type va_arg(va_list ap, type);
void va_end(va_list ap);
void va_copy(va_list dest, va_list src);
```
其中： 
- va_list是用于存放参数列表的数据结构。 
- va_start函数根据函数参数列表中最后一个非可变参数的参数last来初始化参数列表。 
- va_arg函数用于从参数列表中取出一个参数，参数类型由type指定。 
- va_copy函数用于复制参数列表。 
- va_end函数执行清理参数列表的工作。
上述函数通常用宏来实现，例如标准ANSI形式下，这些宏的定义是：  
```
typedef char * va_list; //字符串指针
#define _INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap) ( ap = (va_list)0 )
```
### **2、实例：**
```
#include <stdio.h>;    
#include <string.h>;    
#include <stdarg.h>;    
/* ANSI标准形式的声明方式，括号内的省略号表示可选参数 */
int argFunc(char *str, ...)
{
    va_list ap;    // 定义参数列表变量
    va_start(ap, str);   //确定参数位置，表明从str开始获取参数
    int count = 0;
    char *para;
    while (1)
    {
        para = va_arg(ap, char *);       //取出当前的参数，类型为char *
        if (strcmp(para, "/0") == 0) break;  //采用空串指示参数输入结束 
        printf("parameter %d is: %s/n", count, para);
        count++;
    }
    va_end(ap);    // 将参数列表 ap 清零
    return 0;
}
int main()
{
    argFunc("DEMO", "This", "is", "a", "demo!", "333333", "/0");
    return 0;
}
```
## **二、C++可变参数函数**
首先，所谓可变参数指的是函数的参数个数可变，参数类型不定的函数。为了编写能处理不同数量实参的函数，C++提供了两种主要的方法：如果所有的实参类型相同，可以传递一个名为initializer_list的标准库类型；如果实参的类型不同，我们可以编写可变参数模板。另外，C++还有一种特殊的省略符形参，可以用它传递可变数量的实参，不过这种一般只用于与C函数交互的接口程序。
### **可变参数函数**
#### **1、initializer_list形参**
如果函数的实参数量未知但是全部实参的类型都相同，我们可以使用initializer_list类型的形参（C++11新标准）。和vector一样，initializer_list也是一种模板类型。下面看看initializer_list提供的一些操作：
```
#include<initializer_list> // 头文件 
initializer_list<T> lst;  // 默认初始化，T类型元素的空列表 
initializer_list<T> lst{a,b,c...}; // 初始化为初始值列表的副本 
lst2(lst)   // 拷贝或赋值不会拷贝列表中的元素；拷贝后， 
lst2 = lst  // 原始列表和副本共享元素 
lst.size()  // 列表中的元素数量 
lst.begin()  // 返回指向lst中首元素的指针 
lst.end()   // 返回指向lst中尾元素下一位置的指针
```
下面给出一个例子，需要注意的是，含有initializer_list形参的函数也可以同时拥有其他形参。另外，如果想给initializer_list形参传递一个实参的序列，必须把序列放在一对花括号内：  
```cpp
string func(initializer_list<string> li) 
{ 
  string str(""); 
  for(auto beg=li.begin(); beg!=li.end(); ++beg) 
    str += *beg; 
  return str; 
} 
int main() 
{ 
  cout << func({"This"," ","is"," ","C++"}) << endl; 
  return 0; 
}
```
#### **2、省略符形参**
函数可以用省略符形参”…“表示不定参数部分，省略符形参只能出现在形参列表的最后一个位置，它的形式如下：  
```
void foo(parm_list, ...); 
// 典型例子 
int printf(const char* format, ...)
```
省略符形参应该仅仅用于C和C++通用的类型，因为大多数类类型的对象在传递给省略符形参时都无法正确拷贝。下面是头文件中的几个宏定义：   
```
#include<cstdarg> // C中是<stdarg.h> 
// va_list是一种数据类型，args用于持有可变参数。 
// 定义typedef char* va_list; 
va_list args; 
// 调用va_start并传入两个参数：第一个参数为va_list类型的变量 
// 第二个参数为"..."前最后一个参数名 
// 将args初始化为指向第一个参数（可变参数列表） 
va_start(args, paramN); 
// 检索参数，va_arg的第一个参数是va_list变量，第二个参数指定返回值的类型 
// 每一次调用va_arg会获取当前的参数，并自动更新指向下一个可变参数。 
va_arg(args,type); 
// 释放va_list变量 
va_end(args);
```
下面给出一个例子：  
```
int add_nums(int count,...) 
{ 
  int result = 0; 
  va_list args; 
  va_start(args, count); 
  for(int i=0; i<count; ++i) 
    result += va_arg(args, int); 
  va_end(args); 
  return result; 
} 
int main() 
{ 
  cout << add_nums(4, 25, 25, 50, 50) << endl; 
  return 0; 
}
```
编译器是将参数压入栈中进行传递的。传递实参的时候，编译器会从实参列表中，按从右到左的顺序将参数入栈，对于add_nums(4, 25, 25, 50, 50)的调用，则入栈的顺序是 50, 50, 25, 25, 4 (注意没有可变参数与不可变参数之分)。由于栈的地址是从高到低的，所以在知道了第一个参数地址和参数的类型之后，就可以获取各个参数的地址。
### **可变参数模板**
一个可变参数模板（variadic template）就是一个接受可变数目参数的模板函数或模板类。可变数目的参数被称为参数包（parameter packet）。存在两种参数包：模板参数包（表示零个或多个模板参数）和函数参数包（表示零个或多个函数参数）。   
上述说到我们可以使用一个initializer_list来定义一个可接受可变数目实参的函数，但是所有实参必须具有相同的类型。当我们既不知道要处理的实参数目也不知道它们的类型时，我们就需要使用可变参数的函数模板了。我们用一个省略号来指出一个模板参数或函数参数表示一个包：在一个模板参数列表中，class…或typename…指出接下来的参数表示零个或多个类型的列表；一个类型名后面跟一个省略号表示零个或多个给定类型的非类型参数的列表。在函数参数列表中，如果一个参数的类型是一个模板参数包，则此参数也是一个函数参数包。  
```cpp
// Args是一个模板参数包；rest是一个函数参数包 
template <typename T, typename...Args> 
void foo(const T &t, const Args&...rest);
```
可变参数函数模板通常是递归的。第一步调用处理包中的第一个实参，然后用剩余的实参调用自身。为了终止递归，我们还需要定义一个非可变参数的函数模板：  
```cpp
// 用来终止递归并处理包中最后一个元素 
template <typename T> 
void print(const T &t) 
{ 
  cout << t; 
} 
// 包中除了最后一个元素之外的其他元素都会调用这个版本的print 
template <typename T, typename...Args> 
void print(const T &t, const Args&...rest) 
{ 
  cout << t << " ";   // 打印第一个实参 
  print(rest...);    // 递归调用，打印其他实参 
} 
// 测试 
int main() 
{ 
  print("string1", 2, 3.14f, "string2", 42); 
  cout << endl; 
  return 0; 
}
```
非可变参数版本的print负责终止递归并打印初始调用中的最后一个实参。对于最后一次递归调用print(42)，两个print版本都是可行的。但是，非可变参数模板比可变参数模板更特例化，因此编译器选择非可变参数版本。
