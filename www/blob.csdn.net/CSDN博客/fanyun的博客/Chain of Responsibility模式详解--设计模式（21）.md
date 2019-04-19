# Chain of Responsibility模式详解--设计模式（21） - fanyun的博客 - CSDN博客
2016年07月08日 08:43:19[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：648标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**Chain of Responsibility模式来源：**
        熟悉VC/MFC的都知道，VC是“基于消息，事件驱动”，消息在VC开发中起着举足轻重的作用。在MFC中，消息是通过一个向上递交的方式进行处理，例如个WM_COMMAND消息的处理流程:
  注明：有关MFC消息处理更加详细信息，请参考候捷先生的《深入浅出MFC》。
        MFC提供了消息的处理的链式处理策略，处理消息的请求将沿着预先定义好的路径依次进行处理。消息的发送者并不知道该消息最后是由那个具体对象处理的，当然它也无须也不想知道，但是结构是该消息被某个对象处理了，或者一直到一个终极的对象进行处理了。Chain of Responsibility模式描述其实就是这样一类问题将可能处理一个请求的对象链接成一个链，并将请求在这个链上传递，直到有对象处理该请求（可能需要提供一个默认处理所有请求的类，例如MFC中的CwinApp类。
       实际上现实中也有很多这样的例子，比如员工请假审核，每个公司都有自己的请假制度。我们不妨假设：请假半天只要和部门主管说一声就行了，请假在1天要通过人事部门，而请假超过2天就不那么好申请了，这时可能要总经理或者更高级别的人同意才行了。如果不考虑设计模式直接写代码，要完成这个逻辑就可能用到if—else或者多个if了，看到这个代码，就知道了存在许多的问题，比如现在要扩展怎么办呢？比如又有了董事长，可以处理一个月的请假，这时就要修改这么Manager类中的deal_holiday方法了，显然不符合单一职责原则（SRP）和开放-封闭原则（OCP），这种类自然也是不合理的类了。现在可以说一说职责链模式了，实际上这种情形是适合职责链模式的：现在有一个请假信息需要处理，首先是主管处理；主管处理不了，交给人事部门处理；人事部门处理不了，交给总经理处理（如果总经理是最后可能处理请假信息的管理者，那么总经理一定会给出一个答复）。这就构成了一个职责链。我们就可以选择Chain
 of Responsibility模式来实现。如果我们把每个管理者都写一个类，每个管理者都负责处理请假信息，但是只有当主管处理不了时，才交给人事部门处理，以此类推。那么就是可以写出来一个职责链模式的代码。
**Chain of Responsibility模式作用：**
         使多个对象都有机会处理请求，从而避免请求的发送者和接受者之间的耦合关系。将这个对象连成一条链，并沿着这条链传递该请求，直到有一个对象处理他为止。使系统的扩展时更方便，只需要在增加管理者类，并且这个"职责链”是在客户端动态确定的。
**Chain of Responsibility模式UML结构图：**
![](https://img-blog.csdn.net/20160707094157596?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Chain of Responsibility模式结构：**
        抽象处理者角色(Handler)：定义出一个处理请求的接口。如果需要，接口可以定义出一个方法，以设定和返回对后继的引用。这个角色通常由一个抽象类或接口实现。
         具体处理者角色(ConcreteHandler)：具体处理者接到请求后，可以选择将请求处理掉，或者将请求传给后继，没有的话就自己处理。由于具体处理者持有对后继的引用，因此，如果需要，具体处理者可以访问后继。
Chain of Responsibility模式代码示例：
ChainMode.h
```cpp
#include <iostream>
#include <string>
using namespace std;
//管理者类
class Manager
{
private:
	string name;
	Manager *nextManager;
public:
	Manager(string n, Manager *m = NULL) : name(n), nextManager(m){};
	void set_name(string name);
	string get_name();
	
	//设置下一个要处理请假信息的管理者
	void set_next_manager(Manager *m);
	
	Manager* get_next_manager();
	//处理请假
	virtual void deal_holiday(int day);
};
//主管类
class ZhuGuan : public Manager
{
public:
	ZhuGuan(string n, Manager *m = NULL) :Manager(n, m){};
	virtual void deal_holiday(int day);
};
//人事部门类
class RenShi : public Manager
{
public:
	RenShi(string n, Manager *m = NULL) :Manager(n, m){};
	virtual void deal_holiday(int day);
};
class ZongJingLi : public Manager
{
public:
	ZongJingLi(string n, Manager *m = NULL) :Manager(n, m){};
	virtual void deal_holiday(int day);
};
```
ChainMode.cpp
```cpp
#include <iostream>
#include <string>
#include "ChainMode.h"
using namespace std;
void ZhuGuan::deal_holiday(int day)
{
	if (day <= 0.5)
	{
		cout << "主管：" << get_name() << "同意请假!!" << endl;
	}
	else if (get_next_manager() != NULL)
	{
		get_next_manager()->deal_holiday(day);
	}
}
void RenShi::deal_holiday(int day)
{
	if (day < 2)
	{
		cout << "人事部：" << get_name() << "同意请假!!" << endl;
	}
	else if (get_next_manager() != NULL)
	{
		get_next_manager()->deal_holiday(day);
	}
}
void ZongJingLi::deal_holiday(int day)
{
	if (day < 30)
	{
		cout << "总经理：" << get_name() << "同意请假!!" << endl;
	}
	//总经理室最后一个进行处理的，所以一定要给出一个答复
	else
	{
		cout << "总经理：" << get_name() << "关于你说的请假的事情，由于时间太长，以后再说吧!!" << endl;
	}
}
```
Main.cpp
```cpp
#include <iostream>
#include <string>
#include "ChainMode.h"
int main()
{
	//主管Mike
	ZhuGuan Z_G("Mike");
	//人事部Tom
	RenShi R_S("Tom");
	//总经理Vincent
	ZongJingLi Z_J_L("Vincent");
	Z_G.set_next_manager(&R_S);
	R_S.set_next_manager(&Z_J_L);
	Z_G.deal_holiday(0.5);
	Z_G.deal_holiday(1);
	Z_G.deal_holiday(3);
	Z_G.deal_holiday(31);
	return 0;
}
```
**Chain of Responsibility模式的优缺点：**
          Chain of Responsibility模式的优点：
(1).增强了系统的可扩展性。
(2).使用职责链模式可以避免众多的if或者if-else语句。
(3).使用职责链模式可以减小对象之间的耦合性。使得对象之间的联系减小。
(4).可以根据需要自由组合工作流程。如工作流程发生变化，可以通过重新分配对象链便可适应新的工作流程。
(5).责任的分担。每个类只需要处理自己该处理的工作（不该处理的传递给下一个对象完成），明确各类的责任范围，符合类的最小封装原则。
          Chain of Responsibility模式的缺点：
(1).使用职责链模式的时候会有多个处理者对象，但是实际使用的处理者对象却只有一个，这在某种程度讲是资源的浪费。
(2).同时职责链的建立的合理性要靠客户端来保证，增加了程序的复杂性，也有可能由于职责链导致出错。
**Chain of Responsibility模式的使用总结：**
          Chain of Responsibility模式的最大的一个优点就是给系统降低了耦合性，请求的发送者完全不必知道该请求会被哪个应答对象处理，极大地降低了系统的耦合性。
