# libevent参考手册第四章：与事件一起工作 - u013366022的专栏 - CSDN博客
2016年11月30日 15:19:29[slitaz](https://me.csdn.net/u013366022)阅读数：111
libevent的基本操作单元是事件。每个事件代表一组条件的集合，这些条件包括：
l  文件描述符已经就绪，可以读取或者写入
l  文件描述符变为就绪状态，可以读取或者写入（仅对于边沿触发IO）
l  超时事件
l  发生某信号
l  用户触发事件
所有事件具有相似的生命周期。调用libevent函数设置事件并且关联到event_base之后，事件进入“**已初始化（initialized）**”状态。此时可以将事件添加到event_base中，这使之进入“**未决（pending）**”状态。在未决状态下，如果触发事件的条件发生（比如说，文件描述符的状态改变，或者超时时间到达），则事件进入“**激活（active）**”状态，（用户提供的）事件回调函数将被执行。如果配置为“**持久的（persistent）**”，当执行回调时，事件将保持为未决状态；否则，执行完回调后，事件不再是未决的。删除操作可以让未决事件成为非未决（已初始化）的；添加操作可以让非未决事件再次成为未决的。
## 1 构造事件对象
## 1.1 创建事件
使用event_new（）接口创建事件。
### 接口
#define EV_TIMEOUT      0x01
#define EV_READ         0x02
#define EV_WRITE        0x04
#define EV_SIGNAL       0x08
#define EV_PERSIST      0x10
#define EV_ET           0x20
**typedef****void** (*event_callback_fn)(evutil_socket_t, **short**, **void** *);
**struct** event *event_new(**struct** event_base
 *base, evutil_socket_t fd,
**short** what, event_callback_fn cb,
**void** *arg);
**void** event_free(**struct** event *event);
event_new()试图分配和构造一个用于base的新的事件。what参数是上述标志的集合（它们的语义在下面会描述）。如果fd非负，则它是将被观察其读写事件的文件。事件被激活时，libevent将调用cb函数，cb函数传递这些参数：文件描述符fd，表示所有被触发事件的位字段what，以及构造事件时的arg参数。
发生内部错误，或者传入无效参数时，event_new（）将返回NULL。
所有新创建的事件都处于已初始化和非未决状态，调用event_add（）可以使其成为未决的。
要释放事件，调用event_free（）。对未决或者激活状态的事件调用event_free（）是安全的：在释放事件之前，函数将会使事件成为非激活和非未决的。
### 示例
#include <event2/event.h>
**void** cb_func(evutil_socket_t fd, **short** what, **void** *arg)
{
**const****char** *data
 = arg;
        printf("Got an event on socket %d:%s%s%s%s [%s]",
            (**int**) fd,
            (what&EV_TIMEOUT) ? " timeout" : "",
            (what&EV_READ)    ? " read" : "",
            (what&EV_WRITE)   ? " write" : "",
            (what&EV_SIGNAL)  ? " signal" : "",
            data);
}
**void** main_loop(evutil_socket_t fd1, evutil_socket_t fd2)
{
**struct** event *ev1, *ev2;
**struct** timeval five_seconds = {5,0};
**struct** event_base *base = event_base_new();
*        /* The caller has already set up fd1, fd2 somehow, and make them*
*           nonblocking. */*
        ev1 = event_new(base, fd1, EV_TIMEOUT|EV_READ|EV_PERSIST, cb_func,
           (**char***)"Reading event");
        ev2 = event_new(base, fd2, EV_WRITE|EV_PERSIST, cb_func,
           (**char***)"Writing event");
        event_add(ev1, &five_seconds);
        event_add(ev2, NULL);
        event_base_dispatch(base);
}
上述函数定义在<event2/event.h>中，首次出现在libevent 2.0.1-alpha版本中。event_callback_fn类型首次在2.0.4-alpha版本中作为typedef出现。
## 1.2 事件标志
l  **EV_TIMEOUT**
这个标志表示某超时时间流逝后事件成为激活的。构造事件的时候，EV_TIMEOUT标志是被忽略的：可以在添加事件的时候设置超时，也可以不设置。超时发生时，回调函数的what参数将带有这个标志。
l  **EV_READ**
表示指定的文件描述符已经就绪，可以读取的时候，事件将成为激活的。
l  **EV_WRITE**
表示指定的文件描述符已经就绪，可以写入的时候，事件将成为激活的。
l  **EV_SIGNAL**
用于实现信号检测，请看下面的“构造信号事件”节。
l  **EV_PERSIST**
表示事件是“持久的”，请看下面的“关于事件持久性”节。
l  **EV_ET**
表示如果底层的event_base后端支持边沿触发事件，则事件应该是边沿触发的。这个标志影响EV_READ和EV_WRITE的语义。
从2.0.1-alpha版本开始，可以有任意多个事件因为同样的条件而未决。比如说，可以有两个事件因为某个给定的fd已经就绪，可以读取而成为激活的。这种情况下，多个事件回调被执行的次序是不确定的。
这些标志定义在<event2/event.h>中。除了EV_ET在2.0.1-alpha版本中引入外，所有标志从1.0版本开始就存在了。
## 1.3 关于事件持久性
默认情况下，每当未决事件成为激活的（因为fd已经准备好读取或者写入，或者因为超时），事件将在其回调被执行前成为非未决的。如果想让事件再次成为未决的，可以在回调函数中再次对其调用event_add（）。
然而，如果设置了EV_PERSIST标志，事件就是持久的。这意味着即使其回调被激活，事件还是会保持为未决状态。如果想在回调中让事件成为非未决的，可以对其调用event_del（）。
每次执行事件回调的时候，持久事件的超时值会被复位。因此，如果具有EV_READ|EV_PERSIST标志，以及5秒的超时值，则事件将在以下情况下成为激活的：
l  套接字已经准备好被读取的时候
l  从最后一次成为激活的开始，已经逝去5秒
## 1.4 只有超时的事件
为使用方便，libevent提供了一些以evtimer_开头的宏，用于替代event_*调用来操作纯超时事件。使用这些宏能改进代码的清晰性。
### 接口
#define evtimer_new(base, callback, arg) /
    event_new((base), -1, 0, (callback), (arg))
#define evtimer_add(ev, tv) /
    event_add((ev),(tv))
#define evtimer_del(ev) /
    event_del(ev)
#define evtimer_pending(ev, what, tv_out) /
    event_pending((ev), (what), (tv_out))
除了evtimer_new（）首次出现在2.0.1-alpha版本中之外，这些宏从0.6版本就存在了。
## 1.5 构造信号事件
libevent也可以监测POSIX风格的信号。要构造信号处理器，使用：
### 接口
#define evsignal_new(base, signum, callback, arg) /
event_new(base, signum, EV_SIGNAL|EV_PERSIST, cb, arg)
除了提供一个信号编号代替文件描述符之外，各个参数与event_new（）相同。
### 示例
**struct** event *hup_event;
**struct** event_base *base = event_base_new();
*/* call sighup_function on a HUP signal */*
hup_event = evsignal_new(base, SIGHUP, sighup_function, NULL);
**注意**：信号回调是信号发生后在事件循环中被执行的，所以可以安全地调用通常不能在POSIX风格信号处理器中使用的函数。
**警告**：不要在信号事件上设置超时，这可能是不被支持的。[待修正：真是这样的吗？]
libevent也提供了一组方便使用的宏用于处理信号事件：
### 接口
#define evsignal_add(ev, tv) /
    event_add((ev),(tv))
#define evsignal_del(ev) /
    event_del(ev)
#define evsignal_pending(ev, what, tv_out) /
    event_pending((ev), (what), (tv_out))
evsignal_*宏从2.0.1-alpha版本开始存在。先前版本中这些宏叫做signal_add（）、signal_del（）等等。
### 关于信号的警告
在当前版本的libevent和大多数后端中，每个进程任何时刻只能有一个event_base可以监听信号。如果同时向两个event_base添加信号事件，即使是不同的信号，也只有一个event_base可以取得信号。
kqueue后端没有这个限制。
## 1.6 设置不使用堆分配的事件
出于性能考虑或者其他原因，有时需要将事件作为一个大结构体的一部分。对于每个事件的使用，这可以节省：
l  内存分配器在堆上分配小对象的开销.
l  对event结构体指针取值的时间开销.
l  如果事件不在缓存中，因为可能的额外缓存丢失而导致的时间开销.
对于大多数应用来说，这些开销是非常小的。所以，你应该坚持使用event_new（）函数除非确定在堆上分配事件导致了严重的性能问题。如果将来版本中的event结构体更大，不使用event_new（）可能会导致难以诊断的错误。
不在堆上分配event具有破坏与其他版本libevent二进制兼容性的风险：其他版本中的event结构体大小可能不同。
### 接口
**int** event_assign(**struct** event *event, **struct** event_base
 *base,
    evutil_socket_t fd, **short** what,
**void** (*callback)(evutil_socket_t, **short**, **void** *), **void** *arg);
除了event参数必须指向一个未初始化的事件之外，event_assign（）的参数与event_new（）的参数相同。成功时函数返回0，如果发生内部错误或者使用错误的参数，函数返回-1。
### 示例
#include <event2/event.h>
*/* Watch out!  Including event_struct.h means that your code will not*
* * be binary-compatible with future versions of Libevent. */*
#include <event2/event_struct.h>
#include <stdlib.h>
**struct** event_pair {
         evutil_socket_t fd;
**struct** event read_event;
**struct** event write_event;
};
**void** readcb(evutil_socket_t, **short**, **void** *);
**void** writecb(evutil_socket_t, **short**, **void** *);
**struct** event_pair *event_pair_new(**struct** event_base
 *base, evutil_socket_t fd)
{
**struct** event_pair *p = malloc(**sizeof**(**struct** event_pair));
**if** (!p) **return** NULL;
        p->fd = fd;
        event_assign(&p->read_event, base, fd, EV_READ|EV_PERSIST, readcb, p);
        event_assign(&p->read_event, base, fd, EV_WRITE|EV_PERSIST, writecb, p);
**return** p;
}
也可以用event_assign（）初始化栈上分配的，或者静态分配的事件。
### 警告
不要对已经在event_base中未决的事件调用event_assign（），这可能会导致难以诊断的错误。如果已经初始化和成为未决的，调用event_assign（）之前需要调用event_del（）。
libevent提供了方便的宏将event_assign（）用于仅超时事件或者信号事件。
### 接口
#define evtimer_assign(event, base, callback, arg) /
    event_assign(event, base, -1, 0, callback, arg)
#define evsignal_assign(event, base, signum, callback, arg) /
    event_assign(event, base, signum, EV_SIGNAL|EV_PERSIST, callback, arg)
如果需要使用event_assign（），又要保持与将来版本libevent的二进制兼容性，可以请求libevent告知struct event在运行时应该有多大：
### 接口
size_t event_get_struct_event_size(**void**);
这个函数返回需要为event结构体保留的字节数。再次强调，只有在确信堆分配是一个严重的性能问题时才应该使用这个函数，因为这个函数让代码难以阅读和编写。
注意，将来版本的event_get_struct_event_size()的返回值可能比sizeof(struct event)小，这表示event结构体末尾的额外字节仅仅是保留用于将来版本libevent的填充字节。
下面这个例子跟上面的那个相同，但是不依赖于event_struct.h中的event结构体的大小，而是使用event_get_struct_size（）来获取运行时的正确大小。
### 示例
#include <event2/event.h>
#include <stdlib.h>
*/* When we allocate an event_pair in memory, we'll actually allocate*
* * more space at the end of the structure.  We define some macros*
* * to make accessing those events less error-prone. */*
**struct** event_pair {
         evutil_socket_t fd;
};
*/* Macro: yield the struct event 'offset' bytes from the start of 'p' */*
#define EVENT_AT_OFFSET(p, offset) /
            ((**struct** event*) ( ((**char***)(p))
 + (offset) ))
*/* Macro: yield the read event of an event_pair */*
#define READEV_PTR(pair) /
            EVENT_AT_OFFSET((pair), **sizeof**(**struct** event_pair))
*/* Macro: yield the write event of an event_pair */*
#define WRITEEV_PTR(pair) /
            EVENT_AT_OFFSET((pair), /
**sizeof**(**struct** event_pair)+event_get_struct_event_size())
*/* Macro: yield the actual size to allocate for an event_pair */*
#define EVENT_PAIR_SIZE() /
            (**sizeof**(**struct** event_pair)+2*event_get_struct_event_size())
**void** readcb(evutil_socket_t, **short**, **void** *);
**void** writecb(evutil_socket_t, **short**, **void** *);
**struct** event_pair *event_pair_new(**struct** event_base
 *base, evutil_socket_t fd)
{
**struct** event_pair *p = malloc(EVENT_PAIR_SIZE());
**if** (!p) **return** NULL;
        p->fd = fd;
        event_assign(READEV_PTR(p), base, fd, EV_READ|EV_PERSIST, readcb, p);
        event_assign(WRITEEV_PTR(p), base, fd, EV_WRITE|EV_PERSIST, writecb, p);
**return** p;
}
event_assign（）定义在<event2/event.h>中，从2.0.1-alpha版本开始就存在了。从2.0.3-alpha版本开始，函数返回int，在这之前函数返回void。event_get_struct_event_size（）在2.0.4-alpha版本中引入。event结构体定义在<event2/event_struct.h>中。
## 2 让事件未决和非未决
构造事件之后，在将其添加到event_base之前实际上是不能对其做任何操作的。使用event_add（）将事件添加到event_base。
### 接口
**int** event_add(**struct** event *ev, **const****struct** timeval
 *tv);
在非未决的事件上调用event_add（）将使其在配置的event_base中成为未决的。成功时函数返回0，失败时返回-1。如果tv为NULL，添加的事件不会超时。否则，tv以秒和微秒指定超时值。
如果对已经未决的事件调用event_add（），事件将保持未决状态，并在指定的超时时间被重新调度。
**注意**：不要设置tv为希望超时事件执行的时间。如果在2010年1月1日设置“tv->tv_sec=time(NULL)+10;”，超时事件将会等待40年，而不是10秒。
### 接口
**int** event_del(**struct** event *ev);
对已经初始化的事件调用event_del（）将使其成为非未决和非激活的。如果事件不是未决的或者激活的，调用将没有效果。成功时函数返回0，失败时返回-1。
**注意**：如果在事件激活后，在回调被执行前删除事件，回调将不会执行。
这些函数定义在<event2/event.h>中，从0.1版本就存在了。
## 3 带优先级的事件
多个事件同时触发时，libevent没有定义各个回调的执行次序。可以使用优先级来定义某些事件比其他事件更重要。
在前一章讨论过，每个event_base有与之相关的一个或者多个优先级。在初始化事件之后，但是在添加到event_base之前，可以为其设置优先级。
### 接口
**int** event_priority_set(**struct** event
 *event, **int** priority);
事件的优先级是一个在0和event_base的优先级减去1之间的数值。成功时函数返回0，失败时返回-1。
多个不同优先级的事件同时成为激活的时候，低优先级的事件不会运行。libevent会执行高优先级的事件，然后重新检查各个事件。只有在没有高优先级的事件是激活的时候，低优先级的事件才会运行。
### 示例
#include <event2/event.h>
**void** read_cb(evutil_socket_t, **short**, **void** *);
**void** write_cb(evutil_socket_t, **short**, **void** *);
**void** main_loop(evutil_socket_t fd)
{
**struct** event *important, *unimportant;
**struct** event_base *base;
  base = event_base_new();
  event_base_priority_init(base, 2);
*  /* Now base has priority 0, and priority 1 */*
  important = event_new(base, fd, EV_WRITE|EV_PERSIST, write_cb, NULL);
  unimportant = event_new(base, fd, EV_READ|EV_PERSIST, read_cb, NULL);
  event_priority_set(important, 0);
  event_priority_set(unimportant, 1);
*  /* Now, whenever the fd is ready for writing, the write callback will*
*     happen before the read callback.  The read callback won't happen at*
*     all until the write callback is no longer active. */*
}
如果不为事件设置优先级，则默认的优先级将会是event_base的优先级数目除以2。
这个函数声明在<event2/event.h>中，从1.0版本就存在了。
## 4 检查事件状态
有时候需要了解事件是否已经添加，检查事件代表什么。
### 接口
**int** event_pending(**const****struct** event
 *ev, **short** what, **struct** timeval *tv_out);
#define event_get_signal(ev) /* ... */
evutil_socket_t event_get_fd(**const****struct** event
 *ev);
**struct** event_base *event_get_base(**const****struct** event
 *ev);
**short** event_get_events(**const****struct** event
 *ev);
event_callback_fn event_get_callback(**const****struct** event
 *ev);
**void** *event_get_callback_arg(**const****struct** event
 *ev);
**void** event_get_assignment(**const****struct** event
 *event,
**struct** event_base **base_out,
        evutil_socket_t *fd_out,
**short** *events_out,
        event_callback_fn *callback_out,
**void** **arg_out);
event_pending（）函数确定给定的事件是否是未决的或者激活的。如果是，而且what参数设置了EV_READ、EV_WRITE、EV_SIGNAL或者EV_TIMEOUT等标志，则函数会返回事件当前为之未决或者激活的所有标志。如果提供了tv_out参数，并且what参数中设置了EV_TIMEOUT标志，而事件当前正因超时事件而未决或者激活，则tv_out会返回事件的超时值。
event_get_fd（）和event_get_signal（）返回为事件配置的文件描述符或者信号值。event_get_base（）返回为事件配置的event_base。event_get_events（）返回事件的标志（EV_READ、EV_WRITE等）。event_get_callback（）和event_get_callback_arg（）返回事件的回调函数及其参数指针。
event_get_assignment（）复制所有为事件分配的字段到提供的指针中。任何为NULL的参数会被忽略。
### 示例
#include <event2/event.h>
#include <stdio.h>
*/* Change the callback and callback_arg of 'ev', which must not be*
* * pending. */*
**int** replace_callback(**struct** event
 *ev, event_callback_fn new_callback,
**void** *new_callback_arg)
{
**struct** event_base *base;
    evutil_socket_t fd;
**short** events;
**int** pending;
    pending = event_pending(ev, EV_READ|EV_WRITE|EV_SIGNAL|EV_TIMEOUT,
                            NULL);
**if** (pending) {
*        /* We want to catch this here so that we do not re-assign a*
*         * pending event.  That would be very very bad. */*
        fprintf(stderr,
                "Error! replace_callback called on a pending event!/n");
**return** -1;
    }
    event_get_assignment(ev, &base, &fd, &events,
                         NULL /* ignore old callback */ ,
                         NULL /* ignore old callback argument */);
    event_assign(ev, base, fd, events, new_callback, new_callback_arg);
**return** 0;
}
这些函数声明在<event2/event.h>中。event_pending（）函数从0.1版就存在了。2.0.1-alpha版引入了event_get_fd（）和event_get_signal（）。2.0.2-alpha引入了event_get_base（）。其他的函数在2.0.4-alpha版中引入。
## 5 配置一次触发事件
如果不需要多次添加一个事件，或者要在添加后立即删除事件，而事件又不需要是持久的，则可以使用event_base_once（）。
### 接口
**int** event_base_once(**struct** event_base
 *, evutil_socket_t, **short**,
**void** (*)(evutil_socket_t, **short**, **void** *), **void** *, **const****struct** timeval
 *);
除了不支持EV_SIGNAL或者EV_PERSIST之外，这个函数的接口与event_new（）相同。安排的事件将以默认的优先级加入到event_base并执行。回调被执行后，libevent内部将会释放event结构。成功时函数返回0，失败时返回-1。
不能删除或者手动激活使用event_base_once（）插入的事件：如果希望能够取消事件，应该使用event_new（）或者event_assign（）。
## 6 手动激活事件
极少数情况下，需要在事件的条件没有触发的时候让事件成为激活的。
### 接口
**void** event_active(**struct** event *ev, **int** what, **short** ncalls);
这个函数让事件ev带有标志what（EV_READ、EV_WRITE和EV_TIMEOUT的组合）成为激活的。事件不需要已经处于未决状态，激活事件也不会让它成为未决的。
这个函数定义在<event2/event.h>中，从0.3版本就存在了。
## 7 优化公用超时
当前版本的libevent使用二进制堆算法跟踪未决事件的超时值，这让添加和删除事件超时值具有O(logN)性能。对于随机分布的超时值集合，这是优化的，但对于大量具有相同超时值的事件集合，则不是。
比如说，假定有10000个事件，每个都需要在添加后5秒触发超时事件。这种情况下，使用双链队列实现才可以取得O（1）性能。
自然地，不希望为所有超时值使用队列，因为队列仅对常量超时值更快。如果超时值或多或少地随机分布，则向队列添加超时值的性能将是O（n），这显然比使用二进制堆糟糕得多。
libevent通过放置一些超时值到队列中，另一些到二进制堆中来解决这个问题。要使用这个机制，需要向libevent请求一个“**公用超时(common timeout)**”值，然后使用它来添加事件。如果有大量具有单个公用超时值的事件，使用这个优化应该可以改进超时处理性能。
### 接口
**const****struct** timeval *event_base_init_common_timeout(
**struct** event_base *base, **const****struct** timeval
 *duration);
这个函数需要event_base和要初始化的公用超时值作为参数。函数返回一个指向特别的timeval结构体的指针，可以使用这个指针指示事件应该被添加到O（1）队列，而不是O（logN）堆。可以在代码中自由地复制这个特别的timeval或者进行赋值，但它仅对用于构造它的特定event_base有效。不能依赖于其实际内容：libevent使用这个内容来告知自身使用哪个队列。
### 示例
#include <event2/event.h>
#include <string.h>
*/* We're going to create a very large number of events on a given base,*
* * nearly all of which have a ten-second timeout.  If initialize_timeout*
* * is called, we'll tell Libevent to add the ten-second ones to an O(1)*
* * queue. */*
**struct** timeval ten_seconds = { 10, 0 };
**void** initialize_timeout(**struct** event_base
 *base)
{
**struct** timeval tv_in = { 10, 0 };
**const****struct** timeval
 *tv_out;
    tv_out = event_base_init_common_timeout(base, &tv_in);
    memcpy(&ten_seconds, tv_out, **sizeof**(**struct** timeval));
}
**int** my_event_add(**struct** event *ev, **const****struct** timeval
 *tv)
{
*    /* Note that ev must have the same event_base that we passed to*
*       initialize_timeout */*
**if** (tv && tv->tv_sec == 10 && tv->tv_usec == 0)
**return** event_add(ev, &ten_seconds);
**else**
**return** event_add(ev, tv);
}
与所有优化函数一样，除非确信适合使用，应该避免使用公用超时功能。
这个函数由2.0.4-alpha版本引入。
## 8 从已清除的内存识别事件
libevent的提供了函数，你可以用它来识别出内存已被设置为0（比如说，通过calloc（）分配的，或者使用memset（）或者bzero（）清除了的）的初始化的事件。
### 接口
**int** event_initialized(**const****struct** event
 *ev);
#define evsignal_initialized(ev) event_initialized(ev)
#define evtimer_initialized(ev) event_initialized(ev)
### 警告
这个函数不能可靠地从没有初始化的内存块中识别出已经初始化的事件。除非知道被查询的内存要么是已清除的，要么是已经初始化为事件的，才能使用这个函数。
除非编写一个非常特别的应用，通常不需要使用这个函数。event_new（）返回的事件总是已经初始化的。
### 示例
#include <event2/event.h>
#include <stdlib.h>
**struct** reader {
    evutil_socket_t fd;
};
#define READER_ACTUAL_SIZE() /
    (**sizeof**(**struct** reader)
 + /
     event_get_struct_event_size())
#define READER_EVENT_PTR(r) /
    ((**struct** event *) (((**char***)(r))+**sizeof**(**struct** reader)))
**struct** reader *allocate_reader(evutil_socket_t fd)
{
**struct** reader *r = calloc(1, READER_ACTUAL_SIZE());
**if** (r)
        r->fd = fd;
**return** r;
}
**void** readcb(evutil_socket_t, **short**, **void** *);
**int** add_reader(**struct** reader *r, **struct** event_base
 *b)
{
**struct** event *ev = READER_EVENT_PTR(r);
**if** (!event_initialized(ev))
        event_assign(ev, b, r->fd, EV_READ, readcb, r);
**return** event_add(ev, NULL);
}
event_initialized（）函数从0.3版本就存在了。
## 9 废弃的事件操作函数
2.0版本之前的libevent没有event_assign（）或者event_new（）。替代的是将事件关联到“当前”event_base的event_set（）。如果有多个event_base，需要记得调用event_base_set（）来确定事件确实是关联到当前使用的event_base的。
### 接口
**void** event_set(**struct** event *event,
 evutil_socket_t fd, **short** what,
**void**(*callback)(evutil_socket_t, **short**, **void** *), **void** *arg);
**int** event_base_set(**struct** event_base
 *base, **struct** event *event);
除了使用当前event_base之外，event_set（）跟event_assign（）是相似的。event_base_set（）用于修改事件所关联到的event_base。
event_set（）具有一些用于更方便地处理定时器和信号的变体：evtimer_set（）大致对应evtimer_assign（）；evsignal_set（）大致对应evsignal_assign（）。
2.0版本之前的libevent使用“signal_”作为用于信号的event_set（）等函数变体的前缀，而不是“evsignal_”（也就是说，有signal_set()、signal_add()、signal_del()、signal_pending()和signal_initialized()）。远古版本(0.6版之前)的libevent使用“timeout_”而不是“evtimer_”。因此，做代码考古（code archeology）（注：这个翻译似乎不正确，是否有更专业的术语？比如说，“代码复审”）时可能会看到timeout_add()、timeout_del()、timeout_initialized()、timeout_set()和timeout_pending()等等。
较老版本（2.0版之前）的libevent用宏EVENT_FD()和EVENT_SIGNAL()代表现在的event_get_fd()和event_get_signal()函数。这两个宏直接检查event结构体的内容，所以会妨碍不同版本之间的二进制兼容性。在2.0以及后续版本中，这两个宏仅仅是event_get_fd()和event_get_signal()的别名。
因为2.0之前的版本不支持锁，所以在运行event_base的线程之外的任何线程调用修改事件状态的函数都是不安全的。这些函数包括event_add()、event_del()、event_active()和event_base_once()。
有一个event_once()与event_base_once()相似，只是用于当前event_base。
2.0版本之前EV_PERSIST标志不能正确地操作超时。标志不会在事件激活时复位超时值，而是没有任何操作。
2.0之前的版本不支持同时添加多个带有相同fd和READ/WRITE标志的事件。也就是说，在每个fd上，某时刻只能有一个事件等待读取，也只能有一个事件等待写入。
原文：http://www.wangafu.net/~nickm/libevent-book/Ref4_event.html
