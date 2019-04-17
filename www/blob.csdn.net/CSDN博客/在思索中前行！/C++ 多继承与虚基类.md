# C++ 多继承与虚基类 - 在思索中前行！ - CSDN博客





2015年03月30日 22:49:42[_Tham](https://me.csdn.net/txl16211)阅读数：703








转载来自：[CSDN
insistGoGo  （http://blog.csdn.net/insistgogo）](http://blog.csdn.net/insistgogo)

多继承的定义：派生类的基类大于一个

语法:



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6653301#)[copy](http://blog.csdn.net/insistgogo/article/details/6653301#)

- class  派生类名:继承方式1 基类名1，继承方式2 基类名2...  
- {  
-     <派生类新定义成员>  
- };  


多重继承与构造函数的关系：

多重继承时构造函数的作用：

1）初始化派生类(自己)

2）调用该派生类所有基类构造函数，并且为所有基类传参（参数个数必须包含所有基类所需参数）

构造函数语法：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6653301#)[copy](http://blog.csdn.net/insistgogo/article/details/6653301#)

- 派生类构造函数名(总参数表列): 基类1构造函数(参数表列), 基类2构造函数(参数表列), 基类3构造函数(参数表列)  
- {  
- //派生类中新增数成员据成员初始化语句
- }  


说明：派生类的构造函数的成员初始化列表中必须列出对虚基类构造函数的调用；如果未列出，则表示使用该虚基类的缺省构造函数。 

具体点来说：初始化列表中要包括对 直接基类 + 虚基类 进行调用。

构造函数的执行次序(不含虚基类)：

(1)基类：依派生的次序决定，与构造函数中书写顺序无关

(2)子对象的构造函数

(3)派生类的构造函数

析构函数的执行次序：和上述执行顺序相反

注意：

1）析构函数能继承；

2）派生类中要定义自己的析构函数释放在派生中新增的成员；

3）从基类中继承的成员释放，可以通过基类的析构函数实现；

4）激活析构函数的顺序与构造函数缴活顺序相反。

举例：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6653301#)[copy](http://blog.csdn.net/insistgogo/article/details/6653301#)

- #include <iostream>
- usingnamespace std;  
- class A  
- {  
- public:  
-     A()  
-     {  
-         cout<<"调用A的构造函数"<<endl;  
-     }  
- };  
- class B  
- {  
- public:  
-     B()  
-     {  
-         cout<<"调用B的构造函数"<<endl;  
-     }  
- };  
- class C:public A,public B //这里声明顺序决定了调用基类的顺序
- {  
- private:  
-     A a;  
- public:  
-     C()  
-     {  
-         cout<<"调用C的构造函数"<<endl;  
-     }  
- };  
- void main()  
- {  
-     C c;  
-     system("pause");  
- }  


运行结果：

调用A的构造函数--C的基类

调用B的构造函数--C的基类

调用A的构造函数--C的对象成员

调用C的构造函数--C自己的构造函数

说明：

1、继承/多重继承一般是公有继承，保护继承/私有继承只是在技术讨论较多，实际使用较少。

多继承中同名覆盖原则

当派生类与基类中有同名成员时：

调用派生类的成员：定义派生类对象，直接调用同名函数即可，而自动屏蔽基类的同名函数。

访问基类中成员：应使用基类名限定。

举例：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6653301#)[copy](http://blog.csdn.net/insistgogo/article/details/6653301#)

- #include<iostream>
- usingnamespace std;  
- class A  
- {  
- public:  
- void f()  
-     {  
-         cout<<"调用A的构造函数"<<endl;  
-     }  
- };  
- class B  
- {  
- public:  
- void f()  
-     {  
-         cout<<"调用B的构造函数"<<endl;  
-     }  
- };  
- class C:public A,public B  
- {  
- public:  
- void f()  
-     {  
-         cout<<"调用C的构造函数"<<endl;  
-     }  
- };  
- void main()  
- {  
-     C c;  
-     c.f();//覆盖基类中f函数
-     c.B::f();//通过基类名限制访问
-     c.A::f();  
-     system("pause");  
- }  


多继承带来的二义性：

当派生类从多个基类派生，而这些基类又从同一个基类派生，则在访问此共同基类中的成员时，将产生二义性

即：派生类从同一个基类，沿不同继承方向，得到多个相同的拷贝,不知道要访问哪一个，就产生了二义性。

二义性的常用解决方法：使用作用域运算符(类名::)来解决访问二义性问题使用访问，但是这里的成员都是来源于同一个基类，这时是不能解决问题的，这里就引入虚基类

虚基类：

虚基类的作用：使公共基类只产生一个拷贝，即只对第一个调用的有效，对其他的派生类都是虚假的，没有调用构造函数

使用场合：用于有共同基类的场合

原理：让虚基类的构造函数只执行一次，派生类只得到一套虚基类的成员

语法：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6653301#)[copy](http://blog.csdn.net/insistgogo/article/details/6653301#)

- class 派生类名：virtual 继承方式 类名  //在派生类定义的时候写。
- {  
- 
- }  


注意：声明后，当基类通过多条派生路径被一个派生类继承时，该派生类只继承该基类一次

虚基类的初始化：与一般多继承的初始化在语法上是一样的,但构造函数的调用次序不同. 

派生类构造函数的调用次序:（**先虚基类，后基类，再成员对象，最后自身**）

(1)对虚基类间的构造函数的顺序：根据虚基类间继承的顺序调用

(2)对基类间的构造函数的顺序：根据基类间继承的顺序调用

(3)对成员对象的构造函数的顺序：根据成员对象在类中声明顺序调用

(4)若同一层次中包含多个虚基类,这些虚基类的构造函数按它们说明的次序调用;

(5)若虚基类由非虚基类派生而来,则仍先调用基类构造函数,再调用派生类的构造函数.

举例：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6653301#)[copy](http://blog.csdn.net/insistgogo/article/details/6653301#)

- class  A :  public B, public C,virtualpublic D  
- {}  
- X one;  
- 将产生如下调用次序：  
- D()  
- B()  
- C()  
- A()  


说明：

1)D是A的虚基类，故先调用D的构造函数

2)在同层次的多个虚基类中，从左到右调用，先B到C

