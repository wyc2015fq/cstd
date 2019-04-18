# 组播VLAN - aiaiai010101的博客 - CSDN博客

2018年11月16日 22:42:06[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：156


本篇写得很棒，易懂。

原文地址：[http://blog.sina.com.cn/s/blog_c079d59e0102whkw.html](http://blog.sina.com.cn/s/blog_c079d59e0102whkw.html)

组播VLAN：解决路由器为不同VLAN用户复制多份流量的问题。

![](https://img-blog.csdnimg.cn/20181116222508477.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FpYWlhaTAxMDEwMQ==,size_16,color_FFFFFF,t_70)

**本人注解：为啥从路由器到Trunk交换机要发两份数据，因为VLAN2和VLAN3是两个网段，组播是一个网段传一份数据(想一想，普通组播是一个接口发一份数据，而一个接口实际上对应一个网段，这里VLAN2和VLAN3物理上共用一个路由器接口，但是逻辑上这个接口被分为两个子接口，所以实际上，还是一个接口传一份数据，只不过这里是逻辑上的接口)，然后交换机复制分发。**

![](https://img-blog.csdnimg.cn/20181116222522394.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FpYWlhaTAxMDEwMQ==,size_16,color_FFFFFF,t_70)

二层组播
- 
如果不同VLAN的用户有相同的组播数据需求时，上游路由器仍然需要发送多份相同报文到不同VLAN中。

- 
属于不同VLAN（VLAN2和VLAN3）的用户需要接收相同的组播流，上游路由器RouterA就会把组播数据在每个VLAN内都复制一份然后发送给下游交换机SwitchA。这样，既造成了路由器与二层设备之间带宽的浪费，也增加了路由器额外的负担。

组播VLAN
- 
组播VLAN，在二层网络设备上进行跨VLAN组播复制。

- 
在交换机上部署了组播VLAN功能后，上游路由器不必在每个用户VLAN（VLAN2和VLAN3）内都复制一份组播流，而是数据流在组播VLAN（VLAN4）内复制一份后发送给二层设备。这样就避免了组播流在上游设备的重复复制，不仅节省了网络带宽，又减轻了上游路由器的负担。

基本概念
- 
组播VLAN：网络侧接口所属VLAN，用于实现组播流的汇聚。**一个组播VLAN可以绑定多个用户VLAN**。

- 
用户VLAN：用户侧接口所属VLAN，用于接收组播VLAN的组播数据。**一个用户VLAN只能加入一个组播VLAN**。                 

**本人注解：这其实相当于VLAN的嵌套。**

