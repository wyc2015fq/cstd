# 触摸芯片tsc2003 linux下driver - xqhrs232的专栏 - CSDN博客
2011年09月07日 22:31:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1739
原文地址::[http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/014227.html](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/014227.html)
# [lm-sensors] tsc2003 driver
**Bill Gatliff**[bgat at billgatliff.com](mailto:lm-sensors%40lm-sensors.org?Subject=Re:%20%5Blm-sensors%5D%20tsc2003%20driver&In-Reply-To=%3C436D658E.1020207%40billgatliff.com%3E)
*Sun Nov 6 03:08:14 CET 2005*
- Previous message: [[lm-sensors] tsc2003 driver](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/014223.html)
- Next message: [[lm-sensors] [PATCH 00/25] reduce code in fs/compat_ioctl.c](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/014221.html)
- **Messages sorted by:**[[ date ]](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/date.html#14227)[[ thread ]](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/thread.html#14227)[[ subject ]](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/subject.html#14227)[[ author ]](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/author.html#14227)
Jean:
Jean Delvare wrote:
>*Hi Bill,*>>>>>*Attached is a driver for the tsc2003 touch screen ic.  Tested on 2.6.12.*>>>>>>*Failed. Most probably the list ate your your attachement because it had*>*a bad MIME type. We take text/plain and text/x-patch only. Try again.*>>
Driver source inlined below.
>*Also, what do you expect from us? None of us is willing to review a*>*patch for a 2.6.12 kernel.*>>
I'm on 2.6.12 right now, so the driver is what it is.  If it's of use to 
anyone, then great.  If not, then that's fine too.  Let it sit in the 
email archives until someone finds it.
/*
 *  linux/drivers/i2c/chips/tsc2003.c
 *
 *  Copyright (C) 2005 Bill Gatliff <[bgat at billgatliff.com](http://lists.lm-sensors.org/mailman/listinfo/lm-sensors)>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  Driver for TI's TSC2003 I2C Touch Screen Controller
 */
#include <linux/config.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/i2c-sensor.h>
#include <linux/string.h>
#include <linux/bcd.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <asm/delay.h>
#include <asm/arch/gpio.h>
static unsigned short normal_i2c[]
= {0x48, 0x49, 0x4a, 0x4b, I2C_CLIENT_END };
static unsigned int normal_isa[] = { I2C_CLIENT_ISA_END };
SENSORS_INSMOD_1(tsc2003);
#define DRIVER_NAME "tsc2003"
enum tsc2003_pd {
  PD_POWERDOWN = 0, /* penirq */
  PD_IREFOFF_ADCON = 1, /* no penirq */
  PD_IREFON_ADCOFF = 2, /* penirq */
  PD_IREFON_ADCON = 3, /* no penirq */
  PD_PENIRQ_ARM = PD_IREFON_ADCOFF,
  PD_PENIRQ_DISARM = PD_IREFON_ADCON,
};
enum tsc2003_m {
  M_12BIT = 0,
  M_8BIT = 1
};
enum tsc2003_cmd {
  MEAS_TEMP0 = 0,
  MEAS_VBAT1 = 1,
  MEAS_IN1 = 2,
  MEAS_TEMP1 = 4,
  MEAS_VBAT2 = 5,
  MEAS_IN2 = 6,
  ACTIVATE_NX_DRIVERS = 8,
  ACTIVATE_NY_DRIVERS = 9,
  ACTIVATE_YNX_DRIVERS = 10,
  MEAS_XPOS = 12,
  MEAS_YPOS = 13,
  MEAS_Z1POS = 14,
  MEAS_Z2POS = 15
};
#define TSC2003_CMD(cn,pdn,m) (((cn) << 4) | ((pdn) << 2) | ((m) << 1))
#define ADC_MAX ((1 << 12) - 1)
struct tsc2003_data {
  struct i2c_client client;
  struct device_driver driver;
  struct input_dev idev;
  struct timer_list penirq_timer;
  struct semaphore sem;
  struct task_struct *tstask;
  struct completion tstask_completion;
  struct completion penirq_completion;
  enum tsc2003_pd pd;
  enum tsc2003_m m;
  int penirq;
  int vbat1;
  int vbat2;
  int temp0;
  int temp1;
  int in1;
  int in2;
};
static int tsc2003_i2c_detect (struct i2c_adapter *adapter, int address, 
int kind);
static inline int tsc2003_command (struct tsc2003_data *data,
                                   enum tsc2003_cmd cmd,
                                   enum tsc2003_pd pd)
{
  char c;
  int ret;
  down(&data->sem);
  c = TSC2003_CMD(cmd, pd, data->m);
  ret = i2c_master_send(&data->client, &c, 1);
  up(&data->sem);
  return ret;
}
static int tsc2003_read (struct tsc2003_data *data,
                         enum tsc2003_cmd cmd,
                         enum tsc2003_pd pd,
                         int *val)
{
  char c;
  char d[2];
  int ret;
  c = TSC2003_CMD(cmd, pd, data->m);
  ret = i2c_master_send(&data->client, &c, 1);
  if (ret <= 0) goto err;
  udelay(20);
  ret = i2c_master_recv(&data->client, d, data->m == M_12BIT ? 2 : 1);
  if (ret <= 0) goto err;
  if (val)
    {
      *val = d[0];
      *val <<= 4;
      if (data->m == M_12BIT)
        *val += (d[1] >> 4);
    }
#if defined(CONFIG_I2C_DEBUG_CHIP)
  printk(KERN_ERR "%s: val[%x] = %d\n",
         __FUNCTION__, cmd, (((int)d[0]) << 8) + d[1]);
#endif
  return 0;
 err:
  if (!ret) ret = -ENODEV;
  return ret;
}
static inline int tsc2003_read_temp0 (struct tsc2003_data *d, enum 
tsc2003_pd pd, int *t)
{
  return tsc2003_read(d, MEAS_TEMP0, pd, t);
}
static inline int tsc2003_read_temp1 (struct tsc2003_data *d, enum 
tsc2003_pd pd, int *t)
{
  return tsc2003_read(d, MEAS_TEMP1, pd, t);
}
static inline int tsc2003_read_xpos (struct tsc2003_data *d, enum 
tsc2003_pd pd, int *x)
{
  return tsc2003_read(d, MEAS_XPOS, pd, x);
}
static inline int tsc2003_read_ypos (struct tsc2003_data *d, enum 
tsc2003_pd pd, int *y)
{
  return tsc2003_read(d, MEAS_YPOS, pd, y);
}
static inline int tsc2003_read_pressure (struct tsc2003_data *d, enum 
tsc2003_pd pd, int *p)
{
  return tsc2003_read(d, MEAS_Z1POS, pd, p);
}
static inline int tsc2003_read_in1 (struct tsc2003_data *d, enum 
tsc2003_pd pd, int *t)
{
  return tsc2003_read(d, MEAS_IN1, pd, t);
}
static inline int tsc2003_read_in2 (struct tsc2003_data *d, enum 
tsc2003_pd pd, int *t)
{
  return tsc2003_read(d, MEAS_IN2, pd, t);
}
static inline int tsc2003_read_vbat1 (struct tsc2003_data *d, enum 
tsc2003_pd pd, int *t)
{
  return tsc2003_read(d, MEAS_VBAT1, pd, t);
}
static inline int tsc2003_read_vbat2 (struct tsc2003_data *d, enum 
tsc2003_pd pd, int *t)
{
  return tsc2003_read(d, MEAS_VBAT2, pd, t);
}
static inline int tsc2003_powerdown (struct tsc2003_data *d)
{
  /* we don't have a distinct powerdown command,
     so do a benign read with the PD bits cleared */
  return tsc2003_read(d, MEAS_IN1, PD_POWERDOWN, 0);
}
void tsc2003_init_client (struct i2c_client *client)
{
  struct tsc2003_data *data = i2c_get_clientdata(client);
  data->pd = PD_PENIRQ_DISARM;
  data->m = M_8BIT;
  return;
}
#define PENUP_TIMEOUT 50 /* msec */
static irqreturn_t tsc2003_penirq (int irq, void *v, struct pt_regs *regs)
{
  struct tsc2003_data *d = v;
  complete(&d->penirq_completion);
  return IRQ_HANDLED;
}
static int tsc2003_remove (struct device *dev)
{
  return 0;
}
static void tsc2003_pen_up (unsigned long v)
{
  struct tsc2003_data *d = (struct tsc2003_data *)v;
  input_report_abs(&d->idev, ABS_PRESSURE, 0);
  input_sync(&d->idev);
  return;
}
static inline void tsc2003_restart_pen_up_timer (struct tsc2003_data *d)
{
  mod_timer(&d->penirq_timer, jiffies + (PENUP_TIMEOUT * HZ) / 1000);
}
static int tsc2003ts_thread (void *v)
{
  struct tsc2003_data *d = v;
  struct task_struct *tsk = current;
  int ret;
  d->tstask = tsk;
  daemonize(DRIVER_NAME "tsd");
  allow_signal(SIGKILL);
  complete(&d->tstask_completion);
  printk(KERN_INFO "%s: address 0x%x\n",
         __FUNCTION__, d->client.addr);
  while (!signal_pending(tsk))
    {
      unsigned int x, y, p;
     
      if (!down_interruptible(&d->sem))
        {
          d->pd = PD_PENIRQ_DISARM;
          disable_irq(d->penirq);
          tsc2003_read_xpos(d, PD_PENIRQ_DISARM, &x);
          tsc2003_read_ypos(d, PD_PENIRQ_DISARM, &y);
          tsc2003_read_pressure(d, PD_PENIRQ_DISARM, &p);
         
          /* non-X-Y driver read to avoid glitch in penirq (errata?) */
          tsc2003_read_vbat1(d, PD_PENIRQ_DISARM, 0);
          enable_irq(d->penirq);
         
          input_report_abs(&d->idev, ABS_X, x);
          input_report_abs(&d->idev, ABS_Y, y);
          input_report_abs(&d->idev, ABS_PRESSURE, p);
          input_sync(&d->idev);
         
          do
            {
              ret = 
wait_for_completion_interruptible_timeout(&d->penirq_completion, HZ / 100);
              if (ret)
                {
                  if (d->pd == PD_PENIRQ_DISARM)
                    {
                      /* fake penirq, avoid glitch and then start 
watching for real now */
                      d->pd = PD_PENIRQ_ARM;
                      tsc2003_read_vbat1(d, PD_PENIRQ_ARM, 0);
                      up(&d->sem);
                    }
                  else
                    {
                      /* pen down event, (re)start the pen up timer */
                      tsc2003_restart_pen_up_timer(d);
                    }
                }
              else
                {
                  if (d->pd != PD_PENIRQ_ARM)
                    {
                      /* fake penirq never arrived, which would be the case
                         for a level-triggered irq line */
                      tsc2003_read_vbat1(d, PD_PENIRQ_ARM, 0);
                      d->pd = PD_PENIRQ_ARM;
                      up(&d->sem);
                    }
                  if (timer_pending(&d->penirq_timer))
                    {
                      /* pen is down, time to send (another) sample */
                      break;
                    }
                  else
                    {
                      /* pen is up, nothing to do but wait for it to 
come down */
                      
wait_for_completion_interruptible(&d->penirq_completion);
                      /* pen is down, (re)start the pen up timer */
                      tsc2003_restart_pen_up_timer(d);
                      break;
                    }
                }
            }
          while (!signal_pending(tsk));
        }
    }
     
  d->tstask = NULL;
  complete_and_exit(&d->tstask_completion, 0);
}
static int tsc2003_idev_open (struct input_dev *idev)
{
  struct tsc2003_data *d = container_of(idev, struct tsc2003_data, idev);
  int ret = 0;
  if (down_interruptible(&d->sem))
    return -EINTR;
  if (d->tstask)
    panic(DRIVER_NAME "tsd already running (!). abort.");
  if (d->penirq)
    {
      ret = request_irq(d->penirq, tsc2003_penirq, 0, DRIVER_NAME, d);
      if (!ret)
        {
          set_irq_type(d->penirq, IRQT_FALLING);
          printk(KERN_INFO "%s: irq %d\n", __FUNCTION__, d->penirq);
        }
    }
  init_completion(&d->tstask_completion);
  init_completion(&d->penirq_completion);
  d->penirq_timer.data = (unsigned long)d;
  d->penirq_timer.function = tsc2003_pen_up;
  ret = kernel_thread(tsc2003ts_thread, d, CLONE_KERNEL);
  if (ret >= 0)
    {
      wait_for_completion(&d->tstask_completion);
      ret = 0;
    }
  else
    free_irq(d->penirq, d);
  up(&d->sem);
  return ret;
}
static void tsc2003_idev_close (struct input_dev *idev)
{
  struct tsc2003_data *d = container_of(idev, struct tsc2003_data, idev);
  down_interruptible(&d->sem);
  if (d->tstask)
    {
      send_sig(SIGKILL, d->tstask, 1);
      wait_for_completion(&d->tstask_completion);
    }
  free_irq(d->penirq, d);
  if (timer_pending(&d->penirq_timer))
    del_timer(&d->penirq_timer);
  up(&d->sem);
  return;
}
#if defined(CONFIG_SYSFS) && defined(CONFIG_SENSORS_TSC2003_SYSFS)
static ssize_t show_addr (struct device *dev, char *buf)
{
  struct tsc2003_data *d = container_of(dev->driver, struct 
tsc2003_data, driver);
  return sprintf(buf, "%d\n", d->client.addr);
}
static DEVICE_ATTR(addr, S_IRUGO, show_addr, NULL);
static ssize_t show_vbat1 (struct device *dev, char *buf)
{
  struct tsc2003_data *d = container_of(dev->driver, struct 
tsc2003_data, driver);
  return sprintf(buf, "%d\n", d->vbat1);
}
static DEVICE_ATTR(vbat1, S_IRUGO, show_vbat1, NULL);
static ssize_t show_vbat2 (struct device *dev, char *buf)
{
  struct tsc2003_data *d = container_of(dev->driver, struct 
tsc2003_data, driver);
  return sprintf(buf, "%d\n", d->vbat2);
}
static DEVICE_ATTR(vbat2, S_IRUGO, show_vbat2, NULL);
static ssize_t show_in1 (struct device *dev, char *buf)
{
  struct tsc2003_data *d = container_of(dev->driver, struct 
tsc2003_data, driver);
  return sprintf(buf, "%d\n", d->in1);
}
static DEVICE_ATTR(in1, S_IRUGO, show_in1, NULL);
static ssize_t show_in2 (struct device *dev, char *buf)
{
  struct tsc2003_data *d = container_of(dev->driver, struct 
tsc2003_data, driver);
  return sprintf(buf, "%d\n", d->in2);
}
static DEVICE_ATTR(in2, S_IRUGO, show_in2, NULL);
static ssize_t show_temp0 (struct device *dev, char *buf)
{
  struct tsc2003_data *d = container_of(dev->driver, struct 
tsc2003_data, driver);
  return sprintf(buf, "%d\n", d->temp0);
}
static DEVICE_ATTR(temp0, S_IRUGO, show_temp0, NULL);
static ssize_t show_temp1 (struct device *dev, char *buf)
{
  struct tsc2003_data *d = container_of(dev->driver, struct 
tsc2003_data, driver);
  return sprintf(buf, "%d\n", d->temp1);
}
static DEVICE_ATTR(temp1, S_IRUGO, show_temp1, NULL);
#warning "TODO: this daemon sometimes hangs the touchscreen daemon"
#warning "TODO: under periods of heavy touch screen activity."
#warning "TODO: Use with caution until the bug is squashed."
static int tsc2003s_thread (void *v)
{
  struct tsc2003_data *d = v;
  daemonize(DRIVER_NAME "sd");
  allow_signal(SIGKILL);
  printk(KERN_INFO "%s: address 0x%x\n",
         __FUNCTION__, d->client.addr);
  while (!signal_pending(current))
    {
      if (!down_interruptible(&d->sem))
        {
          if (!timer_pending(&d->penirq_timer))
            {
              tsc2003_read_vbat1(d, d->pd, &d->vbat1);
              tsc2003_read_vbat2(d, d->pd, &d->vbat2);
              tsc2003_read_in1(d, d->pd, &d->in1);
              tsc2003_read_in2(d, d->pd, &d->in2);
              tsc2003_read_temp0(d, d->pd, &d->temp0);
              tsc2003_read_temp1(d, d->pd, &d->temp1);
            }
          up(&d->sem);
        }
      set_task_state(current, TASK_INTERRUPTIBLE);
      schedule_timeout(5 * HZ);
    }
  do_exit(0);
}
#endif
static int tsc2003_probe (struct device *dev)
{
  struct platform_device *p = to_platform_device(dev);
  int irq = platform_get_irq(p, 0);
  struct tsc2003_data *d = container_of(dev->driver, struct 
tsc2003_data, driver);
  int ret = 0;
  d->penirq = irq;
#if defined(CONFIG_SYSFS) && defined(CONFIG_SENSORS_TSC2003_SYSFS)
  ret = kernel_thread(tsc2003s_thread, d, CLONE_KERNEL);
  if (ret >= 0)
    ret = 0;
  device_create_file(dev, &dev_attr_addr);
  device_create_file(dev, &dev_attr_vbat1);
  device_create_file(dev, &dev_attr_vbat2);
  device_create_file(dev, &dev_attr_in1);
  device_create_file(dev, &dev_attr_in2);
  device_create_file(dev, &dev_attr_temp0);
  device_create_file(dev, &dev_attr_temp1);
#endif
  return ret;
}
static int tsc2003_driver_register (struct tsc2003_data *data)
{
  int ret = 0;
  data->driver.name = DRIVER_NAME;
  data->driver.probe = tsc2003_probe;
  data->driver.remove = tsc2003_remove;
  data->driver.bus = &platform_bus_type;
 
  init_MUTEX(&data->sem);
  init_timer(&data->penirq_timer);
  data->penirq_timer.data = (unsigned long)data;
  data->penirq_timer.function = tsc2003_pen_up;
  init_input_dev(&data->idev);
  data->idev.name = DRIVER_NAME;
  data->idev.evbit[0] = BIT(EV_ABS);
  data->idev.open = tsc2003_idev_open;
  data->idev.close = tsc2003_idev_close;
  data->idev.absbit[LONG(ABS_X)] = BIT(ABS_X);
  data->idev.absbit[LONG(ABS_Y)] = BIT(ABS_Y);
  data->idev.absbit[LONG(ABS_PRESSURE)] = BIT(ABS_PRESSURE);
  input_set_abs_params(&data->idev, ABS_X, 0, ADC_MAX, 0, 0);
  input_set_abs_params(&data->idev, ABS_Y, 0, ADC_MAX, 0, 0);
  if (!driver_find(DRIVER_NAME, &platform_bus_type))
    ret = driver_register(&(data->driver));
  if (!ret)
    input_register_device(&data->idev);
  return ret;
}
static int tsc2003_i2c_attach_adapter(struct i2c_adapter *adapter)
{
  printk(KERN_INFO "tsc2003 i2c touch screen controller\n");
  printk(KERN_INFO "Bill Gatliff <[bgat at billgatliff.com](http://lists.lm-sensors.org/mailman/listinfo/lm-sensors)>\n");
 
  return i2c_detect(adapter, &addr_data, tsc2003_i2c_detect);
}
static int tsc2003_i2c_detach_client(struct i2c_client *client)
{
  int err;
  if ((err = i2c_detach_client(client))) {
    dev_err(&client->dev, "Client deregistration failed, "
            "client not detached.\n");
    return err;
  }
  return 0;
}
static struct i2c_driver tsc2003_driver = {
  .owner        = THIS_MODULE,
  .name                 = DRIVER_NAME,
  .flags        = I2C_DF_NOTIFY,
  .attach_adapter    = tsc2003_i2c_attach_adapter,
  .detach_client    = tsc2003_i2c_detach_client,
  //  .command              = tsc2003_command,
};
static int tsc2003_i2c_detect (struct i2c_adapter *adapter, int address, 
int kind)
{
  struct i2c_client *new_client;
  struct tsc2003_data *data;
  int err = 0;
  const char *name = "";
  if (!i2c_check_functionality(adapter,
                               I2C_FUNC_SMBUS_BYTE_DATA
                               | I2C_FUNC_I2C
                               | I2C_FUNC_SMBUS_WORD_DATA))
    goto exit;
  data = kcalloc(1, sizeof(*data), GFP_KERNEL);
  if (!data) {
    err = -ENOMEM;
    goto exit;
  }
  new_client = &data->client;
  i2c_set_clientdata(new_client, data);
  new_client->addr = address;
  new_client->adapter = adapter;
  new_client->driver = &tsc2003_driver;
  new_client->flags = 0;
  /* TODO: I'm pretty sure I'm not dealing with kind correctly */
  if (kind == 0 /* identification */ || kind < 0 /* detection */)
      kind = tsc2003;
  if (kind == tsc2003)
    name = DRIVER_NAME;
  /* try a command, see if we get an ack;
     if we do, assume it's our device */
  printk(KERN_INFO "%s: probing address 0x%x\n",
         __FUNCTION__, address);
  err = tsc2003_powerdown(data);
  if (err >= 0)
    {
      strlcpy(new_client->name, name, I2C_NAME_SIZE);
      err = i2c_attach_client(new_client);
      if (err) goto exit_free;
      tsc2003_init_client(new_client);
      err = tsc2003_driver_register(data);
      if (err < 0) goto exit_free;
     
      printk(KERN_INFO "%s: device address 0x%x attached.\n",
             __FUNCTION__, address);
      return 0;
    }
  /* failure to detect when everything else is ok isn't an error */
  else err = 0;
 exit_free:
  kfree(new_client);
 exit:
  return err;
}
static int __init tsc2003_init(void)
{
  return i2c_add_driver(&tsc2003_driver);
}
static void __exit tsc2003_exit(void)
{
  i2c_del_driver(&tsc2003_driver);
}
MODULE_AUTHOR("Bill Gatliff <[bgat at billgatliff.com](http://lists.lm-sensors.org/mailman/listinfo/lm-sensors)>");
MODULE_DESCRIPTION("TSC2003 Touch Screen Controller driver");
MODULE_LICENSE("GPL");
module_init(tsc2003_init);
module_exit(tsc2003_exit);
-- 
Bill Gatliff
[bgat at billgatliff.com](http://lists.lm-sensors.org/mailman/listinfo/lm-sensors)
- Previous message: [[lm-sensors] tsc2003 driver](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/014223.html)
- Next message: [[lm-sensors] [PATCH 00/25] reduce code in fs/compat_ioctl.c](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/014221.html)
- **Messages sorted by:**[[ date ]](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/date.html#14227)[[ thread ]](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/thread.html#14227)[[ subject ]](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/subject.html#14227)[[ author ]](http://lists.lm-sensors.org/pipermail/lm-sensors/2005-November/author.html#14227)
[More information about the lm-sensors mailing list](http://lists.lm-sensors.org/mailman/listinfo/lm-sensors)
