# 嵌入式 Linux应用程序如何读取(修改)芯片寄存器的值 - DoubleLi - 博客园






这一问题来自项目中一个实际的需求：
我需要在Linux启动之后，确认我指定的芯片寄存器是否与我在uboot的配置一致。

举个例子：
寄存器地址：0x20000010负责对DDR2的时序配置，该寄存器是在uboot中设置，现在我想在Linux运行后，读出改寄存器的值，再来检查该寄存器是否与uboot的配置一致。

Linux应用程序运行的是虚拟空间，有没有什么机制可以是完成我提到的这一需求。若行，还请附些测试代码。
谢谢！

这个需要用mmap()函数将寄存器物理地址映射为用户空间的虚拟地址，即将寄存器的那段内存映射到用户空间，函数介绍如下：

- 
- 
- **void*** mmap(**void** * addr, size_t len, **int** prot, **int** flags, **int** fd, off_t offset);




该函数映射文件描述符 fd 指定文件的 [offset, offset + len] 物理内存区至调用进程的 [addr, addr + len] 的用户空间虚拟内存区，通常用于内存共享或者用户空间程序控制硬件设备，函数的返回值为最后文件映射到用户空间的地址，进程可直接操作该地址。下面是测试代码（仅供参考）：

- #define DDR2_REG_BASE (0x20000000)
- #define MAP_SIZE 4096UL

- #define MAP_MASK (MAP_SIZE - 1)
- 
- **static****unsigned****int** pTestRegBase;
- **static****int** dev_fd;
- 
- dev_fd = open("/dev/mem", O_RDWR | O_NDELAY);
- 
- **if** (dev_fd <</SPAN> 0) {
- LOGE("open(/dev/mem) failed.");
- **return**;
- }
- 
- pTestRegBase = (**void** *)mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd,DDR2_REG_BASE & ~MAP_MASK);
- 
- **if** (MAP_FAILED == pTestRegBase) {
- printf("mmap failed. fd(%d), addr(0x%x), size(%d)\n", dev_fd, DDR2_REG_BASE, MAP_SIZE);
- } **else** {
- **unsigned****int** reg_value = *((**volatile****unsigned****int** *)(pTestRegBase + 10));
- printf("reg_value = 0xx\n", reg_value);
- munmap((**void***)pTestRegBase, MAP_SIZE);
- }
- 
- pTestRegBase = 0;
- 
- **if**(dev_fd)
- close(dev_fd);




这里将DDR2_REG_BASE开始大小为1个page的物理地址映射到了用户空间，然后就可以用pTestRegBase作为起始地址操作寄存器了。


- 
0支持

赞一个！可以看到我的开发板指定位置寄存器的值了。
追问一下：
我在我ubuntu主机(intel x86)上使用运行你给代码，会出现错误：
ajaxhe@bbs:~/program/c$ sudo ./a.out
mmap failed. fd(3), addr(0x20000000), size(4096)
但将15行的程序修改为：
pTestRegBase = (void *)mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, 0);
运行时正常，这里问题出再哪里呢？是不是因为0x20000000~0x20000000+4096在ubuntu的机器上没有地址映射的缘故呢？

- 
0支持

@ajaxhe x86架构寄存器地址我不怎么清楚，像arm架构的处理器寄存器一般都是在高端地址如0xF0000000以上，你可以在程序中把errno值打印出来看看是什么原因引起的。


1、首先写一个内核模块，在该内核模块中设置一个变量，用来存储该寄存器的值。
2、在内核模块中创建一个对应的proc，用来向用户空间输出该寄存器的值。
3、在用户空间直接cat之前创建的proc文件。

需要以内核模块的方式运行

晕,不说详细点,就扣分阿....

写个内核模块
使用ioremap()
映射到user地址空间

看看这里的咚咚

asm/io.h

写个驱动，在驱动里面去读你的寄存器就可以了。
这里有个实例，你可以下载试试：read_register_kmod.tar（下载地址：http://sdrv.ms/ZM8gFi）。









