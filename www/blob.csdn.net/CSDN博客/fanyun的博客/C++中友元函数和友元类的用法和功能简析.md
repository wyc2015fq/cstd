# C++中友元函数和友元类的用法和功能简析 - fanyun的博客 - CSDN博客
2016年11月10日 14:27:47[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：413标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)

       采用类的机制后实现了数据的隐藏与封装，类的数据成员一般定义为私有成员，成员函数一般定义为公有的，依此提供类与外界间的通信接口。但是，有时需要定义一些函数，这些函数不是类的一部分，但又需要频繁地访问类的数据成员，这时可以将这些函数定义为该函数的友元函数。除了友元函数外，还有友元类，两者统称为友元。友元的作用是提高了程序的运行效率（即减少了类型检查和安全性检查等都需要时间开销），但它破坏了类的封装性和隐藏性，使得非成员函数可以访问类的私有成员。
友元函数 ： 
       友元函数是可以直接访问类的私有成员的非成员函数。它是定义在类外的普通函数，它不属于任何类，但需要在类的定义中加以声明，声明时只需在友元的名称前加上关键字friend，其格式如下：
       friend 类型 函数名(形式参数);
       友元函数的声明可以放在类的私有部分，也可以放在公有部分，它们是没有区别的，都说明是该类的一个友元函数。
       一个函数可以是多个类的友元函数，只需要在各个类中分别声明。
       友元函数的调用与一般函数的调用方式和原理一致。
友元类 ： 
       友元类的所有成员函数都是另一个类的友元函数，都可以访问另一个类中的隐藏信息（包括私有成员和保护成员）。       
       当希望一个类可以存取另一个类的私有成员时，可以将该类声明为另一类的友元类。定义友元类的语句格式如下：
       friend class 类名;
       其中：friend和class是关键字，类名必须是程序中的一个已定义过的类。
       例如，以下语句说明类B是类A的友元类：
       class A
       {
             …
       public:
             friend class B;
             …
       };
       经过以上说明后，类B的所有成员函数都是类A的友元函数，能存取类A的私有成员和保护成员。
       使用友元类时注意：
             (1) 友元关系不能被继承。 
             (2) 友元关系是单向的，不具有交换性。若类B是类A的友元，类A不一定是类B的友元，要看在类中是否有相应的声明。
             (3) 友元关系不具有传递性。若类B是类A的友元，类C是B的友元，类C不一定是类A的友元，同样要看类中是否有相应的申明
《windows环境多线程编程原理与应用》中解释：
　如果将类的封装比喻成一堵墙的话，那么友元机制就像墙上了开了一个门，那些得
　到允许的类或函数允许通过这个门访问一般的类或者函数无法访问的私有属性和方　　　
　法。友元机制使类的封装性得到消弱，所以使用时一定要慎重。
　■ 友元类的说明
　　　将外界的某个类在本类别的定义中说明为友元，那么外界的类就成为本类的“朋　　
　　　友”，那个类就可以访问本类的私有数据了。
　　　class Merchant
      {
         private :
            int m_MyMoney;
            int m_MyRoom;
            … …
         Public:
            Friend class Lawyer;
            Int getmoney();
            … …
      };
      Class Lawyer
     {
       Private:
         … …
       Public:
        … …
     };
     只有你赋予某个类为你的友元时，那个类才有访问你的私有数据的权利。
　■ 说明一个函数为一个类的友元函数则该函数可以访问此类的私有数据和方法。定义方法是在类的定义中，在函数名前加上关键字friend.
       《挑战30天C/C++》这样解释：
　    在说明什么是友元之前，我们先说明一下为什么需要友元与友元的缺点: 
        通常对于普通函数来说,要访问类的保护成员是不可能的，如果想这么做那么必须把类的成员都生命成为public(共用的)，然而这做带来的问题遍是任何外部函数都可以毫无约束的访问它操作它，c++利用friend修饰符，可以让一些你设定的函数能够对这些保护数据进行操作，避免把类成员全部设置成public，最大限度的保护数据成员的安全。 
友元能够使得普通函数直接访问类的保护数据，避免了类成员函数的频繁调用，可以节约处理器开销，提高程序的效率，但所矛盾的是，即使是最大限度大保护，同样也破坏了类的封装特性，这即是友元的缺点，在现在cpu速度越来越快的今天我们并不推荐使用它，但它作为c++一个必要的知识点，一个完整的组成部分，我们还是需要讨论一下的。 
在类里声明一个普通数学，在前面加上friend修饰，那么这个函数就成了该类的友元，可以访问该类的一切成员。
        下面我们来看一段代码，看看我们是如何利用友元来访问类的一切成员的
#include <iostream> 
using namespace std; 
class Internet 
{ 
public: 
Internet(char *name,char *address) // 改为：internet(constchar *name , const char *address)
{ 
strcpy(Internet::name,name); 
strcpy(Internet::address,address); 
} 
friend void ShowN(Internet &obj);   //友元函数的声明 
public: 　　　　　　　　　　　　　// 改为：private
char name[20]; 
char address[20]; 
}; 
void ShowN(Internet &obj)        //函数定义,不能写成,voidInternet::ShowN(Internet &obj) 
{ 
cout<<obj.name<<endl;         //可访问internet类中的成员
} 
void main() 
{ 
Internet a("nice","www.nice.com"); 
ShowN(a); 
cin.get(); 
} 
    上面的代码通过友元函数的定义，我们成功的访问到了a对象的保护成员name,友元函数并不能看做是类的成员函数，它只是个被声明为类友元的普通函数，所以在类外部函数的定义部分不能够写成void Internet::ShowN(Internet &obj)，这一点要注意。
===============================================================================
实例：
分别定义一个类A和类B ，各有一个私有整数成员变量通过构造函数初始化；类A有一个成员函数Show(B &b)用来打印A和B的私有成员变量，请分别通过友元函数和友元类来实现此功能。
用友元类和 友元函数做出来的
#include <iostream>
using namespace std;
class B;
class A;
void Show( A& , B& );
class B
{
private:
int tt;
friend class A;
friend void Show( A& , B& );
public:
B( int temp = 100):tt ( temp ){}
};
class A
{
private:
int value;
friend void Show( A& , B& );
public:
A(int temp = 200 ):value ( temp ){}
void Show( B &b )
{
  cout << value << endl;
  cout << b.tt << endl; 
}
};
void Show( A& a, B& b )
{
cout << a.value << endl;
cout << b .tt << endl;
}
int main()
{
A a;
B b;
a.Show( b );
Show( a, b );
      return 0;
}
