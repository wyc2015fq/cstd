# WinCE BSP的BIB文件介绍(转载) - xqhrs232的专栏 - CSDN博客
2010年01月20日 14:15:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：809
原文地址
[http://tech.e800.com.cn/articles/2009/619/1245382898317_1.html](http://tech.e800.com.cn/articles/2009/619/1245382898317_1.html)
在 WinCE中使用的一个重要的文件就是BIB文件，全称Binary Image Builder File。在WinCE的编译过程中会用到BIB文件，应该是在最后的Makeimg阶段。所有的BIB文件会被合并成CE.bib文件，然后 Romimage.exe会根据BIB文件中的描述来决定哪些文件最终被包含到WinCE image中。当然，BIB文件还决定了WinCE设备内存的分配，其中定义了WinCE image占用哪块内存，Framebuffer占用哪块内存等。
在BIB文件中分为4大项：MEMORY项，CONFIG项，MODULES项和FILES项。下面分别作个解释：
MEMORY：定义了内存分配的相关设置，一般在BSP中的config.bib文件中。
CONFIG：在最后的Makeimg阶段，为Romimage.exe提供一些生成WinCE image的配置属性。该项是可选的，一般也在BSP中的config.bib文件中定义。
MODULES：定义了一些会被打包到WinCE image中的模块或者文件，比如dll，exe等。这些文件会被Romimage.exe标记为加载到RAM中或者XIP。我们可以在这里添加自己的 WinCE应用程序或者模块，但是不要添加Managed Binaries，一般指.NET的程序。
FILES：定义了一些操作系统会用到的其他的文件，比如字体文件，图片等。这些文件也会在WinCE运行的时候被加载到RAM中。
下面会详细介绍上面的4大项：
1. MEMORY项
一般都在config.bib文件中定义，开头会有MEMORY的字样。这里定义了为WinCE image以及其他模块预留的RAM，同时也定义了WinCE可以使用的RAM。具体格式如下：
MEMORY
NAME Start Address Memory Size Type
NAME：该内存区域的名字，必须是唯一的。
Start Address：该内存区域的起始地址，用十六进制表示。
Memory Size：该内存区域的大小，用十六进制表示。
Type：内存区域的类型。包涵的多种类型如下。
类型值
 描述
FIXUPVAR
 用于在WinCE编译的Makeimg阶段，就初始化一个内核中的全局变量。
NANDIMAGE
 当创建了一个使用BinFS的image的时候，NAND设备上的WinCE kernel重定向到RAM中的区域，当系统访问该区域的时候，BinFS会负责访问Nand设备上相应的位置，并返回数据给系统，实际上就是在Nand设备上面实现了XIP的功能。
RAM
 定义了被WinCE系统使用的RAM区域，这块内存必须是连续的，这里有一点要注意就是从硬件的角度来说，这块内存不能跨越两片SDRAM，也就是说整个区域空间必须在一片硬件SDRAM上。
RAMIMAGE
 定义了一块内存区域用于加载WinCE image，实际上WinCE启动以后，image会被拷贝到这块内存区域上面运行。一个image只能有一个连续的RAMIMAGE区域。
RESERVED
 这块内存区域会被预留出来，一般用于Frambuffer或者是DMA Buffer，或者是一块共享内存用于EBOOT传递参数给WinCE系统。
EXTENSION
 定义了一块WinCE image中的区域作为ROMHDR extension的数据区域。
2．CONFIG项 
一般在config.bib文件中定义，
定义了一些额外的配置参数，其中一些对于WinCE image来说也很重要。具体格式如下；
CONFIG
ITEM=Parameter
ITEM
 描述
AUTOSIZE
 允许未被使用的WinCE image的RAM被用作WinCE系统的RAM。默认值为ON。
COMPRESSION 
 允许Romimage.exe压缩WinCE image中的可写入部分。默认值为ON。
BOOTJUMP
 定义了跳转跳转页在RAMIMAGE空间的地址。而不是默认情况下的RAMIMAGE的首地址。默认值为NONE。
FSRAMPERCENT
 定义了文件系统使用的内存的百分比。默认值为0x80808080。
Byte 0：第一个2MB中，每1MB所包含的4KB的倍数。
Byte 1：第二个2MB中，每1MB所包含的4KB的倍数。
Byte 2：第三个2MB中，每1MB所包含的4KB的倍数。
Byte 3：剩下的内存中，每1MB所包含的4KB的倍数。
KERNELFIXUPS
 定义了Romimage.exe是否重新定向内核的可写入区域。默认值为ON，内核的可写入区域被重新定向到RAMIMAGE的起始位置。
OUTPUT
 定义了最终生成的image存放的路径。默认为%_FLATRELEASEDIR%。
PROFILE
 定义了是否在WinCE image中包含profiler的结构和符号。默认值为OFF。
RESETVECTOR
 重新指定跳转页的位置，一般针对MIPS芯片从0x9FC00000开始引导的问题。
ROMFLAGS
 内核标记位，可以进行组合：
0x01表示禁用按需分页。
0x02表示禁用完全内核模式，完全内核模式表示所有的线程都运行在内核模式。
0x10表示只信任ROM MODULES中的模块。
0x20表示停止刷新TLB。
0x40表示按照/base链接选项中的地址加载DLL。
ROMSTART
 指WinCE image在内存中的起始地址。
ROMSIZE
 指WinCE image的大小。
ROMWIDTH
 指数据总线的宽度。
ROMOFFSET
 指定一个偏移量来修改.bin文件中的每一个记录的地址。一般用于ROM中的.bin文件加载到RAM来运行的情况，主要是表示存储.bin的位置和运行.bin的位置不一样。
SRE
 使Romimage.exe产生一个.sre文件。默认值为OFF。
X86BOOT
 定义是否插入一条跳转指令，在x86复位向量地址的时候。
3．MODULES项和FILES项
该项列出了所有被包含在WinCE image中的模块以及文件，以及这些模块和文件以什么方式被加载到内存中。具体格式如下：
MODULES
Name Path Memory Type
Name：模块的名字，比如一个dll或者exe文件的文件名。
Path：路径，一般都是WinCE的工程的Release目录。
Memory：指定该模块被放在哪个区域，一般都是NK区域。
Type：定义了文件的类型。具体如下：
类型
 描述
S
 系统文件。
H
 隐藏文件。
R
 只压缩模块的资源部分。
C
 压缩模块的所有部分。
D
 禁止调试。
N
 模块是不可信任的。
P
 告诉Romimage.exe不需要检查CPU的类型。
K
 告诉Romimage.exe必需固定该模块的内核地址。有该标记的模块只能被LoadKernelLibrary函数加载。
X
 告诉Romimage.exe对该模块签名。
M
 运行时加载整个模块，不要按需分页。
L
 告诉Romimage.exe不要分离ROM DLL。
一般FILES项的Type只支持S，H，N，D几个类型，而MODULES项的Type是都支持的。
举个例子吧：
INIT.EXE %_WINCEROOT%/RELEASE/INIT.EXE NK SH
MYDLL.DLL %_WINCEROOT%/RELEASE/MYDLL.DLL NK SHC
对于BIB文件来说同样支持“条件编译”，
我们可以通过设置环境变量来选择性地将某些模块打包到WinCE image中。一般在BSP中，对于一些驱动模块的环境变量我们IF来进行条件判断。而对于WinCE的系统模块来说，一般都是SYSGEN变量，应该使用@CESYSGEN IF来判断。
我们在BSP的开发中最常见的主要就是eboot.bib，config.bib，platform.bib和project.bib。下面简单介绍一下：
project.bib：该文件主要自创建的WinCE工程中所需的一些文件。
platform.bib：该文件包含了和硬件平台相关的文件，主要以驱动程序为主。
config.bib：该文件描述了WinCE系统的内存的配置。
eboot.bib：该文件描述了WinCE的eboot的内存的配置。
还有其他的一些bib文件，在WinCE系统编译后都会背拷贝到工程的release目录下面。比如common.bib，ie.bib等。这些文件列出了WinCE的组件相关的文件，根据用户订制的系统，会被选择性的打包到WinCE image中。
