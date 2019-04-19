# 如何解包／编辑／打包boot.img文件 - xqhrs232的专栏 - CSDN博客
2011年04月21日 16:18:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1290标签：[android																[手机																[脚本																[shell																[工具																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/zhenwenxian/archive/2010/09/01/5856583.aspx](http://blog.csdn.net/zhenwenxian/archive/2010/09/01/5856583.aspx)
感谢本文的作者：Alansj, DarkriftX, RyeBrye, Will, Try OP9, Tonyb486, Timmmm, Lxrose还有好多不知名的作者们在wiki上的不懈努力。
来源：[http://android-dls.com/wiki/index.php?title=HOWTO:_Unpack%2C_Edit%2C_and_Re-Pack_Boot_Images](http://android-dls.com/wiki/index.php?title=HOWTO:_Unpack%2C_Edit%2C_and_Re-Pack_Boot_Images)
目录
1、背景知识
2、boot和recovery映像的文件结构
3、对映像文件进行解包、编辑、打包的常规方法
3.1、另一种解包、编辑、打包的方法
4、将新的映像刷回到手机
5、解包、编辑、打包为我们带来了什么
6、本文讲的内容与使用update.zip刷机包不是一码事
正文
1、背景知识
Android手机的文件系统有许多存储器组成，以下是在adb shell下面的输出：
#cat /proc/mtd
dev:    size   erasesize  name
mtd0: 00040000 00020000 "misc"
mtd1: 00500000 00020000 "recovery"
mtd2: 00280000 00020000 "boot"
mtd3: 04380000 00020000 "system"
mtd4: 04380000 00020000 "cache"
mtd5: 04ac0000 00020000 "userdata"
注意，不同的手机在上述存储设备的顺序可能会各不相同!一定要检查您的手机，确定在以下的操作中选择正确的设备号（mtdX，这个X的序号一定要检查清楚）。
在本向导中，我们主要描述对"recovery"和"boot"的存储设备进行操作；"system"存储设备保存了android系统目录的所有数据（在系统启动后会挂载到“system/”目录）；“userdata”存储设备将保存了android数据目录中的所有数据（在系统启动后会挂载到“data/”目录，里面是会有很多应用数据以及用户的preference之类的配置数据）。
从上面的输出可以看出来，recovery和boot分区对应着/dev/mtd/mtd1和/dev/mtd/mtd2，在你您开始做任何修改之前一定要做两件事情，第一件事情，一定要先对这两个分区进行备份。
可以使用如下命令进行备份：
# cat /dev/mtd/mtd1 > /sdcard/recovery.img
# cat /dev/mtd/mtd2 > /sdcard/boot.img
（注意added by lxros，只有手机获取了ROOT权限以后才能够执行上述的备份命令）
第二件事情，你您应该把你您最喜欢的update.zip刷机包放置到你您的sd卡的根目录上面。如此一来，即使你您在后续的操作中出了问题，也可以启动到recovery模式进行恢复。
另外一个你您需要知道的重要文件是在android系统目录下的/system/recovery.img，此文件是mtd1存储设备的完全拷贝。这个文件在每次关机的时候，会自动地被写回到mtd1存储设备里面。
这会意味着两个事情：
(1)任何对/dev/mtd/mtd1中数据的直接修改都会在下一次重启手机以后消失。
(2)如果希望对/dev/mtd/mtd1进行修改，最简单的做法是用你您自己的recovery.img替换掉/system/recovery.img。当你您创建自己的update.zip刷机包的时候（特别是在做刷机包的适配的时候），如果你您忘记替换这个/system/recovery.img,这个recovery.img就会在关机的时候被烧写到mtd1里面去或许会变砖。一定要注意这一点！
2、boot和recovery映像的文件结构
boot和recovery映像并不是一个完整的文件系统，它们是一种android自定义的文件格式，该格式包括了2K的文件头，后面紧跟着是用gzip压缩过的内核，再后面是一个ramdisk内存盘，然后紧跟着第二阶段的载入器程序（这个载入器程序是可选的，在某些映像中或许没有这部分）。此类文件的定义可以从源代码android-src/system/core/mkbootimg找到一个叫做bootimg.h的文件。
（译者的话，原文是一个叫做mkbootimg.h的文件，但从Android 2.1的代码来看，该文件名应该是改为bootimg.h了）。
/*
** +-----------------+ 
** | boot header     | 1 page
** +-----------------+
** | kernel          | n pages  
** +-----------------+
** | ramdisk         | m pages  
** +-----------------+
** | second stage    | o pages
** +-----------------+
**
** n = (kernel_size + page_size - 1) / page_size
** m = (ramdisk_size + page_size - 1) / page_size
** o = (second_size + page_size - 1) / page_size
**
** 0. all entities are page_size aligned in flash
** 1. kernel and ramdisk are required (size != 0)
** 2. second is optional (second_size == 0 -> no second)
** 3. load each element (kernel, ramdisk, second) at
**    the specified physical address (kernel_addr, etc)
** 4. prepare tags at tag_addr.  kernel_args[] is
**    appended to the kernel commandline in the tags.
** 5. r0 = 0, r1 = MACHINE_TYPE, r2 = tags_addr
** 6. if second_size != 0: jump to second_addr
**    else: jump to kernel_addr
*/
ramdisk映像是一个最基础的小型文件系统，它包括了初始化系统所需要的全部核心文件，例如:初始化init进程以及init.rc（可以用于设置很多系统的参数）等文件。如果你您希望了解更多关于此文件的信息可以参考以下网址：
[http://git.source.android.com/?p=kernel/common.git;a=blob;f=Documentation/filesystems/ramfs-rootfs-initramfs.txt](http://git.source.android.com/?p=kernel/common.git;a=blob;f=Documentation/filesystems/ramfs-rootfs-initramfs.txt)
以下是一个典型的ramdisk中包含的文件列表：
./init.trout.rc
./default.prop
./proc
./dev
./init.rc
./init
./sys
./init.goldfish.rc
./sbin
./sbin/adbd
./system
./data
recovery映像包含了一些额外的文件，例如一个叫做recovery的二进制程序，以及一些对该程序支持性的资源图片文件（当你您按下home+power组合键的时候就会运行这个recovery程序）。
典型的文件列表如下：
./res
./res/images
./res/images/progress_bar_empty_left_round.bmp
./res/images/icon_firmware_install.bmp
./res/images/indeterminate3.bmp
./res/images/progress_bar_fill.bmp
./res/images/progress_bar_left_round.bmp
./res/images/icon_error.bmp
./res/images/indeterminate1.bmp
./res/images/progress_bar_empty_right_round.bmp
./res/images/icon_firmware_error.bmp
./res/images/progress_bar_right_round.bmp
./res/images/indeterminate4.bmp
./res/images/indeterminate5.bmp
./res/images/indeterminate6.bmp
./res/images/progress_bar_empty.bmp
./res/images/indeterminate2.bmp
./res/images/icon_unpacking.bmp
./res/images/icon_installing.bmp
./sbin/recovery
3、对映像文件进行解包、编辑、打包的常规方法
（注意，下面我给你您介绍的是手工命令行方式进行解包以及重新打包的方法，但是我仍然创建了两个perl脚本，这两个脚本可以让你您的解包和打包工作变得轻松许多。
可以参考本文的附件
unpack-bootimg.zip  [http://android-dls.com/files/apps/unpack-bootimg.zip](http://android-dls.com/files/apps/unpack-bootimg.zip)
repack-bootimg.zip  [http://android-dls.com/files/apps/repack-bootimg.zip](http://android-dls.com/files/apps/repack-bootimg.zip)
如果你您很擅长使用16进制编辑器的话，你您可以打开boot.img或者recovery.img，然后跳过开始的2K的头数据，然后寻找一大堆0的数据，在这一堆0的数据后面，紧跟着1F 8B这两个数字（1F 8B是gzip格式的文件的结束标记）。从此文件开始的地方（跳过2K的头），一大堆0后面紧跟着到1F 8B这两个数字为止的全部数据，就是gzip压缩过的linux内核。从1F 8B后面紧跟着的数据一直到文件的结尾包含的全部数据，就是ramdisk内存盘的数据。你您可以把把内核和ramdisk两个文件分别保存下来，在进行分别的修改和处理。我们可以通过un-cpio和un-gzip操作来读取ramdisk文件中的数据，可以使用如下的命令来实现这个目的，以下操作会生成一个目录，直接cd进去就可以看到ramdisk中的数据了：
gunzip -c ../your-ramdisk-file | cpio -i
此命令可以将ramdisk中的所有的文件解包到当前的工作目录下面，然后就可以对它进行编辑了。
当需要重新打包ramdisk的时候，就需要re-cpio然后re-gzip这些数据和目录，可以通过如下命令来实现：（cpio会把所有当前目录下面的文件都打包进去，因此，在进行此步骤之前，请把不需要的文件都清除掉。）
find . | cpio -o -H newc | gzip > ../newramdisk.cpio.gz
最后一步就是通过mkbootimg这个工具，把kernel和ramdisk打包在一起，生成一个boot.img：
mkbootimg --cmdline 'no_console_suspend=1 console=null' --kernel your-kernel-file --ramdisk newramdisk.cpio.gz -o mynewimage.img
这里的mkbootimg工具会在编译android的源代码的时候会在~/android-src/out/host/linux-x86/bin目录下面自动生成。
下载地址：
[http://git.source.android.com/?p=platform/system/core.git;a=tree;f=mkbootimg](http://git.source.android.com/?p=platform/system/core.git;a=tree;f=mkbootimg)
现在，如果不想背这些复杂的命令或者摆弄那个让人眩晕的16进制编辑器的话，可以尝试使用我编写的用于解包和打包的perl脚本了。希望这些脚本能够节约各位的键盘。
3.1、另一种解包、编辑、打包的方法
下载split_bootimg.zip文件，在此zip文件中包含一个perl文件，split_bootimg.pl脚本，该脚本可以读取boot.img头（根据Android源码中的bootimg.h读取）将kernel和ramdisk读取出来，此脚本也会输出内核命令行和板子名字。[http://android-dls.com/files/linux/split_bootimg.zip](http://android-dls.com/files/linux/split_bootimg.zip)
（注意，不要使用从/dev/mtd/mtd2直接拷贝出来的boot.img，此映像可能在读取过程遭到损坏。）
下面是一个从TC4-RC28更新中提取出来的boot.img进行解包操作：
% ./split_bootimg.pl boot.img 
Page size: 2048 (0x00000800)
Kernel size: 1388548 (0x00153004)
Ramdisk size: 141518 (0x000228ce)
Second size: 0 (0x00000000)
Board name: 
Command line: no_console_suspend=1
Writing boot.img-kernel ... complete.
Writing boot.img-ramdisk.gz ... complete.
解包ramdisk的命令如下：
% mkdir ramdisk
% cd ramdisk
% gzip -dc ../boot.img-ramdisk.gz | cpio -i
% cd ..
解码完毕后，就可以修改了（例如，在default.prop设置ro.secure=0等等）
使用mkbootfs工具（mkbootfs工具是编译完毕Android源代码以后，就会在~/android-src/out/host/linux-x86/bin自动生成）来重新创建ramdisk，可以使用如下命令来操作：
% mkbootfs ./ramdisk | gzip > ramdisk-new.gz
使用mkbootimg来重新创建boot.img，mkbootimg也可以在~/android-src/out/host/linux-x86/bin目录中可以找到：
% mkbootimg --cmdline 'no_console_suspend=1 console=null' --kernel boot.img-kernel --ramdisk ramdisk-new.gz -o boot-new.img
（注意：console=null的命令行选现是从TC4-RC30的boot.img引入的，用以去掉root shell）
4、将新的映像刷回到手机
可以将recovery.img拷贝到/system目录下面，然后重新启动手机，让手机自动为你您刷写到mtd里面（工作原理在上面已经提过了）。对于boot.img可以通过将其拷贝到sd卡的根目录，然后通过手机内的刷写工具将此映像写入到手机中。
例如，使用adb工具（Android SDK中的一个工具）将boot.img拷贝到手机的sd卡的根目录：
adb push ./mynewimage.img /sdcard
然后通过adb shell登录手机（获取过ROOT的）的shell交互模式，利用命令行进行交互：
# cat /dev/zero > /dev/mtd/mtd2
   write: No space left on device [this is ok, you can ignore]
# flash_image boot /sdcard/mynewimage.img
然后重启手机。
如果手机能够正常启动，那么祝贺你您，你您的修改和替换已经成功了；如果不能够顺利启动，则需要重新启动进入recovery模式，并且使用update.zip来恢复。
5、解包、编辑、打包为我们带来了什么
可以修改手机开机启动时候的画面，具体的操作的地址为：
[http://forum.xda-developers.com/showthread.php?t=443431](http://forum.xda-developers.com/showthread.php?t=443431)
6、本文讲的内容与使用update.zip刷机包不是一码事
您可以很容易地在其他论坛上看到关于如何自制update.zip刷机包的方法，也可以下载到很多在网络上共享的自制刷机包。例如：近期的多数刷机包都来自对rc30包的修改和调整。在update.zip刷机包里面会包括新的boot.img，recovery.img以及整个system/目录下的若干文件的替换和更新。如果您希望自己自制boot.img以及recovery.img，建议您选取相对较新的更新和版本。（选用较老的映像的话，或许会出现兼容性问题）。
对Gphone文件系统镜像的制作做一个简单的记录。
一、镜像的制作 
前两天对镜像进行了解包和简单的分析。然后把解包后的文件又压缩成了镜像刷会了我的G1手机。在这里我简单记录一下如何压缩镜像。
压缩镜像其实比解压简单得多。只要将ramdisk的文件夹和内核的zImage放在同一个文件夹下。然后将以下工具也放入同个文件夹：mkbootimg 、repack-bootimg.pl 。(此处的mkbootimg并不适合HTC版本的 G2  boot.img的制作、如需创建HTC版本的 G2  boot.img则要修改mkbootimg的源码，具体的修改方法我会在日后放出)
之后就打开shell，进入这个文件夹，然后执行：
./repack-bootimg.pl zImage ramdisk boot.img
很快的，最后在文件夹下产生的 boot.img就是boot.img镜像。其实repack-bootimg.pl在这行过程中会调用同目录下的mkbootimg。而repack-bootimg.pl的使用格式是
repack-bootimg.pl <kernel> <ramdisk-directory> <outfile>
repack-bootimg.pl具体的原理你可以看看这个脚本的代码。recovery.img的镜像制作也是一样的。
二、简单的刷机
在有了镜像之后，就可以刷机了。具体方法如下：
（1）先在G1上插入SD卡，然后把G1和电脑通过USB相连。下载SDK，解包后，通过shell在tools目录下运行： 
            sudo ./adb kill-server
            sudo./adb push (你的boot.img镜像路径) /sdcard
这样，你的boot.img镜像就在Gphone的SD卡中了
（2）然后，执行： 
    sudo ./adb kill-server
    sudo ./adb shell
    * daemon not running. starting it now *
    * daemon started successfully *
    ＃cat /dev/zero > /dev/mtd/mtd2
    write: No space left on device
    ＃flash_image boot /sdcard/(你的boot.img镜像名称)
这样，你就将新的boot.img镜像刷进了Gphone！可以启动看看了.
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/zhenwenxian/archive/2010/09/01/5856583.aspx](http://blog.csdn.net/zhenwenxian/archive/2010/09/01/5856583.aspx)

