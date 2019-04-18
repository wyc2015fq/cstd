# ArrayBlockingQueue - weixin_33985507的博客 - CSDN博客
2017年09月03日 23:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
最近得闲，看了一下阻塞队列的相关实现。在此分享一下我的一些体会。
**BlockingQueue**
ArrayBlockingQueue实现了接口BlockingQueue。顾名思义，BlockingQueue就是阻塞队列的意思。这包含两层意思：一、队列为空，***可以 ***阻塞读请求直到队列中有元素再返回。二、队列为满，***可以 ***阻塞写请求直到队列中有空位可以存放元素。请注意，这里只是可以，而不是一定。
BlockingQueue对读取、插入、删除提供了四种不同的方法：1、抛出异常；2、返回特殊值；3、阻塞直到操作成功；4、阻塞直到成功或者超时。参见下图：
![5737874-5948c553b3905bdd.png](https://upload-images.jianshu.io/upload_images/5737874-5948c553b3905bdd.png)
方法
其中Special value（特殊值）是指null或者true/false。注意，阻塞队列是不允许存入null的，因为null已经在设计时被定为返回的特殊值。
BlockingQueue的实现是线程安全的，但是并不代表它所有的方法（包括继承的方法）都是线程安全的，比如addAll、containsAll、removeAll等方法。这些方法并一定不是原子操作（参见AbstractQueue）。
说了这么多，BlockingQueue最适用的场景是什么呢？生产者消费者模型。
**ArrayBlockingQueue**
ArrayBlockingQueue是BlockingQueue的实现之一，底层采用数组来实现。
从下面的代码中，我们可以看到有一个ReentrantLock，两个Condition。一个notFull的Condition，一个notEmpty的Condition，分别表示队列存在空位的情况（可写）和非空的情况（可读）。
> 
/** The queued items */
final Object[] items;
/** items index for next take, poll, peek or remove */
int takeIndex;
/** items index for next put, offer, or add */
int putIndex;
/** Number of elements in the queue */
int count;
/*
* Concurrency control uses the classic two-condition algorithm
* found in any textbook.
*/
/** Main lock guarding all access */
final ReentrantLock lock;
/** Condition for waiting takes */
private final Condition notEmpty;
/** Condition for waiting puts */
private final Condition notFull;
如果你对AQS的细节了解的话，这里的代码也就好解释了。有一个读线程队列，有一个写线程队列，还有一个获取独占锁的阻塞队列。当线程读取到队列中没有元素时，会进入读线程队列；当线程写入满队列时，会进入写线程队列。只有等到各自的Condition发出signal时，才会转移到阻塞队列，抢夺独占锁。
ArrayBlockingQueue有三个构造函数：
1. 指定队列容量，限制队列的最大长度。
2. 指定独占锁是公平锁还是非公平锁。
3. 使用集合来初始化队列。
第三个构造函数有一个特别的地方：
![5737874-b6db714c23c93b4f.png](https://upload-images.jianshu.io/upload_images/5737874-b6db714c23c93b4f.png)
构造函数
在lock.lock()处加了一处注释。只是为了可见性加锁，而不是为了互斥加锁。这里的可见性是指初始化数组用的集合参数的可见性。需要把整个集合添加到数组之后才能释放锁。
我们在看ArrayBlockingQueue的成员变量时发现有putIndex和takeIndex。这两个标志位分别表示插入元素时存放的位置和取出元素时的位置。ArrayBlockingQueue采用了数组循环存储的方式来存储数据。数组的最后一个位置被占据之后，会回到起点（index=0）存放新的数据。这里的putIndex就负责指向下一个存放的位置。
暂时写到这里，等后续有时间了再补上。
