
# iptables防火墙之主机型 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月13日 21:56:59[Jlins](https://me.csdn.net/dyllove98)阅读数：1382


iptables防火墙本身是不防火的，是靠其中的规则判断数据是丢弃还是放行，而这些规则就存放在iptables的四表五链中。

iptables四表五链
四表
filter（过滤数据包）：INPUT，OUTPUT，FORWARD
nat（地址转换）：PREROUTING，POSTOUTING，OUTPUT
mangle表（拆开，修改，封装数据）：五个链全部包含
raw（状态追踪）：PREROUTING，OUTPUT
五链（根据数据报文的流向，判断报文放在那里）
INPUT：到本机内部来的数据
OUTPUT：本机出去的数据
FORWARD：转发的数据
PREROUTING：刚刚到达本机，还没有路由的
POSTROUTING：马上发出本机的，还没有做路由
还可以使用自定义链，但只在被调用时才能发挥作用，而且如果没有自定义链中的任何规则匹配，还应该有返回机制，自定义链只有为空时才可以被删除，默认链无法删除。
优先次序：
prerouting：raw高于mangle高于nat
input：mangle高于filter
output：raw高于mangle高于nat高于filter
forward：mangle高于filter
postrouting：mangle高于nat
如果在一个位置有多条规则，那么匹配的数据是自上而下生效，如果上面有允许规则，就算是下面有拒绝，那么处理数据包是允许通过
推荐规则顺序：
1.如果规则有关联性：范围小的规则放到上面，范围大的规则放到下面
比如，允许192.168.1.1访问但是拒绝192.168.1.0访问，那么这个时候就需要把192.168.1.1这条规则放到上面
2.如果规则没有关联性：范围大的规则放到上面，范围小的规则放到下面
比如，同时允许192.168.1.0和172.16.0.0网段访问，那么就需要把172.16.0.0这条规则放到最上面
3.访问最频繁的规则放到最上面
比如这台服务器提供web和FTP服务，如果web的访问量大过FTP服务，那么把web的规则放到最上面，FTP放到下面；如果FTP的规则在上面的话，那么数据包到达服务器的时候，会先和FTP的规则做比较，然后在和web做比较这样就浪费了时间。
iptables 语法：
iptables   表名  管理选项  链名   匹配条件    -j  控制条件
命令：管理命令
管理规则：
-A：在链的尾部添加一条规则，如果不写表，默认是filter表
-I 链 [行号]：插入一条规则，插入为对应链上的指定行，如果省略了行，那么为第一条。
-D 链 [行号]：删除指定链中的指定规则。
-R 链 [行号]：替换指定的规则。
管理链：
-F 链：清空指定规则链，如果省略链，则可以实现删除对应表中的所有链。
-P 链：设置指定链的默认策略。
-N：自定义一个新的空链。
-X：删除一个自定义的空链（链比如为空，如果不为空使用-F清空）
-Z：置零指定链中所有规则的计数器。
-E：重命名自定义的链。
查看类：
-L：显示指定表中的所有规则。默认显示的规则和主机，是以协议和主机名出现，如果没有配置dns，查看的速度会非常的慢。
-n：以数字形式显示端口和主机名
-v：显示链及规则的详细信息
-vv：显示更加详细
-x：显示计数器的精确值
--line-numbers:显示规则号码
匹配条件：
通用匹配：
-s：源地址
-d：目的地址
-p：协议{tcp|udp|icmp}
-i：数据报文流入的接口
-o：数据报文流出的接口
扩展匹配：
隐含扩展：
-p tcp :可以使用特定协议拥有的属性扩展
--sport：源端口
--dport：目标端口
--tcp-flags：只检查mask指定的标志位，是逗号分隔的标志位列表：comp：此列表中出现的标记必须为1,comp中没出现，而mask中出现的，必须为0；
--tcp-flags  SYN，FIN，ACK，RST   SYN,ACK
--syn：三次握手的第一次
第一次握手：SYN=1，ACK=0，FIN=0，RST=0；
第二次握手：SYN=1，ACK=1，FIN=0，RST=0；
第三次握手：SYN=0，ACK=1，RST=0，FIN=0；（连接建立完成）
-p icmp
--icmp-type{0|8|3}
0：回应报文，响应自己ping请求
8：请求报文，自己发出的ping请求
3：目标不可达
-p udp：
--sport：源端口
--dport：目标端口
显式扩展：使用额外的匹配机制
-m EXTESTION --sep-opt
start：状态连接
结合ip_conntrack追踪会话的状态
NEW：新连接请求
ESTABLISHED：已建立的连接
INVALID：非法连接（如：SYN=1，FIN=1）
RELATED：相关联的（命令连接和数据连接）
例如：-m state --state ESTABLISHED  -j ACCEPT （放行数据包状态ESTABLISHED，也就是说只要通过三次握手建立连接的数据全部放行）
nultiport:离散的多端口匹配扩展
--source-ports
--destination-ports
--ports
例如：-m multiport --destination-port 21,22,80 -j ACCEPT 合并三个端口
-m iprange 匹配一段Ip地址范围
--src-range
--dst-range
-s ip，new
匹配一组ip地址192.168.0.100-192.168.0.105
|1
|[root@localhost ~]|\# iptables -A INPUT -p tcp -m iprange --src-range 192.168.0.100-192.168.0.105 --dport 22 -j ACCEPT
|

