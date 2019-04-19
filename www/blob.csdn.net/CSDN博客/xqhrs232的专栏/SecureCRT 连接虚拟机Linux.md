# SecureCRT 连接虚拟机Linux - xqhrs232的专栏 - CSDN博客
2016年03月23日 10:38:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：646
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址：[http://www.cnblogs.com/yangjinjin/archive/2013/05/22/3093193.html](http://www.cnblogs.com/yangjinjin/archive/2013/05/22/3093193.html)
相关文章
1、SecureCRT连接Linux–Ubuntu----[http://www.xuebuyuan.com/2041305.html](http://www.xuebuyuan.com/2041305.html)
2、secureCRT连接linux方法----[http://wenku.baidu.com/link?url=hGcSX5ymGsdLZ515iY3KexaJ2xVwWHoM4Mh3szt1zm0Ey52xSFUC75XRpeZkRulLg976zDgrQuksGfGndiJcj9bDA6W7IWoIujCip5gNtoS](http://wenku.baidu.com/link?url=hGcSX5ymGsdLZ515iY3KexaJ2xVwWHoM4Mh3szt1zm0Ey52xSFUC75XRpeZkRulLg976zDgrQuksGfGndiJcj9bDA6W7IWoIujCip5gNtoS)
3、如何使用SecureCRT连接linux系统----[http://jingyan.baidu.com/article/f00622283d7cc5fbd3f0c83f.html](http://jingyan.baidu.com/article/f00622283d7cc5fbd3f0c83f.html)
4、SecureCRT怎么连接linux----[http://zhidao.baidu.com/link?url=49v7TJ2rX_EyGbWCuFLQgFsVN-YUOx1POXZMyszHbBVmgRyOMF1WRpXN6hFr-c6gfC146SdYlO87b83bdbEyFa](http://zhidao.baidu.com/link?url=49v7TJ2rX_EyGbWCuFLQgFsVN-YUOx1POXZMyszHbBVmgRyOMF1WRpXN6hFr-c6gfC146SdYlO87b83bdbEyFa)
最近在学习linux，在学习中遇到了一些问题，现总结一下。
虚拟机我用的是VirtualBox，完美支持中文，可以在电脑中创建虚拟机环境，上手非常简单。具体的安装过程请猛点下面链接。
[http://www.jb51.net/softjc/61599.html](http://www.jb51.net/softjc/61599.html)
然后根据自己电脑的CPU来选择安装的linux虚拟光盘（32位和64位）。
下载地址可以参考这里：
[http://mosn.zjut.com/ubuntu.html](http://mosn.zjut.com/ubuntu.html)
[http://ubuntu.cn99.com/](http://ubuntu.cn99.com/)
打开VirtualBox新建虚拟电脑选择linux ubuntu (分32位和64位，ubuntu 适合新手学习应用)
然后根据一系列步骤安装centos
安装成功后开始设置静态IP
配置文件位于：
　　/etc/sysconfig/network-scripts/ifcfg-eth0
　　DEVICE=eth0
　　ONBOOT=yes
　　BOOTPROTO=static
　　IPADDR=192.168.140.88
　　NETMASK=255.255.255.0
　　GATEWAY=192.168.140.1
　　-----------------------------------------------------------------------------
　　PS：NETMASK和GATEWAY可写可不写，主要的参数为BOOTPROTO和IPADDR
　　使IP地址生效：
　　/sbin/ifdown eth0
　　/sbin/ifup eth0
　　-----------------------------------------------------------------------------
　　以下是如果虚拟机不能上网，就需要设置DNS：
　　打开命令行：
　　echo nameserver 8.8.8.8 > /etc/resolv.conf
　　echo nameserver 8.8.4.4 > /etc/resolv.conf
      关于为何使用 8.8.8.8 和 8.8.4.4 请参考以下地址： [http://www.ha97.com/951.html](http://www.ha97.com/951.html)
　　或者直接 vim /etc/resolv.conf
nameserver 8.8.8.8
　　nameserver 8.8.4.4
　　重启网络，如果还不能上网，请检查网关（GATEWAY）是否设置。
　　SecureCRT配置如下：
![](http://www.2cto.com/uploadfile/2012/0208/20120208091358819.png)
　　hostname为上面设置的IPADDR，Port 22，username和password为虚拟机的用户名和密码。
     连接如果未成功，请检查虚拟机端口和ssh是否开启。
　　一、查看SSH服务是否开启
　　[root@oracle ssh]# /etc/init.d/sshd status
　　openssh-daemon is stopped
　　[root@oracle ssh]# 
　　______________________________________
　　二、开启SSH
　　以root用户登录Linux，
　　[root@oracle ssh]# /etc/init.d/sshd start （或者service sshd  start）
Starting sshd:  [  OK  ]
　　[root@oracle ssh]#
　　______________________________________
　　查看防火墙是否关闭
　　[root@oracle ssh]# /etc/init.d/iptables status
　　iptables: Firewall is not running.
　　[root@oracle ssh]# 
　　______________________________________
　　二、如果开启关闭防火墙
　　1. 在终端运行 setup
　　2. 选择 firewall 回车
　　3. 按tab键切换到disablede项，然后按空格键选择，然后再按tab键切换到ok，按空格件选择
　　4. 按tab键切换到quit，按空格件选择
　　然后用客户端ssh登录Linux
　　______________________________________
　　三、关闭防火墙的另一种方式
　　/etc/init.d/iptables stop
　　______________________________________
　　在/etc/ssh下有一个sshd_config 文件。对这个文件进行修改vi sshd_config。往文件中添加如下内容：
　　Port 22 
　　Protocol 2 
　　PermitRootLogin yes 
　　PasswordAuthentication yes
      然后在/etc/init.d/下，通过service sshd restart命令重启这个服务。你再去试试看连接就可以上了。
　　如果还是连接不上，请在VirtualBox的设置->网络->桥接网卡
　　以上操作基本上就能解决基本的问题了。
