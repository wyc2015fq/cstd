# 《Linux设备驱动程序》——高级字符驱动程序操作 - 一世豁然的专栏 - CSDN博客





2014年11月12日 08:58:43[一世豁然](https://me.csdn.net/Explorer_day)阅读数：999








一、ioctl

 1、ioctl相关简介

   1）、除了读取和写入设备之外，大部分驱动程序还需要另一个能力，即通过设备驱动程序执行各种类型的硬件控制。这些控制通常通过ioctl方法支持，该方法实现了同名的

            系统调用。

   2）、在用户空间，ioctl系统调用具有如下原型：

       int  ioctl(int  fd,  unsigned  long  cmd,  .  .  .  );

       I、一连串的“ . ”在Unix系统调用中代表可变数目的参数表。但在实际中，系统调用不会真正使用可变数目的参数，而是必须具有精确定义的原型。原型中的点并不是数目不定的

            一串参数而是可选参数，习惯上用char  *srgp定义。


       II、第三个参数的具体形式依赖于要完成的控制命令，也就是第二个参数。某些控制命令不需要参数，某些需要一个整数参数，而某些需要一个指针参数。

       III、ioctl系统调用的非结构化本质导致众多内核开发者趋向于丢弃它。让ioctl这种非结构化的iocctl参数在所有系统上表现一致也是非常困难的。

   3）、驱动程序的ioctl方法：

       int  (*ioctl)（struct  inode  *inode,  struct  file  *filep,  unsigned  int  cmd,  unsigne  long  arg）;

       I、inode和filep两个指针的值对应于应用程序传递的文件描述符fd。

       II、参数cmd由用户空间不经修改地传递给驱动程序，可选的arg参数则无论用户程序使用的是指针还是整数值，它都已unsigned  long的形式传递给驱动程序。

       III、如果调用程序没有传递第三个参数，那么驱动程序所接受的arg参数就处于未定义状态。

   4）、大多数ioctl的实现中都包括一个switch语句来根据cmd参数选择对应的操作。不同的命令被赋予不同的数值，为了简化代码，通常再爱吗中使用符号名称代替数值，这些

             符号名由C语言的预处理语句定义。




 2、选择ioctl命令

   1）、在编写ioctl代码之前，需要选择对应不同命令的编号。为了防止对错误的设备使用正确的命令，明琳更应该在系统范围内唯一。

   2）、为方便程序员创建惟一的ioctl命令号，每一个命令号被分为多个位子段。Linux的第一个版本使用了一个十六位整数，高8位是与设备有关的“幻”数，低8位是一个序列号

             码，在设备内唯一。

   3）、要按Linux内核约定的方法为驱动程序选择ioctl编号，应该首先看看include/asm/ioctl.h和Documentational/ioctl-number.txt这两个文件。

       I、头文件定义了要使用的位字段：类型（幻数）、序数、传递方向以及参数大小等等。

       II、ioctl-number.txt文件中罗列了内核所使用的幻数，这样，在选择自己的幻数就能避免和内核冲突。

   4）、定义号码的新方法使用了4个位子段，其含义如下面所给出：

       I、type——幻数（_IOC_TYPEBITS）。

       II、number——序号(_IOC_NRBITS)。

       III、direction——定义数据传输的方向(_IOC_NONE、_IOC_READ、_IOC_WRITE)。

       IV、size——所涉及的用户数据大小。

   5）、<linux/ioctl.h>中包含的<asm/ioctl.h>头文件定义了一些构造命令编号的宏：

      I、_IO(type,nr)——用于构造无参数的命令编号。

      II、_IOR(type,nr,datatype)——用于构造从驱动程序中读取数据的命令编号。

      III、_IOW(type,nr,datatype)——用于构造写入数据的命令编号。

      IV、_IOWR(type,nr,datatype)——用于双向传输。

      V、type和number位子段通过参数传入，而size位子段通过对datatype参数取sizeof获得。

      VI、此头文件还定义了用于揭开位子段的宏：_IOC_DIR(nr)、_IOC_TYPE(nr)、IOC_NR(nr)和_IOC_SIZE(nr)。

   6）、scull中的一些ioctl命令的定义

      I、可以用两种方式实现整数参数传递——通过指针和显式的数值。同样，这两种方法还用于返回整数——通过指针或通过设计返回值。

      II、从任何一个系统调用返回时，正的返回值是受保护的，而负的返回值责被认为是一个错误，并用来设备用户空间中的errno变量。

      III、显式的命令序数没什么特别的含义，仅仅用来区分命令。除了在声明中用到序数之外，在别的地方都不适用它。

      IV、除了少量预定义的命令之外，内核并未使用ioctl的cmd参数，以后也不太可能使用。




 3、返回值

   1）、ioctl的实现通常就是一个基于命令号的switch语句。但当命令号不匹配任何合法的操作时：

      I、有些内核会返回-ENVAL。然而，POSIX标准规定，如果使用了不适合的ioctl参数，听该返回-EINOTTY。

      II、对非法的ioctl命令返回-EINVAL仍然是很普遍的做法。




 4、预定义命令

   1）、尽管ioctl系统调用绝大部分用于操作设备，但还有一些命令是可以由内核识别的。要注意，当这些命令用于我们的设备时，他们会在我们自己的文件操作被调用之前被

             解码。如果为自己的ioctl命令选用了于这些预定义相同的编号，就永远不会受到该命令的请求，而且由于ioctl编号冲突，应用程序的行为无法预测。

   2）、预定义命令分为三组，最后一组名命令只能在宿主文件系统上执行。设备驱动程序开发人员支队第一组感兴趣。

      I、可用于任何文件（普通、设备、FIFO和套接字）的命令。

      II、只用于普通文件的命令。

      III、特定于文件系统类型的命令。

   3）、下列ioctl命令对任何文件都是预定义的：

      I、FIOCLEX。

      II、FIONCLEX。

      III、FIOASYNC。

      IV、FIOQSIZE。

      V、FIONBIO。




 5、使用ioctl参数

   1）、对于ioctl附加参数，如果是整数，直接使用就可以，如果是指针，就需要注意——当用一个指针指向用户控件时，必须确保指向的用户空间是合法的。

   2）、首先通过access_ok验证地址，该函数在<asm/uaccess,h>中声明：

       int  access_ok(int  type,  const  void  *addr,  unsigned long  size);

       I、第一个参数应该是VERIFY_READ或VERIFY，取决于要执行的动作是读取还是写入用户空间内存区。

       II、addr参数是一个用户空间地址。

       III、size是字节数。

       IV、access_ok返回一个布尔值：1表示成功，0表示失败。如果返回失败，驱动程序通常要返回-EFAULT给调用者。

       V、关于access_ok，需要注意两点

              第一，他并没有完成验证内存的全部工作，而只是检查了所引用的内存是否位于进程有对应访问权限的区域内，特别是要确保访问地址没有指向内核空间的内存区。

              第二，大多数驱动程序代码中都不需要真正调用access_ok，因为内存管理程序会处理它。

   3）、程序员可以使用已经为最常用的数据大小优化过的一组函数，其定义在<asm/uaccess.h>中。

      I、put_user(datum,  ptr);

           _ _put_user(datum,  ptr);

      II、get_user(datum,  ptr);

           _ _get_user(datum,  ptr);

   4）、如果使用上面的函数传递大小不符合任一一个特定值的数值，结果通常是编译器给出一条奇怪的消息。在这种情况下，必须使用copy_to_user或者copy_from_user。




 6、权能和受限制操作

   1）、Linux内核提供了权能的系统。基于权能的系统抛弃了那种要么全有要么全无的特权分配方式，二十八特权划分为独立的组。这样，某个特定的用户或程序就可以被授权

             执行某一指定的特权操作，同时又没有执行其他不相关操作的能力。

   2）、内核专为许可管理使用权能并导出了两个系统调用capget和capset，这样就可以从用户空间来管理权能。

   3）、全部权能操作都可以在<linux/capability.h>中找到，其中包含了系统能够理解的所有权能；不修改内核源代码，驱动程序作者或系统管理员就无法定义新的权能。对驱动

             程序开发者来讲有意义的权能如下所示：

      I、CAP_DAC_OVERRIDE。

      II、CAP_NET_ADMIN。

      III、CAP_SYS_MODULE。

      IV、CAP_SYS_RAWIO。

      V、CAP_SYS_TTY_CONFIG。

   4）、在执行一项特权操作之前，设备驱动程序应该检查调用进程是否合有合法的权能。权能的检查通过capable函数实现：

              interesting capable（interesting capability）；




 7、ioctl命令的实现

   1）、scull的ioctl实现。

   2）、从调用方的观点看，传送和接受参数的6中途径如下：

             int  quantum;

             ioctl(fd,  SCULL_IOCSQUANTUM,  &quantem);

             ioctl(fd,  SCULL_IOCTQUANTUM,  quantem);

             ioctl(fd,  SCULL_IOCGQUANTUM,  &quantem);

             quantum = ioctl(fd,  SCULL_IOCQQUANTUM);

             ioctl(fd,  SCULL_IOCXQUANTUM,  &quantum);


             quantum = ioctl(fd,  SCULL_IOCHQUANTUM,  quantum);

       I、正常的驱动程序不会昏庸多种模式。

       II、通常情况下数据交换形式应该保持一致，要么用指针，要么用数值，尽量薄面混用。




 8、非ioctl的设备控制

   1）、通过想设备写入控制序列可以更好地控制设备。在控制台驱动程序中就是用了这一技术，称为“转义序列”，用于控制移动移动光标、改变默认颜色或执行其他的配置任

              务。

   2）、用控制序列的好处是：用户仅通过写数据就可以控制设备，无需使用配置设备的程序，如果我们用这种方法控制设备，发出命令的程序甚至无需运行在设备所在的同一

             系统上。

   3）、通过写入来控制的方式非常适合于那种不传递数据而响应命令的设备。

   4）、当编写这种“面向命令的”驱动程序时，没什么必要实现ioctl方法。在解释器中新增一条命令，其实现和使用都更简单。







二、阻塞I/O

 1、休眠的简单介绍

   1）、当一个进程被置入休眠时，它会被标记为一种特殊的状态并从调度器的运行代码中移走。知道某些情况下修改了这些标记，进程才会在任意CPU上调度，也即运行该进

              程。休眠中的进程会被搁置一旁，等待将来某个事件的发生。

   2）、为了将进程以一种安全的方式进入休眠，需要牢记两条规则。

       I、第一条规则：永远不要再原子上下文中进入休眠。

       II、第二个规则：当进程被唤醒时，进程永远不知道休眠了多长时间，或者休眠期间都发生了什么事。

       III、除非知道进程会被在其他进程唤醒，否则不能休眠。完成唤醒任务的代码还必须能够找到进程。

   3）、在Linux中，一个等待队列通过一个“等待队列头”来管理。等待队列头是一个类型wait_queue_head_t的结构体，定义在<linux/wait.h>中。

       I、可以通过静态定义并初始化一个等待队列头：

            DECLARE_WAIT_QUEUE_HEAD（name）;

       II、或者采用动态定义：

            wait_queue_head_t  my_queue;

            init_waitqueue_head(&my_queue);




 2、简单休眠

   1）、Linux内核中最简单的休眠方法是称为wait_event的宏（以及它的几个变种）；在实现休眠的同时，它也检查进程等待的条件：

       wait_event(queue,  condition);

       wait_event_interruptible(queue,  condition);

       wait_event_timeout(queue,  condition,  timeout);

       wait_event_interruptible_timeout(queue,  condition,  timeout);

       I、queue是等待队列头。它通过“值”传递，而不是通过指针。condition是任意一个布尔表达式，上面的宏在休眠前后都要对该表达式求值。

       II、使用wait_event，进程将被置于非中断休眠。

       III、使用wait_event_interruptible，进程将被置于中断休眠。可返回一个整数值，非零表示休眠被某个信号中断，而驱动程序也需要返回-ERESTARTSYS。

       IV、wait_event_timeout和wait_event_interruptible_timeout只会等待限定的时间，当给定时间到期时，这两个宏都会返回0值，而无论condition如何求值。

   2）、用来唤醒休眠进程的基本函数是wake_up，它也有多种形式，先介绍两个：

      void  wake_up(wait_queue_head_t  *queue);

      void  wake_up_interruptible(wait_queue_head_t  *queue);

      I、wake_up唤醒等待在给定queue上的所有进程。

      II、wake_up_interruptible只会唤醒执行可中断休眠的进程。

      III、在使用wait_event时使用wake_up。在使用wait_event_interruptible时使用wake_up_interruptible。




 3、堵塞和非堵塞型操作

   1）、显式的非堵塞I/O由filp->f_flags中的O_NONBLOCK标志决定。这个标志在<linux.fcntl.h>中定义，头文件在自动包含在<linux/fs.h>中。

   2）、在执行堵塞型操作的情况下，应该实现下列动作以保持和标准语义一致：

      I、如果一个进程调用了read但是还没有数据可读，此进程必须堵塞。数据到达时进程被唤醒，并把数据返回给调用者。

      II、如果一个进程调用了write但缓冲区没有空间，此进程必须被堵塞，而且必须休眠在于读取进程不同的等待队列上。

   3）、在驱动程序中实现输出缓冲区可以提高性能。

   4）、在scull中没有使用输入缓冲区，类似的，也没有输出缓冲区。

   5）、如果指定了O_NONBLOCK标志，read和write的行为就会有所不同。如果在数据没有就绪时调用read或者在缓冲区没有空间时调用write，则该调用简单额返回

             -EAGAIN。

   6）、非堵塞型操作会立即返回，使得应用程序可以查询数据。在处理非堵塞型文件时，应用程序调用stdio函数必须非常小心，因为很容易把一个非堵塞返回误认为是EOF，

             所有必须始终检查errno。

   7）、置于read、write和open熟非堵塞标志影响。




 4、一个堵塞I/O示例




 5、高级休眠

   1）、头文件<linux/wait.h>中的wait_queue_head_t类型后面的数据由一个自旋锁和一个链表组成。链表中保存的是一个等待队列的入口，该入口声明为wait_queue_t类型。

             这个结构包含了休眠进程的信息及其期望被唤醒的相关细节信息。

   2）、将进程置于休眠的步骤：

      I、分配并初始化一个wait_queue_t结构，然后再将其加入到对应的等待队列。

      II、设置进程的状态，将其标记为休眠。

      III、放弃处理器，但在此之前还要做另外一件事情：必须首先检查等待的条件。

      IV、在if测试以及可能的schedule调用（并返回）之后，需要完成一些清理工作。




 6、手工休眠

   1）、第一个步骤是建立并初始化一个等待队列入口：

       DEFINE_WAIT(my_wait);

       也可以通过下列方法完成

       wait_queue_t  my_wait;

       init_wait(&my_wait);


   2）、将等待队列入口添加到队列中，并设置进程的状态：

       void  prepare_to_wait(wait_queue_head_t  *queue,  wait_queue_t  *wait,  int  state);

   3）、在调用prepare_to_wait之后，进程即可调用schedule。当然在这之前，应确保仍有必要等待，一旦schedule返回，就可以清理了：

       void  finish_wait((wait_queue_head_t  *queue,  wait_queue_t  *wait);




 7、独占等待

   1）、独占等待和休眠的不同。

      I、等待对垒入口设置了WQ_FLAG_EXCLUSIEV标志，则会被添加到等待队列的尾部，而没有这个标志则被添加到等待队列的头部。

      II、在某个等待队列上调用wake_up时，他会在唤醒第一个具有WQ_FLAG_EXCLUSIEV标志的进程后停止唤醒其他进程。

   2）、使用独占等待的两个条件

      I、对某个资源存在严重竞争。

      II、并且唤醒单个进程就能完整消耗该资源。

   3）、将进程置于中断等待状态：

      void  prepare_to_wait_exclusive(wait_queue_head_t  *queue,  wait_queue_t  *wait,  int  state);




 8、唤醒相关细节

   1）、wake_up变种。




 9、测试Scullpipe驱动程序







三、poll和select

 1、使用非堵塞I/O的应用程序也经常使用poll、select和epoll系统调用。

   1）、poll、epoll和select的功能本质上是一样的：都允许决定进程是否可以对一个或多个打开的文件做非堵塞的读取或写入。

   2）、这些系统调用也会堵塞进程，直到给定的文件描述符集合中的任何一个可读取或写入。

   3）、这些系统调用常常用于那些需要使用多个输入或输出流而不会堵塞于其中任何一个六的应用程序中。

   4）、对上述系统调用的支持需要来自设备驱动程序的相应支持。三个系统调用均通过驱动程序中的poll方法提供，该方法具有如下原型：

         unisgned  int  (*poll)(struct  file  *filp,  poll_table  *wait);

         I、当用户空间程序在驱动程序关联的文件描述符上执行poll、select和epoll系统调用时，该驱动程序方法被调用。

         II、该设备方法分为两个步骤。

             A、在一个或多个可指示poll状态变化的等待队列上调用poll_wait。

             B、返回一个用来描述操作是否可以立即无阻塞执行的位掩码。


         III、传递给poll方法的第二个参数：poll_table，用于在内核中实现poll、select和epoll系统调用。

   5）、通过poll_wait函数，驱动程序向poll_table结构增加一个等待队列：

      void poll_wait(struct  file  *,  wait_queue_head_t  *,  poll_table  *);

   6）、poll方法执行的第二项任务是返回描述那个操作可以立即执行的位掩码。几个标志用来指明可能的操作（P164-P165）。




 2、与read和write的交互

   1）、poll和select调用的目的是确定接下来的I/O操作是否会堵塞。从这方面来说，它们是read和write的补充poll和select的更重要的用途是它们可以使应用程序同时等待多个

             数据流。

   2）、从设备读取数据。

   3）、向设备写数据。

   4）、刷新待处理输出。

       I、fsync函数可以满足所有数据输出的需求，它通过同名系统调用来调用，该方法的原型是：

            int  (*fsync)(struct  file  *file,  struct  dentry  *dentry,  int  datasync);

           A、如果应用程序需要确保数据已经被传送到设备上，就必须实现fsync方法。一个fsync调用只有在设备被完全刷新时才会返回。是否设置了O_NONBLOCK标志对此没有

                 影响。

           B、参数datasync用于区分fsync和fdatasync这两个系统调用。

           C、fsync方法没有什么特别的地方。这个调用对时间没有严格要求，所以每个驱动程序都可以按照作者的喜好实现它。




 3、底层的数据结构

   1）、当用户应用程序调用了poll、select或epoll函数时，内核会调用由该系统调用引用的全部文件的poll方法，并向它传递一个poll_table_poll_table结构是构成实际数据结

             构的一个简单封装。对poll和select系统调用，后面这个结构是包含poll_table_entry结构的内存页链表。

      I、每个poll_table_entry结构包括一个指向被打开设备的struct  file类型的指针，一个wait_queue_heda_t指针以及一个关联的等待队列入口。

      II、如果轮询（poll）时没有一个驱动程序指明可以进行非堵塞I/O，这个poll调用就进入休眠，知道休眠在其中上的某个（或多个）等待队列唤醒它为止。

      III、poll实现中的驱动程序的poll方法在被调用时为poll_table参数传递指针。

      IV、在poll调用结束时，poll_table结构被重新分配，所有的先前添加到poll表中的等待队列入口都会从这个表以及等待队列中移除。

   2）、poll背后的数据结构（P169  图6-1）。

   3）、出现新的epoll系统调用的原因。







四、异步通知


 1、通过使用异步通知，应用程序可以在数据可用时收到一个信号，而不需要不停地使用轮询来关注数据。




 2、为了启用文件的异步通知机制，用户程序必须执行两个步骤。

   1）、首先，用户程序指定一个进程作为文件的“属主”。当进程使用fcntl系统调用执行F_SETOWN命令时，属主进程的进程ID号就被保存在filp->f_owner中。这一步是必须

             的，目的是为了让内核知道应该通知那个进程。

   2）、然后，为了真正启用异步通知机制，用户程序还必须在设备中设备FASYNC标志，这通过fcntl的F_SETFL命令完成。

   3）、执行完这两个步骤后，输入文件就可以在新数据到达时请求发送一个SIGIO信号。该信号被发送到存放在filp->f_owner中的进程（如果是负值就是进程组）。




 3、需要注意的是：

   1）、并不是所有设备都支持异步通知，我们也可以择不提供异步通知功能。应用程序通常假设只有套接字和终端才有异步通知的能力。

   2）、当进程收到SIGIO信号时，它并不知道是那个输入文件有了新的输入。如果有国语一个文件可以异步通知输入的进程，则应用程序必须借助polll或select来确定输入的来

             源。  




 4、从驱动程序角度来看实现异步信号的详细操作过程

   1）、F_SETOWN被调用时对filp->f_owner赋值，此外什么也不做。

   2）、在执行F_SETOWN启动FASYNC时，调用驱动程序的fasync方法。只要filp->f_flags中的FASYNC标志发生了变化，就会调用该方法，以便把这个变化通知驱动程序，使

              其能正确响应。文件打开时，FASYNC标志被默认为是清除的。

   3）、当数据到达时，所有注册为异步同志的进程都会发送一个SIGIO信号。


      I、第一步实现很简单，在驱动程序部分没什恶魔可做。其他步骤则要涉及维护一个动态数据结构，以跟踪不同的异步读取进程，这种进程keen那个会有好几个。动态数据结

           构并不依赖于特定的设备，内核已经提供了一套合适的通用实现方法。

      II、Linux中这种通用实现方法基于数据结构和两个函数。含有相关声明的头文件是<linux/fs.h>，那个数据结构称为struct  fasync_struct。

      III、和处理等待队列的方式相似，需要把一个该类型的指针插入设备特定的数据结构中去。

   4）、驱动程序要调用的两个函数的原型如下；

      int  fasync_helper(int  id,  struct  file  *filp,  int mode,  struct  fasync_struct  **fa);

      void  kill_fasync(struct  fasync_struct  **fa,  int  sig,  int  band);

      I、当一个打开的文件的FASYNC标志被修改时，调用fasync_helper以便从相关的进程列表中增加火删除文件。除了最后一个参数外，它的其他所有参数都是提供给fasync方

          法的相同函数，因此可直接传递。

      II、在数据到达时，可使用kill_fasync通知所有的相关进程。它的参数包括要发送的信号（通常是SIGIO）和带宽（band），后者几乎总是POLL_IN。

   5）、scullpipe中实现fasync的方法

   6）、当文件关闭时必须调用fasync方法，以便从活动的异步读取进程列表中删除该文件。尽管这个调用只在filp->f_flags设备了FASYNC标志时才是必须的，但不管什么情

             况，调用它不会有什么坏处。

   7）、异步通知所使用的数据结构和struct  wait)queue使用的几乎相同。不同之处在于前者用struct  file替换了atruct  task_struct。队列中的file结构用来获取f_owner，以便给

             进程发送信号。







五、定位设备

 1、llseek实现

   1）、llseek方法实现了lseek和llseek系统调用。

      I、如果涉恶必操作未定义llseek方法，内核默认通过修改filp->f_pos而执行定位，filp->f_pos是文件的当前位置。

      II、为了使lseek系统调用能够正确工作，read和write方法必须通过更新它们收到的偏移量参数来配合。

   2）、如果定位操作对应于设备的一个物理操作，可能需要提供自己的llseek方法。

   3）、scull的llseek实例及其分析。








六、设备文件的访问控制


 1、独享设备

   1）、一次只允许一个进程打开设备有很多令人不快的特性，不过这也是设备驱动程序中最容易实现的访问控制方法。

   2）、scullsingle设备的独享设备实例。




 2、限制每次只由一个用户访问

   1）、在构造独享设备之后，我们要建立允许单个用户在多个进程中打开的设备，但是每次只允许一个用户打开该设备。

   2）、与独享策咯相比，实现这些访问策咯需要更多的技巧，此时需要两个数据项，一个打开技术和设备属主的UID。同样这些数据最好保存在设备结构内部。




 3、替代EBUSY的堵塞型open

   1）、当设备不能访问时返回一个错误，通常这是最合理的方式，但有些情况下可能需要让进程等待设备。

   2）、替代EBUSY的另一个方法是实现堵塞型open。

   3）、堵塞型open实现中的问题：对于交互式用户来说它是令人很不愉快的，用户可能会在等待中猜测设备除了什么问题。




 4、在打开时复制设备

   1）、另一个实现访问控制的方法是：在进程打开设备时创建设备的不同私有副本。显然这种方法只有在设备没有绑定到某个硬件对象时才能实现。






