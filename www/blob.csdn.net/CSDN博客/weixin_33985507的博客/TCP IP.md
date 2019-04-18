# TCP/IP - weixin_33985507的博客 - CSDN博客
2018年01月20日 13:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
具体含义：利用IP进行通讯时所必须用到的协议群的统称。也称为 网际协议族
# 分层协议
应用协议：HTTP，SMTP，FTP，TEL，NET,SNMP
传输协议：TCP，UDP
网际协议：IP，ICMP，ARP
路由控制协议：RIP，OSPF，BGP
# 数据单位：
包：全能型术语
帧：数据链路层中包的单位
数据报：IP和UDP等网络层以上分层中包的单位
段：TCP数据流中的信息
消息：应用层中协议的单位
# IP协议
网际协议，作用：IP寻址，路由，分包与组包
#### 路由：一个数据到达最终目的地址，靠路由控制，IP路由称为多跳路由
数据链路实现一跳内的通讯，IP实现直至目标地址的通讯（点对点）
#### 面向无连接
IP发包前，不需要与目标地址建立连接
原因：1.简化；2.提速(需要有连接时，可以委托上层提供服务）
#### 分包：
MTU：最大传输单元
路由器做分片，目标主机进行重组
原因：数据包可能丢失，即使中间重新组装，下一站依然可能丢失
路径MTU发现：不需要最大MTU，使用路径中的最小MTU，可以避免路由器进行分片处理。
#### IPV4 首部
![1737985-7b9336a7292f4524.png](https://upload-images.jianshu.io/upload_images/1737985-7b9336a7292f4524.png)
IP首部
**协议类型（Protocol）**：表示IP包上层协议编号
[https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml](https://link.jianshu.com?t=https%3A%2F%2Fwww.iana.org%2Fassignments%2Fprotocol-numbers%2Fprotocol-numbers.xhtml)
**16位标识（ID）**：用于分片重组，同一个分片ID相同
**3位标志：**标识分片的相关信息。
0bit：未使用
1bit：0 可以分片，1 不能分片
2bit：被分片情况下 0最后一个分片包，1分片中的包
**片偏移FO：**标识被分片的每一个分段相对于原始数据的位置
**服务类型（TOS）字段：**包括一个3 bit的优先权子字段（现在已被忽略），4 bit的TO S子字段和1 bit未用位但必须置0。
4 bit的TO S分别代表：最小时延、最大吞吐量、最高可靠性和最小费用。4 bit中只能置其中1 bit。如果所有4 bit均为0，意味着是一般服务。
现在大多数的T C P / I P实现都不支持TO S特性，但是自4.3BSD Reno以后的新版系统都对它进行了设置。另外，新的路由协议如O S P F和I S - I S都能根据这些字段的值进行路由决策。
# DNS
管理主机名与IP地址对应关系
