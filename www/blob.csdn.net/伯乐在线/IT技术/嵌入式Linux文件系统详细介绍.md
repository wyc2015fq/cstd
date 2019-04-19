# 嵌入式Linux文件系统详细介绍 - 文章 - 伯乐在线
原文出处： [zxg623](http://blog.chinaunix.net/uid-11848011-id-96187.html)
Linux支持多种文件系统，包括ext2、ext3、vfat、ntfs、iso9660、jffs、romfs和nfs等，为了对各类文件系统进行统一管理，Linux引入了虚拟文件系统VFS(Virtual File System)，为各类文件系统提供一个统一的操作界面和应用编程接口。
Linux下的文件系统结构如下：
![](http://jbcdn2.b0.upaiyun.com/2016/11/19e793b23354d3634d395dd6fa5c394c.jpg)
Linux启动时，第一个必须挂载的是根文件系统；若系统不能从指定设备上挂载根文件系统，则系统会出错而退出启动。之后可以自动或手动挂载其他的文件系统。因此，一个系统中可以同时存在不同的文件系统。
不同的文件系统类型有不同的特点，因而根据[存储](http://www.storworld.com/)设备的硬件特性、系统需求等有不同的应用场合。在嵌入式Linux应用中，主要的[存储](http://www.storworld.com/)设备为RAM(DRAM, SDRAM)和ROM(常采用FLASH存储器)，常用的基于存储设备的文件系统类型包括：jffs2, yaffs, cramfs, romfs, ramdisk, ramfs/tmpfs等。
### **1. 基于FLASH的文件系统**
[Flash](http://webdesign.chinaitlab.com/List_1364.html)(闪存)作为嵌入式系统的主要存储媒介，有其自身的特性。[Flash](http://webdesign.chinaitlab.com/List_1364.html)的写入操作只能把对应位置的1修改为0，而不能把0修改为1(擦除[Flash](http://webdesign.chinaitlab.com/List_1364.html)就是把对应存储块的内容恢复为1)，因此，一般情况下，向Flash写入内容时，需要先擦除对应的存储区间，这种擦除是以块(block)为单位进行的。
闪存主要有NOR和NAND两种技术(简单比较见附录)。Flash存储器的擦写次数是有限的，NAND闪存还有特殊的硬件接口和读写时序。因此，必须针对Flash的硬件特性设计符合应用要求的文件系统；传统的文件系统如ext2等，用作Flash的文件系统会有诸多弊端。
在嵌入式Linux下，MTD(Memory Technology Device,存储技术设备)为底层硬件(闪存)和上层(文件系统)之间提供一个统一的抽象接口，即Flash的文件系统都是基于MTD驱动层的(参见上面的Linux下的文件系统结构图)。使用MTD驱动程序的主要优点在于，它是专门针对各种非易失性存储器(以闪存为主)而设计的，因而它对Flash有更好的支持、管理和基于扇区的擦除、读/写操作接口。
顺便一提，一块Flash芯片可以被划分为多个分区，各分区可以采用不同的文件系统；两块Flash芯片也可以合并为一个分区使用，采用一个文件系统。即文件系统是针对于存储器分区而言的，而非存储芯片。
(1) jffs2
JFFS文件系统最早是由瑞典Axis Communications公司基于Linux2.0的内核为嵌入式系统开发的文件系统。JFFS2是RedHat公司基于JFFS开发的闪存文件系统，最初是针对RedHat公司的嵌入式产品eCos开发的嵌入式文件系统，所以JFFS2也可以用在Linux, uCLinux中。
Jffs2: 日志闪存文件系统版本2 (Journalling Flash FileSystem v2)
主要用于NOR型闪存，基于MTD驱动层，特点是：可读写的、支持数据压缩的、基于哈希表的日志型文件系统，并提供了崩溃/掉电[安全](http://security.chinaitlab.com/)保护，提供“写平衡”支持等。缺点主要是当文件系统已满或接近满时，因为垃圾收集的关系而使jffs2的运行速度大大放慢。
目前jffs3正在开发中。关于jffs系列文件系统的使用详细文档，可参考MTD补丁包中mtd-jffs-HOWTO.txt。
jffsx不适合用于NAND闪存主要是因为NAND闪存的容量一般较大，这样导致jffs为维护日志节点所占用的内存空间迅速增大，另外，jffsx文件系统在挂载时需要扫描整个FLASH的内容，以找出所有的日志节点，建立文件结构，对于大容量的NAND闪存会耗费大量时间。
(2) yaffs：Yet Another Flash File System
yaffs/yaffs2是专为嵌入式系统使用NAND型闪存而设计的一种日志型文件系统。与jffs2相比，它减少了一些功能(例如不支持数据压缩)，所以速度更快，挂载时间很短，对内存的占用较小。另外，它还是跨平台的文件系统，除了Linux和eCos，还支持WinCE, pSOS和ThreadX等。
yaffs/yaffs2自带NAND芯片的驱动，并且为嵌入式系统提供了直接访问文件系统的API，用户可以不使用Linux中的MTD与VFS，直接对文件系统操作。当然，yaffs也可与MTD驱动程序配合使用。
yaffs与yaffs2的主要区别在于，前者仅支持小页(512 Bytes) NAND闪存，后者则可支持大页(2KB) NAND闪存。同时，yaffs2在内存空间占用、垃圾回收速度、读/写速度等方面均有大幅提升。
(3) Cramfs：Compressed ROM File System
Cramfs是Linux的创始人 Linus Torvalds参与开发的一种只读的压缩文件系统。它也基于MTD驱动程序。
在cramfs文件系统中，每一页(4KB)被单独压缩，可以随机页访问，其压缩比高达2:1,为嵌入式系统节省大量的Flash存储空间，使系统可通过更低容量的FLASH存储相同的文件，从而降低系统成本。
Cramfs文件系统以压缩方式存储，在运行时解压缩，所以不支持应用程序以XIP方式运行，所有的应用程序要求被拷到RAM里去运行，但这并不代表比Ramfs需求的RAM空间要大一点，因为Cramfs是采用分页压缩的方式存放档案，在读取档案时，不会一下子就耗用过多的内存空间，只针对目前实际读取的部分分配内存，尚没有读取的部分不分配内存空间，当我们读取的档案不在内存时，Cramfs文件系统自动计算压缩后的资料所存的位置，再即时解压缩到RAM中。
另外，它的速度快，效率高，其只读的特点有利于保护文件系统免受破坏，提高了系统的可靠性。
由于以上特性，Cramfs在嵌入式系统中应用广泛。
但是它的只读属性同时又是它的一大缺陷，使得用户无法对其内容对进扩充。
Cramfs映像通常是放在Flash中，但是也能放在别的文件系统里，使用loopback 设备可以把它安装别的文件系统里。
(4) Romfs
传统型的Romfs文件系统是一种简单的、紧凑的、只读的文件系统，不支持动态擦写保存，按顺序存放数据，因而支持应用程序以XIP(eXecute In Place，片内运行)方式运行，在系统运行时，节省RAM空间。uC[linux](http://linux.chinaitlab.com/)系统通常采用Romfs文件系统。
其他文件系统：fat/fat32也可用于实际嵌入式系统的扩展存储器(例如PDA, Smartphone, 数码相机等的SD卡)，这主要是为了更好的与最流行的[Windows](http://windows.chinaitlab.com/)桌面操作系统相兼容。ext2也可以作为嵌入式Linux的文件系统，不过将它用于FLASH闪存会有诸多弊端。
### **2. ****基于RAM的文件系统**
(1) Ramdisk
Ramdisk是将一部分固定大小的内存当作分区来使用。它并非一个实际的文件系统，而是一种将实际的文件系统装入内存的机制，并且可以作为根文件系统。将一些经常被访问而又不会更改的文件(如只读的根文件系统)通过Ramdisk放在内存中，可以明显地提高系统的性能。
在Linux的启动阶段，initrd提供了一套机制，可以将内核映像和根文件系统一起载入内存。
(2)ramfs/tmpfs
Ramfs是Linus Torvalds开发的一种基于内存的文件系统，工作于虚拟文件系统(VFS)层，不能格式化，可以创建多个，在创建时可以指定其最大能使用的内存大小。(实际上，VFS本质上可看成一种内存文件系统，它统一了文件在内核中的表示方式，并对磁盘文件系统进行缓冲。)
Ramfs/tmpfs文件系统把所有的文件都放在RAM中，所以读/写操作发生在RAM中，可以用ramfs/tmpfs来存储一些临时性或经常要修改的数据，例如/tmp和/var目录，这样既避免了对Flash存储器的读写损耗，也提高了数据读写速度。
Ramfs/tmpfs相对于传统的Ramdisk的不同之处主要在于：不能格式化，文件系统大小可随所含文件内容大小变化。
Tmpfs的一个缺点是当系统重新引导时会丢失所有数据。
### **3. 网络文件系统NFS (Network File System)**
NFS是由Sun开发并发展起来的一项在不同机器、不同操作系统之间通过网络共享文件的技术。在嵌入式Linux系统的开发调试阶段，可以利用该技术在主机上建立基于NFS的根文件系统，挂载到嵌入式设备，可以很方便地修改根文件系统的内容。
以上讨论的都是基于存储设备的文件系统(memory-based file system)，它们都可用作Linux的根文件系统。实际上，Linux还支持逻辑的或伪文件系统(logical or pseudo file system)，例如procfs(proc文件系统)，用于获取系统信息，以及devfs(设备文件系统)和sysfs，用于维护设备文件。
附录：NOR闪存与NAND闪存比较
|NOR FLASH|NAND FLASH|
|----|----|
|接口时序同SRAM,易使用|地址/数据线复用，数据位较窄|
|读取速度较快|读取速度较慢|
|擦除速度慢，以64-128KB的块为单位|擦除速度快，以8－32KB的块为单位|
|写入速度慢(因为一般要先擦除)|写入速度快|
|随机存取速度较快，支持XIP(eXecute In Place，芯片内执行)，适用于代码存储。在嵌入式系统中，常用于存放引导程序、根文件系统等。|顺序读取速度较快，随机存取速度慢，适用于数据存储(如大容量的多媒体应用)。在嵌入式系统中，常用于存放用户文件系统等。|
|单片容量较小，1－32MB|单片容量较大，8－128MB，提高了单元密度|
|最大擦写次数10万次||
