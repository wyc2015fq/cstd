# pjlib深入剖析和使用详解 - DoubleLi - 博客园






1. PJSIP简介

 PJSIP的实现是为了能在嵌入式设备上高效实现SIP/VOIP.其主要特征包括:
    1).极具移植性.(Extremely portable)

    2).非常小的足印.(Very small footprint)
        官方宣称编译后的库<150Kb,我在PC上编译后加上strip后大概173Kb,这对于嵌入
        式设备,是个好消息:)

    3).高性能.(High performance)
       这点我们后面可以看看是否如作者宣称的:)

    4).支持众多的特征.(Many features)
      这点可以从[http://www.pjsip.org/sip_media_features.htm#sip_features](http://www.pjsip.org/sip_media_features.htm#sip_features看出.)
[看出.](http://www.pjsip.org/sip_media_features.htm#sip_features看出.)



    5).充足的SIP文档.(Extensive SIP documentation)
       这是我最初选择该库的原因,当然不是最终的原因,最终的原因是它的code:)

2.  PJSIP的组成.

    其实说是PJSIP不是特别贴切,这个库实际上是几个部分组成的.
 1).PJSIP - Open Source SIP Stack[开源的SIP协议栈]

    2).PJMEDIA - Open Source Media Stack[开源的媒体栈]

    3).PJNATH - Open Source NAT Traversal Helper Library[开源的NAT-T辅助库]

    4).PJLIB-UTIL - Auxiliary Library[辅助工具库]

    5).PJLIB - Ultra Portable Base Framework Library[基础框架库]

  3. PJLIB简介

 要理解好PJSIP，就不得不先说说PJLIB，PJLIB算的上是这个库中最基础的库，正是这个库的优美实现，才让PJSIP变得如此优越。
    PJLIB提供了一系列特征，这是我们下面分析的重点，涉及到：
  1).非动态内存分配[No Dynamic Memory Allocations]
       实现了内存池，获取内存是从与分配的内存池中获取，高性能程序多会自己构造内存池，后面我们会解释该内存池的使用以及基本的原理。根据作者的比较，是常规的 malloc(
)/free()函数的30倍。

    2).OS抽象[Operating System Abstraction]
       实现OS抽象的根本原因在与可移植性，毋庸置疑:).
       涉及到：
        a).线程[Threads.]
     b).线程本地存储[Thread Local Storage.]
        c).互斥[Mutexes.]
     d).信号灯[Semaphores.]
     e).原子变量[Atomic Variables.]
      f).临届区[Critical sections.]
      g).锁对象[Lock Objects.]
       h).事件对象[Event Object.]
      i).时间管理[Time Data Type and Manipulation.]
       j).高解析的时间戳[High Resolution Timestamp.]
      等等，这些我们后面分析代码时一一看来:)

    3).底层的网络相关IO[Low-Level Network I/O]
     这涉及到：
       a).Socket抽象[Socket Abstraction.]
        b).网络地址解析[Network Address Resolution.]
      c).实现针对Socket的select API[Socket select() API.]

    4).时间管理[Timer Management]
       这主要涉及到两个部分，一个是定时器的管理，还有就是时间解析的精度(举例说来，就是能精确到哪个时间等级，比如 POSIX sleep(),就只能以秒为单位，而使用select()则可
以实现毫秒级别的计时)

    5).各种数据结构[Various Data Structures]
      主要有:
        a).针对字符串的操作[String Operations]
      b).数组辅助[Array helper]
       c).Hash表[Hash Tabl]
     d).链表[Linked List]
      e).红黑平衡树[Red/Black Balanced Tree]

    6).异常处理[Exception Construct]
        使用的是TRY/CATCH,知道C++/JAVA之类面向对象语言的人看过会宛而一笑:)

   7).LOG机制[Logging Facility]
      很显然，一个良好的程序，好的LOG机制不可少。这能很方便的让你去调试程序，对此我
