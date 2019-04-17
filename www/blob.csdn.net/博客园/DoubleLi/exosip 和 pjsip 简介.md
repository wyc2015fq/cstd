# exosip 和 pjsip 简介 - DoubleLi - 博客园






** oSIP**

 oSIP的开发开始于2000年7月，第一个版本在2001年5月发 布，到现在已经发展到3.x了。它采用ANSI C编写，而且结 构简单小巧，所以速度特别快，它并不提供高层的SIP会话 控制API,它主要提供一些解析SIP/SDP消息的API和事务处理 的状态机，oSIP的作者还开发了基于oSIP的UA lib:exosip和 proxy server lib:partysip. oSIP支持的功能；



exosip针对UA是对osip进行扩展,oSIP不提供任何快速产生请求消息和响应消息的方法，所有请求消息和响应消息的形成必须调用一组sip message api来手动组装完成,所以作者在osip上基础上开发了exosip,用exosip开发软电话非常方便,仅需几个API就可以完成.exosip中附带一个例子:josua,不过josua相对复杂了点,下面给出一个最简单的例子供大家参考,因为例子实在太简单所以没有给出注释,用到exosip的API的参数请参看exosip源代码,看懂这个例子再研究josua就很简单了.我使用的是osip 2.0.9+exosip 0.77.

#include "assert.h"
#include <conio.h>
#include <iostream>
#include <osip2/osip_mt.h>
#include <eXosip/eXosip.h>
#include <eXosip/eXosip_cfg.h>

using namespace std;

class jcall;

class jcall {
public:
 int cid;
 int did;

 char reason_phrase[50];
 int  status_code;

 char textinfo[256];
 char req_uri[256];
 char local_uri[256];
 char remote_uri[256];
 char subject[256];

 char remote_sdp_audio_ip[50];
 int  remote_sdp_audio_port;
 int  payload;
 char payload_name[50];

 int state;

 jcall() {}

 int build(eXosip_event_t *je)
 {
  jcall *ca = this;

  ca->cid = je->cid;
  ca->did = je->did;

  if (ca->did<1 && ca->cid<1)
  {
   assert(0);
   return -1; /* not enough information for this event?? */
  }

  osip_strncpy(ca->textinfo,   je->textinfo, 255);
  osip_strncpy(ca->req_uri,    je->req_uri, 255);
  osip_strncpy(ca->local_uri,  je->local_uri, 255);
  osip_strncpy(ca->remote_uri, je->remote_uri, 255);
  osip_strncpy(ca->subject,    je->subject, 255);

  if (ca->remote_sdp_audio_ip[0]=='/0')
  {
   osip_strncpy(ca->remote_sdp_audio_ip, je->remote_sdp_audio_ip, 49);
   ca->remote_sdp_audio_port = je->remote_sdp_audio_port;
   ca->payload = je->payload;
   osip_strncpy(ca->payload_name, je->payload_name, 49);

  }

  if (je->reason_phrase[0]!='/0')
  {
   osip_strncpy(ca->reason_phrase, je->reason_phrase, 49);
   ca->status_code = je->status_code;
  }

  ca->state = je->type;
  return 0;
 }

};


jcall call;

void __exit( int r )
{
 char line[256];
 gets( line );
 exit( r );
}

void josua_printf(char* buf)
{
 printf( "/n" );
}


