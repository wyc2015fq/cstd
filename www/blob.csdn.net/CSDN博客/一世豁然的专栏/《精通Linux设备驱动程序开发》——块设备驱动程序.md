# 《精通Linux设备驱动程序开发》——块设备驱动程序 - 一世豁然的专栏 - CSDN博客





2015年04月06日 15:20:18[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1150








一、存储技术

  1、IDE（集成驱动电子设备）是PC中常见的存储接口技术，ATA（高级技术配件）则是相关规范的官方名称。




  2、IDE/ATA标准：

   1）、ATA-1。

   2）、LBA(逻辑块设备寻址)的ATA-2。

   3）、支持SMART功能的ATA-3。

   4）、支持UItraDMA的具有33MB/s吞吐量的ATA-4。

   5）、最大传输速率达66MB/s的ATA-5。

   6）、最大传输速率达100MB/s的ATA-7。

   7）、ATA-7。




  3、CD-ROM和磁带等存储设备使用一种称为ATAPI（ATA包接口）的特殊协议与标准IDE电缆相连接，ATAPI是在ATA-4中引入的。




  4、PC系统中的存储媒介

![](https://img-blog.csdn.net/20150406141020501?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  5、IDE/ATA是一种并行总线技术（有时候称为并行ATA或PATA），它不能扩展至高速传输。SATA（串行ATA）是由PATA发展而来的现代串行总线技术，它支持300MB/s甚至更

        高的传输速率。SATA还支持热交换功能。




  6、libATA是Linux内核中的一种新型ATA子系统。它由一套ATA库例程和使用这个库例程的底层驱动程序组成。libATA同时支持SATA和PATA。




  7、SCSI（小型计算机系统接口）是服务商和高端工作站选用的一种存储技术，比SATA要快一点。其传统上是一种并行接口标准，但也随着SAS（串行附加SCSI）总线技术的

        发展，它也在转向串行操作。




  8、内核SCSI子系统在结构上分为三层：

   1）、皆知所对应的顶层驱动程序。

   2）、扫描SCSI总线或配置设备的中间层。

   3）、底层主机配置器驱动程序。




  9、RAID（廉价磁盘冗余阵列）是某些SCSI和SATA控制器的一种内建技术，有了它就可以实现冗余和可靠性。




  10、在嵌入式消费电子领域常使用小型存储器，这些存储器的传输速率比至今讨论的技术所提供的要慢得多。SD（安全数码）卡其衍生的外形更小的miniSD卡、microSD卡  


           是相机、手机、播放器等设备广泛采用的存储介质。




  11、存储技术以及相关设备驱动程序

![](https://img-blog.csdn.net/20150406144345947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







二、Linux块 I/O层

![](https://img-blog.csdn.net/20150406144844263?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







三、I/O调度器

  1、块设备会有寻道时间，即磁头从当前位置移动到目标磁盘扇区花费的时间。I/O调度器的主要目标是通过尽量减少寻道时间来增加系统吞吐量。




  2、Linux I/O调度器

![](https://img-blog.csdn.net/20150406145648769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







四、块驱动程序数据结构和方法

  1、内核用include/linux/genhd.h中定义的gendisk结构体表示一个磁盘：

![](https://img-blog.csdn.net/20150406145824974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、与每个块驱动程序相关的I/O请求队列用request_queue结构体，该结构体定义在include/linux/blkdev.h中。这是一个大的结构。




  3、每个在request_queue队列中的请求用request结构体描述，该结构定义在include/linux/blkdev.h中：![](https://img-blog.csdn.net/20150406150312483?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、block_device_operation是与file_oprations结构体对应的块驱动程序结构体。后者用于字符驱动程序，前者包含了下面与块驱动程序相关的入口函数：

   1）、标准的函数。

   2）、特别的函数。

![](https://img-blog.csdn.net/20150406150637876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150406150738261?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  5、如果观察request结构体，会发现它关联了bio。bio结构体是块I/O操作在页级粒度的底层描述。

![](https://img-blog.csdn.net/20150406150916513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  6、大部分情况下使用3类方法构件块驱动程序：

   1）、常见的初始化和退出函数。

   2）、block_device_operation中的部分函数。

   3）、request函数。







五、设备实例：简单存储控制器

  1、嵌入式设备上的存储器

![](https://img-blog.csdn.net/20150406151351604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、存储控制器寄存器

![](https://img-blog.csdn.net/20150406151547368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、初始化：初始化驱动程序（代码清单14-1）




  4、块设备操作：块设备操作（代码清单14-2）




  5、磁盘访问：请求函数（代码清单14-3）







六、高级主题（略）




七、调试（略）