是深有体会，任何时候，不要忘记“好的程序，是架构出来的；而能跑的程序，是调试出
来的:)”

    8).随机数以及GUID的产生[Random and GUID Generation]
     GUID指的是"globally unique identifier"，只是一个标识而已，比如说你的省份证，
算的上是一个GUID，当然，准确说来是“china unique identifier”:).

    看了这么多的特征列举，是不是很完备，的确。

    总算是初步列举完了PJLIB的基本特征了，后面我们来说说它的使用与实现:

4.  PJLIB的使用

    有了上述介绍，是不是很想知道这个库的使用，没关系，我们慢慢说来:)

    首先是头文件和编译出来的库的位置，这就不必多说了，除非你没有使用过手动编译的库
，如果不太了解步骤，google一下，啊:)

    1).为了使用这个库，需要使用：
    #include <pjlib.h>
    当然，也可以选择：
   #include <pj/log.h>
    #include <pj/os.h>
    这种分离的方式，不过，简介其间，还是使用第一种吧:),毕竟，你不需要确认到你所
需的函数或者数据结构具体到哪个具体的头文件:)

    2).确保在使用PJLIB之前调用 pj_init()来完成PJLIB库使用前说必须的一些初始化.

    这是一个必不可少的步骤.
    ~~~~~~~~~~~~~~~~~~~~~~~

    3).使用PJLIB的一些建议
    作者对使用PJLIB的程序提出了一些建议，包括如下 ：
       a).不要使用ANSI C[Do NOT Use ANSI C]
        观点很明确，ANSI C并不会让程序具有最大的移植性，应该使用PJSIP库所提供的响
应机制来实现你所需要的功能.

        b).使用pj_str_t取代C风格的字符串[Use pj_str_t instead of C Strings]
       原因之一是移植性，之二则是PJLIB内置的pj_str_t相关操作会更快(性能).

        c).从内存池分配内存[Use Pool for Memory Allocations]
        这很明显，如果你知道为什么会使用内存池的话(提示一下，性能以及易用性:))

        d).使用PJLIB的LOG机制做文字显示[Use Logging for Text Display]
     很明显:)

     还有些关于移植的一些问题，不在我们的讨论范围，如果你需要移植到其它平台或者
环境，请参考[http://www.pjsip.org/pjlib/docs/html/porting_pjlib_pg.htm](http://www.pjsip.org/pjlib/docs/html/porting_pjlib_pg.htm)

5. PJLIB的使用以及原理
    终于开始提及实现原理以及具体的编码了:),前面的列举还真是个琐碎的事情,还是奔主题来:).  
    5.1快速内存池[Fast Memory Pool]
  前面说过,使用内存池的原因在于性能的考虑,原因是C风格的malloc()以及C++风格的new 操作在高性能或实时条件下表现并不太好,原因在于性能的瓶颈在于内存碎片问题.
   下面列举其优点与需要主要的问题:
    优点：
 a).不像其它内存池,允许分配不同尺寸的chunks.
 b).快速.
      内存chunks拥有O(1)的复杂度,并且操作仅仅是指针的算术运算,其间不需要使用锁住任何互斥量.
   c).有效使用内存.
      除了可能因为内存对齐的原因会浪费很少的内存外,内存的使用效率非常高.
  d).可预防内存泄漏.
     在C/C++程序中如果出现内存泄漏问题,其查找过程哪个艰辛,不足为外人道也:(
     [曾经有次用别人的Code,出现了内存泄漏,在开发板上查找N天,又没工具可在开发板上使用,哪个痛苦,想自杀， 原因很简单,你的内存都是从内存池中获取的,就算你没有释放你获取的内存,只要你记得把内存池destroy,那么内存还是会还给系统.

    还有设计带来的一些其它益处,比如可用性和灵活性:
    e).内存泄漏更容易被跟踪.
      这是因为你的内存是在指定的内存池中分配的,只要能很快定位到内存池,内存泄漏的侦测就方便多了.
     f).设计上从内存池中获取内存这一操作是非线程安全的.
     原因是设计者认为内存池被上层对象所拥有,线程安全应该由上层对象去保证,这样的话,没有锁的问题会让内存分配变得非常的快.
 g).内存池的行为像C++中的new的行为,当内存池获取内存chunks会抛出PJ_NO_MEMORY_EXCEPTION异常,当然,因为支持异常处理,也可以使用其它方式让上层程序灵活的定义异常的处理.
   这是异常处理的基本出发点,但是这有大量的争论,原因是这改变了程序的正常流程,谁能去保证这种流程是用户所需要的呢,因此C++中的异常处理饱受争议,请酌情使用]
 h). 可以在后端使用任何的内存分配器.默认情况下是使用malloc/free管理内存池的块,
    a).使用合适的大小来初始化内存池.
      使用内存池时,需要指定一个初始内存池大小, 这个值是内存池的初始值,如果你想要高