int josua_event_get()
{
 int counter =0;
 /* use events to print some info */
 eXosip_event_t *je;
 for (;;)
 {
  char buf[100];
  je = eXosip_event_wait(0,50);
  if (je==NULL)
   break;
  counter++;
  if (je->type==EXOSIP_CALL_NEW)
  {
   printf( "<- (%i %i) INVITE from: %s",
    je->cid, je->did,
    je->remote_uri);
   josua_printf(buf);

   call.build(je);
  }
  else if (je->type==EXOSIP_CALL_ANSWERED)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_CALL_PROCEEDING)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_CALL_RINGING)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_CALL_REDIRECTED)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_CALL_REQUESTFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_CALL_SERVERFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_CALL_GLOBALFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_CALL_CLOSED)
  {
   printf( "<- (%i %i) BYE from: %s",
    je->cid, je->did, je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_CALL_HOLD)
  {
   printf( "<- (%i %i) INVITE (On Hold) from: %s",
    je->cid, je->did, je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_CALL_OFFHOLD)
  {
   printf( "<- (%i %i) INVITE (Off Hold) from: %s",
    je->cid, je->did, je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_REGISTRATION_SUCCESS)
  {
   printf( "<- (%i) [%i %s] %s for REGISTER %s",
    je->rid,
    je->status_code,
    je->reason_phrase,
    je->remote_uri,
    je->req_uri);
   josua_printf(buf);

  }
  else if (je->type==EXOSIP_REGISTRATION_FAILURE)
  {
   printf( "<- (%i) [%i %s] %s for REGISTER %s",
    je->rid,
    je->status_code,
    je->reason_phrase,
    je->remote_uri,
    je->req_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_OPTIONS_NEW)
  {
   printf( "<- (%i %i) OPTIONS from: %s",
    je->cid, je->did,
    je->remote_uri);
   josua_printf(buf);

  }
  else if (je->type==EXOSIP_OPTIONS_ANSWERED)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_OPTIONS_PROCEEDING)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);

  }
  else if (je->type==EXOSIP_OPTIONS_REDIRECTED)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_OPTIONS_REQUESTFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_OPTIONS_SERVERFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_OPTIONS_GLOBALFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_INFO_NEW)
  {
   printf( "<- (%i %i) INFO from: %s",
    je->cid, je->did,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_INFO_ANSWERED)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_INFO_PROCEEDING)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_INFO_REDIRECTED)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_INFO_REQUESTFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_INFO_SERVERFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_INFO_GLOBALFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s",
    je->cid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }

  else if (je->type==EXOSIP_SUBSCRIPTION_ANSWERED)
  {
   printf( "<- (%i %i) [%i %s] %s for SUBSCRIBE",
    je->sid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);

   printf( "<- (%i %i) online=%i [status: %i reason:%i]",
    je->sid, je->did, 
    je->online_status,
    je->ss_status,
    je->ss_reason);
   josua_printf(buf);

  }
  else if (je->type==EXOSIP_SUBSCRIPTION_PROCEEDING)
  {
   printf( "<- (%i %i) [%i %s] %s for SUBSCRIBE",
    je->sid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);

  }
  else if (je->type==EXOSIP_SUBSCRIPTION_REDIRECTED)
  {
   printf( "<- (%i %i) [%i %s] %s for SUBSCRIBE",
    je->sid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_SUBSCRIPTION_REQUESTFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s for SUBSCRIBE",
    je->sid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_SUBSCRIPTION_SERVERFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s for SUBSCRIBE",
    je->sid, je->did, 
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_SUBSCRIPTION_GLOBALFAILURE)
  {
   printf( "<- (%i %i) [%i %s] %s for SUBSCRIBE",
    je->sid, je->did,
    je->status_code,
    je->reason_phrase,
    je->remote_uri);
   josua_printf(buf);
  }
  else if (je->type==EXOSIP_SUBSCRIPTION_NOTIFY)
  {
   printf( "<- (%i %i) NOTIFY from: %s",
    je->sid, je->did,
    je->remote_uri);
   josua_printf(buf);

   printf( "<- (%i %i) online=%i [status: %i reason:%i]",
    je->sid, je->did, 
    je->online_status,
    je->ss_status,
    je->ss_reason);
   josua_printf(buf);

  }
  else if (je->type==EXOSIP_IN_SUBSCRIPTION_NEW)
  {
   printf( "<- (%i %i) SUBSCRIBE from: %s",
    je->nid, je->did,
    je->remote_uri);
   josua_printf(buf);

   /* search for the user to see if he has been
   previously accepted or not! */

   eXosip_notify(je->did, EXOSIP_SUBCRSTATE_PENDING, EXOSIP_NOTIFY_AWAY);
  }
  else if (je->textinfo[0]!='/0')
  {
   printf( "(%i %i %i %i) %s", je->cid, je->sid, je->nid, je->did, je->textinfo);
   josua_printf(buf);
  }


  eXosip_event_free(je);
    }
 if (counter>0)
  return 0;
 return -1;
}

