# (笔记)Ubuntu下安装arm-linux-gcc-4.4.3.tar.gz (交叉编译环境) - xqhrs232的专栏 - CSDN博客
2016年07月25日 17:48:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1612
原文地址::[http://www.cnblogs.com/tdyizhen1314/p/4124886.html](http://www.cnblogs.com/tdyizhen1314/p/4124886.html)
相关文章
1、[arm-linux-gcc工具链下载](http://blog.chinaunix.net/uid-2282111-id-2113354.html) ----[http://blog.chinaunix.net/uid-2282111-id-2113354.html](http://blog.chinaunix.net/uid-2282111-id-2113354.html)

参考了前人的成果，结合自己实践,arm-linux-gcc-4.4.3.tar.gz的下载地址为：
http://ishare.iask.sina.com.cn/f/13836544.html?from=like
本人所用系统是Ubuntu12.04，但只要是Ubuntu应该都可以安照此步骤安装。
1.解压文件
解压 tar zxvf arm-linux-gcc-4.4.3.tar.gz -C/（ 
直接解压后就可以，-C/会让解压的文件自动放在根目录下指定路径，不用管）
下载arm-linux-gcc-4.4.3.tgz到任意的目录下，进入这个文件夹
sudo tar xvzf arm-linux-gcc-4.4.3.tgz –C /
注意:C后面有个空格（不需要）,并且C是大写的（必须）,它是英文单词“Change”的第一个字母,在此是改变目录的意思。执行该命令,将把arm-linux-gcc 安装到/opt/Friendlyarm/toolschain/4.4.3 目录。
2.建立目录
sudo mkdir /usr/local/arm
3.复制文件
sudo cp -r /opt/FriendlyARM/toolschain/4.4.3 /usr/local/arm
5.添加环境变量
网上很多修改环境变量的方法，很杂。有 redhat上面的,fedora。下面一共三种方法，有人都在UBUNTU11.10上试过，可行。（本人只用了第一种方法做过，其他的没有试过）
方法一：修改/etc/bash.bashrc文件（此文件只对当前用户适用）
sudo gedit /etc/bash.bashrc
在最后加上 export PATH=$PATH:/opt/FriendlyARM/toolschain/4.4.3/bin  (路径)(注意等号那边不要有空格)
方法二：修改/etc/profile文件（此文件属于系统级别的环境变量，设置在里面的东西对所有用户适用）
sudo gedit /etc/profile
增加路径设置，在末尾添加如下：
export PATH＝$PATH:/usr/local/arm/4.4.3/bin
方法三：修改/etc/environment文件
sudo gedit /etc/environment
原文件应为
PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games“
将其改为
PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/arm/4.4.3/bin"
关于环境变量问题：
环境变量是什么？例如path，当要求系统运行一个程序而没有告诉它程序所在的完整路径时，系统除了在当前目录下面寻找此程序外，还应到path中指定的路径去找。用户通过设置环境变量，来更好的运行进程。
如果上面不加入环境变量，要运行arm-linux-gcc这个程序，则每次都要把arm-linux-gcc的完整路径写出来。/usr/local/arm/4.4.3/binarm-linux-gcc 文件名 -o 输出文件名
这样非常的麻烦，所以加到path里面，让电脑知道这个程序在哪里。
6.注册环境变量（此步未做，步骤8同样显示安装成功）
立即使新的环境变量生效，不用重启电脑，对应步骤5也有3种方法
方法一：source /root/.bashrc
    或者
   ./etc/bash.bashrc
方法二：source /etc/profile
方法三：立即生效
7.检查是否将路径加入到PATH（此步未做，步骤8同样显示安装成功）
echo $PATH
显示内容/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/arm/4.4.3/bin说明已经将交叉编译器的路径加入PATH。至此，交叉编译环境安装完成。
8.测试是否安装成功
方法一：另开一个终端，输入arm-，然后按两下Tab，如果出现下列信息，则表明安装成功
arm2hpdl                            arm-linux-gprof
arm-elf-addr2line                 arm-linux-ld
arm-elf-ar                           arm-linux-nm
arm-elf-as                           arm-linux-objcopy
arm-elf-c++                        arm-linux-objdump
arm-elf-c++filt                     arm-linux-populate
arm-elf-elf2flt                       arm-linux-ranlib
arm-elf-flthdr                       arm-linux-readelf
arm-elf-g++                         arm-linux-size
arm-elf-gcc                          arm-linux-strings
arm-elf-ld                            arm-linux-strip
arm-elf-ld.real                      arm-none-linux-gnueabi-addr2line
arm-elf-nm                          arm-none-linux-gnueabi-ar
arm-elf-objcopy                   arm-none-linux-gnueabi-as
arm-elf-objdump                  arm-none-linux-gnueabi-c++
arm-elf-protoize                   arm-none-linux-gnueabi-cc
arm-elf-ranlib                       arm-none-linux-gnueabi-c++filt
arm-elf-readelf                     arm-none-linux-gnueabi-cpp
arm-elf-size                         arm-none-linux-gnueabi-ct-ng.config
arm-elf-strings                     arm-none-linux-gnueabi-g++
arm-elf-strip                        arm-none-linux-gnueabi-gcc
arm-elf-unprotoize                arm-none-linux-gnueabi-gcc-4.4.3
arm-linux-addr2line               arm-none-linux-gnueabi-gccbug
--More--
方法二：在终端里输入arm-linux-gcc-v，这个命令会显示arm-linux-gcc信息和版本，如果出现以下信息则表示安装成功
Using built-in specs.
Target: arm-none-linux-gnueabi
Configured with:/opt/FriendlyARM/mini2440/build-toolschain/working/src/gcc-4.4.3/configure--build=i386-build_redhat-linux-gnu--host=i386-build_redhat-linux-gnu --target=arm-none-linux-gnueabi--prefix=/opt/FriendlyARM/toolschain/4.4.3--with-sysroot=/opt/FriendlyARM/toolschain/4.4.3/arm-none-linux-gnueabi//sys-root--enable-languages=c,c++
 --disable-multilib --with-arch=armv4t--with-cpu=arm920t --with-tune=arm920t --with-float=soft--with-pkgversion=ctng-1.6.1 --disable-sjlj-exceptions--enable-__cxa_atexit --with-gmp=/opt/FriendlyARM/toolschain/4.4.3--with-mpfr=/opt/FriendlyARM/toolschain/4.4.3--with-ppl=/opt/FriendlyARM/toolschain/4.4.3--with-cloog=/opt/FriendlyARM/toolschain/4.4.3--with-mpc=/opt/FriendlyARM/toolschain/4.4.3--with-local-prefix=/opt/FriendlyARM/toolschain/4.4.3/arm-none-linux-gnueabi//sys-root--disable-nls
 --enable-threads=posix --enable-symvers=gnu--enable-c99 --enable-long-long --enable-target-optspace
Thread model: posix
gcc version 4.4.3 (ctng-1.6.1)
至此，安装已经全部完成！
一个硬件工程师的故事
