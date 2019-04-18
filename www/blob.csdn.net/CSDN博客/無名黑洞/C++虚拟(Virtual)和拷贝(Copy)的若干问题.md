# C++虚拟(Virtual)和拷贝(Copy)的若干问题 - 無名黑洞 - CSDN博客
2014年03月24日 08:47:44[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：1688
在C++中创立一个空类后，其实里面会自动生成一些内容，他们是：缺省构造函数，缺省拷贝构造函数 ，缺省析构函数，缺省取址运算符，和缺省赋值运算符。但在某些情况下，我们需要自定义这些函数。其中，为了实现类的多态，需要对其中的一些函数使用虚拟(Virtual)技术。总的来说，分为成员函数的虚拟，构造函数的虚拟，析构函数的虚拟，和纯虚函数等。对于拷贝相关的函数，我们需要知道在什么情况下使用拷贝构造函数，什么时候使用赋值运算符。下面来解答这些问题。
**1、虚函数在成员函数中的使用。**
下面代码显示了使用虚成员函数的情况。
```cpp
///用作测试的基函数
class BaseClass{
public:
	int m_id;
public:
	BaseClass(int id){ m_id = id; cout<<"Base class construct. ID: "<<m_id<<endl;}
	~BaseClass(){ cout<<"Base class deconstruct. ID: "<<m_id<<endl; }
	void Run(){ cout<<"Run base class ID: "<<m_id<<endl;}
	virtual void VirtualRun(){ cout<<"Run(virtual) base class ID: "<<m_id<<endl;}
};
///用作测试的派生函数
class DerivedClass: public BaseClass{
public:
	DerivedClass(int id) : BaseClass(id){ m_id = id; cout<<"Derived class construct. ID: "<<m_id<<endl; }
	~DerivedClass(){ cout<<"Derived class deconstruct. ID: "<<m_id<<endl; }
	void Run(){ cout<<"Run derived class ID: "<<m_id<<endl; }
	void VirtualRun(){ cout<<"Run(virtual) derived class ID: "<<m_id<<endl; }
};
///有关须函数方法的测试
void ClassTextI(){
	cout<<"<基类测试>"<<endl;
	BaseClass baseClass(0);
	baseClass.Run();
	cout<<endl;
	///运行结果：依次调用构造函数，调用方法；在BuildClass函数结束的时候，调用析构函数
	///含有虚函数的类可以实例化
	cout<<"<派生类测试>"<<endl;
	DerivedClass derivedClass(1);
	derivedClass.Run();
	cout<<endl;
	///运行结果：依次调用基类和派生类构造函数，调用派生类方法；在BuildClass函数结束的时候，调用基类和派生类析构函数
	cout<<"<基类指针测试>"<<endl;
	BaseClass *p = &baseClass;
	p->Run();
	cout<<endl;
	///使用基类指针指向基类对象
	///运行结果：调用基类方法
	cout<<"<基类指针指向派生类测试>"<<endl;
	p = &derivedClass;
	p->Run();
	cout<<endl;
	///使用基类指针指向派生类对象，看看能不能用基类指针调用派生类方法
	///运行结果：不能，仍然调用了基类方法
	cout<<"<基类指针派生类覆盖虚函数测试>"<<endl;
	p = &derivedClass;
	p->VirtualRun();
	cout<<endl;
	///运行结果：调用了派生类虚方法
	///结果说明了如果要用基类指针调用派生类的方法，必须在基类中声明为虚方法
	cout<<"<析构函数测试>"<<endl;
	///运行结果：先建立的对象后析构
}
```
从上面可以看出，如果要用基类指针来调用派生类的方法，就要使用虚函数。为什么要这样做呢？举例来说，我们定义了一个“猫科动物”类，下面派生出“老虎”和“猫”两个子类，他们都有一个“叫”的方法。我们只需要一个“猫科动物”指针就可以指向“老虎”或“猫”。如果“叫”方法是虚拟的，当指针指向“老虎”时，“叫”自动调用“老虎”的叫声。对猫也一样。这就是类的“多态”（Polymorphism）。
**2、虚函数在析构函数中的使用**
如果用基类指针指向派生类并生成了一个新的派生对象，则会发生派生类析构函数无法正确被调用的情况（调用了基类析构函数）。因此，必须使用虚析构函数，如下：
```cpp
///用作测试的，使用虚析构函数，的基函数
class BaseClassVirtualDeconstructor{
public:
	int m_id;
public:
	BaseClassVirtualDeconstructor(int id){ m_id = id; cout<<"Base class construct. ID: "<<m_id<<endl; }
	virtual ~BaseClassVirtualDeconstructor(){ cout<<"Base class deconstruct. ID: "<<m_id<<endl;}
};
///用作测试的，使用虚析构函数的基函数，的派生函数
class DerivedClassVirtualDeconstructor: public BaseClassVirtualDeconstructor{
public:
	DerivedClassVirtualDeconstructor(int id) : BaseClassVirtualDeconstructor(id){
		cout<<"Derived class construct. ID: "<<m_id<<endl;
		m_id = id;
	}
	~DerivedClassVirtualDeconstructor(){ cout<<"Derived class deconstruct. ID: "<<m_id<<endl;}
};
///有关虚析构函数的测试
void ClassTextII(){
	cout<<"<新派生类测试>"<<endl;
	///普通的派生函数
	BaseClass *p = new DerivedClass(2);
	cout<<endl;
	cout<<"<新派生类析构函数测试>"<<endl;
	delete p;
	///运行结果：调用了基类的析构函数。
}
void ClassTextIII(){
	cout<<"<新派生类测试(基类虚析构函数)>"<<endl;
	///基类是虚析构函数的派生类
	BaseClassVirtualDeconstructor *p = new DerivedClassVirtualDeconstructor(3);
	cout<<endl;
	cout<<"<析构函数测试(基类虚析构函数)>"<<endl;
	delete p;
	///运行结果：调用了基类和派生类的析构函数
	///结果说明了在使用基类指针生成新派生类对象的情况下，只有基类使用虚析构函数的时候，派生类的析构函数才能正确被调用
}
```
**3、虚函数在构造函数中的应用**
把构造函数设为虚函数是没有意义的，因为虚函数是为了多态，而构造函数是对本体的。
**4、纯虚函数的应用**
含有至少一个纯虚函数（在函数名后加 = 0，并忽略函数体）的类叫做抽象类。抽象类不能被实例化，抽象类的派生类必须实现基类所有的纯虚函数。抽象类通常用来阻止用户对其实例化。(而仅含有虚函数的类是可以实例化的)。把构造函数设为纯虚函数也是没有意义的，也没办法通过编译。
```cpp
class AbstractClass{
public:
	int m_id;
	virtual void AbstractMemberFunction() = 0;
};
class DerivedAbstractClass : AbstractClass{
public:
	DerivedAbstractClass(int id){ m_id = id; }
};
void ClassTextIV(){
	//DerivedAbstractClass derivedAbstractClass(4); ///无法通过编译
}
```
**5、拷贝构造函数，赋值运算符拷贝，和克隆函数**
既然有默认的拷贝函数，为什么还要额外定义呢？
原因1：静态变量。默认拷贝函数不处理静态变量。
原因2：浅拷贝和深拷贝。默认拷贝函数使用浅拷贝。
浅拷贝只拷贝引用对象，不单独开辟内存空间；而深拷贝要开辟内存空间。
在很多情况下，浅拷贝会带来问题。比如A被浅拷贝给了B，那么当改变A中某些值（指针指向的内存空间）的时候，B也会被连累改变。这时就需要深拷贝了。/因此，有必要定义自己的拷贝函数。
调用拷贝构造函数的情况：1、作为函数参数（传值）；2、作为函数返回值；3、拷贝
比如Class c1; Class c2 = c2
拷贝构造函数是一种特殊的构造函数，它在上述情况下取代了构造函数来构造对象
虽然在拷贝构造函数的情况3中使用了赋值运算符，但是和赋值运算符拷贝不是一回事
看这种情况：Class c1; Class c2; c2 = c1;
可以看出，当被赋值的对象已经存在时，调用的是赋值运算符拷贝，而不是拷贝构造函数
克隆是一种逻辑概念，它提供了一种跟赋值运算符拷贝不同的拷贝方法
至于克隆到底要怎么实现，那么要看情况。比如，既可以浅拷贝（影子克隆），也可以深拷贝（深度克隆）
```cpp
class CopyExampleClass{
public:
	int *m_id;
	CopyExampleClass(){ m_id = new int(1); }
	~CopyExampleClass(){ cout<<"析构对象: "<<*m_id<<endl; delete m_id; }
	void PrintId(){ cout<<*m_id<<endl;}
	///拷贝构造函数
	CopyExampleClass(const CopyExampleClass &c){
		cout<<"调用拷贝构造函数。"<<endl;
		m_id = new int(*(c.m_id));
	}
	///赋值运算符拷贝
	CopyExampleClass& operator =(const CopyExampleClass &c){
		cout<<"调用赋值运算符拷贝。"<<endl;
		if(this == &c) return *this;
		if(m_id != NULL) delete m_id;
		m_id = new int(*(c.m_id));
		return *this;
	}
	///克隆
	CopyExampleClass* Clone() const {  ///const表示该函数禁止修改成员变量
		cout<<"调用克隆函数。"<<endl;
		return new CopyExampleClass(*this);
		//return new CopyExampleClass();///这样的话是创造新对象而不是克隆
	}
};
void ClassTextV(){
	CopyExampleClass copyExampleClass;
	copyExampleClass.PrintId();///运行结果：1。这个结果作为对照
	cout<<"<拷贝构造函数测试>"<<endl;
	CopyExampleClass copyExampleClass2 = copyExampleClass;///调用了拷贝构造函数
	(*copyExampleClass2.m_id) = 2;///改变2号对象
	copyExampleClass2.PrintId();///运行结果：2
	copyExampleClass.PrintId();///运行结果：1, 如果不用拷贝构造函数，这里结果也会是2
	cout<<"<赋值运算符拷贝测试>"<<endl;
	CopyExampleClass copyExampleClass3;
	copyExampleClass3 = copyExampleClass;
	(*copyExampleClass3.m_id) = 3;///改变3号对象
	copyExampleClass3.PrintId();///运行结果：3
	copyExampleClass.PrintId();///运行结果：1, 如果不用赋值运算符拷贝，这里结果也会是2
	cout<<"<克隆测试>"<<endl;
	CopyExampleClass *copyExampleClass4 = copyExampleClass.Clone();
	(*copyExampleClass4->m_id) = 4;
	copyExampleClass4->PrintId();///运行结果：4
	copyExampleClass.PrintId();///运行结果：1
	delete copyExampleClass4;
}
```
