# （多核DSP快速入门）9.IPC的堆模块HeapMP和链表模块ListMP - tostq的专栏 - CSDN博客





2016年06月29日 23:25:50[tostq](https://me.csdn.net/tostq)阅读数：3044
所属专栏：[多核DSP快速入门](https://blog.csdn.net/column/details/easymulticoredsp.html)









       本节主要介绍下IPC的其他几个模块，主要包含

（1）Heap*MP堆模块

（2）ListMP链表模块

（3）GateMP门模块

（4）MultiProc多核管理模块


**一、Heap*MP模块**

Heap*MP 模块主要有三类模块：

HeapBufMP：固定大小的内存管理器，其分配的所有缓冲区都是一样的，当然也可以通过不同HeapBufMP实例来管理不同的大小的缓冲区。

HeapMultiBufMP：每个HeapMultiBufMP支持8个不同大小的缓冲区。当一个分配需求被发送，HeapMultiBufMP的实例从不同大小的待分配缓冲区中，选择一个能满足要求的最小缓冲区。如果待分配缓冲区为空，那么这个分配就失败了。

HeapMemMP：这是个能分配变长大小的内存管理器。另外HeapMemMP管理共享内存区（Shared memory）的一个缓冲区。




（1）动态创造一个Heap*MP实例

使用Heap*MP_create()函数可以动态地创造一个Heap*MP实例，如同其他IPC模块一样，在创建实例之前，需要初始化参数Heap*MP_Params结构。当创建实例时，共享内存也会被初始化，而实例对象本身却在本地内存中被创建，只有实际缓存和一些共享信息仍残存在共享内存中。

Heap*MP_Params结构的相关参数设置

regionID：分配共享内存的共享区域的序号

name：Heap实例的名字

align：每个块的对齐参数

numBlocks：固定大小块的数目

blockSize：实例中块的大小

gate：用于保护上下文的多处理门

exact：只有当请求分配的大小同块精确匹配时，对分配一个块




（2）打开一个Heap*MP实例

一旦一个Heap*MP实例在一个处理上被创建，那么通过对应相同的共享实例的本地句柄，另一个处理器也可以打开这个Heap，这个远程处理器通过Heap*MP_open()来打开实例。利用NameServer实例，远程处理器通过Heap*MP的名称来区分不同实例，并找到Heap实例地址

Heap*MP_open()通过Heap名称（Heap*MP_Params中设置）来找到Heap*MP实例，并将其对应一个本地的Heap*MP句柄。




（3）关闭一个Heap*MP实例

通过Heap*MP_close()来释放存储在本地内存的Heap*MP实例，只有当前线程已经打开Heap*MP实例并且其他线程没有调用Heap*MP_delete()时，才能使用Heap*MP_close()函数




（4）删除一个Heap*MP实例

创造Heap*MP实例的线程可能通过Heap*MP_delete()函数来释放存储在本地内存中的Heap*MP对象，并标明此Heap所在共享内存区不再被初始化。由此可以看出，delete()函数是同create()函数对应的，而close()函数是同Open()函数对应的




（5）从堆中分配内存

HeapBufMP_alloc函数可以从堆Heap空闲表中获得第一个缓冲区

而HeapMultiMP_alloc函数从不同大小的待分配缓冲区中，选择一个能满足要求的最小缓冲区。

HeapMemMP_alloc函数可以从堆中分配一个请求大小的缓冲区

对于所有的三类函数，消息的高速缓存一致性都是通过共享区域模块（ShareRegion）来管理的

通过Heap*MP_free()来释放三类函数分配的内存

同Heap*MP实例不同，从堆中分配内存是统一保存在共享区域中的堆中，所以内存分配和释放不一定需要在同一个线程中完成，而Heap*MP实例是保存在本地处理器中的，所以其创建和删除是在同一个处理器线程中完成的。




**二、GateMP模块**

GateMP模块是用于本地和远程处理器的上下文保护的，进入一个GateMP模块，可以避免在同一处理器其他线程抢占，同时也避免了远程处理器进入相同的门。GateMP模块通常的用法是用于保护对于共享资源的读写操作。

（1）动态创建GateMP实例

首先需要初始化GateMP实例参数GateMP_Params

a). 远程处理器保护方式设置：remoteProtect选项可以设置为：

GateMP_RemoteProtect_NONE：只创建本地门保护

GateMP_RemoteProctect_SYSTEM：（默认）

b). 本地处理理器保护方式设置：localProtect选项可以设置为：

