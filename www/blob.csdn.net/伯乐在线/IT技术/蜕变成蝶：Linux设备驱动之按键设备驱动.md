# 蜕变成蝶：Linux设备驱动之按键设备驱动 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
在上述的驱动系列博客中，我们已经了解了关于阻塞和非阻塞、异步通知、轮询、内存和I/O口访问、并发控制等知识，按键设备驱动相对来说是比较简单的，本章内容可以加深我们对字符设备驱动架构、阻塞与非阻塞、中断定时器等相关知识的理解。在嵌入式的系统中，按键的硬件原理简单，就是通过一个上拉电阻将处理器的外部中断引脚拉高，电阻的另一端接按钮并接地就可以实现。
### 1.按键的确认流程如下
![](http://images.cnitblog.com/blog2015/563126/201505/121701070956545.png)
### 2 按键驱动中的有关数据结构
### 2.1 按键设备结构体以及定时器

C
```
#define MAX KEY BUF 16 // 键缓冲区大小                                  
typedef unsigned char KEY RET;
 
//设备结构体：
typedef struct
{                              
    unsigned int keyStatus[KEY NUM]; //4个 键的 键状态
    KEY RET buf[MAX KEY BUF]; // 键缓冲区
    unsigned int head, tail; // 键缓冲区头和尾
    wait queue head t wq; //等待队列
    struct cdev cdev;      //cdev 结构体  
} KEY DEV;
 
static struct timer list key timer[KEY NUM];//4个 键去抖定时器
```
### 2.2 按键硬件资源、键值信息结构体

C
```
static struct key info
     {
               
       int irq no;      //中断号
                          
       unsigned int gpio port; //GPIO端口
                 
       int key no;     //键值
          
     } key info tab [4] =
     {
        /* 键所使用的CPU 资源*/
       {  IRQ EINT10, GPIO G2, 1
       }
      ,
       {
        
        IRQ EINT13, GPIO G5, 2
       }
      ,
       {
            
        IRQ EINT14, GPIO G6, 3
       }
      ,
       {
                    
        IRQ EINT15, GPIO G7, 4
       }
      ,
    };
```
### 2.3 按键设备驱动文件操作结构体

C
```
static struct file operations s3c2410 key fops =
     {
                  
       owner: THIS MODULE,
                     
       open: s3c2410 key open,  //启动设备
                      
       release: s3c2410 key release,  //关闭设备
                     
       read: s3c2410 key read,  //读取 键的键值
     };
```
### 3 按键设备的模块加载和卸载函数
3.1 加载函数

C
```
static int    init s3c2410 key init (void)
      {
        ...//申请设备号，添加cdev
      
                  
        request irqs(); //注册中断函数
        keydev .head = keydev .tail = 0; //初始化结构体
                             
        for (i = 0; i < KEY NUM; i++)
                                            
          keydev.keyStatus[i] = KEYSTATUS UP;
           
        init waitqueue head (&(keydev .wq)); //等待队列
       
       //初始化定时器，实现软件的去抖动
                           
       for (i = 0; i < KEY NUM; i++)
             
         setup timer (&key timer[i], key timer handler, i);
       //把 键的序号作为传入定时器处理函数的参数
     }
```
### 3.2 卸载函数

C
```
static void     exit s3c2410 key exit (void)
     {
             
       free irqs(); //注销中断
       ...//释放设备号，删除cdev
     }
```
### 3.3 中断申请函数

C
```
/*申请系统中断，中断方式为下降沿触发*/
                            
      static int request irqs(void)
      {
                     
        struct key info *k;
        int i;
                               
        for (i= 0; i < sizeof(key info tab) / sizeof(key info tab [1]); i++)
        {
                 
          k = key info tab + i;
           
          set external irq (k->irq no, EXT LOWLEVEL, GPIO PULLUP DIS);
                      //设置低电平触发
                       
              if   (request irq (k->irq no,  &buttons irq,  SA INTERRUPT,
     
DEVICE NAME,
           i))  //申请中断，将 键序号作为参数传入中断服务程序
         {
           return  - 1;
         }
       }
       return 0;
     }
```
### 3.4 中断释放函数

C
```
/*释放中断*/
                         
      static void free irqs(void)
      {
                  
        struct key info *k;
        int i;
                             
        for (i= 0; i < sizeof(key info tab) / sizeof(key info tab [1]); i++)
        {
                  
          k = key info tab + i;
             
          free irq (k->irq no, buttons irq); //释放中断
       }
     }
```
### 4 按键设备驱动中断和定时器处理程序
在按键按下之后，将发生中断，在中断处理程序中，应该先关闭中断进去查询模式，延时以消抖如下中断处理过程只有顶半部，没有底半部。
4.1 中断处理程序

C
```
static void s3c2410 eint key (int irq, void *dev id, struct pt regs
*reg)
     {
                   
       int key = dev id;
             
       disable irq (key info tab [key].irq no); //关中断，转入查询 式
       
                                         
       keydev.keyStatus[key] = KEYSTATUS DOWNX;//状态为按下
           _                    
       key timer [key].expires == jiffies + KEY TIMER DELAY1;//延迟
   
       add timer (&key timer[key]); //启动定时器
     }
```
### 4.2 定时器处理流程
按键按下时，该按键将记录字啊缓冲区，同时定时器启动延时，每次记录新的键值时，等待队列被唤醒，其代码如下。

C
```
//按键设备驱动的定时器处理函数
static void key timer handler (unsigned long data)
      {
        int key = data;
         
        if (ISKEY DOWN (key))
        {
                    
          if (keydev.keyStatus[key] == KEYSTATUS DOWNX)
          //从中断进入
          {
                          
            keydev .keyStatus[key] = KEYSTATUS DOWN;
              
           key timer[key].expires == jiffies + KEY TIMER DELAY; //延迟
           keyEvent ();  //记录键值，唤醒等待队列
             
           add timer(&key timer [key]);
         }
         else
         {
              
           key timer[key].expires == jiffies + KEY TIMER DELAY; //延迟
              
           add timer(&key timer [key]);
         }
       }
       else       //键已抬起
       {
                                              
         keydev.keyStatus[key] = KEYSTATUS UP;
             
         enable irq (key info tab [key].irq no);
       }
```
### 5 打开和释放函数
这里主要是设置keydev.head和keydev.tail还有按键事件函数指针keyEvent的值，按键设备驱动的打开、释放函数如下：

C
```
static int s3c2410 key open (struct inode *inode, struct file *filp)
      {
        keydev .head = keydev .tail = 0; //清空 键动作缓冲区
                                                            
        keyEvent = keyEvent raw; //函数指针指向 键处理函数keyEvent raw
        return 0;
      }
      
                          
       static int s3c2410 key release (struct inode *inode, struct file *filp)
      {
                           
       keyEvent = keyEvent dummy; //函数指针指向空函数
       return 0；
　　　}
```
### 6 读函数
读函数主要是提供对按键设备结构体缓冲区的读并复制到用户空间，当keydev.head != keydev.tail时，说明缓冲区有数据，使用copy_to_user()函数拷贝到用户空间，反之根据用户空间是阻塞还是非阻塞读分为以下两种情况：
- 非阻塞读：没有按键缓存，直接返回- EAGAIN；
- 阻塞读：在keydev.wq等待队列上睡眠，直到有按键记录 到缓冲区后被唤醒。

C
```
//按键设备驱动的读函数
 
static ssize t s3c2410 key read (struct file *filp,char *buf,ssize tcount, loff t*ppos)
      {
        retry: if (keydev.head != keydev .tail)
        //当前循环队列中有数据
        {
               
          key ret = keyRead (); //读取按键
                
          copy to user(..); //把数据从内核空间传送到用户空间
        }
       else
       {
                    
         if (filp->f flags &O NONBLOCK)
         //若用户采用非阻塞方式读取
         {
           return  - EAGAIN;
         }
                       
         interruptible sleep on (&(keydev .wq));
           //用户采用阻塞方式读取，调用该函数使进程睡眠
         goto retry;
       }
       return 0;
     }
```
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
