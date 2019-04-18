# linux开机启动顺序 - happyhorizon的算法天空 - CSDN博客
2019年03月05日 17:02:12[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：52
### 文章目录
- [linux的开机启动顺序概述](#linux_2)
- [BIOS: basic input output system 基本输入输出系统](#BIOS_basic_input_output_system__14)
- [MBR: master boot record 主引导记录.](#MBR_master_boot_record__20)
- [主引导程序](#_22)
- [总结](#_32)
- [第一个程序: init](#_init_39)
- [运行等级](#_47)
- [System V initialization方式](#System_V_initialization_61)
- [Upstart方式](#Upstart_69)
- [Ubuntu自动启动程序说明](#Ubuntu_77)
- [用户自定义引导启动程序](#_138)
- [变换运行等级](#_144)
- [参考](#_149)
# linux的开机启动顺序概述
1 加载BIOS硬件信息, 并获取第一个启动设备的代号;
2 读取第一个启动设备的MBR的引导加载程序的启动信息;
3 加载核心操作系统的核心信息, 核心开始解压缩, 并且尝试驱动所有的硬件设备;
4 核心执行init程序并获取运行信息;
5 init执行/etc/init.d的脚本;
6 启动核心的外挂模块/etc/modeprobe.d/中的脚本;
7 init执行运行各个批处理脚本;
8 init执行/etc/init.d/rc.local文件;
9 执行/bin/login程序, 等待用户登录;
10 登录之后开始以shell控制主机.
# BIOS: basic input output system 基本输入输出系统
BIOS非常重要, 记录了主板芯片的相关设置,例如CPU和接口设备的通信频率, 启动设备的搜索顺序,硬盘大小和类型, 系统时间, 外部总线是否启动即插即用(Plug and Play, PnP), 各接口设备的I/O地址, 以及与CPU通信的IRQ中断等信息.
获得BIOS设置之后, 就启动加电自检(Power on self test, POST), 初始化硬件检测, 设置PnP设备, 确定可启动的设备.
BIOS 找到硬盘的第一个扇区之后就查找 MBR 的位置，这是最基本的引导程序（Boot loader），这个程序一旦启动，BIOS 的任务才算圆满。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190305170132362.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
# MBR: master boot record 主引导记录.
## 主引导程序
MBR主引导记录中主要是安装加载程序,  这个引导加载程序猪丫就是用于识别磁盘的格式, 这样才好加载操作系统的核心(kernel).这个时候用户可以对开机选项进行操作, 例如现在主流的grub引导加载程序不仅可以可以支持linux,还可以支持windows.
一般linux的引导加载程序都可以识别window的核心文件,但是windows的引导加载程序却不能识别linux, 这也就是为什么在安装双系统的时候需要格外设置grub引导的原因.
通过引导加载程序读取linux操作系统核心文件后, 就会将核心解压缩到主存储器中, 开始测试和驱动各个外部设备,包括存储, CPU, 网卡, 声卡等等. 这些核心文件通常在/boot里, 名为vmlinuz.
加载核心过程的精彩远不止于此. 通常这时系统只会以只读的方式载入根目录, 有的时候为了某些功能可以用文件方式来读取, 在有的系统时还会建立虚拟盘, 就是说,系统会用initrd命令建立虚拟盘, 用linuxrc程序加载模块映像文件(/boot/initrd-xxx.img).
当核心驱动外部硬件的工作完成后, 就会删除该虚拟盘, 值得注意的是, initrd并非必要, 这样做的目的主要是为了保证启动的顺利进行.
## 总结
总结一下,就是引导加载程序找到Linux的核心文件, 加载到主存储器, 之后用initrd建立虚拟盘, 协助启动过程, 最后将BIOS中的硬件信息数据交给Linux核心检测, 之后加载硬件驱动.
加载BIOS-> MBR基本引导程序(用户开机选项) -> 载入并启动系统核心
# 第一个程序: init
现行的Linux distros主流的有两种init方式：一种是广为流传的System V initialization，它来源于Unix并且至今仍被各种Linux distros所采用；另一种是近几年提出的Upstart方式，基于事件机制，系统的所有服务，任务都是由事件驱动的。据我所知，采用后一种方式的目前有Ubuntu（6.10 and later），Fedora（9.10 and later），Debian（optional）。虽然采用Upstart的发行版并不多，但它旨在取代旧式的System V initialization。
## 运行等级
init首先会读取运行等级的设置, 那么什么是运行等级?
|等级|运行模式|
|----|----|
|0-halt|系统直接关机|
|1-single user mode|单用户维护模式, 用于系统出问题时进行维护|
|2-multiuser mode, without NFS|多用户模式, 但是没有NFS服务|
|3-Full multiuser mode|多用户, 完整含有网络功能的纯文本模式|
|4-unused|系统保留功能|
|5-X11|多用户, 使用X window|
|6-reboot|重启模式|
## System V initialization方式
这种方式是鸟哥的linux私房菜中介绍的init启动方式. 核心加载完成之后, 系统执行的第一个程序就是/sbin/init. init程序会读取设置文件"/etc/inittab" 获取运行等级.
inittab文件负责初始化系统, 设置系统runlevel及进 入各runlevel对应要执行的命令。假设当前inittab中设置的默认runlevle是5，则init会运行/etc/init.d/rc 5命令，该命令会依据系统服务的依赖关系遍历执行/etc/rc5.d中的脚本/程序。进入/etc/rc5.d目录可以发现里面的文件都是到/etc /init.d/下对应的脚本/程序的软链接。以S开头的为启动的意思，以K开头的为停止。并且S/K后面的两位数数字代表了服务的启动顺序（由服务依赖关系决定）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019030516563152.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
## Upstart方式
Upstart job是事件驱动的，系统服务的启动、停止等等均是由事件决定的，反过来，系统服务的启动、停止也可以作为事件源触发其他服务。并且事件并不一定得由系统内部产生，用户可以手工的键入start/stop [Service]产生事件来启动/终止服务。man upstart-evnets查看upstart job所定义的事件，可以发现，runlevel也被当作事件来对待（因runlevel的改变而产生的事件），还有其他如 startup，started，filesystem等等。那么系统服务又是如何知道自己应该什么时候启动，什么时候终止的呢？答案就在于/etc/init中（有的distros可能是在/etc/event.d）。进入/etc/init目录下一看，均是系统服务的配置文件，或者说，是job definition files。
并不仅仅在系统启动初期，在系统运转的任何时期都可以通过发送事件来启动或终止服务。这便是Upstart job的优点之一，除了用于系统初始化，还可以在系统运行阶段发挥作用。
相比之下System V initialization方式下的配置文件一般只用于系统初始化阶段，当然系统运行阶段我们可以通过/etc/init.d/Service start/stop/otherCommand来操作服务，但很明显不如Upstart方式简洁明白。
## Ubuntu自动启动程序说明
Ubuntu init系统初始化流程是怎么样的呢? 实际上Ubuntu采用的是兼容模式，即：系统中既有System V-style启动的服务，也有Upstart启动的服务. 进入/etc文件夹, 可以看到:
如何兼容? 实际上ubuntu是通过/etc/init下的某些配置文件调用/etc/rc${runlevel}.d/中的脚本以启动采用旧式System V-style的服务.
具体来说, 首先进入/etc/init目录（Upstart init会到该目录下读取配置文件），会发现几个跟rc有关的配置文件
```
rc.conf   # rc在系统runlevel变化时被启动
rc-sysinit.conf # rc-sysinit在startup事件发生时被启动
rcS.conf # rcS在系统runlevel为S时启动
```
这几个文件，正是Upstart init处理System V-style服务的关键。rc-sysinit在startup事件发生时被启动，即，Upstart init会首先读取rc-sysinit.conf并执行相关配置和脚本。rc-sysinit.conf的主要工作是设置系统默认runlevel，检测是否存在/etc/inittab或内核命令行，若存在，则按内核命令行>/etc/inittab>默认runlevel的顺序设置系统 runlevel。最后，调用telinit进入设置的runlevel。
由于调用了telinit进入了设定的runlevel，runlevel改变的事件发生，此时rc服务启动（当然其他服务也会）。那么，我们就有必要来看看rc.conf中到底有什么东西。打开rc.conf，注意到最后一行：
```
exec /etc/init.d/rc $RUNLEVEL
```
很明显，`/etc/init.d/rc`被调用了，并且传入了早前设置好的系统runlevel作为参数。而`/etc/init.d/rc`会根据传入 的runlevel参数调用`/etc/rc${runlevel}.d/`下的脚本（以S开头）以启动服务，终止在前次runlevel启动而当前在 runlevel需要终止的服务。
每个rc (1-6).d文件夹下的文件其实都是/etc/init.d文件夹下的文件的一个软连接（类似windows中的快捷方式），也就是说，在 `/etc/init.d`文件夹下是全部的服务程序，而每个rc(1-6).d只链接它自己启动需要的相应的服务程序.
具体来说, 不同的运行等级适用于不同的情况: 0-4-6关机,保留,重启, 1-2-3-5 用户模式.
`/etc/rc${runlevel}.d/`路径下的脚本:
```
K01apache-htcacheclean S03acpid S03whoopsie
K01xl2tpd S03anacron S04avahi-daemon
README S03cgroupfs-mount S04bluetooth
S01apport S03cron S04docker
S01nmbd S03dbus S04lightdm
S01php7.0-fpm S03irqbalance S05cups
S01rsyslog S03kerneloops S05cups-browsed
S01samba-ad-dc S03mysql S05saned
S01shadowsocks S03rsync S06smbd
S01ubuntu-fan S03speech-dispatcher S07grub-common
S01unattended-upgrades S03ssh S07ondemand
S01uuidd S03sysstat S07plymouth
S01virtualbox S03thermald S07rc.local
S02apache2 S03vsftpd
```
总结一下, 首先Ubuntu的/etc路径下
```
/etc/init # 系统服务的配置文件
/etc/init.d  # 所有自动启动的服务程序都在/etc/init.d这个文件夹里，里面的文件全部都是脚本文件.
/etc/rc${run_level}.d # 运行等级0-6要随机启动的程序
```
init时,首先会读取rc-sysinit.conf, 设置系统默认运行等级, 触发了rc服务, 读取rc.conf, 执行该默认运行等级的初始化脚本以启动系统服务. 通常运行等级为5, 也就是自动启动/etc/rc5.d中的脚本.
就这样, ubuntu通过rc-sysinit和rc间接的调用/etc/init.d/rc从而启动System V-style服务，在采用新式Upstart init照顾了旧式的System V init。
# 用户自定义引导启动程序
用户想要随开机启动的命令, 都可以写成shell脚本, 添加到`/etc/rc.d/rc.local`中.
# 变换运行等级
用init命令加上等级的序号就可以,例如`init 3`
# 参考
[https://www.jianshu.com/p/808fa4d82798](https://www.jianshu.com/p/808fa4d82798)
[https://blog.csdn.net/heqiyu34/article/details/18793857](https://blog.csdn.net/heqiyu34/article/details/18793857)
鸟哥的linux私房菜
