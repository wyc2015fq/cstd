# 《Linux设备驱动程序》——内核的数据类型 - 一世豁然的专栏 - CSDN博客





2014年11月21日 11:39:17[一世豁然](https://me.csdn.net/Explorer_day)阅读数：693








一、概论

  1、内核使用的数据类型主要分为三大类：

    1）、类似int这样的标准C语言类型。

    2）、类似u32这样的有确定大小的类型。

    3）、向pid_t这样的用于特定内核对象的类型。







二、使用标准C语言类型

  1、尽管大多数程序员习惯自由使用C标准类型，但编写设备驱动程序时需要小心，以免类型冲突和潜在的代码缺陷。但在某些情况下，我们不能使用C标准类型，因为在不同的

        体系架构上，普通C语言的数据类型所占空间的大小并不相同。




  2、尽管在混合使用不同数据类型时必须小心，但有的时候有理由这样做。这样的一种情况是内存地址，只要一涉及到内核，内存地址就变得很特殊。内核中的普通地址通常是

        unsigned  long，这里用了如下事实：至少在当前Linux支持的所有平台上，指针和long整型的大小总是相同的。







三、为数据分配确定空间的大小

  1、有时候代码需要特定大小的数据项，多半是用来匹配预定义的二进制结构，或者和用户空间进行通信，或者通过在结构中插入“填白”字段来对其数据。




  2、当需要知道自己的数据大小时，内核提供了下列数据类型。所有的这些类型在头文件<asm/types.h>中声明，这个文件又被头文件<linux/types.h>包含：

        u8;                      /*无符号字节（8位）*/


        u16;                  /*无符号字（16位）*/


        u32;                 /*无符号32位值*/


        u64;               /*无符号64位值*/

    1）、相应的有符号类型也存在，但是几乎没用。如果需要他们的话，只需将名字中的u用s替换就行。

    2）、如果一个用户空间程序需要使用这些类型，它可以在名字前加上两个下划线作为前缀。

    3）、以上这些结构是Linux所特有的，使用它们将阻碍软件向其他UNIX变种的移植。




  3、内核使用传统类型，这通常用于其大小与体系结构无关的数据项。这种做法通常是为了保持向后兼容性。







四、接口特定的类型

  1、内核中最常用的数据类型由他们自己的typedef声明，这样可防止出现任何移植问题。“接口特定”是指由某个库定义的一种数据类型，以便为某个特定的数据结构提供接口。

    1）、即使没有定义接口特定的类型，也应该始终使用和内核其余部分一致的，适当的数据类型。




  2、完整的_t类型在<linux/types>中定义，但很少使用这个清单。

    1）、当需要某个特定类型时，可在所需调用的函数原型或者所使用的数据结构中找到这个类型。

    2）、只要驱动使用了程序使用了需要这种“定制”类型的函数，但又不遵守约定的时候，编译器就会产生警告，如果使用WALL编译选项并且精心消除有的警告，就可以驱动代

              码是可移植的。

    3）、_t数据项的主要问题是在我们需要打印它们的时候，不太容易选择正确的printk或者printf的输出格式，并且在一种体系上排除的警告，在另一个体系上可能还会出现。

    4）、当需要打印接口特定的数据类型时，最有效的办法是将其强制转换成可能的最大类型，然后用相应的格式打印。

    5）、尽管_t类型在大多数情况下是正确的解决方案，但有时正确的类型并不存在。这发生在一些还没有被整理的旧接口上。










五、其他有关移植性的问题

 一）、时间间隔

  1、在处理时间间隔的时候，不要假定每秒有100个jifies。




  2、在使用jiffies计算时间间隔的时候，应该用HZ来衡量。







 二）、页大小

  1、使用内存时，要记住内存页的大小为PAGE_SIZE字节，而不是4KB。




  2、在已支持的平台上，页大小范围是从4KB到64KB，有时候他们在相同平台上的不同实现上也是不一致的。这涉及到的宏是PAGE_SIZE和PAGE_SHIFT。







 三）、字节序

  1、不要做字节序的假设。

    1）、尽管PC是按照先是低字节的方式存储多字节数值的，但某些平台上是以高字节的方式工作的。

    2）、只要可能，就应该将代码编写成不依赖于所操作数据的字节序的方式。




  2、头文件<asm/byteorder.h>定义了_ _BIG_ENDLAN或者_ _LITTLE_ENDLAN，取决于处理器的字节序。




  3、Linux定义了一组宏，它可以在处理字节序和特殊字节序之间进行转换。

        u32  cpu_to_le32(u32);

        u32  le32_to_cpu(u32);

    1)、类似的例程有许多，它们都定义在头文件<linux/byteorder/big_endian.h>和<linux/byteorder/little_endian.h>中看到完整的列表。







 四）、数据对其

  1、编写可移植代码的最后一个值得关注的问题是如何访问未对齐的数据。大部分现代体系架构在每次程序试图传输未对齐的数据时都会产生一个异常，这时，数据传输会被异

        常处理程序处理，因此会带来大量性能损失。




  2、如果需要访问未对其的数据，则应该使用下面的宏：

        #include<asm/unaligned.h>

        get_unaligned(ptr);

        put_unaligned(val,  ptr);




  3、另一个关于对齐的问题是数据结构的跨平台可移植性。

    1）、同样的数据结构在不同平台上可能会被编译成不同的布局。

    2）、编译器来根据平台的习惯来对齐数据结构的字段，而不同的平台习惯是不同的。




  4、为了编写可以在不同平台之间可移植的数据项的数据结构，除了规定特定的字节序以外，还应该始终强制数据项的自然对齐。




  5、最后要注意编译器本身也许会悄悄往结构体中插入填充数据，来确保每个字段的对齐可以在目标处理器上取得好的性能。







 五）、指针和错误值

 1、在大部分情况下，失败是通过返回一个NULL指针来表示的。这种技巧是有用的，但他不能传递问题的确切性质。某些接口确实需要返回一个错误编码，以使调用者根据实

       际出错的情况做出正确的选择。




  2、许多内核接口通过吧错误值编码到一个指针值中来返回错误的信息。这种函数必须小心使用，因为它们的返回值不能简单和NULL比较。为了帮助创建和使用这种类型的接

        口，<linux/err.h>中提供了一小组函数。

    1）、返回指针类型的函数可以通过如下函数返回一个错误值：

              void  ERR_PTR(long  error);

    2）、调用者可以使用下列函数来检查返回的指针是否是一个错误码：

              long  IS-ERR(const  void  *ptr);

    3）、如果需要实际的错误代码，可以使用如下函数把它提取出来：

             long  PTR-ERR(const  void  *ptr);










