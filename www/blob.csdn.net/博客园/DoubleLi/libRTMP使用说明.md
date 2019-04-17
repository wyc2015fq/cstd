# libRTMP使用说明 - DoubleLi - 博客园






**名称**

> 
librtmp − RTMPDump Real-Time Messaging Protocol API


**库**

> 
RTMPDump RTMP(librtmp, -lrtmp)


**简介**

> 
#include<librtmp/rtmp.h>


**描述**

> 
实时流协议（Real-TimeMessaging Protocol，RTMP）是用于互联网上传输视音频数据的网络协议。本API提供了支持RTMP， RTMPT,RTMPE, RTMP RTMPS以及以上几种协议的变种(RTMPTE, RTMPTS)协议所需的大部分客户端功能以及少量的服务器功能。尽管Adobe公司已经公布了RTMP协议规范（RTMP specification），但是本工程并不是通过Adobe的协议规范而是通过逆向工程的方式完成的。因此，它的运行方式可能和公布的协议规范有所偏离，但是一般情况下它和Adobe的客户端的运行方式是一模一样的。

RTMPDump 软件包含一个基本的客户端：[**rtmpdump**](http://rtmpdump.mplayerhq.hu/man1/rtmpdump.1),一些示例服务器和一个用来提供对RTMP协议进行支持的库（libRTMP）。本页面对libRTMP的函数进行一个概述。 这些函数可以在 -lrtmp 库中找到。其他还有很多函数,但是还没有为这些函数写文档。

基本的操作如下文所述。

**RTMP_Alloc**() ：用于创建一个RTMP会话的句柄。

**RTMP_Init**()：初始化句柄。

**RTMP_SetupURL**()：设置会话的参数。

**RTMP_Connect**()：建立RTMP链接中的网络连接（NetConnection）。

**RTMP_ConnectStream**()：建立RTMP链接中的网络流（NetStream）。

**RTMP_Read**()：读取RTMP流的内容。

客户端可以在调用**RTMP_Connect**()之前调用**RTMP_EnableWrite**()，然后在会话开始之后调用 **RTMP_Write**()。

**RTMP_Pause**()：流播放的时候可以用于暂停和继续

**RTMP_Seek**()：改变流播放的位置

当**RTMP_Read**()返回0 字节的时候,代表流已经读取完毕，而后可以调用**RTMP_Close**()

**RTMP_Free**()：用于清理会话。

所有的数据都使用 FLV 格式进行传输。一个基本的会话需要一个RTMP URL。RTMP URL 格式如下所示：

 rtmp[t][e|s]://hostname[:port][/app[/playpath]]

支持普通的，隧道的，以及加密的会话。

其他附加的选项可以在URL的后面添加以空格为间隔符的“key=value”形式的字符串。


**选项**

> 
**网络（Network）参数**

这些选项定义了如何连接一个流媒体服务器。

**socks=***host:port*

使用指定 SOCKS4代理。

**连接（Connection）参数**

这些选项定义了RTMP连接（Connect）请求消息的内容。如果没有提供正确的值，流媒体服务器会拒绝连接请求。

**app=***name*

连接到RTMP的应用名，覆盖RTMP URL中的app。有时rtmpdumpURL 无法正确自动解析app名称。这时必须使用该选项。

**tcUrl=***url*

目标流的URL。默认是 rtmp[t][e|s]://host[:port]/app.

**pageUrl=***url*

流媒体所在的网页的URL。默认情况下没有被发送的价值。

**swfUrl=***url*

流媒体使用的SWF播放器的的URL。默认情况下没有被发送的价值。

**flashVer=***version*

swf播放器使用的Flash版本. 默认是"LNX 10,0,32,18"。

**conn=***type:data*

任意AMF数据追加到connect，类型说明，

B 布尔型

N 数字

S 字符串

O 对象

Z 空

对于布尔型必须是0或1作为FALSE或TRUE，

对于对象数据必须以0或1分别作为结束和开始的标制，子对象中数据项前加前缀类型N，并指定值名称，例如：

  -C B:1 -C S:authMe -C O:1 -C NN:code:1.23-C NS:flag:ok -C O:0

**会话（Session）参数**

这些选项在连接成功后生效。

**playpath=***path*

覆盖RTMP URL解析的playpath，rtmpdump有时不能正确解析，通过该选项明确。

**playlist=***0|1*

在play命令之前发生set_playlist命令。否则播放列表将会值包含playpath。

**live=***0|1*

指定媒体是实时流。在实时流中没有恢复和搜索。

**subscribe=***path*

订阅的实时流名称。默认playpath。

**start=***num*

开始到流的秒数（num），实时流无效。

**stop=***num*

停止到流的秒数（num）。

**buffer=***num*

设置缓冲时间，单位毫秒。 默认值36000000。

**timeout=***num*

num秒后没有收到任何数据会话超时，默认值120。

**安全（Security）参数**

这些选项处理额外的身份验证，来自服务器的请求。

**token=***key*

输入安全令牌响应，如果服务器需要使用安全令牌验证。

**jtv=***JSON*

JSON令牌用于传统Justin.tv服务器 ，调用NetStream.Authenticate.UsherToken。

**swfVfy=***0|1*

swf播放器的URL，此选项将替换所以三个--swfUrl,--swfhash, and --swfsize选项。使用此选项时，swf播放器将从指定URL检索，并自动计算哈希和大小。此外信息缓存在一个swfinfo文件在用户主目录，所以它在每次rtmpdump运行时，并不需要检索和重新计算。swfinfo记录URL，生成时间，修改SWF文件时间，它的大小，它的哈希，默认情况下，缓冲信息用于30天，然后重新检测。

**swfAge=***days*

指定使用缓存的swf信息天数，然后重新检查，使用0为经常检查，如果检查显示swf具有相同的修改时间戳，它不会被再次检索。


**例子**

> 
**RTMP_SetupURL**()使用的一个例子字符串：

 "rtmp://flashserver:1935/ondemand/thefile swfUrl=[http://flashserver/player.swf](http://flashserver/player.swf)swfVfy=1"


**参见**

> 
[**rtmpdump**](http://rtmpdump.mplayerhq.hu/man1/rtmpdump.1)(1), [**rtmpgw**](http://rtmpdump.mplayerhq.hu/man8/rtmpgw.8)(8)


**作者**

> 
Andrej Stepanchuk, Howard Chu,The Flvstreamer Team 

[http://rtmpdump.mplayerhq.hu](http://rtmpdump.mplayerhq.hu/)




翻译的有的地方还不是很通顺，以后再改进

原文地址：[http://rtmpdump.mplayerhq.hu/librtmp.3.html](http://rtmpdump.mplayerhq.hu/librtmp.3.html)









