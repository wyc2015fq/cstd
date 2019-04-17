# （多核DSP快速入门）8、利用MessageQ模块的多核灰度转换程序 - tostq的专栏 - CSDN博客





2016年06月29日 23:15:58[tostq](https://me.csdn.net/tostq)阅读数：6995
所属专栏：[多核DSP快速入门](https://blog.csdn.net/column/details/easymulticoredsp.html)













原创文章


转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)



 第3节我们介绍了一个单核的图像灰度转换程序，这一节我们将这个单核的图像灰度转换程序改成多核程序。

       这个多核程序的主要原理非常简单，将整张图片分块，每一块的处理对应一核，这里需要涉及的是IPC的三个模块：

       （1）SharedRegion模块：通过SharedRegion模块将输入图像缓存和输出图像缓存放到共享内存中，使其每个核都能操作图像数据

       （2）MessageQ模块：主核（核0）在将图像文件输入共享内存后，需要将输入图像缓存和输出图像缓存的地址打包成消息，传递给从核。

       （3）Notify模块：从核完成其对应图像块的灰度转换后，需要Notify模块通知主核，完成任务。




       本节Notify模块和SharedRegion模块在上一节已经讲过了，这里只是粗略说明其设置，本节将重点介绍MessageQ模块，同时一步一步完成这个多核灰度转换程序。




**一、新建项目**

       前面已经介绍如何新建CCS项目，这里就不再详细介绍了，不过比较推荐方式是先导入IPC模板例子，然后在模板上进入修改，这样减少了大量配置SYS/BIOS的时间。




**二、导入SharedRegion模块**


导入SharedRegion模块的要点：

       （1）SharedRegion模块是由所有的核所共享，所以最好通过静态设置。在.cfg文件中设置SharedRegion的特性，下面是.cfg文件中的设置

![](https://img-blog.csdn.net/20160629225259674?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       （2）主核0创建存放输入缓冲和输出缓冲的两个共享内存

![](https://img-blog.csdn.net/20160629225332592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


       此时从核因处于等待消息，挂起状态。

       （3）从核收到消息，从消息中读取到两个共享内存的地址

![](https://img-blog.csdn.net/20160629225352034?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


       （4）地址转换。（前一节有提）


       （5）注意在往内存中写的时候，要通过Cache等函数将缓冲写入内存。

       在读的内存的时候，要失效Cache，防止读取到了Cache中的数据，而不是内存中数据。

       下面的这个场景，是核在处理完本身的任务后，等待其他核完成任务，读取共享内存前做的失效Cache的例子

![](https://img-blog.csdn.net/20160629225626709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






**二、导入MessageQ模块**


MessageQ模块，即消息队形（Message Queue）。MessageQ同Notify模块一样，也是用于多核之间的通信的，不过不同的是，Notify模块更加侧重于通知，其只能传递一个参数，而MessageQ却可以传递变长度的消息，更侧重于传递消息，另外不同线程间的消息是独立的，例如对于每个MessageQ来说，存在一个读者却可能有多个写者。在本节的多核图像灰度转换例子来说，我们需要向从核传递两个32bits的地址（inBuf、outBuf），这通过Notify模块是无法办到（只能传递32bits以下的信息）。所以我们必须选择MessageQ模块。


MessageQ模块的主要特点：

1. 实现了处理期间变长消息的传递，所需要传递的消息一般超过32bit；

2. 其消息的传递都是通过操作消息队列来实现的；

3. 每个消息队列可以有多个写者，但只能有一个读者，而每个任务(task)可以对多个消息队列进行读写；

4. 一个宿主在准备接收消息时，必须先创建消息队列，而在发送消息前，需要打开预定的接收消息队列；


**Tips：**

1. 读者（Reader）：读者（reader）是一个线程将消息队列中的消息读出，一个单一的读线程会调用MessageQ_create()，MessageQ_get()，MessageQ_free()及MessageQ_delete()，每一个读线程会拥有一个消息队列，而写线程会打开消息队列并把消息放入。

2. 写者（Writer）：写者（writer）则将消息写入消息队列中去，一个单一的写线程会调用MessageQ_open()，MessageQ_alloc()，MessageQ_put()及MessageQ_close()。

![](https://img-blog.csdn.net/20160629225725406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







**MessageQ组件提供了以下几个API：**

消息队列初始化：MessageQ_Params_init()

消息队列创建/销毁：MessageQ_create()/MessageQ_delete()，create创建消息队列，并分配相应存储空间

消息队列打开/关闭：MessageQ_open()/MessageQ_close()，open时会返回远程处理器上的QueID的地址。

为消息队列分配堆内存：MessageQ_alloc()/MessageQ_free()

为消息队列注册/注销堆内存：MessageQ_registerHeap()/MessageQ_unregisterHeap()

向消息队列中放入/获取消息：MessageQ_put()/MessageQ_get()

**其他逻辑API：**

获取消息队列ID：MessageQ_getQueueId()

获取消息队列中消息数：MessageQ_count()

在消息队列中嵌入消息：MessageQ_setReplyQueue()

为消息队列解阻塞：MessageQ_unblock()

为调试消息队列加入Trace：MessageQ_setMsgTrace()






（1）配置MessageQ模块，通过在.cfg文件中设置，具体的设置对象参数如下：

MessageQ.maxNameLen ：设置MessageQ名称的最大长度

MessageQ.maxRuntimeEntries ：设置可以创建的消息队形最大数目 

MessageQ.numHeaps ：系列中的Heap的数目

MessageQ.tableSection：用于放置名称表的段名

这里不需要上述设置，保持默认就可以了，只需要导入MessageQ模块及一个用于给消息分配堆空间的HeapBufMP模块（下一节将介绍）

![](https://img-blog.csdn.net/20160629225853845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**Tips：**核间连接需要设置启动MessageQ（默认不是启动的），

![](https://img-blog.csdn.net/20160629225915947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）创建MessageQ对象

因为MessageQ对象并不是共享资源，每个消息队列只能有一个读者，所以消息对列MessageQ只能通过动态创建，而不支持静态创建，需要依赖于创建其的处理器核。其创建步骤仍然是先初始化参数后创建。这里需要给每个从核建立一个消息队列，消息队列接收由主核发送的消息。

![](https://img-blog.csdn.net/20160629225937486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20160629230001533?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


MessageQ_create创建函数第一个参数是消息队列名称，是string类型。



（3）消息空间初始化

a). 构建消息结构，这里主要是创建可以打包SRPtr的消息结构体，消息的内部元素类型和数目可以任意，但任何消息结构定义都必须保证其第一个值为一个消息头结构，那么在MessageQ相关的函数的使用时，一般是用MessageQ_Msg来表示消息结构，所以需要将MyMsg强制转换成MessageQ_Msg。

![](https://img-blog.csdn.net/20160629230046605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


b). 给消息分配堆空间，所有发送给消息队列的消息都必须预先在Heap结构中分配，这里主要调用的HeapBufMP模块，这个模块主要分配定长的堆空间，在下节我们将具体介绍这个模块，这里先给出其用法，仍然是先初始化参数再创建，因为只需要发送一个消息就可以了，所以HeapBufMP就只分配一个消息块。另外我们也可以根据不同消息的用途来分配不同的堆，比如比较重要的消息可以分配到片上内存，而不怎么重要的消息就分配到外部内存的堆。

![](https://img-blog.csdn.net/20160629230121972?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


c).  堆的注册与分配

定义堆后，还需要将堆与消息队列挂钩，首先将堆定义一个ID号。MessageQ_registerHeap()函数给一个堆分配一个消息队列的堆编号，当分配一个消息时，将会利用这个堆编号，而不是堆句柄，这个堆编号应该是从0开始增长的，其最大数由MessageQ.numHeap来决定。

![](https://img-blog.csdn.net/20160629230143918?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


之后，利用这个HeapId将堆空间分配给消息

![](https://img-blog.csdn.net/20160629230206372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




（4）发送一个消息

a). 通过消息的名称来打开消息队列，并返回消息队列的ID，这个打开过程，需要等待循环

b). 根据消息队列ID，将消息发送给消息队列

c). 发送消息是在主核中完成，其需要给其他7个从核分别发送消息

d). 注意的是要将自我定义的msg结构同MessageQ_Msg进行强制转换

![](https://img-blog.csdn.net/20160629230547002?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 
     （5）打开一个消息

创建MessageQ的从核通过MessageQ_get()函数来获得消息

![](https://img-blog.csdn.net/20160629230637644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这里的handle表示消息队列的句柄，msg表示输出（打开）的消息，timeout表示等待打开消息的时间，MessageQ_FOREVER表示一直等待，直到收到消息。读取消息的内容：

![](https://img-blog.csdn.net/20160629230701973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




（6）消息及消息队列的删除

这里非常需要注意的是，要处理各消息和各消息队列的释放顺序，比如当前释放消息队列时，也会把消息给释放了，而本节的例子只创建了一个消息堆由多个核共享，此时如果其他核正在处理消息，那么将会出现如下堆错误：


[TMS320C66x_1] ti.sdo.ipc.MessageQ: line 260: assertion failure: A_heapIdInvalid: heapId is invalid

[TMS320C66x_1] xdc.runtime.Error.raise: terminating execution


a). 删除消息，释放消息堆空间. 消息的删除一般是由读线程完成的，当读者收到这个消息，其要么释放（删除）这个消息，要么重新利用这个消息。

![](https://img-blog.csdn.net/20160629230756880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


b). 消息队列的删除，消息的删除一般是由读线程完成的。

![](https://img-blog.csdn.net/20160629230825037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


c). 消息队列的关闭，当写线程（这里是主核）完成发送消息后并以后不再发送消息，其应该关闭消息队列

![](https://img-blog.csdn.net/20160629230840132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（7）有关于MessageQ的其它内容

MessageQ是一个复杂的消息传递模块，本节的例子只是提到了最为简单的用法，这里略微介绍下其它几个要点。

a). 消息ID，MessageQ还可以为每个消息提供一个ID号，通过设置这个ID号，可以让读者识别消息的来源

设置消息ID函数：MessageQ_setMsgId(msg, msgId)

获取消息ID函数：MessageQ_getMsgId(msg, msgId)

另外还有设置消息优先级函数：Void MessageQ_setMsgPri(MessageQ_Msg msg, MessageQ_Priority priority)

b). 关于线程同步

c). 回复队列（replyQueue），很多情况下不需要打开队列，就可以直接将对消息发送。有时很多消息发送后需要返回消息，就可以利用这个回复队列

消息的发送者，可以MessageQ_setReplyQueue通过给发送的消息设置一个回复队列ID号


Void MessageQ_setReplyQueue(MessageQ_Handle handle, MessageQ_Msg msg)；

而消息的读者，可以通过MessageQ_getReplyQueue来获得回复队列ID号，然后通过MessageQ_put直接发送消息。


MessageQ_getReplyQueue(msg)

d). 远程通信，MessageQ也可用于多处理器之间远程通信。

e). MessageQ的通信结构

![](https://img-blog.csdn.net/20160629230942743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**三、导入Notify模块**

导入Notify模块主要是为了让从核通知主核，其任务完成。上一节已经介绍了Notify模块，这里就不多说，主要提下几个要点

（1）首先要保证各核之间存在连接，因为8个核都要连接，所以可以直接设为

![](https://img-blog.csdn.net/20160629231018665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（2）从核注册发送主核的事件

![](https://img-blog.csdn.net/20160629231040885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）设置注册函数，这里需要设置一个信号量（信号量的设置第5节有），当完全收到七个从核的任务完成事件后，激活信号量.

![](https://img-blog.csdn.net/20160629231102510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（4）从核给主核发送任务完成事件

![](https://img-blog.csdn.net/20160629231122679?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（5）主核一直等信号量激活

![](https://img-blog.csdn.net/20160629231139464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**四、例程结果**

（1）关于图像灰度转换程序，这里为了方便，我们直接将图像以unsigned char的数组形式保存。

这个程序非常简单，将图像分成8块，每个核负责其中一块的处理：

![](https://img-blog.csdn.net/20160629231213351?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）软件仿真读取图像的时间实在太长了，所以这里没有对图像进行处理了，只是处理一组数据

读取数据

![](https://img-blog.csdn.net/20160629231233226?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


读出数据

![](https://img-blog.csdn.net/20160629231252742?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）结果

读取的数据结果：

![](https://img-blog.csdn.net/20160629231316889?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


前面两个255 255是主核的处理任务，这里主核没有完成处理，所以我们需要注意是要将主核同从核的任务分开，让从核只做计算任务，而主核负责全局的数据处理。

地址结果：

![](https://img-blog.csdn.net/20160629231344586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






