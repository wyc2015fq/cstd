# Linux安装腾达网卡驱动 - DumpDoctorWang的博客 - CSDN博客





2018年02月01日 14:38:22[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1485








# 一、问题来源

我的网卡是Tenda U1, 要在Ubuntu上做开发，官方提供的驱动始终编译不过，无奈上淘宝，搜到了一款支持Ubuntu的网卡，这才能上网，以后再也不买Tenda的产品了。

同学和我的是同款网卡，也要装驱动，这才重新开始百度，看看能不能找到解决方案。

由于网卡驱动是一类相似的问题，所以我告诉大家该怎么搜索网卡对应的驱动。

# 二、如何搜索网卡对应的驱动

1、首先找到你的网卡的芯片型号，我是在官方的驱动包中找到'**rtl8192eu**'这样的压缩包，从而确定网卡芯片型号；另外一种方法就是打开终端，输入lsusb，找到网卡的ID，复制ID，谷歌一搜，就能搜索到芯片型号。

2、找到芯片型号以后，搜索“芯片型号+linux”, 就能找到相应的解决方案。我是直接去github上搜索芯片型号，比如“RTL8192eu”，然后就能找到对应的驱动，安装过程和下面的类似。

# 二、针对Tenda U1 , 网卡芯片型号为rtl8192eu的解决方案

参考博客：[Linux无线驱动安装历程](http://blog.csdn.net/Strangevy/article/details/52877872)

驱动下载地址：[Github驱动网址](https://github.com/Mange/rtl8192eu-linux-driver)

1、安装方法1

我是先看了第一篇博客，找到可github驱动网址，然后按照github上面的方法编译的网卡驱动，能用，但是网卡的灯不亮。附上命令记录:

```
sudo apt-get install git linux-headers-generic build-essential dkms
sudo make clean
sudo make
sudo make install
sudo modprobe -a 8192eu
```

8192eu是我的网卡的芯片对应的驱动名，不同的网卡可能不一样。注意看sudo make install的结果就能看到对应的名字。

然后将8192eu添加到/etc/modules。
`sudo gedit /etc/modules`
在弹出的编辑器里面, 新加一行8192eu

```
# /etc/modules: kernel modules to load at boot time.
#
# This file contains the names of kernel modules that should be loaded
# at boot time, one per line. Lines beginning with "#" are ignored.

8192eu
```

退出保存，重启就可以自动加载驱动。

注意：每次更新内核之后可能需要按照上述步骤重新编译安装驱动。

2、安装方法2

进入驱动目录，输入以下命令

```bash
sudo apt-get install git linux-headers-generic build-essential dkms
sudo dkms add .
sudo dkms install rtl8192eu/1.0
```

这种安装方法，即使在内核更新时，也会自动重新编译驱动。

然后将8192eu添加到/etc/modules。
`sudo gedit /etc/modules`
在弹出的编辑器里面, 新加一行8192eu

```
# /etc/modules: kernel modules to load at boot time.
#
# This file contains the names of kernel modules that should be loaded
# at boot time, one per line. Lines beginning with "#" are ignored.

8192eu
```

退出保存，重启就可以自动加载驱动。



