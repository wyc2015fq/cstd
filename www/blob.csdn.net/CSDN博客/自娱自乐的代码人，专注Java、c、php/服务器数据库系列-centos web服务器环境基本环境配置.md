# 服务器数据库系列 - centos web服务器环境基本环境配置 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月15日 11:13:27[initphp](https://me.csdn.net/initphp)阅读数：900







- 安装web服务器所需要的包
`yum -y install gcc gcc-c++ gcc-g77 autoconf automake zlib* fiex* libxml* ncurses-devel libmcrypt* libtool-ltdl-devel* libevent glib2 glib2-devel bzip2 bzip2-devel ncurses ncurses-devel curl curl-devel  lib zlib-devel make libevent libevent-devel pcre pcre-devel`- 注释掉系统中不需要用到的用户组和用户

```
cp /etc/passwd /etc/passwdbak #修改之前先备份
vi /etc/passwd #编辑用户，在前面加上#注释掉此行
#adm:x:3:4:adm:/var/adm:/sbin/nologin
#lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
#sync:x:5:0:sync:/sbin:/bin/sync
#shutdown:x:6:0:shutdown:/sbin:/sbin/shutdown
#halt:x:7:0:halt:/sbin:/sbin/halt
#uucp:x:10:14:uucp:/var/spool/uucp:/sbin/nologin
#operator:x:11:0:operator:/root:/sbin/nologin
#games:x:12:100:games:/usr/games:/sbin/nologin
#gopher:x:13:30:gopher:/var/gopher:/sbin/nologin
#ftp:x:14:50:FTP User:/var/ftp:/sbin/nologin #注释掉ftp匿名账号
cp /etc/group /etc/groupbak #修改之前先备份
vi /etc/group #编辑用户组，在前面加上#注释掉此行
#adm:x:4:root,adm,daemon
#lp:x:7:daemon,lp
#uucp:x:14:uucp
#games:x:20:
#dip:x:40:
```
- 禁止非root用户执行init*.d/下的命令

```
chmod -R 700 /etc/rc.d/init.d/*
chmod -R 777 /etc/rc.d/init.d/* #恢复默认设置
```
- 防止非授权用户获得权限

```
chattr +i /etc/passwd
chattr +i /etc/shadow
chattr +i /etc/group
chattr +i /etc/gshadow
chattr +i /etc/services #给系统服务端口列表文件加锁,防止未经许可的删除或添加服务
lsattr /etc/passwd /etc/shadow /etc/group /etc/gshadow /etc/services #显示文件的属性
```

```
#注意：执行以上权限修改之后，就无法添加删除用户了。
#如果再要添加删除用户，需要先取消上面的设置，等用户添加删除完成之后，再执行上面的操作
chattr -i /etc/passwd #取消权限锁定设置
chattr -i /etc/shadow
chattr -i /etc/group
chattr -i /etc/gshadow
chattr -i /etc/services #取消系统服务端口列表文件加锁
```
- 限制不同文件权限

```
chmod 700 /usr/bin #恢复 chmod 555 /usr/bin
chmod 700 /bin/ping #恢复 chmod 4755 /bin/ping
chmod 700 /usr/bin/vim #恢复 chmod 755 /usr/bin/vim
chmod 700 /bin/netstat #恢复 chmod 755 /bin/netstat
chmod 700 /usr/bin/tail #恢复 chmod 755 /usr/bin/tail
chmod 700 /usr/bin/less #恢复 chmod 755 /usr/bin/less
chmod 700 /usr/bin/head #恢复 chmod 755 /usr/bin/head
chmod 700 /bin/cat #恢复 chmod 755 /bin/cat
chmod 700 /bin/uname #恢复 chmod 755 /bin/uname
chmod 500 /bin/ps #恢复 chmod 755 /bin/ps
```
- 禁止使用Ctrl+Alt+Delete 重启

```
cp /etc/inittab /etc/inittabbak
vi /etc/inittab #注释掉下面这一行
#ca::ctrlaltdel:/sbin/shutdown -t3 -r now
```
- yum更新不升级内核包

由于系统与硬件的兼容性问题，有可能升级内核后导致服务器不能正常启动，这是非常可怕的，没有特别的需要，建议不要随意升级内核。

cp /etc/yum.conf /etc/yum.confbak

