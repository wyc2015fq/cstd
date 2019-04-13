
# 反应器组件 ACE_Reactor - 高科的专栏 - CSDN博客

2015年04月22日 15:27:31[高科](https://me.csdn.net/pbymw8iwm)阅读数：559标签：[ace																](https://so.csdn.net/so/search/s.do?q=ace&t=blog)[reactor																](https://so.csdn.net/so/search/s.do?q=reactor&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=ace&t=blog)个人分类：[ace																](https://blog.csdn.net/pbymw8iwm/article/category/3133547)[C/C++																](https://blog.csdn.net/pbymw8iwm/article/category/910215)[
							](https://blog.csdn.net/pbymw8iwm/article/category/3133547)


6.1 反应器组件 ACE_Reactor反应器的基本原理是:
针对关心的某个事件写一个事件处理器(event_handler). 将该事件处理器登记到反应器中(同时指明关心的事件).
然后反应器会自动检测事件的发生. 并调用预先登记的事件处理器中的回调函数.
**所以****ACE Reactor 框架的责任：**
1、检测来自各种事件源的事件的发生。
2、将事件多路分离给其预先登记的事件处理器。
3、分派给处理器所定义的挂钩方法，从而以一种应用定义的方式处理这些事件。

6.2 事件处理器
在ACE中.反应器是ACE_Reactor类的单件对象(因为程序中通常只需要一个反应器).
反应器提供了登记/撤销 事件处理器的接口.register_handler() /remove_handler() .
这些接口要求 事件处理器必须是ACE_Event_Handler类型的. 所以我们的事件处理器类必须从该类继承.
在ACE_Event_Handler类中定义了一些类似"handle_***" 的回调方法. 我们必须在派生类中重写我们敢兴趣的.
在反应器需要检测某个I/O句柄上是否有事件时. 需要知道原始句柄. 这样就需要重写事件处理器类的get_handle()函数.
下边是ACE_Event_Handler 中声明的钩子函数:
handle_signal()  当在反应器上登记的信号发生时. 反应器回调该函数. (不懂)
handle_input()  当来自I/O设备的输入可用时. 反应器自动回调该方法.
handle_exception()  当在反应器上登记的异常事件发生时. (不懂)
handle_timeout() 当在反应器上登记的定时器超时的时候. 回调该方法.
handle_output()  当在IO设备上的输出可用时. 回调该方法.
6.2.1 登记事件处理器
使用 ACE_Reactor 类的register_handler()函数.这个函数有好几个重载形式.
该函数有个参数用来指出感兴趣的事件. 它可以是下边一些常量(定义在ACE_Event_handler类中):
READ_MASK句柄上有数据可读时回调
 handle_input()
WRITE_MASK句柄上可写时回调
 handle_output()
TIMER_MASK回调 handle_close()  不懂怎么用...
ACCEPT_MASK有来自客户端的新的连接请求时回调
 handle_input()
CONNECT_MASK建立连接时回调
 handle_input()
DONT_CALL它用在显式拆除事件处理器的remove_handler()函数中.
 表示拆除前不调用 handler_close() 函数.
6.2.2
 拆除事件处理器
当不在需要处理某个事件时. 需要把对应的事件处理器从反应器中拆除.
有两种拆除事件处理器的办法:
一种是隐式的自动拆除.当事件处理器类中的
 handle_*** 方法返回的int 小于0 时. 反应器会自动调用事件处理器
的Handle_close()方法. 并把事件处理器拆除.
另一种是显式拆除.
 即调用 ACE_reactor::remove_handler(). 这也会调用事件处理器的handle_close(). 然后拆除.
不过. 如果你不需要调用handle_close(). 可以给remove_handler()传递参数 ACE_Event_Handler::DONT_CALL .
具体例子在后边会给出.

我在这里创建了一个简单的Event_handler：
class Event_Handler
{
public:
Event_Handler();
virtual ~Event_Handler();
public:
//! 获取反应器
//! @return 反应器指针
Reactor* reactor();
//! 设置反应器
//! @param reactor 反应器指针
void reactor(Reactor* reactor);
//! 获取通道id
//! @return 通道id
virtual uint32_t get_id() = 0;
//! 获取socket句柄
//! @return socket句柄
virtual SOCKET get_handle() = 0;
//! 处理读
//! @return 处理结果 0:处理正常, -1: 连接被关闭, -2:连接异常
virtual int handle_input() = 0;
//! 处理写
//! @return 处理结果 0:处理正常, -1: 连接被关闭, -2:连接异常
virtual int handle_output() = 0;
//! 连接异常
virtual int handle_exception() = 0;
//! 连接关闭
virtual int handle_close() = 0;
//! 超时
virtual int handle_timeout() = 0;
//! 提交发送任务
//! @param send_task 待发送的任务
virtual int post_packet(Net_Packet *send_packet) = 0;
public:
list_head hashitem;
boolread;
boolwrite;
boolnotify_close;
time_ttimeout;//<! 超时的时间, 0表示没有设置超时
private:
//! 反应器
Reactor *m_reactor;
};
然后创建对应的处理类：
//! @class SOCK_Acceptor
//! @brief tcp监听处理类
class SOCK_Acceptor : public Event_Handler{
...
};
//! @class SOCK_Connector
//! @brief tcp连接处理类
class SOCK_Connector : public Event_Handler{
...
};
//! @class SOCK_Stream
//! @brief tcp通道处理类
class SOCK_Stream : public Event_Handl{
...
};



