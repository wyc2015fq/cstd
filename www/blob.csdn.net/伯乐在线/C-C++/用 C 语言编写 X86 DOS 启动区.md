# 用 C 语言编写 X86 DOS 启动区 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [sunbiaobiao](http://www.jobbole.com/members/sunbiaobiao) 翻译，[黄小非](http://www.jobbole.com/members/huangxiaofei) 校稿。未经许可，禁止转载！
英文出处：[crimsonglow](http://crimsonglow.ca/~kjiwa/x86-dos-boot-sector-in-c.html)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
我一直对如何写OS很感兴趣。OS 有很多组件，其中最为基础之一的就是它的引导机制。如果让我去写产品级的 OS，我会采用 [GNU GRUB](http://www.gnu.org/software/grub/) 或 [LILO](http://lilo.alioth.debian.org/)包来直接实现，但从爱好者的角度，我对 PC 机在引导过程中的细节更感兴趣。（并且）我认为这也是学习一个简单操作系统（MS-DOS）的好途径。 MS-DOS 启动扇区所做的工作非常简单： 加载IO.SYS中指出的前3个扇区到内存，然后执行它。
BIOS 完成自检后， [IBM PC compatible](http://en.wikipedia.org/wiki/IBM_PC_compatible)（IBM PC兼容机）  会读取将头512字节块从磁盘读入到内存中0x07C00的位置，并开始执行相应内容。 引导扇区的最后2字节内容必须是0xAA55， 这个值被称为“引导签名”， （除引导签名之外）这里给代码留出了510个字节的空间。
MS-DOS需要你的磁盘采用FAT文件格式， 在加载启动扇区时，会识别出8字节的 OEM商名称和一个51字节数据结构，该结构被称为BIOS参数块数据结构， 其前3个字节应该是一个跳转指令。（除上述规定数据外）， 这里最后会给代码留出448字节的空间。如果我写一个产品级别的DOS引导扇区，在这么局限的空间下，我应当使用汇编语言。但是，（考虑到本文）作为实验和教学的目的，我感到还是使用C语言易读一些。
![](http://ww4.sinaimg.cn/mw690/63918611gw1elm42j66p3j20em08edhg.jpg)
**图：MS-DOS 引导扇区中的空闲字节和预留字节（1字节 每块）**
BIOS参数块包含关于文件系统分布的重要信息，我们用表格展示一下：

C
```
LengthName
2Bytes per sector
1Sectors per cluster
2Number of reserved sectors
1Number of file allocation tables
2Number of root entries
2Number of sectors (if < 65 536)
1Media descriptor
2Sectors per file allocation table
2Sectors per track
2Number of heads
4Number of hidden sectors
4Number of sectors (if ≥ 65 536)
1Disk drive index
1Reserved
1Volume signature
4Volume ID
11Volume label
8Volume type
```
当引导扇区被加载的时候，CPU会处于实模式，这就意味着，我们只能采用16位的指令集，以及最大1MiB的寻址范围。 前640 KiB 是可供自由分配的内存空间，剩下的384KiB则用来完成各种系统制定的功能，这就是我们熟知的常规内存区和高端内存区。
常规内存的一部分留给系统使用，前1024字节用来存放中断向量表， 下面256字节用来存放BIOS数据， 也就是前文提到的启动的时候加载512B到 [0x07C00, 0x07E00)。我们可以安全地使用在[0x00500, 0x07C00) 的29.75KiB 和在 [0x07E00, 0x80000) 的480.5KiB， 这两块加起来总共 510.25KiB。在[0x80000, 0xA0000)中还有128KiB空闲， 但是一些系统会把它们作为扩展BIOS数据区域使用。
在我自己的引导扇区实现中，我使用[0x07E00, 0x07E05) 5B来存放磁盘中的扇区数，根目录以及IO.SYS的逻辑块地址, 我使用 [0x00500, 0x07C00) 29.75B来存放根目录索引, 每个根目录记录项要占用32B, 这就意味着 IO.SYS 必须为开始的952个记录项中的一个。（MS-DOS 4.0 要求IO.SYS为root目录中的第一个记录）. 下表展示了根目录项的分布图。

C
```
Length	Name
8	Filename
3	Extension
1	Attributes
1	Reserved
1	Creation time, microseconds part
2	Creation time
2	Creation date
2	Last access date
2	Reserved
2	Last modified time
2	Last modified date
2	Cluster offset
4	File size in bytes
```
日期是16位，小端（little-endian）模式以YYYYYYYMMMMDDDDD这种格式存储， 时间戳是16位，小端（little-endian）表示的值以HHHHHMMMMMMSSSSS这种格式存储。
找到IO.SYS后, 我将头3个扇区放在以 0x00700为首地址的空间里, 我认为这3个扇区必须是连续的,这样在[0x00500, 0x00700)  就可以留出512字节的空间，稍后用来给IO.SYS存放引导扇区的完整内容。
我所面临的下一个挑战，是使用16-bit opcodes把代码编译为二进制机器码, 我很高兴地发现，通过使用GCC 和binutils，再加一点小技巧，就能做到这一点. 首先,我在C代码中加了.code16gcc汇编指令声明, 还创建了一个自定义的连接器脚本。 用这个脚本创建的二进制码带有一个带代码段，一个只读数据段,和启动签名。同时也把指令指针指向了正确的内存偏移地址。
你可以在下面的连接中看到我的工作：
- [LICENSE](http://crimsonglow.ca/~kjiwa/src/x86-dos-boot-sector-in-c/LICENSE)
- [boot.c](http://crimsonglow.ca/~kjiwa/src/x86-dos-boot-sector-in-c/boot.c)
- [boot.ld](http://crimsonglow.ca/~kjiwa/src/x86-dos-boot-sector-in-c/boot.ld)
- [Makefile](http://crimsonglow.ca/~kjiwa/src/x86-dos-boot-sector-in-c/Makefile)
作者把代码放 [Google Code](http://code.google.com/p/x86-boot-loader-c/)，遵循 MIT 协议。
