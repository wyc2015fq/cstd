# 《Linux设备驱动程序》——TTY驱动程序 - 一世豁然的专栏 - CSDN博客





2014年12月11日 21:33:44[一世豁然](https://me.csdn.net/Explorer_day)阅读数：975








一、概论

 1、Linux tty驱动程序的核心紧挨在标准字符设备驱动层之下，并提供了一系列的功能，作为接口被终端类型使用。内核负责控制通过tty设备的数据流，并且格式化这些书据。




  2、tty核心概览

    1）、tty核心从用户那里得到将发往tty设备的数据然后把数据发送给tty线路规程驱动程序，该驱动程序负责把数据传送给tty驱动程序。

    2）、tty驱动程序对数据进行格式化，然后才能发送给硬件。

    3）、从tty硬件那里接收的数据将回溯至tty驱动程序，然后流入tty线路规程驱动程序，接着是tty核心，最后用户从tty核心那里得到数据。




  3、tty线路规程对于tty驱动程序来说是不透明的。




  4、有三种类型的tty驱动程序：控制台、串口和pty。




  5、为了却确定目前装载到内核中的是何类型的tty驱动程序，并确定目前使用的是何种tty设备，可查阅/proc/tty/drivers文件。该文件列举了当前使用的不同的tty驱动程序，显示

        了驱动程序的名称、默认的节点名称、驱动程序的主设备号、驱动程序所使用的次设备号以及tty驱动程序的类型。










二、小型TTY驱动程序

 一）、相关简介

  1、任何tty驱动程序的主要数据结构是结构tty_driver。它被用来向tty核心注册和注销驱动程序，对其的描述包含在内核touwenjian<linux/tty_driver.h>中。




  2、为了创建tty_driver结构的对象，必须把该驱动所支持的tty设备的数量作为参数调用函数alloc_tty_driver。

    1）、当函数alloc_tty_driver被调用成功后，tty_driver结构将根据tty驱动程序的需求、用正确的信息初始化。

    2）、tty_driver结构包含了大量的成员，但为使tty驱动程序能正常工作，并不是所有的成员都需要被初始化。




  3、为了向tty核心注册这个驱动程序。必须tty_driver结构传递给tty_register_driver函数。

    1）、当tty_register_driver被调用时，内核将根据tty驱动程序所拥有的所有次设备号，创建所有的不同sysfs_tty文件。

    2）、如果使用了devfs，并且不设置 TTY_DRIVER_NO_DEVFS标志位的情况下，也将创建devfs文件。

    3）、如果要让用户看到系统中已真实存在的设备，并且为用户保持更新。则可以调用tty_register_device，并设置该标识位，而正是devfs用户所期望的。




  3、在注册自身后，驱动程序使用tty_register_device函数注册它所控制的设备。该函数有三个参数：

    1）、属于该设备的tty_driver结构指针。

    2）、设备的次设备号。

    3）、指向tty设备所绑定的device结构指针。如果tty设备没有任何device结构，该参数为NULL。




  4、为了向tty核心注销驱动程序，所有使用tty_register_device函数注册的tty设备都需要使用tty_unregister_device函数清除自身。然后必须调用tty_unregoster_driver注销

         tty_driver结构。







 二）、termios结构

  1、在结构tty_driver中的init_termios变量是一个termios结构。如果用户在端口初始化以前就使用了该端口，那么该变量用来提供一系列安全的设置值。




  2、termios结构被用来为在tty设备上的某个特定段哦哭保存当前所有的设置。这些设置控制着当前的波特率，数据大小，数据流参数和其他一些值。

    1）、tcflags_t  c_ifags——输入模式标志。

    2）、tcflags_t  c_ofags——输出模式标志。

    3）、tcflags_t  c_cfags——控制模式标志。

    4）、tcflags_t  c_lfags——本地模式标志。

    5）、cc_t  c_line——线路规程类型。

    6）、cc_t  c_cc[NCCS]——控制字符数组。




  3、为了使tty驱动程序能正常工作，在tiny_tty_driver变量中所有的字段都是必须的。




  4、driver_name和那么十分相似，但它们有不同的用途。

    1）、在内核所有的tty驱动程序中，driver_name变量被设置成简短、描述性的、惟一的值。

    2）、name成员是在/dev目录中，定义分配给单独tty节点的名字。通过在改名字末尾添加tty设备序号来创建tty设备。它还被用来在/sys/class/tty目录中创建设备名。




  5、tty驱动程序和tty核心都使用flags变量表明当前驱动程序的状态以及该tty驱动程序的类型。驱动程序可以设置flags变量中的三个位：

    1）、TTY_DRIVER_RESET_TERMIOS。

    2）、TTY_DRIVER_REVL_RAW。

    3）、TTY_DRIVER_NO_DEVFS。










三、tty_driver函数指针

 一）、open和close

  1、当用户使用open打开由驱动程序分配的设备节点时，tty核心将调用open函数。tty核心使用分配给该设备tty_struct结构的指针，以及一个文件描述符作为参数调用该函数。




  2、当调用open函数时，tty驱动程序或者将数据保存到传递给它的tty_struct变量中，或者将数据保存在一个静态数组中，然后通过分配给该端口的次设备号进行引用。该步骤

        是必须的，这样在以后调用close、write和其他函数时，tty驱动程序能够知道是对那个设备进行操作。




  3、tiny_serial结构的定义：

     struct  tiny_seral  {

         struct  tty_struct  *tty;

         int  open_count;

         struct  semaphore  sem;

         struct  timer_list  *timer;

     };







 二）、数据流

  1、当数据要被发送给硬件时，用户调用write函数。首先tty核心接收到了该调用，然后内核将数据发送给tty驱动程序的write函数。tty核心同时也告诉tty驱动程序所发送数据的大

         小。




  2、有时由于tty硬件的速度及缓冲区大小的原因，当write函数被调用时，写操作系统所处理的数据不能同时都发送出去。write函数将返回发送给硬件的字符数，因此用户程序

        可以检查该值以判断是否写入了所有的数据。




  3、可以在中断上下文或者用户上下文中调用write函数。tty驱动程序在中断上下文中时，它不会调用任何可能休眠的函数。




  4、当子系统本身需要将一些数据传送到tty设备之外时，可以调用write函数。如果tty驱动程序并未在tty_struct中实现put_char函数，将会发生这类操作，此时，tty核心使用数据

        大小为1的参数回调write函数。







 三）、其他缓冲函数

  1、为了获取正在工作的tty驱动程序，在tty_driver中的chars_in_buffers函数并不是必须的，但是使推荐用。




  2、在tty_driver中有三个回调函数用来刷新驱动程序保存在的任何数据。它们并不需要一定被实现，但是如果tty驱动程序在发送给硬件前缓冲数据，还是推荐使用它们。这个三

        个回调函数是flush_chars函数和wait_until_sent函数与flush_buffer函数。




四）、怎么没有read函数。










四、TTY线路设置

 一）、相关简介

  1、当用户要改变路线设置，或者获得当前的线路设置，只需要调用多个termios用户空间库函数中的一个，也可直接对tty设备节点调用ioctl。tty核心将会把这两种接口的转换

        为一系列的tty驱动程序的回调函数，或者是ioctl调用。




 二）、set_termios

  1、大部分termios的用户空间函数会被库转换成对驱动程序节点的ioctl调用。大量的不同tty_ioctl调用会被tty核心转换成一个对tty驱动程序的set_termios函数调用。




  2、set_termios回调函数需要改变的是哪一个线路设置，然后再tty设备中对其进行改动。tty驱动程序必须能够对在termios结构中所有不同的设置进行解码，并对任何需要的改

         变做出响应。




  3、set_termois函数工作过程：

    1）、首先在要做的是判断是否需要改变设置。

    2）、为了确定所需要的字节大小，可以使用CSIZE掩码把正确的位从cflags变量中分离出去。如果不确定字节大小，习惯上被设置为默认的8数据位。

    3）、为了确定所需要的奇偶校检值，对cflags变量使用PARENB掩码，可以知道是否设置了奇偶校检。如果设置了奇偶校检，使用PARODD掩码能够判断使用的是奇校检还

              是偶校检。

    4）、对cflags变量使用CSTOPB位能够确定出是否使用了停止位。

    5）、有两种基本类型的流控制：软件控制和硬件控制。对cflags使用CRTSCTS掩码能确定用户是否使用的是硬件流控制。确定使用软件流控制的不同模式，以及使用不同的

              开始和停止字符。

    6）、确定使用的波特率。tty核心提供了函数tty_get_baud_rate以达到此目的。该函数返回一个整数值表示对特殊的tty设备所用的波特率。







 三）、tiocmget和tiocmset

  1、当内核要了解特定tty设备控制线路的当前物理值时，tty核心会调用tty驱动程序中的tiocmget函数。这常用于获得串口DTR和RTS控制线的值。

    1）、如果硬件不支持，tty驱动程序无法直接读取串口的MSR和MCR寄存器，将在本地保存这两个值得副本。




  2、当tty核心要为一个特定的tty设备设置控制线路值时，它将调用tty驱动程序中的tiocmset函数。tty核心通过传递两个变量set和clear，告诉tty驱动程序设置成什么值以及清除

        那一位。







五、ioctls

  1、当ioctl(2)为一个设备节点被调用时，tty核心将调用tty_driver结构中的ioctl回调函数。如果tty驱动程序不知道ru如何处理传递给它的ioctl值，它可返回-ENOIOCTLCMD，从

         而让tty核心执行一个通用的操作。




  2、在2.6版本的内核中定义了70个可以发送给tty驱动程序的不同的tty  ioctl。

    1）、TIOCSERGETLSR：获得这个tty设备线路状态寄存器(LSR)的值。

    2）、TIOCGSERIAL：获得串行线路信息。

    3）、TIOCSSERIAL：设置串行线路信息。

    4）、TIOCMIWAIT：等待MSR的变化。

    5）、TIOCGICOUNT：获得中断计数。







六、proc和sysfs对TTY设备的处理

  1、tty核心为任何tty驱动程序都提供了非常简单的办法，用来维护在proc/tty/driver目录中的一个文件。如果驱动程序定义了read_proc或者write_proc函数，将创建这个文件。




  2、当根据tty_driver结构中的TTY_DRIVER_NO_DEVFS标志注册tty驱动程序时，或者创建一个单独的tty设备时，tty核心处理所有的sysfs目录和设备的创建。单独的目录总是

        包含dev文件，这可以让用户空间的工作来判定分配给该设备的主设备号和次设备号。





七、tty_driver结构详解（P558-P560）




八、tty_operations结构详解(P560-P562)




九、tty_struct结构详解（P562-P564）




