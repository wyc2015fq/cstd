# 《Linux设备驱动程序》——字符设备驱动程序 - 一世豁然的专栏 - CSDN博客





2014年11月03日 21:00:47[一世豁然](https://me.csdn.net/Explorer_day)阅读数：723








一、scull的设计

 1、scull——区域装载的简单字符工具。




 2、scull的优点。




 3、scull的源代码实现了下列设备，将由模块实现的每种设备称作一种“类型”。

   1）、scull0-scull3

        这四个设备分别由一个全局变量且持久的区域组成。

   2）、scullpile0-scullpipe3

         这四个FIFO（先入先出）设备与管道类似。

   3）、scullsingle

              scullpriv

              sculluid

              scullwuid

         这些设备与scull0相似，但在合适允许open操作方面有一些限制。







二、主设备号和次设备号

 1、主设备号和次设备号相关介绍


   1）、字符设备驱动程序的设备文件可通过ls -l命令输出第一列中的“c”来识别。快设备也出现在/dev下，它由字符“b”标识。

   2）、通常而言，主设备号标识设备对应的驱动程序。现代的的Linux内核允许多个驱动程序共享主设备号，但看到的多数设备仍然按照“一个主设备号对应一个驱动程序”的原

             则组织。

   3）、次设备号由内核使用，用于确定设备文件所指的设备。依赖于驱动程序的编写方式，可以通过次设备号获得一个指向内核设备的直接指针，也可将次设备号当作设备本

             地数组的索引。




 2、设备编号的内部表达

   1）、在内核中，dev_t类型用来保存设备编号——包括主设备号和次设备号。

   2）、要获得dev_t的主设备号或次设备号：

** MAJOR(dev_t  dev);**

**          MINOR(dev_t  dev);**

   3）、如果要将主设备号和次设备号转换成dev_t 类型：

**MKDEV(int  major,  int  minor);**

   4）、2.6内核可以容纳大量设备。将来dev_t 格式会发生变化，但如果我们小心地编写自己的驱动程序，那么这种变化不会带来问题。




 3、分配和释放设备编号

   1）、在建立一个字符设备驱动之前，驱动程序首先要做的事情就是获得一个或多个设备编号：

** int  register_chrdev_region(dev_t  first,  unsigned  int  count,  char  *name);**

          I、first是要分配的设备编号范围的起始值。first额次设备经常被设置为0，但对该函数并不是必须的。

          II、count是请求的连续设备编号的个数。注意，如果count非常大，则所请求的范围可能和下一个主设备重叠，但只要我们所请求的编号范围是可用的，则不会带来任何问

                题。

          III、name是和该编号范围关联的设备名称，它将出现在/proc/devices和sysfs中。

          IV、register_chrdev_region的返回值在分配成功时为0.在错误情况下，将返回一个负的错误码，并且不能使用所请求的编号区域。

   2）、在运行过程中使用下面的函数，内核将会为我们恰当分配所需要的主设备号：

**  int  alloc_chrdev_region(dev_t  *dev,  unsigned  int  firstminor,  unsigned  int  count,  char  *name);**

          I、dev仅用于输出的参数，在成功完成调用后将保存已分配范围的第一个编号。

          II、firstminor应该是要使用的被请求的第一个次设备编号，它通常是0.

          III、count和name参数是与register_chrdev_region函数一样的。

   3）、设备编号的释放：

         void  unregister_chrdev_region(dev_t  first,  unsigned  int  count);

          I、通常，在模块的清除函数中使用unsigned_chrdev_region函数。

   4）、以上函数为驱动程序的使用分配设备编号，但是它们并没有告诉内核关于拿来这些编号要作什么工作。在用户空间程序可访问上述设备编号之前，驱动程序需要将设备

             编号和内部函数链接起来，这些内部函数用来实现设备的操作。




 4、动态分配主设备号

   1）、一部分主设备号已经静态地分配给了大部分常见设备。将某个已经静态分配好的静态编号用于新驱动程序的机会非常小，尚未被分配的新编号是可用的。

   2）、作为驱动程序作者，我们有如下选择：

        I、可以选定一个尚未被使用的编号。

        II、或者通过动态方式分配主设备号。

   3）、对于一个新的驱动程序，强烈建议读者不要随便选择一个当前未使用的设备号作为主设备号，而应该改使用动态分配机制获取主设备号。所以，驱动程序应该使用

             alloc_chrdev_region而不是register_chrdev_region函数。分配主设备号的最佳方式是：默认采用动态分配，同时保留在加载甚至是编译时指定的主设备号的余地。

   4）、动态分配的缺点是：由于分配的主设备号不能保证始终一致，所以无法预先创建设备节点。







三、一些重要的数据结构

 1、大部分基本的驱动程序操作涉及到三个重要的内核数据结构，分别是file_oprations、file和inode。




 2、文件操作（file_oprations）

   1）、file_oprations结构将驱动程序链接到设备编号。

         I、此结构定义在<linux/fs.h>中，其中包含了一组函数指针、每个打开的文件和一组函数关联。

         II、这些操作主要用来实现系统调用，命名为open、write等等。

   2）、文件操作（P54-P57）




 3、file结构

   1）、此结构与用户空间程序中的FILE没有任何关联。

   2）、file结构代表一个打开的文件。它由内核在open时创建，并传递给在内核文件上进行操作的所有函数，知道最后的close函数。

   3）、在内核源代码中，指向struct file的指针通常被称为file或filp

   4）、struct  file中最重要的成员（P57-P59）




 4、inode结构

   1）、内核用inode结构在内部表示文件，因此它和file结构不同，后者表示打开的文件描述符。

   2）、inode结构中包含大量有关文件的信息。作为常规，只有两个字段对编写驱动代码有用：

         dev_t  i_rdev;

         对表示设备文件的inode结构，该字段包含了真正的设备编号。


         struct  cdev  *i_cdev;

         struct  cdev是表示字符设备的内核的内部结构。当inode指向一个字符设备文件时，该字段包含了struct  cdev结构的指针。

   3）、从你一个inode结构中获得主设备号和次设备号：

**  unsigned  int  iminor(struct  inode  *inode);**

**         unsigned  int  imajor(struct  inode  *inode);**







四、字符设备的注册

 1、分配和初始化struct  cdev结构的方式有两种：

     I、如果打算在运行时获取一个独立的cdev结构，则应该如下编写代码；

        struct  cdev  *my_cdev = cdev_alloc();

        my_cdev->ops = &my_fops;

        此时，可以将cdev结构嵌入到自己的设备特定结构中。以上情况下，需要哦一年以下代码来初始化已分配到的结构：

        void  cdev_init(struct  cdev  *cdev,  struct  file_opration  *fops);

        另外，还有一个struct  cdev的字段需要初始化。和file_opration结构类似，struct  cdev结构也有一个所有者字段，应该被设置为THIS_MODULE。

        在cdev设置好之后，最后的步骤是通过下面的调用告诉内核该结构的信息：

        int  cdev_add(struct  cdev  *dev,  dev_t  num,  unsigned  int  count);

        dev是cev结构，num是该设备对应的第一个设备编号，count是应该和该设备关联的设备编号的数量。

        cdev_add可能会失败。如果他返回一个负的错误码，则设备不会被添加到系统中。





 2、scull中的设备注册

   1）、在scull内部，它通过struct  scull_dev的结构来表示每个设备，该结构定义如下：


         I、struct  scull_dev {

                      struct  scull_qset  *data;

                      int  quantum;

                      int  qset;

                      unsigned  long  size;

                      unsigned  int  access_key;

                      struct  semaphore  sem;

                      struct  cdev  cdev;

                };

   2）、struct  cdev结构必须被初始化并添加到系统中，scull中的代码（P61），因为cdev结构被嵌入到了struct  scull_dev中，因此必须调用cdev_init来执行该结构的初始化。




 3、早期的办法（略）







五、open和release

 1、open方法

   1）、open1方法提供了给驱动程序以初始化的能力，从而为以后的操作完成初始化做准备。在大部分驱动工作中，open应完成如下工作。

          I、检查设备特定的错误。

          II、如果设备首次打开，则对其进行初始化。

          III、如有必要，更新f_op指针。

          IV、分配并填写置于filp->private_data里的数据结构。

   2）、确定要打开的具体设备

          int  (*open)(struct  inode  *inode,  struct  file  *filp);

         I、inode参数在其i_cdev字段中包含了我们所需要的信息，继先前设置的cdev结构。但我们通常不需要cdev结构本身，二十希望得到包含cdev结构的scull_dev结构。

    3）、获取包含cdev结构的scull_dev

          container_of(pointer,  container_type,  container_field);

         I、此宏需要一个container_field字段的指针，该字段包含在container_type类型的结构中，然后返回该字段的结构指针。

         II、在scull_open中，此宏用来找到适当时设备结构

           struct  scull_dev  *dev;

           dev = container_of(inode->i_cdev,  struct  scull_dev,  dev);

           filp->private_data = dev;

          一旦代码找到scull_dev结构之后，scull降低一个指针保存到了file结构的private_data字段中，这样可方便今后对该指针的访问。

   4）、另外一个确定要打开的设备的方法是：检查保存在inode结构中的次设备号，如果利用register_chrdev注册自己的设备，则必须使用该技术。

         I、微简化的scull_open代码（P63）




 2、release方法

   1）、此设备方法完成如下任务：

        I、释放由open分配的、保存在filp->privte_data中的所有内容。

        II、在最后一次操作时关闭设备。

   2）、scull的基本模型没有需要关闭的硬件，因此代码量最少。

       int  scull_release(struct  inode  *inode,  struct  file  *filp)

       {

           return  0;


       };

   3）、并不是每个close系统调用都会引起对release方法的调用。只有那些真正释放设备数据结构的close调用才会调用这个方法。内核对每个file结构维护其被使用多少次额

             计数器。







六、scull的内存使用

 1、scull使用的内存区域分也称为设备，其长度是可变的。写的越多，它就越长；可用更短的文件以覆盖方式写设备时则会变短。




 2、scull驱动程序引入了Linux内核中用于内存管理的两个核心函数：

       void  *kmalloc(size_t  size,  int  flags);

       void  kfree(void  *ptr);

      I、对kmalloc的调用将试图分配size个字节大小的内存；其返回值指向该内存的指针，分配失败返回NULL。

      II、flags参数用来描述内存的分配方法。

      III、由此kmalloc函数分配的内存用kfrre释放。不应该将非kmalloc返回的指针传递给kfree。但可以将NULL传递。




 3、在scull中，每个设备都是一个指针链表，其中每个指针都指向一个scull_qset结构。默认情况下，没这样一个结构通过一个中间指针菽粟最多可引用4000000个字节。scull

       设备的布局（P65  图3-1）。

   1）、为量子和量子集选择合适的数值是一个策略问题而非机制问题，而且最优数值依赖于如何使用设备。

   2）、在scull设备中，用户可以采用几种方式来修改量子和量子集的值：在编译时，可以修改scull.h中的宏SCULL_QUANTUM和SCULL_QSET；在模块加载时，可以设置

              scull_quantum和scull_qset的整数值；或者在运行时，使用ioctl修改当前值以及默认值。

   3）、默认值的选择问题就在于寻找量子和量子集或大或小之间的最佳平衡点。此外，还必须考虑kmalloc内部额设计。

   4）、表示scull设备的scull_dev结构，该结构的quantum和qset字段分别保存设备的量子和量子集大小。但是，实际的数据由另外的接哦古处理，该结构称为：

          struct  csull_qset {

                 void  **data;

                 struct  scull_qset  *next;

          };

   5）、利用struct  scull_dev和struct  scull_qset保存数据（P66-P67代码）







七、read和write

 1、read和write方法完成的任务相似，拷贝数据到应用程序空间，或者反过来从应用程序空间拷贝数据。

**ssize_t  read(struct  file  *filp,  char  _ _user  *buf,  **

**               size_t  count,  loff_t  *offp);**

**     ssize_t  read(struct  file  *filp,const  char  _ _user  *buf,                 size_t  count,  loff_t  *offp);**

     I、参数filp是文件指针。

     II、参数count是请求传输的数据长度。

     III、参数buff是指用户空间的缓冲区，此缓冲区保存要写入的数据或者存放新读入数据的空缓冲区。

     IV、offp是一个指向“long  offset  type（长偏移类型）”对象的指针，此指针指明用户在文件中进行存取操作的位置，返回值是“signed  size  type（有符号的尺寸类型）”。

     V、read和write方法的buff参数是用户空间的指针。因此，内核代码不能直接引用其中的内容。

     VI、驱动程序必须访问用户空间的缓冲区以便完成自己的工作。为了确保安全，这种访问应该始终通过内核提供的专用函数完成。




 2、scull的read和write代码要做的工作就是在用户地址空间和内核地址空间之间进行整段数据的拷贝。此由下面内核函数提供：

**     unsigned   long  copy_to_user(void  _ _user  *to,  const  void  *from,  unsigned  long  count);**

**     unisgned  long  copy_from_user(void  *to,  const  void  _ _user  *from,  unsigned  long  count);**

    1）、当内核空间内运行的代码访问用户空间是要注意。


    2）、这两个函数的作用并不限于在内核空间和用户控件之间拷贝数据，它们还会检查用户空间的指针是否有效。如果指针无效，就不会进行拷贝；另一方面，如果在拷贝过

             程中遇到无效指针，则会仅仅复制部分数据。在这两种情况下，返回值是还需要拷贝的内存数量值。

    3）、如果不需要检查用户空间的指针，则建议使用_ _copy_to_user和_ _copy_from_user。不管read和write方法传输了多少数据，一般而言都应更新*filp所表示的文件位

              置，以便反应在新系统调用成功之后当前的文件位置。适当情况下，内核会将文件位置的改变传播会file结构。




 3、出错时，read和write方法都返回一个负值。大于或等于0的返回值告诉调用程序成功传输了多少个字节。如果在正确传输部分数据之后发生了错误，则返回值必须是成功传

       输了的字节数。




 4、read方法

   1）、调用程序对read的返回值解释（P70）

   2）、read代码（P70）




 5、write方法

   1）、调用程序对write返回值的解释（P71）

   2）、write代码（P72）




 6、readv和writev

   1）、readv和writev系统调用这些向量型的函数具有一个结构数组，每个结构包含一个指向缓冲区的指针和一个长度值。readv调用可用于将指定数量的数据依次读入每个缓冲

             区。writev则是把各个缓冲区的内容收集起来，并将它们在一次写入操作中进行传输。

   2）、readv和writev原型如下：

**  ssize_t  (*readv) (struct  file  *filp,  const  struct  iovec  *iov,**

**                                         unisgned  long  count,  loff_t  *ppos);**

**         ssize_t  (*writev) (struct  file  *filp,  const  struct  iovec  *iov,**

**                                        unisgned  long  count,  loff_t  *ppos);**

         I、filp和ppos参数与read和write方法中的用法相同。

         II、iovec结构定义在<linux/uio.h>中，其形式如下：

**       struct  iovec**

**         {               void  _ _user  *iov_base;**

**               _ _kernel_size_t  iov_len;**

**         }**

         每个iovec结构都描述了一个用于传输的数据块——此数据块的起始位置在iov_base（在用户空间中），长度为iov_len个字节。

        III、count参数指明要操作多少个iovec结构。




