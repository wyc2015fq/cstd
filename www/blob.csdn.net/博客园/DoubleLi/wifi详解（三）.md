# wifi详解（三） - DoubleLi - 博客园






# 1        WLAN驱动结构介绍

## 1.1      SDIO驱动

在drivers/mmc下面是mmc卡，SD卡和SDIO卡驱动部分，其中包括host驱动，card驱动和core部分，由于网络接口卡挂接在SDIO总线上，所以在此之前我们先看一下SDIO的驱动结构。其驱动在drivers/mmc目录下的结构为：



|-- mmc

|   |-- card

|   |-- core

|   |-- host



主要关注的目录是core目录，这个目录是真个驱动的核心目录，是媒体卡的通用代码部分，包括core.c，host.c和sdio.c等。CORE 层完成了不同协议和规范的实现，并为HOST 层的驱动提供了接口函数，该目录完成sdio总线的注册操作，相应的ops操作，以及支持mmc的代码。详细的情况将在函数接口部分详细讨论。

Host目录是不同平台根据平台的特性而编写的host驱动。

## 1.2      Boardcom无线通讯芯片

### 1.2.1       概述

全球有线和无线通信半导体市场的领导者Broadcom（博通）公司（Nasdaq：BRCM）宣布，推出最新无线组合芯片BCM4330，该芯片可支持更多媒体形式和数据应用，且不会增大智能手机、平板电脑及其他移动设备的尺寸或缩短其电池寿命。BCM4330在单个芯片上集成了业界领先的Broadcom 802.11n Wi-Fi、蓝牙和FM无线技术，与分立式半导体器件组成的解决方案相比，在成本、尺寸、功耗和性能上有显著优势，是移动设备的理想选择。

BCM4330采用了新的Wi-Fi和蓝牙标准，可支持新的、令人振奋的应用。例如，Broadcom BCM4330是业界第一款经过蓝牙4.0标准认证的组合芯片解决方案， 集成了蓝牙低功耗（BLE）标准。该标准使蓝牙技术能以超低功耗运行，因此BCM4330非常适用于需要很长电池寿命的系统，如无线传感器、医疗和健身监控设备等。BCM4330还支持Wi-Fi Direct™和蓝牙高速（HS）标准，因此采用BCM4330的移动设备能直接相互通信，而不必先连接到接入点、成为传统网络的一部分，从而为很多无线设备之间新的应用和使用模式创造了机会。

Broadcom一直支持所有主流的操作系统（OS）平台，如MicrosoftWindows和Windows Phone、Google Chrome、Android等等，而且不仅是BCM4330，所有蓝牙、WLAN和GPS芯片组都提供这样的支持。

### 1.2.2       源码

Bcm4330驱动源码一般被厂商单独提供，如果要在开发的LINUX系统中（当然它还支持多种平台）使用该源码，可以添加到linux kernel源码树里，也可以单独组织存放，可以直接编译到kernel，也可以编译成模块，然后再系统启动的流程中或其他适当的实际加载到kernel中，一般建议单独组织并编译成模块在需要的时候加载如kernel。

|-- src

|   |-- bcmsdio

|   |-- dhd

|   |--dongle

|   |--include

|   |-- shared

|   |-- wl



这里主要内容到bcmsdio，dhd和wl三个目录下，bcm4330驱动的入口在dhd/sys/dhd_linux.c文件中的dhd_module()函数，设备的初始化和相关驱动注册都从这里开始，

## 1.3      详细接口及代码分析

### 1.3.1      WIFI驱动流程分析

    以boardcom bcm4329芯片驱动为例，相应的函数流程图如下：       





### 1.3.2       WIFI设备注册流程



Platform_driver_register(wifi_device[_legacy])的调用将wifi_device[_legacy]驱动注册到系统中，wifi_device_legacy是为了兼容老版本的驱动。

Path: wl/sys/wl_android.c

Static struct Platform_driver **wifi_device **={

         .probe     =     wifi_probe

         .remove   =     wifi_remove

         .suspend  =     wifi_supend

         .resume   =     wifi_resume

         .driver     =     {

         .name      =     **“bcmdhd_wlan”**

}

}



Static struct Platform_driver **wifi_device_legacy **={

         .probe     =     wifi_probe

         .remove   =     wifi_remove

         .suspend  =     wifi_supend

         .resume   =     wifi_resume

         .driver     =     {

         .name      =     **“bcm4329_wlan”**

}

}

上面的展示了wifi平台设备驱动的注册过程，那么在平台相关的代码区应该有wifi作为平台设备被初始化和注册的地方：

Path: kernel/arch/arm/mach-msm/msm_

**static struct resource mahimahi_wifi_resources[] = {**

        [0] = {

                .name           = "bcm4329_wlan_irq",

                .start          =MSM_GPIO_TO_INT(MAHIMAHI_GPIO_WIFI_IRQ),

                .end            = MSM_GPIO_TO_INT(MAHIMAHI_GPIO_WIFI_IRQ),

                .flags          = IORESOURCE_IRQ |IORESOURCE_IRQ_HIGHLEVEL | IORESOURCE_IRQ_SHAREABLE,

        },

};



