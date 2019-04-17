# Linux下几种RTP协议实现的比较和JRTPLIB编程讲解 - DoubleLi - 博客园






流媒体指的是在网络中使用流技术传输的连续时基媒体，其特点是在播放前不需要下载整个文件，而是采用边下载边播放的方式，它是视频会议、 IP电话等应用场合的技术基础。RTP是进行实时流媒体传输的标准协议和关键技术，本文介绍如何在Linux下利用JRTPLIB进行实时流媒体编程。 

         随着Internet的日益普及，在网络上传输的数据已经不再局限于文字和图形，而是逐渐向声音和视频等多媒体格式过渡。目前在网络上传输音频/视频 （Audio/Video，简称A/V）等多媒体文件时，基本上只有下载和流式传输两种选择。通常说来，A/V文件占据的存储空间都比较大，在带宽受限的 网络环境中下载可能要耗费数分钟甚至数小时，所以这种处理方法的延迟很大。如果换用流式传输的话，声音、影像、动画等多媒体文件将由专门的流媒体服务器负 责向用户连续、实时地发送，这样用户可以不必等到整个文件全部下载完毕，而只需要经过几秒钟的启动延时就可以了，当这些多媒体数据在客户机上播放时，文件 的剩余部分将继续从流媒体服务器下载。

        流（Streaming）是近年在Internet上出现的新概念，其定义非常广泛，主要是指通过网络传输多媒体数据的技术总称。流媒体包含广义和狭义两 种内涵：广义上的流媒体指的是使音频和视频形成稳定和连续的传输流和回放流的一系列技术、方法和协议的总称，即流媒体技术；狭义上的流媒体是相对于传统的 下载-回放方式而言的，指的是一种从Internet上获取音频和视频等多媒体数据的新方法，它能够支持多媒体数据流的实时传输和实时播放。通过运用流媒 体技术，服务器能够向客户机发送稳定和连续的多媒体数据流，客户机在接收数据的同时以一个稳定的速率回放，而不用等数据全部下载完之后再进行回放。

        由于受网络带宽、计算机处理能力和协议规范等方面的限制，要想从Internet上下载大量的音频和视频数据，无论从下载时间和存储空间上来讲都是不太现 实的，而流媒体技术的出现则很好地解决了这一难题。目前实现流媒体传输主要有两种方法：顺序流（progressive streaming）传输和实时流（realtime streaming）传输，它们分别适合于不同的应用场合。

顺序流传输

        顺序流传输采用顺序下载的方式进行传输，在下载的同时用户可以在线回放多媒体数据，但给定时刻只能观看已经下载的部分，不能跳到尚未下载的部分，也不能在 传输期间根据网络状况对下载速度进行调整。由于标准的HTTP服务器就可以发送这种形式的流媒体，而不需要其他特殊协议的支持，因此也常常被称作HTTP 流式传输。顺序流式传输比较适合于高质量的多媒体片段，如片头、片尾或者广告等。

实时流传输

        实时流式传输保证媒体信号带宽能够与当前网络状况相匹配，从而使得流媒体数据总是被实时地传送，因此特别适合于现场事件。实时流传输支持随机访问，即用户 可以通过快进或者后退操作来观看前面或者后面的内容。从理论上讲，实时流媒体一经播放就不会停顿，但事实上仍有可能发生周期性的暂停现象，尤其是在网络状 况恶化时更是如此。与顺序流传输不同的是，实时流传输需要用到特定的流媒体服务器，而且还需要特定网络协议的支持。

        实时传输协议（Real-time Transport Protocol，PRT）是在Internet上处理多媒体数据流的一种网络协议，利用它能够在一对一（unicast，单播）或者一对多 （multicast，多播）的网络环境中实现传流媒体数据的实时传输。RTP通常使用UDP来进行多媒体数据的传输，但如果需要的话可以使用TCP或者 ATM等其它协议，整个RTP协议由两个密切相关的部分组成：RTP数据协议和RTP控制协议。实时流协议（Real Time Streaming Protocol，RTSP）最早由Real Networks和Netscape公司共同提出，它位于RTP和RTCP之上，其目的是希望通过IP网络有效地传输多媒体数据。



**RTSP,RTP,RTCP的区别：RTSP发起/终结流媒体、RTP传输流媒体数据 、RTCP对RTP进行控制，同步。**

2.1 RTP数据协议

RTP数据协议负责对流媒体数据进行封包并实现媒体流的实时传输，每一个RTP数据报都由头部（Header）和负载（Payload）两个部分组成，其中头部前12个字节的含义是固定的，而负载则可以是音频或者视频数据。RTP数据报的头部格式如图1所示：

