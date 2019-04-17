# C++ 多态与虚函数 - 在思索中前行！ - CSDN博客





2015年03月30日 22:47:15[_Tham](https://me.csdn.net/txl16211)阅读数：786标签：[C++ 多态																[C++虚函数																[多态与虚函数](https://so.csdn.net/so/search/s.do?q=多态与虚函数&t=blog)
个人分类：[C/C++](https://blog.csdn.net/txl16211/article/category/2284665)





什么是多态？

一个 “操作” 随着所 ”传递或捆绑的对象类型的不同 “ 能够做出 ”不同的反应“，其行为模式称为多态。

即，对这个操作，能接受不同类型的参数，而且在处理参数时，会根绝其类型做出不同的反应。

为什么要引入多态？

针对由继承得到一类对象，在处理这类对象时，能够以同一操作处理完所有对象。

遇到的问题：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645827#)[copy](http://blog.csdn.net/insistgogo/article/details/6645827#)

- #include<iostream>
- usingnamespace std;  
- 
- class Base  
- {  
- public:  
- void Show()  
-     {  
-         cout<<"Base"<<endl;  
-     }  
- };  
- 
- class DeriveOne : public Base  
- {  
- public:  
- void Show()  
-     {  
-         cout<<"DeriveOne"<<endl;  
-     }  
- };  
- 
- 
- void AAA(Base base)  
- {  
-     base.Show();  
- }  
- 
- 
- int main()  
- {  
-     Base base;  
-     DeriveOne deriveOne;  
- 
-     AAA(base);  
-     AAA(deriveOne);  
- 
-     system("pause");  
- return 1;  
- }  


输出

![](https://img-blog.csdn.net/20130516112125640)


说明：

在父类Base和派生类DeriveOne中都定义了Show操作，它们分别输出对应类的信息，Base的show操作显示Base的信息，DeriveOne的show操作显示DeriveOne的信息。在函数AAA中，由于参数是base类型，在函数AAA中使用时，无论形参是父类Base还是子类DeriveOne，在函数中，其仅仅表示父类或者子类中的父类的那部分信息。若想显示完全，就必须写两个函数，它们分别接受两种参数，如：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645827#)[copy](http://blog.csdn.net/insistgogo/article/details/6645827#)

- void show(Base base);  
- void show(DeriveOne Derive);  


问题又来了，由于派生类可以转化为基类，因此在调用Show函数时，还需要判断来的类是基类还是派生类。此时，用的时候就很不方便了。

此时，我们想要的是，函数AAA的获得实参可以是继承类集合中的任意类对象，而且其函数体也能根据其实参的改变调用不同的函数，即自动分别实参。这就是我们要求的多态。

举例说明要实现的目标：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645827#)[copy](http://blog.csdn.net/insistgogo/article/details/6645827#)

- #include<iostream>
- usingnamespace std;  
- 
- class Base  
- {  
- public:  
- virtualvoid Show()  
-     {  
-         cout<<"Base"<<endl;  
-     }  
- };  
- 
- class DeriveOne : public Base  
- {  
- public:  
- void Show()  
-     {  
-         cout<<"DeriveOne"<<endl;  
-     }  
- };  
- 
- class DeriveTwo : public Base  
- {  
- public:  
- void Show()  
-     {  
-         cout<<"DeriveTwo"<<endl;  
-     }  
- };  
- 
- //定义一个操作，对多个不同的继承类对象做统一操作
- void AAA(Base& base)    
- {  
-     base.Show();  
- }  
- 
- int main()  
- {  
-     Base base;  
-     DeriveOne deriveOne;  
-     DeriveTwo deriveTwo;  
- 
-     AAA(base);  
-     AAA(deriveOne);  
-     AAA(deriveTwo);  
-     system("pause");  
- return 1;  
- }  


![](https://img-blog.csdn.net/20130516111939534)

说明，这里还是给出一个函数AAA，其调用AAA的实参是有三个，base、deriveOne、deriveTwo，函数AAA能够根据传进来的实参来自定义判断调用哪个类的Show函数。这是我们想要的结果，即函数AAA能够接受整个派生类集合，并能区分不同的类对象。

怎么实现多态？

**虚函数 **+ **基类指针**或**引用**调用。

什么是虚函数？

简单地说，那些被virtual关键字修饰的成员函数，就是虚函数。

虚函数的原理：

虚函数的作用是实现类的继承所体现的多态性，具体点是实现动态联编。

从程序的角度上来说，在定义了虚函数后，可以在派生类中对虚函数重新定义，以实现统一的接口，不同定义过程，在程序的运行阶段动态地选择合适的成员函数。

一旦标记基类的函数为虚函数，便有连锁反应，后面继承的类中一切同名成员函数都变成了虚函数，（即使后面的这些同名函数没有声明该函数为虚函数）。

C++实现运行时多态性的关键途径：在公有派生情况下，一个指向基类的指针可用来访问从基类继承的任何对象。

虚函数语法：

对函数的设置：普通函数的前面加上virtual 



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645827#)[copy](http://blog.csdn.net/insistgogo/article/details/6645827#)

- virtual 函数返回值类型 虚函数名(形参表)  
- {   
- //函数体
- }  


虚函数的调用方式：只能通过指向基类的指针或基类对象的引用来调用虚函数

调用语法：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645827#)[copy](http://blog.csdn.net/insistgogo/article/details/6645827#)

- 指向基类的指针变量名->虚函数名（实参表）  
- 基类对象的引用名. 虚函数名（实参表）  


注意事项一：正常情况下，如果不把函数声明为虚函数，指向基类的指针的访问情况如下：

1）基类指针指向基类对象：基类指针可以直接访问基类对象中的成员

2）基类指针指向派生类对象：基类指针只能访问派生类中的从基类中继承的成员，派生类有同名的函数或成员，也只能调用基类的成员。

如果定义成虚函数时：定义一个基类指针，把不同的派生类对象付给它，会调用对应派生类的函数，而非基类函数。

举例：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645827#)[copy](http://blog.csdn.net/insistgogo/article/details/6645827#)

- #include <iostream>
- usingnamespace std;  
- class A  
- {  
- public:  
- virtualvoid show()  
-     {  
-         cout<<"A"<<endl;  
-     }  
- };  
- class B:public A  
- {  
- public:  
- void show()  
-     {  
-         cout<<"B"<<endl;  
-     }  
- };  
- class C:public A  
- {  
- public:  
- void show()  
-     {  
-         cout<<"C"<<endl;  
-     }  
- };  
- void main()  
- {  
-     A*a;  
-     B b;  
-     C c;  
-     a=&b;  
-     a->show();  
-     a=&c;  
-     a->show();  
-     system("pause");  
- }  


运行结果：B(换行)C(换行)--指向不同的派生类，调用不同的函数

如果不加基类A中的Virtual，则输出结果：A(换行)A(换行)--基类指针，调用派生类中继承的基类成分

注意事项二：如果不是使用基类指针或引用，而仅仅是类之间的复制，那么子类对象就完全转换为父类对象，就不会有多态了。



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645827#)[copy](http://blog.csdn.net/insistgogo/article/details/6645827#)

- Student s; //基类
- GraduateStudent gs; //派生类
- 
- void fn (Student a)//形参传值--而非(Student & a)
- {  
-     a.Show();  
- };  
- 
- fn(gs); //无多态，调用基类的show函数，而不是派生类的show函数。


原因，在参数传递的过程中，已经将实参的性质做了肯定的转变，而对于确定的对象，是没有选择操作可言的。

因此，仅仅对于对象的指针和引用的间接访问，才会发生多态现象。


定义虚函数，实现动态联编需要三个条件： 

1）必须把动态联编的行为定义为类的虚函数---定义虚函数 

2）类之间存在子类型关系，一般表现为一个类从另一个类公有派生而来---类之间是公有继承 

3）基类指针指向派生类的对象，然后使用基类指针调用虚函数

把那些函数设置为虚函数？

如果一个函数在子类和父类中都有，而且子类使用这个函数时，仅仅使用自己就ok，那么可以把这个函数设置为虚函数。

虚函数是怎么实现动态捆绑的？

编译器在编译的时候，看到一个基类函数前面有关键字”virtual“，编译器就知道需要对该函数进行晚捆绑，即不能立即断定虚函数的确切位置，此时编译器将此函数调用转化为一个指针。该指针在该虚函数被调用的时候，根据实际对象来给指针赋值，即指向实际对象的成员函数。这样一来，若实际对象是基类，则调用的是基类成员函数。若实际对象是派生类，则调用的是子类的成员函数。

为了给这个指针赋值，必须在每个实际的对象中额外占有一个指针空间，以指向自己类中的虚函数表。


如：

![](https://img-blog.csdn.net/20130516165011697)


注意，具有虚函数的类，其对象空间比没有虚函数的类多了一个指针空间，而且在使用时需要间接访问虚函数，即采用虚函数，会影响一些程序运行效率。

关于虚函数的详细说明，请见另一篇文章：

虚函数的

注意：

1、使用时，虚函数可以在基类中声明，提供界面。可以在给派生类中定义具体的实现方法，而得到多种方法。

2、多态的实现必须是公有派生。

原因：虚函数为了实现多态，虚函数建立在赋值兼容原则上，而赋值兼容原则成立的前提条件是派生类从基类那里公有派生。

3、派生类对基类中虚函数重新定义时，关键字可以写也可以不写，系统可以自动判断。

因为，虚函数会自动地从基类往下传播，可以虚到底。在函数调用时，总是先碰见基类的函数，只要把该函数设置为虚函数，系统就认为其他派生类函数就为虚的啦。

4、虚函数必须是类的成员函数，不能是友元函数，也不能是静态成员函数。

因为虚函数仅仅适用于有继承关系的类对象，所有普通函数不行。由于静态函数不收对象的捆绑，即使在形象上的捆绑，实际上也没有任何对象的信息，只有类的信息。但是虚函数可以是另一个类的友元函数

5、内敛函数不能是虚函数。

因为内敛函数不能在运行时动态确定位置。所以虚函数都是非内敛的。即使虚函数在类内部定义，在编译时，仍被看做为非内敛的。

6、构造函数不是虚函数。因为构造时，对象还不存在。

7、析构函数是可以虚函数，那么派生而来的所有派生类的析构函数也是虚函数，不管是否使用了关键字virtual

8、一旦一个函数被说明为虚函数，不管经历多少层派生，都将保持其虚特性，即虚特性具有继承性

9、如果在派生类中没有对虚函数重新定义，则它继承其基类的虚函数。

10、虚函数在派生类中重新定义时，可以函数原型必须完全相同，如具有相同的形参个数和形参类型，但返回值可以不同。



纯虚函数：如果基类的函数没有必要或者无法实现，完全要依赖子类去实现的话，可以使用纯虚函数

简单点说，基类提供接口，而不提供定义，派生类提供实现

语法：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645827#)[copy](http://blog.csdn.net/insistgogo/article/details/6645827#)

- virtual 函数返回值类型 虚函数名(形参表)=0；  


注意：从基类继承来的纯虚函数，在派生类中仍是纯虚函数，除非派生类按原型一致地超载该纯虚函数，变成类的纯虚函数



抽象类：包含了纯虚函数的类 

注意：抽象类只能用作其他类的基类，不能建立对象，不能作为参数类型、返回类型或显式转换类型，可声明指针和引用。可以定义返回引用。](https://so.csdn.net/so/search/s.do?q=C++虚函数&t=blog)](https://so.csdn.net/so/search/s.do?q=C++ 多态&t=blog)




