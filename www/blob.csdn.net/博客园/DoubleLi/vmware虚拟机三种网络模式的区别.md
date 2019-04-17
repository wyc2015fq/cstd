# vmware虚拟机三种网络模式的区别 - DoubleLi - 博客园






首先安装了VMware，在其中安装了[Ubuntu系统](https://www.baidu.com/s?wd=Ubuntu%E7%B3%BB%E7%BB%9F&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，正常启动之后开始考虑怎么才能够让主机和虚拟机实现网络互连并且由主机向虚拟机发送文件，通过在网上查阅相关资料，记录学习笔记如下。

学习参考资料：

[linux虚拟机](https://www.baidu.com/s?wd=linux%E8%99%9A%E6%8B%9F%E6%9C%BA&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)中和主机三种网络连接方式的区别

[http://blog.csdn.net/xh16319/article/details/17272113](http://blog.csdn.net/xh16319/article/details/17272113)

虚拟机三种网络模式&桥接模式设置步骤

http://blog.csdn.net/to_baidu/article/details/52593380

VMware NAT模式下设置网络

[http://www.cnblogs.com/kanyun/p/8366040.html](http://www.cnblogs.com/kanyun/p/8366040.html)

winscp连接vmwareubuntu

[http://www.cnblogs.com/supertang/p/4097030.html](http://www.cnblogs.com/supertang/p/4097030.html)

VMware下网络配置三种模式对比（桥接模式|主机模式|网络地址转换）

[http://www.chinastor.com/a/xunihua/vmware/02063N962018.html](http://www.chinastor.com/a/xunihua/vmware/02063N962018.html)





[VMware虚拟机](https://www.baidu.com/s?wd=VMware%E8%99%9A%E6%8B%9F%E6%9C%BA&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)和主机连接有以下三种方式：

一、桥接模式（bridged）

![](https://img-blog.csdn.net/20180311190615965?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM5NDkwNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



1. 该模式下物理网卡和虚拟网卡的IP地址处于同一个网段，子网掩码、网关、DNS等参数都相同。

2.本地物理网卡和虚拟网卡通过VMnet0[虚拟交换机](https://www.baidu.com/s?wd=%E8%99%9A%E6%8B%9F%E4%BA%A4%E6%8D%A2%E6%9C%BA&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)进行桥接，虚拟机和主机位于VMnet0这个[虚拟交换机](https://www.baidu.com/s?wd=%E8%99%9A%E6%8B%9F%E4%BA%A4%E6%8D%A2%E6%9C%BA&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的两端，这个交换机使得两端处在同一个网段中。

3.VMware中的每个虚拟系统相当于一台独立的主机，桥接模式下主机和虚拟机相当于同一个局域网中的两台电脑，虚拟机和宿主机处于同等地位。这种情况下主机和虚拟机要能够通讯，需要在同一个网段，这就需要手动配置虚拟系统的IP地址和子网掩码，保证虚拟机和宿主机处在同一网段中。虚拟机作为一台独立的主机可以与局域网中的任何一台电脑通信。

虚拟系统桥接模式实现网络[连接设置](https://www.baidu.com/s?wd=%E8%BF%9E%E6%8E%A5%E8%AE%BE%E7%BD%AE&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)步骤：

1.在网络连接中设置本地连接（在此为无线网）属性，右键单击选择“属性”，在“VMware bridge protocol”前面打钩。

![](https://img-blog.csdn.net/20180311190734434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM5NDkwNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.打开VMware，在菜单栏中选择“虚拟机”à“设置”，在以下页面中右侧网络连接一栏选择“桥接”。

![](https://img-blog.csdn.net/20180311190753776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM5NDkwNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3.VMware菜单栏选择“编辑”à“虚拟网络编辑器”，在打开的页面中点击右下方“更改设置”，这时页面中会出现VMnet0，在VMnet信息一栏中点击选择“桥接模式”，“桥接到”后面的选项选择宿主机物理网卡的名字（主机网络连接页面中，网络名称下面的那一串字符），然后点击确定。

4.虚拟机开机状态下，点击虚拟机屏幕右上方网络连接按钮，在下拉列表中选择“Edit Connections…”，打开网络设置编辑[界面](https://www.baidu.com/s?wd=%E7%95%8C%E9%9D%A2&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，选中网络点击右侧Edit，选择IPv4 Settings，Method选择“自动（DHCP）”，如下图。

![](https://img-blog.csdn.net/20180311190817118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM5NDkwNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击右下角Save保存修改。这时让虚拟机重新连接一下网络，IP地址自动设置为和主机同一网段的地址，这时虚拟机和主机可以实现通信，桥接完成。

5.在虚拟机桌面右键选择“Openin Terminal”打开虚拟机命令行页面，在页面中输入ifconfig查看虚拟机目前的IP地址，在主机cmd命令行中ping该地址，在虚拟机命令行中ping主机IP地址，发现可以通信，则桥接成功，虚拟机可以联网并且实现了虚拟机和主机之间的通信。

如果发现ping不通的情况，可以查看一下主机或虚拟机的防火墙是否已经关闭。



二、网络地址转换模式（NAT）

![](https://img-blog.csdn.net/20180311190842757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM5NDkwNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



1.不需要任何手动的配置，IP地址等网络配置信息由VMnet8(NAT)虚拟网络的DHCP[服务器](https://www.baidu.com/s?wd=%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)提供，只要宿主机可以上网，虚拟机即可实现网络连接。主机的虚拟网卡VMware Network Adapter VMnet8和虚拟机虚拟网卡的IP地址处于同一个网段，子网掩码、网关、DNS等参数都相同，而主机的真实物理网卡（宿主机相当于路由器）和虚拟机的IP地址并不在同一个范围内。

2.虚拟系统处于虚拟的NAT网络中，虚拟机虚拟出了一个NAT[服务器](https://www.baidu.com/s?wd=%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，使其虚拟网卡能够连接到Internet。在这种连接模式下Guest使用VMnet8[虚拟交换机](https://www.baidu.com/s?wd=%E8%99%9A%E6%8B%9F%E4%BA%A4%E6%8D%A2%E6%9C%BA&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，在主机的网络共享中心中可以看到VMware NetworkAdapter VMnet8虚拟网卡，这块虚拟网卡连接在VMnet8虚拟交换机上与虚拟系统的虚拟网卡进行通信，即它的功能是让主机能够和VMnet8网段通信，但不是为VMnet8网段提供路由功能，去掉这个虚拟网卡Guest仍然可以上网，但是主机无法再访问VMnet8网段，即主机和虚拟机无法再进行通信。

3.VMware Network Adepter VMnet8虚拟网卡的IP地址是在安装VMware时由系统指定生成的，虚拟机虚拟出来的NAT网络所处的网段和VMware Network AdepterVMnet8虚拟网卡所处的网段是相同的，所以主机和虚拟机的两个虚拟网卡之间可以正常通信。

4.在NAT模式下，虚拟机虽然处于内部局域网，但是它是可以访问外网的，因为这时候宿主计算机相当于一台开启了DHCP功能的路由器，宿主机真实的物理网卡（即NAT device）连接着Network，此时虚拟机相当于局域网中的一台真实主机，所以虚拟机可以通过宿主计算机的DHCP来动态获得网络参数，相当于一台计算机通过路由器连接了外部互联网。宿主机可以访问虚拟机，但是局域网内其他主机是无法访问虚拟机的，因为NAT的网络在vmware提供的一个虚拟网络里。（可以通过设置ssh来实现访问，见文末设置步骤）

NAT模式的原理如下：

![](https://img-blog.csdn.net/20180311190905192?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM5NDkwNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

VMware Ubuntu虚拟系统NAT模式实现网络[连接设置](https://www.baidu.com/s?wd=%E8%BF%9E%E6%8E%A5%E8%AE%BE%E7%BD%AE&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)步骤：



1.在VMware菜单栏中选择“虚拟机”à“设置”，打开的窗口中左侧选择“[网络适配器](https://www.baidu.com/s?wd=%E7%BD%91%E7%BB%9C%E9%80%82%E9%85%8D%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)”，右侧选择“自定义”：VMnet8（NAT模式），然后点击“确定”，如下图：

![](https://img-blog.csdn.net/20180311190923817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM5NDkwNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.在VMware菜单栏选择“编辑”à“虚拟网络编辑器”，首先点击右下方“更改设置”进入配置更改模式，点击选择VMnet8虚拟交换机，设置如下：

![](https://img-blog.csdn.net/20180311190938712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM5NDkwNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3.由于主机的虚拟网卡VMwareNetwork Adapter VMnet8和虚拟机虚拟网卡的IP地址处于同一个网段，子网掩码、网关、DNS等参数都相同，在本机的网络和共享中心中打开更改适配器设置，查看虚拟网卡VMware Network Adapter VMnet8的网络配置信息，在上一步打开的虚拟网络编辑器中依次检查修改子网IP,子网掩码，NAT设置（由于开启了DHCP功能，所以正常情况下默认值即是符合要求的）。设置完后点击确定。

4.开启虚拟机，网络设置和上面桥接模式时一样，选择自动获取。

至此完成所有设置，主机ping 虚拟机，虚拟机ping主机（物理网卡IP），虚拟机ping外网都可以成功。

注：网上很多资料采取了为虚拟机设置静态IP的形式，可以在[Ubuntu系统](https://www.baidu.com/s?wd=Ubuntu%E7%B3%BB%E7%BB%9F&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)桌面右上角直接选择Edit Connection…修改，也可以根据网上的教程通过编辑linux的网络配置文件来更改。

要想主机通过远程连接向虚拟系统中传送文件，需要配置Ubuntu的ssh功能。配置过程见文档尾。

三、主机模式（host-only）

![](https://img-blog.csdn.net/20180311190953430?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM5NDkwNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



1.在此模式下所有的虚拟系统之间可以相互通信，但是虚拟系统所处的虚拟网络和宿主机所处的真实网络实际上是被隔离开的。尽管被隔离，但是在这种模式下虚拟系统和主机可以通信，相当于两台机器直接用[双绞线](https://www.baidu.com/s?wd=%E5%8F%8C%E7%BB%9E%E7%BA%BF&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)相连，这时宿主机不再为虚拟机提供路由服务，所以虚拟机不能和外部Network进行通讯。

2.VMware Network Adepter VMnet1是Host用于与Host-Only虚拟网络进行通信的虚拟网卡，但Host-Only网络没有NAT服务，所以虚拟网络是不能连接到外部的互联网的，虚拟机之间形成的虚拟网络是一个全封闭的网络，和外部的唯一联系就是虚拟机能够和主机进行通信。

3.VMware Network Adepter VMnet1虚拟网卡的IP地址也是VMware系统指定的，VMwareNetwork Adepter VMnet1虚拟网卡和虚拟机的虚拟网卡在同一个网段，这种模式下物理网卡和虚拟机的虚拟网卡不在同一个网段。

注：NAT模式与主机模式十分相似，主要不同之处即在于主机模式中宿主机不再具备NAT功能，即不再相当于虚拟机访问Internet的路由器。



NAT模式下实现[外部网络](https://www.baidu.com/s?wd=%E5%A4%96%E9%83%A8%E7%BD%91%E7%BB%9C&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)对虚拟机（ssh）访问功能设置步骤

即使是主机与虚拟机置于同一网段内，主机也不能直接ssh连接虚拟机，还需要做一步端口映射。

1、在VMware系统菜单下选择“编辑：，进入虚拟网络编辑器，选择“NAT设置”，进入“NAT设置”后，在“端口转发”下选择“添加”按钮，添加一条NAT映射规则。其中主机端口即宿主物理网卡的端口，可以任意填写端口号，因为我们这里要实现ssh远程登录功能，这里填写“22”。虚拟机IP地址按照虚拟机实际情况填写，虚拟机端口是22（因为外部设备最终是要通过22端口来访问虚拟机的ssh服务的）。

如果在vmware中建立了多个虚拟机，有几个需要在主机ssh连接，那么就需要配几条端口转发。

2.开启虚拟机在命令行窗口进行ssh配置：（参照网络教程失败了很多次，后来发现在完成了第一步配置之后再按下面的步骤设置即能成功）

（1）检查ssh状态：ssh localhost ssh

connect to hostlocalhost port 22: Connection refused 表示没有安装ssh服务

（2）下载安装ssh：sudo apt-get install openssh-server

（3）启动ssh server：sudo /etc/init.d/ssh start

（4）检查启动状态：ps -e|grep ssh

出现 ssh-agent 和sshd  等表示已经启动

（5）设置root密码：sudo passwd root，根据提示输入

（6）在主机中使用WinSCP连接虚拟机并向虚拟机的桌面发送一个文件，成功。





VMnet0：用于虚拟桥接网络下的虚拟交换机

VMnet1：用于虚拟Host-Only网络下的虚拟交换机

VMnet8：用于虚拟NAT网络下的虚拟交换机

VMware Network Adepter VMnet1：Host用于与Host-Only虚拟网络进行通信的虚拟网卡

VMware Network Adepter VMnet8：Host用于与NAT虚拟网络进行通信的虚拟网卡

VMnet8和VMnet1提供DHCP服务，VMnet0虚拟网络则不提供。









