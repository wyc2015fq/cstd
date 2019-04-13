
# 虚拟机安装CentOS以及SecureCRT设置【完美无错版】 - 无界 - CSDN博客

置顶2012年06月03日 23:28:24[21aspnet](https://me.csdn.net/21aspnet)阅读数：23102


**一、CentOS简介**
CentOS是Linux的发行版之一，它安全、稳定、高效，是我最喜欢的Linux发行版之一。CentOS根据Red Hat Enterprise Linux开放源代码编译而成，与RedHat Linux并没有什么本质上的差别。但Red Hat Enterprise Linux是商业软件，使用必须向RedHat公司付费，而CentOS并没有任何使用上的限制。如果你需要企业级操作系统的稳定性，又不想付费去获得服务支持，CentOS绝对会是你最好的选择。
**二、CentOS下载**
目前CentOS最新版本是6.3，下载地址：
[http://mirrors.163.com/centos/6.3/isos/i386/](http://mirrors.163.com/centos/6.3/isos/i386/)

注意：不要下载LiveCD版，这个安装后重启就没有了。
如果上述地址不能下载，可以去这里找：[Linux开源资源优秀链接汇总【不断更新】](http://blog.csdn.net/21aspnet/article/details/6754126)

**三、虚拟机下载安装**
Windows中安装Linux肯定要虚拟机，建议用Oracle VM VirtualBox就可以，因为这个只有80多M而且是**免费**的不要去破解。
下载：[https://www.virtualbox.org/wiki/Downloads](https://www.virtualbox.org/wiki/Downloads)
也可以用VMware，这个比较大。
**四、CentOS安装步骤(这里仍然以6.2安装界面截图，因为安装变化不大)**
下载完成后得到两个安装镜像文件（**CentOS-6.2-i386-bin-DVD1、CentOS-6.2-i386-bin-DVD2**），安装系统只用到第一个镜像文件（**CentOS-6.2-i386-bin-DVD1**），另一个镜像文件（**CentOS-6.2-i386-bin-DVD2**）是附带的软件包。
注意 ：
1.在虚拟机中要选择内存  >512M  不然没有图形化的安装界面
2.安装过程中如果提示需要第二张盘，只要在设置中修改盘片位置即可。
**3.安装虚拟机**
说明，安装步骤我们只讲要点和容易出错或者这一步搞错走不下去的，很容易的就一部带过一般是下一步就可以。
打开VirtualBox，点“新建”
![](http://hi.csdn.net/attachment/201111/12/0_1321085781mN29.gif)
下一步
![](http://hi.csdn.net/attachment/201111/12/0_1321085849v2Zw.gif)
这时要注意，下拉系统没有CentOS，其实你选Red Hat就可以，因为CentOS完全是和RedHat一致的。
下一步，选默认内存>512M就可以。
下一步，选默认创建新的虚拟硬盘。
下一步，选默认选VID。
下一步，选默认选Dynamicaiiy。
下一步，默认VDI位置。
下一步，创建creat。
好了，这个时候就完成了。
![](http://hi.csdn.net/attachment/201111/12/0_1321086183qtw2.gif)
下面很关键的一步，点“设置”，其他不要变，只要点“Storage”.
![](http://hi.csdn.net/attachment/201111/12/0_1321086320fpb4.gif)
点“控制器”下的“没有盘片”，点“属性”右边的光盘图标，弹出的框选"Choose a virtual....”其实就是选你本地之前下载的操作系统的镜像文件位置。
![](http://hi.csdn.net/attachment/201111/12/0_1321086529PXJR.gif)

**4.安装CentOS**
安装第一步选择：
![](https://img-my.csdn.net/uploads/201206/03/1338738235_3592.png)
选择第一项即可进行安装（**直接回车**），各项具体含义：
安装或升级现有系统
用基本的显卡驱动安装系统
进入系统修复模式
硬盘启动
**是否对安装镜像进行测试**（直接跳过，选skip）
![](https://img-my.csdn.net/uploads/201206/03/1338738300_5620.png)
后面语言键盘选择随个人偏好即可。

点击“NEXT”,语言选“简体中文”。
选美式键盘，下一步，选“基本存储设备”。
![](http://hi.csdn.net/attachment/201111/12/0_13210873073keK.gif)
选“重新初始化”。一直下一步，时区选上海即可。设置密码，注意的是超级用户名是root
![](http://hi.csdn.net/attachment/201111/12/0_13210878814kW5.gif)
下一步，替换现有的Linux
![](http://hi.csdn.net/attachment/201111/12/0_1321087904oVJ2.gif)
**选择安装（分区）方式**（手动设置挂载分区，不要选Create Custom Layout这样要自定义分区，建议选Replace Existing Linux System 替换现有的最快最简单）
选择安装的软件（默认是最小安装Minimal，个人研究学习建议安装桌面，Desktop，不然会很不方便），点Next就开始安装系统
![](https://img-my.csdn.net/uploads/201206/03/1338738367_7103.png)
后面开始安装了。其实都是下一步就可以，默认设置即可。
最后安装好重新启动以后，进入主界面。
![](http://hi.csdn.net/attachment/201111/13/0_1321169245SBBq.gif)
如果觉得虚拟机分辨率很小，那么可以点击“设备”--“安装增强功能”解决。
=======================================
注意你要是想从虚拟机切回windows只要按   右边的ctrl键+鼠标右键  就可以了。
**五：配置网络**：
1.虚拟机选择“桥接”
注意的是在虚拟机中网卡选择，如果你是无线路由器需要选无线Wireless，否则选Realtek
![](https://img-my.csdn.net/uploads/201211/19/1353302841_8292.jpg)

2.查看ip
\#ifconfig

**六:本地Windows通过SecureCRT连接CentOS**
本地SecureCRT中就可以连接虚拟机中的Linux了，需要注意的是SecureCRT要做适当的修改。
**1.选项--会话选项--终端--外观--当前颜色方案以及字符编码修改**
![](https://img-my.csdn.net/uploads/201211/19/1353303809_2765.jpg)

**2.x/y/Zmodem上传下载位置修改**
上传命令
\#rz -y  文件
下载命令
\#sz

七:**VirtualBox虚拟机中打开USB和光驱**
在虚拟主机VirtualBox的菜单中选择设备，分配USB设备，找到对应的USB接口即可，安装好驱动。
![](http://hi.csdn.net/attachment/201111/20/0_13217971735zY8.gif)

**八：VirtualBox虚拟机和主机拷贝文件**
主机windows要和VirtualBox拷贝文件，需要设用数据空间功能。
在VirtualBox的设备的“数据空间”的“添加数据空间”。
![](http://hi.csdn.net/attachment/201111/20/0_1321797756xmJ9.gif)
**九： 其他设置**
手动设置IP：
ifconfig eth0 192.168.1.7 netmask 255.255.255.0
route add default gw 192.168.1.1
ping 8.8.8.8           测试
ping 192.168.1.1   ping网关
重启不用配置可以在
vim  /etc/rc/local配置上述2句即可
===============
【桥接】配置内网【手动DHCP】
![](https://img-blog.csdn.net/20170326153619594)
地址：192.168.1.10X
子网掩码：255.255.255.0
网关：192.168.1.1
DNS：192.168.1.1

\# netstat -rn
Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window irtt Iface
172.16.44.0     0.0.0.0          255.255.255.0    U         0      0          0 vmnet8
172.16.10.0     0.0.0.0          255.255.255.0        U         0      0          0 vmnet1
172.16.0.0       0.0.0.0          255.255.252.0        U         0      0          0 eth0
169.254.0.0     0.0.0.0          255.255.0.0           U         0      0          0 eth0
0.0.0.0         172.16.0.254    0.0.0.0           UG        0      0          0 eth0

(以0.0.0.0开始的行的gateway是默认网关)

DNS:
[root@localhost ~]\# cat /etc/resolv.conf
search               localdomain
nameserver 172.16.0.250
=========================
1.添加一个普通用户，并设置密码
`useradd -Um 2dan`
`passwd 2dan`
2. 查看网络状态并启用有线网络eth0，通过dhcp获取地址，并检查获取到的ip地址
`ifconfig eth0`
`dhclient eth0`
`ifconfig eth0`
3. 进行一次在线升级
`yum update`
4. 安装桌面系统
`yum groupinstall "Desktop"`
`yum groupinstall "X Window System"`
5. 安装中文支持
`yum groupinstall "Chinese Support"`
[注意] 安装minial centos时，如果默认选择的是英文,需要修改/etc/sysconfig/i18n，将LANG修改为`LANG="zh_CN.UTF-8"`
6. 启动gnome
`startx`
然后按下Ctrl+Alt+F2
如果要开杨自动启动gnome,则编辑/etc/inittab文件,将
`id:3:initdefault`
修改为
`id:5:initdefault`
重启后生效。
还是有很多问题收集在这里：

### [CentOS装机必备-基本设置以及缺失文件        ](http://blog.csdn.net/21aspnet/article/details/8173338)
主要是一些缺失文件吧安装


