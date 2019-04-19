# 乾坤合一：Linux设备驱动之I2C核心、总线以及设备驱动 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
在Linux 系统中，I2C 驱动由3 部分组成，即I2C 核心、I2C 总线驱动和I2C 设备驱动，I2C 总线仅仅使用SCL、SDA 这两根信号线就实现了设备之间的数据交互，极大地简化了对硬件资源和PCB 板布线空间的占用
**1 Linux的I2C体系结构**
1.1 组成部分
1) I2C核心
I2C核心提供了I2C总线驱动和设备驱动的注册、注销方法，i2C通信方法等等
2) I2C总线驱动
I2C 总线驱动主要包含了I2C 适配器数据结构i2c_adapter、I2C 适配器的algorithm数据结构i2c_algorithm 和控制I2C适配器产生通信信号的函数。是对I2C硬件体系结构中适配器端的实现适配器可由CPU 控制，甚至可以直接集成在CPU 内部。
3) I2C设备驱动
I2C 设备驱动主要包含了数据结构i2c_driver 和i2c_client ，我们需要根据具体设备实现其中的成员函数。是对I2C硬件体系结构中设备端的实现，设备一般挂接在受 CPU 控制的I2C适配器上，通过I2C 适配器与CPU 交换数据。
1.2 I2C设备
所有的I C 设备都在sysfs 文件系统中显示，存于/sys/bus/i2c/目录，以适配器地址和芯片地址的形式列出，我在Linux上输入命令得出：
![](http://jbcdn2.b0.upaiyun.com/2016/05/fe105a0b2de5b055066105cc4239de25.png)
1.3 i2C.h头文件
内核中的 i2c.h 这个头文件对 i2c_driver 、i2c_client 、i2c_adapter 和i2c_algorithm 这4 个数据结构进行了定义。理解这4个结构体的作用十分关键.下面分析这4个数据结构之间的关系以及作用.至于各个结构具体的代码自己在这个文件中查找就可以啦。
1) i2c_adapter 与i2c_algorithm。
i2c_adapter 对应于物理上的一个适配器，而i2c_algorithm 对应一套通信方法。一个I C 适配器需要i2c_algorithm 中提供的通信函数来控制适配器上产生特定的访问周期。缺少i2c_algorithm 的i2c_adapter 什么也做不了，因此i2c_adapter 中包含其使用的i2c_algorithm 的指针。i2c_algorithm 中的关键函master_xfer()用于产生 I2C 访问周期需要的信号，以i2c_msg （即I2C 消息）为单位
2) i2c_driver 与i2c_client 。
i2c_driver 对应一套驱动方法，是纯粹的用于辅助作用的数据结构，它不对应于任何的物理实体。i2c_client 对应于真实的物理设备，每个I C 设备都需要一个i2c_client来描述。i2c_client 一般被包含在I C 字符设备的私有信息结构体中。 i2c_driver 与i2c_client 发生关联的时刻在i2c_driver 的attach_adapter() 函数被运行时。attach_adapter()会探测物理设备，当确定一个 client 存在时，把该 client 使用的 i2c_client 数据结构的adapter 指针指向对应的i2c_adapter，driver 指针指向该i2c_driver，并会调用 i2c_adapter 的 client_register() 函数。相反的过程发生在 i2c_driver 的 detach_client()函数被调用的时候。
3) i2c_adpater 与i2c_client
i2c_adpater 与 i2c_client 的关系与 I C 硬件体系中适配器和设备的关系一致，即i2c_client 依附于 i2c_adpater.由于一个适配器上可以连接多个 I2C 设备，所以一个i2c_adpater 也可以被多个i2c_client 依附，i2c_adpater 中包括依附于它的i2c_client 的链表。
**2 Linux I2C核心**
2.1 增加/删除i2c_adapter 。


```
int i2c add adapter(struct i2c adapter *adap);
 
int i2c del adapter(struct i2c adapter *adap);
```
2.2 增加/删除i2c_driver 。


```
int i2c register driver(struct module *owner, struct i2c driver *driver);
 
int i2c del driver (struct i2c driver *driver);
 
inline int i2c add driver(struct i2c driver *driver);
```
2.3 i2c_client 依附/脱离


```
int i2c attach client (struct i2c client *client);
 
int i2c detach client (struct i2c client *client)
```
2.4 I2C 传输、发送和接收


```
int i2c transfer (struct i2c adapter * adap, struct i2c msg *msgs, int num);
 
int i2c master send (struct i2c client *client,const char *buf ,intcount);
 
int i2c master recv (struct i2c client *client, char *buf ,int count);
```
2.5 I2C 控制命令分配
下面函数有助于将发给 I C 适配器设备文件 ioctl 的命令分配给对应适配器的algorithm 的algo_control()函数或i2c_driver 的command()函数，如下所示：


```
int i2c control(struct i2c client *client, unsigned int cmd, unsigned long arg);
 
void i2c clients command (struct i2c adapter *adap, unsigned int cmd,void *arg);
```
**3 Linux I2C总线驱动**
3.1 I2C 适配器驱动加载与卸载


