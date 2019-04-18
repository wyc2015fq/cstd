# ACE中日志系统 - 深之JohnChen的专栏 - CSDN博客

2011年12月04日 22:08:44[byxdaz](https://me.csdn.net/byxdaz)阅读数：4620


介绍了ACE中日志系统(Logging Facility）的基本使用

一、简介

以前曾介绍过一个开源日志系统log4cplus，ACE也有自己的Logging Facility，与log4cplus相似，ACE日志系统

也具有线程安全、灵活、分级显示等特点，可以面向程序调试、运行、测试、和维护等全生命周期，可以选择将

信息输出到屏幕、文件、系统日志（如Windows下的Event log）、甚至是远程服务器。除此之外，ACE日志系统

支持回调函数以及运行时刻动态配置（Runtime Configuration ），本文主要参考了《ACE Programmer's Guide,

The: Practical Design Patterns for Network and Systems Programming》。

二、ACE日志系统的基本使用

首先通过一个引例介绍如何调用ACE Log Macro：

#include < ace/Log_Msg.h >

int ACE_TMAIN (int, ACE_TCHAR *[])

{

ACE_DEBUG ((LM_INFO, ACE_TEXT ("Hi ACE Logging Facility!\n")));

return 0;

}

程序需要包含头文件：Log_Msg.h，该文件定义了各种有用的日志输出宏，宏ACE_TMAIN 是ACE定义的

wide-character-enabled entry point ，ACE日志系统缺省输出到stderr，所以对于Console类型的应用程序，

会直接输出到屏幕。运行该程序时屏幕上会显示：

Hi ACE Logging Facility!

可以看出ACE日志系统的宏调用非常简单：

ACE_XXX((severity, formatting-args));

其中，ACE_XXX表示日志输出宏（ACE Logging Macros），包括：

ACE_ERROR((level, string, ...))

ACE_DEBUG((level, string, ...))

ACE_TRACE(string)

ACE_ASSERT(test)

ACE_HEX_DUMP((level, buffer, size [,text]))

ACE_RETURN(value)

ACE_ERROR_RETURN((level, string, ...), value)

ACE_ERROR_INIT( value, flags )

ACE_ERROR_BREAK((level, string, ...))

severity即输出的严重等级（Severity Level），包括：

LM_TRACE Messages indicating function-calling sequence

LM_DEBUG Debugging information

LM_INFO Messages that contain information normally of use only when debugging a program

LM_NOTICE Conditions that are not error conditions but that may require special handling

LM_WARNING Warning messages

LM_ERROR Error messages

LM_CRITICAL Critical conditions, such as hard device errors

LM_ALERT A condition that should be corrected immediately, such as a corrupted database

LM_EMERGENCY A panic condition, normally broadcast to all usersLM_TRACE Messages indicating

function-calling sequence

formatting-args是要输出的内容，格式类似于printf函数的输出格式：

Code Argument Type Displays

A ACE_timer_t 浮点数

a — 导致程序终止（Abort）

c char 单个字符

C char* 字符串（narrow characters）

i,d int 10进制整数

I — 缩进

e,E,f,F,g,G double 双精度浮点数

l — 行号

M — severity level的名称

m — 错误号（errorno）

N — 文件名

n — ACE_Log_Msg::open()指定的程序名

o int 八进制整数

P — 当前进程ID

p ACE_TCHAR* 字符串，后接错误号，同perror

Q ACE_UINT64 64位无符号十进制整数

r void (*)() 函数调用

R int 十进制整数

S int 数字对应的信号名称

s ACE_TCHAR* ACE_TCHAR类型的字符串

T — 当前时间（hour:minute:sec.usec）

D — 时戳（month/day/year/hour:minute:sec.usec）

t — 当前线程ID

u int 无符号十进制整数

w wchar_t Single wide character

W wchar_t* Wide-character string

x,X int 十六进制数

@ void* 指针（十六进制）

% N/A %

我们可以通过一些例子来进一步了解如何使用这些宏、Severity Level和格式，在开始之前，首先澄清一个概念：

一些日志系统（如log4cplus）是靠宏名来区分日志信息类型的，比如：

LOG4CPLUS_DEBUG(Logger::getRoot(),"some text") /* DEDUG 类型 */

LOG4CPLUS_ERROR(Logger::getRoot(),"some text") /* ERROR 类型 */

ACE的日志系统不是靠宏名本身，而是靠宏的第一个参数，即Severity Level来区分日志信息类型：

ACE_DEBUG((LM_DEBUG, ACE_TEXT ("some text\n"))); /* DEDUG 类型 */

ACE_ERROR((LM_DEBUG, ACE_TEXT ("some text\n"))); /* DEDUG 类型 */

ACE_DEBUG((LM_ERROR, ACE_TEXT ("some text\n"))); /* ERROR 类型 */

ACE_ERROR((LM_ERROR, ACE_TEXT ("some text\n"))); /* ERROR 类型 */

从这个角度考虑，宏ACE_DEBUG和ACE_ERROR没有什么区别，几乎可以混用，这两个宏的区别主要体现在两方面：

a) 语义上的差别

b) 语句执行完后，op_status不同，比如：

