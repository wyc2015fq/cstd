# cmem模块/DVSDK2.0 - maopig的专栏 - CSDN博客
2012年01月07日 18:13:48[maopig](https://me.csdn.net/maopig)阅读数：3037标签：[makefile																[linux																[存储																[算法																[服务器																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)
个人分类：[DM368](https://blog.csdn.net/maopig/article/category/919673)
1. CMEM——高速缓存一致性问题的解决多核设计中，共享的二级高速缓存之间数据可能不一致，不同CPU内核的私有高速缓存也可能存在数据不一致，称为高速缓存的一致性问题。解决一致性问题的方法从整体上分可以分成软件方法和硬件方法两种，各有其优势。Davinci采用软件解决方案。
1) 数据完整性。CE中的CMEM模块是用来做共享内存分配的，因为应用程序是运行在MVista Linux上的，在应用程序里malloc到的buffer都是虚拟地址，实际的物理空间不一定连续，当把这个指针传递给算法的时候，数据完整性问题就出现了，因为算法是运行在DSP/BIOS上的，这是一个只有实地址的世界。为了解决这个问题，在共享缓存动态申请空间的时候，就要调用CMEM API。CMEM驱动模式可以用来在ARM上分配连续的物理缓冲区。
2) CACHE一致性问题。在ARM和DSP内核中都有CACHE用来提高使用外部存储的效率。所有的内核在当它们进行读写的时候，都管理着与它们相关的CAHCE的一致性。但是，当数据经共享内存从一个内核发送到另一个内核时，内核是从不进行CACHE的管理的，因为它们无法互相得知。像虚拟地址问题一样，只要编程者注意到的话这个问题是很容易解决的。当客户端写一个客户端存根函数(stub)来对要发送到服务器端的一个变量或者缓冲区进行CACHE回写时，服务器端就写服务端存根函数(stub)来使得收到的这些变量和缓冲区无效。
2. 模块cmemk.ko介绍 模块cmemk.ko为ARM和DSP通信提供连续的内存，装载的格式为： insmod cmemk.ko pools=4x30000, 2x500000 phys_start=0x0 phys_end=0x3000000 / pools_1=4x65536 phys_start_1=0x80000000 phys_end_1=0x80010000 这条命令表明：
（1）在内存0x0~0x2ffffff上开辟了两个内存池，第一个由4个30000字节的缓存组成，第二个由2个500000字节的缓存组成。       
（2）在内存0x80000000~0x8000ffff上开辟了一个由4个65536字节缓存组成的内存池。（注：低版本的cmem只支持一块内存） 按照Davinci内存的划分，cmem的内存空间位于linux和DSP使用的内存之间，在dvsdk2.0中cmem内存空间为118~128M（低版本的dvsdk为120~128M，不过低版本的dvsdk的DSP server程序128M之后的很大一块也没有使用，cmem大一点也没事，不过2.0中DSP server程序内存使用从128M开始） dvsdk2.0中，cmem模块目录为：
 [root@localhost cmem]# pwd /opt/dvsdk_2_00_00_22/linuxutils_2_23_01/packages/ti/sdo/linuxutils/cmem cmem提供的接口在demo中没有出现，而是被中间件进行了封装。dvsdk1.3中内存的分配使用了CE中OSAL包中Memory模块的 Memory_contigAlloc()函数，dvsdk2.0的demo使用了DMAI模块（在CE基础上进一步进行了封装）。 CMEM是一个连续物理存储空间分配模块，使得ARM端Linux进程和DSP端算法之间能够共享缓冲区。当应用程序需要在共享缓存区动态申请一个连续的物理空间时，通过调用CMEM的API可以实现，申请得到的空间可以供DSP端访问，进行算法处理时数据的传递与处理。
 在DVSDK中集成了CMEM模块，CMEM模块安装在Linux服务器下的路径为：/opt/dvevm_1_20/cmem_1_02 在Linux服务器的控制台下，需进行如下操作实现编译： Host # cd /opt/dvevm_1_20/cmem_1_02/packages/ti/sdo/linuxutils/cmem Host # make all Host #make install 运行后，即对cmem模块进行了编译，编译文件为在目录src/module下生成的cmem.ko模块，并将cmem.ko和测试文件安装到了目标板的/opt/nfs/opt/dvsdemos目录下。
 DVSDK中的CMEM Demo提供了测试程序：apitest能够测试CMEM的API功能，translate能够测试虚拟地址与实物理地址之间的转换功能。  
