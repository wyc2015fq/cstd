# 轻松解决远程链接的“Gtk-WARNING **: cannot open display;”或“Cannot connect to display;”问题 - Koma Hub - CSDN博客
2018年05月18日 16:55:11[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：9815
远程连接服务器或者工作站的时候，通常不能打开一个GUI图形用户，而且在打开过程中会出现诸如：
`(gedit:46927): Gtk-WARNING **: cannot open display:`
或者：
`指令名:Cannot connect to display`
的提示。那么如何解决这个问题呢？
把它分为两种链接方式：**用windows下的cmd链接远程的linux**，另一种**是用本地的linux系统链接远程的linux**（两种的远程机器均为linux系统，我感觉不是linux系统也没关系吧！）。
假设两台机器的IP地址分别为：
**IP-本地**（用户可以未知，但实际怎么可能不知道呢！）；
**username-本地**；
**IP-远程**（需已知用户名和密码）；
**username-本地**。
**一.远程机器和本地及其均为Linux系统**
本地机器：我的是VMWare里面安装的CentOS7系统（已经安装X11图形界面）；
远程机器：CentOS5多用户工作站。
保证这两台机器均可以链接以太网（需已知两台机器的以太网IPv4地址），
当采用SSH命令在本地linux的terminal下链接远程机器时：
```
[username-本地@IP-本地]$ ssh username@IP-远程
username@IP-远程's passwd:（输入正确的密码，通常这个密码不可见）
[username-本地@IP-远程]$
```
这样就连接成功了，但是即便本地机器已经安装了X11图形界面，但实际上当输入一个需要图形界面的指令时，还是无法打开图形界面，我用gedit文本编辑器做个演示：
```
[username-远程@IP-远程]$ gedit
(gedit:46927): Gtk-WARNING **: cannot open display:
```
我们需要修改SSH的远程链接方式（-Y）：
```
[username-本地@IP-本地]$ ssh -Y username@IP-远程
username-远程@IP-远程's passwd:（输入正确的密码，通常这个密码不可见）
[username-远程@IP-远程]$ gedit
```
就会打开gedit的图形界面：
![](https://img-blog.csdn.net/20180518162632736)
**二.远程机器为linux系统，本地机器为windows系统**
**1.安装Xming**
（1）下载地址：[https://sourceforge.net/projects/xming/?source=typ_redirect](https://sourceforge.net/projects/xming/?source=typ_redirect)
（2）安装：“下一步”式无脑安装即可。
（3）配置：打开Xming的安装目录，找到文件“X0.hosts”（刚安装的Xming改文件名一般为X0，也可能X1，此处不讨论），以文本形式打开这个文件，这个文件中的内容如下：
`localhost`
将远程机器的IP地址添加到文件中（另起一行，一个IPv4地址占一整行），修改后文件内容如下（以IP地址为***.***.***.***为例）：
```
localhost
***.***.***.***
```
然后重启Xming软件。Xming软件的安装就结束了。
**2.在windows的CMD下连接远程工作站（linux系统）**
```
#你可以这样
D:\> ssh username-远程@IP-远程
username-远程@IP-远程's passwd: 
[username-远程@IP-远程]$ 
#你也可以这样
D:\> ssh -Y username-远程@IP-远程
username-远程@IP-远程's passwd: 
[username-远程@IP-远程]$
```
但是以上不管你加不加“-Y”，当你试图打开图形界面时候形式提示Cannot connect to display或者Gtk-WARNING **：cannot open display;这时候我们就需要在远程机器上配置X11到本地机器的Xming了。
linux的用户文件夹在/home下，在建立用户的时候会有一个隐藏文件“~/.bashrc”（“~/”表示当前用户目录，它等于“/home/username/”），由于此时无法打开图形界面，诸如gedit等一些图形编辑器都无法使用，用vi/vim编辑“.bashrc”文件，在文件中另起一行添加如下内容：
`export DISPLAY=***.***.***.***:0.0`
其中***.***.***.***是本地机器的IPv4地址。然后source一下使其生效：
`$ source ~/.bashrc`
然后在CMD下远程连接的linux的terminal下输入
`$ gedit &`
就跳出了图形界面：
![](https://img-blog.csdn.net/20180518164836346)
完！
**链接**
SSH的基本使用：[https://blog.csdn.net/rong_toa/article/category/7563319](https://blog.csdn.net/rong_toa/article/category/7563319)
Xming的下载地址：[https://sourceforge.net/projects/xming/?source=typ_redirect](https://sourceforge.net/projects/xming/?source=typ_redirect)
参考文章：[http://www.5iops.com/html/2012/os_0716/198.html](http://www.5iops.com/html/2012/os_0716/198.html)
Frequently Asked Questions(FAQs):[https://github.com/mininet/mininet/wiki/FAQ#x11-forwarding](https://github.com/mininet/mininet/wiki/FAQ#x11-forwarding)
