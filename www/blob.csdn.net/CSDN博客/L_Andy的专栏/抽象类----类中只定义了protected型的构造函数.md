# 抽象类----类中只定义了protected型的构造函数 - L_Andy的专栏 - CSDN博客

2012年03月09日 16:54:45[卡哥](https://me.csdn.net/L_Andy)阅读数：3052


 对一个类来说，如果只定义了protected型的构造函数而没有提供public构造函数，无论是在外部还是在派生类中都不能创建该类的对象，但可以有其派生出新的类，这种能派生新类，却不能创建自己对象的类是另一种形式的抽象类例如：

#include <iostream>

using namespace std;

class Base

{

private:

 int x;

protected:

 Base(int xp=0)

 {

  x=xp;

 }

public:

 void disp()

 {

  cout<<"x is "<<x<<endl;

 }

};

class Child:public Base

{

public:

 //Base b,错误，在派生类中不能创建Base类对象；

 Child(int xpp):Base(xpp)

 {

 }

};

int main()

{

 Child c(1);

 //Base b;错误，在外部不能创建Base类对象

 c.disp();

 return 0;

}

Base类中定义了一个Protected型的构造函数，此时无论在外部函数（如main函数）中还是在派生类中创建Base类的对象都是非法的，但是由于protected型的构造函数可以在派生类中进行访问，因此Base类可以派生出Child类。Base类是另一种形式的抽象类。

**延伸：构造函数能否为private型**

  构造函数可以为public型，也可以为protected 型，但构造函数是否可以是private型呢？

答案是肯定的。

  在此时尽管不能直接在外部函数和派生类中使用“类名+对姓名”的形式来创建该类对象，但可以通过类的static函数成员来创建类的实例

#include <iostream>

using namespace std;

class Example

{

private:

 int x;

 Example(int xp)

 {

  x=xp;

 }

public:

 static Example* CreateObject(int xp)  //static函数，不用创建对象便调用

 {

  return new Example(xp);     //申请动态内存

 }

 static void DeleteObject(Example * pE)  //static函数，不用创建对象便调用

 {

  delete pE;        //释放申请到的内存资源

 }

 void disp()         //成员函数

 {

  cout<<"x is "<<x<<endl;

 }

};

int main()

{

 //使用类名访问static函数创建对象，返回指向动态内存的指针

 Example *pExample=Example::CreateObject(5);

 pExample->disp();

 Example::DeleteObject(pExample);

 return 0;

}

输出结果是

x is 5

例子演示了private构造函数的用法，正因为无法创建对象，所以，必须使用static成员函数来创建实例。通过对上述代码的分析可知，static函数同样可以应用在protected型构造函数的场合下。