ACE_Log_Msg *lm = ACE_LOG_MSG;

ACE_DEBUG ((LM_ERROR, ACE_TEXT ("some text\n")));

ACE_ASSERT( 0 == lm->op_status() ); /* ACE_DEBUG执行完后，op_status 是0 */

ACE_ERROR ((LM_ERROR, ACE_TEXT ("some text\n")));

ACE_ASSERT( -1 == lm->op_status() ); /* ACE_ERROR执行完后，op_status 是-1 */

下面举一些典型用例：

【use-case1】 如何在编译时刻Enabling 或Disabling Logging Macros

在#include < ace/Log_Msg.h >之前定义一些宏，可以在编译时刻Enable/Disable logging Macros

/* disable ACE_DEBUG and ACE_ERROR */

#define ACE_NLOGGING

/* enable ACE_DEBUG and ACE_ERROR（缺省） */

#undef ACE_NLOGGING

/* enable ACE_ASSERT（缺省） */

#undef ACE_NDEBUG

/* disable ACE_ASSERT */

#define ACE_NDEBUG

/* enable ACE_TRACE */

#define ACE_NTRACE 0

/* disable ACE_TRACE（缺省） */

#define ACE_NTRACE 1

【use-case2】格式使用

a) 打印完结果后终止（Abort）

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("begin %a end \n")));

输出以下信息后提示Abort信息：

begin Aborting... end

b) 缩进

int ACE_TMAIN (int, ACE_TCHAR *[])

{

ACE_TRACE("main");

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%Itext\n")));

return 0;

}

输出：