static structwifi_platform_data mahimahi_wifi_control = {

** .set_power      = mahimahi_wifi_power,**

        .set_reset      = mahimahi_wifi_reset,

**.set_carddetect = mahimahi_wifi_set_carddetect,**

        .mem_prealloc   = mahimahi_wifi_mem_prealloc,

};



**static struct platform_device mahimahi_wifi_device = {**

**        .name           = "bcm4329_wlan",**

**        .id             = 1,**

**        .num_resources  = ARRAY_SIZE(mahimahi_wifi_resources),**

**        .resource       = mahimahi_wifi_resources,**

**        .dev            = {**

**               .platform_data = &mahimahi_wifi_control,**

**        },**

**};**

上面是对wifi_device设备的初始化，下面是对该设备的注册：

static int __initmahimahi_wifi_init(void)

{

        int ret;



        if (!machine_is_mahimahi())

                return 0;



        printk("%s: start\n",__func__);

       mahimahi_wifi_update_nvs("sd_oobonly=1\r\n", 0);

       mahimahi_wifi_update_nvs("btc_params70=0x32\r\n", 1);

        mahimahi_init_wifi_mem();

**ret = platform_device_register(&mahimahi_wifi_device);**

        return ret;

}



**late_initcall(mahimahi_wifi_init);      //表明在系统启动的后期会自动调用加载该模块**

这样，通过上面的初始化和注册流程，wifi设备作为平台设备和驱动就可以握手成功了，这里的平台驱动只是对wifi设备的简单管理，如对wifi设备的挂起和恢复等操作了。但是在wifi设备初始化之前是不能够被挂起和恢复的，那么wifi设备是如何初始化的呢？

Path: wl/sys/wl_android.c

static int wifi_probe(structplatform_device *pdev)

{

**struct wifi_platform_data *wifi_ctrl =**

**                (structwifi_platform_data *)(pdev->dev.platform_data);**



        DHD_ERROR(("## %s\n",__FUNCTION__));

**wifi_irqres = platform_get_resource_byname(pdev,IORESOURCE_IRQ, "bcmdhd_wlan_irq");**

        if (wifi_irqres == NULL)

                wifi_irqres =platform_get_resource_byname(pdev,

                        IORESOURCE_IRQ,"bcm4329_wlan_irq");

**wifi_control_data = wifi_ctrl;**



**wifi_set_power(1,0);   /* Power On */**

**       wifi_set_carddetect(1); /* CardDetect (0->1) */**



        up(&wifi_control_sem);

        return 0;

}

这是wifi平台设备驱动注册时成功匹配wifi设备后调用的函数wifi_probe()，它的主要工作就是从wifi设备中获取终端资源，并获取wifi_platform_data类型结构赋予wifi_control_data变量，这一步很重要，下面就可以看出了它的重要性。然后调用wifi_set_power和wifi_set_carddetect函数给wifi芯片上电并检测。

int wifi_set_power(int on, unsignedlong msec)

{

        DHD_ERROR(("%s = %d\n",__FUNCTION__, on));

        if (wifi_control_data &&wifi_control_data->set_power) {

**wifi_control_data->set_power(on);**

        }

        if (msec)

                msleep(msec);

        return 0;

}

Wifi_set_power函数中调用wifi_control_data->set_power(on)，wifi_control_data就是刚才说的那个重要变量，注意它是从wifi_device平台设备的wifi_platform_data获取的，那么看看上面的wifi_device初始化的代码：

**static struct platform_device mahimahi_wifi_device = {**

**        .name           = "bcm4329_wlan",**

**        .id             = 1,**

**        .num_resources  = ARRAY_SIZE(mahimahi_wifi_resources),**

**        .resource       = mahimahi_wifi_resources,**

**        .dev            = {**

**                .platform_data =&mahimahi_wifi_control,**

**        },**

**};**



static struct wifi_platform_data**mahimahi_wifi_control**= {

        .set_power      = **mahimahi_wifi_power,**

        .set_reset      = mahimahi_wifi_reset,

        .set_carddetect = **mahimahi_wifi_set_carddetect****,**

        .mem_prealloc   = mahimahi_wifi_mem_prealloc,

};

所以它实际调用的是mahimahi_wifi_power函数，该函数的定义在kernel/arch/arm /mach-msm/board-mahimahi-mmc.c之中：

int mahimahi_wifi_power(int on)

{

       printk("%s: %d\n", __func__, on);



        if (on) {

               config_gpio_table(wifi_on_gpio_table,

                                  ARRAY_SIZE(wifi_on_gpio_table));

               mdelay(50);

        } else {

               config_gpio_table(wifi_off_gpio_table,

                                 ARRAY_SIZE(wifi_off_gpio_table));

        }



        mdelay(100);

        gpio_set_value(MAHIMAHI_GPIO_WIFI_SHUTDOWN_N,on); /* WIFI_SHUTDOWN */

        mdelay(200);



       mahimahi_wifi_power_state = on;

        return 0;

}

