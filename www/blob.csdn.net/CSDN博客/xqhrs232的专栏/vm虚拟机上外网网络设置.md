# vm虚拟机上外网网络设置 - xqhrs232的专栏 - CSDN博客
2017年04月27日 11:50:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：516
原文地址::[http://blog.csdn.net/masterhsia/article/details/5834793](http://blog.csdn.net/masterhsia/article/details/5834793)
相关文章
1、怎样解决VMware虚拟机无法连接外网问题----[http://jingyan.baidu.com/article/bad08e1ee27be709c85121e0.html](http://jingyan.baidu.com/article/bad08e1ee27be709c85121e0.html)
2、vmware虚拟机不能访问外网，该如何设置----[https://zhidao.baidu.com/question/160504540.html](https://zhidao.baidu.com/question/160504540.html)
vm虚拟机上外网网络设置
第一种情况：主机使用PPPOE拨号上网
方法一：NAT方式
1、先关闭虚拟机中的[操作系统](http://lib.csdn.net/base/operatingsystem)，回到虚拟机主界面 
双击主界面右上方的的“Ethernet”，弹出“Network Adapter”对话框，选择“NAT” 
2、启动虚拟机操作系统，设置IP为动态获取，即通过DHCP获得。 
此时虚拟机中的操作系统用的是主机的IP，主机能够上网，那么虚拟机也能。
方法二：Host-only方式
1、先关闭虚拟机中的操作系统，回到虚拟机主界面 
双击主界面右上方的的“Ethernet”，弹出“Network Adapter”对话框，选择“Host－only” 
2、右击拨号上网的连接，打开PPPOE连接属性，选择“高级”，选择“允许其它网络用户通过此计算机的INTERNET连接来连接” 
在“家庭网络”下拉框中，选择“VMware Network Adapter VMnet1”（这块网卡是为host-only专用的） 
VMware Network Adapter VMnet1虚拟网卡的IP会自动变为192.168.0.1 
此时ping 192.168.0.1 能通即可。 
3、进入vmware中，启动[Linux](http://lib.csdn.net/base/linux)操作系统 
用netconfig命令 
将IP，设为192.168.0.2 （与虚拟网卡在同一网段） 
网关为192.168.0.1 即VMware Network Adapter VMnet1虚拟网卡的IP地址 
DNS设置为ISP的DNS，如61.147.37.1 
4、重启网络： 
#service network restart 
此时，只要主机拨号上网后，虚拟机的系统就可以上网，且不用再拨号
方法三：Bridge方式
这种方式，虚拟机最接近一台真实的机器 
1、先关闭虚拟机中的操作系统，回到虚拟机主界面 
双击主界面右上方的的“Ethernet”，弹出“Network Adapter”对话框，选择“Bridge” 
2、宿主机中安装sygate或wingate之类的代理服务器 
3、设置虚拟机的代理服务器为宿主机的IP即可
我的是小区网的PPPOE，我试过装sygate，不能设置单网卡模式，要把你创建的连接填入拔号的列表中，然后本地连接选择你的网卡，下面的选项DHCP打上钩，然后虚拟机中的linux系统网卡设置DHCP自动获取即可. 
我的想法：虚拟机可以与主机进行通讯吗？例如传文件，PING网卡当然能通的，但在linux系统下不能telnet到真实主机了？貌似虚拟机已成了子网中的一台机了，真实主机已成了路由的功能，试问一台机怎么可能telnet到路由。 
考虑一下，如何选择NAT，host-only，真实主机与虚拟机可以通讯吗？可以共享文件吗？ 我想是不可以的。 
因为NAT是用真实主机的IP，host-only是能过VMware Network Adapter VMnet1来造一个子网，子网内就是虚拟机中的系统，里面的系统网关就是这张网卡的IP，虚拟机与真实主机不处于同一地位，即不是同在一局域网中，所以我觉得是不可以的。
第二种情况：在单位局域网内
“Ethernet”要选择“Bridge”方式 
使用这种方式时，虚拟机跟一台真实的机器一样，此时IP设置为局域网中另一个可用IP即可 
网关：局域网网关服务器的地址（或路由器的地址） 
DNS:设置为ISP的DNS服务器地址 
     其实这种直接设置DHCP获取就可以，因为他就像在路由上接上一个口，路由可以自动分配一下IP给虚拟机中的系统，这种情况下就是真实主机与虚拟机中的系统即既能通讯，又都能上外网，PPPOE的用户买个路由就可以，也就是制造一个局域网。
