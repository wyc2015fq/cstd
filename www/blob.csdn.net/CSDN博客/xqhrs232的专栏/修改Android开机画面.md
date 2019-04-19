# 修改Android开机画面 - xqhrs232的专栏 - CSDN博客
2011年03月31日 09:23:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：737
原文地址::[http://www.cnblogs.com/cnhome/archive/2010/05/24/1743051.html](http://www.cnblogs.com/cnhome/archive/2010/05/24/1743051.html)
 修改第二部分开机动画::[http://blog.csdn.net/a345017062/archive/2011/03/04/6222962.aspx](http://blog.csdn.net/a345017062/archive/2011/03/04/6222962.aspx)

Android系统开机动画包括两部分：
开机显示的 ANDROID 文字；
ANDROID发光动画。
这篇文章说的开机动画是第一种，下面开始正文！
1. 制作当前屏幕像素的图片(模拟器默认为320*480)
 使用PS制作一张320*480的图片，保存时选“保存为 Web 所用格式”，然后在弹开的窗口上，“预设”项选择“PNG-24”，保存为android_logo.png
 注：好像只支持png-24，其他格式生成的rle文件显示不正常，有兴趣大家可以再验证一下。
2. 将图片转换为raw格式
 使用linux下的ImageMagick自带的convert命令，进行raw格式转换，命令为：
  convert -depth 8 android_logo.png rgb:android_logo.raw
 注：ubuntu 10.04 默认已经安装ImgageMagick工具，如果当前系统没有安装，可以执行下面的命令安装：
  sudo apt-get install imagemagick
3. 将raw格式转化为rle文件
 需要用到android编译后的rgb2565工具，在android/out/host/linux-x86/bin目录下(android为当前源码所在目录)，转换命令如下：
 rgb2565 -rle < android_logo.raw > initlogo.rle
到目前为止，启动需要显示的图像已经做好了，就是initlogo.rle，注意文件名必须是这个，如果想改文件名，需要修改android/system/core/init/init.h中的宏：
 #define INIT_IMAGE_FILE "/initlogo.rle" 
============================================================================================
下面需要将initlogo.rle加入的android文件系统中
4. 找到ramdisk.img文件(android/out/target/product/generic/ramdisk.img)，将文件名改为ramdisk.img.gz，然后使用下面的命令解压：
 gunzip ramdisk.img.gz
 解压后得到ramdisk.img，可能有人要问，怎么文件名又改回去了？其实不然，使用file ramdisk.img查看一下就知道了：
 解压前：ramdisk.img: gzip compressed data, from Unix
 解压后：ramdisk.img: ASCII cpio archive (SVR4 with no CRC)
 跑题了，还是说正事儿。
5. 使用cpio提取文件：
 新建一个temp目录：
 mkdir temp
 cd temp
 cpio -i -F ../ramdisk.img
6. 导出文件列表：
 cpio -i -t -F ../ramdisk.img > list       
 注：list是一个文本文件，里面存储了ramdisk.img的文件结构，我们需要在这个文件中加入initlogo.rle这一行，修改后的文件如下：
 data
 default.prop
 dev
 init
 init.goldfish.rc
 init.rc
 initlogo.rle 
 proc
 sbin
 sbin/adbd
 sys
 system
7. 生成ramdisk.img
 cpio -o -H newc -O ramdisk.img < list 
 注：根据list文件的描述,生成ramdisk.img文件
8. 用ramdisk.img覆盖sdk目录下的ramdisk.img(android-sdk-windows/platforms/android-2.1/images/ramdisk.img)，最好先备份一下。
9. 启动模拟器，就可以看到我们自己做的的开机界面了。

