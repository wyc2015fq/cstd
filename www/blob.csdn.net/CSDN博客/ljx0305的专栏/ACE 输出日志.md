# ACE 输出日志 - ljx0305的专栏 - CSDN博客
2011年05月04日 14:44:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1457
一个后台服务程序，良好的日志系统是不可缺少的。通过日志系统我们可以代替debug程序对服务程序进行调试。更重要的是我们可以运行状态下对服务程序的状态进行监控和跟踪，帮助我们查找修正运行过程中出现的一些不可预料的错误和异常。ACE提供了强大的对日志系统的支持，能够方便的为我们的应用程序建立良好的日志系统。 
基本的日志记录
•ACE_Log_Msg类使用了类似printf()的日志格式化输出格式使用起来非常方便。
•ACE为每个线程维护一个ACE_Log_Msg类的实例从而让每个线程都有自己的日志输出。
•ACE定义了若干个宏用来简化我们的使用(使用的时候只要包含Log_Msg.h即可)。
•ACE可以方便的重定向日志输出，我们可以把日志输出到我们指定的stream(流)或者linux/unix的syslog,NT EventLog等。
代码示例
基本的日志宏的使用(Simple1.cpp)
代码
// Simple1.cpp,v 1.2 2004/10/06 16:27:14 shuston Exp
#include "ace/Log_Msg.h"
void foo (void);
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TRACE("main");
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IHi Mom/n")));
  foo();
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IGoodnight/n")));
  return 0;
}
void foo (void)
{
  ACE_TRACE ("foo");
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IHowdy Pardner/n")));
}执行结果
ACE_TRACE无效的输出结果 
[root@ckasj Logging]# g++ -o Simple1 ../../../../examples/APG/Logging/Simple1.cpp -I/usr/local/include/ace  -lACE 
[root@ckasj Logging]# ./Simple1
Hi Mom
Howdy Pardner
Goodnight
ACE_TRACE有效的输出结果 
[root@ckasj](mailto:root@ckasj) Logging]# g++ -DACE_NTRACE=0  -o Simple1  ../../../../examples/APG/Logging/Simple1.cpp -I/usr/local/include/ace  -lACE 
[root@ckasj Logging]# ./Simple1
(3086952128) calling main in file `../../../../examples/APG/Logging/Simple1.cpp' on line 8
    Hi Mom
   (3086952128) calling foo in file `../../../../examples/APG/Logging/Simple1.cpp' on line 19
        Howdy Pardner
   (3086952128) leaving foo
    Goodnight
(3086952128) leaving main
注：日志宏是否输出日志信息又3个宏定义的值在编译的时候决定，通过设置ACE_NTRACE,ACE_NDEBUG,ACE_NLOGING的值为1或0来决定是否输出相应的信息，默认情况下编译ACE. ACE_TRACE 选项宏是被屏蔽的没有任何输出1)。如果需要默认情况下ACE_TRACE 生效你需要在编译的时候使用 -DACE_NTRACE选项 
控制调整日志输出的严重级别(Simple2.cpp)
代码 
// Simple2.cpp,v 1.2 2004/10/06 16:27:14 shuston Exp
#include "ace/Log_Msg.h"
void foo(void);
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TRACE ("main");
  //只使LM_DEBUG 和 LM_NOTICE级别的信息有效。
  //ACE_Log_Msg::PROCESS定义了日志设置应用于整个进程，可以选择
  //使用ACE_Log_Msg::THREAD只对本线程起作用
  ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_NOTICE,
                              ACE_Log_Msg::PROCESS);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IHi Mom/n")));
  foo ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%IGoodnight/n")));
  return 0;
}
void foo(void)
{
  ACE_TRACE ("foo");
  ACE_DEBUG ((LM_NOTICE, ACE_TEXT ("%IHowdy Pardner/n")));
}执行结果 
[root@ckasj Logging]# ./Simple2
Howdy Pardner
Goodnight
重定向日志输出
ACE的日志系统默认输出目标是系统的标准错误输出流，同时也可以输出到其他目标。如系统日志记录器（UNIX syslog ,NT Event Log）和我们指定的文件。你可以通过以下语句重定向ACE日志系统的输出目标： 
int ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  //open方法传入应用程序的名称，和目标输出流
  ACE_LOG_MSG->open (argv[0], ACE_Log_Msg::STDERR);
  ...
  ...
}
//或者通过set_flags改变目标输出流
ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%IHi Mom/n")));
ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);如果你通过set_flags()方法重定向日志输出，你可能在调用该方法之前调用一下clr_flags()禁止其他目的输出流。否则日志仍将输出到STDERR(标准错误输出流) 
大多数操作系统都支持系统日志记录器，从后台服务到系统函数调用都有。其思路是应用程序把他们的日志记录重定向到系统的日志记录器然后通过配置系统日志记录器把日志定向到适当的文件或其他目的日志系统。例如，UNIX系统管理员可以配置syslog，让不同类型不同级别的日志重定向到不同的目的地。这样可以提供更好的可伸缩性和可配置性。 
Use_Syslog.cpp
// Use_Syslog.cpp,v 1.2 2004/10/06 16:27:14 shuston Exp
#include "ace/Log_Msg.h"
void foo (void);
int ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  // This will be directed to stderr (the default ACE_Log_Msg
  // behavior).
  ACE_TRACE ("main");
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%IHi Mom/n")));
  // 重定向日志输出到syslog
  ACE_LOG_MSG->open
    (argv[0], ACE_Log_Msg::SYSLOG, ACE_TEXT ("syslogTest"));
  foo ();
  // 恢复到默认配置
  ACE_LOG_MSG->open (argv[0]);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IGoodnight/n")));
  return 0;
}
void foo (void)
{
  ACE_TRACE ("foo");
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IHowdy Pardner/n")));
}不同平台的输出结果
重定向日志到SYSLOG(系统日志记录器)不同的平台表现形式不同，在unix/linux下输出到syslog设置，而在windows nt,windows2000,xp系统输出到EventLog设置，如果系统不提供日志记录器意味着重定向到SYSLOG没有任何效果。 
Linux syslog 显示结果
[root@ckasj Logging]# ./Use_Syslog 
Hi Mom
Goodnight
[root@ckasj ~]# tail  /var/log/messages
Feb  5 20:49:16 ckasj syslogTest[17564]: Howdy Pardner
Feb  5 20:49:25 ckasj sshd(pam_unix)[17579]: session opened for user root by root(uid=0)
Feb  5 20:49:57 ckasj smbd[16958]: [2007/02/05 20:49:57, 0] lib/util_sock.c:read_socket_data(384) 
Feb  5 20:49:57 ckasj smbd[16958]:   read_socket_data: recv failure for 2751. Error = Connection timed out 
Feb  5 20:50:47 ckasj syslogTest[17614]: Howdy Pardner
WINDOWS Event Log 显示结果
D:/project/ACE_wrappers/examples/APG/Logging>Use_Syslog
Hi Mom
Goodnight
查看系统事件查看器 
控制参数参考列表
输出格式化
日志严重级别
常用日志宏
ACE_LOG_MSG常用控制标志
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/crearo/archive/2008/05/07/2412466.aspx](http://blog.csdn.net/crearo/archive/2008/05/07/2412466.aspx)
