# C++模板详解 - fanyun的博客 - CSDN博客
置顶2017年05月31日 21:19:42[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1838
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**一．C++模板的定义：**
       模板是C++支持参数化多态的工具，使用模板可以使用户为类或者函数声明一种一般模式，使得类中的某些数据成员或者成员函数的参数、返回值取得任意类型。
       模板是一种对类型进行参数化的工具。
**二．使用模板的目的：**
　  能够使得程序员编写与类型无关的代码。比如编写了一个交换两个整型int 类型的swap函数，这个函数就只能实现int 型，对double，字符这些类型无法实现，要实现这些类型的交换就要重新编写另一个swap函数。使用模板的目的就是要让这程序的实现与类型无关，比如一个swap模板函数，即可以实现int 型，又可以实现double型的交换。模板可以应用于函数和类（注意：模板的声明或定义只能在全局，命名空间或类范围内进行。即不能在局部范围，函数内进行，比如不能在main函数中声明或定义一个模板）。
**三．C++模板通常有两种形式：**
**1.函数模板**
（1）.函数模板针对仅参数类型不同的函数；
（2）.函数模板通式：
```cpp
template <class 形参名，class 形参名，......> 返回类型函数名(参数列表)
{
　　　　　　函数体
}
```
其中template和class是关键字，class可以用typename 关键字代替，在这里typename和class没区别，<>括号中的参数叫模板形参，模板形参和函数形参很相像，模板形参不能为空。一但声明了模板函数就可以用模板函数的形参名声明类中的成员变量和成员函数，即可以在该函数中使用内置类型的地方都可以使用模板形参名。模板形参需要调用该模板函数时提供的模板实参来初始化模板形参，一旦编译器确定了实际的模板实参类型就称他实例化了函数模板的一个实例。比如swap的模板函数形式为：
```cpp
template <class T> void swap(T& a,T& b){}，
```
当调用这样的模板函数时类型T就会被被调用时的类型所代替，比如swap(a,b)其中a和b是int 型，这时模板函数swap中的形参T就会被int 所代替，模板函数就变为swap(int &a, int &b)。而当swap(c,d)其中c和d是double类型时，模板函数会被替换为swap(double&a, double &b)，这样就实现了函数的实现与类型无关的代码（注意：对于函数模板而言不存在 h(int,int) 这样的调用，不能在函数调用的参数中指定模板形参的类型，对函数模板的调用应使用实参推演来进行，即只能进行 h(2,3) 这样的调用，或者int
 a, b; h(a,b)）。
**2.类模板**
（1）.类模板针对仅数据成员和成员函数类型不同的类。
（2）.类模板通式
```cpp
template<class  形参名，class 形参名，…>   class 类名
{
        …..;
  };
```
　　类模板和函数模板都是以template开始后接模板形参列表组成，模板形参不能为空，一但声明了类模板就可以用类模板的形参名声明类中的成员变量和成员函数，即可以在类中使用内置类型的地方都可以使用模板形参名来声明。比如：
```cpp
template<classT> class A{public: T a; T b; T hy(Tc, T &d);};
```
在类A中声明了两个类型为T的成员变量a和b，还声明了一个返回类型为T带两个参数类型为T的函数hy。
　　类模板对象的创建：比如一个模板类A，则使用类模板创建对象的方法为A<int> m;在类A后面跟上一个<>尖括号并在里面填上相应的类型，这样的话类A中凡是用到模板形参的地方都会被int 所代替。当类模板有两个模板形参时创建对象的方法为A<int, double> m;类型之间用逗号隔开。
　　对于类模板，模板形参的类型必须在类名后的尖括号中明确指定。比如A<2> m;用这种方法把模板形参设置为int是错误的（编译错误：error C2079: 'a' uses undefined class'A<int>'），类模板形参不存在实参推演的问题。也就是说不能把整型值2推演为int 型传递给模板形参。要把类模板形参调置为int 型必须这样指定A<int> m。
　　在类模板外部定义成员函数的方法为：
　　template<模板形参列表> 函数返回类型类名<模板形参名>::函数名(参数列表){函数体}，
比如有两个模板形参T1，T2的类A中含有一个void h()函数，则定义该函数的语法为：
```cpp
template<classT1,class T2> voidA<T1,T2>::h(){}。
```
注意：当在类外面定义类的成员时template后面的模板形参应与要定义的类的模板形参一致。
　　备注：再次提醒注意：模板的声明或定义只能在全局，命名空间或类范围内进行。即不能在局部范围，函数内进行，比如不能在main函数中声明或定义一个模板。
**四．模板的形参**
**1.类型模板形参：**
类型形参由关键字class或typename后接说明符构成，如template<class T> void h(T a){};其中T就是一个类型形参，类型形参的名字由用户自已确定。模板形参表示的是一个未知的类型。模板类型形参可作为类型说明符用在模板中的任何地方，与内置类型说明符或类类型说明符的使用方式完全相同，即可以用于指定返回类型，变量声明等。
在类模板中，当我们声明类对象为：A<int> a，比如template<class T>T g(T a, T b){}，语句调用a.g(2, 3.2)在编译时不会出错，但会有警告，因为在声明类对象的时候已经将T转换为int类型，而第二个实参3.2把模板形参指定为double，在运行时，会对3.2进行强制类型转换为3。当我们声明类的对象为：A<double> a,此时就不会有上述的警告，因为从int到double是自动类型转换。
在函数模板中，不能为同一个模板类型形参指定两种不同的类型，比如template<class T>void h(T a, T b){}，语句调用h(2,3.2)将出错，因为该语句给同一模板形参T指定了两种类型，第一个实参2把模板形参T指定为int，而第二个实参3.2把模板形参指定为double，两种类型的形参不一致，会出错。
类型实参实例：
TemplateDemo.h
```cpp
#ifndef TEMPLATE_DEMO_HXX
#define TEMPLATE_DEMO_HXX
template<class T> class A{
   public:
       T g(T a,T b);
       A();
};
#endif
```
**TemplateDemo.cpp**
```cpp
#include<iostream.h>
#include "TemplateDemo.h"
 
template<class T> A<T>::A(){}
 
template<class T> T A<T>::g(Ta,T b){
   return a+b;
}
```
```cpp
void main(){
   A<int> a;
   cout<<a.g(2,3.2)<<endl;//注意，如果两个变量类型不一致，会报警告
}
```
**2.非类型形参：**
非类型模板形参概念（一般不应用于函数模板中）：
模板的非类型形参也就是内置类型形参，如template<classT, int a> class B{};其中inta就是非类型的模板形参。
非类型模板形参的特点：
(1).非类型形参在模板定义的内部是常量值，也就是说非类型形参在模板的内部是常量。
(2). 非类型模板的形参只能是整型，指针和引用，像double，String, String **这样的类型是不允许的。但是double &，double *，对象的引用或指针是正确的。
(3). 调用非类型模板形参的实参必须是一个常量表达式，即他必须能在编译时计算出结果。
(4).注意：任何局部对象，局部变量，局部对象的地址，局部变量的地址都不是一个常量表达式，都不能用作非类型模板形参的实参。全局指针类型，全局变量，全局对象也不是一个常量表达式，不能用作非类型模板形参的实参。
(5). 全局变量的地址或引用，全局对象的地址或引用const类型变量是常量表达式，可以用作非类型模板形参的实参。
(6).sizeof表达式的结果是一个常量表达式，也能用作非类型模板形参的实参。
(7).当模板的形参是整型时调用该模板时的实参必须是整型的，且在编译期间是常量，比如template <class T, int a> class A{};如果有int b，这时A<int, b> m;将出错，因为b不是常量，如果const int b，这时A<int, b> m;就是正确的，因为这时b是常量。
(8).非类型形参一般不应用于函数模板中，比如有函数模板template<class T, int a> void h(Tb){}，若使用h(2)调用会出现无法为非类型形参a推演出参数的错误，对这种模板函数可以用显示模板实参来解决，如用h<int, 3>(2)这样就把非类型形参a设置为整数3。显示模板实参在后面介绍。
(9). 非类型模板形参的形参和实参间所允许的转换:
1).允许从数组到指针，从函数到指针的转换。如：template <int *a> classA{}; int b[1]; A<b> m;即数组到指针的转换
2).const修饰符的转换。如：template<const int *a> class A{}; int b; A<&b>m;   即从int *到const int *的转换。
3).提升转换。如：template<int a> class A{}; const short b=2; A<b> m; 即从short到int 的提升转换
4).整值转换。如：template<unsigned int a> class A{};   A<3> m; 即从int 到unsigned int的转换。5).常规转换。
**非类型形实参实例1：**由用户自己亲自指定栈的大小，并实现栈的相关操作。
**TemplateDemo.h**
```cpp
#ifndef TEMPLATE_DEMO_HXX
#define TEMPLATE_DEMO_HXX
template<class T,int MAXSIZE> classStack{//MAXSIZE由用户创建对象时自行设置
   private:
       T elems[MAXSIZE];    // 包含元素的数组
       int numElems;    // 元素的当前总个数
   public:
       Stack();    //构造函数
       void push(T const&);    //压入元素
       void pop();        //弹出元素
       T top() const;    //返回栈顶元素
       bool empty() const{     // 返回栈是否为空
           return numElems == 0;
       }
       bool full() const{    // 返回栈是否已满
           return numElems == MAXSIZE;
       }
};
template <class T,int MAXSIZE>
Stack<T,MAXSIZE>::Stack():numElems(0){     // 初始时栈不含元素
   // 不做任何事情
}
template <class T,int MAXSIZE>
void Stack<T, MAXSIZE>::push(Tconst& elem){
   if(numElems == MAXSIZE){
       throw std::out_of_range("Stack<>::push(): stack isfull");
   }
   elems[numElems] = elem;   // 附加元素
   ++numElems;               // 增加元素的个数
}
template<class T,int MAXSIZE>
void Stack<T,MAXSIZE>::pop(){
   if (numElems <= 0) {
       throw std::out_of_range("Stack<>::pop(): empty stack");
   }
   --numElems;               // 减少元素的个数
}
template <class T,int MAXSIZE>
T Stack<T,MAXSIZE>::top()const{
   if (numElems <= 0) {
       throw std::out_of_range("Stack<>::top(): empty stack");
   }
   return elems[numElems-1];  // 返回最后一个元素
}
#endif
```
TemplateDemo.cpp
```cpp
#include<iostream.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include "TemplateDemo.h"
int main(){
   try {
       Stack<int,20> int20Stack;  // 可以存储20个int元素的栈
       Stack<int,40> int40Stack;  // 可以存储40个int元素的栈
       Stack<std::string,40> stringStack; // 可存储40个string元素的栈
       // 使用可存储20个int元素的栈
       int20Stack.push(7);
       std::cout << int20Stack.top() << std::endl;    //7
       int20Stack.pop();
       // 使用可存储40个string的栈
       stringStack.push("hello");
       std::cout << stringStack.top() << std::endl;    //hello
       stringStack.pop();   
       stringStack.pop();    //Exception:Stack<>::pop<>: empty stack
       return 0;
   }
   catch (std::exception const& ex) {
       std::cerr << "Exception: " << ex.what() <<std::endl;
       return EXIT_FAILURE;  // 退出程序且有ERROR标记
   }
}
```
**运行结果：**
**非类型形参演示示例2：**
**TemplateDemo01.h**
```cpp
#ifndef TEMPLATE_DEMO_O1
#define TEMPLATE_DEMO_01
template<typename T> classCompareDemo{
   public:
       int compare(const T&, const T&);
};
template<typename T>
int CompareDemo<T>::compare(constT& a,const T& b){
   if((a-b)>0)
       return 1;
   else if((a-b)<0)
       return -1;
   else
       return 0;
}
#endif
```
测试1：
```cpp
#include<iostream.h>
#include "TemplateDemo01.h"
void main(){
   CompareDemo<int> cd;
   cout<<cd.compare(2,3)<<endl;
}
```
**结果：-1**
测试2：
```cpp
#include<iostream.h>
#include "TemplateDemo01.h"
void main(){
   CompareDemo<double> cd;
   cout<<cd.compare(3.2,3.1)<<endl;
}
```
**结果：1**
**TemplateDemo01.h 改动如下：**
```cpp
#ifndef TEMPLATE_DEMO_O1
#define TEMPLATE_DEMO_01
template<typename T> classCompareDemo{
   public:
       int compare(T&, T&);
};
template<typename T>
int CompareDemo<T>::compare(T&a,T& b){
   if((a-b)>0)
       return 1;
   else if((a-b)<0)
       return -1;
   else
      return 0;
}
#endif
```
**TempalteDemo01.cpp**
```cpp
#include<iostream.h>
#include "TemplateDemo01.h"
 
void main(){
   CompareDemo<int> cd;
   int a=2,b=3;
   cout<<cd.compare(a,b)<<endl;
}
TemplateDemo02.cpp
#include<iostream.h>
template<typename T>
const T& max(const T& a,constT& b){
   return a>b ? a:b;
}
```
```cpp
void main(){
   cout<<max(2.1,2.2)<<endl;//模板实参被隐式推演成double
   cout<<max<double>(2.1,2.2)<<endl;//显示指定模板参数。
   cout<<max<int>(2.1,2.2)<<endl;//显示指定的模板参数，会将函数函数直接转换为int。(备注：会产生警告)
}
```
**3.类模板的默认模板类型形参**
(1).可以为类模板的类型形参提供默认值，但不能为函数模板的类型形参提供默认值。函数模板和类模板都可以为模板的非类型形参提供默认值。
(2).类模板的类型形参默认值形式为：template<classT1, class T2=int> class A{};为第二个模板类型形参T2提供int型的默认值。
(3).类模板类型形参默认值和函数的默认参数一样，如果有多个类型形参则从第一个形参设定了默认值之后的所有模板形参都要设定默认值，比如template<classT1=int, class T2>class A{};就是错误的，因为T1给出了默认值，而T2没有设定。
(4).在类模板的外部定义类中的成员时template 后的形参表应省略默认的形参类型。比如template<class T1, class T2=int> class A{public: void h();}; 定义方法为template<class T1,class T2> void A<T1,T2>::h(){}。
定义类模板类型形参：
演示实例1：
**TemplateDemo.h**
```cpp
#ifndefTEMPLATE_DEMO_HXX
#defineTEMPLATE_DEMO_HXX
template<classT> class A{
    public:
        T g(T a,T b);
        A();
};
#endif
```
**TemplateDemo.cpp**
```cpp
#include<iostream.h>
#include"TemplateDemo.h"
template<classT> A<T>::A(){}
template<classT> T A<T>::g(T a,T b){
    return a+b;
}
void main(){
    A<int> a;
    cout<<a.g(2,3)<<endl;
}
```
结果：5
**类模板的默认模板类型形参示例1：**
**TemplateDemo3.h**
```cpp
#ifndefTEMPLATE_DEMO_03
#defineTEMPLATE_DEMO_03
//定义带默认类型形参的类模板。这里把T2默认设置为int型。
template<classT1,class T2=int> class CeilDemo{
    public:
        int ceil(T1,T2);
};
//在类模板的外部定义类中的成员时template 后的形参表应省略默认的形参类型。
template<classT1,class T2>
intCeilDemo<T1,T2>::ceil(T1 a,T2 b){
    return a>>b;
}
#endif
```
**TemplateDemo3.cpp**
```cpp
#include<iostream.h>
#include "TemplateDemo03.h"
void main(){
   CeilDemo<int> cd;
   cout<<cd.ceil(8,2)<<endl;
}
```
运行结果：2
在类模板的外部定义类中的成员时template 后的形参表应省略默认的形参类型，如果没有省略，不会出现编译错误而是提出警告：
原作者：类模板类型形参默认值和函数的默认参数一样，如果有多个类型形参则从第一个形参设定了默认值之后的所有模板形参都要设定默认值，比如template<class T1=int, classT2>class A{};就是错误的，因为T1给出了默认值，而T2没有设定。
实例测试如下：
类模板的默认模板类型形参示例2：
**TemplateDemo03.h**
```cpp
#ifndef TEMPLATE_DEMO_03
#define TEMPLATE_DEMO_03
template<class T1=int,class T2,classT3> class CeilDemo{
   public:
       int ceil(T1,T2,T3);
};
template<class T1,class T2,class T3>
int CeilDemo<T1,T2,T3>::ceil(T1 a,T2b,T3 c){
   return a+b+c;
}
#endif
```
**TemplateDemo03.cpp**
```cpp
#include<iostream.h>
#include "TemplateDemo03.h"
void main(){
   CeilDemo<int,int> cd;
   cout<<cd.ceil(2,3,4)<<endl;
}
```
类模板的默认模板类型形参示例4：
**TemplateDemo03.h**
```cpp
#ifndef TEMPLATE_DEMO_03
#define TEMPLATE_DEMO_03
template<class T1=int,classT2=double,class T3=double> class CeilDemo{
   public:
       double ceil(T1,T2,T3);
};
template<class T1,class T2,class T3>
double CeilDemo<T1,T2,T3>::ceil(T1a,T2 b,T3 c){
   return a+b+c;
}
#endif
```
**TemplateDemo03.cpp**
```cpp
#include<iostream.h>
#include "TemplateDemo03.h"
void main(){
   CeilDemo<int,double,double> cd;
   cout<<cd.ceil(2,3.1,4.1)<<endl;//会报错
}
```
