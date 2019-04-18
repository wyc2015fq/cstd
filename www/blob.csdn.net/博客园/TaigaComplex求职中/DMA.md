# DMA - TaigaComplex求职中 - 博客园







# [DMA](https://www.cnblogs.com/TaigaCon/archive/2012/12/03/2799407.html)





**DMA**(Direct Memory Access，直接内存存取) 

　　一般来说，计算机对内存数据进行处理的时候，需要从内存把数据读进寄存器，然后进行进一步的操作（比如运算处理）。

　　但是有些数据并不需要运算处理这一类型的操作，只是单纯的移动数据，而把数据读进寄存器，然后再把数据从寄存器写进内存会消耗cpu资源，当需要读写大量数据的时候更是如此，DMA技术就很好地解决了这一问题。

　　DMA，顾名思义，不占用cpu资源，从一个硬件存储区域把一部分连续的数据复制到另一个硬件存储区域。其中硬件包括系统总线上的硬件（内存），和外部总线上的硬件（磁盘，iis外设等）。



　　DMA硬件可以说是一个单纯为了进行数据移动而设计的处理器，传输模式跟cpu一样

**SIZE　=　TC  *  DSZ  *  TSZ**

　　SIZE　：传输的总数据量

　　TC　　：数据传输次数

　　DSZ　：一个数据有多少个原子传输

　　TSZ　：一个原子传输有多少字节



**DMA的原子传输会占用系统总线资源**

　　DMA虽然不会占用cpu资源，但是如果DMA的源跟目的都为内存的时候，由于内存位于系统总线上，DMA会占用总线资源，此时由于系统总线忙，cpu会由于得不到总线资源而无法进行跟外部的交流操作。

　　如果DMA的源或目的为内存的时候，我们需要为其分配一块在物理上连续的内存。如果用kmalloc等函数分配的内存，虽然在虚拟地址看起来是连续的，但是物理上可能不连续，因此需要用到分配物理上连续的内存的函数（dma_alloc_writecombine）。

　　为什么要分配连续的内存空间呢，因为平时cpu使用的时候，寻址会经过mmu（内存管理单元）进行虚拟地址到物理地址的转换，而DMA是不经过mmu直接对内存进行读写的，直接对物理地址进行操作。



**使用DMA**

　　当我们要使用到DMA的时候，需要把传输的源（物理地址）、目的（物理地址）、大小告诉DMA，然后启动DMA，就能进行数据移动了。

　　那么什么时候DMA会结束呢，当DMA传输结束的时候会发出一个中断，我们可以在该中断处理程序内部做进一步的操作（如打印消息等）。



以下是DMA测试模块：

基于s3c2440，通过ioctl进行测试

