# 在VMware虚拟机下使用桥接模式访问外网的方法 - xqhrs232的专栏 - CSDN博客
2017年07月07日 15:02:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：483
原文地址::[http://blog.csdn.net/qq_31387691/article/details/54863090](http://blog.csdn.net/qq_31387691/article/details/54863090)
相关文章
1、怎样Linux虚拟机如何怎样桥接上外网----[https://jingyan.baidu.com/article/5d368d1ece82cb3f60c0572b.html](https://jingyan.baidu.com/article/5d368d1ece82cb3f60c0572b.html)
2、虚拟机桥接|bridge方式上外网----[https://jingyan.baidu.com/article/f79b7cb36db0309144023e89.html](https://jingyan.baidu.com/article/f79b7cb36db0309144023e89.html)
一:桥接
桥接相当于是采用虚拟技术把主机和虚拟机放到一个交换机的局域网下
![](https://img-blog.csdn.net/20151212230533688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
把虚拟机的网络适配器设置成桥接
![](https://img-blog.csdn.net/20151212230620558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
要想以桥接方式连接外网，必须了解真实主机的上网方式，然后去配置虚拟主机（虚拟机里的[Linux](http://lib.csdn.net/base/linux)系统）的配置文件，一般情况下配置文件会藏在/etc/sysconfig/network-scripts/
文件夹下，在里面会有很多配置文件![](https://img-blog.csdn.net/20151212230726200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在这里ifcfg-eth0 就是要修改的配置文件，了解到这里后，就开始正式的配置，查看真实主机的本地连接属性
![](https://img-blog.csdn.net/20151212231319136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
或者在命令行输入ipconfig /all 回车  主要查看是否启用dhcp服务，上图中采用的是自动获取ip地址方式，所以是启用了dhcp服务，所以在虚拟主机里的IP引导协议也要设置成dhcp
![](https://img-blog.csdn.net/20151212231450947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
有时候这里会有网关的配置，记得一定要把网关配置正确或者直接注销
GATEWAY=xxxx.xxxx.xxxx.xxxx
配置成dhcp后，IPADDR的配置就无效了，然后重启网络服务 service network restart
之后就可以像真实主机一样访问外网了。如果真实主机没有开启dhcp 就把那个配置文件的
BootProto=none 这样那个IPADDR=xxxx.xxxx.xxxx.xxxx 就会生效，然后去按照真实主机那样配置ip ，子网掩码，网关 重启服务后生效

