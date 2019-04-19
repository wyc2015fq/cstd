# Memento模式详解--设计模式（17） - fanyun的博客 - CSDN博客
2016年07月02日 08:48:07[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：753
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**Memento模式来源：**
我们在进行软件系统的设计时候是要给用户后悔的权利（实际上可能也是用户要求的权利），我们对一些关键性的操作肯定需要提供诸如撤销（Undo）的操作。那这个后悔药就是Memento模式提供的。
**Memento模式作用：**
    在不破坏封装性的前提下，捕获一个对象的内部状态，并在该对象之外保存这个状态，这样以后就可将该对象恢复到原先保存的状态。
**Memento模式UML结构图如图1所示：**
**![](https://img-blog.csdn.net/20160701142133314?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
Memento模式构成：
      Originator：负责创建一个备忘录Memento，用以记录当前时刻它的内部状态，并可使用备忘录恢复内部状态。Originator可根据需要决定Memento存储Originator的哪些内部状态。
      Memento:负责存储Originator对象的内部状态，并可防止Originator以外的其他对象访问备忘录Memento。备忘录有两个接口，Caretaker只能看到备忘录的窄接口，它只能将备忘录传递给其他对象。Originator能够看到一个宽接口，允许它访问返回到先前状态所需的所有数据。
      Caretaker：负责保存好备忘录Memento,不能对备忘录的内容进行操作或检查。
Memento模式代码示例：
Memento.h
```cpp
#ifndef _MEMENTO_H_
#define _MEMENTO_H_
#include <string>
using namespace std;
//负责存储Originator对象的内部状态，并可防止Originator以外的其他对象访问备忘录Memento。
//备忘录有两个接口，Caretaker只能看到备忘录的窄接口，它只能将备忘录传递给其他对象。Originator能够看到一个宽接口，允许它访问返回到先前状态所需的所有数据。
class Memento
{
private:
    //将Originator为friend类，可以访问内部信息，但是其他类不能访问
    friend class Originator;
    Memento(const string& state);
    ~Memento();
    void SetState(const string& state);
    string GetState();
    string _state;
};
//负责创建一个备忘录Memento，用以记录当前时刻它的内部状态，并可使用备忘录恢复内部状态
class Originator
{
public:
    Originator();
    Originator(const string& state);
    ~Originator();
    void RestoreToMemento(Memento* pMemento);
    Memento* CreateMemento();
    void SetState(const string& state);
    string GetState();
    void show();
protected:
private:
    string _state;
};
//负责保存好备忘录Memento,不能对备忘录的内容进行操作或检查
class Caretaker
{
public:
    Caretaker();
    ~Caretaker();
    void SetMemento(Memento*);
    Memento* GetMemento();
private:
    Memento* _memento;
};
#endif</span>
```
Memento.cpp
```cpp
#include "Memento.h"
#include <iostream>
#include <string>
using namespace std;
Memento::Memento(const string& state)
{
    this->_state = state;
}
Memento::~Memento()
{}
string Memento::GetState()
{
    return this->_state;
}
void Memento::SetState(const string& state)
{
    this->_state = state;
}
Originator::Originator()
{}
Originator::Originator(const string& state)
{
    this->_state = state;
}
Originator::~Originator()
{}
string Originator::GetState()
{
    return this->_state;
}
void Originator::show()
{
    cout << this->_state << endl;
}
void Originator::SetState(const string& state)
{
    this->_state = state;
}
Memento* Originator::CreateMemento()
{
    return new Memento(this->_state);
}
void Originator::RestoreToMemento(Memento* pMemento)
{
    this->_state = pMemento->GetState();
}
Caretaker::Caretaker()
{}
Caretaker::~Caretaker()
{}
Memento* Caretaker::GetMemento()
{
    return this->_memento;
}
void Caretaker::SetMemento(Memento* pMemento)
{
    this->_memento = pMemento;
}</span>
```
Main.cpp
```cpp
#include "Memento.h"
int main()
{
    //初始化对象，状态为“Old”
    Originator* o = new Originator("Old");
    o->show();
    //建立并保存Memento
    Caretaker* pTaker = new Caretaker();
    pTaker->SetMemento(o->CreateMemento());
    //改变状态
    o->SetState("New");
    o->show();
    //恢复状态
    o->RestoreToMemento(pTaker->GetMemento());
    o->show();
    return 0;
}</span>
```
**Memento模式的适用性：**
(1).Memento模式比较适用于功能比较复杂的，但需要维护或记录历史属性的类，或者需要保存的属性只是众多属性中的一小部分时，Originator可以根据保存的Memento信息还原到前一状态。如果在某个系统中使用命令模式时，需要实现命令的撤销功能，那么命令模式可以使用备忘录模式来存储可撤销操作的状态。
**Memento模式的优缺点：**
优点：
(1).当发起人角色的状态有改变时，有可能是个错误的改变，我们使用备忘录模式就可以把这个错误改变还原。
(2).备份的状态是保存在发起人角色之外的，这样，发起人角色就不需要对各个备份的状态进行管理。
缺点：
(1).如果备份的对象存在大量的信息或者创建、恢复操作非常频繁，则可能造成很大的性能开销。
  Memento模式的使用总结：Memento模式的关键就是要在不破坏封装行的前提下，捕获并保存一个类的内部状态，这样就可以利用该保存的状态实施恢复操作。为了达到这个目标，可以在后面的实现中看到我们采取了一定语言支持的技术
      Memento模式的关键就是friend class Originator;我们可以看到，Memento的接口都声明为private，而将Originator声明为Memento的友元类。我们将Originator的状态保存在Memento类中，而将Memento接口private起来，也就达到了封装的功效。
 
     在Originator类中我们提供了方法让用户后悔：RestoreToMemento(Memento* mt)；我们可以通过这个接口让用户后悔。在示例程序中，我们演示了这一点：Originator的状态由old变为new最后又回到了old。
