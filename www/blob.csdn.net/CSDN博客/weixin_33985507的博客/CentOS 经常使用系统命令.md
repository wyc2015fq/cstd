# CentOS 经常使用系统命令 - weixin_33985507的博客 - CSDN博客
2017年08月19日 19:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8

# uname -a 
# 查看内核/操作系统/CPU信息
# head -n 1 /etc/issue   # 查看操作系统版本号
# cat /proc/cpuinfo      # 查看CPU信息
# hostname               # 查看计算机名
# lspci -tv              # 列出全部PCI设备
# lsusb -tv              # 列出全部USB设备
# lsmod                  # 列出载入的内核模块
# env                    # 查看环境变量
资源
# free -m                # 查看内存使用量和交换区使用量
# df -h                  # 查看各分区使用情况
# du -sh <文件夹名>        # 查看指定文件夹的大小
# grep MemTotal /proc/meminfo   # 查看内存总量
# grep MemFree /proc/meminfo    # 查看空暇内存量
# uptime                 # 查看系统执行时间、用户数、负载
# cat /proc/loadavg      # 查看系统负载
磁盘和分区
# mount | column -t      # 查看挂接的分区状态
# fdisk -l               # 查看全部分区
# swapon -s              # 查看全部交换分区
# hdparm -i /dev/hda     # 查看磁盘參数(仅适用于IDE设备)
# dmesg | grep IDE       # 查看启动时IDE设备检測状况
网络
# ifconfig               # 查看全部网络接口的属性
# iptables -L            # 查看防火墙设置
# route -n               # 查看路由表
# netstat -lntp          # 查看全部监听端口
# netstat -antp          # 查看全部已经建立的连接
# netstat -s             # 查看网络统计信息
进程
# ps -ef                 # 查看全部进程
# top                    # 实时显示进程状态
用户
# w                      # 查看活动用户
# id <username>            # 查看指定用户信息
# last                   # 查看用户登录日志
# cut -d: -f1 /etc/passwd   # 查看系统全部用户
# cut -d: -f1 /etc/group    # 查看系统全部组
# crontab -l             # 查看当前用户的计划任务
服务
# chkconfig --list       # 列出全部系统服务
# chkconfig --list | grep on    # 列出全部启动的系统服务
程序
# rpm -qa                # 查看全部安装的软件包
改动hostname，需改动两处：
1）  vim  /etc/sysconfig/network 改动 HOSTNAME=your_hostname
2）vim /etc/hosts  改动 12.160.134.168  your_hostname
**Python 安装**
1） 下载 [Python](http://www.python.org/downloads/)， 直接下载 [Python-2.7.6.tar.xz](http://www.python.org/ftp/python/2.7.6/Python-2.7.6.tar.xz)
2） 安装.tar.xz 解压工具： yum -y install xz
3） 解压.tar.xz文件： unxz Python-2.7.6.tar.xz     和   tar xvf Python-2.7.6.tar
4） cd Python-2.7.6
5） ./configure--prefix=/usr/local--enable-unicode=ucs4--enable-shared LDFLAGS="-Wl,-rpath /usr/local/lib"
make&&makealtinstall
**pip 安装**
1） wget https://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py
2） python2.7ez_setup.py
3） easy_install-2.7pip
4） pip2.7install[packagename]
  pip2.7install--upgrade[packagename]
  pip2.7uninstall[packagename]
