# workqueue原理和分析(转) - h13 - 博客园
workqueue,中文称其为工作队列，是一个用于创建内核线程的接口，通过它创建的内核线程来执行内核其他模块排列到队列里的工作，创建的内核线程被称为工作者线程。要理解工作队列的实现，重点在于理解相关的三个数据结构的含义及关系。
1 表示工作队列类型的数据结构：struct workqueue_struct
- /*
- 
- * The externally visible workqueue abstraction is an array of
- 
- * per-CPU workqueues:
- 
- */
- 
- struct workqueue_struct {
-     struct cpu_workqueue_struct *cpu_wq;   /*工作者线程数组*/
-     struct list_head list;/*连接工作队列类型的链表*/
- const char *name;        /*工作者线程的名称*/           
- int singlethread;         /*是否创建新的工作者线程，0表示采用默认的工作者线程event/n*/
- int freezeable;/* Freeze threads during suspend */
- int rt;
- #ifdef CONFIG_LOCKDEP
-     struct lockdep_map lockdep_map;
- #endif
- };
内核中默认的工作队列为： 
- static struct workqueue_struct *keventd_wq __read_mostly;
其对应的工作者线程为：event/n    其中，n代表当前cpu中processor的个数。
2. 表示工作者线程的数据结构：struct cpu_workqueue_struct
- /*
- * The per-CPU workqueue (if single thread, we always use the first
- * possible cpu).
- */
- struct cpu_workqueue_struct {
- 
-     spinlock_t lock;          /*因为工作者线程需要频繁的处理连接到其上的工作，所以需要枷锁保护*/
- 
-     struct list_head worklist;
-     wait_queue_head_t more_work;
-     struct work_struct *current_work; /*当前工作线程需要处理的工作*/
- 
-     struct workqueue_struct *wq;   /*该工作者线程属于那种类型的工作者队列*/
-     struct task_struct *thread;    /*指向工作者线程的任务结构体*/
- } ____cacheline_aligned;
3. 表示工作的数据结构，即工作者线程处理的对象：struct work_struct 
- struct work_struct {
-     atomic_long_t data;       /*工作处理函数func的参数*/
- #define WORK_STRUCT_PENDING 0        /* T if work item pending execution */
- #define WORK_STRUCT_STATIC 1        /* static initializer (debugobjects)*/
- #define WORK_STRUCT_FLAG_MASK (3UL)
- #define WORK_STRUCT_WQ_DATA_MASK (~WORK_STRUCT_FLAG_MASK)
-     struct list_head entry;        /*连接工作的指针*/
-     work_func_t func;              /*工作处理函数*/
- #ifdef CONFIG_LOCKDEP
-     struct lockdep_map lockdep_map;
- #endif
- };
再分析了以上三个对象后，重点掌握三者之间的关系。工作队列类型，工作者线程以及工作三个数据对象之间的关系如图所示。
![](http://blog.chinaunix.net/attachment/201104/26/20940095_1303816466jtHn.jpg)
workqueue的执行非常简单，即在每次运行工作者线程的时候，去遍历工作者线程对应的工作链表上的工作，逐一进行处理即可，从这里我们也可以猜到，工作队列是没有优先级的，基本按照FIFO的方式进行处理。
转自：http://blog.csdn.net/ustc_dylan/article/details/6371229