模块cmemk.ko为ARM和DSP通信提供连续的内存，装载的格式为：
insmod cmemk.ko pools=4x30000,2x500000 phys_start=0x0 phys_end=0x3000000 pools_1=4x65536 phys_start_1=0x80000000 phys_end_1=0x80010000
表明在内存0x0~0x2ffffff上开辟了两个内存池，第一个由4个30000字节得缓存组成，第二个由2个500000字节的缓存组成。在内存 0x80000000~0x8000ffff上开辟了一个由4个65536字节缓存组成的内存池。（注：低版本的cmem只支持一块内存）
按照Davinci内存的划分，cmem的内存空间位于linux内存和DSP算法使用的内存之间，在dvsdk2.0中cmem内存空间为 118~128M（低版本的dvsdk为120~128M，不过低版本的dvsdk的DSP server程序128M之后的很大一块也没有使用，cmem大一点也没事，不过2.0中DSP server程序内存使用从128M开始）
dvsdk2.0中，cmem模块目录为：
[root@localhost cmem]# pwd 
/opt/dvsdk_2_00_00_22/linuxutils_2_23_01/packages/ti/sdo/linuxutils/cmem
[root@localhost cmem]# tree 
. 
|-- CMEM.xdc 
|-- Makefile 
|-- Rules.make 
|-- apps 
| |-- Makefile 
| |-- apitest 
| | |-- Makefile 
| | `-- apitest.c 
| |-- insmod_rmmod 
| | |-- Makefile 
| | |-- insmod_rmmod.sh 
| | `-- insmod_rmmod_omapL137.sh 
| |-- multi_process 
| | |-- Makefile 
| | `-- multi_process.c 
| `-- translate 
| |-- Makefile 
| `-- translate.c 
|-- include 
| `-- cmem.h 
|-- lib 
| `-- cmem.a470MV 
|-- package 
| |-- build.cfg 
| |-- package.bld.xml 
| |-- package.defs.h 
| |-- package.doc.xml 
| |-- package.ext.xml 
| |-- package.rel.dot 
| |-- package.rel.xml 
| |-- package_ti.sdo.linuxutils.cmem.c 
| |-- ti.sdo.linuxutils.cmem.ccs 
| `-- ti.sdo.linuxutils.cmem.sch 
|-- package.xdc 
|-- package.xs 
|-- readme.txt 
`-- src 
|-- Makefile 
|-- interface 
| |-- Makefile 
| |-- cmem.c 
| |-- cmem.h 
| `-- release 
| `-- cmem.o470MV 
`-- module 
|-- Makefile 
|-- Module.symvers 
|-- cmemk.c 
|-- cmemk.ko 
|-- cmemk.mod.c 
|-- cmemk.mod.o 
`-- cmemk.o
12 directories, 40 files 
[root@localhost cmem]#
cmem提供的接口在demo中没有出现，而是被中间件进行了封装。dvsdk1.3中内存的分配使用了CE中OSAL包中Memory模块的 Memory_contigAlloc()函数，dvsdk2.0的demo使用了DMAI模块（在CE基础上进一步进行了封装）。
CMEM是一个连续物理存储空间分配模块，使得ARM端Linux进程和DSP端算法之间能够**共享缓冲区**。当**应用程序**需要在**共享缓存区**动态申请一个连续的物理空间时，通过调用**CMEM的API**可以实现，**申请得到的空间可以供DSP端访问**，**进行算法处理时数据的传递与处理**。
在DVSDK中集成了CMEM模块，CMEM模块安装在Linux服务器下的路径为：/opt/dvevm_1_20/cmem_1_02
在Linux服务器的控制台下，需进行如下操作实现编译：
Host # cd /opt/dvevm_1_20/cmem_1_02/packages/ti/sdo/linuxutils/cmem
Host # make all
Host #make install
运行后，即对cmem模块进行了编译，编译文件为在目录src/module下生成的cmem.ko模块，并将cmem.ko和测试文件安装到了目标板的/opt/nfs/opt/dvsdemos目录下。
DVSDK中的CMEM Demo提供了测试程序：apitest能够测试CMEM的API功能，**translate能够测试虚拟地址与实物理地址之间**的转换功能。
...............................................................
DM365的视频处理涉及到三个相关处理器，分别是视频采集芯片、ARM处理器和视频图像协处理器（VICP），整个处理流程由ARM核协调。视频处理主要涉及三个处理流程，分别是视频采集、视频编码和对编码后的视频的处理，为了提高性能，通常为每个处理流程提供一个处理线程。
### 视频采集
TVP5146将采集到的视频数据转化为数字信号，并将这些数据送入DM365的BT656接口，然后通过Resize得到所需要的分辨率，然后将这些数据写入到指定的内存中，这些内存空间由cmem模块分配。cmem模块用于分配连续的存储空间，连续的存储空间可以提高数据的读写效率。
### 视频编码
VICP从指定的存储空间中读入视频数据，并将这些数据编码为指定的视频/图像格式，然后将编码后的数据写入到指定的存储空间，这些存储空间通常也是由cmem模块分配。
### 编码数据处理
ARM从指定的内存空间中获取编码后的视频数据，可以将这些编码后的数据保存到本地或远端，也可以使用RTP协议发送到网络。
整个过程由ARM核协调，为了提高效率，大量使用DMA操作和cmem模块分配的连续存储空间。
Linux下文件的类型是不依赖于其后缀名的，但一般来讲：
.ko 是Linux 2.6内核使用的动态连接文件的后缀名，也就是模块文件，用来在Linux系统启动时加载内核模块
.o 是目标文件,相当于windows中的.obj文件
.so 为共享库,是shared object,用于动态连接的,和dll差不多
.a 为静态库,是好多个.o合在一起,用于静态连接
.la 为libtool自动生成的一些共享库，vi编辑查看，主要记录了一些配置信息。可以用如下命令查看file *.la来查看文件类型
创建.a库文件和.o库文件：
$ gcc -c mylib.c
$ ar -r mylib.a mylib.o
.o文件与.ko文件的区别
.o的文件是object文件，.ko是kernel object，与.o的区别在于其多了一些sections,比如.modinfo。.modinfo section是由kernel source里的modpost工具生成的，包括MODULE_AUTHOR, MODULE_DESCRIPTION, MODULE_LICENSE, device ID table以及模块依赖以来关系等等。depmod
 工具根据.modinfo section生成modules.dep, modules.*map等文件，以便modprobe更方便的加载模块。
在2.6内核下编译driver, 首先要确认你的linux系统里有没有kernel tree，检查的方法是看看文件夹/lib/modules/$(uname -r)/build是否指向source code，并进去看看，是否有Makefile, include, scripts等文件夹，在scripts/mod下可以找到前面提到的modpost，这个东东是 生成.ko的关键。 如果没有kernel tree，需要重新build一份kernel来建立这个kernel tree。
在2.4内核中，生成的就是.o文件，2.6 kernel对内核模块的管理做了一些扩展！
