# ubuntu组件集群 - 战斗蜗牛的专栏 - CSDN博客





2010年01月26日 21:21:00[vbskj](https://me.csdn.net/vbskj)阅读数：2352








利用Ubuntu组建机群
参考资料：

[http://mikilab.doshisha.ac.jp/dia/research/report/2005/0510/001/report20050510001.html](http://mikilab.doshisha.ac.jp/dia/research/report/2005/0510/001/report20050510001.html)

[http://micro.ustc.edu.cn/Linux/text/hp.htm](http://micro.ustc.edu.cn/Linux/text/hp.htm)

[http://www.ibm.com/developerworks/cn/linux/cluster/lw-clustering.html](http://www.ibm.com/developerworks/cn/linux/cluster/lw-clustering.html)

现在cluster越来越受到HPC领域的广泛欢迎，本文介绍了利用Ubuntu组建有盘机群的步骤。

操作系统：Ubuntu/GNU Linux 7.10 




1.    设备需求 

组建机群需要的硬件包括：

PC硬件配置：

CPU
 Intel Core 2 Duo E6550(盒)

主板
 Intel DG33BU

内存
 金士顿 1GB×2 DDR2 667 

硬盘
 希捷160G 7200.10 16M SATA-II

光驱
 明基 DW2000 

网卡
 千兆网卡TP-Link TG-3269C

机箱电源
 华硕TA-863 +350W电源



　　1).4台以上配置的PC机。 
　　2).5根足够连接集群系统中每个节点的，使用5类非屏蔽双绞线制作的RJ45缆线 
　　3).1个1000M快速以太网(如：TP-Link TL-SG2224WEB )交换机 
　　4).1张Ubuntu7.10安装盘

其中一台作为服务器（此计算机有两个网卡，一个用来连接外部网络，另外一个用来连接内部网络），其余的机器作为计算节点用，具体硬件连接见下图。





Fig 1.1: 硬件连接图

下面详细介绍组建cluster的软件部分。 

2.    在master 和slave上安装Ubuntu 操作系统 
为了简化系统我们将所有的硬盘划为两个分区，一个作为交换分区/swap，大小为两倍的内存大小；剩下空间另一个为根分区/。具体的安装步骤可以参考下面的链接。 