![](http://blog.chinaunix.net/photo/108006_101013152105.jpg)
其中比较重要的几个域及其意义如下：

 CSRC记数（CC）　　表示CSRC标识的数目。CSRC标识紧跟在RTP固定头部之后，用来表示RTP数据报的来源，RTP协议允许在同一个 会话中存在多个数据源，它们可以通过RTP混合器合并为一个数据源。例如，可以产生一个CSRC列表来表示一个电话会议，该会议通过一个 RTP混合器将所有讲话者的语音数据组合为一个RTP数据源。 
负载类型（PT）　　标明RTP负载的格式，包括所采用的编码算法、采样频率、承载通道等。例如，类型2表明该RTP数据包中承载的是用ITU G.721算法编码的语音数据，采样频率为8000Hz，并且采用单声道。 
序列号　　用来为接收方提供探测数据丢失的方法，但如何处理丢失的数据则是应用程序自己的事情，RTP协议本身并不负责数据的重传。 
 时间戳　　记录了负载中第一个字节的采样时间，接收方能够时间戳能够确定数据的到达是否受到了延迟抖动的影响，但具体如何来补偿延迟抖动则是应用程序自己的事情。 
从RTP 数据报的格式不难看出，它包含了传输媒体的类型、格式、序列号、时间戳以及是否有附加数据等信息，这些都为实时的流媒体传输提供了相应的基础。RTP协议 的目的是提供实时数据（如交互式的音频和视频）的端到端传输服务，因此在RTP中没有连接的概念，它可以建立在底层的面向连接或面向非连接的传输协议之 上；RTP也不依赖于特别的网络地址格式，而仅仅只需要底层传输协议支持组帧（Framing）和分段（Segmentation）就足够了；另外RTP 本身还不提供任何可靠性机制，这些都要由传输协议或者应用程序自己来保证。在典型的应用场合下，RTP 一般是在传输协议之上作为应用程序的一部分加以实现的。

参考：

version (V): 2 bits     
      标明RTP版本号。协议初始版本为0，RFC3550中规定的版本号为2。


       padding (P): 1 bit     
      如果该位被设置，则在该packet末尾包含了额外的附加信息，附加信息的最后一个字节表示额外附加信息的长度（包含该字节本身）。该字段之所以存在是因 为一些加密机制需要固定长度的数据块，或者为了在一个底层协议数据单元中传输多个RTP packets。  


      extension (X): 1 bit
      如果该位被设置，则在固定的头部后存在一个扩展头部，格式定义在RFC3550 5.3.1节。


      CSRC count (CC): 4 bits
      在固定头部后存在多少个CSRC标记。


      marker (M): 1 bit
      该位的功能依赖于profile的定义。profile可以改变该位的长度，但是要保持marker和payload type总长度不变（一共是8 bit）。


      payload type (PT): 7 bits
      标记着RTP packet所携带信息的类型，标准类型列出在RFC3551中。如果接收方不能识别该类型，必须忽略该packet。


      sequence number: 16 bits
      序列号，每个RTP packet发送后该序列号加1，接收方可以根据该序列号重新排列数据包顺序。


      timestamp: 32 bits
      时间戳。反映RTP packet所携带信息包中第一个字节的采样时间。


      SSRC: 32 bits
      数据源标识。在一个RTP Session其间每个数据流都应该有一个不同的SSRC。


      CSRC list: 0 to 15 items, 每个源标识32 bits
      贡献数据源标识。只有存在Mixer的时候才有效。如一个将多声道的语音流合并成一个单声道的语音流，在这里就列出原来每个声道的SSRC。

2.2 RTCP控制协议

RTCP 控制协议需要与RTP数据协议一起配合使用，当应用程序启动一个RTP会话时将同时占用两个端口，分别供RTP 和RTCP使用。RTP本身并不能为按序传输数据包提供可靠的保证，也不提供流量控制和拥塞控制，这些都由RTCP来负责完成。通常RTCP会采用与 RTP相同的分发机制，向会话中的所有成员周期性地发送控制信息，应用程序通过接收这些数据，从中获取会话参与者的相关资料，以及网络状况、分组丢失概率 等反馈信息，从而能够对服务质量进行控制或者对网络状况进行诊断。

RTCP协议的功能是通过不同的RTCP数据报来实现的，主要有如下几种类型：

SR　　发送端报告，所谓发送端是指发出RTP数据报的应用程序或者终端，发送端同时也可以是接收端。 
RR　　接收端报告，所谓接收端是指仅接收但不发送RTP数据报的应用程序或者终端。 
SDES　　源描述，主要功能是作为会话成员有关标识信息的载体，如用户名、邮件地址、电话号码等，此外还具有向会话成员传达会话控制信息的功能。 
BYE　　通知离开，主要功能是指示某一个或者几个源不再有效，即通知会话中的其他成员自己将退出会话。 
APP　　由应用程序自己定义，解决了RTCP的扩展性问题，并且为协议的实现者提供了很大的灵活性。 
RTCP数据报携带有服务质量监控的必要信息，能够对服务质量进行动态的调整，并能够对网络拥塞进行有效的控制。由于RTCP数据报采用的是多播方式，因此会话中的所有成员都可以通过RTCP数据报返回的控制信息，来了解其他参与者的当前情况。

在一个典型的应用场合下，发送媒体流的应用程序将周期性地产生发送端报告SR，该RTCP数据报含有不同媒体流间的同步信息，以及已经发送的数据报 和字节的计数，接收端根据这些信息可以估计出实际的数据传输速率。另一方面，接收端会向所有已知的发送端发送接收端报告RR，该RTCP数据报含有已接收 数据报的最大序列号、丢失的数据报数目、延时抖动和时间戳等重要信息，发送端应用根据这些信息可以估计出往返时延，并且可以根据数据报丢失概率和时延抖动 情况动态调整发送速率，以改善网络拥塞状况，或者根据网络状况平滑地调整应用程序的服务质量。

2.3 RTSP实时流协议

作为一个应用层协议，RTSP提供了一个可供扩展的框架，它的意义在于使得实时流媒体数据的受控和点播变得可能。总的说来，RTSP是一个流媒体表 示协议，主要用来控制具有实时特性的数据发送，但它本身并不传输数据，而是必须依赖于下层传输协议所提供的某些服务。RTSP 可以对流媒体提供诸如播放、暂停、快进等操作，它负责定义具体的控制消息、操作方法、状态码等，此外还描述了与RTP间的交互操作。

RTSP 在制定时较多地参考了HTTP/1.1协议，甚至许多描述与HTTP/1.1完全相同。RTSP之所以特意使用与HTTP/1.1类似的语法和操作，在很 大程度上是为了兼容现有的Web基础结构，正因如此，HTTP/1.1的扩展机制大都可以直接引入到RTSP 中。

由RTSP 控制的媒体流集合可以用表示描述（Presentation Description）来定义，所谓表示是指流媒体服务器提供给客户机的一个或者多个媒体流的集合，而表示描述则包含了一个表示中各个媒体流的相关信 息，如数据编码/解码算法、网络地址、媒体流的内容等。

虽然RTSP服务器同样也使用标识符来区别每一流连接会话（Session），但RTSP连接并没有被绑定到传输层连接（如TCP等），也就是说在 整个 RTSP连接期间，RTSP用户可打开或者关闭多个对RTSP服务器的可靠传输连接以发出RTSP 请求。此外，RTSP连接也可以基于面向无连接的传输协议（如UDP等）。

RTSP协议目前支持以下操作：

 检索媒体　　允许用户通过HTTP或者其它方法向媒体服务器提交一个表示描述。如表示是组播的，则表示描述就包含用于该媒体流的组播地址和端口号；如果表示是单播的，为了安全在表示描述中应该只提供目的地址。 
 邀请加入　　媒体服务器可以被邀请参加正在进行的会议，或者在表示中回放媒体，或者在表示中录制全部媒体或其子集，非常适合于分布式教学。 
 添加媒体　　通知用户新加入的可利用媒体流，这对现场讲座来讲显得尤其有用。与HTTP/1.1类似，RTSP请求也可以交由代理、通道或者缓存来进行处理。

        RTP 是目前解决流媒体实时传输问题的最好办法，如果需要在Linux平台上进行实时流媒体编程，可以考虑使用一些开放源代码的RTP库，如LIBRTP、 JRTPLIB等。JRTPLIB是一个面向对象的RTP库，它完全遵循RFC 1889设计，在很多场合下是一个非常不错的选择，下面就以JRTPLIB为例，讲述如何在Linux平台上运用RTP协议进行实时流媒体编程。

**资源预留协议RSVP**


      1） RSVP协议：
      RSVP (Resorce reSerVation Protocol) 资源预留协议并不是一个路由协议，而是一种IP网络中的信令协议，它与路由协议相结合来实现对网络传输服务质量（QoS）的控制。RSVP是为支持因特网 综合业务而提出的。这是解决IP通信中QoS（服务质量）问题的一种技术，用来保证点端到端的传输带宽。


      2） RSVP协议是如何工作：
      RSVP使用控制数据报，这些数据报在向特定地址传输时包括了需要由路由器检查（有些时候需要更新）的信息,如果路由器需要决定是不是要检查数据报的内容 的时候对上层数据内容进行语法分析。这种分析的代价可不小。现在的情况是，网络终端利用它向网络申请资源，在这种表明“申请” 的信号中，包含着如下的信息：业务的种类？ 使用者类型？ 什么时间？ 需要多大带宽？ 其他参考信息？ 网络在接收到上类信息后，会根据实际情况为此次连接分配一个优先代码，用户利用优先代码进行信息传递时，网络不需重新对业务进行分析与判别，从另外一个角 度来说，利用RSVP 能从一定程度上减少网络对信息处理的时延，提高网络节点的工作效率，改善信息传输的服务质量（QoS）。实时应用用RSVP是为了在传输路径中保持必要的 资源以保证请求能确保到达。
      RSVP是IP路由器为提供更好的服务质量向前迈进的具有深刻意义的一步。传统上IP路由器只负责分组转发，通过路由协议知道邻近路由器的地址。而 RSVP则类似于电路交换系统的信令协议一样，为一个数据流通知其所经过的每个节点（IP路由器），与端点协商为此数据流提供质量保证。


**结束语**


      在前面我们讨论了一些与实时数据传输相关的四个协议：


      1）RTP是实时数据传输协议。它提供时间标志，序列号以及其它能够保证在实时数据传输时处理时间的方法；它是依靠RVSP保证服务质量标准的。
      2）RTCP是RTP的控制部分,是用来保证服务质量和成员管理的。
      3）RTSP是开始和指引流媒体数据从流媒体服务器。它又可叫做"网上录像机控制协议".它是提供远程的控制，具体的数据传输是交给RTP的。
      4）RSVP是Internet上的资源预订协议，使用RSVP预留一部分网络资源（即带宽），能在一定程度上为流媒体的传输提供QoS。就像TCP的重发和滑动窗口等都是

3.1 环境搭建

JRTPLIB 是一个用C++语言实现的RTP库，目前已经可以运行在Windows、Linux、FreeBSD、 Solaris、Unix和VxWorks等多种操作系统上。要为Linux 系统安装JRTPLIB，首先从JRTPLIB的网站（http: //lumumba.luc.ac.be/jori/jrtplib/jrtplib.html）下载最新的源码包，此处使用的是jrtplib- 2.7b.tar.bz2。假设下载后的源码包保存在/usr/local/src目录下，执行下面的命令可以对其进行解压缩：


[root@linuxgam src]# bzip2 -dc jrtplib-2.7b.tar.bz2 | tar xvf -

接下去需要对JRTPLIB进行配置和编译：


[root@linuxgam src]# cd jrtplib-2.7
[root@linuxgam jrtplib-2.7b]# ./configure 
[root@linuxgam jrtplib-2.7b]# make
最后再执行如下命令就可以完成JRTPLIB的安装：

[root@linuxgam jrtplib-2.7b]# make install

3.2 初始化

在使用JRTPLIB进行实时流媒体数据传输之前，首先应该生成RTPSession类的一个实例来表示此次RTP会话，然后调用Create() 方法来对其进行初始化操作。RTPSession类的Create()方法只有一个参数，用来指明此次RTP会话所采用的端口号。清单1给出了一个最简单 的初始化框架，它只是完成了RTP会话的初始化工作，还不具备任何实际的功能。

#include "rtpsession.h"

int main(void)
{
 RTPSession sess;
 sess.Create(5000);
 return 0;
}
如 果RTP会话创建过程失败，Create()方法将会返回一个负数，通过它虽然可以很容易地判断出函数调用究竟是成功的还是失败的，但却很难明白出错的原 因到底什么。JRTPLIB采用了统一的错误处理机制，它提供的所有函数如果返回负数就表明出现了某种形式的错误，而具体的出错信息则可以通过调用 RTPGetErrorString()函数得到。RTPGetErrorString()函数将错误代码作为参数传入，然后返回该错误代码所对应的错误 信息。清单2给出了一个更加完整的初始化框架，它可以对RTP会话初始化过程中所产生的错误进行更好的处理：


#include <stdio.h>
#include "rtpsession.h"

int main(void)
{
  RTPSession sess;
  int status;
  char* msg;

  sess.Create(6000);
  msg = RTPGetErrorString(status);
  printf("Error String: %s\\n", msg);
  return 0;
}
设 置恰当的时戳单元，是RTP会话初始化过程所要进行的另外一项重要工作，这是通过调用RTPSession类的 SetTimestampUnit()方法来实现的，该方法同样也只有一个参数，表示的是以秒为单元的时戳单元。例如，当使用RTP会话传输8000Hz 采样的音频数据时，由于时戳每秒钟将递增8000，所以时戳单元相应地应该被设置成1/8000：

sess.SetTimestampUnit(1.0/8000.0);

3.3 数据发送

当RTP 会话成功建立起来之后，接下去就可以开始进行流媒体数据的实时传输了。首先需要设置好数据发送的目标地址， RTP协议允许同一会话存在多个目标地址，这可以通过调用RTPSession类的AddDestination()、 DeleteDestination()和ClearDestinations()方法来完成。例如，下面的语句表示的是让RTP会话将数据发送到本地主 机的6000端口(注意：如果是需要发到另一个NAT设备后面终端，则需要通过NAT穿透，见后)：



unsigned long addr = ntohl(inet_addr("127.0.0.1"));
sess.AddDestination(addr, 6000);

目标地址全部指定之后，接着就可以调用RTPSession类的SendPacket()方法，向所有的目标地址发送流媒体数据。SendPacket()是RTPSession类提供的一个重载函数，它具有下列多种形式：



int SendPacket(void *data,int len)
int SendPacket(void *data,int len,unsigned char pt,bool mark,unsigned long timestampinc)
int SendPacket(void *data,int len,unsigned short hdrextID,void *hdrextdata,int numhdrextwords)
int SendPacket(void *data,int len,unsigned char pt,bool mark,unsigned long timestampinc,
    unsigned short hdrextID,void *hdrextdata,int numhdrextwords)

SendPacket()最典型的用法是类似于下面的语句，其中第一个参数是要被发送的数据，而第二个参数则指明将要发送数据的长度，再往后依次是RTP负载类型、标识和时戳增量。



sess.SendPacket(buffer, 5, 0, false, 10);

对于同一个RTP会话来讲，负载类型、标识和时戳增量通常来讲都是相同的，JRTPLIB允许将它们设置为会话的默认参数，这是通过调用 RTPSession类的SetDefaultPayloadType()、SetDefaultMark()和 SetDefaultTimeStampIncrement()方法来完成的。为RTP会话设置这些默认参数的好处是可以简化数据的发送，例如，如果为 RTP会话设置了默认参数：

sess.SetDefaultPayloadType(0);
sess.SetDefaultMark(false);
sess.SetDefaultTimeStampIncrement(10);

之后在进行数据发送时只需指明要发送的数据及其长度就可以了：

sess.SendPacket(buffer, 5);

3.4 数据接收

对于流媒体数据的接收端，首先需要调用RTPSession类的PollData()方法来接收发送过来的RTP或者 RTCP数据报。由于同一个RTP会话中允许有多个参与者（源），你既可以通过调用RTPSession类的GotoFirstSource()和 GotoNextSource()方法来遍历所有的源，也可以通过调用RTPSession类的GotoFirstSourceWithData()和 GotoNextSourceWithData()方法来遍历那些携带有数据的源。在从RTP会话中检测出有效的数据源之后，接下去就可以调用 RTPSession类的GetNextPacket()方法从中抽取RTP数据报，当接收到的RTP数据报处理完之后，一定要记得及时释放。下面的代码 示范了该如何对接收到的RTP数据报进行处理：

if (sess.GotoFirstSourceWithData()) {
  do {
    RTPPacket *pack;     
    pack = sess.GetNextPacket();     
    // 处理接收到的数据
    delete pack;
  } while (sess.GotoNextSourceWithData());
}

JRTPLIB为RTP数据报定义了三种接收模式，其中每种接收模式都具体规定了哪些到达的RTP数据报将会被接受，而哪些到达的RTP数据报将会被拒绝。通过调用RTPSession类的SetReceiveMode()方法可以设置下列这些接收模式：

 RECEIVEMODE_ALL　　缺省的接收模式，所有到达的RTP数据报都将被接受； 
 RECEIVEMODE_IGNORESOME 　　除了某些特定的发送者之外，所有到达的RTP数据报都将被接受，而被拒绝的发送者列表可以通过调用AddToIgnoreList()、 DeleteFromIgnoreList()和ClearIgnoreList()方法来进行设置； 
RECEIVEMODE_ACCEPTSOME 　　除了某些特定的发送者之外，所有到达的RTP数据报都将被拒绝，而被接受的发送者列表可以通过调用AddToAcceptList ()、DeleteFromAcceptList和ClearAcceptList ()方法来进行设置。 
3.5 控制信息

JRTPLIB 是一个高度封装后的RTP库，程序员在使用它时很多时候并不用关心RTCP数据报是如何被发送和接收的，因为这些都可以由JRTPLIB自己来完成。只要 PollData()或者SendPacket()方法被成功调用，JRTPLIB就能够自动对到达的 RTCP数据报进行处理，并且还会在需要的时候发送RTCP数据报，从而能够确保整个RTP会话过程的正确性。

而另一方面，通过调用RTPSession类提供的SetLocalName()、SetLocalEMail()、 SetLocalLocation()、SetLocalPhone()、SetLocalTool()和SetLocalNote()方法， JRTPLIB又允许程序员对RTP会话的控制信息进行设置。所有这些方法在调用时都带有两个参数，其中第一个参数是一个char型的指针，指向将要被设 置的数据；而第二个参数则是一个int型的数值，表明该数据中的前面多少个字符将会被使用。例如下面的语句可以被用来设置控制信息中的电子邮件地址：

sess.SetLocalEMail("[xiaowp@linuxgam.comxiaowp@linuxgam.com](mailto:xiaowp@linuxgam.comxiaowp@linuxgam.com)",19);

在RTP 会话过程中，不是所有的控制信息都需要被发送，通过调用RTPSession类提供的 EnableSendName()、EnableSendEMail()、EnableSendLocation()、EnableSendPhone ()、EnableSendTool()和EnableSendNote()方法，可以为当前RTP会话选择将被发送的控制信息。

3.6 实际应用

最后通过一个简单的流媒体发送-接收实例，介绍如何利用JRTPLIB来进行实时流媒体的编程。清单3给出了数据发送端的完整代码，它负责向用户指定的IP地址和端口，不断地发送RTP数据包：


#include <stdio.h>
#include <string.h>
#include "rtpsession.h"

// 错误处理函数
void checkerror(int err)
{
  if (err < 0) {
    char* errstr = RTPGetErrorString(err);
    printf("Error:%s\\n", errstr);
    exit(-1);
  }
}

int main(int argc, char** argv)
{
  RTPSession sess;
  unsigned long destip;
  int destport;
  int portbase = 6000;
  int status, index;
  char buffer[128];

  if (argc != 3) {
    printf("Usage: ./sender destip destport\\n");
    return -1;
  }

  // 获得接收端的IP地址和端口号
  destip = inet_addr(argv[1]);
  if (destip == INADDR_NONE) {
    printf("Bad IP address specified.\\n");
    return -1;
  }
  destip = ntohl(destip);
  destport = atoi(argv[2]);

  // 创建RTP会话
  status = sess.Create(portbase);
  checkerror(status);

  // 指定RTP数据接收端
  status = sess.AddDestination(destip, destport);
  checkerror(status);

  // 设置RTP会话默认参数
  sess.SetDefaultPayloadType(0);
  sess.SetDefaultMark(false);
  sess.SetDefaultTimeStampIncrement(10);

  // 发送流媒体数据
  index = 1;
  do {
    sprintf(buffer, "%d: RTP packet", index ++);
    sess.SendPacket(buffer, strlen(buffer));
    printf("Send packet !\\n");
  } while(1);

  return 0;
}

清单4则给出了数据接收端的完整代码，它负责从指定的端口不断地读取RTP数据包：

#include <stdio.h>
#include "rtpsession.h"
#include "rtppacket.h"

// 错误处理函数
void checkerror(int err)
{
  if (err < 0) {
    char* errstr = RTPGetErrorString(err);
    printf("Error:%s\\n", errstr);
    exit(-1);
  }
}

int main(int argc, char** argv)
{
  RTPSession sess;
  int localport;
  int status;

  if (argc != 2) {
    printf("Usage: ./sender localport\\n");
    return -1;
  }

   // 获得用户指定的端口号
  localport = atoi(argv[1]);

  // 创建RTP会话
  status = sess.Create(localport);
  checkerror(status);

  do {
    // 接受RTP数据
    status = sess.PollData();
 // 检索RTP数据源
    if (sess.GotoFirstSourceWithData()) {
      do {
        RTPPacket* packet;
        // 获取RTP数据报
        while ((packet = sess.GetNextPacket()) != NULL) {
          printf("Got packet !\\n");
          // 删除RTP数据报
          delete packet;
        }
      } while (sess.GotoNextSourceWithData());
    }
  } while(1);

  return 0;
}

随着多媒体数据在Internet上所承担的作用变得越来越重要，需要实时传输音频和视频等多媒体数据的场合也将变得越来越多，如IP电话、视频点 播、在线会议等。RTP是用来在Internet上进行实时流媒体传输的一种协议，目前已经被广泛地应用在各种场合，JRTPLIB是一个面向对象的 RTP封装库，利用它可以很方便地完成Linux平台上的实时流媒体编程。



4 基于jrtplib的NAT穿透



4.1 NAT穿透的基础只是



[有关于NAT穿透的基础知识](http://aphrodit.blog.sohu.com/135861931.html)



4.2 rtp/rtcp传输涉及到的NAT穿透



    rtp/rtcp传输数据的时候，需要两个端口支持。即rtp端口用于传输rtp数据，即传输的多媒体数据；rtcp端口用于传输rtcp控制协议信息。 rtp/rtcp协议默认的端口是rtcp port = rtp port + 1 。详细的说，比如A终端和B终端之间通过rtp/rtcp进行通信，

![](http://1803.img.pp.sohu.com.cn/images/blog/2009/12/14/14/25/1263be13f2eg214.jpg)



如上图，

                                                          本地IP:PORT                                                        NAT映射后IP:PORT

UACA RTP的发送和接收IP:PORT ： 192.168.1.100:8000                                             61.144.174.230:1597

UACA RTCP的发送和接收IP:PORT：192.168.1.100:8001                                             61.144.174.230:1602

UACB RTP的发送和接收IP:PORT ： 192.168.1.10:8000                                                61.144.174.12:8357

UACB RTCP的发送和接收IP:PORT：192.168.1.10:8001                                                61.144.174.12:8420



上图可以得到一下一些信息：

      (一) 本地端口 RTCP PORT = RTP PORT + 1;但是经过NAT映射之后也有可能满足这个等式，但是并不一定有这个关系。

    （二）在NAT设备后面的终端的本地IP:PORT并不被NAT外的设置可知，也就无法通过终端的本地IP:PORT与之通信。而必须通过NAT映射之后的公网IP:PORT作为目的地址进行通信。

    如上图的终端A如果要发送RTP数据到终端B，UACA发送的目的地址只能是：61.144.174.12:8357。同理，UACB发送RTP数据给UACA，目的地址只能是： 61.144.174.230:1597 。

    （三）也许看到这里，如何得到自己的外网IP:PORT呢？如何得到对方的外网IP:PORT呢？这就是NAT IP:PORT转换和穿孔（puncture），下回分解。



4.3 NAT 地址转换



如上所述，终端需要知道自己的外网IP：port，可以通过STUN、STUNT、TURN、Full Proxy等方式。这里介绍通过STUN方式实现NAT穿透。



STUN: Simple Traversal of UDP Through NAT。即通过UDP对NAT进行穿透。

STUNT:Simple Traversal of UDP Through NATs and TCP too.可以通过TCP对NAT进行穿透。

STUN是一个标准协议，具体的协议内容网络上很多。在此不累述了。

为 了通过STUN实现NAT穿透，得到自己的公网IP:PORT，必须有一个公网STUN服务器，以及我们的客户端必须支持STUN Client功能。STUN Client 通过UDP发送一个request到STUN服务器，该请求通过NAT设备的时候会把数据报头中的本地IP:PORT换成该本地IP:PORT对应的公网 IP:PORT，STUN服务器接收到该数据包后就可以把该公网IP:PORT 发送给STUN Client。这样我们就得到了自己的公网IP:PORT。这样别的终端就可以把该公网IP:PORT最为发送UDP数据的目的地址发送UDP数据。



推荐一款STUN client/server 程序代码，[http://sourceforge.net/projects/stun/files/](http://sourceforge.net/projects/stun/files/)

这是一款开源软件。在客户端中的主要函数是下面这个：

NatType 

stunNatType( StunAddress4& dest,       //in 公网STUN服务器地址，如stun.xten.net

             bool verbose,                              //in 调试时是否输出调试信息
             bool* preservePort=0,                //out  if set, is return for if NAT preservers ports or not
             bool* hairpin=0 ,                        //out  if set, is the return for if NAT will hairpin packetsNAT设备是否支持回环
             int port=0,                               // in 本地测试端口port to use for the test, 0 to choose random port
             StunAddress4* sAddr=0        // out NIC to use ，返回STUN返回的本地地址的公网IP:PORT
   );

             输入StunAddress和测试端口port,得到本地IP:PORT对应的公网IP:PORT.



4.4 对jrtplib  的改造



jrtplib中对rtp rtcp端口的处理关系是：rtcp port = rtp port + 1 。这就有问题，本地端口可以按照这个等式来设置端口，但是经过NAT映射之后的公网端口是随机的，有可能并不满足上述等式。

    int portbase = 6000;                        //设置本地rtp端口为6000

    transparams.SetPortbase(portbase);//默认的本地rtcp端口为6001.因为这里是本地设置，所一这样设置OK
    status = sess.Create(sessparams,&transparams);    
    checkerror(status);

    RTPIPv4Address addr(destip,destport); //设置目的地的rtp接收IP:PORT，公网传输的话就要设置为对方的rtp公网IP:PORT
    // AddDestination（）的内部处理是把addr.ip和addr.port+1赋给rtcp。这样如果对方在公网上，就有问题了。因为对方的rtcp port 可能不等于rtp port +1;这就导致对方收不到rtcp数据包。

    status = sess.AddDestination(addr); 



    通过跟踪AddDestination（）函数的实现，发现在class RTPIPv4Destination的构造函数中是这样构造一个发送目的地址的：

        RTPIPv4Destination(uint32_t ip,uint16_t rtpportbase)                    
    {
        memset(&rtpaddr,0,sizeof(struct sockaddr_in));
        memset(&rtcpaddr,0,sizeof(struct sockaddr_in));

        rtpaddr.sin_family = AF_INET;
        rtpaddr.sin_port = htons(rtpportbase);
        rtpaddr.sin_addr.s_addr = htonl(ip);


            rtcpaddr.sin_family = AF_INET;
            rtcpaddr.sin_port = htons(rtpportbase+1);//默认把rtp的端口+1赋给目的rtcp端口。
            rtcpaddr.sin_addr.s_addr = htonl(ip);

        RTPIPv4Destination::ip = ip;
    }



        为了实现：可以自定义目的IP地址和目的rtp port和rtcp port。为了实现这么目标，自己动手改造下面几个函数：构造函数RTPIPv4Destination() 、RTPSession::AddDestination()，思路是在目的地址设置相关函数中增加一个rtcp ip 和port参数。

        RTPIPv4Destination(uint32_t ip,uint16_t rtpportbase,uint32_t rtcpip,uint16_t rtcpport)                    
    {
        memset(&rtpaddr,0,sizeof(struct sockaddr_in));
        memset(&rtcpaddr,0,sizeof(struct sockaddr_in));

        rtpaddr.sin_family = AF_INET;
        rtpaddr.sin_port = htons(rtpportbase);
        rtpaddr.sin_addr.s_addr = htonl(ip);

        /**If rtcport has not been set separately, use the default rtcpport*/
        if ( 0 == rtcpport )
        {
            rtcpaddr.sin_family = AF_INET;
            rtcpaddr.sin_port = htons(rtpportbase+1);
            rtcpaddr.sin_addr.s_addr = htonl(ip);
        }else
        {
            rtcpaddr.sin_family = AF_INET;
            rtcpaddr.sin_port = htons(rtcpport);
            rtcpaddr.sin_addr.s_addr = htonl(ip);
        }

        RTPIPv4Destination::ip = ip;
    }



        int RTPSession::AddDestination(const RTPAddress &addr,const RTPIPv4Address &rtcpaddr)
{
    if (!created)
        return ERR_RTP_SESSION_NOTCREATED;
    return rtptrans->AddDestination(addr,rtcpaddr);
}

       在调用RTPSession::AddDestination、定义RTPIPv4Destination的时候实参也相应增加目的rtcp参数。




       这样改造之后就可以自定义独立的设置目的地址rtp ,rtcp端口了。

5 jrtplib 移植中遇到的问题及解决方法

   把jrtplib移植到arm11平台，遇到一些问题，如下。
5.1 字节序的问题
     jrtplib中的报头的字节序问题，网上可以搜到一些，但都是只言片语，没有详细的把解决方案说出来。ARM采用的是Big-Endian, 而X86采用的是Little-Endian。目前我所采用的解决方法是让两台互相通信的主机所使用的jrtplib的Endian格式一致，都为 Big-Endian或都为Little-Endian，这只需要修改jrtplib-version/src/rtpconfig_unix.h 文件，默认是采用的Little-Endian方式，里面的注释部分有说若加
#define RTP_BIG_ENDIAN
表示采用Big-Endian的字节方式，否则默认为Little-Endian方式。至于原因还没弄清楚。可以发邮件给作者问一下。

5.2 Can't retrieve login name的错误
    上述都没有问题了，又遇到另外的问题，在N800的客户端建立RTPSession的过程中，报了Can't retrieve login name的错误，在网上搜索后，找到一篇博客讲到嵌入式系统由于某些原因系统可能没有login name, 而在RTPSession的Create->InternalCreate->CreateCNAME方法，其中的getlogin_r, getlogin和getenv操作会因为logname为空而返回ERR_RTP_SESSION_CANTGETLOGINNAME的错误。我在 N800的机器上做了个实验，使用getlogin和getenv("LOGNAME")确实都不能得到登录名。要解决上述问题，可以对jrtplib的 源代码进行修改， 即修改RTPSession的CreateCNAME，即使getlogin_r, getlogin和getenv三个函数调用都不能得到登录名，也要设置一个登录名。









