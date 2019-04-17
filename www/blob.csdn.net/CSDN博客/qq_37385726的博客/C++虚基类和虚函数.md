# C++虚基类和虚函数~ - qq_37385726的博客 - CSDN博客





2017年05月09日 01:17:25[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：281








一：虚基类             

我们一般说定义虚基类，是因为为了去解决二义性的问题（在菱形结构中，由于一级派生类函数重名，二级派生类对象调用同名函数时程序产生两种或多种可能，有歧义，就产生了二义性问题；或者像基类的数据成员在二级派生类对象定义的时候申请了两份内存空间，所以程序产生两种或多种可能，有歧义========所以二义性的问题说得直白点就是说程序在运行时产生两种或两种以上的可能，有歧义）

对于菱形结构会出现二义性问题，一级派生类要将基类定义成虚基类，才能去解决二义性问题。

eg1:                                                                             Base 1                                Base2



                                                                                            Level1                 Level2



                                                                                                             Leaf

关系：Level1是Base1和Base2的派生类，Level2是Base1和Base2的派生类，Leaf是Level1和Level2的派生类。

(上述关系为“双菱形结构”，所以一级派生类要将所有基类全定义成虚基类)




代码如下：




```cpp
#include
using namespace std;
#include
class Base1
{
public:
	Base1();
};
class Base2
{
public:
	Base2();
};
class Level1 :virtual public Base1, virtual public Base2   //一级派生类要将所有公共基类声明为虚基类
{
public:
	Level1();
};
class Level2 :virtual public Base1, virtual public Base2  //一级派生类要将所有公共基类声明为虚基类
{
public:
	Level2();
};
class Leaf :public Level1, public Level2
{
public:
	Leaf();
};
//=====================================================
Base1::Base1()
{
	cout << "Base1" << endl;
}
Base2::Base2()
{
	cout << "Base2" << endl;
}
Level1::Level1()
{
	cout << "Level1" << endl;
}
Level2::Level2()
{
	cout << "Level2" << endl;
}
Leaf::Leaf()
{
	cout << "Leaf" << endl;
}
//=========================================
int main(void)
{
	Leaf one;
	system("pause");
	return 0;
}
```





输出结果如下：

![](https://img-blog.csdn.net/20170509002615671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





从输出结果我们也可以知道一点知识，再补充明确两点：

1.构造函数调用顺序：基类>子对象>自己

2.析构函数调用顺序：自己>子对象>基类

3.构造函数初始化顺序：基类>子对象>自己




eg2:                            公共基类：                                                 Furniture                                          数据成员：weight，成员函数：show




                                   一级派生类：                          Sofa                                            Bed                   数据成员：weight，成员函数：show






                                   二级派生类：                                                SofaBed                                         数据成员：weight，成员函数：show









（上述关系为标准的菱形结构,为了避免二义性的问题，应将公共基类定义成虚基类）

代码如下：




```
#include
using namespace std;
#include
class Furniture
{
public:
	Furniture(){};
	Furniture(double weight);
	void show(void);		
protected:
	double weight;
};
class Sofa:virtual public Furniture
{
public:
	Sofa(){};
	Sofa(double weight);
	void show(void);
};
class Bed :virtual public Furniture
{
public:
	Bed(){};
	Bed(double weight);
	void show(void);
};
class SofaBed:public Sofa,public Bed
{
public:
	SofaBed(){};
	SofaBed(double weight);
	void show(void);
};
//=====================================================
Furniture::Furniture(double weight)
{
	this->weight = weight;
}
void Furniture::show(void)
{
	cout << "家具重量为:" << weight << endl;
}
Sofa::Sofa(double weight):Furniture(weight)  //派生类的构造函数应该去初始化基类的数据成员
{
	this->weight = weight;
}
void Sofa::show(void)
{
	cout << "沙发重量为:" << weight << endl;
}
Bed::Bed(double weight):Furniture(weight)  //派生类的构造函数应该去初始化基类的数据成员
{
	this->weight = weight;
}
void Bed::show(void)
{
	cout << "床重量为:" << weight << endl;
}
SofaBed::SofaBed(double weight) :Furniture(weight),Sofa(weight), Bed(weight)
{
	this->weight = weight;
}
void SofaBed::show(void)
{
	cout << "沙发床重量为：" << weight << endl;
}
//=========================================
int main(void)
{
	Furniture furniture(1);
	Sofa sofa(2);
	Bed bed(3);
	SofaBed sofabed(3.5);  //如果上面定义的三个对象都没有初始化，那么这里定义的softbed对象初始化为3.5并不能初始化上面三个变量
			    //虽然它的构造函数里面初始化了基类的构造函数，但是我们要知道，这两者之间有本质的不同
	            //上面三个对象定义的位置有softbed不同，所以自然不可能初始化到它
				//并且一个对象的初始化是在这个对象建立的时候调用其构造函数就做完了的，故，这里如果没有初始化，就调用默认构造函数，系统随机赋值
	Furniture *p;
	sofabed.show();
	p = &furniture;
	p->show();
	p = &sofa;
	p->show();
	p = &bed;
	p->show();
	system("pause");
	return 0;
}
```





运行结果：

![](https://img-blog.csdn.net/20170509010358721?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





从这个运行结果要看到一个很重要的事情：

当公共基类的同名函数不是虚函数时，即在其一级派生类和二级派生类里的同名函数对于公公基类来说是隐藏函数，公共基类的同名函数是被隐藏函数。

通过公共基类的指针去依次调用show函数，显示的结果一定是从公共基类继承来的信息，像自己的“沙发的重量为：”这种是不会有的，因为调用的是公共基类的show函数。




PS：但是如果在公共基类里的show函数是虚函数（那么这时候在其一级派生类和二级派生类里的同名函数对于公公基类来说是覆盖函数，公共基类的同名函数是被覆盖函数），通过公共基类的指针去依次访问show函数，结果如下：

![](https://img-blog.csdn.net/20170509011015671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





即由结果也可以看到，当公共基类为虚函数时，即同名函数之间的关系是覆盖与被覆盖时，通过公共基类的指针去调用show函数，调用的结果是各个类对象自己定义的show函数，而不是只能为公共基类的show函数。




二：虚函数

①虚函数存在的意义是：

1.完成动态联编，动态调用

2.使同名函数调用通过一个统一的接口去调用，更具有整体性




②虚函数定义需遵循的规则

1.在基类和派生类之间定义同名函数，基类必须加上关键字virtual，派生类可加可不加，但是函数原型一定要保证与基类同名函数的函数原型完全一致。（派生类不加virtual也可以是虚函数，这是利用了虚函数的传递性）

2.虚函数必须是成员函数，不能是非成员函数。

3.虚函数仅适用于有继承关系的类对象。

4.虚函数不能是静态成员函数，因为静态成员函数是编译的时候就分配了空间，是属于早期联编的，与虚函数动态联编，晚期联编矛盾。

5.虚函数不能是内联函数，因为内联函数在编译的时候就采用插入函数体的方法，所以也是静态联编的。

6.虚函数不能是构造函数，因为构造函数其实是一块未定型空间，只有在对象创建的时候，对象才是类的实例。（虚函数要分配定型空间）

7.虚函数可以是析构函数，尤其是在派生类有指针变量的时候，为了解决析构不完全的问题，应该将析构函数定义成虚函数。（关于虚析构函数我们一会就介绍）




③为了实现虚函数的实现动态联编的功效，以下两点均必不可少：

1.基类一定要加virtual关键字，派生类函数原型一定要与基类的同名函数完全一致，一模一样。

2.一定要通过基类的指针对象去调用虚函数，使用对象调用是静态联编。

！！！！！！！！！！！！！！！上述两个条件缺一不可




④虚函数的传递性

即在多重继承中，积累的所有派生类（包括基类的儿子，孙子，曾孙···）的原型完全相同的函数都是虚函数，具有虚函数的一切特性。

（这里有一个很重要的点值得注意：即一定要通过公有继承的方式才可以实现虚函数的传递）




⑤虚特性有哪些：

1.可以通过指针或引用的方式实现动态联编，实现动态多态性

2.可以通过基类指针或引用去调用到派生类的同名函数。（因为这时候同名函数之间的关系是覆盖与被覆盖之间的关系）

3.··························




看下面这个代码，就可以解决上面那个show不是虚函数带来的基类指针无法调用到派生类show函数的问题：




```cpp
#include
using namespace std;
#include
class Furniture
{
public:
	Furniture(){};
	Furniture(double weight);
	virtual void show(void);	//加一个virtual就好了，再利用虚函数的传递性，完美解决问题	
protected:
	double weight;
};
class Sofa:virtual public Furniture
{
public:
	Sofa(){};
	Sofa(double weight);
	void show(void);
};
class Bed :virtual public Furniture
{
public:
	Bed(){};
	Bed(double weight);
	void show(void);
};
class SofaBed:public Sofa,public Bed
{
public:
	SofaBed(){};
	SofaBed(double weight);
	void show(void);
};
//=====================================================
Furniture::Furniture(double weight)
{
	this->weight = weight;
}
void Furniture::show(void)
{
	cout << "家具重量为:" << weight << endl;
}
Sofa::Sofa(double weight):Furniture(weight)  //派生类的构造函数应该去初始化基类的数据成员
{
	this->weight = weight;
}
void Sofa::show(void)
{
	cout << "沙发重量为:" << weight << endl;
}
Bed::Bed(double weight):Furniture(weight)  //派生类的构造函数应该去初始化基类的数据成员
{
	this->weight = weight;
}
void Bed::show(void)
{
	cout << "床重量为:" << weight << endl;
}
SofaBed::SofaBed(double weight) :Furniture(weight),Sofa(weight), Bed(weight)
{
	this->weight = weight;
}
void SofaBed::show(void)
{
	cout << "沙发床重量为：" << weight << endl;
}
//=========================================
int main(void)
{
	Furniture furniture(1);
	Sofa sofa(2);
	Bed bed(3);
	SofaBed sofabed(3.5);  //如果上面定义的三个对象都没有初始化，那么这里定义的softbed对象初始化为3.5并不能初始化上面三个变量
			    //虽然它的构造函数里面初始化了基类的构造函数，但是我们要知道，这两者之间有本质的不同
	            //上面三个对象定义的位置有softbed不同，所以自然不可能初始化到它
				//并且一个对象的初始化是在这个对象建立的时候调用其构造函数就做完了的，故，这里如果没有初始化，就调用默认构造函数，系统随机赋值
	Furniture *p;
	sofabed.show();
	p = &furniture;
	p->show();
	p = &sofa;
	p->show();
	p = &bed;
	p->show();
	system("pause");
	return 0;
}
```





/*/*/*/*/*/*/*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*///*/*/*/*/*//*/*/*/*/*/*/*//*/*/*///*/*/*///***/**/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*//*//*/*//*/*//*

几个讲过的东西检验一下自己：

1.将公有继承改成受保护继承或私有继承还能够实现虚特性的传递性吗？

（不能）

2.在菱形结构下，不将公共基类设置为虚基类还能实现虚函数的传递性吗？

（可以）

3.在菱形结构下，如果公共基类的同名函数不是虚函数，在二级派生类的构造函数里还用去初始化公共基类的数据成员吗？

（不用）




/*/*/*/*/*/*/*//*/*/*/*/*//*/*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*/*//*/*//*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*/*/*/*/*//*/*/*/*/*/*//*







三：虚析构函数

再提一下上面说过的知识：构造函数不能是虚函数，析构函数可以是虚函数，并且为了保证数据成员析构完全，基类的析构函数应该要定义成虚函数。

1.如果基类的析构函数不是虚函数，通过delete基类的指针对象是没办法调用派生类的析构函数的，这个也就好像在基类的成员不是虚函数的时候，通过基类的指针是没办法调用到派生类里面定义的同名函数的

2.析构函数是虚函数时，会先调用派生类的析构函数，再调用基类的析构函数，顺序与之前说的析构函数调用顺序完全一致




来看一段代码：



```cpp
#include
using namespace std;
#include
class CPerson
{
public:
	~CPerson();
};
class Cstudent :public CPerson
{
public:
	~Cstudent();
};
//=====================================================
CPerson::~CPerson()
{
	cout << "析构CPerson" << endl;
}
Cstudent::~Cstudent()
{
	cout << "析构Cstudent" << endl;
}
//=========================================
int main(void)
{
	CPerson *p = new Cstudent;
	delete p;
	cout<<"通过指针去析构对象已析构完全"<
```





运行结果：

![](https://img-blog.csdn.net/20170509095547594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








四：纯虚函数和抽象类

①为什么有纯虚函数？

其实我们可以知道，感觉C++就是对现实生活的一个还原，我们在生活中也有抽象（与C++的抽象不同），所以为了去表达一些抽象的概念或行为，不必要求具体的实现或没有具体的物理意义，我们定义了纯虚函数。




②纯虚函数完全应该首先满足虚函数所应具有的条件



1.在基类和派生类之间定义同名函数，基类必须加上关键字virtual，派生类可加可不加，但是函数原型一定要保证与基类同名函数的函数原型完全一致。（派生类不加virtual也可以是虚函数，这是利用了虚函数的传递性）

2.虚函数必须是成员函数，不能是非成员函数

3.虚函数仅适用于有继承关系的类对象

4.虚函数不能是静态成员函数

5.虚函数不能是内联函数

6.虚函数不能是构造函数

7.虚函数可以是析构函数




③定义虚函数的格式

virtual 返回值  函数名  参数表 =0；




④抽象类

具有纯虚函数的类即为抽象类

1.抽象类不能够定义普通对象，但可以定义指针对象，作为公共接口去实现动态联编。

![](https://img-blog.csdn.net/20170509162432659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如图，CShape是抽象类

虽然可以定义CShape类型的指针，但是不能够用CShape的指针指向一个CShape类型的对象，因为这个对象根本建不起来——抽象类不能定义对象




（Ps.这里在补充说一个题外话，声明了一个函数但是没有具体实现，不具备函数功能，不能被调用，而且会报错）

/*贴出错误图【因为不容易发现】*/

![](https://img-blog.csdn.net/20170509163130028?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


show函数只声明，未定义。




2.纯虚函数也具有传递性，如果派生类原型与基类完全一致，并且将其=0，那么它一定也是纯虚函数。

但是如果实现了，只不过实现是一个空函数，那也不是纯虚函数了。






