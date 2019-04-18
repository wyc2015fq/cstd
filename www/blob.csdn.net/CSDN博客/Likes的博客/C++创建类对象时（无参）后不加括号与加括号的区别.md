# C++创建类对象时（无参）后不加括号与加括号的区别 - Likes的博客 - CSDN博客
2018年10月23日 22:25:57[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：97
[https://blog.csdn.net/Windgs_YF/article/details/80927058](https://blog.csdn.net/Windgs_YF/article/details/80927058)
[https://blog.csdn.net/u012750259/article/details/44832769](https://blog.csdn.net/u012750259/article/details/44832769)
## 1、在栈中实例化对象
```cpp
A a;//实例化对象，会调用class A中的构造函数
A a();//声明一个函数，没有传入的参数，返回值为A类型
```
## 2、在堆中实例化对象
### a、没有定义构造函数（编译器自动生成构造函数）
```cpp
int *a = new int;//不会将申请到的int空间初始化
int *a = new int();//则会将申请到的int空间初始化为0。
```
### b、自己定义了构造函数
```cpp
//自己实现了构造函数，下面两种方式完全一样，都会调用构造函数
A* p_a=new A;
A* p_a=new A();
```
