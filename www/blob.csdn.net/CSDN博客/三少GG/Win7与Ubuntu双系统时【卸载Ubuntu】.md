# Win7与Ubuntu双系统时【卸载Ubuntu】 - 三少GG - CSDN博客
2011年10月19日 16:40:29[三少GG](https://me.csdn.net/scut1135)阅读数：36820
简单点讲，
没有Windows安装盘的朋友准备的，使用MBRFix工具进行修复。先进入cmd命令窗口，然后进入mbrfix工具所在的目录（用cd命令），输入命令 MbrFix /drive 0 fixmbr ，再确认一下。重启以后你会发现，没有了Linux，直接可以进入Windows了。
删除Linux分区：可以用系统自带的 磁盘管理 工具删除，也可以使用 pm 工具。pm工具网上下载
# 具体方法1：
# [Win7与Ubuntu双系统时卸载Ubuntu的方法](http://www.linuxidc.com/Linux/2010-03/25129.htm)
1. 下载[MBRFix](http://www.linuxidc.com/Linux/2007-11/8785.htm)工具，放在c盘，利用命令提示符，进入软件所在目录，cd c:\mbrfix    （cd后面一个空格）
2.输入 MBRFix /drive 0 fixmbr /yes
3.重启，发现直接进入Win 7，现在可以用Win 7的磁盘管理（打不开的话，可以用Win 7优化大师里带的）格式化[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2)所在分区（就是没有盘符的，选定删除卷时会提示是其他系统的数据）了~~~
--------------------------------
如果直接在Win 7里面删除[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2)所在的分区，则由于grub也被删除了，导致无法引导Win 7,可以使用安装光盘在dos命令下执行fdisk
 /mbr修复引导区，下面说说没有安装光盘之类的进入dos的解决办法。
下载[MBRFix](http://www.linuxidc.com/Linux/2007-11/8785.htm),把mbrfix.exe复制到c盘根目录下，在cmd里面运行mbrfix /dirve 0 fixmbr
 /yes, 如果出现error:5,则右击mbrfix.exe选择属性将mbrfix的兼容性改为以管理员身份运行。
然后重启一遍，到Win 7下面在磁盘管理里面将[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2)所占用的分区删除即可。
类似文章：
1.**[Windows和Linux双系统下完美卸载linux ——(适用于Vista/XP/Win7+Ubuntu/Fedora/FreeBSD) （本人采用！！！2011.10.20）](http://blog.163.com/gjx0619@126/blog/static/127408393201151211585600/)**
2011-06-12 11:58:05|  分类：[Linux学习](http://blog.163.com/gjx0619@126/blog/#m=0&t=1&c=fks_087070092095083075084083080095085084086071084094085074087)|字号订阅

装了Windows和linux双系统的朋友，在后期要删除linux是个比较头痛的问题，因为MBR已经被linux接管，本文的目的是如何在windows 和linux双系统下，简单，完美地卸载linux。
## 使用MbrFix.exe卸载Windows双系统Linux
不用Windows系统安装盘，不用安装矮人DOS工具箱也可以很简单地实现卸载Linux。双系统卸载Linux的主要问题是当在windows中将linux 分区直接格式化之后，Grub系统引导程序也会被同时删除，所以导致重启后无法进入Windows 或Linux任何一个系统。
因此卸载linux之前，先修复MBR，然后再删除Linux分区就可以了。而MbrFix.exe 就是这样一个Windows 修复MBR的应用程序软件，MbrFix.exe 不仅支持Windows XP，还支持32位或64位的Windows NT, Windows 2000,Windows Server 2003, Windows Vista, Windows Server 2008, Windows 7 和 Windows PE系统。
使用MbrFix.exe卸载Linux的步骤如下：
1. **下载MbrFix.exe**
首先进入Windows 操作系统，进入www.sysint.no 网站[下载MbrFix.exe](http://www.sysint.no.sixxs.org/products/Download/tabid/536/language/en-US/Default.aspx)，从上往下数第四个软件即是；
右下角点击download，另存为.zip格式。
下载完成后解压zip压缩包，将MbrFix.exe 剪切到C盘(系统盘)；
（如果是64位的系统，请用MbrFix64.exe）
![](http://images.blhekai.com/pictures/2011/03/a-300x194.png)
以管理员身份运行
开始–>运行–>cmd–
```
cd
 \
```
命令进入C盘符–>运行命令：  **```MbrFix /drive 0 fixmbr /yes```**–>Enter回车；
（如果是Vista系统，运行命令：  **```MbrFix /drive 0 fixmbr /vista /yes```**–>Enter回车）
（如果是Win7系统，运行命令：  **```MbrFix /drive 0 fixmbr /win7 /yes```**–>Enter回车）
![](http://images.blhekai.com/pictures/2011/03/b-300x193.png)
这样MBR修复就完成了，没有任何提示（没有消息就是最好的消息）。接下来直接删除linux分区就可以了。
2. **格式化删除 Linux分区**
以Win7系统为例：点击“计算机”右键–>管理–>磁盘管理–>看到几个没有盘符名称的分区(看准哦，不要将Windows 分区给误删了。)–>右键–>删除逻辑分区。
![](http://images.blhekai.com/pictures/2011/03/c-300x234.jpg)
重启电脑，Grub 已经不复存在，双系统Linux 卸载完成，电脑启动自动进入Windows系统….
转[http://www.blhekai.com/windows-win7-vista-uninstall-linux-ubuntu.html](http://www.blhekai.com/windows-win7-vista-uninstall-linux-ubuntu.html)
2.
# [windows7下安装ubuntu做出双系统后，单独卸载ubuntu的实际经验（windows7环境下操作，步骤简单）](http://bbs.ylmf.net/forum.php?mod=viewthread&tid=1507962&extra=&page=1)
本人在windows 7 基础上安装了ubuntu 双系统
使用一段时间的ubuntu后，虽然感觉不错，但是为了回收划分给ubuntu的10G硬盘空间（总共才120G），需要 单独卸载ubuntu
由于双系统是grub引导，所以单独在PE下格式化linux的硬盘分区，会造成grub无法引导windows 7 的情况——————也就是开机进不去系统，只有黑色grub画面了
卸载ubuntu的方法，百度可以出来，但是我实际操作了，所以分享一下，供大家研讨
卸载方法很多，说明以下其中的两种：
1、有windows7安装盘的，可以pe环境格式化linux分区后，利用windows7盘修复mbr主引导记录，进去windows7    
    -----------------不详细讲，有安装盘了就上网随便百度，简单
2、身边什么盘都没有的朋友，我就是这种情况，因为假期在家没工具
     可以下载fixmbr工具，在windows7系统环境下操作，重建修复mbr引导，具体如下      ----------------------------好处，windows7环境操作，重启即可
示例：
1.我要修复C盘的xp引导（NT），下载fixmbr工具，放在c盘，利用命令提示符，进入软件所在目录，cd c:\fixmbr    （cd后面一个空格）
2.输入 MBRFix /drive 0 fixmbr /yes             （即  MBRFix空格/drive空格0空格fixmbr空格/yes  ）
3.重启，发现可以直接进入windows7画面了，没有grub画面了
4.然后可以格式化linux分区了（应该会吧）
5.ubuntu单独卸载成功
这样做的好处就是可以在windows7环境下操作，因为我没有系统盘，无需任何复杂工具
我的是windows7 亲自操作，xp  vista理论可以，请自测
刚刚卸载成功，很高兴，留给需要的朋友
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**具体方法2：**
一．安装系统，难免要装装卸卸，进进出出，先不写如何安装，告诉大家怎么快速简 单的卸载。 绝对绝对绝对不要仅仅删除 ubuntu 分区－－这样你将连 windows 也无法启动， 因为指向你启动记录的信息也将丢失。你应当按如下简单几步来进行：
(1) 启动你的 windows 系统
(2)  点击这个链接下载 Mbrfix      这种方法中下载的 fixmbr 工具必须是最新版，不然在 win7 下用不起来
(3)  下载之后，将其解压到你的系统目录（一般是 c:\）
(4) 打开 windows 命令行（开始/附件/命令行）
(5)   键入： cd \ [按回车] mbrfix /drive 0 fixmbr /yes [按回车]
(6) 关掉命令行窗口
(7)  将你的 Ubuntu live CD 放入光驱，重启进入 ubuntu 桌面
(8)   打开 Ubuntu 终端（应用程序/附件/终端）
(9)  在终端输入： sudo gparted [按回车] 打开磁盘管理器。你将要：a- 删除所有的非 windows分区 b- 将你的 windows 分区调整为更大（这步可选－－你也可以保留 ubuntu 分区， 以便下次重返 Ubuntu）
(10)  当你删除完 Ubuntu 分区之后，重启电脑，在启动系统之前取出光盘如果一切顺利的话，你的电脑将自动启动 windows。要注意，你调整磁盘大小之后首次 启动 windows，windows 将运行磁盘检查。 这是正常的，没事。 我是这样的：直接删除，然后用系统安装光盘（如果是 xp 的话）进入修复模式， 在命令行下用命令：fixmbr 就行了。分区调整用 pq 搞定。
 一，划分磁盘分区 已经安装好了 Windows 7，划分出一个分区备用，在磁盘管理里面删除盘符，空间大小可以 根据自己磁盘空间的大小合理安排分区。

其余参考方法：
1.  不推荐！！！
[**冰古**](http://bingu.net.sixxs.org/536/uninstall-ubuntu/)原先使用的是Windows+ubuntu双系统，最近硬盘空间不够用，所以决定把近来都没怎么使用的ubuntu先卸载了。
如果单单把ubuntu所在的分区格式化，这样将导致Windows也不能正常的启动，参考了一下谷歌的建议，方法如下：
1.在Windows下，下载[MbrFix](http://bingu.net.sixxs.org/wp-content/images/MbrFix.7z)，并解压；打开“命令提示符”，cd到MbrFix目录里，输入如下命令MbrFix
 /drive 0 fixmbr，在对话提示中输入y后确定。如无意外，这时你重启后可以看到不再有多系统的启动选择，而是直接进入了Windows。
2.删除ubuntu所在分区。这里的方法很多：使用Windows下的分区软件、使用sudo gparted、使用ubuntu安装盘等等。冰古的手上只有ubuntu的安装盘，而那个什么sudo gparted又不熟而且不想下载分区软件，于是这里我是使用了ubuntu安装盘来删除ubuntu分区的。修改BIOS使用cd启动，塞入ubuntu安装盘进行ubuntu的安装，到达分区步骤时，删除ubuntu所在分区并格式化为fat32格式（会提示出错，继续就行了。格式化自己小心啦），执行后即可退出ubuntu的安装。
嗯，这下又可以下多几部美剧了 ![:smile:](http://bingu.net.sixxs.org/wp-includes/images/smilies/icon_smile.gif)
2. 可尝试！
“有个简单的方法：
用DiskGenius，直接把ubuntu的分区全删了，然后，用DiskGenius重写主引导记录（MBR），然后保存。我用这种办法在win7下卸过很多回linux了。 ”
[双系统下卸载Ubuntu或卸载Windows的方法](http://hi.baidu.com/pcghost/blog/item/0f07c30ee62c68f4aa6457fe.html)
2011年06月29日 星期三 下午 07:58
    装了Windows和Ubuntu 双系统，现在又想卸载ubuntu或者windows，很多人不知道怎么卸载？有的人卸载了一个系统就不能进另外一个系统了。其实要卸载一个系统，方法非常简单。下面分两种情况来介绍
1. 卸载ubuntu，保留windows
一般装了windows和ubuntu双系统后，默认情况下都是使用grub引导双系统启动的，grub是在ubuntu的磁盘分区中的，所以如果我们删除了ubuntu分区，就不能引导进入windows了。正确的卸载方法如下
步骤一： 删除ubuntu所在磁盘分区
如果你使用的是windows 7，直接用系统自带的磁盘管理来删除ubuntu分区即可，步骤为右键点击【计算机】，选择【管理】>【磁盘管理】，找到ubuntu分区并删除；
如果你使用的是windows XP，推荐使用diskgenius删除ubuntu分区。在XP中如果采用上面的方法会有风险，因为在XP中用磁盘管理来删除ubuntu分区时会删除它所在的整个扩展分区(当然如果你的ubuntu独占一个主分区，那用磁盘管理来删是没有问题的)。万一你犯了这个错误，可以看我以前的一篇文章《[误删硬盘分区不用慌，轻松一招全恢复](http://hi.baidu.com/pcghost/blog/item/8460c087b49df32866096e2e.html)》，还来得及补救。
步骤二：重建主引导记录（MBR）
使用diskgenius即可，安装diskgenius，在“硬盘”菜单中有一项“重建主引导记录”，轻轻点一下就可以了，很简单吧。
（另外提供一点小技巧，如果你删除ubuntu后就直接重启，现在进不去windows了，可以采取补救措施：只要使用一张windows系统安装光盘启动电脑，进入“恢复控制台”中，执行命令fixmbr即可修复。）
此时重启电脑就可正常进入windows了，不会出现ubuntu的启动选项了。
2. 卸载windows，保留ubuntu
估计没什么人会这样做吧。不过做起来也是很简单的。
步骤一
在ubuntu中把windows所在磁盘分区删掉（这个太简单，不必多说了吧）。
步骤二
把windows的grub引导记录去掉，这个有两种方法：（1）找到grub.cfg(如果是用的比较老的系统版本，应该是找menu.lst)，编辑并删除windows的引导项；（2）另一种方法就是在终端里执行命令“sudo update-grub”，会自动重建grub列表。
重启电脑，在grub引导启动菜单页面，windows的启动项就已经消失了吧。
[http://hi.baidu.com/pcghost/blog/item/0f07c30ee62c68f4aa6457fe.html](http://hi.baidu.com/pcghost/blog/item/0f07c30ee62c68f4aa6457fe.html)
关键词：windows ubuntu 双系统， 卸载ubuntu， 卸载windows， 主引导记录(MBR)， diskgenius
