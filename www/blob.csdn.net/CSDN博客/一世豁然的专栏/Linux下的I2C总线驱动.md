# Linux下的I2C总线驱动 - 一世豁然的专栏 - CSDN博客





2015年03月29日 18:42:52[一世豁然](https://me.csdn.net/Explorer_day)阅读数：634








本博客转载于：[http://my.csdn.net/weiqing1981127](http://my.csdn.net/weiqing1981127)  【原创作者：南京邮电大学  通信与信息系统专业 研二 魏清】







一．系统理论

1．I2C驱动体系概述

Linux I2C驱动体系结构主要由3部分组成，即I2C核心、I2C总线驱动和I2C设备驱动。I2C核心是I2C总线驱动和I2C设备驱动的中间枢纽，它以通用的、与平台无关的接口实现了I2C中设备与适配器的沟通。I2C总线驱动填充i2c_adapter和i2c_algorithm结构体。I2C设备驱动填充i2c_driver和i2c_client结构体。

2．驱动工程师需要做的事

2.1总线层：根据核心板的芯片手册，编写总线层驱动，例如根据S3C2440中文手册中I2C总线接口一章实现总线层驱动的编写。总线层主要向内核注册一个适配器，并填充适配器的支持类型和方法，内核加载时，会生成一个总线驱动模块。

2.2设备层：设备驱动层主要是针对不同的I2C硬件设备编写驱动程序并为用户提供接口，内核加载时，会生成一个设备驱动模块。I2C子系统下设备驱动的编写有两种模式：一种是用户模式设备驱动这种驱动依赖I2C子系统中的i2c-dev这个驱动，我们需要在应用程序去封装数据，这需要应用程序的开发人员具备相当的硬件基础，另外一种是普通的设备驱动。

3．
*****



二．内核代码

1.内核/drivers/i2c目录下文件分析

（1）  i2c-core.c

这个文件实现了I2C核心的功能以及/proc/bus/i2c*接口。

（2）i2c-dev.c

实现了I2C适配器设备文件的功能，每一个I2C适配器都被分配一个设备。通过适配器访问设备时的主设备号都为89，次设备号为0～255。应用程序通过“i2c-%d”
 (i2c-0, i2c-1, ..., i2c-10, ...)文件名并使用文件操作接口open()、write()、read()、ioctl()和close()等来访问这个设备。i2c-dev.c并没有针对特定的设备而设计，只是提供了通用的read()、write()和ioctl()等接口，应用层可以借用这些接口访问挂接在适配器上的I2C设备的存储空间或寄存器并控制I2C设备的工作方式。

（3）chips文件夹

这个目录中包含了一些特定的I2C设备驱动，如Dallas公司的DS1337实时钟芯片、EPSON公司的RTC8564实时钟芯片和I2C接口的EEPROM驱动等。

（4）busses文件夹

这个文件中包含了一些I2C总线的驱动，如S3C2410的I2C控制器驱动为i2c-s3c2410.c

（5）algos文件夹

实现了一些I2C总线适配器的algorithm。



2.I2C核心

（1）增加/删除i2c_adapter

int i2c_add_adapter(struct i2c_adapter *adap);

int i2c_del_adapter(struct i2c_adapter *adap);

（2）增加/删除i2c_driver

int i2c_register_driver(struct module *owner, struct i2c_driver *driver);

int i2c_del_driver(struct i2c_driver *driver);

inline int i2c_add_driver(struct i2c_driver *driver);
（3）i2c_client依附/脱离
int i2c_attach_client(struct i2c_client *client);

int i2c_detach_client(struct i2c_client *client);
（4）i2c传输、发送和接收
int i2c_transfer(struct i2c_adapter * adap, struct i2c_msg *msgs, int num);

int i2c_master_send(struct i2c_client *client,const char *buf ,int count);

int i2c_master_recv(struct i2c_client *client, char *buf ,int count);


3.I2C总线驱动

我们根据S3C2440核心板I2C总线接口手册编写I2C总线驱动。

3.1硬件部分

在此只提供mini2440的I2C接口通信协议，S3C2440的I2C控制器主要由4个寄存器完成所有的I2C操作的，这4个寄存器是IICON、IICSTAT、IICADD、IICCDS。（请参见Mini2440手册）

3.2软件部分

首先我们要明白总线层驱动编写好是放在/drivers/i2c/buses目录下的。那下面让我们一起分析下I2c_s3c2410.c这个总线驱动吧。

我们在前面说过，I2C总线驱动层主要是填充i2c_adapter和i2c_algorithm结构体，那我们先来看看这两个结构体到底是啥玩意儿把！

struct i2c_adapter {

struct module *owner;

unsigned int id;

unsigned int class;       


const struct i2c_algorithm *algo;  //指向适配器的驱动程序

void *algo_data;    //指向适配器的私有数据

u8 level;              


struct mutex bus_lock;

int timeout;     //超时时间设置

int retries;    //重试次数

struct device dev;        


int nr;      //IDR机制中ID号，有时也作次设备号。

char name[48];

struct completion dev_released;

};



struct i2c_algorithm {

int (*master_xfer)(struct i2c_adapter *adap, struct i2c_msg *msgs,

                  int num);  //指向 I2C总线通信协议的函数

int (*smbus_xfer) (struct i2c_adapter *adap, u16 addr,

                  unsigned short flags, char read_write,

                  u8 command, int size, union i2c_smbus_data *data);

            //实现SMBUS总线通信协议的函数，一般置为NULL

u32 (*functionality) (struct i2c_adapter *); //确定适配器支持的类型

};



对于特定类型的适配器，我们需要在i2c_adapter的基础上进行扩充，S3C2440对应的适配器结构体如下。

struct s3c24xx_i2c {

spinlock_t              lock;

wait_queue_head_t wait;

unsigned int           suspended:1;

struct i2c_msg              *msg;   //适配器到设备真正传输数据的结构体

unsigned int           msg_num;  //消息的个数

unsigned int           msg_idx;  //完成了几个消息

unsigned int           msg_ptr;  //指向当前传输的下一个字节，即在i2c_msg.buf中的

                                         //偏移位置

unsigned int           tx_setup;  //写一个寄存器的时间，这里为50ms

unsigned int           irq;

enum s3c24xx_i2c_state       state;  //适配器的状态，包括空闲、开始、读、写、停止

unsigned long         clkrate;

void __iomem        *regs;   //I2C     
设备寄存器地址

struct clk        *clk;

struct device          *dev;

struct resource              *ioarea;  //指向适配器使用的资源

struct i2c_adapter   adap;   //适配器的主体结构体

#ifdef CONFIG_CPU_FREQ

struct notifier_block      freq_transition;

#endif

};



struct i2c_msg {

__u16 addr;    //从机的地址

__u16 flags;  //消息类型标志

__u16 len;      //消息字节长度

__u8 *buf;      //指向消息数据的缓冲区

};



前面说过，编写I2C总线驱动层主要是填充i2c_adapter和i2c_algorithm结构体，那么可以开始了，让我们先来填充i2c_algorithm结构体吧，代码如下

static const struct i2c_algorithm s3c24xx_i2c_algorithm = {

.master_xfer          = s3c24xx_i2c_xfer,

.functionality          = s3c24xx_i2c_func,

};



我们先看i2c_algorithm中的master_xfer成员，刚才说过，s3c24xx_i2c_xfer是用来确定适配器支持的类型，用于返回总线支持的协议，具体到代码如下

static u32 s3c24xx_i2c_func(struct i2c_adapter *adap)

{

return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL | I2C_FUNC_PROTOCOL_MANGLING;

}



好了，接下来我们把重点放在i2c_algorithm中的functionality成员上，s3c24xx_i2c_func函数用于实现I2C通信协议，将i2c_msg消息传给I2C设备。

static int s3c24xx_i2c_xfer(struct i2c_adapter *adap,

               struct i2c_msg *msgs, int num)

{

struct s3c24xx_i2c *i2c = (struct s3c24xx_i2c *)adap->algo_data;

int retry;

int ret;

struct s3c2410_platform_i2c *pdata = i2c->dev->platform_data;

if (pdata->cfg_gpio)

        pdata->cfg_gpio(to_platform_device(i2c->dev));

for (retry = 0; retry < adap->retries; retry++) {

        ret = s3c24xx_i2c_doxfer(i2c, msgs, num);   //传输到I2C设备的具体函数

        if (ret != -EAGAIN)

               return ret;

        dev_dbg(i2c->dev, "Retrying transmission (%d)\n", retry);

        udelay(100);

}

return -EREMOTEIO;

}



我们可以发现s3c24xx_i2c_xfer其实主要就是调用s3c24xx_i2c_doxfer完成具体数据的传输任务。那我们接着看看s3c24xx_i2c_doxfer做了哪些事情？

static int s3c24xx_i2c_doxfer(struct s3c24xx_i2c *i2c,

                     struct i2c_msg *msgs, int num)

{

unsigned long timeout;  //传输超时

int ret;               //返回传输的消息个数

if (i2c->suspended)    //如果适配器处于挂起状态，则返回

        return -EIO;

ret = s3c24xx_i2c_set_master(i2c);  //将适配器设置为主机发送状态

if (ret != 0) {

        dev_err(i2c->dev, "cannot get bus (error %d)\n", ret);

        ret = -EAGAIN;

        goto out;

}

spin_lock_irq(&i2c->lock);

i2c->msg     = msgs;

i2c->msg_num = num;

i2c->msg_ptr = 0;

i2c->msg_idx = 0;

i2c->state   = STATE_START;   


s3c24xx_i2c_enable_irq(i2c);  //启动适配器的中断号，允许适配器发出中断

s3c24xx_i2c_message_start(i2c, msgs);  //启动适配器的消息传输

spin_unlock_irq(&i2c->lock);

timeout = wait_event_timeout(i2c->wait, i2c->msg_num == 0, HZ * 5);

//设置等待队列，直到i2c->msg_num == 0为真或5ms到来才被唤醒

ret = i2c->msg_idx;

if (timeout == 0)

        dev_dbg(i2c->dev, "timeout\n");

else if (ret != num)

        dev_dbg(i2c->dev, "incomplete xfer (%d)\n", ret);

msleep(1);

out:

return ret;

}

好了，让我们总结总结，我们可以发现s3c24xx_i2c_doxfer主要做了如下几件事：第一，将适配器设置为主机发送状态。第二，设置为中断传输方式。第三，发送启动信号，传输第一个字节。第四，等待超时或者其他函数在i2c->msg_num == 0时唤醒这里的等待队列。



到此为止我们会带来几个疑问：第一，s3c24xx_i2c_enable_irq和s3c24xx_i2c_message_start具体怎么实现的？第二，等待队列在何时被唤醒呢？



首先我们先来研究第一个问题，s3c24xx_i2c_enable_irq实现开中断如下

static inline void s3c24xx_i2c_enable_irq(struct s3c24xx_i2c *i2c)

{

unsigned long tmp;

tmp = readl(i2c->regs + S3C2410_IICCON);

writel(tmp | S3C2410_IICCON_IRQEN, i2c->regs + S3C2410_IICCON);

//将IICCON的D5位置1表示总线在接收或发送一个字节数据后会产生一个中断。

}



下面看看s3c24xx_i2c_message_start实现启动适配器的消息传输的实现吧。

static void s3c24xx_i2c_message_start(struct s3c24xx_i2c *i2c,

                            struct i2c_msg *msg)

{

unsigned int addr = (msg->addr & 0x7f) << 1;  //取从设备地址低7位，并前移1位

unsigned long stat;   //缓存IICSTAT

unsigned long iiccon;  //缓存IICCON

stat = 0;

stat |=  S3C2410_IICSTAT_TXRXEN; //使能发送接收功能，为写地址到IICDS

if (msg->flags & I2C_M_RD) {  //如果读，则主机接收，地址位D0=1

        stat |= S3C2410_IICSTAT_MASTER_RX;

        addr |= 1;

} else                        //如果写，则主机发送，地址位D0=0

        stat |= S3C2410_IICSTAT_MASTER_TX;

if (msg->flags & I2C_M_REV_DIR_ADDR)

        addr ^= 1;

s3c24xx_i2c_enable_ack(i2c);   //使能ACK响应信号

iiccon = readl(i2c->regs + S3C2410_IICCON);

writel(stat, i2c->regs + S3C2410_IICSTAT);

dev_dbg(i2c->dev, "START: %08lx to IICSTAT, %02x to DS\n", stat, addr);

writeb(addr, i2c->regs + S3C2410_IICDS);  //写地址到IICDS寄存器

ndelay(i2c->tx_setup);

dev_dbg(i2c->dev, "iiccon, %08lx\n", iiccon);

writel(iiccon, i2c->regs + S3C2410_IICCON);  //

stat |= S3C2410_IICSTAT_START;  //发送S信号，IICDS寄存器中数据自动发出

writel(stat, i2c->regs + S3C2410_IICSTAT);

}



总结下我们这个3c24xx_i2c_message_start函数吧，这个函数主要做了两件事，第一使能ACK信号。第二，将从机地址和读写方式控制字写入待IICDS中，由IICSTAT发送S信号，启动发送从机地址。

嗯，到现在为止我们已经把前面提出的第一个问题解决了，该轮到解决第二个问题了，s3c24xx_i2c_doxfer中的等待队列何时被唤醒呢？其实分析到现在我们发现s3c24xx_i2c_doxfer调用3c24xx_i2c_message_start只是发送了一个从机的地址。真正的数据传输在哪里呢？其实真正的数据传输我们放在了中断处理函数中实现的。当数据准备好发送时，将产生中断，并调用事先注册的中断处理函数s3c24xx_i2c_irq进行数据传输。中断的产生其实有3种情况，第一，总线仲裁失败。第二，当总线还处于空闲状态，因为一些错误操作等原因，意外进入了中断处理函数。第三，收发完一个字节的数据，或者当收发到的I2C设备地址信息吻合。行，那我们先来看看s3c24xx_i2c_irq到底怎么来传输数据的吧。



static irqreturn_t s3c24xx_i2c_irq(int irqno, void *dev_id)

{

struct s3c24xx_i2c *i2c = dev_id;

unsigned long status;

unsigned long tmp;

status = readl(i2c->regs + S3C2410_IICSTAT);

if (status & S3C2410_IICSTAT_ARBITR) { //仲裁失败下的处理

        dev_err(i2c->dev, "deal with arbitration loss\n");

}

//当总线为空闲状态，突然进入中断，我们需要清除中断信号，继续传输数据

if (i2c->state == STATE_IDLE) {

        dev_dbg(i2c->dev, "IRQ: error i2c->state == IDLE\n");

        tmp = readl(i2c->regs + S3C2410_IICCON);

        tmp &= ~S3C2410_IICCON_IRQPEND;  //清除中断信号,继续传输数据

        writel(tmp, i2c->regs +  S3C2410_IICCON);

        goto out;

}

i2s_s3c_irq_nextbyte(i2c, status);   //传输或接收下一个字节

out:

return IRQ_HANDLED;

}



我们发现其实中断处理函数s3c24xx_i2c_irq中真正传输数据的函数是 i2s_s3c_irq_nextbyte。走了这么久，其实i2s_s3c_irq_nextbyte才是真正的传输数据的核心函数，那我们赶紧来看看 
 i2s_s3c_irq_nextbyte吧。

static int i2s_s3c_irq_nextbyte(struct s3c24xx_i2c *i2c, unsigned long iicstat)

{

unsigned long tmp;

unsigned char byte;

int ret = 0;

switch (i2c->state) {

case STATE_IDLE:       //总线上没有数据传输，则立即返回

        dev_err(i2c->dev, "%s: called in STATE_IDLE\n", __func__);

        goto out;

        break;

case STATE_STOP:             //发出停止信号P

        dev_err(i2c->dev, "%s: called in STATE_STOP\n", __func__);

        s3c24xx_i2c_disable_irq(i2c);   //关中断

        goto out_ack;

case STATE_START:            //发出开始信号S

     //当没有接收到ACK应答信号，说明I2C设备不存在，应停止总线工作

        if (iicstat & S3C2410_IICSTAT_LASTBIT &&

            !(i2c->msg->flags & I2C_M_IGNORE_NAK)) {

               dev_dbg(i2c->dev, "ack was not received\n");

               s3c24xx_i2c_stop(i2c, -ENXIO); //完成发送P信号,唤醒，关中断三个事情

               goto out_ack;

        }

        if (i2c->msg->flags & I2C_M_RD)

               i2c->state = STATE_READ;     //一个读消息

        else

               i2c->state = STATE_WRITE;    //一个写消息

     // is_lastmsg()判断是当前处理的消息是否是最后一个消息，如果是返回1

        if (is_lastmsg(i2c) && i2c->msg->len == 0) {

               s3c24xx_i2c_stop(i2c, 0);

               goto out_ack;

        }

        if (i2c->state == STATE_READ)   //如果是读那进行跳转，注此case无break！

               goto prepare_read;

case STATE_WRITE:

  //当没有接收到ACK应答信号，说明I2C设备不存在，应停止总线工作

        if (!(i2c->msg->flags & I2C_M_IGNORE_NAK)) {

               if (iicstat & S3C2410_IICSTAT_LASTBIT) {

                      dev_dbg(i2c->dev, "WRITE: No Ack\n");

                      s3c24xx_i2c_stop(i2c, -ECONNREFUSED);

                      goto out_ack;

               }

        }

retry_write:

// is_msgend(0判断当前消息是否已经传输完所有字节，如果是返回1

        if (!is_msgend(i2c)) {

               byte = i2c->msg->buf[i2c->msg_ptr++]; //读取待传输数据

               writeb(byte, i2c->regs + S3C2410_IICDS); //将待传输数据写入IICDS

               ndelay(i2c->tx_setup);        //延时50ms，等待发送到总线上

// is_lastmsg()判断是当前处理的消息是否是最后一个消息，如果是返回1

        } else if (!is_lastmsg(i2c)) {  //当前信息传输完，还有信息要传输情况下

               dev_dbg(i2c->dev, "WRITE: Next Message\n");

               i2c->msg_ptr = 0;  //下一条消息字符串的首地址置0

               i2c->msg_idx++;  //表示已经传输完1条消息

               i2c->msg++;  //表示准备传输下一条消息

               if (i2c->msg->flags & I2C_M_NOSTART) { //不处理此新类型消息，停止

                      if (i2c->msg->flags & I2C_M_RD) {

                             s3c24xx_i2c_stop(i2c, -EINVAL);

                      }

               goto retry_write; //当本消息因类型不被处理则继续查看下面是否有消息

               } else { //开始传输消息，将IICDS里的数据发送到总线上

                      s3c24xx_i2c_message_start(i2c, i2c->msg);

                      i2c->state = STATE_START;

               }

        } else { //当前信息传输完，没有信息要传输情况下，停止总线工作

               s3c24xx_i2c_stop(i2c, 0);

        }

        break;

case STATE_READ:

        byte = readb(i2c->regs + S3C2410_IICDS);  //从IICDS读取数据

//将读取到的数据放入缓存区, msg_ptr++直到当前消息传输完毕

        i2c->msg->buf[i2c->msg_ptr++] = byte;


prepare_read:

     // is_msglast()判断如果是消息的最后一个字节，如果是返回1

        if (is_msglast(i2c)) {

    // is_lastmsg()判断是当前处理的消息是否是最后一个消息，如果是返回1

               if (is_lastmsg(i2c))

                      s3c24xx_i2c_disable_ack(i2c); //关闭ACK应答信号

        // is_msgend(0判断当前消息是否已经传输完所有字节，如果是返回1

        } else if (is_msgend(i2c)) {

        // is_lastmsg()判断是当前处理的消息是否是最后一个消息，如果是返回1

               if (is_lastmsg(i2c)) {

                            dev_dbg(i2c->dev, "READ: Send Stop\n");

                      s3c24xx_i2c_stop(i2c, 0);     //发P信号，唤醒等待队列

               } else {


        //当前消息传输完毕，但还有其他消息，则将相关指针指向下一条消息

                            dev_dbg(i2c->dev, "READ: Next Transfer\n");

                      i2c->msg_ptr = 0;  //下一条消息字符串的首地址置0

                      i2c->msg_idx++;   //表示已经传输完1条消息

                      i2c->msg++;   //表示准备传输下一条消息

               }

        }

        break;

}

out_ack:

tmp = readl(i2c->regs + S3C2410_IICCON);

tmp &= ~S3C2410_IICCON_IRQPEND; //清除中断，否则会重复执行中断处理函数

writel(tmp, i2c->regs + S3C2410_IICCON);

out:

return ret;

}



我们终于把这个庞大的i2s_s3c_irq_nextbyte搞定了，在这里需要说明几点，第一，消息分为第一条消息，第二条消息，第三条消息等，总共有msg_num条消息，每发送完一个消息，会msg_idx++。每条消息发送完还需要调用s3c24xx_i2c_message_start进行发送新的起始信号S。第二，第i条消息是一个字符串，按照一个字节一个字节的形式发送，由一个指针msg_ptr指向这个字符串的待发送字节的地址。

在i2s_s3c_irq_nextbyte这个函数中，我们发现有很多s3c24xx_i2c_stop终止函数，

那么让我们来看看这个函数到底怎么终止的吧。



static inline void s3c24xx_i2c_stop(struct s3c24xx_i2c *i2c, int ret)

{

unsigned long iicstat = readl(i2c->regs + S3C2410_IICSTAT);

dev_dbg(i2c->dev, "STOP\n");

iicstat &= ~S3C2410_IICSTAT_START;      //发送P信号

writel(iicstat, i2c->regs + S3C2410_IICSTAT);

i2c->state = STATE_STOP;      //设置适配器状态为停止

s3c24xx_i2c_master_complete(i2c, ret);  //唤醒传输等待队列中的进程

s3c24xx_i2c_disable_irq(i2c);   //禁止中断

}



这个s3c24xx_i2c_stop函数还是很简单的，但里面调用了s3c24xx_i2c_master_complete这个函数来唤醒传输等待队列中的进程，那我们就来看看s3c24xx_i2c_master_complete啦。



static inline void s3c24xx_i2c_master_complete(struct s3c24xx_i2c *i2c, int ret)

{

dev_dbg(i2c->dev, "master_complete %d\n", ret);

i2c->msg_ptr = 0;     //下一条消息字符串的首地址置0

i2c->msg = NULL;    //表示没有可传输的消息

i2c->msg_idx++;   //表示已经传输完1条消息

i2c->msg_num = 0;  //表示没有可传输的消息

if (ret)

        i2c->msg_idx = ret;    //记录已经传输完的信息个数

wake_up(&i2c->wait);   //唤醒等待队列中的进程

}



到此为止，我们已经完成了在I2C总线驱动层填充了i2c_adapter和i2c_algorithm结构体，剩下来我们需要把这两个结构体外包一下，来注册这个适配器，这怎么实现呢？当然我们在上面已经分析了中断处理函数s3c24xx_i2c_irq，那么这个函数什么时候被注册的呢？带着这两个问题我们需要继续往下走，go！

下面两个函数就完成了I2C总线层驱动模块的注册和注销。

static int __init i2c_adap_s3c_init(void)

{ //注册平台设备

return platform_driver_register(&s3c24xx_i2c_driver);

}

static void __exit i2c_adap_s3c_exit(void)

{ //注销平台设备

platform_driver_unregister(&s3c24xx_i2c_driver);

}

那我们来看看这个平台设备吧。

static struct platform_driver s3c24xx_i2c_driver = {

.probe            = s3c24xx_i2c_probe,  //探测函数

.remove          = s3c24xx_i2c_remove,

.id_table  = s3c24xx_driver_ids,

.driver            = {

        .owner    = THIS_MODULE,

        .name      = "s3c-i2c",

        .pm  = S3C24XX_DEV_PM_OPS,

},

};



恩恩，有一定平台驱动基础的朋友应该就明白了，平台驱动的这个探测函数s3c24xx_i2c_probe应该就完成了整个适配器的注册和中断处理函数的注册工作了。别发呆了，那我们就来看看这个s3c24xx_i2c_probe函数吧。



static int s3c24xx_i2c_probe(struct platform_device *pdev)

{

struct s3c24xx_i2c *i2c;   //适配器指针

struct s3c2410_platform_i2c *pdata;   //平台数据

struct resource *res;         //指向资源

int ret;

pdata = pdev->dev.platform_data;   //获取平台数据

if (!pdata) {

        dev_err(&pdev->dev, "no platform data\n");

        return -EINVAL;

}

i2c = kzalloc(sizeof(struct s3c24xx_i2c), GFP_KERNEL);  //分配适配器空间

if (!i2c) {

        dev_err(&pdev->dev, "no memory for state\n");

        return -ENOMEM;

}

//给适配器赋予名字s3c2410-i2c，这个名字会由cat /sys/class/i2c_dev/0/name看到。

strlcpy(i2c->adap.name, "s3c2410-i2c", sizeof(i2c->adap.name));

i2c->adap.owner   = THIS_MODULE;

i2c->adap.algo    = &s3c24xx_i2c_algorithm;   //给适配器一个通信方法！

i2c->adap.retries = 2;   //两次总线仲裁尝试

i2c->adap.class   = I2C_CLASS_HWMON | I2C_CLASS_SPD;

i2c->tx_setup     = 50;    //数据从适配器到总线的时间为50ms

spin_lock_init(&i2c->lock);

init_waitqueue_head(&i2c->wait);   //初始化等待队列

i2c->dev = &pdev->dev;


i2c->clk = clk_get(&pdev->dev, "i2c");   //获取i2c时钟

if (IS_ERR(i2c->clk)) {

        dev_err(&pdev->dev, "cannot get clock\n");

        ret = -ENOENT;

        goto err_noclk;

}

dev_dbg(&pdev->dev, "clock source %p\n", i2c->clk);

clk_enable(i2c->clk);              //使能i2c时钟

res = platform_get_resource(pdev, IORESOURCE_MEM, 0);//获取适配器寄存器资源

if (res == NULL) {

        dev_err(&pdev->dev, "cannot find IO resource\n");

        ret = -ENOENT;

        goto err_clk;

}

i2c->ioarea = request_mem_region(res->start, resource_size(res),  //申请I/O内存

                              pdev->name);

if (i2c->ioarea == NULL) {

        dev_err(&pdev->dev, "cannot request IO\n");

        ret = -ENXIO;

        goto err_clk;

}

i2c->regs = ioremap(res->start, resource_size(res));  //将内存地址映射到虚拟地址

if (i2c->regs == NULL) {

        dev_err(&pdev->dev, "cannot map IO\n");

        ret = -ENXIO;

        goto err_ioarea;

}

dev_dbg(&pdev->dev, "registers %p (%p, %p)\n",

        i2c->regs, i2c->ioarea, res);

i2c->adap.algo_data = i2c;               //将私有数据指向适配器结构体

i2c->adap.dev.parent = &pdev->dev;       //组织适配器的设备模型

ret = s3c24xx_i2c_init(i2c);           //初始化I2C控制器

if (ret != 0)

        goto err_iomap;

i2c->irq = ret = platform_get_irq(pdev, 0);   //获取平台设备的中断号

if (ret <= 0) {

        dev_err(&pdev->dev, "cannot find IRQ\n");

        goto err_iomap;

}

ret = request_irq(i2c->irq, s3c24xx_i2c_irq, IRQF_DISABLED,

                 dev_name(&pdev->dev), i2c);       //注册中断处理函数

if (ret != 0) {

        dev_err(&pdev->dev, "cannot claim IRQ %d\n", i2c->irq);

        goto err_iomap;

}

ret = s3c24xx_i2c_register_cpufreq(i2c);   //在内核中注册一个适配器使用的时钟

if (ret < 0) {

        dev_err(&pdev->dev, "failed to register cpufreq notifier\n");

        goto err_irq;

}

i2c->adap.nr = pdata->bus_num;


ret = i2c_add_numbered_adapter(&i2c->adap);  //向内核中添加适配器

if (ret < 0) {

        dev_err(&pdev->dev, "failed to add bus to i2c core\n");

        goto err_cpufreq;

}

platform_set_drvdata(pdev, i2c);  //将I2C适配器设置为平台设备的私有数据

dev_info(&pdev->dev, "%s: S3C I2C adapter\n", dev_name(&i2c->adap.dev));

return 0;

err_cpufreq:

s3c24xx_i2c_deregister_cpufreq(i2c);

err_irq:

free_irq(i2c->irq, i2c);

err_iomap:

iounmap(i2c->regs);

err_ioarea:

release_resource(i2c->ioarea);

kfree(i2c->ioarea);

err_clk:

clk_disable(i2c->clk);

clk_put(i2c->clk);

err_noclk:

kfree(i2c);

return ret;

}



我们来回顾下这个探测函数s3c24xx_i2c_probe吧，这个函数主要干了六件事。第一，申请一个I2C适配器结构体，并对其赋值。第二，获取I2C时钟资源，并注册时钟。第三，获取资源并最终映射到物理地址。第四，申请中断处理函数。第五，初始化I2C控制器。第六，将I2C适配器添加到内核中。对于resume函数由于做的是跟探测函数相反的操作，在此就无需浪费时间了。

接下来，我们来看看上面第五步初始化I2C控制器所使用的函数s3c24xx_i2c_init。



static int s3c24xx_i2c_init(struct s3c24xx_i2c *i2c)

{

unsigned long iicon = S3C2410_IICCON_IRQEN | S3C2410_IICCON_ACKEN;

struct s3c2410_platform_i2c *pdata;

unsigned int freq;

pdata = i2c->dev->platform_data;

if (pdata->cfg_gpio)  //初始化GPIO口

       pdata->cfg_gpio(to_platform_device(i2c->dev));

writeb(pdata->slave_addr, i2c->regs + S3C2410_IICADD); //写入从机地址

dev_info(i2c->dev, "slave address 0x%02x\n", pdata->slave_addr);

writel(iicon, i2c->regs + S3C2410_IICCON); //开中断，ACK信号使能

if (s3c24xx_i2c_clockrate(i2c, &freq) != 0) { //设置时钟源和时钟频率

        writel(0, i2c->regs + S3C2410_IICCON); //失败则设置为0

        dev_err(i2c->dev, "cannot meet bus frequency required\n");

        return -EINVAL;

}

dev_info(i2c->dev, "bus frequency set to %d KHz\n", freq);

dev_dbg(i2c->dev, "S3C2410_IICCON=0x%02lx\n", iicon);

return 0;

}

在s3c24xx_i2c_init中，我们调用s3c24xx_i2c_clockrate设置了时钟源和时钟频率，继续看下去。



static int s3c24xx_i2c_clockrate(struct s3c24xx_i2c *i2c, unsigned int *got)

{

struct s3c2410_platform_i2c *pdata = i2c->dev->platform_data;

unsigned long clkin = clk_get_rate(i2c->clk);  //获取PLCK时钟，单位为HZ

unsigned int divs, div1;

unsigned long target_frequency;

u32 iiccon;

int freq;

i2c->clkrate = clkin;

clkin /= 1000;        //时钟频率单位转为KHZ

dev_dbg(i2c->dev, "pdata desired frequency %lu\n", pdata->frequency);

target_frequency = pdata->frequency ? pdata->frequency : 100000;

target_frequency /= 1000;   //目标频率，单位KHZ

freq = s3c24xx_i2c_calcdivisor(clkin, target_frequency, &div1, &divs);  //获取分频值

if (freq > target_frequency) {

        dev_err(i2c->dev,

               "Unable to achieve desired frequency %luKHz."   \

               " Lowest achievable %dKHz\n", target_frequency, freq);

        return -EINVAL;

}

*got = freq;

//将分频值写入IICCON相应位

iiccon = readl(i2c->regs + S3C2410_IICCON);

iiccon &= ~(S3C2410_IICCON_SCALEMASK | S3C2410_IICCON_TXDIV_512);

iiccon |= (divs-1);

if (div1 == 512)

        iiccon |= S3C2410_IICCON_TXDIV_512;

writel(iiccon, i2c->regs + S3C2410_IICCON);

//如果设备是2440，则执行下面代码处理

if (s3c24xx_i2c_is2440(i2c)) {

        unsigned long sda_delay;

        if (pdata->sda_delay) {

               sda_delay = (freq / 1000) * pdata->sda_delay;

               sda_delay /= 1000000;

               sda_delay = DIV_ROUND_UP(sda_delay, 5);

               if (sda_delay > 3)

                      sda_delay = 3;

               sda_delay |= S3C2410_IICLC_FILTER_ON;

        } else

               sda_delay = 0;

        dev_dbg(i2c->dev, "IICLC=%08lx\n", sda_delay);

        writel(sda_delay, i2c->regs + S3C2440_IICLC);

}

return 0;

}

在s3c24xx_i2c_clockratet中，我们调用s3c24xx_i2c_calcdivisor根据已知PCLK和目标频率，获取了两个分频系数，我们继续看下去。



static int s3c24xx_i2c_calcdivisor(unsigned long clkin, unsigned int wanted,

                         unsigned int *div1, unsigned int *divs)

{

unsigned int calc_divs = clkin / wanted;

unsigned int calc_div1;

if (calc_divs > (16*16))


        calc_div1 = 512;    //IICLK=PCLK/512

else

        calc_div1 = 16;     //IICLK=PCLK/16

calc_divs += calc_div1-1;

calc_divs /= calc_div1;

if (calc_divs == 0)   //控制分频量程范围

        calc_divs = 1;

if (calc_divs > 17)   //控制分频量程范围

        calc_divs = 17;

*divs = calc_divs;  //分频系数

*div1 = calc_div1;  //时钟源的选择

return clkin / (calc_divs * calc_div1);

}



好了，到现在为止，我们的I2C总线层驱动就已经全部搞定了，我们总结下吧！在基于mini2440的I2C总线层驱动中，我们首先加载了一个平台设备，在平台设备的探测函数中，我们主要注册了适配器和中断处理函数。适配器结构体主要是实现通信方法的函数s3c24xx_i2c_xfer，我们在这里是使用的中断方式进行通信的，这也是大多数的情况下我们的选择，当然我们也可以采用查询的方式进行编写s3c24xx_i2c_xfer函数，只需要判断是读还是写操作就可以了。I2C总线层驱动模块加载后会在sys文件系统下产生一个适配器节点，可以供I2C设备驱动层来进行探测匹配。



4.I2C设备驱动

前面已经说过I2C设备层驱动有两种实现方式，我们选择用户模式设备驱动方式，这种驱动依赖I2C子系统中的i2c-dev这个驱动。I2C设备驱动主要填充i2c_driver和i2c_client结构体，同时提供read，write，ioctl等API供应用层使用。在分析设备驱动层的时候，我们要留意设备驱动层怎么找到总线驱动层相应的适配器的。



让我们先一起来填充一下i2c_driver吧

static struct i2c_driver i2cdev_driver = {

.driver = {

        .name      = "dev_driver",

},

.attach_adapter       = i2cdev_attach_adapter,  //探测适配器函数

.detach_adapter      = i2cdev_detach_adapter,

};



那下面就需要看看这个结构体怎么被注册到内核中的了。

static int __init i2c_dev_init(void)

{

int res;

printk(KERN_INFO "i2c /dev entries driver\n");

res = register_chrdev(I2C_MAJOR, "i2c", &i2cdev_fops);   //注册字符设备提供API

if (res)

        goto out;

i2c_dev_class = class_create(THIS_MODULE, "i2c-dev");  //注册类

if (IS_ERR(i2c_dev_class)) {

        res = PTR_ERR(i2c_dev_class);

        goto out_unreg_chrdev;

}

res = i2c_add_driver(&i2cdev_driver);   //调用核心层函数，注册i2c_driver结构体

if (res)

        goto out_unreg_class;

return 0;

out_unreg_class:

class_destroy(i2c_dev_class);

out_unreg_chrdev:

unregister_chrdev(I2C_MAJOR, "i2c");

out:

printk(KERN_ERR "%s: Driver Initialisation failed\n", __FILE__);

return res;

}



这样，我们的这个I2C设备层驱动就被作为模块加载到内核中了。好了。我们继续看i2c_driver结构体中的i2cdev_attach_adapter怎么来探测适配器的吧。

static int i2cdev_attach_adapter(struct i2c_adapter *adap)

{

struct i2c_dev *i2c_dev;

int res;

//检查内核中是否注册过了适配器，如果没注册直接返回，注册了适配器那么就返

//回一个指向该适配器的i2c_dev结构体

i2c_dev = get_free_i2c_dev(adap); 


if (IS_ERR(i2c_dev))

        return PTR_ERR(i2c_dev);

//注册这个I2C设备到核心层

i2c_dev->dev = device_create(i2c_dev_class, &adap->dev,

                           MKDEV(I2C_MAJOR, adap->nr), NULL,

                           "i2c-%d", adap->nr);

if (IS_ERR(i2c_dev->dev)) {

        res = PTR_ERR(i2c_dev->dev);

        goto error;

}

res = device_create_file(i2c_dev->dev, &dev_attr_name);  //添加设备属性

if (res)

        goto error_destroy;

pr_debug("i2c-dev: adapter [%s] registered as minor %d\n",

         adap->name, adap->nr);

return 0;

error_destroy:

device_destroy(i2c_dev_class, MKDEV(I2C_MAJOR, adap->nr));

error:

return_i2c_dev(i2c_dev);

return res;

}



这样就完成了适配器的探测，至此，我们填充i2c_driver就分析至此了。接下来我们该去填充i2c_client了。此时，我们发现上面提到在i2c_dev_init里注册了一个字符设备，为我们提供了API，那我们来看看这些API里是否能找到我们填充了的i2c_client结构体呢？

static const struct file_operations i2cdev_fops = {

.owner           = THIS_MODULE,

.llseek            = no_llseek,

.read              = i2cdev_read,

.write             = i2cdev_write,

.unlocked_ioctl       = i2cdev_ioctl,

.open             = i2cdev_open,

.release    = i2cdev_release,

};



有字符设备常识的朋友，对上面代码应该很熟悉了。那我们一个个分析这些API吧。

static int i2cdev_open(struct inode *inode, struct file *file)

{

unsigned int minor = iminor(inode);  //由struct inode节点获取次设备号

struct i2c_client *client;

struct i2c_adapter *adap;

struct i2c_dev *i2c_dev;

int ret = 0;

lock_kernel();  //此处代码其实什么也没做

//将这个次设备号当做IDR机制中的ID查看是否有满足ID=minor的适配器

i2c_dev = i2c_dev_get_by_minor(minor);

if (!i2c_dev) {

        ret = -ENODEV;

        goto out;

}

adap = i2c_get_adapter(i2c_dev->adap->nr);   //获取匹配的适配器

if (!adap) {

        ret = -ENODEV;

        goto out;

}

client = kzalloc(sizeof(*client), GFP_KERNEL);   //为i2c_client分配空间

if (!client) {

        i2c_put_adapter(adap);

        ret = -ENOMEM;

        goto out;

}

snprintf(client->name, I2C_NAME_SIZE, "i2c-dev %d", adap->nr);

client->driver = &i2cdev_driver;    //设置i2c_client的驱动

client->adapter = adap;                   //设置i2c_client的适配器

file->private_data = client;        //将i2c_client作为文件的私有数据

out:

unlock_kernel();

return ret;

}



通过上面分析，在i2cdev_open中，我们发现i2c_client的分配并且找到了i2c_client与适配器和设备层驱动的联系。为了给用户提供API，字符设备还有read，write，ioctl。我们主要分析一下i2cdev_ioctl这个函数。

static long i2cdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)

{

struct i2c_client *client = (struct i2c_client *)file->private_data;

unsigned long funcs;



dev_dbg(&client->adapter->dev, "ioctl, cmd=0x%02x, arg=0x%02lx\n",

        cmd, arg);

switch ( cmd ) {

case I2C_SLAVE:

case I2C_SLAVE_FORCE:

        if ((arg > 0x3ff) ||

            (((client->flags & I2C_M_TEN) == 0) && arg > 0x7f))

               return -EINVAL;

        if (cmd == I2C_SLAVE && i2cdev_check_addr(client->adapter, arg))

               return -EBUSY;

        client->addr = arg;   //设置从机地址

        return 0;

case I2C_TENBIT:   //是否有十位地址芯片的设置

        if (arg)

               client->flags |= I2C_M_TEN;

        else

               client->flags &= ~I2C_M_TEN;

        return 0;

case I2C_PEC:        


        if (arg)

               client->flags |= I2C_CLIENT_PEC;

        else

               client->flags &= ~I2C_CLIENT_PEC;

        return 0;

case I2C_FUNCS:  //适配器支持项设置

        funcs = i2c_get_functionality(client->adapter);

        return put_user(funcs, (unsigned long __user *)arg);

case I2C_RDWR:  //读写控制

        return i2cdev_ioctl_rdrw(client, arg);

case I2C_SMBUS:  //SMBUS总线通信协议设置

        return i2cdev_ioctl_smbus(client, arg);

case I2C_RETRIES:    //重试次数设置

        client->adapter->retries = arg;

        break;

case I2C_TIMEOUT:   //超时时间设置

        client->adapter->timeout = msecs_to_jiffies(arg * 10);

        break;

default:

        return -ENOTTY;

}

return 0;

}

在i2cdev_ioctl函数中，我们常用I2C_RETRIES、I2C_TIMEOUT和I2C_RDWR标签，对于I2C_RDWR标签，我们可以发现在此调用了i2cdev_ioctl_rdrw函数，为了清楚怎么进行读写控制，我们继续看看i2cdev_ioctl_rdrw函数吧。



static noinline int i2cdev_ioctl_rdrw(struct i2c_client *client,

        unsigned long arg)

{

struct i2c_rdwr_ioctl_data rdwr_arg;

struct i2c_msg *rdwr_pa;

u8 __user **data_ptrs;

int i, res;

if (copy_from_user(&rdwr_arg,

                  (struct i2c_rdwr_ioctl_data __user *)arg,

                  sizeof(rdwr_arg)))   //复制用户空间的i2c_rdwr_ioctl_data到内核空间

        return -EFAULT;

if (rdwr_arg.nmsgs > I2C_RDRW_IOCTL_MAX_MSGS)  //限制消息个数

        return -EINVAL;

rdwr_pa = (struct i2c_msg *)               //分配t i2c_msg结构体

        kmalloc(rdwr_arg.nmsgs * sizeof(struct i2c_msg),

        GFP_KERNEL);

if (!rdwr_pa)

        return -ENOMEM;

if (copy_from_user(rdwr_pa, rdwr_arg.msgs,

                  rdwr_arg.nmsgs * sizeof(struct i2c_msg))) { //拷贝获取 i2c_msg内容

        kfree(rdwr_pa);

        return -EFAULT;

}

data_ptrs = kmalloc(rdwr_arg.nmsgs * sizeof(u8 __user *), GFP_KERNEL);

if (data_ptrs == NULL) {   //分配消息缓存区失败

        kfree(rdwr_pa);

       return -ENOMEM;

}

res = 0;

for (i = 0; i < rdwr_arg.nmsgs; i++) {

        if ((rdwr_pa[i].len > 8192) ||

            (rdwr_pa[i].flags & I2C_M_RECV_LEN)) { //限制消息长度

               res = -EINVAL;

               break;

        }

        data_ptrs[i] = (u8 __user *)rdwr_pa[i].buf; //将消息字符串给中间变量

        rdwr_pa[i].buf = kmalloc(rdwr_pa[i].len, GFP_KERNEL);

        if (rdwr_pa[i].buf == NULL) {

               res = -ENOMEM;

               break;

        }

        if (copy_from_user(rdwr_pa[i].buf, data_ptrs[i],

                         rdwr_pa[i].len)) {  //将中间变量传给内核中的i2c_msg结构体

                      ++i; /* Needs to be kfreed too */

                      res = -EFAULT;

               break;

        }

}

if (res < 0) {   //分配空间失败时需呀释放空间资源

        int j;

        for (j = 0; j < i; ++j)

               kfree(rdwr_pa[j].buf);

        kfree(data_ptrs);

        kfree(rdwr_pa);

        return res;

}

//调用核心层i2c_transfer函数完成数据的传输

res = i2c_transfer(client->adapter, rdwr_pa, rdwr_arg.nmsgs);

while (i-- > 0) {

//如果传输的数据后期需要读取，则将传输的数据放在中间变量的保存

        if (res >= 0 && (rdwr_pa[i].flags & I2C_M_RD)) {

               if (copy_to_user(data_ptrs[i], rdwr_pa[i].buf,

                              rdwr_pa[i].len))

                      res = -EFAULT;

        }

        kfree(rdwr_pa[i].buf);  //释放资源

}

kfree(data_ptrs);

kfree(rdwr_pa);

return res;

}

我们再来看看i2cdev_ioctl_rdrw这个函数，其实这个函数只是对用户空间传过来的数据进行截取分类存放，然后调用i2c_transfer进行数据传输，我们继续跟踪下去。



int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)

