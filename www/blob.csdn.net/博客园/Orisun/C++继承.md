# C++继承 - Orisun - 博客园







# [C++继承](https://www.cnblogs.com/zhangchaoyang/articles/2723411.html)





先上一段代码：

#include<iostream>

using namespace std;

class Base{
public:
	int a;
protected:
	int b;
private:
	int c;
};

struct Derived:public Base{
};

int main(){
	Derived inst;
	cout<<sizeof(Base)<<endl;
	cout<<sizeof(Derived)<<endl;
	cout<<sizeof(inst)<<endl;
	cout<<inst.a<<endl;
	return 0;
}


输出：

12
12
12
15487104

对应上面的代码，先啰嗦几个与“继承”无关的语法：struct可以继承自一个class；sizeof()可以施加在一个类型上面，也可以施加在一个对象上面；由于inst是局部变量，所以类中的普通类型数据成员是不被初始化的，所以输出a=15487104。

我们看到sizeof(Base)==sizeof(Derived)==12，可见Derived继承了Base的public、protected、private成员。

struct Derived:public Base{
public:
	void func(){
		cout<<b<<endl;
	}
};

int main(){
	Derived inst;
	inst.func();
	return 0;
}


一般在类外是不能访问protected成员的，所以在main()函数中调用cout<<inst.b;中会发生编译错误的，但是我们可以把对protected成员的访问封闭在一个public方法中，通过调用public方法来间接访问protected数据成员。

struct Derived:public Base{
public:
	void func(){
		cout<<c<<endl;
	}
};


上面代码会有编译错误，protected成员好歹在派生类中还可以访问，private成员在派生类中都不能访问。既然成员c在Derived中不能访问，那sizeof(Derived)又何苦要等于12呢？以Derived分配了3个int的空间，但其中一个int却是不可访问的，有什么意义呢？

（1）公有继承：基类成员保持自己的访问级别：基类的public成员在派生类中还是public，基类的protected成员在派生类中还是protected。

（2）受保护继承：基类的public和protected成员在派生类中成为protected成员。

（3）私有继承：基类的所有成员在派生类中成为private成员。

struct Derived:private Base{
};

class Grandson:public Derived{
public:
	void func(){
		cout<<a<<endl;
	}
};


上面的代码有编译错误，由于Derived私有继承自Base，所以a在Derived中已经是private了，在Grandson中不能访问Derived中的私有成员。 

struct Derived:private Base{
public:
	using Base::b;
protected:
	using Base::a;
};


由于Derived私有继承自Base，按说a、b、c在Derived中应该都是私有的，但通过上述方式，在Derived中b中公有的，a中受保护的。

struct Derived:Base


 Derived为struct时默认为公有继承。

class Derived:Base


Derived为class时默认为私有继承。

**友元类**可以访问private和protected成员数据，友元关系不能继承。

#include<iostream>

using namespace std;

class Base{
	friend class Frnd;
private:
	int a;
};

class Derived:public Base{
};

class Frnd{
public:
	void func(){
		Base base;
		cout<<base.a<<endl;　　　　//OK
		Derived derived;
		cout<<derived.a<<endl;　　　　//error
	}
};


**虚函数**

当父类中的某个函数标记为virtual时，子类中如果重载了该方法则自动也具有virtual属性，不管你是否使用了"virtual“关键字。比如下面的代码：

#include<iostream>

using namespace std;

class A{
public:
	virtual void func(){
		cout<<"A.func"<<endl;
	}
};

class B:public A{
public:
	void func(){			//虽然没有显式写明virtual，但该函数依然是虚函数
		cout<<"B.func"<<endl;
	}
};

class C:public B{
public:
	virtual void func(){
		cout<<"C.func"<<endl;
	}
};

int main(){
	A *a=new A();
	A *b=new B();
	A *c=new C();
	a->func();
	b->func();
	c->func();
	return 0;
}


输出：

A.func
B.func
C.func

**内部类**可以访问外部类的所有成员。

class Base{
public:
	int a;
	class Inner{
	public:
		void func(){
			Base base;
			cout<<base.c<<endl;
		}
	};
protected:
	int b;
private:
	int c;
};

int main(){
	Base::Inner inst;
	inst.func();
	return 0;
}


注意在main函数中访问Inner类时要加Base::前缀，另外Inner只有定义在Base的public域下时，在Base之外才可以访问Inner。

#include<iostream>

using namespace std;

class Base{
public:
	Base(){
		cout<<"Base construct"<<endl;
	}
	virtual void foo(){
		cout<<"Base foo"<<endl;
	}
	~Base(){
		cout<<"Base destruct"<<endl;
	}
};

class Derived:public Base{
public:
	Derived(){
		cout<<"Derived construct"<<endl;
	}
	virtual void foo(){
		cout<<"Derived foo"<<endl;
	}
	~Derived(){
		cout<<"Derived destruct"<<endl;
	}
};

int main(){
	Base *inst=new Derived();		//调用Derived的构造函数，在此之前会调用Base的构造函数
	inst->foo(); //调用Derived的foo方法 
	delete inst; //只调用了Base的析构函数 
}

通常情况下Derived拥有比Base更多的数据成员，在Derived的析构函数中需要释放更多的资源，但是我们看到上面的代码中Base和Derived的构造函数都调用了，最后却只调用了Base的析构函数。解决方法：Base中使用虚的析构函数。

#include<iostream>

using namespace std;

class Base{
public:
	Base(){
		cout<<"Base construct"<<endl;
	}
	virtual void foo(){
		cout<<"Base foo"<<endl;
	}
	virtual ~Base(){
		cout<<"Base destruct"<<endl;
	}
};

class Derived:public Base{
public:
	Derived(){
		cout<<"Derived construct"<<endl;
	}
	virtual void foo(){
		cout<<"Derived foo"<<endl;
	}
	~Derived(){
		cout<<"Derived destruct"<<endl;
	}
};

int main(){
	Base *inst=new Derived();　　//调用Derived的构造函数，在此之前会调用Base的构造函数
	inst->foo(); 　　//调用Derived的foo方法 
	delete inst; 　　//调用Derived的析构函数，在此之后会调用Base的析构函数 
}

最佳实践：即便析构函数不做任何工作，继承层次的根类也应该定义一个虚析构函数。

class Base{
public:
	double price(std::size_t) const =0;
};


在函数形参表后面写上=0表示该函数为纯虚函数，含有一个或多个纯虚函数的类为抽象类，不能创建抽象类的实例。抽象类为后代提供了可以覆盖的接口。

void func(Base &base);
Derived inst;
func(inst);


当把基类对象传递给func()函数时，引用base直接绑定到inst对象，没有发生对象的复制，没有发生类型的转换，inst也没有发生任何改变不。

void func(Base base);
Derived inst;
func(inst);


此时情况完全不同，形参的类型是固定的，在编译和运行时都是基类类型对象，如果把派生类对象传递过去，则派生类对象中的基类部分会发生复制。












