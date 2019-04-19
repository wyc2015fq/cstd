# 不能上网诡异问题||强制linux DHCP 客户端重新获取IP (dhclient) - 三少GG - CSDN博客
2011年12月30日 17:56:59[三少GG](https://me.csdn.net/scut1135)阅读数：15111标签：[linux																[ubuntu																[网络																[服务器																[server](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
MLGB,重装系统也没用！！！  2012/03/16
sudo -i  ;   dhclient -r   ;   dhclient   ;  ifconfig del xx.xx.xx.xx   ;  service network-manager restart ; service networking restart 等等乱七八糟指令后居然成功了。。。
顿时怀疑其实也有可能跟网络有关系，不见得是网卡驱动的问题。。。
如果你的ubuntu网络出现异常，就是可视化的网络工具无法使用，新增加的静态IP也不行
那么你可以执行下面的命令，需要root用户；
> 
service network-manager stop
rm /var/lib/NetworkManager/NetworkManager.state
service network-manager start
最后请重启你的系统
________windows下可用1.
 ipconfig /release 2. ipconfig /renew___________
[1.  Ubuntu上不了网](http://www.linuxdiyf.com/viewarticle.php?id=80903)
ip地址不能正确更新（采用DHCP）
解决办法：
直接切换到window下 获取windows可上网ip地址
然后在**网络连接里面，直接指定****ip地址**，网关，掩码，及dns解析 服务器。(PKU windows下查阅后填入即可)
 未避免ip冲突问题，每次切换时可从windows 查看**ip地址后 在ubuntu下更改～**
**问题解决啦！！！2012.2.6 貌似只有上面这个方法.**
**但此方法带来个小问题,更新管理器貌似无法检查到此设置，无法联网更新，不知为何。**
**解决办法：**
**[让Apt使用代理](http://bbs.chinaunix.net/thread-2064736-1-1.html)**
在 Ubuntu 中使用 apt-get 时，为了获得访问或者较好的速度，我们可以通过使用代理来解决这个问题。apt-get 有一个配置文件 apt.conf，它位于 /etc/apt/ 目录下，我们只需对它稍作更改即可。
执行的步骤为：
1. 打开终端，并输入 sudo gedit /etc/apt/apt.conf。这里，你也可以使用自己喜好的编辑器来代替 gedit。另外，如果 apt.conf 文件不存在，你可以创建一个。
2. 添加下列内容：
Acquire {
http::proxy “http://user:pass@yourProxyAddress:port”
}
例如： 我的设置为 **http::proxy “http://162.105.. :8080”  成功！！！**
你必需调整引号中的内容以适应自己的需要。如代理地址、帐号、密码、端口。另外，如果不需要帐号或密码，则可以省略不写。
3. 假如你只想临时让 Apt 使用代理的话，则可以这样做：
export http_proxy=”http://user:pass@youProxyAddress:port/”
++++++++++++++++++++++++++++++++++++++++++++/////////////////////////////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
﻿﻿2012.2.21 还是没解决。。。采用上面2012.2.6的方法才行！
[http://www.cnblogs.com/babykick/archive/2011/03/25/1996006.html](http://www.cnblogs.com/babykick/archive/2011/03/25/1996006.html)
networkmanager显示有线网络设备未托管，现在有线、dsl都没法用了!!
造成问题的原因：
Linux里面有两套管理网络连接的方案：
1、/etc/network/interfaces（/etc/init.d/networking）
2、Network-Manager
两套方案是冲突的，不能同时共存。
第一个方案适用于没有X的环境，如：服务器；或者那些完全不需要改动连接的场合。
第二套方案使用于有桌面的环境，特别是笔记本，搬来搬去，网络连接情况随时会变的。
－－－－－－－－－－－－－
他们两个为了避免冲突，又能共享配置，就有了下面的解决方案：
1、当Network-Manager发现/etc/network/interfaces被改动的时候，则关闭自己（显示为未托管），除非managed设置成真。
2、当managed设置成真时，/etc/network/interfaces，则不生效。
ubuntu网络显示设备未托管的解决办法
在终端下执行
**sudo gedit /etc/NetworkManager/nm-system-settings.conf或打开把里面的false改为true 然后重启问题即可解决**
以管理员身份用文本编辑器打开sudo gedit/etc/NetworkManager/nm-system-settings.conf，把最后一行的“managed=false”改为“managed=true”，重启电脑（也可以重启networkmanager，方法:sudo service network-manager restart），就可以正常连接无线网络和有线连接了。
还有一个问题就是有线连接不可以编辑了，方法如下：
打开/ect/network/interfaces，只保留
auto lo
iface lo inet loopback
﻿﻿﻿﻿﻿﻿
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
问题描述：
## [dhcp client问题](http://www.blogjava.net/jjwwhmm/archive/2008/04/21/194451.html)
最近家里的电脑除了个怪问题.家里用的是东方有线的网络,系统是ubuntu 6 server,系统开机后一段时间,网络就自动断了.ifconfig查看网卡信息,ip地址是对的.但ping不通外面的网络.然后再过一段时间,网络又重新回复,查看syslog,message等日志,只发现这样的日志:
Apr 20 22:26:10 localhost dhclient: DHCPDISCOVER on eth0 to 255.255.255.255 port 67 interval 7
Apr 20 22:26:10 localhost dhclient: DHCPOFFER from 10.119.16.1
Apr 20 22:26:10 localhost dhclient: DHCPREQUEST on eth0 to 255.255.255.255 port 67
Apr 20 22:26:10 localhost dhclient: DHCPACK from 10.119.16.1
Apr 20 22:26:10 localhost dhclient: bound to xxx.xxx.xxx.xxxx(我的ip地址) -- renewal in 1595 seconds.
初步分析应该是ubuntu的dhclient有问题,暂时解决不了.
[http://www.zdh1909.com/html/Cisco/17951.html](http://www.zdh1909.com/html/Cisco/17951.html)
将 Ubuntu 服务器从 DHCP 更改为静态 IP 地址 如果 Ubuntu服务器安装管理器已经设置你的服务器使用DHCP, 你将要设置静态的 IP地址然后人们才能确实去使用它。
在没有GUI图形界面下设置它，要用到文本编辑, 但这才是经典的linux, 对吧?
让我们打开 /etc/network/interfaces 文件。 我将会使用 vi, 但你也可以使用其他编辑器。
sudo vi /etc/network/interfaces
对于主要接口, 通常使用 eth0, 你会看到如下的这些行:
auto eth0
iface eth0 inet dhcp
如你所见, 它正使用DHCP。 我们要把dhcp改为静态的, 并将在后面添加上数项。 显然要为所在的网络定制。
auto eth0
iface eth0 inet static
address 192.168.1.100
netmask 255.255.255.0
network 192.168.1.0
broadcast 192.168.1.255
gateway 192.168.1.1
现在我们要编辑 resolv.conf 文件来配置DNS:
sudo vi /etc/resolv.conf
在这行‘name server xxx.xxx.xxx.xxx’把 x 你服务器的IP。 (你可以用 ifconfig /all 查找)
此时你通常需要移除dhcp客户端(感谢Peter提醒)。 你也许要用 dhcp-client3 来替换以删除。
sudo apt-get remove dhcp-client
现在我们需要重启网络组件:
sudo /etc/init.d/networking restart
Ping www.google.com。 如果响应, 域名解析正常(除非google是你的hosts文件)。
### [Ubuntu下如何手工配置DHCP和静态IP Ubuntu设置静态IP的方法](http://blog.163.com/howl_prowler/blog/static/26619715201011225642570/)
[///////////////////////////////////////////////////](http://www.linuxdiyf.com/viewarticle.php?id=80903)
[2.](http://www.linuxdiyf.com/viewarticle.php?id=80903)
[http://www.linuxdiyf.com/viewarticle.php?id=80903](http://www.linuxdiyf.com/viewarticle.php?id=80903)
Linux Force DHCP client (dhclient) to renew ip address
Linux renew ip command
$ sudo dhclient -r //release ip 释放IP
$ sudo dhclient //获取IP
//////////////// 原文 开始/////////////////////////
[http://www.cyberciti.biz/faq/how ... -client-ip-address/](http://www.cyberciti.biz/faq/howto-linux-renew-dhcp-client-ip-address/)
Now obtain fresh IP:
$ sudo dhclient 
There is no need to restart network service. Above command should work with any Linux distro such as RHEL, Fedora, CentOS, Ubuntu and others. On a related note you can also try out the following commands:
# ifdown eth0
# ifup eth0
# /etc/init.d/network restart 
OR
# /etc/init.d/networking restart 
//////////////// 原文 结束/////////////////////////
参考文章2：[http://vrlinux.com/wenzhangjingxuan/20101022/77873.html](http://vrlinux.com/wenzhangjingxuan/20101022/77873.html)
**一、DHCP服务器提供以下两种配置方法**
**1、地址池：**
　　这种方法指定了一个用来动态的提供给第一个访问网络的DHCP客户端的IP地址池（有时也称作区域或范围）。当DHCP客户端离开网络超过一定时间后，IP地址就会被回收到地址池以供其它DHCP客户端使用。
**2、MAC地址**
这种方法强制使用DHCP来区别每一块连接上网络的网卡的硬件地址，之后这块网卡每次连上网络请求DHCP服务时都为它提供这个固定的IP地址。
**二、在ubuntu中安装DHCP服务**
sudo apt-get install dhcp3-server
这样就完成安装了。
配置DHCP服务器
如果你的Ubuntu服务器上用友2块网卡，你需要选择哪一块网卡用来监听DHCP服务。默认监听的是eth0。可以通过编辑/etc/default/dhcp3-server这个文件来改变这个默认值。
sudo vi /etc/default/dhcp3-server
找到这行，
INTERFACES=”eth0″
使用下面这行替代它
INTERFACES=”eth1″
保存并退出。这一步可选。
接下来你需要为/etc/dhcp3/dhcpd.conf文件创建一个备份。
cp /etc/dhcp3/dhcpd.conf /etc/dhcp3/dhcpd.conf.back
使用下面的命令编辑/etc/dhcp3/dhcpd.conf文件
sudo vi /etc/dhcp3/dhcpd.conf
使用地址池的方法
你需要修改/etc/dhcp3/dhcpd.conf这个配置文件的以下部分：
```
default-lease-time 600;
max-lease-time 7200;
option subnet-mask 255.255.255.0;
option broadcast-address 192.168.1.255;
option routers 192.168.1.254;
option domain-name-servers 192.168.1.1, 192.168.1.2;
option domain-name “yourdomainname.com”;
subnet 192.168.1.0 netmask 255.255.255.0 {
range 192.168.1.10 192.168.1.200;
}
```
保存并退出文件
这会导致DHCP服务器提供一个从192.168.1.10-192.168.1.200这个范围的IP地址给客户端。如果客户端没有请求一个租期的话，服务器会默认提供600秒的地址租期给客户端。最大的（允许的）地址租期是7200秒。
使用MAC地址的方法
使用这种方法你可以保留一个固定地址给一些或者所有机器。在下面的示例中我给server1,server2,printer1和priner2保留了固定的IP地址。
```
default-lease-time 600;
max-lease-time 7200;
option subnet-mask 255.255.255.0;
option broadcast-address 192.168.1.255;
option routers 192.168.1.254;
option domain-name-servers 192.168.1.1, 192.168.1.2;
option domain-name “yourdomainname.com”;
subnet 192.168.1.0 netmask 255.255.255.0 {
range 192.168.1.10 192.168.1.200;
}
host server1 {
hardware ethernet 00:1b:63:ef:db:54;
fixed-address 192.168.1.20;
}
host server2 {
hardware ethernet 00:0a:95:b4:d4:b0;
fixed-address 192.168.1.21;
}
host printer1 {
hardware ethernet 00:16:cb:aa:2a:cd;
fixed-address 192.168.1.22;
}
host printer2 {
hardware ethernet 00:0a:95:f5:8f:b3;
fixed-address 192.168.1.23;
}
```
现在你需要使用下面命令来重启dhcp服务器。
sudo /etc/init.d/dhcp3-server restart
配置Ubuntu的DHCP客户端
如果你想配置你的Ubuntu桌面为DHCP客户端，使用以下步骤。你需要打开/etc/network/interface文件
sudo vi /etc/network/interfaces
确保你的配置文件含有以下行（eth0只是一个示例）
```
auto lo eth0
iface eth0 inet dhcp
iface lo inet loopback
```
保存并退出文件
你需要使用下面的命令重启网络服务
sudo /etc/init.d/networking restart
如何找到DHCP服务器的IP地址
你需要使用下面的命令
sudo dhclient
或者
tail -n 15 /var/lib/dhcp3/dhclient.*.leases
关闭DHCP服务
sudo /etc/init.d/dhcp3-server stop
