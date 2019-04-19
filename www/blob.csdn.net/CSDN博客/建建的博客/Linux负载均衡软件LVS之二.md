# Linux负载均衡软件LVS之二 - 建建的博客 - CSDN博客
2017年03月20日 15:24:55[纪建](https://me.csdn.net/u013898698)阅读数：111
一、  安装LVS软件
1．安装前准备工作
操作系统：统一采用Centos4.4版本。
地址规划，如表1所示：
表1
![](http://img1.51cto.com/attachment/201104/160534999.png)
更详细的信息如图2所示：
![](http://img1.51cto.com/attachment/201104/160011983.png)
图2  LVS DR模式安装部署结构图
 图2中的VIP指的是虚拟IP地址，还可以叫做LVS集群的服务IP，在DR、TUN模式中，数据包是直接返回给用户的，所以，在Director Server上以及集群的每个节点上都需要设置这个地址。此IP在Real Server上一般绑定在回环地址上，例如lo:0，同样，在Director Server上，虚拟IP绑定在真实的网络接口设备上，例如eth0:0。
 各个Real Server可以是在同一个网段内，也可以是相互独立的网段，还可以是分布在internet上的多个服务器。
2．安装操作系统
Centos4.4版本的Linux，内核默认支持LVS功能，为了方便编译安装IPVS管理软件，在安装操作系统时，建议选择如下这些安装包：
 桌面环境：xwindows system、GNOME desktop environment。
 开发工具：development tools、x software development、gnome software、development、kde software development。
系统安装完毕，可以通过如下命令检查kernel是否已经支持LVS的ipvs模块：
[root@localhost ~]#modprobe -l |grep ipvs
/lib/modules/2.6.9-42.ELsmp/kernel/net/ipv4/ipvs/ip_vs_rr.ko
/lib/modules/2.6.9-42.ELsmp/kernel/net/ipv4/ipvs/ip_vs_sh.ko
如果有类似上面的输出，表明系统内核已经默认支持了IPVS模块。接着就可以安装IPVS管理软件了。
3．在Director Serve上安装IPVS管理软件
IPVS提供的软件包有源码方式的也有rpm方式的，这里介绍下源码方式安装IPVS,首先从[http://www.linuxvirtualserver.org/software/ipvs.html](http://www.linuxvirtualserver.org/software/ipvs.html)下载对应版本的ipvs源码，由于我们这里采用的操作系统为Centos4.4版本，因此，下载对应的ipvsadm-1.24版本，接着进行安装：
[root@localhost ~]#tar zxvf ipvsadm-1.24.tar.gz
[root@localhost ~]#cd ipvsadm-1.24
[root@localhost ~]#make
[root@localhost ~]#make install
注意：在make时可能会出现错误编译信息，这是由于编译程序找不到对应内核的原因，按照如下操作就可以正常编译：
[root@localhost ~]#ln -s /usr/src/kernels/2.6.9-42.EL-i686/  /usr/src/linux
也可以下载rpm安装包，通过rpm方式进行安装：
[root@localhost ~]#rpm –ivh ipvsadm-1.24-6.1.i386.rpm
然后执行：
[root@localhost ~]# ipvsadm --help 
如果看到帮助提示，表明IPVS已经成功安装。
4．ipvsadm的用法
（1）Ipvsadm常用的语法和格式如下：
ipvsadm -A|E -t|u|f virutal-service-address:port [-s scheduler] [-p [timeout]] [-M netmask]
ipvsadm -D -t|u|f virtual-service-address
ipvsadm -C
ipvsadm -R
ipvsadm -S [-n]
ipvsadm -a|e -t|u|f virtual-service-address:port -r real-server-address:port
[-g|i|m] [-w weight]
ipvsadm -d -t|u|f virtual-service-address -r real-server-address
ipvsadm -L|l [options]
ipvsadm -Z [-t|u|f virtual-service-address]
ipvsadm --set tcp tcpfin udp
ipvsadm –h
其中：
 virtual-service-address:是指虚拟服务器的IP地址，本文是192.168.60.200
 real-service-address:是指Real Server的IP地址，本文是192.168.60.132/144
 scheduler：指定调度算法
ipvsadm命令选项详细含义如表2所示：
表2
命令选项
含义
-A (--add-service) 在内核的虚拟服务器列表中添加一条新的虚拟IP记录。也就是增加一台新的虚拟服务器。虚拟IP也就是虚拟服务器的IP地址。
-E (--edit-service) 编辑内核虚拟服务器列表中的一条虚拟服务器记录
-D (--delete-service) 删除内核虚拟服务器列表中的一条虚拟服务器记录
-C (--clear) 清除内核虚拟服务器列表中的所有记录
-R (--restore) 恢复虚拟服务器规则
-S (--save) 保存虚拟服务器规则，输出为-R 选项可读的格式
-a (--add-server) 在内核虚拟服务器列表的一条记录里添加一条新的Real Server记录。也就是在一个虚拟服务器中增加一台新的Real Server
-e (--edit-server) 编辑一条虚拟服务器记录中的某条Real Server记录
-d (--delete-server) 删除一条虚拟服务器记录中的某条Real Server记录
-L|-l –list 显示内核中虚拟服务器列表
-Z (--zero) 虚拟服务器列表计数器清零（清空当前的连接数量等）
--set tcp tcpfin udp 设置连接超时值
-t 说明虚拟服务器提供的是tcp服务，此选项后面跟如下格式：
[virtual-service-address:port] or [real-server-ip:port]
-u 说明虚拟服务器提供的是udp服务，此选项后面跟如下格式：
[virtual-service-address:port] or [real-server-ip:port]
-f  fwmark 说明是经过iptables标记过的服务类型
-s   此选项后面跟LVS使用的调度算法
有这样几个选项： rr|wrr|lc|wlc|lblc|lblcr|dh|sh
默认的调度算法是： wlc
-p  [timeout] 在某个Real Server上持续的服务时间。也就是说来自同一个用户的多次请求，将被同一个Real Server处理。此参数一般用于有动态请求的操作中，timeout 的默认值为300 秒。例如：-p 600，表示持续服务时间为600秒。
-r 指定Real Server的IP地址，此选项后面跟如下格式：
 [real-server-ip:port]
-g (--gatewaying) 指定LVS 的工作模式为直接路由模式（此模式是LVS 默认工作模式）
-i (-ipip) 指定LVS 的工作模式为隧道模式
-m (--masquerading) 指定LVS 的工作模式为NAT模式
-w (--weight) weight 指定Real Server的权值
-c (--connection) 显示LVS目前的连接信息 如：ipvsadm -L -c
-L --timeout 显示“tcp tcpfin udp”的timeout值，如：ipvsadm -L --timeout
-L --daemon 显示同步守护进程状态，例如：ipvsadm -L –daemon
-L  --stats 显示统计信息，例如：ipvsadm -L –stats
-L  --rate 显示速率信息，例如：ipvsadm -L  --rate
-L  --sort 对虚拟服务器和真实服务器排序输出，例如：ipvsadm -L --sort
注释：
 在表2中，左边括弧中的内容为ipvsadm每个选项的长格式表示形式，linux命令选项中，有长格式和短格式，短格式的选项用的比较多，实际应用中可以用括弧中的长格式替代短格式，例如，可以用“ipvsadm --clear”代替“ipvsadm -C”。
（2）举例
[root@localhost ~]# ipvsadm -A -t 192.168.60.200:80 -s rr -p 600
以上表示在内核的虚拟服务器列表中添加一条192.168.60.200的虚拟服务器记录，并且指定此虚拟服务器的服务端口为80，然后指定此虚拟服务器的调度策略为轮询调度，并且在每个real server上的持续服务时间为600秒，即10分钟。
[root@localhost ~]# ipvsadm -A -t 192.168.60.188:21 -s wlc
以上表示在内核的虚拟服务器列表中又添加了一条192.168.60.188的虚拟服务器，此虚拟服务器的服务端口为21，即FTP服务。使用的调度策略为wlc，即加权最少链接算法。
[root@localhost ~]# ipvsadm -a -t 192.168.60.200:80 -r 192.168.60.132:80 –g
[root@localhost ~]# ipvsadm -a -t 192.168.60.200:80 -r 192.168.60.144:80 –g
以上两条设置表示在虚拟服务器192.168.60.200中添加两条新的Real Server记录，两个Real Server的IP分别为192.168.60.132和192.168.60.144，参数“-g”指定了虚拟服务器的工作模式为直接路由模式，即DR模式。
这样设置完毕后，当用户访问192.168.60.200的80服务时，LVS会根据设置的调度策略和路由模式将请求分配到192.168.60.132以及192.168.60.144的80端口。
