# TCP/IP包头详解 - 星爷2401 - CSDN博客
2017年05月09日 17:56:53[星爷2401](https://me.csdn.net/leifukes)阅读数：1067
IP很TCP包头都至少包含20个字节。每个byte=8bit
**IP属于网络层协议，TCP属于传输层协议，TCP数据包是包含在IP数据包中的。**
## 先看IP协议：
![](https://img-blog.csdn.net/20170509171934562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
《这样的图片网上很多了，能看懂多少就看懂多少，后面会详细介绍。》
来一个实际的tcpdump抓包，把十六进制打印出来
`tcpdump -i bond0 port 2021 -X -s0`
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on bond0, link-type EN10MB (Ethernet), capture size 65535 bytes
16:46:47.765728 IP 192.168.72.215.51992 > jelly02.servexec: Flags [S], seq 2859784034, win 65535, options [mss 1460,nop,wscale 5,nop,nop,TS val 1135145917 ecr 0,sackOK,eol], length 0
0x0000:  4500 0040 0ab0 4000 3c06 5874 c0a8 48d7  E..@..@.<.Xt..H.
0x0010:  0a0a c80a cb18 07e5 aa74 d762 0000 0000  .........t.b....
0x0020:  b002 ffff cf2d 0000 0204 05b4 0103 0305  .....-..........
0x0030:  0101 080a 43a8 f3bd 0000 0000 0402 0000  ....C...........
第一行：0x0000：4500 0040 0ab0 4000 3c06 5874 c0a8 48d7 
第一个字节分为两段，0-3 4个bit 代表ip协议（version），比如第一个字节的十六进制是45，4就代表ipv4，5就代表IP头部的（Source
 Address）了，也就是我的IP地址。长度，IHL一共有4bit，最大就是1111，那么IP头部的最大长度就是60字节。5代表的是20个字节。
第二个字节是服务类型（type of service），一般是0，表示一般服务，详细的暂不介绍。
第三个是2个字节的代表IP数据报文总长度, 0040 转换成10进制就是64个字节。每行16个字节一共4行 16*4 就是64个字节。
第四个是2个字节0ab0让目的主机判断属于那个分组。（先记住这么回事，如果用到再具体分析）
第五个是2个字节4000，转换为二进制就是“0100 0000”，其中第一位是IP协议目前没有用上的，为0。接着的是两个标志DF和MF。DF为1表示不要分段，MF为1
第六个是2个字节3c06，3c代表Ip数据流的生命周期，06代表的是传输协议（Protocol），在RFC790中标示TCP协议。
第七个是2个字节5874，代表16bit的头校验和（Header Checksum）
第八个是4个字节 c0a8 48d7  代表原地址（Source Address）。
第二行：0x0010： 0a0a c80a cb18 07e5 aa74 d762 0000 0000
第一个是4个字节 0a0a c80a 代表目的地址
**到此为止IP协议的20个字节已经结束。**
**剩下的就是TCP协议：**
**![](https://img-blog.csdn.net/20170509175859040?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
继续刚才第二行：0x0010： 0a0a c80a cb18 07e5 aa74 d762 0000 0000
第一个是2个字节 cb18 代表源地址的端口号，转换成十进制正好是51992，同理后面的07e5代表目的地址的端口号，转换成十进制就是2021
第二个是4个字节 aa74 d762 代表的是顺序号码（sequence Number）简写SEQ。
第三个是4个字节 0000 0000代表的是确认号码ACK。
第三行：0x0020:  b002 ffff cf2d 0000 0204 05b4 0103 0305 
第一个是2个字节b002 ，总共16bit，转换为二进制是"1011000000000010"，含义很多，第一个4bit“1011”代表TCP的头长，十进制是11，代表44个字节。接着的6bit没有用到，后面6bit是标志位，代表ACK，SYN等。

