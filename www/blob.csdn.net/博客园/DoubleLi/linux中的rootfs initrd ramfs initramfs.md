# linux中的rootfs/initrd/ramfs/initramfs - DoubleLi - 博客园






什么是ramfs？
ramfs是空间规模动态变化的RAM文件系统。它非常简单，是用来实现Linux缓存机制(缓存page cache and dentry cache)的文件系统。
通常情况下，Linux的所有文件在内存中都有缓存。需要读取的数据页从支撑存储设备(block device)中读取后，缓存于内存。在支撑存储设备中的数据页执行marked as clean操作。当虚拟文件系统需要支撑存储设备中的数据页内存时，可以释放。基于同样的机制，支撑存储设备的写入操作(写入文件然后写回支撑存储设备，marked as clean)后，也可以释放占用的数据页内存。对于文件目录占用的缓存(dentry: directory entry)，也存在同样的机制。
但是，ramfs中不需要支撑存储设备(没有支撑缓存，但是有缓存)。也就是说，写入ramfs的文件可以正常的分配page cache and dentry cache，但是不能写入支撑存储设备。这些page cache and dentry cache不能被VM释放、回收。
由于ramfs可以基于现有的Linux的文件系统结构，用于实现ramfs的代码很小。一般而言，支撑存储设备的缓存被安装为一个文件系统。所以，ramfs不能通过menuconfig选择，是必然进入内核的。
在ramfs的下面可以一直写入数据，直到写满内存为止。由于VM(Vitual Memory)认为文件应该被写回支撑存储设备，而不是交换空间(swap space)，所以VM不能释放ramfs分配的内存。从而，只有root用户(or trusted user)才能进行ramfs写操作。
-------------------------------------------------------------------------------
什么是ram disk？
"ram disk"是一种实现文件系统的支撑存储（块设备）的、过时的机制(2.6不用了)，就是在RAM上面开辟的综合块设备(synthetic block device)。ram disk的大小是固定的，安装的文件系统大小(不是ramfs)也是固定的。ram disk的使用需要从这个假的块设备到page cache之间拷贝内存，生成和销毁dentry，而且需要文件系统的驱动格式化和解释上面的数据，所以ram disk的机制不再使用。
与ramfs相比，ram disk浪费了内存，也浪费了内存总线的带宽。同时，ram disk还为CPU加重了不必要的负担，污染了CPU的cache(尽管有避免污染的方法，但是非常耗费资源)。ramfs机制非常自然，因为文件访问可以通过page cache and dentry cache。ram disk被弃用的另外一个原因是环回设备(loopback)引入。环回设备提供了一种更加灵活、方便的从文件而不是从内存块中创建综合块设备的方法。
-------------------------------------------------------------------------------
什么是tmpfs？
tmpfs是ramfs的衍生物，用来限制缓存大小、向swap空间写入数据。它是用来保存VM所有文件的文件系统。tmpfs中缓存的内容全部是临时的。一旦卸载，所有的内容都会遗失。它把所有的缓存置于内核，它的规模随着文件的规模同步变化。但是它规模有大小限制，可以修改。它可以把当前不再需要的页写入到swap空间。
-------------------------------------------------------------------------------
什么是rootfs？
rootfs是ramfs的特殊实例，在2.6的内核中必然存在。rootfs不能被卸载(与其添加特殊代码用来维护空的链表，不如把rootfs节点始终加入，因此便于kernel维护：简单、精炼。rootfs是ramfs的一个空实例，占用空间极小)。大部分其他的文件系统安装于rootfs之上。
-------------------------------------------------------------------------------
什么是initramfs？
2.6的Linux内核包含有gzip压缩的cpio格式的文档，可以在内核引导的时候解压缩为rootfs。在解压缩后，kernel将检查rootfs中是否包含init文件。如果init文件存在，内核就会执行这个文件，并且赋予PID=1的进程号。这个init程序将整个系统引导起来，包括定位并且安装真正的root设备。如果在cpio文档解压缩后的rootfs中没有init程序(init文件)，内核执行旧的代码，定位并且安装root分区，执行/sbin/init程序。
-------------------------------------------------------------------------------
**initramfs与initrd的区别**1. initrd是一个单独的文件；initramfs和Linux内核链接在一起(/usr目录下的程序负责生成initramfs文档)。
2. initrd是一个压缩的文件系统映像(可以是ext2等，需要内核的驱动)；initramfs是类似tar的cpio压缩文档。内核中的cpio解压缩代码很小，而且init数据在boot后可以丢弃。
3. initrd运行的程序(initd，不是init)进行部分setup后返回内核；initramfs执行的init程序不返回内核(如果/init需要向内核传递控制权，可以再次安装在/目录下一个新的root设备并且启动一个新的init程序)。
4. 切换到另一个root设备时，initrd执行pivot_root后，卸载ramdisk；initramfs是rootfs，既不能pivot_root，也不能卸载。
initramfs会删掉rootfs的所有内容(find -xdev / -exec rm '{}' ';')，再次安装root到rootfs(cd /newmount; mount --move . /; chroot .)，把stdin/sdout/stderr挂在新的/dev/console上，重新执行init。由于这是一个相当困难的实现过程(包括在使用一个命令之前把它删除)，所以klibc工具包引入一个帮助程序/utils/run_init.c来执行上述过程。其他大部分工具包(包括busybox)把这个命令称为"switch_root"。
-------------------------------------------------------------------------------

