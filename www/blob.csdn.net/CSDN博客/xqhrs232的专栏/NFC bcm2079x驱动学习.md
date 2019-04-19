# NFC bcm2079x驱动学习 - xqhrs232的专栏 - CSDN博客
2018年01月04日 11:30:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：96
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[http://blog.sina.com.cn/s/blog_89f592f501013ze4.html](http://blog.sina.com.cn/s/blog_89f592f501013ze4.html)
相关文章
1、BroadcomNFCChipBCM20795P1DataSheet----[https://wenku.baidu.com/view/15616af802768e9951e738f2.html](https://wenku.baidu.com/view/15616af802768e9951e738f2.html)
2、博通将NFC带入高性价比智能手机和可穿戴设备市场----[http://www.eepw.com.cn/article/234269.htm](http://www.eepw.com.cn/article/234269.htm)
1硬件介绍
Bcm2079x型号NFC开发版，与主机有5根pin脚，分别是两根IIC通信线、中断脚、使能脚、唤醒脚。
驱动需要完成IIC注册，中断的初始化，pin脚初始化。
### 2驱动初始化
#### 2.1初始化流程图
![NFC <wbr>bcm2079x驱动学习](http://s2.sinaimg.cn/mw690/89f592f5tccc4473eb891&690)
#### 2.2代码分析
在驱动代码中，需要静态初始化数据结构，代码如下：
static const unsigned short normal_i2c[] = {0x77, I2C_CLIENT_END};
static struct i2c_driver bcm2079x_driver = {
 .class = I2C_CLASS_HWMON,
 .id_table = bcm2079x_id,
 .probe = bcm2079x_probe,
 .remove = bcm2079x_remove,
 .driver = {
 .owner = THIS_MODULE,
 .name = "bcm2079x-i2c",
 },
 .address_list = normal_i2c,
};
完成probe、remove方法的映射，已经IIC地址的映射，bcm2079x的IIC地址是0x77。
内核加载驱动模块的时候，调用bcm2079x_dev_init()
#define PIO_BASE_ADDRESS  (0x01c20800) //映射的物理地址
#define PIO_RANGE_SIZE  (0x400) //映射的长度
static int __init bcm2079x_dev_init(void)
{
 int ret = 0;
 gpio_addr = ioremap(PIO_BASE_ADDRESS, PIO_RANGE_SIZE);//将一个IO地址空间映射到内核的虚拟地址空间上去，便于访问
 bcm2079x_driver.detect = bcm2079x_detect;
 return i2c_add_driver(&bcm2079x_driver);//注册
exit_ioremap_failed:
 return ret;
}
调用ioremap()将一个IO地址空间映射到内核的虚拟地址空间上去，便于访问，PIO_BASE_ADDRESS是要映射的物理地址，PIO_RANGE_SIZE是要映射的长度。
映射detect方法，在模块加载时，进行硬件检测，判断硬件是否存在，满足条件之后，才会注册i2c设备相关信息，创建i2c-client，并注册i2c
 driver，执行probe操作;看看该方法吧：
int bcm2079x_detect(struct i2c_client *client, struct i2c_board_info *info)
{
 struct i2c_adapter *adapter = client->adapter;
if(twi_id == adapter->nr)//硬件存在
 {
 strlcpy(info->type, BCM2079X_NAME, I2C_NAME_SIZE);
 info->platform_data = &bcm2079x_pdata;
 return 0;
 }else{
 }
}
满足条件之后，就调用bcm2079x_driver.probe()方法了，映射到bcm2079x_probe()中，在这里面完成初始化工作。代码如下：
static int bcm2079x_probe(struct i2c_client *client,
 const struct i2c_device_id *id)
{
 struct bcm2079x_platform_data *platform_data;
 struct bcm2079x_dev *bcm2079x_dev;
 platform_data = client->dev.platform_data;
 ……
platform_data->irq_gpio = gpio_request_ex("bcm2079x_para",
 "bcm2079x_int_port");//获取中断引脚的句柄，是在我们平台上特有的，山寨的做法…..
 ……
//中断初始化，又是一个不标准的山寨做法….
 printk(" INTERRUPT CONFIG\n");
 reg_num = ext_int_num%8;
 reg_addr = ext_int_num/8;
 reg_val = readl(gpio_addr + int_cfg_addr[reg_addr]);//读
 reg_val &= (~(7 << (reg_num * 4)));
 reg_val |= (int_mode << (reg_num * 4));
writel(reg_val,gpio_addr+int_cfg_addr[reg_addr]);
 bcm2079x_clear_penirq();
 reg_val = readl(gpio_addr+PIO_INT_CTRL_OFFSET);
 reg_val |= (1 << ext_int_num);
writel(reg_val,gpio_addr+PIO_INT_CTRL_OFFSET);//写
 //end
//  ret = gpio_request(platform_data->irq_gpio, "nfc_int");这代码备注是掉了，这是标准的？
platform_data->en_gpio = gpio_request_ex("bcm2079x_para", "bcm2079x_en_port");//获取使能引脚的句柄，继续山寨
//  ret = gpio_request(platform_data->en_gpio, "nfc_ven");
platform_data->wake_gpio = gpio_request_ex("bcm2079x_para",
 "bcm2079x_wakeup");//获取唤醒引脚的句柄，山寨
//  ret = gpio_request(platform_data->wake_gpio, "nfc_firm");
 //山寨！！！！使唤醒和使能引脚作为输出口
gpio_write_one_pin_value(platform_data->en_gpio, 0, "bcm2079x_en_port");
gpio_write_one_pin_value(platform_data->wake_gpio, 0,
 "bcm2079x_wakeup");
//  gpio_set_value(platform_data->en_gpio, 0);
//  gpio_set_value(platform_data->wake_gpio, 0);
 bcm2079x_dev = kzalloc(sizeof(*bcm2079x_dev), GFP_KERNEL);//分配内存
 bcm2079x_dev->wake_gpio = platform_data->wake_gpio;//初始化工作
 bcm2079x_dev->irq_gpio = platform_data->irq_gpio;
 bcm2079x_dev->en_gpio = platform_data->en_gpio;
 bcm2079x_dev->client = client;
 //初始化等待队列
init_waitqueue_head(&bcm2079x_dev->read_wq);
 mutex_init(&bcm2079x_dev->read_mutex);
 spin_lock_init(&bcm2079x_dev->irq_enabled_lock);
 //注册驱动
 bcm2079x_dev->bcm2079x_device.minor = MISC_DYNAMIC_MINOR;
 bcm2079x_dev->bcm2079x_device.name = "bcm2079x";
 bcm2079x_dev->bcm2079x_device.fops = &bcm2079x_dev_fops;
 ret = misc_register(&bcm2079x_dev->bcm2079x_device);
 //申请中断，中断号是28
 client->irq = 28;
 ret = request_irq(client->irq, bcm2079x_dev_irq_handler,
 IRQF_TRIGGER_RISING|IRQF_NO_SUSPEND|IRQF_SHARED,
 client->name, bcm2079x_dev);
 bcm2079x_disable_irq(bcm2079x_dev);//先不对中断使能
 i2c_set_clientdata(client, bcm2079x_dev);
 return 0;
}
在我们的平台上，pgio和中断的使用都不标准，非常苦恼，这就是山寨的公司！！
在上面代码中，调用gpio_request_ex()获取引脚的句柄，其实是去解析一个配置文件里面的信息，在配置文件里面定义有IO，这是我们公司自己的一套，山寨！也学习一下吧！得到IO句柄之后，就可以对其进行初始化了，上面就完成了3个IO口的初始化，分别是中断、使能和唤醒。
为bcm2079x_dev数据结构分配内存空间，并初始化一些变量，其定义如下：
struct bcm2079x_dev {
 wait_queue_head_t read_wq;//等待队里
 struct mutex read_mutex;
 struct i2c_client *client;//
 struct miscdevice bcm2079x_device;//注册的micc设备
 unsigned int wake_gpio;//唤醒引脚的句柄
 unsigned int en_gpio;//使能引脚句柄
 unsigned int irq_gpio;//中断引脚句柄
 bool irq_enabled;//中断是否使能
 spinlock_t irq_enabled_lock;
 unsigned int error_write;
 unsigned int error_read;
 unsigned int count_read;
 unsigned int count_irq;
};
接着，要向内核注册该设备的驱动，调用misc_register(&bcm2079x_dev->bcm2079x_device)进行注册。前面也定义了主设备号、设备名称和file_operations结构体。
调用request_irq()完成中断的申请，这里的中断号是28，并注册了中断响应函数bcm2079x_dev_irq_handler，当有中断发生的时候调用它。在这里的初始化工作中，我们暂时不对中断使能。
到此，初始化工作就完成了，等待着用户空间的调用。
### 3用户空间调用
#### 3.1流程图
![NFC <wbr>bcm2079x驱动学习](http://s6.sinaimg.cn/mw690/89f592f5tccc448bea6c5&690)
#### 3.2代码分析
##### 3.2.1打开设备
用户空间通过系统调用open，打开驱动对应的设备节点，那么，将调用到驱动程序中的bcm2079x_dev_open()方法，代码如下：
static int bcm2079x_dev_open(struct inode *inode, struct file *filp)
{
 int ret = 0;
 //获取在probe()方法中分配内存空间的bcm2079x_dev的指针
 struct bcm2079x_dev *bcm2079x_dev = container_of(filp->private_data,
 struct
 bcm2079x_dev,
 bcm2079x_device);
 filp->private_data = bcm2079x_dev;
 bcm2079x_init_stat(bcm2079x_dev);
bcm2079x_enable_irq(bcm2079x_dev); //中断使能
 return ret;
}
在open方法中，主要是完成两件事情，第一是获取在probe()方法中分配内存空间的bcm2079x_dev的指针，并赋值给filp->private_data，第二是对中断的使能。
先看看container_of()方法,作用是根据一个结构体变量中的一个域成员变量的指针来获取指向整个结构体变量的指针。在这里已知的filp->private_data是指向bcm2079x_dev结构体的域成员变量bcm2079x_device的指针。调用container_of()后得到bcm2079x_dev的指针，然后再赋值给filp->private_data.
接下来就对中断使能了，代码如下：
static void bcm2079x_enable_irq(struct bcm2079x_dev *bcm2079x_dev)
{
 unsigned long flags;
 spin_lock_irqsave(&bcm2079x_dev->irq_enabled_lock, flags);
 if (!bcm2079x_dev->irq_enabled) {
 bcm2079x_dev->irq_enabled = true;
enable_irq(bcm2079x_dev->client->irq);//对28号中断使能
 }
 spin_unlock_irqrestore(&bcm2079x_dev->irq_enabled_lock, flags);
}
##### 3.2.2ioctl使能唤醒
用户空间open()驱动节点之后，就获得了该驱动节点的文件描述符，然后就可以对其进行使能唤醒了。通过系统调用ioctl(),调用到驱动程序的bcm2079x_dev_unlocked_ioctl()方法，代码如下：
static long bcm2079x_dev_unlocked_ioctl(struct file *filp,
unsigned
 int cmd, unsigned long arg)
{
 struct bcm2079x_dev *bcm2079x_dev = filp->private_data;//在打开设备的时候对其赋值了
 switch (cmd) {
 case BCMNFC_POWER_CTL://使能
gpio_write_one_pin_value(bcm2079x_dev->en_gpio,
 arg, "bcm2079x_en_port");
 //gpio_set_value(bcm2079x_dev->en_gpio, arg);
 break;
 case BCMNFC_WAKE_CTL://唤醒
gpio_write_one_pin_value(bcm2079x_dev->wake_gpio,
 arg, "bcm2079x_wakeup");
 //gpio_set_value(bcm2079x_dev->wake_gpio, arg);
 break;
 }
 return 0;
}
gpio_write_one_pin_value()方法又看到了(山寨…)，调用它往IO口写高低电平，上面代码分别是给使能和唤醒口写高电平，完成使能唤醒。
##### 3.2.3等待中断
在用户空间的设计中，需要开一个新的线程循环读取数据，当没有数据的时候，线程将进入休眠，等待中断唤醒线程。系统调用的poll()方法在线程中调用，当没有数据的时候，将在poll()方法中休眠。对应的驱动程序方法是bcm2079x_dev_poll():
static unsigned int bcm2079x_dev_poll(struct file *filp, poll_table *wait)
{
 struct bcm2079x_dev *bcm2079x_dev = filp->private_data;
 unsigned int mask = 0;
 unsigned long flags;
poll_wait(filp, &bcm2079x_dev->read_wq, wait);//等待中断唤醒
 spin_lock_irqsave(&bcm2079x_dev->irq_enabled_lock, flags);
 if (bcm2079x_dev->count_irq > 0)
 {
 bcm2079x_dev->count_irq--;
 mask |= POLLIN | POLLRDNORM;
 }
 spin_unlock_irqrestore(&bcm2079x_dev->irq_enabled_lock, flags);
 return mask;
}
在用户空间线程循环中，当没有数据的时候，将在bcm2079x_dev_poll()调用中等待中断的唤醒，而在bcm2079x_dev_poll()方法中，将在中poll_wait()等待。poll_wait()函数，它的原型：
void poll_wait(struct file *filp, wait_queue_head_t *queue, poll_table *wait);
它的作用就是把当前进程添加到wait参数指定的等待列表(poll_table)中。等待唤醒。
##### 3.2.4响应中断，唤醒等待线程
我们在申请中断的时候，注册了一个响应中断的方法，申请中断的代码如下：
request_irq(client->irq, bcm2079x_dev_irq_handler,
 IRQF_TRIGGER_RISING|IRQF_NO_SUSPEND|IRQF_SHARED,
 client->name, bcm2079x_dev);
该方法原形如下：
int request_irq(unsigned int irq, void (*handler)(int irq,void dev_id,), 
unsigned long flags, const char *device, void *dev_id); 
参数irq表示所要申请的硬件中断号。handler为向系统登记的中断处理子程序，中
断产生时由系统来调用，调用时所带参数irq为中断号，dev_id为申请时告诉系统的设备
标识。device为设备名。flag是申请时的选项，它决定中断处理程序的一些特性，其中最重要的是中断处理程序是快速处理程序（flag里设置了SA_INTERRUPT）还是慢速处理程（不设SA _INTERRUPT），快速处理程序运行时，所有中断都被屏蔽，而慢速处理程序运行时，除了正在处理的中断外，其它中断都没有被屏蔽。在LINUX系统中，中断可以被不同的中断处理程序共享，这要求每一个共享此中断的处理程序在申请中断时在flags里设置SA_SH 
IRQ，这些处理程序之间以dev_id来区分。如果中断由某个处理程序独占，则dev_id可以
为NULL。request_irq返回0表示成功，返回-INVAL表示irq>15或handler==NULL，返回- 
EBUSY表示中断已经被占用且不能共享。
bcm2079x_dev_irq_handler方法为向系统登记的中断处理子程序，其代码如下：
static irqreturn_t bcm2079x_dev_irq_handler(int irq, void *dev_id)
{
 struct bcm2079x_dev *bcm2079x_dev = dev_id;
 unsigned long flags;
 int reg_val;
 int ret = -1;
 reg_val = readl(gpio_addr + PIO_INT_STAT_OFFSET);
 if(reg_val&(1<<(BCM2079X_IRQ_NO))){
 bcm2079x_clear_penirq();
 spin_lock_irqsave(&bcm2079x_dev->irq_enabled_lock, flags);
 bcm2079x_dev->count_irq++;
 spin_unlock_irqrestore(&bcm2079x_dev->irq_enabled_lock, flags);
wake_up(&bcm2079x_dev->read_wq);//唤醒等待线程
 return IRQ_HANDLED;
 }
 return ret;
}
在前面我们介绍3.2.3的时候，说到poll()方法中的阻塞方法poll_wait(filp,
 &bcm2079x_dev->read_wq, wait);其一直在等待bcm2079x_dev->read_wq信号，而在handler方法中，调用了wake_up()，就的跳出poll_wait()的等待，这样poll()方法就有了返回值，用户空间我认为有数据可以读了，于是就系统调用read()方法读取数据。
##### 3.2.5读数据
用户空间调用read()方法，读取设备节点的数据，最终调用到驱动程序的bcm2079x_dev_read()方法，代码如下：
static ssize_t bcm2079x_dev_read(struct file *filp, char __user *buf,
 size_t
 count, loff_t *offset)
{
 struct bcm2079x_dev *bcm2079x_dev = filp->private_data;
 unsigned char tmp[MAX_BUFFER_SIZE];
 int total, len, ret;
 total = 0;
 len = 0;
 bcm2079x_dev->count_read++;
 if (count > MAX_BUFFER_SIZE)
 count = MAX_BUFFER_SIZE;
 mutex_lock(&bcm2079x_dev->read_mutex);
 ret = i2c_master_recv(bcm2079x_dev->client, tmp, 4);//通过IIC读取数据
 if (ret == 4) {
 total = ret;
 switch(tmp[0]) {
 case PACKET_TYPE_NCI:
 len = tmp[PACKET_HEADER_SIZE_NCI-1];
 break;
 case PACKET_TYPE_HCIEV:
 len = tmp[PACKET_HEADER_SIZE_HCI-1];
 if (len == 0)
 total--; 
 else
 len--; 
 break;
 default:
 len = 0; 
 break;
 }
 if (len > 0 && (len + total) <= count) {
 ret = i2c_master_recv(bcm2079x_dev->client, tmp+total,
 len);
 if (ret == len)
 total += len;
 }
 }
 mutex_unlock(&bcm2079x_dev->read_mutex);
 if (total > count || copy_to_user(buf, tmp, total)) {//拷贝数据到用户空间
 bcm2079x_dev->error_read++;
 }
 return total;
}
看起来简单吧！

参考：
container_of：[http://www.cnblogs.com/sdphome/archive/2011/09/14/2176624.html](http://www.cnblogs.com/sdphome/archive/2011/09/14/2176624.html)
poll_wart():[http://blog.chinaunix.net/uid-26851094-id-3175940.html](http://blog.chinaunix.net/uid-26851094-id-3175940.html)

