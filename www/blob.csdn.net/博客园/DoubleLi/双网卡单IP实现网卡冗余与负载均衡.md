# 双网卡单IP实现网卡冗余与负载均衡 - DoubleLi - 博客园






WINDOWS下：

所谓双网卡，就是通过软件将双网卡绑定为一个IP地址，这个技术对于许多朋友来说并不陌生，许多高档服务器网卡（例如intel8255x系列、3COM服务器网卡等）都具有多网卡绑定功能，可以通过软硬件设置将两块或者多块网卡绑定在同一个IP 地址上，使用起来就好象在使用一块网卡，多网卡绑定的优点不少.

首先，可以增大带宽，假如一个网卡的带宽是100M，理论上两块网卡就是200M，三块就是 300M，当然实际上的效果是不会是这样简单的增加的，不过经实际测试使用多个网卡对于增加带宽，保持带宽的稳定性肯定是有裨益的，如果交换机等相关条件不错的话，这个效果还是很能令人满意；

其次，可以形成网卡冗余阵列、分担负载，双网卡被绑定成“一块网卡”之后，同步一起工作，对服 务器的访问流量被均衡分担到两块网卡上，这样每块网卡的负载压力就小多了，抗并发访问的能力提高，保证了服务器访问的稳定和畅快，当其中一块发生故障的时候，另一块立刻接管全部负载，过程是无缝的，服务不会中断，直到维修人员到来。先下载一个软件NIC Express下载完软件，先不忙安装，咱们还是先准备好硬件。

第一部分：硬件安装

虽然，理论上讲绑定越多网卡在一起，最终效果提升就越明显，但是考虑到复杂程度，这里就以绑定 双网卡为例进行说明，如果读者觉得好玩，兴致很高的话，按照下面方法愿意绑定多少就绑定多少个网卡，其实一般同一台服务器，绑定2－3块网卡也就够了，太 多了，据说因为链路聚合的先天缺点，会过多占用服务器资源，反过来会影响服务器速度（关于这个我没实验，不过我觉得凡事都离不开物极必反的道理，适度最好）。打开服务器机箱，把两块网卡拧在主板PCI插槽上！：》 硬件的安装很简单，我也不在这里说了。省的大家骂我：》

第二部分：设置调试

下面要进行设置及调试了，也就是要将这两块8139D廉价网卡，如同高档服务器网卡那样绑定在一起，使用同一个IP地址，同时同步工作。其过程并不复杂，估计20分钟足够了。 将刚刚下载的NIC Express软件的安装包NIC4.rar解压缩得到安装文件“NICExpressW2KEE.exe”，双击它启动安装程序，一路NEXT，软件提示输入unlock key（注册码），如果没有注册码，就只好点击Demo，选择试用，这样可以获得30天的免费试用期，在这30天里如果觉得不错，你可以想办法去弄一个注 册码（怎么弄？找小编问，他可能有，哈哈）。到下图所示界面，软件提示选择是否开启LOAD Balancing 功能？什么是LOAD Balancing 功能呢？LOAD Balancing的中文意思可以翻译为负载均衡，在这里就是网络负载均衡。也就是当多块网卡被绑定合一之后，当数据流量很大的时候，软件会自动调整，将数据流量负载均衡地分配到各个网卡上，以减轻单块网卡的压力，达到畅快的访问效果。我们绑定双网卡，其中目的之一就是为了实现负载均衡，我们自然要开启这 个功能，所以，在这里一定要选择“Enabled”。

当然，如果你在这里选择错了也没关系，今后也可以通过NIC Express软件管理界面开启。继续一路NEXT，在Windows XP里安装时如果遇到提示“NIC Express Virtual Miniport”没有通过Windows测试，无法验证它同Windows XP的相容性，不要理会，选择“仍然继续”就行了。

到了下图所示界面，就到了真正绑定网卡的时候了： 大家看到这个界面一共分为上、中、下，三个窗口，上面的是空白，中间的写着8139-2，这个8139-2是我自己起的绑定之后的网卡组的名称，原来这里默认写的是New array，也可以不修改，你也可以根据自己喜好，写成别的名字。在最下面的窗口里列出了目前服务器上安装的两块网卡的名字。我们下一步就是要用鼠标选中下面的两块网卡名字，然后点击界面中间的Add键，把两块网卡加入上面的窗口里，这样两块网卡就这样被加入了网卡组里，初步绑定成一块“网卡”了，今后可 以使用同一个IP地址了。点击OK继续，NIC Express出现一个配置界面，选项很多，但是不必太操心，因为这些配置都不必修改，使用默认值就可以了，直接点击OK、点击Finish完成安装进 程。至此软件安装基本结束，剩下就需要对软件和网卡进行一些必要的设置工作。