-m connlimit:连接数限制
！ --connlimit-above n (一般情况下必须加叹号，加叹号表示如果连接数不到n咋办）
设置每人访问web服务器只能有两个连接数,（生产环境根据实际需求）
|1
|iptables -A INPUT -d 192.168.0.108 -p tcp --dport 80 -m connlimit --connlimit-above 2 -j ACCEPT
|

-m limit 设置
--limit 设置指定时间内允许多少个连接
--limit-burst 设置第一次允许有多少连接同时进来
-m string
--alog {bm|kmp}    bm算法和kmp算法功能一样
--string "匹配字符"
拒绝访问的网页连接中出现h7n9连接名的文件，但是不会检查文件内容
|1
|[root@localhost ~]|\# iptables -I INPUT -d 192.168.0.108 -m string --algo kmp --string "***" -j REJECT
|

拒绝页面内容中出现h7n9,在出站链接做设置不允许页面文件中带有h7n9字样
|1
|[root@localhost ~]|\# iptables -I OUTPUT -s 192.168.0.108 -m string --algo kmp --string "h7n9" -j REJECT
|

几乎每一个条件都支持！取反
执行的动作：
ACCEPR：允许数据包通过
DROP：拒绝数据包通过，直接丢弃数据包
REJECT：拒绝数据包通过，并且告诉发送端，此路不通
DNAT：目标地址转换
SNAT：源地址转换
REDIRECT：端口重定向
MASQUERADE：地址伪装，源地址转换
LOG：记录日志的
KARK：加标记

使用：
如果服务器是一台web服务器，需要设置允许访问web的流量出站和入站，允许ssh远程连接。然后设置默认策略为阻止，这样本机的只能由管理员远程连接和进出web流量。提高了安全性。
|1
|2
|3
|4
|5
|6
|7
|8
|9
|[root@localhost ~]|\# iptables -A INPUT -s 192.168.0.0/24 -d 192.168.0.108 -p tcp --dport 22 -j ACCEPT
|[root@localhost ~]|\# iptables -A OUTPUT -s 192.168.0.108 -d 192.168.0.0/24 -p tcp --dport 22 -j ACCEPT
|[root@localhost ~]|\# iptables -A INPUT -s 192.168.0.0/24 -d 192.168.0.108 -p tcp --dport 22 -j ACCEPT
|[root@localhost ~]|\# iptables -A OUTPUT -s 192.168.0.108 -d 192.168.0.0/24 -p tcp --sport 22 -j ACCEPT
|[root@localhost ~]|\# iptables -P INPUT DROP
|[root@localhost ~]|\# iptables -P OUTPUT DROP
|[root@localhost ~]|\# iptables -P FORWARD DROP
|[root@localhost ~]|\# iptables -I INPUT -d 192.168.0.108 -p tcp --dport 80 -j ACCEPT
|[root@localhost ~]|\# iptables -I OUTPUT -s 192.168.0.108 -p tcp --sport 80 -j ACCEPT
|


访问web服务器，这个时候查看iptables的策略，就可以看到http和ssh都有流量通过，说明策略设置成功
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|[root@localhost ~]|\# iptables -L -n -vv
|Chain INPUT (policy DROP 12 packets, 1062 bytes)
|pkts bytes target     prot opt|in|out|source|destination
|67  7776 ACCEPT     tcp  --  *      *       0.0.0.0|/0|192.168.0.108       tcp dpt:80
|857 56588 ACCEPT     tcp  --  *      *       192.168.0.0|/24|192.168.0.108       tcp dpt:22
|Chain FORWARD (policy DROP 0 packets, 0 bytes)
|pkts bytes target     prot opt|in|out|source|destination
|Chain OUTPUT (policy DROP 0 packets, 0 bytes)
|pkts bytes target     prot opt|in|out|source|destination
|76 29960 ACCEPT     tcp  --  *      *       192.168.0.108        0.0.0.0|/0|tcp spt:80
|412 40288 ACCEPT     tcp  --  *      *       192.168.0.108        192.168.0.0|/24|tcp spt:22
|


由于只允许ssh和web，这个时候自己ping自己也是不同的，如果希望自己可以ping自己
|1
|2
|[root@localhost ~]|\# iptables -A INPUT -s 127.0.0.1 -d 127.0.0.1 -i lo -j ACCEPT
|[root@localhost ~]|\# iptables -A OUTPUT -s 127.0.0.1 -d 127.0.0.1 -o lo -j ACCEPT
|


允许自己ping别人，但是不允许别人ping自己
|1
|2
|[root@localhost ~]|\# iptables -A INPUT -s 192.168.0.108 -p icmp --icmp-type 8 -j ACCEPT
|[root@localhost ~]|\# iptables -A OUTPUT -d 192.168.0.108 -p icmp --icmp-type 0 -j ACCEPT
|


允许新的连接和已建立的链接通信，不允许自己对别人新建立连接
ssh：
|1
|2
|[root@localhost ~]|\# iptables -A INPUT -d 192.168.0.108 -p tcp --dport 22 -m state --state NEW,ESTABLISHED -j ACCEPT
|[root@localhost ~]|\# iptables -A OUTPUT -s 192.168.0.108 -p tcp --sport 22 -m state --state ESTABLISHE -j ACCEPT
|

web
|1
|2
|[root@localhost ~]|\# iptables -A INPUT -d 192.168.0.108 -p tcp --dport 80 -m state --state NEW,ESTABLISHED -j ACCEPT
|[root@localhost ~]|\# iptables -A OUTPUT -s 192.168.0.108 -p tcp --sport 80 -m state --state ESTABLISHED -j ACCEPT
|

如果把第一条规则设置成已建立的连接运行通过，那么就会提高查询规则的效率，比如ssh连接之后还会有数据报文通过，但是再次匹配规则的时候就会被第一条规则匹配到，不再往下查询，从而提高了效率,一条规则顶了上面的两条规则。
|1
|[root@localhost ~]|\# iptables -I OUTPUT -s 192.168.0.108 -m state --state ESTABLISHED -j ACCEPT
|


设置ftp允许出站，由于ftp的数据连接端口是随机的所以需要使用RELATED将管理连接和数据连接关联起来
首先要装载ip_conntrack_ftp和ip_nat_ftp模块
|1
|2
|3
|4
|5
|[root@localhost ~]|\# vim /etc/sysconfig/iptables-config
|IPTABLES_MODULES=|"ip_nat_ftp ip_conntrack_ftp"
|[root@localhost ~]|\# iptables -A INPUT -d 192.168.0.108 -p tcp --dport 21 -m state --state NEW,ESTABLISHED -j ACCEPT
|[root@localhost ~]|\# iptables -A INPUT -d 192.168.0.108 -p tcp -m state --state ESTABLISHED,RELATED -j ACCEPT
|[root@localhost ~]|\# iptables -A OUTPUT -s 192.168.0.108 -m state --state RELATED,ESTABLISHED -j ACCEPT
|


速率限定
设置每分钟只允许5个ping请求可以进来，设置完成后如果客户端再去ping就会发现一会通一会不通了
|1
|[root@localhost ~]|\# iptables -A INPUT -d 192.168.0.108 -p icmp --icmp-type 8 -m limit --limit 5/minute -j ACCEPT
|

设置第一次可以有四个连接进来（--limit-burst），后面进来的就是每分钟5个连接(--limit)
|1
|[root@localhost ~]|\# iptables -R INPUT 3 -d 192.168.0.108 -p icmp --icmp-type 8 -m limit --limit 5/minute --limit-burst 4 -j ACCEPT
|


记录ping日志，记录日志最好设置为几分钟记录一条，不然会产生磁盘io导致性能下降
|1
|[root@localhost ~]|\# iptables -A INPUT -d 192.168.0.108 -p icmp --icmp-type 8 -j LOG --log-prefix "--iptables log for icmp--"
|

查看日志
|1
|2
|[root@localhost ~]|\# tail /var/log/messages
|Nov 10 12:36:55 localhost kernel: --iptables log|for|icmp--IN=eth0 OUT= MAC=00:0c:29:a2:0b:db:34:23:87:fc:ae:89:08:00 SRC=192.168.0.101 DST=192.168.0.108 LEN=60 TOS=0x00 PREC=0x00 TTL=64 ID=30851 PROTO=ICMP TYPE=8 CODE=0 ID=256 SEQ=4199
|


主机型防火墙推荐设置
如果服务器开放了http和ftp而且有需要远程管理。那么可以这样设置，第一条规则允许ESTABLISHED（响应请求或已建立连接）和RELATED（与已有连接相关联性）的数据出站，第二条则是设置允许新连接连接服务器的21，22，80端口。出站设置ESTABLISHED和RELATED。默认策略全部为DROP。这样就提高了查询效率
|1
|2
|3
|[root@localhost ~]|\# iptables -A INPUT -d 192.168.0.108 -p tcp -m state --state ESTABLISHED,RELATED -j ACCEPT
|[root@localhost ~]|\# iptables -A INPUT -d 192.168.0.108 -p tcp -m multiport --destination-ports 21,22,80 -m state --state NEW -j ACCEPT
|[root@localhost ~]|\# iptables -A OUTPUT -s 192.168.0.108 -m state --state RELATED,ESTABLISHED -j ACCEPT

|

![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

