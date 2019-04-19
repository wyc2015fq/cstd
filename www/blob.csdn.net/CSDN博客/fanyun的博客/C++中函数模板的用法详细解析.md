# C++中函数模板的用法详细解析 - fanyun的博客 - CSDN博客
2017年02月26日 21:10:30[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：6985
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)

一、函数模板的产生原因：
函数的重载可以实现一个函数名多用，将功能相同或者类似函数用同一个名来定义。这样可以简化函数的调用形式，但是程序中，仍然需要分别定义每一个函数。那么有什么办法可以让我们少写一些函数呢？C++提供了函数模板就可以极大地方便同类型函数的使用。
二、什么是函数模板？
所谓函数模板实际上是建立一个通用函数，其函数类型和形参类型不具体指定，用一个虚拟的类型来代表，这个通用函数就称为函数模板。凡是函数体相同的函数都可以用这个模板来代替，不必定义多个函数，只需要在模板中定义一次即可。在调用函数时，系统会根据实参的类型来取代模板中的虚拟类型，从而实现了不同函数的功能。
三、函数模板的一般形式
template <typename T>（通用函数定义）或者template <class T>
四、示例比较
普通函数，不使用函数模板：
```cpp
#include<iostream>
using namespace std;
int n_add(int a,int b)                              //定义函数n_add用于int型数据相加
{
 int c;
 c=a+b;
 return c;
} 
double n_add(double a,double b)    //定义函数n_add用于double型函数相加
{
 double c;
 c=a+b;
 return c;
}
int main()
{
 cout<<n_add(8,2)<<endl;             //调用n_add函数
 cout<<n_add(8.35,5.5)<<endl;   //调用n_add函数
 return 0;
}
```
使用函数模板：
```cpp
#include<iostream>
using namespace std;
template<typename T>
T n_add(T a,T b)
{
 T c;
 c=a+b;
 return c;
} 
int main()
{
 cout<<n_add(8,2)<<endl;
 cout<<n_add(8.12,2.2)<<endl;
 return 0;
}
```
对比不使用函数模板和使用函数模板的代码，在函数的调用形式上进行了简化，使用了函数的重载技术，对于所有的数据，统一使用函数n_add进行操作，编译系统会根据数据的类型自动调用对应的函数。我们使用了函数模板，我们就没有必要去一一书写对应的函数，我们只需要构造相应的模板，然后系统会自动判断数据的类型，然后替代对应的虚拟类型。
