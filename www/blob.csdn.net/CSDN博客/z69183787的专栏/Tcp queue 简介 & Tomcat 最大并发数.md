# Tcp queue 简介 & Tomcat 最大并发数 - z69183787的专栏 - CSDN博客
2018年07月25日 12:05:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：90
个人分类：[服务器-Tomcat																[Socket & Tcp/Ip](https://blog.csdn.net/z69183787/article/category/2178111)](https://blog.csdn.net/z69183787/article/category/2175825)
[https://www.cnblogs.com/liuxinan/p/5917168.html](https://www.cnblogs.com/liuxinan/p/5917168.html)
只针对BIO模式，目标请求会sleep两秒再返回结果，通过jmeter测试工具进行并发测试
操作系统：windows && linux
tomcat7测试：
<Connector port="8080" protocol="HTTP/1.1"
connectionTimeout="20000" maxThreads="1" acceptCount="2" 
redirectPort="8443" />
文档：http://localhost:8080/docs/config/http.html
官方解释：acceptCount对暂时无法执行的请求进行队列保存，超出设置则拒绝连接。
测试发现无法限制住最大并发数，所有请求都可以依次执行，每次有1个线程执行（maxThreads=1）
最大并发数如果过大，大于acceptCount值好几倍，会随机出现连接被拒绝。
进一步调查-----------------------------------------------------------------------------------------------------------------------------------
首先关于tcp queue简单介绍：
tcp三次握手：
第一次，客户端发送syn，等待服务端确认，此时客户端进入SYN_SEND状态
第二次，服务端接收syn包并确认，发送syn+ack给客户端，此时服务端进入SYN_RECV状态
第三次，客户端收到syn+ack，再次向服务端发送ack，此时客户端进入ESTAB状态（注意，此时服务端未必进入ESTAB状态）
半连接队列：服务端维护的与客户端保持SYN_RECV状态的连接队列，等待客户端回复，当收到客户端ack后，如果条件允许（全连接队列未达到最大值），服务端进入ESTAB状态，从半连接队列移到全连接队列的队尾。
全连接队列：完成三次握手等待accept。完成三次握手即进入了全连接队列的队尾，当进程调用accept时，全连接队列中的队头项将返回给进程，并从队列中移出连接。如果该队列为空，那么进程将被投入睡眠，直到TCP在该队列中放入一项才唤醒它。
在listen(int sockfd, int backlog)中，backlog在Linux 2.2之后表示的是已完成三次握手但还未被应用程序accept的队列长度。
**全连接队列满，半连接队列未满：**
客户端发出syn分节，服务端收下，并向客户端发出syn+ack。
客户端收到服务端syn+ack后，成为ESTAB状态，向服务端发送第三次握手ack。
服务端收到ack后，发现全连接队列已满，默认情况下，服务端什么也不做，状态依然是SYN_RECV。
客户端会重传syn和ack，当达到一定的阈值（/proc/sys/net/ipv4/tcp_synack_retries）时，客户端与服务端断开连接，服务端删除客户端在半连接队列中的syn分节。
**全连接、半连接队列都满：**
客户端发出syn分节，服务端发现半连接队列已满，直接丢弃syn，使客户端重传syn。
客户端重传syn，再次到达服务端后，服务端发现已经重传过，则收下，并告诉客户端syn+ack。
后续流程与上述一致，相比之下，多了一次客户端重传syn分节。
**修改tcp参数配置：**
tcp_synack_retries和tcp_syn_retries定义SYN 的重试连接次数
/etc/rc.d/rc.local文件中追加：
sysctl -w net.ipv4.tcp_synack_retries=3
sysctl -w net.ipv4.tcp_syn_retries=3
重启。
也可直接执行命令：
sysctl -w net.ipv4.tcp_synack_retries=1
sysctl -w net.ipv4.tcp_syn_retries=3
**关于队列的长度：**
半连接队列：**≈2 * min(backlog, net.ipv4.tcpmax_syn_backlog) **
全连接队列：**min(/proc/sys/net/core/somaxconn**（本系统128）, backlog)，表示最多有 min() + 1个 ESTAB 的连接等待 accept()。
修改somaxconn，
在/etc/sysctl.conf中添加如下：
net.core.somaxconn = 2048
然后在终端中执行
sysctl -p
进一步试验-------------------------------------------------------------------------------------------------------------------------------------------------------------------
在linux下，通过ss -ant进行观察tcp queue
![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160929153908578-1589018600.png)
并发5个请求
![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160929153706703-248115940.png)
acceptCount配置作为socket中的backlog的值，如果acceptCount不设置或者设置为0，则会取默认值，经测试是100。
全连接队列大小为 min(/proc/sys/net/core/somaxconn（本系统128）, 2) + 1=3，即等待服务端accept的ESTAB状态的连接最多有3个，如Recv-Q所示。
3个Recv-Q为323（表示请求bytes数值）的ESTAB状态的连接正等待server accept
1个Recv-Q为0的ESTAB状态的连接表示server端已经accept了请求，只是还没有返回结果（sleep中）。
1个SYN-RECV状态是半连接状态，位于半连接队列当中（此时客户端已经处于ESTAB状态），全连接队列已经满了。
对于处在半连接状态的连接，客户端会定时重发，直至达到阈值，如下：
第一次握手
![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160930150818000-1242898565.png)
 第二次握手
![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160930150944328-52089697.png)
 第三次握手
![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160930151025985-1824045426.png)
 客户端发送数据包
![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160930151252531-1277910419.png)
但由于此时服务端未能将连接从半连接队列移至全连接队列，状态依然是SYN-RECV，即未得到服务端ack，因此客户端继续发数据包
![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160930151622828-642297762.png)
发送几次之后，服务端有了回应,告诉客户端说，你之前发的包丢了
![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160930151715766-484558681.png)
然后客户端也告诉服务端，丢失的状态、对应的id号及次数，这里的43就是第三次握手的43，失败次数是1
![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160930152149188-1098940673.png)
然后客户端又开始重传了
![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160930152336563-694529676.png)
终于服务端受不了了，此时客户端服务端彻底断开。![](https://images2015.cnblogs.com/blog/894129/201609/894129-20160930152434453-489594517.png)
通过调整ipv4.tcp_synack_retries和ipv4.tcp_syn_retries，可以增加重试次数
结论：
对于tomcat中的acceptCount只是全连接队列的大小，就是说客户端和服务端都已经是ESTAB状态的连接，不考虑connectionTime的情况，在此队列中的连接最终都会被处理。对于大于acceptCount的连接请求，如果在tcp重试阈值范围之内完成半连接到全连接的状态转换，那么还是有机会被服务端accept并处理的。
因此，不能说只要大于acceptCount的连接就一定被拒绝！
参考文献：
http://www.cnblogs.com/leezhxing/p/5329786.html
http://blog.chinaunix.net/uid-24782829-id-3456109.html
http://www.cnblogs.com/menghuanbiao/p/5212131.html
http://www.cnblogs.com/zengkefu/p/5606696.html 有详细的源码分析
