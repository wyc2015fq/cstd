# c++动态分配内存，动态申请数组，new/delete工作方式，new/delete与malloc/free区别 - 别说话写代码的博客 - CSDN博客





2017年07月21日 09:35:58[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：406








```cpp
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;
//malloc/free 与new/delete的差别是什么？
//1.malloc/free是c的标准库函数
//  new/delete是表达式
//2.malloc在开辟空间时，并不会对空间进行初始化
//  new在开辟空间时，是可以进行初始化的
//

int main()

{
     int *p1=(int *)malloc (sizeof(int));//申请堆空间内存
     *p1=10;
     cout << "*p1 = " << *p1 <<endl;
     free(p1);//回收空间

     int *p2=new int(10);//动态申请存储空间并赋初值10
//int *p2=new int;//只申请空间并初始化为0

     cout << "*p2 = " << *p2 <<endl;
     delete p2;

     int *p3 = (int * ) malloc (sizeof(int) * 10);
     free(p3);

     int *p4 = new int[10];//数组开辟空间
     for(int i=0;i<10;i++)
     {
         cout<<p4[i]<<" ";
     }
     delete []p4;//释放new的空间

     return 0;

}
```



运行结果为

![](https://img-blog.csdnimg.cn/20181229155927201.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)



### **new/delete工作机制**

•使用new表达式时发生的三个步骤：

1.调用名为operator new的标准库函数，分配足够大的原始的未类型化的内存，以保存指定类型的一个对象

2.运行该类型的一个构造函数去初始化对象

3.返回指向新分配并构造的构造函数对象的指针

•使用delete表达式时，发生的步骤：

1.调用对象的析构函数

2.调用名为operator delete的标准库函数释放该对象所用的内存



### new/delete与malloc/free区别：

**1 new/delete 与 malloc/free的区别**

    运算符是语言自身的特性，有固定的语义，编译器知道意味着什么，由编译器解释语义，生成相应的代码。

    库函数是依赖于库的，一定程度上独立于语言的。编译器不关心库函数的作用，只保证编译，调用函数参数和返回值符合语法，生成call函数的代码。

     实际中，一些高级点的编译器，都会对库函数进行特别处理。

      malloc/free是**库函数**，new/delete是C++**运算符**。对于非内部数据类型而言，光用malloc/free无法满足动态对象都要求。new/delete是运算符，编译器保证调用构造和析构函数对对象进行初始化/析构。但是库函数malloc/free是库函数，不会执行构造/析构。

**2 delete与delete[ ] 区别**

     delete只会调用一次析构函数，而delete[] 会调用没一个成员的析构函数。

     delete 与 new 配套使用； delete[] 与new[]配套使用。

     对于内建的简单数据类型，delete和delete[] 功能相同。

     对于复杂数据类型，delete和delete[]不同，前者删除单个对象，后者删除数组。

**3 初始化**

   malloc申请的空间不会对空间初始化，new在开辟空间后可以初始化。





