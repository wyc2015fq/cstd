# CentOS6.4 上搭建NIS网络信息服务器 - weixin_33985507的博客 - CSDN博客
2017年08月03日 14:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
**[NIS](http://www.1987.name/t/nis)**(Network Information Service)网络信息服务，主要功能是提供用户登录信息给客户端主机查询之用，用于企业局域网[Linux](http://www.1987.name/t/linux)主机账户的集中管理（非跨平台）。NIS服务器在大型企业网络环境可以使用Master/Slave（主控/辅助）架构。
## NIS服务器需知
![NIS](http://www.1987.name/wp-content/uploads/2012/11/NIS.gif?pt=5&ek=1&kp=1&sce=0-12-12)
NIS服务器工作原理图
当客户端主机连接NIS服务器时候，Server端会将相关数据提供给客户机，包含以下信息：
- **/etc/passwd** 用户账号、UID、GID、家目录位置、[shell](http://www.1987.name/shell)等信息。
- **/etc/group** 群组数据以及GID的对应关系，还有该群组成员。
- **/etc/hosts **主机名与IP的对应关系。
- **/etc/services **每一种服务（守护进程）所对应的端口。
- **/etc/protocols** TCP/IP封包协定，如TCP, UDP, ICMP等。
- **/etc/rpc** 每种RPC服务所对应的程序号码。
- **/var/yp/ypservers** NIS服务器所提供的数据库。 
- 
  NIS服务器所提供的数据不是传统的文本数据，而是经过NIS服务器将配置文件数据重新编译成读写速度较快的“数据库”文件系统。
**NIS Server (master/slave)运作流程：**
1、NIS Master将本地系统账户信息制作成数据库档案；
2、NIS Master会主动通知NIS Slave更新数据；
3、NIS Slave也会主动找NIS Master获取更新后的数据；
4、NIS 服务器帐号信息有变化时候需要重新只做数据库档案，且重新同步Master/Slave。
**NIS Client登录时查询流程：**
1、NIS Client登录时候会先查询本地/etc/passwd, /etc/shadow等账户档案。
2、本地找不到账号数据开始向NIS网段主机广播查询，NIS服务器不论Master/Slave都会响应，先响应者优先。
**所需软件包：**
- - **yp-tools** 提供NIS相关命令查询功能
- **ypbind** 提供NIS Client端软件
- **ypserv** 提供NIS Server端软件
- **rpcbind** rpcbind(老版本叫protmap)是一个管理RPC连接的程序，rpcbind服务对NIS是必须的，因为它是NIS的动态端口分配守护进程，如果rpcbind不启动，NIS也无法运行。 
## NIS Server端配置
NIS服务器端主要在于提供数据库给客户端作为验证之用，NIS服务器类型有 Master 与 Slave，本文里只是配置了Master 。
### 服务器端软件包安装
- - yum -y install yp-tools ypbind ypserv rpcbind
NIS相关配置文件
- - - **/etc/ypserv.conf**：ypserv主要配置文件，可控制客户端登录权限。
- **/etc/hosts**：保存NIS Server/Client用到的网络主机名与IP的对应关系，每一台主机名与IP都需要记录！
- **/etc/sysconfig/network**：指定NIS的域名（和 nisdomainname命令实现相同功能）。
- **/var/yp/Makefile**：和建立数据库有关的动作配置文件。 
NIS相关服务
- - **/usr/sbin/ypserv**：NIS服务器的主要提供服务。
- **/usr/sbin/rpc.yppasswdd**：提供的NIS客户端的用户密码修改服务，开启这个服务，NIS客户端可以直接修改在NIS服务器上的密码。 
NIS相关命令
- - **/usr/lib64/yp/ypinit**：建立数据库的指令，（32位系统：/usr/lib/yp/ypinit）。
- **/usr/bin/yppasswd**：NIS客户端操作命令，让用户修改服务器上的密码。 
### 设置NIS的域名
echo 'NISDOMAIN=liebaonis.local' >> /etc/sysconfig/network #设置NIS域名 
echo 'YPSERV_ARGS="-p 1011"' >> /etc/sysconfig/network   #设置NIS端口 
为了让yppasswdd也使用固定端口，便于防火墙管理：
- - sed -i 's/YPPASSWDD_ARGS=/YPPASSWDD_ARGS="--port 1012"/' /etc/sysconfig/yppasswdd
使用命令配置NIS域名的方法，三条命令都可以：
- - nisdomainname liebaonis.local
- - ypdomainname liebaonis.local
- - domainname liebaonis.local
### 主配置文件 /etc/ypserv.conf
- - #NIS服务器大多使用于内部局域网，只要有/etc/hosts即可，不用DNS！
- -  dns: no
- - #预设会有30个数据库被读入内存当中。
- -  files: 30
- - #需不需要将NIS注册到SLP（服务定位协议）
- -  slp: no
- - slp_timeout: 3600
- - #与Master/Slave有关，将同步更新的数据库比对所使用的端口，放置于<1024内。
- -  xfr_check_port: yes
- - # 下面是设定限制客户端或Slave Server查询的权限，利用冒号隔成四部分：
- -  # 
**主机名/IP:NIS域名:可用数据库名称:安全限制**- - # 
**[主机名/IP]**- -  可以使用 network/netmask 如 192.168.100.0/255.255.255.0 
- - # 
**[NIS域名]**- -  之前设定的NIS域名
- - # 
**[可用数据库名称]**- -  就是由 NIS 制作出来的数据库名称；
- - # 
**[安全限制]**- -  包括没有限制(none)、仅能使用<1024端口(port)、拒绝(deny)
- -  #*                                             :     *          :     shadow.byname : port 
- - #*                          : *       : passwd.adjunct.byname : port 
- - vi/etc/ypserv.conf
- - 屏蔽*开头的几行然后，增加行
- - 192.168.0.0/16 : *:* : none
- - 
### 设置主机名和IP地址对应关系
- - echo "192.168.188.30 nis.liebaonis.local nis" >> /etc/hosts #服务器IP与主机名对应 
- - echo "192.168.188.101 xx.liebaonis.local xx" >> /etc/hosts  #客户机IP与主机名对应 
- 
### 启动NIS所有服务
/etc/init.d/ypserv start
/etc/init.d/yppasswdd start
/etc/rc.d/init.d/rpcbind start
chkconfig --level 35 ypserv on
chkconfig --level 35 yppasswdd on
chkconfig --level 35 rpcbind on
### 创建用户和建立NIS数据库
为了防止客户端本地用户跟NIS用户冲突，所以创建NIS用户最好将UID设置为2000以上并建立独立的家目录。
mkdir /nishome
useradd -u 2001 -d /nishome/user1 user1 #用户名
echo password | passwd --stdin user1 #密码
将用户信息转换为NIS数据库文件：
/usr/lib64/yp/ypinit -m
At this point, we have to construct a list of the hosts which will run NIS
servers.  localhost.localdomain is in the list of NIS server hosts.  Please continue to add
the names for the other hosts, one per line.  When you are done with the
list, type a <control D>.
        next host to add:  nismaster.1987.name    #系统主机名自动提取
        next host to add:                         #按Ctrl+D继续
The current list of NIS servers looks like this:
localhost.localdomain
Is this correct?  [y/n: y]  y                     #键入“y”
We need a few minutes to build the databases...
Building /var/yp/1987.name/ypservers...
Running /var/yp/Makefile...
gmake[1]: Entering directory `/var/yp/1987.name'
Updating passwd.byname...
Updating passwd.byuid...
Updating group.byname...
Updating group.bygid...
Updating hosts.byname...
Updating hosts.byaddr...
Updating rpc.byname...
Updating rpc.bynumber...
Updating services.byname...
Updating services.byservicename...
Updating netid.byname...
Updating protocols.bynumber...
Updating protocols.byname...
Updating mail.aliases...
gmake[1]: Leaving directory `/var/yp/1987.name'
localhost.localdomain has been set up as a NIS master server.
Now you can run ypinit -s localhost.localdomain on all slave server.  
如果iptables开启需要开放相应的端口。配置数据库后需要重启NIS相关服务。
## NIS客户端设置NIS客户端设置很简单，配置NIS域名之后启动ypbind即可。
### 客户端软件包安装yum -y install yp-tools ypbind rpcbind
NIS客户端相关配置文件：
- - **/etc/sysconfig/network**：保存NIS的域名。
- **/etc/hosts**：各个NIS服务器的IP与主机名对应。
- **/etc/yp.conf**：这个则是ypbind的主配置文件，里面设定NIS服务器所在。
- **/etc/sysconfig/authconfig**：规范账号登入时的允许认证机制。
- **/etc/pam.d/system-auth**：因为账号通常由PAM 模块所管理，所以你必须要在PAM模块内加入NIS的支持才行。
- **/etc/nsswitch.conf**：规范账号密码与相关信息的查询顺序，默认是先找/etc/passwd再找NIS数据库。 
NIS客户端相关命令：
- - - **/usr/bin/yppasswd**：更改在NIS Database(NIS Server的数据库)的密码。
- **/usr/bin/ypchsh**：同上，但是是更改 [Shell](http://www.1987.name/shell)。
- **/usr/bin/ypchfn**：同上，但是是更改一些用户信息。
- **/usr/sbin/yptest**：直接在NIS Client输入yptest即可检查相关的测试数据。
- **/usr/bin/ypwhich -x**：显示NIS Client与Server之间沟通的数据库有哪些。 
新安装的CentOS 6.4系统执行setup 命令，提示命令找不到
[root@localhost ~]# setup 
-bash: setup: command not found
解决方案：安装setup必要的软件包 setuptool
#安装setuptool
[root@localhost ~]# yum install setuptool 
### 配置NIS客户端
如果手动修改配置文件会相当麻烦，这里使用 setup 命令配置NIS客户端：
1、选择Authentication configuration项回车：
![](http://www.1987.name/wp-content/uploads/2012/11/setup1.gif?pt=5&ek=1&kp=1&sce=0-12-12)
利用 setup 进入 authconfig 认证项目 
2、选择 Use NIS 按Next到下一步：
![](http://www.1987.name/wp-content/uploads/2012/11/setup2.gif?pt=5&ek=1&kp=1&sce=0-12-12)
进入 authconfig 之后，选择 NIS 项目
  3、填写 NIS 域名 (Domain) 和 NIS 服务器的 IP (Server)，按下确定即可：
![](http://www.1987.name/wp-content/uploads/2012/11/setup3.gif?pt=5&ek=1&kp=1&sce=0-12-12)
填写 NIS 域名和 NIS 服务器的 IP
之后系统会打印出以下信息：
Stopping portmap: [ OK ]
Starting portmap:                                          [  OK  ]
Shutting down NIS services:                                [  OK  ]
Turning on allow_ypbind SELinux boolean
Binding to the NIS domain:                                 [  OK  ]
Listening for an NIS domain server.
NIS客户端到这里已经配置完成。