1、修改yum的配置文件 vi /etc/yum.conf 在[main]的最后添加 exclude=kernel*

2、直接在yum的命令后面加上如下的参数：

yum --exclude=kernel* update

查看系统版本 cat /etc/issue

查看内核版本 uname -a
- 关闭yum自动更新

chkconfig --list yum-updatesd #显示当前系统状态

yum-updatesd 0:关闭 1:关闭 2:启用 3:启用 4:启用 5:启用 6:关闭

service yum-updatesd stop #关闭 开启参数为start

停止 yum-updatesd： [确定]

service yum-updatesd status #查看是否关闭

yum-updatesd 已停

chkconfig --level 35 yum-updatesd off #禁止开启启动（系统模式为3、5）

chkconfig yum-updatesd off #禁止开启启动（所有启动模式全部禁止）

chkconfig --list yum-updatesd #显示当前系统状态

yum-updatesd 0:关闭 1:关闭 2:启用 3:关闭 4:启用 5:关闭 6:关闭
- 关闭虚拟控制台

我们知道从控制台切换到 X 窗口，一般采用 Alt-F7 ，为什么呢？因为系统默认定义了 6 个虚拟控制台，

所以 X 就成了第7个。实际上，很多人一般不会需要这么多虚拟控制台的，修改/etc/inittab ，注释掉那些你不需要的。

cp /etc/inittab /etc/inittabbak

vi /etc/inittab

# Run gettys in standard runlevels

1:2345:respawn:/sbin/mingetty tty1

#2:2345:respawn:/sbin/mingetty tty2

#3:2345:respawn:/sbin/mingetty tty3

#4:2345:respawn:/sbin/mingetty tty4

#5:2345:respawn:/sbin/mingetty tty5

#6:2345:respawn:/sbin/mingetty tty6
- 修改history命令记录

```
cp /etc/profile /etc/profilebak
vi /etc/profile
找到 HISTSIZE=1000 改为 HISTSIZE=50
```
- 优化服务器内核

cp /etc/sysctl.conf /etc/sysctl.confbak

vi /etc/sysctl.conf #在文件末尾添加以下内容

net.ipv4.tcp_max_syn_backlog = 65536

net.core.netdev_max_backlog = 32768

net.core.somaxconn = 32768

net.core.wmem_default = 8388608

net.core.rmem_default = 8388608

net.core.rmem_max = 16777216

net.core.wmem_max = 16777216

net.ipv4.tcp_timestamps = 0

net.ipv4.tcp_synack_retries = 2

net.ipv4.tcp_syn_retries = 2

net.ipv4.tcp_tw_recycle = 1

#net.ipv4.tcp_tw_len = 1

net.ipv4.tcp_tw_reuse = 1

net.ipv4.tcp_mem = 94500000 915000000 927000000

net.ipv4.tcp_max_orphans = 3276800

#net.ipv4.tcp_fin_timeout = 30

#net.ipv4.tcp_keepalive_time = 120

net.ipv4.ip_local_port_range = 10024 65535 #（表示用于向外连接的端口范围。缺省情况下很小：32768到61000 注意：这里不要将最低值设的太低，否则可能会占用掉正常的端口！ ）

/sbin/sysctl -p #使配置立即生效
- 系统优化

cp /etc/profile /etc/profilebak2

vi /etc/profile #在文件末尾添加以下内容

ulimit -c unlimited

ulimit -s unlimited

ulimit -SHn 65535

source /etc/profile #使配置立即生效

ulimit -a #显示当前的各种用户进程限制
- 时间配置

```
yum install –y ntp  
安装NTP官方的时间同步程序 (NTP:中国国家授时中心)  
/usr/sbin/ntpdate -s pool.ntp.org  
以NTP官方服务器为准调整本地时间  
 crontab -e  
编辑计划任务列表  
0 3 * * * /usr/sbin/ntpdate -s pool.ntp.org  
编辑文件到此状态，表示每天凌晨3点自动同步时间  
# /sbin/service crond reload  
 重载计划任务配置
```
- 不允许外部ping

```
cp /etc/rc.d/rc.local /etc/rc.d/rc.localbak
vi /etc/rc.d/rc.local #在文件末尾增加下面这一行
echo 1 > /proc/sys/net/ipv4/icmp_echo_ignore_all
```







