# C++ 继承与派生 - 在思索中前行！ - CSDN博客





2015年03月30日 22:45:18[_Tham](https://me.csdn.net/txl16211)阅读数：591








### **继承的意义？**

使程序的设计更符合发展规律，即事物的发展是一个从低级到高级的发展过程，类的继承也是反映由原始的简单代码到丰富的高级代码的过程。它能帮助我们描述事物的层次关系，有效而精确的理解事物，理解事物直到本质。

### **什么叫继承？**

（1）继承使类与类之间建立起一种上下级的层次关系

（2）子类可以重复使用父类的操作和数据成员，子类可以声明新的属性和操作，还可以剔除不适合自己用途的父类操作。

### **为什么要使用继承？**

原因：为了提高代码复用率，缩短程序开发成本

### **继承实现的功能：**

1、继承基类的数据成员：将基类成员全盘吸收

2、增加新的数据成员、改变现有成员的属性：不同方式继承 +  声明一个同名成员，使用重写覆盖技术

3、重新定义已有成员函数

### **继承的分类：**

针对派生类而言，根据基类的个数分：单继承 +  多继承

单继承：派生类的基类只有一个

语法格式：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645215#)[copy](http://blog.csdn.net/insistgogo/article/details/6645215#)

- class 派生类名:继承方式 基类名  
- {  
- //成员声明:类似于普通类
- };  


三种继承方式：公有继承(public)，私有继承(private)，保护继承(protected)

继承后访问属性的种类：针对派生类而言，成员可分为：不可访问成员、私有成员、受保护成员、公有成员

说明：无论是那种派生方式，派生类无法直接使用不可访问变量的，换句话说，派生类不能访问基类的私有成员。

具体如图：

![](http://hi.csdn.net/attachment/201107/29/0_1311940261Yy3j.gif)























继承后的法则：

派生类的成员函数访问基类成员时，

          无论是公有、私有、受保护继承，基类的私有变量均不可访问，但继承后属性为公有、受保护成员均可以直接访问。

派生类的对象类外访问基类成员时，

         若受保护、私有继承时，派生类对象均不可访问基类的成员（无论是哪种访问属性）。

         若公有继承，派生类的对象可以访问基类的公有成员。

 一句话：就派生类而言，基类私有变量不可访问，其他成员的访问情况，按具体继承后的属性而定

根据父亲物品的使用权以及继承，换一种方式理解成员权限的定义：

父亲的东西可以分为几类，只能是父母知道，子女都不可以知道。子女可以知道，但是外人不可以知道。外人可以知道。

根据分类，给出哪种物品使用哪种权限限制：

--只能是父母知道，子女都不可以知道：把这部分东西定义为Private。

--子女可以知道，但是外人不可以知道：把这部分定义为protected。

--外人可以知道：把这部分内容定义为Public

最后根据那些继承方式，对父类物品的访问类型进一步约束，形成自己的访问法则。

注意，无论是哪一种继承方式，继承方式对父亲的孩子能访问父亲哪些的物品不能访问哪些物品是没有限制的，继承方式是为了限制后来的继承。

即无论是哪一种继承方式，子类成员都不可以访问私有成员，但都可以访问父类的受保护和公有成员。当自己作为父类时，根据自己从父亲那里的继承权限，在就会限制自己的子类访问自己的父亲的权限。

调整访问控制

前提：对于在派生类中可以看见的成员（父类除私有成员外的成员），

目标：在派生类中，将基类中的私有成员调整为公有成员，将公有成员变成私有成员





**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645215#)[copy](http://blog.csdn.net/insistgogo/article/details/6645215#)

- class BaseClass  
- {  
- public:  
- int nPublicA;  
- protected:  
- int nProtectedA;  
- private:  
- int nPrivateA;  
- };  
- 
- class DerivedClass : public BaseClass  
- {  
- public:  
- using BaseClass::nPrivateA; /*错误,父类私有成员在子类不可见*/
- using BaseClass::nProtectedA;/*正确,父类受保护成员变公有成员*/
- private:  
- using BaseClass::nPublicA;/*正确,父类公有成员变私有成员*/
- };  


### **派生类的构造函数和析构函数**

类一旦被创建，C++编译器会为其产生四个缺省函数，默认的无参构造函数，默认的拷贝构造函数，默认的析构函数，默认的赋值函数。


对于构造函数：

若父类的构造函数无参，则子类的构造函数可以无参，若父类的构造函数有参，则子类的构造函数必有参，可以使用参数列表为基类构造函数传参。

对于拷贝构造函数和赋值函数：

由于这两类函数的函数名和参数都是确定的，所以不涉及由子类为父类传参的情况。

一句话：无论是父类还是基类，只要有自定义的拷贝构造函数，则在调用拷贝构造函数时，就调用自定义的，否则调用默认的。

若父类没有自定义拷贝构造函数，则子类的拷贝构造函数将调用父类的默认拷贝构造函数。

若父类有自定义的拷贝构造函数，则子类的拷贝构造函数将调用父类的自定义的拷贝构造函数。

注意，对于父类和子类之间的拷贝和赋值时，由于子类中包含父类中的成员，可以按照参数对应赋值




对象构造顺序：

        创建对象时，先基类，再对象类成员的构造函数，后派生类 

        撤销对象时，先派生类，再对象类成员的析构函数，后基类

如果，在类中有多个对象成员，则其调用构造函数的顺序和类中对象声明的顺序有关，而和在参数列表中的顺序无关。析构函数的调用顺序和构造函数的顺序相反。

语法：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645215#)[copy](http://blog.csdn.net/insistgogo/article/details/6645215#)

- 派生类名(派生类构造函数参数表)：基类名(参数)，子对象名(参数)  
- {  
- //派生类的数据成员初始化
- }  


为基类传参：使用参数列表的形式初始化对象

举例：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645215#)[copy](http://blog.csdn.net/insistgogo/article/details/6645215#)

- #include <iostream>
- usingnamespace std;  
- class Base  
- {  
- private:  
- int i;  
- public:  
-     Base(int )  
-     {  
-         cout<<"调用基类构造函数"<<endl;  
-     }  
- };  
- class Derived:public Base  
- {  
- private:  
- int j;  
-     Base B;//基类对象
- public:  
-     Derived(int i,int j):Base(i),B(i)  
-     {  
- this->j=j;  
-         cout<<"调用派生类的构造函数"<<endl;  
-     }  
- };  
- void main()  
- {  
-     Derived d(1,2);  
-     system("pause");  
- }  


运行结果：

调用基类构造函数

调用基类构造函数

调用派生类的构造函数

### 同名覆盖与重写：

产生原因：派生类定义的成员与基类中的成员同名

                    如果是函数的话，函数名和参数完全相同

结果：派生类的成员覆盖了基类的同名成员

具体来数，直接使用派生类对象调用成员时，只能调到派生类自己的，而无法访问基类成员

如果要访问基类被覆盖的成员，则需要加上 类名::成员 访问

例子：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645215#)[copy](http://blog.csdn.net/insistgogo/article/details/6645215#)

- #include <iostream>
- usingnamespace std;  
- class Base  
- {  
- public:  
- void a()  
-     {  
-         cout<<"调用基类的成员函数"<<endl;  
-     }  
- };  
- class Derived:public Base  
- {  
- public:  
- void a()  
-     {  
-         cout<<"调用派生类的成员函数"<<endl;  
-     }  
- };  
- void main()  
- {  
-     Derived d;  
-     d.a(); //调用的是派生类的成员函数，这时会覆盖基类的成员函数
-     d.Base::a();//调用基类的成员函数
-     d.Derived::a();//调用派生类的成员函数
-     system("pause");  
- }  


调用结果：

调用派生类的成员函数

调用基类的成员函数

调用派生类的成员函数

### 派生类为基类赋值：

定义：在公有派生情况下，一个派生类的对象可用于基类对象可使用的任何地方

换句话说：公有派生情况下，一个派生类对象可以直接作为基类的对象使用

具体分3仲情况：

1）  派生类的对象 给 基类对象 赋值

        结果：基类对象能够访问派生类中从基类继承的成员

2）  派生类的对象 给 基类对象的引用 赋值

        结果：基类对象只可以访问派生类中基类的成员，看不到派生类之外的成员，也就不可访问派生类自己定义的成员

3）  派生类的对象 给 基类对象的指针 赋值

        结果：基类指针只可以访问派生类中基类的成员，派生类之外成员不可访问

若使基类指针访问派生类自己的成员，则必须使用显式类型转换，把基类指针显示转换为派生类指针，这时可以访问所有派生类成员。

举例：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645215#)[copy](http://blog.csdn.net/insistgogo/article/details/6645215#)

- #include <iostream>
- usingnamespace std;  
- class Base  
- {  
- public:  
- void a()  
-     {  
-         cout<<"调用基类的成员函数"<<endl;  
-     }  
- };  
- class Derived:public Base  
- {  
- public:  
- void a()  
-     {  
-         cout<<"调用派生类的成员函数"<<endl;  
-     }  
- };  
- void main()  
- {  
-     Base b;  
-     Base *B;  
-     Derived d;  
-     B=&b;  
-     B->a();        //基类指针指向基类，调用的都是基类函数
-     B=&d;   
-     B->a();        //基类指针指向派生类，调用的是派生类中从基类继承的函数
-     b=d;    
-     b.a();         //使用派生类为基类赋值,调用的是派生类中从基类继承的函数
- //  Derived *D=&b; //错误，不能用基类为派生类指针赋值
- //  d=b;           //错误，不能用基类为派生类赋值
-     Base *E;  
-     E=&d;          //必须要先赋值啊，这时派生类指针E才不会乱指。
-     ((Derived*)E)->a();//对基类指针强制转换为指向派生类，这时可以指向派生类自己的成员
-     system("pause");  
- }  


运行结果：

调用基类的成员函数

调用基类的成员函数

调用基类的成员函数

### 注意：

1、在公有继承下，为什么一个指向基类的指针可以指向其公有派生类的对象，但是指向派生类的指针不能指向一个基类的对象？

因为：派生类指向基类时，派生类能力减弱，不行的。

            基类指向派生类时，不是能力扩大，实际上还是指向派生类中包含的基类。

2、为什么前提必须是公有派生？

因为：公有派生：派生后类外还是类可以直接访问，私有或受保护派生：类不可以直接访问基类成员，就没什么意思了

3、引入protected成员的原因：

若一个类，将外界能够访问的操作都公有化了，所有不能被外界访问的成员都私有化，这样一来，在后继的类中，就没有对基类任何可以悄悄改进的余地了，即无法对类中某些功能可扩展的变量扩展功能。所以，继承也需要这样的成员，它们对外界是私有的，对派生的子女是允许访问的。这种设计需求就是访问控制符protected。

此时，类中private成员是一种隐私，子女以及外来人员谁都不能动的

    类中protected成员是基类想后代开放的成员，便于继承者基于此而改进，基类为了长远考虑，可以留下保护成员，但派生类简单而清晰的设计应该是不使用任何保护成员，即只使用公有成员。




4、使用派生类对象对基类对象、基类对象引用、基类对象指针赋值时，是否调用拷贝构造函数？

由于派生类对象包含的对象实体拥有基类对象包含的实体对象，因此可以使用派生类对象对基类对象、基类对象引用、基类对象指针赋值。

（1）在创建对象时，使用派生类对象为基类对象赋值，需要调用拷贝构造函数或赋值函数


（2）由于基类对象引用、基类对象指针只是获得了派生类中基类部分的地址，即取了个别名，此时没有调用拷贝构造函数。



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645215#)[copy](http://blog.csdn.net/insistgogo/article/details/6645215#)

- #include <iostream>
- #include <string>
- usingnamespace std;  
- 
- class BaseClass  
- {  
- public:  
-     BaseClass(string baseA)  
-     {  
-         m_strBaseA = baseA;  
-     }  
- 
-     BaseClass(const BaseClass& other)  
-     {  
-         m_strBaseA = other.m_strBaseA;  
-     }  
- 
- void show()  
-     {cout<<"Base: "<<m_strBaseA<<endl;}  
- private:  
-     string m_strBaseA;  
- };  
- 
- class DerivedClass : public BaseClass  
- {  
- public:  
-     DerivedClass(string strBaseA,string strDerivedA):BaseClass(strBaseA)  
-     {  
-         m_strDerivedA = strDerivedA;  
-     }  
- void show()  
-     {cout<<"Derived: "<<m_strDerivedA<<endl;}  
- private:  
-     string m_strDerivedA;  
- };  
- 
- /*可以使用派生类对基类进行直接复制*/
- int main()  
- {  
- /*
-     使用拷贝构造函数，用派生类对基类赋值
-     由于derive的对象实体包含baseOne的对象实体，
-     使用derive对baseOne赋值，就是将derive中的Base对象实体复制给baseOne
-         **注意：这个过程调用了拷贝构造函数
-     */
-     DerivedClass derive("Base","Derived");  
-     BaseClass baseOne(derive);   
-     derive.show();  
-     baseOne.show();  
- 
- /*
-     使用派生类对象初始化基类对象的引用
-     等价于 baseTwo 是 derive中的Base对象实体的别名
-         **注意：这个过程没有调用了拷贝构造函数
-     */
-     BaseClass& baseTwo = derive;  
-     derive.show();  
-     baseTwo.show();  
- 
- /*
-     使用派生类对象初始化基类对象的指针
-     等价于 baseThree 是 derive中的Base对象实体的别名
-         **注意：这个过程没有调用了拷贝构造函数
-     */
-     BaseClass* baseThree = &derive;  
-     derive.show();  
-     baseThree->show();  
- 
-     system("pause");  
- return 1;  
- }  

![](http://blog.csdn.net/insistgogo/article/details/6645215)
![](https://img-blog.csdn.net/20130515111807767)

5 父类和子类相互转换时的内存状况



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6645215#)[copy](http://blog.csdn.net/insistgogo/article/details/6645215#)

- Student:父类  
- GraduateStudent:子类  
- 
- 
- GraduateStudent gs;  
- (1)Student s = gs;    // 正确
- (2)Student& t = gs;   //正确
- (3)Student* p = &gs;  //正确
- 
- 
- (4)GraduateStudent gs = s;    // 错误
- (5)Graduatestudent* pGS = &s;   //错误  
- 
- 
- (6)Student* pS = reinterpret_cast<Student*>(&gs);  // 正确
- (7)GraduateStudent* pGS = reinterpret_cast<GraduateStudent*>(&s);  // 错误 

分析一：（1）正确，但（4）不正确。这是因为在子类对象中包含有父类对象的实体，可以拿出来父类部分为父类对象赋值。但是基类中数据不充分，不含有子类中的全部信息，所以拒绝执行父类对子类的赋值。对于指针，也是同样原因。（3）正确，（5）错误。原因同上。
分析二：（1）（2）（3）正确，表示子类对象可以为父类对象、父类对象引用、父类对象指针赋值。其中（1）需要调用拷贝函数，而（2）（3）不需要调用拷贝构造函数。


分析三：（6）和（3）一样，而且都正确。原因同分析一。即可以使用子类指针为父类指针赋值。

分析四：（7）和（5）一样，都错误。原因同分析一。即不可以使用父类指针为子类指针赋值。

内存图:

![](https://img-blog.csdn.net/20130516100105685)


从内存图可以看出，

1、父类对象s复制了子类对象gs中的Student部分，构成了名副其实的Student对象实体。

2、父类对象的引用 t 只是引用了子类对象中的Student部分。

3、父类对象的指针 p 指向子类对象gs的首地址，它恰好是子类对象中的Student部分的首地址。

4、子类指针可以强制类型转化给父类指针。这是由于在父类GraduateStudent中，学生对象的地址和研究生对象的地址是重合的，研究生对象指针（子类指针）转化为学生指针（父类指针）时，只是把指针类型换了换而已，而指针的地址即指针指向没有改变。所以，上例中（7）也是错的，因为学生对象和研究生对象的地址不重合。