{

unsigned long orig_jiffies;

int ret, try;

if (adap->algo->master_xfer) {

#ifdef DEBUG

        for (ret = 0; ret < num; ret++) {

               dev_dbg(&adap->dev, "master_xfer[%d] %c, addr=0x%02x, "

                      "len=%d%s\n", ret, (msgs[ret].flags & I2C_M_RD)

                      ? 'R' : 'W', msgs[ret].addr, msgs[ret].len,

                      (msgs[ret].flags & I2C_M_RECV_LEN) ? "+" : "");

        }

#endif



        if (in_atomic() || irqs_disabled()) {

               ret = mutex_trylock(&adap->bus_lock);

               if (!ret)

                      /* I2C activity is ongoing. */

                      return -EAGAIN;

        } else {

               mutex_lock_nested(&adap->bus_lock, adap->level);

        }

        orig_jiffies = jiffies;

        for (ret = 0, try = 0; try <= adap->retries; try++) {

               ret = adap->algo->master_xfer(adap, msgs, num);  //真正的数据传输

               if (ret != -EAGAIN)

                      break;

               if (time_after(jiffies, orig_jiffies + adap->timeout))

                      break;

        }

        mutex_unlock(&adap->bus_lock);

        return ret;

} else {

        dev_dbg(&adap->dev, "I2C level transfers not supported\n");

        return -EOPNOTSUPP;

}

}

