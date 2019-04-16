# 对象（深入C++对象模型） - youbingchen的博客 - CSDN博客





2016年05月26日 20:07:14[youbingchen](https://me.csdn.net/youbingchen)阅读数：710








```cpp
Matrix identity //Matrix 是一个类

main()
{
 //identity必须放在此处初始化
 Matrix m1= identify;
 ...
 return 0;
}
```

Ｃ＋＋　保证，一定会在main函数中第一次用到identity之前，把identity构造出来，而在main函数结束之前把identity结束掉，像identity这样所谓的全局对象，如果有构造函数和析构函数，它需要静态的初始化操作和内存释放操作。 

C++程序中所有的数据部分都被放置在程序的数据部分中，如果显式指定给一个值，此对象将以该值为初值，否则object所配置的内容都为0。

```cpp
int v1 = 1024;
int v2;
```

v1和v2都配置于程序的数据区，v1值为1024，v2值为0（这和C有不同，C并不自动设定初值）。

# 局部静态对象

```cpp
const Matrix& identify()
{
 static Matrix mat_identify;
 //...
 return mat_identify;
}
```
- mat_identify的构造必须只能运行一次，虽然上述函数可能会被调用多次
- mat_identity的析构必须只能运行一次，虽然上述函数可能会被调用多次

# new 和 delete

new 运算符实际上总是以标准的C `malloc()`完成，虽然并没有规定一定得这么做不可，相同情况，delete运算符也是以标准的C `free()`完成的。

# 临时性对象

如果我们有一个函数

```cpp
T operator+ (const T &,const T &)
```

以及两个对象，a和b，那么：

```cpp
a+b;
```

可能会导致一个临时性对象，以放置传回的对象。是否导致一个临时性对象，视编译器的进取性以及上述操作发生时的程序语境而定。

```cpp
T a,b;
T c = a+b;
```

**编译器会产生一个临时性对象，放置`a+b`的结果，然后再利用T的拷贝构造函数，把该临时性对象当做c的初始值。然而比较更可能的转换就是直接以拷贝构造的方式，将`a+b`的值放在c中，于是不需要临时性对象，以及对其构造和析构的调用。此外，视`operator+()的定义而定，named return value (NRV)`优化，这将导致直接在上述c对象中求表达式结果，避免拷贝构造和具体对象的析构**



