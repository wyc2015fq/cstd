# linux下的i2c驱动框架 - TaigaComplex求职中 - 博客园







# [linux下的i2c驱动框架](https://www.cnblogs.com/TaigaCon/archive/2012/11/23/2785169.html)





本驱动基于linux2.6.32.2

i2c遵循的也是总线设备驱动框架,具体流程如下:

**当加入驱动(driver)时:**

**i2c_add_driver　　　　　　　　//进行驱动注册**

**　　　　 i2c_register_driver(THIS_MODULE, driver);**

**　　　　　　　　　　driver_register(&driver->driver);　　　　　　//把驱动加入总线驱动链表**

**　　　　　　　　　　bus_for_each_dev(&i2c_bus_type, NULL, driver, __attach_adapter);　//对于总线上设备链表的每一个设备进行遍历**

**　　　　　　　　　　　　　　　　__attach_adapter　　　　　　　　　　　　　　//对每一次遍历调用__attach_adapter函数**

**　　　　　　　　　　　　　　　　　　　　　　to_i2c_adapter(dev);　　　　　　//寻找出该设备对应的适配器**

**　　　　　　　　　　　　　　　　　　　　　　i2c_detect(adapter, driver);　　　　//对每个适配器进行通信检测,检查是否存在该适配器**

**　　　　　　　　　　　　　　　　　　　　　　driver->attach_adapter(adapter);　　//调用驱动的attach_adapter函数(该函数需要自己设定,当然,做什么也是自己决定)**



**当加入适配器(adapter)时:**

**i2c_add_adapter　　　　　　//进行适配器注册**

**　　　　i2c_register_adapter　　　　**

**　　　　　　　　　device_register(&adap->dev);　　　　//对适配器内的device进行注册**

**　　　　　　　　　　　　　　device_add(dev);　　　　　　//device加入链表**

**　　　　　　　　　bus_for_each_drv(&i2c_bus_type, NULL, adap,i2c_do_add_adapter);　//对于总线上驱动链表的每一个驱动进行遍历**

**　　　　　　　　　　　　　　i2c_do_add_adapter　　　　　　　　//对于每一次遍历调用该函数**

**　　　　　　　　　　　　　　　　　　　　i2c_detect(adapter, driver);　　　　//对每个适配器进行通信检测,检查是否存在该适配器**

**　　　　　　　　　　　　　　　　　　　　driver->attach_adapter(adapter);　　//调用驱动的attach_adapter函数(该函数需要自己设定,当然,做什么也是自己决定)**



总线设备驱动框架遵从硬件设备跟驱动分离的原则

对于i2c设备,

　　　　device就是对应于i2c硬件设备,在device上加了一层封装,也就是adapter,其中adapter提供传输函数

　　　　driver就是i2c设备的驱动操作层,当device跟driver连接上之后,我们就可以在driver的内部函数**attach_adapter**函数内做进一步操作.





**i2c_client：**

i2c_client能用于连接adapter跟driver，起到中间节点的作用。

attach_adapter会提供本driver匹配的adapter结构体指针,我们可以通过构造一个i2c_client来接收该指针



本驱动提供的是字符设备操作,因此需要在attach_adapter内进行fops注册

由于在read/write等需要操作i2c硬件,因此用到函数i2c_transfer函数,该函数最终会调用到adap->algo->master_xfer(adap, msgs, num),

也就是adapter内定义的函数.该函数需要的参数有:相应的adapter,msg(其内部包含该次通信的源,目的,长度),和mun(msg的数目).

i2c_client结构体也用于存储这些传输所需参数。



驱动代码:

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

static struct i2c_client * at24cxx_client;
static int major;
static struct class *cls;
static struct class_device *clsdev;
static struct i2c_driver at24cxx_driver;

static unsigned short ignore[]      = { I2C_CLIENT_END };
static unsigned short normal_addr[] = { 0x50, I2C_CLIENT_END }; /* 地址值是7位 */
                                        /* 改为0x60的话, 由于不存在设备地址为0x60的设备, 所以at24cxx_detect不被调用 */

static unsigned short force_addr[] = {ANY_I2C_BUS, 0x60, I2C_CLIENT_END};
static unsigned short * forces[] = {force_addr, NULL};

static struct i2c_client_address_data addr_data = {
    .normal_i2c    = normal_addr,  /* 要发出S信号和设备地址并得到ACK信号,才能确定存在这个设备 */
    .probe        = ignore,
    .ignore        = ignore,
    //.forces     = forces, /* 强制认为存在这个设备 */
};

