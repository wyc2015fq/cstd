# 虚拟机和主机ping不通解决办法 - xqhrs232的专栏 - CSDN博客
2016年05月30日 15:38:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1579
原文地址::[http://blog.163.com/danshiming@126/blog/static/109412748201251211140194/](http://blog.163.com/danshiming@126/blog/static/109412748201251211140194/)
相关文章
1、如何关闭虚拟机vmware的防火墙？----[http://bbs.csdn.net/topics/300234910](http://bbs.csdn.net/topics/300234910)
虚拟机和主机ping不通  
一般有3种方式：NAT、bridged 、host-Only。如下图：
![虚拟机和主机ping不通解决办法 - danshiming - danshiming的博客](http://img2.ph.126.net/2mJVSnn31zpb67izcp7OCw==/6597743767284397612.jpg)
                                                图1
 Bridged方式：
        在图1中Network connection中选中第1项，即在vm ware虚拟机属性里面设置网卡为桥接，这样启动虚拟机后，虚拟系统就会通过网卡桥接到局域网。一般地宿主机是自动获取局域网ip，那么虚拟机也设置为自动获取局域网ip；如果宿主机不是自动设置为局域网ip，那么就给虚拟机设置一个和宿主机（其IP为:10.232.70.9）同一网段的真实IP，如设置为10.232.70.235，就可以了，一样上网，跟本机无任何区别。采用bridged方式需要有你的局域网网段的ip，这种方式不需要Vm1、Vm8同样可以上外网。
Nat方式：
        在图1中Network connection中选中第2项
使用nat方式默认的虚拟机配置下使用dhcp自动获取ip即可。如果要自己定义静态ip的话，进入 
edit->virtual network setting->host virtual network mapping->vmnet8，点后面的>，subnet可以看到nat所在的网段，自己设置一个该网段的ip也可以（也可以在此修改为其他网段，保证虚拟机中的网段和这里一致），虚拟机网关ip在nat标签下可以看到。 
个人比较喜欢用nat方式，不用受限于局域网环境。但是只能虚拟机访问外部网络，外部除了主机外其他不能访问虚拟机（但是可以通过设置端口映射实现）。
Host-Only以后再补充！
      如果虚拟机里能ping同本机，而本机却ping不通虚拟机，或者虚拟机不能ping通本机，可能有如下原因：
如果是桥接模式，那么可能性1：虚拟机防火墙禁ping，请关闭虚拟机防火墙重试；可能性2：桥接设置的ip有冲突或者是虚拟机桥接服务不正常。
如果是nat模式，那么可能性1：虚拟机防火墙禁ping，请关闭虚拟机防火墙重试；可能性2：本机上的vmnet8网卡被禁用了。可能性3：vbox的nat模式，vpc的共享模式，本来就这样的。
如果是host-only模式，一般就是虚拟机防火墙禁ping了
//=======================================================================================================================
备注::
1> VMWare网络配置方式，我一般使用桥接和主机两种。
     桥接是最简单的方式，一般是你有一个网关，你的windows主机和你VMWare里的各种unix/linux系统，都通过这个网关来通讯.
     而主机方式，是在没有网络的情况下, windows主机操作系统能与VMWare虚拟机上的客户操作系统正常通讯。我们从VMWare的Virtual Network Edit里可以看出，vnet0是用来实现桥接模式的，vnet1是用来实现主机模式的。主机和客户机是直接通过vnet1通讯的。
