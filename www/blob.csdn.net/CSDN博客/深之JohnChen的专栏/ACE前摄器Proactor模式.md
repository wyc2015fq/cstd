# ACE前摄器Proactor模式 - 深之JohnChen的专栏 - CSDN博客

2010年03月12日 10:37:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1512


[版权声明](http://creativecommons.org/licenses/by/3.0/deed.zh)：转载时请以超链接形式标明文章原始出处和作者信息及[本声明](http://bangzhuzhongxin.blogbus.com/logs/11205960.html)
[http://egeho123.blogbus.com/logs/10780720.html](http://egeho123.blogbus.com/logs/10780720.html)

当 OS 平台支持异步操作时，一种高效而方便的实现高性能 Web 服务器的方法是使用前摄式事件分派。使用前摄式事件分派模型设计的 Web 服务器通过一或多个线程控制来处理异步操作的完成。这样，通过集成完成事件多路分离（completion event demultiplexing）和事件处理器分派，前摄器模式简化了异步的 Web 服务器。 

异步的 Web 服务器将这样来利用前摄器模式：首先让 Web 服务器向 OS 发出异步操作，并将回调方法登记到 Completion Dispatcher（完成分派器），后者将在操作完成时通知 Web 服务器。于是 OS 代表 Web 服务器执行操作，并随即在一个周知的地方将结果排队。Completion Dispatcher 负责使完成通知出队，并执行适当的、含有应用特有的 Web 服务器代码的回调。 

使用前摄器模式的主要优点是可以启动多个并发操作，并可并行运行，而不要求应用必须拥有多个线程。操作被应用异步地启动，它们在 OS 的 I/O 子系统中运行直到完成。发起操作的线程现在可以服务 另外的请求了。 

在ACE中，可以通过ACE_Proactor实现前摄器模式。实现方式如下。 

**1。创建服务处理器： **

Proactor框架中服务处理器均派生自ACE_Service_Handler，它和Reactor框架的事件处理器非常类似。当发生IO操作完成事件时，会触发相应的事件完成会调函数。 

**2。实现服务处理器IO操作 **

Proactor框架中所有的IO操作都由相应的异步操作类来完成，这些异步操作类都继承自ACE_Asynch_Operation。常用的有以下几种。 
- 
ACE_Asynch_Read_Stream, 提供从TCP/IP socket连接中进行异步读操作. 

- 
ACE_Asynch_Write_Stream, 提供从TCP/IP socket连接中进行异步写操作. 

使用这些操作类的一般方式如下： 
- 初始化
将相关的操作注册到服务处理器中，一般可通过调用其open方法实现。 
- 发出IO操作
发出异步IO操作请求，该操作不会阻塞，具体的IO操作过程由操作系统异步完成。 
- IO操作完成回调处理
异步IO操作完成后，OS会触发服务处理器中的相应回调函数，可通过该函数的ACE_Asynch_Result参数获取相应的返回值。 

**3。使用连接器或接受器和远端进行连接 **

ACE为Proactor框架提供了两个工厂类来建立TCP/IP连接。 
- 
ACE_Asynch_Acceptor, 用于被动地建立连接 

- 
ACE_Asynch_Connector 用于主动地建立连接 

当远端连接建立时，连接器或接受器便会创建相应的服务处理器，从而可以实现服务处理。 

**4。启动Proactor事件分发处理 **

启动事件分发处理只需如下调用： 

while(true)
        ACE_Proactor::instance ()->handle_events ();

**5。程序示例 **

服务器端： 

服务器端简单的实现了一个EchoServer，流程如下： 

当客户端建立连接时，首先发出一个异步读的异步请求，当读完成时，将所读的数据打印出来，并发出一个新的异步请求。 

#include"ace/Message_Queue.h"
#include"ace/Asynch_IO.h"
#include"ace/OS.h"
#include"ace/Proactor.h"
#include"ace/Asynch_Acceptor.h"

class HA_Proactive_Service : public ACE_Service_Handler
{
public:
~HA_Proactive_Service ()
{
if (this->handle () != ACE_INVALID_HANDLE)
ACE_OS::closesocket (this->handle ());
}

virtualvoid open (ACE_HANDLE h, ACE_Message_Block&)
{
this->handle (h);
if (this->reader_.open (*this) != 0 )
     {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p/n"),
             ACE_TEXT ("HA_Proactive_Service open")));
deletethis;
return;
     }

     ACE_Message_Block *mb = new ACE_Message_Block(buffer,1024);
if (this->reader_.read (*mb, mb->space ()) != 0)
     {
         ACE_OS::printf("Begin read fail/n");
deletethis;
return;
     }

return;
}

//异步读完成后会调用此函数
virtualvoid handle_read_stream
(const ACE_Asynch_Read_Stream::Result &result)
{
     ACE_Message_Block &mb = result.message_block ();
if (!result.success () || result.bytes_transferred () == 0)
     {
         mb.release ();
deletethis;
return;
     }

     mb.copy("");    //为字符串添加结束标记'/0'
     ACE_OS::printf("rev:/t%s/n",mb.rd_ptr());
     mb.release();

     ACE_Message_Block *nmb = new ACE_Message_Block(buffer,1024);
if (this->reader_.read (*nmb, nmb->space ()) != 0)

return;
}

private:
ACE_Asynch_Read_Stream reader_;
char buffer[1024];
};

