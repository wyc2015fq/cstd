# C++精进篇（七）之―操作符重载 - fanyun的博客 - CSDN博客
2016年03月26日 09:16:48[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1680
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
一．运算符重载的含义与定义方式
       C++已有的运算符只适合处理C++的基本数据类型。
       C++允许重新定义已有的运算符（运算符重载），以便它能处理程序员定义类型（类类型）。
       运算符重载就是赋予已有的运算符多重含义。运算符重载与函数重载类似，是它的特殊类型。
       C++通过重新定义运算符，使它能够用于特定类的对象执行特定的功能。
       通过对+，-，*，/运算符的重新定义，使它们可以完成复数、分数等不同类的对象的加、减、乘、除运算操作。增强了C++语言的扩充能力。
       先创建一个运算符函数，一般定义成类的成员函数或友元函数。
二．重载一个运算符原则：
1．不能改变运算符的初始意义。
2．不能改变运算符的参数数目。如重载运算符+时只用一个操作数是错误的。
3．运算符函数不能包括缺省的参数。
4．绝大部分C++运算符都可以重载，除以下的例外：
.   ::   .*  ?
5．除赋值运算符外，其它运算符函数都可以由派生类继承。
6．运算符重载不改变运算符的优先级和结合性，也不改变运算符的语法结构，即单目、双目运算符只能重载为单目、双目运算符。
7．运算符的重载实际上是函数的重载。编译程序对运算符重载的选择，遵循函数重载的选择原则。当遇到不很明显的运算符时，编译程序将去寻找参数匹配的运算符函数。
8．运算符重载可使程序更简洁，使表达式更直观，增强可读性。但使用不宜过多。
9．重载运算符含义必须清楚：
如有一个类Time,它有三个数据成员时、分、秒
class Time{
public:
   Time() {hours=minutes=seconds=0;}
   Time(int h,int m,int s){ hours=h; minutes=m; seconds=s;}
private:
int hours, minutes, seconds;
};
Time t1(8,10,20), t2(9,15,30), t3;
t3=t1+t2;
这里加法（+）运算用于类Time的对象，就是含义不清的。所以不能给类Time定义重载运算符+。
三．运算符重载函数的两种形式：
成员函数形式和友元函数形式，他们都可访问类中的私有成员。
1．重载为类的成员函数
1）X类中重载一元运算符@
返回类型   X：：operator@（）
      { …  }
不指定参数，因为它已带有一个隐含的this参数，对X类的一个对象obj：
       表达式               C++编译器的解释
      @obj                 operator @（obj）
      obj@                 operator @（obj，0）
2）  X类中重载二元运算符@
  返回类型   X：：operator@（参数说明）
      { …  }
由于类的成员函数带有一个this参数，此时只能指定一个参数，对obj对象：
       表达式               C++编译器的解释
[obj1@obj2](mailto:obj1@obj2)            obj1·operator@（obj2）
在多数情况下，将运算符重载为类的成员函数和类的友元函数都是可以的。但成员函数运算符与友元函数运算符也具有各自的一些特点：
(1) 一般情况下，单目运算符最好重载为类的成员函数；双目运算符则最好重载为类的友元函数。
(2) 以下一些双目运算符不能重载为类的友元函数：=、()、[]、->。
(3) 类型转换函数只能定义为一个类的成员函数而不能定义为类的友元函数。
(4) 若一个运算符的操作需要修改对象的状态，选择重载为成员函数较好。
(5) 若运算符所需的操作数（尤其是第一个操作数）希望有隐式类型转换，则只能选用友元函数。
(6) 当运算符函数是一个成员函数时，最左边的操作数（或者只有最左边的操作数）必须是运算符类的一　个类对象（或者是对该类对象的引用）。如果左边的操作数必须是一个不同类的对象，或者是一个内部　类型的对象，该运算符函数必须作为一个友元函数来实现。
(7) 当需要重载运算符具有可交换性时，选择重载为友元函数。
例：给复数运算符重载的四则运算符。复数由实部和虚部构造，定义一个复数类，再在类中重载复数四则运算的运算符。
#include<iostream.h>
class complex
{ 
private: 
     float real,imag;
public:
     complex(float r=0,float i=0);
     complex operator+(complex &c);
     complex operator-(complex &c);
     friend void print(complex &c);
};
complex::complex(float r,float i)
{  
    real=r;  
    imag=i; 
}
complex complex::operator+(complex &c)
{  
    float r=real+c.real;       
    float i=imag+c.imag;      
    return complex(r,i) ;
}
complex complex::operator-(complex &c)
{  
    float r=real-c.real;       
    float i=imag-c.imag;      
    return complex(r, i) ; 
}
void print(complex &c )
{ 
    cout<<'('<<c.real<<','<<c.imag<<')'<<endl;   
}
void main( )
{ 
     complex c1(2.5,3.7), c2(4.2,6.5) ;
     complex c;
     c=c1-c2;             //c=c1·operator-(c2)
     print(c);
     c=c1+c2;             //c=c1·operator+(c2)
     print(c);
 }
该程序中定义一个complex类，定义了2个成员函数作为运算符重载函数。
c1+c2编译程序解释为：c1.operator+(c2)
c1、c2是complex 类的对象。operator+()是运算符+的重载函数。
该运算符重载函数仅有一个参数c2。当重载为成员函数时，双目运算符仅有一个参数。
对单目运算符，重载为成员函数时，不能再显式说明参数。
重载为成员函数时，总是隐含了一个参数，即this指针，它是指向调用该成员函数对象的指针。
在重载运算符函数中，operator +( )中参数用引用传递而不用指针传递。
因为指针传递存在程序的可读性问题。如操作符重载声明为：
complex operator +( complex *c)；
则调用时
    complexc1(2.0,3.0),c2(4.0,-2.0),c3;
      c3=&c1+&c2;   会认为是c1的地址和c2的地址相加
而声明为complex operator +(const complex &c)；
则    c3=c1+c2;
2. 重载为友元函数
    运算符重载函数还可以为友元函数。当重载为友元函数时，没有隐含的参数this指针。这样对双目运算符，友元函数有2个参数，对单目运算符，友元函数有1个参数。但有些运算符不能重载为友元函数，它们是：=、（）、[]、->。
1） X类中重载一元运算符@
  返回类型  operator@（X&obj ）
      { …  }
　只能为友元运算符指定一个参数，对X类的一个对象obj：
       表达式               C++编译器的解释
      @obj                 operator @（obj）
      obj@                 operator @（obj，0）
2） X类中重载二元运算符@
返回类型 operator@(参数说明1,参数说明2)
  {……}]
       两个参数中必须至少有一个是X类类型，设有对象obj1，obj2
       表达式               C++编译器的解释
