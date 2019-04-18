# 《精通Linux设备驱动程序开发》——串行设备驱动程序 - 一世豁然的专栏 - CSDN博客





2014年11月10日 20:40:04[一世豁然](https://me.csdn.net/Explorer_day)阅读数：742








一、相关简介

 1、UART（串行异步收发器）常用来实现串行通信。在PC兼容机硬件上，UART是super I/O芯片组的一部分。（图6-1  PC串行端口连接图）




 2、尽管RS-232通信信道是常见的串行硬件，内核的子系统还是用通用化的方式组织在一起，以便服务不同的用户。







二、层次架构

 1、内核开发者用如下的构建模块去构造分层次的串行架构：

   1）、关注UART或其他底层串行硬件特征的底层驱动程序。

   2）、和底层驱动程序接口的tty驱动程序层。tty驱动程序将上层驱动程序和形形色色的硬件进行了隔离。

   3）、加工用于和tty驱动程序交换数据的线路规程。线路规程勾勒串行层的行为，有助于复用底层的代码来支持不同的技术。




 2、串行子系统层间链接关系图（P119  图6-2）




 3、串行子系统与内核源文件的映射关系（P120  图6-3）







三、UART驱动程序

 1、UART驱动程序围绕三个关键的数据结构展开，这三个关键的数据结构哦都定义在include/linux/serial_core.h中。

   1）、特定UART相关的驱动程序结构，struct_uart_driver:

       struct_uart_driver {

           struct module  *ower;

           const  char  *driver_name;

           const  char  *dev_name;

            /*  .  .  .  */

            int  major;

            int  minor;

            /*  .  .  .  */

            struct  tty_driver  *tty_driver;

       };

   2）、struct  uart_port。UART驱动程序拥有的每个端口都存在uart_port结构的一个实例。

       struct  uart_port {

          spinlock_t  lock;

          unsigned  int  iobase;

          unsigned  char  __iomem  *membase;

          unisgend  int  irq;

          unisgned  int  uartclk;

          unsigned  char  fifosize;

          unsigned  char  x_char;

          /*  .  .  .  */

       };

   3）、struct  uart_ops。这个结构是每个UART驱动程序必须支持的物理硬件上可完成的操作的入口函数的超集。

       struct  uart_ops {

           uint  (*tx_empty)(struct  uart_port  *);

           void  (*set_mctrl)(struct  uart_port  *,  unsigned  int  mctrl);

           uint  (*get_mctrl)(struct  uart_port  *);

           void  (*stop_tx)(struct  uart_port  *);

           void  (*start_tx)(struct  uart_port  *);

           /*  .  .  .  */

           void  (*shutdown)(struct  uart_port  *);

           void  (*set_termios)(struct  uart_port  *,  struct  termios  *new,  struct  termios  *old);

           /*  .  .  .  */

           void  (*config_port)(struct  uart_driver  *,  int);




 2、UART驱动程序为了将自身和内核联系起来，必须完成两个重要的步骤。

   1）、通过调用uart_register_driver(struct  uart_driver  *);向串行核心注册。

   2）、调用uart_add_one_port(struct  uart_driver  *,  struct  uart_port  *)注册其支持的每个接口。




 3、设备实例：手机

   1）、Linux手机上的USB_UART端口（P123  图6-5）。

   2）、USB_UART上的寄存器（P123  表6-1）。

   3）、平台由平台设备和平台驱动程序组成。

   4）、代码清单6-1：Linux手机的USB_UART驱动程序。




 4、RS-485







四、TTY驱动程序

 1、tty驱动程序的核心结构和注册函数。有三个结构非常重要。

   1）、定义于include/linux/tty.h中的struct  tty_struct。此结构包含了和打开的tty相关的所欲哦状态信息。其结构成员众多下面列出一些重要的成员：

      struct  tty_driver {

          int  magic;

          struct  tty_driver  *driver;

          struct  tty_ldisc  ldisc;

          /*  .  .  .   */

          struct  tty_filp_buffer  flip;

          /*  .  .  .   */

          wait_queue_head_t  write_wait;

          wait_queue_head_t  read_wait;

          /*  .  .  .   */

       };

   2）、tty_struct中内嵌的struct  tty_flip_buffer或flip缓冲区。这是数据收集和处理机制的中枢：

      struct  tty_flip_buffer {

         /*  .  .  .  */

         struct  semaphore  pty_sem;

         char  *char_buf_ptr;

         /*  .  .  .  */

         unsigned  char  char_buf[2*TTY_FLIPBUF_SIZE];

         /*  .  .  .  */

      };

      在最近的内核中，struct  tty_flip_buffer结构体有些改动，目前由缓冲区头部（tty_buhead）和缓冲区链表（tty_buffer）组成：

      struct_tty_bufhead {

         /*  .  .  .  */

         struct  semaphore  pty_sem;

         struct  tty_buffer  *head,  tail,  free;

          /*  .  .  .  */

       };

       struct  tty_buffer {

          struct  tty_buffer  *next;

          struct  *char_buf_ptr;

          /*  .  .  .  */

          unsigned  long  data[0];

       };

   3）、定义在include/linux/tty_driver.h文件中的struct  tty_driver。它规定了tty驱动程序和高层之间的编程接口：

      struct  tty_driver [

          int  magic;

          /*  .  .  .  */

          int  major;

          int  minor_start;

          /*  .  .  .  */

          int  (*open)(struct  tty_struct  *tty,  struct  file  *flip);

          void  (*close)(struct  tty_struct  *tty,  struct  file  *flip);

          int  (*write)(struct  tty_struct  *tty,  const  unsigned  char  *buf,  int  count);

          void  (*ut_char)(struct  tty_struct  *tty, unsigned  char  ch);

          /*  .  .  .  .  */




 2、tty驱动程序需要两个步骤才能向内核注册自身：

   1）、通过调用tty_register_driver(struct  tty_driver  *tty_d)向tty核心注册自身。

   2）、调用下面的代码注册它支持的每个单独的tty：

       tty_register_device(struct  tty_driver  *tty_d,  unsigned  device_index,  struct  device  *device);







五、线路规程

 1、线路规程提供了一套灵活的机制，使得用户运行不同的应用程序时，能顾使用相同的串行驱动程序。底层和物理驱动和tty驱动负责从硬件上收发数据，而线路规程则负责处

       理这些数据，并在内核空间和用户kona进之间传递数据。




 2、串行子程序支持17种标准的线路规程。线路规程也实现通过串行传输协议实现的网络接口。




 3、设备实例：触摸控制器

   1）、打开与关闭

      I、为了创建线路规程，需要定义struct  tty_idisc，并向内核注册指定的入口函数集。

      II、代码清单6-2：线路规程操作。

      III、代码清单6-3：打开线路规程。

   2）、读数据过程

      I、对于中断驱动的设备，读取数据的过程通常由一前一后两个线程组成

           由于发起读数据请求而从用户空间发起的底层线程。

           由中断处理程序（接受来自设备的数据）唤醒的底层线程。

      II、线路规程读数据过程（P137  图6-5）。

      III、代码清单6-4：n_touch_receive_buf()方法。

      IV、代码清单6-5：唤醒读线程和中止串行驱动程序。

   3）、写数据过程

   4）、I/O控制

      I、当应用程序打开串行设备时，它通常可向其发出一下三类ioctl。

           串行设备驱动程序支持的命令。

           tty驱动程序支持的命令。

           关联的线程规程支持的命令。

   5）、其他操作

   6）、修改线程规程。

      I、打码清单6-6：从用户空间修改线程规程。







