# 蜕变成蝶：Linux设备驱动之watchdog设备驱动 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
看门狗（watchdog ）分硬件看门狗和软件看门狗。硬件看门狗是利用一个定时器 电路，其定时输出连接到电路的复位端，程序在一定时间范围内对定时器清零 （俗称 “喂狗”），如果程序出现故障，不在定时周期内复位看门狗，就使得看门狗定时器溢出产生复位信号 并重启系统。软件看门狗原理上一样，只是将硬件电路上的定时器用处理器的内部定 时器代替。
1 看门狗的三个寄存器
1.1 watchdog原理
S3C2410内部集成了watchdog，提供3 个寄存器对watchdog 进行操作，这3 个寄存器分别为WTCON （watchdog 控制寄存器）、WTDAT （watchdog 数据寄存器）和WTCNT(watchdog 记数寄存器) S3c2440的看门狗的原理框图如下：
![](http://jbcdn2.b0.upaiyun.com/2016/05/ac22f1d9d2745eb3f7137d96044324ea.jpg)
可以看到，看门狗定时器的频率由PCLK提供，其预分频器最大取值为255+1；另外，通过MUX，可以进一步降低频率。定时器采用递减模式，一旦到0，则可以触发看门狗中断以及RESET复位信号。 看门狗定时器的频率的计算公式如下：

C
```
t_watchdog = 1/[PCLK/(Prescaler value + 1)/Division_factor]
```
1.2 开启S3C2410 的看门狗

C
```
void enable watchdog ()
     {
                
       rWTCON = WTCON DIV64 | WTCON RSTEN;//64分频、开启复位信号
       rWTDAT = 0x8000;//计数目标
                 
       rWTCON |= WTCON ENABLE;//开启看门狗
      }
```
1.3 S3C2410 的看门狗 “喂狗”

C
```
void feed dog ()
{
    rWTCNT=0x8000;
}
```
1.4 看门狗的使用例程

C
```
void main ()
 
     {
    
       init system ();
 
       ...
            
       enable watchdog ();//启动看门狗
 
       ...
 
       while (1)
 
       {
 
          ...
 
             
         feed dog (); //喂狗
 
      }
 
    }
```
2 watchdog中的数据结
有一类设备被称为 “平台设备”，通常 SoC 系统中集成的独立的外设单元都被当作平台设备处理
2.1 platform_device 结构体

C
```
struct platform device
 
     {
 
       const char  * name;//设备名
 
       u32      id;
 
       struct device dev;
            
       u32      num resources;//设备所使用各类资源数量
 
       struct resource * resource;//资源
 
     };
```
2.2 S3C2410 中的平台设备

C
```
struct platform device *s3c24xx uart devs[];
struct platform device s3c device usb; //USB 控制器
                  
       struct platform device s3c device lcd; //LCD 控制器
                      
       struct platform device s3c device wdt; //看门狗
                                                       2
                       
       struct platform device s3c device i2c; //I C 控制器
                      
       struct platform device s3c device iis; //IIS
                   
       struct platform device s3c device rtc; //实时钟
        ...
 
      /*SMDK2410开发板使用的平台设备*/
 
                         
      static struct platform device *smdk2410 devices[]  initdata =
 
      {
        
         &s3c device usb, //USB
             
         &s3c device lcd, //LCD
               
         &s3c device wdt, //看门狗
                               
         &s3c device i2c, //I C
              
         &s3c device iis,  //IIS
 
      };
```
2.3 S3C2410 看门狗的platform_device 结构体

C
```
struct platform device s3c device wdt =
 
      {
 
        .name = "s3c2410-wdt",  //设备名
 
        .id =  - 1, .
   
       num resources = ARRAY SIZE (s3c wdt resource),  //资源数量
                  
        .resource = s3c wdt resource,  //看门狗所使用资源
 
      };
```
2.4 int platform_add_devices()函数

C
```
int platform add devices(struct platform device **devs, int num)
 
      {
 
        int i, ret = 0;
 
        for (i = 0; i < num; i++)
 
         {
ret = platform device register(devs[i]);/*注册平台设备*/
 
          if (ret) /*注册失败*/
 
          {
 
           while (--i >= 0)
    
              platform device unregister(devs[i]);/*注销已经注册的平台设备 */
 
           break;
 
         }
 
        }
 
       return ret;
 
     }
```
2.5 platform_driver 结构体

C
```
struct platform driver
      {
                                        
        int (*probe)(struct platform device *);//探测
                                 
        int (*remove)(struct platform device *);//移除
                                     
       void (*shutdown)(struct platform device *);//关闭
                                    
        int (*suspend)(struct platform device *, pm message t state);// 挂起
                    
        int (*resume)(struct platform device *);//恢复
            
        struct device driver driver;
 
       };
```
2.6 S3C2410 看门狗的platform_driver 结构体

C
```
static struct platform driver s3c2410wdt driver =
 
      {
                          
       .probe      = s3c2410wdt probe,//S3C2410看门狗探测
 
                                  
       .remove     = s3c2410wdt remove,// S3C2410看门狗移除
 
                                  
       .shutdown   = s3c2410wdt shutdown,//S3C2410看门狗关闭
 
                                  
       .suspend    = s3c2410wdt suspend,//S3C2410看门狗挂起
 
                           
        .resume     = s3c2410wdt resume, //S3C2410看门狗恢复
 
        .driver     = {
 
                     
            .owner  = THIS MODULE,
 
           .name   = "s3c2410-wdt",//设备名
 
      },
}；
```
2.7 S3C2410 看门狗所用资源

C
```
static struct resource s3c wdt resource[] =
 
       {
 
         [0] =
 
        {
 
          .start = S3C24XX PA WATCHDOG,     //看门狗I/O 内存开始位置
 
         .end = S3C24XX PA WATCHDOG + S3C24XX SZ WATCHDOG - 1,
 
              //看门狗I/O 内存结束位置
 
          .flags = IORESOURCE MEM,  //I/O 内存资源
 
        } ,
 
        [1] =
 
       {
 
         .start = IRQ WDT, //看门狗开始IRQ 号
 
         .end = IRQ WDT, //看门狗结束IRQ 号
 
         .flags = IORESOURCE IRQ,  //IRQ 资源
 
        }
 
      };
```
2.8 S3C2410 看门狗驱动的miscdevice 结构体

C
```
static struct miscdevice s3c2410wdt miscdev =
 {
                      
        .minor      = WATCHDOG MINOR,//次设备号
 
        .name       = "watchdog",
 
       .fops       = &s3c2410wdt fops,//文件操作结构体
 
};
```
2.9 S3C2410 看门狗驱动的文件操作结构体

C
```
static struct file operations s3c2410wdt fops =
{
             
        .owner      = THIS MODULE,
        
        .llseek     = no llseek,    //seek
                       
        .write      = s3c2410wdt write,     //写函数
                             
        .ioctl      = s3c2410wdt ioctl, //ioctl 函数
                       
        .open       = s3c2410wdt open,  //打开函数
                      
        .release    = s3c2410wdt release,//释放函数
 
};
```
3 加载和卸载函数
驱动模块的加载和卸载函数分别 用 platform_driver_register() 和 platform_driver_ unregister()注册和注销platform_driver
3.1 S3C2410 看门狗驱动

C
```
static int     __init watchdog__init (void)
      {
       printk (banner);
         
        return   platform driver register(&s3c2410wdt driver);//       注 册platform driver
      }
       
      static void     exit watchdog exit (void)
       {
platform driver unregister (&s3c2410wdt driver);//          注  销 platform driver
     }
```
4 探测和移除函数
4.1 探测函数

C
```
static int s3c2410wdt probe (struct platform device *pdev)
      {
        struct resource *res;
        int started = 0;
        int ret;
        int size;
      
        DBG ("%s: probe=%p\n", _ _FUNCTION_ _, pdev);
       
       /* 获得看门狗的内存区域 */
       
                      
       res = platform get resource (pdev, IORESOURCE MEM, 0);
       if (res == NULL)
        {
                 
        printk(KERN INFO PFX "failed to get memory region resouce\n");
         return  - ENOENT;
        }
       
       size = (res->end - res->start) + 1;
       //申请I/O 内存
           
       wdt mem = request mem region (res->start, size, pdev->name);
             
       if (wdt mem == NULL)
        {
             
         printk(KERN INFO PFX "failed to get memory region\n");
         return  - ENOENT;
        }
       
           
       wdt base = ioremap (res->start, size); //设备内存->虚拟地址
               
       if (wdt base == 0)
        {
                    
         printk(KERN INFO PFX "failed to ioremap () region\n");
         return  - EINVAL;
        }
       
                            
       DBG ("probe: mapped wdt base=%p\n", wdt base);
       
       /* 获得看门狗的中断 */
       
                    
       res = platform get resource (pdev, IORESOURCE IRQ, 0);
       if (res == NULL)
{
                   
         printk(KERN INFO PFX "failed to get irq resource\n");
         return  - ENOENT;
        }
       //申请中断
               
         ret = request irq (res->start, s3c2410wdt irq, 0, pdev->name,
pdev);
       if (ret != 0)
        {
              
         printk(KERN INFO PFX "failed to install irq (%d)\n", ret);
         return ret;
        }
       
            
       wdt clock = clk get (&pdev->dev, "watchdog"); //获得看门狗时钟源
          
       if (wdt clock == NULL)
        {
                   
         printk(KERN INFO PFX "failed to find watchdog clock source\n");
         return  - ENOENT;
        }
       
           
       clk enable (wdt clock);
       
       /* 看看是否能设置定时器的超时时间为期望的值，如果不能，使用缺省值 */
       
                        
       if (s3c2410wdt set heartbeat (tmr margin))
        {
                               
         started = s3c2410wdt set heartbeat (
                  
             CONFIG S3C2410 WATCHDOG DEFAULT TIME);
         if (started == 0)
         {
                       
           printk (KERN INFO PFX "tmr margin value out of range, default
%d
                              
             used\n",CONFIG S3C2410 WATCHDOG DEFAULT TIME);
         }
         else
         {
                      
           printk (KERN INFO PFX
             "default timer value is out of range, cannot start\n");
         }
        }
       
      //注册miscdevice
                     
       ret = misc register(&s3c2410wdt miscdev);
       if (ret)
        {
                       
         printk(KERN ERR PFX "cannot registermiscdev on minor=%d (%d)\n",
                      
           WATCHDOG MINOR, ret);
         return ret;
        }
       
               
       if (tmr atboot && started == 0)
        {
                     
         printk(KERN INFO PFX "Starting Watchdog Timer\n");
                 
        s3c2410wdt start ();
        }
       
       return 0;
     }
```
4.2 探测函数

C
```
static int s3c2410wdt remove (struct platform device *dev)
 
      {
    
        if (wdt mem != NULL)
 
        {
        
          release resource (wdt mem); //释放资源
 
                     
          kfree (wdt mem);//释放内存
 
  
          wdt mem = NULL;
 
       }
 
      //释放中断
 
             
   if (wdt irq != NULL)
 
       {
 
        
         free irq (wdt irq->start, dev);
 
       
         wdt irq = NULL;
}
 
       //禁止时钟源
 
 
       if (wdt clock != NULL)
 
       {
 
         clk disable (wdt clock);
 
         clk put (wdt clock);
 
         wdt clock = NULL;
 
        }
    misc deregister(&s3c2410wdt miscdev);//注销miscdevice
 
       return 0;
 
    }
```
5 挂起和恢复函数
5.1 挂起函数

C
```
static   int  s3c2410wdt suspend (struct    platform device   *dev,
 
pm message t state)
      {
        /* 保存看门狗状态，停止它 */
            
       wtcon save = readl (wdt base + S3C2410 WTCON);
           
       wtdat save = readl (wdt base + S3C2410 WTDAT);
       
              
        s3c2410wdt stop ();
       
        return 0;
     }
```
5.2 恢复函数

C
```
static int s3c2410wdt resume (struct platform device *dev)
      {
        /* 恢复看门狗状态 */
                
        writel (wtdat save, wdt base + S3C2410 WTDAT);
                  
        writel (wtdat save, wdt base + S3C2410 WTCNT);
                 
        writel (wtcon save, wdt base + S3C2410 WTCON);
       
                   
        printk (KERN INFO PFX "watchdog %sabled\n", (wtcon save
                 
          &S3C2410 WTCON ENABLE)? "en" : "dis");
      
       return 0;
     }
```
6 打开和释放函数
6.1 打开函数

C
```
static int s3c2410wdt open (struct inode *inode, struct file *file)
      {
               
        if (down trylock (&open lock))  //获得打开锁
          return  - EBUSY;
      
        if (nowayout)
        {
         
            module get (THIS MODULE);
        }
       else
        {
                 
         allow close = CLOSE STATE ALLOW;
        }
       
       /* 启动看门狗 */
                    
       s3c2410wdt start ();
                          
       return nonseekable open (inode, file);
     }
```
6.2 释放函数

C
```
static int s3c2410wdt release (struct inode *inode,struct file *file)
      {
        /* 停止看门狗 */
                 
        if (allow close == CLOSE STATE ALLOW)
        {
                 
          s3c2410wdt stop ();
       }
        else
        {
                 
              printk (KERN CRIT  PFX  "Unexpected  close,  not  stopping
watchdog!\n");
            
         s3c2410wdt keepalive ();
        }
       
             
       allow close = CLOSE STATE NOT;
              
       up (&open lock);  //释放打开锁
       return 0;
     }
```
7 启停watchdog函数和写函数
7.1 启停看门狗函数

C
```
/*停止看门狗*/
                            
       static int s3c2410wdt stop (void)
       {
        unsigned long wtcon;
       
                
        wtcon = readl(wdt base + S3C2410 WTCON);
        //停止看门狗，禁止复位
                        
        wtcon &= ~ (S3C2410 WTCON ENABLE | S3C2410 WTCON RSTEN);
                        
        writel (wtcon, wdt base + S3C2410 WTCON);
       
        return 0;
      }
       
      /*开启看门狗*/
                    
      static int s3c2410wdt start (void)
      {
        unsigned long wtcon;
       
             
        s3c2410wdt stop ();
       
                        
        wtcon = readl(wdt base + S3C2410 WTCON);
        //使能看门狗，128 分频
                      
        wtcon |= S3C2410 WTCON ENABLE | S3C2410 WTCON DIV128;
       
               
        if (soft noboot)
        {
                        
         wtcon |= S3C2410 WTCON INTEN; //使能中断
                          
         wtcon &= ~S3C2410 WTCON RSTEN; //禁止复位
        }
        else
        {
                         
         wtcon &= ~S3C2410 WTCON INTEN; //禁止中断
                      
         wtcon |= S3C2410 WTCON RSTEN; //使能复位
        }
       
                   
        DBG ("%s: wdt count=0x%08x, wtcon=%08lx\n",        FUNCTION    ,
            
          wdt count, wtcon);
                   
        writel (wdt count, wdt base + S3C2410 WTDAT);
                
        writel (wdt count, wdt base + S3C2410 WTCNT);
                    
        writel (wtcon, wdt base + S3C2410 WTCON);
       
        return 0;
      }
```
7.2 写函数

C
```
static ssize t s3c2410wdt write (struct file *file, const char      user *data,
        
        size t len, loff t *ppos)
       {
        /* 刷新看门狗 */
        if (len)
         {
          if (!nowayout)
{
 
            size t i;
              
           allow close = CLOSE STATE NOT;
           for (i = 0; i != len; i++)
           {
             char c;
                  
             if (get user(c, data + i))//用户空间->内核空间
               return  - EFAULT;
             if (c == 'V ')  //如果写入了'V ' ，允许关闭
                
               allow close = CLOSE STATE ALLOW;
           }
         }
             
         s3c2410wdt keepalive ();
        }
       return len;
     }
```
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
