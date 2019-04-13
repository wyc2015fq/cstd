
# linux下的I2C驱动记录（RK） - 嵌入式Linux - CSDN博客

2013年09月04日 16:49:48[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：4857


**现在做的是RK平台的I2C驱动，不是说linux每个外设对应一个设备文件吗？可是找了一下一直没找到，今天特别搞了一下。I2C设备是注册为platform平台驱动的。**
**下面是我用adb命令查看的**
**linux的设备文件不一定只在dev/这个目录下面，像sys/ 目录下面也会有一些设备文件的**
**MID下面的I2C设备的设备文件**

```python
1|root@android:/ # ls sys/bus/i2c/drivers/                                     
RK610_CODEC/    gc0308/         ir-kbd-i2c/     sensors/        tps65910/    
act8846/        gc0328/         nt99340/        sp0838/         wm831x/      
dummy/          gc2035/         rk610_ctl/      sp2518/         
ft5x0x_touch/   gslX680/        rk610_hdmi/     sp2541/
```
**MID下面的I2C设备的I2C地址**

```python
root@android:/ # ls sys/bus/i2c/devices/                                       
0-004c/ 1-002d/ 1-005a/ 2-0040/ 3-003c/ 4-0042/ 4-0060/ i2c-1/  i2c-3/  
1-0014/ 1-0034/ 2-0038/ 3-0021/ 4-0040/ 4-0046/ i2c-0/  i2c-2/  i2c-4/
```

**在板级文件里面，我们可以看到我们的I2C设备地址是多少，代码如下，用i2c_register_board_info这个函数来注册i2c的device。****每一个I2C的硬件接口对应一个i2cX_info**
static void __init rk30_i2c_register_board_info(void)
{
\#ifdef CONFIG_I2C0_RK30
	i2c_register_board_info(0, i2c0_info, ARRAY_SIZE(i2c0_info));
\#endif
\#ifdef CONFIG_I2C1_RK30
	i2c_register_board_info(1, i2c1_info, ARRAY_SIZE(i2c1_info));
\#endif
\#ifdef CONFIG_I2C2_RK30
	i2c_register_board_info(2, i2c2_info, ARRAY_SIZE(i2c2_info));
\#endif
\#ifdef CONFIG_I2C3_RK30
	i2c_register_board_info(3, i2c3_info, ARRAY_SIZE(i2c3_info));
\#endif
\#ifdef CONFIG_I2C4_RK30
	i2c_register_board_info(4, i2c4_info, ARRAY_SIZE(i2c4_info));
\#endif
\#ifdef CONFIG_I2C_GPIO_RK30
	i2c_register_board_info(5, i2c_gpio_info, ARRAY_SIZE(i2c_gpio_info));
\#endif
}
```python
#if defined (CONFIG_TOUCHSCREEN_GSLX680)
    {
        .type           = "gslX680",
        .addr           = 0x40,
        .flags          = 0,
        .platform_data =&gslx680_info,
    },
#endif
```
**然后看I2C的对应的设备驱动，下面给出一个TP的驱动的注册的地方，用i2c_add_driver这个函数去注册i2c的驱动driver**

```python
static const struct i2c_device_id gsl_ts_id[] = {
	{GSLX680_I2C_NAME, 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, gsl_ts_id);

static struct i2c_driver gsl_ts_driver = {
	.driver = {
		.name = GSLX680_I2C_NAME,
		.owner = THIS_MODULE,
	},
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend	= gsl_ts_suspend,
	.resume	= gsl_ts_resume,
#endif
	.probe		= gsl_ts_probe,
	.remove		= __devexit_p(gsl_ts_remove),
	.id_table	= gsl_ts_id,
};
static int __init gsl_ts_init(void)
{
    int ret;
	printk("==gsl_ts_init==\n");
	ret = i2c_add_driver(&gsl_ts_driver);
	printk("ret=%d\n",ret);
	return ret;
}
static void __exit gsl_ts_exit(void)
{
	printk("==gsl_ts_exit==\n");
	i2c_del_driver(&gsl_ts_driver);
	return;
}
module_init(gsl_ts_init);
module_exit(gsl_ts_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GSLX680 touchscreen controller driver");
MODULE_AUTHOR("Guan Yuwei, guanyuwei@basewin.com");
MODULE_ALIAS("platform:gsl_ts");
```

**好了。然后再看一下这里，可以看到 devices/和drivers/**
```python
1|root@android:/ # ls sys/bus/i2c/                                             
devices/            drivers_autoprobe   uevent              
drivers/            drivers_probe
```
**再看一下这个，就很清楚的知道这个设备挂载在哪个I2C接口下面了**


```python
root@android:/ # ll sys/bus/i2c/drivers/gslX680/                               
lrwxrwxrwx root     root              2013-09-04 08:51 2-0040 -> ../../../../devices/platform/rk30_i2c.2/i2c-2/2-0040
--w------- root     root         4096 2013-09-04 08:51 bind
--w------- root     root         4096 2013-09-04 08:51 uevent
--w------- root     root         4096 2013-09-04 08:51 unbind
root@android:/ #
```

```python
root@android:/ # ll sys/bus/i2c/devices/2-0040/                                
lrwxrwxrwx root     root              2013-09-04 08:55 driver -> ../../../../../bus/i2c/drivers/gslX680
drwxr-xr-x root     root              2013-09-04 08:55 input
-r--r--r-- root     root         4096 2013-09-04 08:55 modalias
-r--r--r-- root     root         4096 2013-09-04 08:55 name
drwxr-xr-x root     root              2013-09-04 08:55 power
lrwxrwxrwx root     root              2013-09-04 08:55 subsystem -> ../../../../../bus/i2c
-rw-r--r-- root     root         4096 2013-09-04 08:55 uevent
root@android:/ #
```


**到这里已经能够很直接得去理解这个i2c驱动了。其他对于一些代码的理解，网上很多~**


**自己写了一个I2C的驱动。**

```python
/*
 * drivers/input/touchscreen/gslX680.c
 *
 * Copyright (c) 2012 Shanghai Basewin
 *	Guan Yuwei<guanyuwei@basewin.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/earlysuspend.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/async.h>
#include <mach/gpio.h>
#include <linux/irq.h>
#include <mach/board.h>
#include <linux/workqueue.h>
#include <linux/proc_fs.h>
#include <linux/input/mt.h>
#define WQF_I2C_NAME "wfi2ctest"
static const struct i2c_device_id gsl_ts_id[] = {
	{WQF_I2C_NAME, 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, gsl_ts_id);

static struct i2c_driver wftest_driver = {
	.driver = {
		.name = WQF_I2C_NAME,
		.owner = THIS_MODULE,
	},
};
static int __init wf_init(void)
{
    int ret;
	printk("==wftest_driver==\n");
	ret = i2c_add_driver(&wftest_driver);
	printk("ret=%d\n",ret);
	return ret;
}
static void __exit wf_exit(void)
{
	printk("==wftest_driver==\n");
	i2c_del_driver(&wftest_driver);
	return;
}
module_init(wf_init);
module_exit(wf_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("wqf test i2c driver");
MODULE_AUTHOR("wf , 329410527@qq.com");
MODULE_ALIAS("platform:test");
```


```python
root@android:/ # ls /sys/bus/i2c/devices/                                      
0-004c/ 1-002d/ 1-005a/ 2-0040/ 3-0021/ 4-0040/ 4-0046/ i2c-0/  i2c-2/  i2c-4/ 
1-0014/ 1-0034/ 2-0038/ 2-0041/ 3-003c/ 4-0042/ 4-0060/ i2c-1/  i2c-3/  
root@android:/ # ls /sys/bus/i2c/drivers                                       
drivers/            drivers_autoprobe   drivers_probe       
root@android:/ # ls /sys/bus/i2c/drivers/                                      
RK610_CODEC/    gc0308/         ir-kbd-i2c/     sensors/        tps65910/    
act8846/        gc0328/         nt99340/        sp0838/         wfi2ctest/   
dummy/          gc2035/         rk610_ctl/      sp2518/         wm831x/      
ft5x0x_touch/   gslX680/        rk610_hdmi/     sp2541/         
root@android:/ # ls /sys/bus/i2c/drivers/
```


