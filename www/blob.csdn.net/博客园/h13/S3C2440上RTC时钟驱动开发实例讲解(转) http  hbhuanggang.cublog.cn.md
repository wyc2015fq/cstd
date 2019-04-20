# S3C2440上RTC时钟驱动开发实例讲解(转) http://hbhuanggang.cublog.cn - h13 - 博客园
**一、开发环境**
- 主  机：VMWare--Fedora 9
- 开发板：Mini2440--64MB Nand, Kernel:2.6.30.4
- 编译器：arm-linux-gcc-4.3.2
**二、相关概念**
1、平台设备：
通常在Linux中，把SoC系统中集成的独立外设单元(如：I2C、IIS、RTC、看门狗等)都被当作平台设备来处理。在Linux中用platform_device结构体来描述一个平台设备，在2.6.30.4内核中定义在：include/linux/platform_device.h中，如下： 
struct platform_device {
    const char    * name;   //设备名称
    int        id;
    struct device    dev;
    u32        num_resources;       //设备使用各类资源的数量
    struct resource    * resource;  //设备使用的资源
    struct platform_device_id    *id_entry;
};
现在你不必深入理解这个结构体，只要知道在Linux中是用这个结构体来定义一些平台设备的。比如在：arch/arm/plat-s3c24xx/devs.c中就定义了很多平台设备，下面我就只贴出RTC这一种的： 
* RTC */
static struct resource s3c_rtc_resource[] = {  //定义了RTC平台设备使用的资源，这些资源在驱动中都会用到
    [0] = {  //IO端口资源范围
        .start = S3C24XX_PA_RTC,
        .end = S3C24XX_PA_RTC + 0xff,
        .flags = IORESOURCE_MEM,
    },
    [1] = {  //RTC报警中断资源
        .start = IRQ_RTC,
        .end = IRQ_RTC,
        .flags = IORESOURCE_IRQ,
    },
    [2] = {  //TICK节拍时间中断资源
        .start = IRQ_TICK,
        .end = IRQ_TICK,
        .flags = IORESOURCE_IRQ
    }
};
struct platform_device s3c_device_rtc = {  //定义了RTC平台设备
    .name         = "s3c2410-rtc",  //设备名称
    .id         = -1,
    .num_resources     = ARRAY_SIZE(s3c_rtc_resource), //资源数量
    .resource     = s3c_rtc_resource,  //引用上面定义的资源
};
EXPORT_SYMBOL(s3c_device_rtc);
好了，定义了平台设备，那系统是怎么来使用他的呢？我们打开：arch/arm/mach-s3c2440/mach-smdk2440.c这个ARM 2440平台的系统入口文件，可以看到在系统初始化函数smdk2440_machine_init中是使用platform_add_devices这个函数将一些平台设备添加到系统中的，如下：(至于系统是如何实现添加平台设备的，这里我们不必研究，这些Linux系统都已经做好了的，我们要研究的是后面平台设备的驱动是如何实现的) 
static struct platform_device *smdk2440_devices[] __initdata = {
    &s3c_device_usb,
    &s3c_device_lcd,
    &s3c_device_wdt,
    &s3c_device_i2c0,
    &s3c_device_iis,DE>
&s3c_device_rtc,//这里我们添加上RTC平台设备，默认是没添加的
};  //平台设备列表，也就是说我们要使用一个新的平台设备要先在上面定义，然后加到这个列表中，最后到驱动层去实现该设备的驱动
static void __init smdk2440_machine_init(void)
{
    s3c24xx_fb_set_platdata(&smdk2440_fb_info);
    s3c_i2c0_set_platdata(NULL);
//将上面列表中的平台设备添加到系统总线中
    platform_add_devices(smdk2440_devices, ARRAY_SIZE(smdk2440_devices));
    smdk_machine_init();
}
2、平台设备驱动：
这里所讲的平台设备驱动是指具体的某种平台设备的驱动，比如上面讲的RTC平台设备，这里就是指RTC平台设备驱动。在Linux中，系统还为平台设备定义了平台驱动结构体platform_driver，就好比系统为字符设备定义了file_operations一样，但不要把平台设备跟字符设备、块设备、网络设备搞成了并列的概念，因平台设备也可以是字符设备等其他设备。注意：在被定义为平台设备的字符设备的驱动中，除了要实现字符设备驱动中file_operations的open、release、read、write等接口函数外，还要实现平台设备驱动中platform_driver的probe、remove、suspend、resume等接口函数。好了，在我们搞明白上面这些后，下面我们就来具体详细分析讲解RTC平台设备的驱动现实。
**三、实例讲解**
1、RTC在Linux中的整体结构：
就个人理解，RTC在Linux中整体结构分为两个部分。第一个是部分就是上面所讲的作为平台设备被挂接到系统总线中，这里我把他叫做设备层(呵呵，可能不是很准确的叫法)；第二部分就是驱动部分，这里叫做驱动层。在Linux中要使一个驱动在不同的平台中都能够使用似乎是不可能的，所以我们先看2.6.30.4内核驱动中的RTC部分是单独的一个文件夹，在文件夹中包含了很多不同体系结构的RTC驱动，当然也有S3C2440的RTC驱动，然而在这些驱动中他们都使用了一组文件里面的方法，那么这组文件就是RTC的核心(注意这里的核心不是指对RTC硬件的操作，指的是对RTC操作的方法。对硬件寄存器的操作还是在具体的驱动中)。好了，我们还是用图来说明这种关系吧！！
2、RTC硬件原理图分析：以下是S3C2440AL内部集成的RTC模块结构图和一个外部的晶振接口图
我们从S3C2440内部RTC模块结构图和数据手册得知，RTC在Linux中主要实现两种功能，分别是系统掉电后的时间日期维持和时间日期报警(类似定时器功能)。
①、时间日期维持功能：
主要是由RTC实时时钟控制寄存器RTCCON进行功能的使能控制，由节拍时间计数寄存器TICNT来产生节拍时间中断来实现实时操作系统功能相关的时间和实时同步。其中对时间日期的操作实际上是对BCD码操作，而BCD码则是由一系列的寄存器组成(BCD秒寄存器BCDSEC、BCD分寄存器BCDMIN、BCD小时寄存器BCDHOUR、BCD日期寄存器BCDDATE、BCD日寄存器BCDDAY、BCD月寄存器BCDMON、BCD年寄存器BCDYEAR)。
②、报警功能：
主要由RTC报警控制寄存器RTCALM进行功能使能控制，并产生报警中断。报警时间日期的设置也是对一系列的寄存器进行操作(报警秒数据寄存器ALMSEC、报警分钟数据寄存器ALMMIN、报警小时数据寄存器ALMHOUR、报警日期数据寄存器ALMDATE、报警月数据寄存器ALMMON、报警年数据寄存器ALMYEAR)。
3、RTC驱动实现步骤(建立驱动文件my2440_rtc.c)：
注意：在每步中，为了让代码逻辑更加有条理和容易理解，就没有考虑代码的顺序，比如函数要先定义后调用。如果要编译此代码，请严格按照C语言的规范来调整代码的顺序。
**①、**依然是驱动程序的最基本结构：RTC驱动的初始化和退出部分及其他，如下： 
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
/*RTC平台驱动结构体，平台驱动结构体定义在platform_device.h中，该结构体内的接口函数在第②、④步中实现*/
static struct platform_driver rtc_driver = 
{
    .probe   = rtc_probe, /*RTC探测函数，在第②步中实现*/
    .remove  = __devexit_p(rtc_remove),/*RTC移除函数,在第④步实现,为何使用__devexit_p,在该函数实现的地方再讲*/
    .suspend = rtc_suspend, /*RTC挂起函数，在第④步中实现*/
    .resume  = rtc_resume, /*RTC恢复函数，在第④步中实现*/
    .driver  = 
    {
        /*注意这里的名称一定要和系统中定义平台设备的地方一致，这样才能把平台设备与该平台设备的驱动关联起来*/
        .name   = "s3c2410-rtc", 
        .owner  = THIS_MODULE,
    },
};
static int __init rtc_init(void)
{
    /*将RTC注册成平台设备驱动*/
    return platform_driver_register(&rtc_driver);
}
static void __exit rtc_exit(void)
{
    /*注销RTC平台设备驱动*/
    platform_driver_unregister(&rtc_driver);
}
module_init(rtc_init);
module_exit(rtc_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huang Gang");
MODULE_DESCRIPTION("My2440 RTC driver");
**②、**RTC平台驱动结构中探测函数rtc_probe的实现。探测就意味着在系统总线中去检测设备的存在，然后获取设备有用的相关资源信息，以便我们使用这些信息。代码如下： 
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/rtc.h>
#include <linux/ioport.h>
#include <plat/regs-rtc.h>
/*定义了一个用来保存RTC的IO端口占用的IO空间和经过虚拟映射后的内存地址*/
static struct resource *rtc_mem;
static void __iomem *rtc_base;
/*定义了两个变量来保存RTC报警中断号和TICK节拍时间中断号，NO_IRQ宏定义在irq.h中*/
static int rtc_alarmno = NO_IRQ;
static int rtc_tickno = NO_IRQ;
/*申明并初始化一个自旋锁rtc_pie_lock，对RTC资源进行互斥访问*/
static DEFINE_SPINLOCK(rtc_pie_lock);
/*RTC平台驱动探测函数，注意这里为什么要使用一个__devinit，也到rtc_remove实现的地方一起讲*/
static int __devinit rtc_probe(struct platform_device *pdev)
{
    int ret;
    struct rtc_device *rtc; /*定义一个RTC设备类，rtc_device定义在rtc.h中*/
    struct resource *res; /*定义一个资源，用来保存获取的RTC的资源*/
/*在系统定义的RTC平台设备中获取RTC报警中断号
     platform_get_irq定义在platform_device.h中*/
    rtc_alarmno = platform_get_irq(pdev, 0); 
    if (rtc_alarmno < 0) 
    { 
/*获取RTC报警中断号不成功错误处理
         dev_err定义在device.h中，在platform_device.h中已经引用，所以这里就不需再引用了*/
        dev_err(&pdev->dev, "no irq for alarm\n");
        return -ENOENT;
    }
//在系统定义的RTC平台设备中获取TICK节拍时间中断号
    rtc_tickno = platform_get_irq(pdev, 1);
    if (rtc_tickno < 0) 
    { 
        /*获取TICK节拍时间中断号不成功错误处理*/
        dev_err(&pdev->dev, "no irq for rtc tick\n");
        return -ENOENT;
    }
    /*获取RTC平台设备所使用的IO端口资源，注意这个IORESOURCE_MEM标志和RTC平台设备定义中的一致*/
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (res == NULL) 
    {
        /*错误处理*/
        dev_err(&pdev->dev, "failed to get memory region resource\n");
        return -ENOENT;
    }
/*申请RTC的IO端口资源所占用的IO空间(要注意理解IO空间和内存空间的区别),
     request_mem_region定义在ioport.h中*/
    rtc_mem = request_mem_region(res->start, res->end - res->start + 1, pdev->name);
    if (rtc_mem == NULL) 
    {
        /*错误处理*/
        dev_err(&pdev->dev, "failed to reserve memory region\n");
        ret = -ENOENT;
        goto err_nores;
    }
/*将RTC的IO端口占用的这段IO空间映射到内存的虚拟地址，ioremap定义在io.h中。
     注意：IO空间要映射后才能使用，以后对虚拟地址的操作就是对IO空间的操作，*/
    rtc_base = ioremap(res->start, res->end - res->start + 1);
    if (rtc_base == NULL) 
    {
        /*错误处理*/
        dev_err(&pdev->dev, "failed ioremap()\n");
        ret = -EINVAL;
        goto err_nomap;
    }
    /*好了，通过上面的步骤已经将RTC的资源都准备好了，下面就开始使用啦*/
    /*这两个函数开始对RTC寄存器操作，定义都在下面*/
    rtc_enable(pdev, 1); /*对RTC的实时时钟控制寄存器RTCCON进行操作(功能是初始化或者使能RTC)*/
    rtc_setfreq(&pdev->dev, 1);/*对RTC的节拍时间计数寄存器TICNT的0-6位进行操作，即：节拍时间计数值的设定*/
/*device_init_wakeup该函数定义在pm_wakeup.h中，定义如下：
    static inline void device_init_wakeup(struct device *dev, int val){
        dev->power.can_wakeup = dev->power.should_wakeup = !!val;
    }
    显然这个函数是让驱动支持电源管理的,这里只要知道,can_wakeup为1时表明这个设备可以被唤醒,设备驱动为了支持
    Linux中的电源管理,有责任调用device_init_wakeup()来初始化can_wakeup，而should_wakeup则是在设备的电源状态
    发生变化的时候被device_may_wakeup()用来测试,测试它该不该变化，因此can_wakeup表明的是一种能力，
    而should_wakeup表明的是有了这种能力以后去不去做某件事。好了，我们没有必要深入研究电源管理的内容了，
    要不就扯远了，电源管理以后再讲*/
    device_init_wakeup(&pdev->dev, 1);
/*将RTC注册为RTC设备类，RTC设备类在RTC驱动核心部分中由系统定义好的，
     注意rtcops这个参数是一个结构体，该结构体的作用和里面的接口函数实现在第③步中。
     rtc_device_register函数在rtc.h中定义，在drivers/rtc/class.c中实现*/
    rtc = rtc_device_register("my2440", &pdev->dev, &rtcops, THIS_MODULE);
    if (IS_ERR(rtc)) 
    {
        /*错误处理*/
        dev_err(&pdev->dev, "cannot attach rtc\n");
        ret = PTR_ERR(rtc);
        goto err_nortc;
    }
/*设置RTC节拍时间计数寄存器TICNT的节拍时间计数值的用户最大相对值，
     这里你可能不理解这句，没关系，等你看到rtc_setfreq函数实现后自然就明白了*/
    rtc->max_user_freq = 128;
/*将RTC设备类的数据传递给系统平台设备。
     platform_set_drvdata是定义在platform_device.h的宏，如下：
     #define platform_set_drvdata(_dev,data)    dev_set_drvdata(&(_dev)->dev, (data))
     而dev_set_drvdata又被定义在include/linux/device.h中，如下：
      static inline void dev_set_drvdata (struct device *dev, void *data){
          dev->driver_data = data;
      }*/
    platform_set_drvdata(pdev, rtc);
    return 0;
//以下是上面错误处理的跳转点
 err_nortc:
    rtc_enable(pdev, 0);
    iounmap(rtc_base);
 err_nomap:
    release_resource(rtc_mem);
 err_nores:
    return ret;
}
/*该函数主要是初始化或者使能RTC，
  以下RTC的各种寄存器的宏定义在arch/arm/plat-s3c/include/plat/regs-rtc.h中，
  各寄存器的用途和设置请参考S3C2440数据手册的第十七章实时时钟部分*/
static void rtc_enable(struct platform_device *pdev, int flag)
{
    unsigned int tmp;
/*RTC的实时时钟控制寄存器RTCCON共有4个位，各位的初始值均为0，根据数据手册介绍第0位(即：RCTEN位)
     可以控制CPU和RTC之间的所有接口(即RTC使能功能)，所以在系统复位后应该将RTCCON寄存器的第0为置为1；
     在关闭电源前，又应该将该位清零，以避免无意的写RTC寄存器*/
    if (!flag) 
    {
        /*当flag=0时(即属于关闭电源前的情况)，RTCCON寄存器清零第一位*/
        tmp = readb(rtc_base + S3C2410_RTCCON); /*读取RTCCON寄存器的值*/
        /* tmp & ~S3C2410_RTCCON_RTCEN = 0 即屏蔽RTC使能*/
        writeb(tmp & ~S3C2410_RTCCON_RTCEN, rtc_base + S3C2410_RTCCON);
        tmp = readb(rtc_base + S3C2410_TICNT); /*读取TICNT寄存器的值*/
        /* tmp & ~S3C2410_TICNT_ENABLE后第7位为0，即屏蔽节拍时间中断使能*/
        writeb(tmp & ~S3C2410_TICNT_ENABLE, rtc_base + S3C2410_TICNT); 
    } 
    else 
    {
        /*当flag!=0时(即属于系统复位后的情况)，使能RTC*/
        if ((readb(rtc_base + S3C2410_RTCCON) & S3C2410_RTCCON_RTCEN) == 0)
        {
            dev_info(&pdev->dev, "rtc disabled, re-enabling\n");
            tmp = readb(rtc_base + S3C2410_RTCCON);
            writeb(tmp | S3C2410_RTCCON_RTCEN, rtc_base + S3C2410_RTCCON);
        }
        if ((readb(rtc_base + S3C2410_RTCCON) & S3C2410_RTCCON_CNTSEL))
        {
            dev_info(&pdev->dev, "removing RTCCON_CNTSEL\n");
            tmp = readb(rtc_base + S3C2410_RTCCON);
            writeb(tmp & ~S3C2410_RTCCON_CNTSEL, rtc_base + S3C2410_RTCCON);
        }
        if ((readb(rtc_base + S3C2410_RTCCON) & S3C2410_RTCCON_CLKRST))
        {
            dev_info(&pdev->dev, "removing RTCCON_CLKRST\n");
            tmp = readb(rtc_base + S3C2410_RTCCON);
            writeb(tmp & ~S3C2410_RTCCON_CLKRST, rtc_base + S3C2410_RTCCON);
        }
    }
}
/*该函数主要是对RTC的节拍时间计数寄存器TICNT的0-6位进行操作，即：节拍时间计数值的设定*/
static int rtc_setfreq(struct device *dev, int freq)
{
    unsigned int tmp;
    if (!is_power_of_2(freq)) /*对freq的值进行检查*/
        return -EINVAL;
    spin_lock_irq(&rtc_pie_lock); /*获取自旋锁保护临界区资源*/
    /*读取节拍时间计数寄存器TICNT的值*/
    tmp = readb(rtc_base + S3C2410_TICNT) & S3C2410_TICNT_ENABLE;
/*看数据手册得知，节拍时间计数值的范围是1-127，
     还记得在rtc_enable函数中设置的rtc->max_user_freq=128吗？所以这里要减1*/
    tmp |= (128 / freq) - 1; 
    /*将经运算后值写入节拍时间计数寄存器TICNT中，这里主要是改变TICNT的第0-6位的值*/
    writeb(tmp, rtc_base + S3C2410_TICNT);
    spin_unlock_irq(&rtc_pie_lock);/*释放自旋锁，即解锁*/
    return 0;
}
**③、**RTC设备类的操作。在这一步中，才是对RTC硬件的各种寄存器进行操作，代码如下： 
#include <linux/interrupt.h>
#include <linux/bcd.h>
/*rtc_class_ops是RTC设备类在RTC驱动核心部分中定义的对RTC设备类进行操作的结构体，
  类似字符设备在驱动中的file_operations对字符设备进行操作的意思。该结构体被定义
  在rtc.h中，对RTC的操作主要有打开、关闭、设置或获取时间、设置或获取报警、设置节拍时间计数值等等，
  该结构体内接口函数的实现都在下面*/
static const struct rtc_class_ops rtcops = {
    .open            = rtc_open,
    .release         = rtc_release,
    .irq_set_freq    = rtc_setfreq, /*在第②步中已实现*/
    .irq_set_state   = rtc_setpie,
    .read_time       = rtc_gettime,
    .set_time        = rtc_settime,
    .read_alarm      = rtc_getalarm,
    .set_alarm       = rtc_setalarm,
};
/*RTC设备类打开接口函数*/
static int rtc_open(struct device *dev)
{
    int ret;
/*这里主要的目的是从系统平台设备中获取RTC设备类的数据，和RTC探测函数rtc_probe中
     的platform_set_drvdata(pdev, rtc)的操作刚好相反。这些都定义在platform_device.h中*/
    struct platform_device *pdev = to_platform_device(dev);
    struct rtc_device *rtc_dev = platform_get_drvdata(pdev);
/*申请RTC报警中断服务，中断号rtc_alarmno在RTC探测函数rtc_probe中已经获取得，
     这里使用的是快速中断:IRQF_DISABLED。中断服务程序为:rtc_alarmirq，将RTC设备类rtc_dev做参数传递过去了*/
    ret = request_irq(rtc_alarmno, rtc_alarmirq, IRQF_DISABLED, "my2440-rtc alarm", rtc_dev);
    if (ret) 
    {
        dev_err(dev, "IRQ%d error %d\n", rtc_alarmno, ret);
        return ret;
    }
    /*同上面一样，这里申请的是RTC的TICK节拍时间中断服务，服务程序是:rtc_tickirq*/
    ret = request_irq(rtc_tickno, rtc_tickirq, IRQF_DISABLED, "my2440-rtc tick", rtc_dev);
    if (ret) 
    {
        dev_err(dev, "IRQ%d error %d\n", rtc_tickno, ret);
        goto tick_err;
    }
    return ret;
 tick_err:/*错误处理，注意出现错误后也要释放掉已经申请成功的中断*/
    free_irq(rtc_alarmno, rtc_dev);
    return ret;
}
/*RTC报警中断服务程序*/
static irqreturn_t rtc_alarmirq(int irq, void *argv)
{
    struct rtc_device *rdev = argv; /*接收申请中断时传递过来的rtc_dev参数*/
/*当报警中断到来的时候，去设定RTC中报警的相关信息，具体设定的方法，RTC核心
     部分已经在rtc_update_irq接口函数中实现，函数定义实现在interface.c中*/
    rtc_update_irq(rdev, 1, RTC_AF | RTC_IRQF);
    return IRQ_HANDLED;
}
/*RTC的TICK节拍时间中断服务*/
static irqreturn_t rtc_tickirq(int irq, void *argv)
{
    struct rtc_device *rdev = argv; /*接收申请中断时传递过来的rtc_dev参数*/
/*节拍时间中断到来的时候，去设定RTC中节拍时间的相关信息，具体设定的方法，RTC核心
     部分已经在rtc_update_irq接口函数中实现，函数定义实现在interface.c中*/
    rtc_update_irq(rdev, 1, RTC_PF | RTC_IRQF);
    return IRQ_HANDLED;
}
/*RTC设备类关闭接口函数*/
static void rtc_release(struct device *dev)
{
    /*和rtc_open中的作用相同*/
    struct platform_device *pdev = to_platform_device(dev);
    struct rtc_device *rtc_dev = platform_get_drvdata(pdev);
    /*请见rtc_setpie接口函数中的解释*/
    rtc_setpie(dev, 0);
    /*同rtc_open中中断的申请相对应，在那里申请中断，这里就释放中断*/
    free_irq(rtc_alarmno, rtc_dev);
    free_irq(rtc_tickno, rtc_dev);
}
/*该函数主要是对RTC的节拍时间计数寄存器TICNT的第7位进行操作，即：节拍时间计数的使能功能*/
static int rtc_setpie(struct device *dev, int flag)
{
    unsigned int tmp;
    spin_lock_irq(&rtc_pie_lock);/*获取自旋锁保护临界区资源*/
    /*读取节拍时间计数寄存器TICNT的值*/
    tmp = readb(rtc_base + S3C2410_TICNT) & ~S3C2410_TICNT_ENABLE;
    if (flag)
    {
        tmp |= S3C2410_TICNT_ENABLE; /*根据标志flag的值来判断是要使能还是要禁止*/
    }
    /*将经运算后值写入节拍时间计数寄存器TICNT中，这里主要是改变TICNT的第7位的值*/
    writeb(tmp, rtc_base + S3C2410_TICNT);
    spin_unlock_irq(&rtc_pie_lock);/*释放自旋锁，即解锁*/
    return 0;
}
/*读取RTC中BCD数中的：分、时、日期、月、年、秒*/
static int rtc_gettime(struct device *dev, struct rtc_time *rtc_tm)
{
    unsigned int have_retried = 0;
 retry_get_time:
    rtc_tm->tm_min  = readb(rtc_base + S3C2410_RTCMIN); /*读BCD分寄存器RTCMIN*/
    rtc_tm->tm_hour = readb(rtc_base + S3C2410_RTCHOUR); /*读BCD时寄存器RTCHOUR*/
    rtc_tm->tm_mday = readb(rtc_base + S3C2410_RTCDATE); /*读BCD日期寄存器RTCDATE*/
    rtc_tm->tm_mon  = readb(rtc_base + S3C2410_RTCMON); /*读BCD月寄存器RTCMON*/
    rtc_tm->tm_year = readb(rtc_base + S3C2410_RTCYEAR); /*读BCD年寄存器RTCYEAR*/
    rtc_tm->tm_sec  = readb(rtc_base + S3C2410_RTCSEC); /*读BCD秒寄存器RTCSEC*/
/*我们知道时间是以60为一个周期的，当时、分、秒达到60后，他们的上一级会加1，而自身又从0开始计数
     上面我们最后读的秒，如果读出来的秒刚好是0，那么前面读的分、时等就是上一分钟的，结果就少了一分钟，
     所以就要重新读取*/
    if (rtc_tm->tm_sec == 0 && !have_retried) 
    {
        have_retried = 1;
        goto retry_get_time;
    }
/*将上面读取的时间日期值保存到RTC核心定义的时间结构体中，该结构体定义在rtc.h中，
     这里的bcd2bin主要是编译器对返回值相同时进行优化处理，定义在bcd.h中*/
    rtc_tm->tm_sec  = bcd2bin(rtc_tm->tm_sec);
    rtc_tm->tm_min  = bcd2bin(rtc_tm->tm_min);
    rtc_tm->tm_hour = bcd2bin(rtc_tm->tm_hour);
    rtc_tm->tm_mday = bcd2bin(rtc_tm->tm_mday);
    rtc_tm->tm_mon  = bcd2bin(rtc_tm->tm_mon);
    rtc_tm->tm_year = bcd2bin(rtc_tm->tm_year);
/*这里为什么要加100年和减1月呢，我们查看数据手册得知原来是为了区别1900年和2000年闰年的因素，
     1900年不是闰年而2000年是闰年。这时你或许会问那怎么不考虑1800年或2100年啊？原因很简单，因为
     我们的RTC时钟只支持100年的时间范围，呵呵！！*/
    rtc_tm->tm_year += 100;
    rtc_tm->tm_mon -= 1;
    return 0;
}
/*和上面的rtc_gettime功能相反，将更改后的分、时、日期、月、年、秒写入RTC中BCD数中*/
static int rtc_settime(struct device *dev, struct rtc_time *tm)
{
    /*这里减100年很清楚了吧，因为上面为了区别1900年和2000年时加了100年*/
    int year = tm->tm_year - 100;
    /*RTC时钟只支持100年的时间范围*/
    if (year < 0 || year >= 100) {
        dev_err(dev, "rtc only supports 100 years\n");
        return -EINVAL;
    }
    /*将上面保存到RTC核心定义的时间结构体中的时间日期值写入对应的寄存器中*/
    writeb(bin2bcd(tm->tm_sec), rtc_base + S3C2410_RTCSEC);
    writeb(bin2bcd(tm->tm_min), rtc_base + S3C2410_RTCMIN);
    writeb(bin2bcd(tm->tm_hour), rtc_base + S3C2410_RTCHOUR);
    writeb(bin2bcd(tm->tm_mday), rtc_base + S3C2410_RTCDATE);
    writeb(bin2bcd(tm->tm_mon + 1), rtc_base + S3C2410_RTCMON); /*这里加1月也明白了吧*/
    writeb(bin2bcd(year), rtc_base + S3C2410_RTCYEAR);
    return 0;
}
/*读取RTC中报警各寄存器的：秒、分、时、月、日期、年的值，保存各值到rtc_time结构体中*/
static int rtc_getalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
    unsigned int alm_en;
    struct rtc_time *alm_tm = &alrm->time;
    alm_tm->tm_sec  = readb(rtc_base + S3C2410_ALMSEC);
    alm_tm->tm_min  = readb(rtc_base + S3C2410_ALMMIN);
    alm_tm->tm_hour = readb(rtc_base + S3C2410_ALMHOUR);
    alm_tm->tm_mon  = readb(rtc_base + S3C2410_ALMMON);
    alm_tm->tm_mday = readb(rtc_base + S3C2410_ALMDATE);
    alm_tm->tm_year = readb(rtc_base + S3C2410_ALMYEAR);
    /*获取RTC报警控制寄存器RTCALM的值*/
    alm_en = readb(rtc_base + S3C2410_RTCALM);
    /*判断RTCALM值的第6位，来设置RTC的全局报警使能状态到RTC核心定义的报警状态结构体rtc_wkalrm中*/
    alrm->enabled = (alm_en & S3C2410_RTCALM_ALMEN) ? 1 : 0;
    /*判断如果RTCALM值的第0位的值(秒报警使能)为1时，就设置报警秒的值到rtc_time结构体中*/
    if (alm_en & S3C2410_RTCALM_SECEN)
        alm_tm->tm_sec = bcd2bin(alm_tm->tm_sec);
    else
        alm_tm->tm_sec = 0xff;
    /*判断如果RTCALM值的第1位的值(分报警使能)为1时，就设置报警分的值到rtc_time结构体中*/
    if (alm_en & S3C2410_RTCALM_MINEN)
        alm_tm->tm_min = bcd2bin(alm_tm->tm_min);
    else
        alm_tm->tm_min = 0xff;
    /*判断如果RTCALM值的第2位的值(时报警使能)为1时，就设置报警小时的值到rtc_time结构体中*/
    if (alm_en & S3C2410_RTCALM_HOUREN)
        alm_tm->tm_hour = bcd2bin(alm_tm->tm_hour);
    else
        alm_tm->tm_hour = 0xff;
    /*判断如果RTCALM值的第3位的值(日期报警使能)为1时，就设置报警日期的值到rtc_time结构体中*/
    if (alm_en & S3C2410_RTCALM_DAYEN)
        alm_tm->tm_mday = bcd2bin(alm_tm->tm_mday);
    else
        alm_tm->tm_mday = 0xff;
    /*判断如果RTCALM值的第4位的值(月报警使能)为1时，就设置报警月的值到rtc_time结构体中*/
    if (alm_en & S3C2410_RTCALM_MONEN) 
    {
        alm_tm->tm_mon = bcd2bin(alm_tm->tm_mon);
        alm_tm->tm_mon -= 1; /*这里为什么要递减1，我不是很明白？？？？？？？*/
    } 
    else 
    {
        alm_tm->tm_mon = 0xff;
    }
    /*判断如果RTCALM值的第5位的值(年报警使能)为1时，就设置报警年的值到rtc_time结构体中*/
    if (alm_en & S3C2410_RTCALM_YEAREN)
        alm_tm->tm_year = bcd2bin(alm_tm->tm_year);
    else
        alm_tm->tm_year = 0xffff;
    return 0;
}
/*把上面保存到rtc_time结构体中各值写入RTC中报警各寄存器中*/
static int rtc_setalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
    unsigned int alrm_en;
    struct rtc_time *tm = &alrm->time;
    /*读取RTC报警控制寄存器RTCALM的第6位，把全局报警使能的状态保存到alrm_en变量中*/
    alrm_en = readb(rtc_base + S3C2410_RTCALM) & S3C2410_RTCALM_ALMEN;
    /*把RTC报警控制寄存器RTCALM的值设为0，即将全局报警使能和其他报警使能全部关闭*/
    writeb(0x00, rtc_base + S3C2410_RTCALM);
    if (tm->tm_sec < 60 && tm->tm_sec >= 0) 
    {
/*上面的alrm_en值只记录了RTCALM的第6位(全局报警使能的状态)，这里再加上第0位(秒报警使能的状态)，
         然后将前面保存在rtc_time中报警秒的值写入报警秒数据寄存器ALMSEC中*/
        alrm_en |= S3C2410_RTCALM_SECEN;
        writeb(bin2bcd(tm->tm_sec), rtc_base + S3C2410_ALMSEC);
    }
    if (tm->tm_min < 60 && tm->tm_min >= 0) 
    {
/*加上第1位(分报警使能的状态)，
         然后将前面保存在rtc_time中报警分的值写入报警分钟数据寄存器ALMMIN中*/
        alrm_en |= S3C2410_RTCALM_MINEN;
        writeb(bin2bcd(tm->tm_min), rtc_base + S3C2410_ALMMIN);
    }
    if (tm->tm_hour < 24 && tm->tm_hour >= 0) 
    {
/*加上第2位(时报警使能的状态)，
         然后将前面保存在rtc_time中报警小时的值写入报警小时数据寄存器ALMHOUR中*/
        alrm_en |= S3C2410_RTCALM_HOUREN;
        writeb(bin2bcd(tm->tm_hour), rtc_base + S3C2410_ALMHOUR);
    }
    /*把alrm_en修改过后的值重新写入RTC报警控制寄存器RTCALM中*/
    writeb(alrm_en, rtc_base + S3C2410_RTCALM);
    /*请看下面rtc_setaie函数实现部分*/
    rtc_setaie(alrm->enabled);
    /*根据全局报警使能的状态来决定是唤醒RTC报警中断还是睡眠RTC报警中断*/
    if (alrm->enabled)
        enable_irq_wake(rtc_alarmno);
    else
        disable_irq_wake(rtc_alarmno);
    return 0;
}
/*这里主要还是控制RTC报警控制寄存器RTCALM的第6位(全局报警使能状态)*/
static void rtc_setaie(int flag)
{
    unsigned int tmp;
    tmp = readb(rtc_base + S3C2410_RTCALM) & ~S3C2410_RTCALM_ALMEN;
    if (flag)/*根据标志flag来使能或禁止全局报警*/
        tmp |= S3C2410_RTCALM_ALMEN;
    writeb(tmp, rtc_base + S3C2410_RTCALM);
}
**④、**RTC平台驱动的设备移除、挂起和恢复接口函数的实现，代码如下： 
*注意：这是使用了一个__devexit，还记得在第①步中的__devexit_p和第②步中的__devinit吗？
  我们还是先来讲讲这个：
  在Linux内核中，使用了大量不同的宏来标记具有不同作用的函数和数据结构，
  这些宏在include/linux/init.h 头文件中定义，编译器通过这些宏可以把代码优化放到合适的内存位置，
  以减少内存占用和提高内核效率。__devinit、__devexit就是这些宏之一，在probe()和remove()函数中
  应该使用__devinit和__devexit宏。又当remove()函数使用了__devexit宏时，则在驱动结构体中一定要
  使用__devexit_p宏来引用remove()，所以在第①步中就用__devexit_p来引用rtc_remove*/
