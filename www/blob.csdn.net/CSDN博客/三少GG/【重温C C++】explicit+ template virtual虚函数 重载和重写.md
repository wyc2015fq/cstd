# 【重温C/C++】explicit+||template||virtual虚函数||重载和重写 - 三少GG - CSDN博客
2011年10月13日 14:16:42[三少GG](https://me.csdn.net/scut1135)阅读数：1033标签：[class																[编译器																[overloading																[设计模式																[function																[fun](https://so.csdn.net/so/search/s.do?q=fun&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)](https://so.csdn.net/so/search/s.do?q=overloading&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
explicit   只对构造函数起作用，用来抑制隐式转换。如：   
  class   A   {   
          A(int   a);   
  };   
  int   Function(A   a);   
  当调用   Function(2)   的时候，2   会隐式转换为   A   类型。这种情况常常不是程序员想要的结果，所以，要避免之，就可以这样写：   
  class   A   {   
          explicit   A(int   a);   
  };   
  int   Function(A   a);   
  这样，当调用   Function(2)   的时候，编译器会给出错误信息（除非   Function   有个以   int   为参数的重载形式），这就避免了在程序员毫不知情的情况下出现错误。
总结：explicit   只对构造函数起作用，用来抑制隐式转换。
++++++++++++++++   +++++++++++++++++++++
explicit: 禁止隐式类型转换操作
explicit　它与 virtual、inline 合称为“函数限定符”。它只适用于构造函数。若一个类拥有只带一个参数的构造函数，则可以使用 MyClass object(x) 或 MyClass object = x 来初始化对象，这实际是进行了从参数类型到类类型的转换。若在在构造函数前加上限定符
explicit ，将不再允许这种转换，即不允许 MyClass object = x 这种形式。
++++++++++++++++++++++++++++++
防止隐私转换!造成不必要的麻烦
禁止隐式转换，防止程序员误操作
explicit,和构造函数一起使用. 
explicit   constructor指明构造函数只能显示使用,目的是为了防止不必要的隐式转化. 
举个例子: 
    有这样一段代码: 
class   A 
{ 
      public: 
          A(int); 
      private: 
          int   num; 
}; 
int   Test(const   A&)   //   一个应用函数 
{ 
      ... 
} 
Test(2);   //   正确 
过程是这样的:   编译器知道传的值是int而函数需要的是A类型，但它也同时知道调用A的构造函数将int转换成一个合适的A，所以才有上面成功的调用.换句话说，编译器处理这个调用时的情形类似下面这样： 
    const   A   temp(2);             //   从2产生一个临时A对象 
    Test(temp);           //   调用函数 
如果代码写成如下样子: 
class   A 
{ 
      public: 
        explicit   A(int); 
      private: 
          int   num; 
}; 
int   Test(const   A&)   //   一个应用函数 
{ 
      ... 
} 
Test(2);   //   失败,不能通过隐式类型转换将int类型变量构造成成A类型变量 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
C++ Keywords
Keywords are predefined reserved identifiers that have special meanings. They cannot be used as identifiers in your program. The following keywords are reserved for C++:
**Syntax**
keyword: one of
|**asm**1|**auto**|**bad**_**cast**|**bad**_**typeid**|
|----|----|----|----|
|**bool**|**break**|**case**|**catch**|
|**char**|**class**|**const**|**const**_**cast**|
|**continue**|**default**|**delete**|**do**|
|**double**|**dynamic**_**cast**|**else**|**enum**|
|**except**|**explicit**|**extern**|**false**|
|**finally**|**float**|**for**|**friend**|
|**goto**|**if**|**inline**|**int**|
|**long**|**mutable**|**namespace**|**new**|
|**operator**|**private**|**protected**|**public**|
|**register**|**reinterpret**_**cast   **|**return**|**short**|
|**signed**|**sizeof**|**static**|**static**_**cast**|
|**struct**|**switch**|**template**|**this**|
|**throw**|**true**|**try**|**type**_**info**|
|**typedef**|**typeid**|**typename**|**union**|
|**unsigned**|**using**|**virtual**|**void**|
|**volatile**|**while**| | |
**[<C++实践系列>C++中的模板(template)](http://www.vckbase.com/document/viewdoc/?id=936)**
作者：张笑猛
网上我最喜欢的技术文章是类似某何君所著“CVS快速入门”或者“UML reference card”之类，简短扼要，可以非常快的领着你进入一个新天地。而对于比较长的文章我通常是将其保存到硬盘上，然后准备着“以后有时间”的时候再看，但它们通常的命运都是“闲坐说玄宗”，直到某一天在整理硬盘时将它们以“不知所云”入罪，一并删除。
 这篇小文主要是针对刚刚接触模板概念的读者，希望能帮助读者学习模板的使用。为了避免本文也在诸公的硬盘上遭逢厄运，我决定写的短些。“以后有时间”的时候再补充些内容。
TOC
[1. 简介](http://www.vckbase.com/document/viewdoc/?id=936#1%E3%80%82%E7%AE%80%E4%BB%8B)
[2. 语法](http://www.vckbase.com/document/viewdoc/?id=936#2%E3%80%82%E8%AF%AD%E6%B3%95)
[3. 使用技巧](http://www.vckbase.com/document/viewdoc/?id=936#3%E3%80%82%E4%BD%BF%E7%94%A8%E6%8A%80%E5%B7%A7)
[3.1 语法检查](http://www.vckbase.com/document/viewdoc/?id=936#3%E3%80%821%20%E8%AF%AD%E6%B3%95%E6%A3%80%E6%9F%A5)
[3.2 继承](http://www.vckbase.com/document/viewdoc/?id=936#3%E3%80%822%20%E7%BB%A7%E6%89%BF)
[3.3 静态成员](http://www.vckbase.com/document/viewdoc/?id=936#3%E3%80%823%20%E9%9D%99%E6%80%81%E6%88%90%E5%91%98)
[3.4 模板类的运用](http://www.vckbase.com/document/viewdoc/?id=936#3.4%20%E6%A8%A1%E6%9D%BF%E7%B1%BB%E7%9A%84%E8%BF%90%E7%94%A8)
[4. 参考资料](http://www.vckbase.com/document/viewdoc/?id=936#4.%20%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99)
**1. 简介**
模板是C++在90年代引进的一个新概念，原本是为了对容器类(container classes)的支持[[1]](http://www.vckbase.com/document/viewdoc/?id=936#%5B1%5D)，但是现在模板产生的效果已经远非当初所能想象。
简单的讲，模板就是一种参数化(parameterized)的类或函数，也就是类的形态(成员、方法、布局等)或者函数的形态(参数、返回值等)可以被参数改变。更加神奇的是这里所说的参数，不光是我们传统函数中所说的数值形式的参数，还可以是一种类型(实际上稍微有一些了解的人，更多的会注意到使用类型作为参数，而往往忽略使用数值作为参数的情况)。
举个常用的例子来解释也许模板就从你脑袋里的一个模糊的概念变成活生生的代码了：
在C语言中，如果我们要比较两个数的大小，常常会定义两个宏：
#define min(a,b) ((a)>(b)?(b):(a))
#define max(a,b) ((a)>(b)?(a):(b))
这样你就可以在代码中：
return min(10, 4);
或者：
return min(5.3, 18.6);
这两个宏非常好用，但是在C++中，它们并不像在C中那样受欢迎。宏因为没有类型检查以及天生的不安全(例如如果代码写为min(a++, b--);则显然结果非你所愿)，在C++中被inline函数替代。但是随着你将min/max改为函数，你立刻就会发现这个函数的局限性 —— 它不能处理你指定的类型以外的其它类型。例如你的min()声明为：
int min(int a, int b);
则它显然不能处理float类型的参数，但是原来的宏却可以很好的工作！你随后大概会想到函数重载，通过重载不同类型的min()函数，你仍然可以使大部分代码正常工作。实际上，C++对于这类可以抽象的算法，提供了更好的办法，就是模板：
template <class T> const T & min(const T & t1, const T & t2) {
    return t1>t2?t2:t1;
}
这是一个模板函数的例子。在有了模板之后，你就又自由了，可以像原来在C语言中使用你的min宏一样来使用这个模板，例如：
return min(10,4);
也可以:
return min(5.3, 18.6)
你发现了么？你获得了一个类型安全的、而又可以支持任意类型的min函数，它是否比min宏好呢？
当然上面这个例子只涉及了模板的一个方面，模板的作用远不只是用来替代宏。实际上，**模板是泛化编程(Generic Programming)的基础。所谓的泛化编程，就是对抽象的算法的编程，泛化是指可以广泛的适用于不同的数据类型。例如我们上面提到的min算法。**
**2. 语法**
你千万不要以为我真的要讲模板的语法，那太难为我了，我只是要说一下如何声明一个模板，如何定义一个模板以及常见的语法方面的问题。
template<> 是模板的标志，在<>中，是模板的参数部分。参数可以是类型，也可以是数值。例如：
template<class T, T t>
class Temp{
public:
    ...
    void print() { cout << t << endl; }
private:
    T t_;
};
在这个声明中，第一个参数是一个类型，第二个参数是一个数值。这里的数值，必须是一个常量。例如针对上面的声明：
Temp<int, 10> temp; // 合法
int i = 10;
Temp<int, i> temp; // 不合法
const int j = 10;
Temp<int, j> temp; // 合法
参数也可以有默认值：
template<class T, class C=char> ...
默认值的规则与函数的默认值一样，如果一个参数有默认值，则其后的每个参数都必须有默认值。
。
。
。
**3. 使用技巧**
知道了上面所说的简单语法后，基本上就可以写出自己的模板了。但是在使用的时候还是有些技巧。
3.1 语法检查
对模板的语法检查有一部分被延迟到使用时刻(类被定义[[3]](http://www.vckbase.com/document/viewdoc/?id=936#%5B3%5D)，或者函数被调用)，而不是像普通的类或者函数在被编译器读到的时候就会进行语法检查。因此，如果一个模板没有被使用，则即使它包含了语法的错误，也会被编译器忽略，这是语法检查问题的第一个方面，这不常遇到，因为你写了一个模板就是为了使用它的，一般不会放在那里不用。与语法检查相关的另一个问题是你可以在模板中做一些假设。例如：
template<class T> class Temp{
public:
    Temp(const T & t): t_(t) {}
    void print() { t.print();}
private:
    T t_;
};
在这个模板中，我假设了T这个类型是一个类，并且有一个print()方法(t.print())。我们在简介中的min模板中其实也作了同样的假设，即假设T重载了'>'操作符。
因为语法检查被延迟，编译器看到这个模板的时候，并不去关心T这个类型是否有print()方法，这些假设在模板被使用的时候才被编译器检查。只要定义中给出的类型满足假设，就可以通过编译。
之所以说“有一部分”语法检查被延迟，是因为有些基本的语法还是被编译器立即检查的。只有那些与模板参数相关的检查才会被推迟。如果你没有写class结束后的分号，编译器不会放过你的。
3.2 继承
。
。
。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
虚函数联系到多态，多态联系到继承。所以本文中都是在继承层次上做文章。没了继承，什么都没得谈。
下面是对C++的虚函数这玩意儿的理解。
一， 什么是虚函数（如果不知道虚函数为何物，但有急切的想知道，那你就应该从这里开始）
**简单地说，那些被virtual关键字修饰的成员函数，就是虚函数。虚函数的作用，用专业术语来解释就是实现多态性（Polymorphism），多态性是将接口与实现进行分离；用形象的语言来解释就是实现以共同的方法，但因个体差异而采用不同的策略。**下面来看一段简单的代码
class A{
public:
void print(){ cout<<”This is A”<<endl;}
};
class B:public A{
public:
void print(){ cout<<”This is B”<<endl;}
};
int main(){ //为了在以后便于区分，我这段main()代码叫做main1
A a;
B b;
a.print();
b.print();
}
通过class A和class B的print()这个接口，可以看出这两个class因个体的差异而采用了不同的策略，输出的结果也是我们预料中的，分别是This is A和This is B。但这是否真正做到了多态性呢？No，**多态还有个关键之处就是一切用指向基类的指针或引用来操作对象。**那现在就把main()处的代码改一改。
int main(){ //main2
A a;
B b;
A* p1=&a;
A* p2=&b;
p1->print();
p2->print();
}
运行一下看看结果，哟呵，蓦然回首，结果却是两个This is A。问题来了，p2明明指向的是class B的对象但却是调用的class A的print()函数，这不是我们所期望的结果，那么解决这个问题就需要用到虚函数
class A{
public:
virtual void print(){ cout<<”This is A”<<endl;} //现在成了虚函数了
};
class B:public A{
public:
void print(){ cout<<”This is B”<<endl;} //这里需要在前面加上关键字virtual吗？
};
毫无疑问，class A的成员函数print()已经成了虚函数，那么class B的print()成了虚函数了吗？回答是Yes，我们只需在把基类的成员函数设为virtual，其派生类的相应的函数也会自动变为虚函数。所以，class B的print()也成了虚函数。那么对于在派生类的相应函数前是否需要用virtual关键字修饰，那就是你自己的问题了。
现在重新运行main2的代码，这样输出的结果就是This is A和This is B了。
现在来消化一下，我作个简单的总结，**指向基类的指针在操作它的多态类对象时，会根据不同的类对象，调用其相应的函数，这个函数就是虚函数。**
二， 虚函数是如何做到的（如果你没有看过《Inside The C++ Object Model》这本书，但又急切想知道，那你就应该从这里开始）
。
。
。
**[C++中的虚函数（一）](http://www.vckbase.com/document/viewdoc/?id=950)**
6．用虚函数实现动态连接 在编译期间，C++编译器根据程序传递给函数的参数或者函数返回类型来决定程序使用那个函数，然后编译器用正确的的函数替换每次启动。这种基于编译器的替换被称为静态连接，他们在程序运行之前执行。另一方面，当程序执行多态性时，替换是在程序执行期进行的，这种运行期间替换被称为动态连接。如下例子：
class A{
public:
	virtual void f(){cout << "A::f" << endl;};
};
class B:public A{
public:
	virtual void f(){cout << "B::f" << endl;};
};
class C:public A{
public:
	virtual void f(){cout << "C::f" << endl;};
};
void test(A *a){
	a->f();
};
int main(int argc, char* argv[])
{     
	B *b=new B;
	C *c=new C;
	char choice;
	do{
		cout<<"type  B for class B,C for class C:"<<endl;
			cin>>choice;
		if(choice==''b'')
			test(b);
		else if(choice==''c'')
			test(c);
	}while(1);
	cout<<endl<<endl;
    return 0;
}
在上面的例子中，如果把类A,B,C中的virtual修饰符去掉，看看打印的结果，然后再想想两者的联系。
在基类中调用继承类的函数（如果此函数是虚函数才能如此）
在6中的例子中，test(A *a)其实有一个**继承类指针向基类指针隐式转化的过程**。可以看出利用虚函数我们可以在基类调用继承类函数。但如果不是虚函数，继承类指针转化为基类指针后只可以调用基类函数。反之，如果基类指针向继承类指针转化的情况怎样，这只能进行显示转化，如下例子：
class A {
public:
    void fun() {
        cout << "A::fun" << endl;
    }
    
};
class B : public A {
public:
	void fun() {
	cout << "B::fun" << endl;
	}
	void fun0() {
        cout << "B::fun0" << endl;
    }
};
int main() {
    A *a=new A;
	B *b=new B;
	A *pa;
	B *pb;
	pb=static_cast<B *>(a); //基类指针向继承类指针进行显示转化
    pb->fun0();
	pb->fun();
	    return 0;
}  
重要参考资料！！！：
# [虚函数 (程序语言)](http://zh.wikipedia.org.sixxs.org/wiki/%E8%99%9A%E5%87%BD%E6%95%B0)
[维基百科，自由的百科全书](http://zh.wikipedia.org.sixxs.org/wiki/%E8%99%9A%E5%87%BD%E6%95%B0)
**虚函数**是[面向对象程序设计](http://zh.wikipedia.org.sixxs.org/wiki/%E9%9D%A2%E5%90%91%E5%AF%B9%E8%B1%A1%E7%BC%96%E7%A8%8B)中的一个重要的概念。当从父类中[继承](http://zh.wikipedia.org.sixxs.org/wiki/%E7%BB%A7%E6%89%BF_(%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%A7%91%E5%AD%A6))的时候，虚函数和被继承的函数具有相同的[签名](http://zh.wikipedia.org.sixxs.org/w/index.php?title=%E6%96%B9%E6%B3%95%E7%AD%BE%E5%90%8D&action=edit&redlink=1)。但是在运行过程中，运行系统将根据对象的类型，自动地选择适当的具体实现运行。虚函数是面向对象编程实现[多态](http://zh.wikipedia.org.sixxs.org/wiki/%E5%A4%9A%E5%9E%8B_(%E7%89%A9%E4%BB%B6%E5%B0%8E%E5%90%91%E7%A8%8B%E5%BC%8F%E8%A8%AD%E8%A8%88))的基本手段。
虚函数在[设计模式](http://zh.wikipedia.org.sixxs.org/wiki/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F)方面扮演重要角色。例如，《设计模式》一书中提到的23种设计模式中，仅5个对象创建模式就有4个用到了虚函数（[抽象工厂](http://zh.wikipedia.org.sixxs.org/wiki/%E6%8A%BD%E8%B1%A1%E5%B7%A5%E5%8E%82)、[工厂方法](http://zh.wikipedia.org.sixxs.org/wiki/%E5%B7%A5%E5%8E%82%E6%96%B9%E6%B3%95)、[生成器](http://zh.wikipedia.org.sixxs.org/wiki/%E7%94%9F%E6%88%90%E5%99%A8_(%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F))、[原型](http://zh.wikipedia.org.sixxs.org/wiki/%E5%8E%9F%E5%9E%8B%E6%A8%A1%E5%BC%8F)），只有[单例](http://zh.wikipedia.org.sixxs.org/wiki/%E5%8D%95%E4%BE%8B%E6%A8%A1%E5%BC%8F)没有用到。
## 目录
- [1程式范例](http://zh.wikipedia.org.sixxs.org/wiki/%E8%99%9A%E5%87%BD%E6%95%B0#.E7.A8.8B.E5.BC.8F.E7.AF.84.E4.BE.8B)- [1.1C++](http://zh.wikipedia.org.sixxs.org/wiki/%E8%99%9A%E5%87%BD%E6%95%B0#C.2B.2B)
- [1.2Java](http://zh.wikipedia.org.sixxs.org/wiki/%E8%99%9A%E5%87%BD%E6%95%B0#Java)
- [1.3C#](http://zh.wikipedia.org.sixxs.org/wiki/%E8%99%9A%E5%87%BD%E6%95%B0#C.23)
- [2抽象类和纯虚函数](http://zh.wikipedia.org.sixxs.org/wiki/%E8%99%9A%E5%87%BD%E6%95%B0#.E6.8A.BD.E8.B1.A1.E7.B1.BB.E5.92.8C.E7.BA.AF.E8.99.9A.E5.87.BD.E6.95.B0)- [2.1C++](http://zh.wikipedia.org.sixxs.org/wiki/%E8%99%9A%E5%87%BD%E6%95%B0#C.2B.2B_2)
- [3参考文献](http://zh.wikipedia.org.sixxs.org/wiki/%E8%99%9A%E5%87%BD%E6%95%B0#.E5.8F.82.E8.80.83.E6.96.87.E7.8C.AE)
- 
## 抽象类和纯虚函数
**纯虚函数**或**纯虚方法**是一个需要被非抽象衍生类执行的虚函数. 包含纯虚方法的类被称作**抽象类**; 抽象类不能被直接调用,
一个抽象基类的一个[子类](http://zh.wikipedia.org.sixxs.org/wiki/%E5%AD%90%E7%B1%BB)只有在所有的纯虚函数在该类(或其父类)内给出实现时, 才能直接调用.
纯虚方法通常只有[声明](http://zh.wikipedia.org.sixxs.org/w/index.php?title=%E5%A3%B0%E6%98%8E&action=edit&redlink=1)(签名)而没有定义(实现).
作为一个例子, 抽象基类"MathSymbol"可能提供一个纯虚函数 `doOperation()`, 和衍生类 "Plus" 和 "Minus" 提供`doOperation()` 的具体实现. 由于 "MathSymbol" 是一个抽象概念, 为其每个子类定义了同一的动作, 在 "MathSymbol" 类中执行 `doOperation()` 没有任何意义. 类似的, 一个给定的 "MathSymbol" 子类如果没有 `doOperation()` 的具体实现是不完全的.
虽然纯虚方法通常在定义它的类中没有实现, 在 C++ 语言中, 允许纯虚函数在定义它的类中包含其实现, 这为衍生类提供了备用或默认的行为.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**Overload和Override的区别。**Overloaded的方法是否可以改变返回值的类型?
答：方法的重写Overriding和重载Overloading是Java多态性的不同表现。**重写Overriding是父类与子类之间多态性的一种表现**，
**重载Overloading是一个类中多态性的一种表现**。如果在子类中定义某方法与其父类有相同的名称和参数，我们说该方法被重写 (Overriding)。子类的对象使用这个方法时，将调用子类中的定义，对它而言，父类中的定义如同被“屏蔽”了。如果在一个类中定义了多个同名的方法，它们或有不同的参数个数或有不同的参数类型，则称为方法的重载(Overloading)。Overloaded的方法是可以改变返回值的类型。

