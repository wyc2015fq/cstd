# Mediator模式详解--设计模式（18） - fanyun的博客 - CSDN博客
2016年07月06日 08:42:48[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：585
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**Mediator模式来源：**
    在面向对象系统的设计和开发过程中，对象之间的交互和通信是最为常见的情况，因为对象间的交互本身就是一种通信。在系统比较小的时候，可能对象间的通信不是很多、对象也比较少，我们可以直接硬编码到各个对象的方法中。但是当系统规模变大，对象的量变引起系统复杂度的急剧增加，对象间的通信也变得越来越复杂，这时候我们就要提供一个专门处理对象间交互和通信的类，这个中介者就是Mediator模式。Mediator模式提供将对象间的交互和通讯封装在一个类中，各个对象间的通信不必显势去声明和引用，大大降低了系统的复杂性能（了解一个对象总比深入熟悉n个对象要好）。另外Mediator模式还带来了系统对象间的松耦合。
Mediator模式作用：
       Mediator模式是一种很有用并且很常用的模式，它通过将对象间的通信封装到一个类中，将多对多的通信转化为一对多的通信，降低了系统的复杂性。Mediator还获得系统解耦的特性，通过Mediator，各个Colleague就不必维护各自通信的对象和通信协议，降低了系统的耦合性，Mediator和各个Colleague就可以相互独立地修改了。
        Mediator模式还有一个很显著额特点就是将控制集中，集中的优点就是便于管理，也正式符合了OO设计中的每个类的职责要单一和集中的原则。
Mediator模式UML模式结构图：
![](https://img-blog.csdn.net/20160706145559595?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Mediator模式的构成：**
        Colleage抽象同事类：而ConcreteColleage是具体同事类，每个具体同事只知道自己的行为，而不了解其他同事类的情况，但它们却都认识中介者对象，Mediator是抽象中介者，定义了同事对象到中介者对象的接口，ConcreteMediator是具体中介者对象，实现抽象类的方法，它需要知道所有具体同事类，并从具体同事接受消息，向具体同事对象发出命令。
        Colleage类：抽象同事类。
        Mediator类：抽象中介者类。
**Mediator模式的代码示例：**
Mediator.h
```cpp
#ifndef _MEDIATOR_H_
#define _MEDIATOR_H_
#include <string>
using namespace std;
class Mediator;
class Colleage
{
public:
    virtual ~Colleage();
    virtual void SetMediator(Mediator*);
    virtual void SendMsg(string) = 0;
    virtual void GetMsg(string) = 0;
protected:
    Colleage(Mediator*);
    Mediator* _mediator;
private:
    
};
class ConcreteColleageA : public Colleage
{
public:
    ~ConcreteColleageA();
    ConcreteColleageA(Mediator*);
    virtual void SendMsg(string msg);
    virtual void GetMsg(string);
protected:
private:
};
class ConcreteColleageB : public Colleage
{
public:
    ~ConcreteColleageB();
    ConcreteColleageB(Mediator*);
    virtual void SendMsg(string msg);
    virtual void GetMsg(string);
protected:
private:
};
class Mediator
{
public:
    virtual ~Mediator();
    virtual void SendMsg(string,Colleage*) = 0;
protected:
    Mediator();
private:
};
class ConcreteMediator : public Mediator
{
public:
    ConcreteMediator();
    ~ConcreteMediator();
    void SetColleageA(Colleage*);
    void SetColleageB(Colleage*);
    virtual void SendMsg(string msg,Colleage*);
protected:
private:
    Colleage* m_ColleageA;
    Colleage* m_ColleageB;
};
#endif
```
Mediator.cpp
```cpp
#include "Mediator.h"
#include <iostream>
#include <string>
using namespace std;
Colleage::Colleage(Mediator* pMediator)
{
    this->_mediator = pMediator;
}
Colleage::~Colleage()
{}
void Colleage::SetMediator(Mediator* pMediator)
{
    this->_mediator = pMediator;
}
ConcreteColleageA::ConcreteColleageA(Mediator* pMediator) : Colleage(pMediator)
{
}
ConcreteColleageA::~ConcreteColleageA()
{
}
void ConcreteColleageA::SendMsg(string msg)
{
    this->_mediator->SendMsg(msg,this);
}
void ConcreteColleageA::GetMsg(string msg)
{
    cout << "ConcreteColleageA Receive:"<< msg << endl;
}
ConcreteColleageB::ConcreteColleageB(Mediator* pMediator) : Colleage(pMediator)
{
}
ConcreteColleageB::~ConcreteColleageB()
{
}
void ConcreteColleageB::SendMsg(string msg)
{
    this->_mediator->SendMsg(msg,this);
}
void ConcreteColleageB::GetMsg(string msg)
{
    cout << "ConcreteColleageB Receive:" << msg << endl;
}
Mediator::Mediator()
{}
Mediator::~Mediator()
{}
ConcreteMediator::ConcreteMediator()
{}
ConcreteMediator::~ConcreteMediator()
{}
void ConcreteMediator::SetColleageA(Colleage* p)
{
    this->m_ColleageA = p;
}
void ConcreteMediator::SetColleageB(Colleage* p)
{
    this->m_ColleageB = p;
}
void ConcreteMediator::SendMsg(string msg,Colleage* p)
{
    if(p == this->m_ColleageA)
    {
        this->m_ColleageB->GetMsg(msg);
    }
    else if(p == this->m_ColleageB)
    {
        this->m_ColleageA->GetMsg(msg);
    }
}
```
Main.h
```cpp
#include "Mediator.h"
int main()
{
    ConcreteMediator* pMediator = new ConcreteMediator();
    Colleage* p1 = new ConcreteColleageA(pMediator);
    Colleage* p2 = new ConcreteColleageB(pMediator);
    pMediator->SetColleageA(p1);
    pMediator->SetColleageB(p2);
    p1->SendMsg("xxx");
    p2->SendMsg("ooo");
    return 0;
}
```
Mediator模式的代码说明：
         Mediator模式的实现关键就是将对象Colleague之间的通信封装到一个类种单独处理，为了模拟Mediator模式的功能，这里给每个Colleague对象一个string型别以记录其状态，并通过状态改变来演示对象之间的交互和通信。注意：两个Colleague对象并不知道它交互的对象，并且也不是显示地处理交互过程，这一切都是通过Mediator对象完成的，示例程序运行的结果也正是证明了这一点。
**Mediator模式的优缺点总结：**
（1）.优点是，各个 Colleague 减少了耦合。
（2）.缺点是，由于 Mediator 控制了集中化，于是就把 Colleague 之间的交互复杂性变为了中介者的复杂性，也就是中介者会变的比任何一个 Colleague 都复杂。
**Mediator模式使用总结：**
       Mediator模式是一种很有用并且很常用的模式，它通过将对象间的通信封装到一个类中，将多对多的通信转化为一对多的通信，降低了系统的复杂性。Mediator还获得系统解耦的特性，通过Mediator，各个Colleague就不必维护各自通信的对象和通信协议，降低了系统的耦合性，Mediator和各个Colleague就可以相互独立地修改了。
       Mediator模式还有一个很显著额特点就是将控制集中，集中的优点就是便于管理，也正式符合了OO设计中的每个类的职责要单一和集中的原则。
 Mediator中介者模式很容易在系统中应用，也很容易在系统中误用。当系统中出现了“多对多”交互复杂的对象群时，不要急于使用中介者模式，而要先反思你的系统在设计上是不是合理。