性能,要谨慎选择这个值哦,太大的化会浪费内存,过小又会让内存池自身频繁的去增加内存
,显然这两种情况都不可取.
  b). 注意,内存池只能增加,而不能被缩小(shrink),因为内存池没有函数把内存chunks释
放还给系统,这就要去内存池的构造者和使用者明确使用内存.

    恩,基本的原理都差不多了,后面我们来看看如何使用这个内存池.

  5.2内存池的使用[Using Memory Pool]
     内存池的使用相当的简单,扳个手指头就搞定了,如果你看明白了上面的原理和特征:)

     a).创建内存池工厂[Create Pool Factory]
    PJLIB已经有了一个默认的实现:Caching Pool Factory,这个内存池工厂的初始化使用函数pj_caching_pool_init()


    b).创建内存池[Create The Pool]
   使用pj_pool_create(),其参数分别为内存工厂(Pool Factory),内存池的名字(name),初
始时的大小以及增长时的大小.

    c).根据需要分配内存[Allocate Memory as Required]
    然后,你就可以使用pj_pool_alloc(), pj_pool_calloc(), 或pj_pool_zalloc()从指定
的内存池根据需要去获取内存了:)

    d).Destroy内存池[Destroy the Pool]
     这实际上是把预分配的内存还给系统.

    e).Destroy内存池工厂[Destroy the Pool Factory]
   这没什么好说的.

    #include <pjlib.h>

#define THIS_FILE "main.cpp"

int main()

{

 char errmsg[PJ_ERR_MSG_SIZE];

 pj_caching_pool cp;

 pj_status_t status;

 status=pj_init();

 if(status!=PJ_SUCCESS)

 {

 pj_strerror(status,errmsg,sizeof(errmsg));

 fprintf(stderr,"pj_init error\n");

 return 1;

 }

 //create factory

 pj_caching_pool_init(&cp,NULL,1024*1024);

 pj_pool_t *pool;



 //create pool

 pool=pj_pool_create(&cp.factory,"pool1",4000,4000,NULL);

 if(!pool)

 {

 fprintf(stderr,"error createing pool\n");

 return 1;

 }

 //alloc some size  memory

 void *p;

 p=pj_pool_alloc(pool,(pj_rand()+1)%512);



 //free the pool

 pj_pool_release(pool);



 //free the factory

 pj_caching_pool_destroy(&cp);



 return 0;



}

2. 

PJSIP

基于一个开放的、成熟的SIP开源库进行开发不但可以大大提高效率，也可增强与其他的SIP系统的兼容性。PJSIP是用C编写的，相当优秀的一个SIP协议栈，其主要特征包括：

1. 极具移植性。支持的平台有Windows、Windows Mobile、Linux、Unix、MacOS X、RTEMS、Symbian OS等。

2. 非常小的存储空间。包含完整SIP功能的代码库仅150K。

3. 高性能。采用优秀的内存分配机制，运行速度快。

4. 支持众多SIP特征和扩展。比如IM、presence、event subscription、call transfer、PIDF等。

5. 丰富的SIP文档和范例。



