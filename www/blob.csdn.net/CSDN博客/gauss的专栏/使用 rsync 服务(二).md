# 使用 rsync 服务(二) - gauss的专栏 - CSDN博客
2015年08月15日 11:57:06[gauss](https://me.csdn.net/mathlmx)阅读数：265
个人分类：[linux](https://blog.csdn.net/mathlmx/article/category/2303095)
## 内容提要
- 
掌握两种运行 rsync 服务的配置方法
- 
熟悉配置文件 rsyncd.conf 的常用参数
- 
掌握在生产服务器上同时运行 rsync 服务的配置方法
- 
掌握在备份服务器上运行 rsync 服务的配置方法
- 
重点掌握匿名 rsync 服务的配置方法
## rsync 服务简介
[上一节](http://www.howtocn.org/rsync:use_rsync)讲述了使用 rsync 客户命令进行同步和备份的内容。rsync 还可以以守护进程（daemon）方式运行，若一台主机以 daemon 模式运行 rsync，一般称其为 rsync 服务器。rsync 的 C/S 方式运行方式概述如下：
- 
rsync 客户端连接远程 rsync 守护进程进行数据同步。
- 
rsync 服务器端要开启 rsyncd 服务，默认监听 873 端口，等待客户端去连接。
- 
rsync 服务器可以 独立运行，也可由 Xinetd 运行。CentOS 默认以 Xinetd 运行。
- 
rsync 服务器启动时会读取配置文件，默认为 /etc/rsyncd.conf，其格式类似于 samba 的主配置文件。
- 
用户验证由服务器负责，用户口令文件在 /etc/rsyncd.conf 中指明。
rsync 命令既是客户端程序，同时也是服务程序。在运行 rsync 服务时使用特殊的命令选项，常用的服务选项有：
|选项|说明|
|----|----|
|–daemon|表示以守护进程方式运行|
|–address=ADDRESS|将此服务绑定到指定的 IP 地址运行|
|–port=PORT|指定特殊的监听端口，默认为 873 端口|
|–bwlimit=KBPS|限制 I/O 带宽（单位为 KBytes/秒）|
|–config=FILE|指定配置文件，默认为 /etc/rsyncd.conf|
|–sockopts=OPTIONS|指定自定义 TCP 选项|
|-4, –ipv4|使用 IPv4|
|-6, –ipv6|使用 IPv6|
上面仅仅列出了 rsync 用于服务的选项，rsync 用于客户的选项请参见上节。
## 配置 rsync 服务
### 配置 rsync 服务器的步骤
- 
首先要选择服务器启动方式
- 
对于负荷较重的 rsync 服务器应该使用独立运行方式
- 
对于负荷较轻的 rsync 服务器可以使用 xinetd 运行方式
- 
创建配置文件 rsyncd.conf
- 
对于非匿名访问的 rsync 服务器还要创建认证口令文件
### 以 xinetd 运行 rsync 服务
CentOS 默认以 xinetd 方式运行 rsync 服务。rsync 的 xinetd 配置文件
在 /etc/xinetd.d/rsync。要配置以 xinetd 运行的 rsync 服务需要执行如下的命令：
# chkconfig rsync on
# service xinetd restart
管理员可以修改 /etc/xinetd.d/rsync 配置文件以适合您的需要。例如，您可以修改配置行
server_args = --daemon
在后面添加 rsync 的服务选项。
### 独立运行 rsync 服务
最简单的独立运行 rsync 服务的方法是执行如下的命令：
/usr/bin/rsync --daemon
您可以将上面的命令写入 /etc/rc.local 文件以便在每次启动服务器时运行 rsync 服务。当然，您也可以写一个脚本在开机时自动启动 rysnc 服务。
### 配置文件 rsyncd.conf
两种 rsync 服务运行方式都需要配置 rsyncd.conf，其格式类似于 samba 的主配置文件。
配置文件 rsyncd.conf 默认在 /etc 目录下。为了将所有与 rsync 服务相关的文件放在单独的目录下，可以执行如下命令：
# mkdir /etc/rsyncd
# touch /etc/rsyncd/rsyncd.conf
# ln -s /etc/rsyncd/rsyncd.conf /etc/rsyncd.conf
配置文件 rsyncd.conf 由全局配置和若干模块配置组成。配置文件的语法为：
- 
模块以 [模块名] 开始
- 
参数配置行的格式是 name = value ，其中 value 可以有两种数据类型：
- 
字符串（可以不用引号定界字符串）
- 
布尔值（1/0 或 yes/no 或 true/false）
- 
以 # 或 ; 开始的行为注释
- 
\ 为续行符
#### 全局参数
在文件中 [module] 之外的所有配置行都是全局参数。当然也可以在全局参数部分定义模块参数，这时该参数的值就是所有模块的默认值。
|参数|说明|默认值|
|----|----|----|
|address|在独立运行时，用于指定的服务器运行的 IP 地址。由 xinetd 运行时将忽略此参数，使用命令行上的 –address 选项替代。|本地所有IP|
|port|指定 rsync 守护进程监听的端口号。 由 xinetd 运行时将忽略此参数，使用命令行上的–port 选项替代。|873|
|motd file|指定一个消息文件，当客户连接服务器时该文件的内容显示给客户。|无|
|pid file|rsync 的守护进程将其 PID 写入指定的文件。|无|
|log file|指定 rsync 守护进程的日志文件，而不将日志发送给 syslog。|无|
|syslog facility|指定 rsync 发送日志消息给 syslog 时的消息级别。|daemon|
|socket options|指定自定义 TCP 选项。|无|
#### 模块参数
模块参数主要用于定义 rsync 服务器哪个目录要被同步。模块声明的格式必须为 [module] 形式，这个名字就是在 rsync 客户端看到的名字，类似于 Samba 服务器提供的共享名。而服务器真正同步的数据是通过path 来指定的。可以根据自己的需要，来指定多个模块，模块中可以定义以下参数：
1、基本模块参数
|参数|说明|默认值|
|----|----|----|
|path|指定当前模块在 rsync 服务器上的同步路径，该参数是必须指定的。|无|
|comment|给模块指定一个描述，该描述连同模块名在客户连接得到模块列表时显示给客户。|无|
2、模块控制参数
|参数|说明|默认值|
|----|----|----|
|use chroot|若为 true，则 rsync 在传输文件之前首先 chroot 到 path 参数所指定的目录下。这样做的原因是实现额外的安全防护，但是缺点是需要 root 权限，并且不能备份指向 path 外部的符号连接所指向的目录文件。|true|
|uid|指定该模块以指定的 UID 传输文件。|nobody|
|gid|指定该模块以指定的 GID 传输文件。|nobody|
|max connections|指定该模块的最大并发连接数量以保护服务器，超过限制的连接请求将被告知随后再试。|0（没有限制）|
|lock file|指定支持 max connections 参数的锁文件。|/var/run/rsyncd.lock|
|list|指定当客户请求列出可以使用的模块列表时，该模块是否应该被列出。如果设置该选项为 false，可以创建隐藏的模块。|true|
|read only|指定是否允许客户上传文件。若为 true 则不允许上传；若为 false 并且服务器目录也具有读写权限则允许上传。|true|
|write only|指定是否允许客户下载文件。若为 true 则不允许下载；若为 false 并且服务器目录也具有读权限则允许下载。|false|
|ignore errors|指定 在 rsync 服务器上运行 delete 操作时是否忽略 I/O 错误。一般来说 rsync 在出现 I/O 错误时将将跳过 –delete 操作，以防止因为暂时的资源不足或其它 I/O 错误导致的严重问题。|true|
|ignore nonreadable|指定 rysnc 服务器完全忽略那些用户没有访问权限的文件。这对于在需要备份的目录中有些不应该被备份者获得的文件时是有意义的。|false|
|timeout|该选项可以覆盖客户指定的 IP 超时时间。从而确保 rsync 服务器不会永远等待一个崩溃的客户端。对于匿名 rsync 服务器来说，理想的数字是 600（单位为秒）。|0 (未限制)|
|dont compress|用来指定那些在传输之前不进行压缩处理的文件。该选项可以定义一些不允许客户对该模块使用的命令选项列表。必须使用选项全名，而不能是简称。当发生拒绝某个选项的情况时，服务器将报告错误信息然后退出。例如，要防止使用压缩，应该是：”dont compress = *”。|*.gz *.tgz *.zip *.z *.rpm *.deb *.iso *.bz2 *.tbz|
3、模块文件筛选参数
|参数|说明|默认值|
|----|----|----|
|exclude|指定多个由空格隔开的多个文件或目录(相对路径)，并将其添加到 exclude 列表中。这等同于在客户端命令中使用 –exclude 来指定模式。|空|
|exclude from|指定一个包含 exclude 规则定义的文件名，服务器从该文件中读取 exclude 列表定义。|空|
|include|指定多个由空格隔开的多个文件或目录(相对路径)，并将其添加到 include 列表中。这等同于在客户端命令中使用 –include 来指定模式 。|空|
|include from|指定一个包含 include 规则定义的文件名，服务器从该文件中读取 include 列表定义。|空|
- 
一个模块只能指定一个 exclude 参数、一个 include 参数。
- 
结合 include 和 exclude 可以定义复杂的 exclude/include 规则 。
- 
这几个参数分别与相应的 rsync 客户命令选项等价，唯一不同的是它们作用在服务器端。
- 
关于如何书写规则文件的内容请参见[上节](http://www.howtocn.org/rsync:use_rsync)。
4、模块用户认证参数
|参数|说明|默认值|
|----|----|----|
|auth users|指定由空格或逗号分隔的用户名列表，只有这些用户才允许连接该模块。这里的用户和系统用户没有任何关系。用户名和口令以明文方式存放在 secrets file 参数指定的文件中。|(匿名方式)|
|secrets file|指定一个 rsync 认证口令文件。只有在 auth users 被定义时，该文件才起作用。|空|
|strict modes|指定是否监测口令文件的权限。若为 true 则口令文件只能被 rsync 服务器运行身份的用户访问，其他任何用户不可以访问该文件。|true|
- 
rsync 认证口令文件的权限一定是 600，否则客户端将不能连接服务器。
- 
rsync 认证口令文件中每一行指定一个 用户名:口令 对，格式为：
# 以 "#" 开始的行为注释行
username:passwd
# 一般来说口令最好不要超过8个字符。
若您只配置匿名访问的 rsync 服务器，则无需设置上述参数。
5、模块访问控制参数
|参数|说明|默认值|
|----|----|----|
|hosts allow|用一个主机列表指定哪些主机客户允许连接该模块。不匹配主机列表的主机将被拒绝。|*|
|hosts deny|用一个主机列表指定哪些主机客户不允许连接该模块。|空|
客户主机列表定义可以是以下形式：
- 
单个IP地址。例如：192.168.0.1
- 
整个网段。例如：192.168.0.0/24，192.168.0.0/255.255.255.0
- 
可解析的单个主机名。例如：centos，centos.smartraining.cn
- 
域内的所有主机。例如：*.smartraining.cn
- 
“*”则表示所有。
- 
多个列表项要用空格间隔。
6、模块日志参数
|参数|说明|默认值|
|----|----|----|
|transfer logging|使 rsync 服务器将传输操作记录到传输日志文件。|false|
|log format|指定传输日志文件的字段。|”%o %h [%a] %m (%u) %f %l”|
* 设置了”log file”参数时，在日志每行的开始会添加”%t [%p]“。
- 
可以使用的日志格式定义符如下所示：
- 
%a - 远程IP地址
- 
%h - 远程主机名
- 
%l - 文件长度字符数
- 
%p - 该次 rsync 会话的 PID
- 
%o - 操作类型：”send” 或 “recv”
- 
%f - 文件名
- 
%P - 模块路径
- 
%m - 模块名
- 
%t - 当前时间
- 
%u - 认证的用户名（匿名时是 null）
- 
%b - 实际传输的字节数
- 
%c - 当发送文件时，记录该文件的校验码
## rsync 服务器与备份
### 何时使用 rsync 服务器
- 
若用户在远程主机上有登录帐号，通常可以使用 ssh 方式运行 rsync 而不必配置 rsync 服务。
- 
由于 rsync 服务使用明文口令，所以在不可信任的网络中应该尽量使用 ssh 方式运行 rsync。
- 
在可信任的网络中可以配置使用 rsync 服务器，当然也可以使用 ssh 方式运行 rsync。
- 
最需要架设 rsync 服务器的理由恐怕就是匿名 rsync 服务器了，它可以为客户提供匿名访问的同步资源，
例如：允许用户同步 FTP 软件资源、Linux 发行版本的软件仓库等。
### 部署 rsync 服务器的两种方法
为了备份数据，如何在网络中部署 rsync 服务器呢？通常有两种方法：
- 
在生产服务器上同时运行 rsync 服务
- 
rsync 服务以只读方式提供要备份的数据，从而避免破坏生产服务器上的数据
- 
根据需要，可以配置一个或多个（为了避免风险）主机作为备份主机
- 
在每个备份主机上以”拉”的方式从生产服务器将数据同步到备份主机
- 
在备份服务器上运行 rsync 服务
- 
备份服务器实际上是个数据仓库，他集中收集了网络中所有要备份的主机的数据
- 
备份服务器上运行的 rsync 服务以读写方式提供备份空间
- 
根据需要，可以配置一个或多个（为了避免风险）备份服务器
- 
在每台要备份的主机（包括生产服务器）上以”推”的方式将备份数据写入备份服务器
上述关于备份的思想方法同样也适用于以 ssh 方式运行 rsync 的情况。当然这时无需架设 rsync 服务，所需考虑的问题仅仅是应该以”推”方式运行还是以”拉”方式运行。
## rsync 服务器应用
下面给出几个使用 rsync 服务器的示例。
### 在生产服务器上同时运行 rsync 服务
假设网路中有如下3台计算机
- 
生产服务器 - pandr （192.168.0.220）
- 
备份主机A - backupa （192.168.0.221）
- 
备份主机B - backupb （192.168.0.222）
### 在 pandr 上配置 rsync 服务
1、编辑配置文件
# vi /etc/rsyncd/rsyncd.confuid = nobody
gid = nobody
syslog facility = local3
use chroot = yes
read only = yes
max connections = 4
timeout = 300
motd file = /etc/rsyncd/rsyncd.motd
pid file = /var/run/rsyncd.pid
lock file = /var/run/rsync.lock
hosts allow=192.168.0.221 192.168.0.222
hosts deny=*
secrets file = /etc/rsyncd/rsyncd.secrets
auth users = bua,bub
list=yes
[home]
	uid = root
	gid = root
	path = /home
	comment = product server home
	exclude = www/ samba/ ftp/
[www]
	path = /home/www
	comment = product server www
	exclude = logs/
下面考查一些权限问题：
# tree /home -p
/home
|-- [drwx------] crq
|-- [drwxr-xr-x] ftp
|-- [drwx------] osmond
|-- [drwxr-xr-x] samba
`-- [drwxr-xr-x] www
	|-- [drwxr-xr-x] cgi-bin
|-- [drwxr-xr-x] fcgi-bin
|-- [drwxr-xr-x] htdocs
`-- [drwxr-xr-x] logs
从上面的目录权限可知，每个用户的自家目录对组和其他用户没有任何权限。当 uid 和 gid 设置为 nobody时，rsync客户端由于无权进入用户的自家目录，所以不能同步各个自家目录下的内容。为此，[home] 部分将 uid 和 gid 设置为 root。
rsyncd 默认将日志写入 /var/log/messages 文件，上面的 “syslog facility = local3” 配置将使用LOCAL3 日志设备（facility）。 为此需要在 /etc/syslog.conf 文件中添加如下行：
local3.info /var/log/rsync.log
然后使用如下命令重新启动 syslog
# service syslog restart
2、编辑 rsync 服务的口令文件
# touch /etc/rsyncd/rsyncd.secrets
# chmod 600 /etc/rsyncd/rsyncd.secrets
# vi /etc/rsyncd/rsyncd.secretsbua:backupa-s-password
bub:backupb-s-password
不要在 /etc/rsyncd/rsyncd.secrets 中使用与同名系统用户帐号相同的口令。
3、以 xinetd 运行 rsync 服务
# chkconfig rsync on
# service xinetd restart
4、配置防火墙
使用 iptables 配置允许 rsync 服务端口（默认为 873）通过，同时限制 rsync 客户端的连接。
例如：
# iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 873 -j ACCEPT# iptables -A INPUT -p tcp -s ! 192.168.0.221 --dport 873 -j DROP
可以使用如下命令查看添加的防火墙规则：
# iptables -L
### 在备份主机上从服务器同步
下面配置 rsync 服务器的客户端。下面以 backupa（192.168.0.221）为例进行说明。
首先可以使用如下的命令查看服务器上提供的同步资源：
rsync --list-only bua@192.168.0.220::
rsync --list-only rsync://bua@192.168.0.220/home
然后就可以在客户端配置同步了，配置方法相当灵活，您可以根据自己需要选择不同的配置方法。
1）简单同步（不保存历史归档）
rsync -avzP --delete bua@192.168.0.220::home /backups/192.168.0.220/home
rsync -avzP --delete rsync://bua@192.168.0.220/www /backups/192.168.0.220/www
2）完全备份（保存历史归档）
rsync -avzP --delete bua@192.168.0.220::home /backups/192.168.0.220/$(date +'%y-%m-%d')/home
rsync -avzP --delete rsync://bua@192.168.0.220/www /backups/192.168.0.220/$(date +'%y-%m-%d')/www
上面的方法虽然简单，但是效率不高。因为每次都将服务器提供的资源同步到不同的目录（因为时间在变），所以这只相当于使用 scp 命令进行远程拷贝。
上面的命令没有使用 rsync 的优势功能（即只传输有变化的部分），为此做如下改进：
首先在备份主机上部署如下的目录：
/backups/192.168.0.220/current # 存放同步的目录
/backups/192.168.0.220/archive # 存放归档的目录
接着使用如下命令进行同步：
rsync -avzP --delete bua@192.168.0.220::home /backups/192.168.0.220/current/home
rsync -avzP --delete rsync://bua@192.168.0.220/www /backups/192.168.0.220/current/www
同步之后在备份主机上使用如下命令保存归档：
tar -cjf /backups/192.168.0.220/archive/home-$(date +'%y-%m-%d').tbz \
-C /backups/192.168.0.220/current/home .
tar -cjf /backups/192.168.0.220/archive/www-$(date +'%y-%m-%d').tbz \
-C /backups/192.168.0.220/current/www .
这种改进方法具有如下优点：
- 
每次同步都基于 current 目录，所以只会传输变化的部分，减少了网络流量
- 
使用压缩的归档，减少了备份主机的磁盘占用
- 
由于是在备份主机上执行归档压缩，减少生产服务器运行归档压缩的CPU时间
因此，若需要进行远程的包含归档文件的完全备份，上述方法优于在生产服务器执行 tar 命令然后再上传到备份主机的方法。
若您要在 cron 中执行 rsync，需要在 rsync 命令中添加类似如下的指定口令文件的选项，以避免在终端上输入口令：
--password-file=/root/rsync.password
然后在备份主机上生成口令文件：
echo "backupa-s-password"> /root/rsync.password
chmod 600 /root/rsync.password
3）增量备份（保存历史归档）
您可以参考上一节的内容编写脚本实现普通型增量备份或快照型增量备份。
为了提供安全性，应该配置一个以上的备份主机。配置方法有两种：
- 
每台备份主机都从生产服务器同步数据
- 
为了降低生产服务器负载，一台备份主机也可以从另一台备份主机同步数据，实现二级备份架构
### 在备份服务器上运行 rsync 服务
假设网路中有如下4台计算机
- 
备份服务器A - backupa （192.168.0.221）
- 
备份服务器B - backupb （192.168.0.222）
- 
主机A - hosta （192.168.0.111）
- 
主机B - hostb （192.168.0.112）
### 在备份服务器上配置 rsync 服务
只需在一台备份服务器上配置 rsync 服务。例如，在备份服务器A上配置 rsync 服务，然后在备份服务器B上使用 rsync 客户命令同步备份服务器A即可。
下面在 backupa （192.168.0.221）上配置备份服务器。
1、编辑配置文件
# vi /etc/rsyncd/rsyncd.confuid = root
gid = root
syslog facility = local3
use chroot = yes
read only = no
max connections = 20
timeout = 300
motd file = /etc/rsyncd/rsyncd.motd
pid file = /var/run/rsyncd.pid
lock file = /var/run/rsync.lock
hosts allow=192.168.0.0/24
hosts deny=*
secrets file = /etc/rsyncd/rsyncd.secrets
[hosta-home]
	path = /backups/hosta/home
	comment = hosta home
	list=yes
	auth users = hosta
[hosta-www]
	path = /backups/hosta/www
	comment = hosta www
	list=yes
	auth users = hosta
[hostb-home]
	path = /backups/hostb/home
	comment = hostb home
	list=yes
	auth users = hostb
[hostb-www]
	path = /backups/hostb/www
	comment = hostb www
	list=yes
	auth users = hostb
2、编辑 rsync 服务的口令文件
# touch /etc/rsyncd/rsyncd.secrets
# chmod 600 /etc/rsyncd/rsyncd.secrets
# vi /etc/rsyncd/rsyncd.secretshosta:hosta-s-password
hostb:hostb-s-password
3、创建系统帐号
# useradd hosta
# useradd hostb
# 若不允许系统登录，则无需设置用户口令
4、独立运行 rsync 服务
# chkconfig rsync off
# service xinetd restart
# echo "/usr/bin/rsync --daemon">>/etc/rc.local
# /usr/bin/rsync --daemon
5、配置防火墙使用 iptables 配置允许 rsync 服务端口（默认为 873）通过，同时限制 rsync 客户端的连接。
例如：
# iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 873 -j ACCEPT# iptables -A INPUT -p tcp -s ! 192.168.0.0/24 --dport 873 -j DROP
可以使用如下命令查看添加的防火墙规则：
# iptables -L
### 在主机上向服务器同步
下面配置 rsync 服务器的客户端。每个 rsync 客户端的配置方法都是类似的。下面以 hosta（192.168.0.111）为例进行说明。
首先可以使用如下的命令查看服务器上提供的同步资源：
rsync --list-only hosta@192.168.0.221::
rsync --list-only rsync://hosta@192.168.0.221/hosta-home
然后就可以在客户端以推方式同步到服务器了。
rsync -avzP --delete --exclude "lost+found/" /home/ hosta@192.168.0.221::hosta-home
rsync -avzP --delete /www/ rsync://hosta@192.168.0.221/hosta-www
若您要在 cron 中执行 rsync，需要在 rsync 命令中添加类似如下的指定口令文件的选项，以避免在终端上输入口令：
--password-file=/root/rsync.password
然后在备份主机上生成口令文件：
echo "hosta-s-password"> /root/rsync.password
chmod 600 /root/rsync.password
### 配置匿名 rsync 服务
最简单的 rsync 服务器配置就是匿名服务器。下面说明配置步骤。
1、编辑配置文件
# vi /etc/rsyncd/rsyncd.confuid = nobody
gid = nobody
syslog facility = local3
use chroot = yes
read only = yes
timeout = 600
motd file = /etc/rsyncd/rsyncd.motd
pid file = /var/run/rsyncd.pid
lock file = /var/run/rsync.lock
list=yes
# 设置匿名访问的 Centos 仓库的同步资源
[centos]
	path = /var/www/mirror/centos
	comment = Centos Repository
	max connections = 30
# 设置匿名访问的 ubuntu 仓库的同步资源
[ubuntu]
	path = /var/www/mirror/ubuntu
	comment = Ubuntu Repository
	max connections = 30
# 设置匿名访问的 Ubuntu CN 仓库的同步资源
[ubuntu-cn]
	path = /var/www/mirror/ubuntu-cn
	comment = Ubuntu CN Repository
	max connections = 30
# 设置匿名访问的 FTP 服务器的同步资源
[ftp]
	path = /var/ftp/pub
	comment = Anonymous FTP server
	max connections = 10
# 也可以同时设置其他非匿名访问的同步资源
[tmp]
	path = /tmp
	comment = Temporary Directory
	read only = no
	hosts allow=192.168.0.0/24 127.0.0.0/8 *.sinosmond.com
	hosts deny=*
	secrets file = /etc/rsyncd/rsyncd.secrets
	auth users = user1,user2
	max connections = 5
2、编辑 rsync 服务的口令文件
# touch /etc/rsyncd/rsyncd.secrets
# chmod 600 /etc/rsyncd/rsyncd.secrets
# vi /etc/rsyncd/rsyncd.secretsuser1:user1-s-password
user2:user2-s-password
不要在 /etc/rsyncd/rsyncd.secrets 中使用与同名系统用户帐号相同的口令。
3、独立运行 rsync 服务
# chkconfig rsync off
# service xinetd restart
# echo "/usr/bin/rsync --daemon">>/etc/rc.local
# /usr/bin/rsync --daemon
4、配置防火墙
使用 iptables 配置允许 rsync 服务端口（默认为 873）通过，同时限制 rsync 客户端的连接。
例如：
# iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 873 -j ACCEPT
可以使用如下命令查看添加的防火墙规则：
# iptables -L
有关如何使用 rsync 客户命令从匿名服务器同步数据的内容请参考[上一节](http://www.howtocn.org/rsync:use_rsync)。
## 参考
- 
[http://sunsite.dk/info/guides/rsync/rsync-mirroring.html](http://sunsite.dk/info/guides/rsync/rsync-mirroring.html)
- 
[http://everythinglinux.org/rsync/](http://everythinglinux.org/rsync/)
原文地址：[http://www.howtocn.org/rsync:use_rsync_server](http://www.howtocn.org/rsync:use_rsync_server)
