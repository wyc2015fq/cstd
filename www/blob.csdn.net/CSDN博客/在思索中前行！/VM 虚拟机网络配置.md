# VM 虚拟机网络配置 - 在思索中前行！ - CSDN博客





2014年08月26日 23:00:09[_Tham](https://me.csdn.net/txl16211)阅读数：1200标签：[网络配置](https://so.csdn.net/so/search/s.do?q=网络配置&t=blog)
个人分类：[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)








ＶＭ网络设置，一共有四种模式。
分别是
1：bridge：桥接，直接和真实网卡相连。如果你要让虚拟机也要上网，就必须选这项，并且要配置和真实网卡在同一网段的IP地址。
2：host-only: 仅主机， 假设，你的主机没有网卡，或者网卡坏了，线没插，总之吧，真机网卡不能用，但是现在还需要虚拟机和真机通讯，此时就可以选择这项，因为在安装虚拟机的时候，虚拟机软件会自动的为系统添加这块虚拟网卡。
3.  NAT： 虚拟系统借助NAT(网络地址转换)功能，通过宿主机器所在的网络来访问公网。
4. CUSTOM: 自定义，里面有10个选项
 　 VMnet0(bridge)，和上面的1是一样的。
  　VMnet1(HOST-ONLY)，和上面的2一样的。
  　VMnet8(NAT)，和上面的3是一样。
  　剩下来的2---7还有9，我们可以理解为是几个虚拟交换机。比如说，现在你要做一个实验，开了两个虚拟机，现在只需要这两个虚拟机通讯，那么就可以把这个两个虚拟机放到一个VMNET上，比如都放到7上，就可以理解为，这两个虚拟机是插在了7号交换机上。
以上配置，可以通过点击虚拟机软件菜单中的VM----SETTING可以看到。或者，右键点击你创建好的虚拟机，点你设置（最后一项）。如果要更改，要么虚拟机是关机的，要么是开启的，不能使挂起状态。理解三种工作模式

　　VMWare提供了三种工作模式，它们是bridged(桥接模式)、NAT(网络地址转换模式)和host-only(主机模式)。要想在网络管理和维护中合理应用它们，你就应该先了解一下这三种工作模式。

1.bridged(桥接模式)

　　在这种模式下，VMWare虚拟出来的操作系统就像是局域网中的一台独立的主机，它可以访问网内任何一台机器。在桥接模式下，你需要手工为虚拟系统配置IP地址、子网掩码，而且还要和宿主机器处于同一网段，这样虚拟系统才能和宿主机器进行通信。同时，由于这个虚拟系统是局域网中的一个独立的主机系统，那么就可以手工配置它的TCP/IP配置信息，以实现通过局域网的网关或路由器访问互联网。

　　使用桥接模式的虚拟系统和宿主机器的关系，就像连接在同一个Hub上的两台电脑。想让它们相互通讯，你就需要为虚拟系统配置IP地址和子网掩码，否则就无法通信。
　　如果你想利用VMWare在局域网内新建一个虚拟服务器，为局域网用户提供网络服务，就应该选择桥接模式。

2.host-only(主机模式)

　　在某些特殊的网络调试环境中，要求将真实环境和虚拟环境隔离开，这时你就可采用host-only模式。在host-only模式中，所有的虚拟系统是可以相互通信的，但虚拟系统和真实的网络是被隔离开的。
　　提示:在host-only模式下，虚拟系统和宿主机器系统是可以相互通信的，相当于这两台机器通过双绞线互连。
　　在host-only模式下，虚拟系统的TCP/IP配置信息(如IP地址、网关地址、DNS服务器等)，都是由VMnet1(host-only)虚拟网络的DHCP服务器来动态分配的。
　　如果你想利用VMWare创建一个与网内其他机器相隔离的虚拟系统，进行某些特殊的网络调试工作，可以选择host-only模式。

3.NAT(网络地址转换模式)

　　使用NAT模式，就是让虚拟系统借助NAT(网络地址转换)功能，通过宿主机器所在的网络来访问公网。也就是说，使用NAT模式可以实现在虚拟系统里访问互联网。NAT模式下的虚拟系统的TCP/IP配置信息是由VMnet8(NAT)虚拟网络的DHCP服务器提供的，无法进行手工修改，因此虚拟系统也就无法和本局域网中的其他真实主机进行通讯。采用NAT模式最大的优势是虚拟系统接入互联网非常简单，你不需要进行任何其他的配置，只需要宿主机器能访问互联网即可。
　　如果你想利用VMWare安装一个新的虚拟系统，在虚拟系统中不用进行任何手工配置就能直接访问互联网，建议你采用NAT模式。
　　提示:以上所提到的NAT模式下的VMnet8虚拟网络，host-only模式下的VMnet1虚拟网络，以及bridged模式下的VMnet0虚拟网络，都是由VMWare虚拟机自动配置而生成的，不需要用户自行设置。VMnet8和VMnet1提供DHCP服务，VMnet0虚拟网络则不提供。
**解析虚拟VMware三种网络模式根本区别**


　　提到VMware大家就想起了虚拟机技术，虚拟机技术在最近的几年中得到了广泛的发展，一些大型网络服务商都开始采用虚拟机技术，不仅节省了投资成本，更节约了能源的消耗。

　　我们知道VMware也分几种版本，普通用户最常用的就是Workstation，但是不管使用哪种版本，我们都能发现在安装过程中让我们选择网络模式。在默认情况下会选中桥接模式，但有用户会问其他模式是干什么用的?有时候选择了其他模式之后就不能上网了。今天小编就和大家一起探讨一下VMware 3种网络模式的功能和通信规则。



![](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware001.jpg)

　　网络模式

　　我们首先说一下VMware的几个虚拟设备

　　VMnet0：用于虚拟桥接网络下的虚拟交换机

　　VMnet1：用于虚拟[Host](http://whatis.ctocio.com.cn/searchwhatis/128/5948128.shtml)-Only网络下的虚拟交换机

　　VMnet8：用于虚拟NAT网络下的虚拟交换机

　　VMware [Network](http://whatis.ctocio.com.cn/searchwhatis/367/6093367.shtml) Adepter VMnet1：Host用于与Host-Only虚拟网络进行通信的虚拟网卡

　　VMware Network Adepter VMnet8：Host用于与NAT虚拟网络进行通信的虚拟网卡

　　安装了VMware虚拟机后，会在网络连接对话框中多出两个虚拟网卡，如图：

![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware002.jpg)

一.桥接网络(Bridged [Networking](http://whatis.ctocio.com.cn/searchwhatis/395/6093395.shtml))

　　桥接网络是指本地物理网卡和虚拟网卡通过VMnet0虚拟交换机进行桥接，物理网卡和虚拟网卡在拓扑图上处于同等地位(虚拟网卡既不是Adepter VMnet1也不是Adepter VMnet8)。

![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware003.jpg)

　　桥接网络拓扑图

　　那么物理网卡和虚拟网卡就相当于处于同一个网段，虚拟交换机就相当于一台现实网络中的交换机。所以两个网卡的IP地址也要设置为同一网段。

![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware004.jpg)

　　物理网卡IP地址

![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware005.jpg)

　　虚拟网卡IP地址

　　我们看到，物理网卡和虚拟网卡的IP地址处于同一个网段，子网掩码、网关、DNS等参数都相同。两个网卡在拓扑结构中是相对独立的。

![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware006.jpg)

[ping](http://whatis.ctocio.com.cn/searchwhatis/102/6026102.shtml)结果

　　我们在192.168.15.111上ping192.168.15.96，结果显示两个网卡能够互相通信。如果在网络中存在DHCP服务器，那么虚拟网卡同样可以从DHCP服务器上获取IP地址。所以桥接网络模式是VMware虚拟机中最简单直接的模式。安装虚拟机时它为默认选项。



二.NAT模式

　　在NAT网络中，会用到VMware [Network](http://whatis.ctocio.com.cn/searchwhatis/367/6093367.shtml) Adepter VMnet8虚拟网卡，主机上的VMware Network Adepter VMnet8虚拟网卡被直接连接到VMnet8虚拟交换机上与虚拟网卡进行通信。

![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware007.jpg)

NAT网络模式

　　VMware Network Adepter VMnet8虚拟网卡的作用仅限于和VMnet8网段进行通信，它不给VMnet8网段提供路由功能，所以虚拟机虚拟一个NAT服务器，使虚拟网卡可以连接到Internet。在这种情况下，我们就可以使用端口映射功能，让访问主机[80](http://whatis.ctocio.com.cn/searchwhatis/293/5949293.shtml)端口的请求映射到虚拟机的80端口上。

　　VMware Network Adepter VMnet8虚拟网卡的IP地址是在安装VMware时由系统指定生成的，我们不要修改这个数值，否则会使主机和虚拟机无法通信。

![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware008.jpg)

　　NAT虚拟网卡IP地址

![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware009.jpg)

　　物理网卡IP地址

　　虚拟出来的网段和NAT模式虚拟网卡的网段是一样的，都为192.168.111.X，包括NAT服务器的IP地址也是这个网段。在安装VMware之后同样会生成一个虚拟DHCP服务器，为NAT服务器分配IP地址。

　　当主机和虚拟机进行通信的时候就会调用VMware Network Adepter VMnet8虚拟网卡，因为他们都在一个网段，所以通信就不成问题了。

　　实际上，VMware Network Adepter VMnet8虚拟网卡的作用就是为主机和虚拟机的通信提供一个接口，即使主机的物理网卡被关闭，虚拟机仍然可以连接到Internet，但是主机和虚拟机之间就不能互访了。



三.[Host](http://whatis.ctocio.com.cn/searchwhatis/128/5948128.shtml)-Only模式

　　在Host-Only模式下，虚拟网络是一个全封闭的网络，它唯一能够访问的就是主机。其实Host-Only网络和NAT网络很相似，不同的地方就是 Host-Only网络没有NAT服务，所以虚拟网络不能连接到Internet。主机和虚拟机之间的通信是通过VMware [Network](http://whatis.ctocio.com.cn/searchwhatis/367/6093367.shtml) Adepter VMnet1虚拟网卡来实现的。



![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware010.jpg)

　　Host-Only模式

　　同NAT一样，VMware Network Adepter VMnet1虚拟网卡的IP地址也是VMware系统指定的，同时生成的虚拟DHCP服务器和虚拟网卡的IP地址位于同一网段，但和物理网卡的IP地址不在同一网段。



![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware011.jpg)

　　Host-Only虚拟网卡IP地址



![VMware](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmware012.jpg)

　　物理网卡IP地址

　　Host-Only的宗旨就是建立一个与外界隔绝的内部网络，来提高内网的安全性。这个功能或许对普通用户来说没有多大意义，但大型服务商会常常利用这个功能。如果你想为VMnet1网段提供路由功能，那就需要使用RRAS，而不能使用XP或2000的ICS，因为ICS会把内网的IP地址改为 192.168.0.1，但虚拟机是不会给VMnet1虚拟网卡分配这个地址的，那么主机和虚拟机之间就不能通信了。

综述

　　在VMware的3中网络模式中，NAT模式是最简单的，基本不需要手动配置IP地址等相关参数。至于桥接模式则需要额外的IP地址，如果是在内网环境中还很容易，如果是ADSL宽带就比较麻烦了，ISP一般是不会大方的多提供一个公网IP的。


** VMware虚拟机网络详解(包括多网卡) **


### 大多数人用虚拟机都是单网卡，联网设置比较简单，一旦用到多网卡又蒙了不知道怎么设置，下面这篇文章是Vmware虚拟机包括多网卡的网络设置，供大家参考一下。主机为WindowsXP系统，VMware虚拟机里安装Linux操作系统。


安装完VMware后，主机的网络连接里面出现了两个新的连接，即VMware Network Adapter VMnet1和VMware Network Adapter VMnet8如图。虚拟机里安装的系统就是靠这两个虚拟网卡来联网的。（如下图）
主机上类似antiarp 防火墙一定要关掉 否则虚拟机不能上网。

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest001.jpg)


有以下几种可选方法：
１、Bridged（网桥）方式：
如果主机是通过局域网上网的，可让虚拟机使用与主机网段、网关和ＤＮＳ等的相同的设置，选用一个有效的局域网ＩＰ即可。

桥接方式可实现虚拟机主机 虚拟机互联网 通信（主机与虚拟机可看成是同一局域网的两台机）。但桥接需要一个不同于 原主机上的另外一个独立的IP地址，这对于有些情况（IP和MAC绑定，网管不同意），比较难办到。并且主机拔掉网线后，虚拟机无法与主机通过网络的方式进行通讯。而后面几种没这个问题。所以不太使用桥接方式。

1)主机的IP信息

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest002.jpg)

2）设置VMware网络连接为Bridged

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest003.jpg)

3）虚拟机系统网络设置

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest004.jpg)

**多网卡桥接**
当我有双网卡时，虽然VMnet0:Bridged to an automatically chosen adapter（在VMware的Edit ->Virtual Network settings... 打开Virtual Network Editor）

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest005.jpg)

但是，我尝试了一下，虚拟机只可桥接“本地连接”（VIA网卡），不能桥接“本地连接2”（Realtek网卡）

在Virtual Network Editor -> Automatic Bridging 选项卡里

若我们想桥接“本地连接2”（Realtek网卡）需要在“Exclude adapters" 里add "VIA网卡”（本地连接），这样“本地连接2”就可以桥接了。但是“本地连接”（VIA网卡）却不可以了，即使在“Exclude adapters" 里Remove "VIA网卡”也不行，我猜测 VMware其实只支持一个最初未被排除的网卡桥接！

没关系，我们可以在 Virtual Network Editor -> Host Virtual Network Mapping 选项卡里，VMnet2选择Realtek网卡
![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest006.jpg)

这时，我们回到Virtual Network Editor -> Automatic Bridging 选项卡，发现VMnet0 把 Realtek网卡排除了

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest007.jpg)

桥接“本地连接”（VIA网卡）时，VMware选择VMnet0，

桥接“本地连接2”（Realtek网卡）时，VMware选择VMnet2
![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest008.jpg)

虚拟机上网络相应的设置，就可以了。


２、ＮＡＴ方式：
使用VMware提供的NAT和DHCP服务，虚拟机使用主机中的虚拟网卡VMnet8作为网关，并且TCP/IP设置需遵循程序中关于vmnet8的 DHCP和NAT设置，如IP必须在其DHCP设置的StartIP和EndIP之间，网关需使用其NAT设置的Gateway IP。
NAT方式可实现虚拟机主机 通信,但是 虚拟机--->互联网 即只能够虚拟机访问互联网，但是若想把虚拟机作为服务器，让互联网访问，不可以。
但是如果有多个网卡，虚拟机可以不用更改设置，就可以同时访问多个网卡连接的互联网。
1） 设置VMware 网络连接方式为NAT
![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest009.jpg)

2）在虚拟机里，linux网络设置，只需选择自动获取IP，就可以了！

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest010.jpg)

当然也可以手动设置:
ip设置与vmnet8同网段,gateway，netmask，broadcast设置与vmnet8相同,dns设置与主机相同。
我们在主机的网络连接里如下，没有看到网关（如下图）
![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest011.jpg)

别急，在VMware的Edit ->Virtual Network settings... 中，有相关内容。

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest012.jpg)

这些选项卡包含了VMware的很多高级设置，有兴趣可以研究研究。


3.host-only方式 :
默认使用vmnet1，将虚拟机ip设置与vmnet1同网段,gateway设置成vmnet1的ip,其余设置与vmnet1相同,dns设置与主机相同
例如 vmnet1 ip :192.168.101.1 
虚拟机设置: ip :192.168.101.100 gateway : 192.168.101.1 
这样就实现了虚拟机主机 通信,但是 虚拟机互联网 仍无法通信
1）选择VMware的Host-only
![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest013.jpg)

2）主机的本地连接

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest014.jpg)