PJSIP 开源库由一系列功能库所组成，如图1所示，PJLIB 是系统抽象层，PJLIB-UTIL提供有用的工具函数，PJNATH 解决NAT 穿越问题，PJMEDIA 和PJMEDIA-CODEC 负责SDP 协商、媒体编码和媒体传输，PJSIP 是核心SIP 协议栈，PJSIP-SIMPLE 实现Presence和即时消息，PJSIP-UA 提供SIP 用户代理库，PJSUA 位于最高层，整合了下层模块的全部功能。PJSIP 的每个功能库根据其所在的层次以及负责的功能都提供了丰富的编程接口，方便开发人员使用。

PJSIP 协议栈内部包含多个SIP 消息处理层，如下图2所示，从下往上依次TRANSPORT层、ENDPOINT 层、TRANSACTION 层、UA 层和DIALOG 层。每个消息处理层以模块的形式注册到协议栈中，开发者也可以编写并添加自己的消息处理模块，对SIP 消息进行解析或修改。当TRANSPORT MANAGER 收到SIP 消息包时，会把该SIP EVENT 通知上层的ENDPOINT，而ENDPOINT 会找到对应的接收者，先把EVENT 传给TRANSACTION LAYER，然后再传给UA LAYER（传递的顺序由每个模块的优先权决定），如果 UA LAYER指定要处理TRANSACTION 的EVENT，TRANSACTION LAYER 也会把解析后的EVENT传给UA LAYER。



               PJSIP是一个高度封装的库，实际上它是通过PJSUA子库来实现应用的。一个完整的PJSUA生命周期，首先需要初始化，通过函数init（）来实现。在这个函数中，将创建代理、初始化变量和堆栈，以及创建一个UDP传输并在最后启动代理；第二步将为UA添加用户，如果需要的话，还要向服务器注册用户；当用户添加成功后，此时可以建立一个呼叫连接，发起会话；当会话连接成功后，就可以使用SRTP协议实时传输加密后的数据，进行通话。最后的过程是挂起或销毁呼叫。  

# 1.1 整体结构

        1.1.1 通信结构图







# 1.2 Endpoint的介绍

Sip协议栈的中心环节就是Endpoint，被封装成了pjsip_endpoint类型来使用。来看看它的特性和职责吧。

1.它采用内存池来为所有的sip组件分配内存。

2.它有一个时间堆，为所有的sip组件分配时间。

3.它还有一个通讯协议管理模块，这个管理模块包括通讯协议的管理，以及对消息的解析的打印。还记得sip消息的结构吗？

4.它还有一个单一的PJLIB的io队列，这个io队列什么功能呢？就是对处理不完的网络事件放到队列中。

5.它还有一个线程安全的轮询函数。用于来对应用程序的线程进行事件和socket事件的轮询。

6.它管理着pjsip模块，pjsip模块不创建任何线程。Pjsip是扩展协议栈的主要方法，并不局限于消息的解析和显示。

7.从通讯管理模块接收消息，并且把消息分发到各个模块。

下面具体介绍一下这些功能。

### 1.2.1 内存的分配和释放

所有组件内存的分配都有endpoint进行，这样就可以保证线程的安全，保证整个应用体系执行统一的决定。这些决定很多，举一个例子吧。例如，缓存。

Endpoint提供了两个函数来分配和释放内存池

.pjsip_endpt_create_poll()

.pjsip_endpt_create_poll()

对于endpoint的创建可以用pjsip_endpt_create()函数。

当调用这个函数之后，endpoint就被创建了，创建内存池的工厂由应用程序指定来让endpoint使用。这个内存工厂指针将存在于endpoint的生命周期中，endpoint就用他来创建和释放内存池。

### 1.2.2 时间管理

Endpoint通过控制一个定时器堆来管理定时器。所有组件的定时器的创建和管理都由endpoint进行。

提供的函数有

.pjsip_endpt_schedule_timer();

.pjsip_endpt_cancel_timer();

当endpoint的轮询函数被调用的时候，endpoint来检查时间的终结。

### 1.2.3 轮询处理事件

Endpoint提供了一个函数叫做pjsip_endpt_handle_events()来检查时间和网络时间。应用程序可以确定它多久等到网络事件的发生。

# 1.3 线程安全和线程并发









