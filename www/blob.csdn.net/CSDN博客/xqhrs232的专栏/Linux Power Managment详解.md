# Linux Power Managment详解 - xqhrs232的专栏 - CSDN博客
2011年09月20日 14:54:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：927
原文地址::[http://www1.chinaunix.com/space.php?uid=24517893&do=blog&id=254740](http://www1.chinaunix.com/space.php?uid=24517893&do=blog&id=254740)
相关技术网帖::  标准Linux休眠和唤醒机制分析        [http://womendu.iteye.com/blog/1069922](http://womendu.iteye.com/blog/1069922)
Linux Power Managment
谨以此文纪念过往的岁月
一．前言
在这个对节能要求越来越严格的年代，对设备的电源管理就显的很重要的了，尤其对于可移动设备，在电源有限的情况下，续航能力就显的很重要的。在本文中将介绍linux是如何对设备电源进行管理的。
二．睡眠
Linux的电源管理的主要几个文件集中在/kernel/power/main.c和/driver/base/power/main.c中。主要以platform设备来看linux的睡眠和唤醒。
 不过在看具体的代码之前，需要了解suspend的几个状态，在linux中定义了两种：
#define PM_SUSPEND_ON                ((__force suspend_state_t) 0)  suspend是否是打开的
#define PM_SUSPEND_STANDBY    ((__force suspend_state_t) 1)  suspend备用
#define PM_SUSPEND_MEM           ((__force suspend_state_t) 3)  mem suspend
#define PM_SUSPEND_MAX             ((__force suspend_state_t) 4)
还有几个跟具体平台相关的结构体：
struct platform_suspend_ops {
         int (*valid)(suspend_state_t state);  --判定平台是否支持该种睡眠状态
         int (*begin)(suspend_state_t state);  --初始化一个给定的系统过渡到睡眠状态
         int (*prepare)(void);              --睡眠前的准备
         int (*enter)(suspend_state_t state);  --真正的进入睡眠
         void (*finish)(void); --当系统离开睡眠模式，在nonboot cpus正确后，恢复设备之前调用。
         void (*end)(void); --在正确恢复设备之后调用，表明系统进入工作状态或者在过渡到睡眠状态时出现错误。
         void (*recover)(void);  --从一个挂起失败中恢复系统
};
以s3c6410为例，在s3c6410_pm_init函数中调用suspend_set_ops来设置上述函数，不过该函数本质是将一个全局变量的suspend_ops设置为某一特定平台的platform_suspend_ops。
static struct platform_suspend_ops s3c6410_pm_ops = {
         .enter                = s3c6410_pm_enter,
         .valid                  = suspend_valid_only_mem,
};
在s3c6410中仅仅实现了上面两个函数并没有其他函数。可以从suspend_valid_only_mem的函数名中知道，该平台仅仅支持SUSPEND_MEM。而enter将会在下面的源码中涉及。
我们还是从一个函数开始神秘的linux电源管理。pm_suspend为内核提供了一个可见的睡眠函数，通过调用这个函数可以让系统进入睡眠。我们以传入的参数为PM_SUSPEND_MEM为例来开始我们的电源管理之旅，不过suspend涉及的内容太多，我们更加的关注设备的电源管理。
int pm_suspend(suspend_state_t state)
{
         if (state > PM_SUSPEND_ON && state <= PM_SUSPEND_MAX)
                   return enter_state(state);
         return -EINVAL;
}
对enter_state进行一些删减就会发现没有太多东西。
static int enter_state(suspend_state_t state)
{
         if (!valid_state(state)) --检测平台是否支持传入的睡眠状态。S3c6410仅支持mem状态。
                   return -ENODEV;
         if (!mutex_trylock(&pm_mutex)) --锁定互斥锁
                   return -EBUSY;
         sys_sync();    --同步系统，将缓存中的数据回写到块设备中
下面就将进入真正的suspend。
         suspend_prepare();
         suspend_devices_and_enter(state);
         suspend_finish();
当程序执行到此就证明系统不但睡眠还被唤醒了。不要看从上面的code到这儿仅仅几句话，但是不知道经过了多少岁月。我们将仔细的来看上面的三个函数。
         mutex_unlock(&pm_mutex);
         return 0;
}
对于下面的三个函数我更加的关注第二函数，没有办法啊，第一个和第三个函数我看不懂啊，其中主要是存储和恢复用户空间和进程。对于这个部分，确是不懂。饭还是慢慢来吃啊，一口吃不成一个胖子啊。
suspend_prepare();
suspend_devices_and_enter(state);
suspend_finish();
int suspend_devices_and_enter(suspend_state_t state)
{
         int error, ftrace_save;
         if (!suspend_ops)
                   return -ENOSYS;
         if (suspend_ops->begin) { --如果平台支持begin的话，执行begin
                   error = suspend_ops->begin(state);
                   if (error)
                            goto Close;
         }
         suspend_console();  --将console挂起，从这儿向下系统将不会打印信息，调用printk将会将信息保存在一个缓冲区中。知道console被再次resume，系统才会恢复输出。
         ftrace_save = __ftrace_enabled_save();  --？？？？？？
         error = device_suspend(PMSG_SUSPEND); --将设备挂起
         if (error) {
                   goto Recover_platform;
         }
         if (suspend_ops->prepare) { --睡前准备。
                   error = suspend_ops->prepare();
                   if (error)
                            goto Resume_devices;
         }
         error = disable_nonboot_cpus(); --禁止所有的非引导CPU，在多核中有用，一个CPU就蜕化为0
         if (!error && !suspend_test(TEST_CPUS))
                   suspend_enter(state);  --进入睡眠
         enable_nonboot_cpus();
 Finish:
         if (suspend_ops->finish)
                   suspend_ops->finish();
 Resume_devices:
         device_resume(PMSG_RESUME);  --resume设备
         __ftrace_enabled_restore(ftrace_save);
         resume_console();        --resume 终端输出
 Close:
         if (suspend_ops->end)
                   suspend_ops->end();
         return error;
Recover_platform:
         if (suspend_ops->recover)
                   suspend_ops->recover();
         goto Resume_devices;
}
那来看设备挂起
int device_suspend(pm_message_t state)
{
         int error;
         might_sleep();  --????????
         error = dpm_prepare(state); --准备挂起
         if (!error)
                   error = dpm_suspend(state); --挂起设备
         return error;
}
其实我们真正关心的设备挂起就在这里，在这里就会看到我们平时用的很多的platform平台中的suspend和resum。
static int dpm_prepare(pm_message_t state)
{
         struct list_head list;
         int error = 0;
         INIT_LIST_HEAD(&list);
         mutex_lock(&dpm_list_mtx);
         transition_started = true;
         while (!list_empty(&dpm_list)) { 
                   struct device *dev = to_device(dpm_list.next);  
                   get_device(dev);
                   dev->power.status = DPM_PREPARING;
                   mutex_unlock(&dpm_list_mtx);
                   error = prepare_device(dev, state);
                   mutex_lock(&dpm_list_mtx);
                   if (error) {
                            dev->power.status = DPM_ON;
                            if (error == -EAGAIN) {
                                     put_device(dev);
                                     continue;
                            }
                            put_device(dev);
                            break;
                   }
                   dev->power.status = DPM_SUSPENDING;
                   if (!list_empty(&dev->power.entry))
                            list_move_tail(&dev->power.entry, &list);
                   put_device(dev);
         }
         list_splice(&list, &dpm_list);
         mutex_unlock(&dpm_list_mtx);
         return error;
}
上面的函数如果对设备驱动没有一个大概的了解的话，会看得一头雾水的。那我就慢慢道来，其实也是一种知识的回顾。
list_empty(&dpm_list)学过linux的都知道这句是判定dpm_list为空，那dpm_list是什么东西呢，这个链表上有什么东西呢？不急，咱们还是以platform为例，来看这个链表上有什么。从开辟一个platform设备开始platform_device_alloc-> device_initialize-> device_pm_init
static inline void device_pm_init(struct device *dev)
{
         dev->power.status = DPM_ON;
}
在开始的时候device的power.status会被设置为DPM_ON。
在platform_device_add中我们还要关注一个赋值pdev->dev.bus = &platform_bus_type;设备的总线类型为platform_bus_type，那我来看看这个总线里有什么。
struct bus_type platform_bus_type = {
         .pm           = PLATFORM_PM_OPS_PTR,
};
static struct pm_ext_ops platform_pm_ops = {
         .base = {
                   .prepare = platform_pm_prepare,
                   .complete = platform_pm_complete,
                   .suspend = platform_pm_suspend,
                   .resume = platform_pm_resume,
                   .freeze = platform_pm_freeze,
                   .thaw = platform_pm_thaw,
                   .poweroff = platform_pm_poweroff,
                   .restore = platform_pm_restore,
         },
         .suspend_noirq = platform_pm_suspend_noirq,
         .resume_noirq = platform_pm_resume_noirq,
         .freeze_noirq = platform_pm_freeze_noirq,
         .thaw_noirq = platform_pm_thaw_noirq,
         .poweroff_noirq = platform_pm_poweroff_noirq,
         .restore_noirq = platform_pm_restore_noirq,
};
#define PLATFORM_PM_OPS_PTR  &platform_pm_ops
其实对于每一种总线都会有自己的的struct pm_ext_ops *pm;就是电源管理操作集。你看上面的操作集就定义了关于platform总线的pm管理。上面的device_pm_init仅仅将设备的pm状态改变，并没有具体的操作将device加到dpm_list这个链表上。
platform_device_add->device_add->device_pm_add这个函数挺熟悉的，不过以前看platform的时候并没有仔细的看这个函数。今天我们来看这个函数，所谓学习就是今与昔所学相互印证。
void device_pm_add(struct device *dev)
{
         mutex_lock(&dpm_list_mtx);
         list_add_tail(&dev->power.entry, &dpm_list); 
         mutex_unlock(&dpm_list_mtx);
}
你看这个函数很简单，对就是很简单，一个简简单单的链表添加，就完成了。在linux中，你会发现有很多东西都是采用链表将东西串联起来，利用链表将以设备挂到一个个不同的链表中，实现一个设备的不同的管理。
在device的结构体中有struct dev_pm_info         power;这个成员就是用于表明该设备当前的电源状态。
enum dpm_state {
         DPM_INVALID,
         DPM_ON,  --正常工作状态
         DPM_PREPARING, --device准备进行pm转化
         DPM_RESUMING, --设备即将被唤醒
         DPM_SUSPENDING, --设备即将睡眠
         DPM_OFF,      --设备此时无效
         DPM_OFF_IRQ,  --设备处于深度睡眠
};
struct dev_pm_info {
         pm_message_t                 power_state; 
         unsigned           can_wakeup:1;   
         unsigned           should_wakeup:1;
         enum dpm_state              status;               
         struct list_head        entry;  --这个成员名称就暴露了其作用即是作为一个入口。就可以通过这个链表的节点逆流而上，找到其所属的device
}
到此就应该了解了dpm_list在哪儿被添加了成员。
我们再次回到上面dpm_prepare的函数，继续来看，为了方便从上面的函数中截取一段主要的来看。
while (!list_empty(&dpm_list)) { 
                   struct device *dev = to_device(dpm_list.next);  
                   get_device(dev);
                   dev->power.status = DPM_PREPARING;
                   prepare_device(dev, state);
                   dev->power.status = DPM_SUSPENDING;
                   if (!list_empty(&dev->power.entry))
                            list_move_tail(&dev->power.entry, &list);
                   put_device(dev);
         }
         list_splice(&list, &dpm_list);
上面的code就是从dpm_list这个链表上将一个一个设备拆下来，进行prepare，添加到另外的一个list中。在所有的设备都prepare好了后，直接将list和dpm_list合并就行了。还省了很多资源。
static int prepare_device(struct device *dev, pm_message_t state)
{
         int error = 0;
         down(&dev->sem);
         if (dev->bus && dev->bus->pm && dev->bus->pm->base.prepare) {
                   error = dev->bus->pm->base.prepare(dev);
                   if (error)
                            goto End;
         }
         if (dev->type && dev->type->pm && dev->type->pm->prepare) {
                   error = dev->type->pm->prepare(dev);
                   if (error)
                            goto End;
         }
         if (dev->class && dev->class->pm && dev->class->pm->prepare) {
                   error = dev->class->pm->prepare(dev);
         }
 End:
         up(&dev->sem);
         return error;
}
上面的函数很清晰明了，就是调用该设备所属的bus，type以及class的prepare，当然前提是这些东东都存在的情况下。以一个通用的platform设备为例，其bus->pm=platform_pm_ops，那很明显其prepare为platform_pm_prepare，其实现如下：
static int platform_pm_prepare(struct device *dev)
{
         struct device_driver *drv = dev->driver;
         int ret = 0;
         if (drv && drv->pm && drv->pm->prepare) 
                   ret = drv->pm->prepare(dev);
         return ret;
}
其还是调用了该设备driver的prepare，当然还是其存在的情况下。
在设备prepare完成后就应该是dpm_suspend了。其实现的原理与prepare类似，不讲述了。
下面就来看suspend_enter这个函数。
static int suspend_enter(suspend_state_t state)
{
         int error = 0;
         device_pm_lock();
         arch_suspend_disable_irqs();  --禁止中断
         if ((error = device_power_down(PMSG_SUSPEND))) { --关闭设备电源
                   goto Done;
         }
         error = suspend_ops->enter(state); --进入具体平台的suspend，在其中会使系统进入睡眠程序停止运行，直到系统再次运行才会执行下面的code
         device_power_up(PMSG_RESUME); --恢复设备电源
 Done:
         arch_suspend_enable_irqs(); --使能中断
         BUG_ON(irqs_disabled());
         device_pm_unlock();
         return error;
}
这个函数比较特别一点，函数说明是关闭特殊设备，不知道做何意解。
int device_power_down(pm_message_t state)
{
         struct device *dev;
         int error = 0;
         list_for_each_entry_reverse(dev, &dpm_list, power.entry) {
                   error = suspend_device_noirq(dev, state);
                   if (error) {
                                     break;
                   }
                   dev->power.status = DPM_OFF_IRQ;
         }
         if (!error)
                   error = sysdev_suspend(state);
         if (error)
                   dpm_power_up(resume_event(state));
         return error;
}
关闭某一个设备，该函数运行于所有中断被关闭仅有一个CPU在运行。
static int suspend_device_noirq(struct device *dev, pm_message_t state)
{
         int error = 0;
         if (!dev->bus)
                   return 0;
         if (dev->bus->pm) {
                   error = pm_noirq_op(dev, dev->bus->pm, state);
         } else if (dev->bus->suspend_late) {
                   error = dev->bus->suspend_late(dev, state);
                   suspend_report_result(dev->bus->suspend_late, error);
         }
         return error;
}
static int pm_noirq_op(struct device *dev, struct pm_ext_ops *ops,pm_message_t state)
{
         int error = 0;
         switch (state.event) {
         case PM_EVENT_SUSPEND:
                   if (ops->suspend_noirq) {
                            error = ops->suspend_noirq(dev);
                   }
                   break;
         case PM_EVENT_RESUME:
                   if (ops->resume_noirq) {
                            error = ops->resume_noirq(dev);
                   }
                   Break
}
Return err;
}
还以platform为例：
static int platform_pm_suspend_noirq(struct device *dev)
{
         struct platform_driver *pdrv;
         int ret = 0;
         if (!dev->driver)
                   return 0;
         pdrv = to_platform_driver(dev->driver);
         if (pdrv->pm) {   --当pdrv->pm存在时
                   if (pdrv->pm->suspend_noirq)  
                            ret = pdrv->pm->suspend_noirq(dev);
         } else {  --否则调用suspend_late
                   ret = platform_legacy_suspend_late(dev, PMSG_SUSPEND);
         }
         return ret;
}
static int platform_legacy_suspend_late(struct device *dev, pm_message_t mesg)
{
         struct platform_driver *drv = to_platform_driver(dev->driver);
         struct platform_device *pdev;
         int ret = 0;
         pdev = container_of(dev, struct platform_device, dev);
         if (dev->driver && drv->suspend_late)
                   ret = drv->suspend_late(pdev, mesg);
         return ret;
}
上面就是设备的suspend，而设备的唤醒则相反将前期禁止的中断打开，使能所有的CPU，再次将所suspend的设备resume。将suspend所做的一切的一切都恢复原样。这里就不描述了。各位看官对linux的电源管理应该有了一个总体的理解。
三．总结
对于Android的睡眠则是在其上添加wakelock，这个可以说是对linux的一点补充吧。以后有时间再看。
