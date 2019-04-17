# 【类继承】跑来跑去的类继承_Tencent - CD's Coding - CSDN博客





2015年04月01日 00:39:47[糖果天王](https://me.csdn.net/okcd00)阅读数：367








腾讯笔试里有道题也是让人头晕的累的一塌糊涂，就是给了一群类，谁是谁的父类，子类，集成类，其中还有属性，还有析构什么什么的，问输出的是什么（其实是考察运行的顺序）那么，备忘记录下吧。




# c++ 类的继承与派生



## 一、基本概念

### 1、类的继承

是新的类从已有类那里得到已有的特性。或从已有类产生新类的过程就是类的派生。原有的类称为基类或父类，产生的新类称为派生类或子类。

### 2、派生类的声明

class 派生类名：继承方式 基类名1， 继承方式 基类名2，...，继承方式 基类名n

{

    派生类成员声明；

};



### 3、一个派生类可以同时有多个基类

这种情况称为多重继承，派生类只有一个基类，称为单继承。直接派生，间接派生。



### 4、继承方式规定了如何访问基类继承的成员。

继承方式有public, private, protected。如果不显示给出继承方式，默认为private继承。继承方式指定了派生类成员以及类外对象对于从基类继承来的成员的访问权限。



### 5、派生类继承基类中除构造和析构函数以外的所有成员。



### 6、派生类生成：

   吸收基类成员（除构造析构函数以外的所有成员）；

   改造基类成员（根据继承方式调整基类成员的访问，函数在子类中的覆盖，以及虚函数在子类中的覆盖）；

   添加新的成员；



### 7、公有继承

当类的继承方式为公有继承时，基类的公有和保护成员的访问属性在派生类中不变，而基类的私有成员不可访问。即基类的公有成员和保护成员被继承到派生类中仍作为派生类的公有成员和保护成员。派生类的其他成员可以直接访问它们。无论派生类的成员还是派生类的对象都无法访问基类的私有成员。



### 8、私有继承

当类的继承方式为私有继承时，基类中的公有成员和保护成员都以私有成员身份出现在派生类中，而基类的私有成员在派生类中不可访问。基类的公有成员和保护成员被继承后作为派生类的私有成员，派生类的其他成员可以直接访问它们，但是在类外部通过派生类的对象无法访问。无论是派生类的成员还是通过派生类的对象，都无法访问从基类继承的私有成员。通过多次私有继承后，对于基类的成员都会成为不可访问。因此私有继承比较少用。



### 9、保护继承

保护继承中，基类的公有成员和私有成员都以保护成员的身份出现在派生类中，而基类的私有成员不可访问。派生类的其他成员可以直接访问从基类继承来的公有和保护成员，但是类外部通过派生类的对象无法访问它们，无论派生类的成员还是派生类的对象，都无法访问基类的私有成员。





## 二、派生类的构造函数和析构函数

1、派生类中由基类继承而来的成员的初始化工作还是由基类的构造函数完成，然后派生类中新增的成员在派生类的构造函数中初始化。




2、派生类构造函数的语法：


派生类名::派生类名（参数总表）：基类名1（参数表1），基类名（参数名2）....基类名n（参数名n），内嵌子对象1（参数表1），内嵌子对象2（参数表2）....内嵌子对象n（参数表n）


{


    派生类新增成员的初始化语句；


}

注：构造函数的初始化顺序并不以上面的顺序进行，而是根据声明的顺序初始化。




3、如果基类中没有不带参数的构造函数，那么在派生类的构造函数中必须调用基类构造函数，以初始化基类成员。




4、派生类构造函数执行的次序：


   调用基类构造函数，调用顺序按照它们被继承时声明的顺序（从左到右）；


   调用内嵌成员对象的构造函数，调用顺序按照它们在类中声明的顺序；


   派生类的构造函数体中的内容。


例子：

```cpp
#include <iostream>
#include <time.h>
using namespace std;

class B1
{
public:
    B1(int i)
    {
        cout<<"constructing B1 "<<i<<endl;
    }
};

class B2
{
public:
    B2(int j)
    {
        cout<<"constructing B2 "<<j<<endl;
    }
};

class B3
{
public:
    B3()
    {
        cout<<"constructing B3"<<endl;
    }
};

class C: public B2, public B1, public B3
{
public:
    C(int a, int b, int c, int d):B1(a), memberB2(d), memberB1(c),B2(b)
    {

    }
private:
    B1 memberB1;
    B2 memberB2;
    B3 memberB3;
};

int main() 
{ 
    C obj(1,2,3,4);

    return 0; 
}
```




C(a,b,c,d)赋值


Then Class C: B2,B1,B3


Then Private B1,B2,B3




输出结果为：


constructing B2 2


constructing B1 1


constructing B3


constructing B1 3


constructing B2 4


constructing B3




5、析构函数


派生类的析构函数的功能是在该对象消亡之前进行一些必要的清理工作，析构函数没有类型，也没有参数。析构函数的执行顺序与构造函数相反。


例子：

```cpp
#include <iostream>
#include <time.h>
using namespace std;

class B1
{
public:
    B1(int i)
    {
        cout<<"constructing B1 "<<i<<endl;
    }
    ~B1()
    {
        cout<<"destructing B1"<<endl;
    }
};

class B2
{
public:
    B2(int j)
    {
        cout<<"constructing B2 "<<j<<endl;
    }
    ~B2()
    {
        cout<<"destructing B2"<<endl;
    }
};

class B3
{
public:
    B3()
    {
        cout<<"constructing B3"<<endl;
    }
    ~B3()
    {
        cout<<"destructing B3"<<endl;
    }
};

class C: public B2, public B1, public B3
{
public:
    C(int a, int b, int c, int d):B1(a), memberB2(d), memberB1(c),B2(b)
    {

    }
private:
    B1 memberB1;
    B2 memberB2;
    B3 memberB3;
};

int main() 
{ 
    C obj(1,2,3,4);

    return 0; 
}
```




析构顺序为构造顺序反序




输出结果为：


constructing B2 2


constructing B1 1


constructing B3


constructing B1 3


constructing B2 4


constructing B3





destructing B3


destructing B2


destructing B1


destructing B3


destructing B1


destructing B2



## 三、派生类成员的标识和访问





1、派生类成员属性划分为四种：


   不可访问的成员；私有成员；保护成员；公有成员；




2、作用域分辨


形式为：基类名::成员名；基类名::成员名（参数表）；


如果某派生类的多个基类拥有同名的成员，同时，派生类又新增这样的同名成员，在这种情况下，派生类成员将覆盖所有基类的同名成员。这就需要这样的调用方式才能调用基类的同名成员。


例子：多继承同名 

```cpp
#include <iostream>
#include <time.h>
using namespace std;

class B1
{
public:
    int nV;
    void fun()
    {
        cout<<"member of B1 "<<nV<<endl;
    }
};

class B2
{
public:
    int nV;
    void fun()
    {
        cout<<"member of B2 "<<nV<<endl;
    }
};

class D1: public B1, public B2
{
public:
    int nV;
    void fun()
    {
        cout<<"member of D1 "<<nV<<endl;
    }
};

int main() 
{ 
    D1 d1;
    d1.nV = 1;
    d1.fun();
    d1.B1::nV = 2;
    d1.B1::fun();
    d1.B2::nV = 3;
    d1.B2::fun();

    return 0; 
}
```



输出结果为：


member of D1 1


member of B1 2


member of B2 3


以上通过作用域分辨符，解决了访问基类中被屏蔽的同名成员。




3、如果某个派生类的部分或全部直接基类是从另一个共同的基类派生而来，在这些直接基类中，从上一级基类继承来的成员就拥有相同的名称，因此派生类中也就会产生同名现象，对这种类型的同名成员也要使用作用域分辨符来唯一标识，而且必须用直接基类进行限定。


例子：

```cpp
#include <iostream>
#include <time.h>
using namespace std;

class B0
{
public:
    int nV;
    void fun()
    {
        cout<<"member of B0 "<<nV<<endl;
    }
};

class B1:public B0
{
public:
    int nV1;
};

class B2:public B0
{
public:
    int nV2;
};

class D1:public B1, public B2
{
public:
    int nVd;
    void fund()
    {
        cout<<"member of D1"<<endl;
    }
};

int main() 
{ 
    D1 d1;
    d1.B1::nV = 2;
    d1.B1::fun();
    d1.B2::nV = 3;
    d1.B2::fun();

    return 0; 
}
```



输出结果为：


member of B0 2


member of B0 3


在这种情况下，派生类对象在内存中就同时拥有成员nV及fun的两份拷贝。但是很多情况下，我们只需要这样一个这样的数据拷贝，同一成员的多份拷贝增加了内存的开销。可以通过虚函数来解决这个问题。




4、虚基类


为了解决前面提到的多重拷贝的问题，可以将共同基类设置为虚基类，这时从不同的路径继承过来的同名数据成员在内存中就只有一个拷贝，同一个函数也只有一个映射。


虚基类的声明是在派生类的声明过程，其语法形式为：


class 派生类名::virtual 继承方式 基类名；


例子：

```cpp
#include <iostream>
#include <time.h>
using namespace std;

class B0
{
public:
    int nV;
    void fun()
    {
        cout<<"member of B0 "<<nV<<endl;
    }
};

class B1:virtual public B0
{
public:
    int nV1;
};

class B2:virtual public B0
{
public:
    int nV2;
};

class D1:public B1, public B2
{
public:
    int nVd;
    void fund()
    {
        cout<<"member of D1"<<endl;
    }
};

int main() 
{ 
    D1 d1;
    d1.nV = 2;
    d1.fun();

    return 0; 
}
```



输出结果为：


member of B0 2




5、虚基类及其派生类的构造函数


一般而言，派生类只对其直接基类的构造函数传递参数，但是在虚基类中，不管是直接或间接虚基类的所有派生类，都必须在构造函数的成员初始化列表中列出对虚基类的初始化。


例子：

```cpp
#include <iostream>
#include <time.h>
using namespace std;

class B0
{
public:
    B0(int n)
    {
        nV = n;
    }
    int nV;
    void fun()
    {
        cout<<"member of B0 "<<nV<<endl;
    }
};

class B1:virtual public B0
{
public:
    B1(int a):B0(a)   
    {
    }
    int nV1;
};

class B2:virtual public B0
{
public:
    B2(int a):B0(a)
    {
    }
    int nV2;
};

class D1:public B1, public B2
{
public:
    D1(int a):B0(a), B1(a), B2(a)
    {
    }
    int nVd;
    void fund()
    {
        cout<<"member of D1"<<endl;
    }
};

int main() 
{ 
    D1 d1(1);
    d1.nV = 2;
    d1.fun();

    return 0; 
}
```



以上例子看上去B0的构造函数好像被调用了三次，但是实际上只有D1类中的D1(int a):B0(a), B1(a), B2(a)

才是真正的调用了B0构造函数。



## 四、赋值兼容规则





1、赋值兼容规则是指在需要基类对象的任何地方都可以使用公有派生类的对象来替代。


2、赋值兼容规则中所指的替代包括：


   派生类的对象可以赋值给基类对象；


   派生类的对象可以初始化基类的引用；


   派生类对象的地址可以赋给指向基类的指针。


   在替代之后，派生类对象就可以作为基类的对象使用，但只能使用从基类继承的成员。


例子：

```cpp
#include <iostream>
#include <time.h>
using namespace std;

class B0
{
public:
    void display()
    {
        cout<<"B0::display()"<<endl;
    }
};

class B1:public B0
{
public:
    void display()
    {
        cout<<"B1::display()"<<endl;
    }
};

class B2:public B0
{
public:
    void display()
    {
        cout<<"B2::display()"<<endl;
    }
};

void fun(B0 *ptr)
{
    ptr->display();
}

int main() 
{ 
    B0 b0;
    B1 b1;
    B2 b2;
    fun(&b0);
    b0 = b1;
    fun(&b0);
    b0 = b2;
    fun(&b0);

    return 0; 
}
```



输出结果为：


B0::display()


B0::display()


B0::display()


通过这种赋值兼容后，每次调用的同名函数都是基类的同名函数，如果想调用派生类的，则需要使用虚函数。





# JAVA类继承和初始化类

初始化类的执行顺序：子类初始化-》-》子类构造方法-》父类-》构造方法-》属性-》构造方法中内容-》构造方法下面的内容-》子类属性-》子类构造方法中内容



在Java编程语言中，对象的初始化是非常结构化的，这样做是为了保证安全。在前面的模块中，看到了当一个特定对象被创建时发生了什么。由于继承性，对象被完成，而且下述行为按顺序发生：


（1）存储空间被分配并初始化到0值

（2）进行显式初始化

（3）调用构造方法

（4）层次中的每个类都会发生最后两个步骤，是从最上层开始。


Java技术安全模式要求在子类执行任何东西之前，描述父类的一个对象的各个方面都必须初始化。因此，Java编程语言总是在执行子构造方法前调用父类构造方法的版本。 有继承的类在运行的时候，一定要记得：初始化子类必先初始化父类，这是Java程序的一个基本运行过程。比如：


```java
public class Test extends Parent{
private String name = "Java私塾";
private int age = 2;
public Test(){
age = 1000;//期望能到1000年，呵呵
}
public static void main(String[] args) { 
Test t = new Test();
System.out.println(t.name+"的年龄是"+t.age+"年");
}
}
class Parent{
private int num = 1;
public Parent(){
System.out.println("现在初始化父类");
}
public void test(){
System.out.println("这是父类的test方法");
 }
}
```



上述类的基本运行顺序是： 

（1）：先运行到第7行，这是程序的入口 

（2）：然后运行到第8行，这里要new一个Test，就要调用Test的构造方法 

（3）：就运行到第4行，注意：初始化子类必先初始化父类 

（4）：要先初始化父类，所以运行到第14行 

（5）：然后是第13行，初始化一个类，必须先初始化它的属性 

（6）：然后是第15行 

（7）：然后是第16行，表示父类初始化完成 

（8）：然后是回到子类，开始初始化属性，因此运行到第2行，然后是第3行 

（9）：子类属性初始化完过后，才回到子类的构造方法，执行里面的代码，也就是第5行 

（10）：然后是第6行，表示new一个Test实例完成 

（11）：然后回到main方法中执行第9行 

（12）：然后是第10行 运行结果是： 现在初始化父类 Java私塾的年龄是1000年




