# Android Binder通信机制学习 - u013366022的专栏 - CSDN博客
2016年05月03日 13:41:49[slitaz](https://me.csdn.net/u013366022)阅读数：587
binder驱动和通信库是binder的核心，驱动是用c语言编写的，通信库是c++写的，应用程序通过jni和底层的库产生联系，建议研究binder的读者先读完<Linux内核设计与实现>和<Unix环境高级编程>这两本书做好研究准备，有时间的也可以学学<Unix网络编程卷2进程间通信>。本文不打算详细分析binder的代码，老罗的书写的最详细了
 :-)
**一,和传统IPC方式的比较:**
binder是一种进程间通信的机制，因此要说binder,不得不拿它与其他Unix传统的IPC机制做比较，unix传统的IPC机制分为如下几种(同步方面的如互斥锁，信号量等严格意义上不能算是IPC机制，这里就不列了):
1.消息传递类型:管道,FIFO(有名管道)，Posix消息队列，System V消息队列
2.Posix共享内存区和System V 共享内存区
3.Unix域Socket
4.Sun RPC
它们总结起来是如下三种方式:
**第一种形式:**两个进程共享存留于文件系统中某个文件上的某些信息，为了访问这些信息，每个进程都得穿越内核(read,write,lseek等)，这是最容易理解的通信方式。当然进程共享的文件可以是硬盘上的实际文件，也可以是虚拟文件，这些虚拟文件只是在文件系统中有个访问点。
![](https://img-blog.csdn.net/20131229193547984)
第二种形式:中间的两个进程共享驻留于内核中的某些信息，管道是这种共享类型的一个例子，binder也算是这种类型，进程可以通过访问/dev/binder(open,mmap,ioctl系统调用)从而实现互相通信，binder驱动运行于内核空间，它相当于一个集中式的管理者，大家都可以访问它，靠它这个中间者来实现通信。
![](https://img-blog.csdn.net/20131229193613750)
**第三种形式:**是两个进程有一个双方都能访问的共享内存区，每个进程一旦设置好该共享内存区，就根本不涉及内核而访问其中的数据，共享内存区是可用IPC形式中最快的，它信赖于mmap系统调用。
![](https://img-blog.csdn.net/20131229193621687)
unix提供的众多进程间通信机制各有各的使用场景，它们的性能也有差异性，其实像Unix域socket和管道机制在android代码里使用也很多，但是android最普遍的通信机制却是binder机制，主要有两方面原因，一个是安全性，一个则是性能。
unix传统的进程间通信机制的一个缺点是进程没有一种有效的机制标识消息发起方，虽然这些IPC的形式都经由内核，但是它们并没有提供任何措施使得内核能够标识发送者，这样一些重要的接口就无法做到验证发送者做权限检查，如ActivityManagerService和PackageManagerService的一些特权方法，普通程序是没有办法访问这些方法的，但是这些方法却对system用户开放。包括很多重要的服务都跑在单独的进程中，它们需要权限的隔离检查能力，没有这种能力，android平台的安全性和易用性就会很差。而binder通信中由内核来填充发送方的pid和euid，可以实现非常安全的权限检查。
至于性能方面，android平台本身很组件化，非常多的操作都涉及到了进程间通信，如启动一个activity,注册广播，访问系统服务，响应用户触屏事件等，因此进程间通信本身的消耗非常影响android系统的性能，需要一种高效的机制来实现这些机制，虽然IPC方式中共享内存速度最快，但是要用共享内存来实现这样一个庞大的完善的RPC(远程过程调用)机制是很复杂的，因此android里面使用了自己的binder机制，它也利用了mmap来提升性能，传输过程只需要一次拷贝。
binder相比传统的IPC机制还有一个区别是它实现了RPC机制，传统的进程间通信机制像管道,FIFO和消息队列等，都属于消息传递的类型，这些机制只负责将消息从一个进程传递到另外一个进程去，读写这些消息属于字节流IO的模型，由接收方和发送方定好某种协议来规定如何解释这些消息，虽然消息队列有消息边界，可以区分出每一条消息，但对消息的解释还是需要应用程序自己来做。但是binder通信中驱动和binder通信库的配合形成了一种RPC机制，应用程序可以像调用本地进程对象一样调用另外一个进程的对象，对于应用程序来说都是透明的,由aidl和binder通信库处理好了所有通信细节，包括如何打包和解包，进程间通信底层用什么格式等，应用程序只需要关注利用binder进制实现自己的逻辑就可以了。
二.binder通信概念:
要理解binder通信机制必须要理解几个概念:
1.RPC : 一个进程A中有一个对象ObjectA,另外一个进程B想通过RPC访问这个ObjectA,那么A和B中对该对象的格式必须是一致的，对java语言来说，双方可以引用同一个java文件中的类，对于c++来说，双方可以链接同一个动态库，这种对象定义的一致性是双方通信约定的一部分。
2.数据类型:进程间通信传递基本数据类型这个很容易理解，传递一个int到另外一个进程，对方就将其解释为一个int,但是如何传递一个对象到另外一个进程？像java中的序列化可以将一个对象转换成字节流通过网络传递到另外一个进程或者写进文件（序列化的这种机制能成功还是信赖于java字节码的平台无关性和类的动态加载机制）。binder通信中可以支持传递序列化过的java字节流，但是它也只是将这串字节流当做普通数据类型来处理，它只负责传递，具体如何解释这些数据由双方来约定。事实上binder通信只区分三种数据类型:不做解释的数据类型，binder数据类型和文件描述符。
3.binder引用和binder实体：
第2条所说，binder支持传递binder数据类型，这个binder数据类型又分为binder引用和binder实体，它们都用flat_binder_object结构来描述。binder实体的概念指一个进程中的对象提供了一些服务接口的实现供其他进程访问，因此它算是服务组件，这种数据传递给驱动的时候类型会被设置为BINDER_TYPE_BINDER以告诉驱动对这种数据做特殊处理。而binder引用是用在需要服务的进程一端，这个binder引用的类型为BINDER_TYPE_HANDLE,传递给驱动的时候驱动可以知道这种数据是binder引用。就像在一个进程中使用Foo
 *p = new Foo();一样，在堆上创建的Foo对象可以认为是实体，而p指针可以认为是引用，但这种比喻其实不太准确，下文会有描述，姑且先这样理解。
4.通信角色和访问点:
和unix传统的IPC机制一样，两个进程要进行通信，必须有一个双方都可以访问的名字/接入点，通过该接入点建立通信连接，它是用于客户和服务彼此连接与交换信息的手段，普通管道是在有亲缘关系的进程之间使用的，它们有虚拟地址空间相同的副本，因此可以不需要有接入点。其他通信机制都有这样的一个接入点，比如两个进程通过一个普通文件进行通信，那么该文件名就是双方都知道的，是一个接入点。FIFO有对应的路径名，Posix通信机制有Posix
 IPC名字，而Socket则通过ip地址与端口号来建立通信，Unix域socket则通过本地的socket文件路径来建立通信。
那么binder当中通信的这种接入点是什么呢，binder通信中有四个角色，分别是client组件，server组件，ServiceManager和binder驱动，client组件想和server组件进行通信，它必须去ServiceManager当中通过一个字符串查询Server组件的引用，这个字符串算是一个通信的接入点，它是server组件将自己注册到ServiceManager中去的时候填写的一个值。而ServiceManager对于大家来说是一个公共的接入点，0这个句柄值就是ServiceManager的引用标识。因此总结起来就是client和server通过大家都知道的0号标识去访问ServiceManager,server向ServiceManager注册一个binder实体，并关联一个字符串，表示只要查询这个字符串就能和我通信，而client向ServiceManager传递这个字符串，ServiceManager传递server注册的binder实体的一个引用给client端，client端拿到这个引用就可以和server进行通信了。因此ServiceManager的角色非常类似于网络环境中的DNS服务器，而驱动相当于路由器，做数据分发转储的工作。
三.binder通信过程:
一开始就把ServiceManager给牵涉进来讲会把问题搞复杂，因此这里考虑的一个场景是：C进程是普通的一个进程,S进程是一个服务性的进程，C进程和S进程已经建立了通信连接，S进程传递自己的一个binder实体给C进程(这跟进程传递binder实体给ServiceManager请求注册是类似的过程)，C进程拿到的其实只是一个binder引用，然后C进程再利用这个binder引用调用S进程binder实体中的一个函数。当然S进程不会主动传递binder实体给C进程，通常情况下是C进程通过一个方法来请求S来这样做的，这里简化掉一些无关的方面，只考虑这两个步骤的过程，理解了这个过程有助于理解binder引用和binder实体的知识。
binder引用和binder实体在用户空间的表现分别是BpBinder类和BBinder类，它们都继承自IBinder类。在这里贴上老罗的两幅类图加深理解:
![](https://img-blog.csdn.net/20131229194835000)
这幅图是server端的类图,它举的例子是以MediaPlayerService提供的接口为示例的，IMediaPlayerService属于双方共同约定的接口类，BnMediaPlayerService也是需要自己定义的类，它继承了IMediaPlayerService和BBinder,这个类的主要作用是实现自己的onTransact函数，依次解析client传递过来Parcel包，并且调用相应的函数，而这些函数的实现在MediaPlayerService派生类中，这个派生类是接口真正的实现，继承是一种类型关系，所以可以说MediaPlayerService它就是一个binder实体。
![](https://img-blog.csdn.net/20131229194839546)
上面这幅图是client端的类图，BpMediaPlayerService也继承自IMediaPlayerService接口，但这些接口的实现只是一种代理方式，用来做数据传输的，最终它是引用了BpBinder类，利用这个类的transact将函数调用需要的信息传递给binder驱动，在这里BpBinder是一个binder引用，它持有一个int32_t类型的handle，这个handle类似于文件描述符fd,它对应于某个binder实体，有了这个handle就可以发送给binder驱动告之我想访问这个handle对应的binder实体，而handle与binder实体的对应关系由内核来维护，ServiceManager这个handle值为0,client必须显式通过ServiceManager来请求获取某个server的handle，它的值和文件描述符是类似的，总是最小的未被使用的值，如第一个获取到的handle值就为1，当然如果没有显式通过ServiceManager来获取这个值，直接发送1给内核，内核是不认的，因为并没有建立起binder的引用和实体的对应关系，因此handle值是由内核来分配给client进程的。
知道了binder引用和binder实体的概念以后，接下来分析上述假设的场景，S进程发送一个MediaPlayerService实体给C进程，S其实是将MediaPlayerService的地址发送了出去，这个地址显然是S进程的一个虚拟地址，在其他进程就没有意义了。发送的时候是通过Parcel类的writeStrongBinder函数先打包到数据中，这个函数会生成一个flat_binder_object格式的结构体表示binder引用或者实体，它根据传递过来的IBinder对象的localBinder函数的返回值是不是为null来决定type值填充什么信息，因为BBinder和BpBinder都继承自IBinder,所以这两种类型都可以传递进来，但只有BBinder的localBinder函数返回值不为空，为自身的指针，由于MediaPlayerService继承自BBinder,所以localBinder返回值不为空，结构体的type字段就被指定为BINDER_TYPE_BINDER，否则就为BINDER_TYPE_HANDLE。驱动会根据这个type字段的值来获知传递过来的是引用或者实体从而做不同的处理。
binder引用和binder实体在内核空间的表示是binder_ref和binder_node结构体，它们的关系是一对多的关系，一个binder_ref引用着多个binder_node，内核发现传递过来了一个binder实体对象，它会找到binder实体对应的binder_node结构体，如果找不到会创建一个，并且将binder实体的地址值保存在binder_node结构中，然后会找到目标进程,这里是C进程，为它分配一个句柄值,并且生成一个binder_ref的结构保存在C进程的内核数据中，句柄值被保存为binder_ref的desc字段中，该binder_ref就和binder_node实体建立了关系，下次利用句柄值就可以找到binder_ref结构，从而找到对应的binder_node,从而最终找到当时传递过来的MediaPlayerService地址，因为该地址做为cookie字段保存在了binder_node结构中。binder_ref分配了以后，会生成一个flat_binder_object对象传递给C进程，该对象的type字段为BINDER_TYPE_HANDLE表示这是一个binder引用，并且handle值为分配的句柄值。
C进程拿到flat_binder_object以后会取出handle句柄并传递给一个新创建的BpBinder对象持有，BpBinder又被interface_cast函数给转换成了BpMediaPlayerService对象，其实interface_cast函数做的事情只是new了一个BpMediaPlayerService对象并且让它持有着BpBinder，这样C进程就可以调用BpMediaPlayerService对象来访问S进程的接口了。
C进程调用BpMediaPlayerService访问S进程的接口，会将得到的句柄值传递给内核，内核根据该句柄值找到binder_ref对象，从而可以找到binder_node对象，而binder_node结构存放着该实体所属进程的信息以及地址值，这样就可以找到S进程和S进程的MediaPlayerService实体,而这时S进程正在binder线程中睡眠着等待进程间通信的请求到达，所以它会被唤醒，将地址值强转成BBinder类型的指针，调用它的onTransact方法，最终调用到MediaPlayerService中对应的方法。
进程和ServiceManager进行交互也执行类似于上面的操作，server进程将一个字符串和binder实体传递给ServiceManager,ServiceManager得到的是一个binder引用，它通过一个叫svclist的链表保存字符串和binder引用关联的信息，client进程传递相同的字符串去ServiceManager请求得到binder引用，ServiceManager会回应给client进程它得到的binder引用，内核找到了对应的binder_node对象，但它发现这个对象所属的进程并不等于client进程，它就知道client进程也想拥有一个binder引用，这时候内核就会为client分配一个binder引用并传递给它，这时候ServiceManager和client就都有了一个指向同一个binder实体的binder引用。
C进程和S进程一旦通过ServiceManager建立起通信通道以后，它们后续可以通过该通道传递另外的binder实体或者引用而不再需要经过ServiceManager,有一种说法是这些binder类型的数据叫匿名binder,相应的在ServiceManager注册了名字的binder对象叫实名binder,首先必须通过实名binder建立起连接，才能在此连接上发送匿名binder对象，博客的后面有专门的例子说明这点。
binder引用和binder实体的对应关系图：
![](https://img-blog.csdn.net/20140803145109978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHI4NzM3MDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**四.线程管理:**
传统的通信机制如Socket，属于客户-服务器模型，服务器可以分为迭代和并发两种类型，迭代类型的服务器同一时间只能为一个客户服务，随后的请求必须排队进行，而并发类型的服务器可以开启多个进程或者线程为多个客户端服务，如果没有请求到达，服务器就阻塞在accept()系统调用上，连接建立以后客户端通过read()读取服务器发送过来的数据，如果没有数据就阻塞在read()中。
binder通信的服务端也可以为多个客户同时服务，可以允许多个客户同时调用该服务进程的某些组件的某些函数，服务端通过调用IPCThreadState的joinThreadPool函数将当前线程加入到binder线程池中，它的行为是在一个循环中通过调用ioctl来获取todo队列中的数据，如果队列中没有数据则阻塞在ioctl调用的wait_event_interruptible()中，否则会调用executeCommand()处理客户端发过来的请求。这里的todo队列有两个,一个是该进程的全局todo队列，一个是当前线程所私有的todo队列，
 一个线程只要不接收返回的数据包则应该在全局todo队列中等待新任务，否则就应该在其私有todo队列中等待其他进程通信的返回数据。程序中通过调用ProcessState的startThreadPool()函数产生一个线程并且将该线程加入到binder线程池， 服务端默认产生的binder线程一般就两个，如果短时间内大量的请求到达，binder驱动会发出BR_SPAWN_LOOPER命令通知进程需要产生一些额外的线程加入到线程池中，程序这时候就通过ProcessState的startThreadPool()函数产生线程加入到线程池中。client端发出一个进程间通信请求以后，它也会循环阻塞在ioctl()中等待server进程的回应，直到内核通过BR_REPLY告之通信结束，client就会从循环中返回，也就意味着方法返回了。
**五. 示例代码:**
一个良好的示例胜过千言万语，下面的示例主要涉及三种进程，一个是server进程，一个是client进程，一个是[Android](http://lib.csdn.net/base/15)进程，server进程提供了五个函数由client进程访问，而android进程则通过jni来访问server进程的这些接口。以下示例是基于老罗书上的例子中做了一些扩展，经过他的同意修改并增加了自己的一些内容。
**1.进程通信间协议文件:**
－－－－－－－－－－－－－－－－－－－－IFregService.h :
**[cpp]**[view
 plain](http://blog.csdn.net/dr8737010/article/details/17657307#)[copy](http://blog.csdn.net/dr8737010/article/details/17657307#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/133580/fork)
- #ifndef IFREGSERVICE_H_
- #define IFREGSERVICE_H_
- 
- #include <utils/RefBase.h>
- #include <binder/IInterface.h>
- #include <binder/Parcel.h>
- #include <utils/String8.h>
- 
- #define FREG_SERVICE "shy.luo.FregService"
- 
- usingnamespace android;  
- 
- class ICallback;  
- class IServerFoo;  
- 
- class IFregService: public IInterface  
- {  
- public:  
-     DECLARE_META_INTERFACE(FregService);  
- virtual int32_t getVal() = 0;  
- virtualvoid setVal(int32_t val) = 0;  
- 
- virtualvoid testCallback(const sp<ICallback>& cb) = 0;  
- 
- virtualconst sp<IServerFoo> getServerFoo() = 0;  
- virtualvoid foo(const sp<IServerFoo>&) = 0;  
- };  
- 
- class BnFregService: public BnInterface<IFregService>  
- {  
- public:  
- virtual status_t onTransact(uint32_t code, const Parcel& data,   
-             Parcel* reply, uint32_t flags = 0);  
- };  
- 
- //MyCallback----------------------------------------
- class ICallback : public IInterface{  
- public :  
-         DECLARE_META_INTERFACE(Callback);  
- virtualvoid onReturn(const String8 &) = 0;  
- 
- enum{  
-             PRINT_NAME_TRANSACTION = IBinder::FIRST_CALL_TRANSACTION,  
-         };  
- };  
- class BnCallback : public BnInterface<ICallback>{  
- public:  
- virtual status_t onTransact(uint32_t code, const Parcel& data,   
-             Parcel* reply, uint32_t flags = 0);  
- };  
- 
- //MyClass-------------------------------------------
- class IServerFoo : public IInterface{  
- public :  
-         DECLARE_META_INTERFACE(ServerFoo);  
- virtualvoid printUid() = 0;  
- 
- enum{  
-             PRINT_UID_TRANSACTION = IBinder::FIRST_CALL_TRANSACTION,  
-         };  
- };  
- class BnServerFoo : public BnInterface<IServerFoo>{  
- public:  
- virtual status_t onTransact(uint32_t code, const Parcel& data,   
-             Parcel* reply, uint32_t flags = 0);  
- };  
- 
- #endif
老罗的示例中只有传输基本数据类型的getVal()和setVal()两个接口，这里又增加了三个函数。
IFregService定义了一个Server组件需要提供的接口:
其中getVal和setVal传递的是基本数据类型，只是用来设置一个值和将该值打印出来。
testCallback用来测试回调函数用的，client进程将一个ICallback实体传递给server进程，server进程会回调这个函数，这个ICallback就叫做匿名binder,而且它的实体在client进程中，server进程拿到的只是binder引用。ICallback提供的函数叫onReturn(const String8 &)，由server进程传递一个String8类型的数据过来，client负责将该数据打印出来。
getServerFoo()接口请求server进程传递一个IServerFoo引用过来，然后foo()函数则调用这个IServerFoo接口，它只是简单的通过printUid打印出server进程的uid，该binder实体存在于server进程中。
BnXXX开头的几个类定义了一种继承关系，binder实体对象需要继承自这些类填写自己的实现，BnXXX的onTransact用来将client传输过来的数据解包并且调用相应的实现。
－－－－－－－－－－－－－－－－－－－－IFregService.cpp:
**[cpp]**[view
 plain](http://blog.csdn.net/dr8737010/article/details/17657307#)[copy](http://blog.csdn.net/dr8737010/article/details/17657307#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/133580/fork)
- #define LOG_TAG "IFregService"
- 
- #include <utils/Log.h>
- 
- #include "IFregService.h"
- 
- usingnamespace android;  
- 
- enum
- {  
-     GET_VAL = IBinder::FIRST_CALL_TRANSACTION,  
-     SET_VAL,  
-     TEST_CALLBACK,  
-     GET_SERVER_FOO,  
-     USE_FOO,  
- };  
- 
- class BpFregService: public BpInterface<IFregService>  
- {  
- public:  
-     BpFregService(const sp<IBinder>& impl)   
-         : BpInterface<IFregService>(impl)  
-     {  
- 
-     }  
- 
- public:  
-     int32_t getVal()  
-     {  
-         Parcel data;  
-         data.writeInterfaceToken(IFregService::getInterfaceDescriptor());  
- 
-         Parcel reply;  
-         remote()->transact(GET_VAL, data, &reply);  
- 
-         int32_t val = reply.readInt32();  
- 
- return val;  
-     }  
- 
- void setVal(int32_t val){  
-         Parcel data;  
-         data.writeInterfaceToken(IFregService::getInterfaceDescriptor());  
-         data.writeInt32(val);  
- 
-         Parcel reply;  
-         remote()->transact(SET_VAL, data, &reply);  
-         }  
- 
- void testCallback(const sp<ICallback>& cb){  
-         Parcel data;  
-         data.writeInterfaceToken(IFregService::getInterfaceDescriptor());  
-         data.writeStrongBinder(cb->asBinder());  
-         Parcel reply;  
-         remote()->transact(TEST_CALLBACK, data, &reply);  
-     }  
- const sp<IServerFoo> getServerFoo(){  
-         Parcel data;  
-         data.writeInterfaceToken(IFregService::getInterfaceDescriptor());  
-         Parcel reply;  
-         remote()->transact(GET_SERVER_FOO, data, &reply);  
- return interface_cast<IServerFoo>(reply.readStrongBinder());  
-     }  
- void foo(const sp<IServerFoo>& foo){  
-         Parcel data;  
-         data.writeInterfaceToken(IFregService::getInterfaceDescriptor());  
-         data.writeStrongBinder(foo->asBinder());  
-         Parcel reply;  
-         remote()->transact(USE_FOO, data, &reply);  
-     }  
- 
- };  
- 
- class BpCallback : public BpInterface<ICallback>{  
- public:  
-     BpCallback(const sp<IBinder>& impl)   
-         : BpInterface<ICallback>(impl)  
-     {  
- 
-     }  
- void onReturn(const String8& name){  
-         Parcel data;  
-         data.writeInterfaceToken(ICallback::getInterfaceDescriptor());  
-         data.writeString8(name);  
-         Parcel reply;  
-         remote()->transact(PRINT_NAME_TRANSACTION, data, &reply);  
-     }  
- };  
- 
- class BpServerFoo : public BpInterface<IServerFoo>{  
- public:  
-     BpServerFoo(const sp<IBinder>& impl)   
-         : BpInterface<IServerFoo>(impl)  
-     {  
- 
-     }  
- void printUid(){  
-         Parcel data;  
-         data.writeInterfaceToken(IServerFoo::getInterfaceDescriptor());  
-         Parcel reply;  
-         remote()->transact(IServerFoo::PRINT_UID_TRANSACTION, data, &reply);  
-     }  
- };  
- 
- IMPLEMENT_META_INTERFACE(FregService, "shy.luo.IFregService");  
- IMPLEMENT_META_INTERFACE(Callback, "my.ICallback");  
- IMPLEMENT_META_INTERFACE(ServerFoo, "my.IServerFoo");  
- 
- status_t BnFregService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)  
- {  
- switch(code)  
-     {  
- case GET_VAL:  
-         {  
-             CHECK_INTERFACE(IFregService, data, reply);  
-             int32_t val = getVal();  
-             reply->writeInt32(val);  
- 
- return NO_ERROR;  
-         }  
- case SET_VAL:  
-         {  
-             CHECK_INTERFACE(IFregService, data, reply);  
-             int32_t val = data.readInt32();  
-             setVal(val);  
- 
- return NO_ERROR;  
-         }  
- case TEST_CALLBACK:  
-         {  
-             CHECK_INTERFACE(IFregService, data, reply);  
-             sp<ICallback> cb = ICallback::asInterface(data.readStrongBinder());  
-             testCallback(cb);  
- return NO_ERROR;  
-         }  
- case GET_SERVER_FOO:  
-         {  
-             CHECK_INTERFACE(IFregService, data, reply);  
-             sp<IServerFoo> foo = getServerFoo();  
-             reply->writeStrongBinder(foo->asBinder());  
- return NO_ERROR;  
-         }  
- case USE_FOO:  
-         {  
-             CHECK_INTERFACE(IFregService, data, reply);  
-             sp<IServerFoo> fp = IServerFoo::asInterface(data.readStrongBinder());  
-             foo(fp);  
- return NO_ERROR;  
-         }  
- default:  
-         {  
- return BBinder::onTransact(code, data, reply, flags);  
-         }  
-     }  
- }  
- status_t BnCallback::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)  
- {  
- switch(code)  
-     {  
- case PRINT_NAME_TRANSACTION:  
-         {  
-             CHECK_INTERFACE(ICallback, data, reply);  
-             String8 name = data.readString8();  
-             onReturn(name);  
- 
- return NO_ERROR;  
-         }  
- default:  
-         {  
- return BBinder::onTransact(code, data, reply, flags);  
-         }  
-     }  
- }  
- 
- status_t BnServerFoo::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)  
- {  
- switch(code)  
-     {  
- case PRINT_UID_TRANSACTION:  
-         {  
-             CHECK_INTERFACE(IServerFoo, data, reply);  
-             printUid();  
- 
- return NO_ERROR;  
-         }  
- default:  
-         {  
- return BBinder::onTransact(code, data, reply, flags);  
-         }  
-     }  
- }  
IFregService.h文件中的实现类，BpXXX和BnXXX的实现都在这同一个文件中，这非常类似于aidl工具帮助我们生成的那个java文件，这样的做法是可以约定双方的通信协议，打包和解包的顺序必须一致，函数的code标识也需要一致，client进程和server进程只要同时包含IFregService.h文件，并且都链接IFregService.cpp文件就可以做到双方通信协议的一致性了，这跟java进程都拥有同一个aidl文件进行通信是一样的道理。
BpXXX的类型构造函数中都拥有一个IBinder，此IBinder就是binder引用，它的表现为BpBinder(int32_t handle),client调用BpXXX中的函数，其实BpXXX又通过BpBinder的transact函数将数据传输到了binder驱动并且等待回复。
**2.client进程代码:**
－－－－－－－－－－－－－－－－－－－－IFregService.cpp:
**[cpp]**[view
 plain](http://blog.csdn.net/dr8737010/article/details/17657307#)[copy](http://blog.csdn.net/dr8737010/article/details/17657307#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/133580/fork)
- #define LOG_TAG "FregClient"
- 
- #include <utils/Log.h>
- #include <binder/IServiceManager.h>
- 
- #include "../common/IFregService.h"
- #include <binder/BpBinder.h>
- 
- 
- class Callback : public BnCallback{  
- public :  
- void onReturn(const String8 & name){  
-             printf("i am the callback %s!\n",name.string());      
-         }  
- 
- };  
- int main()  
- {  
-     sp<IBinder> binder = defaultServiceManager()->getService(String16(FREG_SERVICE));  
- if(binder == NULL) {  
-         ALOGE("Failed to get freg service: %s.\n", FREG_SERVICE);  
- return -1;  
-     }  
-     BpBinder* bin = reinterpret_cast<BpBinder *>(binder.get());  
-     printf("i got the ref value is :%d\n",bin->handle());  
- 
-     sp<IFregService> service = IFregService::asInterface(binder);  
- if(service == NULL) {  
-         ALOGE("Failed to get freg service interface.\n");  
- return -2;  
-     }  
- 
-     printf("Read original value from FregService:\n");  
- 
-     int32_t val = service->getVal();  
-     printf(" %d.\n", val);  
- 
-     printf("Add value 1 to FregService.\n");          
- 
-     val += 1;  
-     service->setVal(val);  
- 
-     printf("Read the value from FregService again:\n");  
- 
-     val = service->getVal();  
-     printf(" %d.\n", val);   
- 
-     service->testCallback(new Callback());  
- 
-     sp<IServerFoo> fp = service->getServerFoo();  
-     service->foo(fp);  
- 
- return 0;  
- }  
在这里可以看到client进程通过sp<IBinder> binder = defaultServiceManager()->getService(String16(FREG_SERVICE));获取到了server进程的binder引用，又通过IFregService::asInterface(binder);将IBinder对象转换成了具体的接口对象，它的实现正如上面所述，其实是创建了一个BpFregService对象并且将IBinder对象给持有起来。
接下来client进程就通过产生的sp<IFregService> service调用server进程的函数，仿佛这些函数的实现在本进程一样，需要注意的是Callback类，client进程调用了service->testCallback(new Callback())将本进程的一个binder实体传递给了server进程，server进程拿到该binder引用回过来调用的时候又会经过client进程，这个binder实体就是匿名binder对象，具体server怎么又回调到client中去的，读者可以看一下IPCThreadState::waitForResponse函数switch语句的default分支，这里就不详细分析了。
**3.server进程代码:**
－－－－－－－－－－－－－－－－－－－－FregService.cpp:
**[cpp]**[view
 plain](http://blog.csdn.net/dr8737010/article/details/17657307#)[copy](http://blog.csdn.net/dr8737010/article/details/17657307#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/133580/fork)
- #define LOG_TAG "FregServer"
- 
- #include <stdlib.h>
- #include <fcntl.h>
- 
- #include <utils/Log.h>
- #include <unistd.h>
- #include <sys/types.h>
- 
- #include <binder/IServiceManager.h>
- #include <binder/IPCThreadState.h>
- 
- #include "../common/IFregService.h"
- 
- class MyServerFoo : public BnServerFoo{  
- public :  
- void printUid(){  
-             printf("server pid  is : %ld\n",(longint)getpid());  
-             ALOGI("server pid is : %ld\n",(longint)getpid());  
-         }  
- 
- };  
- class FregService : public BnFregService  
- {  
- public:  
-     FregService():val(88)  
-     {  
-     }  
- 
- virtual ~FregService()  
-     {  
-     }  
- 
- public:  
- staticvoid instantiate()  
-     {  
-         defaultServiceManager()->addService(String16(FREG_SERVICE), new FregService());  
-     }  
- 
-     int32_t getVal()  
-     {  
- return val;  
-     }  
- 
- void setVal(int32_t v){  
-         val = v;  
-    }  
- void testCallback(const sp<ICallback>& cb){  
-         String8 name("Mr Tough Guy!!!");  
-         cb->onReturn(name);  
-    }  
- const sp<IServerFoo> getServerFoo(){  
- returnnew MyServerFoo();     
-    }  
- void foo(const sp<IServerFoo>& fp){  
-         fp->printUid();  
-    }  
- private:  
-     int32_t val;  
- };  
- 
- int main(int argc, char** argv)  
- {  
-     FregService::instantiate();  
- 
-     ProcessState::self()->startThreadPool();  
-     IPCThreadState::self()->joinThreadPool();  
- 
- return 0;  
- }  
server组件将自己的binder实体注册到ServiceManager中以后，就通过startThreadPool()和joinThreadPool()产生了一个线程，并且将它与主线程一起加入到binder线程池中等待客户端的请求。
这样一个完整的示例就跑起来了，通过该示例的研究会对binder 的使用有更深入的理解。
java的示例代码非常类似于MediaPlayer的流程，MediaPlayer通过jni调用android_media_MediaPlayer.cpp中的函数，最终调用到了MediaPlayerService。
