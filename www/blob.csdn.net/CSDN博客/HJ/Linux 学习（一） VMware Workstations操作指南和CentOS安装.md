# Linux 学习（一） VMware Workstations操作指南和CentOS安装 - HJ - CSDN博客
2018年02月03日 22:02:13[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：180
个人分类：[Linux](https://blog.csdn.net/feizaoSYUACM/article/category/7442535)
 一、Linux简述
Linux诞生于上世纪90年代，它的诞生离不开两个重要支柱，就是Unix和以GNU Project为核心的自由软件运动。
目前，Linux有数百个发行版，主流的发行版也有十数个。不过，最常用的要数Ubuntu、Red Hat、CentOS这三种了。其中，CentOS是初学者入门的首选，拥有庞大的网络用户群体、系列版本可以轻松获得、应用范围广具有典型性和代表性，是三个具有代表性的优秀特点。
一个完整的Linux操作系统往往由4部分组成，即内核（Kernel）、外壳（Shell）、实用程序和应用程序。内核是Linux的心脏；外壳是Linux的交流窗口；实用程序则是工具；应用程序则是个字用户提供各种各样的丰富的功能的第三方程序。
二、VMware Workstation 14 操作指南
1.下载
进入浏览器输入“VMwareWorkstation”，查找下载即可（通常来说一般是最新版本）。
2.安装（略）
3.新建虚拟机
   （1）查找左上角文件打开第一个新建即可，进入新建虚拟机向导后会给你两个选择（如下图）
![](https://img-blog.csdn.net/20180203215610754?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
个人建议：如果选第一个典型那么接下来的部分可以跳过。
（2）注意只是安装一个客户机操作系统（也就是裸机）里面什么都没有
建议选择第三项，点击“下一步”。
![](https://img-blog.csdn.net/20180203215648763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（3）上面选择“Linux”，下面找到“CentOS7 64位”（适个人电脑而定），如果你还不知道你电脑（32or64？对不起右键打开“我的电脑”选择“属性”查看）这里啰嗦了两句。
![](https://img-blog.csdn.net/20180203215706138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接着再补一张正图
![](https://img-blog.csdn.net/20180203215725908?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（4）这个保存位置是你的整个裸机要保存的，注意如果待会你要装CentOS的话，那所需要的文件内存空间也是在这个位置的。
![](https://img-blog.csdn.net/20180203215742815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（5）接下来没什么好说的，就是一个劲点“下一步”就好了。
三、CentOS安装
1.下载地址：
x86             [https://isoredirect.centos.org/centos/6/isos/i386/](https://isoredirect.centos.org/centos/6/isos/i386/)
AMD64/Intel64   [https://isoredirect.centos.org/centos/6/isos/x86_64/](https://isoredirect.centos.org/centos/6/isos/x86_64/)
2.安装
（1）在设备栏内选择“CD/DVD（IDE）”，双击进去，然后在连接里面选择使用ISO映像文件，找到你下载好的CentOS 7 ISO文件，点击“确定”即可。
![](https://img-blog.csdn.net/20180203215819711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（2）推荐选择第一个
![](https://img-blog.csdn.net/20180203215828334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（3）慢慢找，有中文版的
![](https://img-blog.csdn.net/20180203215858403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（4）注意安装位置可以自动分区，也可自己配置，不推荐kdump，另外网络配置进入打开一下就行了
![](https://img-blog.csdn.net/20180203215911857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（5）注意选择可视化图形界面
![](https://img-blog.csdn.net/20180203215926397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（6）稍等片刻设置一下root密码，再等不久应该就可以呈现出CentOS操作系统的桌面了。
