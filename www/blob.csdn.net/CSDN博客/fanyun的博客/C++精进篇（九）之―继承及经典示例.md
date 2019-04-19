# C++精进篇（九）之―继承及经典示例 - fanyun的博客 - CSDN博客
2016年03月26日 09:20:59[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：7211
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
[](http://blog.csdn.net/fanyun_01/article/details/50985317)
**继承是C++的重要属性：**
**       计算机程序经常使用类之间的继承关系来表示对象之间的分类关系。在继承关系中，有父类和子类，比如动物类Animal和狗类Dog，Animal是父类，Dog是子类。父类也叫基类，子类也叫派生类，父类子类是相对的，一个类B可能是类A的子类，是类C的父类。       之所以叫继承是因为，子类继承了父类的属性和行为，父类有的属性和行为，子类都有。但子类可以增加子类特有的属性和行为，某些父类有的行为，子类的实现方式可能与父类也不完全一样。       使用继承一方面可以复用代码，公共的属性和行为可以放到父类中，而子类只需要关注子类特有的就可以了，另一方面，不同子类的对象可以更为方便的被统一处理。**
      在C++中有：
        公有继承(public)、私有继承(private)、保护继承(protected)是常用的三种继承方式。
1. 公有继承(public)
公有继承的特点是基类的公有成员和保护成员作为派生类的成员时，它们都保持原有的状态，而基类的私有成员仍然是私有的，不能被这个派生类的子类所访问。
2. 私有继承(private)
私有继承的特点是基类的公有成员和保护成员都作为派生类的私有成员，并且不能被这个派生类的子类所访问。
3. 保护继承(protected)
保护继承的特点是基类的所有公有成员和保护成员都成为派生类的保护成员，并且只能被它的派生类成员函数或友元访问，基类的私有成员仍然是私有的。
下面列出三种不同的继承方式的基类特性和派生类特性。
||public|protected|private|
|----|----|----|----|
|共有继承|public|protected|不可见|
|私有继承|private|private|不可见|
|保护继承|protected|protected|不可见|
在上图中：
1）基类成员对派生类都是：共有和保护的成员是可见的，私有的的成员是不可见的。
         2）基类成员对派生类的对象来说：要看基类的成员在派生类中变成了什么类型的成员。如：私有继承时，基类的共有成员和私有成员都变成了派生类中的私有成员，因此对于派生类中的对象来说基类的共有成员和私有成员就是不可见的。
     为了进一步理解三种不同的继承方式在其成员的可见性方面的区别，下面从三种不同角度进行讨论。
对于公有继承方式
(1) 基类成员对其对象的可见性：
公有成员可见，其他不可见。这里保护成员同于私有成员。
(2) 基类成员对派生类的可见性：
公有成员和保护成员可见，而私有成员不可见。这里保护成员同于公有成员。
(3) 基类成员对派生类对象的可见性：
公有成员可见，其他成员不可见。
所以，在公有继承时，派生类的对象可以访问基类中的公有成员；派生类的成员函数可以访问基类中的公有成员和保护成员。这里，一定要区分清楚派生类的对象和派生类中的成员函数对基类的访问是不同的。
对于私有继承方式
(1) 基类成员对其对象的可见性：
公有成员可见，其他成员不可见。
(2) 基类成员对派生类的可见性：
公有成员和保护成员是可见的，而私有成员是不可见的。
(3) 基类成员对派生类对象的可见性：
所有成员都是不可见的。
所以，在私有继承时，基类的成员只能由直接派生类访问，而无法再往下继承。
对于保护继承方式
这种继承方式与私有继承方式的情况相同。两者的区别仅在于对派生类的成员而言，对基类成员有不同的可见性。
上述所说的可见性也就是可访问性。
关于可访问性还有另的一种说法。这种规则中，称派生类的对象对基类访问为水平访问，称派生类的派生类对基类的访问为垂直访问。
一个私有的或保护的派生类不是子类，因为非公共的派生类不能做基类能做的所有的事，就是指在公开场合，但是在类内部可以的
一、引言
在C++中，类是提供封装的逻辑单位，类的每一个对象都包含有描述其自身状态的数据集合，并且通过接收特定的消息来处理这个数据集合。如果程序设计人员能够通过增加、修改或替换指定类的部分内容的方法对该类进行剪裁，就可以适应不同的应用，从而在很大程度上增强了数据封装的价值，而接下来要讨论的继承就完全可以实现这种操作。
二、与继承有关的基本概念
继承是一个进程，通过继承，一个对象可以获得另一个对象的属性（包括函数），并可向其中加入属于自己的一些特征。作为C++语言的一种重要机制，用继承的方法可以自动为一个类提供来自另一个类的操作和数据结构，进而使程序设计人员在一个一般的类的基础上很快建立一个新的类，而不必从零开始设计每个类。
当一个类被其他的类继承时，被继承的类称为基类（可不是鸡肋^_^），又称为父类。
继承其他类属性的类称为派生类，又称为子类。
一般情况下，继承的进程起源于一个基类的定义，基类定义了其所有派生类的公有属性。从本质上讲，基类具有同一类集合中的公共属性，派生类继承了这些属性，并且增加了自己特有的属性。从任何已存在的类继承的实质就是建造新的派生类。
三、单重继承、多重继承与继承链
从一个基类派生的继承称为单继承，换句话说，派生类只有一个直接基类。单继承声明语句的常用格式为：
class 派生类名: 访问控制关键字 基类名
{
  数据成员和成员函数声明
};
与此相对地，从多个基类派生的继承称为多继承或多重继承，也就是说，一个派生类有多个直接基类。在某些面向对象的语言（如Java)中不支持类间的多重继承而只支持单重继承，即一个类至多只能有一个直接父类，因此实现类似的功能需要借助接口等其他机制。而在C++中提供了多重继承的语法支持，使得问题变得简单了许多。多重继承声明语句的常用格式为：
class 派生类名: 访问控制关键字 基类名1, 访问控制关键字 基类名2，...
{
  数据成员和成员函数声明
};
除了多重继承之外，一个派生类继承多个基类还有一种方法，就是把派生类作为基类再次供别的类继承，产生多层次的继承关系。例如类A派生类B，类B派生类C，则称类A是类B的直接基类，类B是类C的直接基类，类A是类C的间接基类。类的层次结构也叫做继承链。还是上面的例子，当建立类C的对象时，类A 的构造函数最先被调用，接下来被调用的是类B的构造函数，最后是类C的构造函数。析构函数的调用顺序正好相反。当一个派生类继承有层次的类时，继承链上的每个派生类必须将它需要的变量传递给它的基类。
四、公有派生和私有派生
在继承声明语句中，访问控制关键字用于说明在基类定义中所声明的成员和成员函数能够在多大范围内被派生类所访问。访问控制关键字可为public, private或protected。如果访问控制关键字为public，则称派生类从基类公有继承，也称公有派生。如果访问控制关键字为 private，则称派生类从基类私有继承，也称私有派生。现在笔者将公有继承和私有继承的具体区别列表如下。
通过上表，我们可以将两种派生的特点总结如下：
|基类成员|基类private成员|基类public成员| | |
|----|----|----|----|----|
|派生方式|private|public|private|public|
|派生类成员|不可见|不可见|可见|可见|
|外部函数|不可见|不可见|不可见|可见|
（1）无论哪种派生方式，基类中的private成员在派生类中都是不可见的。也就是说，基类中的private成员不允许外部函数或派生类中的任何成员访问。
（2）public派生与private派生的不同点在于基类中的public成员在派生类中的访问属性：
public派生时，基类中的public成员相当于派生类中的public成员。
private派生时，基类中的public成员相当于派生类中的private成员。
因此，private派生确保基类中的方法只能被派生类的对象的方法间接使用，而不能被外部使用。public派生使派生类对象与外部都可以直接使用基类中的方法，除非这些方法已经被重新定义。
五、保护成员与保护派生
如果想做到基类成员只由有派生血缘关系的成员访问，而不被无血缘关系的对象成员访问，无论用公有派生还是私有派生都无法做到。因为基类成员中的私有成员是别的类（包括派生类）成员不能访问的，而基类中的公有成员在public派生时，不仅可以由派生类对象成员访问，也可以由外部函数访问；而在 private派生时，基类中的公有成员虽然允许派生类对象中的成员访问，不允许外部访问，可是再派生出下一级时，由于基类的所有成员已经被私有化，其它类成员也不可再访问。实现只许有派生血缘关系的对象成员访问的方法，是在基类中使用具有另一种访问属性的成员——protected成员。
protected成员是一种血缘关系内外有别的成员。它对派生对象而言，是公有成员，可以访问；对血缘关系外部而言，与私有成员一样被隐藏。
此外，除了允许使用private与public两种派生方式之外，C++还允许使用protected派生方式。现在将三种访问属性不同的成员经三种派生后在派生类中访问属性的变化情况总结如下表，是对上一表格的增进和补充。
|派生方式|基类的public成员|基类的protected成员|基类的private成员|派生方式引起的访问属性变化概括|
|----|----|----|----|----|
|private派生|变为private成员|变为private成员|不可见|基类中的非私有成员都成为派生类中的私有成员|
|protected派生|变为protected成员|变为private成员|不可见|基类中的非私有成员在派生类中的访问属性都降一级|
|public派生|仍为public成员|仍为protected成员|不可见|基类中的非私有成员在派生类中的访问属性保持不变|
需要注意的是，基类的private成员无论经过何种派生，在派生类中都是不可见的。
六、友元类和友元函数
（1）友元函数
通常，类的私有成员只能由本类的成员访问，外部函数只能访问类的成员函数，再由成员函数访问类的私有成员。但是，如果在某个类定义中用friend 声明了一个外部函数（也许是其他类的一个成员）后，这个外部函数便可以例外地访问该类的任何私有成员。用friend声明了的外部函数称为这个类的友元函数。
当友元函数是另一个类的成员函数时，应当注意以下几点：
A：友元函数作为一个类的成员函数时，除应当在它所在的类定义中声明之外，还应当在另一个类中声明它的友元关系，声明语句的格式为：
friend 函数类型 函数所在类名：：函数名（参数列表）；
B：友元函数在引用本类对象的私有成员时无需本类对象的引用参数，但在引用生命它是友元的类的对象中的私有成员时必须有友元类对象的引用参数。
C：一个类的成员函数作另一个类的友元函数时，必须先定义，而不是仅仅声明它。
使用友元函数直接访问对象的私有成员，可以免去再调用类的成员函数所需的开销。同时，友元函数作为类的一个接口，对已经设计好的类，只要增加一条声明语句，便可以使用外部函数来补充它的功能，或架起不同类对象之间联系的桥梁。然而，它同时也破坏了对象封装与信息隐藏，使用时需要谨慎小心。
（2）友元类
也可以把一个类而不仅仅是一个函数声明为另一个类的友元类。这时，只需先声明它而不一定需要先定义。
应当注意，友元关系是单向的，并且只在两个类之间有效。即使类X是类Y的友元，类Y是否是类X的友元也要看类X中是否有相应的声明。即友元关系不具有交换性。若类X是类Y的友元，类Y是类Z的友元，也不一定就说明类X是类Z的友元，即友元关系也不具有传递性。
当一个类要和另一个类协同工作时，使一个类成为另一个类的友元类是很有用的。这时友元类中的每一个成员函数都成为了对方的友元函数。
**c++继承经典例子**
#include <iostream.h>
class Base
{
private:
        int b_number;
public:
        Base( ){}
        Base(int i) : b_number (i) { }
        int get_number( ) {return b_number;}
        void print( ) {cout << b_number << endl;}        
};
class Derived : public Base
{
private:
        int d_number;
public:
// constructor, initializer used to initialize the base part of a Derived object.
        Derived( int i, int j ) : Base(i), d_number(j) { };        
        // a new member function that overrides the print( ) function in Base
        void print( ) 
        {
                cout << get_number( ) << " ";        
                // access number through get_number( )
                cout << d_number << endl;
        }
};
int main( )
{
        Base a(2);
        Derived b(3, 4);
        cout << "a is ";
        a.print( );                // print( ) in Base
        cout << "b is ";
        b.print( );                // print( ) in Derived
        cout << "base part of b is "; 
        b.Base::print( );                // print( ) in Base
        return 0;
}
**没有虚析构函数，继承类没有析构**
//Example:  non- virtual destructors for dynamically allocated objects. 
#include <iostream.h>
#include <string.h>
class Thing
{ public:
virtual void what_Am_I( ) {cout << "I am a Thing./n";}
~Thing(){cout<<"Thing destructor"<<endl;}
};
class Animal : public Thing
{  
public:
virtual void what_Am_I( ) {cout << "I am an Animal./n";}
~Animal(){cout<<"Animal destructor"<<endl;}
};
void main( )
{
   Thing *t =new Thing;      
   Animal*x = new Animal;
   Thing* array[2];
   array[0] = t;                                // base pointer
   array[1] = x;               
    for (int i=0; i<2; i++)  array->what_Am_I( ) ;
   delete array[0];
   delete array[1];
   return ;
}
**纯虚函数，多态#include <iostream.h>**
#include <math.h>
class Point
{
private:
        double x;
        double y;
public:
        Point(double i, double j) : x(i), y(j) { } 
        void print( ) const
        { cout << "(" << x << ", " << y << ")"; }
};
class Figure
{
private:
        Point center;
public:
        Figure (double i = 0, double j = 0) : center(i, j) { }         
Point& location( )
{
return center;
}                  // return an lvalue
   void move(Point p)
{
center = p;
draw( );
}
        virtual void draw( ) = 0; // draw the figure
        virtual void rotate(double) = 0; 
// rotate the figure by an angle                
};
class Circle : public Figure
{
private:
        double radius;
public:
        Circle(double i = 0, double j = 0, double r = 0) : Figure(i, j), radius(r) { }
        void draw( )
        {
                cout << "A circle with center ";
                location( ).print( );
                cout << " and radius " << radius << endl;
        }
        void rotate(double)
        {
                cout << "no effect./n";
        }        // must be defined
};
class Square : public Figure
{
private:
        double side;        // length of the side
        double angle;        // the angle between a side and the x-axis
public:
        Square(double i = 0, double j = 0, double d = 0, double a = 0)        : Figure(i, j), side(d), angle(a) { }
   void draw( )
        {
                cout << "A square with center ";
                location( ).print( );
                cout << " side length " << side << "./n"  
                << "The angle between one side and the X-axis is " << angle << endl;
        }
        void rotate(double a)
        {
               angle += a;
                cout << "The angle between one side and the X-axis is " << angle << endl;
        }
        void vertices( )
        {
                cout << "The vertices of the square are:/n";
                // calculate coordinates of the vertices of the square
          }
};
int main( )
{
        Circle c(1, 2, 3);
        Square s(4, 5, 6);
   Figure *f = &c, &g = s;
        f -> draw( );
        f -> move(Point(2, 2));
        g.draw( );
          g.rotate(1);
s.vertices( );
// Cannot use g here since vertices( ) is not a member of Figure.
        return 0;
}
////////////////////////////////////////////////////////////////////
#include <iostream.h>
#include <string.h>
class Thing
{ 
public:
virtual void what_Am_I( ) {cout << "I am a Thing./n";}
~Thing(){cout<<"Thing destructor"<<endl;}
};
class Animal : public Thing
{ 
public:
virtual void what_Am_I( ) {cout << "I am an Animal./n";}
~Animal(){cout<<"Animal destructor"<<endl;}
};
void main( )
{
   Thing t ; 
        Animal x ;
   Thing* array[2];
   array[0] = &t;                        // base pointer
   array[1] = &x;        
          for (int i=0; i<2; i++)  array->what_Am_I( ) ;
   return ;
}
**多继承**
#include <iostream.h>
class A
{
private:
        int a;
public:
        A(int i) : a(i) { }
        virtual void print( )        {cout << a << endl;}
        int get_a( ) {return a;}
};
class B
{
private:
        int b;
public:
        B(int j) : b(j) { }
        void print( )        {cout << b << endl;}
        int get_b( ) {return b;}
};
class C : public A, public B
{
        int c;
public:
        C(int i, int j, int k) : A(i), B(j), c(k) { }
        void print( )        {A::print( ); B::print( );}
        // use print( ) with scope resolution
        void get_ab( )        {cout << get_a( ) << " " << get_b( ) << endl;}
        // use get_a( ) and get_b( ) without scope resolution
};
int main( )
{
        C x(5, 8, 10);
        A* ap = &x;
        B* bp = &x;
        ap -> print( );                // use C::print( );
        bp -> print( );                // use B::print( );
//        bp -> A::print( );                // as if x is inherited from B only,
                                                // cannot access A::print( );
        x.A::print( );                // use A::print( );
        x.get_ab( );
        return 0;
}
**共同基类的多继承**
#include <iostream.h>
class R
{int r;
public:
        R(int anInt){ r = anInt;};
       printOn(){ cout<<"r="<<r<<endl;} ; };
class A : public R
{
int a;
public:
        A(int int1,int int2):R(int2){ a = int1;};};
class B : public R
{
int b;
public:
        B(int int1,int int2):R(int2){ b = int1;};};
class C : public A, public B
{
int c;
public:
C(int int1,int int2, int int3):A(int2,int3), B(int2,int3){ c = int1;}
};
int main( )
{    
  int i;
        R rr(10);      
A aa(20,30);      
B bb (40,50);
        C cc(5, 7, 9);
        rr.printOn();    
aa.printOn();                  //inherits R printOn  
bb.printOn();                   //inherits R printOn
        //cc.printOn();                  //would give error
        return 0;}
**虚基类**
#include <iostream.h>
class R
{ int r;
public:
        R (int x = 0) : r(x) { }   // constructor in R
        void f( ){ cout<<"r="<<r<<endl;}     
        void printOn(){cout<<"printOn R="<<r<<endl;}
};
class A : public virtual R
{  int a;
public:
        A (int x, int y) : R(x), a(y)  { } // constructor in A
        void f( ){ cout<<"a="<<a<<endl;R::f();}
};
class B : public virtual R
{int b;
public:
        B(int x, int z) : R(x), b(z) { }// constructor in B
        void f( ){ cout<<"b="<<b<<endl;R::f();}
};
class C : public A, public B
{ int c;
public:
// constructor in C, which constructs an R object first
C(int x, int y, int z, int w) : R(x), A(x, y), B(x, z), c(w) { }
void f( ){ cout<<"c="<<c<<endl;A::f(); B::f();}
};
void main()
{  R rr(1000);
   A aa(2222,444);
   B bb(3333,111);
   C cc(1212,345,123,45);
   cc.printOn();     //uses R printOn but only 1 R..no ambiguity
   cc.f();                // shows multiple call of the R::f()
}
////////////////////////////////////////
#include <iostream.h>
class R
{ int r;
public:
        R (int x = 0) : r(x) { }   // constructor in R
        void f( ){ cout<<"r="<<r<<endl;}
};
class A : virtual public R
{ int a ;
protected:
        void fA( ){cout<<"a="<<a<<endl;};
public:
        A (int x, int y) : R(x), a(y)  { } // constructor in A
        void f( ) {fA( ); R::f( );}
};
class B : virtual public R
{  int b;
protected:
        void fB( ){cout<<"b="<<b<<endl;};
public:
        B (int x, int y) : R(x), b(y)  { } // constructor in A
        void f( ) {fB( ); R::f( );}
};
class C : public A, public B
{ int c;
protected:
        void fC( ){ cout<<"c="<<c<<endl;};        
public:
C(int x, int y, int z, int w) : R(x), A(x, y), B(x, z), c(w) { }
void f( )
        {  
                   R::f( );                    // acts on R stuff only
                A::fA( );            //acts on A stuff only
                B::fB( );                   // acts on B stuff only
                fC( );                  // acts on C stuff only
        }
};
void main()
{  R rr(1000);
   A aa(2222,444);
   B bb(3333,111);
   C cc(1212,345,123,45);
   cc.f();
}
**私有继承**
// Access levels
#include <iostream.h>
class Base
{
private:
        int priv;
protected:
        int prot;
        int get_priv( ) {return priv;}
public:
        int publ;
        Base( );
        Base(int a, int b, int c) : priv(a), prot(b), publ(c) { }
        int get_prot( ) {return prot;}
        int get_publ( ) {return publ;}
};
class Derived1 : private Base        // private inheritance
{
public:
        Derived1 (int a, int b, int c) : Base(a, b, c) { }
        int get1_priv( ) {return get_priv( );}
        // priv not accessible directly
        int get1_prot( ) {return prot;}
      int get1_publ( ) {return publ;}
};
class Leaf1 : public Derived1
{
public:
        Leaf1(int a, int b, int c) : Derived1(a, b, c) { }
        void print( )
        {
                cout << "Leaf1 members: " << get1_priv( ) << " "
//                        << get_priv( )        // not accessible
                        << get1_prot( ) << " "
//                        << get_prot( )         // not accessible
//                        << publ         // not accessible
                        << get1_publ( ) << endl;
        }  // data members not accessible.  get_ functions in Base not accessible
};
class Derived2 : protected Base // protected inheritance
{
public:
        Derived2 (int a, int b, int c) : Base(a, b, c) { }
};
class Leaf2 : public Derived2
{
public:
        Leaf2(int a, int b, int c) : Derived2(a, b, c) { }
        void print( )
        {
                cout << "Leaf2 members: " << get_priv( ) << " "
//                        << priv                 // not accessible
                        << prot << " "
                        << publ << endl;
        }  // public and protected data members accessible.  get_ functions in Base accessible. 
};
class Derived3 : public Base  // public inheritance
{
public:
        Derived3 (int a, int b, int c) : Base(a, b, c) { }
};
class Leaf3 : public Derived3
{
public:
        Leaf3(int a, int b, int c) : Derived3(a, b, c) { }
        void print( )
        {
                cout << "Leaf3 members: " << get_priv( ) << " "
                        << prot << " "
                        << publ << endl;
        }  // public and protected data members accessible.  get_ functions in Base accessible
};
int main( )
{
        Derived1 d1(1, 2, 3);
        Derived2 d2(4, 5, 6);
        Derived3 d3(7, 8, 9);
//        cout << d1.publ;                // not accessible
//        cout << d1.get_priv( );        // not accessible
//        cout << d2.publ;                // not accessible
//        cout << d2.get_priv( );        // not accessible
        cout << d3.publ;                // OK
        cout << d3.get_prot( );        // OK
        Leaf1 lf1(1, 2, 3);
        Leaf2 lf2(4, 5, 6);
        Leaf3 lf3(7, 8, 9);
//         cout << lf1.publ << endl;                    // not accessible
//         cout << lf2.publ << endl;                // not accessible
        cout << lf3.publ << endl;                 // OK
        return 0;
}
**多级继承**
// Point-Circle-Cylinder
#include <iostream.h>
// THE POINT CLASS
class Point
{
friend ostream & operator<<(ostream &,Point &);
public:
//  constructor
        Point (double xval =0, double yval=0 )
        { x=xval; y=yval;};  
protected:       // accessed by derived class
        double  x;
        double  y;
};
ostream & operator << (ostream & os,
                              Point &  apoint)
{
cout <<" Point:X:Y: "<<apoint.x << "," 
                      << apoint.y<< "/n";
  return os;  
}
//The Circle class  inherits from class Point
class Circle : public Point
{
friend ostream & operator<<(ostream &,Circle&);
public:
Circle (double r=0,double xval=0,double yval=0) 
                             :Point(xval,yval), radius(r)
{ 
//radius = r;
}
double area()
{ 
return (3.14159* radius *radius);
}
protected:
  double radius;
};
//note casting circle to point
ostream & operator <<(ostream & os, Circle & aCircle)
{
cout<< "Circle:radius:" << aCircle.radius;
os<< aCircle.x << "/n"; 
os<< aCircle.y << "/n";        
return os;      
}
// THE CYLINDER CLASS
class  Cylinder  : public Circle
{
friend ostream & operator << (ostream & ,Cylinder &);
public:
Cylinder (double hv=0,double rv=0, 
                      double xv=0,double yv=0 )
                           : Circle( xv,yv,rv)
{
height = hv;
}        
double  area ( );
protected:     // may have derived classes
        double  height;
};
double Cylinder :: area ( )
{ // Note that cylinder area uses Circle area
return  2.0* Circle::area() + 2.0*3.14159* radius*height;
}
ostream & operator << (ostream & os,
                        Cylinder & acylinder)
{ 
cout << "cylinder dimensions: ";
  cout << "x: " <<acylinder.x;
  cout << "  y: " <<acylinder.y ;
  cout << "  radius: " <<acylinder.radius ;
  cout << "  height: " <<acylinder.height 
                        << endl;
  return os; 
}
int main(void)
{
Point p(2,3);
Circle c(7,6,5);
Cylinder cyl(10,11,12,13);
cout << p;
cout << c;
cout << "area of cirle:" << c.area() << endl;
cout<< cyl;
cout<<"area of cylinder:"<< cyl.area()<<endl ;
cout<<"area of cylinder base is "  
                 << cyl.Circle::area() << endl;
return 0;
}
**protected 访问控制属性在继承的意义**
//Example of treating derived class object as base class objects. Point------Circle
#include <iostream.h>
// THE POINT CLASS
class Point
{ 
friend ostream & operator<<(ostream &,Circle&);
public:
Point (double xval =0, double yval=0 ) { x=xval; y=yval;};  
public:
void print()
{
cout <<" Point:X:Y: "<<x << "," <<y<< "/n";
}
protected:       // accessed by derived class
double  x;    double  y;
};
ostream & operator << (ostream & os, Point &  apoint)
{
cout <<" Point:X:Y: "<<apoint.x << ","<< apoint.y<< "/n";
  return os;  
}
//The Circle class  inherits from class Point
class Circle : public Point
{
friend ostream & operator<<(ostream &,Circle&);
public:
Circle (double r=0,double xval=0,double yval=0):Point(xval,yval)
{ radius = r;};
void print()
{
cout<< "Circle:radius:" <<radius<<endl;
cout <<" Point:X:Y: "<<x << "," <<y<< "/n";
}
double area()
{ return (3.14159* radius *radius);};
protected:
double radius;
};
//note casting circle to point
ostream & operator <<(ostream & os, Circle & aCircle)
{
cout<< "Circle:radius:" << aCircle.radius;
cout<< (Point) aCircle << "/n";           
return os;      
}
//We will look at a few main programs based on previous class definitions. Casting and assignments
void main (void )
{
Point p(2,3);         cout <<"Point P=  "<< p;
Point pp(0,0);       cout <<"Point PP=  "<< pp;
Circle c(7,6,5);     cout <<"Circle c=  "<< c;        //radius =7
pp = p;             cout <<"Point PP=  "<< pp;    //built in assign =
// a circle is a member of the point class so assign a circle to a point.
pp = c;           //legal; also assignment O.K.
cout <<"Point PP=  "<< pp;
pp= (Point) c;    // but better  use the cast
cout <<"Point PP=  "<< pp;  //note we get only the point part of the Circle
//c = (Circle) pp;   //  illegal Cannot convert 'class Point' to 'class Circle'
//c=pp;                 //illegal assignment not defined
Point*  p;
p = &c;
P->print();    //call base class print
((Circle*)p)->print();
Point& r = c;
r.print();
((Circle&)r).print();
}
**类的兼容性规则**
#include <iostream.h>
class Base
{ 
public:  
void func( ) 
{cout << "Base class function./n";} 
};
class Derived : public Base
{ 
public:  
void func( ) 
{cout << "Derived class function./n";}
};
void foo(Base b)
{ b.func( ); }
int main( )
{
   Derived d;
   Base b;
   Base * p = &d;
   Base& br = d;
   b = d;
   b.func( );
   d.func( );
   p -> func( );
   foo(d);
   br.func( );
   return 0;
}
**虚析构函数，防止内存泄露**
#include <iostream.h>
#include <string.h>
class Base
{
protected:
        int id;
        char * name;
public:
        // default constructor
        Base(int a = 0, char * s = "") : id(a)
        {
                if (!s) 
{ 
name = NULL; 
}
                else
                {
                        name = new char[strlen(s) + 1];
                        strcpy(name, s);
                }
                cout << "base default constructor/n";
        }
                // copy constructor
        Base(const Base& b) : id(b.id)
        {
                if (!b.name) { name = NULL; }
                else
                { 
                        name = new char[strlen(b.name) + 1];
        strcpy(name, b.name);
}
                    cout << "base copy constructor/n";
        } 
        // destructor
      ~Base( ) 
        {
            if( name != NULL )        delete [ ] name; 
                cout << "base destructor/n";
        }
        const Base& operator= (const Base& b);                
friend ostream& operator << (ostream&, const Base&);
};
const Base& Base:perator= (const Base& b)
{
        if (this != &b)                        // Check if an object is assigned to itself.
        {
             id = b.id;
                delete [ ] name;                //  Destroy the old object.
                if (!b.name) { name = NULL; }
                else
                {
        name = new char[strlen(b.name) + 1];
        strcpy(name, b.name);
                }
        }
            cout << "base assignment operator/n";
        return *this;
}
ostream& operator << (ostream& out, const Base& b)
{
        out << "Base member id = " << b.id << endl;
        out << "Base member name = " << b.name << endl;
        return out;
}
class Derived : public Base
{
private:
        float f;
        char * label;
public:
        // default constructor
        Derived(int a = 0, char * s = "", float x = 0, char * t = "") : Base(a, s), f(x)
        {
                if (!t) { label = NULL; }
                else
                {
        label = new char [strlen(t) + 1]; 
        strcpy(label, t);
}
                cout << "derived default constructor/n";
        }
        // copy constructor
        Derived(const Derived& d) : Base(d), f(d.f)
                // d used as an instance of Base
        {
                if(!d.label) { label = NULL; }
                else
                {
                        label = new char [strlen(d.label) + 1];
        strcpy(label, d.label);
}
                cout << "derived copy constructor/n";
        }
        // destructor
        ~Derived( )          
        {
                delete [ ] label; 
                cout << "derived destructor/n";
        }
        const Derived& operator= (const Derived& d);
friend ostream& operator << (ostream&, const Derived&);
};
const Derived& Derived:perator= (const Derived& d)
{
        if (this != &d)
        {
                delete [ ] label;
                Base:perator=(d);        //  Assign the Base part of d to the Base
// part of the object that calls this operator;
f = d.f;
if (!d.label) { label = NULL; }
else
{
        label = new char [strlen(d.label) + 1];
                        strcpy(label, d.label);
                }
                cout << "derived assignment operator/n";
        }
        return *this;
}
ostream& operator << (ostream& out, const Derived& d)
{
        out << (Base)d;                // Convert d to Base object to output Base members.
        out << "Derived member f = " << d.f << endl;
        out << "Derived member label = " << d.label << endl;
        return out;
}
int main( )
{
        Derived d1;
Derived  d2(d1);
        return 0;
}
 以上示例转自：http://blog.csdn.net/zhaori/article/details/1700356
