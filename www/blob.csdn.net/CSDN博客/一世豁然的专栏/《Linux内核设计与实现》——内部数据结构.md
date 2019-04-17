# 《Linux内核设计与实现》——内部数据结构 - 一世豁然的专栏 - CSDN博客





2014年11月10日 15:59:17[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1210








一、链表

 1、链表是一种存放和操作可变数据元素（通常称为节点）的数据结构。

   1）、链表和静态数组不同在于，它所包含的元素都是动态创建并插入链表的，在编译时不必知道具体需要创建多少个元素。

   2）、另外也因为链表中的内阁元素的创建时间各不相同，所以它们在内存中无需毡筒连续内存区。




 2、单向链表和双向链表

   1）、单向链表

      I、实例。

      II、链表结构体（图6-1）。

   2）、双向链表

      I、每个元素包含一个指向前一个元素的指针，因为他们可以同时向前和向后相互连接的链表。

      II、实例。

      III、链表结构体（图6-2）。




 3、环形链表

   1）、链表中最后一个元素指向链表的首元素，这种链表因为首尾相连，被称为唤醒链表。

   2）、环形链表也存在双向链表（图6-4）和单向链表（图6-3）两种形式。




 4、沿链表移动

   1）、沿链表的移动只能线性移动。先访问某个元素，然后沿元素的向后指针访问下一个元素。如果需要随机访问数据，一般不适用链表。

   2）、有时，首元素会用特殊指针表示——头指针，利用头指针可方便、快速地找到链表的起始端。在飞环形链表里，向后指针指向NULL的元素是尾元素，而在环形链表里向

             后指针指向头元素的元素是尾元素。




 5、Linux内核中的实现

   1）、链表数据结构

      struct  list_head {

                     struct  list_head  *next;

                     struct  list_head  *prev;

      };

      I、使用宏container_of()可以从链表指针找到服结构中包含的任何变量。

   2）、定义一个链表（fox例程）

      I、动态创建例程

      II、静态创建例程。


   3）、链表头

      I、指向真个链表的一个标准的索引指针——即链表的头文件。可以从任何一个节点起遍历链表，直到看到所有节点。

      II、从特殊指针索引到整个链表：

            static  LIST_HEA(fox_list);

   4）、操作链表

      I、向链表中增加一个节点

           list_add(struct  list_head  *new,  struct  list_head  *head);

           该函数向指定链表的head节点后插入new节点。

      II、从链表中删除一个节点

            list_del(struct  list_head  *entry);

            该函数从链表中删除entry元素。

            从链表中删除一个节点并对其重新初始化

            list_del_init();

            list_del_init(struct  list_head  *entry);

      III、移动和合并链表节点

            把节点从一个链表移到另一个链表：

            list_move(struct  list_head  *list,  struct  list_head  *head);

            把节点从一个链表移到另一个链表的末尾：

            list_move_tail(struct  list_head  *list,  struct  list_head  *head);

            检查链表是否为空：

            list_empty(struct  list_head  *head);

            把两个未连接的链表合并在一起：

            list_splice(struct  list_head  *list,  struct  list_head  *head);

            把两个未连接的链表合并在一起，并重新初始化原来的链表：

            list_splice_init();

   5）、遍历列表

       I、基本方法

           使用list_for_each()宏。该宏使用两个list_head类型的参数，第一个参数用来指向当前项，第二个参数是需要遍历的链表的以头节点形式存在的list_head。

       II、可用的方法

            list_for_entry(pos,  head,  member); 

            pos指向包含list_head节点对象的指针，可将它看作是list_entry宏的返回值。

            head指向头节点的指针，即遍历开始位置。

       III、反向遍历链表

            list_for_each_entry_reverse(pos,  head,  member);

       IV、遍历的同时删除

            list_for_each_entry_safe(pos,  next,  head,  member);

       V、其他链表方法







二、队列

 1、队列也称FIFO。Linux内核通用队列实现称为kfifo。




 2、kfifo

   1）、Linux的kfifo两个主要操作：enqueue（入队列）和dequeue（出队列）。

   2）、kfifo对象维护了两个偏移量：入口编译和出口偏移。入口偏移是指下一次入队列时的位置，出口偏移是指下一次出队列时的位置。

   3）、enqueue操作拷贝数据到队列中的入口偏移。dequeue操作从队列中出口偏移处拷贝数据。




 3、创建队列

   1）、首先应该对对队列进行初始化(动态)：

             int  kfifo_alloc(struct  kfifo  *fifo,  unisgned int  size,  gfp_t  gfp_mask);

   2）、自己分配缓冲：

             void  kfifo_init(struct  kfifo  *fifo,  void  *buffer,  unsigne  int  size);

   3）、静态声明kfifo：

             DECLARE_KFIFO(name,  size);

             INIT_KFIFO(name);




 4、推入队列数据

        unsigend  int  kfifo_in（struct  kfifo  *kfifo,  const  void  *from,  unsigned  int  len）;




 5、摘取队列数据

   1）、摘取数据：


       unsigned  int  kfifo_out(struct  kfifo  *kfifo,   void  *from,  unsigned  int  len);

   2）、查看队列数据

       unisgned  int  kfifo_out_peek(struct  kfifo  *kfifo,  void  *to,  unsigned  int  len,  unisgned  offset);




 6、获取队列长度

   1）、获得用于存储kfifo队列的空间的总体大小（以字节为单位）：

             static  inline  unsigned  int  kfifo_size(struct  kfifo  *fifo);

   2）、返回kfifo队列中已推入的数据大小：

             static  inline  unsigned  int  kfifo_len(struct  kfifo  *fifo);

   3）、查看队列中好友多少可用空间：

             static  inline  unsigned  int  kfifo_avail(struct  kfifo  *fifo);

   4）、查看kfifo为空或满：

             static  inline  unsigned  int  kfifo_is_empty(struct  kfifo  *fifo);

             static  inline  unsigned  int  kfifo_is_fulll(struct  kfifo  *fifo);




 7、重置和撤销队列：

   1）、重置队列：

             static  inline  void  kfifo_reset(struct  kfifo  *fifo);

   2）、撤销一个使用kfifo_alloc()分配的队列：

             void  kfifo_free(struct  kfifo  *fifo);




 8、队列使用举例







三、映射

 1、一个映射，也常称关联数据，其实是一个由唯一键组成的集合，而每个键必然关联一个特定的值。这种键到值得关联称为映射。映射至少支持三个操作：

       *  Add  (key,  value)

       *  Remove  (key)

       *  value = Lookup  (key)

   1）、散列表可实现映射。

   2）、映射也可以通过自平衡二叉搜索树存储数据。二叉搜索树同时满足顺序保证，这将给用户的按序遍历带来很好的性能。还有就是不需要散列函数，需要的键类型只要可

             以定义<=操作算子便可以。




 2、Linux内核提供了映射数据结构。但并非一个通用的映射。

   1）、Linux映射的目标是：映射一个唯一的标识符（UID）到一个指针。

   2）、除了实现标准的映射操作以外，Linux还在add操作作基础上实现了allocate操作。这个allocate操作不但可以向map中加入键值对，而且还可产生UID。

   3）、idr数据结构用于映射用户空间的UID。




 3、初始化一个idr

   1）、首先需要静态定义或者动态分配一个idr数据结构。然后调用idr_init():

             void  idr_init(struct  idr  *idp);




 4、分配一个新的UID

   1）、分配新的UID步骤：

       I、告诉idr你需要分配新的UID，允许其在必要时调整后备树的大小。

       II、真正的请求新的UID

   2）、调整后备树大小的方法：

       int  idr_pre_get(struct  idr  *idp,  gfp_t  gfp_mask);

   3）、实际执行获取新的UID，并且添加到idr的方法：

       int  idr_get_new(struct  idr  *idp,  void  *ptr,  int  *id);

   4）、使调用者可指定一个最小的UID返回值：

       int  idr_getnew_above(struct  idr  *idp,  void  *ptr,  int  starting_id,  int  *id);




 5、查找UID

       void  *idr_find(struct  idr  *idp,  int  id);




 6、删除UID

   1）、从idr中删除UID使用方法：

      void  idr_remove(struct  idr  *idp,  int  id);




 7、撤销idr

   1）、撤销idr

      void  idr_destroy(struct  idr  *idp);

   2)、强制删除所有的UID

      void idr_remove_all(struct  idr  *idp);