GateMP_LocalProtect_NONE：不支持任何的本地上下保护，一般用于单线程的本地应用。

GateMP_LocalProtect_INTERRUPT：使用SYS/BIOS GateHwi来实施，由此来避免硬件中断

GateMP_LocalProtect_TASKLET：使用SYS/BIOS GateSwi来实施，由此来避免软件中断

GateMP_LocalProtect_THREAD：使用SYS/BIOS GateMutexPri（基于信号量模块的）来实施


c). name：名称

d). regionID：分配Gate实例的共享区域SharedRegion的ID




（2）打开Gate实例

一旦处理器创建一个GateMP实例，其他处理器可以一个指向相同实例的本地句柄来找开这个实例了

GateMP模块也是通过NameServer实例来允许一个远程处理器来指向本地Gate实例

关闭实例是利用GateMP_close()实例的




（3）进入GateMP实例

通过GateMP_enter()来进入一个门，这个函数进入了调用线程的本地门，当进入该门后，该本地门那被阻塞，如果此门已被远程处理器进入，那么GateMP_enter函数将自旋，直到远程处理器离开此门。

GateMP_enter()返回一个“key”，而这个"key"将被GateMP_leave()用于离开此门




**三、List模块**

这个模块是用于支持链表对象而设置的，这个链表被设置为双链表形式，所以链表中的元素，可以从链表中的任意上被插入或删除，另外链表没有最大大小的限制。

（1）链表中先入先出设置

要组成一个链表，首先需要一个链表元素类型List_Elem，另外还需要一个头，通过List_put()来在链表尾上增加元素，而通过list_get()来移除并返回链表头上的元素，这个一个典型的先入先出的队列

![](https://img-blog.csdn.net/20160629232211277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




（2）链表中的迭代

List_next()下一个元素

List_prev()上一个元素

使用：

![](https://img-blog.csdn.net/20160629232232058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）插入和删除链表元素

![](https://img-blog.csdn.net/20160629232259544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


另外一种后入先出的栈结构方法是利用：List_putHead()和List_get()函数

![](https://img-blog.csdn.net/20160629232319670?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**四、MultiProc模块**

在多核环境下，许多IPC模块都需要区分当前处理器，而MultiProc模块就是用于管理处理器ID的

（1）设置处理器ID

静态设置（在.cfg文件中设置）：MultiProc.setConfig();

动态设置：MultiProc_setLocalId(Id);

（2）获得ID

MultiProc_getId("name")根据处理器的名称获得Id


MultiProc_self()返回当前处理器Id

MultiProc_getname(Id)获得处理器名称


（3）不同类处理器：这里的类指的是系统中共享同样内存的一组处理器，如果系列中有多组处理器芯片，可以通过MultiProc.baseIdOfCluster及MultiProc.numProcessors来设置不同处理器芯片，比如如果我们有两个C6678芯片，每个芯片都有8个核心，我们可以如下设置

![](https://img-blog.csdn.net/20160629232410825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


而Ipc_start()及Ipc_attach()只能用于连接或同步相同类的处理核心

因此为创建一个在多个类（clusters）之间的核连接，我们可以人工地利用MessageQ和NameServerMessageQ模块来创建，NameServerMessageQ模块支持不同类之间通过MessageQ来进行NameServer请求，

要创建一个不同类之间的核链接，允许调用MessageQ_open()函数，一旦两个核心之间的消息传递被建立，MessageQ可以在两个不同类的核心间使用



