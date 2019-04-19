# Linux 内核编译步骤及配置详解 - xqhrs232的专栏 - CSDN博客
2017年03月08日 15:18:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：221
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.cnblogs.com/xiaocen/p/3717993.html](http://www.cnblogs.com/xiaocen/p/3717993.html)
相关文章
1、[Linux内核配置与编译](http://blog.csdn.net/david_xtd/article/details/26502709)----[http://blog.csdn.net/david_xtd/article/details/26502709](http://blog.csdn.net/david_xtd/article/details/26502709)
前言
   Linux内核是操作系统的核心，也是操作系统最基本的部分。
   Linux内核的体积结构是单内核的、但是他充分采用了微内核的设计思想、使得虽然是单内核、但工作在模块化的方式下、并且这个模块可以动态装载或卸 载；Linux负责管理系统的进程、内存、设备驱动程序、文件和网络系统，决定着系统的性能和稳定性。如是我们在了解Linux内核的基础上根据自己的需 要、量身定制一个更高效，更稳定的内核，就需要我们手动去编译和配置内核里的各项相关的参数和信息了。
注意、如果两个内核模块的版本不完全相同是不可以跨版本使用的。
正文
   首先我们要去获得Linux内核的压缩文件、获得的路径很多了、最直接的就是去内核官网获得了(http://www.kernel.org),也可以到各镜像站上去下载、这里就不再说明了。
注意：/usr/src；一般而言、我们制做linux内核的时候源码一般放在这个路径下、并且有一个链接叫linux什么的、所以展开时应该放在这个路径下。
   我这里下载有一个3.13.2版本的内核、展开时指定一个展开路径就可以了，文件比较大、70多M、可能需要点时间的。
   # tar xf linux-3.13.2.tar.xz -C /usr/src/
[](http://s3.51cto.com/wyfs02/M02/22/4E/wKioL1MapH-Bjd82AABmpA1PjrU567.jpg)
   展开之后在/usr/src/这个目录下就看到了生成一个linux-3.13.2的文件了
   为以后使用方便、我们给他创建一个链接：
   # ln -sv linux-3.13.2 linux
[](http://s3.51cto.com/wyfs02/M01/22/4D/wKiom1MapNKh74XPAAB5IEKw_vI902.jpg)
   我们cd到linux目录里面看一下里面的文件看：
   # cd /usr/src/linux
[](http://s3.51cto.com/wyfs02/M00/22/4E/wKioL1MapOHQgrpoAACzmJ7T90Q230.jpg)
   里面的目录我这里就不一一介绍了、有兴趣的朋友可以去了解一下哦。
到下一步就是去配置内核了、这步配置内核的方法有多种、每一个make就是一种方法、我们只在选一种就可以了：
```
make config：遍历选择所要编译的内核特性
   make allyesconfig：配置所有可编译的内核特性
   make allnoconfig：并不是所有的都不编译，而是能选的都回答为NO、只有必须的都选择为yes。
   make menuconfig：这种就是打开一个文件窗口选择菜单，这个命令需要打开的窗口大于80字符的宽度，打开后就可以在里面选择要编译的项了
   下面两个是可以用鼠标点选择的、比较方便哦：
   make kconfig(KDE桌面环境下，并且安装了qt开发环境)
   make gconfig(Gnome桌面环境，并且安装gtk开发环境)
   menuconfig：使用这个命令的话、如果是新安装的系统就要安装gcc和ncurses-devel这两个包才可以打开、然后再里面选择就可以了、通这个方法也是用得比较多的：
```
[](http://s3.51cto.com/wyfs02/M02/22/4D/wKiom1MapVCCGRSaAAMP-pFlTno636.jpg)
   如果想方便点又不想用别的方法那就复制当前系统上的/boot/config-版本-平台，这个文件到/usr/src/linux/.config覆 盖这个文件，再改一下里面的配置信息；哪些用得着就保留、哪些用不着就删除了都可以、有什么新的也可以加进来、这就是按自己的需求来配置了；
[](http://s3.51cto.com/wyfs02/M00/22/4E/wKioL1MapU-xI3JzAAGGyCZYZIg932.jpg)
   注意：如果编译是在远程连接上的话、make会出问题的、如果远程连接断开的话那运行的进程是不是也都断开了、这个make的进程一定是当前系统上运行的子进程、有些一旦父进程挂了、子进程也将不复存在、俗话说：皮之不存毛将焉附，是这个道理吧；
   所以我们使用一个叫screen这个命令来操作，没安装自己安装上就是了、可以在一个窗口上启动n个虚拟的桌面，即使退出了也会保留之前的内容，切换进screen就可以看到窗口标题栏上的screen标志了：
[](http://s3.51cto.com/wyfs02/M02/22/4D/wKiom1Mapd_w1zGTAAEcrGcM8i0433.jpg)
   # screen -ls:可以查看当前启用了多少个screen
   按Ctrl+a，松开再按d可以隐藏screen桌面
   因为编译内核需要比较长的时间、所以我们在screen上编译、然后再把screen桌面隐藏就可以了；
   重新接入screen：
   # screen -r 跟上screenID就可以了
   然后我们就开始编译吧；(我这里编译差不多用了两小时、make的时候我不指定cpu的核心数、默认就是用一个核来编译、所以很久)
   # make
[](http://s3.51cto.com/wyfs02/M00/22/4D/wKiom1MappTS0l7PAADEZe4oUD8150.jpg)
   编译好了之后我们就可以进行下一步了：
   # make modules_install
   这步完了之后你可以查看一下/lib/modules/目录下就会生成一个以版本号命名的一个文件模块了
[](http://s3.51cto.com/wyfs02/M00/22/4E/wKiom1Mawq7yelmIAABNeTaicBs308.jpg)
    modules之后再往下走、敲命令吧
   # make install
   安装完之后会在/boot/目录下生成一个内核文件vmlinuz-3.13.2、还有几个跟你当前编译的版本一样的文件、可以ls去看一下：
   # ls /boot/
[](http://s3.51cto.com/wyfs02/M00/22/4F/wKioL1MaxPbj4wuFAAD2w1YRV1Q097.jpg)
   如果没出什么问题到这里我们应该算是编译好了一个新内核了；可以到grub.conf配置文件时看一下：
   # vim /boot/grub/grub.conf
[](http://s3.51cto.com/wyfs02/M00/22/4E/wKiom1Maxdiy9RmsAAGyhgb-QWs068.jpg)
   好了、要不我们重启一下看看、OK、重启吧！如果顺利的话就可以进到内核选择界面了，选项我们编译的新内核就OK了，好了、到这就结束了！
[](http://s3.51cto.com/wyfs02/M02/22/4F/wKioL1Maxz2ge3GNAAC6wW-iFY0072.jpg)
   总结一下我们的安装步骤：
```
1、获取内核源码，解压至/usr/src
           # tar xf linux-3.13.5.tar.xz -C /usr/src
           # ln -sv /usr/src/linux-3.13.5  /usr/src/linux
       2、配置内核特性(选择一种方法就可以了)
           make config：遍历选择所要编译的内核特性
           make allyesconfig：配置所有可编译的内核特性
           make allnoconfig：并不是所有的都不编译
           make menuconfig：这种就是打开一个文件窗口选择菜单
           make kconfig(KDE桌面环境下，并且安装了qt开发环境)
           make gconfig(Gnome桌面环境，并且安装gtk开发环境)
       3、编译内核
           # make [-j #] ： #号最多为CPU物理核心总数的两倍，这样会快点哦            
       4、安装内核模块
           # make modules_install
       5、安装内核
           # make install
       6、验正并测试
           # cat /boot/grub/grub.conf
           查看新内核是否已经添加, 而后重启系统并测试
```
结束：
   其实编译一个新的内核好像也不是很难是吧、跟我们之前源码编译的httpd的步骤差不多、也就是第一步麻烦点、细心点就OK了，当然如果有什么不对的地方还望各路诸侯多多提点！
