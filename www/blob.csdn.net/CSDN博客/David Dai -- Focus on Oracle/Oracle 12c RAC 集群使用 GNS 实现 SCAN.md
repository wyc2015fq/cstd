
# Oracle 12c RAC 集群使用 GNS 实现 SCAN - David Dai -- Focus on Oracle - CSDN博客


2015年01月21日 16:32:45[Dave](https://me.csdn.net/tianlesoftware)阅读数：14940所属专栏：[Oracle 12c](https://blog.csdn.net/column/details/oracle12c.html)



**注：本文谢绝转载！**

# 1   说明
在前面的Blog中，讲过RAC 中SCAN 的配置有三种方式：
（1） /etc/hosts
（2） DNS
（3） GNS
具体链接如下：
Oracle RAC 集群 SCAN 说明
[http://www.cndba.cn/Dave/article/758](http://www.cndba.cn/Dave/article/758)
Oracle 12c RAC 集群使用DNS 实现 SCAN
[http://www.cndba.cn/Dave/article/757](http://www.cndba.cn/Dave/article/757)

Oracle 12c RAC 修改SCAN 配置
[http://www.cndba.cn/Dave/article/756](http://www.cndba.cn/Dave/article/756)

如果使用GNS来做SCAN，那么就需要配置DNS和 DHCP。 并且整个RAC的架构，也会发生变化。
在我们传统的架构中，PUBLIC IP,PRIVATE IP，VIP 都是我们预先分配好的，但如果使用GNS 来做SCAN,那么只需要固定PRIVATEIP 和 PUBLIC IP， VIP和SCAN IP 由GNS 从DHCP 中动态获取。

本测试的RAC搭建IP 分配如下表
|节点
|Public IP
|Private IP
|VIP
|SCAN VIP
|GNS VIP
|SCAN
|
|rac1
|192.168.56.5
|192.168.57.5
|从DHCP动态分配
|从DHCP动态分配
|192.168.56.201
|由DNS|解析
|从|DHCP|中获得，并由|GNS|进行域名解析
|
|rac2
|192.168.56.6
|192.168.57.6
|
|dns/dhcp
|192.168.56.200
|

# 2   安装配置DNS

## 2.1             安装DNS
安装：
[root@rac1yum.repos.d]\# yum search bind-
Loadedplugins: refresh-packagekit, security
====================================================================N/S Matched: bind-=====================================================================
bind-chroot.x86_64: A chroot runtime environment for the ISC BIND DNS server, named(8)
bind-devel.i686: Header files and libraries needed for BIND DNS development
bind-devel.x86_64: Header files and libraries needed for BIND DNS development
bind-dyndb-ldap.x86_64: LDAP back-end plug-in for BIND
bind-libs.x86_64: Libraries used by the BIND DNS packages
bind-libs.i686: Libraries used by the BIND DNS packages
bind-sdb.x86_64: BIND server with database backends and DLZ support
bind-utils.x86_64: Utilities for querying DNS name servers
[root@rac1sbin]\# yum install bind-chroot.x86_64
依赖的包会自动解决。
[root@rac1etc]\# rpm -qa | grep "^bind"
bind-chroot-9.8.2-0.30.rc1.el6.x86_64
bind-9.8.2-0.30.rc1.el6.x86_64
bind-libs-9.8.2-0.30.rc1.el6.x86_64
bind-utils-9.8.2-0.30.rc1.el6.x86_64
[root@rac1etc]\#

## 2.2             配置DNS
安装完成后，bind的主配置文件是/etc/named.conf；区域类型配置文件是/etc/named.rfc1912.zones；区域配置文件在/var/named/下;

### 2.2.1 修改name.conf 文件

[root@davedns ~]\# cat /etc/named.conf
//
// named.conf
//
// Provided by Red Hat bind package to configure the ISC BINDnamed(8) DNS
// server as a caching only nameserver (as a localhost DNS resolveronly).
//
// See /usr/share/doc/bind*/sample/ for example named configurationfiles.
//
options {
//         listen-onport 53 { 127.0.0.1; };默认监听IP为127.0.0.1，用//需注释掉
//         listen-on-v6port 53 { ::1; };注释掉
directory "/var/named";存放区域配置文件的目录
dump-file "/var/named/data/cache_dump.db";
statistics-file"/var/named/data/named_stats.txt";
memstatistics-file"/var/named/data/named_mem_stats.txt";
//         allow-query     { localhost; };允许查询的客户端的IP，默认为本机，注释掉。
recursionyes; 是否开启迭代查询功能
dnssec-enable yes;
dnssec-validationyes;
dnssec-lookasideauto;
/* Path to ISC DLVkey */
bindkeys-file"/etc/named.iscdlv.key";
managed-keys-directory"/var/named/dynamic";
};
logging {
channel default_debug{
file"data/named.run";
severitydynamic;
};
};
//这里必须注释掉原来的file，重新加上file
zone "." IN {
type hint;
//        file"named.ca";
file"/dev/null";
};
include "/etc/named.rfc1912.zones";
include "/etc/named.root.key";

### 2.2.2 配置正反向解析Zone
修改Zone 文件：/etc/named.rfc1912.zones, 添加正向解析和反向解析的Zone 定义。
--配置正向解析Zone
zone"cndba.cn" IN {
typemaster;
file"cndba.cn.zone";
allow-update{ none; };
};
--配置反向解析Zone
zone"56.168.192.in-addr.arpa" IN {
typemaster;
file"56.168.192.local";
allow-update{ none; };
};
这里需要注意的是，反向解析从左到右读取ip地址时是以相反的方向解释的，所以需要将ip地址反向排列。这里，192.168.56.*网段的反向解析域名为"56.168.192.in-addr.arpa”。

### 2.2.3 配置正向解析的区域文件
在之前的name.conf 配置中指定的区域文件目录是：/var/named。 所以在这个目录里创建正向解析的区域文件。
文件名就是在Zone中定义的file名。
[root@davedns ~]\# touch /var/named/cndba.cn.zone

添加如下内容：
$TTL    86400
@      IN     SOA dns.cndba.cn.  root.cndba.cn. (
42              ; serial (d.adams)
3H              ; refresh
15M             ; retry
1W              ; expiry
1D)            ; minimum
IN      NS      dns.cndba.cn.
dns    IN      A      192.168.56.200
gns   IN      A       192.168.56.201
$ORIGINcndba.cn.
@       IN     NS      gns.cndba.cn.

### 2.2.4 配置反向解析的区域文件
在/var/named 目录下创建反向解析的区域文件，文件名也是之前在Zone中定义的：56.168.192.local。
[root@davedns~]\# touch /var/named/56.168.192.local
添加如下内容：
$TTL    86400
@               IN SOA  dns.cndba.cn.      root.cndba.cn. (
1997022700 ; Serial
28800      ; Refresh
14400      ; Retry
3600000    ; Expire
86400)    ; Minimum
IN      NS     dns.cndba.cn.
200     IN     PTR     dns.cndba.cn.
201     IN     PTR     gns.cndba.cn.

### 2.2.5 重启DNS
[root@davedns~]\# chkconfig named on
[root@davedns~]\# service named restart
Stoppingnamed: .                                         [  OK  ]
Startingnamed:                                           [  OK  ]
[root@rac2named]\#

### 2.2.6 验证DNS
在所有节点的/etc/resolv.conf文件中添加如下内容：
[root@rac1~]\# cat /etc/resolv.conf
\#Generated by NetworkManager
nameserver192.168.56.200
nameserver192.168.56.201
optionsrotate
optionstimeout:2
optionsattempts:5
[root@rac1~]\#
[root@rac2~]\# nslookup dns.cndba.cn
Server:       192.168.56.200
Address:   192.168.56.200\#53
Name:  dns.cndba.cn
Address:192.168.56.200
[root@rac2~]\# nslookup gns.cndba.cn
Server:       192.168.56.200
Address:   192.168.56.200\#53
Name:  gns.cndba.cn
Address:192.168.56.201
[root@rac2~]\#
[root@rac1~]\# nslookup gns.cndba.cn
Server:       192.168.56.200
Address:   192.168.56.200\#53
Name:  gns.cndba.cn
Address:192.168.56.201
[root@rac1~]\# nslookup dns.cndba.cn
Server:       192.168.56.200
Address:   192.168.56.200\#53
Name:  dns.cndba.cn
Address:192.168.56.200
[root@rac1~]\#

# 3   安装配置DHCP

## 3.1             安装DHCP
[root@davedns~]\# yum install dhcp
[root@davedns~]\# rpm -qa | grep "^dhcp"
dhcp-common-4.1.1-43.P1.el6.centos.x86_64
dhcp-4.1.1-43.P1.el6.centos.x86_64

## 3.2             配置DHCP
[root@davedns~]\# vim /etc/dhcp/dhcpd.conf
:r /usr/share/doc/dhcp*/dhcpd.conf.sample        --可以导入配置文件在修改。
也可以直接添加如下内容：

[root@davednsdhcp]\# cat dhcpd.conf
\#
\# DHCPServer Configuration file.
\#   see /usr/share/doc/dhcp*/dhcpd.conf.sample
\#   see 'man 5 dhcpd.conf'
ddns-update-styleinterim;
ignoreclient-updates;
subnet192.168.56.0 netmask 255.255.255.0 {
option routers                  192.168.56.1;
option subnet-mask              255.255.255.0;
option nis-domain               "cndba.cn";
option domain-name              "cndba.cn";
option domain-name-servers      192.168.56.200;
option time-offset              -18000; \# Eastern Standard Time
range dynamic-bootp 192.168.56.100192.168.56.150;
default-lease-time 21600;
max-lease-time 43200;
}
[root@davednsdhcp]\#

## 3.3             重启DHCP
[root@davednsdhcp]\# chkconfig dhcpd on
[root@davednsdhcp]\# chkconfig dhcpd --list
dhcpd             0:off  1:off  2:on   3:on   4:on   5:on   6:off
[root@davednsdhcp]\# service dhcpd restart
Shuttingdown dhcpd:                                       [  OK  ]
Startingdhcpd:                                           [  OK  ]
[root@davednsdhcp]\#


# 4   安装RAC
12cRAC 安装的详细步骤参考如下链接：
Oracle12c RAC 搭建手册
[http://www.cndba.cn/Dave/article/767](http://www.cndba.cn/Dave/article/767)

我们这里贴出不同的地方。
修改hosts文件，注释掉vip的配置，由程序动态获取。
[root@rac2~]\# cat /etc/hosts
127.0.0.1   localhost
192.168.56.5rac1
192.168.57.5rac1-priv
\#192.168.56.7rac1-vip
192.168.56.6rac2
192.168.57.6rac2-priv
\#192.168.56.8rac2-vip
\#192.168.56.9rac-scan
[root@rac2~]\#

![](https://img-blog.csdn.net/20150121163217040?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150121163222937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150121163157062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150121163235121?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



# 5   验证
[grid@rac1grid]$ crs_stat -t
Name           Type           Target    State    Host
------------------------------------------------------------
ora....SM.lsnrora....er.type ONLINE    ONLINE    rac1
ora....ER.lsnrora....er.type ONLINE    ONLINE    rac1
ora....AF.lsnrora....er.type OFFLINE   OFFLINE
ora....N1.lsnrora....er.type ONLINE    ONLINE    rac2
ora....N2.lsnrora....er.type ONLINE    ONLINE    rac1
ora....N3.lsnrora....er.type ONLINE    ONLINE    rac1
ora.MGMTLSNR   ora....nr.type ONLINE    ONLINE   rac1
ora.OCR.dg     ora....up.type ONLINE    ONLINE   rac1
ora.asm        ora.asm.type   ONLINE   ONLINE    rac1
ora.cvu        ora.cvu.type   ONLINE   ONLINE    rac1
ora.gns        ora.gns.type   ONLINE   ONLINE    rac1
ora.gns.vip    ora....ip.type ONLINE    ONLINE   rac1
ora.mgmtdb     ora....db.type ONLINE    ONLINE   rac1
ora....networkora....rk.type ONLINE    ONLINE    rac1
ora.oc4j       ora.oc4j.type  ONLINE   ONLINE    rac1
ora.ons        ora.ons.type   ONLINE   ONLINE    rac1
ora....C1.lsnrapplication    ONLINE    ONLINE   rac1
ora.rac1.ons   application    ONLINE   ONLINE    rac1
ora.rac1.vip   ora....t1.type ONLINE    ONLINE   rac1
ora....C2.lsnrapplication    ONLINE    ONLINE   rac2
ora.rac2.ons   application    ONLINE   ONLINE    rac2
ora.rac2.vip   ora....t1.type ONLINE    ONLINE   rac2
ora.scan1.vip  ora....ip.type ONLINE    ONLINE   rac2
ora.scan2.vip  ora....ip.type ONLINE    ONLINE   rac1
ora.scan3.vip  ora....ip.type ONLINE    ONLINE   rac1
[grid@rac1grid]$
[grid@rac1grid]$ crsctl stat res -t
--------------------------------------------------------------------------------
Name           Target  State       Server                   Statedetails
--------------------------------------------------------------------------------
LocalResources
--------------------------------------------------------------------------------
ora.ASMNET1LSNR_ASM.lsnr
ONLINE  ONLINE      rac1                     STABLE
ONLINE  ONLINE      rac2                     STABLE
ora.LISTENER.lsnr
ONLINE  ONLINE      rac1                     STABLE
ONLINE  ONLINE      rac2                     STABLE
ora.OCR.dg
ONLINE  ONLINE      rac1                     STABLE
ONLINE  ONLINE      rac2                     STABLE
ora.net1.network
ONLINE  ONLINE      rac1                     STABLE
ONLINE  ONLINE      rac2                     STABLE
ora.ons
ONLINE  ONLINE      rac1                     STABLE
ONLINE  ONLINE      rac2                     STABLE
--------------------------------------------------------------------------------
ClusterResources
--------------------------------------------------------------------------------
ora.LISTENER_SCAN1.lsnr
1        ONLINE ONLINE       rac2                     STABLE
ora.LISTENER_SCAN2.lsnr
1        ONLINE ONLINE       rac1                     STABLE
ora.LISTENER_SCAN3.lsnr
1        ONLINE ONLINE       rac1                     STABLE
ora.MGMTLSNR
1       ONLINE  ONLINE       rac1                     169.254.236.113 192.
168.57.5,STABLE
ora.asm
1       ONLINE  ONLINE       rac1                     Started,STABLE
2       ONLINE  ONLINE       rac2                     Started,STABLE
3       OFFLINE OFFLINE                               STABLE
ora.cvu
1       ONLINE  ONLINE       rac1                     STABLE
ora.gns
1       ONLINE  ONLINE       rac1                     STABLE
ora.gns.vip
1       ONLINE  ONLINE       rac1                     STABLE
ora.mgmtdb
1       ONLINE  ONLINE       rac1                     Open,STABLE
ora.oc4j
1       ONLINE  ONLINE       rac1                     STABLE
ora.rac1.vip
1        ONLINE ONLINE       rac1                     STABLE
ora.rac2.vip
1        ONLINE ONLINE       rac2                     STABLE
ora.scan1.vip
1        ONLINE ONLINE       rac2                     STABLE
ora.scan2.vip
1        ONLINE ONLINE       rac1                     STABLE
ora.scan3.vip
1        ONLINE ONLINE       rac1                     STABLE
--------------------------------------------------------------------------------
[grid@rac1grid]$

[grid@rac1grid]$ srvctl config scan
SCANname: scan.cndba.cn, Network: 1
SubnetIPv4: 192.168.56.0/255.255.255.0/eth0, dhcp
SubnetIPv6:
SCAN 0IPv4 VIP: -/scan1-vip/192.168.56.101
SCANVIP is enabled.
SCANVIP is individually enabled on nodes:
SCANVIP is individually disabled on nodes:
SCAN 1IPv4 VIP: -/scan2-vip/192.168.56.102
SCANVIP is enabled.
SCANVIP is individually enabled on nodes:
SCANVIP is individually disabled on nodes:
SCAN 2IPv4 VIP: -/scan3-vip/192.168.56.103
SCANVIP is enabled.
SCANVIP is individually enabled on nodes:
SCANVIP is individually disabled on nodes:
[grid@rac1grid]$
注意这里的IP地址，是从DHCP里自动获取的。

[grid@rac1grid]$ srvctl config vip -node rac1
VIPexists: network number 1, hosting node rac1
VIPIPv4 Address: -/rac1-vip/192.168.56.100
VIPIPv6 Address:
VIP isenabled.
VIP isindividually enabled on nodes:
VIP isindividually disabled on nodes:
[grid@rac1grid]$ srvctl config vip -node rac2
VIPexists: network number 1, hosting node rac2
VIPIPv4 Address: -/rac2-vip/192.168.56.104
VIPIPv6 Address:
VIP isenabled.
VIP isindividually enabled on nodes:
VIP isindividually disabled on nodes:
[grid@rac1grid]$
这里，vip也是一样，自动获取。


--------------------------------------------------------------------------------------------
版权所有，文章禁止转载，否则追究法律责任!
**About Dave:**
--------------------------------------------------------------------------------------------
QQ:492913789
Email:ahdba@qq.com
Blog:http://www.cndba.cn/dave
Weibo:[http://weibo.com/tianlesoftware](http://weibo.com/tianlesoftware)
Twitter:[http://twitter.com/tianlesoftware](http://twitter.com/tianlesoftware)
Facebook:[http://www.facebook.com/tianlesoftware](http://www.facebook.com/tianlesoftware)
Linkedin:[http://cn.linkedin.com/in/tianlesoftware](http://cn.linkedin.com/in/tianlesoftware)
**Dave 的QQ群：**
--------------------------------------------------------------------------------------------
注意:加群必须注明表空间和数据文件关系 | 不要重复加群
CNDBA_1:62697850 (空)    CNDBA_2: 62697716 (满)   CNDBA_3: 283816689
CNDBA_4:391125754   CNDBA_5: 104207940    CNDBA_6:62697977   CNDBA_7: 142216823（满）