[obj1@obj2](mailto:obj1@obj2)          operator @（obj1，obj2）
例：用友元函数代替成员函数编上述程序：
#include<iostream.h>
class complex
{ private: 
     float real,imag;
public:
     complex(float r=0,float i=0);
     friend complex operator+(complex &c1,complex &c2);
     friend complex operator-(complex &c1,complex &c2);
     friend void print(complex &c);
};
complex::complex(float r,float i)
{  real=r;  imag=i;  }
complex operator+(complex &c1,complex &c2)
{  float r=c1.real+c2.real;     float i=c1.imag+c2.imag;  
   return complex(r,i) ; }
complex operator-(complex &c1,complex &c2)
{  float r=c1.real-c2.real;       float i=c1.imag-c2.imag;  
   return complex(r, i) ; }
void print(complex &c )
{ cout<<'('<<c.real<<','<<c.imag<<')'<<endl;   }
void main( )
{ complex c1(2.5,3.7), c2(4.2,6.5) ;
 complex c;
 c=c1-c2;             //c=c1·operator-(c2)
 print(c);
 c=c1+c2;             //c=c1·operator+(c2)
 print(c);
 }
双目运算符重载为成员函数时，仅有一个参数，另一个被隐含；
重载为友元函数时，有两个参数，没有隐含参数；
c1+c2编译程序解释为：operator+( c1，c2)
调用如下函数，进行求值，
complex operator +(const complex&c1,consrt complex &c2)
结论1：对二元运算符，将它重载为一个友元函数比重载为一个成员函数要便于使用。作为一个友元函数，二元运算符不要求第一个参数一定为某类的对象。
结论2：对一元运算符，将它重载为一个成员函数最恰当。重载为友员函数也可以。
例：
#include <iostream.h>
class A{
public:
       A(){X=Y=0;}
       A(int i,int j){X=i;Y=j;}
    A(A &p){X=p.X;Y=p.Y;}
    A& operator =(A &p);
       int getX(){return X;}
       int getY(){return Y;}
private:
       int X,Y;
};
A& A::operator =(A &p)
{
       X=p.X;
       Y=p.Y;
       cout<<"Assignment operator called.\n";
       return *this;
}
void main()
{
       A a(7,8);
       A b;
       b=a;
       cout<<b.getX()<<","<<b.getY()<<endl;
}
Assignment operator called.
7，8
该程序中，在类A中定义了一个赋值运算符函数，被定义为成员函数。
b=a解释为： b.operator=(a)
调用下列函数：A& A::operator=(A &p)完成赋值操作。
 我补充点内容：
在c++中可以这样调用一个 []操作符：
下面的代码中我重载了[]，
class temp{ 
public: 
 const char& operator [] (intindex) const
    {
            return buffer[index];
    }
　　char* buffer  
} C++ 操作符重载
 那么我可以通过temp().operator[](4);来调用[]。
由此观之C++的操作符重载其实就是函数的调用。
还有操作符重载中返回左值和右值是按照等号来区分的，可以这么理解左值就是引用（要被赋值），右值就是临时变量。
 类成员 非类成员友元 左值 右值
