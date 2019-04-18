# VMware下Linux上网配置 - 深之JohnChen的专栏 - CSDN博客

2007年11月19日 03:22:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3280


VMware下Linux上网配置

        安装了VMware,经过一番折腾，可以看到图形界面了。为了实现在Linux上网，然后继续，怎么在VMware上网。在网上找到了一片文章，然后傻瓜操作。

    虚拟机版本：VMware Workstation 5.5.1
    主机：Windows xp sp2（简称XP）
    虚拟机：Redhat9 linux (简称VMlinux）
    前提：XP能正常上网

    设置过程：
    1、在XP上：本地连接-->属性-->高级-->Internet连接共享中选择“允许其他网络用户通过此计算机的Internet 连接来连接(N)”，在家用网络连接(H):中，选择“VMware Network Adapter VMnet1”。
    2、在XP上：查看控制面板-->网络和Internet连接-->网络连接-->VMware Network Adapter VMnet1-->属性-->常规-->Internet协议(TCP/IP)-->属性-->常规发现，VMnet1的 IP地址是自动。在常规选项中选择VMWare Bridge Protocol.
    3、在虚拟机软件上：虚拟机(M)-->设置-->硬件-->以太网-->网络连接 选择“Bridge：Connected directly to the physical network”。
    4、在VMlinux上：控制面部-->网络-->eth0设置如下：IP为192.168.11.110/255.255.255.0 缺省网关：192.168.11.1。把DNS设置成与XP中的一致，把VMlinux中的主机地址设置成192.168.11.110。设置OK后，在root用户下执行#/sbin/service network restart，然后VMlinux就可以上网了。

呵呵，这个方法比较占用系统资源，至于不占用系统资源的链接方式，各位自己去找。

