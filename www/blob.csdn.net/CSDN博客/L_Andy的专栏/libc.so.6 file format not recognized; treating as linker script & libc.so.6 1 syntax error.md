# libc.so.6: file format not recognized; treating as linker script & libc.so.6:1: syntax error - L_Andy的专栏 - CSDN博客

2017年08月18日 11:10:13[卡哥](https://me.csdn.net/L_Andy)阅读数：699



ubuntu下安装树莓派3的交叉编译器时出的状况：

vm1404@ubuntu:~$arm-bcm2708hardfp-linux-gnueabi-gcc test.c -o test

***../arm-bcm2708hardfp-linux-gnueabi/sysroot/lib/libc.so.6:file format not recognized; treating as linker script

***../arm-bcm2708hardfp-linux-gnueabi/sysroot/lib/libc.so.6:1:syntax error

collect2: error: ld returned 1 exit status

vm1404@ubuntu:~$

问题解决了，老状况，链接文件失效导致。

下载文件时，习惯性的在windows中下载，下完发现不是tar、bz等linux的压缩文件，然后就在windows下解压，然后再拷贝到虚拟机中，该过程导致很多的链接失效，导致了上面的问题。

下图是在linux下正确解压缩后的效果。进入原来编译时出问题的地方，可见很多都是链接(linux下这样的链接特别多)，原来在windows下解压缩后导致链接全部失效，这个问题以前也发生过。。。切记吧。。。

![](https://img-blog.csdn.net/20160803171815897)

