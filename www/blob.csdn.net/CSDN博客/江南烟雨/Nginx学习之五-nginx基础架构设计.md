# Nginx学习之五-nginx基础架构设计 - 江南烟雨 - CSDN博客
2013年07月03日 20:31:45[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：7873
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)
一、优秀的模块化设计
（1）高度抽象的模块接口
所有的模块都遵循着同样的ngx_module_t接口设计规范。
（2）模块接口简单，灵活性高
模块的基本接口ngx_module_t足够简单，只涉及模块的初始化、退出以及对配置项的处理，这同时带来了足够的灵活性，是的nginx比较简单的实现了动态可修改性，也就是保持服务正常运行下使得系统功能发生改变。
[102](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L102) struct [ngx_module_s](http://lxr.evanmiller.org/http/ident?i=ngx_module_s) {
[103](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L103)[ngx_uint_t](http://lxr.evanmiller.org/http/ident?i=ngx_uint_t)            ctx_index;//当前模块在某一类模块中的索引
[104](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L104)[ngx_uint_t](http://lxr.evanmiller.org/http/ident?i=ngx_uint_t)            index;//当前模块在ngx_modules数组中的序号
[105](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L105)[106](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L106)[ngx_uint_t](http://lxr.evanmiller.org/http/ident?i=ngx_uint_t)            spare0;
[107](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L107)[ngx_uint_t](http://lxr.evanmiller.org/http/ident?i=ngx_uint_t)            spare1;
[108](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L108)[ngx_uint_t](http://lxr.evanmiller.org/http/ident?i=ngx_uint_t)            spare2;
[109](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L109)[ngx_uint_t](http://lxr.evanmiller.org/http/ident?i=ngx_uint_t)            spare3;
[110](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L110)[111](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L111)[ngx_uint_t](http://lxr.evanmiller.org/http/ident?i=ngx_uint_t)            version;
[112](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L112)[113](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L113)     void                 *ctx;//指向一类模块的上下文结构体，特定类型模块的公共接口
[114](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L114)[ngx_command_t](http://lxr.evanmiller.org/http/ident?i=ngx_command_t)        *commands;//处理配置项
[115](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L115)[ngx_uint_t](http://lxr.evanmiller.org/http/ident?i=ngx_uint_t)            type;//模块类型
[116](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L116)[117](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L117)[ngx_int_t](http://lxr.evanmiller.org/http/ident?i=ngx_int_t)           (*init_master)([ngx_log_t](http://lxr.evanmiller.org/http/ident?i=ngx_log_t) *log);//框架从来不会用到
[118](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L118)[119](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L119)[ngx_int_t](http://lxr.evanmiller.org/http/ident?i=ngx_int_t)           (*init_module)([ngx_cycle_t](http://lxr.evanmiller.org/http/ident?i=ngx_cycle_t) *[cycle](http://lxr.evanmiller.org/http/ident?i=cycle));//初始化所有模块时调用。在master/worker模式下，在启动子进程前完成
[120](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L120)[121](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L121)[ngx_int_t](http://lxr.evanmiller.org/http/ident?i=ngx_int_t)           (*init_process)([ngx_cycle_t](http://lxr.evanmiller.org/http/ident?i=ngx_cycle_t) *[cycle](http://lxr.evanmiller.org/http/ident?i=cycle));//正常服务器调用
[122](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L122)[ngx_int_t](http://lxr.evanmiller.org/http/ident?i=ngx_int_t)           (*init_thread)([ngx_cycle_t](http://lxr.evanmiller.org/http/ident?i=ngx_cycle_t) *[cycle](http://lxr.evanmiller.org/http/ident?i=cycle));
[123](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L123)     void                (*exit_thread)([ngx_cycle_t](http://lxr.evanmiller.org/http/ident?i=ngx_cycle_t) *[cycle](http://lxr.evanmiller.org/http/ident?i=cycle));
[124](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L124)     void                (*exit_process)([ngx_cycle_t](http://lxr.evanmiller.org/http/ident?i=ngx_cycle_t) *[cycle](http://lxr.evanmiller.org/http/ident?i=cycle));//服务停止前调用
[125](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L125)[126](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L126)     void                (*exit_master)([ngx_cycle_t](http://lxr.evanmiller.org/http/ident?i=ngx_cycle_t) *[cycle](http://lxr.evanmiller.org/http/ident?i=cycle));//master进程退出前调用
[127](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L127)[128](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L128)     uintptr_t             spare_hook0;
[129](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L129)     uintptr_t             spare_hook1;
[130](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L130)     uintptr_t             spare_hook2;
[131](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L131)     uintptr_t             spare_hook3;
[132](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L132)     uintptr_t             spare_hook4;
[133](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L133)     uintptr_t             spare_hook5;
[134](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L134)     uintptr_t             spare_hook6;
[135](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L135)     uintptr_t             spare_hook7;
[136](http://lxr.evanmiller.org/http/source/core/ngx_conf_file.h#L136) };
上述代码中void*类型的成员ctx一般用于表示在不同类型的模块中一种类型模块所具备的通用性几接口。
（3）多层次、多类别的模块设计
Nginx常用模块机器之间的关系：
![](https://img-blog.csdn.net/20130703203024812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以注意到，ngx_module_t接口中有一个type成员，指明了nginx允许在设计模块时定义模块类型这个概念。
配置模块的类型是NGX_CONF_MODULE，仅有一个模块。配置模块是所有模块的基础，它实现了最基本的配置项的解析功能（就是解析nginx.conf文件）。
nginx定义了一种基础类型的模块：核心模块，模块类型是NGX_CORE_MODULE。定义核心模块的目的是使得非模块化的框架代码只关注与如何调用6个核心模块。
事件模块、HTTP模块、mail模块这三种模块的共性是：实际上它们在核心模块中各有1个模块作为自己的代言人，并在同类模块中有1个作为核心业务与管理功能的模块。
事件模块是HTTP模块和mail模块的基础。

二、事件驱动架构
nginx采用完全的事件驱动架构来处理业务，这与传统的web服务器（如Apache）是不同的。
对于传统web服务器而言，采用的所谓事件驱动往往局限在tcp连接建立、关闭事件上。一个连接建立后，在其关闭之前的所有操作都不再是事件驱动，这时会退化成按序执行每个操作的批处理模式，这样每个请求在连接建立后都将始终占用着系统资源，直到连接关闭后才会释放资源。
这种传统的web服务器往往把一个进程或线程作为事件消费者，当一个请求产生的事件被该进程处理时，直到这个请求处理结束时进程资源都将被这一个请求所占用。
Nginx则不然，不会使用进程或线程来作为事件消费者，所谓的事件消费者只能是某个模块（在这里没有进程的概念）。只有事件收集、分发器才有资格占用进程资源，它们会在分发某个事件时调用事件消费模块使用当前占用的资源。
Nginx处理事件的简单模型如下图：
![](https://img-blog.csdn.net/20130703203103968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从上面的内容可一看出传统web服务器与nginx之间的重要差别：前者是每个事件消费者独占一个进程资源，后者的事件消费者只是被事件分发者短期调用而已。这种设计使得网络性能、用户感知的请求时延都得到提升，每个用户的请求都所产生的事件都得到及时响应，整个服务器的网络吞吐量都会由于事件的及时响应而增大。但这也会带来一个问题：事件消费者都不能有阻塞行为，否则由于长时间占用事件分发者进程而导致其它事件得不到及时响应。
三、请求的多阶段异步处理
请求的多阶段异步处理是基于事件（也只能基于事件）驱动架构实现。就是把一个请求的处理过程按照事件的触发方式分为多个节点，每个阶段都由事件收集、分发器来触发。
其优势：
这种设计配合事件驱动架构，将极大地提高网络的性能，同时使得每个进程全力运转，不会或者尽量少的出现进程休眠状况。因为一旦出现进程休眠，必然减少并发处理事件的数目，一定会降低网络性能，同时会增加请求处理时间的平均时延。这时，网络性能无法满足业务需求将只能增加进程数目，进程数目过多就会增加操作系统内核的额外操作：进程间切换。频繁的进程切换仍会消耗CPU等资源，从而降低网络性能。同时，休眠的进程会使进程占用的内存得不到有效释放，这最终必然导致系统可用内存的下降，从而影响系统能够处理的最大并发连接数。
参考资料：
《深入理解Nginx》