调用gpio_set_value操作wifi芯片，给wifi芯片上电。那么来看看wifi_set_ carddetect函数究竟干了什么：

Path：wl/sys/wl_android.c

static int wifi_set_carddetect(int on)

{

       DHD_ERROR(("%s = %d\n", __FUNCTION__, on));

        if(wifi_control_data && wifi_control_data->set_carddetect) {

               wifi_control_data->set_carddetect(on);

        }

        return 0;

}

同样会调用wifi_device的mahimahi_wifi_set_carddetect函数：

Path:kernel/arch/arm/mach-msm/board-mahimahi-mmc.c

int mahimahi_wifi_set_carddetect(int val)

{

       pr_info("%s: %d\n", __func__, val);

       mahimahi_wifi_cd = val;

        if(wifi_status_cb) {

**wifi_status_cb(val,wifi_status_cb_devid);**

        } else

               pr_warning("%s: Nobody to notify\n", __func__);

        return 0;

}

Wifi_status_cb代码：

**static int mahimahi_wifi_status_register(**

                        void (*callback)(intcard_present, void *dev_id),

                        void *dev_id)

{

        if (wifi_status_cb)

                return -EAGAIN;

**wifi_status_cb = callback;**

        wifi_status_cb_devid = dev_id;

        return 0;

}



static unsigned intmahimahi_wifi_status(struct device *dev)

{

        return mahimahi_wifi_cd;

}



static structmmc_platform_data mahimahi_wifi_data = {

        .ocr_mask               = MMC_VDD_28_29,

        .built_in               = 1,

        .status                 = mahimahi_wifi_status,

        .**register_status_notify= mahimahi_wifi_status_register,**

        .embedded_sdio          = &mahimahi_wifi_emb_data,

};

由上面代码；不难看出会有个地方调用mahimahi_wifi_status_register设置wifi_status_cb这个回调函数，可以跟踪这个mahimahi_wifi_data结构体，来看看它被传递给了谁：

int msm_add_sdcc(unsigned intcontroller, struct mmc_platform_data *plat,

                 unsigned int stat_irq,unsigned long stat_irq_flags);



int __initmahimahi_init_mmc(unsigned int sys_rev, unsigned debug_uart)

