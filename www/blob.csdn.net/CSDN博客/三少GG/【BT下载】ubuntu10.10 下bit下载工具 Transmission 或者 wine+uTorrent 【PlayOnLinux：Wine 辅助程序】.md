# 【BT下载】ubuntu10.10 下bit下载工具 Transmission 或者 wine+uTorrent 【PlayOnLinux：Wine 辅助程序】 - 三少GG - CSDN博客
2011年09月06日 13:05:47[三少GG](https://me.csdn.net/scut1135)阅读数：1839
**一。transmission-2.04修改后能够下载六维啦**
**ubuntu10.10 自带的版本就是transmission 2.04**，按理说应该好用。但是貌似六维空间由于采用纯IPV6服务器，而且官方说明要使用utorrent或者经过特殊处理的transmission。
1. 2.04的deb包(注释掉web.c中这个内容，就可以连接六维。)： http://bt.neu6.edu.cn/517449
2. 修改版transmission2.13增加Deb包 http://bt.neu6.edu.cn/660405 (这个更牛: 坛子里有把transmission2.12中两行代码删除，使transmission能够下载六维的资源。
但是把两行代码删除不是一种好的解决方法，会导致上传速度为0。IPv6这个问题在2.13版中依然存在)
**二。 wine+uTorrent**
1.wine，是一款优秀的Linux系统平台下的模拟器软件，用来将Windows系统下的软件在Linux系统下稳定运行，该软件更新频繁，日臻完善，可以运行许多大型Windows系统下的软件
2.utorrent 3.0 Build 25583  下载 ：http://www.skycn.com/soft/24599.html#downurl
详细介绍：
1.Wine (Wine Is Not an Emulator)[即Wine不仅仅是一个模拟器]是一个在Linux和UNIX之上的,Windows 3.x 和 Windows APIs的实现.
它是一个Windows兼容层**,用通俗的话说,就是一个Windows模拟器**
**（一般软件请安装在/home/../WinSys下，直接在菜单栏中可找到）**
这个层既提供了一个用来从Windows源进出到UNIX的开发工具包(Winelib),
也提供了一个程序加载器,该加载器允许不用任何修改Windows 3.1/95/NT的二进制文件,就可以运行在Intel Unix及其衍生版本下.
Wine可以工作在绝大多数的UNIX版本下,包括Linux, FreeBSD, 和 Solaris. Wine不需要Microsoft Windows,
因为这是一个完全由百分之百的免费代码组成的,可以选择的实现,但是它却可以随意地使用本地系统的DLLs,如果它们是可以被利用的话.
Wine的发布是完全公开源代码的,并且是免费发行的。　　目前 Wine 仍在发展阶段，但是较新的版本可以运行一些著名软件，甚至是 Photoshop CS3!
如何安装：
[http://bt.neu6.edu.cn/497346](http://bt.neu6.edu.cn/497346)
本来想用Transmission的，但是试了好久也没搞定，就用Wine试了一下，居然可以用IPV6下载，而且还挺快的。先安装wine，在ubuntu终端中输入
> 
**sudo apt-get install wine**
然后下载一个utorrent.exe，放到比如说/home/xxx/Downloads文件夹下
然后输入
> 
**  wine utorrent.exe**
就可以运行了，非常简单
如图，最小化后在状态栏里。![Screenshot.png](http://assets1.edubt.cn:8080/attachments/day_100726/10072622051baf94d32988096e.png)
评价： 用过，不过比起WIN下的utorrent而言，还是慢了不少
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
【PlayOnLinux：Wine 辅助程序】
2008-01-18 [Toy](http://linuxtoy.org.sixxs.org/archives/author/admin/) Posted in [Apps](http://linuxtoy.org.sixxs.org/category/apps), [Tools](http://linuxtoy.org.sixxs.org/category/apps/tools)[RSS](http://linuxtoy.org.sixxs.org/archives/playonlinux.html/feed)[Trackback](http://linuxtoy.org.sixxs.org/archives/playonlinux.html/trackback)
关于 Wine 的辅助程序，去年我们介绍了一个 [Wine-Doors](http://linuxtoy.org.sixxs.org/archives/wine-doors.html)。这是另外一个：[PlayOnLinux](http://www.playonlinux.com.sixxs.org/en/)。PlayOnLinux 是使用 Python 写成的图形化前端，主要用来辅助 Wine 在 Linux 中安装面向 Windows 平台的程序和游戏，如 M$ Office 2003。另外，此程序也支持使用[Dosbox](http://linuxtoy.org.sixxs.org/archives/dosbox.html)
 来安装基于 Dos 的程序和游戏。
PlayOnLinux 包括如下特点：
- 允许每个安装的程序有自己的 wineprefix 和目录；
- 能够模拟重启；
- 包含针对游戏的 Directx 安装选项；
- 可以执行显卡测试 (使用 Glxgears、Glxmux 等)；
- 可以更新安装脚本；
- 可以指定 Wine 版本；
- 可以指定 Wine 的 git 版本；
- 能够自动建立桌面快捷方式。
