# （多核DSP快速入门）7.利用SharedRegion的核间内存共享 - tostq的专栏 - CSDN博客





2016年05月06日 22:14:56[tostq](https://me.csdn.net/tostq)阅读数：5680
所属专栏：[多核DSP快速入门](https://blog.csdn.net/column/details/easymulticoredsp.html)








原创文章
转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)

目录：[http://blog.csdn.net/tostq/article/details/51245979](http://blog.csdn.net/tostq/article/details/51245979)




       本来这一节是讲多核图像灰度转换的例程，不过内容太多，就拆开成两节，多核DSP的内容主要在单核的基础上增加了核间内存共享及核间通信，上一节介绍了一个利用Notify通知机制的简单核间通信方法，这一节我们将介绍利用SharedRegion模块进行核间内存共享的方法。而下一节将是通过MessageQ模块来进行复杂的消息传递方法。

本节仍然以一个简单的例子来结合上节的Notify模块来介绍SharedRegion模块




**一、新建项目**
前面已经介绍如何新建CCS项目，这里就不再详细介绍了，不过比较推荐方式是先导入IPC模板例子，然后在模板上进入修改，这样减少了大量配置SYS/BIOS的时间。

本文的例子主要做了以下几个任务：

（1）核0创建共享内存，将数据写入内存，然后通过Notify将内存地址发送给从核。

（2）从核（1至7核）接收核0的通知，打开内存地址，读取数据。

（3）完成。






**二、导入SharedRegion模块**


SharedRegion模块是IPC从名字上就可以看出，其是一个共享区域，特别是对于多处理器环境下，SharedRegion模块就是用于让一个内存区域能被不同处理器共享并操作。这个模块会给每个处理器上创建一个共享内存区域查找表，这个查找表保证各个处理器能查看到系统内的所有共享区域。查找表中共享内存区域在所有查找表中的区域ID是一致的，在运行时，查找表可以通过共享区域ID及共享区域名称来快速查找共享区域。

（1）添加查找表

添加查找表实际上是指在查找表中添加共享区域的入口（Entry），在导入SharedRegion模块后，默认就已经添加了区域ID为0的SharedRegion，这个ID为0的共享区域是可以被所有处理器所共享的。查找表的添加有两种方式，一种是静态添加，这种方式下添加共享区域一般都所有核共享的，另一种是动态添加，这种方式可以添加一些只能部分核有效的共享区域。

a). 静态添加：静态添加主要是在.cfg文件中完成，在调用Ipc_start()时，完成SharedRegion模块的配置。以下代码都是直接在.cfg文件中添加的

首先导入SharedRegion模块，完成全局设置，首先SharedRegion.numEntries来设置系统中最大共享区域的数目。如果所有共享内存区域在所有处理器中都有相同基地址时，translate选项将会被设置为false意思是不用进行地址转换，而如果设置为true，则需要做地址转换。（本节的例程只需要用到一个共享区域，所以这两个选项其实都可以不用设置，默认就好了）

![](https://img-blog.csdn.net/20160506220116844?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

添加共享区域入口（Entry）：

![](https://img-blog.csdn.net/20160506220153344?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里的Entry参数含义如下：


       1）base：区域的基地址，不同处理器其基地址可以是不同的

       2）len：区域的长度，同一个共享区域在所有处理器的查找表中的长度应该是相同的

       3）ownerProcID：管理该区域的处理器ID，如果存在区域所有者，这个区域所有者（owner）就是创造HeapMemMp实例的，而其他核打开这个实例

       4）isValid：表明该区域在当前核上是否可用，判断当前核能否使用此共享区域的。

       5）cacheLineSize：这个值在所有核的查找表中都应该是相同的

       6）createHeap：表明是否需要给当前区域创建一个堆

       7）name：区域的名称

一个查找表中最大数目进入数目通常使用ShareRegion.numEntries中静态设置。因为每个处理器都存储着SharedRegion查找表中共享内存区域的信息，所以当在一个查找表中增加或删除共享区域入口时，都必须更新所有其他查找表中的数据，查找表中的入口越多，那么查找表的查找时间就越长。

b). 动态添加（一般少见，这里就稍微提一下就好了）：动态添加主要通过SharedRegion_setEntry()函数来设置如下入口参数。本节介绍的多核图像灰度转换程序中需要一个共享区域，且这个共享区域是对于所有核都是有效，所以不需要动态方法来设置该区域对于某个核属性了。

![](https://img-blog.csdn.net/20160506220332542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）使用共享区域内存，其一般性的使用方法：


heap = (IHeap_Handle)SharedRegion_getHeap(regionId);  // 通过区域ID获得共享区域的堆句柄

buf = Memory_alloc(heap, size, align, NULL);  // 通过堆分配区域内存

参数分析：

       1）regionId: 是区域ID，在静态添加中.cfg文件中通过SharedRegion.setEntryMeta中设置的，如果不知道，可以通过SharedRegion_getId()根据特定的地址来获得当前区域Id，或者通过SharedRegion_getIdByName()根据区域的名称来获得区域Id。

       2）buf: 分配的内存缓冲区指针，

       3）heap: 可用于分配内存的堆句柄

       4）size: 分配的内存缓冲区大小

       5）align: 对齐参数

添加相应头文件：

![](https://img-blog.csdn.net/20160506220509153?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


主任务函数中添加：

![](https://img-blog.csdn.net/20160506220527127?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）地址转换：在一个拥有共享内存区域的环境中，一个主要的问题是这些共享区域在不同处理器上可能会被映射到不同地址空间，即同样一块共享内存区域在不同处理的本地地址空间的逻辑位置是不同，因此此时就需要在不同处理器之间转换的地址指针。先前在静态设置中将SharedRegion.translate选项设置为true，则需要做地址转换。而设置为false时，不同处理器上同一样的内存缓冲区其对应地址都是相同，所以不需要做地址转换。这里为了描述共享内存间的地址转换过程，我们将其设置为true（默认）。

首先需要介绍两个概念：

共享区域地址SRPtr：共享区域地址是当前内存缓冲区在共享区域SharedRegion上的地址，可以视为全局地址，指向同一内存的SRPtr地址在所有处理器上都是相同，所以我们给其他核传递的就是这个地址。共享区域指针（SRPtr）是一个32位指针，其由一个ID和其偏移组成。一个SRPtr的最重要的位是用于表明ID号的，而ID号表明当前区域入口在查找表中的位置。


本地地址addr：共享区域地址SRPtr是不能在本地使用，一般情况下，需要转换成本地地址才能被本地处理器使用。

关于地址转换的几个函数：


SharedRegion_getPtr()根据共享区域指针来获得本地指针


SharedRegion_getSRPtr()根据给定的本地指针及区域ID来获得当前共享区域指针

![](https://img-blog.csdn.net/20160506220638159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


主核将共享区域地址指针通过MessageQ传递给了从核，然后在从核上进行相应的地址转换，得到本地地址


![](https://img-blog.csdn.net/20160506220652690?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（4）缓存Cache问题

主核在往内存写数据时，很有可能只是将数据写Cache中，还没有写入到内存中去，所以这个时候，从核肯定不能从内存中读到写入数据。这个问题非常重要，困扰我一两天了=_=||，解决方法是将回写Cache。具体步骤：

a). 导入头文件




#include<ti/sysbios/hal/Cache.h>




b). 给内存写入数据后，回写Cache

Void Cache_wb(Ptr blockPtr, SizeT byteCnt, Bits16 type,
 Bool wait);

blockPrt是指内存地址，byteCnt指回写数据大小，type指缓存类型，wait指是否等待。回写全部Cache可以调用Void Cache_wbAll();

c). 在从内存读出数据前，先让Cache失效

Void  Cache_disable(Bits16 type);

type指缓存类型，这里可以选择Cache_Type_ALL，即全部缓存




**三、Notify模块**

上一节我们介绍了Notify模块，这一节就不详细说了，本节的Notify模块主要有以下几个要点

（1）首先要保证各核之间存在连接，因为8个核都要连接，所以可以直接设为

（2）虽然只有主核向从核发送消息，但主核和从核之间都要注册事件

![](https://img-blog.csdn.net/20160506220833972?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）设置注册函数，主要完成地址传递，另外这里需要设置一个信号量（信号量的设置第5节有），从核只有地址传递后才能激活信号量，开始读取内存数据

![](https://img-blog.csdn.net/20160506220907597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（4）主核给从核发送可以读取数据事件


![](https://img-blog.csdn.net/20160506220923581?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（5）主核一直等信号量激活

![](https://img-blog.csdn.net/20160506220941676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**四、例程结果分析**

![](https://img-blog.csdn.net/20160506221009176?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


通过上面结果，可以看出核1-7完美读取了核0写入共享内存的数据。
需要注意的是有些情况下，即使在需要地址转换的情况下，各处理器的本地地址也有可能是相同的，就像本节的例子，但是当共享内存区域的数据成份比较复杂时，同一区域在不同处理器的本地地址就很可能不同了，所以地址转换是非常有必要的。







本节例程下载地址：[https://github.com/tostq/EasyMulticoreDSP/tree/master/7.SharedMem](https://github.com/tostq/EasyMulticoreDSP/tree/master/7.SharedMem)




