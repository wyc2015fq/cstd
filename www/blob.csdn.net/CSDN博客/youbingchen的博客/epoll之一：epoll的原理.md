# epoll之一：epoll的原理 - youbingchen的博客 - CSDN博客





2016年06月22日 13:36:33[youbingchen](https://me.csdn.net/youbingchen)阅读数：335








> 
之前 我写过一篇博客[IO复用之select poll epoll 函数](http://blog.csdn.net/youbingchen/article/details/51613932#t5)，大概介绍了一下epoll和select,poll的一些区别，接下来要 从原理剖析epoll的强大的原因


# select和poll的工作机制

在某一时刻，进程收集有事件连接时，大部分的连接是没有发生事件，但是 select和poll的工作机制，在 每次收集事件 的 时候，都把全部 的 连接的 套接字传给操作系统（这首先就是用户态内存到内核态内存的大量复制），同时由操作系统内核寻找这些连接上有没有事件发生，将是一个很耗时的事情，所以 select和epoll只能处理几千个并发连接（也受到操作系统文件描述符的限制）。

# epoll的工作机制

首先epoll在Linux内核中申请了一个 简易的文件系统，将一个 poll或select调用分成了3个部分：
- 调用`epoll_create`建立1个epoll对象（在epoll文件系统中 给这个 句柄 分配资源）
- 调用`epoll_ctl`向`epoll`添加大量的连接套接字
- 调用`epoll_wait`收集发生事件 的连接。

> 
**这样，只需要在进程启动的时候建立 1个epoll对象，并在需要的 时候向他添加或删除连接 就可以，在实际收集事件时，epoll_wait的效率会非常的高，调用epoll_wait时并没有向它传递全部的 连接，内核也不需要遍历全部的 连接**


### 详解epoll_create方法

> 
当一个进程调用epoll_create方法时，Linux内核会创建 一个eventpoll结构体，这个 结构体中有两个 成员与epoll使用 的 方式密切相关


```cpp
struct eventpoll{
 /*红黑树的根节点，这颗树存储着所有添加到epoll的事件，也就是这个epoll监控 的事件*/
 struct rb_root rbr;

 // 双向链表rallist保存着将要 通过 epoll_wait返回给用户的、满足条件的事件 
 struct list_head rdllist;
 ...
}
```

> 
每一个epoll对象都有一个独立的eventpoll结构体，这个 结构体会在内核 空间创造独立 的内存，用于存储使用epoll_ctl方法向epoll对象添加 进来的事件。这些事件都会 挂在rbr红黑树中，这样重复添加的事件就 可以通过红黑树而高效地识别出来。所有添加到epoll的事件都会与设备(如网卡)驱动程序建立 回调关系，也就是说，相应的事件发生时会调用这里的回调方法。这个 回调的方法 在内核叫做ep_poll_callback,它会把这样的事件放在上面的rdllist双向链表中。


在 epoll中，对于每个 事件 都会 建立一个epitem结构体

```cpp
struct epitem
{
    //红黑树节点
    struct rb_node rbn;
    //双链 表节点
    struct list_head rdllink;
    //事件 句柄信息 
    struct epoll_filefd ffd;
    //指向其所属的eventpoll对象
    struct  eventpoll *ep;
    // 期待 的事件类型
    struct epoll_event  event;
}
```

> 
这里 包含 每一个事件对应着的信息，当 调用epoll_wait检查是否有 发生事件的连接时，只是检查eventpoll对象中的rdllist双向链表是否有epitem元素而已，如果rdllist链表不为空，则把这里的事件复制到用户态 的 内存中，同时将事件数量返回给用户。




