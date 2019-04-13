
# CentOS 常用系统命令 - 阳光岛主 - CSDN博客

2014年04月27日 22:31:53[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：10638个人分类：[Linux/Shell																](https://blog.csdn.net/sunboy_2050/article/category/655460)



\# uname -a
\#查看内核/操作系统/CPU信息
\# head -n 1 /etc/issue   \#查看操作系统版本
\# cat /proc/cpuinfo      \#查看CPU信息
\# hostname               \#查看计算机名
\# lspci -tv              \#列出所有PCI设备
\# lsusb -tv              \#列出所有USB设备
\# lsmod                  \#列出加载的内核模块
\# env                    \#查看环境变量
资源
\# free -m                \#查看内存使用量和交换区使用量
\# df -h                  \#查看各分区使用情况
\# du -sh <目录名>        \#查看指定目录的大小
\# grep MemTotal /proc/meminfo   \#查看内存总量
\# grep MemFree /proc/meminfo    \#查看空闲内存量
\# uptime                 \#查看系统运行时间、用户数、负载
\# cat /proc/loadavg      \#查看系统负载
磁盘和分区
\# mount | column -t      \#查看挂接的分区状态
\# fdisk -l               \#查看所有分区
\# swapon -s              \#查看所有交换分区
\# hdparm -i /dev/hda     \#查看磁盘参数(仅适用于IDE设备)
\# dmesg | grep IDE       \#查看启动时IDE设备检测状况
网络
\# ifconfig               \#查看所有网络接口的属性
\# iptables -L            \#查看防火墙设置
\# route -n               \#查看路由表
\# netstat -lntp          \#查看所有监听端口
\# netstat -antp          \#查看所有已经建立的连接
\# netstat -s             \#查看网络统计信息
进程
\# ps -ef                 \#查看所有进程
\# top                    \#实时显示进程状态
用户
\# w                      \#查看活动用户
\# id <用户名>            \#查看指定用户信息
\# last                   \#查看用户登录日志
\# cut -d: -f1 /etc/passwd   \#查看系统所有用户
\# cut -d: -f1 /etc/group    \#查看系统所有组
\# crontab -l             \#查看当前用户的计划任务
服务
\# chkconfig --list       \#列出所有系统服务
\# chkconfig --list | grep on    \#列出所有启动的系统服务
程序
\# rpm -qa                \#查看所有安装的软件包

修改hostname，需修改两处：
1）  vim/etc/sysconfig/network 修改 HOSTNAME=your_hostname
2）vim /etc/hosts  修改 12.160.134.168your_hostname

**Python 安装**
1） 下载[Python](http://www.python.org/downloads/)， 直接下载[Python-2.7.6.tar.xz](http://www.python.org/ftp/python/2.7.6/Python-2.7.6.tar.xz)
2） 安装.tar.xz 解压工具：yum -y install xz
3） 解压.tar.xz文件： unxz Python-2.7.6.tar.xz     和   tar xvf Python-2.7.6.tar
4） cd Python-2.7.6
5）./configure--prefix=/usr/local--enable-unicode=ucs4--enable-sharedLDFLAGS="-Wl,-rpath /usr/local/lib"
make&&makealtinstall

**pip 安装**
1）wgethttps://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py
2）python2.7ez_setup.py
3）easy_install-2.7pip
4）pip2.7install[packagename]
pip2.7install--upgrade[packagename]
pip2.7uninstall[packagename]


