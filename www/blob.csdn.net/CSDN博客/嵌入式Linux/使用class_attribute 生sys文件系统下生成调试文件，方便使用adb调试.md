
# 使用class_attribute 生sys文件系统下生成调试文件，方便使用adb调试 - 嵌入式Linux - CSDN博客

2015年07月22日 14:44:24[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：728



```python
/*
  *  och177.c - Linux kernel modules for ambient light sensor
  *
  *  Copyright (C) 2011
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
  */
 
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/wakelock.h>

static struct class *dunk_1_gpio_test_class=NULL;

static ssize_t dunk_1_gnss_set_high(struct class *cls, struct class_attribute *attr, const  char *_buf, size_t _count)
{
	  int gpio=0;
	  sscanf(_buf, "%d", &gpio);
	  printk("%s: gpio: %d \n", __FUNCTION__, gpio);
	  
	  gpio_direction_output(gpio, 1);
      return _count;
}

static ssize_t dunk_1_gnss_set_low(struct class *cls, struct class_attribute *attr, const  char *_buf, size_t _count)
{
	  int gpio=0;
	  sscanf(_buf, "%d", &gpio);
	  printk("%s: gpio: %d \n", __FUNCTION__, gpio);
	  gpio_direction_output(gpio, 0);
      return _count;
}

//这里可以去看一下class_attribute这个结构体，这个结构体对理解这个声明有用
static struct class_attribute dunk_1_gpio_test_attr[] = {
__ATTR(output_high,0666, NULL, dunk_1_gnss_set_high),
__ATTR(output_low,0666, NULL, dunk_1_gnss_set_low),
__ATTR_NULL,
};

static int __init dunk_1_gpio_test_init(void)
{
  int ret;
  int i = 0;
  printk("%s start\n", __FUNCTION__);
	//用class_create在sys/class/下面生成sys文件系统
  dunk_1_gpio_test_class=class_create(THIS_MODULE,"dunk_1_gpioex_test");
  if(IS_ERR(dunk_1_gpio_test_class))
  {
  	printk("create dunk_1_gnss fail \n");
  	return PTR_ERR(dunk_1_gpio_test_class);;
  }
  for (i = 0 ; NULL != attr_name(dunk_1_test_attr[i]);i++)
  {
	  ret = class_create_file(dunk_1_gpio_test_class, &dunk_1_gpio_test_attr[i]);
	  if (0 != ret)
	  {
		  printk("creat %s class file fail\n",attr_name(dunk_1_gpio_test_attr[i]));
		  break;
	  }
  }
  printk("%s end\n", __FUNCTION__);
  return 0;
	
}
static void __exit dunk_1_gpio_test_exit(void)
{
    int i = 0;
	printk("dunk_1 gnss module cleanup start.\n");
	for (i = 0 ; NULL != attr_name(dunk_1_gpio_test_attr[i]);i++)
	{
        class_remove_file(dunk_1_gpio_test_class, &dunk_1_gpio_test_attr[i]);
    }
	class_destroy(dunk_1_gpio_test_class);
	printk("dunk_1 gnss module cleanup OK!\n");
}
MODULE_AUTHOR("dunk_1 <dunk_1@163.com.cn>");
MODULE_DESCRIPTION("dunk_1gpio test MODULE");
MODULE_LICENSE("GPL");
MODULE_VERSION("ver0.1");
module_init(dunk_1_gpio_test_init);
module_exit(gota_gpio_test_exit);
```


