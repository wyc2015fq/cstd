# VirtualBox中的虚拟网络环境设置 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月12日 19:52:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：27









网络连接其实是面向网卡的，当一台电脑拥有多块网卡，并且每块网卡连接了不同的网络就能理解了。

默认情况下，VirtualBox为虚拟网卡提供了四种网络环境：Network AddressTranslation(NAT)、Bridged networking、Internal networking和Host-only networking。现在分别介绍四种网络环境下虚拟机网卡的信息。



（一）NAT是VirtualBox为新建虚拟机默认设置的网卡环境。VirtualBox虚拟出一个路由器，为虚拟机中的网卡分配参数：

IP地址 10.0.2.15

子网掩码 255.255.255.0

广播地址 10.0.2.255

默认网关 10.0.2.2

DNS服务器与主机中的相同

DHCP服务器 10.0.2.2

其中10.0.2.2分配给主机，也就是用主机作网关，利用主机的网络访问inertnet。虚拟机通过10.0.2.2能访问主机中搭建的网络服务，但是主机不能访问虚拟机中搭建的网络服务（需要用端口转接才能访问）。同时，使用NAT网络环境的各个虚拟机之间也不能相互访问，因为它们的IP地址都是10.0.2.15！即使设置在NAT网络环境中的网卡为手动指定地址！

![](https://img-blog.csdn.net/20131012194915390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





（二） Bridged networking网络环境中的虚拟网卡与主机中的网卡属于同一个等级，就像真实存在的网卡接入了你主机所在的网络中一样。在这种网络环境中虚拟网卡获得的参数由主机所在的网络环境分配，虚拟机与主机之间是否能够通过网络相互访问也由主机所在的网络环境决定，当然，同时设置了Bridged networking网络环境的虚拟机之间网络互访也由主机所在的网络环境决定。

![](https://img-blog.csdn.net/20131012194952687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







（三）Internal networking网络环境为设置了Internalnetworking网路环境的各个虚拟网卡提供了一个与主机隔绝的虚拟局域网。在Internalnetworking中的网卡不能自动获得任何参数，除非手动设置或者在Internal networking网络环境中的另一台虚拟机中架设DHCP服务器。在Internal networking中，各个设置为Internal networking网络环境的虚拟机之间可以任意访问（虚拟机防火墙允许条件下），但不能访问主机的网络服务甚至internet。

![](https://img-blog.csdn.net/20131012195033187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



（四）默认情况下Host-only networking网络环境利用VirtualBox虚拟出的DHCP服务器，为在Host-only networking中的虚拟网卡分配参数：

IP地址 192.168.56.101 --- 254

子网掩码 255.255.255.0

广播地址 192.168.56.255

默认网关 无

DNS服务器 无

DHCP服务器 192.168.56.100

其中192.168.56.1分配给主机，主机能ping通各个Host-only networking下的虚拟机，但虚拟机不能ping通主机。在Host-only networking网络环境中，主机网卡与各个Host-only networking虚拟网卡构成一个局域网，主机能访问各个虚拟机（虚拟机防火墙允许条件下），各个虚拟机之间也能相互访问，但各个虚拟机都不能访问internet。

![](https://img-blog.csdn.net/20131012195053468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Host－only网络环境

![](https://img-blog.csdn.net/20131012195100078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

VirtualBox网络环境综合应用

附：

NAT网络环境下主机访问虚拟机端口转接：

cd %VBOX_INSTALL_PATH%

将主机8080端口的tcp连接转接到指定虚拟机的8000端口

VboxManagemodifyvm"VMname"—natpf1"linkName,tcp,,8080,,8000"



［参］：VirtualBox® User Manual Version 3.2.10 Edition






