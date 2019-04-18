# 《Linux程序设计》——数据管理 - 一世豁然的专栏 - CSDN博客





2014年09月25日 21:30:31[一世豁然](https://me.csdn.net/Explorer_day)阅读数：892








一、内存管理

 1、简单的内存分配

**  #include<stdlib.h>**

**     void  *malloc(size_t  size);**

     I、可以利用以上函数来分配内存。

     I、代码（P215）。





 2、分配大量内存（代码P216-217）。




 3、滥用内存（代码P219）。




 4、空指针（代码P219-220）。




 5、释放内存

** #include<stdlib.h> **

**     void  free(void  *ptr_to_memory);**

     I、调用frre时使用的指针参数必须是指向由malloc、calloc或realloc调用所分配的内存。




 6、其他内存分配函数

** #include<stdlib.h>**

**    void  *calloc(size_t  number_of_elements,  size_t  element_size);**

**    void  *realloc(void  *existing_memory,  size_t  new_size);**

    I、calloc函数的作用为一个结构数组分配内存，因此需要把元素个数和每个元素的大小作为指针，它所分配的内存将全部初始化为0。后续调用calloc无法保证能返回一个连

        续的空间。

    II、realloc函数用来改变先前已经分配的内存长度。它需要传递一个指向先前通过malloc，calloc或realloc调用分配的内存的指针，然后根据new_size参数的值来添加或减少

         其长度。如果无法调整内存块大小，会返回一个空指针null。







二、文件锁定

 1、文件锁定的方法

  1）、原子操作

  2）、允许锁定文件的一部分，从而可以独享对这一部分内容的访问。




 2、锁文件只是建议锁，而不是强制锁，在后者中，程序将强制锁的行为。




 3、可以利用fcntl.h头文件中定义的open系统调用，并带上O_CEAT和0_EXCL标志以原子操作（代码P223）额方式在进入临界区之前创造锁文件，然后再退出临界区用unlink

       系统调用删除锁文件（代码P224）。




 4、区域锁定

   1）、文件段锁定或文件区域锁定：文件中的某个特定部分被锁定，但其他程序可以访问这个文件中的其他部分。

   2）、实现方式：fcntl系统调用和lockf调用。fcntl和lockf的锁机制不能同时工作：它们使用不同的底层实现，因此绝不要混合使用这两种类型的调用。

   3）、fcntl函数

**  #include<fcntl.h>**

**      int  fcntl(int  fildes,  int  command, . . .);**

      I、fcntl对一个打开的文件描述符进行操作，并能根据comman参数的设置完成不同的任务。

      II、command参数：F_GETLK，F_SETLK，F_SETTLKW。

      III、但使用command参数选项时fcntl的第三个参数必须指向flock结构的指针，所以·应该使用如下函数。

**     int  fcntl(int  fildes,  int  command,  struct  flock  *flock_structure);**

      I、flock结构成员：short  l_type,  short  l_whence,  off_t  l_start,  off_t  l_len,  off_t  l_pid。（P227  表7-2）

        A、I_type:（P226  表7-1）

        B、I_whence、I_start、I_len成员定义了文件中的一个区域，即一个连续的字节集合。

        C、I_pid参数用来记录持有锁的进程。




 5、锁定状态下的读写操作

  1）、使用fcntl锁定文件。（代码P228-229）

  2）、测试文件上的锁。（代码P230-231）




 6、文件锁的竞争

  1）、文件锁的竞争。（代码P233-234）




 7、其他锁命令

**#include<unistd.h>**

**    int  lockf(int  fildes,  int  function,   off_t  size_to_lock);**

    I、function参数取值：

      A、F_ULOCK（解锁）。

      B、F_LOCK（设置独占锁）。

      C、F_TLOCK（测试并设置独占锁）。

      D、F_TEST（测试其他进程设置的锁）。

   II、size_to_lock参数是操作的字节数，它从文件的当前偏移值开始计算。




 8、死锁

   1）、两个或多个程序彼此相互等待都无法继续执行下去的情况。

   2）、死锁的避免：两个或多个程序只需要使用相同的顺序来锁定它们需要的字节或规定一个更大的区域即可。







三、数据库

 1、dbm数据库的编译

   1）、情况一

          I、在C源文件中包含头文件ndbm.h。

          II、使用编译选项-I/usr/inlcude/gdbm包含目录/usr/include/gdbm。

         III、使用编译选项-lgdbm链接gdbm库。

    2）、情况二

          I、在C源文件中包含头文件gdbm-ndbm.h而不是ndbm.h。

          II、使用编译选项-I/usr/inlcude/gdbm包含目录/usr/include/gdbm。

         III、使用编译选项-lgdbm_compat  --lgdbm链接其他的gdbm兼容库。




 2、datnum数据类型

   1）、成员：void  *dptr；  size_t  dsize；datnum是一个用typedef语句定义的类型。

   2）、在使用dbm库时，如果要引用一个数据块，必须声明一个datnum类型的变量，将成员dptr指向数据的起点，并把成员dsize设为包含数据的长度。




 3、当打开一个数据库时，通常会创建两个物理文件，它们的后缀分别是.pag和.dir，并返回一个dbm指针，它被用来访问这两个文件。




 4、dbm访问函数

**#include<ndbm.h>**

**    DBM  *dbm_open(const  char  *filename,  int  file_open_flags,  mode_t  file_mode);**

**    int  dbm_store(DBM  *database_descriptor,  datum  key,  datnum  content,  int  store_mode);**

**    datum  dbm_fetch(BM  *database_escriptor,  datum  key);**

**    void  dbm_close(DBM  *database_descriptor);**

    1)、dbm_open函数

         I、此函数用来打开已有额数据库，也可以用来创建新数据库。filename参数是一个基本文件名，它不含.pag和.dir后缀。其余的参数与open系统调用函数的第二个和第三

              个参数一样。

         II、dbm_open返回一个指向DBM类型的指针。它被用于所有后续对数据库的访问。如果失败，它将返回（DBM *）0。


    2)、dbm_store函数

        I、此函数把数据存储到数据库中。为了定义想要存储的数据和用来引用它的索引。必须设置两个datum类型的参数：一个用于引用索引，一个用于实际数据。

        II、参数store_mode用于控制当试图以一个已有的关键字来存储数据时会发生的情况。如果它被设置为dnm_insert，存储操作将失败并且dbm_store返回1.如果它被设置

              为dbm_replace，则新数据将覆盖已有数据并且dbm_store返回0.当发生其他错误时，dbm_store将返回一个负值。


    3)、dbm_fetch函数

        I、此函数用于从数据库中检索数据。它使用一个先前dbm_open调用返回的指针和一个指向关键字的datum类型结构作为其参数。

        II、此函数返回一个datum类型的结构。如果在数据库中找到与这个关键字关联的数据，返回的datum结构的dptr和size成员的值将被设置为相应数据的值。如果没有找到关

              键字，dptr将被设置为null。


    4)、此函数关闭dbm_open打开的数据库。它的参数是先前dbm_open调用返回的dbm指针。




 5、其他dbm函数

** #include<ndbm.h>**

**    int  dbm_delete(DBM  *database_descriptor,  datum  key);**

**    int  dbm_error(DBM  *database_descriptor);      **

**    int  dbm_clearerr(DBM  *database_descriptor);**

**    datum  dbm_firstkey(DBM  *database_descriptor);**

**    datum d  bm_nextkey(DBM  *database_descriptor);**

    1）、dbm_delete函数——用于从数据库中删除数据项。它在成功时返回0。

    2）、dbm_error函数——用于测试数据库中是否有错误发生，如果没有就返回0。

    3）、dbm_clearerr函数——用于清除数据库中所有已被置位的错误发条件标志。

    4）、dbm_firstkey和dbm_nextkey函数——一般成对使用来对数据库中的所有关键字进行扫描。它需要的循环结构如下：

**  DBM  *dbm_ptr;**

**           datum  key;**

**           for(key = dbm_firstkey(db_ptr);  key.dptr;  key = dbm_nextkey(db_ptr))**




