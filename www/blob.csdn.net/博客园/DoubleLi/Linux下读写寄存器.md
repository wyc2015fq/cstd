# Linux下读写寄存器 - DoubleLi - 博客园






arm裸机下读写寄存器很容易,各个寄存器和内存的地址是单一地址空间,他们是用相同的指令进行读写操作的.而在linux下就要复杂很多,因为linux支持多个体系架构的CPU。比如arm和x86就不一样，具体的差别我暂时也说不上来，这个涉及到CPU体系的设计。目前我只关心：linux为了支持多个硬件体系，在IO访问上做了自己的接口。可以通过IO内存和IO端口这两种方式进行IO访问。在LED的例子上给出这两种方式的具体实现：

1.利用IO Port的方式：

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h> /* printk() */
#include <linux/slab.h>  /* kmalloc() */
#include <linux/fs.h>  /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/ioport.h>

#include <mach/regs-gpio.h>
#include <asm/system.h>  /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_*_user */
#include <asm/io.h>

#define LED_NUM   4

struct led_dev
{
 struct cdev dev;
 unsigned port;
 unsigned long offset;
};

struct led_dev led[4];
dev_t dev = 0;
static struct resource *led_resource;


int led_open(struct inode *inode, struct file *filp)
{
 struct led_dev *led; /* device information */

 led = container_of(inode->i_cdev, struct led_dev, dev);
 filp->private_data = led; /* for other methods */

 return 0;          /* success */
}

int led_release(struct inode *inode, struct file *filp)
{
 return 0;
}

ssize_t led_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
 return 0;
}

ssize_t led_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
 char data;
 struct led_dev *led;
 u32 value;
 printk(KERN_INFO "debug by baikal: led dev write\n");

 led = (struct led_dev *)filp->private_data;
 copy_from_user(&data,buf,count);
 if(data == '0')
 { 
  printk(KERN_INFO "debug by baikal: led off\n"); 
  value = inl((unsigned)(S3C2410_GPBDAT));
  outl(value | 1<<led->offset,(unsigned)(S3C2410_GPBDAT)); 
  //value = ioread32(led->base);
  //iowrite32( value | 1<<led->offset, led->base);  
 }
 else
 {
  printk(KERN_INFO "debug by baikal: led on\n");
  value = inl((unsigned)(S3C2410_GPBDAT));
  outl(value & ~(1<<led->offset),(unsigned)(S3C2410_GPBDAT)); 
  //value = ioread32(led->base);
  //iowrite32( value & ~(1<<led->offset), led->base);
 }
}

struct file_operations led_fops = {
 .owner =    THIS_MODULE,
 .read =    led_read,
 .write =    led_write,
 //.ioctl =    led_ioctl,
 .open =    led_open,
 .release =  led_release,
};

static int led_init(void)
{ 
 int result, i;



 result = alloc_chrdev_region(&dev, 0, LED_NUM,"LED");
 if (result < 0) {
  printk(KERN_WARNING "LED: can't get major %d\n", MAJOR(dev));
  return result;
 }
 led_resource = request_region(0x56000014,0x4,"led");
 if(led_resource == NULL)
 {
  printk(KERN_ERR " Unable to register LED I/O addresses\n");
  return -1;
 }
 for(i = 0; i < LED_NUM; i++)
 {
  cdev_init( &led[i].dev, &led_fops);
  //led[i].port = ioport_map(0x56000014,0x4);
  //led[i].base = ioremap(0x56000014,0x4);
  led[i].offset = i + 5;  //leds  GPB5\6\7\8
  led[i].dev.owner = THIS_MODULE;
  led[i].dev.ops = &led_fops;
  result = cdev_add(&led[i].dev,MKDEV(MAJOR(dev),i),1);
  if(result < 0)
  {
   printk(KERN_ERR "LED: can't add led%d\n",i);
   return result;
  }
 }

 return 0;
}

static void led_exit(void)
{
 int i;
 release_region(0x56000014,0x4);
 for( i = 0; i < LED_NUM; i++)
 {
  //iounmap(led[i].base);

  cdev_del(&led[i].dev); 
 }
 unregister_chrdev_region(dev, LED_NUM);

}


module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR("Baikal");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple LED Driver");

2.利用IO Mem的方式：

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h> /* printk() */
#include <linux/slab.h>  /* kmalloc() */
#include <linux/fs.h>  /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/ioport.h>

#include <asm/system.h>  /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_*_user */
#include <asm/io.h>

#define LED_NUM   4

struct led_dev
{
 struct cdev dev;
 void __iomem *base;
 unsigned long offset;
};

struct led_dev led[4];
dev_t dev = 0;


int led_open(struct inode *inode, struct file *filp)
{
 struct led_dev *led; /* device information */

 led = container_of(inode->i_cdev, struct led_dev, dev);
 filp->private_data = led; /* for other methods */

 return 0;          /* success */
}

int led_release(struct inode *inode, struct file *filp)
{
 return 0;
}

ssize_t led_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
 return 0;
}

ssize_t led_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
 char data;
 struct led_dev *led;
 u32 value;
 printk(KERN_INFO "debug by baikal: led dev write\n");

 led = (struct led_dev *)filp->private_data;
 copy_from_user(&data,buf,count);
 if(data == '0')
 { 
  printk(KERN_INFO "debug by baikal: led off\n");  
  value = ioread32(led->base);
  iowrite32( value | 1<<led->offset, led->base);  
 }
 else
 {
  printk(KERN_INFO "debug by baikal: led on\n");
  value = ioread32(led->base);
  iowrite32( value & ~(1<<led->offset), led->base);
 }
}

struct file_operations led_fops = {
 .owner =    THIS_MODULE,
 .read =    led_read,
 .write =    led_write,
 //.ioctl =    led_ioctl,
 .open =    led_open,
 .release =  led_release,
};

static int led_init(void)
{ 
 int result, i;



 result = alloc_chrdev_region(&dev, 0, LED_NUM,"LED");
 if (result < 0) {
  printk(KERN_WARNING "LED: can't get major %d\n", MAJOR(dev));
  return result;
 }

 for(i = 0; i < LED_NUM; i++)
 {
  cdev_init( &led[i].dev, &led_fops);
  request_mem_region(0x56000014,0x4,"led");
  led[i].base = ioremap(0x56000014,0x4);
  led[i].offset = i + 5;  //leds  GPB5\6\7\8
  led[i].dev.owner = THIS_MODULE;
  led[i].dev.ops = &led_fops;
  result = cdev_add(&led[i].dev,MKDEV(MAJOR(dev),i),1);
  if(result < 0)
  {
   printk(KERN_ERR "LED: can't add led%d\n",i);
   return result;
  }
 }

 return 0;
}

static void led_exit(void)
{
 int i;  
 release_mem_region(0x56000014,0x4);
 for( i = 0; i < LED_NUM; i++)
 {
  iounmap(led[i].base);

  cdev_del(&led[i].dev); 
 }
 unregister_chrdev_region(dev, LED_NUM);

}


module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR("Baikal");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple LED Driver");

目前，对于具体体系上的linux在移植过程中如何实现这两种方式的方法还不清楚，现在只是会用。等以后有机会了再慢慢理清楚。









