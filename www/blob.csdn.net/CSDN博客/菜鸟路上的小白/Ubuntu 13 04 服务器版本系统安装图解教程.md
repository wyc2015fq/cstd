# Ubuntu 13.04 服务器版本系统安装图解教程 - 菜鸟路上的小白 - CSDN博客





2016年09月05日 18:11:15[somnus_小凯](https://me.csdn.net/u012486840)阅读数：355
个人分类：[linux](https://blog.csdn.net/u012486840/article/category/6382672)









Ubuntu分为桌面版（desktop）和服务器版（Server）

下面为大家介绍服务器版本Ubuntu Server 13.04的详细安装过程。

附Ubuntu Server 13.04系统镜像下载地址：

32位：[http://mirrors.163.com/ubuntu-releases/13.04/ubuntu-13.04-server-i386.iso](http://mirrors.163.com/ubuntu-releases/13.04/ubuntu-13.04-server-i386.iso)

64位：[http://mirrors.163.com/ubuntu-releases/13.04/ubuntu-13.04-server-amd64.iso](http://mirrors.163.com/ubuntu-releases/13.04/ubuntu-13.04-server-amd64.iso)

用启动盘成功引导之后，出现下面的界面

![](http://files.jb51.net/file_images/article/201305/2086.jpg)

选择语言：中文（简体）

![](http://files.jb51.net/file_images/article/201305/2087.jpg)

默认第一项：安装Ubuntu服务器版

![](http://files.jb51.net/file_images/article/201305/2088.jpg)

询问是否继续安装所选择的语言版本，选择“yes”

![](http://files.jb51.net/file_images/article/201305/2089.jpg)

选择“否”默认键盘模式

![](http://files.jb51.net/file_images/article/201305/2090.jpg)

系统运维 [www.osyunwei.com](http://www.osyunwei.com/) 温馨提醒：qihang01原创内容?版权所有,转载请注明出处及原文链接

配置键盘：汉语

![](http://files.jb51.net/file_images/article/201305/2091.jpg)

键盘布局：汉语

![](http://files.jb51.net/file_images/article/201305/2092.jpg)

![](http://files.jb51.net/file_images/article/201305/2093.jpg)

![](http://files.jb51.net/file_images/article/201305/2094.jpg)

设置主机名字

![](http://files.jb51.net/file_images/article/201305/2095.jpg)

创建新用户：输入新用户全名（注意：不是登录账号，相当于描述）

![](http://files.jb51.net/file_images/article/201305/2096.jpg)

输入：账号的用户名（这个是登录账号）

![](http://files.jb51.net/file_images/article/201305/2097.jpg)

设置密码

![](http://files.jb51.net/file_images/article/201305/2098.jpg)

再次输入密码

![](http://files.jb51.net/file_images/article/201305/2099.jpg)

密码复杂性验证，选择是，也可以选否，重新设置复杂的密码

![](http://files.jb51.net/file_images/article/201305/2104.jpg)

选择否

![](http://files.jb51.net/file_images/article/201305/2105.jpg)

选择手动分区，进入下面的界面

![](http://files.jb51.net/file_images/article/201305/2100.jpg)

分区向导，选择需要安装的磁盘

![](http://files.jb51.net/file_images/article/201305/2101.jpg)

是

![](http://files.jb51.net/file_images/article/201305/2102.jpg)

可以看到上面所选磁盘的容量信息，点击磁盘空闲空间，我这里磁盘大小是5.4G

![](http://files.jb51.net/file_images/article/201305/2103.jpg)

创建新分区

![](http://files.jb51.net/file_images/article/201305/2106.jpg)

大小：1GB

![](http://files.jb51.net/file_images/article/201305/2107.jpg)

主分区

![](http://files.jb51.net/file_images/article/201305/2108.jpg)

开始

![](http://files.jb51.net/file_images/article/201305/2109.jpg)

选择交换空间

![](http://files.jb51.net/file_images/article/201305/2110.jpg)

![](http://files.jb51.net/file_images/article/201305/2111.jpg)

系统运维 [www.osyunwei.com](http://www.osyunwei.com/) 温馨提醒：qihang01原创内容?版权所有,转载请注明出处及原文链接

点分区设定结束，这样就设置好了swap分区（一般为内存大小的2倍），继续重复上面的步骤，在划分一个根分区

![](http://files.jb51.net/file_images/article/201305/2112.jpg)

选择空闲空间

![](http://files.jb51.net/file_images/article/201305/2113.jpg)

创建新分区

![](http://files.jb51.net/file_images/article/201305/2114.jpg)

分区大小，这里默认显示剩余空间（分区之前先要规划好，我这里只分swap和跟分区"/"）,

所以剩余空间全部给“/”分区，点继续

![](http://files.jb51.net/file_images/article/201305/2115.jpg)

主分区

![](http://files.jb51.net/file_images/article/201305/2116.jpg)

挂载点：/ 

点分区设定结束

![](http://files.jb51.net/file_images/article/201305/2117.jpg)

分区设定结束并将修改写入磁盘

![](http://files.jb51.net/file_images/article/201305/2118.jpg)

是

![](http://files.jb51.net/file_images/article/201305/2119.jpg)

![](http://files.jb51.net/file_images/article/201305/2120.jpg)

![](http://files.jb51.net/file_images/article/201305/2121.jpg)

默认，继续

![](http://files.jb51.net/file_images/article/201305/2122.jpg)

![](http://files.jb51.net/file_images/article/201305/2123.jpg)

没有自动更新

![](http://files.jb51.net/file_images/article/201305/2124.jpg)

用空格键选中OpenSSH server 点继续（服务器根据自己需求。其它软件后面再安装）

![](http://files.jb51.net/file_images/article/201305/2125.jpg)

![](http://files.jb51.net/file_images/article/201305/2126.jpg)

默认，是

![](http://files.jb51.net/file_images/article/201305/2127.jpg)

继续，系统自动重启

进入登陆界面

![](http://files.jb51.net/file_images/article/201305/2128.jpg)

![](http://files.jb51.net/file_images/article/201305/2129.jpg)

![](http://files.jb51.net/file_images/article/201305/2130.jpg)

输入上面设置的用户名和密码登录系统

**至此，Ubuntu Server 13.04安装完成。**



