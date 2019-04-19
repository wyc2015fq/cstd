# kvm_用户5706034127_新浪博客
|||

KVM详解：：http://blog.chinaunix.net/uid-30022178-id-5749329.html
http://www.cnblogs.com/wudonghang/p/5167145.html
[https://segmentfault.com/a/1190000000644069#articleHeader2](https://segmentfault.com/a/1190000000644069#articleHeader2)
基础教程：[https://wenku.baidu.com/view/f5bc1b1ef90f76c660371a53.html](https://wenku.baidu.com/view/f5bc1b1ef90f76c660371a53.html)
[http://www.linuxidc.com/Linux/2015-03/114462.htm](http://www.linuxidc.com/Linux/2015-03/114462.htm)
ubuntu教程：http://jingyan.baidu.com/article/b907e627cfffc946e7891cd5.html
安装教程，控制台安装：[https://serverfault.com/questions/257962/kvm-guest-installed-from-console-but-how-to-get-to-the-guests-console](https://serverfault.com/questions/257962/kvm-guest-installed-from-console-but-how-to-get-to-the-guests-console)
[http://blog.csdn.net/fanjia2008/article/details/53452861](http://blog.csdn.net/fanjia2008/article/details/53452861)
[http://blog.csdn.net/starshine/article/details/6990599](http://blog.csdn.net/starshine/article/details/6990599)
 libvirt错误总结：[http://liuzhijun.iteye.com/blog/1783698](http://liuzhijun.iteye.com/blog/1783698)
libvirt使用：http://smilejay.com/2013/03/libvirt-configuration-and-usage/
http://blog.csdn.net/qiuhan0314/article/details/43667227
http://blog.csdn.net/linglong102/article/details/46313903
KVM虚拟机的创建、管理与迁移 [：
http://www.linuxidc.com/Linux/2012-06/61690.htm](http://www.linuxidc.com/Linux/2012-06/61690.htm)
http://koumm.blog.51cto.com/703525/1290996
连接到虚拟机 console
http://www.topjishu.com/5084.html
连接到ubuntu虚拟机：http://blog.csdn.net/dobell/article/details/14442457
注意ubuntu是grub2，，一般centos是grub
如果是grub2系统，请修改/etc/default/grub文件，
GRUB_CMDLINE_LINUX=" console=tty0 console=ttyS0"，然后update-grub
而且不是串行设备
sudo shutdown 关机
http://blog.csdn.net/ubuntu64fan/article/details/8137236
安装vnc：
http://blog.csdn.net/jollyjumper/article/details/12292735
客户端和服务器端，客户端vnc有很多个版本。
centos安装配置vncserver，远程登录服务器或者虚拟机：[http://blog.csdn.net/gg296231363/article/details/6899655](http://blog.csdn.net/gg296231363/article/details/6899655)
7 vi /etc/libvirt/qemu.conf 
 vnc_listen =
"0.0.0.0" 
//使监听所有端口，否则只监听本地，监听本地是指只能从服务器本地登录指定虚拟机如 vncviewer
127.0.0.1：端口号(如5902)，如果监听所有端口则可以从远程通过服务器IP：端口号登录虚拟机，如10.92.0.51:5902
 vnc_password = "123456"
### 1. 修改qemu.conf
# vi /etc/libvirt/qemu.conf 
vnc_listen = "0.0.0.0"
说明1：vnclisten 默认绑定127.0.0.1 在配置文件里指定VNC 绑定0.0.0.0IP,就不用在安装kvm虚拟机时指定vnclisten参数了。
说明2：在虚拟主机上有很多个虚拟机的时候，需要指定每个虚拟机的端口，否则将会很乱。
修改qemu.conf就不用在每个虚拟机的xml文件里面修改vnc的地址了
---------------------------------------------------------------------------------
[root@target ~]# virt-viewer -c qemu:///system
node4 
#本地管理虚拟机，system：获取system权限，注意qemu后是三个/
 （注意有个冒号）
[root@manager ~]# virt-viewer -c
qemu+ssh://root@192.168.32.40/system node4 
#远程linux通过virt-viewer+ssh管理虚拟机
 （这个方法有问题）
连接方式主要分为两大类：
1. 本地连接
 1.1
适用情况：当我们在kvm hypervisor上时，可使用本地连接的方式；当然也能使用网络连接的方式（回环网络）
 1.2
连接工具：
1.2.1 virt-manager，和windows下的vmware很是相似，图形界面友好；
1.2.2 virt-viewer，连接命令：**virt-viewer -c qemu:///systemVMachinename；**
1.2.3 console，连接命令：**virsh consoleVMachinename;**
2. 网络连接
 2.1
适用情况：不能使用上面工具连接时
 2.2
连接工具：
2.2.1
vnc，「事先配置好vnc服务」
连接命令： 1. virsh vncdisplay/domdisplay VMachinename 
2. vncviewer ip:port，其中ip:port是由命令1获得；
或把上面两条命令结合起来：vncviewer $(virsh vncdisplay VMachinename)
2.2.2 ssh，「事先配置好ssh服务」
连接命令： ssh user@ip
------------------------------------------------------------------------------------
ubuntu上安装虚拟机，没必要配置网络，默认的就直接能上网。
客户级是ubuntu的，grub是grub2，所以回合centos有所不同。
console链接和vnc是不同的链接方式。
------------------------------------------------------------------------------------
学会使用virt-manager
使用kvm，还要开启tcp，监听更多的ip，关闭防火墙
https://askubuntu.com/questions/262700/qemu-kvm-vnc-support
VNC support should be available by default. So as you said there
may be a problem in firewall or qemu/libvirt configuration.
Regarding qemu by default it only accepts local vnc connections.
To change that edit 
------------------------------------------------------------------------------------
kvm迁移，在服务器上，也没有图形化界面，所以，还是本地创建好镜像img和配置文件，再传上去。
可以本地创建一个小的，然后，上传上去后，在扩容。
------------------------------------------------------------------------------------
------------------------------------------------------------------------------------![kvm](http://s6.sinaimg.cn/mw690/006e9W8vzy7cPXdFDIF55&690)
![kvm](http://s2.sinaimg.cn/mw690/006e9W8vzy7cPXdKmt321&690)
