
# open(-dev-ietctl, O_RDWR) 参数含义 - 嵌入式Linux - CSDN博客

2015年04月28日 10:49:37[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1310


这是文件I/O的常用函数，open函数，open函数用来打开一个设备，他返回的是一个整型变量，如果这个值等于-1，说明打开文件出现错误，如果为大于0的值，那么这个值代表的就是文件描述符。一般的写法是if((fd=open("/dev/ttys0",O_RDWR | O_NOCTTY | O_NDELAY)<0){
perror("open");
}
这个事常用的一种用法fd是设备描述符，linux在操作硬件设备时，屏蔽了硬件的基本细节，只把硬件当做文件来进行操作，而所有的操作都是以open函数来开始，它用来获取fd，然后后期的其他操作全部控制fd来完成对硬件设备的实际操作。你要打开的/dev/ttyS0，代表的是串口1，也就是常说的com1，后面跟的是一些控制字。int open(const char *pathname, int oflag, …/*, mode_t mode * / ) ;这个就是open函数的公式。控制字可以有多种，我现在给你列出来：
O_RDONLY 只读打开。
O_WRONLY 只写打开。
O_RDWR 读、写打开。
O_APPEND 每次写时都加到文件的尾端。
O_CREAT 若此文件不存在则创建它。使用此选择项时，需同时说明第三个参数mode，用其说明该新文件的存取许可权位。
O_EXCL 如果同时指定了O_CREAT，而文件已经存在，则出错。这可测试一个文件是否存在，如果不存在则创建此文件成为一个原子操作。
O_TRUNC 如果此文件存在，而且为只读或只写成功打开，则将其长度截短为0。
O_NOCTTY 如果p a t h n a m e指的是终端设备，则不将此设备分配作为此进程的控制终端。
O_NONBLOCK 如果p a t h n a m e指的是一个F I F O、一个块特殊文件或一个字符特殊文件，则此选择项为此文件的本次打开操作和后续的I / O操作设置非阻塞方式。
O_SYNC 使每次w r i t e都等到物理I / O操作完成。
这些控制字都是通过“或”符号分开（|）
当调用系统调用open时，操作系统会将文件系统对应设备文件的inode中的file_operations安装进用户进程的task_struct中的file_struct，然后再调用具体文件的file_operations中的open函数，其他的read、write等等也是如此，所以实际上open操作实际上是一个连接过程，我个人认为称其为open容易误导。至于file_operations中的针对底层硬件的操作，简单地说实际上就是对具体硬件接口的控制/状态寄存器和数据寄存器的读写，如硬盘这种硬件有其自己的控制器，所以他自己内部会定期处理寄存器的命令和数据，完成相应的操作请求，请求完成后cpu发中断