[http://mikilab.doshisha.ac.jp/~t-kohata/memowiki/pukiwiki.php?sarge](http://mikilab.doshisha.ac.jp/~t-kohata/memowiki/pukiwiki.php?sarge)

3.    配置环境和网络 
安装完操作系统之后根据实际情况配置下面的文件。 

l         #vi /etc/apt/sources.list 

deb [http://debian.cn99.com/ubuntu](http://debian.cn99.com/ubuntu) gutsy main multiverse restricted universe 

deb-src [http://debian.cn99.com/ubuntu](http://debian.cn99.com/ubuntu) gutsy main multiverse restricted universe

deb [http://debian.cn99.com/ubuntu](http://debian.cn99.com/ubuntu)  gutsy-security main multiverse restricted universe   

l         #aptitude update

l         #aptitude upgrade  

l         IP地址分配表


 Master
 Slaves

Name
 hana
 node1
 node2
 node3

IP address
 Outside: 172.20.11.226
Inside: 192.168.1.1
 192.168.1.2
 192.168.1.3
 192.168.1.4


l         # vi /etc/network/options

ip_forward=yes          //make sure it is ‘yes’

spoofprotect=yes        

yncookies=no

4.    安装基本软件包 
安装必要的软件包。 

l         #aptitude install libncurses5-dev binutils bin86 gcc g++ make modutils g77 perl bzip2 rcconf 

5.    RSH 
在master上面工作，通常使用 BASH 这个shell 来传达给kernel 工作的讯息，以使主机master正确的工作；而如果在不同的主机之间，那就可以使用 R Shell 来进行指令的下达喔，如此一来，我们就可以直接在 A 机器，向 B 机器下达工作的指令，而不需要登入 B 机器。另外由于这个 RSH 是一个有安全隐患的服务，我们可以直接登入 RSH 主机，并且在上面进行指令的下达，为了避免还要输入密码的问题，因此通常 RSH 已经将信任主机设定好了，不过，由于 RSH 会激活一些 port 来监听 Clients 的需求，而偏偏这些 port 与 daemon 都还挺危险的，因此，Cluster 最好是设定在内部网域当中，并使用私有 IP ，比较能够避免危险。此外，那个 Master 也要设定好一定程度的严密防火墙。 

5.1           Master 机器 
l         #aptitude install rsh-client

首先下载安装rsh-client，接下来编辑以下文件。 

l         #vi /etc/hosts        在该文件中输入各个主机内部主机名称以及查询系统。

#IP address         domain name           hostname

192.168.1.1         (hana.cluster)           hana

192.168.1.2         (node1.cluster)          node1 

192.168.1.3         (node2.cluster)          node2

192.168.1.4         (node3.cluster)          node3

l         #vi /etc/network/interfaces 

配置网络，最为服务器的主机有两个卡，一一进行配置。

auto eth0

iface eth0 inet static

   address  172.20.11.226

   netmask 255.255.255.0

   network  172.20.11.0

   broadcast 172.20.11.255

   gateway 172.20.11.6

auto eth1

iface eth1 inet static

   address  192.168.1.1

   netmask 255.255.255.0

   network  192.168.1.0

broadcast 192.168.1.255

pre-up iptables –t nat –A POSTROUTING –s 192.168.1.0/24 –d 0.0.0.0/0 –o eth1 –j MASQUERADE 

l         #ifdown –a

l         #ifup –a

l         #/etc/init.d/networking restart

5.2           Slave 机器 
下载安装rsh-client 和 rsh-server。 

l         #aptitude install rsh-client rsh-server

由于我们只允许局域网内部进行访问，所以在这里添加允许访问的机器的IP地址。 

设置允许root进行rsh、rlogin

l         # vi /etc/pam.d/rlogin

    将authrequiredpam_securetty.so这一行开头添加#，从而注释该行。

l         #vi /etc/hosts.equiv

192.168.1.1

192.168.1.2

192.168.1.3

192.168.1.4

以上主机允许匿名访问，接下来配置下面的文件(红色部分表示要修改或添加的内容) 

l         #vi /ect/pam.d/rsh

auth     required   pam_nologin.so

auth     required   pam_env.so

auth     required   pam_rhosts_auth.so /

                                hosts_equiv_rootok promiscuous

account      required   pam_unix_acct.so

session       required   pam_unix_session.so

l         #vi /etc/pam.d/rlogin

#%PAM-1.0

auth            required   pam_nologin.so

#auth          required   pam_securetty.so

auth            sufficient pam_rhosts_auth.so /

                                hosts_equiv_rootok promiscuous

auth            required   pam_unix.so nullok

account             required   pam_unix.so

password   required   pam_unix.so nullok use_authtok obscure /

                                    min=4 max=8

session              required   pam_unix.so

l         #vi /etc/network/interfaces

auto eth0

iface eth0 inet static

    address 192.168.1.2 (or 3, 4)  

    netmask 255.255.255.0

    network  192.168.1.0

    broadcast 192.168.1.255

    gateway 192.168.1.1

配置完了之后重新启动网络。 

l         #/etc/init.d/networking restart

6.    MPI 
MPI（message passing interface）主要用来处理处理并行计算时节点之间的信息交换。MPI本身不是一个软件，只是一个通信标准，具体实现可以选用MPICH 

[http://www-uinx.mcs.anl.gov/mpi/mpich/](http://www-uinx.mcs.anl.gov/mpi/mpich/)

6.1           Master 机器 
安装MPICH软件包。 

l         #aptitude install mpich mpi-bin libmpich1.0-dev

下来配置下面的文件，在该文件中添加上允许信息交换的节点。 

l         #vi /etc/mpich/machines.LINUX

#localhost

#IP address

192.168.1.2

192.168.1.3

192.168.1.4

192.168.1.1(Master的IP要放在最后)

6.2           Slave 机器 
安装MPICH软件包。 

l         #aptitude install mpich mpi-bin libmpich1.0-dev

接下来配置下面的文件，在该文件中添加上允许信息交换的节点。 

l         #vi /etc/mpich/machines.LINUX

#localhost

#IP address

192.168.1.1

192.168.1.2

192.168.1.3

192.168.1.4

7.    NFS 
7.1           Master机器 
l         #aptitude install nfs-kernel-server 

l         #groupadd  nogroup

l         #useradd  nobody

l         #chown  nobody.nogroup  –R  /home  

l         #vi /etc/exports

# Shared direction   Node name (Model)   

/home          192.168.1.0/255.255.255.0(rw,no_root_squash,async,subtree_check)

l         #/etc/init.d/nfs-kernel-server restart

7.2           Slave机器 
l         #aptitude install nfs-common

l         #vi /etc/fstab

       在该文件中加入主机启动时可以mount的目录。这里mount的是远程master主机。mount point与 Source dev的目录要一致，否者运算时会出现路径找不到.

#Source dev   mount point   FS type    option          n  n

hana:/home    /home       nfs      defaults,rw,nolock   0   1

l         #/etc/init.d/nfs-common restart

l         #mount –a

l         #/etc/init.d/networking restart

8.    NIS
NIS(network information service) 使master和slave具有相同的账号群组关系。这样不同的机器具可以以相同的用户id以及群组id登录访问公共的信息 

8.1           Master机器 
架构NIS server请先确定是否有安装了相关的套件，缺一不可，如下列：ypserv、yppasswdd。如果没有安装进行安装。本文中已经安装过了，接下来直接安装NIS，在安装的过程中可能会需要输入NIS的域名，任意输入。主意这个NIS domain不是DNS domain。 

l         #aptitude install nis 

接下来设定NIS server 

1. 修改配制文件 /etc/defaultdomain 内容如下, 设定 NIS 的域:

hana

2. 执行 

#domainname hana

3. 修改配置文件 /etc/default/nis 确保有如下设置, 设定为 NIS 主服务器 

NISSERVER=hana

4. 修改配置文件 /etc/ypserv.conf

ypserver hana

5. 重启服务 

#/etc/init.d/nis restart

6.测试

使用 ypwhich 命令测试与服务器的连接情况 

# ypwhich
                   192.168.16.2

7.安全配制

在配置文件 /etc/ypserv.securenets 中,添加可以访问的客户机的ip地址范围,比如允许 192.168.1.0/255.255.255.0 网络段的所有主机访问

255.0.0.0                  127.0.0.0

255.255.255.0              192.168.1.0

8.2           Slave机器 
l         #aptitude install nis 

接下来是设定NIS client，而你必须先确定有安装ypbind和yp-tools套件 

1. 在配置文件 /etc/hosts 中添加 NIS 服务器的记录 

192.168.1.1 hana

2. 修改 /etc/defaultdomain 内容如下, 设定 NIS 域 

hana

3. 执行命令 

# domainname hana

4 修改配置文件 /etc/yp.conf, 加入如下内容, 指定索要使用的 NIS 服务器 

ypserver hana

5. 确保配置文件 /etc/nsswitch.conf 中，passwd, group, shadow 和 netgroup 条目如下设置 

passwd:         hana     compat

group:          hana      compat

shadow:         hana     compat



hosts:          hana      files dns

networks:       hana      files



protocols:      hana        db files

services:       hana        db files

ethers:         hana       db files

rpc:            hana      db files



netgroup:       hana

6. 在文件 /etc/passwd 尾部加入

+::::::              (6个:)

7. 在文件 /etc/shadow 尾部加入 

+::::::::            (8个:)

8. 在文件 /etc/group 尾部加入: 

+:::                 (3个:)

l         #/etc/init.d/nis restart

9.    测试 
到现在我们的cluster已经组建结束，现在来测试一下。测试步骤如下。 

l         # cp /use/share/doc/libmpich1.0-dev/examples/cpi.c  /home/hana

l         # cd /home/hana

l         # mpicc cpi.c

l         # mpirun -np 1(2,3,4) a.out

如果要安装meep或mpi，则运行如下命令：

l         #apt-get install mpb h5utils hdf5-tools libhdf5-mpich-dev mpb-mpi meep meep-mpi

运行meep程序时，可参照下面步骤进行：

1.         进入共享目录#cd /home/hana

2.         更改用户#su nobody

3.         用4个节点进行并行运行$mpirun –np 4 /usr/bin/meep-mpi ring.ctl|tee ring.out

10.             增加计算节点的方法 
当我们需要增加一个新的计算节点到该机群中去的时候，有两种方法可以实现，一种是采用全面介绍得slave机器的组建过程，另外我们也可以采用硬盘拷贝的方法来实现，因为所有的计算节点的功能是一样的。拷贝结束之后再修改一些本机固有的文件。比如/etc/network/interface,/etc/hostname等等。硬拷贝可以利用下面一条命令。 

#dd if=/dev/sda of=/dev/sdb

或者使用systemimager工具进行节点安装并更新

在服务器hana上安装nfs-kernel-server, rsh-client, rsh-server, systemimager-server

在所有节点机上安装nfs-common, rsh-client, rsh-server, systemimager-clien

1.首先配置好一个节点node1，然后在node1上运行prepareclient：#prepareclient –server hana

2.在服务器上运行：

#mkdir /var/lib/systemiamger/images/clusternode

#getimage -golden-client node1 -image clusternode
node1是配置好的节点，clusternode是镜像目录名，这个目录缺省是在/var/lib/systemiamger/images下。getimage读取完节点机的信息后，会问你是否配置自动安装： y 会执行addclients来配置自动安装信息。

服务器上生成镜像后，在其余的节点机上运行
#updateclient -server hana -image clusternode
系统就会自动的clone一个节点出来。（注：在更新节点机时，最好先备用一下boot目录，更新完成后再还原boot目录，以防止更新后节点机的grub被破坏而启动不了）
node2:~# updateclient –server hana -image clusternode
Updating image...
receiving file list ... done
集群各个节点的更新与升级可以用C3 + systemimager来完成：
1.配置好一个节点，prepareclient；
2.在systemimager-server上，用getimage 来更新节点信息；
3.用cexecs -p -c "updateclient ... "在全部节点上更新。


本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/eagelangel/archive/2008/07/15/2656829.aspx](http://blog.csdn.net/eagelangel/archive/2008/07/15/2656829.aspx)



