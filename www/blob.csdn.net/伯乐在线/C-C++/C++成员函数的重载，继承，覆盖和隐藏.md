# C++成员函数的重载，继承，覆盖和隐藏 - 文章 - 伯乐在线
原文出处： [红黑联盟，2013-05-11](http://www.2cto.com/kf/201305/209880.html)
**一、 C++成员函数的重载**
C++中的成员函数有四种，分别是普通成员函数，virtual虚函数，const成员函数。
(1) void func(int a);
(2) virtual void func(int a);
(3) void func(int a) const;
如果在一个类中，声明这四种函数，哪是重复定义？哪些是重载？
其中(1)(2)是重复定义，故编译不能通过，而(3)与(1)(2)是不同类型的函数，是重载。
成员函数被重载的特征是：
(1)具有相同的作用域(即同一个类定义中)；
(2)函数名字相同
(3)参数类型，顺序 或 数目不同(包括const参数和非const函数)
(4)virtual关键字可有可无。
从成员函数的重载特征中，可以知道(1)(2)是重复定义。那么(3)为什么和(1)(2)不同呢？
因为类中的函数，都会自动添加一个自身类指针this，所以
void func(int a) ==== void func(Base * this, int a)
virtual func(int a) ==== virtual func(Base *this, int a)
void func(int a)const === void func(const Base *this, int a) const
所以(3)可以与(1)(2)发生重载，因为参数有一个const。
**二 、C++成员函数的继承**


```
#include <iostream> 
using namespace std; 
 
class Base { 
public: 
    void f(int a){ 
        cout << "Base::f(int a)" << endl; 
    } 
    virtual void g(int a) { 
        cout << "virtual Base::g(int a)" << endl; 
    } 
}; 
 
class Derived : public Base 
{ 
public: 
    void h(int a) { 
        cout << "Derivd::h(int a)" << endl; 
    } 
}; 
 
 
int main() 
{ 
    Base b; 
    b.f(3); 
    b.g(4); 
 
 
    Derived d; 
    d.f(3); 
    d.g(4); 
    d.h(3); 
} 
#include <iostream>
using namespace std;
class Base {
public:
 void f(int a){
  cout << "Base::f(int a)" << endl;
 }
 virtual void g(int a) {
  cout << "virtual Base::g(int a)" << endl;
 }
};
class Derived : public Base
{
public:
 void h(int a) {
  cout << "Derivd::h(int a)" << endl;
 }
};
int main()
{
 Base b;
 b.f(3);
 b.g(4);
 Derived d;
 d.f(3);
 d.g(4);
 d.h(3);
}
```
Base b的对象模型：
![](http://img.blog.csdn.net/20130510154022142)
Derived d的对象模型：
![](http://img.blog.csdn.net/20130510154041095)
则在子类Derived d中继承了父类中的virtual void g(int a) ; void f(int a);
运行结果为：
![](http://img.blog.csdn.net/20130510154226092)
**三、C++成员函数的覆盖**
覆盖是指派生类重新实现(或者改写)了基类的成员函数，其特征是：
(1)不同的作用域(非别位于派生类和基类中)；
(2)函数名称相同
(3)参数列表完全相同；
(4)基类函数必须是虚函数。
从(4)中我们得知覆盖只是针对虚函数的。


```
#include <iostream> 
using namespace std; 
 
class Base { 
public: 
    void f(int a){ 
        cout << "Base::f(int a)" << endl; 
    } 
    virtual void g(int a) { 
        cout << "virtual Base::g(int a)" << endl; 
    } 
}; 
 
class Derived : public Base 
{ 
public: 
    void h(int a) { 
        cout << "Derivd::h(int a)" << endl; 
    } 
    virtual void g(int a) { 
        cout << "virtual Derived::g(int a)" << endl; 
    } 
}; 
 
int main() 
{ 
    Base b; 
    b.f(3); 
    b.g(4); 
 
    Derived d; 
    d.f(3); 
    d.g(4); 
    d.h(3); 
} 
#include <iostream>
using namespace std;
class Base {
public:
 void f(int a){
  cout << "Base::f(int a)" << endl;
 }
 virtual void g(int a) {
  cout << "virtual Base::g(int a)" << endl;
 }
};
class Derived : public Base
{
public:
 void h(int a) {
  cout << "Derivd::h(int a)" << endl;
 }
 virtual void g(int a) {
  cout << "virtual Derived::g(int a)" << endl;
 }
};
int main()
{
 Base b;
 b.f(3);
 b.g(4);
 Derived d;
 d.f(3);
 d.g(4);
 d.h(3);
}
```
Derived d对象模型如下：
![](http://img.blog.csdn.net/20130510154701725)
其中Derived中重新定义了基类的虚成员函数virtual void g(int a)；
![](http://img.blog.csdn.net/20130510154844863)
**四、C++成员函数的隐藏**
隐藏是指派生类的成员函数遮蔽了与其同名的基类成员函数，具体规则如下：
(1) 派生类的函数与基类的函数同名，但是参数列表有所差异。此时，不论有无virtual关键字，基类的函数在派生类中将被隐藏。(注意别与重载混合)
(2)派生类的函数与基类的函数同名，参数列表也相同，但是基类函数没有virtual关键字。此时，基类的函数在派生类中将被吟唱。(注意别与覆盖混合)
判断下面哪些函数是覆盖，哪些函数是隐藏？


```
#include <iostream> 
using namespace std; 
 
class Base { 
public: 
    virtual void f(float x) { 
        cout << "virtual Base::f(float) " << x << endl; 
    } 
    void g(float x) { 
        cout << "Base::g(float) " << x << endl; 
    } 
    void h(float x) { 
        cout << "Base::h(float) " << x << endl; 
    } 
}; 
 
class Derived : public Base{ 
public: 
    virtual void f(float x) { 
        cout << "virtual Derived::f(float) " << x << endl; 
    } 
    void g(int x) { 
        cout << "Derived::g(int) " << x << endl; 
    } 
    void h(float x) { 
        cout << "Derived::h(float) " << x << endl; 
    } 
}; 
 
int main(void) 
{ 
    Derived d; 
    Base *pb = &d; 
    Derived *pd = &d; 
 
    pb->f(3.14f); 
    pd->f(3.14f); 
 
    pb->g(3.14f); 
    pd->g(3.14f); 
 
    pb->h(3.14f); 
    pd->h(3.14f); 
} 
#include <iostream>
using namespace std;
class Base {
public:
 virtual void f(float x) {
  cout << "virtual Base::f(float) " << x << endl;
 }
 void g(float x) {
  cout << "Base::g(float) " << x << endl;
 }
 void h(float x) {
  cout << "Base::h(float) " << x << endl;
 }
};
class Derived : public Base{
public:
 virtual void f(float x) {
  cout << "virtual Derived::f(float) " << x << endl;
 }
 void g(int x) {
  cout << "Derived::g(int) " << x << endl;
 }
 void h(float x) {
  cout << "Derived::h(float) " << x << endl;
 }
};
int main(void)
{
 Derived d;
 Base *pb = &d;
 Derived *pd = &d;
 pb->f(3.14f);
 pd->f(3.14f);
 pb->g(3.14f);
 pd->g(3.14f);
 pb->h(3.14f);
 pd->h(3.14f);
}
```
其中子类Derived中 vitual void f(float x)  是覆盖，而void g(int x) 和void h(float x)都是隐藏。
运行结果：
![](http://img.blog.csdn.net/20130510160059072)
再看一个例子：


```
#include <iostream> 
using namespace std; 
 
class Base 
{ 
public: 
    virtual void f(int a) { 
        cout << "virtual Base::f(int a)" << endl; 
    } 
 
    void f(double d) { 
        cout << "Base::f(double d)" << endl;  
    } 
}; 
 
class Derived : public Base 
{ 
public: 
 
    void f(double d) { 
        cout << "Derivd::f(double d)" << endl; 
    } 
}; 
 
int main() 
{ 
    Derived d; 
    d.f(3); 
    d.f(2.5); 
 
    Derived *pd = new Derived(); 
    pd->f(3); 
    pd->f(2.5); 
 
    Base b; 
    b.f(5); 
    b.f(3.5); 
 
    Base *pBase = new Derived(); 
    pBase->f(5); 
    pBase->f(3.5); 
} 
#include <iostream>
using namespace std;
class Base
{
public:
 virtual void f(int a) {
  cout << "virtual Base::f(int a)" << endl;
 }
 void f(double d) {
  cout << "Base::f(double d)" << endl;
 }
};
class Derived : public Base
{
public:
 void f(double d) {
  cout << "Derivd::f(double d)" << endl;
 }
};
int main()
{
 Derived d;
 d.f(3);
 d.f(2.5);
 Derived *pd = new Derived();
 pd->f(3);
 pd->f(2.5);
 Base b;
 b.f(5);
 b.f(3.5);
 Base *pBase = new Derived();
 pBase->f(5);
 pBase->f(3.5);
}
```
其中父类中的void f(double d)隐藏了子类的virtual void f(int a),  void f(double d)函数。
所以在主函数中


```
Derived d; 
d.f(3); 
d.f(2.5); 
 
Derived *pd = new Derived(); 
pd->f(3); 
pd->f(2.5); 
Derived d;
d.f(3);
d.f(2.5);
Derived *pd = new Derived();
pd->f(3);
pd->f(2.5);只要通过Derived对象或者Derived指针执行f()函数，都只执行void Derived::f(double d)该函数。
[html]
Base *pBase = new Derived(); 
pBase->f(5); 
pBase->f(3.5); 
Base *pBase = new Derived();
pBase->f(5);
pBase->f(3.5);
```
在调用pBase->f(5)时，首先要去pBase类中找到对应需要执行的函数，因为Base类中有两个函数virtual void f(int a) 和 void f(double)重载，因为该实参是5，为int类型，所以要调用virtual void f(int a)函数，因为该f(int a)是一个虚函数，所以再去判断pBase所指向的具体对象，具体对象为Derived子类，再去Derived子类的虚函数表中找到void f(int a)函数。因为Derived子类继承了父类Base的虚函数vitural void f(int a)，所以输出 virtual Base::f(int a);
在调用pBase->f(3.5)时，首先要去pBase类中找到对应需要执行的函数，因为因为Base类中有两个函数virtual void f(int a) 和 void f(double)重载，因为该实参是3.5，为double类，所以要调用void f(double d)函数，因为该函数是一个普通成员函数，故直接输出。 void Base::f(double d);
![](http://img.blog.csdn.net/20130510162149907)
再举一个例子：


```
#include <iostream> 
using namespace std; 
 
class Base 
{ 
public: 
    virtual void f(int a) { 
        cout << "virtual Base::f(int a)" << endl; 
    } 
 
    void f(double d) { 
        cout << "Base::f(double d)" << endl;  
    } 
}; 
 
class Derived : public Base 
{ 
public: 
 
    void f(int a) { 
        cout << "virtual Derived::f(int a)" << endl; 
    } 
}; 
 
int main() 
{ 
    Derived d; 
    d.f(3); 
    d.f(2.5); 
 
    Derived *pd = new Derived(); 
    pd->f(3); 
    pd->f(2.5); 
 
    Base b; 
    b.f(5); 
    b.f(3.5); 
 
    Base *pBase = new Derived(); 
    pBase->f(5); 
    pBase->f(3.5); 
} 
#include <iostream>
using namespace std;
class Base
{
public:
 virtual void f(int a) {
  cout << "virtual Base::f(int a)" << endl;
 }
 void f(double d) {
  cout << "Base::f(double d)" << endl;
 }
};
class Derived : public Base
{
public:
 void f(int a) {
  cout << "virtual Derived::f(int a)" << endl;
 }
};
int main()
{
 Derived d;
 d.f(3);
 d.f(2.5);
 Derived *pd = new Derived();
 pd->f(3);
 pd->f(2.5);
 Base b;
 b.f(5);
 b.f(3.5);
 Base *pBase = new Derived();
 pBase->f(5);
 pBase->f(3.5);
}
```
子类Derived中的void f(int a)既覆盖了基类Base的虚函数virtual void f(int a)，也隐藏了基类的virtual void f(int a),  void f(double d)函数。


```
Derived d; 
d.f(3); 
d.f(2.5); 
 
Derived *pd = new Derived(); 
pd->f(3); 
pd->f(2.5); 
Derived d;
d.f(3);
d.f(2.5);
Derived *pd = new Derived();
pd->f(3);
pd->f(2.5);
```
同理所有用子类对象或者子类指针来调用f()函数时，都只执行virtual void f(int a)，输出virtual Derived::f(int a)


```
view plaincopyprint?Base *pBase = new Derived(); 
pBase->f(5); 
pBase->f(3.5); 
Base *pBase = new Derived();
pBase->f(5);
pBase->f(3.5);
```
pBase->f(5)，首先去Base类中寻找相应的函数，同理Base类中的两个函数virtual void f(int a)和void f(double d)是重载函数，因为实参为5,为int类型，所以我们要调用virtual void f(int a)虚函数，因为该函数是虚函数，所以要去判断pBase指向的具体对象，因为pBase指向的是一个子类的对象，所以需要去子类的虚函数表中取找virtual void f(int a)函数，找到之后，执行该函数，故输出virtual Derived::f(int a)。
pBase->f(3.5),首先去Base类中寻找相应的函数，同理Base类中的两个函数virtual void f(int a)和void f(double d)是重载函数，因为实参为3.5,为double类型，所以我们要调用void f(double d)，因为该函数为普通成员函数，故执行输出： void Base::f(double d);
![](http://img.blog.csdn.net/20130510163034815)
