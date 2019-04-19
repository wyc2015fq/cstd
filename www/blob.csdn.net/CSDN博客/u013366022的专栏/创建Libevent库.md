# 创建Libevent库 - u013366022的专栏 - CSDN博客
2016年11月30日 15:01:10[slitaz](https://me.csdn.net/u013366022)阅读数：260
## Setting up the Libevent library
Libevent有一些被整个进程共享的全局设置.这些设置会影响到整个库.在使用Libevent库的其余部分之前,你应该先对这些设置做些修改.否则的话Libevent可能会以一种前后矛盾的状态终止( If you don’t, Libevent could wind up in an inconsistent state).
### Libevent中的日志信息
Libevent可以将内部的错误和警告记录日志.它同样可以记录debug信息如果Libevent编译的时候是以支持日志的方式编译的话。默认的话,这些信息是输出到stderr的.你可以通过提供你自己的日志函数重载这种行为.
Interface
```
#define EVENT_LOG_DEBUG 0
#define EVENT_LOG_MSG   1
#define EVENT_LOG_WARN  2
#define EVENT_LOG_ERR   3
/* Deprecated; see note at the end of this section */
#define _EVENT_LOG_DEBUG EVENT_LOG_DEBUG
#define _EVENT_LOG_MSG   EVENT_LOG_MSG
#define _EVENT_LOG_WARN  EVENT_LOG_WARN
#define _EVENT_LOG_ERR   EVENT_LOG_ERR
typedef void (*event_log_cb)(int severity, const char *msg);
void event_set_log_callback(event_log_cb cb);
```
你可以通过写一个自己的函数来重载Libevent的日志行为,函数类型是event_log_cb这种,然后将其作为参数传递给event_set_log_callback()。无论何时Libevent需要将一条消息记录日志的时候,它都会被消息传递给你提供的函数.你可以通过给event_set_log_callback()传递一个NULL参数来恢复Libevent的默认日志行为.
Examples
```
#include <event2/event.h>
#include <stdio.h>
static void discard_cb(int severity, const char *msg)
{
    /* This callback does nothing. */
}
static FILE *logfile = NULL;
static void write_to_file_cb(int severity, const char *msg)
{
    const char *s;
    if (!logfile)
        return;
    switch (severity) {
        case _EVENT_LOG_DEBUG: s = "debug"; break;
        case _EVENT_LOG_MSG:   s = "msg";   break;
        case _EVENT_LOG_WARN:  s = "warn";  break;
        case _EVENT_LOG_ERR:   s = "error"; break;
        default:               s = "?";     break; /* never reached */
    }
    fprintf(logfile, "[%s] %s\n", s, msg);
}
/* Turn off all logging from Libevent. */
void suppress_logging(void)
{
    event_set_log_callback(discard_cb);
}
/* Redirect all Libevent log messages to the C stdio file 'f'. */
void set_logfile(FILE *f)
{
    logfile = f;
    event_set_log_callback(write_to_file_cb);
}
```
NOTE
在用户自定义的event_log_cb回调内部调用Libevent函数是不安全的.例如,如果你试图写一个日志回调函数,在这个函数内,你使用了bufferevents向一个网络socket发送warning信息,你很可能遇到奇怪的难以调试的bug.这种约束(注：指不能在自定义的回调函数内部用libevent的函数)在未来版本的Libevent中可能会去除.
通常来讲,debug日志并未启用,也不会被发送给日志回调函数.如果Libevent被编译为支持记录日志的话,你可以手动开启这个功能.
Interface
#define EVENT_DBG_NONE 0
#define EVENT_DBG_ALL 0xffffffffu
**void** event_enable_debug_logging(ev_uint32_t which);
在大多数情况下,调试日志信息都是冗长且无用的.调用event_enable_debug_logging()时传参EVENT_DBG_NONE将获得默认的行为,调用event_enable_debug_logging()时传参EVENT_DBG_ALL将开启所有的调试日志。未来版本的Libevent可能会增加更加细粒度的选项.
这些函数声明于<event2/event.h>.event_enable_debug_logging()首次出现于Libevent 2.1.1-alpha,其余的首次出现于Libevent 1.0c。
COMPATIBILITY NOTE
在Libevent 2.0.19-stable之前,EVENT_LOG_*宏的名字是以下划线开头的：_EVENT_LOG_DEBUG, _EVENT_LOG_MSG, _EVENT_LOG_WARN,以及_EVENT_LOG_ERR.这些名字不建议继续使用,应该只为了后向兼容Libevent 2.0.18-stable及更早的版本才使用。在未来版本的Libevent中会移除他们.
### 处理严重错误
当Libevent检测到一个不可恢复的内部错误(例如一个毁坏的数据结构),它的默认行为是调用exit()或者abort()来退出当前正在运行的进程.这些错误通常意味着某处有一个Bug：要么在你的代码内部,要么就是Libevent自身有bug.
如果你希望你的应用程序更优雅地处理这些严重错误,你可以提供一个函数供Libevent调用来代替exit().
Interface
**typedef void** (*event_fatal_cb)(**int** err);
**void** event_set_fatal_callback(event_fatal_cb cb);
首先定义一个Libevent遇到严重错误时应该调用的函数,然后将其传递给event_set_fatal_callback().这样在Libevent遇到严重错误时,就会调用你提供的函数.
你的函数不应该将控制权交回给Libevent,否则可能引起未定义的行为,而且Libevent为了避免程序崩溃还是会退出.一旦你的函数被调用了,你就不该再调用任何Libevent的函数了.
这些函数声明于<event2/event.h>.首次出现于Libevent 2.0.3-alpha。
### 内存管理
默认地,Libevent使用C标准库的内存管理函数从堆上分配内存.你也可以让Libevent使用你提供的内存分配器代替malloc,realloc,free.你想这么做可能因为你有更高效的内存分配器,或是你有一个带内存泄露检测功能的内存分配器.
Interface
**void** event_set_mem_functions(**void** *(*malloc_fn)(size_t sz),
**void** *(*realloc_fn)(**void** *ptr, size_t sz),
                             **void** (*free_fn)(**void** *ptr));
下面是一个简单的例子,用能统计分配了多少字节的replacement_malloc,replacement_realloc代替了Libevent原本的内存分配函数.实际上,对下面的例子你可能想要做一些加锁处理以避免在多线程情况下出现错误.
```
#include <event2/event.h>
#include <sys/types.h>
#include <stdlib.h>
/* This union's purpose is to be as big as the largest of all the
 * types it contains. */
union alignment {
    size_t sz;
    void *ptr;
    double dbl;
};
/* We need to make sure that everything we return is on the right
   alignment to hold anything, including a double. */
#define ALIGNMENT sizeof(union alignment)
/* We need to do this cast-to-char* trick on our pointers to adjust
   them; doing arithmetic on a void* is not standard. */
#define OUTPTR(ptr) (((char*)ptr)+ALIGNMENT)
#define INPTR(ptr) (((char*)ptr)-ALIGNMENT)
static size_t total_allocated = 0;
static void *replacement_malloc(size_t sz)
{
    void *chunk = malloc(sz + ALIGNMENT);
    if (!chunk) return chunk;
    total_allocated += sz;
    *(size_t*)chunk = sz;
    return OUTPTR(chunk);
}
static void *replacement_realloc(void *ptr, size_t sz)
{
    size_t old_size = 0;
    if (ptr) {
        ptr = INPTR(ptr);
        old_size = *(size_t*)ptr;
    }
    ptr = realloc(ptr, sz + ALIGNMENT);
    if (!ptr)
        return NULL;
    *(size_t*)ptr = sz;
    total_allocated = total_allocated - old_size + sz;
    return OUTPTR(ptr);
}
static void replacement_free(void *ptr)
{
    ptr = INPTR(ptr);
    total_allocated -= *(size_t*)ptr;
    free(ptr);
}
void start_counting_bytes(void)
{
    event_set_mem_functions(replacement_malloc,
                            replacement_realloc,
                            replacement_free);
}
```
NOTES
- 
替换掉Libevent的内存管理函数会影响之后所有涉及到分配内存,调整已分配内存大小,释放内存的调用.所以(如果你想用自己的内存管理函数)你要确保在你调用Libevent函数之前已经替换了这些函数.否则的话,有可能你用C库的malloc分配了一块内存,但是在释放内存的时候却调用了自己提供的free()函数.
- 
你的malloc和realloc函数应该返回和C库一样进行内存对其的内存块.
- 
你的realloc函数应该能正确处理realloc(NULL,sz).(也就是把realloc(NULL,sz)当作malloc(sz))
- 
你的realloc函数应该能正确处理realloc(ptr,0).(也就是把realloc(NULL,sz)当作free(ptr))
- 
你的free函数没有必要处理free(NULL).
- 
你的malloc函数没有必要处理malloc(0).
- 
你提供的内存管理函数应该是线程安全的,如果你想在多个线程内使用Libevent.
- 
Libevent使用这些函数分配内存并返回给你.所以如果你想释放一块由你自己版本的malloc和realloc函数分配的内存,你必须用自己版本的free函数释放这部分内存
event_set_mem_functions()声明于<event2/event.h>.该函数首次出现于Libevent 2.0.1-alpha.
Libevent可以编译为不支持event_set_mem_functions().如果这样的话,在程序内使用event_set_mem_functions() 将无法编译或链接.在Libevent 2.0.2-alpha及之后的版本,你可以通过检查是否定义了EVENT_SET_MEM_FUNCTIONS_IMPLEMENTED宏来判断event_set_mem_functions()是否存在.
### 锁和线程
你可能已经知道了当你写多线程程序时,在多个线程内同时访问同一份数据的时候往往是不安全的.
Libevent中的结构在多线程下有3种情况
- 
有些结构天然地就是单线程:无论何时,多线程内同时访问都是不安全的.
- 
有些结构是可选加锁的(optionally locked).你可以针对每一个对象告知Libevent你是否希望立刻在多线程下使用它.
- 
有些结构总是加锁的.如果Libevent正以支持锁的方式运行的话,这些结构在多线程下总是安全的.
在Libevent中你想获取锁的话,你要告诉Libevent你想使用哪一个locking函数.如果你要调用一个Libevent函数,这个函数要分配一个在多线程内共享的结构.那么在这之前要先告知Libevent使用哪一个locking函数.
如果你在使用pthreads库或者原生的windows线程代码,那么你走运了.已有一些预定义好的函数使得Libevent使用正确的pthreads或者windows函数.
Interface
```
#ifdef WIN32
int evthread_use_windows_threads(void);
#define EVTHREAD_USE_WINDOWS_THREADS_IMPLEMENTED
#endif
#ifdef _EVENT_HAVE_PTHREADS
int evthread_use_pthreads(void);
#define EVTHREAD_USE_PTHREADS_IMPLEMENTED
#endif
```
上面的函数均是成功返回0,失败返回-1.
如果你想使用一个新的线程库,那么你还是有点工作要做的.你需要定义一些函数使用你自己的线程库实现下面内容:
- 
Locks
- 
locking
- 
unlocking
- 
lock allocation
- 
lock destruction
- 
Conditions
- 
condition variable creation
- 
condition variable destruction
- 
waiting on a condition variable
- 
signaling/broadcasting to a condition variable
- 
Threads
- 
thread ID detection
然后使用evthread_set_lock_callbacks和evthread_set_id_callback接口通知Libevent使用这些函数.
Interface
```
#define EVTHREAD_WRITE  0x04
#define EVTHREAD_READ   0x08
#define EVTHREAD_TRY    0x10
#define EVTHREAD_LOCKTYPE_RECURSIVE 1
#define EVTHREAD_LOCKTYPE_READWRITE 2
#define EVTHREAD_LOCK_API_VERSION 1
struct evthread_lock_callbacks {
       int lock_api_version;
       unsigned supported_locktypes;
       void *(*alloc)(unsigned locktype);
       void (*free)(void *lock, unsigned locktype);
       int (*lock)(unsigned mode, void *lock);
       int (*unlock)(unsigned mode, void *lock);
};
int evthread_set_lock_callbacks(const struct evthread_lock_callbacks *);
void evthread_set_id_callback(unsigned long (*id_fn)(void));
struct evthread_condition_callbacks {
        int condition_api_version;
        void *(*alloc_condition)(unsigned condtype);
        void (*free_condition)(void *cond);
        int (*signal_condition)(void *cond, int broadcast);
        int (*wait_condition)(void *cond, void *lock,
            const struct timeval *timeout);
};
int evthread_set_condition_callbacks(
        const struct evthread_condition_callbacks *);
```
evthread_lock_callbacks结构描述了与锁有关的回调及他们的功能.就上述版本而言,lock_api_version字段必须设为EVTHREAD_LOCK_API_VERSION.supported_locktypes字段必须设置为EVTHREAD_LOCKTYPE_*常量的位掩码,以表明支持何种锁类型(以2.0.4-alpha为例,EVTHREAD_LOCK_RECURSIVE(递归锁)是强制的,而EVTHREAD_LOCK_READWRITE(读写锁)是可以不使用的).alloc函数返回一个特定类型的新锁.free函数释放某个锁所持有的全部相关资源.lock函数必须以指定模式获取锁,成功返回0,失败返回非0.unlock函数解锁一把锁,成功返回0,失败返回非0.
已知的锁类型如下:
- 0
一个常规的,非必须的递归锁.
- EVTHREAD_LOCKTYPE_RECURSIVE
一种不会阻止当前已持有该锁的线程再一次获取该锁行为的锁.别的线程可以在当前持有该锁的线程释放锁后获取该锁,当前持有该锁的线程获取几次该锁,别的线程就能获取几次.
(注:同一个线程可以多次获取同一个递归锁，不会产生死锁。而如果一个线程多次获取同一个非递归锁，则会产生死锁)
- EVTHREAD_LOCKTYPE_READWRITE
一种允许多条线程在"读"时均可持有的锁,但是在"写"时只允许一个线程持有它.一个writer排除所有reader.
已知的锁模式如下:
- EVTHREAD_READ
For READWRITE locks only: acquire or release the lock for reading.
仅对读写锁有效:获取或释放锁以用以"读".
- EVTHREAD_WRITE
For READWRITE locks only: acquire or release the lock for writing.
仅对读写锁有效:获取或释放锁以用以"写".
- EVTHREAD_TRY
仅对locking有效：仅在锁可以立即获取到的情况下获取锁.
id_fn参数代表一个返回值类型为unsigned long的函数,返回值标识了一条线程.对同一条线程,该函数要返回同一个数,对同一时刻运行的不同线程,该函数不能返回同一数值.
evthread_condition_callbacks结构描述了与条件变量相关的回调.就上述版本而言,lock_api_version字段必须设置为EVTHREAD_CONDITION_API_VERSION.alloc_condition()函数必须返回一个指向新的条件变量的指针.它接受0作为其参数.free_condition()函数必须释放由一个条件变量所持有的资源.wait_condition()函数接受3个参数,一个alloc_condition分配的条件变量,一个由evthread_lock_callbacks分配的锁,一个可选的超时时间.一旦该函数被调用,它就持有锁,直到条件被触发或是达到超时时间.wait_condition()在错误时返回-1,条件触发返回0,超时返回1.在其返回前,它需要确保确实持有锁.最后,signal_condition()应该使等待某个条件的一条线程而被唤醒(如果broadcast参数为false)或所有等待某个条件的线程被唤醒(如果broadcast参数为true).只有在持有与条件相关的锁的情况下,这种情况(指线程的唤醒)才会发生.
更多与条件变量有关的信息,请查阅pthreads的pthread_cond_*函数或是windows的CONDITION_VARIABLE函数的相关文档.
Examples
```
For an example of how to use these functions, see evthread_pthread.c and
evthread_win32.c in the Libevent source distribution.
```
这部分介绍的函数声明于<event2/thread.h>.这些函数中的大部分首次出现于Libevent 2.0.4-alpha.2.0.1-alpha到2.0.3-alpha版本使用旧的接口来设置锁相关函数.
event_use_pthreads()函数要求你的程序链接event_pthreads库.condition-variable函数使Libevent 2.0.7-rc中加入的,以解决某些棘手的死锁问题.
Libevent可以编译为不支持锁.如果这样(指编译为不支持锁)的话,那么使用了上述线程相关函数的程序将无法运行.
### 调试锁使用
为了帮助调试锁的使用,Libevent有一个可选的“lock debugging”特性,它包装了与锁相关的调用以捕获典型的锁错误,包括：
- 
解锁一个我们并未持有的锁
- 
重新锁定(re-locking)一个非递归锁
如果上述之一lock错误出现了,Libevent将会以断言失败(assertion failure)退出.
Interface
```
void evthread_enable_lock_debugging(void);
#define evthread_enable_lock_debuging() evthread_enable_lock_debugging()
```
Note 该函数必须在锁创建或使用之前调用.为了安全起见,在你设置了线程函数以后就调用该函数.
该函数是于Libevent 2.0.4-alpha引入的,2.0.4中它的名字有一个拼写错误,“evthread_enable_lock_debuging()”.在2.1.1-alpha中修正了为evthread_enable_lock_debugging(),目前这两个名字都是支持的.
### 调试事件使用
在使用events时有一些常见错误,Libevent可以检测到并报告给你.包括：
- 
将一个未初始化的event视为已初始化的
- 
尝试再一次初始化一个已经处于监控中的事件.
追踪哪些事件已初始化会导致Libevent消耗额外的内存和CPU,所以你应该只在你真的需要debug你的程序的时候再开启debug模式.
Interface
```
void event_enable_debug_mode(void);
```
该函数必须在event_base创建之前调用.
使用debug模式时,如果你的程序使用event_assign()[不是event_new()]创建了大量的事件,那么有可能耗尽你机器的内存。之所以出现这种情况,是因为Libevent无法判断使用event_assign()创建的事件何时不再使用.(当你对一个event_new()创建的事件调用event_free()的时候,Libevent可以得知该event已经无效).你如果想避免在debug时耗尽内存,你可以显示地告知Libevent这些事件将不再委派(给Libevent监听).
Interface
```
void event_debug_unassign(struct event *ev);
```
在debug模式未开启的状态下,调用event_debug_unassign()是无效的的.
Example
```
#include <event2/event.h>
#include <event2/event_struct.h>
#include <stdlib.h>
void cb(evutil_socket_t fd, short what, void *ptr)
{
    /* We pass 'NULL' as the callback pointer for the heap allocated
     * event, and we pass the event itself as the callback pointer
     * for the stack-allocated event. */
    struct event *ev = ptr;
    if (ev)
        event_debug_unassign(ev);
}
/* Here's a simple mainloop that waits until fd1 and fd2 are both
 * ready to read. */
void mainloop(evutil_socket_t fd1, evutil_socket_t fd2, int debug_mode)
{
    struct event_base *base;
    struct event event_on_stack, *event_on_heap;
    if (debug_mode)
       event_enable_debug_mode();
    base = event_base_new();
    event_on_heap = event_new(base, fd1, EV_READ, cb, NULL);
    event_assign(&event_on_stack, base, fd2, EV_READ, cb, &event_on_stack);
    event_add(event_on_heap, NULL);
    event_add(&event_on_stack, NULL);
    event_base_dispatch(base);
    event_free(event_on_heap);
    event_base_free(base);
}
```
细节的event调试信息是一个特性,只有在编译Libevent时指定CFLAGS环境变量为"-DUSE_DEBUG".启用这个标识后,任何依赖于Libevent的程序,将输出详细的底层的日志信息.这些日志包括但不限于:
- 
事件的附加信息(event additions)
- 
事件的删除信息(event deletions)
- 
平台相关的事件通知信息(platform specific event notification information)
This feature cannot be enabled or disabled via an API call so it must only be used in developer builds.
These debugging functions were added in Libevent 2.0.4-alpha.
这个特性无法通过一个API开启或者关闭,所以只能使用于开发版(developer bulids)中.
这些debug函数是在Libevent 2.0.4-alpha加进来的.
### 检测Libevent版本
新版本的Libevent加了特性并移除了Bug.有时候你可能想检测Libevent版本,以便：
- 
检测已安装版本的Libevent是否支持你的程序
- 展示版本信息以便调试
- 
检测版本信息,这样你就可以警告用户该版本的Bug或者解决这些bug
 Interface
```
#define LIBEVENT_VERSION_NUMBER 0x02000300
#define LIBEVENT_VERSION "2.0.3-alpha"
const char *event_get_version(void);
ev_uint32_t event_get_version_number(void);
```
上述宏定义了Libevent的编译时(compile-time)版本.而上述函数返回的是运行时(run-time)版本.注意：如果你的程序是动态链接了Libevent的话,这些版本(指compile-time version和run-time version)可能会是不同的.
你可以获取两种格式的Libevet版本：适合于向使用者展示的字符串格式,或者是适合于数字比较的4-byte整数格式.整数格式的用高字节表示主版本号,第二个字节表示副版本号,第三个字节表示补丁版本号,低字节代表release状态(0表示是release版本,非零表示是开发版本).
所以Libevent 2.0.1-alpha的release版本用数字表示就是[02 00 01 00],或者0x02000100.介于2.0.1-alpha和2.0.2-alpha之间的开发版的数字版本号可能为[02 00 01 08]或0x02000108.
Example: Compile-time checks
```
#include <event2/event.h>
#if !defined(LIBEVENT_VERSION_NUMBER) || LIBEVENT_VERSION_NUMBER < 0x02000100
#error "This version of Libevent is not supported; Get 2.0.1-alpha or later."
#endif
int
make_sandwich(void)
{
        /* Let's suppose that Libevent 6.0.5 introduces a make-me-a
           sandwich function. */
#if LIBEVENT_VERSION_NUMBER >= 0x06000500
        evutil_make_me_a_sandwich();
        return 0;
#else
        return -1;
#endif
}
```
Example: Run-time checks
```
#include <event2/event.h>
#include <string.h>
int
check_for_old_version(void)
{
    const char *v = event_get_version();
    /* This is a dumb way to do it, but it is the only thing that works
       before Libevent 2.0. */
    if (!strncmp(v, "0.", 2) ||
        !strncmp(v, "1.1", 3) ||
        !strncmp(v, "1.2", 3) ||
        !strncmp(v, "1.3", 3)) {
        printf("Your version of Libevent is very old.  If you run into bugs,"
               " consider upgrading.\n");
        return -1;
    } else {
        printf("Running with Libevent version %s\n", v);
        return 0;
    }
}
int
check_version_match(void)
{
    ev_uint32_t v_compile, v_run;
    v_compile = LIBEVENT_VERSION_NUMBER;
    v_run = event_get_version_number();
    if ((v_compile & 0xffff0000) != (v_run & 0xffff0000)) {
        printf("Running with a Libevent version (%s) very different from the "
               "one we were built with (%s).\n", event_get_version(),
               LIBEVENT_VERSION);
        return -1;
    }
    return 0;
}
```
这部分介绍的宏和函数定义于<event2/event.h>.event_get_version()首次出现于Libevent 1.0c,其余的首次出现于Libevent 2.0.1-alpha.
### 释放全局的Libevent结构
即便你已经释放了所有的你分配的对象,还是有一些全局的结构遗留(未被释放)了.通常这不是什么问题:一旦进程退出,系统会清理这些资源的.不过不释放这些结构可能导致调试工具认为Libevent内存泄露.如果你希望确保Libevent释放了所有的内部使用的全局数据结构,你可以调用：
Interface
```
void libevent_global_shutdown(void);
```
这个函数不会释放掉任何Libevent函数返回给你的结构.如果你希望在进程退出前释放掉所有内存的话,你需要自己手动释放events,event_bases,bufferevents等.
调用libevent_global_shutdown()可能会导致别的Libevent函数有一些意想不到的问题.所以除非这是最后一个你想使用的Libevent函数了,否则不要调用他.有一个例外是libevent_global_shutdown()是幂等(idempotent)的(也就是可以多次调用的):在libevent_global_shutdown()被调用之后你还是可以再一次调用它.
这个函数声明于<event2/event.h>.它是于Libevent 2.1.1-alpha引入的.
