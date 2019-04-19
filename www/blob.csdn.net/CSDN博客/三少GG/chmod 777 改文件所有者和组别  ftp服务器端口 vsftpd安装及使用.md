# chmod 777||改文件所有者和组别|| ftp服务器端口 vsftpd安装及使用 - 三少GG - CSDN博客
2011年12月01日 20:39:59[三少GG](https://me.csdn.net/scut1135)阅读数：3965
                
在Unix和Linux的各种操作系统下，每个文件（文件夹也被看作是文件）都按读、写、运行设定权限。
例如我用ls -l命令列文件表时，得到如下输出:
-rw-r--r-- 1 bu users 2254 2006-05-20 13:47 tt.htm
从第二个字符起rw-是说用户bu有读、写权，没有运行权，接着的r--表示用户组users只有读权限，没有运行权，最后的r--指其他人（others）只有读权限，没有写权和运行权。这是系统默认设置，我可以改写tt.htm，同组的人和其他人只有权读，没人有权运行，因为只是一个html文件，不必运行。这在Novell的directory services之前很先进。
**读、写、运行三项权限可以用数字表示，就是r=4,w=2,x=1。所以，上面的例子中的rw-r--r--用数字表示成644。**
反过来说777就是rwxrwxrwx，意思是该登录用户（可以用命令id查看）、他所在的组和其他人都有最高权限。
再多说一句。我用chmod o-r tt.htm命令改权限，o-r是others的权限中减掉读。结果是
-rw-r----- 1 bu users 2254 2006-05-20 13:47 tt.htm
**如果用命令chmod 777 tt.htm**，结果是
-rwxrwxrwx 1 bu users 2254 2006-05-20 13:47 tt.htm
任何人都有读、写、运行三项权限。
===
### linux 改变文件权限命令chmod
2011-08-11 00:57:18|  分类：[linux](http://blog.163.com/fan_yishan/blog/#m=0&t=1&c=fks_087071085087087065092087082095080081087074086084087064)|  标签：|字号大中小订阅
改变九个属性 ( chmod )：
**数字类型改变档案权限**
其实， Linux 档案的属性不只九个呦！不过这里不打算写的太复杂！先说一些简单的，较难的以后有空再说吧！先复习一下刚刚上面说的： 
-rwxrwxrwx
这九个属性是三个三个一组的！其中，我们可以使用数字来代表各个属性，各属性的对照表如下： 
r:4
w:2
x:1
由于同一组的数字是相加的！例如当属性为 [ -rwxrwx--- ] 则是： 
owner  = rwx  = 4+2+1 = 7
group  = rwx  = 4+2+1 = 7
others = ---  = 0+0+0 = 0
所以，这样的属性造成的数值就是 770 啰！而更改属性的指令为 chmod，他的语法为： 
语法：
chmod [-R] xyz 档案或目录
xyz 为同三组 rwx 属性数值的相加
举例来说，如果要将 .bashrc 这个档案所有的属性都打开，那么就下达： 
[root@test root]# ls –al .bashrc
-rw-r--r--    1 root     root          226 Feb 16  2002 .bashrc
[root@test root]# chmod 777 .bashrc
[root@test root]# ls –al .bashrc
-rwxrwxrwx    1 root     root          226 Feb 16  2002 .bashrc
看到了吗？属性改变了喔！由于一个档案有三组属性，所以你可以发现上面 777 为三组，而由于我们将所有的属性都打开，所以数字都相加，亦即『r+w+x = 4+2+1 = 7』。
那如果要将属性变成『 -rwxr-xr-- 』呢？那么就成为 
[4+2+1][4+0+1][4+0+0]=754 啰！所以你需要下达 chmod 754 filename。最
常发生的一个问题就是，常常我们以 vi 编辑一个 shell 的文字文件后，他的属性通常是 -rw-rw-rw- 也就是 666 的属性，如果要将他变成可执行档，并且不要让其它人修改此一档案的话，那么就需要 -rwxr-xr-x 这一个 755 的属性，所以chmod 755 test.sh 就需要这样做啰！
另外，有些档案你不希望被其它人看到，例如 -rwxr-----，那么就下达 chmod 740 filename 吧！
**符号类型改变档案型态**
还**有一个改变属性的方法呦！从之前的介绍中我们可以发现，基本上就九个属性分别是(1)user (2)group (3)others 三群啦！那么我们就可以藉由 u, g, o 来代表三群的属性！此外， a 则代表 all 亦即全部的三群！那么读写的属性就可以写成了 r, w, x 啰！也就是可以使用底下的方式来看：**
|**chmod**|**u g o a**|**+(加入) -(除去) =(设定)**|**r w x**|**档案或目录**|
来实作一下吧！假如我们要『设定』一个档案的属性为『-rwxr-xr-x』时，基本上就是： 
- u：   具有可读、写、执行 
- g 与 o：  具有读与执行 
所以就是： 
[root@test root]# chmod u=rwx,og=rx .bashrc
[root@test root]# ls –al .bashrc
-rwxr-xr-x    1 root     root          226 Feb 16  2002 .bashrc
请注意， r=rwx,og=rx 这一段文字之间并没有空格符隔开呦！不要搞错啰！那么假如是『 -rwxr-xr-- 』？可以使用『 
chmod u=rwx,g=rx,o=r filename 』来设定。此外，如果我不知道原先的档案属性，而我只想要增加 .bashrc 这个档案的每个人均可写入的权限，那么我就可以使用：
[root@test root]# ls –al .bashrc
-rwxr-xr-x    1 root     root          226 Feb 16  2002 .bashrc
[root@test root]# chmod a+w .bashrc
[root@test root]# ls –al .bashrc
-rwxrwxrwx    1 root     root          226 Feb 16  2002 .bashrc*
而如果是要将属性去掉而不更动其它的属性呢？！例如要拿掉所有人的 x 的属性，则 
[root@test root]# chmod a-x .bashrc
[root@test root]# ls –al .bashrc
-rw-rw-rw-    1 root     root          226 Feb 16  2002 .bashrc
知道 +, -, = 的不同点了吗？对啦！ = 与 – 的状态下，只要是没有指定到的项目，则该属性『不会被变动』，例如上面的例子中，由于仅以 – 拿掉 x 则其它两个保持当时的值不变！呵呵！多多实作一下，你就会知道如何改变属性啰！
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**Ubuntu修改文件所有者和组别**
例:**sudo chown user:group location_of_files_or_folders**
### NAME
chown - 修改文件所有者和组别
 
     览
        chown [options] user [:group] file...
 
        POSIX 选项： [-R]
 
        GNU   选项(最短格式):   [-cfhvR]   [--dereference]  [--reference=rfile]
        [--help] [--version] [--]
 
     述
        chown          修改每个由第一个非选项参数声明的给定          file(     ) 
        的用户和/或组的所有权.如下:
        如果只给出了用户名(或者数字用户标识),那么该用户即成为每个指定
        文件的所有者,而该文件的组别并不改变.如果用户名后面紧跟着冒号和
        组名(或者是数字组标识),并且它们之间没有空格,那么文件的组所有权
        也随之改变.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## [vsFTPd 服务器初学者指南（欢迎大家参与补充）](http://www.linuxsir.org/main/?q=node/152)
**作者：**北南南北，正在修订之中
**来自：**LinuxSir.Org
**摘要：**vsFTPD是一款小巧易用FTP服务器程序；本文面向初学者的一点疑问，能让初学者在最短的时间内学会最简单的vsftpd服务器的架设；本文应该算是初学者练手篇；正在更新之中；本文基于 vsftpd-2.0.3；
**欢迎大家共同修订本教程，有些功能的实现，需要大家参与；**您可以以留言的形式参与，我会每天把大家的留言整理加入文档；谢谢。
**目录**
[1、vsFTPd，目前常用FTP服务器套件；](http://www.linuxsir.org/main/?q=node/152#1)
[2、ftp 用户管理解说；](http://www.linuxsir.org/main/?q=node/152#2)
[2.1、匿名ftp 用户和用户组的理解；](http://www.linuxsir.org/main/?q=node/152#2.1)
[2.2、匿名ftp 用户和ftp用户组是否可以删除；](http://www.linuxsir.org/main/?q=node/152#2.2)
[3、vsFTPd的安装；](http://www.linuxsir.org/main/?q=node/152#3)
[4、vsFTPd的服务器的启动和关闭；](http://www.linuxsir.org/main/?q=node/152#4)
[4.1、vsFTPd服务器启动和关闭的通用方法；](http://www.linuxsir.org/main/?q=node/152#4.1)
[4.2、在Fedora/Redhat/CentOS中vsFTPd 服务器的启动和关闭：](http://www.linuxsir.org/main/?q=node/152#4.2)
[5、vsFTPd的服务器和防火墙及SELINUX的关系；](http://www.linuxsir.org/main/?q=node/152#5)
[6、500 OOPS: vsftpd: refusing to run with writable anonymous root](http://www.linuxsir.org/main/?q=node/152#6)
[7、关于vsFTPd配置文件的说明；](http://www.linuxsir.org/main/?q=node/152#7)
[8、关于vsFTPd服务器的一些常用功能的实现；](http://www.linuxsir.org/main/?q=node/152#8)
[8.1、关于匿名上传下载的实现；](http://www.linuxsir.org/main/?q=node/152#8.1)
[8.2、关于添加本地用户及打开读写权限示例；](http://www.linuxsir.org/main/?q=node/152#8.2)
[8.4、如何实现虚拟路径？](http://www.linuxsir.org/main/?q=node/152#8.4)
[8.5、打开vsFTP服务器的日志功能；](http://www.linuxsir.org/main/?q=node/152#8.5)
[8.6、如何定制欢迎信息；](http://www.linuxsir.org/main/?q=node/152#8.6)
[8.7、硬盘空间有限，如何把帐号ftp默认的路径/var/ftp更改到别处？](http://www.linuxsir.org/main/?q=node/152#8.7)
[8.8、如何让vsFTP服务器限制链接数，以及每个IP最大的链接数？](http://www.linuxsir.org/main/?q=node/152#8.8)
[8.9、如何限制传输速度？](http://www.linuxsir.org/main/?q=node/152#8.9)
[8.10、如何有选择的把用户限制在家目录中呢？](http://www.linuxsir.org/main/?q=node/152#8.10)
**+++++++++++++++++++++++++++++++++++++++++++正文+++++++++++++++++++++++++++++++++++++++++++**
**1、vsFTPd，目前常用FTP服务器套件；**
vsFTPd是一款在Linux发行版中最受推崇的FTP服务器程序；特点是小巧轻快，安全易用；能让其自身特点得发发挥和掌握，也然最主要的是会用； 
目前在开源操作系统中常用的FTPD套件主要有ProFTPD、PureFTPd和wuftpd等；至于哪个FTP服务器套件更好，哪个是你最熟悉的，哪个就是最好的；
**2、ftp 用户管理解说；**
FTP服务器对用户的管理，在默认的情况下是根据 /etc/passwd及/etc/group 来进行的，所以我们一定要了解Linux系统用户和用户组的管理，用户和用户组的管理是一切应用的的基础，有的弟兄不想去了解基础的东西，就想一步成架好各种服务器，事实证明这种学习方法是最不明智的；虽然在您可能在几分钟启动了ftp服务器，但遇到问题时，您不一定能知道是哪里出错；所以基础还是极为重要的；
**推荐文章：**[《Linux 用户（user）和用户组（group）管理概述》](http://www.linuxsir.org/main/?q=node/91)
**2.1、匿名ftp 用户和用户组的理解；**
当我们访问各大FTP上访问时，可能我们不去想我们以什么身份登录的，如果他的FTP允许匿名登录的话；比如我们在浏览器上打入；
```
ftp://mirrors.kernel.org
或
ftp://ftp:ftp@mirrors.kernel.org
```
我们会发现上面的两行最终都能访问，而且显示的结果也完全一样，最终都跳到 ftp://mirrors.kernel.org 地址；那我们访问这个FTP时，是不是有用户和密码呢？是的，也是需要在的，只是在服务器端允许匿名访问，而匿名访问的用户名和密码都是ftp，只是我们因为匿名访问，没有感觉到他有用户名和密码罢了。第二个地址就是以ftp用户，密码也是ftp来访问 ftp://mirrors.kernel.org；
如果我们以ftp命令连接 mirrors.kernel.org 时，我们会发现需要输入用户ftp，密码ftp才能访问；
在FTP服务器中，匿名用户的用户名和密码都是ftp ；这个用户可以在您的操作系统中的 /etc/passwd 中能找得到；可能有类似下面的一行；
`ftp:x:14:50:FTP User:/var/ftp:/sbin/nologin`
**说明：**
/etc/passwd 是系统用户的配置文件；/etc/group是系统用户组配置文件，您可以通过 [《Linux 用户（user）和用户组（group）管理概述》](http://www.linuxsir.org/main/?q=node/91) 及其相关文档中了解一些用户管理的一些基础知识；
在ftp用户这行中，我们看到七个字段，每个字段写字段之间用:号分割；
ftp 是用户名
x 是密码字段，是隐藏的；
14 是用户的UID字段，可以自己来设定，不要和其它用户的UID相同，否则会造成系统安全问题；
50 用用户组的GID，可以自己设定，不要和其它用户组共用FTP的GID，否则会造成系统全全问题；
FTP User 是用户说明字段；
/var/ftp 是ftp用户的家目录，可以自己来定义；
/sbin/nologin 这是用户登录SHELL ，这个也是可以定义的，/sbin/nologin 表示不能登录系统；系统虚拟帐号（也被称为伪用户）一般都是这么设置。比如我们把ftp用户的/sbin/nologin 改为 /bin/bash ，这样ftp用户通过本地或者远程工具ssh或telnet以真实用户身份登录到系统。这样做对系统来说是不安全的；如果您认为一个用户没有太大的必要登录到系统，就可以只给他FTP帐号的权限，也就是说只给他FTP的权限，而不要把他的SHELL设置成 /bin/bash 等；
**关于ftp用户组的理解：**
我们查看 /etc/group 的时候，会发现类似这样一条；
`ftp:x:50:`
/etc/group 是用户组的管理配置文件，上面这行表示用户组ftp，x是密码段，50是GID；我们对照在/etc/passwd中的ftp那行得知ftp用户是属于ftp用户组的，因为ftp用户那行中的GID和ftp用户组的GID是相同的；
**2.2、匿名ftp 用户和ftp用户组是否可以删除**
在一般情况下是不能把/etc/passwd和/etc/group 中有把ftp用户和用户组的行删除的，因为FTP服务器是需要他们来对FTP用户实现管理，在默认的情况下。
虽然不能删除，但对/etc/passwd及/etc/group中的ftp用户及ftp用户组的一些相关的东西是能修改的；比如我们可以把ftp用户的家目录改掉，也可以把ftp用户的UID改掉 ... ... 前提是你对用户管理有所了解，系统用户管理是一切应用基础之一，可能初学Linux的弟兄并不了解用户管理的重要性，但慢慢您就会发现这个道理；
**3、vsFTPd的安装；**
在最新的各大发行版中的安装盘中都有vsFTPd的软件包，您用相应发行版提供的软件包管理工具就能安装上；当然您可以到各大发行版的FTP镜像中找到vsftpd的软件包；当然也能用软件包的管理工具在线安装；vsftpd的软件包本来不大，花不了多长时间，两分钟就够了吧；因为本文是初学者教程，我严重推荐您用发行版提供的软件来安装，不推荐您自己来编译源码包的安装方式；
**如果您用的是Fedora 或Redhat 系统，可以用下面的命令在线安装；**
`[root@localhost ~]# yum install vsftpd`
**如果是debian 类系统，可以用apt 来在线安装；**
`[root@localhost ~]# apt-get install vsftpd`
**如果您是RPM的系统，也可以找到vsftpd-xxxx.rpm 的包来通过rpm命令来安装；**
`[root@localhost ~]# rpm -ivh vsftpd*.rpm `
**您可以下载源码包来安装**
比如我们下载的是 vsftpd-2.0.3.tar.gz ；
```
[root@localhost ~]# tar zxvf vsftpd-2.0.3.tar.gz
[root@localhost ~]# cd vsftpd-2.0.3
[root@localhost ~]# make ;make install
[root@localhost ~]# cp vsftpd.conf /etc
```
然后修改/etc/vsftpd.conf ，在配置文件的最后一行加入下面一行；
`listen=yes`
源码包安装的方法，如果您的系统是RPM包管理的系统，可以删除/etc/xinetd.d/vsftpd 这个文件；然后启动xinetd 服务器；
```
[root@localhost ~]# /etc/init.d/xinetd restart
停止 xinetd：                                              [  确定  ]
启动 xinetd：                                              [  确定  ]
```
vsFTPd运行有两种模式，在RPM包管理的系重审统，大多是由Fedora/Redhat 开发而来，对于这样的系统有xinted服务器一说；对于非RPM包管理的系统，一般没有xinted这一说。为了保证本文档的统一，我们都不要用xinetd模式，而用initd运行模式来启动和管理服务器，也就是独立运行模式； 至于什么是xinted和inted 模式，您可以参考[《 用vsFTPd自架Linux网络安装服务器》](http://www.linuxsir.org/bbs/showthread.php?t=42075)
**4、vsFTPd的服务器的启动和关闭；**
vsFTPd服务器启动和关闭是极容易的；我们可以直接把服务器运行调到后台运行；这是几乎所有服务器管理的运行方式；
**4.1、vsFTPd服务器启动和关闭的通用方法；**
运行vsftpd命令，要用到root权限，如果您当前操作用户并非是root，可以用su切换到root权限；关于root权限的切换，您有两种方式，一种是su，另一种是sudo；**请参考：**[《Linux 系统中的超级权限的控制》](http://www.linuxsir.org/main/?q=node/104)
**vsFTPd服务器的运行；**
```
[root@localhost ~]# /usr/sbin/vsftpd &
[root@localhost ~]# /usr/local/sbin/vsftpd &
```
**vsFTPd服务器是否运行起来；**
我们可以通过pgrep vsftpd 来查看vsftpd是否运行起来；
```
[root@localhost ~]# pgrep vsftpd
4248
```
上面显示vsFTPd服务器运行起来了，您可以通过ftp命令、lftp工具或gftp或其它的FTP客户端来测试连接；
**vsFTPd 服务器关闭：**
我们用pkill vsftpd 来杀死vsftpd进程，这样就把vsFTPd关闭，用psgrep vsftpd 来查看进程，如果没有进程，证明vsFTPd已经关掉；当然也要用到root权限。
```
[root@localhost ~]# pkill vsftpd
[root@localhost ~]# pgrep vsftpd
```
**4.2、在Fedora/Redhat/CentOS中vsFTPd 服务器的启动和关闭：**
在Fedora/Redhat/CentOS中，也可以用下面的方法来启动vsFTPd；当然也要用到root权限；
```
[root@localhost beinan]# /etc/init.d/vsftpd start
为 vsftpd 启动 vsftpd：                                    [  确定  ]
```
重新启动vsFTPd用下面的命令；
```
[root@localhost beinan]# /etc/init.d/vsftpd restart
关闭 vsftpd：                                              [  确定  ]
为 vsftpd 启动 vsftpd：                                    [  确定  ]
```
关掉vsFTPd服务器，应该用下面的命令；
```
[root@localhost beinan]# /etc/init.d/vsftpd stop
关闭 vsftpd：                                              [  确定  ]
```
**5、vsFTPd的服务器和防火墙及SELINUX的关系；（重要）**
我在论坛上看到有些弟兄说vsFTPd服务器启动正常，但却不能访问或用户不能上传文件，我感觉应该是防火墙或SELINUX的事；可能FTPD服务器被防火墙或SELINUX的安全机制防住了。所以您必须要让防火墙通过ftp，当然也要让SELINUX通过ftp服务器才行；
在Fedora/Redhat/CentOS中，您要设置一下防火墙，可以把防火墙关掉，或者在自定义中让ftp “通过”防火墙；
`[root@localhost ~]# system-config-securitylevel-tui`
或者运行一下如下的命令，清除防火墙规则（通用）；
`[root@localhost beinan]# iptables -F`
关于SELINUX服务器的解说 ，可能老手或新手对SELINUX都有点麻烦，建议您SELINUX；或者让vsFTPd服务器跳过SELINUX启动；也就是说要用到**[4.1、vsFTPd服务器启动和关闭的通用方法；](http://www.linuxsir.org/main/?q=node/152#4.1)**；这在Fedora/Redhat/CentOS中这样启动vsFTPd服务器是有效的；
当然您可也可以关掉SELINUX，在/etc/selinux/config 配置文件如下；
```
/etc/selinux/config
# This file controls the state of SELinux on the system.
# SELINUX= can take one of these three values:
#       enforcing - SELinux security policy is enforced.
#       permissive - SELinux prints warnings instead of enforcing.
#       disabled - SELinux is fully disabled.
SELINUX=Disabled  #这样就把SELINUX服务器关掉了，请重新启动系统；
# SELINUXTYPE= type of policy in use. Possible values are:
#       targeted - Only targeted network daemons are protected.
#       strict - Full SELinux protection.
SELINUXTYPE=targeted
```
**6、500 OOPS: vsftpd: refusing to run with writable anonymous root**
如果我们已经把vsFTPd服务器启动好了，但登录测试是会出现类似下面的提示；
500 OOPS: vsftpd: refusing to run with writable anonymous root
这表示ftp用户的家目录的权限不对，应该改过才对；
```
[root@localhost ~]# more /etc/passwd |grep ftp
ftp:x:1000:1000:FTP User:/var/ftp:/sbin/nologin
```
我们发现ftp用户的家目录在/var/ftp，就是这个/var/ftp的权限不对所致，这个目录的权限是不能打开所有权限的；是您运行了chmod 777 /var/ftp所致；如果没有ftp用户这个家目录，当然您要自己建一个；
如下FTP用户的家目录是不能针对所有用户、用户组、其它用户组完全开放；
```
[root@localhost ~]# ls -ld /var/ftp
drwxrwxrwx  3 root root 4096 2005-03-23  /var/ftp
```
**修正这个错误，应该用下面的办法；**
```
[root@localhost ~]# chown  root:root /var/ftp
[root@localhost ~]# chmod  755 /var/ftp
```
有的弟兄可能会说，那匿名用户的可读、可下载、可上传怎么办呢？这也简单，在/var/ftp下再建一个目录，权限是777的就行了，再改一改vsftpd.conf就OK了；没有什么难的；
vsFTPd出于安全考虑，是不准让ftp用户的家目录的权限是完全没有限制的，您可以去读一下vsFTPd的文档就明白的了；否则也不能称为最安全的FTP服务器了，对不对？
**7、关于vsFTPd配置文件的说明；**
vsftpd.conf是vsFTPd服务器的配置文件，此文件一般是/etc/vsftpd.conf或/etc/vsftpd/vsftpd.conf ，以系统为准吧。。
vsftpd.conf配置文件就是vsFTPd服务器的全局控制文件，此配置文件中，每行应该算做一个规则；前面带有#号的服务器不会解释，这和apache 的配置文件类似；#后所接的内容一般是说明性的，或者是关掉某些功能的选项；
vsftpd.conf 这个配置文件并不包括所有您想实现的功能，有些功能的实现，我们要自己动手来解决，我们要学会查vsFTPd的文档和FAQ等，虽然是英文的，但都是比较简单的；我们要习惯看洋文文档，为什么呢？因为Linux本来就是洋人厉害，重量级的软件都是洋人的。呵，兄弟不要抱怨，谁让我们落后了呢？
**注意：**修改完配置文件后，一定要重启vsFTPd服务器才能生效，切记~~
**8、关于vsFTPd服务器的一些常用功能的实现；**
**8.1、关于匿名上传下载的实现；**
关于匿名上传，其实也比较简单，首先我们要修改一下vsftpd.conf ，此文件位于 /etc/目录下，可能是/etc/vsftpd.conf，也可能是/etc/vsftpd/vsftpd.conf文件。以您的系统环境为准；
**首先：我们要改一下vsftpd.conf，确保有以下几行；**
```
anonymous_enable=YES
anon_upload_enable=YES
anon_mkdir_write_enable=YES
anon_umask=022
```
**其次：在ftp用户家目录的下建一个文件夹，并修改其权限为完全开放；**
**ftp用户的家目录在哪？我们前面已经说了，要通过/etc/passwd来查看；也可以通过finger ftp来查看；**
```
**[root@localhost beinan]# finger ftp**
Login: ftp                              Name: FTP User
Directory: **/var/ftp    **                 Shell: /sbin/nologin
```
这说明ftp用户的家目录在/var/ftp ，我们要在这个目录下建一个目录，然后把他的权限设置为任何用户可读可写可执行就行了；一般的情况下，在发行版中，有一个/var/ftp/pub的目录，如果没有，您也可以自己建一个；把配置文件改好后，只要把/var/ftp下的任何一个目录的权限打开，都可以用来匿名上传和下载；
比如您想让匿名用户上传和下载都在/var/ftp/pub，就可以把/var/ftp/pub的权限打开，如果没有这个目录，您要自己建一个；
```
[root@localhost ~]# mkdir /var/ftp/pub
[root@localhost ~]# chmod 777 /var/ftp/pub
```
这样上传的时候传到pub目录就OK了，对不对？？
**8.2、关于添加本地用户及打开读写权限示例；**
FTP用户一般是不能登录系统的，这也是为了安全。在系统中，没有权限登录系统的用户一般也被称之为虚拟用户；虚拟用户也是要写进/etc/passwd中；这只是一种虚拟用户的方法，但说实在的并不是真正的虚拟用户，只是把他登录SHELL的权限去掉了，所以他没有能力登录系统；
如果我们想把beinan这个用户目录定位在/opt/beinan这个目录中，并且不能登录系统；我们应该如下操作
```
[root@localhost ~]# adduser -d /opt/beinan -g ftp -s /sbin/nologin beinan
[root@localhost ~]# passwd beinan
Changing password for user beinan.
New password:
Retype new password:
passwd: all authentication tokens updated successfully.
[root@localhost ~]#
```
其实这还是不够的，还要改一下配置文件vsftpd.conf ，以确保本地虚拟用户能有读写权限；
```
local_enable=YES
write_enable=YES
local_umask=022
```
**8.4、如何实现虚拟路径？**
**比如：**
```
/home/a 映射为 ftp://localhost/a
/home/b/c 则为 ftp://localhost/c
```
其实这个不能说是vsFTPd的内容，其实我们早就接触过了，可能我们没有注意，我们可以通过如下的方法来实现。
`[root@localhost ~]# mount --bind [原有的目录] [新目录]`
比如我的ftp的默认目录是/var/ftp，我想把/mnt/LinG/WinSoft文件夹，映射到/var/ftp目录中，我就如下操作
我们要先在/var/ftp目录中建一个目录
`[root@localhost ~]# mkdir /var/ftp/WinSoft`
然后执行mount命令
`[root@localhost ~]# mount --bind /mnt/LinG/WinSoft /var/ftp/WinSoft`
这样就OK了。 
**8.5、打开vsFTP服务器的日志功能；**
把下面xferlog_file前面的#号对掉，也就是把vsftp的log功能打开，这样我们就能在/var/log目录下查看vsftpd.log。这是vsFTP的日志功能，这对于我们来说是极为重要的。
`xferlog_file=/var/log/vsftpd.log`
**8.6、如何定制欢迎信息；**
如何定制欢迎信息，也就是我们登入有些FTP之后，会出现类似：欢迎您来到LinuxSir FTP，在这里，您会得到最真诚的帮助，如果有什么问题和建议，请来信，多谢。
实现这个并不难，我们可以查看vsftpd.conf文件中，是否有这行。
```
dirmessage_enable=YES
message_file=.message
```
如果没有就加上，如果dirmessage_enable=YES前面有#号，就把#号去掉。其实FTP信息默认的就是.message，所以可以不加 message_file= 来指定。自己指定也行，无所谓的事；
然后我们制定一个.message文件，写上您想要写的东西，比如是.message的内容是如下的：
```
--欢迎您来到LinuxSir FTP！
--在这里，您会得到最真诚的帮助；
--如果有什么问题和建议，请来信，多谢。
```
我们可以用编辑器来写这个.message，我想这个过程就不用说了吧。
然后我们把.message这个文件复制到各个用户的家目录中。比如我的FTP的一个用户是beinan，这个用户所在的家目录是/opt/beinan
我们就要把.message放在/opt/beinan这个目录下。如果系统用户ftp，他的目录就是/var/ftp这个目录，这个是默认的，当匿名用户登入时就访问的是/var/ftp这个目录。我们要让匿名用户能看到欢迎信息。就要把.message放在/var/ftp目录中。其它的用户，也无非就是类似的操作。
**8.7、硬盘空间有限，如何把帐号ftp默认的家目录更改到别处？**
我的硬盘空间有限，如何把帐号ftp默认的路径/var/ftp更改到别处？或者是，我的linux所有的目录都放在/根分区，因为空间紧张，我能否把ftp这个用户的默认路径放到别的分区？？
对于ftp这个用户的管理，我们应该查看/etc/passwd ，然后修改ftp用户那行；
`ftp:x:14:50:FTP User:/var/ftp:/sbin/nologin`
比如我们想把ftp用户的家目录改为/opt/ftp，则要把类似上一行改为 
`ftp:x:14:50:FTP User:/opt/ftp:/sbin/nologin`
然后我们要建立ftp用户的新的家目录；
```
[root@localhost ~]# mkdir /opt/ftp
[root@localhost ~]# chmod 755 /opt/ftp
[root@localhost ~]# chown root:root /opt/ftp
```
如果默认的ftp磁盘空间紧张，我们其实也可以用虚拟路径映射的方法来解决；也就是mount --bind的办法；看情况吧，哪个适合就是最好的方法；
**8.8、如何让vsFTP服务器限制链接数，以及每个IP最大的链接数？**
应该改vsFTPd服务器的配制文件vsftpd.conf，加入下面的两行：
```
max_clients=数字
max_per_ip=数字
```
**举例：**我想让我的vsFTP最大支持链接数为100个，每个IP，最多能支持5个链接，所以我应该在vsftpd.conf中加上如下的两行：
```
max_clients=100
max_per_ip=5
```
改好了配制文件，不要忘记启动vsftp服务器。
**8.9、如何限制传输速度？**
`anon_max_rate=数字 `
注：这是匿名的速度
`local_max_rate=数字 `
注：这是vsFTP服务器上本地用户的速度
注：这个数字的单位是byte，所以我们要计算一下。比如我想让匿名用户和vsFTP上的用户都以80KB下载，所以这个数字应该是1024x80=81920
所以我们要在vsftpd.conf中加入下面的两行
```
anon_max_rate=81920
local_max_rate=81920
```
**8.10、如何有选择的把用户限制在家目录中呢？**
我们要自己建一个文件，在/etc目录中或/etc/vsftpd目录中（假如您的vsftpd配置文件都是放在这个目录的话）；
`#touch /etc/vsftpd.chroot_list`
以beinan和nanbei这两个用户限制在他们所在的家目录中，而其它的FTP用户不做此限制。
在vsftpd.chroot_list这个文件中，把beinan和nanbei添上去就行，注意，每个用户占一行。
```
beinan
nanbei
```
然后改/etc/vsftpd/vsftpd.conf文件，找如下的两行
```
chroot_list_enable=YES
chroot_list_file=/etc/vsftpd.chroot_list
```
如果没有这样的两行，就可以自己添加上去也是一样的。
设置好后，重新vsFTPD服务器。
补充一之补充：如何把系统内所有的FTP用户都限制在家目录中呢？？经juliaugong兄的提示，我查找了vsFTPd的洋文说明，证明这个选项是一刀切的解决所有的用户都能限制在家目录中
我们可以通过更改vsftpd.conf文件，加入如下的一行
`chroot_local_user=YES`
改完配制文件，不要忘记重启vsFTPd服务器；
**8.11、如何让绑定IP到vsFTPd？**
如何让绑定IP到vsFTPd？也就是说，如何让用户只能通过某个IP来访问FTP。其实这个功能很有意思。如果绑定的是内网的IP，外部是没有办法访问的。如果绑定的是对外服务的IP，内网也只能通过对外服务的IP来访问FTP
在vsftpd.conf中加一行，以我的局域网为例，请看第一帖中的操作环境，这样外网就不能访问我的FTP了，内网也可能通过192.168.0.2来访问FTP；
`listen_address=192.168.0.2`
- 由 北南南北 在 2006/01/07 - 18:21 发表
++++++++++++++++++++++++++++++++++++其余参考内容++++++++++++++++++++++++++++++++++++++++++++++++++
1. [ubuntu 10.10 安装vsftpd](http://blog.csdn.net/glorin/article/details/6281061)
2. [ubuntu 10.10下建FTP服务器（用vsftpd)（转）](http://hi.baidu.com/mac_zhong/blog/item/5a8800ca43ed6f26e4dd3be0.html)
3.  [Ubuntu 10.10 配置ftp服务器](http://www.linuxidc.com/Linux/2011-05/35962.htm)
/////////////////问题解决！！！////////////
你不要运行/usr/sbin/下的那个命令
**vsftpd启动是通过脚本的, **也就是service这种
或者用绝对路径, 一般会放在/etc/init.d里
**sudo /etc/init.d/vsftpd start**
** 查看方法：**
**sudo service vsftpd  start**
**sudo service vsftpd status**
**sudo netstat -tulnp | sed -n -r '/20|21/p'**
//////////////
还是不能上传。。。
配置文件中：
anonymous_enable=NO
local_enable=YES
write_enable=YES
local_umask=022
anon_upload_enable=YES
anon_mkdir_write_enable=YES
FileZilla Client工具中上传时还是提示：
响应: 553 Could not create file.
本地用户应该不会有问题啊
查看下, ftp登录时的目录
然后看帐户在这个目录下有没有w权限
///////////////////
