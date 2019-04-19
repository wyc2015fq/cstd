# Linux中IPsec的实现--(实例) - xqhrs232的专栏 - CSDN博客
2017年08月11日 18:00:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：6968
原文地址::[http://yuanye1128.blog.51cto.com/8808749/1410804/](http://yuanye1128.blog.51cto.com/8808749/1410804/)
相关文章
1、linux下搭建 ipsec vpn服务器（centos+ubuntu）----[http://blog.csdn.net/cnliwy/article/details/54346933](http://blog.csdn.net/cnliwy/article/details/54346933)
2、[在linux环境下构建ipsec vpn的过程](http://blog.csdn.net/jeason29/article/details/45308783)----[http://blog.csdn.net/jeason29/article/details/45308783](http://blog.csdn.net/jeason29/article/details/45308783)
3、在 Linux 内使用 IPSEC 配置网络对网络和点对点的 VPN 连接----[https://www.ibm.com/developerworks/cn/linux/l-ipsec/](https://www.ibm.com/developerworks/cn/linux/l-ipsec/)
# Linux中IPsec的实现
**IPsec-VPN****：**
virtual private network--虚拟专用网
**VPN作用****：**
通过公网实现远程连接，将私有网络联系起来
**VPN的类型：**
1、overlay的VPN，例如IPsec-VPN
2、peer-to-peer的VPN，例如MPLS-VPN
还可以分为二层VPN和三层VPN
IPsec-VPN是三层的VPN
**IPsec-VPN的分类:**
1、site-to-site VPN 也叫 LAN-to-LAN VPN （要求两个站点都要有固定的IP）
2、EASY-VPN 也叫 remote VPN （通常用于连接没有固定IP的站点）
**IPsec-VPN提供三个特性：**
1、authentication  每一个IP包的认证
2、data integrity  验证数据完整性,保证在传输过程中没有被人为改动
3、confidentiality （私密性）数据包的加密
----------------------------------------------------------------------------------------
**IP sec 的组成****：**
IPsec协议集包括三个协议：
**1、internet key exchange(IKE)密匙交换协议**  协议双方使用的算法，密匙，协商在两个对等体之间建立一条遂道的参数，协商完成再用下面的方法封装数据。
  IKE动态的，周期性的在两个PEER之间更新密钥
**2、encapsulating secutity payload(ESP)封装安全负载**  可以对数据包认证，加密,封装，IP中协议号--50，通常使用3DES来进行加密
**3、authentication header (AH)**  只提供认证，封装，不提供加密，明文传送,IP中协议号--51
IPsecVPN的两种模式--
**一、传输模式：**
不产生新的IP头，在原包头之后插入一个字段,当通信点等于加密点用这种方法
原始IP头 | (ESP或AH) | Data
![wKioL1NyPAqAYrHiAAGbnQXUh64912.jpg](http://s3.51cto.com/wyfs02/M01/27/63/wKioL1NyPAqAYrHiAAGbnQXUh64912.jpg)
**二、****隧道****模式：**
产生一个新IP包头，当通信点不等于加密点用这种方法，site-to-site的VPN就是这种模式,因为通信点的IP头通常不是一个公网上可路由的头部，而新的IP头用的是两个peer之间的IP地址。
新IP头 | (ESP或AH) | 原始IP头 | Data
![wKioL1NyPAqCJ30mAAHHDGqZZsA575.jpg](http://s3.51cto.com/wyfs02/M01/27/63/wKioL1NyPAqCJ30mAAHHDGqZZsA575.jpg)
通信点：实际通信的设备
加密点：进行加密的设备
ESP封装中只对原始IP分组进行完整性检验
AH封装中进行完整性检验还包括新的IP头
--------------------------------------------------------------------------------------------
**IKE密匙交换协议**
IKE的作用：用于在两个peer之间协商建立IPsec-VPN通道
1、协商参数
2、产生KEY，交换KEY、更新KEY
3、对双方进行认证
4、对密钥进行管理
由**三个不同的协议**组成：
1、ISAKMP--定义了信息交换的体系结构,也就是格式
2、SKEME--实现公钥加密认证的机制
3、Oakley--提供在两个IPsec对等体间达成相同加密密钥的基于模式的机制
ISAKMP基于UDP，源目端口都是500
site-to-site ipsec VPN的协商过程,分两个阶段
要想在两个站点之间安全的传输IP数据流，它们之间必须要先进行协商，协商它们之间所采用的加密算法，封装技术以及密钥。这个协商过程是通过IKE来完成的，IKE协商分两个阶段运行：
**阶段一：**在两个对等体设备之间建立一个安全的管理连接。没有实际的数据通过这个连接。这个管理连接是用来保护第二阶段协商过程的。
**阶段二：**当对等体之间有了安全的管理连接之后，它们就可以接着协商用于构建安全数据连接的安全参数，这个协商过程是安全的，加了密的。协商完成后，将在两个站点间形成安全的数据连接。用户就可以利用这些安全的数据连接来传输自已的数据了。
**第一阶段：**建立ISAKMP SA  协商的是以下信息：
1、对等体之间采用何种方式做认证，是预共享密钥还是数字证书。
2、双方使用哪种加密算法
3、双方使用哪种HMAC方式，是MD5还是SHA
4、双方使用哪种Diffie-Hellman密钥组
5、使用哪种协商模式（主模式或主动模式）
6、还要协商SA的生存期
**第二阶段****：**建立IPsec
 SA  协商的是以下信息：
1、双方使用哪种封装技术，AH还是ESP
2、双方使用哪种加密算法
3、双方使用哪种HMAC方式，是MD5还是SHA
4、使用哪种传输模式，是隧道模式还是传输模式
5、还要协商SA的生存期
---------------------------------------------------------------------
**SA--安全关联**
SA是两个通信实体经协商建立起来的一种协定，它们决定了用来保护数据包安全的IPsec协议、转码方式、密钥、以及密钥的有效存在时间等等。任何IPsec实施方案始终会构建一个SA数据库（SA DB），由它来维护IPsec协议用来保障数据包安全的SA记录。
SA是单向的--如果两个主机（比如A和B）正在通过ESP进行安全通信，那么主机A就需要有一个SA，即SA（OUT），用来处理外发的数据包，另外还需要有一个不同的SA，即SA（IN）用来处理进入的数据包。主机A的SA（OUT）和主机B的SA（IN）将共享相同的加密参数（比如密钥）。
SA还要根据协议来区分，如果两个主机间同时使用ESP和AH，对于ESP和AH会生成不同的SA。
SADB--安全关联数据库，包含双方协商的IKE和IPsec安全信息
SPI--安全参数索引，是一个32位的值，用来标识用于处理数据包的特定的那个安全联盟。或者这样理解，用于唯一定义一条单向的IPsec通道。这个号码存在于ESP包头中，通道两端必须一致。
SA分为两种--
**1、IKE（ISAKMP）SA **  协商对IKE数据流进行加密以及对对等体进行验证的算法
**2、IPsec SA  **         协商对对等体之间的IP数据流进行加密的算法
对等体之间的IKE SA只能有一个
对等体之间的IPsec SA可以有多个
PFS--完善转发安全性，是一种IKE协商中发起者可以向响应者提供建议的属性，是一种强制对等体双方在快速模式交换中产生新的DH秘密的属性。这允许使用新的DH秘密生成用于加密数据的加密密钥。
**案例：**
**要求：某公司总部fw1 192.168.1.0网段，两个分支分别是fw2  2.0和  fw3  3.0网段，中间是Internet，需要建立两条隧道让总部与两个分支相连。**
**拓扑图：**
![wKiom1NyPBjhhcG9AADioXHnUGc267.jpg](http://s3.51cto.com/wyfs02/M02/27/63/wKiom1NyPBjhhcG9AADioXHnUGc267.jpg)
**步骤：**
**首先配置ISP，使用华为三层交换机代替**
[ISP]vlan 2
[ISP-vlan2]port eth0/0/2
[ISP-vlan2]vlan 10
[ISP-vlan10]port eth0/0/10
[ISP-vlan10]vlan
 20
[ISP-vlan20]port eth0/0/20
[ISP]int vlan 2
[ISP-Vlanif2]ip add 61.130.130.2 255.255.255.0
[ISP]int vlan 10
[ISP-Vlanif2]ip add 61.130.130.6 255.255.255.0
[ISP]int vlan 20
[ISP-Vlanif2]ip add 61.130.130.10 255.255.255.0
开始配置防火墙FW1、FW2、FW3的各个端口ip以及静态路由
FW1：
[FW1]int eth0/1
[FW1-Ethernet0/1]ip add 192.168.1.1 24
[FW1-Ethernet0/1]loopback
[FW1-Ethernet0/1]int eth0/0
[FW1-Ethernet0/0]ip
 add 61.130.130.1 30
[FW1]ip route 0.0.0.0 0 61.130.130.2
[FW1]firewall zone untrust
[FW1-zone-untrust]add int eth0/0
FW2:
[FW2]int eth0/1
[FW2-Ethernet0/1]ip add 192.168.2.1
 24
[FW2-Ethernet0/1]loopback
[FW2-Ethernet0/1]int
 eth0/0
[FW2-Ethernet0/0]ip
 add 61.130.130.5 30
[FW2]ip
 route 0.0.0.0 0 61.130.130.6
[FW2]firewall zone untrust
[FW1-zone-untrust]add int eth0/0
FW3:
[FW3]int eth0/1
[FW3-Ethernet0/1]ip add 192.168.3.1
 24
[FW3-Ethernet0/1]loopback
[FW3-Ethernet0/1]int
 eth0/0
[FW3-Ethernet0/0]ip
 add 61.130.130.9 30
[FW3]ip
 route 0.0.0.0 0 61.130.130.10
[FW3]firewall zone untrust
[FW3-zone-untrust]add int eth0/0

开始做总部与分支之间的隧道
**FW1：**
[FW1]acl number 3000 match-order auto
#访问控制列表
[FW1-acl-adv-3000]rule 10 permit ip source 192.168.1.0 0.0.0.255 destination 192.168.2.0 0.0.0.255
[FW1-acl-adv-3000]rule 20 deny ip source any destination any
#添加访问控制列表规则
[FW1]ipsec proposal tran1
#安全协议，命名为tran1
[FW1-ipsec-proposal-tran1]encapsulation-mode tunnel
#采取封装模式为tunnel隧道
[FW1-ipsec-proposal-tran1]transform esp
#协商传输协议为esp
[FW1-ipsec-proposal-tran1]esp authentication-algorithm md5
#该协议加密算法为md5
[FW1-ipsec-proposal-tran1]esp encryption-algorithm des
#该协议验证算法为des
[FW1]ipsec policy policy1 10 isakmp
#创建安全策略，并表明采取的方式为isakmp
[FW1-ipsec-policy-isakmp-policy1-10]security acl 3000
#应用访问控制列表
[FW1-ipsec-policy-isakmp-policy1-10]proposal tran1
#使用安全协议tran1
[FW1]ike peer fw2
#生成ike，并指出他的邻居为fw2
[FW1-ike-peer-fw2]local-address 61.130.130.1
#本地ip地址
[FW1-ike-peer-fw2]remote-address 61.130.130.5
#远端ip地址
[FW1-ike-peer-fw2]pre-shared-key 123456
#定义一个共享公钥
[FW1]ipsec policy policy1 10 isakmp
[FW1-ipsec-policy-isakmp-policy1-10]ike-peer fw2
#指出ike的邻居为fw2
[FW1]int eth0/0
#进入接口
[FW1-Ethernet0/0]ipsec policy policy1
#在接口上应用通往fw2上的提议
添加FW3的配置
[FW1]acl number 3001 match-order auto
[FW1-acl-adv-3001]rule 10 permit ip source 192.168.1.0 0.0.0.255
 destination 192.168.3.0 0.0.0.255
[FW1-acl-adv-3001]rule 20 deny ip source any destination any
[FW1]ipsec proposal tran2
[FW1-ipsec-proposal-tran2]encapsulation-mode tunnel
[FW1-ipsec-proposal-tran2]transform esp
[FW1-ipsec-proposal-tran2]esp authentication-algorithm md5
[FW1-ipsec-proposal-tran2]esp encryption-algorithm des
[FW1]ipsec policy policy1 20 isakmp
[FW1-ipsec-policy-isakmp-policy1-10]security acl 3001
[FW1-ipsec-policy-isakmp-policy1-10]proposal tran2
[FW1]ike peer fw3
[FW1-ike-peer-fw3]local-address
 61.130.130.1
[FW1-ike-peer-fw3]remote-address 61.130.130.5
[FW1-ike-peer-fw3]pre-shared-key 123456
[FW1]ipsec policy policy1 20 isakmp
[FW1-ipsec-policy-isakmp-policy1-10]ike-peer fw3
[FW1]int eth0/0
[FW1-Ethernet0/0]ipsec policy policy1
**FW2：**
[FW2]acl number 3000 match-order auto
[FW2-acl-adv-3000]rule 10 permit ip source 192.168.2.0
 0.0.0.255 destination 192.168.1.0 0.0.0.255
[FW2-acl-adv-3000]rule 20 deny ip source any destination any
[FW2]ipsec proposal tran1
[FW2]ipsec-proposal-tran1]encapsulation-mode tunnel
[FW2-ipsec-proposal-tran1]transform esp
[FW2-ipsec-proposal-tran1]esp authentication-algorithm md5
[FW2-ipsec-proposal-tran1]esp encryption-algorithm des
[FW2]ipsec policy policy1 10 isakmp
[FW2-ipsec-policy-isakmp-policy1-10]security acl 3000
[FW2-ipsec-policy-isakmp-policy1-10]proposal tran1
[FW2]ike peer fw1
[FW2-ike-peer-fw1]local-address
 61.130.130.5
[FW2-ike-peer-fw1]remote-address
 61.130.130.1
[FW2-ike-peer-fw1]pre-shared-key
 123456
[FW2]ipsec policy policy1 10 isakmp
[FW2-ipsec-policy-isakmp-policy1-10]ike-peer fw1
[FW2]int eth0/0
[FW2-Ethernet0/0]ipsec policy policy1
**FW3：**
[FW3]acl number 3001 match-order
 auto
[FW3-acl-adv-3001]rule
 10 permit ip source 192.168.3.0 0.0.0.255 destination 192.168.1.0
 0.0.0.255
[FW3-acl-adv-3000]rule 20 deny ip source any destination any
[FW3]ipsec proposal tran2
[FW3-ipsec-proposal-tran2]encapsulation-mode
 tunnel
[FW3-ipsec-proposal-tran2]transform
 esp
[FW3-ipsec-proposal-tran2]esp
 authentication-algorithm md5
[FW3-ipsec-proposal-tran2]esp
 encryption-algorithm des
[FW3]ipsec policy policy120
 isakmp
[FW3-ipsec-policy-isakmp-policy1-20]security
 acl 3000
[FW3-ipsec-policy-isakmp-policy1-20]proposal
 tran2
[FW3]ike peer fw1
[FW3-ike-peer-fW1]local-address
 61.130.130.9
[FW3-ike-peer-fw1]remote-address
 61.130.130.1
[FW3-ike-peer-fw1]pre-shared-key
 123456
[FW3]ipsec policy policy1 10 isakmp
[FW3-ipsec-policy-isakmp-policy1-10]ike-peer fw1
[FW3]int eth0/0
[FW3-Ethernet0/0]ipsec policy policy1
测试：
配置隧道后，总部fw1已经可以与两个分支连通
![wKioL1NyO47QpCqfAAHP7Fw6v0s896.jpg](http://s3.51cto.com/wyfs02/M00/27/63/wKioL1NyO47QpCqfAAHP7Fw6v0s896.jpg)
分公司也可以与总部通信
![wKiom1NyO7iyRV8KAAHd_Di3ldQ590.jpg](http://s3.51cto.com/wyfs02/M01/27/63/wKiom1NyO7iyRV8KAAHd_Di3ldQ590.jpg)

本文出自 “[稳稳的幸福](http://yuanye1128.blog.51cto.com/)” 博客，请务必保留此出处[http://yuanye1128.blog.51cto.com/8808749/1410804](http://yuanye1128.blog.51cto.com/8808749/1410804)

