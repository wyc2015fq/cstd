# 标准C++四个类型转换详解 - fanyun的博客 - CSDN博客
2016年05月31日 15:27:13[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4273
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
C++中的类型转换分为两种：
1.      隐式类型转换（而对于隐式变换，就是标准的转换，在很多时候，不经意间就发生了，比如int类型和float类型相加时，int类型就会被隐式的转换位float类型，然后再进行相加运算。）；
2.      显式类型转换。
        关于强制类型转换的问题，很多书都讨论过，写的最详细的是C++ 之父的《C++的设计和演化》。最好的解决方法就是不要使用C风格的强制类型转换，而是使用标准C++的类型转换符：static_cast, dynamic_cast。标准C++中有四个类型转换符：static_cast、dynamic_cast、reinterpret_cast、和 const_cast。下面对它们一一进行介绍。
**1.     static_cast**
static_cast的转换格式：static_cast< type-id > ( expression )
该运算符把expression转换为type-id类型，但没有运行时类型检查来保证转换的安全性。它主要有如下几种用法：
**①**用于类层次结构中基类和子类之间指针或引用的转换。
进行上行转换（把子类的指针或引用转换成基类表示）是安全的；
进行下行转换（把基类指针或引用转换成子类表示）时，由于没有动态类型检查，所以是不安全的。
**②**用于基本数据类型之间的转换。如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。
**③**把空指针转换成目标类型的空指针。
**④**把任何类型的表达式转换成void类型。
注意：static_cast不能转换掉expression的const、volitale、或者__unaligned属性。
**2.     dynamic_cast**
**        主要用于执行“安全的向下转型（safe down casting）”，也就是说，要确定一个对象是否是一个继承体系中的一个特定类型。**
dynamic_cast的转换格式：dynamic_cast< type-id > ( expression )
        该运算符把expression转换成type-id类型的对象。Type-id必须是类的指针、类的引用或者void*；如果type-id是类指针类型，那么expression也必须是一个指针，如果type-id是一个引用，那么expression也必须是一个引用。
dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。
        在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的；在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全。在多态类型之间的转换主要使用dynamic_cast，因为类型提供了运行时信息。
（1）.上行转换
        比如B继承自A，B转换为A，进行上行转换时，是安全的，如下：
```cpp
#include <iostream>
using namespace std;
class A
{
	// ......
};
class B : public A
{
	// ......
};
int main()
{
	B *pB = new B;
	A *pA = dynamic_cast<A *>(pB); // Safe and will succeed
}
```
（2）.多重继承之间的上行转换：
         C继承自B，B继承自A，这种多重继承的关系；但是，关系很明确，使用dynamic_cast进行转换时，也是很简单的：
```cpp
class A
{
	// ......
};
class B : public A
{
	// ......
};
class C : public B
{
	// ......
};
int main()
{
	C *pC = new C;
	B *pB = dynamic_cast<B *>(pC); // OK
	A *pA = dynamic_cast<A *>(pC); // OK
}
```
         而上述的转换，static_cast和dynamic_cast具有同样的效果。而这种上行转换，也被称为隐式转换；比如我们在定义变量时经常这么写：B *pB = new C;这和上面是一个道理的，只是多加了一个dynamic_cast转换符而已。
（3）.转换成void*
可以将类转换成void *，例如：
```cpp
class A
{
public:
	virtual void f(){}
	// ......
};
class B
{
public:
	virtual void f(){}
	// ......
};
int main()
{
	A *pA = new A;
	B *pB = new B;
	void *pV = dynamic_cast<void *>(pA); // pV points to an object of A
	pV = dynamic_cast<void *>(pB); // pV points to an object of B
}
```
        但是，在类A和类B中必须包含虚函数，为什么呢？因为类中存在虚函数，就说明它有想让基类指针或引用指向派生类对象的情况，此时转换才有意义；由于运行时类型检查需要运行时类型信息，而这个信息存储在类的虚函数表中，只有定义了虚函数的类才有虚函数表。
（4）.如果expression是type-id的基类，使用dynamic_cast进行转换时，在运行时就会检查expression是否真正的指向一个type-id类型的对象，如果是，则能进行正确的转换，获得对应的值；否则返回NULL，如果是引用，则在运行时就会抛出异常；例如：
```cpp
class B
{
	virtual void f(){};
};
class D : public B
{
	virtual void f(){};
};
void main()
{
	B* pb = new D;   // unclear but ok
	B* pb2 = new B;
	D* pd = dynamic_cast<D*>(pb);   // ok: pb actually points to a D
	D* pd2 = dynamic_cast<D*>(pb2);   // pb2 points to a B not a D, now pd2 is NULL
}
```
这个就是下行转换，从基类指针转换到派生类指针。
对于一些复杂的继承关系来说，使用dynamic_cast进行转换是存在一些陷阱的；比如，钻石结构：
D类型可以安全的转换成B和C类型，但是D类型要是直接转换成A类型呢？
```cpp
class A
{
	virtual void Func() = 0;
};
class B : public A
{
	void Func(){};
};
class C : public A
{
	void Func(){};
};
class D : public B, public C
{
	void Func(){}
};
int main()
{
	D *pD = new D;
	A *pA = dynamic_cast<A *>(pD); // You will get a pA which is NULL
}
```
        如果进行上面的直接转，你将会得到一个NULL的pA指针；这是因为，B和C都继承了A，并且都实现了虚函数Func，导致在进行转换时，无法进行抉择应该向哪个A进行转换。正确的做法是：
```cpp
int main()
{
	D *pD = new D;
	B *pB = dynamic_cast<B *>(pD);
	A *pA = dynamic_cast<A *>(pB);
}
```
这就是我在实现QueryInterface时，得到IUnknown的指针时，使用的是*ppv = static_cast<IX *>(this);而不是*ppv = static_cast<IUnknown *>(this);
**dynamic_cast的讨论：**
         在探究 dynamic_cast 的设计意图之前，值得留意的是很多 dynamic_cast 的实现都相当慢。
         例如，至少有一种通用的实现部分地基于对类名字进行字符串比较。如果你在一个位于四层深的单继承体系中的对象上执行 dynamic_cast，在这样一个实现下的每一个 dynamic_cast 都要付出相当于四次调用strcmp 来比较类名字的成本。对于一个更深的或使用了多继承的继承体系，付出的代价会更加昂贵。
        对 dynamic_cast 的需要通常发生在这种情况下：你要在一个你确信为派生类的对象上执行派生类的操作，但是你只能通过一个基类的指针或引用来操控这个对象。
有两个一般的方法可以避免这个问题：
        第一个，使用存储着直接指向派生类对象的指针的容器，从而消除通过基类接口操控这个对象的需要。当然，这个方法不允许你在同一个容器中存储所有可能的基类的派生类的指针。为了与不同的窗口类型一起工作，你可能需要多个类型安全（type-safe）的容器。
        一个候选方法可以让你通过一个基类的接口操控所有可能的 Window 派生类，就是在基类中提供一个让你做你想做的事情的虚函数。例如，尽管只有 SpecialWindows 能 blink，在基类中声明这个函数，并提供一个什么都不做的缺省实现或许是有意义的：
所以：
（1）避免强制转型的随时应用，特别是在性能敏感的代码中应用 dynamic_casts，如果一个设计需要强制转型，设法开发一个没有强制转型的侯选方案。
（2）如果必须要强制转型，设法将它隐藏在一个函数中。客户可以用调用那个函数来代替在他们自己的代码中加入强制转型。
（3）尽量用 C++ 风格的强制转型替换旧风格的强制转型。它们更容易被注意到，而且他们做的事情也更加明确。
**3.     reinpreter_cast**
用法：reinpreter_cast<type-id>(expression)
        type-id必须是一个指针、引用、算术类型、函数指针或者成员指针。它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针（先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值）。
该运算符的用法比较多。
**4.     const_cast**
const_cast的转换格式：const_cast<type_id> (expression)
       该运算符用来修改类型的const或volatile属性。除了const 或volatile修饰之外， type_id和expression的类型是一样的。
常量指针被转化成非常量指针，并且仍然指向原来的对象；常量引用被转换成非常量引用，并且仍然指向原来的对象；常量对象被转换成非常量对象。
Voiatile和const类试。举如下一例：
```cpp
#include <iostream>
using namespace std;
class CA
{
public:
	CA() :m_iA(10){}
	int m_iA;
};
int main()
{
	const CA *pA = new CA;
	// pA->m_iA = 100; // Error
	CA *pB = const_cast<CA *>(pA);
	pB->m_iA = 100;
	// Now the pA and the pB points to the same object
	cout << pA->m_iA << endl;
	cout << pB->m_iA << endl;
	const CA &a = *pA;
	// a.m_iA = 200; // Error
	CA &b = const_cast<CA &>(a);
	b.m_iA = 200;
	// Now the a and the b reference to the same object
	cout << b.m_iA << endl;
	cout << a.m_iA << endl;
}
```