通过上面的跟踪，我们很快发现i2c_transfer其实是调用了master_xfer函数进行的数据传输。至于master_xfer干什么的，我们再I2C总线层驱动分析中已经讲的很清楚了，master_xfer就是我们适配器的I2C通信协议函数。

好了，I2C的设备驱动层我们也讲完了，回顾一下，I2C的设备驱动层主要是填充了i2c_driver和i2c_client结构体，然后注册了i2c_driver，在i2c_driver中，我们探测了适配器。同时为了给用户提供API，我们还注册了一个字符设备，在字符设备中的open函数中，我们完成了i2c_client结构体的填充，并获取了匹配的适配器。最后我们讲了ioctl函数，重点分析了I2C_RDWR标签下最终调用适配器的I2C通信协议函数master_xfer完成数据传输。



5.*****



三．测试代码

我们已经分析了mini2440的I2C总线层驱动已经内核为我们提供的i2c-dev.c文件。系统中i2c-dev.c文件定义的主设备号为89的设备可以方便地给应用程序提供读写I2C设备寄存器的能力，使得工程师大多数时候并不需要为具体的I2C设备驱动定义文件操作接口。在此我们就使用i2c-dev.c文件为用户提供API。我们利用mini2440外接一个256byte的E2PROM芯片AT24C08进行I2C驱动的测试。根据I2C设备地址的设计规则，要求D7位为读写位，D6-D3位为器件类型，D2-D0为自定义地址。我们可以通过查看mini2440外围电路发现AT24C08的A0-A2均接地，表明AT24C08自定义地址D2-D0=000，而AT24C08器件类型编号为1010，所以我们的从机设备AT24C08地址为0X50。



