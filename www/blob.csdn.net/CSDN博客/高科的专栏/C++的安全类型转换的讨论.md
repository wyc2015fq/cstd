
# C++的安全类型转换的讨论 - 高科的专栏 - CSDN博客

2013年11月24日 11:12:38[高科](https://me.csdn.net/pbymw8iwm)阅读数：6677


关于强制类型转换的问题，很多书都讨论过，写的最详细的是C++ 之父的《C++的设计和演化》。最好的解决方法就是不要使用C风格的强制类型转换，而是使用标准C++的类型转换符：static_cast, dynamic_cast。标准C++中有四个类型转换符：static_cast、dynamic_cast、reinterpret_cast、和 const_cast。下面对它们一一进行介绍。
**I.static_cast**
用法：static_cast <type-id > ( expression )
该运算符把expression转换为type-id类型，但没有运行时类型检查来保证转换的安全性。它主要有如下几种用法：
**①**用于类层次结构中基类和子类之间指针或引用的转换。
进行上行转换（把子类的指针或引用转换成基类表示）是安全的；
进行下行转换（把基类指针或引用转换成子类表示）时，由于没有动态类型检查，所以是不安全的。
**②**用于基本数据类型之间的转换。如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。
**③**把空指针转换成目标类型的空指针。
**④**把任何类型的表达式转换成void类型。
注意：static_cast不能转换掉expression的const、volitale、或者__unaligned属性。
**II.dynamic_cast**
**主要用于执行****“****安全的向下转型（****safe down casting****）****”****，也就是说，要确定一个对象是否是一个继承体系中的一个特定类型。**
用法：dynamic_cast <type-id > ( expression )
该运算符把expression转换成type-id类型的对象。Type-id必须是类的指针、类的引用或者void *；如果type-id是类指针类型，那么expression也必须是一个指针，如果type-id是一个引用，那么expression也必须是一个引用。

dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。

在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的；在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全。
class B{
public:
int m_iNum;
virtual void foo();
};
class D:public B{
public:
char *m_szName[100];
};
void func(B *pb){
D *pd1 = static_cast<D *>(pb);
D *pd2 = dynamic_cast<D*>(pb);
}
在上面的代码段中，如果pb指向一个D类型的对象，pd1和pd2是一样的，并且对这两个指针执行D类型的任何操作都是安全的；但是，如果pb指向的是一个 B类型的对象，那么pd1将是一个指向该对象的指针，对它进行D类型的操作将是不安全的（如访问m_szName），而pd2将是一个空指针。另外要注意：B要有虚函数，否则会编译出错；static_cast则没有这个限制。这是由于运行时类型检查需要运行时类型信息，而这个信息存储在类的虚函数表（关于虚函数表的概念，详细可见<Inside c++ object model>）中，只有定义了虚函数的类才有虚函数表，没有定义虚函数的类是没有虚函数表的。
另外，dynamic_cast还支持交叉转换（cross cast）。如下代码所示。
class A{
public:
int m_iNum;
virtual void f(){}
};
class B:public A{
};
class D:public A{
};
void foo(){
B *pb = new B;
pb->m_iNum = 100;
D *pd1 = static_cast<D *>(pb); //copile error
D *pd2 = dynamic_cast<D *>(pb); //pd2 is NULL
delete pb;
}
在函数foo中，使用static_cast进行转换是不被允许的，将在编译时出错；而使用 dynamic_cast的转换则是允许的，结果是空指针。
//////////////////////////////////////////////////////////////////////////////////////////////////////
**向上转换，比较安全，不再举例。**
**向void*转换**
如果 type-id 是一个 void* ，运行时检查将决定表达式的实际类型。结果是一个到 expression 指向的完整对象。例如：
class A { ... };
class B { ... };
void f()
{
A* pa = new A;
B* pb = new B;
void* pv = dynamic_cast<**void***>(pa);
// pv指向一个 A 类型的对象
...
pv = dynamic_cast<void*>(pb);
// pv 指向一个 B 类型的对象
}
**向下转换**
如果 type-id 不是 void* ，运行时检查指向expression 的对象能否转换为指向 type-id 类型的对象。
如果 expression 类型是 type-id 的基类，运行时检查是否expression实际是一个指向type-id类型的完整对象，如果是，结果返回指向type-id类型的完整对象，否则返回NULL。例如：
class B { ... };
class D : public B { ... };
void f()
{
B* pb = new D;                // unclear but ok
B* pb2 = new B;
// 判断一个基类指针指向的对象是否是一个指定子类型
D* pd = dynamic_cast<**D***>(pb);     //ok: pb实际指向 D, 返回D*
D* pd2 = dynamic_cast<**D***>(pb2);  // pb2实际指向 B 而不是 D,转换失败， pd2 是 NULL
...
}
**在多重继承的情况，可能导致二义性。**
看一下下面的类继承层次：


指向类型D的指针转换为B或C都正常，但如果从D转换到A将会怎么样来？这个结果导致转换的二义性错误；为了结果这个问题，你可以指向两次明确的转型，例如：
void f()
{
D* pd = new D;
A* pa = dynamic_cast<A*>(pd);     //错误：二义性
B* pb = dynamic_cast<B*>(pd);     //首先转换到 B
A* pa2 = dynamic_cast<A*>(pb);  // ok:明确的
}
在使用虚基类的时候就导致更复杂的模糊；看下面的类层次图：


在这个继承层次中，A是虚基类。假定一个类E的实例并且一个指向A子对象的指针，一次到B的dynamic_cast会由于不明确性导致失败，你必须首先转换到适当的层次，然后再向上转换到确定的层次，一直按照这种方式直到到达正确的B对象。
看下面的类层次图：


假定一个类型E的对象和一个指向D子对象的指针，从D子对象导航到左上A子对象，必须执行三个转换。从D到E的dynamic_cast的转换，然后一个从E到B的转换（可以是dynamic_cast或者隐式转换），最终是从B到A的转换，例如：
void f(D* pd)
{
E* pe = dynamic_cast<E*>(pd);// 这里的 D 实际上是 E 类型的对象
B* pb = pe;     // upcast, implicit conversion
A* pa = pb;     // upcast, implicit conversion
}
**交叉转换**
dynamic_cast 操作能执行交叉转换，使用上面相同的类层次，从 B 子对象到 D 子对象转换是可能的，只要完整的对象是 E 。
由于交叉转换，从 D 指针到左上角 A 子对象的指针是可行的；首先从 D 到 B 的交叉转换，然后隐式从 B 到 A 的转换。例如：
void f(D* pd)
{
B* pb = dynamic_cast<B*>(pd);     // cross cast
A* pa = pb;                 // upcast, implicit conversion
}
一个 NULL 指针值通过 dynamic_cast 转换到一个 NULL 指针。
当使用**dynamic_cast <***type-id***> (***expression***)时，**如果 expression 不能安全的转换到 type-id，运行时检查导致转型失败，例如：
class A { ... };
class B { ... };
void f()
{
A* pa = new A;
B* pb = dynamic_cast<B*>(pa);     // fails, not safe;   B not derived from A
...
}
转换失败的指针类型是 NULL 指针。失败的引用类型转换抛出 bad_cast_exception 异常；如果 expression没有指向或引用一个有效的对象将抛出 __non_rtti_object 异常。
**再看下面看一个交叉转换例子:**
struct B1{ virtual ~B1(){} };
structB2{ virtual ~B2(){} };
structD1 : B1, B2{};
intmain()
{
D1 d;
B1* pb1 = &d;
B2* pb2 = dynamic_cast<B2*>(pb1);//L1
B2* pb22 = static_cast<B2*>(pb1); //L2 失败
return 0;
}
上述定义中可以看到,B1和B2是不相关的类,从L1可以看到,dynamic_cast允许这种转换:只要B1存在多态方法. L2将编译失败,static_cast并不允许两个完全不相干的类互相转换.
dynamic_cast的这种特性，在提取一个对象的某个接口的时候，非常有用，它很类似于实现了COM的QueryInterface的功能。
**dynamic_cast的讨论：**
在探究 dynamic_cast的设计意图之前，值得留意的是很多dynamic_cast的实现都相当慢。
例如，至少有一种通用的实现部分地基于对类名字进行字符串比较。如果你在一个位于四层深的单继承体系中的对象上执行 dynamic_cast，在这样一个实现下的每一个 dynamic_cast 都要付出相当于四次调用strcmp 来比较类名字的成本。对于一个更深的或使用了多继承的继承体系，付出的代价会更加昂贵。
对 dynamic_cast 的需要通常发生在这种情况下：你要在一个你确信为派生类的对象上执行派生类的操作，但是你只能通过一个基类的指针或引用来操控这个对象。
有两个一般的方法可以避免这个问题：
第一个，使用存储着直接指向派生类对象的指针的容器，从而消除通过基类接口操控这个对象的需要。当然，这个方法不允许你在同一个容器中存储所有可能的基类的派生类的指针。为了与不同的窗口类型一起工作，你可能需要多个类型安全（type-safe）的容器。
一个候选方法可以让你通过一个基类的接口操控所有可能的 Window 派生类，就是在基类中提供一个让你做你想做的事情的虚函数。例如，尽管只有 SpecialWindows 能 blink，在基类中声明这个函数，并提供一个什么都不做的缺省实现或许是有意义的：
所以：
·避免强制转型的随时应用，特别是在性能敏感的代码中应用 dynamic_casts，如果一个设计需要强制转型，设法开发一个没有强制转型的侯选方案。
·如果必须要强制转型，设法将它隐藏在一个函数中。客户可以用调用那个函数来代替在他们自己的代码中加入强制转型。
·尽量用 C++ 风格的强制转型替换旧风格的强制转型。它们更容易被注意到，而且他们做的事情也更加明确。
**III.reinpreter_cast**
用法：reinpreter_cast<type-id>(expression)
type-id必须是一个指针、引用、算术类型、函数指针或者成员指针。它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针（先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值）。
该运算符的用法比较多。
**IV.const_cast**
用法：const_cast<type_id>(expression)
该运算符用来修改类型的const或volatile属性。除了const 或volatile修饰之外， type_id和expression的类型是一样的。
常量指针被转化成非常量指针，并且仍然指向原来的对象；常量引用被转换成非常量引用，并且仍然指向原来的对象；常量对象被转换成非常量对象。
Voiatile和const类试。举如下一例：
class B{
public:
int m_iNum;
}
void foo(){
const B b1;
b1.m_iNum = 100; //comile error
B b2 = const_cast<B>(b1);
b2. m_iNum = 200; //fine
}
上面的代码编译时会报错，因为b1是一个常量对象，不能对它进行改变；使用const_cast把它转换成一个常量对象，就可以对它的数据成员任意改变。注意：b1和b2是两个不同的对象。