int main(int argc, char* argv[])
{
 int i;
 memset( &call, 0, sizeof(call) );

 cout << "Usage:"<< endl;
 cout << "a - answering call"<< endl;
 cout << "h - hangup"<< endl;
 cout << "r - ringing"<< endl;
 cout << "c - call"<< endl;
 cout << "q - quit"<< endl;

 FILE* logfile = fopen( "logfile.txt", "w");
 // osip_trace_initialize( (_trace_level)0, logfile );
 // osip_trace_initialize( (_trace_level)8, stdout );
 i = eXosip_init(stdin, stdout, 5060);
 if (i!=0)
    {
  fprintf (stderr, "test: could not initialize eXosip/n");
  __exit(0);

    }


 eXosip_sdp_negotiation_remove_audio_payloads();
 eXosip_sdp_negotiation_add_codec(osip_strdup("0"),
  NULL,
  osip_strdup("RTP/AVP"),
  NULL, NULL, NULL,
  NULL,NULL,
  osip_strdup("0 PCMU/8000"));

 eXosip_sdp_negotiation_add_codec(osip_strdup("8"),
  NULL,
  osip_strdup("RTP/AVP"),
  NULL, NULL, NULL,
  NULL,NULL,
  osip_strdup("8 PCMA/8000"));

 /* register callbacks? */
 eXosip_set_mode(EVENT_MODE);

 osip_message_t *invite;
 i = eXosip_build_initial_invite(&invite,
  "sip:192.168.197.5:5060",       //被叫对IP
  "sip:192.168.197.254:5060",     //自己IP
  NULL,
  "oSIP phone");
 if (i!=0)
 {
  fprintf (stderr, "eXosip_build_initial_invite failed/n");
  __exit(0);
 }


 cout << "oSIP>";
 bool run = true;
 while( run )
 {
  josua_event_get();

  if ( _kbhit() )
  {
   switch ( _getch() )
   {
   case 'a':
    cout << "answer"<< endl;
    eXosip_lock();
    eXosip_answer_call(call.did, 200, 0);
    eXosip_unlock();
    break;

   case 'h':
    cout << "hangup"<< endl;
    eXosip_lock();
    eXosip_terminate_call( call.cid, call.did );
    eXosip_unlock();
    break;

   case 'r':
    cout << "ringing"<< endl;
    eXosip_lock();
    eXosip_answer_call(call.did, 180, 0);
    eXosip_unlock();
    break;

   case 'c':
    cout << "call"<< endl;
    eXosip_lock();
    eXosip_initiate_call(invite, NULL, NULL, "10500");
    eXosip_unlock();
    break;

   case 'q':
    cout << "quit"<< endl;
    run = false;
    break;

   default :
    cout << "/noSIP>";
   }
  }
 }


 return 0;
}















**pjsip**



如果你对SIP/VoIP技术感兴趣,哪希望你不要错过:),如果你对写出堪称优美的Code感兴趣,那么你也不可错过:)这期间我想分析一下一个实际的[协议](http://www.rosoo.net/a/list_46_1.html)栈的设计到实现的相关技术,算是自己的一个学习经历记录.最初选择这个库做分析的原因很简单,文档齐全:),其它良好的特征则是慢慢发现的:)　www.pjsip.org

1.    PJSIP简介

PJSIP的实现是为了能在[嵌入式](http://www.rosoo.net/a/embedded/)设备上高效实现SIP/VOIP.其主要特征包括:

    1).极具移植性.(Extremely portable)

        当前可支持平台包括:

            * Win32/x86 (Win95/98/ME, NT/2000/XP/2003, mingw).

                * arm, WinCE and [Windows Mobile](http://www.rosoo.net/a/list_35_1.html).

                * [Linux](http://www.rosoo.net/a/list_49_1.html)/x86, (user mode and as kernel module(!)).

                * Linux/alpha

                * Solaris/ultra.

                * MacOS X/powerpc

                * RTEMS (x86 and powerpc).

        正移植到:

                * [Symbian](http://www.rosoo.net/a/list_36_1.html) OS

    2).非常小的足印.(Very small footprint)

        官方宣称编译后的库<150Kb,我在PC上编译后加上strip后大概173Kb,这对于嵌入式设备,是个好消息:)

    3).高性能.(High performance)

        这点我们后面可以看看是否如作者宣称的:)

    4).支持众多的特征.(Many features)

        这点可以从http://www.pjsip.org/sip_media_features.htm#sip_features看出.

    5).充足的SIP文档.(Extensive SIP documentation)

        这是我最初选择该库的原因,当然不是最终的原因,最终的原因是它的code:)

