# vmware常见问题 - 深之JohnChen的专栏 - CSDN博客

2019年03月30日 01:14:48[byxdaz](https://me.csdn.net/byxdaz)阅读数：108


一、虚拟机如何让屏幕自适应客户机窗口大小

1、安装vmware tools

2、【查看】-【自动调整大小】-选择【自动适应客户机】， 勾选自动适应窗口

![](https://img-blog.csdnimg.cn/20190330011425354.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9jaGVuemhlbmd5aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

注意：如果安装了vmware tools 后  ”立即适应客户机 ”仍为灰色，不能自适应的情况。执行下面命令，安装工具。

sudo apt-get install open-vm-tools open-vm-tools-desktop open-vm-tools-dkms

二、ubuntu安装界面显示不完整或者KDevelop开发程序向导界面显示不完整等情况，解决办法：

按住Alt键，然后鼠标左键点击界面窗口，可以移动窗口到合适的位置，将button按钮显示出来。

三、最常见的是系统突然死机，造成有文件没有来得及保存，而造成虚拟机无法打开

解决方法：找到虚拟机的存储目录，搜索*.lck，将查到的文件或文件夹删除即可。

四、提示“Vmware workstation打开虚拟机失败，提示xxx.vmx文件不是有效的配置文件”

这个时候虚拟机无法启动，且*.vmx文件已经变成空文件，自然无法打开虚拟机，这个时候需要使用vmware.log文件中的一些信息对虚拟机进行恢复，从而可以打开虚拟机。

网址：http://blog.csdn.net/kang389110772/article/details/51024126

五、虚拟机整理碎片

虚拟机使用时间久了也会产生碎片化的问题，但整理碎片无法解决虚拟机越用越大的问题

菜单栏：虚拟机->设置->硬件->硬盘->实用工具->碎片整理

六、删除快照的方法

删除快照也能节省大量内存。同时只有删除了快照后才能成功shrink虚拟机

菜单栏：虚拟机->快照->快照管理器->选中需要删除的快照->删除。等待即可。

七、VMware虚拟机中如何配置静态IP

https://blog.csdn.net/Readiay/article/details/50866709

八、配置VMware Workstation 主机模式下让虚拟机上网设置详细

https://blog.csdn.net/jxpaiwp/article/details/47807309

九、VMware 虚拟机无法连接网络解决办法

https://blog.csdn.net/m0_37259197/article/details/78221016


