# vSphere可用性之三准备实验环境 - weixin_33985507的博客 - CSDN博客
2010年09月07日 10:26:41[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
第三章 准备实验环境
  在上篇内容中，讲述了进行VMware HA实验所必需的软硬件条件。接下来将使用这些来搭建实验环境。主要内容为依据拓扑图安装ESX主机系统、ISCSI存储系统。
此次实验环境的建置分为以下四个方面来进行：
1、 存储部分
2、 ESX主机部分
3、 管理部分
4、 网络配置部分
一、拓扑说明
如下图所示中，以上三个部分的内容已展示的很清楚，图中的存储和ESX主机部分分别在DELL 存储及硬件服务器进行配置及安装。
![clip_image002](http://rickyfang.blog.51cto.com/attachment/201009/7/1213_1283826389SPXC.jpg)
二、软件及操作系统在硬件上的相关放置如下：
1、Dns 角色
无论是生产环境，还是测试环境，如果需要实现VMware的高可用性，也就是VMware HA均需要正确的名称解析来做保证的，一般如果企业存在有微软的AD域基础架构，直接可以手工的方式在DNS服务器角色（有可能集成在DC中）中新建正向解析及反向解析A记录的。 也就是说，承担vCenter Server角色的机器与两台ESX主机之间能互相解析得到。
如果没有DNS或是不想使用DNS服务，需要在vCenter Server角色的机器与两台ESX主机上分别建立相应的IP地址与机器名的对应关系。也就是HOSTS记录。
在上述拓扑中已实现了DNS解析功能，且正反均正常工作。
DNS角色在Windows server 2003 Ent With sp2的系统上实现。
DNS名称：it168test.com
IP Add：192.168.1.2
2、vCenter Server角色
vCenter Server是配置、置备和管理IT环境的中央点，通过此可以建立数据中心及群集，从而实现HA、DRS、FT等功能。（同DNS角色安装在同一台机器上，建议在实际生产环境中分开单独安装）
vCenter Server角色在Windows server 2003 Ent With sp2的系统上实现。
Netbios名称：Win2k3VC.it168test.com
IP Add：192.168.1.2
3、 ESX 主机角色
DELL R710服务器在设计上就是支持虚拟化技术的，不但支持VMware平台的虚拟化技术解决方案，还支持Microsoft的虚拟化解决方案。兼容性不存在问题，且在内存扩展及与存储连接方面都有很好的解决。此动手实验中的两台物理服务器的硬盘均做了RAID 5。
两台ESX主机均是ESX 4.0 UPDATE1的版本。
其中一台ESX主机：
Netbios名称：Esxone.it168test.com
IP Add：192.168.1.10
其中另一台ESX主机：
Netbios名称：EsxTwo.it168test.com
IP Add：192.168.1.20
4、虚拟机：
操作系统：Windows server 2003 Ent With sp2
IP Add:192.168.1.31
三、接下来介绍如何安装ESX主机、配置存储、管理机器：
1、如何安装ESX主机
VMware ESX4.0 U1的安装较为简单，如果有过LINUX安装经验的话，基本上都能够较为顺利的实现，此处并不作为重点说明。
但需要注意
**注意：由于会使用到共享存储，在进行ESX****主机安装时，请改变ESX****主机所分配硬件的大小，一般为8G****便可以。**
**注意：此时的ESX****主机均有四张千M****网卡。且DELL****的ISCSI****存储连接此ESX****主机是通过光纤卡（FCOE****）来连接ESX****主机的。要想完成VMotion****的测试，这些是必不可少的条件（尽管整个实验场景是以VMwareHA****为主）。在接下来的添加存储至主机里会有相关Service Console****及VMKERNEL****的安装建议的（关于虚拟交换及网络）。**
2、如何安装ISCSI存储
一般情况下，大家是不太可能有专属的NAS存储或是FC SAN存储来完成这些实验的，经济条件的限制导到致了很多朋友在学习一些技术时忘而却步。好在IT领域的开源及分享精神的发扬光大，让我们有了使用免费的如OpenFiler来做为ESX提供ISCSI共享存储的机会，更让我们有了在自己有限的硬件上完成VMware HA实验的可能。
OpenFiler的新的版本为2.3，和之前2.2版本相比有了一些改变。大家可以根据自己的喜欢及目前的技术能力来选择安装。此处仍不提供如何安装的方法，请参考如下链接：
[Guide to installing OpenFiler Virtual Shared Storage](http://knowledge.xtravirt.com/index.php?option=com_remository&func=download&id=17&chk=c4f10723d7ab2fe7e2ceb1914c9a4308&no_html=1)
这个链接所提供的文章内容是基于OpenFiler2.2版本的，大家完全可以参考使用，根据我使用2.3版本的经验来看，在添加用于共享的存储盘时，可以使用动态磁盘类型。
在本次动手实验中使用的是DELL EQL PS6010的存储，并在搭建整个虚拟化基础架构过程中，得到了DELL技术工程师的现场技术支持，在讨论了整个架构后，帮助设计了存储的容量规划，以及在后面章节中将通过其进行的存储间的复制和手动VMOTION（改变存储位置）。
DELL EQL PS6010在管理方面具有不错的特点如：
- PS组管理器
- SAN HeadQuarters多SAN历史性能监控
- 快速配置
- 远程安装向导
- 基于角色的管理
- 卷管理
下图就展示了通过IE浏览器（需在此机器上安装管理插件）通过网络的方式远程进行管理的，以上的特点在使用过程中均得到了体验。
![clip_image004](http://rickyfang.blog.51cto.com/attachment/201009/7/1213_1283826392c0jI.jpg)
而在本次动手实验中，最关注的还是它在与虚拟化相结合方面的表现：
||- 多路自动复制- 即时卷还原- 多路径I/O[1]()- 多卷、可写快照- 快照和自动复制调度程序- 卷克隆- 卷一致性组|- 多路自动复制- 即时卷还原- 多路径I/O[1]()- 多卷、可写快照- 快照和自动复制调度程序- 卷克隆- 卷一致性组|
|----|----|----|
|- 多路自动复制- 即时卷还原- 多路径I/O[1]()- 多卷、可写快照- 快照和自动复制调度程序- 卷克隆- 卷一致性组| | |
以上的特点，加上vShpere的一些功能支持，让这次进行企业功能实验非常顺利的实现，尤其是在卷克隆及复制方面。
下面的连接就是关于DELL EQL PS6010存储管理及卷复制功能实现的一些视频，可以通过此了解下：**EQLreplica**
如何在DELL EQL PS6010上，创建卷，划分LUN呢？这一步很关键，据生产经验来看，一般一个LUN不要太大，需要好好规划，一般里面放置16个左右的VMDK的文件便可。而如果是SQL等对性能要求较高的应用跑在虚拟化，建议专门分在一个LUN上，且此硬盘最好是光纤盘等。通过下面的视频可以了解到如何划分LUN以及如何在WINDOWS平台下挂载：**eql_luncreate**
3、安装vCenter Server及vSphere Client
安装vCenter Server也很简单，不像ESX那样是LINUX平台，就是一个应用程序，安装在服务器的角色中便可。
在选择安装组件时，把Update Manager和Licence Server也一并装上便可。
安装过程中需要输入登陆访问vCenter Serve的授权账号。可不同于域及本机的本地账号。
安装后的截图如下：
![clip_image006](http://rickyfang.blog.51cto.com/attachment/201009/7/1213_1283826394dPli.jpg)
vCenter Server安装完后，如何才能使用呢？需要在IE地址栏里输入[https://VC-IP](https://VC-IP),然后在打开的页面中下载vSphere Client。
![clip_image008](http://rickyfang.blog.51cto.com/attachment/201009/7/1213_1283826397RbhT.jpg)
下载安装便可，也可以直接从ESX主机上下载使用。方法和从vCenter Server一样。而且根据本机的系统不同，下载的语言版不同。我下载的是中文简体版的。
vSphere Client可以安装在vCenter Server上，也可以安装在另外一台同一子网内的机器上（建议不要跨路由）。
vCenter Server及vSphere Client均可以安装在ESX主机上(相对ESX主机来说或者相对于vSphere来说，安装在ESX主机上的vCenter Server和vSphere Clientt则是虚拟机啦)。这取决于你的规划及部署。
4、网络配置说明
为了配合HA及VMotion的实验，更为了贴近生产环境，这次在选择DELL服务器，也是考虑到硬件服务器支持多块网卡，同时又支持FCOE的光纤卡。
每台服务器上有四块千M网卡：
![clip_image010](http://rickyfang.blog.51cto.com/attachment/201009/7/1213_1283826399CC6C.jpg)
每台服务器上的建两个虚拟交换机，其中vmnic0与vmnic1用于service console和VM网络。Vmnic2与vmnic3用于VMKernel。
注意：在实际生产中，建议把service console和vmkernel位于同一个虚拟交机上。把vm网络单独位于一个虚拟交换机上，同时在其上建置多个VLAN。
注意：如果在ESX主机上的VM网络所连接的物理交换机上同样设置了VLAN,需要开启其连接端口的TRUANK功能，以便能将不同VLAN的VM的网络流量分流到相应的物理交换机的VLAN里。
![clip_image012](http://rickyfang.blog.51cto.com/attachment/201009/7/1213_1283826401kCRw.jpg)
