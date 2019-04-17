# WMware Linux局域网网络访问采用桥接模式而不是NAT - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月08日 18:08:29[boonya](https://me.csdn.net/boonya)阅读数：771








在本机上安装虚拟机以学习Linux系统，因为网络连接的模式不对，所以一直在Host不能ping通VMware的系统所对应的IP。

原因：两个IP地址不在同一个网段，因为使用了NAT连接，虽然外网是通的但是不能访问内网。

#### 1. 网络设置

![](https://img-blog.csdn.net/20150608180608663?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


#### 2. 主机与虚拟机互相pingIP地址

Linux查看自己的IP地址：


![](https://img-blog.csdn.net/20150608180807093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Windows ping Linux IP地址：



![](https://img-blog.csdn.net/20150608180632656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




