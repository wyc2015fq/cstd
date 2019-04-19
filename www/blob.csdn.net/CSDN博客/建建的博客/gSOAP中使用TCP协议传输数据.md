# gSOAP中使用TCP协议传输数据 - 建建的博客 - CSDN博客
2017年02月17日 15:49:06[纪建](https://me.csdn.net/u013898698)阅读数：853
http://blog.csdn.net/flyfish1986/article/details/7626169
    gSOAP中使用TCP协议传输数据
  2012-5-31 邵盛松
 一  模型
  TCP/IP是一个协议族（Internet protocol suite），包含众多的协议，传输控制协议(TCP)和网际协议(IP)分属不同的层次，是保证数据完整传输的两个基本的重要协议。
![](https://img-my.csdn.net/uploads/201206/02/1338626388_3005.png)
在上图TCP/IP模型与SOAP模型中 层级是一一对应的
不同的协议层对数据有不同的称谓，在传输层叫做段（segment），在网络层叫做数据报（datagram），在链路层叫做帧（frame）。数据封装成帧后发到传输介质上，到达目的主机后每层协议再剥掉相应的头部，最后将应用层数据交给应用程序处理。
在SOAP中传输层可以使用TCP协议也可以使用UDP协议，gSOAP支持TCP和UDP
下图采用TCP传输数据
![](https://img-my.csdn.net/uploads/201206/02/1338626443_2042.png)
二 TCP Header结构
![](https://img-my.csdn.net/uploads/201206/02/1338626467_5611.png)
控制位的描述
URG(urgent紧急)
ACK(acknowledgement 确认) 
PSH(push传送)
RST(reset重置) 
SYN(synchronous建立联机) 
FIN(finish结束) 
三 TCP的连接 俗称三次握手
![](https://img-my.csdn.net/uploads/201206/02/1338626488_6982.png)
简单理解
C对S说： 我可以连接你吗？
S对C说 ：可以的。
C对S说：OK，我连接了。
具体描述如下
描述中包括TCP Header结构中的32位序号(Sequence number) ,32位确认序号(Acknowledge number),SYN,ACK.
　　第一次握手 Client发送位码为syn=1,随机产生seq number的数据包到server，server由SYN=1知道，Client要求连接;
　　第二次握手 server收到请求后要确认连接信息，向Client发送ack number=(Client的seq+1),syn=1,ack=1,随机产生seq number的包
　　第三次握手 Client收到后检查ack number是否正确，即第一次发送的seq number+1,以及ack是否为1，若正确，Client会再发送ack number=(server的seq+1),ack=1，server收到后确认seq值与ack=1则连接建立成功。
四 TCP的关闭 俗称四次挥手
TCP连接是全双工的，可以同时发送和接受数据，关闭的时候要关闭这两个方向的通道
下图是 Client主动关闭连接
![](https://img-my.csdn.net/uploads/201206/02/1338626524_2101.png)
  第一次挥手：Client给Server发送FIN，请求关闭连接 
  第二次挥手：Server收到FIN之后给Client返回确认ACK，同时关闭Receive通道，Client收到对自己的FIN确认后，关闭Send通道
  第三次挥手： Server关闭连接，给Client发送FIN
  第四次挥手：Client收到后给Server回复ACK确认，同时Client关闭Receive通道，进入TIME_WAIT状态。Server接收到Client对自己的FIN的确认ACK，关闭Send通道
状态的说明
CLOSE_WAIT： 表示在等待关闭。当主动关闭连接的一方关闭SOCKET后发送FIN报文给被动关闭一方，被动关闭一方回应一个ACK报文给对方，此时被动关闭一方则进入到CLOSE_WAIT状态
FIN_WAIT_1：是当Socket在已经连接的状态时主动关闭连接，向对方发送了FIN报文，此时该Socket进入到FIN_WAIT_1状态。而当对方回应ACK报文后，则进入到FIN_WAIT_2状态
FIN_WAIT_2：表示半连接，挥了两次手的状态等待对方的Fin报文
TIME_WAIT：TCP协议中主动关闭连接的一方要处于TIME_WAIT状态，等待两个MSL（maximum segment lifetime）的时间后才能回到CLOSED状态，在TIME_WAIT期间仍然不能再次监听同样的server端口。
LAST_ACK: 被动关闭一方在发送FIN报 文后，最后等待对方的ACK报文。当收到ACK报文后进入CLOSED状态。
CLOSED：已经完全关闭
     gSOAP中使用TCP协议传输数据
  2012-5-31 邵盛松
 一  模型
  TCP/IP是一个协议族（Internet protocol suite），包含众多的协议，传输控制协议(TCP)和网际协议(IP)分属不同的层次，是保证数据完整传输的两个基本的重要协议。
![](https://img-my.csdn.net/uploads/201206/02/1338626388_3005.png)
在上图TCP/IP模型与SOAP模型中 层级是一一对应的
不同的协议层对数据有不同的称谓，在传输层叫做段（segment），在网络层叫做数据报（datagram），在链路层叫做帧（frame）。数据封装成帧后发到传输介质上，到达目的主机后每层协议再剥掉相应的头部，最后将应用层数据交给应用程序处理。
在SOAP中传输层可以使用TCP协议也可以使用UDP协议，gSOAP支持TCP和UDP
下图采用TCP传输数据
![](https://img-my.csdn.net/uploads/201206/02/1338626443_2042.png)
二 TCP Header结构
![](https://img-my.csdn.net/uploads/201206/02/1338626467_5611.png)
控制位的描述
URG(urgent紧急)
ACK(acknowledgement 确认) 
PSH(push传送)
RST(reset重置) 
SYN(synchronous建立联机) 
FIN(finish结束) 
三 TCP的连接 俗称三次握手
![](https://img-my.csdn.net/uploads/201206/02/1338626488_6982.png)
简单理解
C对S说： 我可以连接你吗？
S对C说 ：可以的。
C对S说：OK，我连接了。
具体描述如下
描述中包括TCP Header结构中的32位序号(Sequence number) ,32位确认序号(Acknowledge number),SYN,ACK.
　　第一次握手 Client发送位码为syn=1,随机产生seq number的数据包到server，server由SYN=1知道，Client要求连接;
　　第二次握手 server收到请求后要确认连接信息，向Client发送ack number=(Client的seq+1),syn=1,ack=1,随机产生seq number的包
　　第三次握手 Client收到后检查ack number是否正确，即第一次发送的seq number+1,以及ack是否为1，若正确，Client会再发送ack number=(server的seq+1),ack=1，server收到后确认seq值与ack=1则连接建立成功。
四 TCP的关闭 俗称四次挥手
TCP连接是全双工的，可以同时发送和接受数据，关闭的时候要关闭这两个方向的通道
下图是 Client主动关闭连接
![](https://img-my.csdn.net/uploads/201206/02/1338626524_2101.png)
  第一次挥手：Client给Server发送FIN，请求关闭连接 
  第二次挥手：Server收到FIN之后给Client返回确认ACK，同时关闭Receive通道，Client收到对自己的FIN确认后，关闭Send通道
  第三次挥手： Server关闭连接，给Client发送FIN
  第四次挥手：Client收到后给Server回复ACK确认，同时Client关闭Receive通道，进入TIME_WAIT状态。Server接收到Client对自己的FIN的确认ACK，关闭Send通道
状态的说明
CLOSE_WAIT： 表示在等待关闭。当主动关闭连接的一方关闭SOCKET后发送FIN报文给被动关闭一方，被动关闭一方回应一个ACK报文给对方，此时被动关闭一方则进入到CLOSE_WAIT状态
FIN_WAIT_1：是当Socket在已经连接的状态时主动关闭连接，向对方发送了FIN报文，此时该Socket进入到FIN_WAIT_1状态。而当对方回应ACK报文后，则进入到FIN_WAIT_2状态
FIN_WAIT_2：表示半连接，挥了两次手的状态等待对方的Fin报文
TIME_WAIT：TCP协议中主动关闭连接的一方要处于TIME_WAIT状态，等待两个MSL（maximum segment lifetime）的时间后才能回到CLOSED状态，在TIME_WAIT期间仍然不能再次监听同样的server端口。
LAST_ACK: 被动关闭一方在发送FIN报 文后，最后等待对方的ACK报文。当收到ACK报文后进入CLOSED状态。
CLOSED：已经完全关闭
