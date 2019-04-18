# mysql thread pool 问题排查 - z69183787的专栏 - CSDN博客
2016年11月29日 10:40:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1489
# **问题分析：**
## **基本原理：**
没有引入thread pool前，MySQL使用的是one thread per connection，一旦connection增加到一定程度，MySQL的性能将急剧下降甚至被压跨。引入thread pool后将会解决上述问题,同时会减少MySQL内部的线程数（节省内存）及频繁创建线程的开销（节省cpu)。
## **thread pool是如何工作的呢？**
在MySQL内部有一个专用的thread用来监听数据库连接请求，当一个新的请求过来，如果采用以前的模型（one-thread-per-connection），main listener（这是主线程中的listener，为了避免与thread group 中的listener混淆，我们称之为“Main listener”）将从thread cache中取出1个thread或创建1个新的thead立即处理该连接请求，由该thread完成该连接的整个生命周期；而如果采用thread
 pool模型，这个连接请求将会被随机放到一个thread group（thread pool由多个thread group 组成）的队列中，之后该thread group中worker thread从队列中取出并建立连接，一旦连接建立，该连接对应的socket句柄将与该thread group中的listener关联起来，之后该连接将在该thread group中完成它的生命周期。
 接下来我们来说一下thread group 。thread group是thread pool的核心组件，所有的操作都是发生在thread group。thread pool由多个(数量由参数thread_pool_size来决定，默认等于cpu个数)thrad group组成。一个连接请求被随机地绑定到一个thread group，每个thread group独立工作，并且占用一个核的cpu,所以thread group都会最大限度地保持一个thread处于ACTIVE状态，并且最好只有一个。
thread group中的thread一般有4个状态：
- TP_STATE_LISTENER
- TP_STATE_IDLE
- TP_STATE_ACTIVE
- TP_STATE_WAITING
当一个线程作为listener运行时就处于“TP_STATE_LISTENER”，它通过epoll的方式监听联接到该thread group的所有连接，当一个socket就绪后，listener将决定是否唤醒一个thread或自己处理该socket。此时如果thread group的队列为空，它将自己处理该socket并将状态更改为“ACTIVE”，之后该thread 在MySQL Server内部处理“工作”，当该线程遇到锁或异步IO(比如将数据页读入到buffer
 pool)这些wait时，该thread将通过回调函数的方式告诉thread pool，让其把自己标记为“WAITING”状态。
此时，假设队列中有了新的socket准备就绪，是立即创建新的线程还是等待刚才的线程执行结束呢？
由于thread pool最初设计的目标是保持一定数量的线程处于“ACTIVE”状态，具体的实现方式就是控制thread group的数量和thread group内部处于"ACTIVE"状态的thread的数量。控制thread group内部的ACTIVE状态的数量，方法就是最大限度地保证处于ACTIVE状态的线程个数是1。很显然当前thread group中有一个处于WAITING状态的thread了，如果再启用一个新的线程并且处于ACTIVE状态，刚才的线程由WAITING变为ACTIVE状态时，此时将会有2个“ACTIVE”状态的线程，和最初的目标似乎相背，但显然也不能让后续就绪的socket一直等待下去，那应该怎么处理？
那么此时需要一个权衡了，提供了这样的一个方法：对正在ACTIVE或WAITING状态的线程启用一个计数器，超过计数器后将该thread标记为stalled，然后thread group创建新的thread或唤醒sleep的thread处理新的sokcet，这样将是一个很好的权衡。超时时间该参数thread_pool_stall_limit来决定，默认是500ms。
如果一个线程无事可做，它将保持空闲状态（TP_STATE_WAITING）一定时间（thread_pool_idle_timeout参数决定，默认是60秒）后“自杀”。
## **和我们遇到的具体问题相关的点：**
假设上文提到的由“ACTIVE”转化为“WAITING”状态的线程（标记为“线程A”）所执行的“SQL"是可能是一个标准的慢sql（命名为SQLA，需要锁等待或从IO读数据），那么后续需要执行的SQL（命名SQLB）要么等待线程A结束（有可能没有超过500ms，该查询就结束了），要么需要创建新的线程（超过500ms），不管哪种情况，SQLB都会在线程等待上花费很多时间，此时SQLB就是我cat监控系统上看到的慢SQL。又因为SQLA不一定都是慢SQL，所以SQLB也不是每次在线程等待上花费较多的时间，这就吻合我们看到的现象“一定比例的慢SQL”。
![](http://wiki.sankuai.com/download/attachments/613783743/image2016-10-9%2017%3A28%3A24.png?version=1&modificationDate=1476005307000&api=v2)
## **解决方法：**
调整thread_pool_stall_limit=10，这样就强迫被SQLA更快被标记为stalled，然后创建新的线程来处理SQLB。
理论上讲：
1.绝大部分oltp的应用，10ms内是能够查出数据的，所以这类sql不会导致work thread（thread running）数增加。
2.对于一部分超过10ms的sql，会导致work thread增加，但不会超过 thread_pool_oversubscribe这个参数值，所以总体work thread线程仍然可控
3.该参数已经在线上所有服务器运行2月多，目前没有发现与其相关的问题，在一定程度上说明该参数值是安全的。
4.据称阿里的数据库的thread_pool_stall_limit参数值也10ms，另外MySQL5.7 默认值是6ms，比10ms更激进，但可能还会基于其它优化配合才需要调整到6ms(此2因素可忽略，仅作参考)。
