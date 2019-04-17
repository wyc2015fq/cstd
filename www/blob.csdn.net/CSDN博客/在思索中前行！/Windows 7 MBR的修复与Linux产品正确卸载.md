# Windows 7 MBR的修复与Linux产品正确卸载 - 在思索中前行！ - CSDN博客





2014年11月21日 15:07:04[_Tham](https://me.csdn.net/txl16211)阅读数：2020








这几天折腾系统很令人崩溃，但也明白了开机引导流程具体如何。

觉得Centos 7不好用，想卸载Redhat安装Ubuntu，为了图方便直接把红帽的硬盘区格式化了。于是开机引导崩溃，咨询了下大神，大神叫我去搜下mbr修复，于是就有了这篇备忘录。




———————————————以下部分为引用—————————————————————

**一、MBR的修复（问题描述：直接格式化Linux分区导致Grub无法引导开机）**

**（MBR，全称为Master Boot Record，即硬盘的主引导记录。）**

在存在win7与ubuntu双系统的情况下，进入windows后直接格式化硬盘分区将ubuntu删除，重新启动后出现如下提示。

error：no such partition

grub rescue

**问题：**引导程序出错

**解决办法:**用U盘做一个“老毛桃winpe”的启动盘，要提醒的是只是制作一个启动盘，而不是安装盘。



**具体步骤：**

1. 将u盘格式化；

2.下载一个“老毛桃的u盘启动制作工具”，完成后解压并运行；

3.插入U盘之后点击 ![](http://www.laomaotao.net/uploads/allimg/121225/1-12122509233Q36.png) 按钮，一直到完成，弹出即可。

4.进入BIOS设置U盘的启动顺序为第一个。

5. 接下来进入win7pe界面，在桌面或者开始菜单中寻找“WINDOWS引导修复”的软件，点击自动修复即可。


——————————————————引用结束——————————————————————




**二、正常方式卸载Linux**（先修复Mbr到win7 再格式化Linux）



方法1：有windows7安装盘的，可以pe环境格式化linux分区后，利用WINDOWS引导修复修复mbr主引导记录，进去windows7。

方法2：如果没有任何的系统安装盘的话，下载fixmbr工具，在windows7系统环境下操作，重建修复mbr引导，然后格式化相关的硬盘分区。




**方法二详解：**

## (适用于Vista/XP/Win7+Ubuntu/Fedora/FreeBSD)



**1. 下载MbrFix.exe**

首先进入Windows 操作系统，进入www.sysint.no 网站下载MbrFix.exe；
下载完成后解压zip压缩包，将MbrFix.exe 剪切到C盘(系统盘)；
（如果是64位的系统，请用MbrFix64.exe）
开始–>运行–>cmd–cd \命令进入C盘符–>在C盘根目录（cd ..; cd..; 直到在根目录下）下运行命令：

** MbrFix /drive 0 fixmbr /yes –>Enter回车**；

（如果是Vista系统，运行命令：** MbrFix /drive 0 fixmbr /vista /yes –>Enter回车**）
（如果是Win7系统，运行命令：** MbrFix /drive 0 fixmbr /win7 /yes –>Enter回车**）这样MBR修复就完成了，没有任何提示（没有消息就是最好的消息）。接下来直接删除linux分区就可以了。
**2. 格式化删除 Linux分区**

以Win7系统为例：点击“计算机”右键–>管理–>磁盘管理–>找到Ununtu分区–>右键–>删除逻辑分区。

重启电脑，Grub 已经不复存在，双系统Linux 卸载完成，电脑启动自动进入Windows系统….



原因：由于双系统是grub引导，所以格式化linux的硬盘分区，会造成grub无法引导windows 7 的情况——开机进不去系统，只有黑色grub画面。如果出现了无法进入win7系统的情况时，接下来将需要进行修复引导区的工作，也就是第一大步骤讲的修复工作。





**另外Ubuntu下可以直接烧录U盘安装盘。**

参考资料：

[Linux开机启动流程](http://www.ruanyifeng.com/blog/2013/08/linux_boot_process.html)

[Win7开机引导流程](http://jingyan.baidu.com/article/eb9f7b6dac24c3869364e801.html)

[双系统的卸载和启动错误：grub的修复](http://blog.csdn.net/gaojya/article/details/9412445)


[Windows下完美卸载Linux系统](http://blog.csdn.net/gebron/article/details/6719449)



