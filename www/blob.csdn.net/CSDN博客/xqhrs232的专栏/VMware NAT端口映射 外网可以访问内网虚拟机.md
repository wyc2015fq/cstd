# VMware NAT端口映射 外网可以访问内网虚拟机 - xqhrs232的专栏 - CSDN博客
2017年07月10日 11:03:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：563
原文地址::[http://blog.csdn.net/u012540337/article/details/16966223](http://blog.csdn.net/u012540337/article/details/16966223)
相关文章
1、
# [外网访问Vmware虚拟机中的某个服务（如http）](http://blog.csdn.net/mecho/article/details/24396065)----[http://blog.csdn.net/mecho/article/details/24396065](http://blog.csdn.net/mecho/article/details/24396065)
# 2、
巧用VMware虚拟机NAT端口映射使外网访问虚拟机----[http://www.neatstudio.com/show-2013-1.shtml](http://www.neatstudio.com/show-2013-1.shtml)
3、如何从外网访问虚拟机中web服务器----[https://jingyan.baidu.com/article/1612d50043fcebe20f1eee44.html](https://jingyan.baidu.com/article/1612d50043fcebe20f1eee44.html)
我想尝试的是利用本机的ip+port来访问虚拟机上的web服务器，因为这样的话，我就能够将我的web服务器部署成为一个能让外网访问的服务器了，首先说下我的环境：
主机：系统win7，ip地址172.18.186.210 虚拟机版本是VMware Workstation 7，虚拟机下安装了Centos操作系统，ip地址是192.168.202.128，部署了LAMP，成为一个web服务器
网络环境：校园网(有认证服务器，需要账号认证才能上网)，虚拟机采用NAT的方式联网
在校园网的环境下，可以把校园网内的其他主机看成是外网的主机。其实，虚拟机采用NAT联网的方式，可以让我们联想到计算机网络的相关知识，可以把我们的主机看成是一部路由器，然后虚拟机就是处在一个局域网中的一台主机，共享路由器一个端口的地址对外网进行访问，但是外网要访问这部主机，就需要在路由器上做端口映射。于是乎，我们要做的就是在VMware上做端口映射，将主机的一个端口映射到虚拟机的ip地址，那么外网访问虚拟机，只要输入本机的ip:port就可以访问到虚拟机了。首先，设置虚拟机网络：
选择：编辑 --> 编辑虚拟网络，如下图所示：                                     
![](https://img-blog.csdn.net/20131126164834234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjU0MDMzNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
得到如下的设置窗口(由于NAT的联网方式是使用VMnet8这个网卡的，所以我们编辑这个网卡，点击“VMnet8”)：                                  
![](https://img-blog.csdn.net/20131126165131218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjU0MDMzNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到，我的虚拟机是在192.168.202.0这个网段，我们点击NAT Settings(如上述红框内所示)，出现如下界面：                                          
![](https://img-blog.csdn.net/20131126165209640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjU0MDMzNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击上图红色框的“Add”，可以设置主机端口与虚拟机ip和端口的映射，里面存在的那个映射是我已经设定好的，点击Add之后会出现下面的设置界面：                                            
![](https://img-blog.csdn.net/20131126165242687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjU0MDMzNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我将主机的5901端口映射到虚拟机的“192.168.202.128”的80端口了，因为web服务自动开放的端口是80，所以，只要我们访问“http://172.18.186.210:5901”，就可以访问到虚拟机的80端口，也就是web服务了。如果是按照我之前的博文搭建的web服务器的话，你就能访问到“index.php”这个文件了(也就是之前新建的web服务器/var/www/html/下的一个php文件，/var/www/html/是web服务器的默认网页程序目录)。
其实说起来，虚拟机下的NAT上网和端口映射都是很简单的，操作方便，当然，里面包含了许多虚拟网络机制了，封装的好，面对用户透明，就显得很智能了。还有桥接等上网方式，面对不同的应用，都有其独特的优势，值得我们深入学习。
以上是我的一些实践和理解，欢迎大家讨论。有问题的可以在评论中提出，我会尽快回复的了。还是那句，喜欢可以点个赞，不喜勿喷！
[](http://blog.csdn.net/u012540337/rss/list)
版权声明：本文为博主原创文章，未经博主允许不得转载。
## 顶4踩0
