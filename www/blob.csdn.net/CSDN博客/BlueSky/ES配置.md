# ES配置 - BlueSky - CSDN博客
2016年07月12日 17:31:29[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：449
ES集群内部节点访问，需配置：
`#network.publish_host为节点之间通信的IP地址</span>`
```
network.publish_host: slave1
#discovery.zen.ping.unicast.hosts为节点之间探寻的集群范围
```
`discovery.zen.ping.unicast.hosts: ["master","slave1", "slave2"]`
一般内部配置成内网IP，这样能够保证数据安全性
