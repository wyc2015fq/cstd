# Ubuntu操作系统安装使用教程 - 三少GG - CSDN博客
2010年04月30日 19:14:00[三少GG](https://me.csdn.net/scut1135)阅读数：952标签：[ubuntu																[firefox																[windows																[linux																[debian																[flash](https://so.csdn.net/so/search/s.do?q=flash&t=blog)](https://so.csdn.net/so/search/s.do?q=debian&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
[**http://www.williamlong.info/archives/1905.html**](http://www.williamlong.info/archives/1905.html)
随着微软的步步紧逼，包括早先的[Windows黑屏计划](http://www.williamlong.info/archives/1532.html)、[实施](http://www.williamlong.info/archives/1538.html)，[逮捕番茄花园作者](http://www.williamlong.info/archives/1468.html)并[判刑](http://www.williamlong.info/archives/1904.html)，种种迹象表明，中国用户免费使用盗版Windows的日子将不会太长久了，那么这个世界上有没有即免费又易用的操作系统呢？答案是有，那就是Ubuntu操作系统。
　　Ubuntu是一个流行的Linux操作系统，基于Debian发行版和GNOME桌面环境，和其他Linux发行版相比，Ubuntu非常易用，和Windows相容性很好，非常适合Windows用户的迁移，预装了大量常用软件，中文版的功能也较全，支持拼音输入法，预装了Firefox、Open Office、多媒体播放、图像处理等大多数常用软件，一般会自动安装网卡、音效卡等设备的驱动，对于不打游戏不用网银的用户来说，基本上能用的功能都有了，在Windows操作系统下不用分区即可安装使用，就如同安装一个应用软件那么容易，整个Ubuntu操作系统在Windows下就如同一个大文件一样，很容易卸载掉。下面我就介绍一下Ubuntu操作系统安装使用的方法，供Ubuntu新手参考，希望能起到Linux扫盲的作用。
**下载Ubuntu**
　　Ubuntu有三个版本，分别是桌面版（Desktop Edition），服务器版（Server Edition），上网本版（Netbook Remix），普通桌面电脑使用桌面版即可，下载地址[请点这里](http://www.ubuntu.com/getubuntu/download)，32位CPU请选择32bit version，上网本则可下载Netbook Remix，目前Ubuntu已经占据三分之一的上网本市场，仅次于WIndows
 XP系统。Google的Chrome操作系统强有力的对手就是Ubuntu Netbook Remix。
　　目前最新的版本是9.04版，下载后的文件名是ubuntu-9.04-desktop-i386.iso，大小是698M，通过迅雷下载非常快，大约半个小时左右可以下载完毕。
**安装Ubuntu**
　　在Windows下可以不用重新分区，直接像安装一个应用程序那样安装Ubuntu，安装方法是，先使用一个虚拟光驱（[例如微软的Windows虚拟光驱](http://www.williamlong.info/archives/991.html)）装载ubuntu-9.04-desktop-i386.iso文件，然后运行根目录下的wubi.exe，运行前要**将本地磁盘的名字都修改为英文名**，否则会出现错误信息“UnicodeEncodeError:
 'ascii' codec can't encode characters in position 0-3: ordinal not in range(128)”而无法运行。
　　运行之后，会出现如下界面，选择“Install inside Windows”即可在Windows下直接安装而无需分区。
![Ubuntu Wubi](http://writeblog.csdn.net/upload/1905_1.jpg)
　　接着出现下面的安装界面，选择一个磁盘，然后将语言选择为“Chinese（Simplified）简体中文”，Installation size为Ubuntu环境的总共磁盘大小，然后是登录用户名和密码，设置好了以后就点安装继续。
![Ubuntu Wubi](http://writeblog.csdn.net/upload/1905_2.jpg)
　　后面的安装操作很简单，不需要手动干预就可以直接安装好整个操作系统，大部分的硬件驱动都可以自动安装好。提示安装完毕后，重启系统，就可以使用Ubuntu了。
**自动登录Ubuntu**
　　Ubuntu默认是每次登录都是要输入用户名和密码的，这是基于安全方面的考虑，不过对于桌面版，大家都习惯自己的电脑能自动登录，类似Windows XP系统那样，通过一些设置可以实现Ubuntu自动登录。设置的方法是：点击“系统”—“系统管理”—“登录窗口” （需要输入管理员密码），然后在“安全”选项页—勾选（启用自动登录），然后在下拉列表里选择自己的用户名。之后Ubuntu就能够自动登录了。
**开机自动运行程序**
　　类似Windows的启动菜单，在Linux也可以实现开机自动运行一些命令，比较简单的方法是修改 /etc/rc.local 文件，将需要执行的命令添加进去。
**桌面设置**
　　Ubuntu的桌面，默认有两个任务栏，一个在上面，一个在下面，通常习惯Windows的用户喜欢将上面的移到下面，Ubuntu的面板无法拖动，在上面点右键后，可以让其显示在屏幕下端。
　　桌面背景设置和Windows很类似，在“桌面”上点右键，点更改桌面背景，就可以进行修改设置。
**修改root密码**
　　Ubuntu默认的用户并不是root，我们可以通过操作来使用root这个超级管理员帐号，以获得更大的权限。先打开终端，然后执行下面的语句
　　sudo passwd root
　　就可以修改超级管理员root的密码，之后就可以使用su命令切换到root用户来执行某些更高权限的操作。
**Hosts修改**
　　在Windows下，我们上Twitter等网站都需要修改hosts文件，在Linux下也有hosts文件，文件位于/etc/hosts，使用root用户可以编辑修改这个文件，主机名和IP的格式与Windows的完全相同，例如：
　　127.0.0.1 localhost
**在Ubuntu下安装软件**
　　Ubuntu下的软件安装有几种方式，常用的是deb包的安装方式，deb是debian系列的Linux包管理方式，ubuntu属于debian的派生，也默认支持这种软件安装方式，当下载到一个deb格式的软件后，直接在界面上就可以安装。
　　另一种常见的安装方式是源代码编译安装，很多软件会提供了源代码给最终用户，用户需要自行编译安装，先使用tar将源代码解压缩到一个目录下，然后进入这个目录，执行以下三条命令：
　　./configure
　　make
　　sudo make install
　　执行完成后，即可完成软件的编译和安装。
　　还有一种方式是apt-get的安装方法，APT是Debian及其衍生发行版的软件包管理器，APT可以自动下载，配置，安装二进制或者源代码格式的软件包，因此简化了Unix系统上管理软件的过程。常用的安装命令是：
　　sudo apt-get install 软件名
　　sudo apt-get remove 软件名
**Firefox浏览器的更新**
　　Ubuntu安装完成后会自动安装一个Firefox浏览器，遗憾的是这个Firefox版本通常较低，例如Ubuntu 9.04会安装Firefox 3.0，不过我们可以想办法下载最新的Firefox覆盖掉老版本Firefox，具体方法是，先上Firefox官方网站下载最新的Linux版本Firefox，然后将其解压缩到某一个目录下，例如firefox目录，进入终端，到这个目录的父目录，执行下面的语句：
　　sudo cp -r firefox /usr/lib/firefox-3.5.2
　　sudo mv /usr/bin/firefox /usr/bin/firefox.old
　　sudo ln -s /usr/lib/firefox-3.5.2/firefox /usr/bin/firefox-3.5.2
　　sudo ln -s /usr/bin/firefox-3.5.2 /usr/bin/firefox
　　之后就可以将Firefox成功替换为最新的Firefox 3.52版本，未来的Firefox更新也可以使用这种方法。
**Firefox的Flash问题**
　　经过我的实际测试，Ubuntu自动安装的Flash插件swfdec存在很多问题，在Firefox中，很多网页的Flash无法显示，包括Google音乐和开心网等，因此建议使用下面两条语句将其卸载。
　　sudo apt-get remove swfdec-mozilla
　　sudo apt-get remove swfdec-gnome
　　之后可安装官方的Adobe Flash Player的Linux版，下载地址是： [http://get.adobe.com/flashplayer/](http://get.adobe.com/flashplayer/)
　　安装完成后，还要解决中文乱码问题，解决方法是执行下面语句：
　　sudo cp /etc/fonts/conf.d/49-sansserif.conf /etc/fonts/conf.d/49-sansserif.conf.bak
　　sudo rm /etc/fonts/conf.d/49-sansserif.conf
　　之后，Firefox的Flash就完全正常了，在Firefox中访问开心网等Flash网站，显示都正常。
**安装常用软件**
　　介绍完了安装的方法和Firefox，下面就可以去各个网站下载一些常用的Linux软件来安装了，下面是我整理的一些常用的Linux软件列表：
　　Linux QQ：访问[这个地址](http://im.qq.com/qq/linux/)，下载deb文件安装，可以在Linux下玩腾讯QQ。
　　防火墙 firestarter： 使用 sudo apt-get install firestarter 安装。
　　杀毒软件 AntiVir: 虽然Linux下的病毒很少，但对于新手还是有必要安装一个杀毒软件，访问[这个地址](http://www.free-av.com/)可以下载免费版的AntiVir杀毒软件，这个软件我曾经在《[五个最佳的防病毒软件](http://www.williamlong.info/archives/1373.html)》中介绍过。
　　rpm 转 deb 工具： 使用 sudo apt-get install alien 安装
　　JAVA环境安装： JRE的安装 sudo apt-get install sun-java6-jre ，JDK的安装 sudo apt-get install sun-java6-jdk
　　eclipse安装： 先到[这个地址](http://www.eclipse.org/downloads/)下载最新的eclipse，然后使用tar xvfz eclipse-php-galileo-linux-gtk.tar.gz -C /opt 解压缩后就可以使用。
![eclipse安装](http://writeblog.csdn.net/upload/1905_3.jpg)
　　Picasa 3 for Linux安装： 访问[这个地址](http://picasa.google.com/linux/)，下载后直接安装。
　　Google Earth安装： 在[这里](http://earth.google.com/download-earth.html)下载最新版本的Google Earth，下载下来是个BIN文件，在图形界面上右击 GoogleEarthLinux.bin，在“权限”选项卡中勾选“允许以程序执行文件”，如下图。
![Google Earth安装](http://writeblog.csdn.net/upload/1905_4.jpg)
　　之后在终端上执行 ./GoogleEarthLinux.bin 即可安装。
![Google Earth安装](http://writeblog.csdn.net/upload/1905_5.jpg)
**安装LAMP环境**
　　Ubuntu的桌面版也可以安装[LAMP](http://www.williamlong.info/archives/1908.html)（Linux + Apache + MySQL + PHP）环境，这里我介绍一个最简单的方法，就是使用XAMPP，这个项目我曾经在《[常见的WAMP集成环境](http://www.williamlong.info/archives/1281.html)》中介绍过，XAMPP不但支持Windows，还支持Linux，[在其网站](http://www.apachefriends.org/zh_cn/index.html)下载之后，运行下面两条命令：
　　tar xvfz xampp-linux-1.7.2.tar.gz -C /opt
　　/opt/lampp/lampp start
　　就可以启动[LAMP](http://www.williamlong.info/archives/1908.html)环境，XAMPP是功能全面的集成环境，软件包中包含Apache、MySQL、SQLite、PHP、Perl、FileZilla FTP Server、Tomcat等等，很适合开发环境使用。
**安装程序添加程序菜单和桌面**
　　有些程序是直接解压缩安装的，因此不会添加“应用程序”的菜单项，我们可以手动将其添加菜单项，具体方法是，打开“系统”—“首选项”—“主菜单”，新增即可。
　　添加桌面快捷方式是，在桌面上点右键，创建启动器。这个“启动器”就是Windows里面的“快捷方式”。
　　将“应用程序”的菜单项创建到桌面快捷方式的方法是，在“应用程序”的菜单项上单击鼠标右键，选择“将此启动器添加到桌面”或“将此启动器添加到面板”，就可以了。
　　以上是我在安装使用Ubuntu时整理的教程，如果你还没有安装过这个操作系统，相信在这个教程的指引下，你就能轻松驾驭这个Linux系统，如果你在使用Ubuntu的过程中也有一些经验和心得，请留言与大家分享。
