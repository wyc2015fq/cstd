# rsync - 三少GG - CSDN博客
2013年07月31日 18:48:19[三少GG](https://me.csdn.net/scut1135)阅读数：1863
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)

正规文档：1.  [http://www.howtocn.org/rsync:use_rsync_server](http://www.howtocn.org/rsync:use_rsync_server)
                  2. [http://www.nginx.cn/980.html](http://www.nginx.cn/980.html)
                  3. [http://wangxiang2010.blog.51cto.com/287785/455756](http://wangxiang2010.blog.51cto.com/287785/455756)
应用文档：1. [http://www.centos.bz/tag/rsync/](http://www.centos.bz/tag/rsync/)
  2. [http://www.turbocms.com/support/kb/6809.shtml](http://www.turbocms.com/support/kb/6809.shtml)
Rsync是一个远程数据同步工具，可通过LAN 或互联网快速同步多台主机间的文件。Rsync 本来是用以取代 rcp的一个工具，它当前由 rsync.samba.org 维护。Rsync 使用所谓的"Rsync演算法"来使本地和远程两个主机之间的文件达到同步，这个算法只传送两个文件的不同部分，而不是每次都整份传送，因此速度相当快。
Rsync 的特色：
- 
快速：第一次同步时 rsync 会复制全部内容，但在下一次只传输修改过的文件。
- 
安全：rsync 允许通过 ssh 协议来加密传输数据。
- 
更少的带宽：rsync 在传输数据的过程中可以实行压缩及解压缩操作，因此可以使用更少的带宽。
- 
特权：安装和执行 rsync 无需特别的权限
基本语法：
> 
rsync options source destination
源和目标都可以是本地或远程，在进行远程传输的时候，需要指定登录名、远程服务器及文件位置
Fedora rsync工作原理
在两台电脑间同步数据时，必须设置其中一台安装"Fedora rsync server"(Fedora rsync必须以daemon的方式运行，关于daemon，可以参考后面的资料)，同时配置 /etc/Fedora rsyncd.conf。 
有了一台Fedora rsync服务器以后，我们就可以向其他多台机器上同步数据，来建立数据的备份或者镜像。由于使用了“同步算法”和文件比较压缩机制，所以Fedora rsync的工作效率非常高。
使用Fedora rsync离不开ssh(如果对于ssh不熟悉的，可以参考资料中的链接)。
服务器配置
通过配置一个Fedora rsync服务器，我们就可以将他的文件同步回来，如果在两台机器上都配置了Fedora rsync服务，我们就可以实现双向的同步。Fedora rsync作为daemon运行有两种方式，一种是通过inet daemon，一种是采用单独运行的方法。
建议使用单独运行的方法，通过一个cron来定期的同步文件，这种方法可以应对比通过inet方式大得多的访问量。
　　二、配置rsync服务
　　配置一个简单的rsync服务并不复杂，你需要修改或建立一些配置文件。
　　1.rsyncd.conf
　　# vi /etc/rsyncd.conf
　　rsyncd.conf是rsync服务的主要配置文件，它控制rsync服务的各种属性，下面给出一个例子：
```
#先定义整体变量
secrets file = /etc/rsyncd/rsyncd.secrets
motd file = /etc/rsyncd/rsyncd.motd
read only = yes
list = yes
uid = nobody
gid = nobody
hosts allow=192.168.1.0/255.255.255.0 10.0.1.0/255.255.255.0 **(要加上允许的client的ip地址！有可能非局域网)**
hosts deny=*
max connections = 5
log file = /var/log/rsyncd.log
pid file = /var/run/rsyncd.pid
lock file = /var/run/rsyncd.lock
#再定义要rsync目录
[restricted]
comment = restricted directory
path = /home/grant/restricted
auth users = gxxgx,rsync
[mini]
uid = root
gid = root
comment = mini project
path = /home/xxx/mini
```
Debug:
1) 常见错误 rsync: failed to connect to X.X.X.X: No route to host (113) rsync error: error in socket
 IO (code 10) at clientserver.c(107) [sender=2.6.8] 两台主机能相互ping通，报错信息 却说 没有路由到主机! 故障原因：对方没开机、防火墙阻挡、通过的网络上有防火墙阻挡，都有可能。 解决方法：关闭防火墙，或者把防火墙的tcp udp 的873端口打开，允许rsync通过。 实施方案：
防火墙的启动与停止 
```
**service iptables start / stop  （成功，需要sudo）**
```
允许rsync通过防火墙 为防重启将规则清除，我将规则直接加到规则配置文件里边了（/etc/sysconfig/iptables），如下：
```
-A INPUT -p tcp -s X.X.X.X --dport 873 -j ACCEPT
```
注意，这条规则要加在REJECT规则前。 加好后，重启下 iptables（# service iptables restart）。
centos下 貌似需要sudo -i. service iptables stop.
问题五：[http://xinkang120.blog.163.com/blog/static/194668223201110292431571/](http://xinkang120.blog.163.com/blog/static/194668223201110292431571/)
**********************
打开防火墙
iptables ???
iptables -L
结果如下
Chain INPUT (policy ACCEPT)
target prot opt source destination
ACCEPT tcp -- anywhere anywhere tcp dpt:rsync
**********************
2) time out
rsync: failed to connect to 203.100.192.66:** Connection timed out** (110)
rsync error: error in socket IO (code 10) at clientserver.c(124) [receiver=3.0.5]
检查服务器的端口netstat –tunlp，远程telnet测试。
~~在上面的配置文件中，限定了xxx这个子网中，只有xxx的机器可以来访问这台rsync服务器的rsync服务。配置文件的后面部分定义了两个rsync的目录，目录restricted是只有知道gxxgx、rsync两个账号的人才能使用的，而mini目录是无需账号就可以访问的。rsync在定义目录时还提供了一些其它选项，可以作更严格的控制。~~
　　2.rsyncd.secrets
　　# vi /etc/rsyncd.secrets
　　rsyncd.secrets是存储rsync服务的用户名和密码的，它是一个明文的文本文件，下面给出一个rsyncd.secrets文件的例子：
　　terry:123456
　　rsync:abcdef
　　因为rsyncd.secrets存储了rsync服务的用户名和密码，所以非常重要，因此文件的属性必须
设为600，只有所有者可以读写：
　　# chmod 600 /etc/rsyncd.secrets
　　3.rsyncd.motd
　　# vi /etc/rsyncd.motd
　　rsyncd.motd记录了rsync服务的欢迎信息，你可以在其中输入任何文本信息，如：
　　Welcome to the rsync service!
**rsync服务器端配置**
1），我知道的简单注释一下
pid file = /var/run/rsyncd.pid        ＃进程存放位置.pid后缀
use chroot = no                              ＃不允许使用chroot
#read only = yes                              ＃是不是只允许只读
#hosts allow=192.168.1.72/255.255.255.0 192.168.1.5/255.255.255.0        ＃允许的IP
max connections = 5                       ＃最大允许的连接数
motd file = /etc/rsyncd.motd         ＃同步时，提示输入密码前显示的内容
log file = /var/log/rsyncd.log            ＃rsync同步的log文件
log format = %t %a %m %f %b       ＃设置log文件里面的每一行前面的日期形势
syslog facility = local3
timeout = 300                                   ＃超时时间设置
[test]
path = /var/www/zhangying/test           ＃要被同步的目录
list=false                                                    ＃不显示列表
read only = true                                          ＃只读
ignore errors                                              ＃忽视错误
auth users = zhangying                             ＃同步用户
secrets file = /etc/rsyncd.secrets              ＃密码文件
comment = pics test                                  ＃注释
uid = root                                                 #所属用户
gid = root                                                 ＃所属组
2），创建密码文件
nano /etc/rsyncd.secrets
zhangying:**********
保存一下就可以了
3），启动rsync,并查看
启动rsync
 daemon
rsync --daemon
[root@BlackGhost local]# ps -e|grep rsync
16745 ?        00:00:00 rsync
**启动rsync服务器**
启动rsync 服务器相当简单，--daemon 是让rsync 以服务器模式运行；
```
```bash
/usr/bin/**rsync**
```
**```bash--daemon  --config=``````bash/etc/rsyncd/rsyncd``````bash.conf```**
```
sync -参数 用户名@同步服务器的IP::rsyncd.conf中那个方括号里的内容 本地存放路径 如:
[root@BlackGhost local]# **rsync -avzP gxxxgx@192.168.1.110::mini /home/gxx/RsyncTest**
![rsync同步数据](http://blog.51yip.com/wp-content/uploads/2010/02/img_21.jpg)
rsync同步数据
上图中的adfadfadf就是写在下面的文件里面的。
motd file = /etc/rsyncd.motd
说明：
-a 参数，相当于-rlptgoD，-r 是递归 -l 是链接文件，意思是拷贝链接文件；-p 表示保持文件原有权限；-t 保持文件原有时间；-g 保持文件原有用户组；-o 保持文件原有属主；-D 相当于块设备文件；
-z 传输时压缩；
-P 传输进度；
-v 传输时的进度等信息，和-P有点关系，自己试试。可以看文档；
```
```bash
rsync
```
```bash
-avzP  --delete linuxsir@linuxsir.org::linuxsirhome   linuxsirhome
```
```
这回我们引入一个** --delete 选项，表示客户端上的数据要与服务器端完全一致，如果 linuxsirhome目录中有服务器上不存在的文件，则删除。最终目的是让linuxsirhome目录上的数据完全与服务器上保持一致；**用的时候要小心点，最好不要把已经有重要数所据的目录，当做本地更新目录，否则会把你的数据全部删除；
```
```bash
rsync
```
```bash
-avzP  --delete ** --password-**
```
**```bashfile``````bash=``````bashrsync```**
```bash
**.password** 
 linuxsir@linuxsir.org::linuxsirhome   linuxsirhome
```
```
如果需要系统自动同步
**1客户端必须配置密码文件 /etc/rsync.pas   给与相关权限chmod 600 /etc/rsync.pas**
2在crontab中增加一条命令，设置多久自动执行一次。
3为了安全，建议增加一条iptables命令
**样例：**
1 在本地机器上对两个目录进行同步
> 
$ rsync -zvr /var/opt/installation/inventory/ /root/temp 
building file list ... done 
sva.xml 
svB.xml 
. 
sent 26385 bytes received 1098 bytes 54966.00 bytes/sec 
total size is 44867 speedup is 1.63 
$
参数：
-z 开启压缩 
-v 详情输出 
-r 表示递归
2 利用 rsync -a 让同步时保留时间标记
rsync 选项 -a 称为归档模式，执行以下操作
- 
递归模式
- 
保留符号链接
- 
保留权限
- 
保留时间标记
- 
保留用户名及组名
> 
$ rsync -azv /var/opt/installation/inventory/ /root/temp/ 
building file list ... done 
./ 
sva.xml 
svB.xml 
. 
sent 26499 bytes received 1104 bytes 55206.00 bytes/sec 
total size is 44867 speedup is 1.63 
$
3 仅同步一个文件
> 
$ rsync -v /var/lib/rpm/Pubkeys /root/temp/ 
Pubkeys
sent 42 bytes received 12380 bytes 3549.14 bytes/sec 
total size is 12288 speedup is 0.99
4 从本地同步文件到远程服务器
> 
$ rsync -avz /root/temp/ [thegeekstuff@192.168.200.10:/home/thegeekstuff/temp/](mailto:thegeekstuff@192.168.200.10:/home/thegeekstuff/temp/)
Password: 
building file list ... done 
./ 
rpm/ 
rpm/Basenames 
rpm/Conflictname
sent 15810261 bytes received 412 bytes 2432411.23 bytes/sec 
total size is 45305958 speedup is 2.87
就像你所看到的，需要在远程目录前加上 ssh 登录方式，格式为 [username@machinename:path](mailto:username@machinename:path)
5 同步远程文件到本地
和上面差不多，做个相反的操作
> 
$ rsync -avz [thegeekstuff@192.168.200.10:/var/lib/rpm](mailto:thegeekstuff@192.168.200.10:/var/lib/rpm) /root/temp 
Password: 
receiving file list ... done 
rpm/ 
rpm/Basenames 
. 
sent 406 bytes received 15810230 bytes 2432405.54 bytes/sec 
total size is 45305958 speedup is 2.87
6 同步时指定远程 shell
用 -e 参数可以指定远程 ssh ，比如用 rsync -e ssh 来指定为 ssh
> 
$ rsync -avz -e ssh [thegeekstuff@192.168.200.10:/var/lib/rpm](mailto:thegeekstuff@192.168.200.10:/var/lib/rpm) /root/temp 
Password: 
receiving file list ... done 
rpm/ 
rpm/Basenames
sent 406 bytes received 15810230 bytes 2432405.54 bytes/sec 
total size is 45305958 speedup is 2.87
7 不要覆盖被修改过的目的文件
使用 rsync -u 选项可以排除被修改过的目的文件
> 
$ ls -l /root/temp/Basenames 
total 39088 
-rwxr-xr-x 1 root root 4096 Sep 2 11:35 Basenames
$ rsync -avzu [thegeekstuff@192.168.200.10:/var/lib/rpm](mailto:thegeekstuff@192.168.200.10:/var/lib/rpm) /root/temp 
Password: 
receiving file list ... done 
rpm/
sent 122 bytes received 505 bytes 114.00 bytes/sec 
total size is 45305958 speedup is 72258.31
$ ls -lrt 
total 39088 
-rwxr-xr-x 1 root root 4096 Sep 2 11:35 Basenames
8 仅仅同步目录权（不同步文件）
使用 -d 参数
> 
$ rsync -v -d [thegeekstuff@192.168.200.10:/var/lib/](mailto:thegeekstuff@192.168.200.10:/var/lib/) . 
Password: 
receiving file list ... done 
logrotate.status 
CAM/ 
YaST2/ 
acpi/
sent 240 bytes received 1830 bytes 318.46 bytes/sec 
total size is 956 speedup is 0.46
9 查看每个文件的传输进程
使用 - -progress 参数
> 
$ rsync -avz - -progress [thegeekstuff@192.168.200.10:/var/lib/rpm/](mailto:thegeekstuff@192.168.200.10:/var/lib/rpm/) /root/temp/ 
Password: 
receiving file list ... 
19 files to consider 
./ 
Basenames 
5357568 100% 14.98MB/s 0:00:00 (xfer#1, to-check=17/19) 
Conflictname 
12288 100% 35.09kB/s 0:00:00 (xfer#2, to-check=16/19) 
. 
. 
. 
sent 406 bytes received 15810211 bytes 2108082.27 bytes/sec 
total size is 45305958 speedup is 2.87
10 删除在目的文件夹中创建的文件
用 - -delete 参数
> 
# Source and target are in sync. Now creating new file at the target. 
$ > new-file.txt
$ rsync -avz - -delete [thegeekstuff@192.168.200.10:/var/lib/rpm/](mailto:thegeekstuff@192.168.200.10:/var/lib/rpm/) . 
Password: 
receiving file list ... done 
deleting new-file.txt 
./
sent 26 bytes received 390 bytes 48.94 bytes/sec 
total size is 45305958 speedup is 108908.55
11 不要在目的文件夹中创建新文件
有时能只想同步目的地中存在的文件，而排除源文件中新建的文件，可以使用 - -exiting 参数
> 
$ rsync -avz --existing [root@192.168.1.2:/var/lib/rpm/](mailto:root@192.168.1.2:/var/lib/rpm/) . 
[root@192.168.1.2's](mailto:root@192.168.1.2' rel=) password: 
receiving file list ... done 
./
sent 26 bytes received 419 bytes 46.84 bytes/sec 
total size is 88551424 speedup is 198991.96
12 查看源和目的文件之间的改变情况
用 -i 参数
> 
$ rsync -avzi [thegeekstuff@192.168.200.10:/var/lib/rpm/](mailto:thegeekstuff@192.168.200.10:/var/lib/rpm/) /root/temp/ 
Password: 
receiving file list ... done 
>f.st.... Basenames 
.f....og. Dirnames
sent 48 bytes received 2182544 bytes 291012.27 bytes/sec 
total size is 45305958 speedup is 20.76
输出结果中在每个文件最前面会多显示 9 个字母，分别表示为
> 已经传输 
f 表示这是一个文件 
d 表示这是一个目录 
s 表示尺寸被更改 
t 时间标记有变化 
o 用户被更改 
g 用户组被更改
13 在传输时启用包含和排除模式
> 
$ rsync -avz - -include 'P*' - -exclude '*' [thegeekstuff@192.168.200.10:/var/lib/rpm/](mailto:thegeekstuff@192.168.200.10:/var/lib/rpm/) /root/temp/ 
Password: 
receiving file list ... done 
./ 
Packages 
Providename 
Provideversion 
Pubkeys
sent 129 bytes received 10286798 bytes 2285983.78 bytes/sec 
total size is 32768000 speedup is 3.19
14 不要传输大文件
使用 - - max-size 参数
> 
$ rsync -avz - -max-size='100K' [thegeekstuff@192.168.200.10:/var/lib/rpm/](mailto:thegeekstuff@192.168.200.10:/var/lib/rpm/) /root/temp/ 
Password: 
receiving file list ... done 
./ 
Conflictname 
Group 
Installtid 
Name 
Sha1header 
Sigmd5 
Triggername
sent 252 bytes received 123081 bytes 18974.31 bytes/sec 
total size is 45305958 speedup is 367.35
15 传输所有文件
不管有没有改变，再次把所有文件都传输一遍，用 -W 参数
> 
# rsync -avzW [thegeekstuff@192.168.200.10:/var/lib/rpm/](mailto:thegeekstuff@192.168.200.10:/var/lib/rpm/) /root/temp 
Password: 
receiving file list ... done 
./ 
Basenames 
Conflictname 
Dirnames 
Filemd5s 
Group 
Installtid 
Name
sent 406 bytes received 15810211 bytes 2874657.64 bytes/sec 
total size is 45305958 speedup is 2.87
四、利用rsync保持Linux服务器间的文件同步实例
　　现在假设有两台Linux服务器A(192.168.100.21)和B(192.168.100.90)，服务器A中的
/home/terry和服务器B中的/home/terry这两个目录需要保持同步，也就是当服务器A中文件发生
改变后，服务器B中的文件也要对应去改变。
　　我们按上面的方法，在服务器A上安装rsync，并将其配置为一台rsync服务器，并将/home/terry
目录配置成rsync共享出的目录。然后在服务器B上安装rsync，因为B只做客户端，所以无需配置。
然后在服务器B，建立以下脚本：
　　#!/bin/bash
　　/usr/loca/rsync/bin/rsync -vazu -progress --delete
　　--password-file=/etc/rsync.secret terry@192.168.100.21:/terry/ /home
　　将这个脚本保存为AtoB.sh，并加上可执行属性：
　　# chmod 755 /root/AtoB.sh
　　然后，通过crontab设定，让这个脚本每30分钟运行一次。执行命令：
　　# crontab -e
　　输入以下一行：
　　0,30 * * * * /root/AtoB.sh
　　保存退出，这样服务器B每个小时的0分和30分时都会自动运行一次AtoB.sh，AtoB.sh是负责
保持服务器B和服务器A同步的。这样就保证了服务器A的所有更新在30钟后，服务器B也一样取
得了和服务器A一样的最新的资料。
　　五、其它应用
　　rsync除了同步文件及目录之外，还可以利用它来实现对远程网站的远程备份。如果再结合脚本和Crontab就能实现定时自动远程备份。其可以实现与商业化的备份和镜象产品的类似效果，但完全免费。
         附：rsync有六种不同的工作模式：
　　1. 拷贝本地文件；当SRC和DES路径信息都不包含有单个冒号":"分隔符时就启动这种工作模式。
　　2.使用一个远程shell程序（如rsh、ssh）来实现将本地机器的内容拷贝到远程机器。当DST
路径地址包含单个冒号":"分隔符时启动该模式。
　　3.使用一个远程shell程序（如rsh、ssh）来实现将远程机器的内容拷贝到本地机器。当SRC
地址路径包含单个冒号":"分隔符时启动该模式。
　　4. 从远程rsync服务器中拷贝文件到本地机。当SRC路径信息包含"::"分隔符时启动该模式。
　　5. 从本地机器拷贝文件到远程rsync服务器中。当DST路径信息包含"::"分隔符时启动该模式。
　　6. 列远程机的文件列表。这类似于rsync传输，不过只要在命令中省略掉本地机信息即可。
四、利用rsync保持Linux服务器间的文件同步实例
　　现在假设有两台Linux服务器A(192.168.100.21)和B(192.168.100.90)，服务器A中的
/home/terry和服务器B中的/home/terry这两个目录需要保持同步，也就是当服务器A中文件发生
改变后，服务器B中的文件也要对应去改变。
　　我们按上面的方法，在服务器A上安装rsync，并将其配置为一台rsync服务器，并将/home/terry
目录配置成rsync共享出的目录。然后在服务器B上安装rsync，因为B只做客户端，所以无需配置。
然后在服务器B，建立以下脚本：
　　#!/bin/bash
　　/usr/loca/rsync/bin/rsync -vazu -progress --delete
　　--password-file=/etc/rsync.secret terry@192.168.100.21:/terry/ /home
　　将这个脚本保存为AtoB.sh，并加上可执行属性：
　　# chmod 755 /root/AtoB.sh
　　然后，通过crontab设定，让这个脚本每30分钟运行一次。执行命令：
　　# crontab -e
　　输入以下一行：
　　0,30 * * * * /root/AtoB.sh
　　保存退出，这样服务器B每个小时的0分和30分时都会自动运行一次AtoB.sh，AtoB.sh是负责
保持服务器B和服务器A同步的。这样就保证了服务器A的所有更新在30钟后，服务器B也一样取
得了和服务器A一样的最新的资料。
　　五、其它应用
　　rsync除了同步文件及目录之外，还可以利用它来实现对远程网站的远程备份。如果再结合脚本和Crontab就能实现定时自动远程备份。其可以实现与商业化的备份和镜象产品的类似效果，但完全免费。
         附：rsync有六种不同的工作模式：
　　1. 拷贝本地文件；当SRC和DES路径信息都不包含有单个冒号":"分隔符时就启动这种工作模式。
　　2.使用一个远程shell程序（如rsh、ssh）来实现将本地机器的内容拷贝到远程机器。当DST
路径地址包含单个冒号":"分隔符时启动该模式。
　　3.使用一个远程shell程序（如rsh、ssh）来实现将远程机器的内容拷贝到本地机器。当SRC
地址路径包含单个冒号":"分隔符时启动该模式。
　　4. 从远程rsync服务器中拷贝文件到本地机。当SRC路径信息包含"::"分隔符时启动该模式。
　　5. 从本地机器拷贝文件到远程rsync服务器中。当DST路径信息包含"::"分隔符时启动该模式。
　　6. 列远程机的文件列表。这类似于rsync传输，不过只要在命令中省略掉本地机信息即可。x
