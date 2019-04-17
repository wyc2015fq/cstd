# 关于Linux启动时挂载rootfs的几种方式 - DoubleLi - 博客园






一直对Linux启动时挂载根文件系统的过程存在着很多疑问，今天在水木精华区找到了有用的资料，摘录如下：

1、Linux启动时，经过一系列初始化之后，需要mount 根文件系统，为最后运行init进程等做准备，mount 根文件系统有这么几种方式：

1)文件系统已经存在于硬盘(或者类似的设备)的某个分区上了，kernel根据启动的命令行参数(root=/dev/xxx),直接进行mount。 这里有一个问题，在root文件系统本身还不存在的情况下，kernel如何根据/dev/xxx来找到对应的设备呢?注意：根文件系统和其他文件系统的mount方式是不一样的，kernel通过直接解析设备的名称来获得设备的主、从设备号，然后就可以访问对应的设备驱动了。所以在init/main.c中有很长一串的root_dev_names(如hda,hdab,sda,sdb,nfs,ram,mtdblock……)，通过这个表就可以根据设备名称得到设备号。注意，bootloader或内核中设定的启动参数(root=/dev/xxx)只是一个代号，实际的根文件系统中不一定存在这个设备文件!

2)从软驱等比较慢的设备上装载根文件系统，如果kernel支持ramdisk，在装载root文件系统时，内核判断到需要从软盘(fdx)mount(root=/dev/fd0)，就会自动把文件系统映象复制到ramdisk，一般对应设备ram0，然后在ram0上mount根文件系统。 从源码看，如果kernel编译时没有支持ramdisk，而启动参数又是root=/dev/fd0,系统将直接在软盘上mount，除了速度比较慢，理论上是可行的(没试过，不知道是不是这样?)

3)启动时用到initrd来mount根文件系统。注意理解ramdisk和initrd这两个概念，其实ramdisk只是在ram上实现的块设备，类似与硬盘操作，但有更快的读写速度，它可以在系统运行的任何时候使用，而不仅仅是用于启动;initrd(boot loaderinitialized RAMdisk)可以说是启动过程中用到的一种机制，具体的实现过程也使用ramdisk技术。就是在装载linux之前，bootloader可以把一个比较小的根文件系统的映象装载在内存的某个指定位置，姑且把这段内存称为initrd(这里是initrd所占的内存，不是ramdisk，注意区别)，然后bootloader通过传递参数的方式告诉内核initrd的起始地址和大小(也可以把这些参数编译在内核中)，在启动阶段就可以暂时的用initrd来mount根文件系统。initrd的最初的目的是为了把kernel的启动分成两个阶段：在kernel中保留最少最基本的启动代码，然后把对各种各样硬件设备的支持以模块的方式放在initrd中，这样就在启动过程中可以从initrd所mount的根文件系统中装载需要的模块。这样的一个好处就是在保持kernel不变的情况下，通过修改initrd中的内容就可以灵活的支持不同的硬件。在启动完成的最后阶段，根文件系统可以重新mount到其他设备上，但是也可以不再 重新mount(很多嵌入式系统就是这样)。initrd的具体实现过程是这样的：bootloader把根文件系统映象装载到内存指定位置，把相关参数传递给内核，内核启动时把initrd中的内容复制到ramdisk中(ram0)，把initrd占用的内存释放掉，在ram0上mount根文件系统。从这个过程可以看出，内核需要对同时对ramdisk和initrd的支持(这种需要都编入内核，不能作为模块)。

2、嵌入式系统根文件系统的一种实现方法：对于kernel和根文件系统都存储在flash中的系统，一般可以利用linux启动的initrd的机制。具体的过程前面已经比较清楚了，还有一点就是在启动参数中传递root=/dev/ram0，这样使得用initrd进行mount的根文件系统不再切换，因为这个时候实际的设备就是ram0。还有就是initrd的起始地址参数为虚拟地址，需要和bootloader中用的物理地址对应。









