# 优秀开源项目之三：高性能、高并发、高扩展性和可读性的网络服务器架构State Threads - DoubleLi - 博客园






# 译文在后面。

# State Threads for Internet Applications

## Introduction

State Threads is an application library which provides a foundation for writing fast and highly scalable Internet Applications on UNIX-like platforms. It combines the simplicity of the multithreaded programming paradigm, in which one thread supports each simultaneous connection, with the performance and scalability of an event-driven state machine architecture.

## 1. Definitions

### 1.1 Internet Applications

An Internet Application (IA) is either a server or client network application that accepts connections from clients and may or may not connect to servers. In an IA the arrival or departure of network data often controls processing (that is, IA is a data-driven application). For each connection, an IA does some finite amount of work involving data exchange with its peer, where its peer may be either a client or a server. The typical transaction steps of an IA are to accept a connection, read a request, do some finite and predictable amount of work to process the request, then write a response to the peer that sent the request. One example of an IA is a Web server; the most general example of an IA is a proxy server, because it both accepts connections from clients and connects to other servers.

We assume that the performance of an IA is constrained by available CPU cycles rather than network bandwidth or disk I/O (that is, CPU is a bottleneck resource).

### 1.2 Performance and Scalability

The performance of an IA is usually evaluated as its throughput measured in transactions per second or bytes per second (one can be converted to the other, given the average transaction size). There are several benchmarks that can be used to measure throughput of Web serving applications for specific workloads (such as SPECweb96, WebStone, WebBench). Although there is no common definition for scalability, in general it expresses the ability of an application to sustain its performance when some external condition changes. For IAs this external condition is either the number of clients (also known as "users," "simultaneous connections," or "load generators") or the underlying hardware system size (number of CPUs, memory size, and so on). Thus there are two types of scalability: load scalability and system scalability, respectively.

The figure below shows how the throughput of an idealized IA changes with the increasing number of clients (solid blue line). Initially the throughput grows linearly (the slope represents the maximal throughput that one client can provide). Within this initial range, the IA is underutilized and CPUs are partially idle. Further increase in the number of clients leads to a system saturation, and the throughput gradually stops growing as all CPUs become fully utilized. After that point, the throughput stays flat because there are no more CPU cycles available. In the real world, however, each simultaneous connection consumes some computational and memory resources, even when idle, and this overhead grows with the number of clients. Therefore, the throughput of the real world IA starts dropping after some point (dashed blue line in the figure below). The rate at which the throughput drops depends, among other things, on application design.

We say that an application has a good load scalability if it can sustain its throughput over a wide range of loads. Interestingly, the SPECweb99 benchmark somewhat reflects the Web server's load scalability because it measures the number of clients (load generators) given a mandatory minimal throughput per client (that is, it measures the server's capacity). This is unlike SPECweb96 and other benchmarks that use the throughput as their main metric (see the figure below).

