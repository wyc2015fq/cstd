# 构造函数中的this指针 - ljx0305的专栏 - CSDN博客
2008年02月28日 13:06:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：640标签：[class																[compiler																[delete																[语言																[c++																[fun](https://so.csdn.net/so/search/s.do?q=fun&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)
**构造函数中的this指针**
作者：[温昱](mailto:xinxiu123@sina.com)
说明：感谢网页C++ FAQ Lite的作者Marshall Cline和译者申旻；感谢《C++语言核心》一书的作者Gregory Satir和Doug Brown及译者张铭泽
-------------------------------原理-------------------------------
某些人认为不应该在构造函数中使用this指针，因为这时this对象还没有完全形成。
但是，只要小心，是可以在构造函数中使用this指针的：
●在函数体中
●初始化列表中
因为“对象还没有完全形成”不意味着“什么都没有”。
在进入构造函数（及其chaining）之前，Compiler会：
●给class的instance分配内存
●建立运行时刻系统所需的信息（如vtbl等）
●##缺省地## 构造所有类成员
![](http://www.vckbase.com/document/journal/vckbase26/images/this-pattern.gif)
-----------------------------【能】---------------------------------
构造函数的函数体（或构造函数所调用的函数）【能】可靠地访问：
●基类中声明的数据成员
●构造函数所属类声明的数据成员
这是因为所有这些数据成员被保证在构造函数函数体开始执行时已经被完整的建立。
-----------------------------【不能】---------------------------------
构造函数的函数体（或构造函数所调用的函数）【不能】向下调用：
●被派生类重定义的虚函数
这是因为在基类的构造函数执行期间，“对象还不是一个派生类的对象”。
---------------------------【有时】-----------------------------------
以下是【有时】可行的：
●传递 this 对象的任何一个数据成员给另一个数据成员的初始化程序
你必须确保该数据成员已经被初始化。好消息是你能使用一些不依赖于你所使用的编译器的显著的语言规则，来确定那个数据成员是否已经（或者还没有）被初始化。坏消息是你必须知道这些语言规则（例如，基类子对象首先被初始化（如果有多重和／或虚继承，则查询这个次序！），然后类中定义的数据成员根据在类中声明的次序被初始化）。如果你不知道这些规则，则不要从this对象传递任何数据成员（不论是否显式的使用了this关键字）给任何其他数据成员的初始化程序！如果你知道这些规则，则需要小心。
----------------------------用途----------------------------------
好的OO设计强调“高聚合”，这样会产生很多小的责任单一的对象(其实“单一责任原则”根本就是最基础的OO原则)。
那么，小对象之间的协作就需要配置(其实“协作可配置”本身就是我们希望的灵活性所在)：
●比如Observer模式中subject和observer的协作需要调subject.RegistorObserver(observer)来配置
●再比如多媒体框架DirectShow中filterGraph和videoWindow的协作需要调filterGraph.SetVideoWindow(videoWindow)来配置
而构造函数是很典型的配置时机，举例如下：
class CMyWindow : public CWnd
{
private:
CFilterGraph filterGraph;
public
CMyWindow() { filterGraph.SetVideoWindow(this); };
};
--------------------------附录------------------------------------
顺便总结基础知识
析构函数subclass::~subclass() ==〉superclass::~superclass()
虚函数析构函数class::vfun()
需要明了的是，【构造/析构/普通】和【虚/非虚】是完全独立的分类方式：
●只要是“构造/析构”就“串联(chaining)”
●只要是“虚函数”就“可能obj.vfun()”
它们可以“一起生效”但“不互相干扰”，比如虚析构函数的情况，看下面的例子：
class superclass
{
        virtual ~superclass() { println("superclass::~superclass()") };
};
class subclass : public superclass
{
        virtual ~subclass() { println("subclass::~subclass()") };
};
执行
superclass  * super = new subclass();
delete super;
的结果是打印出
subclass::~subclass()
superclass::~superclass()
这意味着当执行delete super;时：
●是否是chaining式call呢？是。因为是析构函数。
●那chaining call从哪里开始呢？从subclass::~subclass() ==〉superclass::~superclass()，
因为superclass * super的实际对象的类型是subclass。 
转自:http://www.vckbase.com/document/viewdoc/?id=933
