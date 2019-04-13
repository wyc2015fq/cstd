
# 网络协议：NTP：1：网络时间协议基础 - 知行合一 止于至善 - CSDN博客

2019年03月12日 20:53:48[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：556


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
接触到NTP时间同步有很多场景，当前常见场景在于纯局域网内部的计算机集群的时间同步和纠偏，或者以其中一台可以链接外部互联网的计算机作为枢纽，连接标准的NTP服务器进行时间同步和纠偏。这篇文章介绍一下NTP协议的基础知识。
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 什么是NTP
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)NTP是网络时间协议(Network Time Protocol)的缩写，它是用来同步网络上的各台计算机的时间的协议。它是美国David L. Mills博士设计实现的.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190312100958724.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
它是由时间协议、ICMP时间戳消息及IP时间戳选项发展而来。现在最新的标准化文档为RFC 5905.
[
](https://img-blog.csdnimg.cn/20190312100958724.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 为什么需要NTP
[
](https://img-blog.csdnimg.cn/20190312100958724.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)为什么需要NTP服务，主要原因有两个
[
](https://img-blog.csdnimg.cn/20190312100958724.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)网络上的各台计算机上的时间不同步
不同步的原因有很多，比如手动修改了操作系统的时间，比如CMOS电池重置导致的时间复位等等。
网络上的各台计算机时间不精确
我们所使用的钟表，精度有一定的误差，一般较高的精度平均每年也会达到一分钟左右的误差，虽然对日常使用基本不造成影响，但是在计算机的世界这已经会造成问题。
什么是准确的
目前最为准确的装备是什么呢？
原子钟是目前世界上最准确的计时工具，它在20世纪50年代出现，利用院子袖手或释放能量是发射出的电磁波来计时，非常稳定，再加上一些列精密仪器的控制，可以达到2000万年才误差1秒，这个精确度基本能够实现目前所有的需求。
为什么不能直接使用格林威治时间
格林威治时间是以地球自转为基础的，而由于地球自转本身就不规则，而且在缓慢加速。因为格林威治时间计时的基础都存在偏差，所以格林威治时间已经不再作为标准时间使用。
什么是新的标准时间
缩写为UTC（Universal Time Coordinated）的世界协调时是准确的标准时间，UTC正式使用原子钟进行报时的国际标准时间。
获取的方式
而获取的方式也多种，可以通过卫星，比如GPS或者北斗。也可以直接从网络上的NTP服务器进行获取。关于使用ntpdate进行时间同步的相关说明，可以参看如下文章：
[https://blog.csdn.net/liumiaocn/article/details/79250793](https://blog.csdn.net/liumiaocn/article/details/79250793)
时间同步的详细
在RFC 5905中，对于时间同步进行了详细的说明，其实也并不复杂：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190312104441521.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
计算方法
简单的来说，服务器的时间加上传输的延时的纠正即为要设定的时间。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190312104513869.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
简化说明
为了更加容易地理解，对于上述RFC的实际实现进行如下更为简单的说明：
NTP客户端NTP服务器T1: 客户端发送NTP请求的时间戳（客户端时间）客户端NTP请求T2: 服务器收到NTP请求的时间戳(服务器时间)T3: 服务器回复NTP请求的时间戳(服务器时间)客户端NTP请求T4: 客户端收到NTP回复的时间戳（客户端时间）NTP客户端NTP服务器
4个时间
时间说明备注|
|---|---|---|
T1客户端发送NTP请求的时间戳客户端时间|
T2服务器收到NTP请求的时间戳服务器时间|
T3服务器回复NTP请求的时间戳服务器时间|
T4客户端收到NTP回复的时间戳客户端时间|
两个延时
时间说明|
|---|---|
d1客户端发送NTP请求到服务器端收到时间|
d2服务器回复NTP请求到客户端收到时间|
关系
假定带求解的NTP服务器和NTP客户端需要调整的时间为变量t，则关联关系计算公式为：
T2 = T1 + t + d1
T4 = T3  - t + d2
假定（延时对称性）
假定：d1 == d2
说明：考虑到客户端向服务器发送与服务器向客户端回复是一个NTP包的往返，假定两者的延时时间d1与d2相等。NTP的精度在一定程度上取决于往返回路的延时对称性。
求解
在此假定基础上进行求解，可以轻松地算出：
需要调整的时间： t = ((T2-T1) - (T4-T3) )/2
整体延时： d1 + d2 = (T2 - T1) + (T4 - T3)
分层模型
NTP中是分层（stratum）的，ntpd方式下stratum最多可以设定15个层级。stratum 0 是没有偏移offset的起点（比如原子钟），在网络上不能直接使用，另外stratum 16被认为是无未被同步的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190312131120369.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
NTP服务器的架设
在CentOS上使用NTPD和Chrony进行NTP服务器的架设，在后续的文章中将进行进一步的说明。
参考内容
[https://tools.ietf.org/html/rfc5905](https://tools.ietf.org/html/rfc5905)

