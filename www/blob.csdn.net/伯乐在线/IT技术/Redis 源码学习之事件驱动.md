# Redis 源码学习之事件驱动 - 文章 - 伯乐在线
原文出处： [天凉好个秋](http://beautyboss.farbox.com/post/redis/redisyuan-ma-xue-xi-zhi-shi-jian-qu-dong)
`Redis`基于多路复用技术实现了一套简单的事件驱动库，代码在`ae.h`、`ae.c`以及`ae_epoll.c`、`ae_evport.c`和`ae_kqueue.c`、`ae_select.c`这几个文件中。其中`ae`表示的是`antirez eventloop`的意思。
`Redis`里面包含两种事件类型:`FileEvent`和`TimeEvent`。
`Redis`采用`IO`多路复用技术，所有的事件都是在一个线程中进行处理。`Redis`的事件驱动模型可以以以下为代码进行表示:


```
int main(int argc,char **argv) 
{
    while(true) {
        // 等待事件到来:wait4Event();
        // 处理事件:processEvent()
    }
}
```
在一个死循环中等待事件的到来，然后对事件进行处理，以此往复。这就是一个最经典的网络编程模型。
### 1.基本数据结构
**aeEventLoop**


```
/* State of an event based program */
typedef struct aeEventLoop {
    int maxfd;   /* highest file descriptor currently registered */
    int setsize; /* max number of file descriptors tracked */
    long long timeEventNextId;
    time_t lastTime;     /* Used to detect system clock skew */
    aeFileEvent *events; /* Registered events */
    aeFiredEvent *fired; /* Fired events */
    aeTimeEvent *timeEventHead;
    int stop;
    void *apidata; /* This is used for polling API specific data */
    aeBeforeSleepProc *beforesleep;
} aeEventLoop;
```
`aeEventLoop`是`Redis`中事件驱动模型的核心，封装了整个事件循环，其中每个字段解释如下：
- `maxfd`:已经接受的最大的文件描述符。
- `setsize`:当前循环中所能容纳的文件描述符的数量。
- `timeEventNextId`:下一个时间事件的`ID`.
- `lastTime`:上一次被访问的时间，用来检测系统时钟是否被修改。
- `events`:指针，指向保存所有注册的事件的数组首地址。
- `fired`:指针，保存所有已经买被触发的事件的数组首地址。
- `timeEventHead`:`Redis`用一个链表来存储所有的时间事件，`timeEventHead`是指向这个链表的首节点指针。
- `stop`:停止整个事件循环。
- `apiData`:指针，指向`epoll`结构。
- `beforeSleep`:函数指针。每次实现循环的时候，在阻塞直到时间到来之前，会先调用这个函数。
**aeFileEvent和aeTimeEvent**
这两个结构分别表示文件事件和时间事件，定义如下


```
typedef struct aeFileEvent {
    int mask; /* one of AE_(READABLE|WRITABLE) */
    aeFileProc *rfileProc; // 函数指针，写事件处理
    aeFileProc *wfileProc; // 函数指针，读事件处理
    void *clientData; // 具体的数据
} aeFileEvent;
```
其中`mask`表示文件事件类型掩码，可以是`AE_READABLE`表示是可读事件，`AE_WRITABLE`为可写事件。`aeFileProc`是函数指针。


```
/* Time event structure */
typedef struct aeTimeEvent {
    long long id; // 事件ID
    long when_sec; // 事件触发的时间:s
    long when_ms; // 事件触发的时间:ms
    aeTimeProc *timeProc; // 函数指针
    aeEventFinalizerProc *finalizerProc; // 函数指针:在对应的aeTieEvent节点被删除前调用，可以理解为aeTimeEvent的析构函数
    void *clientData; // 指针，指向具体的数据 
    struct aeTimeEvent *next; // 指向下一个时间事件指针
} aeTimeEvent;
```
**aeFiredEvent**
`aeFiredEvent`结构表示一个已经被触发的事件，结果如下:


```
/* A fired event */
typedef struct aeFiredEvent {
    int fd; // 事件被触发的文件描述符
    int mask; // 被触发事件的掩码，表示被触发事件的类型
} aeFiredEvent;
```
`fd`表示事件发生在哪个文件描述符上面，`mask`用来表示具体事件的类型。
**aeApiState**
`Redis`底层采用`IO`多路复用技术实现高并发，具体实现可以采用`kqueue`、`select`、`epoll`等技术。对于`Linux`来说，`epoll`的性能要优于`select`，所以以`epoll`为例来进行分析。


```
typedef struct aeApiState {
    int epfd;
    struct epoll_event *events;
} aeApiState;
```
`aeApiState`封装了跟`epoll`相关的数据，`epfd`保存`epoll_create()`返回的文件描述符。
### 具体实现细节
**事件循环启动:`aeMain()`**
事件驱动的启动代码位于`ae.c`的`aeMain()`函数中，代码如下:


```
void aeMain(aeEventLoop *eventLoop) {
    eventLoop->stop = 0;
    while (!eventLoop->stop) {
        if (eventLoop->beforesleep != NULL)
            eventLoop->beforesleep(eventLoop);
        aeProcessEvents(eventLoop, AE_ALL_EVENTS);
    }
}
```
从`aeMain()`方法中可以看到，整个事件驱动是在一个`while()`循环中不停地执行`aeProcessEvents()`方法，在这个方法中执行从客户端发送过来的请求。
**初始化:`aeCreateEventLoop()`**
`aeEventLoop`的初始化是在`aeCreateEventLoop()`方法中进行的，这个方法是在`server.c`中的`initServer()`中调用的。实现如下:


```
aeEventLoop *aeCreateEventLoop(int setsize) {
    aeEventLoop *eventLoop;
    int i;
    if ((eventLoop = zmalloc(sizeof(*eventLoop))) == NULL) goto err;
    eventLoop->events = zmalloc(sizeof(aeFileEvent)*setsize);
    eventLoop->fired = zmalloc(sizeof(aeFiredEvent)*setsize);
    if (eventLoop->events == NULL || eventLoop->fired == NULL) goto err;
    eventLoop->setsize = setsize;
    eventLoop->lastTime = time(NULL);
    eventLoop->timeEventHead = NULL;
    eventLoop->timeEventNextId = 0;
    eventLoop->stop = 0;
    eventLoop->maxfd = -1;
    eventLoop->beforesleep = NULL;
    // 调用aeApiCreate()初始化epoll相关的数据
    if (aeApiCreate(eventLoop) == -1) goto err;
    /* Events with mask == AE_NONE are not set. So let's initialize the
     * vector with it. */
    for (i = 0; i < setsize; i++)
        /**
         * 把每个刚新建的aeFileEvent.mask设置为AE_NONE
         * 这点是必须的
         */
        eventLoop->events[i].mask = AE_NONE;
    return eventLoop;
err:
    if (eventLoop) {
        zfree(eventLoop->events);
        zfree(eventLoop->fired);
        zfree(eventLoop);
    }
    return NULL;
}
```
在这个方法中主要就是给`aeEventLoop`对象分配内存然后并进行初始化。其中关键的地方有：
1、调用`aeApiCreate()`初始化`epoll`相关的数据。`aeApiCreate()`实现如下:


```
static int aeApiCreate(aeEventLoop *eventLoop) {
    // 1.分配内存
    aeApiState *state = zmalloc(sizeof(aeApiState));
    if (!state) return -1;
    // 1.分配events内存，epoll_event的大小为setsize
    state->events = zmalloc(sizeof(struct epoll_event)*eventLoop->setsize);
    if (!state->events) {
        zfree(state);
        return -1;
    }
    // 2.调用epoll_create()生成epoll文件描述符，并保存在epfd这个域中
    state->epfd = epoll_create(1024); /* 1024 is just a hint for the kernel */
    if (state->epfd == -1) {
        zfree(state->events);
        zfree(state);
        return -1;
    }
    // 把apidata指针指向第一步中分配的内存地址
    eventLoop->apidata = state;
    return 0;
}
```
在`aeApiCreate()`方法中主要完成以下三件事：
1. 分配`aeApiState`结构需要的内存。
2. 调用`epoll_create()`方法生成`epoll`的文件描述符，并保存在`aeApiState.epfd`字段中。
3. 把第一步分配的`aeApiState`的内存地址保存在`EventLoop->apidata`字段中。
2、初始化`events`中的`mask`字段为为`AE_NONE`。
**生成`fileEvent`:`aeCreateFileEvent()`**
`Redis`使用`aeCreateFileEvent()`来生成`fileEvent`，代码如下:


```
int aeCreateFileEvent(aeEventLoop *eventLoop, int fd, int mask,
        aeFileProc *proc, void *clientData)
{
    // 1. 检查新增的fd是否超过所能容纳最大值
    if (fd >= eventLoop->setsize) {
        errno = ERANGE;
        return AE_ERR;
    }
    aeFileEvent *fe = &eventLoop->events[fd];
    // 2. 调用aeApiAddEvent()方法把对应的fd以mask模式添加到epoll监听器中
    if (aeApiAddEvent(eventLoop, fd, mask) == -1)
        return AE_ERR;
    // 3. 设置相应的字段值
    fe->mask |= mask;
    if (mask & AE_READABLE) fe->rfileProc = proc;
    if (mask & AE_WRITABLE) fe->wfileProc = proc;
    fe->clientData = clientData;
    if (fd > eventLoop->maxfd)
        // 如果有需要则修改maxfd字段的值
        eventLoop->maxfd = fd;
    return AE_OK;
}
```
`aeCreateFileEvent()`方法主要做了一下三件事:
- 检查新增的fd是否超过所能容纳最大值。
- 调用aeApiAddEvent()方法把对应的fd以mask模式添加到epoll监听器中。
设置相应的字段值。其中最关键的步骤是第二步，`aeApiAddEvent()`方法如下：


```
static int aeApiAddEvent(aeEventLoop *eventLoop, int fd, int mask) {
aeApiState *state = eventLoop->apidata;
struct epoll_event ee = {0}; /* avoid valgrind warning */
/* If the fd was already monitored for some event, we need a MOD
 * operation. Otherwise we need an ADD operation. */
int op = eventLoop->events[fd].mask == AE_NONE ?
        EPOLL_CTL_ADD : EPOLL_CTL_MOD;
ee.events = 0;
mask |= eventLoop->events[fd].mask; /* Merge old events */
if (mask & AE_READABLE) ee.events |= EPOLLIN;
if (mask & AE_WRITABLE) ee.events |= EPOLLOUT;
ee.data.fd = fd;
if (epoll_ctl(state->epfd,op,fd,&ee) == -1) return -1;
return 0;
}
```
生成`timeEvent`:`aeCreateTimeEvent()`
`aeCreateTimeEvent()`方法主要是用来生成`timeEvent`节点，其实现比较简单，代码如下所示:


```
long long aeCreateTimeEvent(aeEventLoop *eventLoop, long long milliseconds,
        aeTimeProc *proc, void *clientData,
        aeEventFinalizerProc *finalizerProc)
{
    // 1. 获取id
    long long id = eventLoop->timeEventNextId++;
    aeTimeEvent *te;
    // 2. 分配内存
    te = zmalloc(sizeof(*te));
    if (te == NULL) return AE_ERR;
    // 3. 设置aeTimeEvent的各个字段的值
    te->id = id;
    aeAddMillisecondsToNow(milliseconds,&te->when_sec,&te->when_ms);
    te->timeProc = proc;
    te->finalizerProc = finalizerProc;
    te->clientData = clientData;
    // timeEventHead总是指向最新添加的timeEvent节点
    te->next = eventLoop->timeEventHead;
    eventLoop->timeEventHead = te;
    return id;
}
```
**处理`timeEevnt`:`processTimeEvents()`**
`Redis`在`processTimeEvents()`方法中来处理所有的`timeEvent`，实现如下:


```
static int processTimeEvents(aeEventLoop *eventLoop) {
    int processed = 0;
    aeTimeEvent *te, *prev;
    long long maxId;
    time_t now = time(NULL);
    /**
     * 如果系统时间被调整到将来某段时间然后又被设置回正确的时间,
     * 这种情况下链表中的timeEvent有可能会被随机的延迟执行,因
     * 此在这个情况下把所有的timeEvent的触发时间设置为0表示及执行
     */
    if (now < eventLoop->lastTime) {
        te = eventLoop->timeEventHead;
        while(te) {
            te->when_sec = 0;
            te = te->next;
        }
    }
    eventLoop->lastTime = now; // 设置上次运行时间为now
    prev = NULL;
    te = eventLoop->timeEventHead;
    maxId = eventLoop->timeEventNextId-1;
    while(te) {
        long now_sec, now_ms;
        long long id;
        /**
         * 删除已经被标志位 删除 的时间事件
         */
        if (te->id == AE_DELETED_EVENT_ID) {
            aeTimeEvent *next = te->next;
            if (prev == NULL)
                eventLoop->timeEventHead = te->next;
            else
                prev->next = te->next;
            if (te->finalizerProc)
                // 在时间事件节点被删除前调用finlizerProce()方法
                te->finalizerProc(eventLoop, te->clientData);
            zfree(te);
            te = next;
            continue;
        }
        if (te->id > maxId) {
            /**
             * te->id > maxId 表示当前te指向的timeEvent为当前循环中新添加的,
             * 对于新添加的节点在本次循环中不作处理。
             * PS:为什么会出现这种情况呢？有可能是在timeProc()里面会注册新的timeEvent节点?
             * 对于当前的Redis版本中不会出现te->id > maxId这种情况
             */
            te = te->next;
            continue;
        }
        aeGetTime(&now_sec, &now_ms);
        if (now_sec > te->when_sec ||
            (now_sec == te->when_sec && now_ms >= te->when_ms))
        {
            // 如果当前时间已经超过了对应的timeEvent节点设置的触发时间，
            // 则调用timeProc()方法执行对应的任务
            int retval;
            id = te->id;
            retval = te->timeProc(eventLoop, id, te->clientData);
            processed++;
            if (retval != AE_NOMORE) {
                // 要执行多次，则计算下次执行时间
                aeAddMillisecondsToNow(retval,&te->when_sec,&te->when_ms);
            } else {
                // 如果只需要执行一次，则把id设置为-1，再下次循环中删除
                te->id = AE_DELETED_EVENT_ID;
            }
        }
        prev = te;
        te = te->next;
    }
    return processed;
}
```
在这个方法中会
- 判断系统时间有没有调整过，如果调整过，则会把`timeEvent`链表中的所有的`timeEvent`的触发时间设置为0，表示立即执行。
- 对`timeEvent`链表进行遍历，对于每个`timeEvent`节点，如果有：
- 如果已经被标记为删除(`AE_DELETED_EVENT_ID`)，则立即释放对应节点内存，遍历下个节点。
- 如果`id`大于`maxId`,则表示当前节点为本次循环中新增节点，咋本次循环中不错处理，继续下个节点。
- 如果当前节点的触发时间大于当前时间，则调用对应节点的`timeProc()`方法执行任务。根据`timeProc()`方法的返回，又分为两种情况:
- 返回为`AE_NOMORE`，表示当前`timeEvent`节点属于一次性事件，标记该节点`ID`为`AE_DELETED_EVENT_ID`,表示删除节点，该节点将会在下一轮的循环中被删除。
- 返回不是`AE_NOMORE`，表示当前`timeEvent`节点属于周期性事件，需要多次执行，调用`aeAddMillisecondsToNow()`方法设置下次被执行时间。
**处理所有事件:`aeProcessEvents()`**
`Redis`中所有的事件，包括`timeEvent`和`fileEvent`都是在`aeProcessEvents()`方法中进行处理的，刚方法实现如下：


```
/* Process every pending time event, then every pending file event
 * (that may be registered by time event callbacks just processed).
 * Without special flags the function sleeps until some file event
 * fires, or when the next time event occurs (if any).
 *
 * If flags is 0, the function does nothing and returns.
 * if flags has AE_ALL_EVENTS set, all the kind of events are processed.
 * if flags has AE_FILE_EVENTS set, file events are processed.
 * if flags has AE_TIME_EVENTS set, time events are processed.
 * if flags has AE_DONT_WAIT set the function returns ASAP until all
 * the events that's possible to process without to wait are processed.
 *
 * The function returns the number of events processed. */
int aeProcessEvents(aeEventLoop *eventLoop, int flags)
{
    int processed = 0, numevents;
    /**
     * 既没有时间事件也没有文件事件，则直接返回
     */
    if (!(flags & AE_TIME_EVENTS) && !(flags & AE_FILE_EVENTS)) return 0;
    /**
     * -1 == eventloop->maxfd 表示还么有任何aeFileEvent被添加到epoll    
     * 事件循环中进行监听
     */
    if (eventLoop->maxfd != -1 ||
        ((flags & AE_TIME_EVENTS) && !(flags & AE_DONT_WAIT))) {
        int j;
        aeTimeEvent *shortest = NULL;
        struct timeval tv, *tvp;
        /**
         * 如果有aeFileEvent需要处理，就先要从所有待处理的
         * aeTimeEvent事件中找到最近的将要被执行的aeTimeEvent节点
         * 并结算该节点触发时间
         */
        if (flags & AE_TIME_EVENTS && !(flags & AE_DONT_WAIT))
            shortest = aeSearchNearestTimer(eventLoop);
        if (shortest) {
            long now_sec, now_ms;
            aeGetTime(&now_sec, &now_ms);
            tvp = &tv;
            /* How many milliseconds we need to wait for the next
             * time event to fire? */
            // 计算epoll_wait()需要等待的时间
            long long ms =
                (shortest->when_sec - now_sec)*1000 +
                shortest->when_ms - now_ms;
            if (ms > 0) {
                tvp->tv_sec = ms/1000;
                tvp->tv_usec = (ms % 1000)*1000;
            } else {
                tvp->tv_sec = 0;
                tvp->tv_usec = 0;
            }
        } else {
            // 如果flags设置了AE_DONT_WAIT,则设置epoll_wait()等待时间为0，
            // 即立刻从epoll中返回
            if (flags & AE_DONT_WAIT) {
                tv.tv_sec = tv.tv_usec = 0;
                tvp = &tv;
            } else {
                /* Otherwise we can block */
                tvp = NULL; /* wait forever */
            }
        }
        // 调用aeApiPoll()进行阻塞等待事件的到来，等待时间为tvp
        numevents = aeApiPoll(eventLoop, tvp);
        for (j = 0; j < numevents; j++) {
            aeFileEvent *fe = &eventLoop->events[eventLoop->fired[j].fd];
            int mask = eventLoop->fired[j].mask;
            int fd = eventLoop->fired[j].fd;
            int rfired = 0;
            /* note the fe->mask & mask & ... code: maybe an already processed
             * event removed an element that fired and we still didn't
             * processed, so we check if the event is still valid. */
            // fe->mask && mask 的目的是确保对应事件时候还有效
            if (fe->mask & mask & AE_READABLE) {
                rfired = 1;
                fe->rfileProc(eventLoop,fd,fe->clientData,mask);
            }
            if (fe->mask & mask & AE_WRITABLE) {
                if (!rfired || fe->wfileProc != fe->rfileProc)
                    fe->wfileProc(eventLoop,fd,fe->clientData,mask);
            }
            processed++;
        }
    }
    /* Check time events */
    if (flags & AE_TIME_EVENTS)
        // 处理aeTimeEvent
        processed += processTimeEvents(eventLoop);
    return processed; /* return the number of processed file/time events */
}
```
该方法的入参`flag`表示要处理哪些事件，可以取以下几个值 ：
- `AE_ALL_EVENTS`:`timeEvent`和`fileEvent`都会处理。
- `AE_FILE_EVENTS`:只处理`fileEvent`。
- `AE_TIME_EVENTS`:只处理`timeEvent`。
- `AE_DONT_WAIT`:要么立马返回，要么处理完那些不需要等待的事件之后再立马返回。
`aeProcessEvents()`方法会做下面几件事:
- 判断传入的`flag`的值，如果既不包含`AE_TIME_EVENTS`也不包含`AE_FILE_EVENTS`则直接返回。
- 计算如果有`aeFileEvent`事件需要进行处理，则先计算`epoll_wait()`方法需要阻塞等待的时间，计算方式如下:
- 先从`aeTimeEvent`事件链表中找到最近的需要被触发的`aeTimeEvent`节点并计算需要被触发的时间，该被触发时间则为`epoll_wait()`需要等待的时间。
- 如果没有找到最近的`aeTimeEvent`节点，表示没有`aeTimeEvent`节点被加入链表，则判断传入的`flags`是否包含`AE_DONT_WAIT`选项，则设置`epoll_wait()`需要等待时间为0，表示立即返回。
- 如果没有设置`AE_DONT_WAIT`,则设置需要等待时间为`NULL`,表示`epoll_wait()`一直阻塞等待知道有`fileEvent`事件到来。
- 调用`aeApiPoll()`方法阻塞等待事件的到来，阻塞时间为第二步中计算的时间。`aeApiPoll()`实现如下：


```
static int aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp) {
    aeApiState *state = eventLoop->apidata;
    int retval, numevents = 0;
    // 1. 根据传入的tvp计算需要等待时间，然后调用epoll_wait()进行阻塞等待
    retval = epoll_wait(state->epfd,state->events,eventLoop->setsize,
            tvp ? (tvp->tv_sec*1000 + tvp->tv_usec/1000) : -1);
    // 有事件到来
    if (retval > 0) {
        int j;
        numevents = retval;
        for (j = 0; j < numevents; j++) {
            int mask = 0;
            struct epoll_event *e = state->events+j;
            // 2. 计算到来的event的类型
            if (e->events & EPOLLIN) mask |= AE_READABLE;
            if (e->events & EPOLLOUT) mask |= AE_WRITABLE;
            if (e->events & EPOLLERR) mask |= AE_WRITABLE;
            if (e->events & EPOLLHUP) mask |= AE_WRITABLE;
            // 3. 把有事件发生的fd以及对应的mask类型拷贝到eventloop->fired数组中
            eventLoop->fired[j].fd = e->data.fd;
            eventLoop->fired[j].mask = mask;
        }
    }
    return numevents;
}
```
`aeApiPoll()`会做下面几件事:- 根据传入的`tvp`计算需要阻塞的时间，然后调用`epoll_wait()`进行阻塞等待。
- 有事件到来之后先计算对应事件的类型。
- 把事件发生的`fd`以及对应的类型`mask`拷贝到`fired`数组中。
- 从`aeApiPoll()`方法返回之后，所有事件已经就绪了的`fd`以及对应事件的类型`mask`已经保存在`eventLoop->fired[]`数组中。依次遍历`fired`数组，根据`mask`类型，执行对应的`frileProc()`或者`wfileProce()`方法。
- 如果传入的`flags`中有`AE_TIME_EVENTS`,则调用`processTimeEvents()`执行所有已经到时间了的`timeEvent`。
