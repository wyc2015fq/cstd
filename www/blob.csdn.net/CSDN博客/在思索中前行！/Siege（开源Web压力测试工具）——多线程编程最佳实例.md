# Siege（开源Web压力测试工具）——多线程编程最佳实例 - 在思索中前行！ - CSDN博客





2014年11月16日 16:08:00[_Tham](https://me.csdn.net/txl16211)阅读数：1170








      在英语中，“Siege”意为围攻、包围。同时Siege也是一款使用纯C语言编写的开源WEB压测工具，适合在GNU/Linux上运行，并且具有较强的可移植性。之所以说它是多线程编程的最佳实例，主要原因是Siege的实现原理中大量运用了多线程的各种概念。Siege代码中用到了互斥锁、条件变量、线程池、线程信号等很多经典多线程操作，因此对于学习多线程编程也大有裨益。最近花了一些时间学习到了Siege的源代码，本文将介绍一下Siege压测工具的内部原理，主要供系统测试同学、以及学习多线程编程的同学们参考。

## **一、工具背景**

Siege是一名叫做Jeff Fulmer的伙计发起的开源项目，他的主页是：[http://www.joedog.org/](http://www.joedog.org/) 。从页面上看，Jeff Fulmer自从1999年起便开始“serving the Internets”，也算是一名老程序员了。Siege可谓是作者最杰出的作品。这款压测工具的名称“围攻”也比较生动形象展示了工具用途，即“围攻web服务器”。

Siege使用多线程实现，支持随机访问多个URL，可以通过控制并发数、总请求数（or压测时间）来实现对web服务的压测。Siege支持http，https，ftp三种请求方式，支持GET和POST方法，压测方式为同步压测，全部源代码总共13000行。功能还是非常全面的，很适合web开发在服务器开发完成后进行自测时使用。

## **二、工具使用**

该工具主要在Linux环境下使用，下载链接为：[http://download.joedog.org/siege/](http://download.joedog.org/siege/) 。安装方式和正常的linux环境软件安装步骤大致相同，先解压缩，再 config->make->make install。

```
$ tar –xzvf siege-3.0.8.tar.gz
$ cd siege-3.0.8
$ ./config
$ make
$ make install
```

在安装中需要注意的是make和make install可能会要求管理员权限，所以可能需要在make 和make install前面加上sudo。

使用方法如下：

`siege [options]` 或者 `siege [options] URL`其中`options`可选项有：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

-V --version 打印版本信息
-h --help 打印帮助信息
-v --verbose 在测试过程中输出更多的通知信息
-C --config 打印当前的配置信息（siege有一个名为.siegerc的配置文件）
-q --quite 此选项会覆盖掉--verbose，是安静模式，在测试中减少信息输出
-g --get 显示http头信息，适用于debug
-c --concurrent 最为常用的参数，每次测试必设置，并发数量，例 -c10代表10个并发
-i --internet 随机点击URL，在同时测试多个URL时可以使用，模拟用户随机访问的情形
-b --benchmark 每个请求之间没有延时，也是很常用的设置
-t --time 非常常用的参数，设置测试的时间，默认以分钟为单位，其他单位要自己设置，例如 -t10s，测试持续10秒
-r --reps 非常常用的参数，指定了测试几个回合结束，本参数和-t都可用来设置测试结束条件。
-f --file 指定一个存放URL链接的文件。siege支持随机访问多个url，因此这些url链接在文件中提供，较为常用。
-l --log 指定log文件，如果没有指定的话siege也有默认文件保存位置，文件名siege.log
-d --delay 指定时间延迟，在每个请求发出后，再随机延迟一段时间再发下一个
-H --header 指定http请求头部的一些内容
-A --user-agent 指定http请求中user-agent字段内容
-T --content-type 指定http请求中的content-type字段内容

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

上面列了一大坨参数，其实还没有列全，有一些更少用的没有列出来。实际上，如果只是简单使用的话，大部分都不需要搞清楚。上文中有几个常用的功能选项已经注明（`-b`, `-c`, `-t`, `-r`, `-f`），掌握这几个基本就够用了。我们先来简单使用一下，有一个更清楚的认识。

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

horstxu@horstxu-Lenovo-G400:~/Downloads/siege-3.0.8$ siege http://www.[某个网站].com -c10 -t5s -b
** SIEGE 3.0.8
** Preparing 10 concurrent users for battle.
The server is now under siege...
HTTP/1.1 200   0.14 secs:    1917 bytes ==> GET  /
HTTP/1.1 200   0.15 secs:    1917 bytes ==> GET  /
……………………
HTTP/1.1 200   0.16 secs:    1917 bytes ==> GET  /
Lifting the server siege...      done.

Transactions:          325 hits
Availability:       100.00 %
Elapsed time:         4.89 secs
Data transferred:         0.59 MB
Response time:         0.15 secs
Transaction rate:        66.46 trans/sec
Throughput:         0.12 MB/sec
Concurrency:         9.85
Successful transactions:         325
Failed transactions:            0
Longest transaction:         0.21
Shortest transaction:         0.11

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

上面省略号省略了一些冗余的输出，并且我们屏蔽网站域名免得打广告。在上面的测试中，我们设置了10个并发用户，测试5秒时间，并且每个请求之间没有时延，也就是收到回复后马上发出下一个。测试的结果是，4.89秒内完成了325次请求，共传输0.59MB的数据，平均响应时间0.15秒，平均每秒66.46次请求，拓扑量0.12MB每秒，并发数平均9.85。统计的数据还算比较全面。

## **三、原理介绍**

先简单画一下程序的流程图，如下图所示

![](http://images.cnitblog.com/blog/566097/201410/291745478627180.png)

如果并发用户数为n，那么就会相应创建n个压测线程，每个线程模拟1个用户。除了压测线程之外，主函数会额外生成2个线程，我们暂且称之为计时线程和控制线程。计时线程用于等待一开始我们设定的压测时间，到时间后通过线程信号通知控制线程。随后控制线程通过改变与压测线程共享的压测停止标志位，并发送终止信号来实现压测线程的停止。每个压测线程都会从结构体CREW中读取压测任务，这些压测任务由主函数添加。每个线程的测试数据均会输出到client结构体数组中，最后由主函数统一收集结果，并打印在屏幕上。

这一过程当中涉及的线程操作有条件变量，用于等待CREW中有压测任务到来，另外在计时线程中也用到了条件变量进行计时操作；互斥锁，用于改变CREW结构体成员的值时加锁保护数据；线程信号，用于线程间的相互通知；信号屏蔽字，用于将到来的异步信号用同步的方法去处理。源码中一大堆以pthread开头的函数操作，如果不清楚细节的话可以翻阅一下《UNIX环境高级编程》这本编程圣经来查阅一下。接下来我们进行更详细一些的代码分析。

## **四、源码分析**

### 4.1 `CREW`与`client`两个结构体

`CREW`是用来统一管理所有压测线程的结构体，它在主函数中被声明，因此可以被所有线程共享。对其中成员变量的改动也需要加锁后进行。`CREW`结构体如下：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

struct CREW_T //用于管理所有压测线程的结构体
{
    int              size; //目标并发数目，即压测线程个数
    int              maxsize; //最大并发数目，即压测线程个数
    int              cursize; //目前的可用并发数，压测中时这个数字随压测线程实时变化
    int              total; //实际启动的并发数
    WORK             *head; //压测任务链表头部
    WORK             *tail; //压测任务链表尾部
    BOOLEAN          block; //当已经达到最大并发时，则不准再添加新的压测线程
    BOOLEAN          closed; //压测线程是否已经关闭
    BOOLEAN          shutdown; //压测线程是否应该停止了
    pthread_t        *threads; //长度为size的数组，存储线程号
    pthread_mutex_t  lock; //修改本结构体都要先加锁
    pthread_cond_t   not_empty; //用于表示cursize不为0的条件
    pthread_cond_t   not_full; //用于表示cursize不等于maxsize的条件
    pthread_cond_t   empty; //用于表示cursize等于0的条件
};

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

每个压测线程都会维护属于自己的一份`client`，他们共同构成一个长度为n的数组。该结构体用于存储属于压测线程的相关信息，例如请求的响应时间，请求次数，数据流量等。这些统计信息最终将会反映给主进程做汇总输出。

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

typedef struct
{
    int      id; //client编号，对于n个线程编号分别从0至n-1
    unsigned long  hits; //共完成几次transaction，每完成一次请求加1
    unsigned long  bytes; //收到的数据总量
    unsigned int   code; //返回码是小于400的，或者等于401，等于407，则该计数加1
    unsigned int   fail; //失败计数，只要返回码大于等于400，且不是401也不是407，则该计数加1
    unsigned int   ok200; //返回码是200的数量，200为成功请求
    ARRAY  urls; //要访问的URL列表
    struct {
        DCHLG *wchlg;
        DCRED *wcred;
        int    www;
        DCHLG *pchlg;
        DCRED *pcred;
        int  proxy;
        struct {
            int  www;
            int  proxy;
        } bids;
        struct {
            TYPE www;
            TYPE proxy;
        } type;
    } auth; //本结构体用于设置代理服务器信息以及鉴权信息
    int      status; //连接状态信息，包括未连接，正在连接，待读取等
    float    time; //统计请求花费的总时长
    unsigned int rand_r_SEED; //随机数种子，用于随机访问URL的场景
    float himark; //最慢一次请求花费的时间
    float lomark; //最快一次请求花费的时间
} CLIENT;

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

写到这里，其实本程序代码为什么有13000行之多已经可以看到原因了。作者对于很多模块都进行了封装，比如C语言没有的BOOLEAN类型，数组操作ARRAY类型，压测任务链表操作WORK类型，已经与C++中的class有些类似。我们可以举个简单的例子，比如`WORK`类型是这么定义的：

```
typedef struct work
{
    void          (*routine)();
    void          *arg;
    struct work   *next;
} WORK;
```

这里面的`routine`是一个函数指针，而`arg`是要传给前面函数的参数。整个压测任务由一个单向链表来存储在`CREW`中。程序中这样的例子还有很多，就不再赘述。接下来我们关注一下计时线程、控制线程、压测线程的核心代码。

### 4.2 计时线程

计时线程在到达一定时间之后，会向控制线程发送SIGTERM信号，通知控制线程停止压测。该函数并不算复杂，下面是核心代码，我们略去了一些不必要的代码，只展示出了最重要的部分：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

void siege_timer(pthread_t handler) //handler是控制线程的id
{
    int err;
    time_t now;
    struct timespec timeout;
    pthread_mutex_t timer_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  timer_cond  = PTHREAD_COND_INITIALIZER; //专门用来计时的条件变量

    if (time(&now) < 0) { 
        NOTIFY(FATAL, "unable to set the siege timer!"); 
    }
    timeout.tv_sec=now + my.secs; //设置超时时间，my.secs就是我们设置的压测时间，以秒为单位
    timeout.tv_nsec=0;

    pthread_mutex_lock(&timer_mutex); 
    for (;;) {
        err = pthread_cond_timedwait( &timer_cond, &timer_mutex, &timeout);//使用条件变量进行计时操作
        if (err == ETIMEDOUT) { 
            /* timed out  */
            pthread_kill(handler, SIGTERM); //向handler线程发送sigterm信号
            break;  
        } else {
            continue;
        }
    }
    pthread_mutex_unlock(&timer_mutex);
    return;
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

这段代码还是比较容易理解的，条件变量在到时之前根本不会被激活，基本上是因为计时到了而返回，这也是`pthread_cond_timedwait`的作用。为了使用条件变量，外面又包了一层互斥锁`timer_mutex`，虽然根本不会有其他线程来抢这把锁。到时间后，通过`pthread_kill`来向其他线程发送信号。

### 4.3 控制线程

控制线程其实只做一件事情，即等待计时线程发送终止信号，收到信号后调用相关函数取消正在执行的压测线程。这次同样略去一些代码，只看最核心的控制线程部分。相关代码如下：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

void sig_handler(CREW crew)
{
    int gotsig = 0; 
    sigset_t  sigs;

    sigemptyset(&sigs);
    sigaddset(&sigs, SIGHUP);
    sigaddset(&sigs, SIGINT);
    sigaddset(&sigs, SIGTERM);
    sigprocmask(SIG_BLOCK, &sigs, NULL); //设置信号屏蔽字，在sigwait之前必须先屏蔽信号

    /** 
     * Now wait around for something to happen ... 
     */
    sigwait(&sigs, &gotsig);//阻塞等待线程信号，用于响应计时线程pthread_kill发来的信号
    fprintf(stderr, "\nLifting the server siege..."); 
    crew_cancel(crew); //取消CREW中的所有任务，即让压测线程停止下来

    /**
     * The signal consistently arrives early,
     * so we artificially extend the life of
     * the siege to make up the discrepancy. 
     */
    pthread_usleep_np(501125); //人为使线程睡眠一小会，上面英文为原作者的注释 

    pthread_exit(NULL);
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

### 4.4 压测线程

计时和控制线程还是比较容易理解的，代码结构也相对较为简单，接下来就瞧一下最为繁琐的压测线程。主函数将会通过for循环来创建n个压测线程，每个线程执行如下函数（同样略去了非关键代码）：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

private void *crew_thread(void *crew)//压测线程，共有size个，取决于命令行-c后面的数字
{
    WORK *workptr; //压测函数结构体的指针，真正的压测逻辑都在这里的函数中实现
    CREW this = (CREW)crew; //这里的结构体CREW正是前文4.1节中提到的CREW，用于管理所有压测线程

    while(TRUE){//这里是死循环，压测一直在循环执行中，除非调用pthread_exit退出
        pthread_mutex_lock(&(this->lock));
        while((this->cursize == 0) && (!this->shutdown)){//如果目前可用并发数cursize是空的，则等待
            pthread_cond_wait(&(this->not_empty), &(this->lock)); //一开始创建的size个压测线程都会卡在这里
        }

        if(this->shutdown == TRUE){ //线程停止，则释放锁，退出，这里是唯一可以停止压测的地方
            pthread_mutex_unlock(&(this->lock));
            pthread_exit(NULL);
        }
        workptr = this->head; //取出第一个节点上的压测程序
        this->cursize--; //可用并发数减一
        if(this->cursize == 0){ //更新CREW中压测任务链表的值
            this->head = this->tail = NULL;
        }else{
            this->head = workptr->next;
        }
        if((this->block) && (this->cursize == (this->maxsize - 1))){
            pthread_cond_broadcast(&(this->not_full));
        }
        if(this->cursize == 0){ //现在并发量如果为0，唤醒empty condition
            pthread_cond_signal(&(this->empty));
        }
        pthread_mutex_unlock(&(this->lock));

        (*(workptr->routine))(workptr->arg);//这里才真正在执行压测函数

        xfree(workptr);
    }

    return(NULL);
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

细心观察一下即可发现，这里面都是各种加锁解锁的操作，在修改`CREW`前后需要拿到互斥锁才可以进行。一直到很靠后面一句`(*(workptr->routine))(workptr->arg)`才真正开始执行压测函数。并且从中还可以知道，只有把`CREW`的`shutdown`字段改为`true`才可以使压测线程停下来。那么接下来观察一下`workptr->routine`的内容。

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

void *start_routine(CLIENT *client)//主要的压测函数，每个线程通过此函数进行压测
{
    int       ret;     //function return value  
    CONN *C = NULL;    // connection data (sock.h)  

    int type, state;

    C = xcalloc(sizeof(CONN), 1);
    C->sock = -1;

    pthread_cleanup_push((void*)clean_up, C); //设置线程清理函数，在线程退出时会被调用
    pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, &type);//修改取消类型为异步取消
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &state);//设置线程取消状态为可以取消

    for (……) {
        URL tmp = array_get(client->urls);//选择一个url
        if (tmp != NULL && url_get_hostname(tmp) != NULL) {
            if ((ret = __request(C, tmp, client))==FALSE) { //访问该url
                __increment_failures();
            }
        }
        if (my.failures > 0 && my.failed >= my.failures) {
            break;
        }
    }

    /**
     * every cleanup must have a pop
     */
    pthread_cleanup_pop(0); //该设置用于取消线程清理函数，pop与push经常成对使用
    if (C->sock >= 0){
        C->connection.reuse = 0;    
        socket_close(C);
    }
    xfree(C);
    C = NULL;
    return(NULL);
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

上面就是`workptr->routine`具体锁执行的函数。需要注意的是源代码中这个函数比较长，我们简略了一大部分，只列出来了少部分主要代码。实际上，`start_routine`依然没有列出来最细节的部分，比如每个压测线程中必须要执行的构建socket，连接服务器connect，写入请求write，收到请求read等过程。这些过程都在上面代码中出现的`__request`函数中。列出上文这些内容主要想讲述几个pthread开头的函数在压测中的作用。

`pthread_cleanup_push`与`pthread_cleanup_pop`用于设置线程清理函数，也就是当线程收到cancel信号退出前会调用的函数。有点类似于常见的try-catch-finally组合中finally的功能。push用于添加线程清理函数，pop用于删除线程清理函数。需要注意的是线程清理函数使用栈来存储，可以设置多个，push和pop类似栈的操作，后来的先出去。pop一次只会删除最晚的一个线程清理函数。

`pthread_setcanceltype`表示当函数收到取消（`pthread_cancel`）的信号，应该采取何种取消策略。`PTHREAD_CANCEL_ASYNCHRONOUS`表示异步取消，即任意时间收到取消信号，线程都可以终止；另外还有一种参数为`PTHREAD_CANCEL_DEFERRED`，表示推迟取消，即收到取消信号后一直到线程遇到某取消点才会取消。至于取消点的设置，POSIX中默认了很多系统调用出现时都会出现取消点。这些函数列表大家可以自行去网上搜一下，就不再罗列了。

`pthread_setcancelstate`用于设置线程取消状态，可以是`PTHREAD_CANCEL_ENABLE`或`PTHREAD_CANCEL_DISABLE`，分别表示可以取消或不可以被取消。如果设为不可以被取消，不管前面的setcanceltype怎么设置，碰到取消信号时线程都会将该信号挂起并无视它。

在`__request`函数中，由于需要区分ftp、http、https请求，需要判断是否长连接，还可能需要设置cookie，区别POST，GET等不同请求方式，这导致该函数又调用一大堆其他函数，层层深挖下去都列出来可能就是一部长篇小说的长度了。我们就不再深究，更深层的内容其实不难理解。我们在最后来瞧一眼主函数如何将以上这些内容串联起来。

### 4.5 main主函数

main函数是将所有压测流程串联起来的调度程序。在阅读源码的时候，从main函数入手应该是比较明智的选择，这样也比较容易看出来整个程序的组织。Siege中的main函数也有300余行代码，这些代码并没有太难理解的地方，我们使用注释+省略号+伪代码的方式大概浏览一下main函数如何将以上过程进行串联。如下：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

int main(int argc, char *argv[])
{
    /*设置信号屏蔽字*/
    sigemptyset(&sigs); 
    sigaddset(&sigs, SIGHUP);
    sigaddset(&sigs, SIGINT);
    sigaddset(&sigs, SIGALRM);
    sigaddset(&sigs, SIGTERM);
    sigprocmask(SIG_BLOCK, &sigs, NULL);

    /*读取命令行参数、配置文件并解析*/
    ……

    /*用for循环创建压测线程*/
    crew = new_crew(my.cusers, my.cusers, FALSE);

    /*设置URL相关参数，将要压测的连接存入内存中*/
    ……

    /*创建控制线程*/
    pthread_create(&cease, NULL, (void*)sig_handler, (void*)crew);

    /*创建计时线程，注意第三个参数和创建控制线程的第一个参数都是cease*/
    pthread_create(&timer, NULL, (void*)siege_timer, (void*)cease);

    for (x = 0; x < my.cusers && crew_get_shutdown(crew) != TRUE; x++) {
        /*向CREW中添加压测任务*/
        /*也就是设置CREW中WORK链表的值*/
        /*设置完毕后通过pthread_cond_broadcast解除被条件变量阻塞的压测线程*/
        ……
        crew_add(crew, (void*)start_routine, &(client[x]));
        ……
    }

    /*等待所有压测线程结束*/
    crew_join(crew, TRUE, &statusp);

    /*从client数组中读取压测数据，汇总到DATA结构体中*/
    ……

    /*将汇总的DATA结构体内容输出*/
    ……

    /*释放资源，退出*/
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

源码暂且看到这里，一共13000行代码，如果每一行都搞明白可能会累死。我们最重要的是了解到最核心的内容，清楚整个程序的设计原理即可。Siege除了帮助大家认识一种新的压测工具实现方法以外，其多线程的各种操作也是让人大开眼界的。

## **五、 Siege的优缺点**

Siege是由多线程实现的同步压测工具，它实现的是模拟n个用户不停地访问某个URL的场景。由于多线程开销会比多进程小一些，因此该压测工具比多进程的压测工具在系统开销上会好很多。程序提供了到时停止（到一定时间停止压测）和到量停止（访问一定次数后停止压测）两种压测方法，支持同时压测多个URL，也能够随机选取URL进行压测。支持ftp、http、https，可以发送GET、POST、HEAD等多种请求，可以设置鉴权、cookies。并且程序中特意增加了许多解决不同平台上兼容性的代码。已经是非常完善的一个工具了，并且到目前位置，Siege的版本依然在更新中。

不过，Siege对于压力控制并不够精确，只能粗略地根据并发用户数去控制一下压力大小。考虑这样一种场景，我希望每秒钟向服务器发送1000个请求，并且第0至1ms发一个，第1至2ms发一个，第2至3ms发一个，……，这样精度的控制Siege是无法达到的。当然，对于同步压测程序来说，这样的精度比较难以实现。另外，Siege的时间控制并不精确，比如在本文中使用Siege的章节可以看到，我想要测试5s，但是实际输出的测试时间为4.89s。Siege的计时方式是通过times函数取得压测经历的的系统时钟数，并通过`sysconf(_SC_CLK_TCK)`取得系统每秒时钟数，两者相除得来。另外一个小的缺点是，由于使用多线程实现，一个进程可以开启的线程数量本身是有限的，并且线程过多的情况下CPU在线程间切换也是一笔不小的开销，十分影响效率。因此Siege的使用过程中还要注意开启的并发用户数不能太多。

![](http://images.cnitblog.com/blog/566097/201410/291751243153259.png)

最后的最后还要展示一下Siege的源代码文件，13000行代码是由以上这一大坨源文件构成，乍一看上去还小吃了一惊，一个小工具写了如此复杂的代码。其实从文件名可以看出来每个文件都有很强的封装思想，如果利用C++来写，一定会比目前的纯C清晰很多。不过作为一款linux系统上运行的工具，可能作者认为纯C语言一定是linux编程的首选吧。



