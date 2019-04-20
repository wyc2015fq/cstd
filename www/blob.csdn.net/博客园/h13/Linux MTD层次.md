# Linux MTD层次 - h13 - 博客园
MTD(memory technology device内存技术设备)是用于访问memory设备（ROM、flash）的Linux的子系统。MTD的主要目的是为了使新的memory设备的驱 动更加简单，为此它在硬件和上层之间提供了一个抽象的接口。MTD的所有源代码在/drivers/mtd子目录下。我将CFI接口的MTD设备分为四层 （从设备节点直到底层硬件驱动），这四层从上到下依次是：设备节点、MTD设备层、MTD原始设备层和硬件驱动层。 
　　一、Flash硬件驱动层：硬件驱动层负责在init时驱动Flash硬件，Linux 
MTD设备的NOR　Flash芯片驱动遵循CFI接口标准，其驱动程序位于drivers/mtd/chips子目录下。NAND型Flash的驱动程
序则位于/drivers/mtd/nand子目录下。
　　二、MTD原始设备：原始设备层有两部分组成，一部分是MTD原始设备的通用代码，另一部分是各个特定的Flash的数据，例如分区。 
用于描述MTD原始设备的数据结构是mtd_info，这其中定义了大量的关于MTD的数据和操作函数。mtd_table（mtdcore.c）则是所
有MTD原始设备的列表，mtd_part（mtd_part.c）是用于表示MTD原始设备分区的结构，其中包含了mtd_info，因为每一个分区都
是被看成一个MTD原始设备加在mtd_table中的，mtd_part.mtd_info中的大部分数据都从该分区的主分区
mtd_part->master中获得。 
在drivers/mtd/maps/子目录下存放的是特定的flash的数据，每一个文件都描述了一块板子上的flash。其中调用
add_mtd_device()、del_mtd_device()建立/删除mtd_info结构并将其加入/删除mtd_table（或者调用
add_mtd_partition()、del_mtd_partition()（mtdpart.c）建立/删除mtd_part结构并将
mtd_part.mtd_info加入/删除mtd_table 中）。 
　　三、MTD设备层：基于MTD原始设备，linux系统可以定义出MTD的块设备（主设备号31）和字符设备（设备号90）。MTD字符设备的定义在
mtdchar.c中实现，通过注册一系列file 
operation函数（lseek、open、close、read、write）。MTD块设备则是定义了一个描述MTD块设备的结构
mtdblk_dev，并声明了一个名为mtdblks的指针数组，这数组中的每一个mtdblk_dev和mtd_table中的每一个
mtd_info一一对应。 
　　四、设备节点：通过mknod在/dev子目录下建立MTD字符设备节点（主设备号为90）和MTD块设备节点（主设备号为31），通过访问此设备节点即可访问MTD字符设备和块设备。 
　　五、根文件系统：在Bootloader中将JFFS（或JFFS2）的文件系统映像jffs.image（或jffs2.img）烧到flash的
某一个分区中，在/arch/arm/mach-your/arch.c文件的your_fixup函数中将该分区作为根文件系统挂载。 
　　六、文件系统：内核启动后，通过mount 命令可以将flash中的其余分区作为文件系统挂载到mountpoint上。
