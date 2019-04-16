# 实战VMware的三种网络模式 - 菜鸟路上的小白 - CSDN博客





2016年09月24日 10:34:46[somnus_小凯](https://me.csdn.net/u012486840)阅读数：9096
个人分类：[java](https://blog.csdn.net/u012486840/article/category/6382670)









## 一、实验目的


最近在使用VMware搭建虚拟网络环境时遇到了很多问题，经过对VMware网络模式的一番学习和实战，总算是对其有了一定的认识。所以决定完成一次比较完整的针对VMware网络配置的实验，并写下这篇博客记录整个实验的过程。在进行虚拟机实验时，我们往往关注下面这三个问题：
- 虚拟机和主机能不能互相访问
- 虚拟机能不能访问Internet
- 外部网络如何访问虚拟机中的服务


我们会通过后面的实验分别利用VMware的不同网络模式来解决这三个问题。VMware提供了三种不同的网络模式，分别是`Bridged`(桥接模式)、`NAT`(网络地址转换模式)、`Host-only`(主机模式)。不同的网络模式适用于不同的网络环境，实现不同的需求，你需要根据实际情况选择合适的模式。
 就网络环境来说，我们常见的家庭网络拓扑结构有下面两种：
- 主机通过拨号直接连接Internet
- 主机处于局域网环境中，通过路由器拨号连接Internet


如果你是属于第一种网络环境，由于是ISP分配你的公网IP（假设只有一个地址），则不能使用桥接模式，因为桥接模式需要你拥有属于你机器相同网段内的另一个IP地址。这种情况下可以使用`NAT`和`Host-only`。而如果是属于第二种网络环境，则三种模式可以任意选用。
 就需求来说，你可能只是想简单配置一个虚拟机来玩玩，也可能是为局域网内的其他机器提供服务，或者是进行一些特殊目的的网络实验，这个就要具体情况具体对待了。

## 二、实验环境


本次实验软硬件环境如下：
- 操作系统：Windows 7 旗舰版（32位）
- CPU：Intel(R) Core(TM) i3
- 内存：4G
- 虚拟机：VMware® Workstation 8.0.0 build-471780


网络环境为家庭局域网，由路由器通过PPPOE拨号上网，如下图所示：

![network-basic](https://img-blog.csdn.net/20160924103233856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 三、实验步骤


了解了我们的需求和网络环境之后，就开始我们的实验之旅吧。首先是下载并安装VMware Workstation，然后下载虚拟机上需要的操作系统镜像，并安装到虚拟机中。我这里创建了三个虚拟机，分别是Windows XP、Ubuntu和Backtrack操作系统。

### 3.1 桥接模式


桥接模式是三种模式中最简单的一种，VMware安装的时候默认就使用这种配置方式。在这种模式下，虚拟机相当于局域网中的一台独立机器，和主机处于同一个网段，公用同一个网关。桥接模式使用的是虚拟机的VMnet0网卡，一般情况下，在虚拟机中将网络设置成自动获取IP就能直接联网。示意图如下：

![bridge](https://img-blog.csdn.net/20160924103245693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在桥接模式下，虚拟机和主机可以互相ping通，虚拟机可以访问Internet，虚拟机上的服务也可以通过虚拟机IP地址在本机直接访问，如果有任何问题，可以按下面的步骤检查各个配置：
- 检查本地连接的属性里，是否勾选了`VMware Bridge Protocol`，如果没勾选上，则虚拟机和本机不能互相ping通，如下图：

![bridge-protocol](https://img-blog.csdn.net/20160924103258606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 检查虚拟机的IP地址，看是否和本机处于同一个网段内，如果不是，可以手工设置下IP地址，网关和DNS服务器配置可以参考本机，和本机一样即可
- 检查本机防火墙和虚拟机防火墙

### 3.2 NAT模式


上面也说了，如果你不在局域网内，只有一个IP，那么NAT模式正适合你。当然如果你在局域网内，NAT模式也未尝不可，不过使用NAT模式后，主机就变成了双网卡：本身的网卡连接Internet或连接拨号的路由器，另一个虚拟网卡VMnet8连接由虚拟机组成的一个虚拟网络。从外部网络来看，无法直接访问这个虚拟网络。虚拟网络则通过本机上的NAT虚拟服务器进行转发访问Internet。示意图如下：

![nat](https://img-blog.csdn.net/20160924103310911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


NAT模式是让虚拟机实现访问Internet最快的方式，几乎不用任何配置，只要主机能上网，那么虚拟机也就肯定能上网。如果有任何问题，可以通过下面的步骤进行排查：
- 检查主机上VMware的NAT服务和DHCP服务是否开启，如下图：

![nat-service](https://img-blog.csdn.net/20160924103325240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 检查虚拟机的IP地址，是否和虚拟机NAT配置的Subnet Address在同一个网段内，选择Edit -> Virtual Network Editor可以看到NAT的配置信息
- 检查主机和虚拟机的防火墙设置 默认情况下，NAT配置好之后，主机和虚拟机之间应该可以互相访问，虚拟机也可以借助主机上的NAT访问Internet，但是外部网络无法访问虚拟机上的服务。如果需要让同一个局域网内的其他机器（譬如：192.168.0.100）访问虚拟机上的WEB服务，可以通过NAT的端口转发（Port Forwarding）功能来实现，如下图。具体的配置细节可以看[参考链接](http://firefore.com/vmware-2.html)。

![nat-port-forwarding](https://img-blog.csdn.net/20160924103339498?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 3.3 Host-only模式


Host-only模式和NAT一样，也相当于主机双网卡，网络拓扑和NAT也是一样，只是主机不提供NAT功能了，所以虚拟网络只能和主机访问，不能访问Internet。如果需要一个完全隔离的网络环境，则Host-only最合适不过。Host-only相当于使用双绞线直接连接虚拟机和主机，这是最原始的网络结构，当然也是最灵活的。这种情况下虚拟机就不能访问Internet了吗？局域网下的其他机器就不能访问虚拟机上的服务了吗？当然不是。如果我们自己在主机上搭建起我们自己的NAT服务和DHCP服务，那么Host-only其实和NAT是一样的。从下面的示意图也可以看出，Host-only和NAT的唯一区别就在于，主机上少了NAT这个部分。

![host-only](https://img-blog.csdn.net/20160924103352748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


类似于NAT，具体的配置这里略过。下面通过Windows上的ICS服务（Internet Connection Sharing，Internet连接共享）来实现Host-only模式的虚拟机访问Internet。ICS是Windows上的一种共享网络访问的服务，类似于mini版NAT，提供了NAT地址转换和DHCP的功能，但不支持端口转发（Port Forwarding）。 首先在网络连接里找到当前正在使用的连接，选择属性 -> 共享，选中“允许其他网络用户通过此计算机的Internet连接来连接”，然后在网络连接下拉框中选择Host-only对应的虚拟网卡（这里是VMnet1），如下图：

![ics](https://img-blog.csdn.net/20160924103404530?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在确定的时候，可能会弹出对话框提示错误：“Internet连接共享访问被启用时，出现了一个错误（null）”，这时去服务中找到Windows Firewall，启动即可。 ICS配置好之后，Host-only就和NAT一样了，在虚拟机中设置自动获取IP或手工设置IP，保证和VMnet1处于同一个网段内，如果一切顺利，就可以在虚拟机中访问Internet了。

## 四、总结


通过这次的实验，重新学习并巩固了计算机网络的相关知识。特别是`NAT`和
```
Port
 Forwarding
```
，是网络管理中很重要的手段。虽然模式各不相同，但是在局域网环境下，通过各种技术手段，最终都能实现相同的目的：虚拟机和主机互相访问（三种模式都可以），虚拟机访问Internet（Bridged和NAT直接可以访问，Host-only通过ICS也可以），外网访问虚拟机上的服务（Bridged直接访问，NAT通过端口转发也可以，Host-only通过架设自己的NAT服务也应该可以）。 本次实验环境比较简单，也没有考虑双网卡的情况，可以参考下面的链接进一步研究。

## 参考
- [简单区分Vmware的三种网络连接模式(bridged、NAT、host-only)](http://www.slyar.com/blog/vmware-bridged-nat-hostonly.html)
- [Vmware虚拟机下三种网络模式配置](http://blog.csdn.net/collection4u/article/details/14127671)
- [VMWare虚拟机 网络连接模式](http://lilinji.blog.51cto.com/5441000/1264307) [汇总帖]
- [VMware虚拟机上网络连接（network
 type）的三种模式--bridged、host-only、NAT](http://www.cnblogs.com/xiaochaohuashengmi/archive/2011/03/15/1985084.html) [汇总帖]
- [VMWare虚拟机下为Ubuntu
 12.04.1配置静态IP（NAT方式）](http://www.cnblogs.com/objectorl/archive/2012/09/27/vmware-ubuntu-nat-static-ip-settings.html)
- [VMware
 Workstation虚拟机网络连接杂记、给Windows虚拟机配置固定IP](http://www.cnblogs.com/zhujian/archive/2012/06/14/2550033.html)
- [解决Windows 7/win8 使用VMware虚拟机的NAT
 不能上网](http://www.blogfeng.com/vmware-nat.html)
- [使用
 ICS（Internet 连接共享）](http://windows.microsoft.com/zh-cn/windows/using-internet-connection-sharing#1TC=windows-7)
- [internet连接共享访问被启用时，出现一个错误](http://jingyan.baidu.com/article/d5c4b52bcb9f04da560dc59d.html)
- [VMware Workstation虚拟机实例：让外网访问虚拟机](http://firefore.com/vmware-1.html)
- [VMware Workstation实例二：单IP的虚拟机提供外网访问](http://firefore.com/vmware-2.html)



