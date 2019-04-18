# 关于HttpServer的time-wait - gauss的专栏 - CSDN博客
2015年04月16日 13:47:23[gauss](https://me.csdn.net/mathlmx)阅读数：382
个人分类：[服务器架构与网络编程](https://blog.csdn.net/mathlmx/article/category/944322)
TIME_WAIT状态
根据TCP协议，主动发起关闭的一方，会进入TIME_WAIT状态，持续2*MSL(Max Segment Lifetime)，缺省为240秒，在这个post中简洁的介绍了为什么需要这个状态。
值得一说的是，对于基于TCP的HTTP协议，关闭TCP连接的是Server端，这样，Server端会进入TIME_WAIT状态，可想而知，对于访问量大的Web Server，会存在大量的TIME_WAIT状态，假如server一秒钟接收1000个请求，那么就会积压240*1000=240，000个 TIME_WAIT的记录，维护这些状态给Server带来负担。当然现代操作系统都会用快速的查找算法来管理这些TIME_WAIT，所以对于新的
 TCP连接请求，判断是否hit中一个TIME_WAIT不会太费时间，但是有这么多状态要维护总是不好。
HTTP协议1.1版规定default行为是Keep-Alive，也就是会重用TCP连接传输多个 request/response，一个主要原因就是发现了这个问题。还有一个方法减缓TIME_WAIT压力就是把系统的2*MSL时间减少，因为 240秒的时间实在是忒长了点，对于Windows，修改注册表，在HKEY_LOCAL_MACHINE\ SYSTEM\CurrentControlSet\Services\
 Tcpip\Parameters上添加一个DWORD类型的值TcpTimedWaitDelay，一般认为不要少于60，不然可能会有麻烦。
对于大型的服务，一台server搞不定，需要一个LB(Load Balancer)把流量分配到若干后端服务器上，如果这个LB是以NAT方式工作的话，可能会带来问题。假如所有从LB到后端Server的IP包的 source address都是一样的(LB的对内地址），那么LB到后端Server的TCP连接会受限制，因为频繁的TCP连接建立和关闭，会在server上留下TIME_WAIT状态，而且这些状态对应的remote
 address都是LB的，LB的source port撑死也就60000多个(2^16=65536,1~1023是保留端口，还有一些其他端口缺省也不会用），每个LB上的端口一旦进入 Server的TIME_WAIT黑名单，就有240秒不能再用来建立和Server的连接，这样LB和Server最多也就能支持300个左右的连接。如果没有LB，不会有这个问题，因为这样server看到的remote address是internet上广阔无垠的集合，对每个address，60000多个port实在是够用了。
一开始我觉得用上LB会很大程度上限制TCP的连接数，但是实验表明没这回事，LB后面的一台Windows Server 2003每秒处理请求数照样达到了600个，难道TIME_WAIT状态没起作用？用Net Monitor和netstat观察后发现，Server和LB的XXXX端口之间的连接进入TIME_WAIT状态后，再来一个LB的XXXX端口的 SYN包，Server照样接收处理了，而是想像的那样被drop掉了。翻书，从书堆里面找出覆满尘土的大学时代买的《UNIX
 Network Programming, Volume 1, Second Edition: Networking APIs: Sockets and XTI》，中间提到一句，对于BSD-derived实现，只要SYN的sequence number比上一次关闭时的最大sequence number还要大，那么TIME_WAIT状态一样接受这个SYN，难不成Windows也算BSD-derived?有了这点线索和关键字 (BSD)，找到这个post，在NT4.0的时候，还是和BSD-derived不一样的，不过Windows
 Server 2003已经是NT5.2了，也许有点差别了。
做个试验，用Socket API编一个Client端，每次都Bind到本地一个端口比如2345，重复的建立TCP连接往一个Server发送Keep-Alive=false 的HTTP请求，Windows的实现让sequence number不断的增长，所以虽然Server对于Client的2345端口连接保持TIME_WAIT状态，但是总是能够接受新的请求，不会拒绝。那如果SYN的Sequence
 Number变小会怎么样呢？同样用Socket API，不过这次用Raw IP，发送一个小sequence number的SYN包过去，Net Monitor里面看到，这个SYN被Server接收后如泥牛如海，一点反应没有，被drop掉了。
