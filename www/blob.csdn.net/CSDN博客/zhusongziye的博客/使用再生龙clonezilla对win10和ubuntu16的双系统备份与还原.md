# 使用再生龙clonezilla对win10和ubuntu16的双系统备份与还原 - zhusongziye的博客 - CSDN博客





2017年09月25日 22:02:59[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：2612








**                                                                                                         win10和ubuntu16的双系统备份与还原**

上一段时间台式机安装了win10和ubuntu16的双系统，好不容易安装上了。就想着把这个系统备份下，如果后面发生了意外还可以还原。我是想备份整个磁盘，就可以备份这两个系统了。找了好多资料，ghost貌似不支持ext4的Linux文件，所以不能把ubuntu和win10一起备份。

苍天不愧有心人，阿拉终于想起来之前用过再生龙clonezilla，功能很强大，支持Windows和Linux系统文件。于是乎搜了下资料，证实一下。再生龙是台湾同胞开发的一个工具，开源的，很好用。官网下载地址：

[http://clonezilla.nchc.org.tw/clonezilla-live/download/download.php?branch=stable](http://clonezilla.nchc.org.tw/clonezilla-live/download/download.php?branch=stable)





根据你电脑系统和需要，进行下载，
制作启动盘。小编下载的zip格式的：clonezilla-live-2.5.2-31-amd64.zip



**先说下我的环境：**

我的台式机双系统：win10 64bit, Ubuntu16.04 64bit（基于win10上安装的Ubuntu）

我的台式机硬盘：一个Toshiba 240GB的SSD（双系统安装在此SSD，另外还有一个希捷的2T的3.5寸的大硬盘，此大硬盘用来存储SSD双系统的备份）

我的笔记本：win7 32bit，用来制作再生龙启动盘

U盘一个：4GB以及以上即可，用来制作再生龙启动盘



**再生龙启动盘制作方法：**

再生龙官网已经说的很清楚，在此我使用的是下面的方法：

根据我的系统配置（台式机，也就是需要备份的机器）我下载的是：

clonezilla-live-2.5.2-31-amd64.zip

PS: Zip 档案为 USB flash
或 USB
随身硬盘装置使用，ISO
档案为烧录 CD/DVD
使用。



下面是制作方法：

1、下载再生龙单机板 zip
档.



2、如果您的 USB
装置已经存在一个 FAT
格式的分割区(至少
200 MB)，请跳到步骤(3).

否则，请在您的 USB装置上产生一个
FAT16或
FAT32 格式的分割区(至少
200 MB)。

PS: 此处我为方便，是把整个U盘都格式化成了FAT32格式了。



3、解压缩 zip 并把其中所有的档案放置您的
FAT 分割去中，并保持目录架构.
例如："COPYING"
档案应该在 USB
装置的根目录下(如：G:\COPYING).



4、浏览您的 USB 装置，在
utils\win32\ 目录下(如：G:\utils\win32\)
并以『管理者身分执行』 makeboot.bat。

警告! makeboot.bat
必须在您的 USB 装置上执行.

警告!   不要从您机器上的硬盘执行
makeboot.bat!

这样可能导致您的 Windows
无法启动!!!



5、依画面指示进行，完成制作。



**使用再生龙备份：**

制作完成后，把U盘插在台式机上，更改BIOS启动项为U盘。保存退出，重启后进入再生龙系统界面如下：





选择第一项，回车即可



我选择的简体中文



选择，不修改键盘映射



选择使用再生龙





选择第一项



上图第一个硬盘是准备存储备份系统镜像的，第二个240G点是安装双系统的硬盘，按照提示按CTRL+C键





上面六个分区，sda开头的是我台式机2T硬盘的分区，我准备我双系统盘备份在这个硬盘一个区里，**注意此分区一定要比需要备份的双系统盘要大（**建议至少您选择一个至少有欲备份分区一半大小的分区作为存在备份的分区！！！！）**。**下方sdb开头的为需要备份的双系统盘。选择要存放备份映像的盘符，这里我选择的是我要放备份的sda8



选择该分区下，你想要把备份文件放到哪个文件夹下！这里我选择“顶级目录”，也就是根目录下！



选择第一个



选择专家模式



选择需要备份的硬盘，如上图



如上图，选择第一个



保持默认即可



默认选择第一项



使用默认值



默认选择第一个



可以选择是，也可选择否。我选的是



选不加密



默认选择第一项



按Enter键继续



输入y，继续



再生龙就开始自动备份啦，，等着完成。。。



按Enter键继续



选择关机，等关机后，移除U盘。即可。那么双系统就备份好啦。



来看下备份长得什么样子，哈哈。。。如下图





哈哈。。。备份成功完成。



**使用再生龙还原：**

**一样的道理，大家自行研究下。我就不写了。给大家一个自学的机会。如果你成功了，请记下来分享给大家。**



参考连接：

http://www.linuxidc.com/Linux/2012-03/57302p2.htm   在Linux上用dd命令实现Ghost功能



http://www.360doc.com/content/14/0726/08/1892865_397125953.shtml    再生龙软件使用详细教程



http://storysky.blog.51cto.com/628458/291587/    用"再生龙"Clonezilla
来克隆Linux系统



http://clonezilla.nchc.org.tw/clonezilla-live/download/download.php?branch=stable   再生龙下载地址



https://www.pendrivelinux.com/all-in-one-usb-dsl   pendrivelinux官网



时间不多一一就不上传图片了，大家请下载word版查看：

链接：http://pan.baidu.com/s/1nuO2UHr 密码：trn1

















