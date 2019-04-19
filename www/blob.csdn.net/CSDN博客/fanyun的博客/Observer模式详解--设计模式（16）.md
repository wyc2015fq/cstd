# Observer模式详解--设计模式（16） - fanyun的博客 - CSDN博客
2016年07月02日 08:47:42[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2332
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**Observer模式来源：**
Observer模式应该可以说是应用最多、影响最广的模式之一。
         因为Observer的一个实例Model/View/Control（MVC）结构在系统开发架构设计中有着很重要的地位和意义，MVC实现了业务逻辑和表示层的解耦。在MFC中，Doc/View（文档视图结构）提供了实现MVC的框架结构（有一个从设计模式（Observer模式）的角度分析分析Doc/View。在Java阵容中，Struts则提供和MFC中Doc/View结构类似的实现MVC的框架。另外Java语言本身就提供了Observer模式的实现接口。
        当然，MVC只是Observer模式的一个实例。Observer模式要解决的问题为：建立一个一（Subject）对多（Observer）的依赖关系，并且做到当“一”变化的时候，依赖这个“一”的多也能够同步改变。
        最常见的一个例子就是：对同一组数据进行统计分析时候，我们希望能够提供多种形式的表示（例如以表格进行统计显示、柱状图统计显示、百分比统计显示等）。这些表示都依赖于同一组数据，我们当然需要当数据改变的时候，所有的统计的显示都能够同时改变。Observer模式就是解决了这一个问题而产生的，如图1所示。
![](https://img-blog.csdn.net/20160701102402636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                                        图1  Observer模式示例图
**Observer模式作用：**
观察者模式定义了一种一对多的依赖关系，让多个观察者对象同时监听某一个主题对象，这个主题对象在状态发生变化时，会通知所有观察者对象，使它们能够自动更新自己。
**Observer模式UML结构图如图2所示：**
**![](https://img-blog.csdn.net/20160701104915254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
Observer模式的构成：
Subject类：可翻译为主题或抽象通知者，一般用一个抽象类或者一个接口实现。它把所有对观察者对象的引用保存在一个聚集里，每个主题都可以有任何数量的观察者。抽象主题提供一个接口，可以增加和删除观察者对象。
      Observer类：抽象观察者，为所有的具体观察者定义一个接口，在得到主题的通知时更新自己。这个借口叫做更新接口。抽象观察者一般用一个抽象类或者一个接口实现。更新接口通常包含一个Update()方法。
      ConcreteSubject类：叫做具体主题或具体通知者，将有关状态存入具体通知者对象；在具体主题的内部状态改变时，给所有登记过的观察者发出通知。通常用一个具体子类实现。
      ConcreteObserver类：具体观察者，实现抽象观察者角色所要求的更新接口，以便使本身的状态与主题的状态相协调。具体观察者角色可以保存一个指向一个具体主题对象的引用。
Observer模式的代码示例：
**Observer.h**
```cpp
#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <string>
#include <list>
using namespace std;
class Subject;
class Observer
{
public:
    ~Observer();
    virtual void Update(Subject*)=0;
protected:
    Observer();
private:
};
class ConcreteObserverA : public Observer
{
public:
    ConcreteObserverA();
    ~ConcreteObserverA();
    virtual void Update(Subject*);
protected:
private:
    string m_state;
};
class ConcreteObserverB : public Observer
{
public:
    ConcreteObserverB();
    ~ConcreteObserverB();
    virtual void Update(Subject*);
protected:
private:
    string m_state;
};
class Subject
{
public:
    ~Subject();
    virtual void Notify();
    virtual void Attach(Observer*);
    virtual void Detach(Observer*);
    virtual string GetState();
    virtual void SetState(string state);
protected:
    Subject();
private:
    string m_state;
    list<Observer*> m_lst;
};
class ConcreteSubjectA : public Subject
{
public:
    ConcreteSubjectA();
    ~ConcreteSubjectA();
protected:
private:
};
class ConcreteSubjectB : public Subject
{
public:
    ConcreteSubjectB();
    ~ConcreteSubjectB();
protected:
private:
};
#endif
```
**Observer.cpp**
```cpp
#include "Observer.h"
#include <iostream>
#include <algorithm>
using namespace std;
Observer::Observer()
{}
Observer::~Observer()
{}
ConcreteObserverA::ConcreteObserverA()
{}
ConcreteObserverA::~ConcreteObserverA()
{}
void ConcreteObserverA::Update(Subject* pSubject)
{
    this->m_state = pSubject->GetState();
    cout << "The ConcreteObserverA is " << m_state << std::endl;
}
ConcreteObserverB::ConcreteObserverB()
{}
ConcreteObserverB::~ConcreteObserverB()
{}
void ConcreteObserverB::Update(Subject* pSubject)
{
    this->m_state = pSubject->GetState();
    cout << "The ConcreteObserverB is " << m_state << std::endl;
}
Subject::Subject()
{}
Subject::~Subject()
{}
void Subject::Attach(Observer* pObserver)
{
    this->m_lst.push_back(pObserver);
    cout << "Attach an Observer\n";
}
void Subject::Detach(Observer* pObserver)
{
    list<Observer*>::iterator iter;
    iter = find(m_lst.begin(),m_lst.end(),pObserver);
    if(iter != m_lst.end())
    {
        m_lst.erase(iter);
    }
    cout << "Detach an Observer\n";
}
void Subject::Notify()
{
    list<Observer*>::iterator iter = this->m_lst.begin();
    for(;iter != m_lst.end();iter++)
    {
        (*iter)->Update(this);
    }
}
string Subject::GetState()
{
    return this->m_state;
}
void Subject::SetState(string state)
{
    this->m_state = state;
}
ConcreteSubjectA::ConcreteSubjectA()
{}
ConcreteSubjectA::~ConcreteSubjectA()
{}
ConcreteSubjectB::ConcreteSubjectB()
{}
ConcreteSubjectB::~ConcreteSubjectB()
{}
```
Main.cpp
```cpp
#include "Observer.h"
#include <iostream>
using namespace std;
int main()
{
    Observer* p1 = new ConcreteObserverA();
    Observer* p2 = new ConcreteObserverB();
    Observer* p3 = new ConcreteObserverA();
    Subject* pSubject = new ConcreteSubjectA();
    pSubject->Attach(p1);
    pSubject->Attach(p2);
    pSubject->Attach(p3);
    pSubject->SetState("old");
    pSubject->Notify();
    cout << "-------------------------------------" << endl;
    pSubject->SetState("new");
    pSubject->Detach(p3);
    pSubject->Notify();
    return 0;
}
```
        在Observer模式的实现中，Subject维护一个list作为存储其所有观察者的容器。每当调用Notify操作就遍历list中的Observer对象，并广播通知改变状态（调用Observer的Update操作）。目标的状态state可以由Subject自己改变（示例），也可以由Observer的某个操作引起state的改变（可调用Subject的SetState操作）。Notify操作可以由Subject目标主动广播（示例），也可以由Observer观察者来调用（因为Observer维护一个指向Subject的指针）。运行示例程序，可以看到当Subject处于状态“old”时候，依赖于它的两个观察者都显示“old”，当目标状态改变为“new”的时候，依赖于它的两个观察者也都改变为“new”。
**Observer模式****使用场景：**
（1）.当一个对象的改变需要同时改变其他对象的时候，而且它不知道具体有多少对象有待改变时，应该考虑使用观察者模式。
（2）.观察者模式所做的工作其实就是在解除耦合。让耦合的双方都依赖于抽象，而不是依赖于具体。从而使得各自的变化都不会影响另一边的变化。
**Observer模式优缺点总结：**
观察者模式的优点：
（1）. Subject和Observer之间是松耦合的，分别可以各自独立改变。
（2）. Subject在发送广播通知的时候，无须指定具体的Observer，Observer可以自己决定是否要订阅Subject的通知。
（3）. 遵守大部分GRASP原则和常用设计原则，高内聚、低耦合。
观察者模式的缺点：
（1）.松耦合导致代码关系不明显，有时可能难以理解。
（2）. 如果一个Subject被大量Observer订阅的话，在广播通知的时候可能会有效率问题。
**Observer模式的使用总结：**
        Observer是影响极为深远的模式之一，也是在大型系统开发过程中要用到的模式之一。除了MFC、Struts提供了MVC的实现框架，在Java语言中还提供了专门的接口实现Observer模式：通过专门的类Observable及Observer接口来实现MVC编程模式，其UML图可以表示为：Java中实现MVC的UML图。这里的Observer就是观察者，Observable则充当目标Subject的角色。
![](https://img-blog.csdn.net/20160701111212451?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       Observer模式也称为发布－订阅（publish-subscribe），目标就是通知的发布者，观察者则是通知的订阅者（接受通知）。