static int __devexit rtc_remove(struct platform_device *dev)
{
    /*从系统平台设备中获取RTC设备类的数据*/
    struct rtc_device *rtc = platform_get_drvdata(dev);
    platform_set_drvdata(dev, NULL); /*清空平台设备中RTC驱动数据*/
    rtc_device_unregister(rtc); /*注销RTC设备类*/
    rtc_setpie(&dev->dev, 0); /*禁止RTC节拍时间计数寄存器TICNT的使能功能*/
    rtc_setaie(0); /*禁止RTC报警控制寄存器RTCALM的全局报警使能功能*/
    iounmap(rtc_base); /*释放RTC虚拟地址映射空间*/
    release_resource(rtc_mem); /*释放获取的RTC平台设备的资源*/
    kfree(rtc_mem); /*销毁保存RTC平台设备的资源内存空间*/
    return 0;
}
/*对RTC平台设备驱动电源管理的支持。CONFIG_PM这个宏定义在内核中，
  当配置内核时选上电源管理，则RTC平台驱动的设备挂起和恢复功能均有效，
  这时候你应该明白了在第②步中为什么要有device_init_wakeup(&pdev->dev, 1)这句吧！！*/
#ifdef CONFIG_PM
static int ticnt_save; /*定义一个变量来保存挂起时的TICNT值*/
/*RTC平台驱动的设备挂起接口函数的实现*/
static int rtc_suspend(struct platform_device *pdev, pm_message_t state)
{
    ticnt_save = readb(rtc_base + S3C2410_TICNT); /*以节拍时间计数寄存器TICNT的值为挂起点*/
    rtc_enable(pdev, 0); /*挂起了之后就禁止RTC控制使能*/
    return 0;
}
/*RTC平台驱动的设备恢复接口函数的实现*/
static int rtc_resume(struct platform_device *pdev)
{
    rtc_enable(pdev, 1); /*恢复之前先使能RTC控制*/
    writeb(ticnt_save, rtc_base + S3C2410_TICNT); /*恢复挂起时的TICNT值，RTC节拍时间继续计数*/
    return 0;
}
#else /*配置内核时没选上电源管理,RTC平台驱动的设备挂起和恢复功能均无效,这两个函数也就无需实现了*/
#define rtc_suspend NULL
#define rtc_resume NULL
#endif
好了，到此RTC驱动程序编写完成了。在这里不知大家有没有留意，在前面的概念部分中我们讲到过，如果把一个字符设备注册成为一个平台设备，除了要实现平台设备驱动中platform_driver的接口函数外，还要实现字符设备驱动中file_operations的接口函数，但是从上面的驱动代码中看，这里并没有对RTC进行file_operations的操作，这是怎么回事啊？原来对RTC进行file_operations的操作在RTC的核心部分已经由系统提供了。在第②步的探测函数rtc_probe中，首先用rtc_device_register注册为RTC设备类，我们看rtc_device_register的实现(在class.c中)，又调用了rtc_dev_prepare(rtc)，其实现在rtc-dev.c中，那么在这里面才对RTC进行了file_operations操作，对RTC驱动的设备号也在rtc-dev.c中处理的。
**四、回过头再来分析理解具体RTC驱动程序代码的结构**
在上面的各步骤中，我已对RTC驱动程序的每行代码的作用都做了详细的讲述，但到结尾部分后，也许你会有点找不着北的感觉。的确，整个代码太长，而且用文字的方式也确实很难把整个驱动的结构描述清晰。下面，我就用图形的方式来概括上面各步骤之间的关系，使整个驱动程序的结构更加清晰明了。
**五、结束语**
通过对RTC驱动的实现，我们对平台设备有了进一步的了解，这对我们以后编写I2C、IIS、看门狗等设备的驱动有了很大的帮助。另外，可以看出在对具体硬件操作的时候实际是对该硬件的各种寄存器进行访问读写，所以这就要求我们在编写一个设备驱动之前必须先了解该设备的数据手册，列出所有的寄存器及功能，这样才能在编写驱动时正确的对设备进行访问。