```
//I2C 总线驱动的模块加载和卸载函数模板如下代码
 
       static int      init i2c adapter xxx init (void)
 
       {
 
         xxx adpater hw init ();
 
         i2c add adapter(&xxx adapter);
 
       }
 
       static void      exit i2c adapter xxx exit (void)
 
       {
  
         xxx adpater hw free ();
 
        i2c del adapter(&xxx adapter);
 
      } <br>//xxx_adpater_hw_init()和xxx_adpater_hw_free()函数的实现都与具体的CPU 和I2C 设备硬件直接相关
```
3.2 I2C 总线通信方法
其通信方法主要是实现 i2c_algorithm 的master_xfer() 函数和functionality()函数。functionality()函数非常简单，用于返回algorithm 所支持的通信协议，master_xfer() 函数在I2C 适配器上完成传递给它的i2c_msg 数 中的每个I2C 消息，以下代码 所示为xxx 设备的master_xfer() 函数模板。

C
```
static int i2c adapter xxx xfer (struct i2c adapter *adap, struct i2c msg *msgs, int num)
       {
         ...
         for (i = 0; i < num; i++)
         {
          
           i2c adapter xxx start (); /*产生开始位*/
           /*是读消息*/
                         
           if (msgs[i]->flags &I2C M RD)
          {
           
            i2c adapter xxx setaddr((msg->addr << 1) | 1); /*发送从设备读地址*/
 
            i2c adapter xxx wait ack (); /*获得从设备的ack*/
           
            i2c adapter xxx readbytes(msgs[i]->buf, msgs[i]->len); /*读取msgs[i] ->len
              长的数据到msgs[i]->buf*/
          }
          else
　　　　 /*是写消息*/
         {
           
           i2c adapter xxx setaddr(msg->addr << 1); /*发送从设备写地址*/
             
           i2c adapter xxx wait ack (); /*获得从设备的ack*/
         
           i2c adapter xxx writebytes(msgs[i]->buf, msgs[i]->len); /*读取msgs[i] ->len
             长的数据到msgs[i]->buf*/
         }
        }
          
       i2c adapter xxx stop (); /*产生停止位*/
     }
```
3.3 xxx_i2c  结构体
多数I2C 总线驱动会定义一个 xxx_i2c 结构体，作为 i2c_adapter 的algo_data（类似“私有数据”），其中包含I C 消息数 指针、数 索引及I C 适配器algorithm访问控制用的自旋锁、等待队列等，而master_xfer()函数完成消息数 中消息的处理也可通过对 xxx_i2c 结构体相关成员的访问来控制，xxx_i2c 结构体的定义如下：


```
struct xxx i2c
       {
                
        spinlock t        lock;
              
        wait queue head t     wait;
              
        struct i2c msg  *msg;
                              
        unsigned int          msg num;
                                
        unsigned int          msg idx;
                                   
        unsigned int          msg ptr;
        ...
            
       struct i2c adapter adap;
      };
```
**4 Linux I2C设备驱动**
I2C 设备驱动要使用 i2c_driver和 i2c_client 数据结构并填充其中的成员函数
4.1 模块加载与卸载
I2C 设备驱动的加载与卸载函数模板代码如下：


```
static int      init yyy init (void)
       {
         int res;
         /*注册字符设备*/
                 
         res = register chrdev (YYY MAJOR, "yyy", &yyy fops); //老内核接口
//说明注册“yyy ”这个字符设备时，使用的 file_operations 结构体为 yyy_fops说明注册“yyy ”这个字符设备时，使用的 file_operations结构体为yyy_fops
         if (res)
           goto out;
             
         /*添加i2c driver*/
            
         res = i2c add driver (&yyy driver);
        if (res)
             
          goto out unreg class;
        return 0;
 
        out unreg chrdev : unregister chrdev (I2C MAJOR, "i2c");
                       
        out: printk (KERN ERR "%s: Driver Initialisation failed\n",          FILE );
        return res;
      }
 
      static void      exit yyy exit (void)
      {
            
        i2c del driver (&i2cdev driver);
                
        unregister chrdev (YYY MAJOR, "yyy");
      }
```
4.2 i2c_driver 成员函数
i2c_add_driver的执行会引发i2c_driver 结构体中yyy_attach_adapter()函数的执行，我们可以在yyy_attach_adapter() 函数里探测物理设备。为了实现探测，yyy_attach_adapter() 函数里面也只需简单地调用I2C 核心的i2c_probe()函数,I2C设备驱动的i2c_attach_adapter 函数代码如下：


```
static int yyy attach adapter(struct i2c adapter *adapter)
{
   return i2c probe (adapter, &addr data, yyy detect);
} //第1 个参数是i2c_adapter指针，第2 个参数是要探测的地址数据，第3个参数是具体的探测函数
```
i2c_probe()函数会引发yyy_detect()函数的调用，可以在yyy_detect()函数中初始化i2c_client,I2C设备驱动的detect 函数代码如下：


