# 设置RTSP, RTP和RTCP端口号 - 建建的博客 - CSDN博客
2017年10月14日 20:30:19[纪建](https://me.csdn.net/u013898698)阅读数：735
1.设置RTSP端口号
设置RTSP端口号的处理位于ARTSPConnection.cpp文件中，首先从url中获取端口号，如果读取不到，则设置为默认端口554。代码处理如下：
[cpp][view
 plain](http://blog.csdn.net/ameyume/article/details/7347363#)[copy](http://blog.csdn.net/ameyume/article/details/7347363#)
- ARTSPConnection::ParseURL(  
- 
- constchar *colonPos = strchr(host->c_str(), ':');  
- 
- if (colonPos != NULL) {  
-         unsigned long x;  
- if (!ParseSingleUnsignedLong(colonPos + 1, &x) || x >= 65536) { // rtsp端口必须小于65536
- returnfalse;  
-         }  
- 
-         *port = x;  
- 
- size_t colonOffset = colonPos - host->c_str();  
- size_t trailing = host->size() - colonOffset;  
-         host->erase(colonOffset, trailing);  
-     } else {  
-         *port = 554; // 如果从url中获取不到端口，则设置默认端口554
-     }  
--------------------------------------------------------------------------------------------------------
2.设置RTP和RTCP端口号
设置RTP和RTCP端口号的处理位于ARTPConnection.cpp文件中中，与设置RTSP端口号的处理分别在不同的文件中。
（1）函数声明：创建一对相邻端口的UDP数据报套接字。rtp套接字是偶数端口，rtcp套接字端口比rtp端口号大一个号。
[cpp][view
 plain](http://blog.csdn.net/ameyume/article/details/7347363#)[copy](http://blog.csdn.net/ameyume/article/details/7347363#)
- // Creates a pair of UDP datagram sockets bound to adjacent ports
- // (the rtpSocket is bound to an even port, the rtcpSocket to the
- // next higher port).
- staticvoid MakePortPair(  
- int *rtpSocket, int *rtcpSocket, unsigned *rtpPort);  
（2）函数定义：具体设置RTP和RTCP端口
[cpp][view
 plain](http://blog.csdn.net/ameyume/article/details/7347363#)[copy](http://blog.csdn.net/ameyume/article/details/7347363#)
- // static
- void ARTPConnection::MakePortPair(  
- int *rtpSocket, int *rtcpSocket, unsigned *rtpPort) {  
-     *rtpSocket = socket(AF_INET, SOCK_DGRAM, 0);  
-     CHECK_GE(*rtpSocket, 0);  
- 
-     bumpSocketBufferSize(*rtpSocket);  
- 
-     *rtcpSocket = socket(AF_INET, SOCK_DGRAM, 0);  
-     CHECK_GE(*rtcpSocket, 0);  
- 
-     bumpSocketBufferSize(*rtcpSocket);  
- 
-     unsigned start = (rand() * 1000)/ RAND_MAX + 15550; // 最小端口号为15550
-     start &= ~1; // 与1取反后的值做与操作，结果就是start变成偶数，端口从偶数开始
- 
- for (unsigned port = start; port < 65536; port += 2) { // rtp最大端口号为65535，此时rtcp端口号为65536
- struct sockaddr_in addr;  
-         memset(addr.sin_zero, 0, sizeof(addr.sin_zero));  
-         addr.sin_family = AF_INET;  
-         addr.sin_addr.s_addr = htonl(INADDR_ANY);  
-         addr.sin_port = htons(port);  
- 
- if (bind(*rtpSocket,  
-                  (conststruct sockaddr *)&addr, sizeof(addr)) < 0) {  
- continue;  
-         }  
- 
-         addr.sin_port = htons(port + 1);  
- 
- if (bind(*rtcpSocket,  
-                  (conststruct sockaddr *)&addr, sizeof(addr)) == 0) {  
-             *rtpPort = port;  
- return; // 绑定rtcp端口后返回
-         }  
-     }  
- 
-     TRESPASS();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