int main(int argc, char *argv[]) 
{
int port=3000;
    ACE_Asynch_Acceptor<HA_Proactive_Service> acceptor;

if (acceptor.open (ACE_INET_Addr (port)) == -1)
return -1;

while(true)
        ACE_Proactor::instance ()->handle_events ();

return 0; 
}

客户端： 

客户端代码比较简单，就是每隔1秒钟将当前的系统时间转换为字符串形式通过异步形式发送给服务器，发送完成后，释放时间字符的内存空间。 

#include"ace/Message_Queue.h"
#include"ace/Asynch_IO.h"
#include"ace/OS.h"
#include"ace/Proactor.h"
#include"ace/Asynch_Connector.h"

class HA_Proactive_Service : public ACE_Service_Handler
{
public:
~HA_Proactive_Service ()
{
if (this->handle () != ACE_INVALID_HANDLE)
ACE_OS::closesocket (this->handle ());
}

virtualvoid open (ACE_HANDLE h, ACE_Message_Block&)
{
this->handle (h);
if (this->writer_.open (*this) != 0 )
     {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p/n"),
             ACE_TEXT ("HA_Proactive_Service open")));
deletethis;
return;
     }

     ACE_OS::printf("connceted");

for(int i=0;i<10;i++)    //每隔秒中发送时间至服务器
     {
         ACE_OS::sleep(1);
         time_t now = ACE_OS::gettimeofday().sec();
char *time = ctime(&now);        //获取当前时间的字符串格式
         ACE_Message_Block *mb = new ACE_Message_Block(100);
         mb->copy(time);

if (this->writer_.write(*mb,mb->length()) !=0)
         {
             ACE_OS::printf("Begin read fail/n");
deletethis;
return;
         }
     }

return;
}

//异步写完成后会调用此函数
virtualvoid handle_write_dgram
(const ACE_Asynch_Write_Stream::Result &result)
{
     ACE_Message_Block &mb = result.message_block ();
     mb.release();
return;
}

private:
ACE_Asynch_Write_Stream writer_;
};

int main(int argc, char *argv[]) 
{

    ACE_INET_Addr addr(3000,"192.168.1.142"); 

    HA_Proactive_Service *client = new HA_Proactive_Service();
    ACE_Asynch_Connector<HA_Proactive_Service> connector;

    connector.open();
if (connector.connect(addr) == -1)
return -1;

while(true)
        ACE_Proactor::instance ()->handle_events ();

return 0; 
}

