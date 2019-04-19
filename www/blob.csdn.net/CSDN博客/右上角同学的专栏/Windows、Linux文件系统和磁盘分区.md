# Windows、Linux文件系统和磁盘分区 - 右上角同学的专栏 - CSDN博客
2016年06月27日 20:51:25[右上角徐](https://me.csdn.net/u011032983)阅读数：2138
**磁盘分区过程**
Windows系统磁盘分区：
Windows系统可以再图形化界面看到分区，如图所示，本电脑有两块磁盘：磁盘0、磁盘1。磁盘0有2个主分区，对应盘符或路径为C：、D：；磁盘1有2个主分区，对应盘符或路径E：、F：，文件系统类型都为NFTS，其中有2G未分配分区。
![](https://img-blog.csdn.net/20160627205322117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对未分配分区的磁盘分区时，设置分区的文件系统类型（相当于Linux里的分区格式化过程）、大小（Linux磁盘分区时设置），盘符或路径名（相当于Linux里的挂载过程），然后系统会自动执行格式化，该盘即可使用。
linux系统磁盘分区：
linux系统命令查看磁盘分区情况：fdisk –l命令，磁盘hdb未分配分区，磁盘hda有2个分区，分区hda1和分区hda2，需要将磁盘分区挂载到不同文件路径下，类似于Windows中分区对应C：、D：…
不同的是Linux系统下，不论几个分区挂载到哪一个目录，归根到底只有一个根目录，一个独立且唯一的文件结构，而Windows不同分区对应不同根目录。
![](https://img-blog.csdn.net/20160627205403290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       对未分配分区的磁盘分区时，执行fdisk  /dev/hdb进行分区，设置分区大小、第几个分区、主分区或者扩展分区;格式化分区，执行mkfs –t ext3 -c /dev/sdb1；挂载分区，执行mount /dev/sdb1/mnt
       另外不管Windows还是Linux，分配逻辑分区时，必须首先分配为扩展分区。
文件系统类型
文件系统类型表示操作系统读取磁盘上存储的文件数据的方式，常见的有FAT系列、NTFS、ext系列。
FAT系列用于Windows系统，采用链表方式存储文件的块号，从FAT12到FAT16再到FAT32的升级是为了满足磁盘容量增加这一状况，原因是磁盘容量增加必然会引起簇的大小和簇内碎片的增加，所以需要增加FAT表项数量即增大表项位数（但增加的表项会减缓运行速度），所以出现从12位到16位再到32位的变迁。
Ntfs更为强大，支持64位的磁盘块号，ext系列用于Linux系统，使用extents方式来标识块的位置：就是structext4_**inode**的这个成员：__le32i_block[EXT4_N_BLOCKS];/* Pointers to blocks */
另外，FAT易产生内部碎片，Ntfs不那么容易产生碎片，ext也是会产生碎片的，但数量极少，基本不需要额外的磁盘整理软件，网上有一说法是，ext不容易产生碎片是由于Linux比较强大的延迟写入技术导致的，在写入之前，ext有足够多的机会重新分配块的位置，保证碎片更少。但不意味着就能完全避免碎片。相比之下，由于Windows的延迟写入不如Linux强大，所以相对而言，更容易产生碎片，但NTFS远远比FAT产生的碎片要少，并且影响也小。
