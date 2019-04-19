# C++精进篇（八）之―函数重定义、重载、重写、重入 - fanyun的博客 - CSDN博客
2016年03月26日 09:17:16[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1467
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
　C++函数重定义、重载、重写、重入
1. 重写 (override): 
       父类与子类之间的多态性。子类重新定义父类中有相同名称和参数的虚函数。
       函数重写的特点：
（1）不同的范围，分别位于基类和派生类中
（2）重写函数必须有相同的类型，名称和参数列表 (即相同的函数原型)
（3）重写函数的访问修饰符可以不同。尽管 virtual 是 private 的，派生类中重写改写为 public,protected 也是可以的
（4）被重写的函数不能是 static 的。基类函数必须有virtual关键字 ( 即函数在最原始的基类中被声明为 virtual ) 。
      特殊情况：若派生类重写的虚函数属于一个重载版本，则该重写的函数会隐藏基类中与虚函数同名的其他函数。
      作用效果：父类的指针或引用根据传递给它的子类地址或引用，动态地调用属于子类的该函数。这个晚绑定过程只对virtual函数起作用。具体原理是由虚函数表（VTABLE）决定的。
2. 重载 (overload): 
       指函数名相同，但是它的参数表列个数或顺序，类型不同。但是不能靠返回类型来判断。
       函数重载的特点：
（1）相同的范围（在同一个类中）
（2）函数名字相同
（3）形参列表不同（可能是参数个数  or  类型  or  顺序不同），返回值无要求 
      特殊情况：若某一个重载版本的函数前面有virtual修饰，则表示它是虚函数。但它也是属于重载的一个版本
     不同的构造函数（无参构造、有参构造、拷贝构造）是重载的应用
     作用效果和原理：编译器根据函数不同的参数表，将函数体与函数调用进行早绑定。重载与多态无关，只是一种语言特性，与面向对象无关。
（4）virtual关键字可有可无
3. 重定义 (redefining): 
       子类重新定义父类中有相同名称的非虚函数 ，重定义(隐藏)指派生类的函数屏蔽了与其同名的基类函数( 参数列表可以不同 ) 。
（1）如果派生类的函数和基类的函数同名，但是参数不同，此时，不管有无virtual，基类的函数被隐藏。
（2）如果派生类的函数与基类的函数同名，并且参数也相同，但是基类函数没有vitual关键字，此时，基类的函数被隐藏。
       函数重定义的特点：
（1）不在同一个作用域（分别位于基类、派生类）  
（2）函数的名字必须相同  
（3）对函数的返回值、形参列表无要求
     特殊情况：若派生类定义的该函数与基类的成员函数完全一样（返回值、形参列表均相同），且基类的该函数为virtual，则属于派生类重写基类的虚函数。
     作用效果：若重新定义了基类中的一个重载函数，则在派生类中，基类中该名字的函数（即其他所有重载版本）都被自动隐藏，包括同名的虚函数。
4. 重写与重载的区别 (override) PK (overload)
（1） 方法的重写是子类和父类之间的关系，是垂直关系；方法的重载是同一个类中方法之间的关系，是水平关系。
（2） 重写要求参数列表相同；重载要求参数列表不同。
（3） 重写关系中，调用那个方法体，是根据对象的类型（对象对应存储空间类型）来决定；重载关系，是根据调用时的实参表与形参表来选择方法体的。 
示例1：
```cpp
class Base
{
   private:
      virtual voiddisplay() { cout<<"Base display()"<<endl; }
      voidsay(){  cout<<"Base say()"<<endl;  }
   public:
      void exec(){display(); say(); }
      voidf1(string a)  { cout<<"Base f1(string)"<<endl; }
      void f1(inta) { cout<<"Base f1(int)"<<endl; }   //overload
};
class DeriveA:public Base
{
   public:
      voiddisplay() { cout<<"DeriveA display()"<<endl;}   //override
      void f1(inta,int b) { cout<<"DeriveA f1(int,int)"<<endl;}   //redefining
      void say() {cout<<"DeriveA say()"<<endl; }   //redefining
};
class DeriveB:public Base
{
   public:
      void f1(inta) { cout<<"DeriveB f1(int)"<<endl; }  //redefining
};
  int main()
{
   DeriveA a;
   Base *b=&a;
   b->exec();       //display():version of DeriveAcall(polymorphism)                          
               //say():version of Base called(allways )
  a.exec();        //same result as laststatement
   a.say();
  //a.f1(1);         //error:no matchingfunction, hidden !!
   DeriveB c;
  c.f1(1);         //version of DeriveBcalled
}
```
示例2：
```cpp
#include <iostream>
#include <windows.h>
using namespace std;
class Base
{
public:
	int a;
	Base()
	{
		cout << "Base() 无参构造函数！" << endl;
	}
	Base(int data)
	{
		a = data;
		cout << "Base(int data) 有参构造函数！" << endl;
	}
	Base(const Base &tmp)
	{
		a = tmp.a;
		cout << "Base 拷贝构造函数！！" << endl;
	}
	//基类中对函数名fuc1，进行了重载。其中最后一个重载函数为虚函数
	void fuc1()const
	{
		cout << "调用 void Base::fuc1()" << endl;
	}
	//overload
	int fuc1(int data) const
	{
		cout << "调用 Base fuc1(int data)" << endl;
		return 1;
	}
	//overload    虚函数
	virtual double fuc1(int dataA, int dataB)
	{
		cout << "调用 Base fuc1(int a,int b)" << endl;
		return dataA*dataB / 2.0;
	}
};
class  Derived : public Base
{
public:
	//先调用基类的构造函数，然后对象成员的构造函数，最后才是派生类的构造函数
	Base base;
	Derived()
	{
		cout << "Derived() 无参构造函数被调用！" << endl;
	}
	//对基类的函数名fuc1，进行了重定义。则会隐藏基类中的其他fuc1函数
	int  fuc1() const
	{
		cout << " 调用 Derived fuc1()函数" << endl;
		return 1;
	}
	//重写基类的虚函数
	//redefine   override
	double fuc1(int dataA, int dataB)
	{
		cout << "调用 Derived fuc1(int dataA,int dataB)函数" << endl;
		return (dataA + dataB) / 2.0;
	}
};
int main()
{
	//-----test 1------------------------
	cout << "-------test 1------------" << endl;
	//Base class
	Base base;
	base.fuc1();
	base.fuc1(12);
	base.fuc1(10, 20);
	//derived class
	Derived derived;
	derived.fuc1();
	//derived.fuc1(2); //error C2661: 'fuc1' : no overloaded function takes 1 parameters
	derived.fuc1(10, 20);
	//--------test 2----------------------------------
	cout << "-------test 2------------" << endl;
	Base b = derived;
	b.fuc1();
	b.fuc1(10, 20);//调用的是基类的函数，不发生多态
	//--------test 3----------------------------------------
	cout << "-------test 3------------" << endl;
	Base &bR = derived;//引用
	b.fuc1();//fuc1()不是虚函数，调用基类的函数
	bR.fuc1(10, 20);//调用的是派生类的函数，发生多态
	//--------test 4--------------------------------------
	cout << "-------test 4------------" << endl;
	Base* pB = &derived;
	b.fuc1();
	pB->fuc1(10, 20);//调用的是派生类的函数，发生多态
	Sleep(50000);
	return 1;
}
```
         分析：test 1中进行了重载测试，根据传递参数的不一样，调用不同的函数  （早绑定，与多态无关）。test 2中Test b = d;定义了一个基类对象，用派生类对象来进行初始化。这会调用基类的拷贝构造函数，生成基类的对象b，基类的拷贝构造函数初始化b的VPTR，指向b的VTABLE。因此所有的函数调用都只发生在基类，不会产生多态。
 这是一个对象切片过程（参见《C++编程思想.第二版》），对象切片是当它拷贝到一个新的对象时，会去掉原来对象的一部分，而不是像使用指针或引用那样简单地改变地址的内容。test 3和test 4中，定义的基类指针和引用，故会发生多态。
        注意：在 C++ 中若基类中有一个函数名被重载，在子类中重定义该函数，则基类的所有版本将被隐藏——即子类只能用子类定义的，基类的不再可用。基类class Base中定义了名为fuc1的3个重载函数，其中最后一个是虚函数; 派生类class
  Derived中对fuc1进行了重定义，所以会隐藏基类中名为fuc1的版本。其中派生类的double fuc1(int dataA,int dataB)属于对虚函数的重写。
5. 函数重入
      一个函数在执行的过程中，还可以被另外一个地方调用执行。比如递归函数调用，多线程的多个线程同时执行一个函数。重入也就是同时执行一个函数的意思。而可重入表示的是一个函数是否可以同时执行而不出错。如果一个函数执行过程会记录状态，比如使用全局变量记录，或者使用类成员变量记录，然后根据状态执行不同的代码，这样就会导致第一个执行的和第二个同时执行的效果不一样，因为第一个执行的已经改变了状态，导致第二个执行的就是另外一个效果。这样的表示不可重入。
     所以，要让函数重入，请保证不在函数中不可记录状态、将函数独立成一个完整功能体，不与外部交涉。传进去一个参数一定得到一个固定的结果，不管是多少个同时执行都如此。
