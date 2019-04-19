# linux下基于jrtplib库的实时传送实现 - maopig的专栏 - CSDN博客
2011年08月07日 16:21:06[maopig](https://me.csdn.net/maopig)阅读数：2014
linux 下基于jrtplib库的实时传送实现
一、RTP 是进行实时流媒体传输的标准协议和关键技术
 实时传输协议（Real-time Transport Protocol，PRT）是在 Internet 上处理多媒体数据流的一种网络协议，利用它能够在一对一（unicast，单播）或者一对多（multicast，多播）的网络环境中实现传流媒体数据的实时传输。RTP 通常使用 UDP 来进行多媒体数据的传输，但如果需要的话可以使用 TCP 或者 ATM 等其它协议。
 协议分析 ：每一个RTP数据报都由头部（Header）和负载（Payload）两个部分组成，其中头部前 12 个字节的含义是固定的，而负载则可以是音频或者视频数据。
      RTP 是目前解决流媒体实时传输问题的最好办法，要在 Linux 平台上进行实时传送编程，可以考虑使用一些开放源代码的 RTP 库，如 LIBRTP、JRTPLIB 等。JRTPLIB 是一个面向对象的 RTP 库，它完全遵循 RFC 1889 设计，在很多场合下是一个非常不错的选择。JRTPLIB 是一个用 C++ 语言实现的 RTP 库，这个库使用socket 机制实现网络通讯 因此可以运行在 Windows、Linux、FreeBSD、Solaris、Unix和VxWorks 等多种操作系统上。
二、JRTPLIB 库的使用方法及程序实现
 (1)JRTPLIB  函数 的使用
 a、在使用 JRTPLIB 进行实时流媒体数据传输之前，首先应该生成 RTPSession 类的一个实例来表示此次 RTP 会话，然后调用 Create() 方法来对其进行初始化操作。RTPSession 类的 Create() 方法只有一个参数，用来指明此次 RTP 会话所采用的端口号。
 RTPSession sess;  sess.Create(5000); 
 b、设置恰当的时戳单元，是 RTP 会话初始化过程所要进行的另外一项重要工作，这是通过调用 RTPSession 类的 SetTimestampUnit() 方法来实现的，该方法同样也只有一个参数，表示的是以秒为单元的时戳单元。
 sess.SetTimestampUnit(1.0/8000.0);
 c、当 RTP 会话成功建立起来之后，接下去就可以开始进行流媒体数据的实时传输了。首先需要设置好数据发送的目标地址，RTP 协议允许同一会话存在多个目标地址，这可以通过调用 RTPSession 类的 AddDestination()、DeleteDestination() 和 ClearDestinations() 方法来完成。例如，下面的语句表示的是让 RTP 会话将数据发送到本地主机的 6000 端口： 
 unsigned long addr = ntohl(inet_addr("127.0.0.1")); 
 sess.AddDestination(addr, 6000);
 d、目标地址全部指定之后，接着就可以调用 RTPSession 类的 SendPacket() 方法，向所有的目标地址发送流媒体数据。SendPacket() 是 RTPSession 类提供的一个重载函数
对于同一个 RTP 会话来讲，负载类型、标识和时戳增量通常来讲都是相同的，JRTPLIB 允许将它们设置为会话的默认参数，这是通过调用 RTPSession 类的 SetDefaultPayloadType()、SetDefaultMark() 和 SetDefaultTimeStampIncrement() 方法来完成的。为 RTP 会话设置这些默认参数的好处是可以简化数据的发送，例如，如果为 RTP 会话设置了默认参数： 
 sess.SetDefaultPayloadType(0);
  sess.SetDefaultMark(false);  
 sess.SetDefaultTimeStampIncrement(10);
之后在进行数据发送时只需指明要发送的数据及其长度就可以了： 
 sess.SendPacket(buffer, 5); 
 e、对于流媒体数据的接收端，首先需要调用 RTPSession 类的 PollData() 方法来接收发送过来的 RTP 或者 RTCP 数据报。由于同一个 RTP 会话中允许有多个参与者（源），你既可以通过调用 RTPSession 类的 GotoFirstSource() 和 GotoNextSource() 方法来遍历所有的源，也可以通过调用 RTPSession 类的 GotoFirstSourceWithData() 和 GotoNextSourceWithData() 方法来遍历那些携带有数据的源。在从 RTP 会话中检测出有效的数据源之后，接下去就可以调用 RTPSession 类的 GetNextPacket() 方法从中抽取 RTP 数据报，当接收到的 RTP 数据报处理完之后，一定要记得及时释放。
JRTPLIB 为 RTP 数据报定义了三种接收模式，其中每种接收模式都具体规定了哪些到达的 RTP 数据报将会被接受，而哪些到达的 RTP 数据报将会被拒绝。通过调用 RTPSession 类的 SetReceiveMode() 方法可以设置下列这些接收模式： 
? RECEIVEMODE_ALL　　缺省的接收模式，所有到达的 RTP 数据报都将被接受； 
? RECEIVEMODE_IGNORESOME　　除了某些特定的发送者之外，所有到达的 RTP 数据报都将被接受，而被拒绝的发送者列表可以通过调用 AddToIgnoreList()、DeleteFromIgnoreList() 和 ClearIgnoreList() 方法来进行设置； 
? RECEIVEMODE_ACCEPTSOME　　除了某些特定的发送者之外，所有到达的 RTP 数据报都将被拒绝，而被接受的发送者列表可以通过调用 AddToAcceptList ()、DeleteFromAcceptList 和 ClearAcceptList () 方法来进行设置。 下面是采用第三种接收模式的程序示例。
 if (sess.GotoFirstSourceWithData()) {   
  do {   
   sess.AddToAcceptList(remoteIP, allports,portbase);
          sess.SetReceiveMode(RECEIVEMODE_ACCEPTSOME);
    RTPPacket *pack;         
    pack = sess.GetNextPacket();            // 处理接收到的数据    
    delete pack;   } 
  while (sess.GotoNextSourceWithData()); 
  }
  （2）程序流程图
发送：获得接收端的 IP 地址和端口号        创建 RTP 会话        指定 RTP 数据接收端 设置 RTP 会话默认参数   发送流媒体数据
接收：获得用户指定的端口号  创建RTP会话  设置接收模式  接受RTP数据  检索RTP数据源  获取RTP数据报  删除RTP数据报
三、环境搭建及编译方法
（1）Toolchain的安装
 首先找到xscale-arm-toolchain.tgz文件，假设该文件包放在/tmp/下
 #cd /
 #tar -zxvf /tmp/xscale-arm-toolchain.tgz
 再设置环境变量
 #export PATH=/usr/local/arm-linux/bin:$PATH
 最后检查一下交叉编译工具是否安装成功
 #arm-linux-g++ --version
 看是否显示arm-linux-g++的版本，如有则安装成功。
（2）JRTPLIB 库的交叉编译及安装
 首先从 JRTPLIB 的网站（[http://lumumba.luc.ac.be/jori/jrtplib/jrtplib.htmll](http://lumumba.luc.ac.be/jori/jrtplib/jrtplib.htmll)） 下载最新的源码包，此处使用的是jrtplib-2.8.tar，假设下载后的源码包放在/tmp下，执 行下面的命令对其解压缩：
 #cd /tmp
 #tar -zxvf jrtplib-2.8.tar
 然后要对jrtplib进行配置和编译
 #cd jrtplib-2.8
 #./configure CC=arm-linux-g++ cross-compile=yes
 修改Makefile文件
 将链接命令ld 和ar改为arm-linux-ld和 arm-linux-ar
 #make
 最后再执行如下命令就可以完成 JRTPLIB 的安装：
 #make install
(3)程序编译
 a、配置编译环境
 可以用export来配置，也可以用编写Makefile的方法。这里采用Makefile。
 编写Make*file&:*
INCL = -I/usr/local/include
CFLAGS = -pipe -O2 -fno-strength-reduce
LFLAGS = /usr/local/lib/libjrtp.a -L/usr/X11R6/lib
LIBS = -LX11 -LXext /usr/local/lib/libjrtp.a
CC = arm-linux-g++
main:main.o
 $(CC) $(LFLAGS) $(INCL) -o main main.o $(LIBS)
main.o:main.cpp
clean:
 rm -f main
 rm -f *.o
.SUFFIXES:.cpp
.cpp.o:
 $(CC) -c $(CFLAGS) $(INCL) -o $@ $<         /*  $@表示目标的完整名字      */
          /* $<表示第一个依赖文件的名字 */
 b、编译
 假设发送和接收程序分别放在/tmp/send和/tmp/receive目录下
 #cd /tmp/send
 #make
 #cd /tmp/receive
 #make
四、易出错误及注意问题
 1、找不到一些标准的最 基本的一些头文件。
  主要是因为Toolchain路径没安装对，要 严格按照步骤安装。
 2、找不到使用的jrtplib库中的一些头文件。
  在 jrtplib的安装目录下，include路径下不能再有别的目录。
 3、recieve函数接收数据包不能正确提出所要数据。
  由于每一个RTP数据报都由头部（Header）和负载（Payload）两个部分组成，若使用getrawdata()是返回整个数据包的数据，包含传输媒体的类型、格式、序列号、时间戳以及是否有附加数据等信息。getpayload()函数是返回所发送的数据。两者一定要分清。
 4、设置RECEIVEMODE_ACCEPTSOME　　接收模式后，运行程序接收端不能接包。
  IP地址格式出了问题。iner_addr()与ntohl()函数要用对，否则参数传不进去，接受列表中无值，当然接收不了数据包。
 5、编译通过，但测试时接收端不能接收到数据。
  可能是接收机防火墙未关闭。运行：
  #iptables -F
  也可能是IP地址没有设置好。运行：
  #ifocnfig eth0  *.*.*.*  netmask *.*.*.*
 6、使用jrtolib库时，在程序中include 后最好加上库所在的路径。
五、程序
send:
#include <stdio.h>
#include <string.h>
#include "rtpsession.h"
// 错误处理函数
void checkerror(int err)
{
  if (err < 0) {
    char* errstr = RTPGetErrorString(err);
    printf("Error:%s\\n", errstr);
    exit(-1);
  }
}
int main(int argc, char** argv)
{
  RTPSession sess;
  unsigned long destip;
  int destport;
  int portbase = 6000;
  int status, index;
  char buffer[128];
  if (argc != 3) {
    printf("Usage: ./sender destip destport\\n");
    return -1;
  }
  // 获得接收端的IP地址和端口号
  destip = inet_addr(argv[1]);
  if (destip == INADDR_NONE) {
    printf("Bad IP address specified.\\n");
    return -1;
  }
  destip = ntohl(destip);
  destport = atoi(argv[2]);
  // 创建RTP会话
  status = sess.Create(portbase);
  checkerror(status);
  // 指定RTP数据接收端
  status = sess.AddDestination(destip, destport);
  checkerror(status);
  // 设置RTP会话默认参数
  sess.SetDefaultPayloadType(0);
  sess.SetDefaultMark(false);
  sess.SetDefaultTimeStampIncrement(10);
  // 发送流媒体数据
  index = 1;
  do {
    sprintf(buffer, "%d: RTP packet", index ++);
    sess.SendPacket(buffer, strlen(buffer));
    printf("Send packet !\\n");
  } while(1);
  return 0;
}
receive:
#include <stdio.h>
#include "rtpsession.h"
#include "rtppacket.h"
// 错误处理函数
void checkerror(int err)
{
  if (err < 0) {
    char* errstr = RTPGetErrorString(err);
    printf("Error:%s\\n", errstr);
    exit(-1);
  }
}
int main(int argc, char** argv)
{
  RTPSession sess;
  int localport,portbase;
  int status;
  unsigned long remoteIP;
  if (argc != 4) {
    printf("Usage: ./sender localport\\n");
    return -1;
  }
   // 获得用户指定的端口号
  remoteIP = inet_addr(argv[1]);
  localport = atoi(argv[2]);
  portbase = atoi(argv[3]);
  // 创建RTP会话
  status = sess.Create(localport);
  checkerror(status);
  //RTPHeader *rtphdr;
  unsigned long timestamp1;
  unsigned char * RawData;
  unsigned char temp[30];
  int lengh ,i;
  bool allports = 1;
  sess.AddToAcceptList(remoteIP, allports,portbase);
     do {
 //设置接收模式
        sess.SetReceiveMode(RECEIVEMODE_ACCEPTSOME);
   sess.AddToAcceptList(remoteIP, allports,portbase);
    // 接受RTP数据
    status = sess.PollData();
 // 检索RTP数据源
    if (sess.GotoFirstSourceWithData()) {
      do {
        RTPPacket* packet;
        // 获取RTP数据报
        while ((packet = sess.GetNextPacket()) != NULL) {
          printf("Got packet !\n");
   timestamp1 = packet->GetTimeStamp();
   lengh=packet->GetPayloadLength();
   RawData=packet->GetPayload();
   for(i=0;i<lengh;i++){
      temp[i]=RawData[i];
  printf("%c",temp[i]);
   }
   temp[i]='\0';
   printf("  timestamp: %d lengh=%d data:%s\n",timestamp1,lengh,&temp);
          // 删除RTP数据报
          delete packet;
        }
      } while (sess.GotoNextSourceWithData());
    }
  } while(1);
  return 0;
}
