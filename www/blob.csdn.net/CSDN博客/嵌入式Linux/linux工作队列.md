
# linux工作队列 - 嵌入式Linux - CSDN博客

2016年01月06日 11:13:53[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：276


这里对Linux的工作队列(work_queue)不做深层次的挖掘，只对如何使用它以及一些简单的结构做简单地介绍。
Linux源代码(3.0.8)中和工作队列(work_queue)相关的结构主要在
`include/linux/workqueue.h`这个头文件中，这里就不摘抄了。这里就直接给出例子代码，在结合稍作解释：
```python
#include <linux/module.h>
```
```python
#include <linux/init.h>
```
```python
#include <linux/workqueue.h>
```
static struct work_struct work;
static void work_handler(struct work_struct *data)
{
printk(“just a demo for work queue.\n”);
}
static int __init workqueue_init(void)
{
printk(“init work queue demo.\n”);
INIT_WORK(&work, work_handler);
schedule_work(&work);
return 0;
}
static void __exit workqueue_exit(void)
{
printk(“exit work queue demo.\n”);
}
MODULE_LICENSE(“GPL”);
MODULE_AUTHOR(“zhuqinggooogle@gmail.com”);
module_init(workqueue_init);
module_exit(workqueue_exit);
一个简单的工作队列(work_queue)的演示就完成了。我们来编译后，insmod到系统看看：
/mnt/3520d $ insmod hi_work_queue.ko
init work queue demo
just a demo for work queue.
从系统中rmmod看一下：
/mnt/3520d $ rmmod hi_work_queue
exit work queue demo.
如果你对Linux的工作队列(work_queue)有稍微的了解，你看到这里会提问，“我们的工作队列项提交到了哪个工作队列线程上面呢？”，这就得从
`schedule_work`函数入手。看一下其/**
* schedule_work - put work task in global workqueue
* @work: job to be done
*
* Returns zero if @work was already on the kernel-global workqueue and
* non-zero otherwise.
*
* This puts a job in the kernel-global workqueue if it was not already
* queued and leaves it in the same position on the kernel-global
* workqueue otherwise.
*/
int schedule_work(struct work_struct *work)
{
return queue_work(system_wq, work);
}
扯到
`system_wq`这个全局变量，我们来看看他到底是什么。在
`kernel/workqueue.c`这个文件的底部给出了定义：
system_wq = alloc_workqueue(“events”, 0, 0);
看到这就清楚了，刚才是把工作队列项提交了默认的工作线程events上的。那我们自己可以创建一个工作队列线程吗？可以把自己的工作队列项提交到上面吗？当然，可以。下面给出一个demo代码：
```python
#include <linux/module.h>
```
```python
#include <linux/init.h>
```
```python
#include <linux/workqueue.h>
```
```python
static
```
```python
struct
```
```python
workqueue_struct *
```
```python
queue
```
```python
= NULL;
```
```python
static
```
```python
struct
```
```python
work_struct work;
```
```python
static
```
```python
void
```
```python
work_handler(
```
```python
struct
```
```python
work_struct *data)  
{  
    printk(
```
```python
"just a demo for work queue.\n"
```
```python
); 
}
```
```python
static
```
```python
int
```
```python
__init workqueue_init(
```
```python
void
```
```python
)  
{
```
```python
queue
```
```python
= create_singlethread_workqueue(
```
```python
"workqueue demo"
```
```python
);
```
```python
if
```
```python
(!
```
```python
queue
```
```python
)
```
```python
return
```
```python
-
```
```python
1
```
```python
;  
    printk(
```
```python
"init work queue demo.\n"
```
```python
);
    INIT_WORK(&work, work_handler);  
    queue_work(
```
```python
"queue"
```
```python
, &work);
```
```python
return
```
```python
0
```
```python
;    
}
```
```python
static
```
```python
void
```
```python
__exit workqueue_exit(
```
```python
void
```
```python
)  
{  
       printk(
```
```python
"exit work queue demo.\n"
```
```python
);
       destroy_workqueue(
```
```python
queue
```
```python
);  
}  
MODULE_LICENSE(
```
```python
"GPL"
```
```python
);
MODULE_AUTHOR(
```
```python
"zhuqinggooogle@gmail.com"
```
```python
);  
module_init(workqueue_init);  
module_exit(workqueue_exit);
```
我们来insmod看一下：
/mnt/3520d$insmod hi_work_queue.ko   
init work queue demo.  
just a demo for work queue.  
/mnt/3520d$
/mnt/3520d $ ps | grep “workqueue demo”
728 root         0 SW<  [workqueue demo]
你会发现多了一个内核线程workqueue demo，这就是我们代码中自己创建的。