六、链表

  1、当使用这些链表时，应该始终牢记这些链表函数不进行人恶化锁定，如果你的驱动程序有可能试图对同一个链表执行并发操作的话，则有责任实现一个锁方案。否则，崩溃

        的链表结构体，数据丢失，内核混乱等问题是很难诊断的。





  2、为了使用链表机制，驱动程序必须包含头文件<linux/list.h>。该文件定义了一个简单的list_head类型的结构体。

        struct list_head {

             struct  list_head  *nest,  *prev;

        };




  3、在使用之前，必须初始化链表头：

    1）、INIT_LIST_HEAD初始化

              struct  list_head  todo_list;

              INIT_LIST_HEAD(&todo_list);

    2）、可以在编译时初始化：

              LIST_HEAD(todo_list);







  4、头文件<linux/list.h>中声明了下列操作链表的函数：

      I、list_add(struct  list_head  *new,  struct  list_head  *head);

      II、list_add_tail(struct  list_head  *new,  struct  list_head  *head);

      III、list_del(struct  list_head  *entry);

             list_del_init(struct  list_head  *entry);

      IV、list_move(struct  list_head  *entry,  struct  list_head  *head);

             list_move_tail(struct  list_head  *entry,  struct  list_head  *head);

       V、list_empty(struct  list_head  *head);

       VI、list_splice(struct  list_head  *list,  struct  list_head  *head);

       VII、list_entry(struct  list_head  *ptr,  type_of_struct,  field_name);




  5、list_entry函数变体

     I、list_for_each(struct  list_head  *curser,  struct  list_head  *list);

     II、list_for_each_prev(struct  list_head  *curser,  struct  list_head  *list);

     III、list_for_each_safe(struct  list_head  *cursor,  struct  list_head  *next,  struct  list_head  *list);

     IV、list_for_each_entry(type  *curser,  struct  list_head  *list,  member);

            list_for_each_entry_safe(type  *curser,  type  *next,  struct  list_head  *list,  member);




