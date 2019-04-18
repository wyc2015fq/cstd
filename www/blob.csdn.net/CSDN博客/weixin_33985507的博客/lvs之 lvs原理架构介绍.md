# lvs之 lvs原理架构介绍 - weixin_33985507的博客 - CSDN博客
2017年06月15日 13:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**一、 概念**
lvs的术语：
 Router:GWIP
 vs:virtual server,director
 rs:real server 
 CIP:client IP
 VIP:virtual server IP
 DIP:ditecter IP(connect with rs)
 RIP:real server IP
 用户请求的IP一定是VIP,否则vs就失去了负载均衡的调度意义
LVS方式的cluster从结构上可分为两部分：前端的负载均衡器(称之为director)和后端的真实服务器(称之为real server)。cluster前端的director将来自外界的请求调度到cluster后端不同的real server去执行。real server负责真正的提供各种应用服务，比如：Web、FTP、Mail等服务。real server的数量可以根据实际需求进行增加、减少。
**二、lvs的工作过程**
**![](https://images2015.cnblogs.com/blog/824142/201706/824142-20170615130149743-1442068345.png)**
**三、lvs的类型**
lvs有三种通用标准模型
(1)lvs-nat (网络地址映射)
(2)lvs-dr  (直接路由)
(3)lvs-tun (IP隧道)
3.1、  LVS NAT的特性(实质是多目标的DNAT)：
1、RS应该使用私有地址；
2、RS的网关的必须指向DIP；
3、RIP和DIP必须在同一网段内；
4、请求和响应的报文都得经过Director；在高负载场景中，Director很可能成为系统性能瓶颈；
5、支持端口映射；
6、RS可以使用任意支持集群服务的OS；	
 lvs-nat：工作流程如图：
![](https://images2015.cnblogs.com/blog/824142/201706/824142-20170615130702931-1460912384.png)
3.2、  LVS DR类型的特性：
	1、RS可以使用私有地址；但也可以使用公网地址，此时可以直接通过互联网连入RS以实现配置、监控等；
	2、RS的网关一定不能指向DIP；
	3、RS跟Dirctory要在同一物理网络内（不能由路由器分隔，因为VS通过封装MAC地址到RS）；
	4、请求报文经过Directory，但响应报文一定不经过Director
	5、不支持端口映射；
	6、RS可以使用大多数的操作系统；
由于DR类型中,VS、RS的VIP都是一样，如果在同一网段内会造成地址冲突，因此要解决地址冲突有一下三种方法：
	禁止RS响应对VIP的ARP广播请求：
	1、在前端路由上实现静态MAC地址VIP的绑定；
		前提：得有路由器的配置权限；
		缺点：Directory故障转时，无法更新此绑定；
	2、arptables
		前提：在各RS在安装arptables程序，并编写arptables规则
		缺点：依赖于独特功能的应用程序
	3、修改Linux内核参数
		前提：RS必须是Linux；
		缺点：适用性差；
		两个参数：
			arp_announce：定义通告模式
			arp_ignore：定义收到arp请求的时响应模式
		配置专用路由，以使得响应报文首先通过vip所配置的lo上的别名接口
	lvs-dr：工作流程如图
![](https://images2015.cnblogs.com/blog/824142/201706/824142-20170615131001415-1414502727.png)
3.2、  lvs-tun：IP隧道
	1、RIP、DIP、VIP都得是公网地址；
	2、RS的网关不会指向也不可能指向DIP；
	3、请求报文经过Directory，但响应报文一定不经过Director；
	4、不支持端口映射；
	5、RS的OS必须得支持隧道功能；
	lvs-tun：工作流程如图：也是基于lvs-dr的模型，只不过不同的是，rs和vs不必在同一个物理的网络（实现物理冗余），而是通过隧道技术进行vs和rs间的通信
![](https://images2015.cnblogs.com/blog/824142/201706/824142-20170615131103009-1214198940.png)
**四、 lvs 十个调度算法： rr、wrr、lc、wlc、lblc、lblcr、dh、sh、sed、 nq**
1.轮叫调度（Round Robin）(简称rr)
2.加权轮叫（Weighted Round Robin）（简称wrr)
3.最少链接（Least Connections）(LC)
4.加权最少链接（Weighted Least Connections）(WLC)
5.基于局部性的最少链接（Locality-Based Least Connections）(LBLC)
6.带复制的基于局部性最少链接（Locality-Based Least Connections with Replication）(LBLCR)
7.目标地址散列（Destination Hashing）(DH)
8.源地址散列（Source Hashing）(SH)
9. 最短的期望的延迟（Shortest Expected Delay Scheduling SED）(SED)
10.最少队列调度（Never Queue Scheduling NQ）(NQ)
最常用的两个算法介绍： 
2.加权轮叫（Weighted Round Robin）（简称wrr)
调度器通过“加权轮叫”调度算法根据真实服务器的不同处理能力来调度访问请求。这样可以保证处理能力强的服务器能处理更多的访问流量。调度器可以自动问询真实服务器的负载情况，并动态地调整其权值。
4.加权最少链接（Weighted Least Connections）(WLC)
在集群系统中的服务器性能差异较大的情况下，调度器采用“加权最少链接”调度算法优化负载均衡性能，具有较高权值的服务器将承受较大比例的活动连接负载。调度器可以自动问询真实服务器的负载情况，并动态地调整其权值。
 说明： 部分参考 http://6638225.blog.51cto.com/6628225/1866241  感谢作者。
