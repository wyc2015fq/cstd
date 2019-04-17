# CentOS 6.4中升级编译安装GCC 4.8.1 + GDB 7.6.1 + Eclipse 以及Kdump配置 - 在思索中前行！ - CSDN博客





2015年06月02日 11:05:09[_Tham](https://me.csdn.net/txl16211)阅读数：1690标签：[centos 6.4 升级gcc 4.8																[centos 安装Eclipse																[centos 配置Kdump																[centos 安装GDB7.6.1](https://so.csdn.net/so/search/s.do?q=centos 安装GDB7.6.1&t=blog)
个人分类：[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)





## **在[CentOS](http://www.linuxidc.com/topicnews.aspx?tid=14) 6.4中编译安装GCC 4.8.1 + GDB 7.6.1 + Eclipse**




今天在[isocpp](http://isocpp.org/)上看到“[GCC
 4.8.1 released, C++11 feature complete](http://isocpp.org/blog/2013/05/gcc-4.8.1-released-c11-feature-complete)”这个消息，非常兴奋。终于有一个全面支持C++11语言特性的编译器了！


当然了，gcc仅仅是语言特性上全面支持C++11，libstdc++貌似还没有完全跟上，不过我已经挺满足了，至少可以去尝试各种特性了。另外，之前也[有消息说Clang功能上也100%支持C++11了](http://isocpp.org/blog/2013/04/clang-reaches-100-fully-complaint-compiler-expected-this-summer)，但是内部还在做测试，[Clang3.3的release版据说6月5号正式发布](http://isocpp.org/blog/2013/05/gcc-4.8.1-released-c11-feature-complete)，最近对于C++来说大事真多啊，很期待！


今天下午看到这个消息，晚上就实在忍不住了，赶紧把最新的gcc下载下来编译安装起来，下面介绍下安装步骤：


安装之前，还是提醒下大家：一些基本的准备工作我就不说了，比如得装个Linux（CentOS，Ubuntu等等），然后装上基本的编译环境，用较老的编译环境编译新的编译器，这叫[自举（bootstrap）](http://blog.csdn.net/bokee/article/details/5380044)。如果编译的过程，提示出错，就按照提示去把环境装好，不再详细说明。


**一、编译安装gcc 4.8.1**

**1. 安装gcc和g++**


新安装的CentOS缺少编译环境，必须先安装旧版本的gcc， 然后再进行自举编译


yum -y install gcc 

yum -y install gcc-c++

**2. 下载gcc 4.8.1源码包**

[http://ftp.tsukuba.wide.ad.jp/software/gcc/releases/gcc-4.8.1/gcc-4.8.1.tar.gz](http://ftp.tsukuba.wide.ad.jp/software/gcc/releases/gcc-4.8.1/gcc-4.8.1.tar.gz)


将源码包放在home目录的Download目录中

**3. 解压缩gcc 4.8.1源码包**


cd /root/Downloads/


tar -zxvf gcc-4.8.1.tar.gz

**4. 下载编译所需的依赖包**


cd /root/Downloads/gcc-4.8.1


./contrib/download_prerequisites


cd ..

**5. 新建目录存放编译结果**


mkdir gcc-build-4.8.1

**6. 进入新目录，并执行configure命令，产生makefile**


cd gcc-build-4.8.1 

../gcc-4.8.1/configure --enable-checking=release --enable-languages=c,c++ --disable-multilib

**7. 编译gcc 4.8.1**


make -j4

**8. 安装gcc 4.8.1**


sudo make install

**9. 更新libstdc++.so.6**

sudo cp /root/Downloads/gcc-build-4.8.1/prev-x86_64-unknown-linux-gnu/libstdc++-v3/src/.libs/libstdc++.so.6.0.18 /usr/lib 
sudo ln -sf /usr/lib/libstdc++.so.6.0.18 /usr/lib/libstdc++.so.6

红字部分在不同平台上有所不同，注意，如果是64位系统，则命令中的lib要替换为lib64

（按照作者的步骤安装后，还需要一个步骤更新一下libstdc++.so.6，否则可能会出现/usr/lib/libstdc++.so.6: version `GLIBCXX_3.4.15' not found的问题（编译运行：[http://www.cplusplus.com/reference/functional/bind/](http://www.cplusplus.com/reference/functional/bind/)的bind_example），更新步骤如下：
1. #cp ~/gcc-build-4.8.1/prev-i686-pc-linux-gnu/libstdc++-v3/src/.libs/libstdc++.so.6.0.18  /usr/lib64
2. #ln -sf /usr/lib64/libstdc++.so.6.0.18  /usr/lib64/libstdc++.so.6）

**10. 检查版本**


g++ --version


g++ (GCC) 4.8.1 

Copyright (C) 2013 Free Software Foundation, Inc. 

This is free software; see the source for copying conditions.  There is NO 

warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


gcc --version


gcc (GCC) 4.8.1 

Copyright (C) 2013 Free Software Foundation, Inc. 

This is free software; see the source for copying conditions.  There is NO 

warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

**二、编译安装gdb 7.6.1**

**1. 先卸载gdb旧版本**


rpm -qa | grep gdb


rpm -e --nodeps gdb-7.2-60.el6_4.1.i686



**2. 安装Texinfo文档系统**


yum install ncurses-devel



**3. 下载gdb 7.6.1源码包**

[http://ftp.gnu.org/gnu/gdb/gdb-7.6.1.tar.gz](http://ftp.gnu.org/gnu/gdb/gdb-7.6.1.tar.gz)


将源码包放在home目录的Download目录中

**4. 解压缩gdb 7.6.1源码包**


cd /root/Downloads/


tar -zxvf gdb-7.6.1.tar.gz

**5. 生成makefile**


cd /root/Downloads/gdb-7.6.1


./configure


make


sudo make install

**6. 将编译好的gdb拷贝到/usr/bin目录中**

sudo cp /root/Downloads/gdb-7.6.1/gdb/gdb /usr/bin/gdb 
**7. 查看安装是否成功。**

gdb -v






## 编译一下能够较好支持C++11的GCC。




一、准备工作

我们先来到GCC官方网站上的[下载页面](https://gcc.gnu.org/mirrors.html)，找一个最近的镜像，然后在release里找到一个对应的源码包。如[](http://mirrors-usa.go-parts.com/gcc/releases/gcc-4.8.1/gcc-4.8.1.tar.gz)[http://mirrors-usa.go-parts.com/gcc/releases/gcc-4.8.1/gcc-4.8.1.tar.gz](http://mirrors-usa.go-parts.com/gcc/releases/gcc-4.8.1/gcc-4.8.1.tar.gz)，这是我需要的。然后我们把它下载下来，找个地方，解压缩。


二、解决依赖

向来这就是一个比较头疼的问题，不过好在可以使用download_prerequisites。将目录切换到解压的源码目录，执行一下./contrib/download_prerequisites，所需的东西就会自动下载好了。如果在中国大陆的话，这可能需要一段时间；在美国可以以秒计算耗时。


三、开始生成

建立一个目录，名字随便取，比如就叫build吧，然后切换进去。接着我们需要生成Makefile。执行../configure --enable-checking=release --enable-languages=c,c++ --disable-multilib来生成Makefile。因为我只需要C和C++，所以在参数里写好了。接着执行make -j4编译。这当然是最耗时的步骤，慢慢等吧。我在一台i5-3470、8G内存的机器上只花了一杯茶的时间，然而在一台15刀一年的廉价OpenVZ美国VPS上花了三个多小时。


四、更新运行库 将新的libstdc++拷贝出来：cp prev-x86_64-unknown-linux-gnu/libstdc++-v3/src/.libs/libstdc++.so.6.0.18 /usr/lib/libstdc++.so.6，然后建立链接：ln -sf /usr/lib/libstdc++.so.6.0.18 /usr/lib/libstdc++.so.6，这样就完成了。


用gcc -v或gcc --version检查一下情况：gcc 版本 4.8.1 (GCC)。大功告成。之后使用-std=c++11算是无压力了。




**三、安装Eclipse**

**1. 下载Eclipse IDE for C/C++ Developers**


cd /root/Downloads


wget [http://mirror.hust.edu.cn/eclipse//technology/epp/downloads/release/luna/R/eclipse-cpp-luna-R-linux-gtk-x86_64.tar.gz](http://mirror.hust.edu.cn/eclipse//technology/epp/downloads/release/luna/R/eclipse-cpp-luna-R-linux-gtk-x86_64.tar.gz)

**2. 解压安装Eclipse**


tar xvzf eclipse-cpp-luna-R-linux-gtk-x86_64.tar.gz


mv /root/Downloads/eclipse /usr/local/Eclipse4C


rm -rf eclipse-cpp-luna-R-linux-gtk-x86_64.tar.gz

**3. 创建桌面快捷方式**


在系统桌面上单击右键，在弹出菜单中选择“Create Launcher ...”，按照如下截图进行设置：

![](http://www.linuxidc.com/upload/2015_01/150119190156901.png)


其中，Command参数为/usr/local/Eclipse4C/eclipse，点击OK按钮便可在桌面创建快捷方式

**4. 设置Workspace**


mkdir -p /home/Workspace/C


在桌面上双击打开Eclipse4C快捷方式，将Workspace目录指定为/home/Workspace/C，如下图所示：

![](http://www.linuxidc.com/upload/2015_01/150119190319062.png)

**5. 安装ShellED插件**


在Eclipse的菜单栏上找到Help → Install New Software ...，打开Install窗口，配置好之后，如下图所示：

![](http://www.linuxidc.com/upload/2015_01/150119190319063.png)


勾选Shell Script之后，一路Next下去即可


ShellED的下载链接为：[http://sourceforge.net/projects/shelled/files/shelled/update/](http://sourceforge.net/projects/shelled/files/shelled/update/)

**6. 为C/C++工程添加ShellED支持**


右键点击工程名称，在弹出的菜单中选择ShellED → Add Shell Script Support

![](http://www.linuxidc.com/upload/2015_01/150119190319061.png)


经过如上设置，Eclipse便可以在C/C++工程中运行Shell脚本了


GCC编译器入门  [http://www.linuxidc.com/Linux/2015-01/111431.htm](http://www.linuxidc.com/Linux/2015-01/111431.htm)

[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2) 12.04嵌入式交叉编译环境arm-linux-GCC搭建过程图解 [http://www.linuxidc.com/Linux/2013-06/85902.htm](http://www.linuxidc.com/Linux/2013-06/85902.htm)


Ubuntu 12.10安装交叉编译器arm-none-linux-gnueabi-GCC [http://www.linuxidc.com/Linux/2013-03/82016.htm](http://www.linuxidc.com/Linux/2013-03/82016.htm)


Ubuntu下Vim+GCC+GDB安装及使用 [http://www.linuxidc.com/Linux/2013-01/78159.htm](http://www.linuxidc.com/Linux/2013-01/78159.htm)


Ubuntu下两个GCC版本切换 [http://www.linuxidc.com/Linux/2012-10/72284.htm](http://www.linuxidc.com/Linux/2012-10/72284.htm)

**GCC 的详细介绍**：[请点这里](http://www.linuxidc.com/Linux/2012-03/57328.htm)
**GCC 的下载地址**：[请点这里](http://www.linuxidc.com/down.aspx?id=452)




## CENTOS 升级GCC到4.8.2




     使用操作系统：Centos 6.4 Desktop，64bit；

　　原GCC版本：4.4.7；

　　目标：升级GCC到4.8.2，以支持C++11。
- 
获取GCC 4.8.2包：wget http://gcc.skazkaforyou.com/releases/gcc-4.8.2/gcc-4.8.2.tar.gz；

- 
解压缩：tar -xf gcc-4.8.2.tar.gz；

- 
进入到目录gcc-4.8.2，运行：./contrib/download_prerequisites。这个神奇的脚本文件会帮我们下载、配置、安装依赖库，可以节约我们大量的时间和精力。

- 
建立输出目录并到目录里：mkdir gcc-build-4.8.2；cd gcc-build-4.8.2；

- 
../configure –enable-checking=release –enable-languages=c,c++ –disable-multilib。–enable-languages表示你要让你的gcc支持那些语言，–disable-multilib不生成编译为其他平台可执行代码的交叉编译器。–disable-checking生成的编译器在编译过程中不做额外检查，也可以使用–enable-checking=xxx来增加一些检查；

- 
编译：make；注意此步和上一步，比较耗时；

- 
安装：make  install；

- 
验证：gcc -v；或者g++ -v，如果显示的gcc版本仍是以前的版本，就需要重启系统；或者可以查看gcc的安装位置：which gcc；然后在查看版本 /usr/local/bin/gcc -v，通常gcc都安装在该处位置，如果显示为；


升级Centos系统之后，运行filezilla时，出现如下错误的提示信息：

./filezilla: /usr/lib/libstdc++.so.6: version `GLIBCXX_3.4.15' not found (required by ./filezilla)

如果是64位系统报错信息如下：

./filezilla: /usr/lib64/libstdc++.so.6: version `GLIBCXX_3.4.15' not found (required by ./filezilla)

原因是没有GLIBCXX_3.4.15版本，或是更高的版本。

输入命令查询一下结果：

[root@localhost ~]# strings /usr/lib/libstdc++.so.6 | grep GLIBCXX

GLIBCXX_3.4

GLIBCXX_3.4.1

GLIBCXX_3.4.2

GLIBCXX_3.4.3

GLIBCXX_3.4.4

GLIBCXX_3.4.5

GLIBCXX_3.4.6

GLIBCXX_3.4.7

GLIBCXX_3.4.8

GLIBCXX_3.4.9

GLIBCXX_3.4.10

GLIBCXX_3.4.11

GLIBCXX_3.4.12

GLIBCXX_3.4.13

GLIBCXX_FORCE_NEW

GLIBCXX_DEBUG_MESSAGE_LENGTH

我们看到当前GCC版本中的确没有GLIBCXX_3.4.15。

64位系统命令为：

[root@localhost ~]# strings /usr/lib64/libstdc++.so.6 | grep GLIBCXX

下载新版本，地址：http://ftp.de.debian.org/debian/pool/main/g/gcc-4.7/libstdc++6_4.7.2-5_i386.deb

64位系统：

http://ftp.de.debian.org/debian/pool/main/g/gcc-4.7/libstdc++6_4.7.2-5_amd64.deb

假设下载的文件放在/data0/software/。

解压：

[root@localhost software]# ar -x libstdc++6_4.7.2-5_i386.deb && tar xvf data.tar.gz

解压后出现/data0/software/usr目录；

[root@localhost software]# cd  /data0/software/usr/lib/i380-linux-gun

[root@localhost i386-linux-gnu]# cp libstdc++.so.6.0.17 /usr/lib

[root@localhost i386-linux-gnu]# /usr/lib

[root@localhost lib]# rm libstdc++.so.6

[root@localhost lib]# ln libstdc++.so.6.0.17 libstdc++.so.6

注意64位系统为

[root@localhost software]# cd  /data0/software/usr/lib/x86_64-linux-gnu

[root@localhost x86_64-linux-gnu]# cp libstdc++.so.6.0.17 /usr/lib64

[root@localhost x86_64-linux-gnu]# /usr/lib

[root@localhost lib64]# rm libstdc++.so.6

[root@localhost lib64]# ln libstdc++.so.6.0.17 libstdc++.so.6

重新建立软连接，我们再输入命令查询一下结果：

[root@localhost lib]# strings /usr/lib/libstdc++.so.6 | grep GLIBCXX

GLIBCXX_3.4

GLIBCXX_3.4.1

GLIBCXX_3.4.2

GLIBCXX_3.4.3

GLIBCXX_3.4.4

GLIBCXX_3.4.5

GLIBCXX_3.4.6

GLIBCXX_3.4.7

GLIBCXX_3.4.8

GLIBCXX_3.4.9

GLIBCXX_3.4.10

GLIBCXX_3.4.11

GLIBCXX_3.4.12

GLIBCXX_3.4.13

GLIBCXX_3.4.14

GLIBCXX_3.4.15

GLIBCXX_3.4.16

GLIBCXX_3.4.17

GLIBCXX_DEBUG_MESSAGE_LENGTH

已经安装好GLIBCXX_3.4.15啦。





## [centos下搭建kdump环境](http://blog.chinaunix.net/uid-26557245-id-3199660.html)








kexec是一个快速启动机制，允许通过已经运行的内核的上下文启动一个Linux内核，不需要经过BIOS。BIOS可能会消耗很多时间，特别是带有众多数量的外设的大型服务器。这种办法可以为经常启动机器的开发者节省很多时间。


kdump是一个新的，而且非常可信赖的内核崩溃转储机制。崩溃转储数据可以从一个新启动的内核的上下文中获取，而不是从已经崩溃的内核的上下文。当系统崩溃时，kdump使用kexec启动到第二个内核。第二个内核通常叫做捕获内核（capture kernel），以很小内存启动，并且捕获转储镜像。


第一个内核保留了内存的一部分，第二个内核可以用来启动。注意，在启动时，kdump保留了一定数量的重要的内存，这改变了红帽企业Linux 5最小内存需求。为了计算系统需要的真正最小内存，可以参看 http://www.[RedHat](http://www.linuxidc.com/topicnews.aspx?tid=10).com/rhel/details/limits/
 上列出的最小内存需求，加上kdump使用的内存数量，以决定真正的最小内存的需求。


因为第一个内核的内存内容已经被保留，所以kexec可以不经过BIOS，启动捕获内核。这是内核崩溃转储的根本。






怎样配置kdump


1.确认kexec-tools已经安装：


#rpm -q kexec-tools


2.确认kernel-debuginfo和其支持包kernel-debuginfo-common已经安装

#rpm -qa|grep kernel

下载地址[http://debuginfo.centos.org/6/x86_64/](http://debuginfo.centos.org/6/x86_64/)

3.配置/etc/kdump.conf文件，指定vmcore将被转储的路径。可以通过scp拷贝到另一个服务器，也可以是裸设备，或者本地的文件系统。

path /var/crash

4.修改/etc/sysctl.conf文件添加以下内容：



vm.panic_on_oom = 1

kernel.panic_on_unrecovered_nmi = 0

kernel.unknown_nmi_panic = 0

kernel.panic_on_oops = 1







5.修改一些启动参数，为捕获很保留一块内存。对于i386和x86_64架构，编辑/etc/grub.conf，在内核行的末尾添加

ro root=LABEL=/1 rhgb quiet crashkernel=128M@32M


下面是一个带有kdump选项的/etc/grub.conf文件：


# grub.conf generated by anaconda

#

# Note that you do not have to rerun grub after making changes to this file

# NOTICE:  You do not have a /boot partition.  This means that

#          all kernel and initrd paths are relative to /, eg.

#          root (hd0,0)

#          kernel /boot/vmlinuz-version ro root=/dev/hda1

#          initrd /boot/initrd-version.img

#boot=/dev/hda

default=0

timeout=5

splashimage=(hd0,0)/boot/grub/splash.xpm.gz

hiddenmenu

title [Red Hat](http://www.linuxidc.com/topicnews.aspx?tid=10) Enterprise Linux Client (2.6.17-1.2519.4.21.el5)

        root (hd0,0)

        kernel /boot/vmlinuz-2.6.17-1.2519.4.21.el5 ro root=LABEL=/ rhgb quiet crashkernel=128M@16M

        initrd /boot/initrd-2.6.17-1.2519.4.21.el5.img

6.修改之后，重启系统。128M内存（从16M开始）不被正常的系统使用，为捕获内核保留。注意，free -m的输出会显示内存比不加参数时少了128M，这就是我们所期望的。


注意：可以使用小于128M，但是只使用64M做测试被证实是不可靠的。

在/boot/grub/menu.[ls](http://www.linuxso.com/command/ls.html)t/中需要添加这样的参数，即crashkernel=X@Y，其中X是转储空间大小（确切的讲，是转储文件的最大大小），Y是转储的内存偏移。各种参考资料，包括官方给出的资料都是填写128M@16M，但是有的时候，16M偏移的内存已经被占用。这个时候，需要改成32M，相应的，在make
 menuconfig 时候的编译选项CONFIG_PHYSICAL_START=0x1000000 也需要改成0x2000000。发生内存冲突时候，kdump服务起不来，报错是缺少crashkernel这个启动选项，而官方的文档中的解决方法只是说重新检查这个启动文件的书写，真是很迷惑人。我检查很多次都没有发现问题，最终搜索一下午，在网上的一个bug报告中发现了这个问题，唬人呀！





7.现在，保留内存已经设置了，打开kdump初始脚本，启动服务：


#  chkconfig kdump on

#  service kdump start

8.可以通过kexec加载内核镜像，让系统准备捕获一个崩溃时产生的vmcore。可以通过sysrq强制系统崩溃：


# echo "c" > /proc/sysrq-trigger

这造成kernel panic，紧跟着系统重启kdump内核。当启动进程进入到启动kdump服务器时，vmcore将会被拷贝到你在/etc/kdump.conf文件中指定的位置。



注意：


终端frame-buffer和X将运行不正常。在运行一些类似于在内核配置上添加了"vga=791"或者运行X的系统，在通过kexec启动内核时，终端显示将不清楚。记住，kdump内核仍旧能够创建转储。当系统重启，显示将会恢复到正常状态。](https://so.csdn.net/so/search/s.do?q=centos 配置Kdump&t=blog)](https://so.csdn.net/so/search/s.do?q=centos 安装Eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=centos 6.4 升级gcc 4.8&t=blog)