3)基类构造函数调用完后，在调用A的构造函数

举例：使用虚基类和不使用虚基类的说明：

错误代码：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6653301#)[copy](http://blog.csdn.net/insistgogo/article/details/6653301#)

- #include<iostream>      
- usingnamespace std;  
- class A  
- {  
- protected:  
- int a;  
- public:  
-     A(int a)  
-     {  
- this->a=a;  
-     }  
- };  
- class B1: public A  
- {  
- public:  
-     B1(int a):A(a)  
-      {  
-      }  
- };  
- class B2: public A  
- {  
- public:  
-     B2(int a):A(a)  
-     {  
- this->a=a;  
-     }  
- };  
- class C:public B1,public B2  
- {  
- public:  
-     C(int a):B1(a),B2(a) //没有使用虚基类，声明时，只写C的直接基类B1和B2，不写虚基类构造函数A
-     {  
- 
-     }  
- void display()  
-     {  
-         cout<<"a="<<a<<endl;//使用A::a区分也不行，这里的a是从A得到的，B1和B2都继承到了，到D中就有两份拷贝，都来自A产生歧义
-     }  
- };  
- void main()  
- {  
-     D d(1);  
-     d.display();  
-     system("pause");  
- }  


基类的层次图：

![](http://hi.csdn.net/attachment/201108/2/0_1312264534L371.gif)

正确代码：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6653301#)[copy](http://blog.csdn.net/insistgogo/article/details/6653301#)

- #include<iostream>
- usingnamespace std;  
- class A  
- {  
- protected:  
- int a;  
- public:  
-     A(int a)  
-     {  
- this->a=a;  
-     }  
- };  
- class B:virtualpublic A  
- {  
- public:  
-     B(int a):A(a)  
-      {  
-      }  
- };  
- class C:virtualpublic A  
- {  
- public:  
-     C(int a):A(a)  
-     {  
- this->a=a;  
-     }  
- };  
- class D:public B,public C  
- {  
- public:  
-     D(int a):B(a),C(a),A(a)//使用虚基类，声明时是 D的直接基类B和C + 直接基类的共同基类A
-     {  
- 
-     }  
- void display()  
-     {  
-         cout<<"a="<<a<<endl;  
- //使用虚基类时，A只执行一次，调用B的时候调用一次虚基类，调用C时，就没有调用A了，D中的a也只有一个拷贝，因而不产生歧义
-     }  
- };  
- void main()  
- {  
-     D d(1);  
-     d.display();  
-     system("pause");  
- }  


使用虚基类后的层次图：

![](http://hi.csdn.net/attachment/201108/2/0_13122646183qdN.gif)

注意：

1、派生类构造函数初始化列表对虚基类的处理：

有虚基类和没费基类两种情况下，派生类构造函数的书写情况是不一样的，上面注释有代码。

没有虚基类的多继承，派生类构造函数的声明只包括其直接的基类

有虚基类的多继承，派生类构造函数的声明不仅包含其直接基类还要包含直接基类的虚基类。

2、

1）运行时，C创建对象时，先找到直接基类B1，调用直接基类B1的构造函数时，又调用A的构造函数，无基类，直接调用

2）之后，在调用B1的构造函数，之后再调B2的构造函数时，发现有基类A，但是A为虚基类，已经调用过一次，不再调用

3）之后，直接调用B2的构造函数，完了，就直接调用C的构造函数

说明：

1、虚基类怎么保证初始化派生类对象时，只被调用一次？

因为：初始化列表中要包括对 直接基类 + 虚基类 进行调用，但仅仅用建立对象的最远派生类的构造函数调用虚基类的构造函数，而该派生类的所有基类中列出的对虚基类

的构造函数的调用在执行中被忽略，即对其他基类来说，这个基类是虚假的，而不再调用虚基类，从而保证对虚基类子对象只初始化一次。

2、一个类可以在一个类族中既被用作虚基类，也被用作非虚基类。




