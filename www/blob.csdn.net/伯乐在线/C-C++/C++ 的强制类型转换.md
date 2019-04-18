# C++ 的强制类型转换 - 文章 - 伯乐在线
原文出处： [CBDoctor](http://www.cnblogs.com/CBDoctor/archive/2011/12/06/2277918.html)
Q:什么是C风格转换？什么是static_cast, dynamic_cast 以及 reinterpret_cast？区别是什么？为什么要注意？
A:转换的含义是通过改变一个变量的类型为别的类型从而改变该变量的表示方式。为了类型转换一个简单对象为另一个对象你会使用传统的类型转换操作符。
比如，为了转换一个类型为doubole的浮点数的指针到整型：
代码


```
int i;
double d;
i = (int) d;
```
或者：


```
i = int (d);
```
对于具有标准定义转换的简单类型而言工作的很好。然而，这样的转换符也能不分皂白的应用于类（class）和类的指针。ANSI-C++标准定义了四个新的转换符：’reinterpret_cast’, ‘static_cast’, ‘dynamic_cast’ 和 ‘const_cast’，目的在于控制类(class)之间的类型转换。
代码:


```
reinterpret_cast<new_type>(expression)
dynamic_cast<new_type>(expression)
static_cast<new_type>(expression)
const_cast<new_type>(expression)
```
### 1 reinterpret_cast
reinterpret_cast 转换一个指针为其它类型的指针。它也允许从一个指针转换为整数类型。反之亦然。（译注：是指针具体的地址值作为整数值？）
这个操作符能够在非相关的类型之间转换。操作结果只是简单的从一个指针到别的指针的值的二进制拷贝。在类型之间指向的内容不做任何类型的检查和转换。如果情况是从一个指针到整型的拷贝，内容的解释是系统相关的，所以任何的实现都不是方便的。一个转换到足够大的整型能够包含它的指针是能够转换回有效的指针的。
代码:


```
class A {};
class B {};
A * a = new A;
B * b = reinterpret_cast<B *>(a);
```
reinterpret_cast 就像传统的类型转换一样对待所有指针的类型转换。
### 2 static_cast
static_cast 允许执行任意的隐式转换和相反转换动作。（即使它是不允许隐式的）
意思是说它允许子类类型的指针转换为父类类型的指针（这是一个有效的隐式转换），同时，也能够执行相反动作：转换父类为它的子类。在这最后例子里，被转换的父类没有被检查是否与目的类型相一致。
代码：


```
class Base {};
class Derived : public Base {};
Base *a = new Base;
Derived *b = static_cast<Derived *>(a);
```
static_cast 除了操作类型指针，也能用于执行类型定义的显式的转换，以及基础类型之间的标准转换:
代码:


```
double d = 3.14159265;
int i = static_cast<int>(d);
```
### 3 dynamic_cast
dynamic_cast只用于对象的指针和引用。当用于多态类型时，它允许任意的隐式类型转换以及相反过程。不过，与static_cast不同，在后一种情况里（注：即隐式转换的相反过程），dynamic_cast会检查操作是否有效。也就是说，它会检查转换是否会返回一个被请求的有效的完整对象。
检测在运行时进行。如果被转换的指针不是一个被请求的有效完整的对象指针，返回值为NULL.
代码：


```
class Base { virtual dummy() {} };
class Derived : public Base {};
Base* b1 = new Derived;
Base* b2 = new Base;
Derived* d1 = dynamic_cast<Derived *>(b1); // succeeds
Derived* d2 = dynamic_cast<Derived *>(b2); // fails: returns 'NULL'
```
如果一个引用类型执行了类型转换并且这个转换是不可能的，一个bad_cast的异常类型被抛出：
代码:


```
class Base { virtual dummy() {} };
class Derived : public Base { };
Base* b1 = new Derived;
Base* b2 = new Base;
Derived d1 = dynamic_cast<Derived &*>(b1); // succeeds
Derived d2 = dynamic_cast<Derived &*>(b2); // fails: exception thrown
```
### 4 const_cast
这个转换类型操纵传递对象的const属性，或者是设置或者是移除：
代码:


```
class C {};
const C *a = new C;
C *b = const_cast<C *>(a);
```
其它三种操作符是不能修改一个对象的常量性的。注意：’const_cast’也能改变一个类型的volatile qualifier。
### C++ 的四种强制转型形式每一种适用于特定的目的
- dynamic_cast 主要用于执行“安全的向下转型（safe downcasting）”，也就是说，要确定一个对象是否是一个继承体系中的一个特定类型。它是唯一不能用旧风格语法执行的强制转型，也是唯一可能有重大运行时代价的强制转型。
- static_cast 可以被用于强制隐型转换（例如，non-const 对象转型为 const 对象，int 转型为 double，等等），它还可以用于很多这样的转换的反向转换（例如，void* 指针转型为有类型指针，基类指针转型为派生类指针），但是它不能将一个 const 对象转型为 non-const 对象（只有 const_cast 能做到），它最接近于C-style的转换。
- const_cast 一般用于强制消除对象的常量性。它是唯一能做到这一点的 C++ 风格的强制转型。
- reinterpret_cast 是特意用于底层的强制转型，导致实现依赖（implementation-dependent）（就是说，不可移植）的结果，例如，将一个指针转型为一个整数。这样的强制转型在底层代码以外应该极为罕见。