驱动：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


  1 #include <linux/module.h>
  2 #include <linux/kernel.h>
  3 #include <linux/fs.h>
  4 #include <linux/init.h>
  5 #include <linux/delay.h>
  6 #include <linux/device.h>
  7 #include <linux/irq.h>
  8 #include <linux/gpio.h>
  9 #include <linux/dma-mapping.h>
 10 #include <linux/poll.h>
 11 
 12 #include <linux/sched.h>
 13 
 14 #include <linux/interrupt.h>
 15 
 16 #include <asm/uaccess.h>
 17 #include <mach/irqs.h>
 18 #include <asm/io.h>
 19 #include <mach/regs-gpio.h>
 20 #include <mach/hardware.h>
 21 
 22 struct s3c_dma_regs
 23 {
 24     unsigned long disrc;
 25     unsigned long disrcc;
 26     unsigned long didst;
 27     unsigned long didstc;
 28     unsigned long dcon;
 29     unsigned long dstat;
 30     unsigned long dcsrc;
 31     unsigned long dcdst;
 32     unsigned long dmasktrig;
 33 };
 34 
 35 #define MEM_CPY_NO_DMA 0
 36 #define MEM_CPY_DMA         1
 37 
 38 #define BUF_SIZE (512*1024)
 39 
 40 static int major=0;
 41 
 42 static char *src;
 43 static char *dst;
 44 
 45 static u32 src_phys;
 46 static u32 dst_phys;
 47 
 48 static struct class *cls;
 49 static struct class_device *dma_device;
 50 
 51 static volatile struct s3c_dma_regs *dma_regs;
 52 
 53 static DECLARE_WAIT_QUEUE_HEAD(dma_waitq);
 54 /*中断时间标志，中断处理程序将它置1，ioctl将它清0*/
 55 static volatile int ev_dma=0;
 56 
 57 static int  s3c_dma_ioctl(struct inode* inode,struct file *file,unsigned int cmd,unsigned long arg)
 58 {
 59     int i;
 60     
 61     memset(src,0xAA,BUF_SIZE);
 62     memset(src,0x55,BUF_SIZE);
 63     
 64     switch(cmd)
 65     {
 66         case MEM_CPY_NO_DMA:
 67             {
 68                 for(i=0;i<BUF_SIZE;i++)
 69                 dst[i]=src[i];
 70                 
 71                 if(memcmp(src,dst,BUF_SIZE)==0)
 72                     {
 73                         printk("MEM_CPY_NO_DMA OK\n");
 74                     }
 75                 else
 76                     {
 77                         printk("MEM_CPY_NO_DMA ERROR\n");
 78                     }
 79                 break;
 80             }
 81         case MEM_CPY_DMA:
 82             {
 83                 ev_dma=0;
 84                 /*把源、目的、长度告诉dma*/
 85                 dma_regs->disrc        =        src_phys;                                    /*源地址*/
 86                 dma_regs->disrcc    =        (0<<1)|(0<<0);                        /*源位于AHB（系统总线），源地址递增*/
 87                 
 88                 dma_regs->didst        =        dst_phys;                                    /*目的地址*/
 89                 dma_regs->didstc    =        (0<<2)|(0<<1)|(0<<0);            /*目的地址位于AHB，目的地址递增*/
 90                 
 91                                                                                                                 /*长度*/
 92                 dma_regs->dcon        =        (1<<30)|(1<<29)|(0<<28)|(1<<27)|(0<<23)|(0<<20)|(BUF_SIZE<<0);            /*AHB，使能中断，一次传输一个数据,whole service mode,软件触发,一个数据传输一个字节,传输次数*/
 93                 
 94                 /*启动dma*/
 95                 dma_regs->dmasktrig=    (1<<1)|(1<<0);                        /*打开dma，软件触发*/
 96                 
 97                 /*如何知道dma什么时候完成？*/
 98                 /*休眠，等待dma处理完成后发出中断，在中断处理程序中唤醒*/
 99                 wait_event_interruptible(dma_waitq,ev_dma);
100                 
101                 if(memcmp(src,dst,BUF_SIZE)==0)
102                     {
103                         printk("MEM_CPY_DMA OK\n");
104                     }
105                 else
106                     {
107                         printk("MEM_CPY_DMA ERROR\n");
108                     }
109                 
110                 break;
111             }
112     }
113     return 0;
114 }
115 
116 static struct file_operations dma_fops={
117     .owner=THIS_MODULE,
118     .ioctl=s3c_dma_ioctl,
119 };
120 
121 static irqreturn_t s3c_dma_irq(int irq,void *devid)
122 {
123     /*唤醒*/
124     ev_dma=1;
125     wake_up_interruptible(&dma_waitq);
126     return IRQ_HANDLED;
127 }
128 
129 static int s3c_dma_init(void)
130 {
131     if(request_irq(IRQ_DMA3,s3c_dma_irq,0,"s3c_dma",1))
132         {
133             printk("cannot request_irq for dma\n");
134             return -EBUSY;
135         }
136     
137     /*分配dma所用的缓冲区*/
138     src=dma_alloc_writecombine(NULL,BUF_SIZE,&src_phys,GFP_KERNEL);
139     if(NULL==src)
140         {
141             free_irq(IRQ_DMA3,1);
142             printk("can not alloc buffer src\n");
143             return -ENOMEM;
144         }
145     dst=dma_alloc_writecombine(NULL,BUF_SIZE,&dst_phys,GFP_KERNEL);
146         if(NULL==dst)
147         {
148             free_irq(IRQ_DMA3,1);
149             dma_free_writecombine(NULL,BUF_SIZE,src,src_phys);
150             printk("can not alloc buffer dst\n");
151             return -ENOMEM;
152         }
153         
154     major=register_chrdev(0,"s3c_dma",&dma_fops);
155     
156     /*为了自动创建设备节点*/
157     cls=class_create(THIS_MODULE,"s3c_dma");
158     dma_device=device_create(cls,NULL,MKDEV(major,0),NULL,"dma");
159     
160     dma_regs=ioremap(0x4B0000C0,sizeof(struct s3c_dma_regs));
161     
162     return 0;
163 }
164 
165 static void s3c_dma_exit(void)
166 {    
167     iounmap(dma_regs);
168     device_unregister(dma_device);
169     class_destroy(cls);
170     unregister_chrdev(major,"s3c_dma");
171     dma_free_writecombine(NULL,BUF_SIZE,src,src_phys);
172     dma_free_writecombine(NULL,BUF_SIZE,dst,dst_phys);
173     free_irq(IRQ_DMA3,1);
174 }
175 
176 module_init(s3c_dma_init);
177 module_exit(s3c_dma_exit);
178 
179 MODULE_LICENSE("GPL");
```

测试代码：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


 1 #include <stdio.h>
 2 #include <string.h>
 3 #include <sys/types.h>
 4 #include <sys/stat.h>
 5 #include <fcntl.h>
 6 #include <sys/ioctl.h>
 7 
 8 #define MEM_CPY_NO_DMA 0
 9 #define MEM_CPY_DMA         1
10 
11 void print_usage(char *name)
12 {
13     printf("Usage:\n");
14     printf("%s<nodma|dma>\n",name);
15 }
16 
17 int main(int argc,char **argv)
18 {
19     int fd;
20     int count=100;
21     if(argc!=2)
22         {
23             print_usage(argv[0]);
24             return -1;
25         }
26         
27     fd=open("/dev/dma",O_RDWR);
28     if(fd<0)
29         {
30             printf("can't open /dev/dma\n");
31             return -1;
32         }
33         
34     if(strcmp(argv[1],"nodma")==0)
35         {
36             while(count--)
37             {
38                 ioctl(fd,MEM_CPY_NO_DMA);
39             }
40         }
41     else if(strcmp(argv[1],"dma")==0)
42         {
43             while(count--)
44             {
45                 ioctl(fd,MEM_CPY_DMA);
46             }
47         }
48     else
49         {
50             print_usage(argv[0]);
51             return -1;
52         }
53 }
```














