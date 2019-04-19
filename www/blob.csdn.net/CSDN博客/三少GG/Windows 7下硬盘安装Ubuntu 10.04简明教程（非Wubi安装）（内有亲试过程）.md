# Windows 7下硬盘安装Ubuntu 10.04简明教程（非Wubi安装）（内有亲试过程） - 三少GG - CSDN博客
2010年12月16日 10:37:00[三少GG](https://me.csdn.net/scut1135)阅读数：3086标签：[ubuntu																[windows																[2010																[终端																[linux																[xp](https://so.csdn.net/so/search/s.do?q=xp&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
[http://antknox.blog.163.com/blog/static/11590881720107305465152/](http://antknox.blog.163.com/blog/static/11590881720107305465152/)
Windows 7下硬盘安装Ubuntu 10.04简明教程（非Wubi安装）
[日期：2010-05-03]
来源：Ubuntu社区 作者：topeak
相信不少朋友和我一样，都安装了Windows7系统。 随着电脑软硬件的快速更新换代，WindowsXP已经告别了历史的舞台，Windows7成了大多数Windows用户的选择。但不管Windows怎 么变，我们还是一样喜爱Ubuntu系统。操作系统变了，我们的安装方式也变了，以前的XP下的安装方式已经不行。
下面本人将介绍适合新 手、简单易懂的Windows 7下硬盘安装Ubuntu10.04简明教程：
1、在Windows7中下载并安装[EasyBCD_1.7.2](http://www.linuxidc.com/Linux/2007-12/10060.htm)
2、将 Ubuntu-10.04-desktop-i386.iso(注意自己是否是这个版本，名字！！！）文件复制到C盘根目录，用压缩软件打开，再打开casper目录，把initrd.lz、 vmlinuz二个文件解压到C盘根目录下。
3、运行EasyBCD_1.7.2进入操作窗口后，
点Add/Remove Entries
再点右下边的NeoGrub
再点Install NeoGrub
再点Save
选中NeoGrub Bootloader，再点Configure
（以上操作看似复杂其实简单，具体操作请看图片）
![clip_image001](http://img.ph.126.net/csAewE6084_naxhaki7YJQ==/3264828254868322188)
4、在出现menu.lst记事本窗口中输入以下内容并保存：
root
kernel /vmlinuz iso-scan/filename=/Ubuntu-10.04-desktop-i386.iso boot=casper splash
initrd /initrd.lz
boot
(注意自己是否是这个版本，名字！！！filename= 自己所下载版本名字）
5、电脑重启后，就会发现启动菜单会多了一项NeoGrub Bootloader，选择后会进入Ubuntu 10.04光盘系统。按快捷键运行（Alt+F2）,在运行框内输入：
sudo umount -l /isodevice   （注意-l 和/isodevice 之间的空格）
最后点桌面上的Ubuntu安装图标及可开始正常安装
安装成功后，重启电脑会直接进入Ubuntu 10.04，打开终端输入以下命令：
sudo update-grub
再次重启及可出现Windows7引导菜单
希望以上教程能帮助到大家，如果你按以上方法安装成功了，请回贴支持一下，让更多的朋友都能看到本贴，帮助到更多需要在Windows7下硬盘安装Ubuntu 10.04的朋友们，谢谢大家了。
下面是亲试过程：
准备工作：
硬件准备：
硬盘划分空间…. 15G空白分区
软件准备：
EsayBCD
ubuntu系统 镜像
 相关软件：可以在  电驴资源贴 找到
链接:
主题: 系统安装-Windows 7下硬盘安装Ubuntu 10.04简明教程（非Wubi安装）(内有亲试过程)-所需的软件
[http://www.verycd.com/groups/antknox/1163181.topic](http://www.verycd.com/groups/antknox/1163181.topic)
附录：Grant
1. EasyBCD
EasyBCD : 实现Vista与Linux、BSD、Mac OS X等的多重启动
　　EasyBCD是一款免费软件，目前最高版本为EasyBCD 1.7.2。
　　EasyBCD 能够极好地支持多种操作系统与Windows Vista结合的多重启动，包括Linux、 Mac OS X、 BSD等，当然也包括微软自家的Windows 2000/XP。任何在安装Windows Vista前其能够正常启动的系统，通过EasyBCD，均可保证其在安装Windows Vista后同样能够启动。同时，在设置方面极为简单，完全摆脱BCDEdit的繁琐冗长命令，用户只需选择相应的平台与启动方式(如Linux下的Grub或LILO)，即可完成。
　　此外， EasyBCD 还提供了“自动备份MBR (Automated MBR Backup)”的选项，这样，用户可以保存系统启动选项的每次改动，并在出现启动故障时选择合适的备份恢复。
下载地址: [http://www.onlinedown.net/soft/58174.htm](http://www.onlinedown.net/soft/58174.htm)
2.buntu 版本：Ubuntu 10.04.1 LTS
[http://www.ubuntu.com/desktop/get-ubuntu/download](http://www.ubuntu.com/desktop/get-ubuntu/download)
======================================================================================================
======================================================================================================
"
重新启动你的电脑吧，选择进入安装ubuntu，进行安装，一定要记得要手动选择分区。
还需要注意的一点是：你原来双系统（win7+xp），要分清楚那个是第一个分区，也就是说系统的boot从那个盘中读出来的，那么你就把这些东西放到那个盘里面就可以了。一般的情况下，先装xp再装win7的都是xp在第一个分区上面的。
**二，安装ubuntu**
重启机器。在启动项选择ubuntu. 进入Ubuntu桌面。打开终端，输入代码:sudo umount -l /isodevice这一命令取消掉对光盘所在 驱动 器的挂载，否则分区界面找不到分区。
双击安装Ubuntu，根据提示进行安装即可。再提醒一遍要选择手动选择分区。
**三，修复启动项**
重新启动ubuntu之后，结果发现选择windows 7的启动项，又进入到Ubuntu的安装界面。下面来说明如何修复，进人Ubuntu系统，打开“应用程序---附件--终端”，命令如下sudo gedit /etc/default/grub
修改GRUB_TIMEOUT="10" 
然后在终端中输入sudo update-grub
update 命令会自动找到 windows 7 启动项。并且自动更新 /boot/grub/grub.cfg 文件。进入“位置---计算机---win7系统盘“
将安装Ubuntu系统时复制到C盘的那些文件(vmlinuz，initrd.lz，grldr，grldr.mbr，grub.exe，menu.lst，boot.ini，ubuntu.iso)全部移到回收站。特别是ISO文件，如果不删除，重启进入win7后，又将进入Ubuntu的安装界面。
好了，到这里，可以进入Linux下面看看了。
"
solution2: (reference)
[http://www.ivemusic.net/archiver/?tid-21892.html](http://www.ivemusic.net/archiver/?tid-21892.html)
### Win7下硬盘安装Ubuntu 10.10简明教程（非Wubi）
[i=s] 本帖最后由 gensaya 于 2010-11-21 22:44 编辑 [/i]
基于此帖修改：[url]http://forum.ubuntu.org.cn/viewtopic.php?f=77&t=268932[/url]
在此对原作者表示感谢。
[quote]
前言：相信不少朋友和我一样，都安装了Win7系统。随着电脑软硬件的快速更新换代，XP已经告别了历史的舞台，Win7成了大多数Windows用户的选择。但不管Windows怎么变，我们还是一样喜爱Ubuntu。操作系统变了，我们的安装方式也变了，以前的XP下的安装方式已经不行。
下面本人将介绍适合新手、简单易懂的硬盘安装方法：
[/quote]
0、下载ubuntu10.10镜像。本文以 ubuntu-10.10-desktop-i386.iso 为例。 注①
1、下载EasyBCD，并在Win7中安装。本文以 EasyBCD 2.0.2 为例。
2、将ubuntu-10.10-desktop-i386.iso文件复制到C盘根目录，用压缩软件打开，再打开casper目录，把initrd.lz、vmlinuz二个文件解压到C盘根目录下。
3、运行EasyBCD进入操作窗口后，
⑴Add New Entry
⑵右上角的NeoGrub
⑶Install 
⑷Configure，在出现menu.lst记事本窗口中输入以下内容并保存：[code]
root
kernel /vmlinuz iso-scan/filename=/ubuntu-10.10-desktop-i386.iso boot=casper splash
initrd /initrd.lz
boot
[/code][img]http://item.slide.com/r/1/121/i/ANPIkuaQmz-0MRNbkJeuhBimwnW3NNAf/[/img]
4、电脑重启后，就会发现启动菜单会多了一项NeoGrub Bootloader，选择后会进入Ubuntu 10.10光盘系统。打开终端（Alt+F2），输入：sudo umount -l /isodevice
最后点桌面上的Ubuntu安装图标即可开始正常安装。注②
5、安装完毕之后，回到win7，用easybcd删除NeoGrub。重新建立ubuntu的引导菜单：
[img]http://item.slide.com/r/1/207/i/oEVgRTQz3T_d9Vmgy94Qd76-GINm4qhK/[/img]
Add New Entry ， Linux/BSD ， type选择grub2。
[img]http://item.slide.com/r/1/249/i/lEiFfHA44D_0V79b_VoYx5s1dpfajoPX/[/img]
6、重启电脑，选择NeoLinux，进入Ubuntu 10.10，Alt+F2输入以下命令：
sudo update-grub2
注①：不推荐“ubuntu-10.10-dvd-i386.iso”，相对于693MB的iso，自带大量语言文件。安装过程中会发现速度非常慢，原因在于Ubuntu会配置卸载很多的不常用的语言。事实上安装Ubuntu后再online更新系统语言很方便。
[img]http://item.slide.com/r/1/156/i/6V1JU82h7T-PWK57-InZvONLXSwfdm0m/[/img]
注②：在开始执行安装的分区界面，下方会有一个高级选项，安装引导程序默认是安装到C盘的，就是改写到mbr。你要做的就是改变安装路径，要改为安装ubuntu的分区，比如sda8，这个要在分区的那一步看清楚。网上都说硬盘安装ubuntu会使软改win7失效，我在若干台电脑上按此方法安装ubuntu，无论是硬改软改还是原生支持slic2.1，都不影响已经激活的win7。
**迷失的风儿** 发表于 2010-11-18 18:52
Ubuntu 10.10
对这个的认识度还是很低.  有空图文给介绍一下?
**gensaya** 发表于 2010-11-18 21:09
[i=s] 本帖最后由 gensaya 于 2010-11-18 21:13 编辑 [/i]
[b] [url=http://www.ivemusic.net/redirect.php?goto=findpost&pid=419449&ptid=21892]2#[/url] [i]迷失的风儿[/i] [/b]
这方面我也是新手。elvba懂的比较多。
"图文介绍"？我真的不知该介绍什么。官网是个好地方：[url]www.ubuntu.org.cn[/url]
周围的情况是，不想用ubuntu的你怎么劝也没用，想用的大部分都是好奇，也不知道装ubuntu有什么好处。
我个人情况是，我离不开Win，因为学校网站只支持IE，chrome很悲剧。而且作业什么的老师也指定MS。我想大家也是如此吧，所以我能介绍的是如何Win7与ubuntu共存，其他的大家自己摸索吧，很有趣的。
用ubuntu我最大的感受就是绝大多是开源的。当然开始时会有点不适应。
**elvba** 发表于 2010-11-19 03:24
[i=s] 本帖最后由 elvba 于 2010-11-19 03:30 编辑 [/i]
我一直认为saya在ubuntu方面比我懂的多的....
前不久用过一次wubi，wubi不会有分区的时候的烦恼，第一次安装ubuntu的时候查了好多资料才去分区的....用起来和硬盘安装没啥差别，wubi管理也方便，不想用ubuntu的时候可以像卸载程序一样直接卸载掉，备份ubuntu也方便。具体的见[url]http://forum.ubuntu.org.cn/viewtopic.php?f=77&t=206945[/url]
现在倾向于wubi方式安装ubuntu。
对于ubuntu我的认识是：
1.强大的ubuntu论坛。
2.开源。
3.对视频画质有要求的话可以不考虑ubuntu。
4.ubuntu的3D桌面特效非常强大，win7相比之下差了很多很多。
5.可以多个桌面操作，而且切换就像切换程序一样迅速。
6.喜欢折腾电脑的话推荐用ubuntu，对电脑的认识很有帮助。
7.那个系统引导我到现在还没个头绪，有次别人电脑win7系统引导出了问题我怎么也恢复不了.......
8.硬盘安装是有点复杂了（对于电脑了解一般的同学），最方便的还是刻盘安装，但习惯windows的会卡在分区那儿，不知道哪个分多少，怎么分，害怕把windows的分区给灭了。wubi直接下一步就行，分区那儿可以不管。
9.电脑没有任何操作系统，手上只有ubuntu镜像的情况，想通过PE直接安装ubuntu的方法我找了很多资料都需要windows的那几个文件。。。这点可以无视。
10.ubuntu中不要对游戏要求太高，虽然它的特效比windows好了不知多少倍，但游戏效果确实不行，一般的游戏的话可以在ubuntu中用虚拟机吧。虚拟机没试过....
11.ubuntu上手很容易的。
12.要是我有笔记本的话，我很乐意用ubuntu的~笔记本的啥独立显卡其实和台式机的集显一个档次，屏幕那么小也不用追求什么画质，笔记本上的应用ubuntu完全能行。
13.可以感受到命令的强大，简单高效。
差不多就这么多了.....
**gensaya** 发表于 2010-11-19 08:39
elvba说的很好，wubi是一种方便的安装方式。
不过只要能把硬盘分区搞定，尽量选择硬盘安装吧：
1.不想用ubuntu时，可以用EasyBCD删除启动项，然后删除分区。所以wubi在这方面没优势。
2.wubi磁盘性能会有损失。
3.硬盘安装比光盘快。
有机会我把分区过程截图传上来。
ubuntu现在习惯了，上网还是chrome，听歌用qmmp，qq用webqq(这个真囧），办公用openoffice，视频用自带的就行。
不知是不是驱动的原因，看视频会有撕裂&锯齿感{:neko_05:}
**秋庭里香** 发表于 2010-11-19 12:25
双系统其实很折腾人……
**gensaya** 发表于 2010-11-19 12:42
双系统有用的
**秋庭里香** 发表于 2010-11-19 13:20
我发现我在ubuntu下只是重复我在windows下能做的……
对了，想请教下怎么将Ubuntu装在F盘，我在windows下已经将它格式化了
**gensaya** 发表于 2010-11-19 14:08
ubuntu上黄网不怕中毒，这个win7做不到吧？{:neko_20:}
小小想把ubuntu装在F盘？wubi吗，我不会{:nm_04:}
硬盘安装的话，比如说你F盘200G，那么右键压缩卷，得到一个自由空间，其中1000MB留给交换空间，剩下的留给/
如果F盘小的话，win下直接删了，安装ubuntu时再划分。
