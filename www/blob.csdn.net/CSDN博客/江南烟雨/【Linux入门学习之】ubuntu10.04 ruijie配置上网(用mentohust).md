# 【Linux入门学习之】ubuntu10.04 ruijie配置上网(用mentohust) - 江南烟雨 - CSDN博客
2011年12月22日 21:02:18[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：4983
今天终于把Linux下ubuntu用ruijie鋭捷联网给搞定了，而且十分稳定，不掉线！爽！配置起来也很快。
首先说一下，我的电脑是在Windows7上用wubi安装的ubuntu双系统，版本是10.04.32位。
下面我就介绍一下我的配置过程：
1、配置好ubuntu10.04的网络信息。
点击系统-首选项-网络连接，
![](http://hi.csdn.net/attachment/201112/22/0_13245580159Rdc.gif)
然后编辑eth0的具体信息：
选择IPv4settings菜单项，Method选择manual，然后填好IP地址、子网掩码、网关和DNS。
**2、下载并安装mentohust。**
下载地址：[mentohust](http://code.google.com/p/mentohust/wiki/Install)
下载ubuntu发行版：mentohust_0.3.4-1_i386.deb，然后双击即可安装。
64位下载地址：[64位mentohust](http://code.google.com/p/mentohust/downloads/detail?name=mentohust_0.3.4-1_amd64.deb&can=2&q=)
**3、首先在/etc目录下面新建mentohust文件夹：**
sudo mkdir /etc/mentohust
然后将win7系统下鋭捷的安装目录下面的W32N55.dll 和8021x.exe两个文件拷贝至本机。然后拷贝至刚才新建的文件夹里面。
cp W32N55.dll /etc/mentohust
cp 8021x.exe /etc/mentohust
然后进行mentohust的配置：
sudo mentohust
![](http://hi.csdn.net/attachment/201112/22/0_1324558873wpPV.gif)
选择网卡1.接着输入自己的用户名和密码。选择组播地址1：鋭捷私有。然后选择DHCP方式0（静态IP选择这个）。
过一会儿就OK 了。
每次开机时，输入sudo mentohust即可连接上网了。
经测试过，十分稳定。呵呵~~
