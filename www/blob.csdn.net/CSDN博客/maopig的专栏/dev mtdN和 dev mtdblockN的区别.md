# /dev/mtdN和/dev/mtdblockN的区别 - maopig的专栏 - CSDN博客
2013年12月25日 13:47:53[maopig](https://me.csdn.net/maopig)阅读数：1464
个人分类：[u-boot/驱动开发](https://blog.csdn.net/maopig/article/category/1090389)
1、/dev/mtdn是linux中的MTD架构中，系统自己实现的mtd分区所对应的字符设备，其里面添加了一些ioctl，支持很多命令，如MEMGETINFO，MEMERASE等。
     而mtd-util中的flash_eraseall等工具，就是以这些ioctl为基础而实现的工具，实现一些关于flash的操作，比如，mtd工具中的flash_eraseall中的：
 if(ioctl(fd,MEMGETINFO,&meminfo) != 0)
 {
  perror("MEMGETINFO");
  close(fd);
  exit(1);
 }
其中，MEMGETINFO，就是linux mtd中的/drivers/mtd/nand/mtdchar.c中的：
 case MEMGETINFO:
  info.type = mtd->type;
  info.flags = mtd->flags;
  info.size = mtd->size;
  info.erasesize = mtd->erasesize;
  info.writesize = mtd->writesize;
  info.oobsize = mtd->oobsize;
  /* The below fields are obsolete */
  info.ecctype = -1;
  info.eccsize = 0;
  if (copy_to_user(argp, &info, sizeof(struct mtd_info_user)))
   return -EFAULT;
  break;
而/dev/mtdblockn，是NandFlash驱动中，驱动在用add_mtd_partitions()添加MTD设备分区而生成的对应的块设备。根据以上内容也更加明白，为什么不能用nandwrite，flash_eraseall，flash_erase等工具去对/dev/mtdblockn去操作了，因为/dev/mtdblock中不包含对应的ioctl，不支持你这么操作。
2、mtd char设备的主设备号是90；而mtdblock设备的主设备号是31；
     此设备号定义在/include/linux/mtd/mtd.h中
3、mtd块设备的大小可以通过查看分区信息来得到：
# cat /proc/partitions
major minor  #blocks  name
  31        0        128 mtdblock0
  31        1        128 mtdblock1
  31        2        512 mtdblock2
  31        3       2048 mtdblock3
  31        4      32768 mtdblock4
  31        5     486400 mtdblock5
上面显示的块设备的大小，是block的数目，每个block是1KB；
而每个字符设备，其实就是对应着上面的饿每个设备，即/dev/mtd0对应/dev/mtdblock0，以此类推，换句话说，mtdblockn的一些属性，也就是mtdn的属性，比如大小。
4、对每个mtd字符设备的操作，比如利用nandwrite去对/dev/mtd0写数据，实际就是操作/dev/mtdblock0。而这些操作里面涉及到得offset都指的是mtd分区内的偏移。比如向/dev/mtd1的offset为0的位置写入数据，实际操作的物理偏移是offset=/dev/mtd0的大小=128KB。
