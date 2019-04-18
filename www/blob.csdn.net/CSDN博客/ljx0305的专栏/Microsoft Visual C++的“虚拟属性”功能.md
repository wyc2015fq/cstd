# Microsoft Visual C++的“虚拟属性”功能 - ljx0305的专栏 - CSDN博客
2009年02月06日 13:44:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：575标签：[microsoft																[c++																[structure																[编译器																[struct																[rest](https://so.csdn.net/so/search/s.do?q=rest&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=structure&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)
个人分类：[VC																[C++](https://blog.csdn.net/ljx0305/article/category/380566)](https://blog.csdn.net/ljx0305/article/category/401790)
Microsoft [Visual C++](http://dev.rdxx.com/VC/)的“虚拟属性”功能
**译者注：**
       本文简单介绍了使用Microsoft Visual C++中的__declspec关键字来实现“属性（Property）”这个C++中没有的特性的方法。有关__declspec关键字的更详细的信息，可以参考MSDN。
       __declspec关键字不是标准C++的一部分，因此这种实现“属性”的方法只适用于Visual C++，如果想要了解在标准C++中模拟实现“属性”的方法，请参考：
http://www.csdn.net/develop/read_article.asp?id=18361
**正文：**
       很多遗留下来的传统C++代码中常常会出现用public或protected关键字修饰的成员变量，您可以直接去访问它们（译者注，如果是protected，是指可以在其派生类中直接访问），而不是通过一组简单的get/set方法。举个例子来说，如下的结构定义就是这样的情况：
typedef struct tagMyStruct 
{ 
   long m_lValue1; 
   ...             // Rest of the structure definition. 
} SMyStruct;
在使用这个结构体的客户端程序中就可以看到散布着大量类如下面列出的代码：
SMyStruct       MyStruct; 
long            lTempValue; 
MyStruct.m_lValue1 = 100;       // Or any other value that is to be assigned to it. 
... 
lTempValue = MyStruct.m_lValue1;
    在这种情况下，一旦这段代码需要在一个多线程的环境下应用，你就会遇到一个麻烦。因为没有get/set方法的存在，你不可能简单的在SMyStruct的定义中加上一个临界区（或互斥量）来保护包括m_lValue1在内的所有公有成员变量。
       如果您是使用Microsoft Visual C++编译器，您就可以很方便的找到一个解决这个问题的方案。
       您只需把您的结构体重写为如下的形式：
typedef struct tagMyStruct 
{ 
   __declspec(property(get=GetValue1, put=PutValue1))
   long  m_lValue1; 
   ...                // Rest of the structure definition. 
   long GetValue1() 
   { 
      // Lock critical section 
      return m_lInternalValue1; 
      // Unlock critical section. 
   } 
   void PutValue1(long lValue) 
   { 
      // Lock critical section 
      m_lInternalValue1 = lValue; 
      // Unlock critical section 
   } 
private: 
   long m_lInternalValue1; 
        // Define critical section member variable. 
} SMyStruct;
       这就是您要做的全部！
       在这以后，对于如下的代码：
MyStruct.m_lValue1 = 100
       编译器会自动转换为：
MyStruct.PutValue(100)
对于如下的代码：
lTempValue = MyStruct.m_lValue1
编译器会自动转换为：
lTempValue = MyStruct.GetValuel()
这样的特性能带来很多有用的功能，您甚至可以用它为您原来的结构体或类加上引用计数的功能！
**译者补充：**
对于类如数组的情况，VC也提供了相应的支持，如下的例子：
#include <iostream>
using namespace std;
class MyStruct 
{ 
public:
   __declspec(property(get=GetValue1, put=PutValue1))
   int t[][]; //以二维数组来演示
   int GetValue1(int x, int y) //x,y分别对应第一维和第二维的下标
   { 
      return m_lInternalValue1[x][y]; 
   } 
   void PutValue1(int x,int y, int lValue) //x,y分别对应第一维和第二维的下标，lValue为要赋的值
   { 
      m_lInternalValue1[x][y] = lValue; 
   } 
private: 
   int m_lInternalValue1[3][3]; 
};
int main()
{
    MyStruct ms;
    for (int i = 0; i < 3; i++)
       for (int j = 0; j < 3; j++)
           ms.t[i][j] = i * j;
    return 0;
}
在VC6和VC7中，对于多维数组的处理略有不同，如上面的
   __declspec(property(get=GetValue1, put=PutValue1))
   int t[][]; 
在VC6中可以简单的写成int t[];即可支持两维的数组，而在VC7中必须写成int t[][];才可以。
引用:http://dev.rdxx.com/VC/Property/2002-5/7/040601191.shtml