2.    PJSIP的组成.

    其实说是PJSIP不是特别贴切,这个库实际上是几个部分组成的.

    1).PJSIP - Open Source SIP Stack[开源的SIP协议栈]

    2).PJMEDIA - Open Source Media Stack[开源的媒体栈]

    3).PJNATH - Open Source NAT Traversal Helper Library[开源的NAT-T辅助库]

    4).PJLIB-UTIL - Auxiliary Library[辅助工具库]

    5).PJLIB - Ultra Portable Base Framework Library[基础框架库]

    而在最上层库的目录分为:(可以使用tree -d -L 1 查看)

    $TOP/build        [包含Makefile]

    $TOP/build.symbian    [针对symbian的Makefile]

    $TOP/pjlib        [参考上面]

    $TOP/pjlib-util        [参考上面]

    $TOP/pjnath        [参考上面]

    $TOP/pjmedia        [参考上面]

    $TOP/pjsip        [参考上面]

    $TOP/pjsip-apps

    $TOP/third_party

    而在每个子目录,可以看到分为:

    bin                    [编译后产生的二进制文件]

    build                    [Makefile]

        build/output

        build/wince-evc4

    docs                    [doxygen的文档,用doxygen docs/doxygen.cfg产生]

    include                    [头文件]

    lib                    [编译后产生的库]

    src                    [源代码]

3.    PJLIB简介 
    要理解好PJSIP，就不得不先说说PJLIB，PJLIB算的上是这个库中最基础的库，正是这个库的优美实现，才让PJSIP变得如此优越。

    PJLIB提供了一系列特征，这是我们下面分析的重点，涉及到：

    1).非动态内存分配[No Dynamic Memory Allocations]

        实现了内存池，获取内存是从与分配的内存池中获取，高性能程序多会自己构造内存池，后面我们会解释该内存池的使用以及基本的原理。根据作者的比较，是常规的 malloc()/free()函数的30倍。

    2).OS抽象[Operating System Abstraction]

        实现OS抽象的根本原因在与可移植性，毋庸置疑:).

        涉及到：

        a).线程[Threads.]

        b).线程本地存储[Thread Local Storage.]

        c).互斥[Mutexes.]

        d).信号灯[Semaphores.]

        e).原子变量[Atomic Variables.]

        f).临届区[Critical sections.]

        g).锁对象[Lock Objects.]

        h).事件对象[Event Object.]

        i).时间管理[Time Data Type and Manipulation.]

        j).高解析的时间戳[High Resolution Timestamp.]

        等等，这些我们后面分析代码时一一看来

    3).低层的网络相关IO[Low-Level Network I/O]

        这涉及到：

        a).Socket抽象[Socket Abstraction.]

        b).网络地址解析[Network Address Resolution.]

        c).实现针对Socket的select API[Socket select() API.]

    4).时间管理[Timer Management]

        这主要涉及到两个部分，一个时定时器的管理，还有就是时间解析的精度(举例说来，就是能精确到哪个时间等级，比如 POSIX sleep(),就只能以秒为单位，而使用select()则可以实现毫秒级别的计时)

    5).各种数据结构[Various Data Structures]

        主要有:

        a).针对字符串的操作[String Operations]

        b).数组辅助[Array helper]

        c).Hash表[Hash Tabl]

        d).链表[Linked List]

        e).红黑平衡树[Red/Black Balanced Tree]

    6).异常处理[Exception Construct]

        使用的是TRY/CATCH,知道[C++](http://www.rosoo.net/a/list_100_1.html)/JAVA之类面向对象语言的人看过会宛而一笑:)

    7).LOG机制[Logging Facility]

        很显然，一个良好的程序，好的LOG机制不可少。这能很方便的让你去调试程序，对此我是深有体会，任何时候，不要忘记“好的程序，是架构出来的；而能跑的程序，是调试出来的:)”

    8).随机数以及GUID的产生[Random and GUID Generation]

        GUID指的是"globally unique identifier"，只是一个标识而已，比如说你的省份证，算的上是一个GUID，当然，准确说来是“china unique identifier”:). 看了这么多的特征列举，是不是很完备，的确。总算是初步列举完了PJLIB的基本特征了，后面我们来说说它的使用与实现:

