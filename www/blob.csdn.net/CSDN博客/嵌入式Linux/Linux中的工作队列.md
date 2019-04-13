
# Linux中的工作队列 - 嵌入式Linux - CSDN博客

2016年01月06日 10:17:32[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：350


工作队列(work queue)是Linux kernel中将工作推后执行的一种机制。这种机制和BH或Tasklets不同之处在于工作队列是把推后的工作交由一个内核线程去执行，因此工作队列的优势就在于它允许重新调度甚至睡眠。
工作队列是2.6内核开始引入的机制，在2.6.20之后，工作队列的数据结构发生了一些变化，因此本文分成两个部分对2.6.20之前和之后的版本分别做介绍。
I、2.6.0~2.6.19
数据结构：
struct work_struct {
unsigned long pending;
struct list_head entry;
void (*func)(void*);
void *data;
void *wq_data;
struct timer_list timer;
};
pending是用来记录工作是否已经挂在队列上；
entry是循环链表结构；
func作为函数指针，由用户实现；
data用来存储用户的私人数据，此数据即是func的参数；
wq_data一般用来指向工作者线程（工作者线程参考下文）；
timer是推后执行的定时器。
work_struct的这些变量里，func和data是用户使用的，其他是内部变量，我们可以不用太过关心。
API：
INIT_WORK(_work, _func, _data);
int schedule_work(struct work_struct *work);
int schedule_delayed_work(struct work_struct *work, unsigned long delay);
void flush_scheduled_work(void);
int cancel_delayed_work(struct work_struct *work);
1、初始化指定工作，目的是把用户指定的函数_func及_func需要的参数_data赋给work_struct的func及data变量。
2、对工作进行调度，即把给定工作的处理函数提交给缺省的工作队列和工作者线程。工作者线程本质上是一个普通的内核线程，在默认情况下，每个CPU均有一个类型为“events”的工作者线程，当调用schedule_work时，这个工作者线程会被唤醒去执行工作链表上的所有工作。
3、延迟执行工作，与schedule_work类似。
4、刷新缺省工作队列。此函数会一直等待，直到队列中的所有工作都被执行。
5、flush_scheduled_work并不取消任何延迟执行的工作，因此，如果要取消延迟工作，应该调用cancel_delayed_work。
以上均是采用缺省工作者线程来实现工作队列，其优点是简单易用，缺点是如果缺省工作队列负载太重，执行效率会很低，这就需要我们创建自己的工作者线程和工作队列。
API：
struct workqueue_struct *create_workqueue(const char *name);
int queue_work(struct workqueue_struct *wq, struct work_struct *work);
int queue_delayed_work(struct workqueue_struct *wq, struct work_struct *work, unsigned long delay);
void flush_workqueue(struct workqueue_struct *wq);
void destroy_workqueue(struct workqueue_struct *wq);
1、创建新的工作队列和相应的工作者线程，name用于该内核线程的命名。
2、类似于schedule_work，区别在于queue_work把给定工作提交给创建的工作队列wq而不是缺省队列。
3、延迟执行工作。
4、刷新指定工作队列。
5、释放创建的工作队列。
下面一段代码可以看作一个简单的实作：
void my_func(void *data)
{
char*name = (char*)data;
printk(KERN_INFO “Hello world, my name is %s!\n”, name);
}
struct workqueue_struct *my_wq = create_workqueue(“my wq”);
struct work_struct my_work;
INIT_WORK(&my_work, my_func, “Jack”);
queue_work(my_wq, &my_work);
destroy_workqueue(my_wq);
II、2.6.20~2.6.??
自2.6.20起，工作队列的数据结构发生了一些变化，使用时不能沿用旧的方法。
数据结构：
typedef void (*work_func_t)(struct work_struct *work);
struct work_struct {
atomic_long_t data;
struct list_head entry;
work_func_t func;
};
与2.6.19之前的版本相比，work_struct瘦身不少。粗粗一看，entry和之前的版本相同，func和data发生了变化，另外并无其他的变量。
entry我们不去过问，这个和以前的版本完全相同。data的类型是atomic_long_t，这个类型从字面上看可以知道是一个原子类型。第一次看到这个变量时，很容易误认为和以前的data是同样的用法，只不过类型变了而已，其实不然，这里的data是之前版本的pending和wq_data的复合体，起到了以前的pending和wq_data的作用。
func的参数是一个work_struct指针，指向的数据就是定义func的work_struct。
看到这里，会有两个疑问，第一，如何把用户的数据作为参数传递给func呢？以前有void *data来作为参数，现在好像完全没有办法做到；第二，如何实现延迟工作？目前版本的work_struct并没有定义timer。
解决第一个问题，需要换一种思路。2.6.20版本之后使用工作队列需要把work_struct定义在用户的数据结构中，然后通过container_of来得到用户数据。具体用法可以参考稍后的实作。
对于第二个问题，新的工作队列把timer拿掉的用意是使得work_struct更加单纯。首先回忆一下之前版本，只有在需要延迟执行工作时才会用到timer，普通情况下timer是没有意义的，所以之前的做法在一定程度上有些浪费资源。所以新版本中，将timer从work_struct中拿掉，然后又定义了一个新的结构delayed_work用于处理延迟执行：
struct delayed_work {
struct work_struct work;
struct timer_list timer;
};
下面把API罗列一下，每个函数的解释可参考之前版本的介绍或者之后的实作：
INIT_WORK(struct work_struct *work, work_func_t func);
INIT_DELAYED_WORK(struct delayed_work *work, work_func_t func);
int schedule_work(struct work_struct *work);
int schedule_delayed_work(struct delayed_work *work, unsigned long delay);
struct workqueue_struct *create_workqueue(const char *name);
int queue_work(struct workqueue_struct *wq, struct work_struct *work);
int queue_delayed_work(struct workqueue_struct *wq, struct delayed_work *work, unsigned long delay);
void flush_scheduled_work(void);
void flush_workqueue(struct workqueue_struct *wq);
int cancel_delayed_work(struct delayed_work *work);
void destroy_workqueue(struct workqueue_struct *wq);
其中，1、2、4、7和以前略有区别，其他用法完全一样。
实作：
struct my_struct_t {
char *name;
struct work_struct my_work;
};
void my_func(struct work_struct *work)
{
struct my_struct_t *my_name = container_of(work, struct my_struct_t, my_work);
printk(KERN_INFO “Hello world, my name is %s!\n”, my_name->name);
}
struct workqueue_struct *my_wq = create_workqueue(“my wq”);
struct my_struct_t my_name;
my_name.name = “Jack”;
INIT_WORK(&(my_name.my_work), my_func);
queue_work(my_wq, &(my_name.my_work));
destroy_workqueue(my_wq);

