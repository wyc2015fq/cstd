# C++ 虚拟继承 - gauss的专栏 - CSDN博客
2013年01月04日 00:06:23[gauss](https://me.csdn.net/mathlmx)阅读数：163
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)
**C++中虚拟继承的概念**
为了解决从不同途径继承来的同名的数据成员在内存中有不同的拷贝造成数据不一致问题，将共同基类设置为虚基类。这时从不同的路径继承过来的同名数据成员在内存中就只有一个拷贝，同一个函数名也只有一个映射。这样不仅就解决了二义性问题，也节省了内存，避免了数据不一致的问题。
class 派生类名：virtual 继承方式  基类名
virtual是关键字，声明该基类为派生类的虚基类。
在多继承情况下，虚基类关键字的作用范围和继承方式关键字相同，只对紧跟其后的基类起作用。
声明了虚基类之后，虚基类在进一步派生过程中始终和派生类一起，维护同一个基类子对象的拷贝。
C++虚拟继承
**◇概念：**
C++使用虚拟继承（Virtual Inheritance），解决从不同途径继承来的同名的数据成员在内存中有不同的拷贝造成数据不一致问题，将共同基类设置为虚基类。这时从不同的路径继承过来的同名数据成员在内存中就只有一个拷贝，同一个函数名也只有一个映射。
**◇解决问题：**
解决了二义性问题，也节省了内存，避免了数据不一致的问题。**◇同义词：**虚基类（把一个动词当成一个名词而已）当在多条继承路径上有一个公共的基类，在这些路径中的某几条汇合处，这个公共的基类就会产生多个实例(或多个副本)，若只想保存这个基类的一个实例，可以将这个公共基类说明为虚基类。
**◇语法：**
class 派生类: virtual 基类1，virtual 基类2，...，virtual 基类n
{
...//派生类成员声明
};
**◇执行顺序**
首先执行虚基类的构造函数，多个虚基类的构造函数按照被继承的顺序构造；
执行基类的构造函数，多个基类的构造函数按照被继承的顺序构造；
执行成员对象的构造函数，多个成员对象的构造函数按照申明的顺序构造；
执行派生类自己的构造函数；
析构以与构造相反的顺序执行；
***mark***
从虚基类直接或间接派生的派生类中的构造函数的成员初始化列表中都要列出对虚基类构造函数的调用。但只有用于建立对象的最派生类的构造函数调用虚基类的构造函数，而该派生类的所有基类中列出的对虚基类的构造函数的调用在执行中被忽略，从而保证对虚基类子对象只初始化一次。
在一个成员初始化列表中同时出现对虚基类和非虚基类构造函数的调用时，虚基类的构造函数先于非虚基类的构造函数执行。
**◇因果：**
多重继承->二义性->虚拟继承解决
**◇二义性：**
  1: //-----------------------------------------------------
 2: //名称：blog_virtual_inherit.cpp 3: //说明：C++虚拟继承学习演示 4: //环境：VS2005 5: //blog：pppboy.blog.163.com 6: //---------------------------------------------------- 7: #include "stdafx.h" 8: #include <iostream> 9: usingnamespace std; 10: 11: //Base 12: class Base 13: { 14: public: 15: Base(){cout << "Base called..."<< endl;} 16: void print(){cout << "Base print..." <<endl;} 17: private: 18: }; 19: 20: //Sub 21: class Sub //定义一个类 Sub 22: { 23: public: 24: Sub(){cout << "Sub called..." << endl;} 25: void print(){cout << "Sub print..." << endl;} 26: private: 27: }; 28: 29: //Child 30: class Child : public Base , public Sub //定义一个类Child 分别继承自 Base ，Sub 31: { 32: public: 33: Child(){cout << "Child called..." << endl;} 34: private: 35: }; 36: 37: int main(int argc, char* argv[]) 38: { 39: Child c; 40: 41: //不能这样使用，会产生二意性，VC下error C2385 42: //c.print();  43: 44: //只能这样使用 45: c.Base::print(); 46: c.Sub::print(); 47: 48: system("pause"); 49: return 0; 50: }
