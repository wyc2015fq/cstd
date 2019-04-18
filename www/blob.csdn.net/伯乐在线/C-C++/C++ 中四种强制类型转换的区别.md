# C++ 中四种强制类型转换的区别 - 文章 - 伯乐在线
原文出处： [梦醒潇湘love](http://blog.chinaunix.net/uid-26548237-id-3954104.html)
使用标准C++的类型转换符：static_cast、dynamic_cast、reinterpret_cast和const_cast。
#### 1、static_cast
用法：static_cast (expression)
该运算符把expression转换为type-id类型，但没有运行时类型检查来保证转换的安全性。它主要有如下几种用法：
（1）用于类层次结构中基类和派生类之间指针或引用的转换
进行上行转换（把派生类的指针或引用转换成基类表示）是安全的
进行下行转换（把基类的指针或引用转换为派生类表示），由于没有动态类型检查，所以是不安全的
（2）用于基本数据类型之间的转换，如把int转换成char。这种转换的安全也要开发人员来保证
（3）把空指针转换成目标类型的空指针
（4）把任何类型的表达式转换为void类型
注意：static_cast不能转换掉expression的const、volitale或者__unaligned属性。
#### 2、dynamic_cast
用法：dynamic_cast (expression)
该运算符把expression转换成type_id类型的对象。type_id必须是类的指针、引用或者void*；
如果type_id是类指针类型，那么expression也必须是一个指针，如果type_id是一个引用，那么expression也必须是一个引用。
dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。
在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的；
在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全。


```
class B
{
public:
      int m_iNum;
       virtual void foo();
};
class D:public B
{
    public:
       char *m_szName[100];
};
void func(B *pb)
{
    D *pd1 = static_cast<D *>(pb);
    D *pd2 = dynamic_cast<D *>(pb);
}
```
在上面的代码段中，如果pb指向一个D类型的对象，pd1和pd2是一样的，并且对这两个指针执行D类型的任何操作都是安全的；
但是，如果pb指向的是一个B类型的对象，那么pd1将是一个指向该对象的指针，对它进行D类型的操作将是不安全的（如访问m_szName），而pd2将是一个空指针。
另外要注意：B要有虚函数，否则会编译出错；static_cast则没有这个限制。这是由于运行时类型检查需要运行时类型信息，而这个信息存储在类的虚函数表（关于虚函数表的概念，详细可见）中，只有定义了虚函数的类才有虚函数表，没有定义虚函数的类是没有虚函数表的。
另外，dynamic_cast还支持交叉转换，如下所示。


```
class A
{
public:
    int m_iNum;
    virtual void f(){}
};
class B:public A
{
    
};
class D:public A
{
    
};
void foo()
{    
    B *pb = new B;    
    pb->m_iNum = 100;    
    D *pd1 = static_cast<D *>(pb); //compile error    
    D *pd2 = dynamic_cast<D *>(pb); //pd2 is NULL    
    delete pb;    
}
```
在函数foo中，使用static_cast进行转换是不被允许的，将在编译时出错，而使用dynamic_cast转换则是允许的，结果是空指针。
#### 3、reinterpret_cast
用法：reinterpret_cast (expression)
type-id必须是一个指针、引用、算术类型、函数指针或者成员指针。
它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针（先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值）。
该运算符的用法比较多。
(static_cast .与. reinterpret_cast比较，见下面 )
该运算符平台移植性比价差。
#### 4、const_cast
用法：const_cast (expression)
该运算符用来修改类型的const或volatile属性。除了const 或volatile修饰之外， type_id和expression的类型是一样的。
常量指针被转化成非常量指针，并且仍然指向原来的对象；
常量引用被转换成非常量引用，并且仍然指向原来的对象；常量对象被转换成非常量对象。
volatile和const类型，举例如下所示。


```
class B
{
public:    
    int m_iNum;    
}
void foo()
{
    const B b1;    
    b1.m_iNum = 100; //comile error    
    B b2 = const_cast<B>(b1);    
    b2. m_iNum = 200; //fine    
}
```
上面的代码编译时会报错，因为b1是一个常量对象，不能对它进行改变；
使用const_cast把它转换成一个非常量对象，就可以对它的数据成员任意改变。注意：b1和b2是两个不同的对象。
#### 5、比较
##### （1）dynamic_cast vs static_cast


```
class B 
{ 
    ... 
};
class D : public B 
{ 
    ...
};
void f(B* pb)
{    
    D* pd1 = dynamic_cast<D*>(pb);    
    D* pd2 = static_cast<D*>(pb);    
}
```
If pb really points to an object of type D, then pd1 and pd2 will get the same value. They will also get the same value if pb == 0.
If pb points to an object of type B and not to the complete D class, then dynamic_cast will know enough to return zero. However, static_cast relies on the programmer’s assertion that pb points to an object of type D and simply returns a pointer to that supposed D object.
即dynamic_cast可用于继承体系中的向下转型，即将基类指针转换为派生类指针，比static_cast更严格更安全。dynamic_cast在执行效率上比static_cast要差一些，但static_cast在更宽上范围内可以完成映射，这种不加限制的映射伴随着不安全性。static_cast覆盖的变换类型除类层次的静态导航以外，还包括无映射变换、窄化变换(这种变换会导致对象切片,丢失信息)、用VOID*的强制变换、隐式类型变换等…
##### （2）static_cast vs reinterpret_cast
reinterpret_cast是为了映射到一个完全不同类型的意思，这个关键词在我们需要把类型映射回原有类型时用到它。我们映射到的类型仅仅是为了故弄玄虚和其他目的，这是所有映射中最危险的。(这句话是C++编程思想中的原话)
static_cast 和 reinterpret_cast 操作符修改了操作数类型。它们不是互逆的； static_cast 在编译时使用类型信息执行转换，在转换执行必要的检测(诸如指针越界计算, 类型检查). 其操作数相对是安全的。另一方面；reinterpret_cast 仅仅是重新解释了给出的对象的比特模型而没有进行二进制转换， 例子如下：


```
int n=9; double d=static_cast < double > (n);
```
上面的例子中, 我们将一个变量从 int 转换到 double。 这些类型的二进制表达式是不同的。 要将整数 9 转换到 双精度整数 9，static_cast 需要正确地为双精度整数 d 补足比特位。其结果为 9.0。
而reinterpret_cast 的行为却不同:


```
int n=9;
    double d=reinterpret_cast<double & > (n);
```
这次, 结果有所不同. 在进行计算以后, d 包含无用值. 这是因为 reinterpret_cast 仅仅是复制 n 的比特位到 d, 没有进行必要的分析.
因此, 你需要谨慎使用 reinterpret_cast.
引自：http://www.cppblog.com/lapcca/archive/2010/11/30/135081.aspx
补充：
（1）static_cast：在功能上基本上与C风格的类型转换一样强大，含义也一样。它有功能上的限制。例如，你不能用static_cast像用C风格转换一样把struct转换成int类型或者把double类型转换成指针类型。另外，static_cast不能从表达式中去除const属性，因为另一个新的类型转换符const_cast有这样的功能。
可以静态决议出类型的转换可能性，即使是在继承体系中，即使包括了多重继承和虚继承，只要可以进行静态决议就可以转换成功
（2）const_cast：用于类型转换掉表达式的const或volatile属性。通过使用const_cast，你向人们和编译器强调你通过类型转换想做的只是改变一些东西的constness或者volatieness属性。这个含义被编译器所约束。如果你试图使用const_cast来完成修改constness或者volatileness属性之外的事情，你的类型转换将被拒绝。
（3）dynamic_cast：它被用于安全地沿着类的继承关系向下进行类型转换。这就是说，你能用dynamic_cast把指向基类的指针或引用转换成指向其派生类或其兄弟类的指针或引用，而且你能知道转换是否成功。失败的转换将返回空指针（当对指针进行类型转换时）或者抛出异常（当对引用进行类型转换时）。
（4）reinterpret_cast：使用这个操作符的类型转换，其转换结果几乎都是执行期定义。因此，使用reinterpret_cast的代码很难移植。reinterpret_casts的最普通的用途就是在函数指针类型之间进行转换。
举例如下：


```
#include <iostream>
using namespace std;
class A 
{ 
public: 
    virtual void foo() 
    { 
    } 
}; 
class B 
{ 
public: 
    virtual void foo() 
    { 
    } 
}; 
class C : public A , public B 
{ 
public: 
    virtual void foo() { } 
}; 
void bar1(A *pa) 
{ 
    B *pc = dynamic_cast<B*>(pa); 
} 
void bar2(A *pa) 
{ 
    B *pc = static_cast<B*>(pa); //error
} 
void bar3() 
{ 
    C c; 
    A *pa = &c; 
    B *pb = static_cast<B*>(static_cast<C*>(pa)); 
} 
int main()
{
    return 0;
}
```
A、bar1无法通过编译
B、bar2无法通过编译
C、bar3无法通过编译
D、bar1可以正常运行，但是采用了错误的cast方法
解答：
选B。
dynamic_cast是在运行时遍历继承树，所以，在编译时不会报错。但是因为A和B没啥关系，所以运行时报错(所以A和D都是错误的)。
static_cast：编译器隐式执行的任何类型转换都可由它显示完成。其中对于：（1）基本类型。如可以将int转换为double(编译器会执行隐式转换)，但是不能将int*用它转换到double*（没有此隐式转换）。（2）对于用户自定义类型，如果两个类无关，则会出错（所以B正确），如果存在继承关系，则可以在基类和派生类之间进行任何转型，在编译期间不会出错。所以bar3可以通过编译（C选项是错误的)。
