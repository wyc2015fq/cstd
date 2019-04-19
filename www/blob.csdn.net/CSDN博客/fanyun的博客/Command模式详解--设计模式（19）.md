# Command模式详解--设计模式（19） - fanyun的博客 - CSDN博客
2016年07月06日 08:58:20[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3603
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**Command模式来源：**
命令模式又称为行动（Action）模式或事务（Transaction）模式，Command模式通过将请求封装到一个对象（Command）中，并将请求的接受者存放到具体的ConcreteCommand类中（Receiver）中，从而实现调用操作的对象和操作的具体实现者之间的解耦。
**Command模式作用：**
        将一个请求封装为一个对象，从而使你可用不同的请求对客户进行参数化；对请求排队或记录请求日志，以及支持可撤销的操作。由于“行为请求者”与“行为实现者”的紧耦合，使用命令模式，可以对请求排队或记录请求日志，以及支持可撤销的操作。 
Command模式UML结构图如图1所示：
**![](https://img-blog.csdn.net/20160706140334796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
Command模式构成：
ConcreteCommand将一个接收者对象绑定于一个操作，调用接收者相应的操作，以实现Execute。
客户（Client）类：创建了一个具体命令(ConcreteCommand)对象并确定其接收者。
命令（Command）类：声明了一个给所有具体命令类的抽象接口。这是一个抽象角色。
具体命令（ConcreteCommand）类：定义一个接受者和行为之间的弱耦合；实现Execute()方法，负责调用接收考的相应操作。Execute()方法通常叫做执方法。
请求者（Invoker）类：负责调用命令对象执行请求，相关的方法叫做行动方法。
接收者（Receiver）类：负责具体实施和执行一个请求。任何一个类都可以成为接收者，实施和执行请求的方法叫做行动方法。
**Command模式代码示例：**
Command.h
```cpp
#ifndef _COMMAND_H_
#define _COMMAND_H_
class Command
{
public:
    virtual ~Command();
    virtual void Execute()=0;
protected:
    Command();
private:
};
class Receiver;
class ConcreteCommand : public Command
{
public:
    ConcreteCommand(Receiver* pReceiver);
    ~ConcreteCommand();
    virtual void Execute();
protected:
private:
    Receiver* _recv;
};
class Invoker
{
public:
    Invoker(Command* pCommand);
    ~Invoker();
    void Invoke();
protected:
private:
    Command* _cmd;
};
class Receiver
{
public:
    Receiver();
    ~Receiver();
    void Action();
protected:
private:
};
#endif
```
Command.cpp
```cpp
#include "Command.h"
#include <iostream>
using namespace std;
Command::Command()
{}
Command::~Command()
{}
ConcreteCommand::ConcreteCommand(Receiver* pReceiver)
{
    this->_recv = pReceiver;
}
ConcreteCommand::~ConcreteCommand()
{}
void ConcreteCommand::Execute()
{
    this->_recv->Action();
}
Receiver::Receiver()
{}
Receiver::~Receiver()
{}
void Receiver::Action()
{
    cout << "Receiver::Action" << endl;
}
Invoker::Invoker(Command* pCommand)
{
    this->_cmd = pCommand;
}
Invoker::~Invoker()
{}
void Invoker::Invoke()
{
    this->_cmd->Execute();
}
```
Main.cpp
```cpp
#include "Command.h"
int main()
{
    //创建具体命令对象pCmd并设定它的接收者pRev
    Receiver* pRev = new Receiver();
    Command* pCmd = new ConcreteCommand(pRev);
    //请求绑定命令
    Invoker* pInv = new Invoker(pCmd);
    pInv->Invoke();
    return 0;
}</span>
```
Command模式适用性：
在下面的情况下应当考虑使用命令模式：
(1).使用命令模式作为"CallBack"在面向对象系统中的替代。"CallBack"讲的便是先将一个函数登记上，然后在以后调用此函数。
(2).需要在不同的时间指定请求、将请求排队。一个命令对象和原先的请求发出者可以有不同的生命期。换言之，原先的请求发出者可能已经不在了，而命令对象本身仍然是活动的。这时命令的接收者可以是在本地，也可以在网络的另外一个地址。命令对象可以在串形化之后传送到另外一台机器上去。
(3).系统需要支持命令的撤消(undo)。命令对象可以把状态存储起来，等到客户端需要撤销命令所产生的效果时，可以调用undo()方法，把命令所产生的效果撤销掉。命令对象还可以提供redo()方法，以供客户端在需要时，再重新实施命令效果。
(4).如果一个系统要将系统中所有的数据更新到日志里，以便在系统崩溃时，可以根据日志里读回所有的数据更新命令，重新调用Execute()方法一条一条执行这些命令，从而恢复系统在崩溃前所做的数据更新。
Command模式优缺点：
**Command模式的优点：**
(1).它能较容易地设计一个命令队列；
(2).在需要的情况下，可以较容易地将命令记入日志；
(3).允许接收请求的一方决定是否要否决请求；
(4).可以容易地实现对请求的撤销和重做；
(5).由于加进新的具体命令类不影响其他的类，因此增加新的具体命令类很容易。
**Command模式的缺点：**
(1).使用命令模式可能会导致某些系统有过多的具体命令类。因为针对每一个命令都需要设计一个具体命令类，因此某些系统可能需要大量具体命令类，这将影响命令模式的使用。
**Command模式使用总结：**
    Command模式把请求一个操作的对象与知道怎么执行一个操作的对象分割开。Command模式关键就是讲一个请求封装到一个类中(Command），再提供处理对象（Receiver），最后Command命令由Invoker激活。另外，我们可以将请求接收者的处理抽象出来作为参数传给Command对象，实际也就是回调的机制来实现这一点。也就是讲处理操作方法地址通过参数传递给Command对象，Command对象在适当的时候再调用该函数。
    Command模式将调用操作的对象和知道如何实现该操作的对象解耦，在上面Command的结构图中,Invoker对象根本就不知道具体的是哪个对象在处理Execute操作（当然要知道是Command类别的对象）。
    在Command要增加新的处理操作对象很容易，我们可以通过创建新的继承自Command的子类来实现这一点。
    Command模式可以和Memento模式结合起来，支持取消的操作。
    Command[模式](http://blog.csdn.net/hguisu/article/details/7549895)的本质是对命令进行封装，将发出命令的责任和执行命令的责任分割开。每一个命令都是一个操作：请求的一方发出请求，要求执行一个操作；接收的一方收到请求，并执行操作。
[Command模式](http://blog.csdn.net/hguisu/article/details/7549895)允许请求的一方和接收的一方独立开来，使得请求的一方不必知道接收请求的一方的接口，更不必知道请求是怎么被接收，以及操作是否被执行、何时被执行，以及是怎么被执行的。
    Command模式使请求本身成为一个对象，这个对象和其他对象一样可以被存储和传递。
    Command模式的关键在于引入了抽象命令接口，且发送者针对抽象命令接口编程，只有实现了抽象命令接口的具体命令才能与接收者相关联。