点击桌面“开始”菜单，选择执行菜单里的“NIC Express Enterprise Edition”选项，这是NIC Express软件自带的一个监控程序，首先点击“setting”选项，在这里可以设置网卡流量计量单位，可以按照Packets/Sec、Mbits /Sec、Kbits/Sec三种单位来计算，一般都是使用默认的Mbits/Sec来计算，也就是兆/每秒，其实在这里只需要修改一下“Graph Detail（图形显示）”即可，将默认的“By Protocol”改成“By Incoming/Outgoing”，别的不需要改。

如果你想分别观察绑定组中每块网卡的流量或者整个组的流量，只需在“Select Device”选项中选择一下设备即可。最后点击“Advanced”选项，设定网卡绑定的工作模式，在这里也可以点击“Load Balancing Disable”来关闭网卡负载均衡功能。使用NIC Express绑定的双网卡组默认工作于“NIC Express ELB”模式下，这是NIC Express所特有的一种工作模式，实际效果***。我们都知道利用昂贵的Intel PROSET绑定的Intel 8255x的组合是运行在“802.3ad”工作模式下的，这一模式在NIC Express中也可以选择，但是多数使用者在实际使用后都认为“NIC Express ELB”模式的效果优于“802.3ad”模式，大家今后可以自己实践测试比较一下。如果你也使用默认的“NIC Express ELB”模式，那么“Advanced”选项里也就不用改什么，一切默认即可。至此NIC Express的设置结束。

第三部分：“虚拟网卡”设定

最后还要看看网卡的设置，用鼠标在桌面“网上邻居”上点击右键弹出“网络和拨号连接”窗口，可以看到原来的两个网卡连接图标已经变成了三个，多出来的一个图标就是“NIC Express Virtual Adapter”，这个就是绑定后的网卡组，这个网卡组的使用和使用单一网卡完全一样，相当于一个单一的“虚拟网卡”。 用鼠标在这个图标上点击右键选择属性，可以为这个“虚拟网卡”设定IP地址、子网掩码、网关等等，其实在安装“NIC Express”过程中，如果服务器原来的单一网卡已经设置了这些，那么“虚拟网卡”就会自动设定和原来的网卡一样。在这里选择“NIC Express Transport for Ethernet”还可以自由对网卡绑定组进行调整，例如，减少或者加入一块网卡，这个较之Intel PROSET要方便许多，Intel PROSET在增加或者减少绑定组里的网卡之前必须删除原来的绑定组然后重新创建。好了，好了，至此一切都已经做好，我们的服务器已经成为一台地地道道的 “双网卡冗余服务器”了。

