# 安装编译好的Android镜像到模拟器上 - V__KING__的专栏 - CSDN博客





2013年10月25日 09:22:03[v__king__](https://me.csdn.net/V__KING__)阅读数：2359标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[android](https://blog.csdn.net/V__KING__/article/category/1704545)










安装编译好的Android镜像到模拟器上。


    1. 设置环境变量：

**USER-NAME@MACHINE-NAME:~/Android$ export PATH=$PATH:~/Android/out/host/linux-x86/bin**

**USER-NAME@MACHINE-NAME:~/Android$ export ANDROID_PRODUCT_OUT=~/Android/out/target/product/generic**

其中，~/Android/out/host/linux-x86/bin有我们要执行的emulator命令，而~/Android/out/target/product/generic是Android镜像存放目录，下面执行emulator命令时会用到。当你没有将ANDROID_PRODUCT_OUT设置，而去运行emulator时，就不能运行模拟器


    2. 运行模拟器。

**USER-NAME@MACHINE-NAME:~/Android$ emulator**

模拟器运行需要四个文件，分别是Linux Kernel镜像zImage和Android镜像文件system.img、userdata.img和ramdisk.img。执行emulator命令时，如果不带任何参数，则Linux Kernel镜像默认使用~/Android/prebuilt/android-arm/kernel目录下的kernel-qemu文件，而Android镜像文件则默认使用ANDROID_PRODUCT_OUT目录下的system.img、userdata.img和ramdisk.img，也就是我们刚刚编译出来的镜像问题。


    当然，我们也可以以指定的镜像文件来运行模拟器，即运行emulator时，即：

**    USER-NAME@MACHINE-NAME:~/Android$ emulator -kernel ./prebuilt/android-arm/kernel/kernel-qemu -sysdir ./out/target/product/generic -system system.img -data userdata.img -ramdisk ramdisk.img**

到这里，我们就可以在模拟器上运行我们自己编译的Android镜像文件了，是不是很酷呢？但是注意，这里说的Android镜像文件，只是包括system.img、userdata.img和ramdisk.img这三个文件，而Linux Kernel镜像用的是Android为我们预编译好的kernel-qemu镜像。那么，有没有办法使用我们自己编译的Linux Kernel镜像呢？答案上肯定的，这样我们就可以完全DIY自己的Android系统了！我将在下一篇文章描述如果编译自己的Linux
 Kernel镜像，敬请期待~



