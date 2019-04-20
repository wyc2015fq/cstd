# s3c2410 RTC驱动框架linux内核源码分析 - h13 - 博客园
s3c2410 RTC驱动框架linux内核源码分析
```
代码
/*********************************************************************************************************
*  @Description:s3c2410的rtc驱动的实现，rtc(real time clock)实时时钟的驱动是个很好的
*    理解如果编写驱动的硬件，它包括了最基本的硬中断，软中断的底层机制；
*    s3c2410的RTC驱动的实现个人认为更是对linux设备驱动一个很好的例子，他是通过二层结构来
*    实现的一个驱动，上层是一个arm common的公共层，对上提供标准的通用的RTC操作接口，下层由
*    我们来实现针对自己的chip和自己要提供的功能来实现的一层驱动；
*
*  @FileTree:
**********************************************************************************************************
linux-2.6.14.6
|
|--arch
|    |
|    |--arm
|    |    |--mach-s3c2410
|    |    |    |-devs.c        //包含了对各个部件的resource的分配和定义，在这看rtc的资源；
|    |    |--common
|    |    |    |-rtctime.c        //一个arm平台的通用rtc函数层，它对上隐藏了各种soc的rtc driver的区别；
|    |    |--kernel
|    |    |    |-time.c        //内核的初始化例程time_init()会调用的xxx_cmos_xxx函数的实现；定义了全局自旋锁rtc_lock用来串行化所有CPU对RTC的操作
|
|--drivers
|    |--char
|    |    |-s3c2410-rtc.c            //具体的s3c2410上的rtc chip的驱动实现,如果需要在arm平台的
|    |                    板子上实现一个驱动，改写它就ok了。
|
|--include
|    |--asm-arm
|    |    |--arch-s3c2410
|    |    |    |-regs-rtc.h    //S3C2410 Internal RTC register definition refer to datasheet;
|    |    |-rtc.h                //arm平台rtc操作抽象层rtctime.c对应的.h
|    |--linux
|    |    |-time.h            //mktime的实现；
|    |    |-rtc.h                //公用RTC .h
*
*[小结]
*1)提供给user的接口，在arch/arm/common/rtctime.c,include/asm-arm/rtc.h中实现，调用操作硬件驱动在drivers/char/s3c2410-rtc.c,include/asm-arm/arch-s3c2410/regs-rtc.h实现;
*2)提供给kernel的接口，在arch/arm/kernel/time.c,include/linux/time.h中实现，调用操作硬件驱动在drivers/char/s3c2410-rtc.c,include/asm-arm/arch-s3c2410/regs-rtc.h实现;
**********************************************************************************************************
*
*  @Author:    liyangth@gmail.com         
*                                
*  @Function    List:    <Functions>
*
*                        <Static functions>
*
*    
*    @Changelog:
*    2007-06-24    LiYang    First version
*
*    @FQA:
*    [50%]Q1.在驱动中要将设备注册到总线，必须将设备封装成struct device_driver；调查这个结构体中的每个成员.
*    [0%]Q2.在板子（什么类型）上什么样的设备要用总线（什么类型）注册？
*    [90%]Q3.
*        struct device            --总线设备
*        struct device_driver    --总线设备驱动
*        struct platform_device    --平台设备
*        struct resource            --平台资源
*        
*    [！0%]Q4.初始化rtc register的函数的后面的flag具体控制什么? (在s2s65a里是否可以用它控制是softReset or hardwareReset)
*
*    [0%]Q5.什么时候调用suspend, resume?
**********************************************************************************************************/
/**//*****************************************************************************
 * Structures & Unions & Enums (#typedef)
 */
/**//*[include/linux/device.h] 
 *总线设备驱动结构体，将它注册到板子的总线上 
 */
static struct device_driver s3c2410_rtcdrv = ...{
    .name        = "s3c2410-rtc",
    .owner        = THIS_MODULE,
    .bus        = &platform_bus_type,    //总线类型，貌似不用管
    .probe        = s3c2410_rtc_probe,    //自检->初始化REG->注册到上一层
    .remove        = s3c2410_rtc_remove,    //注销
    .suspend    = s3c2410_rtc_suspend,    //[挂起？？？]
    .resume        = s3c2410_rtc_resume,    //[重起？？？]
};
/**//*[/include/asm-arm/rtc.h] 
 *底层特别操作集，将他注册到上层的arm common操作层 
 */
static struct rtc_ops s3c2410_rtcops = ...{            
    .owner        = THIS_MODULE,
    .open        = s3c2410_rtc_open,
    .release    = s3c2410_rtc_release,
    .ioctl        = s3c2410_rtc_ioctl,
    .read_time    = s3c2410_rtc_gettime,
    .set_time    = s3c2410_rtc_settime,
    .read_alarm    = s3c2410_rtc_getalarm,
    .set_alarm    = s3c2410_rtc_setalarm,
    .proc        = s3c2410_rtc_proc,
};
/**//*****************************************************************************
 * Global Variables
 */
 
 
 
s3c2410-rtc.c
|
|/**//* IRQ Handlers */
|-s3c2410_rtc_alarmirq(int irq, void *id, struct pt_regs *r)
|    |
|    |-rtc_update(1, RTC_AF | RTC_IRQF);    //获得中断标志，和唤醒read阻塞，异步通知；
|
|-s3c2410_rtc_tickirq(int irq, void *id, struct pt_regs *r)
|
|/**//* Update control registers,与硬件实现有关，refer to datasheet */
|-s3c2410_rtc_setaie(int to)
|-s3c2410_rtc_setpie(int to)
|-s3c2410_rtc_setfreq(int freq)
|
|/**//* 实现了要插到上层arm common层的具体的硬件操作,来填充struct rtc_ops，这个具体与硬件相关的操作集会用
|    register_rtc注册到上层的 */
|
|-s3c2410_rtc_gettime(struct rtc_time *rtc_tm)
|
|-s3c2410_rtc_settime(struct rtc_time *tm)
|
|-s3c2410_rtc_getalarm(struct rtc_wkalrm *alrm)
|
|-s3c2410_rtc_setalarm(struct rtc_wkalrm *alrm)
|
|/**//*
| * 插入到上层ioctl中的ioctl,上层中已经通过这个driver中的gettime,settime在其ioctl中实现了取得和设置时间
| *，和一些共同的ioctl操作了
| * 所以我们在这只要实现与硬件不同部分的ioctl操作 */
|-s3c2410_rtc_ioctl(unsigned int cmd, unsigned long arg)
|
|-s3c2410_rtc_proc(char *buf)
|-s3c2410_rtc_open(void)
|    |
|    |1.注册申请闹钟中断ISQ        --s3c2410_rtc_alarmirq
|    |2.周期中断ISQ            --s3c2410_rtc_tickirq
|    
|-s3c2410_rtc_release(void)
|
|/**//* Initialize RTC Regs */
|-s3c2410_rtc_enable(struct device *dev, int en)
|    |
|    |-if(!en)
|    |/**//*before power off, the RTCEN bit should be cleared to
|    |    0 to prevent inadvertent writing into RTC registers.*/
|    |1.将控制R的RTCEN位清0;
|    |
|    |2.disable interrupt.
|    |
|    |-else    /**//* re-enable the device, and check it is ok  */
|    |
|    |1.将控制R的RTCEN位致1。
|    |
|    |2.BCD count select.--0=Merge BCD counters
|    |
|    |3.RTC clock count reset.--0=noreset
|    |
|
|
|
|-s3c2410_rtc_probe(struct device *dev)
|    |[??]struct platform_device *pdev = to_platform_device(dev);    //通过这个设备找到它宿主平台的大设备；
|    |     struct resource *res;
|    |
|    |/**//* find the IRQs,RTC有2中中断，周期中断和闹钟中断 */
|    |-s3c2410_rtc_tickno = platform_get_irq(pdev, 1);    //从平台上取得一个IRQ号给这个设备；
|    |
|    |/**//* get the memory region */
|    |-res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
|    |
|    |/**//* 向内核申请资源空间 */
|    |-s3c2410_rtc_mem = request_mem_region(res->start, res->end-res->start+1,
                     pdev->name);    //res->start这些资源的分配和在哪个段下，可以看../mach-s3c2410/devs.c 
|    |
|    |/**//* 然后将物理地址映射到虚拟地址，这样驱动和内核就可以看到设备的I/O regs了 */
|    |-s3c2410_rtc_base = ioremap(res->start, res->end - res->start + 1);
|    |
|    |/**//* 初始化设备regs */
|    |-s3c2410_rtc_enable(dev, 1);    //可以用后面这个1(这个flag在s2s65a中可以用ram0-7来保存，掉电不清的)来控制是softreset还是hardwarereset.
|    |
|    |-s3c2410_rtc_setfreq(s3c2410_rtc_freq);    //设定RTC周期频率；
|    |
|    |/**//* 最关键的一步，将与具体不同的底层硬件相关的设备驱动注册给arm通用操作层common/rtctime.c */
|    |-register_rtc(&s3c2410_rtcops);
|
|-s3c2410_rtc_remove(struct device *dev)
|    |-unregister_rtc(&s3c2410_rtcops);    //从上一层将s3c2410的rtc的device dirver operation set拔下来。
|    |
|    |-s3c2410_rtc_setpie(0);                //disable 周期中断
|    |
|    |-s3c2410_rtc_setaie(0);                //disable alarm interrupt
|    |
|    |-iounmap(s3c2410_rtc_base);
|    |
|    |-release_resource(s3c2410_rtc_mem);
|    |
|    |-kfree(s3c2410_rtc_mem);        //[MQA]哪块kmalloc了呢，为什么这要free?
|
|
|#ifdef CONFIG_PM //如果电源控制开关打开
|
|-s3c2410_rtc_suspend(struct device *dev, pm_message_t state, u32 level)
|    |
|    |if (level == SUSPEND_POWER_DOWN)     //SUSPEND_POWER_DOWN在include/linux/device.h（generic, centralized driver model）中定义，这个里面是否是对设备的一些公用的行为的操作宏的定义呢？
|    |    |1.保存周期中断寄存器的值；
|    |    |
|    |    |2.从RTC中读出时间
|    |    |-s3c2410_rtc_gettime(&tm);        //[local variable]struct rtc_time tm;
|    |    |
|    |    |3.将从RTC取出的时间Convert Gregorian date to seconds since 01-01-1970 00:00:00.
|    |    |-rtc_tm_to_time(&tm, &time.tv_sec);    //[arch/arm/common/rtctime.c]
|    |    |
|    |    |4.将系统时间和RTC时间的差值保存到s3c2410_rtc_delta里;
|    |    |-save_time_delta(&s3c2410_rtc_delta, &time);    
|    |    |
|    |    |5.启动RTC，注意这次启动后面的falg为0了；
|    |    |-s3c2410_rtc_enable(dev, 0);
|
|
|-s3c2410_rtc_resume(struct device *dev, u32 level)
|    |
|    |1.启动RTC，注意这次启动后面的falg为1了；
|    |-s3c2410_rtc_enable(dev, 1);
|    |
|    |2.从RTC中读出时间
|    |-s3c2410_rtc_gettime(&tm);
|    |
|    |3.转换
|    |-rtc_tm_to_time(&tm, &time.tv_sec);
|    |
|    |4.利用在suspend中保存的delta来恢复系统时间
|    |-restore_time_delta(&s3c2410_rtc_delta, &time);
|    |    
|    |5.恢复周期中断寄存器的值；
|    
|[LiY]
|suspend(暂停，挂起)和resume(恢复，再开始)有点象关机前保存现场和开机后再恢复现场一样；
|
|#else
|#define s3c2410_rtc_suspend NULL
|#define s3c2410_rtc_resume  NULL
|#endif
|
|
|module_init(s3c2410_rtc_init)
|-s3c2410_rtc_init
|    |-driver_register(&s3c2410_rtcdrv);
|    |    |[purpose]register driver with bus;
|
|module_exit(s3c2410_rtc_exit)
|-s3c2410_rtc_exit(void)
|    |-driver_unregister(&s3c2410_rtcdrv)
/**//*****************************************************************************
 * Extern Function Details
 */
/**//**[drivers/base/driver.c]
 *    driver_register - register driver with bus
 *    @drv:    driver to register
 *
 *    We pass off most of the work to the bus_add_driver() call,
 *    since most of the things we have to do deal with the bus
 *    structures.
 *
 *    The one interesting aspect is that we setup @drv->unloaded
 *    as a completion that gets complete when the driver reference
 *    count reaches 0.
 */
int driver_register(struct device_driver * drv)
...{
    klist_init(&drv->klist_devices, klist_devices_get, klist_devices_put);
    init_completion(&drv->unloaded);
    return bus_add_driver(drv);
}
/**//**[drivers/base/platform.c]
 *    platform_get_irq - get an IRQ for a device
 *    @dev: platform device
 *    @num: IRQ number index
 */
int platform_get_irq(struct platform_device *dev, unsigned int num)
...{
    struct resource *r = platform_get_resource(dev, IORESOURCE_IRQ, num);
    return r ? r->start : 0;
}
/**//**[drivers/base/platform.c]
 *    platform_get_resource - get a resource for a device
 *    @dev: platform device
 *    @type: resource type
 *    @num: resource index
 */
struct resource *
platform_get_resource(struct platform_device *dev, unsigned int type,
              unsigned int num)
...{
    int i;
    for (i = 0; i < dev->num_resources; i++) ...{
        struct resource *r = &dev->resource[i];
        if ((r->flags & (IORESOURCE_IO|IORESOURCE_MEM|
                 IORESOURCE_IRQ|IORESOURCE_DMA))
            == type)
            if (num-- == 0)
                return r;
    }
    return NULL;
}
/**//*[../include/linux/ioport.h]
 * Resources are tree-like, allowing
 * nesting etc..
 */
struct resource ...{
    const char *name;
    unsigned long start, end;
    unsigned long flags;
    struct resource *parent, *sibling, *child;
};
/**//**[arch/arm/kernel/time.c]
 * save_time_delta - Save the offset between system time and RTC time
 * @delta: pointer to timespec to store delta
 * @rtc: pointer to timespec for current RTC time
 *
 * Return a delta between the system time and the RTC time, such
 * that system time can be restored later with restore_time_delta()
 *
 *[LiY]返回一个系统时间和RTC时间的差值，在重起或系统去读RTC后，可以再用这个差值(delta)来恢复系统时间(系统时间不一定非要和RTC时间相同的);
 */
void save_time_delta(struct timespec *delta, struct timespec *rtc)
...{
    set_normalized_timespec(delta,
                xtime.tv_sec - rtc->tv_sec,
                xtime.tv_nsec - rtc->tv_nsec);
}
/**//***[arch/arm/kernel/time.c]
 * restore_time_delta - Restore the current system time
 * @delta: delta returned by save_time_delta()
 * @rtc: pointer to timespec for current RTC time
 */
void restore_time_delta(struct timespec *delta, struct timespec *rtc)
...{
    struct timespec ts;
    set_normalized_timespec(&ts,
                delta->tv_sec + rtc->tv_sec,
                delta->tv_nsec + rtc->tv_nsec);
    do_settimeofday(&ts);
}
/**//*
 *[arch/arm/common/rtctime.c]
*/
void rtc_update(unsigned long num, unsigned long events)
...{
    spin_lock(&rtc_lock);
    rtc_irq_data = (rtc_irq_data + (num << 8)) | events;
    spin_unlock(&rtc_lock);
    wake_up_interruptible(&rtc_wait);
    kill_fasync(&rtc_async_queue, SIGIO, POLL_IN);
}
```
