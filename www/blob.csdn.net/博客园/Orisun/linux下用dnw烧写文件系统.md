# linux下用dnw烧写文件系统 - Orisun - 博客园







# [linux下用dnw烧写文件系统](https://www.cnblogs.com/zhangchaoyang/articles/1991649.html)





首先针对qt-4.5的文件系统天嵌已经制作好了，在 TQ2440_CD\Linux资源\Linux源码包\2.6.30.4 里
有：root_qt_4.5_2.6.30.4_20100601.tar.gz

有必要了解一下这个Qt4.5的文件系统是如何制作成的，在《Qt程序开发完全手册.pdf》的6.6节有讲解。

如果从头做一个文件系统，参见《Linux移植.pdf》之让系统“跑”起来step9--11。

如果想用4.7版本上也只需在qt-4.5文件系统上稍作修改。

把文件系统编译成bin文件时针对64M的板子用mkyaffsimage_2命令。


针对256M的板子要用mkyaffs2image命令，我用错了命令结果在烧写时出现错误：


NAND erase: device 0 offset 0x24c000, size 0x3db4000
Skipping bad block at  0x00fb8000
Erasing at 0x3ffc000 -- 100% complete.
OK

NAND write: device 0 offset 0x24c000, size 0x3db4000
Input block length is not page aligned
Data did not fit into device, due to bad blocks
64700416 bytes written: ERROR

注意安装dnw之前在先

sudo apt-get install linux-source
sudo apt-get install glibc-source

1.下载dnw_for_linux.tar.gz

2.解压tar xvf d
nw_for_linux.tar.gz

3.进入目录cd dnw_for_linux/secbulk

4.编译make -C 
/lib/modules/`uname -r`/build M=`pwd` modules

5.给系统加载模块
$sudo insmod ./secbulk.ko

6.编译dnw 
 $cd ../dnw      &       gcc -o dnw dnw.c

7.
执行dnw下载 $sudo ./dnw /path/to/what/you/want/download












