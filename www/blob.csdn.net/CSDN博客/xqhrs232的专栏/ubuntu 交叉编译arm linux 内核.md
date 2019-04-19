# ubuntu 交叉编译arm linux 内核 - xqhrs232的专栏 - CSDN博客
2016年07月25日 17:33:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1727
原文地址::[http://www.cnblogs.com/foxhengxing/archive/2011/02/05/1949368.html](http://www.cnblogs.com/foxhengxing/archive/2011/02/05/1949368.html)
相关文章
1、[交叉编译 Linux 内核 for ARM](http://blog.csdn.net/junllee/article/details/7060526)----[http://blog.csdn.net/junllee/article/details/7060526](http://blog.csdn.net/junllee/article/details/7060526)
2、Ubuntu内核编译教程-编译Linux4.5.0内核----[http://www.linuxidc.com/Linux/2016-04/129955.htm](http://www.linuxidc.com/Linux/2016-04/129955.htm)
3、Ubuntu 12.04嵌入式交叉编译环境arm-linux-gcc搭建过程图解----[http://www.linuxidc.com/Linux/2013-06/85902.htm](http://www.linuxidc.com/Linux/2013-06/85902.htm)
4、[gcc 4.5.1交叉编译器构建过程](http://blog.csdn.net/cnsword/article/details/6118624)----[http://blog.csdn.net/cnsword/article/details/6118624](http://blog.csdn.net/cnsword/article/details/6118624)
下载arm-linux-gcc 4.2.3
[http://code.google.com/p/princess-alist/downloads/detail?name=arm-linux-gcc-4.3.2.tgz&can=2&q=](http://code.google.com/p/princess-alist/downloads/detail?name=arm-linux-gcc-4.3.2.tgz&can=2&q=)
安装arm-linux-gcc 4.2.3
sudo tar  xvzf arm-linux-gcc-4.3.2.tar.gz  -C /
此时将安装到/usr/local/arm下面
设置环境变量
gedit ~/.profile
在打开的文件最后加入以下命令
export PATH=$PATH:/usr/local/arm/4.3.2/bin
接着执行: source ~/.profile
然后输入 arm-linux-gcc -v ,如果能显示到版本就代表成功了
要编译内核之前，需要先注销一下再进入系统，我也不知道为什么
linux 内核编译
下载linux 2.6.26 内核源代码
解压下载下来的压缩包，修改解压出来的根目录中的makefile文件，定位到193行处
更改
#ARCH  ?= $(SUBARCH)
#CROSS_COMPILE ?=
为
ARCH  ?= arm
CROSS_COMPILE ?= arm-linux-
make menuconfig 
选择 Load an Alternate Configation File 回车
               删除.config，  输入linux-2.6.26/arch/arm/configs中你要选择的XXXXX_defconfig文件的绝对路径
选择 save an Alternate Configation File 回车
        此处显示的是我们刚选择的defconfig文件路径，删除掉，改为linux 2.6.26 解压路径下的.config(XXXXXX/.config)
make zImage
编译顺利的话，几分钟后会在arm/boot 下生成 zImage
参考文章：
[http://www.ej38.com/showinfo/linux-104820.html](http://www.ej38.com/showinfo/linux-104820.html)
