# Ubuntu小私房(3)--Grub,Burg 的使用 - u013366022的专栏 - CSDN博客
2015年02月02日 10:11:49[slitaz](https://me.csdn.net/u013366022)阅读数：969
# Grub是什么？
> 
GNU GRUB 和GRUB是GRand Unified Bootloader的缩写，它是一个多重操作系统启动管理器。用来引导不同系统，如windows，linux。GRUB是多启动规范的实现，它允许用户可以在计算机内同时拥有多个操作系统，并在计算机启动时选择希望运行的操作系统。GRUB可用于选择操作系统分区上的不同内核，也可用于向这些内核传递启动参数。
> 
> 
> 
> 
![](https://img-blog.csdn.net/20130724170451265)
> 
# 安装&修复Grub2
> 
> 在使用光盘或者其他方式安装Ubuntu时候已经自带了Grub2，不需要额外安装，如果Grub2因配置错误或者其他原因而无法使用，也可一使用livecd进行修复。
> 
用Ubuntulive CD 修复 Ubuntu 12.04 Grub 引导具体修复步骤如下：
> 
> 
**[java]**[view
 plain](http://blog.csdn.net/incyanggan/article/details/9450373#)[copy](http://blog.csdn.net/incyanggan/article/details/9450373#)[print](http://blog.csdn.net/incyanggan/article/details/9450373#)[?](http://blog.csdn.net/incyanggan/article/details/9450373#)
- 1、用 Ubuntu 12.04 的 Live CD 光盘启动系统，启动光盘后不要选安装而选试用；  
- 2、打开终端，输入终端命令： sudo fdisk -l 此时可以看到显示出来的整个硬盘上所有分区的列表，请记录下 Ubuntu 根分区 / 和/boot分区安装的位置；  
- 3、在终端输入命令：  
- sudo -i  (取得root特权）  
- mount  /dev/sdaX  /mnt （X 为记录下的 Ubuntu 根分区）  
- mount  /dev/sda*  /mnt/boot （*为 Ubuntu /boot分区，/boot没有单独分区则不要执行这个命令）  
- grub- install  –root-directory=/mnt/  /dev/sda  (注意此命令中 root 前面是两个减号横杠）  
- 4.执行完以上步骤和命令，重新启动系统，这时您会看到熟悉的Grub启动菜单。  
# 修改Grub2默认启动
> 
>       在未修改情况下，每次进入系统Grub都默认从第一项进入，然后这也对多系统的用户并不是很便利，其实可以通过修改实现记忆上一次进入的系统。
> 可以通过修改/etc/default/grub文件  （这个文件里参数负责存有开机默认启动项、倒记时时间等等内容）
> 
> 
> 1.在终端下输入
> 
> 
**[java]**[view
 plain](http://blog.csdn.net/incyanggan/article/details/9450373#)[copy](http://blog.csdn.net/incyanggan/article/details/9450373#)[print](http://blog.csdn.net/incyanggan/article/details/9450373#)[?](http://blog.csdn.net/incyanggan/article/details/9450373#)
- sudo gedit /etc/default/grub  
> 
> 然后可以看到
> ![](https://img-blog.csdn.net/20130724145141265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5jeWFuZ2dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 
> 2.在里面找到并修改成如下
> 
> 
**[java]**[view
 plain](http://blog.csdn.net/incyanggan/article/details/9450373#)[copy](http://blog.csdn.net/incyanggan/article/details/9450373#)[print](http://blog.csdn.net/incyanggan/article/details/9450373#)[?](http://blog.csdn.net/incyanggan/article/details/9450373#)
- GRUB_DEFAULT="saved"//修改为saved为了记忆上一次进入的系统
- GRUB_SAVEDEFAULT="true"//这行一定要加，网上又很多保存了saved但是无效因为少了这行
- GRUB_HIDDEN_TIMEOUT="0"
- GRUB_HIDDEN_TIMEOUT_QUIET="true"
- GRUB_TIMEOUT="3"
- GRUB_DISTRIBUTOR="`lsb_release -i -s 2> /dev/null || echo Debian`"
- GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
- GRUB_CMDLINE_LINUX=""
![](https://img-blog.csdn.net/20130724145558000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5jeWFuZ2dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 3.然后保存，出来还要更新一下
> 
> 
**[java]**[view
 plain](http://blog.csdn.net/incyanggan/article/details/9450373#)[copy](http://blog.csdn.net/incyanggan/article/details/9450373#)[print](http://blog.csdn.net/incyanggan/article/details/9450373#)[?](http://blog.csdn.net/incyanggan/article/details/9450373#)
- sudo update-grub  
> 4.重启就可以看到效果了
> 
Grub2配置详解
> 
**[java]**[view
 plain](http://blog.csdn.net/incyanggan/article/details/9450373#)[copy](http://blog.csdn.net/incyanggan/article/details/9450373#)[print](http://blog.csdn.net/incyanggan/article/details/9450373#)[?](http://blog.csdn.net/incyanggan/article/details/9450373#)
- : /etc/default/grub  
- # 设定默认启动项，推荐使用数字  
- GRUB_DEFAULT=0
- # 注释掉下面这行将会显示引导菜单  
- #GRUB_HIDDEN_TIMEOUT=0
- # 黑屏，并且不显示GRUB_HIDDEN_TIMEOUT过程中的倒计时  
- GRUB_HIDDEN_TIMEOUT_QUIET=true
- # 设定超时时间，默认为10秒  
- # 设定为-1取消倒计时  
- GRUB_TIMEOUT=10
- # 获得发行版名称（比如Ubuntu, Debian）  
- GRUB_DISTRIBUTOR=`lsb_release -i -s 2> /dev/null || echo Debian`  
- # 将会导入到每个启动项（包括recovery mode启动项)的'linux'命令行  
- GRUB_CMDLINE_LINUX=""
- # 同上，但是只会添加到 normal mode 的启动项  
- GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
- # 取消注释以允许图形终端（只适合grub-pc）  
- #GRUB_TERMINAL=console  
- # 分辨率设定，否则采用默认值  
- #GRUB_GFXMODE=640x480  
- # 取消注释以阻止GRUB将传递参数 "root=UUID=xxx" 传递给 Linux  
- #GRUB_DISABLE_LINUX_UUID=true
- # 取消启动菜单中的“Recovery Mode”选项  
- #GRUB_DISABLE_LINUX_RECOVERY="true"
- # 当GRUB菜单出现时发出鸣音提醒  
- #GRUB_INIT_TUNE="480 440 1"
- 配置进阶  
- 获得当前GRUB2有效的启动项  
- $ grep menuentry /boot/grub/grub.cfg  
- 配置默认启动项  
- GRUB_DEFAULT=x  
- 注释：x为数字，以指定启动列表中第x项为启动项，x从0开始计数  
- GRUB_DEFAULT=”xxxx”  
- 注释：指出具体的启动项名称，例如GRUB_DEFAULT=”Ubuntu, Linux 2.6.31-9-generic”  
- GRUB_DEFAULT=saved  
- 注释：该项将使用grub-set-default和grub-reboot命令来配置默认启动项  
- 例如：  
- $ sudo grub-set-default0
- 上面这条语句将会持续有效，直到下一次修改；下面的命令则只有下一次启动的时候生效：  
- $ sudo grub-reboot 0
- 将下次选择的启动项设为默认  
- 只需要在/etc/default/grub中添加这行  
- GRUB_SAVEDEFAULT=true
- 隐藏引导菜单  
- GRUB_HIDDEN_TIMEOUT=X  
- 注释：启动过程中，黑屏X秒后引导默认的启动项  
- 注意1：启动过程中按SHIFT可以显示菜单  
- 注意2：如果等号后面不跟任何值，也将会显示引导菜单  
- 取消倒计时  
- GRUB_TIMEOUT=-1
# 图形配置利器Grub Customizer
> 
Grub Customizer是一款图形化GRUB软件，包括GRUB2/BURG设置和菜单项，其主要功能如下：
*GRUB等待时间；
*屏幕分辨率；
*GRUB2 色彩设置；
*GRUB 背景2图片；
*开机选项设置；
![](https://img-blog.csdn.net/20130724165937765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5jeWFuZ2dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如何安装Grub Customizer？
通过官方PPA源可以安装：
> 
> 
> 
**[java]**[view
 plain](http://blog.csdn.net/incyanggan/article/details/9450373#)[copy](http://blog.csdn.net/incyanggan/article/details/9450373#)[print](http://blog.csdn.net/incyanggan/article/details/9450373#)[?](http://blog.csdn.net/incyanggan/article/details/9450373#)
- 打开终端以下命令：  
- sudo add-apt-repository ppa:danielrichter2007/grub-customizer  
- sudo apt-get update  
- sudo apt-get install grub-customizer  
> 
即可安装成功，功能比较简单易用。
# 利用Burg实现图形引导
> 
简介
BURG 是基于 GRUB 的多重引导工具，全称为 Brand-new Universal loadeR from GRUB，也可称为 GRUB 的增强版，它的特色就是可以使用图形界面方式来替换原本只有文本的启动菜单，从而让你的 PC 具备更酷的启动效果。
详情请参考其在google的项目页面：[http://code.google.com/p/burg/](http://code.google.com/p/burg/)
![](https://img-blog.csdn.net/20130724171017984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5jeWFuZ2dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
安装方法
**[java]**[view
 plain](http://blog.csdn.net/incyanggan/article/details/9450373#)[copy](http://blog.csdn.net/incyanggan/article/details/9450373#)[print](http://blog.csdn.net/incyanggan/article/details/9450373#)[?](http://blog.csdn.net/incyanggan/article/details/9450373#)
- //第一步   添加源  执行
- sudo gedit /etc/apt/sources.list   
- //在其末尾添加
- deb http://ppa.launchpad.net/bean123ch/burg/ubuntu maverick main
- deb-src http://ppa.launchpad.net/bean123ch/burg/ubuntu maverick main
- 
- //第二步 当然是不能忘的
- sudo apt-get update  
- 
- //第三步update后会出现如下未导入公钥的错误，不用理会
- gpg --keyserver keyserver.ubuntu.com --recv 55708F1EE06803C5                  //下载密钥
- gpg --export --armor 55708F1EE06803C5 | sudo apt-key add-                //导入密钥
- 
- //第四步安装burg
- sudo apt-get install burg burg-themes burg-emu  
- //上述命令安装了三个程序：burg主程序；burg-themes//主题；burg-emu为burg模拟器
- //安装期间一路回车即可，有的需要输入y
- 
- //第五步
- sudo burg-install "(hd0)"
- //这步是必须的，他会自动检测你所安装的系统生成burg.cfg文件
- 
- //第六步
- sudo update-burg  
- 至此安装完成，在终端出入burg-emu就可预览到效果。  
- 
- //第七步
- sudo gedit /boot/burg/burg.cfg   
- //在该配置文件中可以编辑系统菜单项，包括删除不需要//的菜单项，修改系统名字。
卸载方法
**[java]**[view
 plain](http://blog.csdn.net/incyanggan/article/details/9450373#)[copy](http://blog.csdn.net/incyanggan/article/details/9450373#)[print](http://blog.csdn.net/incyanggan/article/details/9450373#)[?](http://blog.csdn.net/incyanggan/article/details/9450373#)
- F1帮助    
- F2切换主题    
- F3修改分辨率  
- 
- 卸载可以在终端下执行：  
- 
- sudo apt-get remove burg burg-themes burg-emu  

# 利用super-boot-manager优化主题
> 
Super boot manager，缩写为SBM。它是一款基于buc，可以用来管理Burg,Grub2,Plymouth一站式工具。可以让其他工具望而生畏，不敢弄斧班门。
> 
目标效果
![](https://img-blog.csdn.net/20130724172303015)
如何安装？
在Ubuntu 12.04/Linux Mint 13中安装超级引导管理器 
**[java]**[view
 plain](http://blog.csdn.net/incyanggan/article/details/9450373#)[copy](http://blog.csdn.net/incyanggan/article/details/9450373#)[print](http://blog.csdn.net/incyanggan/article/details/9450373#)[?](http://blog.csdn.net/incyanggan/article/details/9450373#)
- 打开终端，输入命令：   
- sudo add-apt-repository ppa:ingalex/super-boot-manager  
- sudo apt-get update  
- sudo apt-get install super-boot-manager  
安装后
运行软件可以设置相应的启动设置，然后可以看到
![](https://img-blog.csdn.net/20130724173421796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5jeWFuZ2dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 
在主题项中自动下载好主题，非常便利下载好了顺便应用更改，最后在在终端输入
**[java]**[view
 plain](http://blog.csdn.net/incyanggan/article/details/9450373#)[copy](http://blog.csdn.net/incyanggan/article/details/9450373#)[print](http://blog.csdn.net/incyanggan/article/details/9450373#)[?](http://blog.csdn.net/incyanggan/article/details/9450373#)
- burg-emu   
便可以查看效果，成功～
![](https://img-blog.csdn.net/20130724174107765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5jeWFuZ2dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

> 作者：YangGan
> 出处：[](http://www.cnblogs.com/skynet/)[http://blog.csdn.net/incyanggan](http://blog.csdn.net/incyanggan)
> 本文基于[署名
 2.5 中国大陆](http://creativecommons.org/licenses/by/2.5/cn/)许可协议发布，欢迎转载，演绎或用于商业目的，但是必须保留本文的署名[Yanggan](http://blog.csdn.net/incyanggan)（包含链接）.
