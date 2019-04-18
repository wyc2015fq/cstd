# 动态vlan和静态vlan - aiaiai010101的博客 - CSDN博客

2018年11月27日 15:21:37[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：595


本文转自：[http://blog.sina.com.cn/s/blog_4dbfc1db0100xzhc.html](http://blog.sina.com.cn/s/blog_4dbfc1db0100xzhc.html)

静态VLAN。静态VLAN也称为基于端口的VLAN，因为用户的主机属于哪个VLAN是根据交换机的端口属于哪个VLAN而定的。网络管理员首先把端口分配到不同的VLAN内，根据规划把用户的主机与相应的端口相连，这样就把用户分配到了对应的VLAN内。本文主要讨论静态VLAN的配置。

动态VLAN。动态VLAN的实现方法有多种，目前最普遍的实现方法是基于MAC地址的动态VLAN。基于MAC地址的动态VLAN需要一台VMPS（VLAN Membership Policy Server）VLAN 管理策略服务器,VMPS可以是一台具有该功能的交换机（如Catalyst 5000 交换机）或是一台外部服务器，VMPS中维护着MAC地址与VLAN的对应关系表。另外，需要把交换机的端口设置为支持动态VLAN属性的端口。当交换机的支持动态VLAN的端口收到数据帧时，通过使用该数据帧的源MAC地址查询VMPS，从而建立起端口与VLAN的对应关系。一个动态使用该数据帧的源MAC地址查询VMPS，从而建立起端口与VLAN的对应关系。一个动态的端口在某一时刻只能属于一个VLAN，属于同一VLAN的多个主机可以连接在交换机的同一个端口上。

