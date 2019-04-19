# C++多线程编程经验总结 - fanyun的博客 - CSDN博客
2017年07月26日 13:04:29[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：424
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
1.    软件片段的架构是一套控制软件操作的规则、模式、进程、执行协议和断言。
多线程架构：一种将工作模式分解为两个或更多并发执行现场的软件架构。
分解软件：分割为单独逻辑任务的过程，供软件的工作模式来执行。由于软件的数据组织和执行流程依赖于对应的并发或并行化模型的可用性。
2.算法基本执行单元：一条语句(或子语句)
    进程执行基本单元：线程(thread)
    最大执行单元：对话(session)
    多线程：一个进程内有两个或更多线程并发执行
    并行算法：一个线程内包含一条或多条并发执行的指令，这些指令共同形成一个算法，多线程架构被分解成线程，并行算法被分解成单挑指令。当讨论并行算法时，所讨论单元指单条指令或子指令；当讨论多线程时，所讨论单元指包含一套指令的逻辑任务，以过程或函数的形式出现。并行算法面对并发的微观方面，多线程面对并发的宏观方面。
3.多线程常见架构三种：
 (1)客户机/服务器:DDE动态数据交换。单个DDE代理可能既是客户机也是服务器，消息系统完成。
   网络环境中常见客户机/服务器模型应用：文件服务器、数据库服务器、事务服务器、应用服务器、逻辑服务器
 (2)事件驱动编程(GUI)
 (3)黑板(blackboard)
4.类层次：一些通过继承相关联的类的集合。表示一个人物、地点、事件或思想的逻辑家族，也可以包含其他类作为数据成员。
常见类类型：抽象基类、具体类、节点类、接口类、容器和集合类、应用框架类、域类。
5.抽象基类：为后代提供蓝图，不能声明实例，必须首先从基类派生一个新类，并为抽象类中声明的所有纯虚函数提供实际的定义。抽象类必须至少有一个有用的后代。（与具体类形成鲜明对比，具体类不支持派生后代），对于抽象类，必须至少有一个纯虚函数。virtual return type function()=0;
虽然纯抽象虚函数可用于强制实施接口策略，但它并不是百分之百安全，不能阻止派生类的设计者定义成员函数完成该成员函数之外的其他事情，可以讲他们实现为哑元函数(即什么也不做)，即纯抽象虚函数不能强迫派生类实现执行特定任务的函数。
6.具体类：思想终结者(用于包含安全类和多线程类的类层次的终点)
   具体类可以构建于其他类，但具体类作为进一步继承的终点。基类具有一般性，但具体类具有特殊性，不会继续特殊化。多线程类层次中为用户提供一个具体类，就不必担心用户在具体类中对成员函数意义的改变。具体类没有虚函数，不打算用来继承，而是直接使用。
7.节点类：最强大类类型，提供继承和多态基础。设计用于被继承,但不包含纯虚函数。非终点类，可以立即使用但同时也着眼于未来，即设计具有可重用性，提供在派生类中覆盖的虚成员函数。节点类即可以是一个基类，也可以是一个派生类。节点类使用了基类的指针、引用、虚函数和运行时多态。当程序员修改这个层次时，应完整理解运行时多态、深层复制构造函数、异常处理和签名分辨率。在多线程环境中，节点类会增添混乱，使用务必小心。
8.容器和集合类：常规C++类的一个特别应用。
   集合和容器是一般性目的的分组结构。常用容器类：数组和矢量类Array/Vector、列表类List、双端队列类Deque、队列和优先权队列类Queue/Priority Queue
 用于操纵异类或同类对象。用于管理一组对象，其方式与传统数组管理传统数据类型(如整型或字符)的方式一样。使用C++类和结构(struct)这一结构来定义集合和容器。C++标准库包括STL标准模板库，它是用作具体类的容器类的一个库。
9.在多线程程序中，集合和容器类与线程交互的主要途径：线程间通信ITC、多线程服务器
进程间通信方式：使用管道、命名管道、动态数据交换、命令行参数、共享内存或文件。这些结构作为多个执行进程间的外部链接，让这些进程相互通信与传递数据或命令，通过调用操作系统API来管理(如创建、同步化、阻塞、取消阻塞、缓冲、关闭和销毁结构关联内存开销)。
线程间通信方式：通过一个全局变量、全局数组、全局结构，或通过传统参数传递来通信。在面向对象程序中，两个线程可以通过全局容器和集合类来通信，形式如下：
面向对象列表、面向对象矢量、面向对象集、面向对象多集、面向对象图标、面向对象双端队列、面向对象队列、面向对象映射、面向对象多重映射。
【注意】面向对象组件进行线程间通信具有巨大优点。
10.实现搜索引擎的一种最有效技术：将搜索分成一系列并发执行的线程，每个线程可以访问所需要的搜索组件。线程可以将集合和容器对象用作线程间通信的渠道。
多线程搜索引擎组件：集合mt_set和容器对象lqueue组件、线程对象ct_thread。
将多线程搜索引擎分成若干不同的任务，一个任务可以读取互联网上所有文件的索引，另一个任务可以为预备匹配预搜索这些文件的索引，以及其他多个任务进行详细的预备性匹配。
指派线程A来读取主索引，当线程A读取主索引时，可以指派线程B开始从预备匹配的主索引过滤每个索引。保存预备匹配的文件名时，指派线程C来检查我们所需要的项，指派线程D来检查文件中我们不想要的项；一旦全面启动，线程将并发执行，这些线程可以共享集合(set)和容器对象(queue)进行线程间通信。
定义4个集set:(1)当前被分析文件的单词集合 (2)我们感兴趣主题的单词集合 (3)我们不感兴趣的单词集合 (4)我们感兴趣的单词集合的相关项。
当线程A从主索引中读取文件名时，它将这些文件名及其位置放进面向对象队列中。线程B删除并处理这些文件名来访问他们的单个索引。线程B查找预备匹配时，它将匹配放入另一个面向对象队列中，同时线程B还将当前位于队列前端的文件放进集A。一旦集A中存在一个文件，线程D和线程C使用基本集运算(交集、并集、差集)来进行相应的文件分析。
 (1)线程安全集合(mt_set)——更容易在多线程环境中使用：
template <class T>class_mt_set:virtual private mutex{
  private:
       set<T,less<T>>S;
  public:
       mt_set(void);
       mt_set(set<T,less<T>>X);
       set<T,less<T>> setUnion(set<T,less<T>>X);
       set<T,less<T>> intersection(set<T,less<T>>X);
       int membership(set<T,less<T>>X);
       set<T,less<T>> difference( set<T,less<T>>X);
 };
(2)线程安全容器队列(lqueue)：
template <class T> classlqueue:virtual private named_mutex,virtual private event_mutex{
  protected:
     deque SafeQueue;
  public:
     lqueue(char *MName,int Own,char *EName,int Initial,unsigned long Dur);
     inline void insert(T X);
     inline T remove(void);
     inline T front(void);
     inline T back(void);
     inline unsigned int empty(void);
     inline void erase(void);
     void wait(void);
     void broadCast(void);
};
[注意]在多线程环境中，使用外部迭代器具有固定的难度，因此去掉了mt_set类和lqueue类的迭代器功能。
两个类都是修改STL容器类的接口类，都私有继承了用于保护类内部临界区的互斥量。
(3)线程对象(ct_thread)（POSIX Pthread API）——隐藏了对应操作系统环境的实现细节，用于设计在不同操作系统环境中可移植的程序。
class ct_thread{
 private:
    pthread_t ThreadId;
    pthread_attr_t *Attr;
 public:
    ct_thread(void);
    ct_thread(pthread_attr_t *Attribute);
    ~ct_thread(void);
    void begin(FunctionPtr PFN,void *X);
    void wait(void);
    pthread_t threadHandle(void);
    pthread_t threadId(void);
};
11.作为多线程服务器的容器类：
当容器或集合类用作线程间通信的工具，线程位于类外部；当容器或集合类用作一个服务器时，类本身将包含多个线程。多线程集服务器[set_server类]：用于将类分解成多个线程的简单技术。
 set_server类用于提供多线程服务，多线程服务由mt_set组件来实现。即set_server类的用户请求一个服务，set_server类然后创建一个线程来执行这个服务，该线程将请求传递给mt_set类，mt_set类完成这个请求，并返回结果。
12.多线程类简单架构基石：6个基本组件
宿主类、线程类、互斥和事件类、友元成员函数、域类、强制转换基本元素
13.宿主类：用户与之交互的类(多线程应用程序框架、多线程服务器)
  宿主类被分解成两个或更多的线程，每个线程执行宿主类一个友元函数，当执行友元函数时，宿主类将this指针作为一个参数传递给该线程。this指针在友元函数中被强制转换成指向适当类型的类。this指针形成了宿主类与位于另一个线程中的域类间的线程间通信。每个友元函数将创建一个或多个域对象(domain object)来完成线程请求。
template<class T>  struct server_argument{  //用于保存多个线程间以及主机与客户间的输入或输出参数
 set<T,less<T>> A; //需要在友元函数中构建的域类信息
 set<T,less<T>> B; //参数、消息(来自客户的请求)
 set<T,less<T>> Result; //包含对象集B中信息操作或处理结果
}
template<class T> classset_server:virtual private mutex{ //服务器类中每个成员函数表示一个客户的请求，每个成员函数创建一个执行某友元函数的线程。
 protected:
    server_argument<T> Argument;
 public:
    set_server(void);
    //用于创建客户请求线程的友元函数指针
    friend void *intersection(void *X);
    friend void *setUnion(void *X);
    friend void *difference(void *X);
    friend void *membership(void *X);
    set<T,less<T>> intersect(set<T,less<T>>X,set<T,less<T>> Y);
    set<T,less<T>> setUnion(set<T,less<T>>X,set<T,less<T>> Y);
    set<T,less<T>> difference(set<T,less<T>>X,set<T,less<T>> Y);
    int membership(set<T,less<T>>X,set<T,less<T>>Y);};
//并集线程实现代码：
template<class T>set<T,less<T>>
set_server<T>::setUnion(set<T,less<T>>X,set<T,less<T>>Y)
{ lock();
 ct_thread Thread;
 Argument.A=X;
 Argument.B=Y;
 Thread.begin(::setUnion,this);  //友元函数创建域对象与服务器对象进行通信,使用setUnion友元函数指针(即线程入口点)创建线程并执行setUnion()友元函数。
 Thread.wait();
 unlock();
 return(Argument.Result);
}
setUnion()友元函数声明如下：
void *setUnion(void *X)
{ set_server<String> *Server;
 Server=static_cast<set_server<String> *>(X);
 mt_set<String> SafeSet(Server->Argument.A);
 Server->Argument.Result=SafeSet.setUnion(Server->Argument.B); //保存set_server对象的Argument数据成员中并集的结果
}
14.void* 充当线程所创建函数的一种命令行参数。
void*参数几乎可以给线程的主函数传递任何内容。该参数可以强制转换成任何一种类型。对于多线程对象，void*将被转换成this指针指向的任何类型。
set_server类是一个线程，setUnion()成员函数位于另一个线程，而this指针是两个线程间的通信点。通过类型转换static_cast建立了两个线程间的通信链接后，setUnion()友元函数创建域对象mt_set类。
15.多线程服务器简单程序: 使用多线程集服务器来执行集A和集B之间的并集，当执行两个集之间的并集时，两个集中的成员被联合，并变成第三个集的成员。
#include "setserver.cpp"
#include<set.h>
#include<iostream.h>
set<String,less<String>> A;
set<String,less<String>> B;
set<String,less<String>> C;
set<String,less<String>>::iteratorI;
void main(void)
{ cout<<"Start mainthread"<<endl;
 A.insert("hello");
 B.insert("I am alive");
 set_server<String> Server;
 C=Server.setUnion(A,B);
 I=C.begin();
 cout<<"from main thread"<<endl;
 while(I!=C.end())
  {cout<<*I<<endl;
   I++;
  }
}
16.应用框架类：属于最容易理解但最难以构建的类层次，为思想建模(即为对象交互、工作模式、动作序列、软件事务和进程内容来建模)。当设计面向对象应用框架时，对象是相关联动作序列。应用框架用作整个应用的模式，它具体化应用具有的基础结构或骨架而没有提供应用的细节。
程序是特定问题的一般性解决方案，应用框架是某问题类别的特定解决方案。
 设计类库(提供面向对象访问磁盘I/O、数学函数、字符串操纵、内存管理、图像处理等等）与设计应用框架相比，前者基于过程库更容易实现。
过程库进行构建无法适应于改变的环境、软件的大型化，而类库改进了这种状况。通过添加面向对象特征，可以在过程库的基础上改进得到类库。
应用框架设计需要对软件分解有一个全面的理解。分解为提供者实现和客户实现部分。在多线程应用框架中，框架执行的工作模式必须分解为一套线程一般性部分和特定应用部分。
【注意】C++通过抽象基类、虚基类、虚函数、类复合、类聚集、友元函数、模板和新类型转换运算符来支持软件分解。
17.应用框架类层次：
 应用框架是框架提供者与框架用户间的合作软件契约(如交通工具框架提供者组件：引擎、底盘长度、座位数量和点火系统；而交通工具用户组件：引擎、底盘长度、座位数量、点火系统、交通车体)。框架设计者提供框架的部分功能(理想情况下提供大部分功能)，同时，该框架的用户完善框架的功能。
框架提供者应标识和提供应用必须执行的基本工作模式，框架用户要提供所有抽象虚函数和构造函数参数的定义。
18.框架分解：2个组件(框架设计者提供的一般性方式捕获的特定动作序列以及对象间关系的框架部分、框架用户提供的ensemble域知识专家知识规则和特定解决方案的策略)，简单来说即框架应用形式和应用具体内容，框架则提供应用的主要控制流程。
框架指定动作序列由三种成员函数来实现：a.虚成员函数(用户可覆盖替换，选择权取决于用户) b.纯虚成员函数(用户提供自己的定义，必须提供的实际函数否则程序不能编译，纯虚函数都必须在派生类中定义)c.常规成员函数(实现普通动作序列，派生类不需要更改或提供，在框架中提供的常规函数越多越好)
19.提供ensemble:
  应用框架内容可通过继承、复合/结合来提供。一般来说，程序员在使用框架之前，必须继承框架类、为所有纯虚函数提供定义，覆盖必须的虚函数 来提供框架的内容。
  当通过构造函数、对象的指针和引用提供ensemble时，即是通过复合来构建框架内容。
20.集合和容器类 与 应用框架类的区别：
  集合和容器类：为各种各样方式使用的一般目的的结构。
  应用框架：只提供广泛问题类别的一种解决方案。(如：查询处理器框架：获取字符串、验证字符串、解析字符串和判断字符串动作序列)，框架不能做其他任何事。
