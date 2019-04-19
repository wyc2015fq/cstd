# vmware虚拟机上ubuntu 10.04不能连上外网解决办法----本人备注 - xqhrs232的专栏 - CSDN博客
2017年07月06日 18:20:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：232
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/dengchendeng/article/details/8954744](http://blog.csdn.net/dengchendeng/article/details/8954744)
相关文章
1、[vm虚拟机上外网网络设置](http://blog.csdn.net/masterhsia/article/details/5834793)----[http://blog.csdn.net/masterhsia/article/details/5834793](http://blog.csdn.net/masterhsia/article/details/5834793)
# vmware虚拟机上ubuntu 10.04不能连上外网解决办法
最近安装了ubuntu 10.04到vmware虚拟机上，网络出现了问题，折磨了好几天才弄好。一开始是可以ping通外网的，后面就不知道为啥ping不通了，连svn也用不了
经过几天的摸索，终于找到了一些解决问题的办法，一般步骤如下：
1、要确保网卡已经成功获得ip地址，没有获得ip地址的，可以执行
sudo ifconfig eth0 up
sudo /etc/init.d/networking restart
执行上面两条命令后，用ifconfig查看ip地址
如果执行上述两条命令还未获取到地址，则可以查看配置文件/etc/network/interfaces文件
一般配置情况如下：
auto lo
iface lo inet loopback
auto eth0
iface eth0 inet dhcp
auto eth1
iface eth1 inet static
address 10.1.1.100
gateway 10.1.1.1
netmask 255.255.255.0
这里配置的意思是：第一个lo是回环网络，第二个是eth0，采用dhcp方式获取ip地址，第三个eth1采用静态方式分配ip，这个用于跟宿主机windows通信
2、如果已经获取到了ip，可以ping一个同一网段的ip地址看看能不能成功，一般情况下，如果ping的ip地址存在的话，都是可以ping通的
接着可以ping一个外网的ip地址试试，如果不能成功，一般是网关设置的问题，解决如下：
（1）查看windows宿主机上的网关地址
（2）用route命令查看网关信息
如果route命令查看到的网关地址没有跟windows宿主机上看到的一致的网关地址，则用下面的命令添加：
route add default gw 网关地址（该地址是windows宿主机上查看到的网关地址）
如果route命令查看到的网关地址有跟windows宿主机上看到的一致的网关地址，也执行一下“route add default gw 网关地址”命令，我就是被这个给迷惑的
我一执行这个命令，立马就可以上网了，哈哈
不过“route add default gw 网关地址”命令执行只是对当前生效，若机器重启就无效了，应把该命令添加到/etc/profile里面，这样重启也能生效了
3、如果能平通外网的ip地址，而不能ping通域名的话，那就是域名服务器的问题，可通过查看文件/etc/resolv.conf，文件内容一般是下面这样的：
nameserver 172.16.200.251
nameserver 172.16.203.253
nameserver是关键字，后面跟的是域名服务器地址，域名服务器地址也可以从windows宿主机上查到
如果检查上面三步都没有问题，但还是上不了网，那……要看具体情况了，估计是某一步设置有问题
//=====================================================================================================================
本人备注
1>虚拟机的网络模式设置为NAT模式
2>虚拟机的/etc/network/interfaces对eth0设置为DHCP方式------获取的IP地址是VMnet8虚拟网卡的网段地址----比如192.168.7.128的地址
3>route add default gw------奇怪了，也是执行了这个就能上外网了
4>我的外网是通过无线网卡访问的外网
5>route/nameserver
