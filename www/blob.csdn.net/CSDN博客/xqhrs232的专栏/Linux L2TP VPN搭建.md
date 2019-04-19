# Linux L2TP VPN搭建 - xqhrs232的专栏 - CSDN博客
2018年07月30日 09:57:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1188
原文地址::[https://blog.csdn.net/qq_39265492/article/details/80404693](https://blog.csdn.net/qq_39265492/article/details/80404693)
相关文章
1、WIN7下VPN的建立方法（PPTP与L2TP）----[https://jingyan.baidu.com/article/fdbd427713f4a2b89f3f487b.html](https://jingyan.baidu.com/article/fdbd427713f4a2b89f3f487b.html)
### 首先配置网络YUM源：
通过下面这条命令get到阿里云的网络yum源到/etc/yum.repos.d/epel.repo文件下。
`wget -O /etc/yum.repos.d/epel.repo http://mirrors.aliyun.com/repo/epel-6.repo`
### 1、一些预先准备的环境
yum install -y make gcc gmp-devel xmlto bison flex xmlto libpcap-devel lsof vim-enhanced man
yum install openswan ppp xl2tpd
### 2、配置
（1）vi /etc/ipsec.conf
替换为如下内容，把下面0.0.0.0换成服务器的外网IP（注意一定要有字符缩进，距离不要改变）
config setup
    nat_traversal=yes
    virtual_private=%v4:10.0.0.0/8,%v4:192.168.0.0/16,%v4:172.16.0.0/12
    oe=off
    protostack=netkey
conn L2TP-PSK-NAT
    rightsubnet=vhost:%priv
    also=L2TP-PSK-noNAT
conn L2TP-PSK-noNAT
    authby=secret
    pfs=no
    auto=add
    keyingtries=3
    rekey=no
    ikelifetime=8h
    keylife=1h
    type=transport
    left=0.0.0.0  
    leftprotoport=17/1701
    right=%any
    rightprotoport=17/%any
（2）编辑/etc/ipsec.secrets
替换内容如下：（0.0.0.0换成服务器的外网IP）
include /etc/ipsec.d/*.secrets
0.0.0.0 %any: PSK "jelly"
（3）修改/添加 /etc/sysctl.conf并生效
在/etc/sysctl.conf的末尾加上如下内容
net.ipv4.ip_forward = 1
net.ipv4.conf.default.rp_filter = 0
net.ipv4.conf.all.send_redirects = 0
net.ipv4.conf.default.send_redirects = 0
net.ipv4.conf.all.log_martians = 0
net.ipv4.conf.default.log_martians = 0
net.ipv4.conf.default.accept_source_route = 0
net.ipv4.conf.all.accept_redirects = 0
net.ipv4.conf.default.accept_redirects = 0
net.ipv4.icmp_ignore_bogus_error_responses = 1
生效上面的修改使用如下命令
sysctl -p
（4）验证ipsec运行状态
ipsec restart
ipsec verify
如果出现如下内容，说明已经成功：
- Checking your system to see if IPsec got installed and started correctly:
- Version check and ipsec on-path                                 [OK]
- Linux Openswan U2.6.32/K2.6.32-431.20.3.0.1.el6.centos.plus.x86_64 (netkey)
- Checking for IPsec support in kernel                            [OK]
- SAref kernel support                                           [N/A]
- NETKEY:  Testing for disabled ICMP send_redirects              [OK]
- NETKEY detected, testing for disabled ICMP accept_redirects     [OK]
- Checking that pluto is running                                  [OK]
- Pluto listening for IKE on udp 500                             [OK]
- Pluto listening for NAT-T on udp 4500                          [OK]
- Checking for 'ip' command                                       [OK]
- Checking /bin/sh is not /bin/dash                               [OK]
- Checking for 'iptables' command                                 [OK]
- Opportunistic Encryption Support                                [DISABLED]
（5） 编辑 /etc/xl2tpd/xl2tpd.conf  这一步可以跳过
ip range 写客户端的内网IP段，local ip写客户端内网IP
[lns default]
ip range = 10.10.0.2-10.10.0.100
local ip = 10.10.0.1
require chap = yes
refuse pap = yes
require authentication = yes
name = LinuxVPNserver
ppp debug = yes
pppoptfile = /etc/ppp/options.xl2tpd
length bit = yes
bps = 1000000
（6）配置用户名,密码:编辑 /etc/ppp/chap-secrets
# Secrets for authentication using CHAP
# client        server  secret                  IP addresses
username * "userpass" *
注：username换成你要登录的用户名，userpass换成密码
（7）重启xl2tp
service xl2tpd restart
（8）添加自启动
chkconfig xl2tpd on
chkconfig iptables on
chkconfig ipsec on
（9）转发规则（需要设置，否则连上VPN无法上网）
yum install iptables-services -y
iptables -t nat -A POSTROUTING -s 10.10.0.0/24 -o eth0 -j MASQUERADE
service iptables save
注：10.10.0.0/24跟/etc/xl2tpd/xl2tpd.conf的设置相对应，eth0要改成你内网的网络名字
（10）关闭防火墙
systemctl stop firewalld
接下来就可以用电脑连接：
首先，需要修改注册表regedit
[HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/RasMan/Parameters]
ProhibitIPSec的值修改为1
如果没有ProhibitIPSec项，需要点击右键新建dword，重命名为ProhibitIPSec，值设置为1
其次，重启你的本地电脑（必须重启哦）
连接方式和pptp的vpn连接方式相同！！！