```
static int yyy detect (struct i2c adapter *adapter, int address, int kind)
       {
               
        struct i2c client *new client;
               
        struct yyy data *data;
        int err = 0;
 
        if (!i2c check functionality (adapter, I2C FUNC XXX)
           goto exit;
 
      if (!(data = kzalloc (sizeof(struct yyy data), GFP KERNEL)))
//分配私有信息结构体的内存，i2c_client 也被创建
       {
           err =  - ENOMEM;
           goto exit;
       }
  
       new client = &data->client;
             
       new client->addr = address;
          
       new client->adapter = adapter;
           
       new client->driver = &yyy driver;
            
       new client->flags = 0;
       
       /* 新的client 将依附于adapter */
    
       if ((err = i2c attach client (new client)))
//调用内核的i2c_attach_client()知会I C 核心系统中包含了一个新的I C 设备                  
           goto exit kfree;
       
        
       yyy init client (new client); //初始化i2c_client 对应的I2C 设备
       return 0;
              
       exit kfree: kfree (data);
       exit: return err;
     }
```
I2C 设备驱动的卸载函数进行 i2c_del_driver调用后，会引发与 yyy_driver 关联的每个i2c_client 与之解除关联,函数yyy_detach_client() 的设计的代码如下：


```
static int yyy detach client (struct i2c client *client)
      {
        int err;
          
        struct yyy data *data = i2c get clientdata (client);
//i2c_get_clientdata()函数用于从 yyy_data私有信息结构中的i2c_client  的指针获yyy_data  的指针
 
        if ((err = i2c detach client (client)))
//调用I C 核心函数i2c_detach_client()，这个函数会引发i2c_adapter 的client_unregister()函数被调用
         return err;
       
        kfree (data);    //释放yyy_data的内存。
       return 0;
      }
```
4.3 yyy_command() 的实现
它实现了针对设备的控制命令。具体的控制命令是设备相关的,假设yyy 设备接受两类命令YYY_CMD1 和YYY_CMD2 ，而处理这两个命令的函数分别yyy_cmd1()和yyy_cmd2(), I2C 设备驱动的command 函数代码如下：


```
static int yyy command (struct i2c client *client, unsigned int cmd, void  *arg)
 
      {
 
        switch (cmd)
 
         {
 
           case YYY CMD1:
 
           return yyy cmd1(client, arg);
 
          case YYY CMD2:
 
             return yyy cmd2(client, arg);
 
          default:
 
            return  - EINVAL;
 
        }
 
     }
```
具体命令的实现是通过 件i2c_msg 消息数 ，并调用I2C 核心的传输、发送和接收函数，由I C 核心的传输、发送和接收函数调用I C 适配器对应的algorithm相关函数来完成的.
4.4 文件操作接口
作为一种字符类设备，Linux I C 设备驱动的文件操作接口与普通的设备驱动是完全一致的，但是在其中要使用i2c_client 、i2c_driver、i2c_adapter 和i2c_algorithm 结构体和I2C 核心，并且对设备的读写和控制需要借助体系结构中各组成部分的协同合作，下面举一个I2C 设备文件的接口写函数的例子：

C
```
static ssize t yyy write (struct file *file, char *buf, size t count, loff t off)
       {
 
       struct       i2c client       *client       =      (struct i2c client*)file->private data;
     
        i2c msg msg[1];
         char *tmp;
         int ret;
         tmp = kmalloc(count, GFP KERNEL);
         if (tmp == NULL)
          return  - ENOMEM;
 
        if (copy from user (tmp, buf, count))
        {
          kfree (tmp);
          return  - EFAULT;
        }
       
        msg[0].addr = client->addr;//地址
        msg[0].flags = 0;               //0 为写
        msg[0].len = count;             //要写的字节数
        msg[0].buf = tmp;               //要写的数据
 
        ret = i2c transfer (client->adapter, msg, 1);  //传输I C 消息
        return (ret == 1) ? count : ret;
      }
```
I2C 设备的写操作经历 了如下几个步骤：
- 从用户空间到字符设备驱动写函数接口，写函数构造I2C 消息数 。
- 写函数把构造的I C 消息数 传递给I2C 核心的传输函数i2c_transfer() 。
- I2 C 核心的传输函数 i2c_tr ansfer() 找到对应适配器 algorithm 的通信方法函数 master_xfer()去最终完成 I2C 消息的处理。
4.5 i2c-dev.c 文件分析
i2c-dev.c 中提供i2cdev_read()、i2cdev_write()函数来对应用户空间要使用的read()和write() 文件操作接口，这两个函数分别调用 I C 核心的 i2c_master_recv() 和i2c_master_send()函数来构造一条I2C 消息并引发适配器algorithm 通信函数的调用， 完成消息的传输.但是i2c-dev.c 中i2cdev_read()和i2cdev_write()函数不具备太强的通用性，没有太大的实用价值，只能适用于非RepStart 模式的情况。对于两条以上消息成的读写，在用户空间需要 织i2c_msg 消息数组并调用I2C_RDWR IOCTL 命令。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
