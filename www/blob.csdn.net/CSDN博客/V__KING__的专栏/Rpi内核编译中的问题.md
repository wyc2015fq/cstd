# Rpi内核编译中的问题 - V__KING__的专栏 - CSDN博客





2015年12月05日 10:59:44[v__king__](https://me.csdn.net/V__KING__)阅读数：459








# Rpi内核编译中的问题

[http://shumeipai.nxez.com/2013/10/09/raspberry-pi-kernel-cross-compiler.html](http://shumeipai.nxez.com/2013/10/09/raspberry-pi-kernel-cross-compiler.html)

> 
/home/catia/android-sdks/platform-tools/aapt:  

  error while loading shared libraries: libz.so.1: cannot open shared object file:  

  No such file or directory
其实这个问题就是你64位的操作系统没有32位的类库,而android的SDK是要32位支持的,如果没装就只能是报上面的错!

打开终端运行下面的命令:

> 
sudo apt-get update 

  sudo apt-get install ia32-libs


运行第二个命令时有可能会说找不到或者被其它的所替代,然后再把终端列出的安装就好了.

> 
make ARCH=arm CROSS_COMPILE=../RpiTools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi- menuconfig

make ARCH=arm CROSS_COMPILE=../RpiTools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi- -j4






