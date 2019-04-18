# ACE反应器(Reactor)模式 - 深之JohnChen的专栏 - CSDN博客

2011年12月04日 21:49:53[byxdaz](https://me.csdn.net/byxdaz)阅读数：1713


[ACE反应器(Reactor)模式(1)](http://www.cnblogs.com/TianFang/archive/2006/12/13/591332.html)

[http://www.cnblogs.com/TianFang/archive/2006/12/13/591332.html](http://www.cnblogs.com/TianFang/archive/2006/12/13/591332.html)

[ACE反应器(Reactor)模式(2)](http://www.cnblogs.com/TianFang/archive/2006/12/18/595808.html)

[http://www.cnblogs.com/TianFang/archive/2006/12/18/595808.html](http://www.cnblogs.com/TianFang/archive/2006/12/18/595808.html)

[ACE反应器(Reactor)模式(3)](http://www.cnblogs.com/TianFang/archive/2006/12/18/595938.html)

[http://www.cnblogs.com/TianFang/archive/2006/12/18/595938.html](http://www.cnblogs.com/TianFang/archive/2006/12/18/595938.html)

[ACE反应器(Reactor)模式(4)](http://www.cnblogs.com/TianFang/archive/2006/12/18/596012.html)

[http://www.cnblogs.com/TianFang/archive/2006/12/18/596012.html](http://www.cnblogs.com/TianFang/archive/2006/12/18/596012.html)

**1．ACE反应器框架简介**

反应器（Reactor）：用于事件多路分离和分派的体系结构模式 

通常的，对一个文件描述符指定的文件或设备, 有两种工作方式: 阻塞与非阻塞。所谓阻塞方式的意思是指, 当试图对该文件描述符进行读写时, 如果当时没有东西可读,或者暂时不可写, 程序就进入等待状态, 直到有东西可读或者可写为止。而对于非阻塞状态, 如果没有东西可读, 或者不可写, 读写函数马上返回, 而不会等待。 

在前面的章节中提到的Tcp通信的例子中，就是采用的阻塞式的工作方式：当接收tcp数据时，如果远端没有数据可以读，则会一直阻塞到读到需要的数据为止。这种方式的传输和传统的被动方法的调用类似，非常直观，并且简单有效，但是同样也存在一个效率问题，如果你是开发一个面对着数千个连接的服务器程序，对每一个客户端都采用阻塞的方式通信，如果存在某个非常耗时的读写操作时，其它的客户端通信将无法响应，效率非常低下。 

一种常用做法是：每建立一个Socket连接时，同时创建一个新线程对该Socket进行单独通信（采用阻塞的方式通信）。这种方式具有很高的响应速度，并且控制起来也很简单，在连接数较少的时候非常有效，但是如果对每一个连接都产生一个线程的无疑是对系统资源的一种浪费，如果连接数较多将会出现资源不足的情况。 

另一种较高效的做法是：服务器端保存一个Socket连接列表，然后对这个列表进行轮询，如果发现某个Socket端口上有数据可读时（读就绪），则调用该socket连接的相应读操作；如果发现某个Socket端口上有数据可写时（写就绪），则调用该socket连接的相应写操作；如果某个端口的Socket连接已经中断，则调用相应的析构方法关闭该端口。这样能充分利用服务器资源，效率得到了很大提高。 

在Socket编程中就可以通过select等相关API实现这一方式。但直接用这些API控制起来比较麻烦，并且也难以控制和移植，在ACE中可以通过Reactor模式简化这一开发过程。 

反应器本质上提供一组更高级的编程抽象，简化了事件驱动的分布式应用的设计和实现。除此而外，反应器还将若干不同种类的事件的多路分离集成到易于使用的API中。特别地，反应器对基于定时器的事件、信号事件、基于I/O端口监控的事件和用户定义的通知进行统一地处理。 

ACE中的反应器与若干内部和外部组件协同工作。其基本概念是反应器框架检测事件的发生（通过在OS事件多路分离接口上进行侦听），并发出对预登记事件处理器（event handler）对象中的方法的"回调"（callback）。该方法由应用开发者实现，其中含有应用处理此事件的特定代码。 

使用ACE的反应器，只需如下几步： 
- 
创建事件处理器，以处理他所感兴趣的某事件。 

- 
在反应器上登记，通知说他有兴趣处理某事件，同时传递他想要用以处理此事件的事件处理器的指针给反应器。 

随后反应器框架将自动地： 
- 
在内部维护一些表，将不同的事件类型与事件处理器对象关联起来。 

- 
在用户已登记的某个事件发生时，反应器发出对处理器中相应方法的回调。 

反应器模式在ACE中被实现为ACE_Reactor类，它提供反应器框架的功能接口。 

如上面所提到的，反应器将事件处理器对象作为服务提供者使用。反应器内部记录某个事件处理器的特定事件的相关回调方法。当这些事件发生时，反应器会创建这种事件和相应的事件处理器的关联。 
- 事件处理器
事件处理器就是需要通过轮询发生事件改变的对象列表中的对象，如在上面的例子中就是连接的客户端，每个客户端都可以看成一个事件处理器。 
- 回调事件
就是反应器支持的事件，如Socket读就绪，写就绪。拿上面的例子来说，如果某个客户端（事件处理器）在反应器中注册了读就绪事件，当客户端给服务器发送一条消息的时候，就会触发这个客户端的数据可读的回调函数。 

在反应器框架中，所有应用特有的事件处理器都必须由ACE_Event_Handler的抽象接口类派生。可以通过重载相应的"handle_"方法实现相关的回调方法。 

使用ACE_Reactor基本上有三个步骤： 
- 创建ACE_Event_Handler的子类，并在其中实现适当的"handle_"方法，以处理你想要此事件处理器为之服务的事件类型。 
- 通过调用反应器对象的register_handler()，将你的事件处理器登记到反应器。 
- 在事件发生时，反应器将自动回调相应的事件处理器对象的适当的handle_"方法。 

下面我就以一个Socket客户端的例子为例简单的说明反应器的基本用法。

#include<ace/OS.h>
#include<ace/Reactor.h>
#include<ace/SOCK_Connector.h>

#include<string>
#include<iostream>
usingnamespace std;

class MyClient:public ACE_Event_Handler 
{
public:
bool open()
    {
        ACE_SOCK_Connector connector;
        ACE_INET_Addr addr(3000,"127.0.0.1");
        ACE_Time_Value timeout(5,0);
if(connector.connect(peer,addr,&timeout) != 0)
        {
            cout<<endl<<"connecetd fail";
returnfalse;
        }
        ACE_Reactor::instance()->register_handler(this,ACE_Event_Handler::READ_MASK);
        cout<<endl<<"connecetd ";
returntrue;
    }

    ACE_HANDLE get_handle(void) const
    {
return peer.get_handle();
    }

int handle_input (ACE_HANDLE fd)
    {
int rev=0;
        ACE_Time_Value timeout(5,0);
if((rev=peer.recv(buffer,1000,&timeout))>0)
        {
            buffer[rev]='\0';
            cout<<endl<<"rev:\t"<<buffer<<endl;
        }
return 3;
    }

private:
    ACE_SOCK_Stream peer;
char buffer[1024];
};

int main(int argc, char *argv[]) 
{
    MyClient client;
    client.open();

while(true)
    {
        ACE_Reactor::instance()->handle_events(); 
    }

return 0; 
}

在这个例子中，客户端连接上服务器后，通过ACE_Reactor::instance()->register_handler(this,ACE_Event_Handler::READ_MASK)注册了一个读就绪的回调函数，当服务器端给客户端发消息的时候，会自动触发handle_input()函数，将接收到的信息打印出来。

在Socket编程中，常见的事件就是"读就绪"，"写就绪"，通过对这两个事件的捕获分发，可以实现Socket中的异步操作。 

**Socket编程中的事件处理器 **

在前面我们已经介绍过，在ACE反应器框架中，任何都必须派生自ACE_Event_Handler类，并通过重载其相应会调事件处理函数来实现相应的回调处理的。在Socket编程中，我们通常需要重载的函数有 
- handle_input()
当I/O句柄（比如UNIX中的文件描述符）上的输入可用时，反应器自动回调该方法。 
- handle_output()
当I/O设备的输出队列有可用空间时，反应器自动回调该方法。 
- handle_close()
当事件处理器中的事件从Reactor中移除的时候调用。 

此外，为了使Reactor能通过I/O句柄找到对应的事件处理器，还必须重载其get_handle()方法以使得Reactor建立起I/O句柄和事件处理器的关联。 

**使用Reactor框架。 **

下面我们将以一个客户端的程序为例，介绍如何在Socket编程中使用Reactor框架。 

一．建立一个客户端对象（事件处理器）。 

客户端对象就是一个事件处理器，其声明如下： 

class Client:public ACE_Event_Handler
{
public:
    ACE_HANDLE get_handle(void) const;
int handle_input (ACE_HANDLE fd);
int handle_close (ACE_HANDLE handle,
ACE_Reactor_Mask close_mask);
    ACE_SOCK_Stream& Peer();
private:
    ACE_SOCK_Stream peer;
}; 

在Client端中我只关心"读就绪"事件，故只重载了handle_input函数（大多数应用下只需要重载handle_input函数）。另外，在客户端还保存了一个ACE_SOCK_Stream的peer对象用来进行Socket通信，同时封装了一个Peer()函数返回它的引用。 

二．重载相应回调处理函数 

ACE_SOCK_Stream& Client::Peer()
{
return peer;
}

ACE_HANDLE Client::get_handle(void) const
{
return peer.get_handle();
}

int Client::handle_input (ACE_HANDLE fd)
{
int rev=0;
if((rev = peer.recv(buffer,1000))>0)
    {
        buffer[rev]='\0';
        cout<<endl<<"rev:\t"<<buffer<<endl;
return 0;
    }
else//Socket连接发生错误，返回-1，在Reactor中注销事件，触发handle_close函数
    {
return -1;
    }
}

int Client::handle_close (ACE_HANDLE handle,
                         ACE_Reactor_Mask close_mask)
{
    cout<<endl<<"connecetd closed";
return ACE_Event_Handler::handle_close(handle,close_mask);
} 

几个函数的功能都非常简单，这里就不多做介绍了。 

三．在Reactor中注册事件 

首先让我们来看看相应的main函数的代码： 

int main(int argc, char *argv[]) 
{
    Client client;
    ACE_SOCK_Connector connector;
    ACE_INET_Addr addr(3000,"127.0.0.1");
    ACE_Time_Value timeout(5,0);
if(connector.connect(client.Peer(),addr,&timeout) != 0)
    {
        cout<<endl<<"connecetd fail";
return 0;
    }

    ACE_Reactor::instance()->register_handler(&client,ACE_Event_Handler::READ_MASK);

while(true)
{
ACE_Reactor::instance()->handle_events(); 
}

return 0; 
} 

在这里可以看到，使用Reactor框架后，依然首先通过ACE_SOCK_Connector的connect函数来建立连接。建立连接后，可以通过ACE_Reactor::instance()->register_handler函数来实现Reactor的注册，实现I/O事件和Client对象的handle_input方法相关联，它的第一个参数是事件处理器的地址，第二个参数是事件类型，由于这里只关心读就绪事件，故注册的事件类型是ACE_Event_Handler::READ_MASK。

四．启动Reactor事件循环 

通过如上设置后，我们就可以通过ACE_Reactor::instance()->handle_events()启动Reactor循环了，这样，每当服务器端有数据发送给客户端时，当客户端的数据就绪时，就回触发Client对象的handle_input函数，将接收的数据打印出来。

通常的做法是，将Reactor事件循环作为一个单独的线程来处理，这样就不会阻塞main函数。 

五．注销Reactor事件 

Reactor事件的注销一般有两种方式，显式和隐式，下面将分别给予介绍。 
- 隐式注销。
当Reactor捕获事件后，会触发相应的"handle_"处理函数，当"handle_"处理函数返回值大于或等于0时，表示处理事件成功，当返回值小于0时，表示处理事件失败，这时Reactor会自动注销该句柄所注册的所有事件，并触发handle_close函数，以执行相应的资源清理工作。
在本例中，当handle_input函数里的recv函数接收到0个数时，说明socket发生错误（大多为Socket连接中断），此时返回－1，则系统自动注销相应事件。 
- 显示注销。
调用Reactor对象的remove_handler方法移除，它有两个参数，第一个是所注册的事件反应器对象，第二个是所要注销的事件。 

在这个示例程序里，连接方只有一个Socket连接，Reactor的优势并没有体现出来，但在一些网络管理系统里，连接方需要对多个需要管理的设备（服务器端）进行连接，在这种情况下使用Reactor模式，只需要多开一个Reactor事件循环线程就能实现事件多路分发复用，并且不会阻塞，通过面向对象的回调方式管理，使用起来非常方便。

**在服务器端使用Reactor框架 **

使用Reactor框架的服务器端结构如下： 

服务器端注册两种事件处理器，ClientAcceptor和ClientService ，ClientService类负责和客户端的通信，每一个ClientService对象对应一个客户端的Socket连接。 ClientAcceptor专门负责被动接受客户端的连接，并创建ClientService对象。这样，在一个N个Socket连接的服务器程序中，将存在1个ClientAcceptor对象和N个ClientService对象。 

整个服务器端流程如下： 
- 
首先创建一个ClientAcceptor对象，该对象在Reactor上注册ACCEPT_MASK事件，Reactor将自动在监听端口建立Socket监听。 

- 
如果有对该端口的Socket连接时，Reactor将自动回调handle_input方法，ClientAcceptor重载此方法，并创建一个ClientService对象，用于处理和Client的通信。 

- 
ClientService对象根据服务器的具体功能实现，其处理过程和客户端程序类似，注册相应的回调事件并分发即可。 

代码如下： 

#include<ace/OS.h>
#include<ace/Reactor.h>
#include<ace/SOCK_Connector.h>
#include<ace/SOCK_Acceptor.h>
#include<ace/Auto_Ptr.h>

class ClientService : public ACE_Event_Handler
{
public:
    ACE_SOCK_Stream &peer (void) { returnthis->sock_; }

int open (void)
    {
//注册读就绪回调函数
returnthis->reactor ()->register_handler(this, ACE_Event_Handler::READ_MASK);
    }

virtual ACE_HANDLE get_handle (void) const { returnthis->sock_.get_handle (); }

virtualint handle_input (ACE_HANDLE fd )
    {
//一个简单的EchoServer，将客户端的信息返回
int rev = peer().recv(buf,100);
if(rev<=0)
return -1;

        peer().send(buf,rev);
return 0;
    }

// 释放相应资源
virtualint handle_close (ACE_HANDLE, ACE_Reactor_Mask mask)
    {
if (mask == ACE_Event_Handler::WRITE_MASK)
return 0;
        mask = ACE_Event_Handler::ALL_EVENTS_MASK |
            ACE_Event_Handler::DONT_CALL;
this->reactor ()->remove_handler (this, mask);
this->sock_.close ();
deletethis;    //socket出错时，将自动删除该客户端，释放相应资源
return 0;
    }

protected:
char buf[100];
    ACE_SOCK_Stream sock_;
};

class ClientAcceptor : public ACE_Event_Handler
{
public:
virtual ~ClientAcceptor (){this->handle_close (ACE_INVALID_HANDLE, 0);}

int open (const ACE_INET_Addr &listen_addr)
    {
if (this->acceptor_.open (listen_addr, 1) == -1)
        {
            ACE_OS::printf("open port fail");
return -1;
        }
//注册接受连接回调事件
returnthis->reactor ()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);
    }

virtual ACE_HANDLE get_handle (void) const
    { returnthis->acceptor_.get_handle (); }

virtualint handle_input (ACE_HANDLE fd )
    {
        ClientService *client = new ClientService();
        auto_ptr<ClientService> p (client);

if (this->acceptor_.accept (client->peer ()) == -1)
        {
            ACE_OS::printf("accept client fail");
return -1;
        }
        p.release ();
        client->reactor (this->reactor ());
if (client->open () == -1)
            client->handle_close (ACE_INVALID_HANDLE, 0);
return 0;
    }

virtualint handle_close (ACE_HANDLE handle,
        ACE_Reactor_Mask close_mask)
    {
if (this->acceptor_.get_handle () != ACE_INVALID_HANDLE)
        {
            ACE_Reactor_Mask m = ACE_Event_Handler::ACCEPT_MASK |
                ACE_Event_Handler::DONT_CALL;
this->reactor ()->remove_handler (this, m);
this->acceptor_.close ();
        }
return 0;
    }

protected:
    ACE_SOCK_Acceptor acceptor_;
};

int main(int argc, char *argv[]) 
{
    ACE_INET_Addr addr(3000,"192.168.1.142");
    ClientAcceptor server;
    server.reactor(ACE_Reactor::instance());
    server.open(addr);

while(true)
    {
        ACE_Reactor::instance()->handle_events(); 
    }

return 0; 
}

代码功能比较简单，需要注意以下几点： 
- 
这里注册事件的方式和前面的文章中方式不一样，是通过ACE_Event_Handler类的reactor()方法设置和获取reactor的指针，比较直观和方便。前面的文章是通过ACE_Reactor::instance()来获取的一个单体reactor的指针。 

- 当客户端Socket连接关闭时，需要释放相应资源，需要注意一下ClientService对象的handle_close方法中释放资源的相应代码。

定时器的实现 

通过Reactor机制，还可以很容易的实现定时器的功能，使用方式如下。 
- 
编写一个事件反应器，重载handle_timeout()方法，该方法是定时器的触发时间到时，会自动触发该方法。 

- 
通过Reactor的schedule_timer()方法注册定时器。 

- 
启动reacotr的handle_events()事件分发循环。 

- 
当不想使用定时器时，可以通过Reactor的cancel_timer()方法注销定时器。 

下面的代码简单的实现了一个定时器，并具有基本的开启，关闭功能。 

#include<ace/OS.h>
#include<ace/Reactor.h>

class MyTimerHandler : public ACE_Event_Handler
{
private:
int inteval;    //执行时间间隔
int delay;        //延迟执行时间
int timerid;

public:
    MyTimerHandler(int delay,int inteval)
    {
this->delay=delay;
this->inteval=inteval;
    }

int open()    //注册定时器
    {
        ACE_Time_Value delaytime(inteval);
        ACE_Time_Value intevaltime(inteval);
        timerid = reactor()->schedule_timer(this,
            0,    //传递handle_timeout给的参数
            delaytime,
            intevaltime);
return timerid;
    }

int close()    //取消定时器
    {
return reactor()->cancel_timer(timerid);
    }

//定时器回调函数
int handle_timeout (const ACE_Time_Value ¤t_time,
constvoid * = 0)
    {
        time_t epoch = ((timespec_t)current_time).tv_sec;
        ACE_DEBUG ((LM_INFO,
            ACE_TEXT ("handle_timeout: %s\n"),
            ACE_OS::ctime (&epoch)));
return 0;
    }
};

int main(int argc, char *argv[]) 
{
    MyTimerHandler * timer = new MyTimerHandler (3,5);
    timer->reactor(ACE_Reactor::instance());
    timer->open();

for(int i=0;i<2;i++)    //触发次handle_timeout事件
    {
        ACE_OS::printf("\n%d\n",i);
        ACE_Reactor::instance()->handle_events();
    }

    timer->close();
    ACE_OS::printf("cancel timer");
while(true)
        ACE_Reactor::instance()->handle_events();
return 0; 
}

代码功能比较简单，这里就不多做介绍了。