**initial RAM disk**

**Linux****初始****RAM**[**磁盘**](http://baike.baidu.com/view/157418.htm)**（initrd）是在系统引导过程中挂载的一个临时根****文件系统**，用来支持两阶段的引导过程。initrd文件中包含了各种可执行程序和驱动程序，它们可以用来挂载实际的根文件系统，然后再将这个 initrd RAM 磁盘卸载，并释放内存。在很多嵌入式Linux 系统中，initrd 就是最终的根文件系统。

-------------------------------------------------------------------------------
Populating initramfs
2.6的内核缺省情况下总是生成一个gzipped的cpio文档，并且和内核链接在一起。这个文档缺省是空的，在X86环境下的大小是134字节。
CONFIG_INITRAMFS_SOURCE配制参数指定initramfs文档的源，并且自动的嵌入到二进制文件中。这个参数可以指向一个gzipped的cpio文档，一个包含文件的目录，或者文件描述的文本文件。例如文本文件：
   dir /dev 755 0 0
   nod /dev/console 644 0 0 c 5 1
   nod /dev/loop0 644 0 0 b 7 0
   dir /bin 755 1000 1000
   slink /bin/sh busybox 777 0 0
   file /bin/busybox initramfs/busybox 755 0 0
   dir /proc 755 0 0
   dir /sys 755 0 0
   dir /mnt 755 0 0
   file /init initramfs/init.sh 755 0 0
在内核编译完成后，可以执行/usr/gen_init_cpio命令获得cpio文档。配置文件的一个优势是不需要root权限，也不需要在新的文档中生成设备节点。在上述文档中的两个file命令用来发现initramfs目录下面的init.sh文件和busybox文件。内核不需要外部的cpio工具实现initramfs的cpio文档。如果在配置时指定了一个目录而不是一个描述文件，内核编译时将从这个目录生成一个描述文件(作为/usr/gen_init_cpio.c的输入)。内核编译时cpio生成代码和内核浑然一体，boot时解压缩程序也和内核浑然一体。
如果不使用配置文件或者配置目录，而使用定制的cpio文档时，需要外部的cpio工具。例如下面的命令可以从cpio映像文件抽取包含的文件、压缩cpio映像文件：
    cpio -i -d -H newc -F initramfs_data.cpio --no-absolute-filenames   
下面的shell脚本可以生成一个定制的cpio.gz文档，可以用来代替配置文件生成的cpio文档：
   #!/bin/sh
   if [ $# -ne 2 ]
   then
     echo "usage: mkinitramfs directory imagename.cpio.gz"
     exit 1
   fi
   if [ -d "$1" ]
   then
     echo "creating $2 from $1"
     (cd "$1"; find . | cpio -o -H newc | gzip) > "$2"
   else
     echo "First argument must be a directory"
     exit 1
   fi