3）在虚拟机linux里，设置网络

4、共享主机的Internet连接的方式（如果主机是Ｗｉｎ９８系统可能不适用）：
在主机的“本地连接”属性的高级里面共享Internet连接。如果选的是VMnet1虚拟网卡，则在VMware的Ethernet设置需选用Host- only；如果选VMnet8，则Ethernet设置需选用NAT。共享连接后，那个被选中的虚拟网卡的ＩＰ会默认被设为192.168.0.1（如果主机也是通过局域网的192.168.0.1网关上网的，则需把这个虚拟网卡设置为不同的网段ＩＰ，如192.168.1.1）这时候虚拟机里面就可以通过把这个虚拟网卡作为网关来上网了。实际上是一个简单的路由NAT，所以NAT方式就不讨论了，我们只以Host-only为例。
共享host-only方式，可实现虚拟机主机 通信,但是 虚拟机--->互联网 即只能够虚拟机访问互联网，但是若想把虚拟机作为服务器，让互联网访问，不可以。
1）选择VMware的Host-only

2）设置主机的本地连接属性
![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest015.jpg)

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest016.jpg)

被选中的虚拟网卡的ＩＰ会默认被设为192.168.0.1，当然也可以更改。而且共享连接取消后，还会变回到共享连接前的网络设置。

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest017.jpg)

在虚拟机linux里，设置网络：

![VMware虚拟机网络详解(包括多网卡) （转） - langxingdemeili@126 - 狼性的魅力，何在？](http://images.cnblogs.com/cnblogs_com/xiaochaohuashengmi/243888/o_vmbest018.jpg)

总结：

共享连接方式和NAT方式，都可以实现虚拟机主机 ， 虚拟机--->互联网 通信，NAT方式设置简单，且可以实现同时访问多个物理网卡。
桥接方式可实现虚拟机主机 虚拟机互联网 通信，若虚拟机作为服务器，只有这种方式可行。但需独立的IP给虚拟机，且若物理网卡连接断开，虚拟机与主机也不能连接。
host-only方式只实现虚拟机主机 通信 ，一般不单独采用。

以上是VMware虚拟机网络设置包括多网卡，通过本文可以实现虚拟机网络畅通无阻，从而方便大家更好的用虚拟机。





