# Linux 内核通知链机制的原理及实现 - 文章 - 伯乐在线
原文出处： [yangyefeng820803](http://blog.chinaunix.net/uid-31087949-id-5752653.html)
**一、概念：**
大多数内核子系统都是相互独立的，因此某个子系统可能对其它子系统产生的事件感兴趣。为了满足这个需求，也即是让某个子系统在发生某个事件时通知其它的子 系统，Linux内核提供了通知链的机制。通知链表只能够在内核的子系统之间使用，而不能够在内核与用户空间之间进行事件的通知。 通知链表是一个函数链表，链表上的每一个节点都注册了一个函数。当某个事情发生时，链表上所有节点对应的函数就会被执行。所以对于通知链表来说有一个通知 方与一个接收方。在通知这个事件时所运行的函数由被通知方决定，实际上也即是被通知方注册了某个函数，在发生某个事件时这些函数就得到执行。其实和系统调 用signal的思想差不多。
**二、数据结构：**
通知链有四种类型：
1、原子通知链（ Atomic notifier chains ）：通知链元素的回调函数（当事件发生时要执行的函数）只能在中断上下文中运行，不允许阻塞。对应的链表头结构：


```
struct atomic_notifier_head
{
	spinlock_t lock;
	struct notifier_block *head;
};
```
2、可阻塞通知链（ Blocking notifier chains ）：通知链元素的回调函数在进程上下文中运行，允许阻塞。对应的链表头：


```
struct blocking_notifier_head
{
	struct rw_semaphore rwsem;
	struct notifier_block *head;
};
```
3、原始通知链（ Raw notifier chains ）：对通知链元素的回调函数没有任何限制，所有锁和保护机制都由调用者维护。对应的链表头：


```
struct raw_notifier_head
{
      struct notifier_block *head;
};
```
4、SRCU 通知链（ SRCU notifier chains ）：可阻塞通知链的一种变体。对应的链表头：


```
struct srcu_notifier_head
{
	struct mutex mutex;
	struct srcu_struct srcu;
	struct notifier_block *head;
};
```
通知链的核心结构：


```
struct notifier_block
{
	int (*notifier_call)(struct notifier_block *, unsigned long, void *);
	struct notifier_block *next;
	int priority;
};
```
其中notifier_call是通知链要执行的函数指针，next用来连接其它的通知结构，priority是这个通知的优先级，同一条链上的notifier_block{}是按优先级排列的。内核代码中一般把通知链命名为xxx_chain, xxx_nofitier_chain这种形式的变量名。
**三、运作机制：**
通知链的运作机制包括两个角色：
- 被通知者：对某一事件感兴趣一方。定义了当事件发生时，相应的处理函数，即回调函数。但需要事先将其注册到通知链中（被通知者注册的动作就是在通知链中增加一项）。
- 通知者：事件的通知者。当检测到某事件，或者本身产生事件时，通知所有对该事件感兴趣的一方事件发生。他定义了一个通知链，其中保存了每一个被通知者对事件的处理函数（回调函数）。通知这个过程实际上就是遍历通知链中的每一项，然后调用相应的事件处理函数。
包括以下过程：
- 通知者定义通知链。
- 被通知者向通知链中注册回调函数。
- 当事件发生时，通知者发出通知（执行通知链中所有元素的回调函数）。
被通知者调用 notifier_chain_register 函数注册回调函数，该函数按照优先级将回调函数加入到通知链中：


```
staticint notifier_chain_register(struct notifier_block **nl,struct notifier_block *n)
{
	while ((*nl) != NULL)
	{
	   if (n->priority > (*nl)->priority)
	      break;
	   nl = &((*nl)->next);
	}
        n->next = *nl;
	rcu_assign_pointer(*nl, n);return 0;
}
```
注销回调函数则使用 notifier_chain_unregister 函数，即将回调函数从通知链中删除：


```
staticint notifier_chain_unregister(struct notifier_block **nl,struct notifier_block *n)
{
	while ((*nl) != NULL)
	{
	  if ((*nl) == n)
	  {
	    rcu_assign_pointer(*nl, n->next);return 0;
	  }nl = &((*nl)->next);
	}
	return -ENOENT;
}
```
通知者调用 notifier_call_chain 函数通知事件的到达，这个函数会遍历通知链中所有的元素，然后依次调用每一个的回调函数（即完成通知动作）：


```
staticint __kprobes notifier_call_chain(struct notifier_block **nl,unsignedlong val,void*v,int nr_to_call,int*nr_calls)
{
	int ret = NOTIFY_DONE;
	struct notifier_block *nb, *next_nb;nb = rcu_dereference(*nl);while (nb && nr_to_call)
	{
	  next_nb = rcu_dereference(nb->next);
	#ifdef CONFIG_DEBUG_NOTIFIERS
	if (unlikely(!func_ptr_is_kernel_text(nb->notifier_call)))
	{
	  WARN(1, "Invalid notifier called!");
	  nb = next_nb;
	  continue;
	}
	#endif
	ret = nb->notifier_call(nb, val, v);
	if (nr_calls)
	(*nr_calls)++;
	if ((ret & NOTIFY_STOP_MASK) == NOTIFY_STOP_MASK)
	  break;
	nb = next_nb;
	nr_to_call--;
	}
	return ret;
}
```
参数nl是通知链的头部，val表示事件类型，v用来指向通知链上的函数执行时需要用到的参数，一般不同的通知链，参数类型也不一样，例如当通知一个网卡被注册时，v就指向net_device结构，nr_to_call表示准备最多通知几个，-1表示整条链都通知，nr_calls非空的话，返回通知了多少个。
每个被执行的notifier_block回调函数的返回值可能取值为以下几个：
- NOTIFY_DONE：表示对相关的事件类型不关心。
- NOTIFY_OK：顺利执行。
- NOTIFY_BAD：执行有错。
- NOTIFY_STOP：停止执行后面的回调函数。
- NOTIFY_STOP_MASK：停止执行的掩码。
Notifier_call_chain()把最后一个被调用的回调函数的返回值作为它的返回值。
**四、举例应用：**
在这里，写了一个简单的通知链表的代码。实际上，整个通知链的编写也就两个过程：
- 首先是定义自己的通知链的头节点，并将要执行的函数注册到自己的通知链中。
- 其次则是由另外的子系统来通知这个链，让其上面注册的函数运行。
这里将第一个过程分成了两步来写，第一步是定义了头节点和一些自定义的注册函数（针对该头节点的），第二步则是使用自定义的注册函数注册了一些通知链节点。分别在代码buildchain.c与regchain.c中。发送通知信息的代码为notify.c。
代码1 buildchain.c。它的作用是自定义一个通知链表test_chain，然后再自定义两个函数分别向这个通知链中加入或删除节点，最后再定义一个函数通知这个test_chain链：


```
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/notifier.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");/*
* 定义自己的通知链头结点以及注册和卸载通知链的外包函数
*//*
* RAW_NOTIFIER_HEAD是定义一个通知链的头部结点，
* 通过这个头部结点可以找到这个链中的其它所有的notifier_block
*/
static RAW_NOTIFIER_HEAD(test_chain);
/*
* 自定义的注册函数，将notifier_block节点加到刚刚定义的test_chain这个链表中来
* raw_notifier_chain_register会调用notifier_chain_register
*/
int register_test_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_register(&test_chain, nb);
}
EXPORT_SYMBOL(register_test_notifier);
int unregister_test_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_unregister(&test_chain, nb);
}
EXPORT_SYMBOL(unregister_test_notifier);
/*
* 自定义的通知链表的函数，即通知test_chain指向的链表中的所有节点执行相应的函数
*/
int test_notifier_call_chain(unsigned long val, void *v)
{
	return raw_notifier_call_chain(&test_chain, val, v);
}
EXPORT_SYMBOL(test_notifier_call_chain);
/*
* init and exit
*/
static int __init init_notifier(void)
{
	printk("init_notifiern");
	return 0;
}
static void __exit exit_notifier(void)
{
	printk("exit_notifiern");
}
module_init(init_notifier);
module_exit(exit_notifier);
```
代码2 regchain.c。该代码的作用是将test_notifier1 test_notifier2 test_notifier3这三个节点加到之前定义的test_chain这个通知链表上，同时每个节点都注册了一个函数：


```
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/notifier.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");/*
* 注册通知链
*/
extern int register_test_notifier(struct notifier_block*);
extern int unregister_test_notifier(struct notifier_block*);static int test_event1(struct notifier_block *this, unsigned long event, void *ptr)
{
	printk("In Event 1: Event Number is %dn", event);
	return 0;
}
static int test_event2(struct notifier_block *this, unsigned long event, void *ptr)
{
	printk("In Event 2: Event Number is %dn", event);
	return 0;
}
static int test_event3(struct notifier_block *this, unsigned long event, void *ptr)
{
	printk("In Event 3: Event Number is %dn", event);
	return 0;
}
/*
* 事件1，该节点执行的函数为test_event1
*/
static struct notifier_block test_notifier1 =
{
	.notifier_call = test_event1,
};
/*
* 事件2，该节点执行的函数为test_event1
*/
static struct notifier_block test_notifier2 =
{
	.notifier_call = test_event2,
};
/*
* 事件3，该节点执行的函数为test_event1
*/
static struct notifier_block test_notifier3 =
{
	.notifier_call = test_event3,
};
/*
* 对这些事件进行注册
*/
static int __init reg_notifier(void)
{
	int err;
	printk("Begin to register:n");
	err = register_test_notifier(&test_notifier1);
	if (err)
	{
	printk("register test_notifier1 errorn");
	return -1;
	}
	printk("register test_notifier1 completedn");
	err = register_test_notifier(&test_notifier2);
	if (err)
	{
	printk("register test_notifier2 errorn");
	return -1;
	}
	printk("register test_notifier2 completedn");
	err = register_test_notifier(&test_notifier3);
	if (err)
	{
	printk("register test_notifier3 errorn");
	return -1;
	}
	printk("register test_notifier3 completedn");
	return err;
}
/*
* 卸载刚刚注册了的通知链
*/
static void __exit unreg_notifier(void)
{
	printk("Begin to unregistern");
	unregister_test_notifier(&test_notifier1);
	unregister_test_notifier(&test_notifier2);
	unregister_test_notifier(&test_notifier3);
	printk("Unregister finishedn");
}
module_init(reg_notifier);
module_exit(unreg_notifier);
```
代码3 notify.c。该代码的作用就是向test_chain通知链中发送消息，让链中的函数运行：


```
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/notifier.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");extern int test_notifier_call_chain(unsigned long val, void *v);/*
* 向通知链发送消息以触发注册了的函数
*/
static int __init call_notifier(void)
{
	int err;
	printk("Begin to notify:n");
	/*
	* 调用自定义的函数，向test_chain链发送消息
	*/
	printk("==============================n");
	err = test_notifier_call_chain(1, NULL);
	printk("==============================n");
	if (err)
	printk("notifier_call_chain errorn");
	return err;
}
static void __exit uncall_notifier(void)
{
	printk("End notifyn");
}
module_init(call_notifier);
module_exit(uncall_notifier);
```
Makefile文件：


```
obj-m:=buildchain.o regchain.o notify.o
CURRENT_PATH := $(shell pwd)
LINUX_KERNEL := $(shell uname -r)
KERNELDIR := /usr/src/linux-headers-$(LINUX_KERNEL)all:
make -C $(KERNELDIR) M=$(CURRENT_PATH) modulesclean:
make -C $(KERNELDIR) M=$(CURRENT_PATH) clean
```
运行（注意insmod要root权限）：


```
make
insmod buildchain.ko
insmod regchain.ko
insmod notify.ko
```
这样就可以看到通知链运行的效果了：


```
init_notifier
Begin to register:
register test_notifier1 completed
register test_notifier2 completed
register test_notifier3 completed
Begin to notify:
==============================
In Event 1: Event Number is 1
In Event 2: Event Number is 1
In Event 3: Event Number is 1
```