**◇多重继承：**
 1: //----------------------------------------------------- 2: //名称：blog_virtual_inherit.cpp 
3: //说明：C++虚拟继承学习演示 4: //环境：VS2005 5: //blog：pppboy.blog.163.com 6: //---------------------------------------------------- 7: #include "stdafx.h"  8: #include <iostream> 9: usingnamespace std;  10: 11: int gFlag = 0;  12: 13: class Base  14: { 15: public:  16: Base(){cout << "Base called : " << gFlag++ << endl;}  17: void print(){cout << "Base print" <<endl;}  18: }; 19: 20: class Mid1 : public Base  21: { 22: public:  23: Mid1(){cout << "Mid1 called" << endl;}  24: private:  25: }; 26: 27: class Mid2 : public Base  28: { 29: public:  30: Mid2(){cout << "Mid2 called" << endl;}  31: }; 32: 33: class Child:public Mid1, public Mid2  34: { 35: public:  36: Child(){cout << "Child called" << endl;}  37: }; 38: 39: int main(int argc, char* argv[])  40: { 41: Child d; 42: 43: //不能这样使用，会产生二意性 44: //d.print(); 45: 46: //只能这样使用 47: d.Mid1::print(); 48: d.Mid2::print(); 49: 50: system("pause");  51: return 0;  52: } 53:
//output
 Base called : 0 Mid1 called**Base called : 1** Mid2 called Child called Base print Base print
**◇虚拟继承**
在派生类继承基类时，加上一个virtual关键词则为虚拟继承
 1: //----------------------------------------------------- 2: //名称：blog_virtual_inherit.cpp 
3: //说明：C++虚拟继承学习演示 4: //环境：VS2005 5: //blog：pppboy.blog.163.com 6: //---------------------------------------------------- 7: #include "stdafx.h"  8: #include <iostream> 9: usingnamespace std;  10: 11: int gFlag = 0;  12: 13: class Base  14: { 15: public:  16: Base(){cout << "Base called : " << gFlag++ << endl;}  17: void print(){cout << "Base print" <<endl;}  18: }; 19: 20: class Mid1 :**virtual**public Base  21: { 22: public:  23: Mid1(){cout << "Mid1 called" << endl;}  24: private:  25: }; 26: 27: class Mid2 : **virtual**public Base  28: { 29: public:  30: Mid2(){cout << "Mid2 called" << endl;}  31: }; 32: 33: class Child:public Mid1, public Mid2  34: { 35: public:  36: Child(){cout << "Child called" << endl;}  37: }; 38: 39: int main(int argc, char* argv[])  40: { 41: Child d; 42: 43: //这里可以这样使用 44: d.print(); 45: 46: //也可以这样使用 47: d.Mid1::print(); 48: d.Mid2::print(); 49: 50: system("pause");  51: return 0;  52: } 53:
//output
 1: Base called : 0 2: Mid1 called 3: Mid2 called 4: Child called 5: Base print 6: Base print 7: Base print 8: 请按任意键继续. . .
**◇通过输出的比较**
1.在多继承情况下，虚基类关键字的作用范围和继承方式关键字相同，只对紧跟其后的基类起作用。2.声明了虚基类之后，虚基类在进一步派生过程中始终和派生类一起，维护同一个基类子对象的拷贝。3.观察类构造函数的构造顺序，拷贝也只有一份。**◇与虚函数关系**虚拟继承与虚函数有一定相似的地方，但他们之间是绝对没有任何联系的。再想一次：虚拟继承，虚基类，虚函数。**◇**[**菱形结构虚继承对象的内存模**](http://tech.163.com/05/1209/17/24I177I10009159Q.html)**[**型**](http://tech.163.com/05/1209/17/24I177I10009159Q.html)**[](http://tech.163.com/05/1209/17/24I177I10009159Q.html)