![](http://img.blog.csdn.net/20161122094144669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

System scalability is the ability of an application to sustain its performance per hardware unit (such as a CPU) with the increasing number of these units. In other words, good system scalability means that doubling the number of processors will roughly double the application's throughput (dashed green line). We assume here that the underlying operating system also scales well. Good system scalability allows you to initially run an application on the smallest system possible, while retaining the ability to move that application to a larger system if necessary, without excessive effort or expense. That is, an application need not be rewritten or even undergo a major porting effort when changing system size.

Although scalability and performance are more important in the case of server IAs, they should also be considered for some client applications (such as benchmark load generators).

### 1.3 Concurrency

Concurrency reflects the parallelism in a system. The two unrelated types are virtual concurrency and real concurrency.

Virtual (or apparent) concurrency is the number of simultaneous connections that a system supports.

Real concurrency is the number of hardware devices, including CPUs, network cards, and disks, that actually allow a system to perform tasks in parallel.

An IA must provide virtual concurrency in order to serve many users simultaneously. To achieve maximum performance and scalability in doing so, the number of programming entities than an IA creates to be scheduled by the OS kernel should be kept close to (within an order of magnitude of) the real concurrency found on the system. These programming entities scheduled by the kernel are known as kernel execution vehicles. Examples of kernel execution vehicles include Solaris lightweight processes and IRIX kernel threads. In other words, the number of kernel execution vehicles should be dictated by the system size and not by the number of simultaneous connections.

## 2. Existing Architectures

There are a few different architectures that are commonly used by IAs. These include the Multi-Process, Multi-Threaded, and Event-Driven State Machine architectures.

### 2.1 Multi-Process Architecture

In the Multi-Process (MP) architecture, an individual process is dedicated to each simultaneous connection. A process performs all of a transaction's initialization steps and services a connection completely before moving on to service a new connection.

User sessions in IAs are relatively independent; therefore, no synchronization between processes handling different connections is necessary. Because each process has its own private address space, this architecture is very robust. If a process serving one of the connections crashes, the other sessions will not be affected. However, to serve many concurrent connections, an equal number of processes must be employed. Because processes are kernel entities (and are in fact the heaviest ones), the number of kernel entities will be at least as large as the number of concurrent sessions. On most systems, good performance will not be achieved when more than a few hundred processes are created because of the high context-switching overhead. In other words, MP applications have poor load scalability.

On the other hand, MP applications have very good system scalability, because no resources are shared among different processes and there is no synchronization overhead.

The Apache Web Server 1.x ([Reference 1]) uses the MP architecture on UNIX systems.

### 2.2 Multi-Threaded Architecture

In the Multi-Threaded (MT) architecture, multiple independent threads of control are employed within a single shared address space. Like a process in the MP architecture, each thread performs all of a transaction's initialization steps and services a connection completely before moving on to service a new connection.

Many modern UNIX operating systems implement a many-to-few model when mapping user-level threads to kernel entities. In this model, an arbitrarily large number of user-level threads is multiplexed onto a lesser number of kernel execution vehicles. Kernel execution vehicles are also known as virtual processors. Whenever a user-level thread makes a blocking system call, the kernel execution vehicle it is using will become blocked in the kernel. If there are no other non-blocked kernel execution vehicles and there are other runnable user-level threads, a new kernel execution vehicle will be created automatically. This prevents the application from blocking when it can continue to make useful forward progress.

Because IAs are by nature network I/O driven, all concurrent sessions block on network I/O at various points. As a result, the number of virtual processors created in the kernel grows close to the number of user-level threads (or simultaneous connections). When this occurs, the many-to-few model effectively degenerates to a one-to-one model. Again, like in the MP architecture, the number of kernel execution vehicles is dictated by the number of simultaneous connections rather than by number of CPUs. This reduces an application's load scalability. However, because kernel threads (lightweight processes) use fewer resources and are more light-weight than traditional UNIX processes, an MT application should scale better with load than an MP application.

Unexpectedly, the small number of virtual processors sharing the same address space in the MT architecture destroys an application's system scalability because of contention among the threads on various locks. Even if an application itself is carefully optimized to avoid lock contention around its own global data (a non-trivial task), there are still standard library functions and system calls that use common resources hidden from the application. For example, on many platforms thread safety of memory allocation routines (malloc(3), free(3), and so on) is achieved by using a single global lock. Another example is a per-process file descriptor table. This common resource table is shared by all kernel execution vehicles within the same process and must be protected when one modifies it via certain system calls (such as open(2), close(2), and so on). In addition to that, maintaining the caches coherent among CPUs on multiprocessor systems hurts performance when different threads running on different CPUs modify data items on the same cache line.

In order to improve load scalability, some applications employ a different type of MT architecture: they create one or more thread(s) per task rather than one thread per connection. For example, one small group of threads may be responsible for accepting client connections, another for request processing, and yet another for serving responses. The main advantage of this architecture is that it eliminates the tight coupling between the number of threads and number of simultaneous connections. However, in this architecture, different task-specific thread groups must share common work queues that must be protected by mutual exclusion locks (a typical producer-consumer problem). This adds synchronization overhead that causes an application to perform badly on multiprocessor systems. In other words, in this architecture, the application's system scalability is sacrificed for the sake of load scalability.

Of course, the usual nightmares of threaded programming, including data corruption, deadlocks, and race conditions, also make MT architecture (in any form) non-simplistic to use.

### 2.3 Event-Driven State Machine Architecture

In the Event-Driven State Machine (EDSM) architecture, a single process is employed to concurrently process multiple connections. The basics of this architecture are described in Comer and Stevens [Reference 2]. The EDSM architecture performs one basic data-driven step associated with a particular connection at a time, thus multiplexing many concurrent connections. The process operates as a state machine that receives an event and then reacts to it.

In the idle state the EDSM calls select(2) or poll(2) to wait for network I/O events. When a particular file descriptor is ready for I/O, the EDSM completes the corresponding basic step (usually by invoking a handler function) and starts the next one. This architecture uses non-blocking system calls to perform asynchronous network I/O operations. For more details on non-blocking I/O see Stevens [Reference 3].

To take advantage of hardware parallelism (real concurrency), multiple identical processes may be created. This is called Symmetric Multi-Process EDSM and is used, for example, in the Zeus Web Server ([Reference 4]). To more efficiently multiplex disk I/O, special "helper" processes may be created. This is called Asymmetric Multi-Process EDSM and was proposed for Web servers by Druschel and others [Reference 5].

EDSM is probably the most scalable architecture for IAs. Because the number of simultaneous connections (virtual concurrency) is completely decoupled from the number of kernel execution vehicles (processes), this architecture has very good load scalability. It requires only minimal user-level resources to create and maintain additional connection.

Like MP applications, Multi-Process EDSM has very good system scalability because no resources are shared among different processes and there is no synchronization overhead.

Unfortunately, the EDSM architecture is monolithic rather than based on the concept of threads, so new applications generally need to be implemented from the ground up. In effect, the EDSM architecture simulates threads and their stacks the hard way.

## 3. State Threads Library

The State Threads library combines the advantages of all of the above architectures. The interface preserves the programming simplicity of thread abstraction, allowing each simultaneous connection to be treated as a separate thread of execution within a single process. The underlying implementation is close to the EDSM architecture as the state of each particular concurrent session is saved in a separate memory segment.

### 3.1 State Changes and Scheduling

The state of each concurrent session includes its stack environment (stack pointer, program counter, CPU registers) and its stack. Conceptually, a thread context switch can be viewed as a process changing its state. There are no kernel entities involved other than processes. Unlike other general-purpose threading libraries, the State Threads library is fully deterministic. The thread context switch (process state change) can only happen in a well-known set of functions (at I/O points or at explicit synchronization points). As a result, process-specific global data does not have to be protected by mutual exclusion locks in most cases. The entire application is free to use all the static variables and non-reentrant library functions it wants, greatly simplifying programming and debugging while increasing performance. This is somewhat similar to a co-routine model (co-operatively multitasked threads), except that no explicit yield is needed -- sooner or later, a thread performs a blocking I/O operation and thus surrenders control. All threads of execution (simultaneous connections) have the same priority, so scheduling is non-preemptive, like in the EDSM architecture. Because IAs are data-driven (processing is limited by the size of network buffers and data arrival rates), scheduling is non-time-slicing.

Only two types of external events are handled by the library's scheduler, because only these events can be detected by select(2) or poll(2): I/O events (a file descriptor is ready for I/O) and time events (some timeout has expired). However, other types of events (such as a signal sent to a process) can also be handled by converting them to I/O events. For example, a signal handling function can perform a write to a pipe (write(2) is reentrant/asynchronous-safe), thus converting a signal event to an I/O event.

To take advantage of hardware parallelism, as in the EDSM architecture, multiple processes can be created in either a symmetric or asymmetric manner. Process management is not in the library's scope but instead is left up to the application.

There are several general-purpose threading libraries that implement a many-to-one model (many user-level threads to one kernel execution vehicle), using the same basic techniques as the State Threads library (non-blocking I/O, event-driven scheduler, and so on). For an example, see GNU Portable Threads ([Reference 6]). Because they are general-purpose, these libraries have different objectives than the State Threads library. The State Threads library is not a general-purpose threading library, but rather an application library that targets only certain types of applications (IAs) in order to achieve the highest possible performance and scalability for those applications.

### 3.2 Scalability

State threads are very lightweight user-level entities, and therefore creating and maintaining user connections requires minimal resources. An application using the State Threads library scales very well with the increasing number of connections.

On multiprocessor systems an application should create multiple processes to take advantage of hardware parallelism. Using multiple separate processes is the only way to achieve the highest possible system scalability. This is because duplicating per-process resources is the only way to avoid significant synchronization overhead on multiprocessor systems. Creating separate UNIX processes naturally offers resource duplication. Again, as in the EDSM architecture, there is no connection between the number of simultaneous connections (which may be very large and changes within a wide range) and the number of kernel entities (which is usually small and constant). In other words, the State Threads library makes it possible to multiplex a large number of simultaneous connections onto a much smaller number of separate processes, thus allowing an application to scale well with both the load and system size.

### 3.3 Performance

Performance is one of the library's main objectives. The State Threads library is implemented to minimize the number of system calls and to make thread creation and context switching as fast as possible. For example, per-thread signal mask does not exist (unlike POSIX threads), so there is no need to save and restore a process's signal mask on every thread context switch. This eliminates two system calls per context switch. Signal events can be handled much more efficiently by converting them to I/O events (see above).

### 3.4 Portability

The library uses the same general, underlying concepts as the EDSM architecture, including non-blocking I/O, file descriptors, and I/O multiplexing. These concepts are available in some form on most UNIX platforms, making the library very portable across many flavors of UNIX. There are only a few platform-dependent sections in the source.

### 3.5 State Threads and NSPR

The State Threads library is a derivative of the Netscape Portable Runtime library (NSPR) [Reference 7]. The primary goal of NSPR is to provide a platform-independent layer for system facilities, where system facilities include threads, thread synchronization, and I/O. Performance and scalability are not the main concern of NSPR. The State Threads library addresses performance and scalability while remaining much smaller than NSPR. It is contained in 8 source files as opposed to more than 400, but provides all the functionality that is needed to write efficient IAs on UNIX-like platforms.


||NSPR|State Threads|
|----|----|----|
|Lines of code|~150,000|~3000|
|Dynamic library size(debug version)| | |
|IRIX|~700 KB|~60 KB|
|Linux|~900 KB|~70 KB|



## Conclusion

State Threads is an application library which provides a foundation for writing Internet Applications. To summarize, it has the following advantages:

It allows the design of fast and highly scalable applications. An application will scale well with both load and number of CPUs.

It greatly simplifies application programming and debugging because, as a rule, no mutual exclusion locking is necessary and the entire application is free to use static variables and non-reentrant library functions.

The library's main limitation:

All I/O operations on sockets must use the State Thread library's I/O functions because only those functions perform thread scheduling and prevent the application's processes from blocking.

## References


- Apache Software Foundation, http://www.apache.org.
- Douglas E. Comer, David L. Stevens, Internetworking With TCP/IP, Vol. III: Client-Server Programming And Applications, Second Edition, Ch. 8, 12.
- W. Richard Stevens, UNIX Network Programming, Second Edition, Vol. 1, Ch. 15.
- Zeus Technology Limited, http://www.zeus.co.uk.
- Peter Druschel, Vivek S. Pai, Willy Zwaenepoel, Flash: An Efficient and Portable Web Server. In Proceedings of the USENIX 1999 Annual Technical Conference, Monterey, CA, June 1999.
- GNU Portable Threads, http://www.gnu.org/software/pth/.
- Netscape Portable Runtime, http://www.mozilla.org/docs/refList/refNSPR/.



## Other resources covering various architectural issues in IAs


- Dan Kegel, The C10K problem, http://www.kegel.com/c10k.html.
- James C. Hu, Douglas C. Schmidt, Irfan Pyarali, JAWS: Understanding High Performance Web Systems, http://www.cs.wustl.edu/~jxh/research/research.html.



Portions created by SGI are Copyright ? 2000 Silicon Graphics, Inc. All rights reserved.

# 网络架构库：StateThreads

## 介绍

StateThreads是一个C的网络程序开发库，提供了编写高性能、高并发、高可读性的网络程序的开发库，支持UNIX-like平台。它结合了多线程编写并行成的简单性，一个进程支持多个并发，支持基于事件的状态机[架构](http://lib.csdn.net/base/architecture)的高性能和高并发能力。

(译注：提供了EDSM的高性能、高并发、稳定性，“多线程”形式的简单编程方式，用setjmp和longjmp实现的一个线程模拟多线程，即用户空间的多线程，类似于现在的协程和纤程)

## 1. 定义

### 1.1 网络程序（Internet Applications）

网络程序（Internet Application）（IA）是一个网络的客户端或者服务器程序，它接受客户端连接，同时可能需要连接到其他服务器。在IA中，数据的到达和发送完毕经常操纵控制流，就是说IA是数据驱动的程序。对每个连接，IA做一些有限的工作，包括和peer的数据交换，peer可能是客户端或服务器。IA典型的事务步骤是：接受连接，读取请求，做一些有限的工作处理请求，将相应写入peer。一个iA的例子是Web服务器，更典型的例子是代理服务器，因为它接受客户端连接，同时也连接到其他服务器。

我们假定IA的性能由CPU决定，而不是由网络带宽或磁盘IO决定，即CPU是系统瓶颈。

### 1.2 性能和可扩展性

IA的性能一般可以用吞吐量来评估，即每秒的事务数，或每秒的字节数（两者可以相互转换，给定事务的平均大小就可以）。有很多种工具可以用来测量Web程序的特定负载，譬如SPECweb96, WebStone, WebBench。尽管对扩展性没有通用的定义，一般而言，可扩展性指系统在外部条件改变时维持它的性能的能力。对于IAs而言，外部条件指连接数（并发），或者底层硬件（CPU数目，内存等）。因此，有两种系统的扩展性：负载能力和系统能力。

（译注：scalability可扩展性，指条件改变了系统是否还能高效运行，譬如负载能力指并发（条件）增多时系统是否能承担这么多负载，系统能力指CPU等增多时是否能高效的利用多CPU达到更强的能力）

下图描述了客户端数目增多时系统的吞吐量的变化，蓝色线条表示理想状况。最开始时吞吐量程线性增长，这个区间系统和CPU较为空闲。继续增长的连接数导致系统开始饱和，吞吐量开始触及天花板（CPU跑满能跑到的吞吐量），在天花板之后吞吐量变为平行线不再增长，因为CPU能力到达了极限。在实际应用中，每个连接消耗了计算资源和内存资源，就算是空闲状态，这些负担都随连接数而增长，因此，实际的IA吞吐量在某个点之后开始往下落（蓝色虚线表示）。开始掉的点，不是其他的原因，而是由系统[架构](http://lib.csdn.net/base/architecture)决定的。

我们将系统有好的负载能力，是指系统在高负载时仍能很好的工作。SPECweb99基准[测试](http://lib.csdn.net/base/softwaretest)能较好的反应系统的负载能力，因为它测量的是连接在最小流量需求时系统能支持的最大连接数（译注：如图中Capacity所指出的点即灰色斜线和蓝色线交叉的点）。而不像SPECweb96或其他的基准[测试](http://lib.csdn.net/base/softwaretest)，是以系统的吞吐量来衡量的（译注：图中Max throughout，即蓝色线的天花板）。

![](http://img.blog.csdn.net/20161122094144669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

系统能力指程序在增加硬件单元例如加CPU时系统的性能，换句话说，好的系统能力意味着CPU加倍时吞吐量会加倍（图中绿色虚线）。我们假设底层[操作系统](http://lib.csdn.net/base/operatingsystem)也具有很好的系统能力。好的系统能力指假设程序在一个小的机器上运行很好，当有需要换到大型服务器上运行时也能获得很高的性能。就是说，改变服务器环境时，系统不需要重写或者费很大的劲。



(译注：

纵坐标是吞吐量，横坐标是连接数。

灰色的线（min acceptable throughout pre client）表示是客户端的需要的吞吐量，至少这个量才流畅。

蓝色表示理想状态的server，系统能力一直没有问题，能达到最大吞吐量，CPU跑满能跑到的吞吐量。

蓝色虚线表示实际的server，每个连接都会消耗CPU和内存，所以在某个临界点之后吞吐量开始往下掉，这个临界点就是系统结构决定的。好的系统架构能将临界点往后推，稳定的支持更高的并发；差的架构在并发增加时可能系统就僵死了。

灰色虚线表示两个测量基准，一个是SPECweb96测量的是系统最大吞吐量，一个是SPECweb99测量每个连接在最小要求流量下系统能达到的最大连接数，后者更能反应系统的负载能力，因为它测量不同的连接的状况下系统的负载能力。

负载能力指的是系统支撑的最大负载，图中的横坐标上的值，对应的蓝色线和灰色线交叉的点，或者是蓝色线往下掉的点。

系统能力指的是增加服务器能力，如加CPU时，系统的吞吐量是否也会增加，图中绿色线表示。好的系统能力会在CPU增加时性能更高，差的系统能力增加CPU也不会更强。

)

尽管性能和扩展性对服务器来讲更重要，客户端也必须考虑这个问题，例如性能测试工具。

### 1.3 并发

并发反应了系统的并行能力，分为虚拟并发和物理并发：

虚拟并发是指[操作系统](http://lib.csdn.net/base/operatingsystem)同时支持很多并发的连接。

物理并发是指硬件设备，例如CPU，网卡，硬盘等，允许系统并行执行任务。

IA必须提供虚拟并发来支持用户的并发访问，为了达到最大的性能，IA创建的由内核调度的编程实体数目基本上和物理并发的数量要保持一致（在一个数量级上）（译注：有多少个CPU就用多少个进程）。内核调度的编程实体即内核执行对象（kernel execution vehicles），包括Solaris轻量级进程，IRIX内核线程。换句话说，内核执行对象应该由物理条件决定，而不是由并发决定（译注：即进程数目应该由CPU决定，而不是由连接数决定）。

## 2. 现有的架构

IAs(Internet Applications)有一些常见的被广泛使用的架构，包括基于进程的架构（Multi-Process）,基于线程的架构（Multi-Threaded）, 和事件驱动的状态机架构（Event-Driven State Machine）。

### 2.1 基于进程的架构：MP

（译注：Multi-Process字面意思是多进程，但事件驱动的状态机EDSM也常用多进程，所以为了区分，使用“基于进程的架构”，意为每个连接一个进程的架构）

在基于进程的架构（MP）中，一个独立的进程用来服务一个连接。一个进程从初始化到服务这个连接，直到服务完毕才服务其他连接。

用户Session是完全独立的，因此，在这些处理不同的连接的进程之间，完全没有同步的必要。因为每个进程有自己独立的地址空间，这种架构非常强壮。若服务某个连接的进程崩溃，其他的连接不会受到任何影响。然而，为了服务很多并发的连接，必须创建相等数量的进程。因为进程是内核对象，实际上是最“重”的一种对象，所以至少需要再内核创建和连接数相等的进程。在大多数的系统中，当创建了上千个进程时，系统性能将大幅降低，因为超负荷的上下文切换。也就是说，MP架构负载能力很弱，无法支持高负载（高并发）。

另一方面，MP架构有很高的系统能力（利用系统资源，稳定性，复杂度），因为不同的进程之间没有共享资源，因而没有同步的负担。

ApacheWeb服务器就是采用的MP架构。

### 2.2 基于线程的架构：MT

（译注：Multi-Threaded字面意思是多线程，但侧重一个线程服务一个连接的方式，用“基于线程”会更准确）

在基于线程（MT）架构中，使用多个独立的线程，它们共享地址空间。和MP结构的进程一样，每个线程独立服务每个连接直到服务完毕，这个线程才用来服务其他连接。

很多现代的UNIX操作系统实现了一个多对一的模型，用来映射用户空间的线程到系统内核对象。在这个模型中，任意多数量的用户空间线程复用少量的内核执行对象，内核执行对象即为虚拟处理器。当用户空间线程调用了一个阻塞的系统调用时，内核执行对象也会在内核阻塞。如果没有其他没有阻塞的内核执行对象，或者有其他需要运行的用户空间线程，一个新的内核执行对象会被自动创建，这样就防止一个线程阻塞时其他线程都被阻塞。

由于IAs由网络IO驱动，所有的并发连接都会阻塞在不同的地方。因此，内核执行对象的数目会接近用户空间线程的数目，也就是连接的数目。此时，多对一的模型就退化为一对一的模型，和MP架构一样，内核执行对象的数目由并发决定而不是由CPU数目决定。和MP一样，这降低了系统的负载能力。尽管这样，由于内核线程是轻量级进程，使用了较少的资源，比内核进程要轻，MT架构比MP架构在负载能力方面稍强一些。

在MT架构中，内核线程共享了地址空间，各种同步锁破坏了系统能力。尽管程序可以很小心的避免锁来提高程序性能（是个复杂的任务），标准库函数和系统调用也会对通用资源上锁，例如，平台提供的线程安全函数，例如内存分配函数（malloc，free等）都是用了一个全局锁。另外一个例子是进程的文件描述表，这个表被内核线程共享，在系统调用（open，close等）时需要保护。除此之外，多核系统中需要在CPU之间维护缓存的一致，当不同的线程运行在不同的CPU上并修改同样的数据时，严重降低了系统的性能。

为了提高负载能力，产生了一些不同类型的MT架构：创建多组线程，每组线程服务一个任务，而不是一个线程服务一个连接。例如，一小组线程负责处理客户端连接的任务，另外一组负责处理请求，其他的负责处理响应。这种架构的主要优点是它对并发和线程解耦了，不再需要同等数量的线程服务连接。尽管这样，线程组之间必须共享任务队列，任务队列需要用锁来保护（典型的生产者-消费者问题）。额外的线程同步负担导致在多处理器系统上性能很低。也就是说，这种架构用系统能力换取了负载能力（用性能换高并发）。

当然，线程编程的噩梦，包括数据破坏，死锁，条件竞争，也导致了任何形式的MT架构无法实用。

### 2.3 基于事件的状态机架构：EDSM

在基于事件驱动的状态机架构（EDSM）中，一个进程用来处理多个并发。Comer和Stevens[Reference 2]描述了这个架构的基础。EDSM架构中，每次每个连接只由数据驱动一步（译注：例如，收一个包，动作一次），因此必须复用多个并发的连接（译注：必须复用一个进程处理多个连接），进程设计成状态机每次收到一个时间就处理并变换到下一个状态。

在空闲状态时，EDSM调用select/poll/epoll等待网络事件，当一个特殊的连接可以读写时，EDSM调用响应的处理函数处理，然后处理下一个连接。EDSM架构使用非阻塞的系统调用完成异步的网络IO。关于非阻塞的IO，请参考Stevens [Reference 3]。

为了利用硬件并行性能，可以创建多个独立的进程，这叫均衡的多进程EDSM，例如ZeusWeb服务器[Reference 4]（译注：商业的高性能服务器）。为了更好的利用多磁盘的IO性能，可以创建一些辅助进程，这叫非均衡的多进程EDSM，例如DruschelWeb服务器[Reference 5]。

EDSM架构可能是IAs的最佳架构，因为并发连接完全和内核进程解耦，这种架构有很高的负载能力，它仅仅需要少量的用户空间的资源来管理连接。

和MP架构一样，多核的EDSM架构也有很高的系统能力（多核性能，稳定性等），因为进程间没有资源共享，所以没有同步锁的负担。

不幸的是，EDSM架构实际上是基于线程的概念（译注：状态机保存的其实就是线程的栈，上次调用的位置，下次继续从这个状态开始执行，和线程是一样的），所以新的EDSM系统需要从头开始实现状态机。实际上，EDSM架构用很复杂的方式模拟了多线程。

## 3. State Threads Library

StateThreads库结合了上面所有架构的优点，它的api提供了像线程一样的编程方式，允许一个并发在一个“线程”里面执行，但这些线程都在一个进程里面。底层的实现和EDSM架构类似，每个并发连接的session在单独的内存空间。

（译注：StateThreads提供的就是EDSM机制，只是将状态机换成了它的“线程”（协程或纤程），这些“线程”实际上是一个进程一个线程实现但表现起来像多线程。所以StateThread的模型是EDSM的高性能和高并发，然后提供了MT的可编程性和简单接口，简化了EDSM的状态机部分。）

### 3.1 状态改变和调度

每个并发的session包含它自己的栈环境（栈指针，PC，CPU寄存器）和它的栈。从概念上讲，一次线程上下文切换相当于进程改变它的状态。当然除了进程之外，并没有使用线程（译注：它是单线程的方式模拟多线程）。和其他通用的线程库不一样，StateThreads库的设计目标很明确。线程上下文切换（进程状态改变）只会在一些函数中才会发生（IO点，或者明确的同步点）。所以，进程级别的数据不需要锁来保护，因为是单线程。整个程序可以自由的使用静态变量和不可重入的函数，极大的简化了编程和调试，从而增加了性能。这实际上是和协程（co-routine）类似，但是不需要显式的用yield指定——线程调用阻塞的IO函数被阻塞而交出控制权是早晚的事。所有的线程（并发连接）都有同样的优先级，所以是非抢占式的调度，和EDSM架构类似。由于IAs是数据驱动（处理流程由网络缓冲区大小和数据到达的次序决定），调度不是按时间切片的。

只有两类的外部事件可以被库的调度器处理，因为只有这类事件能被select/poll检测到：

1. IO事件：一个文件描述符可读写时。

2. 定时器时间：指定了timeout。

尽管这样，其他类型的事件（譬如发送给进程的信号）也能被转换成IO事件来处理。例如，信号处理函数收到信号时可以写入pipe，因此将信号转换成了IO事件。

为了能更好的发挥硬件并行的性能，和EDSM架构一样，可以创建均衡和非均衡的进程。进程管理不是库的功能，而是留给用户处理。

有一些通用的线程库，实现了多对一的模型（多个用户空间的线程，对一个内核执行对象），使用了和StateThreads库类似的技术（非阻塞IO，事件驱动的调度器等）。譬如，GNU Portable Threads [Reference 6]。因为他们是通用库，所以它们和StateThreads有不同的目标。StateThreads不是通用的线程库，而是为少数的需要获得高性能、高并发、高扩展性和可读性的IAs系统而设计的。

### 3.2 可扩展性

StateThreads是非常轻量级的用户空间线程，因此创建和维护用户连接需要很少的资源。使用StateThreads的系统在高并发时能获得很高性能。

多CPU的系统上，程序需要创建多个进程才能利用硬件的平行能力。使用独立的进程是唯一获取高系统能力的方式，因为复制进程的资源是唯一的方式来避免锁和同步这种负担的唯一方式。创建UNIX进程一般会复制进程的资源。再次强调，EDSM架构中，并发的连接和系统对象（进程线程）没有任何的联系，也就是说，StateThreads库将大量并发复用到了少量的独立的进程上，因此获得很高的系统能力和负载能力。

### 3.3 性能

高性能是StateThreads库的主要目标之一，它实现了一系列的系统调用，尽可能的提高线程创建和切换的速度。例如，没有线程级别的信号屏蔽（和POSIX线程不一样），所以线程切换时不需要保存和恢复进程的信号屏蔽字，这样在线程切换时少了两个系统调用。信号事件能被高效的转换成IO事件（如上所述）。

### 3.4 便携性

StateThreads库使用了和EDSM架构同样的基础概念，包括非阻塞IO，文件描述符，IO复用。这些概念在大多数的UNIX平台都通用，所以UNIX下库的通用性很好，只有少数几个平台相关的特性。

### 3.5 State Threads 和 NSPR

StateThreads库是从Netscape Portable Runtime library (NSPR) [Reference 7]发展来的。NSPR主要的目标是提供一个平台无关的系统功能，包括线程，线程同步和IO。性能和可扩展性不是NSPR主要考虑的问题。StateThreads解决了性能和可扩展性问题，但是比NSPR要小很多；它仅仅包含8个源文件，却提供了在UNIX下写高效IAs系统的必要功能：


||NSPR|State Threads|
|----|----|----|
|Lines of code|~150,000|~3000|
|Dynamic library size(debug version)| | |
|IRIX|~700 KB|~60 KB|
|Linux|~900 KB|~70 KB|



## 总结

StateThreads是一个提供了编写IA的基础库，它包含以下优点：

1. 能设计出高效的IA系统，包括很高的负载能力和系统能力。

2. 简化了编程和调试，因为没有同步锁，可以使用静态变量和不可重入函数。

它主要的限制：

1. 所有socket的IO必须要使用库的IO函数，因为调度器可以避免被阻塞（译注：用操作系统的socket的IO函数自然调度器就管不了了）。

## References


- Apache Software Foundation, http://www.apache.org.
- Douglas E. Comer, David L. Stevens, Internetworking With TCP/IP, Vol. III: Client-Server Programming And Applications, Second Edition, Ch. 8, 12.
- W. Richard Stevens, UNIX Network Programming, Second Edition, Vol. 1, Ch. 15.
- Zeus Technology Limited, http://www.zeus.co.uk.
- Peter Druschel, Vivek S. Pai, Willy Zwaenepoel, Flash: An Efficient and Portable Web Server. In Proceedings of the USENIX 1999 Annual Technical Conference, Monterey, CA, June 1999.
- GNU Portable Threads, http://www.gnu.org/software/pth/.
- Netscape Portable Runtime, http://www.mozilla.org/docs/refList/refNSPR/.



## Other resources covering various architectural issues in IAs


- Dan Kegel, The C10K problem, http://www.kegel.com/c10k.html.
- James C. Hu, Douglas C. Schmidt, Irfan Pyarali, JAWS: Understanding High Performance Web Systems, http://www.cs.wustl.edu/~jxh/research/research.html.



译注：

用State Thread写了几个程序。

## 开启10K和30K个线程的程序：




**[cpp]**[view plain](http://blog.csdn.net/caoshangpa/article/details/53282330#)[copy](http://blog.csdn.net/caoshangpa/article/details/53282330#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #include <stdio.h>    
- 
- /*  
- build and execute  
-     gcc -I../obj -g huge_threads.c ../obj/libst.a  -o huge_threads;  
-     ./huge_threads 10000  
- 10K report:  
-     10000 threads, running on 1 CPU 512M machine,  
-     CPU 6%, MEM 8.2% (~42M = 42991K = 4.3K/thread)  
- 30K report:  
-     30000 threads, running on 1CPU 512M machine,  
-     CPU 3%, MEM 24.3% (4.3K/thread)  
- */    
- #include <st.h>     
- 
- void* do_calc(void* arg){    
- int sleep_ms = (int)(long int)(char*)arg * 10;    
- 
- for(;;){    
-         printf("in sthread #%dms\n", sleep_ms);    
-         st_usleep(sleep_ms * 1000);    
-     }    
- 
- return NULL;    
- }    
- 
- int main(int argc, char** argv){    
- if(argc <= 1){    
-         printf("Test the concurrence of state-threads!\n"    
- "Usage: %s <sthread_count>\n"    
- "eg. %s 10000\n", argv[0], argv[0]);    
- return -1;    
-     }    
- 
- if(st_init() < 0){    
-         printf("error!");    
- return -1;    
-     }    
- 
- int i;    
- int count = atoi(argv[1]);    
- for(i = 1; i <= count; i++){    
- if(st_thread_create(do_calc, (void*)i, 0, 0) == NULL){    
-             printf("error!");    
- return -1;    
-         }    
-     }    
- 
-     st_thread_exit(NULL);    
- 
- return 0;    
- }    




原文链接：[http://blog.csdn.net/win_lin/article/details/8242653](http://blog.csdn.net/win_lin/article/details/8242653)









