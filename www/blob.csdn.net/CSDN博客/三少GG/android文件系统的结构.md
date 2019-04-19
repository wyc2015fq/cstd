# android文件系统的结构 - 三少GG - CSDN博客
2011年05月21日 12:31:00[三少GG](https://me.csdn.net/scut1135)阅读数：821标签：[android																[system																[image																[工具																[file																[unix](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
[http://www.61ic.com/Mobile/Android/201104/32906.html](http://www.61ic.com/Mobile/Android/201104/32906.html)
在源码目录下的out/target/product/generic/目录下运行
emulator -image system.img -data userdata.img -ramdisk ramdisk.img 命令
1、android文件系统的结构
android源码编译后得到system.img,ramdisk.img,userdata.img映像文件。其中， ramdisk.img是emulator的文件系统，system.img包括了主要的包、库等文件，userdata.img包括了一些用户数据，emulator加载这3个映像文件后，会把 system和 userdata分别加载到 ramdisk文件系统中的system和 userdata目录下。因此，我们可以把ramdisk.img里的所有文件复制出来，system.img和userdata.img分别解压到
 ramdisk文件系统中的system和 userdata目录下。
2、分离android文件系统出来
system.img,ramdisk.img,userdata.img映像文件是采用cpio打包、gzip压缩的，可以通过file命令验证：
file ramdisk.img，输出：
ramdisk.img: gzip compressed data, from Unix, last modified: Wed Mar 18 17:16:10 2009
Android源码编译后除了生成system.img，userdata.img之外还生成system和 userdata文件夹，因此不需要解压它们。Android源码编译后还生成root文件夹，其实root下的文件与 ramdisk.img 里的文件是一样的，不过这里还是介绍怎样把 ramdisk.img解压出来:
将ramdisk.img复制一份到任何其他目录下，将其名称改为ramdisk.img.gz，并使用命令
gunzip ramdisk.img.gz
然后新建一个文件夹，叫ramdisk吧，进入，输入命令
cpio -i -F ../ramdisk.img
这下，你就能看见并操作ramdisk里面的内容了。
然后把Android源码编译后生成的system和 userdata里的文件复制到 ramdisk/system和 ramdisk/userdata下。这样就得到一个文件系统了。
system.img & userdata.img
这两个镜像都属于yaffs2文件格式，生成方式是一样的。我们就以 system.img为例来说明。
System.img >> system目录树
所需工具unyaffs, 下载地址 http://code.google.com/p/unyaffs/downloads/list
# mkdir system
# cd system
# unyaffs ../system.img
system目录树 >> system.img
所需工具 mkyaffs2image，Android源码编译后会生成该工具，在 out/host/linux-x86/bin 目录下。
命令格式：
# mkyaffs2image system/ system_new.img
