# Linux系统中/dev/mtd与/dev/mtdblock的区别 - maopig的专栏 - CSDN博客
2012年12月17日 23:36:03[maopig](https://me.csdn.net/maopig)阅读数：1327
个人分类：[u-boot/驱动开发/linux																[Linux](https://blog.csdn.net/maopig/article/category/850267)](https://blog.csdn.net/maopig/article/category/930963)

MTD(memory technology device内存技术设备)是用于访问memory设备（ROM、flash）的Linux的子系统。MTD的主要目的是为了使新的memory设备的驱动更加简单，为此它在硬件和上层之间提供了一个抽象的接口。MTD的所有源代码在/drivers/mtd子目录下。我将CFI接口的MTD设备分为四层（从设备节点直到底层硬件驱动），这四层从上到下依次是：设备节点、MTD设备层、MTD原始设备层和硬件驱动层。
**MTD字符驱动程序允许直接访问flash器件**，**通常用来在flash上创建文件系统**，也可以用来直接访问不频繁修改的数据。
**MTD块设备驱动程序可以让flash器件伪装成块设备，实际上它通过把整块的erase block放到ram里面进行访问，然后再更新到flash，用户可以在这个块设备上创建通常的文件系统。**
1. /dev/mtdN 是Linux 中的MTD架构中，系统自己实现的mtd分区所对应的**字符设备(将mtd设备分成多个区，每个区就为一个字符设备)**，其里面添加了一些ioctl，支持很多命令，如MEMGETINFO，MEMERASE等。
而mtd-util中的flash_eraseall等工具，就是**以这些ioctl为基础而实现的工具**，**实现一些关于Flash的操作**。比如，mtd 工具中的 flash_eraseall中的：
if (ioctl(fd, **MEMGETINFO**, &meminfo) != 0) {
   fprintf(stderr, "%s: %s: unable to get MTD device info\n", exe_name, mtd_device);
   return 1;
}
其中，MEMGETINFO，就是Linux MTD中的drivers/mtd/mtdchar.c中的：
static int mtd_ioctl(struct inode *inode, struct file *file,
       u_int cmd, u_long arg)
{
。。。。。
case **MEMGETINFO**:
   info.type = mtd->type;
   info.flags = mtd->flags;
   info.size = mtd->size;
   info.erasesize = mtd->erasesize;
   info.writesize = mtd->writesize;
   info.oobsize = mtd->oobsize;
   /* The below fields are obsolete */
   info.ecctype = -1;
   info.eccsize = 0;
   if (copy_to_user(argp, &info, sizeof(struct mtd_info_user)))
    return -EFAULT;
   break;
。。。
}
而/dev/mtdblockN，是**Nand Flash驱动**中，驱动用add_mtd_partitions()添加MTD设备分区（其实就是将mtd设备进行不同的分区，当mtd设备还是一样的，所以mtdblock分区与mtd分区肯定是对应的），而生成的对应的**块设备**。
根据以上内容，也就更加明白，为什么不能用nandwrite,flash_eraseall,flash_erase等工具去对/dev/mtdblockN去操作了。因为**/dev/mtdblock中不包含对应的ioctl**，也就没有定义对应的命令，不支持你这么操作。
2. mtd char 设备的主设备号是90，而mtd block设备的主设备号是31：
# ls /dev/mtd* -l 
crw-r-----    1 root     root      90,   0 May 30 2007 /dev/mtd0
crw-r-----    1 root     root      90,   2 May 30 2007 /dev/mtd1
crw-r-----    1 root     root      90,   4 Jul 17 2009 /dev/mtd2
crw-r-----    1 root     root      90,   6 May 30 2007 /dev/mtd3
crwxrwxrwx    1 root     root      90,   8 May 30 2007 /dev/mtd4
crwxrwxrwx    1 root     root      90, 10 May 30 2007 /dev/mtd5
crwxrwxrwx    1 root     root      90, 12 May 30 2007 /dev/mtd6
crwxrwxrwx    1 root     root      90, 14 May 30 2007 /dev/mtd7
crwxrwxrwx    1 root     root      90, 16 May 30 2007 /dev/mtd8
crwxrwxrwx    1 root     root      90, 18 May 30 2007 /dev/mtd9
# ls /dev/mtdblock* -l
brw-r-----    1 root     root      31,   0 May 30 2007 /dev/mtdblock0
brw-r-----    1 root     root      31,   1 May 30 2007 /dev/mtdblock1
brw-r-----    1 root     root      31,   2 May 30 2007 /dev/mtdblock2
brw-r-----    1 root     root      31,   3 May 30 2007 /dev/mtdblock3
brwxrwxrwx    1 root     root      31,   4 May 30 2007 /dev/mtdblock4
brwxrwxrwx    1 root     root      31,   5 May 30 2007 /dev/mtdblock5
brwxrwxrwx    1 root     root      31,   6 May 30 2007 /dev/mtdblock6
brwxrwxrwx    1 root     root      31,   7 May 30 2007 /dev/mtdblock7
brwxrwxrwx    1 root     root      31,   8 May 30 2007 /dev/mtdblock8
brwxrwxrwx    1 root     root      31,   9 May 30 2007 /dev/mtdblock9
此设备号，定义在/include/linux/mtd/mtd.h中 ：
#define MTD_CHAR_MAJOR   90
#define MTD_BLOCK_MAJOR 31
3. 其中，mtd的块设备的大小，可以通过查看分区信息获得：
# cat /proc/partitions
major minor #blocks name
31     0       1024 mtdblock0
31     1       8192 mtdblock1
31     2     204800 mtdblock2
31     3      65536 mtdblock3
31     4     225280 mtdblock4
上面中显示的块设备大小，是block的数目，每个block是1KB。
**而每个字符设备，其实就是对应着上面的每个块设备。即/dev/mtd0对应/dev/mtdblock0，其他以此类推。换句话说，mtdblockN的一些属性，也就是mtdN的属性，比如大小。**
4。对每个mtd字符设备的操作，比如利用nandwrite去对/dev/mtd0写数据，实际就是操作/dev/mtdblock0。
而这些操作里面涉及到的偏移量offset，都指的是此mtd 分区内的偏移。比如向/dev/mtd1的offset为0的位置写入数据，实际操作的是物理偏移offset=/dev/mtd0的大小=1MB=0x100000。
5.mtd的字符设备和块设备的命名规则，可以参考下表：
##### Table 7-1. MTD /dev entries, corresponding MTD user modules, and relevant device major numbers
/dev entry
Accessible MTD user module
Device type
Major number
*mtd**N*
char device
char
90
*mtdr**N*
char device
char
90
*mtdblock**N*
block device, read-only block device, JFFS, and JFFS2
block
31
*nftl**LN*
NFTL
block
93
*ftl**LN*
FTL
block
44
##### Table 7-2. MTD /dev entries, minor numbers, and naming schemes
/dev entry
Minor number range
Naming scheme
*mtd**N*
0 to 32 per increments of 2
*N* = minor / 2
*mtdr**N*
1 to 33 per increments of 2
*N* = (minor - 1) / 2
*mtdblock**N*
0 to 16 per increments of 1
*N* = minor
*nftl**LN*
0 to 255 per sets of 16
*L* = set;[[2]](http://book.opensourceproject.org.cn/embedded/oreillybuildembed/opensource/belinuxsys-chp-7-sect-1.html#belinuxsys-chp-7-fnote-2)*N* = minor - (set - 1) x 16;
*N* is not appended to entry name if its value is zero.
*ftl**LN*
0 to 255 per sets of 16
*Same as NFTL.*
The Linux MTD,YAFFS Howto上面这样写道：
Erase the mtdblock0
/>eraseall /dev/mtd0
Create the mount directory and mount
/>mkdir -p /mnt/flash0
/>mount -t yaffs /dev/mtdblock0 /mnt/flash0
为什么eraseall对mtd0操作？而不对mtdblock0操作？nand不是块设备嘛，mtdblock就是块设备呀。mtd0,mtd1与mtdblock0,mtdblock1是不是一一对应的？