实验环境：内核linux2.6.32.2，arm-linux-gcc交叉编译器，mini2440开发板。

内核配置：选中核心层i2c_core.c、设备驱动层i2c-dev.c和总线驱动层I2c_s3c2410.c

为了用户空间与内核空间的信息交互，我们还需要在用户空间引入内核空间的两个结构体i2c_msg和i2c_rdwr_ioctl_data。具体测试代码如下



#include <stdio.h>

#include <linux/types.h>

#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/ioctl.h>

#include <errno.h>

#define I2C_RETRIES 0x0701

#define I2C_TIMEOUT 0x0702

#define I2C_RDWR 0x0707

struct i2c_msg

{

unsigned short addr;   //从机的地址

unsigned short flags;  //设置读写

#define I2C_M_TEN 0x0010

#define I2C_M_RD 0x0001

unsigned short len;  //一个消息字符串的字节数

unsigned char *buf;  //消息字符串

};

struct i2c_rdwr_ioctl_data

{

struct i2c_msg *msgs;


int nmsgs;    //传输消息的个数

};

int main()

{

int fd,ret;

struct i2c_rdwr_ioctl_data e2prom_data;

fd=open("/dev/i2c/0",O_RDWR);

//dev/i2c/0是在注册i2c-dev.c后产生的，代表一个可操作的适配器。如果不使用i2c-dev.c

//的方式，就没有，也不需要这个节点

if(fd<0){

              perror("open error");

              }

  e2prom_data.nmsgs=2;   //传输的信息个数

  e2prom_data.msgs=(struct i2c_msg*)malloc(e2prom_data.nmsgs*sizeof(struct i2c_msg));

  if(!e2prom_data.msgs){

         perror("malloc error");

               exit(1);

         }

ioctl(fd,I2C_TIMEOUT,100);   //超时时间100ms

//ioctl(fd,I2C_M_TEN,0);

ioctl(fd,I2C_RETRIES,2);     //重试次数2次

e2prom_data.nmsgs=1;       //传输1条信息

(e2prom_data.msgs[0]).len=2;   //信息长度为2个字节

(e2prom_data.msgs[0]).addr=0x50;//从机的地址

(e2prom_data.msgs[0]).flags=0; //  flag=0表示写

(e2prom_data.msgs[0]).buf=(unsigned char*)malloc(2);  //申请空间

(e2prom_data.msgs[0]).buf[0]=0x01;//第一个传输的数据是AT24C08存储空间的地址

(e2prom_data.msgs[0]).buf[1]=0x74;//第二个传输的数据是向AT24C08地址空间写的数

ret=ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);  //调用ioctl

  if(ret<0){

  perror("ioctl error1");

         }

   sleep(1);

e2prom_data.nmsgs=2;     //传输2条信息

(e2prom_data.msgs[0]).len=1; //信息的长度为1个字节

(e2prom_data.msgs[0]).addr=0x50; //从机的地址

(e2prom_data.msgs[0]).flags=0;// flag=0表示写

(e2prom_data.msgs[0]).buf[0]=0x01;//第一个传输的数据是AT24C08存储空间的地址

(e2prom_data.msgs[1]).len=1;//信息的长度为1个字节

(e2prom_data.msgs[1]).addr=0x50;//从机的地址

(e2prom_data.msgs[1]).flags=I2C_M_RD;// flag= I2C_M_RD表示读

(e2prom_data.msgs[1]).buf=(unsigned char*)malloc(1); //申请空间

(e2prom_data.msgs[1]).buf[0]=0;  //清空

ret=ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data); //调用ioctl

if(ret<0){

      perror("ioctl error2");

        }

  printf("buff[0]=%x\n",(e2prom_data.msgs[1]).buf[0]);  //打印读到的数据，应该是0x74

  close(fd);

  return 0;



}



测试结果：

虚拟机下编译arm-linux-gcc  i2c.c  -o i2c

在超级终端下运行：./i2c

可以见到：buff[0]=74







