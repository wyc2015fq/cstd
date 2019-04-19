# kernel mtd 分区与UBOOT 分区的理解 - maopig的专栏 - CSDN博客
2013年12月12日 16:53:23[maopig](https://me.csdn.net/maopig)阅读数：3550
今天做内核移植，准备添加NAND flash的驱动，做到MTD分区时，想起在一本书上看到的一句话，说的是分区时每个区之间没有间隙，前一个区的结束地址是后一个区的起始地址。可是当我看我的开发板的教程时，分区如下：
static struct mtd_partition smdk_default_nand_part[] = {
      [0] = {
           .name     = "uboot",
           .offset = 0x00000000,
           .size     = 0x00040000,
      },
      [1] = {
           .name     = "kernel",
           .offset = 0x00200000,
           .size     = 0x00300000,
      },
      [2] = {
           .name     = "yaffs2",
           .offset = 0x00500000,
           .size     = MTDPART_SIZ_FULL
      }
};
很显然，在uboot和kernel分区中存在间隙，心中产生了疑问，难道MTD分区的时候要有注意的问题？通过各方查找资料和查阅书籍，找到了原因。不对的地方还请大家指正。
        首先说一下Linux下固态存储设备(NAND flash算其中一种)对系统组件的安排方式，一般为
|引导加载程序|引导参数|内核|根文件系统|
也就是说，在NAND flash中，各部分的程序是这样安排的，但哪一端是高地址是依体系结构不同而不同的，对于ARM，引导加载程序在最低地址处，因此，无论是uboot的分区还是内核MTD分区，引导加载程序的分区都放在了最低地址处。那么，两个分区到底怎么联系起来，而我们又该怎么设置MTD分区呢？先给出我的开发板uboot的分区信息：
**bootargs=noinitrd root=/dev/mtdblock2  init=/linuxrc console=ttySAC0**
**mtdparts=mtdparts=nandflash0:256k@0(bios),128k(params),128k(toc),512k(eboot),1024k(logo),3m(kernel),-(root)**
       然后说一下MTD分区，这个分区是内核可以识别的分区，也就是说，内核的操作都是基于MTD分区的；而uboot的分区只是为了方便操作，例如，我想将内存中0x30000000地址处的内容写到NAND flash的偏移量为2M的地址处，即uboot分区中kernel的起始位置，一般情况，我们要写
nand write 0x30000000 0x00200000
但如果有了uboot的分区，我们可以写
nand write 0x30000000 kernel
        说清上面的问题，为了进一步阐述后面的问题，这里再讲一下我对uboot引导过程的理解，当系统启动后，uboot开始执行，他分两个阶段完成工作，主要是一些初始化，然后，加载内核并传递内核参数，之后跳入内核执行，内核完成它的初始化工作，其中包括挂载文件系统。
       现在，我们可以翻回头看上面程序中的MTD分区了。MTD分区中的uboot分区明显对应了uboot分区中的bios分区（从0开始，大小为128K），而MTD分区中的kernel和yaffs2分区的起始地址和大小也分别对应uboot分区中的kernel和root分区。而因为我们不需要uboot分区中的其他部分，所以在MTD分区中出现了这一部分空隙。但为什么这么安排呢？
       回想我们在烧写程序时候的操作，比如我们选择烧写内核镜像，此时，uboot实际执行了一条语句，类似于
**#define kernel 0x00200000**
**memcpy(kernel,0x30000000,SZ_3M)**
       这里我们烧写程序的入口地址是0x30000000，也就是说，uboot的下载模式将我们烧写到内存0x30000000处的数据，搬到了NAND flash的kernel处，保存了起来，**因此，这里要清楚，我们烧写程序时，实际是将程序先烧写到了内存当中，然后由内存搬运到NAND flash中，**如果此时我们的MTD分区与uboot中的分区是一致的，那么内核将来运行时可以很方便的找到内核程序所在的位置，同样，对文件系统的yaffs2分区也是如此，而且，与内核分区相比，文件系统的分区将显得更加重要，因为将直接影响到根文件系统能否挂载，这里是因为上面提到的一行
**bootargs=noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0**
       这里，uboot指定了根文件系统的代码来自于mtdblock2，也就是MTD分区的第三个分区(第一个编号为0)，也就是我上面说到的，uboot完成初始化后，加载内核，而内核要完成文件系统的挂载，他从哪里找文件系统？就是这里！/dev/mtdblock2!
       所以，现在我们看到，MTD分区的原因，而且最关键的在这里，其他分区如果与uboot的分区不一致还情有可原，但如果MTD分区中文件系统的分区与uboot中的root分区不一致，将会直接导致系统无法启动！
**当然，之前操作的都是物理地址，当内核真正运行起来以后，将开始使用虚拟地址。**
同样的，其他几个引导参数也应该得到满足，系统才可能正常运行起来
init=/linuxrc   init进程的位置。
console=ttySAC0  终端对应tty设备，因此，在引导系统前，串口驱动移植应当完成
