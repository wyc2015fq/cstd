# 设计模式之Decorator，C++ - 长歌行 - CSDN博客





2012年11月28日 17:10:22[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1170
个人分类：[设计架构](https://blog.csdn.net/iuhsihsow/article/category/1076810)

所属专栏：[初探设计模式（C++实现）](https://blog.csdn.net/column/details/baadf00df00dabadfds0.html)












原创 [http://blog.csdn.net/iuhsihsow](http://blog.csdn.net/iuhsihsow)

多多关注哦




最近重构程序，必须得研究下设计模式，于是写了一个C++例子。







//装饰模式

 //动态的给一个对象添加一些额外的职责，

 //就添加功能来说，装饰模式比生成子类更加灵活


 //（1） 装饰对象和真实对象有相同的接口。这样客户端对象就可以和真实对象相同的方式和装饰对象交互。

 //（2） 装饰对象包含一个真实对象的引用（reference）

 //（3） 装饰对象接受所有来自客户端的请求。它把这些请求转发给真实的对象。

 //（4） 装饰对象可以在转发这些请求以前或以后增加一些附加功能。这样就确保了在运行时，不用修改给定对象的结构就可以在外部增加附加的功能。在面向对象的设计中，通常是通过继承来实现对给定类的功能扩展。


不大会UML，图只能大概看看


![](https://img-my.csdn.net/uploads/201211/28/1354094030_7327.png)





下面就是代码了


 //注意，一定要有一个非装饰类，上面可以无限挂载装饰类

 //挂载过程可以用函数，工厂类等隐藏

 //这个类里面没有component对象，否则行为函数会运行不止




```cpp

```
#include "stdafx.h"
 #include <Windows.h>
 #include <iostream>
 using namespace std;
 
 #define DELETE_OBJECT(p)		{ if (p) delete p;	p = NULL; }
 #define DELETE_ARRAY(p)			{ if (p) delete []p; p = NULL;}
 
 //假设情景
 //1.打印论文正文
 void Demand1();
 //2.新需求，打印正文之前还要打印页眉
 void Demand2();
 //3.新需求，打印正文之后还要打印页脚，之前要打印页眉
 void Demadn3();
 //通过三个需求来看装饰模式
 
 
 class Component
 {
 public:
 	virtual void Print() = 0;
 protected:
 private:
 };
 
 class SpecialComponent : public Component
 {
 public:
 	void Print()
 	{
 		printf("Mission begin\n");
 	}
 
 protected:
 private:
 };
 
 class Decorator : public Component
 {
 public:
 	void SetComponent(Component *pComponent)
 	{
 		m_pComponent = pComponent;
 	}
 	void Print()
 	{
 		m_pComponent->Print();
 	}
 
 protected:
 	Component *m_pComponent;
 };
 
 
 class PrintBody : public Decorator
 {
 public:
 	void Print()
 	{
 		m_pComponent->Print();
 		printf("Print body\n");
 	}
 
 
 protected:
 private:
 };
 
 class PrintHead : public Decorator
 {
 public:
 	void Print()
 	{
 		m_pComponent->Print();
 		printf("Print Head\n");
 		SelfBehavior();
 	}
 	void SelfBehavior()
 	{
 		printf("Author is tanben\n");
 	}
 protected:
 private:
 };
 
 class PrintEnd : public Decorator
 {
 public:
 	void Print()
 	{
 		m_pComponent->Print();
 		printf("Print End\n");
 		NewAddedBehavior();
 	}
 	void NewAddedBehavior()
 	{
 		printf("Day: 2012/11/28\n");
 	}
 protected:
 private:
 };
 
 
 //有时候为了让业务逻辑更加清晰，
 //可以将某些固定顺序的操作设计为一个类
 class Demand1Factory
 {
 public:
 	Component * CreateComponet()
 	{
 		m_pDemand1 = new SpecialComponent();
 		m_pBody = new PrintBody();
 		m_pBody->SetComponent(m_pDemand1);
 		return m_pBody;
 	}
 	void Release()
 	{
 		DELETE_OBJECT(m_pDemand1);
 		DELETE_OBJECT(m_pBody);
 	}
 protected:
 	SpecialComponent *m_pDemand1;
 	PrintBody		 *m_pBody;	
 };
 
 
 
 
 
 
 
 int _tmain(int argc, _TCHAR* argv[])
 {
 
 	Demand1();
 	Demand2();
 	Demadn3();
 
 	return 0;
 }
 
 
 
 void Demand1()
 {
 	Demand1Factory d1Factory;
 	Component *pDemand = d1Factory.CreateComponet();
 	pDemand->Print();
 	d1Factory.Release();
 }
 
 void Demand2()
 {
 	SpecialComponent *pSC = new SpecialComponent();
 	PrintBody *pBody = new PrintBody();
 	PrintHead *pHead = new PrintHead();
 	pHead->SetComponent(pSC);
 	pBody->SetComponent(pHead);
 	pBody->Print();
 	DELETE_OBJECT(pSC);
 	DELETE_OBJECT(pBody);
 	DELETE_OBJECT(pHead);
 }
 
 void Demadn3()
 {
 	SpecialComponent *pSC = new SpecialComponent;
 	PrintBody *pBody = new PrintBody();
 	PrintHead *pHead = new PrintHead();
 	PrintEnd  *pEnd = new PrintEnd();
 	pHead->SetComponent(pSC);
 	pBody->SetComponent(pHead);
 	pEnd->SetComponent(pBody);
 	pEnd->Print();
 	DELETE_OBJECT(pSC);
 	DELETE_OBJECT(pBody);
 	DELETE_OBJECT(pHead);
 	DELETE_OBJECT(pEnd);
 }




