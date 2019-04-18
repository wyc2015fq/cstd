# time_wait和close_wait - gauss的专栏 - CSDN博客
2015年04月16日 13:48:10[gauss](https://me.csdn.net/mathlmx)阅读数：274
个人分类：[C/C++/内存管理																[服务器架构与网络编程](https://blog.csdn.net/mathlmx/article/category/944322)](https://blog.csdn.net/mathlmx/article/category/605906)
总结：
一端忘记close，将造成另一端大量的close_wait的状态。
主动执行close的一端，在量特别大的情况下，对so_linger没有做设置，将造成大量的time_wait状态的连接。
TCP状态转移要点
TCP协议规定，对于已经建立的连接，网络双方要进行四次握手才能成功断开连接，如果缺少了其中某个步骤，将会使连接处于假死状态，连接本身占用的资源不会被释放。网络服务器程序要同时管理大量连接，所以很有必要保证无用连接完全断开，否则大量僵死的连接会浪费许多服务器资源
客户端TCP状态迁移：
CLOSED->SYN_SENT->ESTABLISHED->FIN_WAIT_1->FIN_WAIT_2->TIME_WAIT->CLOSED
服务器TCP状态迁移：
CLOSED->LISTEN->SYN收到->ESTABLISHED->CLOSE_WAIT->LAST_ACK->CLOSED
当客户端开始连接时，服务器还处于LISTENING，客户端发一个SYN包后，他就处于SYN_SENT状态,服务器就处于SYS收到状态,然后互相确认进入连接状态ESTABLISHED.
1、LISTENING状态
　服务启动后首先处于侦听（LISTENING）状态。
2、ESTABLISHED状态
　ESTABLISHED的意思是建立连接。表示两台机器正在通信。
3、CLOSE_WAIT
对方主动关闭连接或者网络异常导致连接中断，这时我方的状态会变成CLOSE_WAIT 此时我方要调用close()来使得连接正确关闭
4、TIME_WAIT
我方主动调用close()断开连接，收到对方确认后状态变为TIME_WAIT，缺省为240秒。TCP协议规定TIME_WAIT状态会一直持续2MSL(即两倍的分段最大生存期)，以此来确保旧的连接状态不会对新连接产生影响。处于TIME_WAIT状态的连接占用的资源不会被内核释放，所以作为服务器，在可能的情况下，尽量不要主动断开连接，以减少TIME_WAIT状态造成的资源浪费。
目前有一种避免TIME_WAIT资源浪费的方法，就是关闭socket的LINGER选项。但这种做法是TCP协议不推荐使用的，在某些情况下这个操作可能会带来错误
断开连接的时候， 当发起主动关闭的左边这方发送一个FIN过去后，右边被动关闭的这方要回应一个ACK，这个ACK是TCP回应的，而不是应用程序发送的，此时，被动关闭的一方就处于CLOSE_WAIT状态了。如果此时被动关闭的这一方不再继续调用closesocket,那么他就不会发送接下来的FIN，导致自己老是处于CLOSE_WAIT。只有被动关闭的这一方调用了closesocket,才会发送一个FIN给主动关闭的这一 方，同时也使得自己的状态变迁为LAST_ACK。
出现大量CLOSE_WAIT的原因很简单，就是某一方在网络连接断开后，没有检测到这个错误，没有执行closesocket，导致了这个状态的实现，这在TCP/IP协议的状态变迁图上可以清楚看到。同时和这个相对应的还有一种叫TIME_WAIT的。**一端的Socket调用close后，另一端的Socket没有调用close**
另外，把SOCKET的SO_LINGER设置为0秒拖延（也就是立即关闭）在很多时候是有害处的。 
还有，把端口设置为可复用是一种不安全的网络编程方法
当主动关闭的一方发送FIN到被动关闭这边后，被动关闭这边的TCP马上回应一个ACK过去，同时向上面应用程序提交一个ERROR，导 致上面的SOCKET的send或者recv返回SOCKET_ERROR，正常情况下，如果上面在返回SOCKET_ERROR后调用了closesocket,那么被动关闭的者一方的TCP就会发送一个FIN过去，自己的状态就变迁到LAST_ACK
![image](http://lh6.google.com/image/pengjiaheng/Rf_SsNsyXiI/AAAAAAAAABs/4SXbYlu3v3s/TCP-State.JPG?imgmax=576)
虚线和实线分别对应服务器端(被连接端)和客户端端(主动连接端)。
结合上图使用netstat -na命令即可知道到当前的TCP连接状态。一般LISTEN、ESTABLISHED、TIME_WAIT是比较常见。
**分析：**
这个问题主要因为TCP的结束流程未走完，造成连接未释放。现设客户端主动断开连接，流程如下
**Client 消息 Server**
close()
------ FIN ------->
FIN_WAIT1 CLOSE_WAIT
<----- ACK -------
FIN_WAIT2 
close()
<------ FIN ------ 
TIME_WAIT LAST_ACK
------ ACK -------> 
CLOSED
CLOSED
如上图所示，由于Server的Socket在客户端已经关闭时而没有调用关闭，造成服务器端的连接处在“挂起”状态，而客户端则处在等待应答的状态上。此问题的典型特征是：**一端处于FIN_WAIT2 ，而另一端处于CLOSE_WAIT**
于基于TCP的HTTP协议，关闭TCP连接的是Server端，这样，Server端会进入TIME_WAIT状态，可 想而知，对于访问量大的Web Server，会存在大量的TIME_WAIT状态，假如server一秒钟接收1000个请求，那么就会积压240*1000=240，000个TIME_WAIT的记录，维护这些状态给Server带来负担。当然现代操作系统都会用快速的查找算法来管理这些TIME_WAIT，所以对于新的TCP连接请求，判断是否hit中一个TIME_WAIT不会太费时间，但是有这么多状态要维护总是不好。
