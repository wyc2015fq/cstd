# ACE接受器－连接器模式 - 深之JohnChen的专栏 - CSDN博客

2011年12月04日 22:00:49[byxdaz](https://me.csdn.net/byxdaz)阅读数：1313标签：[service																[class																[stream																[concurrency																[网络																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=concurrency&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)
个人分类：[ACE网络编程](https://blog.csdn.net/byxdaz/article/category/628882)


接受器－连接器设计模式（Acceptor-Connector）使分布式系统中的连接建立及服务初始化与一旦服务初始化后所执行的处理去耦合。 

这样的去耦合通过三种组件来完成：**acceptor**、**connector** 和 **servicehandler**（服务处理器）。 
- 连接器主动地建立到远地接受器组件的连接，并初始化服务处理器来处理在连接上交换的数据。 
- 接受器被动地等待来自远地连接器的连接请求，在这样的请求到达时建立连接，并初始化服务处理器来处理在连接上交换的数据。 
- 初始化的服务处理器执行应用特有的处理，并通过连接器和接受器组件建立的连接来进行通信。 

**结构 **

**1. 服务处理器（Service Handler）：**Service Handler 实现应用服务，通常扮演客户角色、服务器角色，或同时扮演这两种角色。它提供挂钩方法，由 Acceptor 或 Connector 调用，以在连接建立时启用应用服务。此外，Service Handler 还提供数据模式传输端点，其中封装了一个 I/O 句柄。一旦连接和初始化后，该端点被 Service Handler 用于与和其相连的对端交换数据。 

**2. 接受器（Acceptor）：**Acceptor 是一个工厂，实现用于被动地建立连接并初始化与其相关联的 Service Handler 的策略。此外，Acceptor 包含有被动模式的传输端点工厂，它创建新的数据模式端点，由 Service Handler 用于在相连的对端间传输数据。通过将传输端点工厂绑定到网络地址，比如 Acceptor 在其上侦听的 TCP 端口号，Acceptor的 open 方法对该工厂进行初始化。 
一旦初始化后，被动模式的传输端点工厂侦听来自对端的连接请求。当连接请求到达时，Acceptor 创建 Service Handler，并使用它的传输端点工厂来将新连接接受进Service Handler 中。 

**3. 连接器（Connector）：**Connector 是一个工厂，实现用于主动地建立连接并初始化与其相关联的 Service Handler 的策略。它提供方法，由其发起到远地 Acceptor 的连接。同样地，它还提供另一个方法，完成对 Service Handler 的启用；该处理器的连接是被同步或异步地发起的。Connector 使用两个分开的方法来透 明地支持异步连接建立。 

**4. 分派器（Dispatcher）：**为 Acceptor，Dispatcher 将在一或多个传输端点上接收到的连接请求多路分离给适当的 Acceptor。Dispatcher允许多个 Acceptor 向其登记，以侦听同时在不同端口上从不同对端而来的连接。 为 Connector，Dispatcher 处理异步发起的连接的完成。在这种情况下，当异步连接被建立时，Dispatcher 回调 Connector。Dispatcher 允许多个 Service Handler 通过一个 Connector 来异步地发起和完成它们 的连接。注意对于同步连接建立，Dispatcher 并不是必需的，因为发起连接的线程控制也完成服务服务处 理器的启用。
Dispatcher 通常使用事件多路分离模式来实现，这些模式由反应器（Reactor）或前摄器（Proactor） 来提供，它们分别处理同步和异步的多路分离。同样地，Dispatcher 也可以使用主动对象（Active Obj ect）模式[5]来实现为单独的线程或进程。 

**协作 **

**Acceptor 组件协作 **

Acceptor 和 Service Handler 之间的协作。这些协作被划分为三个阶段： 

1. 端点初始化阶段： 

为被动地初始化连接，应用调用 Acceptor 的 open 方法。该方法创建被动模式的传 输端点，将其绑定到网络地址，例如，本地主机的 IP 地址和 TCP 端口号，并随后侦听来自对端 Connector 的连接请求。其次，open 方法将 Acceptor 对象登记到 Dispatcher，以使分派器能够在连接事件 到达时回调 Acceptor。最后，应用发起 Dispatcher 的事件循环，等待连接请求从对端 Connector 到来。 

2. 服务初始化阶段： 

当连接请求到达时，Dispatcher 回调 Acceptor 的accept 方法。该方法装配以下活动 所必需的资源： 
- 创建新的 Service Handler， 
- 使用它的被动模式传输端点工厂来将连接接受进 该处理器的数据模式传输端点中， 
- 通过调用 Service Handler 的 open 挂钩将其启用。Servic e Handler 的 open 挂钩可以执行服务特有的初始化，比如分配锁、派生线程、打开日志文件，和/或将 该 Service Handler 登记到 Dispatcher。 

3. 服务处理阶段： 

在连接被动地建立和 Service Handler 被初始化后，服务处理阶段开始了。在此阶段， 应用级通信协议，比如 HTTP 或 IIOP，被用于在本地 Service Handler 和与其相连的远地 Peer 之间、 经由前者的 peer_stream_端点交换数据。当交换完成，可关闭连接和 Service Handler，并释放资源。 

**Connector 组件协作 **

Connector 组件可以使用同步和异步两种方式来初始化它的 Service Handle，这里仅介绍一下同步时的协作情况。 

同步的 Connector 情况中的参与者之间的协作可被划分为以下三个阶段： 
- 连接发起阶段：
为在 Service Handler 和它的远地 Peer 之间发起连接，应用调用 Connector 的 connect 方法。该方法阻塞调用线程的线程控制、直到连接同步完成，以主动地建立连接。 
- 服务初始化阶段：
在连接完成后，Connector 的 connect 方法调用 complete 方法来启用 Service Handl er。complete 方法通过调用 Service_Handler 的 open 挂钩方法来完成启用；open 方法执行服务特有的 初始化。 
- 服务处理阶段：
此阶段与 Service Handler 被 Acceptor 创建后所执行的服务处理阶段相类似。特别地， 一旦 Service Handler 被启用，它使用与和其相连接的远地 Service Handler 交换的数据来执行应用特 有的服务处理。 

**实现 **

运行一般步骤： 
- 创建 Service Handler； 
- 被动地或主动地将 Service Handler 连接到它们的远地对端；以及 
- 一旦连接，启用 Service Handler。 

主要角色：Service Handler（服务处理器）、Acceptor 和 Connector。 

**服务处理器：**该抽象类继承自 Event_Handler，并为客户、服务器或同时扮演两种角色的组件所提供 的服务处理提供通用接口。应用必须通过继承来定制此类，以执行特定类型的服务。Service Handler 接口如下所示： 

template <class PEER_STREAM>
class Service_Handler : public Event_Handler
{
public:
//连接成功后的初始化入口函数 (子类定义).
virtualint open (void) = 0;
//返回通信流的引用
    PEER_STREAM &peer (void)
    {
return peer_stream_;
    }
};

一旦 Acceptor 或 Connector 建立了连接，它们调用 Service Handler 的 open 挂钩。该纯虚方法必须被 Concrete Service Handler 子类定义；后者执行服务特有的初始化和后续处理。 

**连接器：**该抽象类实现主动连接建立和初始化 Service Handler 的通用策略。它的接口如下所示： 

template <class SERVICE_HANDLER,class PEER_CONNECTOR>
class Connector : public Event_Handler
{
public:
enum Connect_Mode
    {
        SYNC, //以同步方式连接
        ASYNC //以异步方式连接
    };
// 主动连接并激活服务处理器
int connect (SERVICE_HANDLER *sh,
const PEER_CONNECTOR::PEER_ADDR &addr,
        Connect_Mode mode);
protected:
//定义连接激活策略
virtualint connect_service_handler(SERVICE_HANDLER *sh,
const PEER_CONNECTOR::PEER_ADDR &addr,
        Connect_Mode mode);
// Defines the handler's concurrency strategy.
virtualint activate_service_handler(SERVICE_HANDLER *sh);
// 当以异步方式连接完成时激活服务处理器
virtualint complete (HANDLE handle);
private:
// IPC mechanism that establishes
// connections actively.
    PEER_CONNECTOR connector_;
    };

Conncetor 通过特定类型的 PEER CONNECTOR 和 SERVICE HANDLER 被参数化。PEER CONNECTO R 提供的传输机制被 Connector 用于主动地建立连接，或是同步地、或是异步地。SERVICE HANDLER提供的服务对与相连的对端交换的数据进行处理。C++参数化类型被用于使（1）连接建立策略与（2）服务处理器类型、网络编程接口和传输层连接协议去耦合。 

参数化类型是有助于提高可移植性的实现决策。例如，它们允许整体地替换 Connector 所用的 IPC 机 制。这使得 Connector 的连接建立代码可在含有不同网络编程接口（例如，有 socket，但没有 TLI；反之 亦然）的平台间进行移植。 

Service Handler 的 open 挂钩在连接成功建立时被调用。 

**接受器（Acceptor）：**该抽象类为被动连接建立和初始化 Service Handler 实现通用的策略。Acceptor 的接 口如下所示： 

template <class SERVICE_HANDLER,
class PEER_ACCEPTOR>
class Acceptor : public Event_Handler
{
public:
// Initialize local_addr transport endpoint factory
// and register with Initiation_Dispatcher Singleton.
virtualint open(const PEER_ACCEPTOR::PEER_ADDR &local_addr);
// Factory Method that creates, connects, and
// activates SERVICE_HANDLER's.
virtualint accept (void);
protected:
//定义服务处理器的创建策略
virtual SERVICE_HANDLER *make_service_handler (void);
// 定义服务处理器的连接策略
virtualint accept_service_handler(SERVICE_HANDLER *);
//定义服务处理器的激活策略
virtualint activate_service_handler(SERVICE_HANDLER *);
// Demultiplexing hooks inherited from Event_Handler,
// which is used by Initiation_Dispatcher for
// callbacks.
virtual HANDLE get_handle (void) const;
virtualint handle_close (void);
private:
// IPC mechanism that establishes
// connections passively.
    PEER_ACCEPTOR peer_acceptor_;
}; 

Acceptor 通过特定类型的 PEER ACCEPTOR 和 SERVICE HANDLER 被参数化。PEER ACCEPTOR 提供的传输机制被 Acceptor 用于被动地建立连接。SERVICE HANDLER 提供的服务对与远地对端交换的 数据进行处理。注意 SERVICE HANDLER 是由应用层提供的具体的服务处理器。 

参数化类型使 Acceptor 的连接建立策略与服务处理器的类型、网络编程接口及传输层连接发起协议去耦合。就如同 Connector 一样，通过允许整体地替换 Acceptor 所用的机制，参数化类型的使用有助于提高可移植性。这使得连接建立代码可在含有不同网络编程接口（比如有 socket，但没有 TLI；反之亦然）的平台间移植。 

make_service_handler 工厂方法定义 Acceptor 用于创建 SERVICE HANDLER 的缺省策略。如下所示： 

template <class SH, class PA> SH *
Acceptor<SH, PA>::make_service_handler (void)
{
returnnew SH;
} 

缺省行为使用了"请求策略"（demand strategy），它为每个新连接创建新的 SERVICE HANDLER。但是， Acceptor 的子类可以重定义这一策略，以使用其他策略创建 SERVICE HANDLE，比如创建单独的单体 （Singleton）[10]或从共享库中动态链接 SERVICE HANDLER。 

accept_service_handler 方法在下面定义 Acceptor 所用的 SERVICE HANDLER 连接接受策略： 

template <class SH, class PA> int
Acceptor<SH, PA>::accept_service_handler(SH *handler)
{
    peer_acceptor_->accept (handler->peer ());
} 

缺省行为委托 PEER ACCEPTOR 所提供的 accept 方法。子类可以重定义 accept_service_handler 方法，以 执行更为复杂的行为，比如验证客户的身份，以决定是接受还是拒绝连接。 

Activate_service_handler 定义 Acceptor 的 SERVICE HANDLER 并发策略： 

程序示例： 

在ACE中，默认的服务处理器是ACE_Svc_Handler，这也是一个模版类，可以通过相关的参数特化。由于ACE_Svc_Handler继承自ACE_Task和ACE_Event_Handler，功能相当强大，同时也存在一定开销，如果需要减小开销可以自己写一个仅继承自ACE_Event_Handler的服务处理器。 

为了演示简单，我这里就以一个EchoServer的服务器端和客户端为例，其中接收器和连接器都采用缺省策略，并没有进行重载。 

服务器端： 

#include"ace/Reactor.h"
#include"ace/Svc_Handler.h"
#include"ace/Acceptor.h"
#include"ace/Synch.h"
#include"ace/SOCK_Acceptor.h"

class My_Svc_Handler; 
typedef ACE_Acceptor<My_Svc_Handler,ACE_SOCK_ACCEPTOR> MyAcceptor; 

class My_Svc_Handler: 
public ACE_Svc_Handler <ACE_SOCK_STREAM,ACE_NULL_SYNCH> 
{ 
public: 
int open(void*) 
    { 
        ACE_OS::printf("\nConnection established\n");

//注册相应事件
        ACE_Reactor::instance()->register_handler(this, 
            ACE_Event_Handler::READ_MASK); 
return 0; 
    }

int handle_input(ACE_HANDLE) 
    { 
int rev = peer().recv(data,1024); 
if(rev == 0)
        {
deletethis;
        }
else
        {
            data[rev]='\0';
            ACE_OS::printf("<<rev:\t %s\n",data); 
            peer().send(data,rev+1);
return 0; 
        }
    }
private: 
char data[1024]; 
}; 
int main(int argc, char* argv[]) 
{ 
    ACE_INET_Addr addr(3000); 
    MyAcceptor acceptor(addr,ACE_Reactor::instance()); 

while(1) 
        ACE_Reactor::instance()->handle_events(); 
} 

客户端： 

#include"ace/Reactor.h"
#include"ace/Svc_Handler.h"
#include"ace/Connector.h"
#include"ace/Synch.h"
#include"ace/SOCK_Connector.h"

class My_Svc_Handler; 
typedef ACE_Connector<My_Svc_Handler,ACE_SOCK_CONNECTOR> MyConnector; 

class My_Svc_Handler: 
public ACE_Svc_Handler <ACE_SOCK_STREAM,ACE_NULL_SYNCH> 
{ 
public: 
int open(void*) 
    { 
        ACE_OS::printf("\nConnection established\n");

//注册相应事件
        ACE_Reactor::instance()->register_handler(this, 
            ACE_Event_Handler::READ_MASK); 
return 0; 
    }

int handle_input(ACE_HANDLE) 
    { 
int rev = peer().recv(data,1024); 
if(rev == 0)
        {
deletethis;
        }
else
        {
            data[rev]='\0';
            ACE_OS::printf("<<rev:\t %s\n",data); 
return 0; 
        }
    }

int sendData(char *msg)
    {
        ACE_OS::printf("<<send:\t %s\n",msg);
return peer().send(msg,strlen(msg));    
    }

private: 
char data[1024]; 
}; 
int main(int argc, char* argv[]) 
{ 
    ACE_INET_Addr addr(3000,"192.168.1.142"); 

    My_Svc_Handler *svchandler = new My_Svc_Handler();
    MyConnector connector;

if(connector.connect(svchandler,addr)==-1)
    {
        ACE_OS::printf("Connect fail");
    }

    svchandler->sendData("hello wrold");

while(1) 
        ACE_Reactor::instance()->handle_events(); 
}