[http://blog.163.com/handsomcyp@126/blog/static/2502114120078291460541/](http://blog.163.com/handsomcyp@126/blog/static/2502114120078291460541/)

LINUX下：

一、修改网卡配置文件

::::::::::::::

/etc/sysconfig/network-scripts/ifcfg-bond0

::::::::::::::

**DEVICE=bond0**

**IPADDR=192.168.100.199**

**NETWORK=192.168.100.0**

**NETMASK=255.255.255.0**

**USERCTL=no**

**BOOTPROTO=none**

**ONBOOT=yes**

::::::::::::::

/etc/sysconfig/network-scripts/ifcfg-eth0

::::::::::::::

# Intel Corporation 82545EM Gigabit Ethernet Controller (Copper)

**DEVICE=eth0**

**USERCTL=no**

**ONBOOT=yes**

**MASTER=bond0**

**SLAVE=yes**

**BOOTPROTO=none**

::::::::::::::

/etc/sysconfig/network-scripts/ifcfg-eth1

::::::::::::::

# Intel Corporation 82545EM Gigabit Ethernet Controller (Copper)

**DEVICE=eth1**

**USERCTL=no**

**ONBOOT=yes**

**MASTER=bond0**

**SLAVE=yes**

**BOOTPROTO=none**

::::::::::::::

# DEVICE=物理设备名   
IPADDR=IP地址    
NETMASK=掩码值    
NETWORK=网络地址    
BROADCAST=广播地址    
GATEWAY=网关地址    
ONBOOT=[yes|no]（引导时是否激活设备）    
USERCTL=[yes|no]（非root用户是否可以控制该设备）    
BOOTPROTO=[none|static|bootp|dhcp]（引导时不使用协议|静态分配|BOOTP协议|DHCP协议）    
HWADDR = 你的MAC地址

有的参数并非必需。

http://blog.chinaunix.net/uid-9967220-id-1995610.html

还有其实参数，比如PEERDNS=[yes|no],如果是通过DHCP获取IP，是否要将DNS写入/etc/resolv.conf,应该会覆盖原来的内容！

Max_bonds=2如果绑定多个网卡，最好加上这个参数，否则可能在网络重启后会提示找不到BOND1之类的消息！

二、修改系统模块组件

1、在/etc/modprobe.conf添加如下信息：

alias bond0 bonding

options bond0 miimon=100 mode=1

上面的options的配置可以写在/etc/sysconfig/network-scripts/ifcfg-bond0：

BONDING_OPTS="mode=1 miimon=100"

这个options里还可以加primary=eth1，指定默认主网卡，默认应该是eth0

如果系统存在多块网卡，eth0，eth1，eth2,eth3，我们现在希望两两做备份，这样在系统就可以配置多个bonding了，需要在加载bonding的时候加载一个参数，max_bonds，比如：
modprobe bonding max_bonds=2

我们可以加入
alias bond0 bonding

   options bonding mod=blance-rr miimon=100 max_bonds=2

   alias bond1 bonding

   options bonding mod=active-backup miimon=100 primary=eth2 max_bonds=2

多个bonding，最好加上这个参数，否则可能在网络重启后会提示找不到BOND1之类的消息！

2、测试系统模块配置信息的正确性

# modprobe bonding

3、重启network服务或者重启电脑，使配置生效

# service network restart或者reboot

4、核实配置的正确性

# less /proc/net/bonding/bond0

5、查看网卡状态及相关信息

mii-tool

ethtool eth0/eth1

三、如果网卡参数里不写MASTER=bond0和SLAVE=yes，就要执行

在/etc/rc.d/rc.local 加入如下一行（启动时执行此行命令）   
  ifenslave bond0 eth0 eth1

如果写了的话，这一步就可有可无了！

[http://www.doc88.com/p-119695623633.html](http://www.doc88.com/p-119695623633.html)

引用：

modprobe.conf配置文件中的解释：

miimon是用来进行链路检测的。比如miimon=100，那么系统每隔100毫秒就检测一次链路的 链接状态，如果一条线路不通就转入另外一天链路

mode的值表示bonding的工作模式，一个有0、1、2、3、4、5、6七种模式，常用的有0、1两种，可以根据工作模式适当的选择：

0、（balance-rr）Round-robin policy：（平衡轮循环策略）：传输数据包的顺序是一次传输，知道最后一个传输完毕，此模式提供负载均衡和容错能力（这个好像要交换机支持，不知道什么意思）, 在这种情况下，带宽理论上是双倍的，断开一个网卡只是带宽减少了

1、（active-backup）Active-backup policy：（主-备策略）：只有一个设备处于活动的状态。一个如果宕掉了则立马有备份转移到另外一个主设备。mac地址是外部可见的。这个模式提供了容错能力，在这种情况下，ETH1作为备份，是NO ARP状态的，可以使用ifconfig查看到，只有ETH0断开，它才会接管

2、（blance-xor）XOR policy：（广播策略）：将所有数据包传输给所有接口。此模式提供了容错能力

3、（blance-xor）XOR policy：（平衡策略）：传输根据源地址布尔值选择传输设备。此模式提供负载平衡和容错能力

4、（802.3 ad）IEEE 802.3ad Dynamic link aggregation：（IEEE 802.3ad动态链接聚合）：创建共享相同的速度和双工设置的聚合组

5、（balance-tlb）Adaptive transmit load balancing：（适配器传输负载均衡）：没有特殊策略，第一个设备传不通就用另一个设备接管第一个设备正在处理的mac地址，帮助第一个进行传输

6、（balance-alb）Adaptive load balancing：（适配器传输负载均衡）：大致意思是包括mod5，bonding驱动程序截获ARP在本地系统发送的请求，用其中之一的硬件地址覆盖从属设备的原地址。就像是服务器上不同的人使用不同的硬件地址一样（这个好像不用交换机支持）

[http://blog.sina.com.cn/s/blog_67be3b450101dew5.html](http://blog.sina.com.cn/s/blog_67be3b450101dew5.html)

下面这里也有介绍

[http://wenku.baidu.com/view/4acd9f3b0912a216147929aa.html](http://wenku.baidu.com/view/4acd9f3b0912a216147929aa.html)



停止bondingifenslave -d bond0 eth0

   ifenslave -d bond0 eth1

   ifconfig bond0 down

   modprobe -r bonding
[http://os.it168.com/a2008/0331/978/000000978427.shtml](http://os.it168.com/a2008/0331/978/000000978427.shtml)





对于双网卡双IP的那种要求，可能要手机加一条路由就可以了，因为不加的话有一个网段应该会无效的，这个网上也有，根据自己的情况设置吧，不在这篇文章的范围内！

查看这里http://mcmvp.blog.51cto.com/5497438/1266728









