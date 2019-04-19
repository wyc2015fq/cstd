# Modern C++(三)decltype关键字 - xcyl的口袋 - CSDN博客
2016年10月29日 02:48:58[xcyl](https://me.csdn.net/cai6811376)阅读数：266
C++11中新添加了decltype关键字，它用于在编译时推导表达式的类型。因为decltype在编译期推导类型，并不会计算表达式的值。 
我们看一下它是如何使用的
```cpp
//注释为变量类型
int a = 5;
decltype(a) b = 5;          //int
decltype(a + b) c = 5;      //int
const int& d = a;
decltype(d) e = 5;          //const int&
const int* f = new int(5);
decltype(f) g = &a;         //const int*
decltype(a)* h = &a;        //int*
const decltype(a)* i = &a;  //const int*
decltype(h)* j = &h;        //int**
decltype(i)* k = &i;        //const int**
```
decltype推导有以下规则`decltype(e)`
> - 
当e为标识符表达式、类访问表达式，则推导的类型为e的具体类型
- 
若e为函数调用，则推导出的类型为函数调用的返回值类型
- 
若e的类型为左值，则推导出的是e的左值引用，否则为e原始类型
推导规则1
```cpp
int a = 5;
decltype(a) b = a;                  //int
const int& c = a;
decltype(c) d = a;                  //const int&
struct DeclTypeTest
{
    int a;
    static const int b = 5;
};
DeclTypeTest test;
decltype(test.a) e = 5;             //int
decltype(DeclTypeTest::b) f = 5;    //const int
```
根据推导规则1，b、d、e、f都保留了表达式的原属性
推导规则2
```cpp
int funcr();                //纯右值
int& funcl();               //左值
int&& funcx();              //x值
const int cfuncr();         //纯右值
const int& cfuncl();        //左值
const int&& cfuncx();       //x值
int a = 5;
decltype(funcr()) b = 5;    //int
decltype(funcl()) c = a;    //int&
decltype(funcx()) d = 5;    //int&&
decltype(cfuncr()) e = 5;   //int
decltype(cfuncl()) f = a;   //const int&
decltype(cfuncx()) g = 5;   //const int&&
struct DeclTypeTest
{
    int a;
    static const int b = 5;
};
const DeclTypeTest getTest();
decltype(getTest()) h = DeclTypeTest();     //const DeclTypeTest
```
关于左值、右值、x值的概念，我会在接下来的文章中进行介绍，根据推导规则2，推导的类型为函数的返回值类型。但是e的类型为int而不是const int，但是h为const DeclTypeTest类型。这是因为对于纯右值pod类型将忽略const/volatile限定，类类型保留。
推导规则3
```
struct MyStruct
{
    int a;
};
const MyStruct str = MyStruct();
decltype(str.a) a = 0;      //int
decltype((str.a)) b = a;    //const int&
```
a的结果参考推导规则1，b与a的差别在于在使用decltype的时候使用了括号表达式，使用了括号表达式，仍为左值，则根据推导规则3，b的类型为一个左值引用类型。str为const限定类型，则str.a是const int类型的左值，所以最后b的类型为const int&。
典型的decltype应用，返回类型后置
我们看一下这个例子
```cpp
template<typename R, typename TL, typename TR>
R Add(TL tl, TR tr)
{
    return tl + tr;
}
int a = 5;
float b = 5.0;
auto c = Add<decltype(a + b)>(a, b);
```
这里我们发现一个问题，通常外部调用者不知道函数的具体实现。我们可以试着使用decltype在函数定义时表示出函数的返回值。
```cpp
template<typename TL, typename TR>
decltype(tl + tr) Add(TL tl, TR tr)
{
    return tl + tr;
}
```
这样使用是错误的，在计算decltype时还没有对tl和tr进行定义。
这里C++11中提供了返回类型后置的语法，我们看看它是如何使用的
```cpp
template<typename TL, typename TR>
auto Add(TL tl, TR tr) -> decltype(tl + tr)
{
    return tl + tr;
}
```
这是使用auto和decltype使用的典型场景。
decltype多用在泛型编程中，更多的用法希望大家多多去发掘~
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.easydarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
