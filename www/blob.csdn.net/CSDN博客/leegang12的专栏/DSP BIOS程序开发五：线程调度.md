# DSP/BIOS程序开发五：线程调度 - leegang12的专栏 - CSDN博客
2013年12月22日 22:33:34[leegang12](https://me.csdn.net/leegang12)阅读数：1645
【声明：内容源于网络】
[http://blog.csdn.net/ghjk014/article/category/849797](http://blog.csdn.net/ghjk014/article/category/849797)
[http://blog.csdn.net/zzsfqiuyigui/article/details/6940795](http://blog.csdn.net/zzsfqiuyigui/article/details/6940795)
一. DSP/BIOS的高级应用
       DSP在同一时间执行多个任务，当外部数据变为有效或者出现控制信号时，对外部事件进行响应。通常将这些任务称为线程，DSP/BIOS采用广义的定义：DSP执行的所有独立的指令流。一个线程是一个单独的控制点，可能包含一个字程序、一个宏或者是一个函数调用。
二、四种线程类型：HWI，SWI，TSK，IDL
怎样选择：
1：严格的实时性
如果线程的执行需要严格的实时性，而线程执行需要的时间又很少时，可以使用硬件中断或者时钟函数完成硬件中断线程将由外部的同步事件(如A/D转换器的中断信号)触发而执行。HWI函数或中断服务子程序将在中断发生后执行。DSP/BIOS的应用程序中，HWI中断函数冷处理频率为200KHz的中断(这是一个概数),处理时间在2-5us之间时，使用硬件中断线程。
硬件中断线程有极小的中断潜伏期(中断触发到中断服务子程序的第一条指令开始执行的时间)
硬件中断线程推荐使用汇编语言。
硬件中断处理函数中，可以将SWI软件中断对象或TSK任务对象放到执行队列里面，但它们必须等到所有的
硬件中断线程结束后才有效，所以要使HWI硬件中断函数尽快结束。
硬件中断函数在调用某些PIP管道模块的API函数时，PIP_alloc,PIP_free,PIP_get,PIP_put等，读写通知函数也将在中断响应中调用执行。
2：部分实时性
SWI软件中断或TSK任务线程来完成一些非实时性的处理任务。
二者有如下特点：能完成实时的处理任务但允许处理时间相对较长、允许被其他线程抢先。
如果有以下情况之一，使用TSK：
处理函数需要等待某些资源，以便继续运行
处理函数与其它线程之间有复杂的联系或数据共享要求
处理函数有自己的堆栈空间而不是用系统的堆栈空间
处理函数用到LCK、MBX、SEM等内核模块
处理函数在创建、删除、退出、就绪、切换时调用钩子函数
因为：
任务可以被挂起，等待条件满足而继续运行，而SWI不能；
任务之间通信和同步有SEM、MBX、LCK等，SWI没有；
任务使用自己的堆栈，而SWI使用系统堆栈；
任务可调用钩子函数
软件中断：
往往伴随着硬件中断的发生。一般地若在硬件中断服务函数中调用SWI_post函数，产生软件中断。
其适合处理一些发生速率较低的任务，对实时性要求不苛刻的任务。SWI可以帮助HWI将一些非严格实时性
的处理放到低优先级的线程中。(注意：硬件中断响应过程中，系统关中断)
由于软件中断不能挂起，所以软件中断处理之前必须所有的数据准备好，用邮箱判断。
3：周期性的服务
周期性地或在固定的时间间隔内完成处理任务，一般地，时间间隔比处理任务所需的时间长得多，就用PRD。
周期性函数属于SWI软件中断的PRD_swi对象，所有周期函数具有同样的SWI软件中断优先级，所以周期函数不能
抢先于其它周期函数。
假若其它的SWI或者TSK处理时间较长，此时需要提高整个PRD周期函数的优先级。
多个周期函数如果被同一个系统时钟触发，那么执行顺序是按照创建时候的顺序执行。
4：不需要实时性
收集统计数据，与自己交换检测数据，用IDL线程。
IDL线程当主函数返回之后，DSP/BIOS内核调用该应用程序所用到的DSP/BIOS模块的初始化启动代码。结束后，
进入IDL等待循环，不停地依次调用IDL后台对象中的所有函数。
三、任务的通信和同步 MBX邮箱管理模块
概述：MBX邮箱模块使用一主函数通过邮箱对象的句柄管理邮箱的访问。可配置邮箱能容纳的信息数量。
邮箱大小的单位是：字
(1)MBX_Handle mbx = MBX_create(Uns msgsize, Uns mbxlength, MBX_Attrs * attrs)
信息大小、邮箱长度、目前邮箱参数没有定义(保留)，此函数将调用MEM_alloc函数创建邮箱数据结构
struct MBX_Attrs{
int segid; // default is 0
}
(2)void MBX_delete(MBX_Handle mbx)
删除指定邮箱，调用MEM_free释放存储空间
(3)bool status = MBX_pend(MBX_Handle mbx, Ptr msg, Uns timeout)
msg是存放邮箱信息的指针，如果等待时间大于timeout则返回。如果邮箱中有信息，此函数将复制第一条信息
到msg所指的存储空间，返回TRUE，否则函数挂起当前任务，直到超时或调用MBX_post函数。
如果timeout取SYS_FOREVER，当前任务会一直挂起，直到MBX_post被调用，若timeout为0，直接返回。
(4)bool status = MBX_post(MBX_Handle mbx, Ptr msg, Uns timeout)
此函数在将信息写入邮箱之前，需要检查邮箱是否有容纳新信息的空间，若有，写入并返回。调用函数时，
若有更高优先级的任务就绪，或者邮箱已满且timeout不为0，则任务切换。
同理，如果timeout取SYS_FOREVER，当前任务会一直挂起，直到MBX_pend被调用，若timeout为0，直接返回。
三、任务的通信和同步 SEM旗语管理模块
概述：SEM旗语模块使用一组函数通过旗语对象的句柄来管理旗语的使用。DSP/BIOS内核提供的旗语是信号量，借
此实现线程的同步和相互作用。
(1)int count = SEM_count(SEM_Handle sem)
返回sem制定的旗语计数器的当前值
(2)int count = SEM_create(int count, SEM_Attrs * attrs)
传入初始的旗语信号量计数值和属性参数(目前无定义，保留)成功则返回对象句柄，否则返回NULL
(3)void SEM_delete(SEM_Handle sem)
删除指定的旗语，并且调用MEM_free函数释放空间
(4)void SEM_ipost(SEM_Handle sem)
使得处于等待旗语的任务由阻塞状态(Blocked)变为就绪状态(Ready).如果没有等待旗语的任务，函数仅仅
对信号旗语计数器加1并返回。
此函数类似于SEM_post函数，一般地在SWI或者HWI中使用SEM_ipost函数，在任务线程中使用SEM_post函数。
(5)void SEM_new(SEM_Handle sem, int count)
初始化指定旗语对象的计数器，只能用于静态创建的旗语计数器进行初始化，调此函数不发生任务切换。
(6)bool status = SEM_pend(SEM_Handle sem, Uns timeout)
如果旗语计数器大于0，此函数对旗语减1返回TRUE，否则会暂停当前任务的运行，直到该函数的旗语达到。
在timeout时间之后，暂停的任务会变为就绪，若timeout等于SYS_FOREVER，则必须有SEM_post函数才能取消。
若超时，函数返回FALSE。若旗语计数器为0，而超时参数不为0，则任务切换
(7)void SEM_post(SEM_Handle sem)
类似于SEM_ipost函数。
(8)void SEM_reset(SEM_Handle sem, int count)
复位旗语计数器并重新开始计数，调此函数不发生任务切换。
四、任务的通信和同步 QUE队列管理模块
概述：QUE模块通过队列句柄的访问来管理一系列队列操作函数。
每个队列包含0个或者多个有序的元素项，其中每个元素项都是一个结构体变量。
它的第一个成员是类型为QUE_Elem的变量，该结构体成员用作内部指针。
(1)QUE_Handle queue = QUE_create(QUE_Attrs * attrs)
队列属性参数目前保留。成功返回新队列对象句柄，失败返回NULL。
(2)void QUE_delete(QUE_Handle queue)
删除队列
(3)Ptr elem = QUE_dequeue(QUE_Handle queue)
删除队列最前面的元素项并返回该项的指针，此指针是一个指向结构体的指针，该结构第一个成员必须是
QUE_Elem类型的成员。
注意：多任务共享队列时：使用QUE_get函数，此函数取元素时禁止中断。
(4)bool empty = QUE_empty(QUE_Handle queue)
判定队列是否为空
(5)void QUE_enqueue(QUE_Handle queue, Ptr elem)
在队尾插入一个元素项，参数elem是一个指向结构体的指针。
注意：多任务共享队列时：使用QUE_put函数，此函数取元素时禁止中断。
(6)void * elem = QUE_get(QUE_Handle queue)
如果队列不为空，则此函数删除最前面元素项，并返回指向其的指针，如果队列为空，返回此队列本身。
判定队列是否为空的方法：
if( (QUE_Handle)(elem = QUE_get(q))!=q )//队列非空
(7)QUE_Elem * elem = QUE_head(QUE_Handle queue)
返回一个指向队列中最靠前元素的指针，队列为空，返回此队列本身。
(8)void QUE_insert(Ptr qelem, Ptr elem)
在原队列的qelem前面插入新元素项elem，多任务共享队列时，此函数应和一些避免冲突的函数配合使用。
(9)void QUE_new(QUE_Handle queue)
初始化指定的队列对象，使队列变空。
当使用变量说明方法静态创建队列时，初始化此队列。若队列原来为空，其元素不被处理，而是遗弃。
(10)Ptr elem = QUE_next(Ptr qelem)
返回元素qelem的下一个元素项的指针，多任务共享队列时，此函数应和一些避免冲突的函数配合使用。
(11)Ptr elem = QUE_prev(Ptr qelem)
返回元素qelem的前一个元素项的指针，多任务共享队列时，此函数应和一些避免冲突的函数配合使用。
(12)void QUE_put(QUE_Handle queue, void * elem)
在队尾添加元素项，自动禁止中断
(13)void QUE_remove(Ptr qelem)
删除队列中的元素项，由于队列是双向链表，所以不要删除头结点。
