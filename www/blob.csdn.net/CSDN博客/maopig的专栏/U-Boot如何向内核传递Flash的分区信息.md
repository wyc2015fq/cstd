# U-Boot如何向内核传递Flash的分区信息 - maopig的专栏 - CSDN博客
2012年12月16日 23:34:34[maopig](https://me.csdn.net/maopig)阅读数：2719
1.如何对nand 分区。修改mtdparts环境变量就可以了么？
对于目前的U-boot而言,是的.而且, 设置了mtdparts变量之后,你可以在nand read/write/erase命令中直接使用分区的名字而不必指定分区的偏移位置.
set bootargs noinitrd console=ttySAC0 root=/dev/mtdblock3 rootfstype=jffs2  mtdparts=nand_flash:128k(u-boot)ro,64k(u-boot envs),3m(kernel),30m(root.jffs2),30m(root.yaffs)
2 内核通过bootargs找到文件系统，bootargs中的mtdblockx即代表分区，block1，2，3代表哪个分区是如何确定的。
事实上,bootargs中的"root=/dev/mtdblockx"只是告诉内核,root fs从第x个(x=0,1,2...)MTD分区挂载,mtdblock0对应第一个分区,mtdblock1对应第二个分区,以此类推.至于这个分区对应MTD device(NAND Flash)的哪一段范围,取决于内核读到的MTD分区信息,这个分区信息可以通过:
1) 写死在MTD层的NAND Controller驱动或者内核其他部分代码里
2) 通过U-boot传递给内核的命令行中的mtdparts=...部分解析得出,解析的规则同u-boot中mtdparts变量的赋值规则
3) 其他可以让内核知道分区信息的任何办法
3 在u－boot中给nand分区后是否要对应修改kernel的代码？
如果你用的是通过内核命令行给MTD层传递u-boot中的MTD分区信息,那是不需要的,在这种情况下,内核读取到的分区信息始终和u-boot中的保持一致(推荐的做法)
如果你用的是把分区信息写死在内核源代码里的方法,那最好保证它和u-boot中的保持一致,即同步修改内核的相关部分代码
