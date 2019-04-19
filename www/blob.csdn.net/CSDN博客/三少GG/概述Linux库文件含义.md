# 概述Linux库文件含义 - 三少GG - CSDN博客
2010年09月12日 01:45:00[三少GG](https://me.csdn.net/scut1135)阅读数：809标签：[linux																[library																[gtk																[path																[oracle																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=gtk&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
# **秘籍：概述Linux库文件含义**
　　经过长时间学习linux库文件的概念，今天小编也给大家搜集了关于库的概念以及作用，希望大家看后会有很多收获。
**1.什么是linux库**
　　在windows平台和linux平台下都大量存在着库。本质上来说库是一种可执行代码的二进制形式，可以被操作系统载入内存执行。由于windows和linux的本质不同，因此二者库的二进制是不兼容的。本文仅限于介绍linux下的库。
**2.linux库的种类**
　　linux库有两种：静态库和共享库。二者的不同点在于代码被载入的时刻不同。静态库的代码在编译过程中已经被载入可执行程序，因此体积较大。共享库的代码是在可执行程序运行时才载入内存的，在编译过程中仅简单的引用，因此代码体积较小。
**3.库存在的意义**
　　库是别人写好的现有的，成熟的，可以复用的代码，你可以使用但要记得遵守许可协议。现实中每个程序都要依赖很多基础的底层库，不可能每个人的代码都从零开始，因此库的存在意义非同寻常。共享库的好处是，不同的应用程序如果调用相同的库，那么在内存里只需要有一份该共享库的实例。
**4.库文件是如何产生的在linux下**
　　静态库的后缀是.a，它的产生分两步
　　Step 1.由源文件编译生成一堆.o，每个.o里都包含这个编译单元的符号表
　　Step 2.ar命令将很多.o转换成.a，成文静态库
　　动态库的后缀是.so，它由gcc加特定参数编译产生。
　　例如:
　　$ gcc -fPIC -c *.c $ gcc -shared -Wl,-soname, libfoo.so.1 -o libfoo.so.1.0 *.
**5.linux库文件是如何命名的，有没有什么规范**
　　在linux下，库文件一般放在/usr/lib /lib下，静态库的名字一般为libxxxx.a，其中xxxx是该lib的名称动态库的名字一般为libxxxx.so.major.minor，xxxx是该lib的名称，major是主版本号， minor是副版本号
**　6.如何知道一个可执行程序依赖哪些库**
　　ldd命令可以查看一个可执行程序依赖的共享库，例如# ldd /bin/lnlibc.so.6
　　=> /lib/libc.so.6 (0×40021000)/lib/ld-linux.so.2
　　=> /lib/ld- linux.so.2 (0×40000000)
　　可以看到ln命令依赖于libc库和ld-linux库
**7.可执行程序在执行的时候如何定位共享库文件**
　　当系统加载可执行代码时候，能够知道其所依赖的库的名字，但是还需要知道绝对路径此时就需要系统动态载入器(dynamic linker/loader)对于elf格式的可执行程序，是由ld-linux.so*来完成的，它先后搜索elf文件的 DT_RPATH段—环境变量LD_LIBRARY_PATH—/etc/ld.so.cache文件列表—/lib/,/usr/lib目录找到库文件后将其载入内存
**8.在新安装一个库之后如何让系统能够找到他**
　　如果安装在/lib或者/usr/lib下，那么ld默认能够找到，无需其他操作。
　　如果安装在其他目录，需要将其添加到/etc/ld.so.cache文件中，步骤如下
　　1.编辑/etc/ld.so.conf文件，加入库文件所在目录的路径
　　2.运行ldconfig，该命令会重建/etc/ld.so.cache文件
　　以上给大家讲解的是linux库文件的概念。
## linux绝对路径1全面分解linux库文件路径的添加
时间:2010-06-12 17:19来源:未知 作者:admin 点击:次
全面剖析linux库文件路径的添加摘要：本文讲解linux库文件路径中添加自己的路径。添加方法也极其简单，将库文件的绝对路径直接写进去，第二种搜索路径的设置方式对于程序连接时的库。标签：linux库文件路径oracle帮您准确洞察各个物流环节 linux操作系统中，
　　全面剖析linux库文件路径的添加摘要：本文讲解linux库文件路径中添加自己的路径。添加方法也极其简单，将库文件的绝对路径直接写进去，第二种搜索路径的设置方式对于程序连接时的库。标签：linux库文件路径oracle帮您准确洞察各个物流环节
　　linux操作系统中，linux库文件路径还是比较常用的，于是我研究了一下linux库文件路径，在这里拿出来和大家分享一下，希望对大家有用。
　　库文件在连接（静态库和共享库）和运行（仅限于使用共享库的程序）时被使用，其搜索路径是在系统中进行设置的。一般linux系统把/lib和/usr/lib两个目录作为默认的库搜索路径，所以使用这两个目录中的库时不需要进行设置搜索路径即可直接使用。对于处于默认库搜索路径之外的库，需要将库的位置添加到库的搜索路径之中。设置库文件的搜索路径有下列两种方式，可任选其一使用：
　　在环境变量ld_library_path中指明库的搜索路径。
　　在/etc/ld.so.conf文件中添加库的搜索路径。
　　将自己可能存放linux库文件的路径都加入到/etc/ld.so.conf中是明智的选择
　　添加方法也极其简单，将库文件的绝对路径直接写进去就ok了，一行一个。例如：
　　/usr/x11r6/lib
　　/usr/local/lib
　　/opt/lib
　　需要注意的是：第二种搜索路径的设置方式对于程序连接时的库（包括共享库和静态库）的定位已经足够了，但是对于使用了共享库的程序的执行还是不够的。这是因为为了加快程序执行时对共享库的定位速度，避免使用搜索路径查找共享库的低效率，所以是直接读取库列表文件/etc/ld.so.cache从中进行搜索的。/etc/ld.so.cache是一个非文本的数据文件，不能直接编辑，它是根据/etc/ld.so.conf中设置的搜索路径由/sbin/ldconfig命令将这些搜索路径下的共享库文件集中在一起而生成的（ldconfig命令要以root权限执行）。因此，为了保证程序执行时对库的定位，在/etc/ld.so.conf中进行了库搜索路径的设置之后，还必须要运行/sbin/ldconfig命令更新/etc/ld.so.cache文件之后才可以。ldconfig,简单的说，它的作用就是将/etc/ld.so.conf列出的路径下的库文件缓存到/etc/ld.so.cache以供使用。因此当安装完一些linux库文件路径，(例如刚安装好glib)，或者修改ld.so.conf增加新的库路径后，需要运行一下/sbin/ldconfig使所有的库文件都被缓存到ld.so.cache中，如果没做，即使库文件明明就在/usr/lib下的，也是不会被使用的，结果编译过程中抱错，缺少xxx库，去查看发现明明就在那放着，搞的想大骂computer蠢猪一个。
　　在程序连接时，对于linux库文件（静态库和共享库）的搜索路径，除了上面的设置方式之外，还可以通过-l参数显式指定。因为用-l设置的路径将被优先搜索，所以在连接的时候通常都会以这种方式直接指定要连接的库的路径。
　　前面已经说明过了，搜索linux库文件路径的设置有两种方式：在环境变量ld_library_path中设置以及在/etc/ld.so.conf文件中设置。其中，第二种设置方式需要root权限，以改变/etc/ld.so.conf文件并执行/sbin/ldconfig命令。而且，当系统重新启动后，所有的基于gtk2的程序在运行时都将使用新安装的gtk+库。不幸的是，由于gtk+版本的改变，这有时会给应用程序带来兼容性的问题，造成某些程序运行不正常。为了避免出现上面的这些情况，在gtk+及其依赖库的安装过程中对于库的搜索路径的设置将采用第一种方式进行。这种设置方式不需要root权限，设置也简单：
　　$exportld_library_path=/opt/gtk/lib:$ld_library_path
　　可以用下面的命令查看ld_libray_path的设置内容：
　　$echo$ld_library_path
　　至此，库的两种设置就完成了。
　　以上给大家讲解的是linux库文件，关于linux系统中如何添加自己的库文件路径。
(责任编辑：admin)
