
# VMware配置NAT模式与主机连接 - Wesley's Blog - CSDN博客


2018年07月16日 20:16:38[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：205


# VMWare虚拟机三种网络方式
HostOnly：在某些特殊的网络调试环境中，要求将真实环境和虚拟环境隔离开，这时你就可采用host-only模式，在这种模式下宿主机上的所有虚拟机是可以相互通信的，但虚拟机和真实的网络是被隔离开的。
Bridge：VMWare虚拟出来的操作系统就像是局域网中的一台独立的主机，它可以访问网内任何一台机器，你需要手工为虚拟系统配置IP地址、子网掩码，而且还要和宿主机器处于同一网段，这样虚拟系统才能和宿主机器进行通信，虚拟系统和宿主机器的关系，就像连接在同一个Hub上的两台电脑。从网络技术上理解相当于在宿主机前端加设了一个虚拟交换机，然后宿主机和所有虚拟机共享这个交换机。
Nat：使用NAT模式，就是让虚拟系统借助NAT的功能，通过宿主机所在的网络来访问公网，可以在不同网段。在这种模式下宿主机成为双网卡主机，同时参与现有的宿主局域网和新建的虚拟局域网，但由于加设了一个虚拟的NAT服务器，使得虚拟局域网内的虚拟机在对外访问时，使用的则是宿主机的IP地址，这样从外部网络来看，只能看到宿主机，完全看不到新建的虚拟局域网。
# 配置NAT模式
选项栏：虚拟机->设置->网络适配器->NAT模式（一般也默认是这个模式）
![这里写图片描述](https://img-blog.csdn.net/20180716201439475?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后通过ifconfig来查看ip地址
![这里写图片描述](https://img-blog.csdn.net/20180716201453506?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后就可以直接通过ssh进行连接了（可能还需要安装一下ssh）
![这里写图片描述](https://img-blog.csdn.net/20180716201506353?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

