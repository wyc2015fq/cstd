# ic_card 驱动程序开发 - h13 - 博客园
　　IC卡又称集成电路卡，它是在大小和普通[信用卡](http://www.cnblogs.cc2/view/8153.htm)相同的塑料[卡片](http://www.cnblogs.cc2/view/724169.htm)上嵌置一个或多个集成电路构成的。集成电路芯片可以是存储器或向处理器。作为一种身份验证的工具，IC卡经常出现于各种电子设备中，作为一种身份的确认的工具。
　　下面以笔者参与的会议系统开发中的ICcard驱动程序为例，介绍linux系统下是如何进行IC card驱动程序开发的。
　　1、是ic_card.h文件：
```
#ifndef    IC_CARD
#define    IC_CARD
#include <mach/regs-gpio.h>
#include <linux/wait.h>
#include <linux/ioctl.h>
#define IC_CARD_MAJOR    231
#ifndef ICCARD_NR_DEVS
#define ICCARD_NR_DEVS 1    /*设备数*/
#endif
#define IC_RMM_CMD     0x30    //读主存命令字
#define IC_WMM_CMD     0x38    //写主存命令字
#define IC_VER_CMD     0x33    //校验密码
#define IC_RSM_CMD     0x31    //读密码存储区
#define IC_WSM_CMD     0x39    //写密码存储区
#define IC_RPM_CMD     0x34    //读保护存储区
#define IC_WPM_CMD     0x3c    //写保护存储区
#define IC_RDSTART_ADDR    (volatile unsigned char*)0X26
#define IC_WRSTART_ADDR    (volatile unsigned char*)0X26
#define IC_IOCTL_MAGIC    'i'
#define CFG_RD_COUNT    _IOW(IC_IOCTL_MAGIC,0,unsigned int)
#define CFG_WR_COUNT    _IOW(IC_IOCTL_MAGIC,1,unsigned int)
#define SETB_CARD_CLK()   __raw_writel(__raw_readl(S3C2410_GPGDAT)|(1<<0),S3C2410_GPGDAT)
#define CLR_CARD_CLK()    __raw_writel(__raw_readl(S3C2410_GPGDAT)&(~(1<<0)),S3C2410_GPGDAT)
#define SETB_CARD_RESRT() __raw_writel(__raw_readl(S3C2410_GPGDAT)|(1<<1),S3C2410_GPGDAT)
#define CLR_CARD_RESRT()  __raw_writel(__raw_readl(S3C2410_GPGDAT)&(~(1<<1)),S3C2410_GPGDAT)
#define SETB_CARD_IO()    __raw_writel(__raw_readl(S3C2410_GPGDAT)|(1<<3),S3C2410_GPGDAT)
#define CLR_CARD_IO()    __raw_writel(__raw_readl(S3C2410_GPGDAT)&(~(1<<3)),S3C2410_GPGDAT)
#define SETB_CARD_POWER()  __raw_writel(__raw_readl(S3C2410_GPGDAT)|(1<<7),S3C2410_GPGDAT)
#define CLR_CARD_POWER()  __raw_writel(__raw_readl(S3C2410_GPGDAT)&(~(1<<7)),S3C2410_GPGDAT)
wait_queue_head_t r_wait;            //读等待队列
wait_queue_head_t w_wait;            //写等待队列
typedef struct _ic_card_dev
{
    unsigned char* readbufstart;    //读入数据缓冲区首指针
//    char* readbufend;    //读入数据缓冲区末指针 
    unsigned char* writebufstart;    //写入数据缓冲区首指针
//    char* writebufend;    //写入数据缓冲区末指针
    int readcount;        //读入数据量
    int writecount;        //写入数据量
    char* readp;            //读人数据当前指针
    int readnum;            //已经读入量
    char* writep;            //当前写入数据指针
    int writenum;        //当前写入量
    int ic_state;            //卡当前状态，O为无卡，1为有卡
    int statechange;        //卡状态变化标志 
    int irqno;
    int r_press;
    int w_press;
}ic_card_dev;
#endif /*IC_CARD*/
```
　　2、实现模块的加载和卸载函数：
```
1 /*注册设备驱动程序*/
  2  static int ic_card_init(void)
  3 {
  4     int ret;
  5     dev_t devno;
  6     ICDEBUG(KERN_ALERT "init ic card\n");
  7     devno = MKDEV(ic_major, 0);    
  8     if(IC_CARD_MAJOR)
  9     {    
 10         ret = register_chrdev_region(devno, 1, DEV_NAME);
 11         ICDEBUG(KERN_ALERT "register_chrdev_region ret:%d\n",ret);
 12     }
 13     else
 14     {
 15         ret=alloc_chrdev_region(&devno, 0, 2, DEV_NAME);
 16         ic_major=MAJOR(devno);
 17         ICDEBUG(KERN_ALERT "alloc_chrdev_region ret:%d,major:%d\n",ret,ic_major);
 18     }
 19     if(ret<0)
 20     {
 21         ICDEBUG(KERN_ALERT "if ret<0 ret:%d\n",ret);
 22         return ret;
 23     }
 24 
 25     cdev_init(&cdev_ic, &ic_fops);
 26     cdev_ic.owner=THIS_MODULE;
 27     cdev_ic.ops= &ic_fops;
 28     
 29      //向系统注册该字符设备
 30      ret = cdev_add(&cdev_ic, devno, ICCARD_NR_DEVS);
 31     ICDEBUG(KERN_ALERT "cdev_add ret:%d\n",ret);
 32 
 33     if (ret)
 34     {
 35         ICDEBUG("iccard init error\r\n");
 36         unregister_chrdev_region(devno,1);
 37         goto failregion;
 38     } 
 39 
 40 
 41     ic_card_class = class_create(THIS_MODULE, DEV_NAME);
 42     if(IS_ERR(ic_card_class))
 43     {
 44         ICDEBUG("can not creat a ic_card_class class!\n");
 45         goto failcls;
 46     }
 47     device_create(ic_card_class, NULL, MKDEV(IC_CARD_MAJOR, 0), NULL, DEV_NAME);
 48 
 49     /*注册中断*/
 50     ret=ic_request_irqs();
 51     if(ret<0)
 52     {
 53         ICDEBUG(KERN_ALERT "ic_card_request_irqs:ret:%d\n",ret);
 54         goto faildev;
 55     }    
 56 
 57     /*初始化时钟*/
 58     init_timer(&ic_timer);
 59     ic_timer.function=ic_timer_handler;
 60 
 61     spin_lock_init(&lock);
 62 
 63  /*    ic_dev_info=kmalloc(ICCARD_NR_DEVS*sizeof(struct ic_card_dev),GFP_KERNEL);
 64     if(!ic_dev_info)
 65     {
 66         ret =  -ENOMEM; 
 67         goto fail;
 68     }
 69  */
 70     //初始化等待对列
 71       init_waitqueue_head(&r_wait);
 72      init_waitqueue_head(&w_wait);
 73     ICDEBUG("ic card init success!\r\n");
 74     
 75      return 0;
 76 faildev:
 77     ICDEBUG(KERN_ALERT "init ic card fail\n");
 78     device_destroy(ic_card_class, MKDEV(IC_CARD_MAJOR, 0));
 79 failcls:
 80     class_destroy(ic_card_class);
 81 failregion:
 82     unregister_chrdev_region(devno, 1);
 83     return ret;
 84     
 85 }
 86 
 87  /*注销设备驱动程序*/
 88  static void ic_card_exit(void)
 89 {
 90     /*释放中断*/
 91     ic_free_irqs();
 92 
 93     cdev_del(&cdev_ic);
 94 //    kfree(ic_dev_info);
 95     device_destroy(ic_card_class, MKDEV(IC_CARD_MAJOR, 0));
 96     class_destroy(ic_card_class);
 97     unregister_chrdev_region(MKDEV(ic_major,0), 1);
 98     ICDEBUG(KERN_ALERT "ic card unregister\n");
 99 
100 }
101 
102 module_init(ic_card_init);
103 module_exit(ic_card_exit);
```
首先，判断IC_CARD_MAJOR,即是否为设备指定了主设备号，如果IC_CARD_MAJOR不为0，则调用register_chrdev_region（）函数注册设备驱动程序。通过register_chrdev_region（）函数用于分配指定的设备编号范围。如果申请的设备编号范围跨越了主设备号，它会把分配范围内的编号按主设备号分割成较小的子范围，并在每个子范围上调用 __register_chrdev_region() 。如果其中有一次分配失败的话，那会把之前成功分配的都全部退回。
如果IC_CARD_MAJOR为0，则调用 alloc_chrdev_region（）动态分配主设备号给驱动程序。
 linux 2.6 内核采用struct cdev来记录字符设备的信息，通过cdev_init初始化一个静态分配的cdev对象，详细请参考[http://www.360doc.com/content/11/0228/12/6030028_96836815.shtml](http://www.360doc.com/content/11/0228/12/6030028_96836815.shtml)
调用cdev_add()函数向系统注册该字符设备. 
　　class_create()函数及device_create()函数使得module被加载时，udev daemon就会自动在/dev下创建IC_CARD设备文件。
　　然后申请中断，初始化定时器，定时器ic_timer用于消除插卡中的抖动问题，再调用init_waitqueue_head()初始化等待队列。
至此，ic_card驱动程序加载完成。
3、各IC_card操作函数：
```
static struct file_operations ic_fops={
    .open    =ic_open,
    .write    =ic_card_wrtie,
    .read    =ic_card_read,
    .poll    =ic_poll,
    .ioctl    =ic_ioctl,
    .release=ic_release,
    .fsync    =NULL,
};
```
具体实现如下：
```
/*调用ic_request_irqs注册中断处理程序
*配置引脚
*/
int ic_open (struct inode *inode, struct file *filp)
{
    int num;
    int ret=0;
    unsigned long flags;
    ICDEBUG(KERN_ALERT "open ic card\n");
    //获取次设备号
        num = MINOR(inode->i_rdev);
    ICDEBUG(KERN_ALERT "MINOR:%d\n",num);
    if (num >= ICCARD_NR_DEVS) 
            return -ENODEV;
    spin_lock_irqsave(&lock,flags);
    filp->private_data=&ic_dev_info[num];
//检测IC卡是否已插入设备
    s3c2410_gpio_cfgpin(S3C2410_GPG6,S3C2410_GPG6_INP);    //ICCARD_INTEREPT
    if((__raw_readw(S3C2410_GPGDAT)&(1<<6))==0<<6)
    {
        ic_dev_info[num].ic_state=1;    //代表卡已插入
        ic_dev_info[num].r_press=1;    //设备可读
        ic_dev_info[num].w_press=1;    //设备可写
        ICDEBUG(KERN_ALERT "has ic card device\n");
    }
    else
    {
        ic_dev_info[num].ic_state=0;    //代表卡已插入
        ic_dev_info[num].r_press=0;    //设备可读
        ic_dev_info[num].w_press=0;    //设备可写
        ICDEBUG(KERN_ALERT "no found ic card device\n");
    }
    spin_unlock_irqrestore(&lock,flags);
    //初始化输入输出引脚
    init_gpio();
    ICDEBUG(KERN_ALERT "open ic card success\n");
    return ret;
}
int ic_release(struct inode *inode, struct file *filp)
{
    //ic_free_irqs();
    return 0;
}
/*
*从ic卡中读出一个字节的数据
*/
int ic_card_read_byte(char *ch)
{
    int ret=0;
    int ii;
    for(ii = 8; ii>0; ii--)
    {
        CLR_CARD_CLK();
        udelay(5);
        *ch=(*ch)>>1;        //从低位读起 
        //if((__raw_readw(S3C2410_GPGDAT)&0x0008)==0x0008)
        if((__raw_readw(S3C2410_GPGDAT)&(1<<3))==1<<3)
        {
            *ch|=0x80;
            ICDEBUG(KERN_ALERT "<------------->\n");
        }
        else
        {
            ICDEBUG(KERN_ALERT "<|||||||||||||>\n");
        }
        udelay(5);
        SETB_CARD_CLK();
    }
    ICDEBUG(KERN_ALERT "ic_card_read_byte:ch is>>>>>(%c)\n",*ch);
    return ret;
}
int ic_card_write_byte(char *ch)
{
    int ret=0;
    int ii;
    ICDEBUG(KERN_ALERT "enter ic_card_write_byte,ch is %c\n",*ch);
    for(ii= 0; ii<8; ii++)
    {
        CLR_CARD_CLK();
        udelay(5);
        if(*ch&0x01)
        {
            ICDEBUG(KERN_ALERT "[+++++$++++++++]\n");
            SETB_CARD_IO();    //输入1
        }
        else
        {
            ICDEBUG(KERN_ALERT "[-----$--------]\n");
            CLR_CARD_IO();        //输入0
        }
        udelay(5);        
        SETB_CARD_CLK();
        udelay(10);
        *ch=*ch>>1;
    }
    return ret;
    
}
/*
*复位和复位应答
*/
static int ic_card_reset(void)
{
    unsigned char temp_ch;
    int temp_i = 4;
    s3c2410_gpio_cfgpin(S3C2410_GPG3,S3C2410_GPG3_OUTP);    //OUT
    udelay(5);
    CLR_CARD_RESRT();
    CLR_CARD_CLK();
    SETB_CARD_IO();
    udelay(5);
    SETB_CARD_RESRT();
    udelay(5);
    SETB_CARD_CLK();
    udelay(30);
    CLR_CARD_CLK();
    udelay(5);
    CLR_CARD_RESRT();
    s3c2410_gpio_cfgpin(S3C2410_GPG3,S3C2410_GPG3_INP);    //IN
    udelay(10);
    do{
        //temp_ch = ic_card_read_byte();
        ic_card_read_byte(&temp_ch);
        ICDEBUG(KERN_ALERT "ic_card_reset:temp_ch is(%c)\n",temp_ch);
    }while(--temp_i>0);
    CLR_CARD_CLK();
    s3c2410_gpio_cfgpin(S3C2410_GPG3,S3C2410_GPG3_OUTP);    //IN
    udelay(5);
    SETB_CARD_IO();
    udelay(5);
    CLR_CARD_CLK();
    udelay(5);
    return 0;
}
/*
*命令开始条件
*/
void ic_card_start_cmd(void)
{
    s3c2410_gpio_cfgpin(S3C2410_GPG3,S3C2410_GPG3_OUTP);    //out
    CLR_CARD_CLK();
    SETB_CARD_IO();
    udelay(5);
    SETB_CARD_CLK();
    udelay(5);
    CLR_CARD_IO();
    udelay(5);
}
/*
*命令结束条件
*/
void ic_card_end_cmd(void)
{
    s3c2410_gpio_cfgpin(S3C2410_GPG3,S3C2410_GPG3_OUTP);    
    CLR_CARD_CLK();
    CLR_CARD_IO();
    udelay(5);
    SETB_CARD_CLK();
    udelay(5);
    SETB_CARD_IO();
    udelay(5);
}
/*发送命令*/
void ic_card_send_cmd(const char cmd,const char addr,const char count)
{
    ICDEBUG(KERN_ALERT "enter ic_card_read:cmd is %c,addr is %c,count is %c\n",cmd,addr,count);
    ic_card_start_cmd();
    ic_card_write_byte(&cmd);
    ic_card_write_byte(&addr);
    ic_card_write_byte(&count);
    ic_card_end_cmd();
}
void ic_card_breakoperator(void)
{
    s3c2410_gpio_cfgpin(S3C2410_GPG3,S3C2410_GPG3_OUTP);    
    CLR_CARD_CLK();
    CLR_CARD_RESRT();
    CLR_CARD_IO();
    udelay(5);
    SETB_CARD_RESRT();
    SETB_CARD_IO();
    udelay(5);
    CLR_CARD_RESRT();
    udelay(5);
}
/*从设备中读取指定长度的数据*/
ssize_t ic_dev_read(struct file *filep,const char *buf, size_t count, loff_t *offset)
{
    int ret=0;
// wait_event_interruptible();    
    //char *kbuf;
    int istate;
    int sizetmp;
    char* pbuf;
    //unsigned char ch=0;
    ic_card_dev* icdev=(ic_card_dev*)filep->private_data;
    ICDEBUG(KERN_ALERT "enter ic_dev_read\n");
    istate = icdev->ic_state;
    ICDEBUG(KERN_ALERT "ic_dev_istate:%d\n",istate);
    pbuf=buf;
    if(!istate)
    {
        return -ENODEV;
    }
    if(!pbuf)
    {
        return -1;
    }
    sizetmp=count;
    s3c2410_gpio_cfgpin(S3C2410_GPG3,S3C2410_GPG3_INP);    //将GPG3配置成输入引脚
    CLR_CARD_CLK();
    udelay(5);
    do{
        ic_card_read_byte(pbuf++);
        //*buf++ = ic_card_read_byte();
        ret++;
        ICDEBUG(KERN_ALERT "[ic_card_read:buf is (%c) ret:%d]\n",*(pbuf-1),ret);
    }while(--sizetmp);
    return ret;
    
}
/*往设备中写入数据*/
ssize_t ic_dev_wrtie(struct file *filep, char *buf, size_t count, loff_t *offset)
{
    int ret = 0;
    int istate;
    unsigned long flags;
    int sizetmp;
    ICDEBUG(KERN_ALERT "enter ic_dev_wrtie\n");
    spin_lock_irqsave(&lock,flags);
    istate = ((ic_card_dev*)(filep->private_data))->ic_state;
    spin_unlock_irqrestore(&lock,flags);
    if(!istate)
    {
        return -ENODEV;
    }
    if(!buf)
        return -1;
//    wait_event_interruptible();
    s3c2410_gpio_cfgpin(S3C2410_GPG3, S3C2410_GPG3_OUTP);//将GPG3配置成输出引脚
    sizetmp=count;
    do
    {
        ic_card_write_byte(buf++);    //按字节发送至ic卡
    }while(--sizetmp);
    return ret;
}
/*file_operations read*/
ssize_t ic_card_read(struct file *filep, char __user *buf, size_t count, loff_t *offset)
{
//int ii;
    int ret;
    char* kbuf;
    unsigned long flags;
    int r_press;
    ICDEBUG(KERN_ALERT "enter ic_card_read\n");
    spin_lock_irqsave(&lock,flags);
    r_press=((ic_card_dev*)filep->private_data)->r_press;
    spin_unlock_irqrestore(&lock,flags);
    ICDEBUG(KERN_ALERT "ic_card_read:r_press is:%d\n",r_press);
    
    wait_event_interruptible(r_wait, r_press!=0);
    kbuf=kmalloc(count,GFP_KERNEL);
    if(!kbuf)
    {
        return -ENOMEM;
    }
    spin_lock_irqsave(&lock,flags);
    ((ic_card_dev*)filep->private_data)->w_press=0;
    spin_unlock_irqrestore(&lock,flags);
    ic_card_reset();
    ic_card_send_cmd((const char)IC_RMM_CMD,(const char)((ic_card_dev*)(filep->private_data))->readbufstart,0xff);
    ret = ic_dev_read(filep, kbuf,  count,  offset);
    ICDEBUG(KERN_ALERT "[ic_card_read kbuf is:%s]\n",kbuf);
    if(ret>=0)
    {
        ret = copy_to_user(buf,kbuf,count);
    }
    ICDEBUG(KERN_ALERT "<ic_dev_read kbuf:(%s),buf:(%s)ret:%d;count:%d>\n",kbuf,buf,ret,count);
    ic_card_breakoperator();
    spin_lock_irqsave(&lock,flags);
    ((ic_card_dev*)filep->private_data)->w_press=1;
    spin_unlock_irqrestore(&lock,flags);
    kfree(kbuf);
    return ret;
}
/*file_operations write*/
ssize_t ic_card_wrtie(struct file *filep, char __user *buf, size_t count, loff_t *offset)
{
    int ret;
    char* kbuf;
    unsigned long flags;
    int w_press;
    ICDEBUG(KERN_ALERT "enter ic_card_wrtie\n");
    spin_lock_irqsave(&lock,flags);
    w_press=((ic_card_dev*)filep->private_data)->w_press;
    spin_unlock_irqrestore(&lock,flags);
    kbuf=kmalloc(count,GFP_KERNEL);
    if(!kbuf)
    {
        return -ENOMEM;
    }
    wait_event_interruptible(r_wait, w_press!=0);
    //if(w_press)
    //{
    spin_lock_irqsave(&lock,flags);
    ((ic_card_dev*)filep->private_data)->r_press=0;
    spin_unlock_irqrestore(&lock,flags);
    ic_card_reset();
    ic_card_send_cmd((const char)IC_WMM_CMD,(const char)((ic_card_dev*)(filep->private_data))->writebufstart,count);
    if(copy_from_user(kbuf, buf, count))
    {
        kfree(kbuf);
        return -EFAULT;
    }
    ICDEBUG(KERN_ALERT "copy_from_user kbuf------------->:%s\n",kbuf);
    ret = ic_dev_wrtie(filep, kbuf, count, offset);
    printk(KERN_ALERT "ic_dev_wrtie ret : %d\n",ret);
    ic_card_breakoperator();
    spin_lock_irqsave(&lock,flags);
    ((ic_card_dev*)filep->private_data)->w_press=1;
    spin_unlock_irqrestore(&lock,flags);
    kfree(kbuf);
    ICDEBUG(KERN_ALERT "ic_card_wrtie success\n");
    return ret;
}
int ic_ioctl(struct inode *inode, struct file *filep, unsigned int cmd, unsigned long arg)
{
    unsigned int icount;
    unsigned long flags;
    spin_lock_irqsave(&lock,flags);
    switch(cmd)
    {
        case CFG_RD_COUNT:
            icount = (unsigned int)arg;
            ((ic_card_dev*)filep->private_data)->readcount=icount;
            break;
        case CFG_WR_COUNT:
            icount=(unsigned int)arg;
            ((ic_card_dev*)filep->private_data)->writecount=icount;
            break;
        default:
            break;
    }
    spin_unlock_irqrestore(&lock,flags);
    return 0;
}
/*
#define POLLIN   0x0001  //有数据可以读入,read不会阻塞,注意:select的请情况下,即使到EOF也是ready的.
#define POLLPRI  0x0002 //紧急数据,比如TCP,或者packet模式的peseudo-terminal发现slave的状态有变化.
#define POLLOUT  0x0004 //写入不会阻塞.
#define POLLERR  0x0008 //输出出错
#define POLLHUP  0x0010 //Hang up (output only).
#define POLLNVAL  0x0020 //Invalid request: fd not open (output only).
The rest seem to be more-or-less nonstandard. Check them! 
#define POLLRDNORM 0x0040 //POLLIN.
#define POLLRDBAND 0x0080 //高优先级的数据read for read (generally unused on Linux).
#define POLLWRNORM 0x0100 //Equivalent to POLLOUT.
#define POLLWRBAND 0x0200 //Priority data may be written.
#define POLLMSG  0x0400
#define POLLREMOVE 0x1000
*/
unsigned int ic_poll (struct file *filep, struct poll_table_struct *wait)
{
    int ic_num=0;//test
    unsigned int mask = 0;
    unsigned long flags;
    //加入这句话是为了在读写状态发生变化的时候，通知核心层，让核心层重新调用poll函数查询信息。
也就是说这两句只会在select阻塞的时候用到
    //当利用select函数发现既不能读又不能写时，select函数会阻塞，但是此时的阻塞并不是轮询，
而是睡眠，通过下面两个队列发生变化时通知select
    poll_wait(filep, &r_wait, wait);
    poll_wait(filep, &w_wait, wait);
    spin_lock_irqsave(&lock,flags);
    if(ic_dev_info[ic_num].r_press != 0)
        mask |= POLLIN | POLLRDNORM;    //可读，同时写上POLLRDNORM
    if(ic_dev_info[ic_num].w_press !=0)
        mask |= POLLOUT | POLLWRNORM;
    spin_unlock_irqrestore(&lock,flags); 
     return mask;
}
```
```
//#define ICDEBUG
#ifdef     ICDEBUG
    #ifdef __KERNEL__
        #define    ICDEBUG(fmt,args...)    printk(KERN_ALERT "ic card:"fmt,##args)
    #else
        #define ICDEBUG(fmt,args...)    fprintf(stderr,fmt,##args)
    #endif
#else
    #define ICDEBUG(fmt,args...)
#endif
#define DEV_NAME    "ic_card"
#define IC_TIMER_DELAY    (HZ/50)
#define ICCARD_INT_NAME  "ic_irq_handler"
static int ic_major=IC_CARD_MAJOR;
static struct class* ic_card_class;
/*linux 内核定时器
struct timer_list { 
struct list_head list; 
unsigned long expires; 
unsigned long data; 
void (*function)(unsigned long); 
};
*/
static struct timer_list ic_timer;
/*自旋锁基本API:
头文件: <linux/spinlock.h> 
数据类型: spinlock_t 
初始化, 以下两种方法: 
静态: spinlock_t my_lock = SPIN_LOCK_UNLOCKED; 
动态: void spin_lock_init(spinlock_t *lock); 
获得锁: void spin_lock(spinlock_t *lock); 
释放锁: void spin_unlock(spinlock_t *lock); 
*/
spinlock_t lock;
/*
内核中每个字符设备都对应一个 cdev 结构的变量，下面是它的定义：
struct cdev {
   struct kobject kobj;          // 每个 cdev 都是一个 kobject
   struct module *owner;       // 指向实现驱动的模块
   const struct file_operations *ops;   // 操纵这个字符设备文件的方法
   struct list_head list;       // 与 cdev 对应的字符设备文件的 inode->i_devices 的链表头
   dev_t dev;                   // 起始设备编号
   unsigned int count;       // 设备范围号大小
}; 
一个 cdev 一般它有两种定义初始化方式：静态的和动态的。
静态内存定义初始化：
struct cdev my_cdev;
cdev_init(&my_cdev, &fops);
my_cdev.owner = THIS_MODULE;
动态内存定义初始化：
struct cdev *my_cdev = cdev_alloc();
my_cdev->ops = &fops;
my_cdev->owner = THIS_MODULE;
两种使用方式的功能是一样的，只是使用的内存区不一样，一般视实际的数据结构需求而定。*/
struct cdev cdev_ic;
/*IC_CARD设备结构体*/
static ic_card_dev ic_dev_info[ICCARD_NR_DEVS]=
{
    {
        IC_RDSTART_ADDR,    /*readbufstart*/
        IC_WRSTART_ADDR,    /*writebufstart*/
        0,                    /*readcount*/
        0,                    /*writecount*/
        NULL,                /*readp*/
        0,                    /*readnum*/
        NULL,                /*writep*/
        0,                    /*writenum*/
        
        0,                    /*ic_state*/
        0,                    /*statechange*/
        IRQ_EINT14,                    /*irqno*/
        0,                    /*r_press*/
        0,                    /*w_press*/
    },
};
/*将输入引脚配置成中断引脚*/
static void pin_input_to_int(void)
{
    __raw_writel((__raw_readl(S3C2410_GPGCON) & (~(0x03<<12))) | (0x02<<12),S3C2410_GPGCON);
    __raw_writel((__raw_readl(S3C2410_EXTINT1) & (~(0x07<<24))) |(0x06<<24),S3C2410_EXTINT1);
}
/*将中断引脚配置成输入引脚*/
static void pin_int_to_input(void)
{
    __raw_writel(__raw_readl(S3C2410_GPGCON)&(~(0x03<<12)),S3C2410_GPGCON);
}
/**中断服务程序，通知ic卡已插入*/
static irqreturn_t ic_interrupt_proc(int irq,void *dev_id)
{    
    unsigned long flags;
    spin_lock_irqsave(&lock,flags);
    if(ic_dev_info[0].statechange==0)
    {
        ic_dev_info[0].statechange=1;
        spin_unlock_irqrestore(&lock,flags);
    }
    else
    {
        spin_unlock_irqrestore(&lock,flags);
        return IRQ_HANDLED;
    }
    ICDEBUG(KERN_ALERT "+++++++++++++++++++++++++++++++\n");
    /*将中断引脚配置成为输入引脚，消除抖动*/
    pin_int_to_input();
    ic_timer.expires = jiffies + IC_TIMER_DELAY;        //delay 20 ms    
     //全局变量jiffies用来记录自系统启动以来产生的节拍的总数
    add_timer(&ic_timer);
    return IRQ_HANDLED;
}
/*定时器回调函数，当设定时间到来时，自动调用本程序*/
static void ic_timer_handler(unsigned long time)
{
    int ic_num=0;//test
    unsigned long flags;
    ICDEBUG(KERN_ALERT "enter ic_interrupt_proc\n");
    spin_lock_irqsave(&lock,flags);
    if(ic_dev_info[ic_num].ic_state==0)    
    {
        ic_dev_info[ic_num].ic_state=1;    //代表卡已插入
        ic_dev_info[ic_num].r_press=1;    //设备可读
        ic_dev_info[ic_num].w_press=1;    //设备可写
        wake_up_interruptible(&r_wait);
        wake_up_interruptible(&w_wait);
        ICDEBUG(KERN_ALERT "ic device enter,wake up interrupt ic_interrupt_proc\n");
    }
    else
    {
        ic_dev_info[ic_num].ic_state=0;    //代表卡已拔出
        ic_dev_info[ic_num].r_press=0;    //设备不可读
        ic_dev_info[ic_num].w_press=0;    //设备不可写
        ICDEBUG(KERN_ALERT "ic device leave,wake up interrupt ic_interrupt_proc\n");
    }
    del_timer(&ic_timer);
    /*重新配置中断引脚*/
    pin_input_to_int();
    ic_dev_info[0].statechange=0;
    spin_unlock_irqrestore(&lock,flags);
}
/*ic卡插入时和拔出时（下降沿/上升沿）产生的中断处理程序的注册*/
static int ic_request_irqs(void)
{
    int ret,i;
    for(i=0;i<ICCARD_NR_DEVS;i++)
    {
        ret = request_irq(ic_dev_info[i].irqno,ic_interrupt_proc,IRQ_TYPE_EDGE_BOTH,DEV_NAME,NULL);
        ICDEBUG(KERN_ALERT "interrupt:number:%d\n",ic_dev_info[i].irqno);
    }
    return ret;
}
static void ic_free_irqs(void)
{
    int i;
    for(i=0;i<ICCARD_NR_DEVS;i++)
        free_irq(ic_dev_info[i].irqno,NULL);
    return;
}
/*
*初始化通用输入/输出引脚
*/
static void init_gpio(void)
{
    s3c2410_gpio_cfgpin(S3C2410_GPG0,S3C2410_GPG0_OUTP);    //CLK
    s3c2410_gpio_cfgpin(S3C2410_GPG1,S3C2410_GPG1_OUTP);    //RST
    s3c2410_gpio_cfgpin(S3C2410_GPG3,S3C2410_GPG3_OUTP);    //IN
//    s3c2410_gpio_cfgpin(S3C2410_GPG5,S3C2410_GPG5_INP);        //OUT
    s3c2410_gpio_cfgpin(S3C2410_GPG6,S3C2410_GPG6_EINT14);    //ICCARD_INTEREPT
    s3c2410_gpio_cfgpin(S3C2410_GPG7,S3C2410_GPG7_OUTP);    //
//    s3c2410_gpio_cfgpin(S3C2410_GPG10,S3C2410_GPG10_OUTP);    //
//    s3c2410_gpio_cfgpin(S3C2410_GPG11,S3C2410_GPG11_OUTP);    //
    set_irq_type(IRQ_EINT14, IRQ_TYPE_EDGE_BOTH);//设置中断为下降沿触发
}
```
