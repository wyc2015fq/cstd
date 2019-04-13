
# 在CentOS6.2下安装DNS服务软件Bind并快速配置简单实例 - 高科的专栏 - CSDN博客

2012年08月22日 20:44:35[高科](https://me.csdn.net/pbymw8iwm)阅读数：6476


[实践Ok]在CentOS6.2下安装DNS并快速配置实例，共八步，心路历程如下：
背景介绍：
在日常的开发中，往往会在测试机和外网的Http的Url实际接口是不一样的，在测试机一个Url地址，在外网中又是一个地址。
这样在我们在开发时，在上线时就会去修改一下接口，在线下测试时用线下的Url接口，最后的情况是，在SVN的管理中会变得
很混乱，也就是在上线上的版本和线下测试的版本在改动上来回切换，版本号不段的切换，为此，为了解决这个问题，必须引入
自己建立一套内网测试系统的DNS来实现其和外网一样的环境，进而在实际开发中就不会出现因URL的接口包含的不同域名而导致
了代码版本管理的版本号不段攀升。
实际操作：
在Linux下尤其是CentOS这样的免费系统上有Yum这样的包管理工具来安DNS服务器，但是我在实践中发现其并不理想，可能是打出的Rpm包多少有些
不太让人满意，我试过用直接运行：yum install bind bind-utils bind-libs bind-chroot caching-nameserver，但后来在配置DNS时出现这样那样的问题.
找不到配置文件?（loading from master file 225.168.192.in-add.arpa failed: file not found）我X,什么权限问题都试了，但就是搞不定，于是否，
改用源码安装，这样来得更保险此些，操控性也强很多，有人问我，你的人生由谁来操盘：我操！
我的上手安装和配置如下：
一：开始安装DNS服务器Bind：
下载 bind  http://www.isc.org,解压bind-9.9.1-P1.tar.gz。
tar -zxvf bind-9.9.1-P1.tar.gz
进入 bind-9.9.1-P1.tar.gz文件夹
cd bind-9.9.1-P1
创建安装目录，我是安装在 /usr/local/named
mkdir /usr/local/named
编译,指定安装目录，指定man目录，开启多线程支持（测试环境也就没有必要搞多线程了，要整看帮助。）
./configure --prefix=/usr/local/named
Make 大约需要几分钟，只要不报错就继续下去。
make
Make install 安装
make install
没有报错，就表示安装成功了。
创建以下目录以备用
mkdir /usr/local/named/namedb
二：开始配置bind
创建 rndc.conf文件，用bind自带程序生成
cd /usr/local/named/
sbin/rndc-confgen > etc/rndc.conf   //一直死在这儿了，只得强制终止。
查原因，在网上搜索：linux安装dns，rndc-confgen没反应，哈，有一篇文章说到点上了，
Url：http://www.nginxs.com/linux/43.html，他说是，摘录如下：
在官方网站上看到这么一条信息
You must use the keyboard to create entropy, since your system is lacking
/dev/random (or equivalent)
start typing:
rndc-confgen: generate key: out of entropy
大概意思就是服务器上没有random产生器，这种情况下我们就手动伪造一个文件代替/dev/random的功能
\#\#\#新建一个 random 文件随即输入一串数字“记得要长~~
shell $> vim random
asdkfjalsjdflajsldfjlasjdflajsldfjalsjdflajslfjalsjflasjfl
\#\#\#查看 rndc-confgen 帮助
shell $> ../sbin/rndc-confgen –help
rndc-confgen: invalid argument –
Usage:
rndc-confgen [-a] [-b bits] [-c keyfile] [-k keyname] [-p port] [-r randomfile] [-s addr] [-t chrootdir] [-u user]
-a:           generate just the key clause and write it to keyfile (/usr/local/named/etc/rndc.key)
-b bits:      from 1 through 512, default 128; total length of the secret
-c keyfile:   specify an alternate key file (requires -a)
-k keyname:   the name as it will be used  in named.conf and rndc.conf
-p port:      the port named will listen on and rndc will connect to
-r randomfile: a file containing random data
-s addr:      the address to which rndc should connect
-t chrootdir: write a keyfile in chrootdir as well (requires -a)
-u user:      set the keyfile owner to “user” (requires -a)
[view plain](http://www.justwinit.cn/post/5338/#)[copy to clipboard](http://www.justwinit.cn/post/5338/#)[print](http://www.justwinit.cn/post/5338/#)[?](http://www.justwinit.cn/post/5338/#)
shell $>../sbin/rndc-confgen -r random>rndc.key
shell $> ../sbin/rndc-confgen -r random > rndc.key,上面这一句是核心，自己整个Random文件是桥梁。
tail -10 rndc.key | head -9 | sed 's/\# //g' > named.conf
把这个named.conf放在etc的文件夹子下：/usr/local/named/etc/named.conf。
ok 问题解决了，接下来我们配置 我们的 域名服务器吧。
三：配置篇
编辑named.conf
\# vi  /usr/local/named/etc/named.conf
写入以下内容：我的Ip：192.168.225.128 ，注意：225.168.192.in-add.arpa是Ip的反解，刚好倒过来。
[view plain](http://www.justwinit.cn/post/5338/#)[copy to clipboard](http://www.justwinit.cn/post/5338/#)[print](http://www.justwinit.cn/post/5338/#)[?](http://www.justwinit.cn/post/5338/#)
key "rndc-key" {
algorithm hmac-md5;
secret "SnK8Ph1zCAnnykZ07qt+TQ==";
};
controls {
inet 127.0.0.1 port 953
allow { 127.0.0.1; } keys { "rndc-key"; };
};
options {
Directory "/usr/local/named";
Pid-file "named.pid";
Allow-query { any ;};
Dump-file "/usr/local/named/data/cache_dump.db";
Statistics-file "/usr/local/named/data/named_stats.txt";
};
Zone "localhost" in {
Type master;
File "localhost.zone";
};
Zone "0.0.127.in-addr.arpa" in {
Type master;
File "localhost.rev";
};
zone "51test.com" {
type master;
file "51test.com.zone";
};zone "225.168.192.in-add.arpa" IN {
type master;
file "225.168.192.in-add.arpa";
};

key "rndc-key" {
  algorithm hmac-md5;
  secret "SnK8Ph1zCAnnykZ07qt+TQ==";
};
controls {
  inet 127.0.0.1 port 953
    allow { 127.0.0.1; } keys { "rndc-key"; };
};
options {
  Directory "/usr/local/named";
  Pid-file "named.pid";
  Allow-query { any ;};
  Dump-file "/usr/local/named/data/cache_dump.db";
  Statistics-file "/usr/local/named/data/named_stats.txt";
};
Zone "localhost" in {
  Type master;
  File "localhost.zone";
};
Zone "0.0.127.in-addr.arpa" in {
  Type master;
  File "localhost.rev";
};
zone "51test.com" {
      type master;
      file "51test.com.zone";
};zone "225.168.192.in-add.arpa" IN {
        type master;
         file "225.168.192.in-add.arpa";
};
退出，保存。
创建并编辑 localhost.zone 文件
vi  /usr/local/named/localhost.zone
写入以下内容：
$TTL 3600
$ORIGIN 127.0.0.1.
@  1D IN SOA localhost. root.localhost. (
42         ;
3H         ;
15M        ;
1W         ;
3600);
1D IN NS  127.0.0.1
1D IN A   127.0.0.1

$TTL 3600
$ORIGIN 127.0.0.1.
@  1D IN SOA localhost. root.localhost. (
                     42         ;
                     3H         ;
                     15M        ;
                     1W         ;
                     3600);
   1D IN NS  127.0.0.1
   1D IN A   127.0.0.1
创建并编辑 localhost.rev 文件
\# vi /usr/local/named/localhost.rev
$TTL 3600
@  IN SOA  localhost.  root.localhost. (
1; serial
3600; refresh every hour
900;  retry every 15 minutes
3600000; expire 1000 hours
3600); minimun 1 hour
IN NS localhost.
1  IN PTR localhost.

$TTL 3600
@  IN SOA  localhost.  root.localhost. (
              1; serial
              3600; refresh every hour
              900;  retry every 15 minutes
              3600000; expire 1000 hours
              3600); minimun 1 hour
   IN NS localhost.
1  IN PTR localhost.
因前面在，51test.com这个是在其他网页上抄过来的简单示例，于是也就顺便把文件也贴上：
vi  /usr/local/named/51test.com.zone
$TTL 86400
@        IN        SOA        www.51test.com.  root.localhost (
2 ; serial
28800 ; refresh
7200 ; retry
604800 ; expire
86400 ; ttl
)
IN      NS      www.51test.com.
www      IN      A       192.168.225.128
admin     IN      A      192.168.225.128

$TTL 86400
@        IN        SOA        www.51test.com.  root.localhost (
                        2 ; serial
                        28800 ; refresh
                        7200 ; retry
                        604800 ; expire
                        86400 ; ttl
                        )
        IN      NS      www.51test.com.
www      IN      A       192.168.225.128
admin     IN      A      192.168.225.128
再不是51test.com的反解配置文件：
vi /usr/local/named/225.168.192.in-add.arpa
$TTL    86400
@       IN      SOA     51test.com.  root.51test.com.  (
1997022700 ; Serial
28800      ; Refresh
14400      ; Retry
3600000    ; Expire
86400 )    ; Minimum
@         IN      NS      51test.com.
128       IN      PTR    www.51test.com.
128       IN      PTR    admin.51test.com.

$TTL    86400
@       IN      SOA     51test.com.  root.51test.com.  (
                                      1997022700 ; Serial
                                      28800      ; Refresh
                                      14400      ; Retry
                                      3600000    ; Expire
                                      86400 )    ; Minimum
@         IN      NS      51test.com.
128       IN      PTR    www.51test.com.
128       IN      PTR    admin.51test.com.
配置注意事项：
配置文件中的 "@" 符号前不能有任何空白字符
配置文件中的 "IN" 字符前必须有空格或TAB
到此，配置大功告成。
四：
下载一个named.root 到/usr/local/named/下
ftp://ftp.rs.internic.net/domain/named.root
Ftp失效，从这儿下载：
http://www.smth.edu.cn/bbsgcon.php?board=FreeBSD&num=1209
特别注意：bind的配置文档是区分大小写的,因现在那个FTP好像下载不了，于是我也贴下面。
vi /usr/local/named/named.ca
[view plain](http://www.justwinit.cn/post/5338/#)[copy to clipboard](http://www.justwinit.cn/post/5338/#)[print](http://www.justwinit.cn/post/5338/#)[?](http://www.justwinit.cn/post/5338/#)
more named.root
; This file holds the information on root name servers needed to
; initialize cache of Internet domain name servers
; (e.g. reference this file in the "cache .<file>"
; configuration file of BIND domain name servers).
;
; This file is made available by InterNIC
; under anonymous FTP as
; file /domain/named.root
; on server FTP.INTERNIC.NET
;
; last update: Nov 5, 2002
; related version of root zone: 2002110501
;
;
; formerly NS.INTERNIC.NET
;
. 3600000 IN NS A.ROOT-SERVERS.NET.
A.ROOT-SERVERS.NET. 3600000 A 198.41.0.4
;
; formerly NS1.ISI.EDU
;
. 3600000 NS B.ROOT-SERVERS.NET.
B.ROOT-SERVERS.NET. 3600000 A 128.9.0.107
;
; formerly C.PSI.NET
;
. 3600000 NS C.ROOT-SERVERS.NET.
C.ROOT-SERVERS.NET. 3600000 A 192.33.4.12
;
; formerly TERP.UMD.EDU
;
. 3600000 NS D.ROOT-SERVERS.NET.
D.ROOT-SERVERS.NET. 3600000 A 128.8.10.90
;
; formerly NS.NASA.GOV
;
. 3600000 NS E.ROOT-SERVERS.NET.
E.ROOT-SERVERS.NET. 3600000 A 192.203.230.10
;
; formerly NS.ISC.ORG
;
. 3600000 NS F.ROOT-SERVERS.NET.
F.ROOT-SERVERS.NET. 3600000 A 192.5.5.241
;
; formerly NS.NIC.DDN.MIL
;
. 3600000 NS G.ROOT-SERVERS.NET.
G.ROOT-SERVERS.NET. 3600000 A 192.112.36.4
;
; formerly AOS.ARL.ARMY.MIL
;
. 3600000 NS H.ROOT-SERVERS.NET.
H.ROOT-SERVERS.NET. 3600000 A 128.63.2.53
;
; formerly NIC.NORDU.NET
;
. 3600000 NS I.ROOT-SERVERS.NET.
I.ROOT-SERVERS.NET. 3600000 A 192.36.148.17
;
; operated by VeriSign, Inc.
;
. 3600000 NS J.ROOT-SERVERS.NET.
J.ROOT-SERVERS.NET. 3600000 A 192.58.128.30
;
; housed in LINX, operated by RIPE NCC
;
. 3600000 NS K.ROOT-SERVERS.NET.
K.ROOT-SERVERS.NET. 3600000 A 193.0.14.129
;
; operated by IANA
;
. 3600000 NS L.ROOT-SERVERS.NET.
L.ROOT-SERVERS.NET. 3600000 A 198.32.64.12
;
; housed in Japan, operated by WIDE
;
. 3600000 NS M.ROOT-SERVERS.NET.
M.ROOT-SERVERS.NET. 3600000 A 202.12.27.33
; End of File

more named.root
; This file holds the information on root name servers needed to
; initialize cache of Internet domain name servers
; (e.g. reference this file in the "cache . <file>"
; configuration file of BIND domain name servers).
;
; This file is made available by InterNIC
; under anonymous FTP as
; file /domain/named.root
; on server FTP.INTERNIC.NET
;
; last update: Nov 5, 2002
; related version of root zone: 2002110501
;
;
; formerly NS.INTERNIC.NET
;
. 3600000 IN NS A.ROOT-SERVERS.NET.
A.ROOT-SERVERS.NET. 3600000 A 198.41.0.4
;
; formerly NS1.ISI.EDU
;
. 3600000 NS B.ROOT-SERVERS.NET.
B.ROOT-SERVERS.NET. 3600000 A 128.9.0.107
;
; formerly C.PSI.NET
;
. 3600000 NS C.ROOT-SERVERS.NET.
C.ROOT-SERVERS.NET. 3600000 A 192.33.4.12
;
; formerly TERP.UMD.EDU
;
. 3600000 NS D.ROOT-SERVERS.NET.
D.ROOT-SERVERS.NET. 3600000 A 128.8.10.90
;
; formerly NS.NASA.GOV
;
. 3600000 NS E.ROOT-SERVERS.NET.
E.ROOT-SERVERS.NET. 3600000 A 192.203.230.10
;
; formerly NS.ISC.ORG
;
. 3600000 NS F.ROOT-SERVERS.NET.
F.ROOT-SERVERS.NET. 3600000 A 192.5.5.241
;
; formerly NS.NIC.DDN.MIL
;
. 3600000 NS G.ROOT-SERVERS.NET.
G.ROOT-SERVERS.NET. 3600000 A 192.112.36.4
;
; formerly AOS.ARL.ARMY.MIL
;
. 3600000 NS H.ROOT-SERVERS.NET.
H.ROOT-SERVERS.NET. 3600000 A 128.63.2.53
;
; formerly NIC.NORDU.NET
;
. 3600000 NS I.ROOT-SERVERS.NET.
I.ROOT-SERVERS.NET. 3600000 A 192.36.148.17
;
; operated by VeriSign, Inc.
;
. 3600000 NS J.ROOT-SERVERS.NET.
J.ROOT-SERVERS.NET. 3600000 A 192.58.128.30
;
; housed in LINX, operated by RIPE NCC
;
. 3600000 NS K.ROOT-SERVERS.NET.
K.ROOT-SERVERS.NET. 3600000 A 193.0.14.129
;
; operated by IANA
;
. 3600000 NS L.ROOT-SERVERS.NET.
L.ROOT-SERVERS.NET. 3600000 A 198.32.64.12
;
; housed in Japan, operated by WIDE
;
. 3600000 NS M.ROOT-SERVERS.NET.
M.ROOT-SERVERS.NET. 3600000 A 202.12.27.33
; End of File
五：运行Bind程序，如下方式加载配置文件启动，加上调试信息参数：
/usr/local/named/sbin/named -gc /usr/local/named/etc/named.conf
如果运行结果最后一行显示Running
表明安装并启动成功。
最后需要注意的内容是不要忘了是否真正的开启服务器端口（以下是named.conf文件中端口的设置语句，可以看到端口号是 53：telnet x.x.x.x 53）
当然也可以用：nmap localhost ，没有yum安装，这种小工具问题不大用yum来做。
yum install nmap
Total download size: 2.3 M
Installed size: 7.5 M
Is this ok [y/N]: y
Downloading Packages:
root@192.168.225.128:/usr/local/named\# nmap localhost
Starting Nmap 5.21 ( http://nmap.org ) at 2012-06-18 03:09 CST
Nmap scan report for localhost (127.0.0.1)
Host is up (0.000033s latency).
Hostname localhost resolves to 2 IPs. Only scanned 127.0.0.1
Not shown: 994 closed ports
PORT     STATE SERVICE
22/tcp   open  ssh
53/tcp   open  domain
80/tcp   open  http
111/tcp  open  rpcbind
3306/tcp open  mysql
9000/tcp open  cslistener
端口号是 53就是DNS Bind的端口。
六：停止及启动：
root@192.168.225.128:/usr/local/named\# ps aux|grep name
root     19024  0.0  1.2  10624  6216 pts/0    T    03:00   0:00 /usr/local/named/sbin/named -gc /usr/local/named/etc/named.conf
root@192.168.225.128:/usr/local/named\# kill -9 19024
root@192.168.225.128:/usr/local/named\# ps aux|grep name
[2]+  已杀死               /usr/local/named/sbin/named -gc /usr/local/named/etc/named.conf
七：本机测试，外部机器测试是否dns生效：
vi /etc/resolv.conf
加上，去掉注释其他Dns的Ip值：
nameserver 192.168.225.128
启动Dns后，清楚Bind服务器上的Dns缓存：
root@192.168.225.128:/usr/local/named\# ./sbin/rndc flush
root@192.168.225.128:/usr/local/named\# ping www.51test.com
PING www.51test.com (192.168.225.128) 56(84) bytes of data.
64 bytes from 192.168.225.128: icmp_seq=1 ttl=64 time=0.025 ms
64 bytes from 192.168.225.128: icmp_seq=2 ttl=64 time=0.058 ms
八：将服务设置为开机自启,把named 添加到启动项，随操作系统一起启动即可：
root@192.168.225.128:/usr/local/named\# chkconfig --list named 
named           0:关闭  1:关闭  2:关闭  3:关闭  4:关闭  5:关闭  6:关闭
root@192.168.225.128:/usr/local/named\# chkconfig --level 345 named on
root@192.168.225.128:/usr/local/named\# chkconfig --list named        
named           0:关闭  1:关闭  2:关闭  3:启用  4:启用  5:启用  6:关闭
但是，我们现在是源码的安装，So，得这样子喔。
修改来自Apache里的启动shell（参看一下自启动的标准），也可以不用修改，直接如下也成，内容如下：
\#!/bin/bash  
\#chkconfig:345 61 61  
\#description: dns start shell  
echo "Starting DNS daemon..."
pkill -9 named  
ulimit -SHn 51200  
nohup  /usr/local/named/sbin/named -gc /usr/local/named/etc/named.conf > /dev/null 2>&1 &
1）保存为/etc/init.d/dns
脚本执行权限添加：
2）chmod -R a+x /etc/init.d/dns  //env: /etc/init.d/dns: 权限不够 ,必须给加上。
3）把脚本拷贝至/etc/init.d/目录下,执行命令:
ln -s /etc/init.d/dns /etc/rc.d/rc3.d/S61dns
ln -s /etc/init.d/dns /etc/rc.d/rc4.d/S61dns
ln -s /etc/init.d/dns /etc/rc.d/rc5.d/S61dns
再执行：
chkconfig --add dns  //解释：相当于Windows下的自动启动服务，特别要注意：chkconfig –add dns的时候，出现dns服务不支持chkconfig。
如果想让服务支持chkconfig，必须定义服务的启动级，启动优先级，关闭优先级，还有描述，如上，必须得加上描述等。
chkconfig --levels dns345 on //也成
开机启动设置完毕：
service dns start
-----------------------------------------------
;特别要注意：chkconfig –add dns的时候，出现dns服务不支持chkconfig。
chkconfig --add dns
service dns start
Starting DNS daemon...
ps aux|grep named
root     19441  0.0  1.1  10364  5872 pts/2    S    04:29   0:00 /usr/local/named/sbin/named -gc /usr/local/named/etc/named.conf
下次一开机也就会运行这个脚本，以启动自己定义的Dns服务器。
reboot
reboot
启动后，查看是否自己就启动了呢？如下：
root@192.168.225.128:~\# ps aux|grep named
root      1272  0.0  1.1  10232  5680 ?        S    05:20   0:00 /usr/local/named/sbin/named -gc /usr/local/named/etc/named.conf
root      2039  0.0  0.1   5956   756 pts/0    S+   05:22   0:00 grep named
root@192.168.225.128:~\# ps aux|grep named
root      1272  0.0  1.1  10232  5680 ?        S    05:20   0:00 /usr/local/named/sbin/named -gc /usr/local/named/etc/named.conf
root      2039  0.0  0.1   5956   756 pts/0    S+   05:22   0:00 grep named
果然，又启动起来了，大功告成，整完收工。
Dns服务器从安装到调试配置并测试完成，Eof。Write and Opt  By:jackxiang 2012/06/17 20:30:00

后来，我自己配置了一个新的域名：naimanqi.com.cn ，我的Linux机器IP是：192.168.1.105
修改配置文件：
1）共用如下：
[view plain](http://www.justwinit.cn/post/5338/#)[copy to clipboard](http://www.justwinit.cn/post/5338/#)[print](http://www.justwinit.cn/post/5338/#)[?](http://www.justwinit.cn/post/5338/#)
options {
Directory"/usr/local/named";
Pid-file"named.pid";
Allow-query { any ;};
Dump-file"/usr/local/named/data/cache_dump.db";
Statistics-file"/usr/local/named/data/named_stats.txt";
};
2）添加如下两个正向解析和反向解析的配置文件：
options {
        Directory "/usr/local/named";
        Pid-file "named.pid";
        Allow-query { any ;};
        Dump-file "/usr/local/named/data/cache_dump.db";
        Statistics-file "/usr/local/named/data/named_stats.txt";
};
2）添加如下两个正向解析和反向解析的配置文件：
[view plain](http://www.justwinit.cn/post/5338/#)[copy to clipboard](http://www.justwinit.cn/post/5338/#)[print](http://www.justwinit.cn/post/5338/#)[?](http://www.justwinit.cn/post/5338/#)
vi  /usr/local/named/etc/named.conf
zone"naimanqi.com.cn"{
type master;
file"naimanqi.com.cn.zone";
};zone"1.168.192.in-add.arpa"IN {
type master;
file"1.168.192.in-add.arpa";
};
vi  /usr/local/named/etc/named.conf
zone "naimanqi.com.cn" {
      type master;
      file "naimanqi.com.cn.zone";
};zone "1.168.192.in-add.arpa" IN {
        type master;
         file "1.168.192.in-add.arpa";
};
3）反向解析文件：
[view plain](http://www.justwinit.cn/post/5338/#)[copy to clipboard](http://www.justwinit.cn/post/5338/#)[print](http://www.justwinit.cn/post/5338/#)[?](http://www.justwinit.cn/post/5338/#)
vi /usr/local/named/1.168.192.in-add.arpa
$TTL86400
@       IN      SOA     naimanqi.com.cn.  root.naimanqi.com.cn.  (
1997022700 ; Serial
28800      ; Refresh
14400      ; Retry
3600000    ; Expire
86400 )    ; Minimum
@         IN      NS      naimanqi.com.cn.
128       IN      PTR    www.naimanqi.com.cn.
128       IN      PTR    admin.naimanqi.com.cn.
vi /usr/local/named/1.168.192.in-add.arpa
$TTL    86400
@       IN      SOA     naimanqi.com.cn.  root.naimanqi.com.cn.  (
                                      1997022700 ; Serial
                                      28800      ; Refresh
                                      14400      ; Retry
                                      3600000    ; Expire
                                      86400 )    ; Minimum
@         IN      NS      naimanqi.com.cn.
128       IN      PTR    www.naimanqi.com.cn.
128       IN      PTR    admin.naimanqi.com.cn.
4)正向解析的文件：
[view plain](http://www.justwinit.cn/post/5338/#)[copy to clipboard](http://www.justwinit.cn/post/5338/#)[print](http://www.justwinit.cn/post/5338/#)[?](http://www.justwinit.cn/post/5338/#)
vi  /usr/local/named/naimanqi.com.cn.zone
$TTL86400
@        IN        SOA        naimanqi.com.cn.  root.localhost (
2 ; serial
28800 ; refresh
7200 ; retry
604800 ; expire
86400 ; ttl
)
IN      NS      naimanqi.com.com.
IN      A       192.168.1.105
www      IN      A       192.168.1.105
admin     IN      A      192.168.1.105
vi  /usr/local/named/naimanqi.com.cn.zone
$TTL 86400
@        IN        SOA        naimanqi.com.cn.  root.localhost (
                        2 ; serial
                        28800 ; refresh
                        7200 ; retry
                        604800 ; expire        
                        86400 ; ttl
                        )
        IN      NS      naimanqi.com.com.
         IN      A       192.168.1.105
www      IN      A       192.168.1.105
admin     IN      A      192.168.1.105
注意这一行：          IN      A       192.168.1.105
这个目的是把浏览器访问：naimanqi.com.cn这个域名也给解析到IP: 192.168.1.105上来，跟在后面的www  IN      A       192.168.1.105 这个是对www解析的，其实它是一个二级域名和admin 这种二级域名是一个意思：admin     IN      A      192.168.1.105 。。。EOF。
最后，可以这样玩这个DNS，这个CentOs6.3的Linux 不是建立在这个NAT下的嘛，后修改为Bridge（桥连），这样后，反正是在一个网段内（路由器下），把自己外网的这台Windows下的机器的DNS由192.168.1.1修改为：192.168.1.105，这样既可以调试刚才配置的内部DNS进行调试，也可 以访问外网，为什么呢？是因为：
[view plain](http://www.justwinit.cn/post/5338/#)[copy to clipboard](http://www.justwinit.cn/post/5338/#)[print](http://www.justwinit.cn/post/5338/#)[?](http://www.justwinit.cn/post/5338/#)
C:\DocumentsandSettings\Administrator>nslookup justwinit.cn
DNS request timed out.
timeout was 2 seconds.
*** Can't find server nameforaddress 192.168.1.105: Timed out
*** Default servers are not available
Server:  UnKnown
Address:  192.168.1.105
Non-authoritative answer:
Name:    justwinit.cn
Address:  72.46.128.86
C:\Documents and Settings\Administrator>nslookup justwinit.cn
DNS request timed out.
    timeout was 2 seconds.
*** Can't find server name for address 192.168.1.105: Timed out
*** Default servers are not available
Server:  UnKnown
Address:  192.168.1.105
Non-authoritative answer:
Name:    justwinit.cn
Address:  72.46.128.86
是因，内部DNS没有找到那个地址后，它自动转给上一级DNS去了，后得到那个外部网站的IP地址了。但修改为NAT后，好像启动Centos里的DNS后，再在Windows上修改IP为桥连后的IP好像DNS解析不了，在Linux自己修改 /etc/resolve.conf后是可以的，所以，建议用桥连方式来开发，少用NAT方式，这样可以在一个路由器下的电脑都可以配置上这个Dns来访问该网站和开发网部（samba），很是方便，而DNS设置后，如果没有这个网站在内网，它便会访问外网，这种机制也是很好的

