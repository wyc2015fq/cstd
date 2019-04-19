# Ubuntu下安装OpenSSH Server并在客户端远程连接Ubuntu - 建建的博客 - CSDN博客
2017年03月01日 10:24:49[纪建](https://me.csdn.net/u013898698)阅读数：159
个人分类：[Linux](https://blog.csdn.net/u013898698/article/category/6754383)

本文主要是向读者介绍了如何在Ubuntu系统下安装OpenSSH Server并在客户端远程连接Ubuntu，共有两种方法，一种是命令行安装；另一种是通过[Ubuntu](http://os.51cto.com/art/200901/104983.htm) Software
 Center安装，希望对大家能有帮助！
方法一（推荐）：命令行安装
只需要一条命令：
sudo apt-get install openssh-server
随后，Ubuntu 会自动下载并安装 openssh server，并一并解决所有的依赖关系。
想了解更详细的内容，请查看：http://www.linuxidc.com/Linux/2011-04/34588p3.htm
方法二 ：通过 Ubuntu Software Center 安装
首先进入 Ubuntu Software Center，然后在搜索栏里输入“openssh-server”；
随后会看到搜索结果里有openssh-server；
接着点击“Install”就可安装。
下面介绍在[Windows](http://cbncz2008.blog.51cto.com/651764/477015)系统下远程连接Linux，我用的是Putty远程连接工具：
1. 首先要确保你的Linux可以联网，并启动openssh server。
sudo /etc/init.d/ssh start
2. 然后通过下面的命令查看[IP地址](http://finger.blog.51cto.com/211738/57990)：
ifconfig
![查看IP地址](http://images.51cto.com/files/uploadimg/20110419/172520802.jpg)
3. 然后在客户端启动Putty 输入要远程连接的Linux IP地址，[端口](http://greyshine.blog.51cto.com/1003856/545465)号默认为22，就可远程连接到Linux。
![输入要远程连接的Linux IP地址](http://images.51cto.com/files/uploadimg/20110419/172607291.jpg)
4. 连接成功后的界面。
![连接成功](http://images.51cto.com/files/uploadimg/20110419/172631998.jpg)
