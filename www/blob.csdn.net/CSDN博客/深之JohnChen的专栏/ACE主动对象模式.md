# ACE主动对象模式 - 深之JohnChen的专栏 - CSDN博客

2011年12月23日 14:37:08[byxdaz](https://me.csdn.net/byxdaz)阅读数：1263标签：[string																[object																[os																[class																[任务																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=os&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[ACE网络编程](https://blog.csdn.net/byxdaz/article/category/628882)


 ACE主动对象模式

转载：[http://www.cnblogs.com/TianFang/archive/2006/12/11/589168.html](http://www.cnblogs.com/TianFang/archive/2006/12/11/589168.html)

主动对象模式用于降低方法执行和方法调用之间的耦合。该模式描述了另外一种更为透明的任务间通信方法。 

传统上，所有的对象都是被动的代码段，对象中的代码是在对它发出方法调用的线程中执行的，当方法被调用时，调用线程将阻塞，直至调用结束。而主动对象却不一样。这些对象具有自己的命令执行线程，主动对象的方法将在自己的执行线程中执行，不会阻塞调用方法。 

例如，设想对象"A"已在你的程序的main()函数中被实例化。当你的程序启动时，OS创建一个线程，以从main()函数开始执行。如果你调用对象A的任何方法，该线程将"流过"那个方法，并执行其中的代码。一旦执行完成，该线程返回调用该方法的点并继续它的执行。但是，如果"A"是主动对象，事情就不是这样了。在这种情况下，主线程不会被主动对象借用。相反，当"A"的方法被调用时，方法的执行发生在主动对象持有的线程中。另一种思考方法：如果调用的是被动对象的方法（常规对象），调用会阻塞（同步的）；而另一方面，如果调用的是主动对象的方法，调用不会阻塞（异步的）。 

由于主动对象的方法调用不会阻塞，这样就提高了系统响应速度，在网络编程中是大有用武之地的。 

在这里我们将一个"Logger"（日志记录器）对象对象为例来介绍如何将一个传统对象改造为主动对象，从而提高系统响应速度。 

Logger的功能是将一些系统事件的记录在存储器上以备查询，由于Logger使用慢速的I/O系统来记录发送给它的消息，因此对Logger的操作将会导致系统长时间的等待。 

其功能代码简化如下： 

class Logger: public ACE_Task<ACE_MT_SYNCH>
{
public:
void LogMsg(const string& msg)
    {
        cout<<endl<<msg<<endl;
        ACE_OS::sleep(2);
    }
}; 

为了实现实现记录日志操作的主动执行，我们需要用命令模式将其封装，从而使得记录日志的方法能在合适的时间和地方主动执行，封装方式如下： 

class LogMsgCmd: public ACE_Method_Object
{
public:
    LogMsgCmd(Logger *plog,const string& msg)
    {
this->log=plog;
this->msg=msg;
    }

int call()
    {
this->log->LogMsg(msg);
return 0;
    }

private:
    Logger *log;
    string msg;
};

class Logger: public ACE_Task<ACE_MT_SYNCH>
{
public:
void LogMsg(const string& msg)
    {
        cout<<endl<<msg<<endl;
        ACE_OS::sleep(2);
    }

    LogMsgCmd *LogMsgActive(const string& msg)
    {
new LogMsgCmd(this,msg);
    }
}; 

这里对代码功能做一下简单的说明： 

ACE_Method_Object是ACE提供的命令模式借口，命令接口调用函数为int call()，在这里通过它可以把每个操作日志的调用封装为一个LogMsgCmd对象，这样，当原来需要调用LogMsg的方法的地方只要调用LogMsgActive即可生成一个LogMsgCmd对象，由于调用LogMsgActive方法，只是对命令进行了封装，并没有进行日志操作，所以该方法会立即返回。然后再新开一个线程，将LogMsgCmd对象作为参数传入，在该线程中执行LogMsgCmd对象的call方法，从而实现无阻塞调用。 

然而，每次对一个LogMsg调用都开启一个新线程，无疑是对资源的一种浪费，实际上我们往往将生成的LogMsgCmd对象插入一个命令队列中，只新开一个命令执行线程依次执行命令队列中的所有命令。并且，为了实现对象的封装，命令队列和命令执行线程往往也封装到Logger对象中，代码如下所示： 

#include"ace/OS.h"
#include"ace/Task.h"
#include"ace/Method_Object.h"
#include"ace/Activation_Queue.h"
#include"ace/Auto_Ptr.h"

#include<string>
#include<iostream>
usingnamespace std;

class Logger: public ACE_Task<ACE_MT_SYNCH>
{
public:
    Logger()
    {
this->activate();
    }

int svc();
void LogMsg(const string& msg);
void LogMsgActive (const string& msg);

private:
    ACE_Activation_Queue cmdQueue;    //命令队列
};

class LogMsgCmd: public ACE_Method_Object
{
public:
    LogMsgCmd(Logger *plog,const string& msg)
    {
this->log=plog;
this->msg=msg;
    }

int call()
    {
this->log->LogMsg(msg);
return 0;
    }

private:
    Logger *log;
    string msg;
};

void Logger::LogMsg(const string& msg)
{
    cout<<endl<<msg<<endl;
    ACE_OS::sleep(2);
}

//以主动的方式记录日志
void Logger::LogMsgActive(const string& msg)
{
//生成命令对象，插入到命令队列中
    cmdQueue.enqueue(new LogMsgCmd(this,msg));
}

int Logger::svc()
{
while(true)
    {
//遍历命令队列，执行命令
        auto_ptr<ACE_Method_Object> mo
            (this->cmdQueue.dequeue ());

if (mo->call () == -1)
break;
    }
return 0;
}

int main (int argc, ACE_TCHAR *argv[])
{
    Logger log;
    log.LogMsgActive ("hello");

    ACE_OS::sleep(1);
    log.LogMsgActive("abcd");

while(true)
        ACE_OS::sleep(1);

return 0;
} 

在这里需要注意一下命令队列ACE_Activation_Queue对象，它是线程安全的，使用方法比较简单，这里我也不多介绍了。 

主动对象的基本结构就是这样，然而，由于主动对象是异步调用的，又引出了如下两个新问题： 
- 方法调用线程如何知道该方法已经执行完成？ 
- 如何或得方法的返回值？ 

要解决这两个问题，首先得介绍一下ACE_Future对象，ACE_Future是表示一个会在将来被赋值的"期货"对象，可以通过ready()函数查询它是否已经被赋值。该对象创建的时候是未赋值的，后期可以通过set()函数来进行赋值，所赋的值可以通过get()函数来获取。 

下面代码演示了它的基本用法： 

#include"ace/Future.h"

#include<string>
#include<iostream>
usingnamespace std;

void get_info(ACE_Future<string> &fu)
{
    string state = fu.ready()?"ready":"not ready";
    cout<<endl<<state<<endl;
if(fu.ready())
    {
        string value;
        fu.get(value);
        cout<<"value:\t"<<value<<endl;
    }
}

int main(int argc, char *argv[])
{
    ACE_Future<string> fu;
    get_info(fu);
    fu.set("12345");
    get_info(fu);

return 0;
} 

通过ACE_Future对象来解决上述两个问题的方法如下： 
- 首先创建ACE_Future对象用以保留返回值。 
- 调用主动命令时将ACE_Future对象作为参数传入，生成的命令对象中保存ACE_Future对象的指针。 
- 命令执行线程执行完命令后，将返回值通过set()函数设置到ACE_Future对象中。 
- 调用线程可以通过ACE_Future对象的ready()函数查询该命令是否执行完成，如果命令执行完成，则可通过get()函数来获取返回值。 

使用的时候要注意一下ACE_Future对象的生命周期。 

为了演示了如何获取主动命令的执行状态和结果，我将上篇文章中的代码改动了一下，日志类记录日志后，会将记录的内容作为返回值返回，该返回值会通过ACE_Future对象返回，代码如下： 

#include"ace/OS.h"
#include"ace/Task.h"
#include"ace/Method_Object.h"
#include"ace/Activation_Queue.h"
#include"ace/Auto_Ptr.h"

#include"ace/Future.h"

#include<string>
#include<iostream>
usingnamespace std;

class Logger: public ACE_Task<ACE_MT_SYNCH>
{
public:
    Logger()
    {
this->activate();
    }

int svc();
    string LogMsg(const string& msg);
void LogMsgActive (const string& msg,ACE_Future<string> *result);

private:
    ACE_Activation_Queue cmdQueue; //命令队列
};

class LogMsgCmd: public ACE_Method_Object
{
public:
    LogMsgCmd(Logger *plog,const string& msg,ACE_Future<string> *result)
    {
this->log=plog;
this->msg=msg;
this->result=result;
    }

int call()
    {
        string reply = this->log->LogMsg(msg);
        result->set(reply);
return 0;
    }

private:
    ACE_Future<string> *result;
    Logger *log;
    string msg;
};

string Logger::LogMsg(const string& msg)
{
    ACE_OS::sleep(2);
    cout<<endl<<msg<<endl;
return msg;
}

//以主动的方式记录日志
void Logger::LogMsgActive(const string& msg,ACE_Future<string> *result)
{
//生成命令对象，插入到命令队列中
    cmdQueue.enqueue(new LogMsgCmd(this,msg,result));
}

int Logger::svc()
{
while(true)
    {
//遍历命令队列，执行命令
        auto_ptr<ACE_Method_Object> mo
            (this->cmdQueue.dequeue ());

if (mo->call () == -1)
break;
    }
return 0;
}

void get_info(ACE_Future<string> &fu)
{
    string state = fu.ready()?"ready":"not ready";
    cout<<endl<<state<<endl;
if(fu.ready())
    {
        string value;
        fu.get(value);
        cout<<"value:\t"<<value<<endl;
    }
}

int main (int argc, ACE_TCHAR *argv[])
{
    ACE_Future<string> result;
    Logger log;
    log.LogMsgActive ("hello",&result);

while(true)
    {
        get_info(result);
if(result.ready())
break;
        ACE_OS::sleep(1);
    }

    cout<<endl<<"cmd end"<<endl;

while(true)
        ACE_OS::sleep(1);

return 0;
} 

代码比较简单，这里就不多解释了。 

这种查询模式比较简单有效，但存在一个问题：调用线程必须不断轮询ACE_Future对象以获取返回值，这样的效率比较低。可以通过观察者模式解决这个问题：在ACE_Future对象上注册一个观察者，当ACE_Future对象的值发生改变（异步命令执行完成）时主动通知该观察者，从而获取返回值。 

ACE中的观察者模式可以通过ACE_Future_Observer来实现，使用方法如下： 

#include"ace/Future.h"

#include<string>
#include<iostream>
usingnamespace std;

class MyObserver:public ACE_Future_Observer<string>
{
virtualvoid update (const ACE_Future<string> &future)
    {
        string value;
        future.get(value);
        cout<<endl<<"change:\t"<<value<<endl;
    }
};

int main(int argc, char *argv[])
{
    MyObserver obv;
    ACE_Future<string> fu;

    fu.attach(&obv);

    ACE_OS::sleep(3);
    fu.set("12345");

while(true)
        ACE_OS::sleep(3);

return 0;
}

通过观察者模式，可以更有效，及时的获取异步命令的返回值,但同时也增加了程序结构的复杂度并且难以调试，使用的时候应该根据需要选取合适的方式。


