# 内核kernel以及根文件系统rootfs是如何映射到对应的nand flash的 - maopig的专栏 - CSDN博客
2012年03月08日 23:37:05[maopig](https://me.csdn.net/maopig)阅读数：1623标签：[flash																[struct																[脚本																[工具																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)
个人分类：[u-boot/驱动开发/linux																[网络](https://blog.csdn.net/maopig/article/category/869532)](https://blog.csdn.net/maopig/article/category/930963)
初始化代码读取uboot到内存里面，然后跳转到uboot那里去执行uboot，uboot初始化必要的硬件，加载一些[驱动](http://write.blog.csdn.net/a/list_34_1.html)，其中包括nand
 flash的[驱动](http://write.blog.csdn.net/a/list_34_1.html)，然后根据uboot里面设置的一个启动命令：
nand read 0x30007FC0 0x100000 0x200000;bootm 0x30007FC0
意思就是，先去读取nand flash，从0x100000开始，长度为0x200000的数据到memory的0x30007FC0处，然后bootm表示从memory的0x30007FC0开始运行。
也就是去运行内核镜像了。
然后，内核会自己解压缩，然后执行,
初始化硬件，
加载驱动模块，
最后去挂载rootfs，
而此文件系统是什么格式的，是从uboot里面定义的：
#define CONFIG_BOOTARGS   "root=/dev/mtdblock2 rw init=/linuxrc console=ttyS0,115200 mem=16M rootfstype=yafffs2"
并在从uboot掉转到内核运行时候，传递给内核的，这样内核在加载rootfs的时候，才知道，要以什么格式，比如上面的yaffs2格式，去加载此文件系统。
此文件系统，也是你实现自己用相应的文件系统制作工具，制作的，然后烧写到对应的位置的。
上面中root=/dev/mtdblock2，表示，要去/dev/mtdblock2,也就是mtd的第3个分区，去加载。
而这里的mtd的第3个分区具体对应的nand flash中的的地址，是你在内核中，一般是在core.c自己定义的的nand flash的分区。
一般是uboot是第一个分区，内核kernel是第二个，然后就是rootfs是第三个分区，也就是/dev/mtdblock2。
例如分区：
static struct mtd_partition rm9200_partitions[3] =
{
        {        /* uboot 256K */
                .name = "uboot",
                .size = 0x40000,
                .offset = 0
        },
        {        /* kernel 1.768M */
                .name = "kernel",
                .size = 0x1C0000,
                .offset = 0x40000
        },
        {        /* rootfs 2M */
                .name = "rootfs",
                .size = 0x200000,
                .offset = 0x200000
        },
};
如果按照上面分区，定义的/dev/mtdblock2的起始地址是0x200000，还算成大小是2M的位置，然后，内核启动挂载rootfs的时候，就是，以上面从uboot传过来的参数中yaffs2格式，到nand
 flash 的2M的地址读取并加载rootfs，加载完成后，这样，里面对应的根目录，以及所有的文件，文件夹就都可以识别了。
然后才会去读取并运行初始化脚本相关的东西，最后初始化console控制台，然后才能看到那个常见的＃，才可以输入命令，比如ls，才可以和系统交互。
所以，实际就是这个rootfs，对应着mtdblock2，对应的某个nand flash上的地址，比如此处的2M的地方。而不是内核kernel这个文件映射的。