4.    PJLIB的使用

    有了上述介绍，是不是很想知道这个库的使用，没关系，我们慢慢说来:)

    首先是头文件和编译出来的库的位置，这就不必多说了，除非你没有使用过手动编译的库，如果不太了解步骤，google一下，啊:)

    1).为了使用这个库，需要使用：

    #include <pjlib.h>

    当然，也可以选择：

    #include <pj/log.h>

    #include <pj/os.h>

    这种分离的方式，不过，简介其间，还是使用第一种吧:),毕竟，你不需要确认到你所需的函数或者数据结构具体到哪个具体的头文件:)

    2).确保在使用PJLIB之前调用 pj_init()来完成PJLIB库使用前说必须的一些初始化.

    这是一个必不可少的步骤.

    ~~~~~~~~~~~~~~~~~~~~~~

    3).使用PJLIB的一些建议

    作者对使用PJLIB的程序提出了一些建议，包括如下 ：

        a).不要使用ANSI C[Do NOT Use ANSI C]

        观点很明确，ANSI C并不会让程序具有最大的移植性，应该使用PJSIP库所提供的响应机制来实现你所需要的功能.

        b).使用pj_str_t取代C风格的字符串[Use pj_str_t instead of C Strings]

        原因之一是移植性，之二则是PJLIB内置的pj_str_t相关操作会更快(性能).        

        c).从内存池分配内存[Use Pool for Memory Allocations]

        这很明显，如果你知道为什么会使用内存池的话(提示一下，性能以及易用性:))

        d).使用PJLIB的LOG机制做文字显示[Use Logging for Text Display]

        很明显:)

     还有些关于移植的一些问题，不在我们的讨论范围，如果你需要移植到其它平台或者环境，请参[http://www.pjsip.org/pjlib/docs/html/porting_pjlib_pg.htm](http://www.pjsip.org/pjlib/docs/html/porting_pjlib_pg.htm)

5.    PJLIB的使用以及原理

    终于开始提及实现原理以及具体的编码了:),前面的列举还真是个琐碎的事情,还是奔主题来:).

    5.1快速内存池[Fast Memory Pool]

    前面说过,使用内存池的原因在于性能的考虑,原因是C风格的malloc()以及C++风格的new操作在高性能或实时条件下表现并不太好,原因在于性能的瓶颈在于内存碎片问题

    下面列举其优点与需要主要的问题:

    优点：

    a).不像其它内存池,允许分配不同尺寸的chunks.

    b).快速.

        内存chunks拥有O(1)的复杂度,并且操作仅仅是指针的算术运算,其间不需要使用锁住任何互斥量.

    c).有效使用内存.

        除了可能因为内存对齐的原因会浪费很少的内存外,内存的使用效率非常高.

    d).可预防内存泄漏.

        在C/C++程序中如果出现内存泄漏问题,其查找过程哪个艰辛,不足为外人道也:(

        [曾经有次用别人的Code,出现了内存泄漏,在开发板上查找N天,又没工具可在开发板上使用,哪个痛苦,想自杀:(]

        原因很简单,你的内存都是从内存池中获取的,就算你没有释放你获取的内存,只要你记得把内存池destroy,那么内存还是会还给系统.

    还有设计带来的一些其它益处,比如可用性和灵活性:

    e).内存泄漏更容易被跟踪.

        这是因为你的内存是在指定的内存池中分配的,只要能很快定位到内存池,内存泄漏的侦测就方便多了.

    f).设计上从内存池中获取内存这一操作是非线程安全的.

        原因是设计者认为内存池被上层对象所拥有,线程安全应该由上层对象去保证,这样的话,没有锁的问题会让内存分配变得非常的快.

    g).内存池的行为像C++中的new的行为,当内存池获取内存chunks会抛出PJ_NO_MEMORY_EXCEPTION异常,当然,因为支持异常处理,也可以使用其它方式让上层程序灵活的定义异常的处理.

    [这是异常处理的基本出发点,但是这有大量的争论,原因是这改变了程序的正常流程,谁能去保证这种流程是用户所需要的呢,因此C++中的异常处理饱受争议,请酌情使用]

    h). 可以在后端使用任何的内存分配器.默认情况下是使用malloc/free管理内存池的块,但是应用程序也可以指定自己的策略(strategy),例如从一个全局存储空间分配内存.

    恩,要知道,任何事务都是两面的(颇为佩服创造出“双赢”这个词的语言天才, 不过，文字游戏对于技术人员不能说是件好事情:(),好了,使用时,不要认为这个内存池是哪种"perfect"的技术,要记得"任何设计,都是在各种限制条件中的一个折中,对于'戴着镣铐的舞蹈',除了'舞蹈',也不要忘记'镣铐'哦",不要忘了告诫:):

    告诫[Caveats]:

    a).使用合适的大小来初始化内存池.

        使用内存池时,需要指定一个初始内存池大小, 这个值是内存池的初始值,如果你想要高性能,要谨慎选择这个值哦,太大的化会浪费内存,过小又会让内存池自身频繁的去增加内存,显然这两种情况都不可取.

    b). 注意,内存池只能增加,而不能被缩小(shrink),因为内存池没有函数把内存chunks释放还给系统,这就要去内存池的构造者和使用者明确使用内存.

    恩,基本的原理都差不多了,后面我们来看看如何使用这个内存池.

    5.2内存池的使用[Using Memory Pool]

     内存池的使用相当的简单,扳个手指头就搞定了,如果你看明白了上面的原理和特征:)

     a).创建内存池工厂[Create Pool Factory]

     上面不是提及内存池的内部分配策略以及异常处理方式么, 其实这就是指定这个的:)

     当然,不需要你每个内存池都自己取指定策略和异常处理方式,PJLIB已经有了一个默认的实现:Caching Pool Factory,这个内存池工厂的初始化使用函数pj_caching_pool_init()    

     b).创建内存池[Create The Pool]

     使用pj_pool_create(),其参数分别为内存工厂(Pool Factory),内存池的名字(name),初始时的大小以及增长时的大小.

     c).根据需要分配内存[Allocate Memory as Required]

     然后,你就可以使用pj_pool_alloc(), pj_pool_calloc(), 或pj_pool_zalloc()从指定的内存池根据需要去获取内存了:)

     d).Destroy内存池[Destroy the Pool]

     这实际上是把预分配的内存还给系统    

     e).Destroy内存池工厂[Destroy the Pool Factory]

     这没什么好说的.

     很简单吧:)

     作者在文档中给出了一个例子:

     如下：