static ssize_t at24cxx_read(struct file *file, char __user *buf, size_t size, loff_t * offset)
{
    unsigned char address;
    unsigned char data;
    struct i2c_msg msg[2];
    int ret;
    
    /* address = buf[0] 
     * data    = buf[1]
     */
    if (size != 1)
        return -EINVAL;
    
    copy_from_user(&address, buf, 1);

    /* 数据传输三要素: 源,目的,长度 */

    /* 读AT24CXX时,要先把要读的存储空间的地址发给它 */
    msg[0].addr  = at24cxx_client->addr;  /* 目的 */
    msg[0].buf   = &address;              /* 源 */
    msg[0].len   = 1;                     /* 地址=1 byte */
    msg[0].flags = 0;                     /* 表示写 */

    /* 然后启动读操作 */
    msg[1].addr  = at24cxx_client->addr;  /* 源 */
    msg[1].buf   = &data;                 /* 目的 */
    msg[1].len   = 1;                     /* 数据=1 byte */
    msg[1].flags = I2C_M_RD;                     /* 表示读 */

    printk("adapter name:%s\n",at24cxx_client->adapter->name);
    if(at24cxx_client->adapter==NULL)
    printk("adapter not init\n");
    ret = i2c_transfer(at24cxx_client->adapter, msg, 2);
    printk("transfer_ret:%d\n",ret);
    if (ret == 2)
    {
        copy_to_user(buf, &data, 1);
        return 1;
    }
    else
        return -EIO;
}

static ssize_t at24cxx_write(struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    unsigned char val[2];
    struct i2c_msg msg[1];
    int ret;
    
    /* address = buf[0] 
     * data    = buf[1]
     */
    if (size != 2)
        return -EINVAL;
    
    copy_from_user(val, buf, 2);

    /* 数据传输三要素: 源,目的,长度 */
    msg[0].addr  = at24cxx_client->addr;  /* 目的 */
    msg[0].buf   = val;                   /* 源 */
    msg[0].len   = 2;                     /* 地址+数据=2 byte */
    msg[0].flags = 0;                     /* 表示写 */

    if(at24cxx_client->adapter==NULL)
    printk("adapter not init\n");
    printk("adapter name:%s\n",at24cxx_client->adapter->name);
    ret = i2c_transfer(at24cxx_client->adapter, msg, 1);
    printk("transfer_ret:%d\n",ret);
    if (ret == 1)
        return 2;
    else
        return -EIO;
}

static struct file_operations at24cxx_fops = {
    .owner = THIS_MODULE,
    .read  = at24cxx_read,
    .write = at24cxx_write,
};

static int at24cxx_detect(struct i2c_adapter *adapter, int address, int kind)
{    
    printk("at24cxx_detect\n");

    /* 构构一个i2c_client结构体: 以后收改数据时会用到它 */
    at24cxx_client = kzalloc(sizeof(struct i2c_client), GFP_KERNEL);
    at24cxx_client->addr    = address;
    at24cxx_client->adapter = adapter;
    if(adapter==NULL)
    printk("adapter not init/n");
    at24cxx_client->driver  = &at24cxx_driver;
    strcpy(at24cxx_client->name, "at24cxx");
//    i2c_detect_address(at24cxx_client,0,&at24cxx_driver);
    
    major = register_chrdev(0, "at24cxx", &at24cxx_fops);

    cls = class_create(THIS_MODULE, "at24cxx");
    clsdev=device_create(cls, NULL, MKDEV(major, 0), NULL, "at24cxx"); /* /dev/at24cxx */
    
    return 0;
}

static int at24cxx_attach(struct i2c_adapter *adapter)
{
    at24cxx_detect(adapter,addr_data.normal_i2c[0],0);
    return 0;//i2c_probe(adapter, &addr_data);
}

static int at24cxx_detach(struct i2c_adapter *adapter)
{
    printk("at24cxx_detach\n");
    device_unregister(clsdev);
    class_destroy(cls);
    unregister_chrdev(major, "at24cxx");

//    i2c_detach_client(client);
    kfree(i2c_get_clientdata(at24cxx_client));

    return 0;
}


/* 1. 分配一个i2c_driver结构体 */
/* 2. 设置i2c_driver结构体 */
static struct i2c_driver at24cxx_driver = {
    .driver = {
        .name    = "at24cxx",
    },
    .attach_adapter = at24cxx_attach,
    .detach_adapter  = at24cxx_detach,
    .address_data        =    &addr_data,
};



static int at24cxx_init(void)
{
    i2c_add_driver(&at24cxx_driver);
    return 0;
}

static void at24cxx_exit(void)
{
    i2c_del_driver(&at24cxx_driver);
}

module_init(at24cxx_init);
module_exit(at24cxx_exit);

MODULE_LICENSE("GPL");
```



测试代码:

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/* i2c_test r addr
 * i2c_test w addr val
 */

void print_usage(char *file)
{
    printf("%s r addr\n", file);
    printf("%s w addr val\n", file);
}

int main(int argc, char **argv)
{
    int fd;
    unsigned char buf[2];
    
    if ((argc != 3) && (argc != 4))
    {
        print_usage(argv[0]);
        return -1;
    }

    fd = open("/dev/at24cxx", O_RDWR);
    if (fd < 0)
    {
        printf("can't open /dev/at24cxx\n");
        return -1;
    }

    if (strcmp(argv[1], "r") == 0)
    {
        buf[0] = strtoul(argv[2], NULL, 0);
        read(fd, buf, 1);
        printf("data: %c, %d, 0x%2x\n", buf[0], buf[0], buf[0]);
    }
    else if (strcmp(argv[1], "w") == 0)
    {
        buf[0] = strtoul(argv[2], NULL, 0);
        buf[1] = strtoul(argv[3], NULL, 0);
        write(fd, buf, 2);
    }
    else
    {
        print_usage(argv[0]);
        return -1;
    }
    
    return 0;
}
```












