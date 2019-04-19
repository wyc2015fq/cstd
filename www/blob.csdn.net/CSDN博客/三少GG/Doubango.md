# Doubango - 三少GG - CSDN博客
2012年07月05日 17:25:00[三少GG](https://me.csdn.net/scut1135)阅读数：3627
**开源sip项目doubango ，android平台编译——imsdroid  **
imsdroid 装载Android模拟器上，一下对整个安装过程以及遇到的问题进行总结：
1  利用svn下载整个imsdroid项目相关文件夹，地址：
http://imsdroid.googlecode.com/svn/branches/2.0
2  打开eclipse，File->Import->General ->Exsiting Project to Workspace->Select project path选择刚下载的文件中的android-ngn-stack工程。
3  android-ngn-stack点击鼠标右键properity->liberary->Add JARs->android-ngn-stack->lib->simole-xml-2.3.4.jar.
4 设定工程android版本为Android 2.1(minSDK-7)，这个必须与project.properties中“target=android-7”p匹配，不匹配会出问题。
5 设定 Jave Compiler 为1.6
6  效仿2中方法，导入工程 IMSDroid,并进行3~5中的操作。
7更改项目IMSDroid->Resource->Linked Resources,将下面路径设定为..android-ngn-stack\src的位置。这一步很关键，guidence里面没有给出，但是如果不设定，就会出错。
在实际操作中还遇到多出问题
1 run->Android Project，报错"Conversion to dalvik format failed with error 1"，这个错误网上结识很多，原因是因为java使用proguard（混编）技术，dalvik是一种虚拟机，把java文件转化成dalvik format，如果不成功就报错。按照网上的方法，都没有解决这个问题，最后发现是工程的java build path引用的多余的path导致冲突，删除不用的path问题就解决了！
++++++++++++++++++++++++
### [IMSDroid介绍](http://blog.csdn.net/vincent1941/article/details/7262906)
分类： [开源项目](http://blog.csdn.net/vincent1941/article/category/367881)2012-02-15
 23:39648人阅读[评论](http://blog.csdn.net/vincent1941/article/details/7262906#comments)(1)[收藏]()[举报](http://blog.csdn.net/vincent1941/article/details/7262906#report)
支持视频的开源SIP客户端不多，IMSDroid看起来不错，准备研究下。（另外还有SIPDroid和linphone了，SIPDroid好像不支持视频，linphone怎么样？ ）。
有几个问题考虑：
1) IMSDroid的架构，哪些部分使用C/C++实现，及数据流？
2) 编解码是否可以使用硬件编码器？
3) 回声抑制问题？
4) 网络传输NAT问题？
下载源码，源码目录下有个android-ngn-stack.pdf文档介绍了软件的基本组成，一共有三层：
1) IMSDroid: 客户端界面
2) android-ngn-stack: 对doubangovoip框架的一个java层封装 。android-ngn-stack主要是为开发者在Android平台上开发VoIP应用提供软件栈(stack),
 它为上层应用提供三种级别的接口：Low, Medium和High。Low最灵活，但是调用最复杂。android-ngn-stack包含一个org.doubango.tinyWRAP包，提供对底层doubangovoip的JNI调用。tinyWRAP.so是对底层doubango的封装。
3) doubangovoip：C/C++(?)实现的VoIP功能的库，包括以下库：
**[html]**[view
 plain](http://blog.csdn.net/vincent1941/article/details/7262906#)[copy](http://blog.csdn.net/vincent1941/article/details/7262906#)
- tinySAK (Swiss Army Knife): Utilities functions (SHA-1, MD5, HMAC, String, List, Timers, Thread, Mutex, Semaphore, ...)  
- tinyNET: Networking (DNS, DHCPv4/v6, STUN, TURN, ICE, ENUM, Sockets, ...)  
- tinyHTTP: HTTP stack (CONNECT, DELETE, GET, HEAD, OPTIONS, PATCH, POST, PUT, TRACE, ...)  
- tinyXCAP: XCAP stack (AUID manager, URL generator) without XML parser (See Java code for parsers)  
- tinyIPSec: IPSec SA manager. Useless for Android but you MUST have it  
- tinySMS: SMS over IP (SM-TL, SM-RL) for IMS/LTE networks  
- tinySIGCOMP: Signaling Compression  
- tinySDP: SDP protocol  
- tinyRTP: RTP/RTCP protocols  
- tinyMSRP: MSRP protocol (Chat and File Transfer)  
- tinyMEDIA: Media plugins manager (Audio, video, Codecs, sessions, MSRP, QoS, ...)  
- tinyDAV(Doubango Audio Video): Media plugins implementation  
- tinySIP: SIP/IMS stack  
下面是IMSDroid最新版本的特征：
IMSDroid v2.x preview is now available for developers
The source code is under branches/2.0 and
 depends and doubango v2.x
New features: 
- The SIP/IMS Stack is 7 times faster
- NGN (Next Generation Network) stack for developers (android-ngn-stack)
- Better audio quality (Adaptive jitter buffer, noise suppression, automatic resampling, gain control, ...)
- Better video quality (low latency, low cpu usage, ...)
- VP8 video codec
- Multi-line
- MSRP chat