(700) calling main in file `D:\aceprj\testlog\main.cpp' on line 24

text

(700) leaving main

（其中，700是ACE_TRACE控制输出的当前线程ID）

c) HEX Dump（调试时候比较有用）

char szBuf[128] = "hello world!";

ACE_HEX_DUMP((LM_DEBUG, szBuf, 128,"szBuf: "));

输出：

szBuf: - HEXDUMP 128 bytes

68 65 6c 6c 6f 20 77 6f 72 6c 64 21 00 00 00 00 hello world!....

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................

d)函数调用

void foo(void)

{

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("in func foo\n")));

}

int ACE_TMAIN (int, ACE_TCHAR *[])

{

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("call function %r\n"),foo));

return 0;

}

输出：

in func foo

call function

e) 进制转换

ACE_UINT64 uint64A = 210113198510308319;

int hexA = 0xBEEE3F;

int octA = 9;

ACE_timer_t tmInterval = 0.000001;

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("UINT64: %Q\n"),uint64A));

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("HEX: %X, hex: %x\n"),hexA,hexA));

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ptr in hex:%@\n"),&hexA));

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Octal number of 9:%o\n"),octA));

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("timer's Interval: %A\n"),tmInterval));

输出：

UINT64: 210113198510308319

HEX: BEEE3F, hex: beee3f

Ptr in hex:0012FE30

Octal number of 9:11

timer's Interval: 0.000001

f)综合使用

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D, %M, [%t], Line:%l\n")));

ACE_DEBUG ((LM_ERROR, ACE_TEXT ("%D, %M, [%t], Line:%l\n")));

输出：

Mon Mar 20 2005 20:02:16.912000, LM_DEBUG, [3296], Line:24

Mon Mar 20 2006 20:02:16.912000, LM_ERROR, [3296], Line:25

【use-case3】宏使用

a) ACE_RETURN

int foo(void)

{

ACE_RETURN(3);

}

int ACE_TMAIN (int, ACE_TCHAR *[])

{

foo();

ACE_ASSERT( 3 == ACE_LOG_MSG->op_status() );

return 0;

}

输出：（无任何输出）

b) ACE_ERROR_RETURN

int foo(void)

{

ACE_ERROR_RETURN((LM_DEBUG,"Return error\n"), 3);

}

int ACE_TMAIN (int, ACE_TCHAR *[])

{

foo();

ACE_ASSERT( 3 == ACE_LOG_MSG->op_status() );

return 0;

}

输出：

Return error

c) ACE_ERROR_BREAK

int i = 0;

while(1)

{

if( i > 10 )

{

ACE_ERROR_BREAK((LM_DEBUG,"break from while\n"));

}

i++;

}

ACE_DEBUG((LM_DEBUG, "i = %d\n", i));

输出：

break from while

i = 11

【use-case4】 如何在运行时刻Enabling 或Disabling Logging Severities

由于缺省情况下在进程范围内所有logging severities都会被输出，因此需要通过priority_mask、

ACE_Log_Msg::enable_debug_messages、ACE_Log_Msg::disable_debug_messages来过滤输出。

其中priority_mask的用法如下：

/* Get the current ACE_Log_Priority mask. */

u_long priority_mask (MASK_TYPE = THREAD);

/* Set the ACE_Log_Priority mask, returns original mask. */

u_long priority_mask (u_long, MASK_TYPE = THREAD);

这里MASK_TYPE可选择：ACE_Log_Msg::PROCESS（作用范围是进程）或ACE_Log_Msg::THREAD（作用范围是线程）

enable_debug_messages/disable_debug_messages的用法如下：

static void disable_debug_messages

(ACE_Log_Priority priority = LM_DEBUG);

static void enable_debug_messages

(ACE_Log_Priority priority = LM_DEBUG);

这里输入参数可选择任意需要打开或屏蔽的logging severity，不像函数名称暗示的那样只能控制LM_DEBUG

类型，另外由于是静态函数，enable_debug_messages/disable_debug_messages的设置对整个进程有效。

注意一下priority_mask与enable_debug_messages/disable_debug_messages的区别：

priority_mask将进程（或线程）的logging severitys用其第一个参数所代替，清除之前的设置。

enable_debug_messages将进程的logging severitys用其参数去累加，不清除之前的设置。

disable_debug_messages将进程的logging severitys用其参数去屏蔽，不清除之前的设置。

a) 整个进程仅允许输出LM_ERROR和LM_WARNING

ACE_DEBUG((LM_DEBUG, "Debug Text1\n"));

ACE_DEBUG((LM_ERROR, "Error Text1\n"));

ACE_LOG_MSG->priority_mask (LM_ERROR | LM_WARNING, ACE_Log_Msg::PROCESS);

ACE_DEBUG((LM_DEBUG, "Debug Text2\n"));

ACE_DEBUG((LM_ERROR, "Error Text2\n"));

或者：

ACE_DEBUG((LM_DEBUG, "Debug Text1\n"));

ACE_DEBUG((LM_ERROR, "Error Text1\n"));

/* 禁用了进程所有的logging severitys */

ACE_LOG_MSG->priority_mask (0, ACE_Log_Msg::PROCESS);

ACE_Log_Msg::enable_debug_messages (LM_ERROR);

ACE_Log_Msg::enable_debug_messages (LM_WARNING);

ACE_DEBUG((LM_DEBUG, "Debug Text2\n"));

ACE_DEBUG((LM_ERROR, "Error Text2\n"));

输出：

Debug Text1

Error Text1

Error Text2

b) 每个线程控制各自的logging severity

#include < ace/Task.h > /* for ACE_Thread_Manager */

#include < ace/Log_Msg.h >

void service(void)

{

ACE_LOG_MSG->priority_mask (LM_INFO, ACE_Log_Msg::THREAD);

ACE_DEBUG((LM_DEBUG, "in service, MsgType:%M, ThreadID: %t\n"));

ACE_DEBUG((LM_INFO, "in service, MsgType:%M, ThreadID: %t\n"));

}

void worker(void)

{

ACE_LOG_MSG->priority_mask (LM_DEBUG, ACE_Log_Msg::THREAD);

ACE_DEBUG((LM_DEBUG, "in worker, MsgType:%M, ThreadID: %t\n"));

ACE_DEBUG((LM_INFO, "in worker, MsgType:%M, ThreadID: %t\n"));

}

int ACE_TMAIN (int, ACE_TCHAR *[])

{

ACE_LOG_MSG->priority_mask (0, ACE_Log_Msg::PROCESS);

ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC)service);

ACE_Thread_Manager::instance ()->spawn_n (3, (ACE_THR_FUNC)worker);

ACE_Thread_Manager::instance ()->wait();

return 0;

}

输出：

in service, MsgType:LM_INFO, ThreadID: 3428

in worker, MsgType:LM_DEBUG, ThreadID: 2064

in worker, MsgType:LM_DEBUG, ThreadID: 1240

in worker, MsgType:LM_DEBUG, ThreadID: 1800

ACE可以将输出重新定向到stderr（缺省）、系统日志、输出流（Output Stream）、甚至是回调函数，在C/S结构

中，ACE也可以实现客户端输出定向到服务器。本节将详细讨论这些内容。

三 ACE的重新定向设置或改变ACE日志输出目标（logging sink）可以通过ACE_Log_Msg的方法open来完成，另外可通过set_flags和

clr_flags来配合实现输出到多个目标。open方法的原型：

/**

* Initialize the ACE logging facility. Supplies the program name

* that is available to each logging message call. Default arguments

* set up logging to STDERR only.

*

* @param prog_name The name of the calling program.

* @param options_flags A bitwise-or of options flags used to set the

* initial behavior and logging sink(s). (see the

* enum above for the valid values).

* @param logger_key The name of ACE_FIFO rendezvous point where the

* local client logger daemon is listening for logging

* messages. Only meaningful if the LOGGER bit is

* set in the @a flags argument.

*/

int open (const ACE_TCHAR *prog_name,

u_long options_flags = ACE_Log_Msg::STDERR,

const ACE_TCHAR *logger_key = 0);

第一个参数prog_name表示程序名称，可以任意设置，可通过ACE_Log_Msg的program_name方法获取，另外

当第二个参数设置中有ACE_Log_Msg::VERBOSE时，会在输出信息前加上前缀，前缀中就包含该program_name。第二个参数可以是:

STDERR Write messages to STDERR

LOGGER Write messages to the local client logger daemon

OSTREAM Write messages to the assigned output stream

MSG_CALLBACK Write messages to the callback object

VERBOSE Prepends program name, timestamp, host name, process ID, and message priority

to each message

VERBOSE_LITE Prepends timestamp and message priority to each message

SILENT Do not print messages at all

SYSLOG Write messages to the system's event log

CUSTOM Write messages to the user-provided back end第三个参数logger key是针对第二个参数为LOGGER（C/S结构）时需要设置的，表示客户端程序端口值，比如：

ACE_DEFAULT_LOGGER_KEY。一旦调用了该方法，其后的输出将重新定位到第二个参数所指定的sink上。

set_flags、clr_flags的原型：// Enable the bits in the logger's options flags.

void set_flags (unsigned long f);// Disable the bits in the logger's options flags.

void clr_flags (unsigned long f);参数含义同open函数的第二个参数options_flags，其中set_flags调用可在现有输出目标基础上增加新的

输出目标（叠加），clr_flags则从中删除指定的输出目标。下面描述一下重定向到不同sink的例子。

1.输出到stderr这个简单：

ACE_LOG_MSG->open (argv[0], ACE_Log_Msg::STDERR);

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test\n")));或者：

ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test\n")));2.输出到System LoggerACE_LOG_MSG->open(argv[0], ACE_Log_Msg::SYSLOG, ACE_TEXT ("ACE log"));

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test\n")));注意重新定向到System Logger不能用set_flags、clr_flags来实现。Windows操作系统下，会输出到系统的

Event Log中，其中Event Source名称为Open函数第三个参数。Unix/Linux下会输出到syslog facility。另

外Windows下LM_STARTUP、LM_SHUTDOWN、LM_TRACE、LM_DEBUG、LM_INFO 会对应EVENTLOG_INFORMATION_TYPE；

LM_NOTICE、LM_WARNING会对应EVENTLOG_WARNING_TYPE、LM_ERROR、LM_CRITICAL、LM_ALERT、LM_EMERGENCY

会对应EVENTLOG_ERROR_TYPE。3.输出到Output Streams下例将日志输出到文件（ofstream）：#include < ace/Log_Msg.h >

#include < ace/streams.h >int ACE_TMAIN (int, ACE_TCHAR *argv[])

{

ACE_OSTREAM_TYPE *output = new ofstream ("test.txt");

ACE_LOG_MSG->msg_ostream (output, 1);

ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test\n"))); return 0;

}上例中首先需要通过msg_ostream来assign一个ACE OSTREAM，然后再指定log sink为ACE_Log_Msg::OSTREAM，

指定log sink既可用set_flags也可用open方法，注意上例中set_flags之后紧接着又用clr_flags屏蔽掉先前

指定的ACE_Log_Msg::STDERR（缺省sink），这样就不会同时输出到stderr了。看一下msg_ostream的注释（解释的很明白，不赘述）：/**

* delete_stream == 1, forces Log_Msg.h to delete the stream in

* its own ~dtor (assumes control of the stream)

* use only with proper ostream (eg: fstream), not (cout, cerr)

*/

void msg_ostream (ACE_OSTREAM_TYPE *, int delete_ostream);

上例中由于设置了delete_stream == 1，所以不应再显示地调用delete output;来删除ofstream。

全局性的ostream还包括cout、cerr、clog等，这里再举一个输出到cout的例子，其它情况类似： ACE_LOG_MSG->msg_ostream (&cout);

ACE_LOG_MSG->open (argv[0], ACE_Log_Msg::OSTREAM);

ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test\n")));4.输出到Callback在ACE中，将日志输出重新定向到回调函数其实很简单，只需要四步：（1）定义ACE_Log_Msg_Callback的派生类，实现纯虚函数virtual void log (ACE_Log_Record &log_record).（2）在ACE_Log_Msg中注册该派生类（比如 MyCallback）：

Callback *callback = new Callback;

ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);

ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);

ACE_LOG_MSG->msg_callback (callback);

（3）调用日志输出宏，比如ACE_DEBUG,就会将输出重新定向到MyCallback的log方法中。（4）退出前的清理工作：

ACE_LOG_MSG->clr_flags (ACE_Log_Msg::MSG_CALLBACK);

delete callback;注意到ACE_Log_Msg_Callback的log方法的参数类型ACE_Log_Record， 可以利用其print方法重新定制输出： int print (const ACE_TCHAR host_name[], u_long verbose_flag, FILE *fp);

或者：

int print (const ACE_TCHAR host_name[], u_long verbose_flag, ACE_OSTREAM_TYPE &stream);

第一个参数表示主机名，第二个参数可选ACE_Log_Msg::VERBOSE、VERBOSE_LITE或0（原始信息），

第三个参数可以是文件指针或者是标准输出流，比如： log_record.print (ACE_TEXT (""), ACE_Log_Msg::VERBOSE, cout); /* 输出到屏幕 */

log_record.print (ACE_TEXT (""), 0, ofstream("test.txt",ios::app)); /* 输出到文件 */关于ACE_Log_Record进一步使用，请参考以下例子：/* main.cpp */

#include < ace/Task.h >

#include < ace/Log_Msg.h >

#include " MyCallback.h "int ACE_TMAIN (int, ACE_TCHAR *[])

{

MyCallback *callback = new MyCallback; ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);

ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);

ACE_LOG_MSG->msg_callback (callback); ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test1\n")));

ACE_DEBUG ((LM_INFO, ACE_TEXT ("test2\n"))); ACE_LOG_MSG->clr_flags (ACE_Log_Msg::MSG_CALLBACK);

delete callback; return 0;

}

/* MyCallback.h */

#include < ace/streams.h >

#include < ace/Log_Msg.h >

#include < ace/Log_Msg_Callback.h >

#include < ace/Log_Record.h >

#include < ace/streams.h >

#include < ace/Log_Msg_Callback.h >

#include < ace/Log_Record.h >

#include < ace/SString.h >

#include < ace/OS.h >class MyCallback : public ACE_Log_Msg_Callback

{

public:

void log (ACE_Log_Record &log_record)

{

cerr << "Log Message Received:" << endl;

unsigned long msg_severity = log_record.type (); ACE_Log_Priority prio = ACE_static_cast (ACE_Log_Priority, msg_severity);

const ACE_TCHAR *prio_name = ACE_Log_Record::priority_name (prio);

cerr << "\tType: "

<< ACE_TEXT_ALWAYS_CHAR (prio_name)

<< endl; cerr << "\tLength: " << log_record.length () << endl; const time_t epoch = log_record.time_stamp ().sec ();

cerr << "\tTime_Stamp: "

<< ACE_TEXT_ALWAYS_CHAR (ACE_OS::ctime (&epoch))

<< flush; cerr << "\tPid: " << log_record.pid () << endl; ACE_CString data (">> ");

data += ACE_TEXT_ALWAYS_CHAR (log_record.msg_data ()); cerr << "\tMsgData: " << data.c_str () << endl;

}

};输出：

Log Message Received:

Type: LM_DEBUG

Length: 32

Time_Stamp: Mon Mar 27 17:03:06 2005

Pid: 2752

MsgData: >> test1Log Message Received:

Type: LM_INFO

Length: 32

Time_Stamp: Mon Mar 27 17:03:06 2005

Pid: 2752

MsgData: >> test2

需要提醒的是，由于ACE_Log_Msg是线程相关的，而且spawn出来的线程没有继承性，因此Callback仅对单个

线程有效，如果想实现多个线程的callback，需要分别在各个线程中调用：

ACE_LOG_MSG->msg_callback (callback);5.C/S结构中输出到服务器可用于分布式系统的日志处理（distributed logger），logging server运行在某台主机，并接收来自其它主机

的logging requests. 在其它主机（也可能在本机）需要运行一个logging client daemon进程，相当于一个

proxy，实现了你自己编写的程序与logging server的交互。自己编写的程序格式如下：#include < ace/Log_Msg.h >int ACE_TMAIN (int, ACE_TCHAR *argv[])

{

ACE_LOG_MSG->open (argv[0],

ACE_Log_Msg::LOGGER,

ACE_DEFAULT_LOGGER_KEY); ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sent to Logging Server\n"))); return 0;

}在运行你自己编写的程序之前需要：（1）首先运行logging server：

%ACE_ROOT%/netsvcs/servers/main -f server.conf（2）然后运行logging client daemon：

%ACE_ROOT%/netsvcs/servers/main -f client.conf其中server.conf内容如下：

dynamic Logger Service_Object * ACE:_make_ACE_Logging_Strategy() "-s log.txt -f STDERR|OSTREAM"

dynamic Server_Logging_Service Service_Object * netsvcs:_make_ACE_Server_Logging_Acceptor()

active "-p 20009"该文件每行以dynamic开始，第一行定义了logging strategy，将会把日志输出到log.txt（-s 参数），同时

输出到STDERR。第二行定义了logging server侦听（listen）端口为20009。client.conf内容如下：

dynamic Client_Logging_Service Service_Object * netsvcs:_make_ACE_Client_Logging_Acceptor()

active "-p 20009 -h localhost"该文件同样以dynamic开始，定义了logging client daemon的侦听端口为2009（与server.conf一致），同时

-h指定目标机器名称或ip（这里是本机）。运行了logging server和logging client daemon之后，编译运行上面那个程序， logging server会显示：

Sent to Logging Server同时在logging server所在目录（%ACE_ROOT%/netsvcs/servers/）下会生成log.txt,内容是：

starting up Logging Server at port 20009 on handle 1900

Sent to Logging Server注意到之所以如此麻烦地启动两级logging管理，主要是考虑到分布式处理过程中可能出现的瓶颈问题。当然也可以直接通过ACE_SOCK_Stream实现与logging server的交互，而不用logging client daemon，细节

以后讨论。而利用server.conf和client.conf来配置分布式logging服务涉及到了ACE的Runtime Configuration，

细节同样在以后会讨论到。对其中参数的解释列举如下：-f Specify ACE_Log_Msg flags (OSTREAM, STDERR, LOGGER, VERBOSE, SILENT, VERBOSE_LITE) used to

control logging.

-i The interval, in seconds, at which the log file size is sampled (default is 0; do not sample

by default).

-k Specify the rendezvous point for the client logger（比如ACE_DEFAULT_LOGGER_KEY）

-m The maximum log file size in Kbytes.

-n Set the program name for the %n format specifier.

-N The maximum number of log files to create.

-o Request the standard log file ordering (keeps multiple log files in numbered order). Default

is not to order log files.

-p Pass in the processwide priorities to either enable (DEBUG, INFO, WARNING, NOTICE, ERROR,

CRITICAL, ALERT, EMERGENCY) or to disable (~DEBUG, ~INFO, ~WARNING, ~NOTICE, ~ERROR, ~CRITICAL,

~ALERT, ~EMERGENCY).

-s Specify the file name used when OSTREAM is specified as an output target.

-t Pass in the per instance priorities to either enable (DEBUG, INFO, WARNING, NOTICE, ERROR,

CRITICAL, ALERT, EMERGENCY) or to disable (~DEBUG, ~INFO, ~WARNING, ~NOTICE, ~ERROR, ~CRITICAL,

~ALERT, ~EMERGENCY).

-w Cause the log file to be wiped out on both start-up and reconfiguration.

