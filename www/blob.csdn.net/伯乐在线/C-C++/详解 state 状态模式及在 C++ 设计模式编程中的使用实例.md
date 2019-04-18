# 详解 state 状态模式及在 C++ 设计模式编程中的使用实例 - 文章 - 伯乐在线
原文出处： [梦在天涯](http://www.jb51.net/article/80713.htm)
每个人、事物在不同的状态下会有不同表现（动作），而一个状态又会在不同的表现下转移到下一个不同的状态（State）。最简单的一个生活中的例子就是：地铁入口处，如果你放入正确的地铁票，门就会打开让你通过。在出口处也是验票，如果正确你就可以 ok，否则就不让你通过（如果你动作野蛮，或许会有报警（Alarm），：））。
有限状态自动机（FSM）也是一个典型的状态不同，对输入有不同的响应（状态转移）。
通常我们在实现这类系统会使用到很多的 Switch/Case 语句，Case 某种状态，发生什么动作，Case 另外一种状态，则发生另外一种状态。但是这种实现方式至少有以下两个问题：
当状态数目不是很多的时候，Switch/Case 可能可以搞定。但是当状态数目很多的时候（实际系统中也正是如此），维护一大组的 Switch/Case 语句将是一件异常困难并且容易出错的事情。
状态逻辑和动作实现没有分离。在很多的系统实现中，动作的实现代码直接写在状态的逻辑当中。这带来的后果就是系统的扩展性和维护得不到保证。
状态模式就是被用来解决上面列出的两个问题的，在状态模式中我们将状态逻辑和动作实现进行分离。当一个操作中要维护大量的 case 分支语句，并且这些分支依赖于对象的状态。状态模式将每一个分支都封装到独立的类中。
**状态模式典型的结构图为：**
![201639173716593.jpg (558×259)](http://jbcdn2.b0.upaiyun.com/2016/12/89b12107585823bef0f8be8e24afed27.jpg)
**状态模式的实现**代码片断 1：State.h

```
//state.h
#ifndef _STATE_H_
#define _STATE_H_
class Context; //前置声明
class State{
  public:
  State();
  virtual ~State();
  virtual void OperationInterface(Context* ) = 0;
  virtual void OperationChangeState(Context*) = 0;
  protected:
  bool ChangeState(Context* con,State* st);
  private:
  //bool ChangeState(Context* con,State* st);
};
class ConcreteStateA:public State{
  public:
  ConcreteStateA();
  virtual ~ConcreteStateA();
  virtual void OperationInterface(Context* );
  virtual void OperationChangeState(Context*);
  protected:
  private:
};
class ConcreteStateB:public State{
  public:
  ConcreteStateB();
  virtual ~ConcreteStateB();
  virtual void OperationInterface(Context* );
  virtual void OperationChangeState(Context*);
  protected:
  private:
};
#endif //~_STATE_H_
```
代码片断 2：State.cpp

```
//State.cpp
#include "State.h"
#include "Context.h"
#include <iostream>
using namespace std;
State::State(){
}
State::~State(){
}
void State::OperationInterface(Context* con){
  cout<<"State::.."<<endl;
}
bool State::ChangeState(Context* con,State* st){
  con->ChangeState(st);
  return true;
}
void State::OperationChangeState(Context* con){
}
///
ConcreteStateA::ConcreteStateA(){
}
ConcreteStateA::~ConcreteStateA(){
}
void ConcreteStateA::OperationInterface(Context* con){
  cout<<"ConcreteStateA::OperationInterface
  ......"<<endl;
}
void ConcreteStateA::OperationChangeState(Context* con){
  OperationInterface(con);
  this->ChangeState(con,new ConcreteStateB());
}
///
ConcreteStateB::ConcreteStateB(){
}
ConcreteStateB::~ConcreteStateB(){
}
void ConcreteStateB::OperationInterface(Context* con){
  cout<<"ConcreteStateB::OperationInterface......"<<endl;
}
void ConcreteStateB::OperationChangeState(Context* con){
  OperationInterface(con);
  this->ChangeState(con,new ConcreteStateA());
}
```
代码片断 3：Context.h

```
//context.h
#ifndef _CONTEXT_H_
#define _CONTEXT_H_
class State;
/**
*
**/
class Context{
  public:
  Context();
  Context(State* state);
  ~Context();
  void OprationInterface();
  void OperationChangState();
  protected:
  private:
  friend class State; //表明在 State 类中可以访问 Context 类的 private 字段
  bool ChangeState(State* state);
  private:
  State* _state;
};
#endif //~_CONTEXT_H_
```
代码片断 4：Context.cpp

```
//context.cpp
#include "Context.h"
#include "State.h"
Context::Context(){
}
Context::Context(State* state){
  this->_state = state;
}
Context::~Context(){
  delete _state;
}
void Context::OprationInterface(){
  _state->OperationInterface(this);
}
bool Context::ChangeState(State* state){
  ///_state->ChangeState(this,state);
  this->_state = state;
  return true;
}
void Context::OperationChangState(){
  _state->OperationChangeState(this);
}
```
代码片断 5：main.cpp

```
//main.cpp
#include "Context.h"
#include "State.h"
#include <iostream>
using namespace std;
int main(int argc,char* argv[]){
  State* st = new ConcreteStateA();
  Context* con = new Context(st);
  con->OperationChangState();
  con->OperationChangState();
  con->OperationChangState();
  if (con != NULL)
    delete con;
  if (st != NULL)
    st = NULL;
  return 0;
}
```
代码说明：状态模式在实现中，有两个关键点：
1.将状态声明为 Context 的友元类（friend class），其作用是让状态模式访问 Context的 protected 接口 ChangeSate（）。
状态及其子类中的操作都将 Context*传入作为参数，其主要目的是状态类可以通过这个指针调用 Context 中的方法（在本示例代码中没有体现）。这也是状态模式和 Strategy模式的最大区别所在。
2.运行了示例代码后可以获得以下的结果：连续 3 次调用了 Context 的 OprationInterface（）因为每次调用后状态都会改变（A－B－A），因此该动作随着 Context 的状态的转变而获得了不同的结果。
**关于State模式的一些需要注意的地方**
这个模式使得软件可以在不同的state下面呈现出完全不同的特征
- 不同的theme使得相同的元素呈现出不同的特点
- 不同的state下面相同的操作产生不同的效果
- 不同的状态对相同的信息产生不同的处理
这个模式使得操作的state逻辑更加的清楚，省去了无数的state判断，而state的扩展性和可维护性和执行效率也大幅度的上升。关于state，有如下几点要注意的地方：
**1.所有的state应该被一个类（State Manager Class）管理:**
state之间的跳转和转换是非常复杂的，有时一些state可能要跳转的目标state有几十个，这个时候我们需要一个管理类（State Manager ）来统一的管理这些state的切换，例如目标state的初始化和申请跳转state的结束处理，以及一些state间共享数据的存储和处理。与其称这个Manager 为管理类，不如说是一个中间类，它实现了state之间的解隅，使得各个state之间不比知道target state的具体信息，而只要向Manager申请跳转就可以了。使得各个state的模块化更好，更加的灵活
**2.所有的state都应该从一个state基类继承:**
既然state要教给一个manager来管理，那么自然的，这些state都应该从一个父类继承下来，这样manager并不需要知道很多子类的信息，一个最单纯的manager只要只要管理一个这样的基类的指针就可以了。另外，我们还可以统一的把state的一些共有的属性放在这里
**3.state应该实现为一个singleton:**
state并不需要总是被申请，这样可能会造成管理上的混乱，state资源的申请也不应该可以任意进行，事实上，state的申请权限应该只有 Manager才有，并且有且只有一次。在这样的情况下，state的构造函数似乎应该被声明为protected or private ，而Manager应该被声明为state的友元，但是友元被看成是破坏类的封装性的一种做法，这一点上，我很矛盾，所以在这一条上我只能采取一种漠视的态度。
**4.应该做一个state么？这是一个问题：**
state可以说是if-else的一种替代品，极端的情况下面state可以让你的程序中if-else程序块消失得无影无踪，但是，这并不是银弹。state对于状态可预知的情况下非常有效，但是对于state不可预知，或者相似的state数量太多。过多的state会造成class的粒度过细，程序反而不简洁。在这样的情况下，你应该考虑使用if-else程序块来替代state。
例如：
有这样的一个程序，它可以生成任意形状的多边形，而多边形的各个节点是可以移动的，问题就来了。
我并不知道用户将要使用多少个节点的多边形，因此我无法的创建那么多相应的state来使得这样一个程序正常工作。state大多数都是确定的，对于不确定的，state似乎无能为力，例如此例
一种解决方法是我利用Manager传递给state一个state参数，让state有机会知道用户的操作意图，在这个例子里面是让state知道用户打算操作某一个节点，而state根据这个state参数来处理用户的操作，比如说，state得到的是用户操作的某一个点的index ,而state只要写
points[index].moveTo(points[index].getX()+offset_x , points[index].getY()+offset_y)；
就可以，从而避免了state过多出现的问题。
