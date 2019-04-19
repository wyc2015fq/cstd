# 【My Ora Life】Oracle Linux 下安装 Oracle 11g - 三少GG - CSDN博客
2011年06月22日 14:25:00[三少GG](https://me.csdn.net/scut1135)阅读数：1242标签：[oracle																[linux																[server																[桌面环境																[authentication](https://so.csdn.net/so/search/s.do?q=authentication&t=blog)](https://so.csdn.net/so/search/s.do?q=桌面环境&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)
安装过程:直接 ./runInstaller
按提示进行即可
知识储备：
**1.Linux 常用命令集：**
[http://linux.chinaitlab.com/Special/linuxcom/](http://linux.chinaitlab.com/Special/linuxcom/)
ls  unzip   ls *o*bm*  rm -rf *0*bm*
cat 向屏幕输出  **`# cat``/proc/version cat/etc/issue`**
**切换 root账户: sudo -i  sudo sh**
2. VNC(Virtual Network Computing)虚拟网络计算工具，本质上来说是一个远程显示系统，管理员通过它不仅仅可以在运行程序的本地机上察看桌面环境，而且可以从Internet上的任何地方察看远程机器的运行情况，而且它具有跨平台的特性。 Linux 要使用远程桌面需要安装VNC，好在Red Hat Enterprise Linux AS 5.0 已经自带了VNC，默认也已经安装了，只要配置一下就可以了。但是Windows客户端还是要安装的。
**VNC的安装与使用【本人实践过程】**
**[http://blog.csdn.net/mengcong2007_hunter/archive/2009/04/02/4043327.aspx](http://blog.csdn.net/mengcong2007_hunter/archive/2009/04/02/4043327.aspx)**
[b] VNC的安装与使用[/b]。
说明：文章内容比较简单，献给那些初学者作为参考。
      文章分为两部分，第一部分为VNC简介，第二部分为VNC的安装与使用。
      文章为小弟结合书籍与小弟的实际操作总结出来的，如有错误与疏漏之处，还请指出。
fish_ys  2003.8.25
[b]『VNC简介』[/b]
  网络遥控技术是指由一部计算机（主控端）去控制另一部计算机（被控端），而且当主控端在控制端时，就如同用户亲自坐在被控端前操作一样，可以执行被控端的应用程序，及使用被控端的系统资源。
  VNC（Virtual Network Computing）是一套由AT&T实验室所开发的可操控远程的计算机的软件，其采用了GPL授权条款，任何人都可免费取得该软件。VNC软件主要由两个部分组成：VNC server及VNC viewer。用户需先将VNC server安装在被控端的计算机上后，才能在主控端执行VNC viewer控制被控端。
（在windows中也由一套著名的网络遥控软件――Symantec公司推出的pcAnywhere。
  VNC server与VNC viewer支持多种操作系统，如Unix系列（Unix，Linux，Solaris等），windows及MacOS，因此可将VNC server 及VNC viewer分别安装在不同的操作系统中进行控制。如果目前操作的主控端计算机没有安装VNC viewer，也可以通过一般的网页浏览器来控制被控端。
  整个VNC运行的工作流程如下：
（1）        VNC客户端通过浏览器或VNC Viewer连接至VNC Server。
（2）        VNC Server传送一对话窗口至客户端，要求输入连接密码，以及存取的VNC Server显示装置。
（3）        在客户端输入联机密码后，VNC Server验证客户端是否具有存取权限。
（4）        若是客户端通过VNC Server的验证，客户端即要求VNC Server显示桌面环境。
（5）        VNC Server通过X Protocol 要求X Server将画面显示控制权交由VNC Server负责。
（6）        VNC Server将来由 X Server的桌面环境利用VNC通信协议送至客户端，并且允许客户端控制VNC Server的桌面环境及输入装置。
[b]『VNC的安装与使用』[/b]
本人的操作环境：被控端 Redhat8.0，主控端Windows XP。
[b]1．        载VNC Server与VNC viewer.[/b]
VNC Server下载地址：http://www.linuxeden.com/download/softdetail.php?softid=744
VNC viewer下载地址：http://download.pchome.net/php/dl.php?sid=2603
[b]2．安装。[/b]
rpm –Uvh vnc-3.3.6-2.i386.rpm (如果是源代码请看包里的说明)
安装winvnc.exe
[b]3.在Linux上启动VNC Server[/b]
执行vncserver命令：
[root@linux root]# vncserver
You will require a password to access your desktops.
Password:                       ----为了不想任何人都可以任意遥控此计算机。因此当第 
Verify:                          ---1次启动VNC server时，会要求设置网络遥控的密码。
New ‘X’ desktop is linux:1         ----一定要记住这一行稍后会用到。
Creating default startup script /root/.vnc/xstartup
Starting applications specified in /root/.vnc/xstartup
Log file is /root/.vnc/linux:1.log
（经上述步骤后，便已启动了VNC Server。如果你想要更改VNC Server的密码，只要执行vncpasswd命令即可。）
[b]4．在Microsoft Windows上运行VNC Viewer[/b]
直接运行“vncviewer.exe”,系统会出现”Connection details”对话框。
在“Connection details”对话框中的“VNC server”文本框中输入VNC Server的IP地址（或主机名及显示装置编号，（请看3。在Linux上启动VNC server的这一行，New ‘X’ desktop is linux:1 得到此信息），例如：192.168.0.1：1（冒号后面的1是执行VNC Server生成的显示装置编号），单击“OK”按钮后，VNC Server即会开始检查所输入的信息，若是信息错误，系统会出现“Failed to connect to server”的错误信息：若是信息正确，则会接着出现“VNC
 Authentication”对话框。
  若是在“VNC Authentication”对话框中输入的密码正确，就可以成功地打开Linux的桌面窗口。
[b]5. 从浏览器远程遥控。[/b]
启动VNC Server 后直接打开浏览器，在地址栏中输入被控端的网址或IP地址，并在网址后加上“：5800＋显示编号”的端口号即可操控该计算机。
例如：http://192.168.01.:5801 (如果显示编号为1，一般第一次设置的显示编号都是1，就用5800＋1＝5801。)
[b]6．FAQ[/b]
**A．为什么连接后，不能显示桌面，而只有一个Terminal窗口？**试着修改/root/.vnc/xstartup,把最后一行 twm& 改成 gnome-session& or  kde&(据说KDE在目前的VNC Viewer上的表现不太稳定)
**B．        为什么重新启动VNC Server后，连接不上了？**因为重新启动VNC Server时，系统会指定一个新的显示编号，需使用此新的编号，否则就无法连接。
（以上只是本人遇到的一些问题，而且用VNC也不是很多，很不全面，还望多提宝贵意见）
**C．      为什么我的vnc viewer可以正常连入，但是字母和数字都不能显示？**
这个问题也折腾我半天。查了网上的资料才知道。vnc server这边会要求vnc viewer这边OS的输入法要和vnc server那边OS的语言要一致。一般我们装的Linux OS都是E文的，所以使用vnc viewer连入的时候，请关闭本地OS的中文输入法，切换到英语就可以了。赶快试试吧。
**G: 终端关闭后如何打开？：**
**左键按住，选择xterm！也是唯一能进行的操作。。。如果没有界面的话**
参考文章：+++++++
VNCServer 配置：
1. 启动 VNC Server： 打开终端执行：vncserver
2. 默认情况下VNC Viewer只能看到 VNC Server的命令行。 要VNC Viewer上可以看到Linux桌面需要做如下设置： 在VNC Viewer所在的机器上，取消/root/.vnc/xstartup文件中下面两行的注释即可: unset SESSION_MANAGER exec /etc/X11/xinit/xinitrc 3. Linux启动后自动运行VNCServer
1) 在 系统设置>服务器设置>服务 中把 vncserver 打勾。
2) 打开配置文件/etc/sysconfig/vncservers，去掉下面这行的注释， VNCSERVERS="1:root" VNCSERVERARGS[1]="-geometry 800x600" （这里注意一下，默认系统配置里有 –nolisten tcp 和 –nohttpd ，这两个是阻止Xwindows登陆和HTTP方式VNC登陆的，如果需要图形界面，那就删除这部分。） 1表示VNC以桌面1运行，这样可以启动root的桌面。
4. VNC端口 如果需要从外网控制内网的某台Linux，则需要打开防火墙相应的端口，并在路由器上进行端口映射。 VNC给浏览器的端口是5800+N，给vncviewer的端口是5900+N，N是设置的display号
Windows 客户端 VNC Viewer 的配置： 1. 从 [http://www.realvnc.com/download.html](http://www.realvnc.com/download.html) 下载 VNC Free Edition for Windows Version 4.1.2 2. 打开 VNCViewer ： 填入VNCServer 的IP:编号(1或2或...) 3. VNCViewer 切换全屏模式：F8
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
oracle linux "[Oracle Linux](http://www.oracle.com/us/technologies/linux/index.html) is fully compatible—both source and binary—with
Red Hat Enterprise Linux. "It is free to download, use and distribute in the form of [ISO images](http://edelivery.oracle.com/linux) as well as individua**l **[**RPM**packages](http://public-yum.oracle.com/).
+++++++++++++++++++++++++++++++++++++++
[http://download.oracle.com/docs/cd/B28359_01/install.111/b32285/toc.htm#BHCJCGCB](http://download.oracle.com/docs/cd/B28359_01/install.111/b32285/toc.htm#BHCJCGCB)
**Oracle® Database Quick Installation Guide11*g* Release 1 (11.1) for Linux x86-64**
Results of a Successful Installation
After you successfully install Oracle Database:
- 
The database that you created and the default Oracle Net listener process run on the system.
- 
Oracle Enterprise Manager Database Control run on the system and can be accessed by using a Web browser.
If you are not logged in as the `root` user, then enter the following command to switch user to `root`:
$ sudo sh
password:
#
To determine the distribution and version of Linux installed, enter the following command:
**`# cat``/proc/version`**
The following or later version of packages for Asianux 3, Oracle Linux 5, and Red Hat Enterprise Linux 5 should be installed:
> binutils-2.17.50.0.6
compat-libstdc++-33-3.2.3
compat-libstdc++-33-3.2.3 (32 bit)
elfutils-libelf-0.125
elfutils-libelf-devel-0.125
gcc-4.1.1
gcc-c++-4.1.1
glibc-2.5-12
glibc-2.5-12 (32 bit)
glibc-common-2.5
glibc-devel-2.5
glibc-devel-2.5-12 (32 bit)
libaio-0.3.106
libaio-0.3.106 (32 bit)
libaio-devel-0.3.106
libgcc-4.1.1
libgcc-4.1.1 (32 bit)
libstdc++-4.1.1
libstdc++-4.1.1 (32 bit)
libstdc++-devel 4.1.1
make-3.81
numactl-devel-0.9.8.x86_64
sysstat-7.0.0
To determine whether the required packages are installed, enter command similar to the following:
# rpm -q package_name
If a package is not installed, then install it from the Linux distribution media or download the required package version from the Linux vendor's Web site.
> 
rpm -q binutils compat-db control-center gcc gcc-c++ glibc glibc-common gnome-libs libstdc++ libstdc++-devel make pdksh sysstat xscreensaver libaio openmotif21
如果出现缺失某个包的话可以google上去下载或者到安装盘里寻找
如:
#rpm –I sysatat-5.0.5-1.i386