{

        ……



**msm_add_sdcc(1, &mahimahi_wifi_data, 0, 0);**



       ……

        if (system_rev > 0)

                msm_add_sdcc(2,&mahimahi_sdslot_data, 0, 0);

        else {

                mahimahi_sdslot_data.status =mahimahi_sdslot_status_rev0;

               mahimahi_sdslot_data.register_status_notify = NULL;

               set_irq_wake(MSM_GPIO_TO_INT(MAHIMAHI_GPIO_SDMC_CD_REV0_N), 1);

**msm_add_sdcc(2, &mahimahi_sdslot_data,**

         ……

}

可以跟踪到这里Path：kernel/arch/arm/mach-msm/devices-msm7x30.c

**struct platform_device msm_device_sdc1 = {**

**        .name           = "msm_sdcc",**

**        .id             = 1,**

**        .num_resources  = ARRAY_SIZE(resources_sdc1),**

**        .resource       = resources_sdc1,**

**        .dev            = {**

**               .coherent_dma_mask      =0xffffffff,**

**        },**

**};**



**struct platform_device msm_device_sdc2 = {**

**        .name           = "msm_sdcc",**

**        .id             = 2,**

**        .num_resources  = ARRAY_SIZE(resources_sdc2),**

**        .resource       = resources_sdc2,**

**        .dev            = {**

**               .coherent_dma_mask      =0xffffffff,**

**        },**

**};**



struct platform_devicemsm_device_sdc3 = {

        .name           = "msm_sdcc",

        .id             = 3,

        .num_resources  = ARRAY_SIZE(resources_sdc3),

        .resource       = resources_sdc3,

        .dev            = {

                .coherent_dma_mask      = 0xffffffff,

        },

};



struct platform_device msm_device_sdc4= {

        .name           = "msm_sdcc",

        .id             = 4,

        .num_resources  = ARRAY_SIZE(resources_sdc4),

        .resource       = resources_sdc4,

        .dev            = {

                                                                                                                         439,2-16      62%

                .coherent_dma_mask      = 0xffffffff,

        },

};



**static struct platform_device *msm_sdcc_devices[] __initdata = {**

        &msm_device_sdc1,

        &msm_device_sdc2,

        &msm_device_sdc3,

        &msm_device_sdc4,

};



int __initmsm_add_sdcc(unsigned int controller, struct mmc_platform_data *plat,

                        unsigned int stat_irq,unsigned long stat_irq_flags)

{

        ……



**pdev =msm_sdcc_devices[controller-1]; //因为传过来的controller是1，所以下面注册的是第一个平台设备**

**        pdev->dev.platform_data= plat;  //被传递给平台设备的platform_data**



        res =platform_get_resource_byname(pdev, IORESOURCE_IRQ, "status_irq");

        if (!res)

                return -EINVAL;

        else if (stat_irq) {

                res->start = res->end =stat_irq;

                res->flags &=~IORESOURCE_DISABLED;

                res->flags |=stat_irq_flags;

        }



**return platform_device_register(pdev); //如上所述**

}

那么这个平台设备是什么呢，就是sd卡控制器，也就是前面说的host驱动所驱动的主机控制设备。

Path: drivers/mmc/host/msm_sdcc.c

static struct platform_drivermsmsdcc_driver = {

        .probe          = **msmsdcc_probe,**

        .suspend        = msmsdcc_suspend,

        .resume         = msmsdcc_resume,

        .driver         = {

                .name   = **"msm_sdcc",**

        },

};



static int __initmsmsdcc_init(void)

{

**return platform_driver_register(&msmsdcc_driver);**

}

驱动成功匹配设备后，调用probe函数：

static int

msmsdcc_probe(structplatform_device *pdev)

{

......

if (stat_irqres &&!(stat_irqres->flags & IORESOURCE_DISABLED)) {

……

        } else if(plat->register_status_notify) {

**plat->register_status_notify(msmsdcc_status_notify_cb,host);**

        } else if (!plat->status)

......

}

msmsdcc_status_notify_cb调用msmsdcc_check_status函数：

msmsdcc_status_notify_cb(intcard_present, void *dev_id)

{

        struct msmsdcc_host *host = dev_id;



        printk(KERN_DEBUG "%s:card_present %d\n", mmc_hostname(host->mmc),

               card_present);

**msmsdcc_check_status((unsigned long) host);**

}

msmsdcc_check_status调用mmc_detect_change函数：

static void

msmsdcc_check_status(unsignedlong data)

{

        ……

        if (status ^ host->oldstat) {

                pr_info("%s: Slot statuschange detected (%d -> %d)\n",

                        mmc_hostname(host->mmc),host->oldstat, status);

                if (status &&!host->plat->built_in)

**mmc_detect_change(host->mmc, (5 * HZ) / 2);**

                else

**mmc_detect_change(host->mmc, 0);**

        }



        host->oldstat = status;



out:

        if (host->timer.function)

                mod_timer(&host->timer,jiffies + HZ);

}

可以看到mmc_detect_change被调用了，这个函数触发了一个延时工作：

void mmc_detect_change(structmmc_host *host, unsigned long delay)

{

……



**mmc_schedule_delayed_work(&host->detect, delay);**

}

这个时候它会在delay时间后，执行host->detect延时工作对应的函数，在host驱动注册并匹配设备成功后执行的probe函数里，会调用mmc_alloc_host动态创建一个mmc_host：

msmsdcc_probe(structplatform_device *pdev)

{

......

/*

         * Setup our host structure

         */



**mmc = mmc_alloc_host(sizeof(struct msmsdcc_host),&pdev->dev);**

        if (!mmc) {

                ret = -ENOMEM;

                goto out;

        }

......

}

mmc_alloc_host初始化工作入口：

struct mmc_host*mmc_alloc_host(int extra, struct device *dev)

{

......

**INIT_DELAYED_WORK(&host->detect, mmc_rescan);**

......

}

mmc_rescan是core.c中一个很重要的函数，它遵照 SDIO 卡协议的 SDIO 卡启动过程，包括了非激活模式、卡识别模式和数据传输模式三种模式共九种状态的转换，你需要参照相关规范来理解。

void mmc_rescan(structwork_struct *work)

{

        struct mmc_host *host =

                container_of(work, structmmc_host, detect.work);

......

**mmc_power_up(host);**

**        sdio_reset(host);**

**        mmc_go_idle(host);**



**       mmc_send_if_cond(host, host->ocr_avail);**



        /*

         * First we search for SDIO...

         */

**err = mmc_send_io_op_cond(host, 0, &ocr);**

        if (!err) {

**if (mmc_attach_sdio(host, ocr))**

                        mmc_power_off(host);

                extend_wakelock = 1;

                goto out;

        }

......

}

这个mmc_attach_sdio函数很重要，它是SDIO卡的初始化的起点，主要工作包括：匹配SDIO卡的工作电压，分配并初始化mmc_card结构，然后注册mmc_card到系统中：

/*

 * Starting point for SDIO card init.

 */

int mmc_attach_sdio(structmmc_host *host, u32 ocr)

{

        ……



** mmc_attach_bus(host,&mmc_sdio_ops);  //初始化host的bus_ops**



       ……



**host->ocr = mmc_select_voltage(host, ocr); //匹配SDIO卡工作电压**



        ……



        /*

         * Detect and init the card.

         */

**err = mmc_sdio_init_card(host, host->ocr, NULL, 0);//检测，分配初始化mmc_card**

        if (err)

                goto err;

        card = host->card;

/*

         * If needed, disconnect card detectionpull-up resistor.

         */

        err = sdio_disable_cd(card);

        if (err)

                goto remove;



        /*

         *** Initialize (but don't add) all present functions.**

         */

**for (i = 0; i < funcs; i++, card->sdio_funcs++) {**

**#ifdef CONFIG_MMC_EMBEDDED_SDIO**

**                if(host->embedded_sdio_data.funcs) {**

**                       struct sdio_func *tmp;**



**                        tmp = sdio_alloc_func(host->card);**

**                        if(IS_ERR(tmp))**

**                               goto remove;**

**                       tmp->num = (i + 1);**

**                       card->sdio_func[i] = tmp;**

**                       tmp->class = host->embedded_sdio_data.funcs[i].f_class;**

**                       tmp->max_blksize = host->embedded_sdio_data.funcs[i].f_maxblksize;**

**                       tmp->vendor = card->cis.vendor;**

**                       tmp->device = card->cis.device;**

**                } else {**

#endif

                        err =sdio_init_func(host->card, i + 1);

                        if (err)

                                goto remove;

#ifdefCONFIG_MMC_EMBEDDED_SDIO

                }

#endif

        }



        mmc_release_host(host);



        /*

         * First add the card to the drivermodel...

         */

**err = mmc_add_card(host->card);     //添加mmc_card**

        if (err)

                goto remove_added;



        /*

         * ...then the SDIO functions.

         */

        for (i = 0;i < funcs;i++) {

**err =sdio_add_func(host->card->sdio_func[i]);              //将sdio_func加入系统**

                if (err)

                       goto remove_added;

        }



        return 0;

......

}

这样，SDIO卡已经初始化成功并添加到了驱动中。上面说的过程是在SDIO设备注册时的调用流程，mmc_rescan是整个流程主体部分，由它来完成SDIO设备的初始化和添加。其实上面的流程只是创建，初始化，添加SDIO设备的一条线，还有另外的两条线也会调用mmc_rescan函数进行SDIO设备的上述操作：

（1）    加载SDIO host驱动模块

（2）    SDIO设备中断

#### 1.3.2.1        加载SDIO host驱动模块

Host作为平台设备被注册，前面也有列出相应源码：

static struct platform_drivermsmsdcc_driver = {

        .probe          = **msmsdcc_probe**,

        .suspend        = msmsdcc_suspend,

        .resume         = msmsdcc_resume,

        .driver         = {

                .name   = "msm_sdcc",

        },

};



static int __initmsmsdcc_init(void)

{

** returnplatform_driver_register(&msmsdcc_driver);**

}



Probe函数会调用mmc_alloc_host函数（代码前面已经贴出）来创建mmc_host结构变量，进行必要的初始化之后，调用mmc_add_host函数将它添加到驱动里面：

int mmc_add_host(structmmc_host *host)

{

        ……



        err =device_add(&host->class_dev);

        if (err)

                return err;

**mmc_start_host(host);**

        if (!(host->pm_flags &MMC_PM_IGNORE_PM_NOTIFY))

                register_pm_notifier(&host->pm_notify);



        return 0;

}

       Mmc_start_host定义如下：

void mmc_start_host(structmmc_host *host)

{

**mmc_power_off(host);**

**       mmc_detect_change(host, 0);**

}

mmc_power_off中对 ios进行了设置，然后调用 **mmc_set_ios(host);**

**host->ios.power_mode = MMC_POWER_OFF;**

**       host->ios.bus_width = MMC_BUS_WIDTH_1;**

**       host->ios.timing =MMC_TIMING_LEGACY;**

**       mmc_set_ios(host);**

mmc_set_ios(host) 中的关键语句 host->ops->set_ios(host, ios)，实际上在host驱动的probe函数中就已经对host->ops进行了初始化：

……

/*

         * Setup MMC host structure

         */

**mmc->ops = &msmsdcc_ops;**

……



static const structmmc_host_ops msmsdcc_ops = {

        .request        = msmsdcc_request,

        .set_ios        =**msmsdcc_set_ios,**

        .enable_sdio_irq =msmsdcc_enable_sdio_irq,

};



所以实际上调用的是msmsdcc_set_ios，关于这个函数就不介绍了，可以参考源码，再看 mmc_detect_change(host, 0)，最后一句是：

      mmc_schedule_delayed_work(&host->detect,delay);

实际上就是调用我们前面说的延时函数 mmc_rescan，后面的流程是一样的。

#### 1.3.2.2        SDIO设备中断

SDIO设备通过SDIO总线与host相连，SDIO总线的DAT[1]即pin8可以作为中断线使用，当SDIO设备向host产生中断时，host会对终端做出相应的动作，在host驱动的probe函数中申请并注册相应的中断函数：

static int

msmsdcc_probe(structplatform_device *pdev)

{

......

  cmd_irqres = platform_get_resource_byname(pdev, IORESOURCE_IRQ,

                                                 "cmd_irq");

        pio_irqres =platform_get_resource_byname(pdev, IORESOURCE_IRQ,

                                                 "pio_irq");

** stat_irqres =platform_get_resource_byname(pdev, IORESOURCE_IRQ,**

**                                                  "status_irq");**

......

  if (stat_irqres && !(stat_irqres->flags &IORESOURCE_DISABLED)) {

                unsigned long irqflags =IRQF_SHARED |

                        (stat_irqres->flags& IRQF_TRIGGER_MASK);



**host->stat_irq = stat_irqres->start;**

**ret = request_irq(host->stat_irq,**

**                                  msmsdcc_platform_status_irq,**

**                                 irqflags,**

**                                 DRIVER_NAME " (slot)",**

**                                 host);**

                if (ret) {

                        pr_err("%s: Unableto get slot IRQ %d (%d)\n",

                              mmc_hostname(mmc), host->stat_irq, ret);

                        goto clk_disable;

                }

        }

......

}

当产生相应的中断时调用msmsdcc_platform_status_irq中断处理函数，这个函数的处理流程：

**msmsdcc_platform_status_irq—>**

**msmsdcc_check_statusà**

**mmc_detect_changeà**

**mmc_rescanà**

那么，这里为何调用mmc_rescan呢？因为前面说过mmc_rescanrescan函数主要用于SDIO设备的初始化，如果SDIO设备产生中断不应该是已经初始化可以使用了吗？其实mmc_rescan还有其它的工作，从函数名就能看出来它还有再扫描检测功能，即如果设备产生了中断，mmc_rescan函数一开始就会再次检测所有挂接在该host上的所有SDIO设备，确认是否存在，如果不存在就做相应的释放工作，以确保数据的一致性。如果检测到了新的设备那么它就会创建一个新的mmc_card，初始化并添加该设备。

中断引发的调用mmc_rescan动作的意义：实现了SDIO设备的热插拔功能。

### 1.3.3       WIFI驱动流程（二）



  此调用流程由dhd_bus_register发起，通过sdio_register_driver注册一个sdio设备驱动，然后通过dhdsdio_probe初始化并注册一个网络设备，网络设备的注册标志着wifi驱动已经成功加载，关于网络设备的创建，初始化和注册后面会有详细介绍，先来理一下上面的调用流程，：



**dhd_mudule_init—>             //path:dhd/sys/dhd_linux.c**

**Dhd_bus_registerà        // dhd/sys/dhd_sdio.c**

**Bcmsdh_registerà         // bcmsdio/sys/bcmsdh_linux.c**

**Sdio_function_inità              // bcmsdio/sys/bcmsdh_sdmmc_linux.c**

**Sdio_register_driverà  // bcmsdio/sys/bcmsdh_sdmmc_linux.c**

**Bcmsdh_sdmmc_probeà//bcmsdio/sys/bcmsdh_sdmmc_linux.c**

**Bcmsdh_probeà//bcmsdio/sys/bcmsdh_linux.c**

**Bcmsdio_probeà //dhd/sys/dhd_sdio.c**

这里注意上面两个红色标记的函数，sdio_register_driver注册了一个sdio设备，在匹配成功后调用bcmsdh_sdmmc_probe函数，这个函数会调用bcmsdh_probe。这里有一点要注意：浏览bcmsdh_linux.c文件可以看出，在bcmsdh_register函数中，当定义了BCMLXSDMMC宏时，会调用sdio_function_init函数，否则调用driver_register函数：

int

bcmsdh_register(bcmsdh_driver_t*driver)

{

        int error = 0;



**drvinfo = *driver; //注意这里，后面会介绍到它的用处**



#if defined(BCMPLATFORM_BUS)

**#if defined(BCMLXSDMMC)**

**       SDLX_MSG(("Linux Kernel SDIO/MMC Driver\n"));**

**        error =sdio_function_init();**

**#else**

**       SDLX_MSG(("Intel PXA270 SDIO Driver\n"));**

**        error =driver_register(&bcmsdh_driver);**

**#endif /* defined(BCMLXSDMMC) */**

        return error;

#endif /*defined(BCMPLATFORM_BUS) */



#if !defined(BCMPLATFORM_BUS)&& !defined(BCMLXSDMMC)

#if (LINUX_VERSION_CODE <KERNEL_VERSION(2, 6, 0))

        if (!(error =pci_module_init(&bcmsdh_pci_driver)))

                return 0;

#else

        if (!(error =pci_register_driver(&bcmsdh_pci_driver)))

                return 0;

#endif



        SDLX_MSG(("%s: pci_module_initfailed 0x%x\n", __FUNCTION__, error));

#endif /* BCMPLATFORM_BUS */



        return error;

}

上面的流程中有sdio_function_init的调用出现，所以这里实际上BCMLXSDMMC宏被定义了，bcmsdh_probe函数只是作为一个普通函数被调用，如果不定义该宏，那么bcmsdh_probe函数会被作为驱动匹配设备后第一个调用的函数而被自动调用。

再看看dhdsdio_probe函数调用的玄机，从上面的bcmsdh_register函数可以看出它的参数被传递给了drvinfo，看看bcmsdh_register的调用地方：

**static bcmsdh_driver_t dhd_sdio = {**

**        dhdsdio_probe,**

**        dhdsdio_disconnect**

**};**



int

dhd_bus_register(void)

{

        DHD_TRACE(("%s: Enter\n",__FUNCTION__));



        return **bcmsdh_register(&dhd_sdio);**

}

上面传递的参数是dhd_sdio结构变量，被用两个函数初始化了，那么哪一个是attach呢？需要找到定义bcmsdh_driver_t结构定义的地方：

Path：src/include/bcmsdh.h

/* callback functions */

typedef struct {

        /* attach to device */

**void *(*attach)(uint16 vend_id, uint16 dev_id, uint16 bus,uint16 slot,**

**                       uint16 func, uint bustype, void * regsva, osl_t * osh,**

**                       void * param);**

        /* detach from device */

        void (*detach)(void *ch);

} bcmsdh_driver_t;

没错，就是第一个dhdsdio_probe函数，再来看看什么地方调用了这个attach函数：

Path:bcmsdio/sys/bcmsdh_linux.c



#ifndef BCMLXSDMMC

static

#endif /* BCMLXSDMMC */

int bcmsdh_probe(struct device*dev)

{

......

if (!(sdhc->ch =** drvinfo.attach**((vendevid>> 16),

                                        (vendevid & 0xFFFF), 0, 0, 0, 0,

                                        (void*)regs, NULL, sdh))) {

                SDLX_MSG(("%s: device attachfailed\n", __FUNCTION__));

                goto err;

        }



        return 0;

......

}

红色部分的函数调用是drvinfo.attach，就是上面传递过来的dhdsdio_probe函数了，仔细阅读你会发现上面那个bcmsdh_driver_t结构体定义的地方有个说明，即把该结构的成员函数当做callback函数来使用，这就是它的用意所在。

### 1.3.4       网络设备注册流程



上面是网络设备注册流程，在dhdsdio_probe函数中先后对dhd_attach和dhd_net_attach两个函数调用，dhd_attach主要用于创建和初始化dhd_info_t和net_device两个结构变量，然后调用dhd_add_if将创建的net_device变量添加到dhd_info_t变量的iflist列表中（支持多接口）。

Dhd_attach的流程如下：

dhd_pub_t *

dhd_attach(osl_t *osh, structdhd_bus *bus, uint bus_hdrlen)

{

        dhd_info_t *dhd = NULL;

        struct net_device *net = NULL;



......

        /* Allocate etherdev, including spacefor private structure */

**if (!(net = alloc_etherdev(sizeof(dhd)))) {   //网络设备的创建**

                DHD_ERROR(("%s: OOM -alloc_etherdev\n", __FUNCTION__));

                goto fail;

        }

        dhd_state |=DHD_ATTACH_STATE_NET_ALLOC;





        /* Allocate primary dhd_info */

**if (!(dhd = MALLOC(osh, sizeof(dhd_info_t)))) { //dhd的创建**

                DHD_ERROR(("%s: OOM -alloc dhd_info\n", __FUNCTION__));

                goto fail;

        }

......

**/* Set network interface name if it was provided as moduleparameter */**

        if (iface_name[0]) {

                int len;

                char ch;

                strncpy(net->name,iface_name, IFNAMSIZ);

                net->name[IFNAMSIZ - 1] = 0;

                len = strlen(net->name);

                ch = net->name[len - 1];

                if ((ch > '9' || ch <'0') && (len < IFNAMSIZ - 2))

                        strcat(net->name,"%d");

        }



**if (dhd_add_if(dhd, 0, (void *)net, net->name, NULL, 0, 0)== DHD_BAD_IF)   //将前面创建的net添加到iflist列表中**

                goto fail;

        dhd_state |= DHD_ATTACH_STATE_ADD_IF;

......

**Memcpy(netdev_priv(net), &dhd, sizeof(dhd)); //关联dhd和net**



**//dhd的初始化工作**

}

Dhd_add_if的添加网络接口流程：

int

**dhd_add_if(dhd_info_t *dhd, int ifidx, void *handle, char *name,**

**        uint8 *mac_addr,uint32 flags, uint8 bssidx)**

{

        dhd_if_t *ifp;



        DHD_TRACE(("%s: idx %d,handle->%p\n", __FUNCTION__, ifidx, handle));



        ASSERT(dhd && (ifidx <DHD_MAX_IFS));



**ifp =dhd->iflist[ifidx];**

        if (ifp != NULL) {

                if (ifp->net != NULL) {

                       netif_stop_queue(ifp->net);

                       unregister_netdev(ifp->net);

**free_netdev(ifp->net);   //如果已经存在，释放net成员**

                }

        } else

**if ((ifp = MALLOC(dhd->pub.osh,sizeof(dhd_if_t))) == NULL) {**

**                       DHD_ERROR(("%s: OOM - dhd_if_t\n", __FUNCTION__));      //否则，创建一个dhd_if_t结构变量**

                        return -ENOMEM;

                }



        memset(ifp, 0, sizeof(dhd_if_t));

**ifp->info = dhd;      //进行系列初始化，添加工作**

**       dhd->iflist[ifidx] = ifp;**

**       strncpy(ifp->name, name, IFNAMSIZ);**

        ifp->name[IFNAMSIZ] = '\0';

        if (mac_addr != NULL)

**memcpy(&ifp->mac_addr, mac_addr,ETHER_ADDR_LEN);**



        if (handle == NULL) {

                ifp->state = DHD_IF_ADD;

                ifp->idx = ifidx;

                ifp->bssidx = bssidx;

               ASSERT(&dhd->thr_sysioc_ctl.thr_pid >= 0);

               up(&dhd->thr_sysioc_ctl.sema);

        } else

**ifp->net = (struct net_device *)handle;             //handle即一个net_device变量**



        return 0;

}

这样，一个net_device网路设备就被添加到了接口管理列表中了，但是这是网路设备还没有完成初始化和注册工作，bcmsdio_probe函数随后对dhd_net_attach的调用完成了这个操作：

int

dhd_net_attach(dhd_pub_t*dhdp, int ifidx)

{

        dhd_info_t *dhd = (dhd_info_t*)dhdp->info;

        struct net_device *net = NULL;

        int err = 0;

        uint8 temp_addr[ETHER_ADDR_LEN] = {0x00, 0x90, 0x4c, 0x11, 0x22, 0x33 };



        DHD_TRACE(("%s: ifidx %d\n",__FUNCTION__, ifidx));



        ASSERT(dhd &&dhd->iflist[ifidx]);



**net = dhd->iflist[ifidx]->net;              //首先从刚才添加的接口列表中取出net，然后进行下面的系列初始化工作**

        ASSERT(net);

**//根据内核版本信息，选择对net成员函数的初始化方式，假设是2.6.30的版本**

#if (LINUX_VERSION_CODE <KERNEL_VERSION(2, 6, 31))

        ASSERT(!net->open);

**net->get_stats = dhd_get_stats;**

**        net->do_ioctl =dhd_ioctl_entry;**

**       net->hard_start_xmit = dhd_start_xmit;**

**       net->set_mac_address = dhd_set_mac_address;**

**       net->set_multicast_list = dhd_set_multicast_list;**

**        net->open =net->stop = NULL;**

#else

        ASSERT(!net->netdev_ops);

        net->netdev_ops = &dhd_ops_virt;

#endif



        /* Ok, link into the network layer...*/

        if (ifidx == 0) {

                /*

                 * device functions for theprimary interface only

                 */

#if (LINUX_VERSION_CODE <KERNEL_VERSION(2, 6, 31))

**net->open = dhd_open;**

**               net->stop = dhd_stop;**

#else

                net->netdev_ops = &dhd_ops_pri;

#endif

        } else {

                /*

                 * We have to use the primaryMAC for virtual interfaces

                                                                                                                          3417,1-8      66%

                 */

                memcpy(temp_addr,dhd->iflist[ifidx]->mac_addr, ETHER_ADDR_LEN);

                /*

                 * Android sets the locallyadministered bit to indicate that this is a

                 * portable hotspot.  This will not work in simultaneous AP/STAmode,

                 * nor with P2P.  Need to set the Donlge's MAC address, andthen use that.

                 */

** if(!memcmp(temp_addr, dhd->iflist[0]->mac_addr,**

**                        ETHER_ADDR_LEN)) {**

                        DHD_ERROR(("%sinterface [%s]: set locally administered bit in MAC\n",

                        __func__,net->name));

                        temp_addr[0] |= 0x02;

                }

        }



       net->hard_header_len = ETH_HLEN + dhd->pub.hdrlen;

#if LINUX_VERSION_CODE >=KERNEL_VERSION(2, 6, 24)

**net->ethtool_ops = &dhd_ethtool_ops;**

#endif /* LINUX_VERSION_CODE>= KERNEL_VERSION(2, 6, 24) */



#ifdefined(CONFIG_WIRELESS_EXT)

#if WIRELESS_EXT < 19

**net->get_wireless_stats = dhd_get_wireless_stats;**

#endif /* WIRELESS_EXT < 19*/

#if WIRELESS_EXT > 12

**net->wireless_handlers = (struct iw_handler_def*)&wl_iw_handler_def;   //这里的初始化工作很重要，之后的ioctl流程会涉及到对它的使用**

#endif /* WIRELESS_EXT > 12*/

#endif /*defined(CONFIG_WIRELESS_EXT) */



        dhd->pub.rxsz =DBUS_RX_BUFFER_SIZE_DHD(net);

**//设置设备地址**

**memcpy(net->dev_addr, temp_addr, ETHER_ADDR_LEN);**



** if ((err =register_netdev(net)) != 0) {       //注册net**

                DHD_ERROR(("couldn'tregister the net device, err %d\n", err));

                goto fail;

        }





……

}

到这里net网络设备就被注册到系统中了，设备准备好了就好对设备进行访问了









