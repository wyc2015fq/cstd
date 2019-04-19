# Linux 内核里的“智能指针” - 文章 - 伯乐在线
原文出处： [wwang 的博客](http://www.cnblogs.com/wwang/archive/2010/12/02/1894847.html)
众所周知，C/C++语言本身并不支持垃圾回收机制，虽然语言本身具有极高的灵活性，但是当遇到大型的项目时，繁琐的内存管理往往让人痛苦异常。现代的C/C++类库一般会提供智能指针来作为内存管理的折中方案，比如STL的auto_ptr，Boost的Smart_ptr库，QT的QPointer家族，甚至是基于C语言构建的GTK+也通过引用计数来实现类似的功能。Linux内核是如何解决这个问题呢？同样作为C语言的解决方案，Linux内核采用的也是引用计数的方式。如果您更熟悉C++，可以把它类比为Boost的shared_ptr，或者是QT的QSharedPointer。
在Linux内核里，引用计数是通过 struct kref 结构来实现的。在介绍如何使用 kref 之前，我们先来假设一个情景。假如您开发的是一个字符设备驱动，当设备插上时，系统自动建立一个设备节点，用户通过文件操作来访问设备节点。
![](http://ww1.sinaimg.cn/mw690/aa213e02jw1eu4t41s9efj20fz0avmy8.jpg)
如上图所示，最左边的绿色框图表示实际设备的插拔动作，中间黄色的框图表示内核中设备对象的生存周期，右边蓝色的框图表示用户程序系统调用的顺序。如果用户程序正在访问的时候设备突然被拔掉，驱动程序里的设备对象是否立刻释放呢？如果立刻释放，用户程序执行的系统调用一定会发生内存非法访问；如果要等到用户程序close之后再释放设备对象，我们应该怎么来实现？kref就是为了解决类似的问题而生的。
kref的定义非常简单，其结构体里只有一个原子变量。

C
```
struct kref {
 atomic_t refcount;
};
```
Linux内核定义了下面三个函数接口来使用kref：

C
```
void kref_init(struct kref *kref);
void kref_get(struct kref *kref);
int kref_put(struct kref *kref, void (*release) (struct kref *kref));
```
我们先通过一段伪代码来了解一下如何使用kref。

C
```
struct my_obj
{
 int val;
 struct kref refcnt;
};
struct my_obj *obj;
void obj_release(struct kref *ref) 
{
 struct my_obj *obj = container_of(ref, struct my_obj, refcnt);
 kfree(obj);
}
device_probe() 
{
 obj = kmalloc(sizeof(*obj), GFP_KERNEL);
 kref_init(&obj->refcnt);
}
device_disconnect() 
{
 kref_put(&obj->refcnt, obj_release);
}
.open() 
{
 kref_get(&obj->refcnt);
}
.close() 
{
 kref_put(&obj->refcnt, obj_release);
}
```
在这段代码里，我们定义了obj_release来作为释放设备对象的函数，当引用计数为0时，这个函数会被立刻调用来执行真正的释放动作。我们先在device_probe里把引用计数初始化为1，当用户程序调用open时，引用计数又会被加1，之后如果设备被拔掉，device_disconnect会减掉一个计数，但此时refcnt还不是0，设备对象obj并不会被释放，只有当close被调用之后，obj_release才会执行。
看完伪代码之后，我们再来实战一下。为了节省篇幅，这个实作并没有建立一个字符设备，只是通过模块的加载和卸载过程来对感受一下kref。

C
```
#include <linux/kernel.h>
#include <linux/module.h>
struct my_obj {
 int val;
 struct kref refcnt;
};
struct my_obj *obj;
void obj_release(struct kref *ref)
{
 struct my_obj *obj = container_of(ref, struct my_obj, refcnt);
 printk(KERN_INFO "obj_release\n");
 kfree(obj);
}
static int __init kreftest_init(void)
{
 printk(KERN_INFO "kreftest_init\n");
 obj = kmalloc(sizeof(*obj), GFP_KERNEL);
 kref_init(&obj->refcnt);
 return 0;
}
static void __exit kreftest_exit(void)
{
 printk(KERN_INFO "kreftest_exit\n");
 kref_put(&obj->refcnt, obj_release);
 return;
}
module_init(kreftest_init);
module_exit(kreftest_exit);
MODULE_LICENSE("GPL");
```
通过kbuild编译之后我们得到kref_test.ko，然后我们顺序执行以下命令来挂载和卸载模块。
> 
sudo insmod ./kref_test.ko
sudo rmmod kref_test
此时，系统日志会打印出如下消息：
> 
kreftest_init
kreftest_exit
obj_release
这正是我们预期的结果。
有了kref引用计数，即使内核驱动写的再复杂，我们对内存管理也应该有信心了吧。
接下来主要介绍几点使用kref时的注意事项。
Linux内核文档kref.txt罗列了三条规则，我们在使用kref时必须遵守。
规则一：
> 
If you make a non-temporary copy of a pointer, especially if it can be passed to another thread of execution, you must increment the refcount with kref_get() before passing it off；
规则二：
> 
When you are done with a pointer, you must call kref_put()；
规则三：
> 
If the code attempts to gain a reference to a kref-ed structure without already holding a valid pointer, it must serialize access where a kref_put() cannot occur during the kref_get(), and the structure must remain valid during the kref_get().
对于规则一，其实主要是针对多条执行路径（比如另起一个线程）的情况。如果是在单一的执行路径里，比如把指针传递给一个函数，是不需要使用kref_get的。看下面这个例子：

C
```
kref_init(&obj->ref);
// do something here
// ...
kref_get(&obj->ref);
call_something(obj);
kref_put(&obj->ref);
// do something here
// ...
kref_put(&obj->ref);
```
您是不是觉得call_something前后的一对kref_get和kref_put很多余呢？obj并没有逃出我们的掌控，所以它们确实是没有必要的。
但是当遇到多条执行路径的情况就完全不一样了，我们必须遵守规则一。下面是摘自内核文档里的一个例子：

C
```
struct my_data
{
 .
 .
 struct kref refcount;
 .
 .
};
void data_release(struct kref *ref)
{
 struct my_data *data = container_of(ref, struct my_data, refcount);
 kfree(data);
}
void more_data_handling(void *cb_data)
{
 struct my_data *data = cb_data;
 .
 . do stuff with data here
 .
 kref_put(&data->refcount, data_release);
}
int my_data_handler(void)
{
 int rv = 0;
 struct my_data *data;
 struct task_struct *task;
 data = kmalloc(sizeof(*data), GFP_KERNEL);
 if (!data)
 return -ENOMEM;
 kref_init(&data->refcount);
 kref_get(&data->refcount);
 task = kthread_run(more_data_handling, data, "more_data_handling");
 if (task == ERR_PTR(-ENOMEM)) {
 rv = -ENOMEM;
 goto out;
 }
 .
 . do stuff with data here
 .
 out:
 kref_put(&data->refcount, data_release);
 return rv;
}
```
因为我们并不知道线程more_data_handling何时结束，所以要用kref_get来保护我们的数据。
注意规则一里的那个单词“before”，kref_get必须是在传递指针之前进行，在本例里就是在调用kthread_run之前就要执行kref_get，否则，何谈保护呢？
对于规则二我们就不必多说了，前面调用了kref_get，自然要配对使用kref_put。
规则三主要是处理遇到链表的情况。我们假设一个情景，如果有一个链表摆在你的面前，链表里的节点是用引用计数保护的，那你如何操作呢？首先我们需要获得节点的指针，然后才可能调用kref_get来增加该节点的引用计数。根据规则三，这种情况下我们要对上述的两个动作串行化处理，一般我们可以用mutex来实现。请看下面这个例子：

C
```
static DEFINE_MUTEX(mutex);
static LIST_HEAD(q);
struct my_data
{
 struct kref refcount;
 struct list_head link;
};
static struct my_data *get_entry()
{
 struct my_data *entry = NULL;
 mutex_lock(&mutex);
 if (!list_empty(&q)) {
 entry = container_of(q.next, struct my_q_entry, link);
 kref_get(&entry->refcount);
 }
 mutex_unlock(&mutex);
 return entry;
}
static void release_entry(struct kref *ref)
{
 struct my_data *entry = container_of(ref, struct my_data, refcount);
 list_del(&entry->link);
 kfree(entry);
}
static void put_entry(struct my_data *entry)
{
 mutex_lock(&mutex);
 kref_put(&entry->refcount, release_entry);
 mutex_unlock(&mutex);
}
```
这个例子里已经用mutex来进行保护了，假如我们把mutex拿掉，会出现什么情况？记住，我们遇到的很可能是多线程操作。如果线程A在用container_of取得entry指针之后、调用kref_get之前，被线程B抢先执行，而线程B碰巧又做的是kref_put的操作，当线程A恢复执行时一定会出现内存访问的错误，所以，遇到这种情况一定要串行化处理。
我们在使用kref的时候要严格遵循这三条规则，才能安全有效的管理数据。
