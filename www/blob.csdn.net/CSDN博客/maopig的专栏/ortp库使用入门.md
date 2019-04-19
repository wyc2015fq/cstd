# ortp库使用入门 - maopig的专栏 - CSDN博客
2012年07月04日 18:19:04[maopig](https://me.csdn.net/maopig)阅读数：1055标签：[session																[signal																[buffer																[user																[null																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)
个人分类：[RTSP/RTP](https://blog.csdn.net/maopig/article/category/861027)
原创作品，允许转载，转载时请务必以超链接形式标明文章 [原始出处](http://ticktick.blog.51cto.com/823160/345642) 、作者信息和本声明。否则将追究法律责任。[http://ticktick.blog.51cto.com/823160/345642](http://ticktick.blog.51cto.com/823160/345642)
    我们知道， RTP（Real-timeTransportProtocol）是用于Internet上针对多媒体数据流的一种传输协议，做流媒体传输方面的应用离不开RTP协议的实现及使用，为了更加快速地在项目中应用RTP协议实现流媒体的传输，我们一般会选择使用一些RTP库，例如使用c++语言编写的JRTPLIB库，网上关于RTP协议以及JRTPLIB库的介绍已经很多了，在此我也不再赘述，文本主要介绍实现了RTP协议的另一种开源库——ORTP库，这个库是纯使用c语言编写，由于我们的项目是基于Linux下的c语言编程，故我们选择了ortp作为我们的第三方库，在此我也对该库进行一个简单地介绍，希望对其他ortp的初学者有所帮助。
# 一、简介
ORTP是一个支持RTP以及RFC3550协议的库，有如下的特性：
（1）使用C语言编写，可以工作于windows, Linux, 以及 Unix平台
（2）实现了RFC3550协议，提供简单易用的API。支持多种配置，RFC3551为默认的配置。
（3）支持单线程下的多个RTP会话，支持自适应抖动处理。
（4）基于GPL版权声明。
ORTP可以在其官方网站上（[http://www.linphone.org/index.php/eng/code_review/ortp](http://www.linphone.org/index.php/eng/code_review/ortp)）下载，下载解压后得到ORTP的源码包和示例程序（tests）。其帮助文档在docs目录下，也可以在[http://download.savannah.gnu.org/releases/linphone/ortp/docs/](http://mirror.veriportal.com/savannah/linphone/ortp/docs/)在线查看。
关于ORTP的资料并不多，主要是其源码、帮助文档以及示例程序，关于示例程序说明如下：
rtprecv.c 和rtpsend.c 展示了如何接收和发送单RTP数据流。
mrtprecv.c mrtpsend.c 展示了如何同时接收和发送多个RTP数据流。
# 二、主要函数介绍
**rtp_session_init**
函数原型：**void rtp_session_init (RtpSession * session, int mode)**
函数功能：执行rtp会话的一些必要的初始化工作
参数含义：
**session**： rtp会话结构体，含有一些rtp会话的基本信息
**mode**：传输模式，有以下几种，决定本会话的一些特性。
RTP_SESSION_RECVONLY：只进行rtp数据的接收
RTP_SESSION_SENDONLY：只进行rtp数据的发送
RTP_SESSION_SENDRECV：可以进行rtp数据的接收和发送
执行的操作：
1.设置rtp包缓冲队列的最大长度
2.根据传输模式设置标志变量的值
3.随机产生SSRC和同步源描述信息
4.传入全局的av_profile，即使用默认的profile配置
5.初始化rtp包缓冲区队列
6.发送负载类型默认设置为0（pcmu音频），接收负载类型默认设置为-1（未定义）
7.将**session**的其他成员的值均设置一个默认值。
**rtp_session_set_scheduling_mode**
函数原型：**void rtp_session_set_scheduling_mode (RtpSession * session, int yesno)**
函数功能： RtpScheduler管理多个session的调度和收发的控制，本函数设置是否使用该session调度管理功能。
参数含义：
**session： **rtp会话结构体
**yesno:**是否使用rtp session的系统调度功能
说明：
如果yesno为1，则表明使用系统的session调度管理功能，意味着可以使用以下功能：
1. 可以使用session_set_select在多个rtp会话之间进行选择，根据时间戳判定某个会话是否到达了收发的时间。
2. 可以使用rtp_session_set_blocking_mode()设置是否使用阻塞模式来进行rtp包的发送和接收。
如果yesno为0，则表明该会话不受系统管理和调度。
关于rtp session的管理和调度，由全局的变量RtpScheduler *__ortp_scheduler来负责，该变量必须通过ortp_scheduler_init()
来进行初始化操作。
**rtp_session_set_blocking_mode**
函数原型：**void rtp_session_set_blocking_mode (RtpSession * session, int yesno)**
函数功能：设置是否使用阻塞模式，
参数含义：
**session： **rtp会话结构体
**yesno: ** 是否使用阻塞模式
说明：
阻塞模式只有在scheduling mode被开启的情况下才能使用，本函数决定了rtp_session_recv_with_ts() 和 rtp_session_send_with_ts()两个函数的行为，如果启用了阻塞模式，则rtp_session_recv_with_ts()会一直阻塞直到接收RTP包的时间点到达（这个时间点由该函数参数中所定义的时间戳来决定），当接收完RTP数据包后，该函数才会返回。同样，rtp_session_send_with_ts()也会一直阻塞直到需要被发送的RTP包的时间点到达，发送结束后，函数才返回。
**rtp_session_signal_connect**
函数原型：**int rtp_session_signal_connect (RtpSession * session, const char *signal,   RtpCallback cb, unsigned long user_data)**
函数功能：本函数提供一种方式，用于通知应用程序各种可能发生的RTP事件（信号）。可能通过注册回调函数的形式来实现本功能。
参数含义：
**session：** rtp会话结构体
**signal： **信号的名称
**cb：**回调函数
**user_data：**传递给回调函数的数据
返回值：0表示成功，-EOPNOTSUPP表示信号名称不存在，-1表示回调函数绑定错误
说明：
信号的名称必须是以下字符串中的一种：
"ssrc_changed" : 数据流的同步源标识改变
"payload_type_changed" : 数据流的负载类型改变
"telephone-event_packet" : telephone-event RTP包(RFC2833)被接收
"telephone-event" : telephone event 发生
"network_error" : 网络错误产生，传递给回调函数的是描述错误的字符串（const char *型）或者错误码（int型）
"timestamp_jump" : 接收到的数据包发生了时间戳的跳跃。
要取消事件（信号）的监听，可以使用下面这个函数
int rtp_session_signal_disconnect_by_callback ( RtpSession * session, const char * signal_name, RtpCallback cb )
**rtp_session_set_local_addr**
函数原型：**int rtp_session_set_local_addr( RtpSession * session, const char * addr,int port)**
函数功能：设置本地rtp数据监听地址
参数含义：
**session：** rtp会话结构体
**addr： **本地IP地址，例如127.0.0.1，如果为NULL，则系统分配0.0.0.0
**port：**监听端口，如果设置为-1，则系统为其自动分配端口
返回值： 0表示成功
说明：
如果是RTP_SESSION_SENDONLY（只发送）型会话，则不需要进行本设置，而必须设置rtp_session_set_remote_addr()
来设置远程目的地址。
如果采用了系统自动分配监听端口，则可以通过int rtp_session_get_local_port(const RtpSession *session)
来获取系统分配的监听端口号。
**rtp_session_set_remote_addr**
函数原型：**int rtp_session_set_remote_addr (RtpSession * session, const char * addr, int port)**
函数功能：设置RTP发送的目的地址
参数含义：
**session：** rtp会话结构体
**addr： **目的IP地址
**port：**目的地址的监听端口号
返回值： 0表示成功
**rtp_session_set_send_payload_type**
函数原型：**int rtp_session_set_send_payload_type (RtpSession * session, int paytype)**
函数功能：设置RTP发送数据的负载类型
参数含义：
**session：** rtp会话结构体
**paytype：**负载类型
返回值： 0表示成功，-1表示负载未定义
说明：
负载类型在payloadtype.h文件中有详细的定义，RTP接收端有着类似的负载类型设置函数，int rtp_session_set_recv_payload_type ( RtpSession * session, int paytype )
，注意，发送的负载类型必须与接收的负载类型一致才能正常完成收发。
**rtp_session_send_with_ts**
函数原型：**int rtp_session_send_with_ts (RtpSession * session, const char * buffer, int len,uint32_t userts)**
函数功能：发送RTP数据包
参数含义：
**session：** rtp会话结构体
**buffer： **需要发送的RTP数据的缓冲区
**len：**需要发送的RTP数据的长度
**userts：** 本RTP数据包的时间戳
返回值： 成功发送到网络中的字节数
说明：
发送RTP数据需要自己管理时间戳的递增，每调用一次本函数，请根据实际情况对userts进行递增，具体递增的规则见RTP协议中的说明。
例如：如果发送的是采样率为90000Hz的视频数据包，每秒25帧，则时间戳的增量为：90000/25 = 3600
时间戳的起始值为随机值，建议设置为0 。
**rtp_session_recv_with_ts**
函数原型：int rtp_session_recv_with_ts (RtpSession * session, char * buffer,int len, uint32_t time, int * have_more)
函数功能：接收RTP数据包
参数含义：
**session：** rtp会话结构体
**buffer： **存放接收的RTP数据的缓冲区
**len：**期望接收的RTP数据的长度
**time：**期望接收的RTP数据的时间戳
**have_more：**标识接收缓冲区是否还有数据没有传递完。当用户给出的缓冲区不够大时，为了标识缓冲区数据未取完，则have_more指向的数据为1，期望用户以同样的时间戳再次调用本函数；否则为0，标识取完。
**rtp_session_destroy**
【原型】： void rtp_session_destroy(RtpSession *session)
【功能】：摧毁rtp会话对象，释放资源
【参数】：session已经创建的RTP会话对象
# 三、程序示例
      下面，我简单地通过程序演示了怎么使用ortp进行rtp数据包的发送，接收端的程序待以后有时间再整理出来吧。
       注：示例代码我已经整理出来了，见博文： [《ortp编程示例代码》](http://ticktick.blog.51cto.com/823160/704891)
- //////////////////////////////////////////////////////////////////////////    
- /// COPYRIGHT NOTICE    
- // Copyright (c) 2009, 华中科技大学ticktick Group    
- /// All rights reserved.     
- ///     
- /// @file    ortpSend.c      
- /// @brief   ortpSend的测试    
- ///    
- /// 本文件示例使用ortp库进行rtp数据包的发送   
- ///     
- /// @version 1.0       
- /// @author  tickTick   
- /// @date    2010/07/07     
- /// @E-mail  lujun.hust@gmail.com    
- ///    
- /// 修订说明：创建文件    
- //////////////////////////////////////////////////////////////////////////     
- 
- #include <ortp/ortp.h> 
- #include <signal.h> 
- #include <stdlib.h> 
- 
- #ifndef _WIN32  
- #include <sys/types.h> 
- #include <sys/time.h> 
- #include <stdio.h> 
- #endif 
- 
- // 时间戳增量
- #define TIME_STAMP_INC  160  
- #define BYTES_PER_COUNT 65535
- // 时间戳  
- uint32_t g_user_ts;  
- 
- /**  初始化    
-  *      
-  *   主要用于对ortp以及其它参数进行初始化   
-  *   @param:  char * ipStr 目的端IP地址描述串    
-  *   @param:  iint port 目的端RTP监听端口    
-  *   @return:  RtpSession * 返回指向RtpSession对象的指针,如果为NULL，则初始化失败    
-  *   @note:      
-  */
- RtpSession * rtpInit(char * ipStr,int port)  
- {  
- // Rtp会话对象   
-     RtpSession *session;  
- char *ssrc;  
- 
- // 时间戳初始化   
-     g_user_ts = 0;  
- 
- // ortp的一些基本初始化操作  
-     ortp_init();  
-     ortp_scheduler_init();  
- // 创建新的rtp会话对象 
-     session=rtp_session_new(RTP_SESSION_SENDONLY);    
- 
-     rtp_session_set_scheduling_mode(session,1);  
-     rtp_session_set_blocking_mode(session,1);  
- // 设置远程RTP客户端的的IP和监听端口（即本rtp数据包的发送目的地址）  
-     rtp_session_set_remote_addr(session,ipStr,port);  
- // 设置负载类型   
-     rtp_session_set_payload_type(session,0);  
- 
- // 获取同步源标识   
-     ssrc=getenv("SSRC");  
- if (ssrc!=NULL)   
-     {  
-         printf("using SSRC=%i.\n",atoi(ssrc));  
-         rtp_session_set_ssrc(session,atoi(ssrc));  
-     }  
- 
- return session;  
- 
- }  
- 
- /**  发送rtp数据包    
-  *      
-  *   主要用于发送rtp数据包    
-  *   @param:  RtpSession *session RTP会话对象的指针    
-  *   @param:  const char *buffer 要发送的数据的缓冲区地址     
-  *   @param: int len 要发送的数据长度    
-  *   @return:  int 实际发送的数据包数目    
-  *   @note:     如果要发送的数据包长度大于BYTES_PER_COUNT，本函数内部会进行分包处理    
-  */
- int rtpSend(RtpSession *session,constchar *buffer, int len)  
- {  
- int curOffset = 0;  
- int sendBytes = 0;  
- int clockslide=500;   
- // 发送包的个数 
- int sendCount = 0;  
- 
-     ortp_message("send data len %i\n ",len);  
- 
- // 是否全部发送完毕   
- while(curOffset < len )  
-     {  
- // 如果需要发送的数据长度小于等于阙值，则直接发送 
- if( len <= BYTES_PER_COUNT )  
-         {  
-             sendBytes = len;  
-         }  
- else
-         {  
- // 如果当前偏移 + 阈值 小于等于 总长度，则发送阈值大小的数据 
- if( curOffset + BYTES_PER_COUNT <= len )  
-             {  
-                 sendBytes = BYTES_PER_COUNT;  
-             }  
- // 否则就发送剩余长度的数据 
- else
-             {  
-                 sendBytes = len - curOffset;  
-             }  
-         }  
- 
-         ortp_message("send data bytes %i\n ",sendBytes);  
- 
-         rtp_session_send_with_ts(session,(char *)(buffer+curOffset),sendBytes,g_user_ts);  
- 
- // 累加 
-         sendCount ++;  
-         curOffset += sendBytes;                   
-         g_user_ts += TIME_STAMP_INC;  
- 
- // 发送一定数据包后休眠一会 
- if (sendCount%10==0)   
-         {  
-             usleep(20000);  
-         }     
-     }  
- return 0;  
- }  
- 
- /**  结束ortp的发送，释放资源    
-  *     
-  *   @param:  RtpSession *session RTP会话对象的指针    
-  *   @return:  0表示成功    
-  *   @note:        
-  */
- int rtpExit(RtpSession *session)  
- {  
-     g_user_ts = 0;  
- 
-     rtp_session_destroy(session);  
-     ortp_exit();  
-     ortp_global_stats_display();  
- 
- return 0;  
- }  
- 
- // 主函数，进行测试 
- int main()  
- {  
- // 待发送的数据缓冲区 
- char * pBuffer = "123445356234134234532523654323413453425236244123425234";  
- 
-     RtpSession * pRtpSession = NULL;  
- // 向（192.201.0.51，8000）目的地址发送rtp包 
-     pRtpSession = rtpInit("192.201.0.51",8000);  
- if(pRtpSession==NULL)  
-     {  
-         printf("error rtpInit");  
- return 0;  
-     }  
- 
- // 循环发送 
- while(1)  
-     {  
- if( rtpSend(pRtpSession,pBuffer,20) != 0)  
-         {  
-             printf("error rtpInit");  
- break;  
-         }  
-         usleep(10000);  
-         printf("sleep");  
-     }  
- 
- // 退出 
-     rtpExit(pRtpSession);  
- 
- return 0;  
- } 
