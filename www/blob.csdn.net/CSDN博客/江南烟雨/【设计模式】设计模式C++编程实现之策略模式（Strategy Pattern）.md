# 【设计模式】设计模式C++编程实现之策略模式（Strategy Pattern） - 江南烟雨 - CSDN博客
2012年06月26日 21:25:18[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2252标签：[设计模式](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)
个人分类：[设计模式](https://blog.csdn.net/xiajun07061225/article/category/1174429)
在介绍具体的设计模式之前，先介绍一下OO（Objected-Oreinted）基础以及OO原则。
- OO基础：抽象、封面装、多态、继承。
- OO原则：
- 封装变化。
- 多用组合，少用继承。
- 针对接口编程，不针对实现编程。
策略模式定义：
定义了算法族，分别封装起来，让他们之间可以相互替换，此模式让算法的变化独立于使用算法的客户。
一个很重要的设计原则是：
找出应用中可能需要变化之处，把它们独立出来，不要和那些不需要变化的代码混在一起。
把变化的部分取出来并封装，好让其他部分不会受到影响。结果会使代码变化引起不经意的后果变少，系统变得更有弹性。
在策略模式中，算法被另外封装成类，它的实现不会被绑死在某个基类的子类中。
它的优点是适合类中的成员以方法为主，算法经常变动的情况。简化了单元测试，因为每个算法都有自己的类，可以通过自己的接口单独测试。
当然这需要客户端做出判断。
其UML图：
![](https://img-my.csdn.net/uploads/201206/26/1340717305_8635.jpg)
![](https://img-my.csdn.net/uploads/201206/26/1340717127_8678.jpg)
下面实现了一个应用了策略模式的例子。
具有各种不同飞行行为的鸭子，各个子类可以设置自己的飞行行为。
代码：
```cpp
#include <iostream>
using namespace std;
//飞行方法接口抽象接口类
class Fly{
public:
	virtual void fly() = 0;
};
class FlyWithWings:public Fly
{
public:
	void fly()
	{
		cout << "I'm flying" << endl;
	}
};
class FlyNoWay:public Fly
{
public:
	void fly()
	{
		cout << "I can't fly" << endl;
	}
};
class FlyRocketPowered:public Fly
{
public:
	void fly()
	{
		cout << "I'm flying with a rocket" << endl;
	}
};
//Duck抽象基类定义
class Duck{
public:
	Duck():m_fly(NULL){}
	//设定Fly的具体方法
	void setFly(Fly *pFly)
	{
		if (m_fly)
		{
			delete m_fly;//删除原来的fly方法所占空间
		}
		m_fly = pFly;
	}
	//运行Fly方法
	void performFly()
	{
		if (m_fly)
		{
			m_fly->fly();
		}
	}
	//析构函数
	~Duck()
	{
		if (m_fly)
		{
			delete m_fly;
			m_fly = NULL;
		}
	}
protected:
	Fly *m_fly;
};
//Duck子对象定义
class MallardDuck:public Duck{
public:
	MallardDuck()
	{
		cout << "I'm a Mallard duck" << endl;
		m_fly = new FlyWithWings();
	}
};
//Duck子对象定义
class ModelDuck:public Duck{
public:
	ModelDuck()
	{
		cout << "I'm a Model duck" << endl;
		m_fly = new FlyNoWay();
	}
};
int main()
{
	MallardDuck myDuck1;
	myDuck1.performFly();
	myDuck1.setFly(new FlyRocketPowered());
	myDuck1.performFly();
	ModelDuck myDuck2;
	myDuck2.performFly();
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201206/26/1340717203_6863.jpg)