- #include <pjlib.h> 
- #define THIS_FILE    "pool_sample.c" 
- 
- static void my_perror(const char *title, pj_status_t status) 
-    { 
- char errmsg[PJ_ERR_MSG_SIZE]; 
- 
-         pj_strerror(status, errmsg, sizeof(errmsg)); 
-         PJ_LOG(1,(THIS_FILE, "%s: %s [status=%d]", title, errmsg, status)); 
-    } 
- 
- static void pool_demo_1(pj_pool_factory *pfactory) 
-    { 
-         unsigned i; 
-         pj_pool_t *pool; 
- 
- // Must create pool before we can allocate anything 
-         pool = pj_pool_create(pfactory,  // the factory 
- "pool1",   // pool's name 
-                               4000,      // initial size 
-                               4000,      // increment size 
-                               NULL);     // use default callback. 
- if (pool == NULL) { 
-             my_perror("Error creating pool", PJ_ENOMEM); 
- return; 
-         } 
- 
- // Demo: allocate some memory chunks 
- for (i=0; i<1000; ++i) { 
- void *p; 
- 
-             p = pj_pool_alloc(pool, (pj_rand()+1) % 512); 
- 
- // Do something with p 
-             ... 
- 
- // Look! No need to free p!! 
-         } 
- 
- // Done with silly demo, must free pool to release all memory. 
-         pj_pool_release(pool); 
-    } 
- 
- int main() 
-    { 
-         pj_caching_pool cp; 
-         pj_status_t status; 
- 
- // Must init PJLIB before anything else 
-         status = pj_init(); 
- if (status != PJ_SUCCESS) { 
-             my_perror("Error initializing PJLIB", status); 
- return 1; 
-         } 
- 
- // Create the pool factory, in this case, a caching pool, 
- // using default pool policy. 
-         pj_caching_pool_init(&cp, NULL, 1024*1024 ); 
- 
- // Do a demo 
-         pool_demo_1(&cp.factory); 
- 
- // Done with demos, destroy caching pool before exiting app. 
-         pj_caching_pool_destroy(&cp); 
- 
- return 0; 
-    } 

BTW:如果要实现嵌入式设备上的SIP电话或者其它，PJSIP是我所见的Coding和Design最为优秀的了，就算不是为了实现SIP协议栈，它的Coding方式，以及调试的接口，各种基础组件的定义，也让人深刻，可以当作一份优秀的源代码来阅读，就像小说一般:) 我就不解释了:)

 from:http://blog.csdn.net/chinabinlang/article/details/41043779









