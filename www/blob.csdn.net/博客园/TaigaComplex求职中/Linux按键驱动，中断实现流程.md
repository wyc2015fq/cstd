# Linux按键驱动，中断实现流程 - TaigaComplex求职中 - 博客园







# [Linux按键驱动，中断实现流程](https://www.cnblogs.com/TaigaCon/archive/2012/11/03/2752771.html)





驱动代码：

```
1 #include <linux/module.h>
  2 #include <linux/kernel.h>
  3 #include <linux/fs.h>
  4 #include <linux/init.h>
  5 #include <linux/delay.h>
  6 #include <linux/device.h>
  7 #include <linux/irq.h>
  8 #include <linux/gpio.h>
  9 
 10 #include <linux/sched.h>
 11 
 12 #include <linux/interrupt.h>
 13 
 14 #include <asm/uaccess.h>
 15 #include <mach/irqs.h>
 16 #include <asm/io.h>
 17 #include <mach/regs-gpio.h>
 18 #include <mach/hardware.h>
 19 
 20 
 21 #define __IRQT_FALEDGE IRQ_TYPE_EDGE_FALLING
 22 #define __IRQT_RISEDGE IRQ_TYPE_EDGE_RISING
 23 #define __IRQT_LOWLVL IRQ_TYPE_LEVEL_LOW
 24 #define __IRQT_HIGHLVL IRQ_TYPE_LEVEL_HIGH
 25 #define IRQT_NOEDGE (0)
 26 #define IRQT_RISING (__IRQT_RISEDGE)
 27 #define IRQT_FALLING (__IRQT_FALEDGE)
 28 #define IRQT_BOTHEDGE (__IRQT_RISEDGE|__IRQT_FALEDGE)
 29 #define IRQT_LOW (__IRQT_LOWLVL)
 30 #define IRQT_HIGH (__IRQT_HIGHLVL)
 31 #define IRQT_PROBE IRQ_TYPE_PROBE
 32 
 33 
 34 volatile unsigned long *gpgcon;
 35 volatile unsigned long *gpgdat;
 36 
 37 static DECLARE_WAIT_QUEUE_HEAD(button_waitq);
 38 static volatile int ev_press=0;
 39 
 40 struct pin_desc{
 41     unsigned int pin;//设备号dev_id
 42     unsigned int key_val;//read返回值
 43 };
 44 
 45 struct pin_desc pins_desc[6]={  //6个按键
 46     {S3C2410_GPG(0),0x01},
 47     {S3C2410_GPG(3),0x02},
 48     {S3C2410_GPG(5),0x03},
 49     {S3C2410_GPG(6),0x04},
 50     {S3C2410_GPG(7),0x05},
 51     {S3C2410_GPG(11),0x06},
 52 };
 53 
 54 unsigned char keyval;
 55 
 56 static irqreturn_t buttons_irq(int irq,void *dev_id)
 57 {
 58     struct pin_desc *pindesc=(struct pin_desc *)dev_id;
 59     unsigned int pinval;
 60     pinval=s3c2410_gpio_getpin(pindesc->pin);
 61     if(pinval)
 62         {
 63             keyval=0x80|pindesc->key_val;
 64         }
 65         else
 66             {
 67                 keyval=pindesc->key_val;
 68             }
 69             ev_press=1;
 70             wake_up_interruptible(&button_waitq);
 71     return 0;
 72 }
 73 
 74 static int key_drv_open(struct inode *inode,struct file *file)
 75 {
 76     printk("key_drv_open\n");
 77     request_irq(IRQ_EINT8,buttons_irq,IRQT_BOTHEDGE,"S1",&pins_desc[0]);
 78     request_irq(IRQ_EINT11,buttons_irq,IRQT_BOTHEDGE,"S2",&pins_desc[1]);
 79     request_irq(IRQ_EINT13,buttons_irq,IRQT_BOTHEDGE,"S3",&pins_desc[2]);
 80     request_irq(IRQ_EINT14,buttons_irq,IRQT_BOTHEDGE,"S4",&pins_desc[3]);
 81     request_irq(IRQ_EINT15,buttons_irq,IRQT_BOTHEDGE,"S5",&pins_desc[4]);
 82     request_irq(IRQ_EINT19,buttons_irq,IRQT_BOTHEDGE,"S6",&pins_desc[5]);
 83     
 84     return 0;
 85 }
 86 
 87 ssize_t key_drv_read(struct file *file,char __user *buf,size_t size,loff_t *ppos)
 88 {    
 89     //unsigned char key_val;
 90     if(size!=1)
 91         {
 92             return -EINVAL;
 93         }
 94         
 95         wait_event_interruptible(button_waitq,ev_press);
 96         
 97     copy_to_user(buf,&keyval,1);
 98     
 99     ev_press=0;
100     
101     return sizeof(keyval);
102 }
103 
104 int key_drv_close(struct inode *inode,struct file *file)
105 {
106     free_irq(IRQ_EINT8,&pins_desc[0]);
107     free_irq(IRQ_EINT11,&pins_desc[1]);
108     free_irq(IRQ_EINT13,&pins_desc[2]);
109     free_irq(IRQ_EINT14,&pins_desc[3]);
110     free_irq(IRQ_EINT15,&pins_desc[4]);
111     free_irq(IRQ_EINT19,&pins_desc[5]);
112     return 0;
113 
114 }
115 
116 static struct file_operations key_drv_fops={
117     .owner=THIS_MODULE,
118     .open=key_drv_open,
119     .read=key_drv_read,
120     .release=key_drv_close,
121 };
122 
123 int major;
124 static struct class *key_drv_class;
125 static struct class_device *key_drv_device;
126 
127 static int key_drv_init(void)
128 {
129     major=register_chrdev(0,"key_drv",&key_drv_fops);
130     key_drv_class=class_create(THIS_MODULE,"key_drv");
131     key_drv_device=device_create(key_drv_class,NULL,MKDEV(major,0),NULL,"buttons");
132     
133     gpgcon = (volatile unsigned long *)ioremap(0x56000060,16);
134     gpgdat = gpgcon+1;
135     return 0;
136 }
137 
138 static int key_drv_exit(void)
139 {
140     device_unregister(key_drv_device);
141     class_destroy(key_drv_class);
142     unregister_chrdev(major,"key_drv");
143     iounmap(gpgcon);
144     return 0;
145 }
146 
147 module_init(key_drv_init);
148 module_exit(key_drv_exit);
149 
150 MODULE_LICENSE("GPL");
```



测试代码：

```
1 #include <sys/types.h>
 2 #include <sys/stat.h>
 3 #include <fcntl.h>
 4 #include <stdio.h>
 5 
 6 
 7 int main(int argc, char **argv)
 8 {
 9     int fd;
10     unsigned char val;
11     fd = open("/dev/buttons", O_RDWR);
12     if (fd < 0)
13     {
14         printf("cannot open/n");
15         return 1;
16     }
17     while(1)
18     {
19         read(fd,&val,1);
20         printf("key_val=0x%x\n",val);
21         }
22     
23     return 0;
24 }
```



流程如下：

![](https://pic002.cnblogs.com/images/2012/421096/2012110401444150.jpg)