四、二叉树

 1、树结构是一个能提供分层的树型数据结构的特定数据结构。一个二叉树是每个节点最多只有两个出边的·1树——也就是，一个数，其节点具有0个，1个或者2个子节点。




 2、二叉搜索树

   1）、一个二叉搜索树（BST）是一个节点有序的二叉树，其顺序通常遵循下列法则：

      I、根的左分支节点值都小于根节点值。

      II、右分支点值都大于根节点。

      III、所有的子数也都是二叉搜索树。




 3、自平衡二叉搜索树

   1）、一个节点的深度是指从其根节点起，到达它一共需经过的父节点数目。处于数底层的节点（再也没有子节点）称为叶子节点。一个树的高度是指树中的处于最底层节点

             的深度。

   2）、一个平衡二叉树是一个所有叶子节点深度不超过1的二叉搜索树。一个自平衡二叉搜索树是指其操作都试图位置（半）平衡的二叉搜索树。

   3）、红黑树

      I、红黑树是一种自平衡搜索树。Linux的主要自平衡二叉树数据结构就是红黑树。

      II、红黑树遵循六个属性

         所有的节点要么着红色，要么着黑色。

         叶子节点都是黑色。

         叶子节点不包含数据。

        所有非叶子节点都有两个子节点。

        如果一个节点是红色，则它的子节点都是黑色。

        在一个节点到其叶子节点的路径中，如果总是包含同样数目的黑色节点，则该路径相比其他路径是最短的。

   4）、rbtree

      I、Linux实现的红黑树称为rbtree。

      II、rbtree的根节点由数据结构rb_tree描述。创建一个红黑树要分配一个新的rb_tree结构，并且需要初始化为特殊值RB_ROOT。

      III、rntree的实现并没有提供搜索和插入例程，这些例程希望由rbtree的用户自己定义。







五、数据结构以及选择

 1、使用链表

   1）、如果对数据集合的主要操作是遍历数据。

   2）、另外，当性能并非主要考虑因素，或者当你需要存储相对较少的数据项时，或者当你需要和内核中其他使用链表的代码交互时，也该优先选择链表。

   3）、需要存储一个不大不明的数据集合。





 2、如果代码符合生产者/消费者模式，则使用队列




 3、如果需要存储大量数据，并且检索迅速，那么红黑树最好。







六、算法复杂度

 1、算法

   1）、算法就是一系列的指令，它可能有一个多个输入，最后产生一个结果或输出。




 2、大O符号

   1）、渐进表示法就是上限——它是一个函数，其值从一个起始点之后总是超过我们所研究的函数的值，也就说上限增长等于或者快于我们研究的函数。一个O符号用来描述这

             种增长率。




 3、大Θ符号




 4、时间复杂度（表6-1）







