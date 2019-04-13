
# DPDK-实战之ipsec-secgw（安全网关） - 3-Number - CSDN博客


2017年10月25日 16:25:21[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：1905


# 0x01 缘由
看到了比较陌生的名词觉得比较高大上，于是想探究下。
# 0x02 介绍
IPsec 安全网关应用是用DPDK cryptodev框架的一个实际应用例子。
这个应用说明用DPDK实现的一个安全网关，基于RFC4301, RFC4303, RFC3602、RFC2404标准。IKE不实现，因此仅仅手动设置安全策略。
安全策略（SP）用ACL规则实现，安全关联（SA）存储在一个表，路由是通过LPM实现。
应用将网口分为保护和非保护。因此，流量接收在一个非保护和或保护的网口上被考虑inbonud或outbound。方向不同，outbound代表数据包出方向，即从设备的某接口出去的方向。inbound代表数据包进入方向，即由设备的某接口进来的方向。
IPsec Inbound 流量路径是：
从一个网口读包；
在IPv4和ESP之间分配包；
执行Inbound SA查询为 ESP包基于它们的SPI；
执行校验和解密；
移除ESP和外层IP头；
Inbound SP用ACL检查解密的包和其他IPv4数据包；
路由；
转发包到网口；
Outbound流量方向是：
从一个网口读包；
执行OutboundSP检查，用ACL；
执行Outbound 为IPsec保护的包执行SA查询；
添加ESP和外层IP头；
执行加密和处理；
路由；
写包到网口；
# 0x04 运行
编译规则文件：
修改大叶表大小：
EAL: Not enough memory available on socket 1! Requested: 2048MB, available:
 256MB
运行命令：
```python
./build/ipsec-secgw -l 3,4 -n 4 --socket-mem 0,2048 --vdev "cryptodev_null_pmd" -- -p 0x3 -P -u 0x3 --config="(0,1，,3),(1,1,4)" -f sample.cfg
```
运行报错
```python
CRYPTODEV: [cryptodev_null_pmd] cryptodev_null_create() line 207: failed to create cryptodev vdev
   CRYPTODEV: [cryptodev_null_pmd] cryptodev_null_create() line 229: driver cryptodev_null_pmd:                 cryptodev_null_create failed
   PMD: Closing null crypto device cryptodev_null_pmd_0 on numa socket 0
```

# 0x05 总结
IPSec（InternetProtocolSecurity）是安全联网的长期方向。它通过端对端的安全性来提供主动的保护以防止专用网络与 Internet 的攻击。在通信中，只有发送方和接收方才是唯一必须了解 IPSec 保护的计算机。在 Windows
 2000、Windows XP 和 Windows Server 2003 家族中，IPSec 提供了一种能力，以保护工作组、局域网计算机、域客户端和服务器、分支机构（物理上为远程机构）、Extranet 以及漫游客户端之间的通信。
这块业务如果后期涉及了，再去仔细研究。

