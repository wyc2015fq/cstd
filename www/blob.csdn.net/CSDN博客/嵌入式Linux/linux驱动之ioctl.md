
# linux驱动之ioctl - 嵌入式Linux - CSDN博客

2014年05月19日 08:59:18[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：551个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)


大部分驱动除了需要具备读写设备的能力之外，还需要具备对硬件控制的能力。一、在用户空间，使用**ioctl系统调用来控制设备**，原型如下：
intioctl(intfd,unsignedlongcmd,...);/*fd:文件描述符
cmd:控制命令
...:可选参数:插入*argp，具体内容依赖于cmd*/
用户程序所作的只是通过命令码告诉驱动程序它想做什么，至于**怎么解释这些命令和怎么实现这些命令，这都是驱动程序要做的事情**。
二、**驱动ioctl方法**：
int(*ioctl) (structinode *inode,structfile *filp,unsignedintcmd,unsignedlongarg);/*inode与filp两个指针对应于应用程序传递的文件描述符fd，这和传递open方法的参数一样。
cmd 由用户空间直接不经修改的传递给驱动程序
arg 可选。*/
在驱动程序中实现的ioctl函数体内，实际上是有一个switch {case}结构，每一个case对应一个命令码，做出一些相应的操作。怎么实现这些操作，这是每一个程序员自己的事情，因为设备都是特定的。关键在于怎么样组织命令码，因为在ioctl中命令码是唯一联系用户程序命令和驱动程序支持的途径。
在Linux核心中是这样定义一个**命令码**的：
____________________________________
| 设备类型  | 序列号 |  方向 | 数据尺寸 |
|----------|--------|------|-------- |
| 8 bit　　 |  8 bit   | 2 bit |8~14 bit|
|----------|--------|------|-------- |
这样一来，一个命令就变成了一个整数形式的命令码。但是命令码非常的不直观，所以Linux Kernel中提供了一些宏，这些宏可根据便于理解的字符串生成命令码，或者是从命令码得到一些用户可以理解的字符串以标明这个命令对应的设备类型、设备序列号、数据传送方向和数据传输尺寸。
**1、定义命令：**
内核提供了一些宏来帮助定义命令：
//nr为序号，datatype为数据类型,如int_IO(type, nr )//没有参数的命令_IOR(type, nr, datatype)//从驱动中读数据_IOW(type, nr, datatype)//写数据到驱动_IOWR(type,nr, datatype)//双向传送
定义命令例子:
\#defineMEM_IOC_MAGIC 'm'//定义类型\#defineMEM_IOCSET _IOW(MEM_IOC_MAGIC,0,int)\#defineMEM_IOCGQSET _IOR(MEM_IOC_MAGIC, 1, int)
**2、实现命令：**
定义好了命令，下一步就是要实现ioctl函数了，ioctl的实现包括三个技术环节：
**1）返回值；**
ioctl函数的实现是根据命令执行的一个switch语句，但是，当命令不能匹配任何一个设备所支持的命令时，通常返回-EINVAL(非法参数)；
**2）参数使用；**
用户使用　　int ioctl(int fd,unsinged long cmd,**...**)　　时，**...**就是要传递的参数；
再通过　　int (*ioctl)(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long**arg**)　　中的arg传递；
如果arg是一个**整数**，可以直接使用；
如果是**指针**，我们必须确保这个用户地址是有效的，因此，使用之前需要进行正确检查。
内部有检查的，**不需要检测**的：
copy_from_user
copy_to_user
get_user
put_user
**需要检测**的：
__get_user
__put_user
检测函数**access_ok（）**:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
staticinlineintaccess_ok(inttype,constvoid*addr, unsignedlongsize)/*type :是VERIFY_READ 或者VERIFY_WRITE用来表明是读用户内存还是写用户内存;
addr:是要操作的用户内存地址;
size:是操作的长度。如果ioctl需要从用户空间读一个整数，那么size参数就等于sizeof(int)；
返回值：Access_ok返回一个布尔值：1，是成功（存取没问题）；0，是失败，ioctl返回-EFAULT；*/![复制代码](http://common.cnblogs.com/images/copycode.gif)
**3）命令操作；**
switch(cmd)
{case:
     ... ...
}
三、**ioctl实例分析**：
（1）**memdev.h：**
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)View Code
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/*mem设备描述结构体*/structmem_dev                                     
{char*data;                      
  unsignedlongsize;       
};/*定义幻数*/\#defineMEMDEV_IOC_MAGIC  'k'/*定义命令*/\#defineMEMDEV_IOCPRINT   _IO(MEMDEV_IOC_MAGIC, 1)\#defineMEMDEV_IOCGETDATA _IOR(MEMDEV_IOC_MAGIC, 2, int)\#defineMEMDEV_IOCSETDATA _IOW(MEMDEV_IOC_MAGIC, 3, int)\#defineMEMDEV_IOC_MAXNR 3\#endif/* _MEMDEV_H_ */![复制代码](http://common.cnblogs.com/images/copycode.gif)

**（2）****memdev.c：**（驱动程序）
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)View Code
![复制代码](http://common.cnblogs.com/images/copycode.gif)
staticintmem_major = MEMDEV_MAJOR;
module_param(mem_major,int, S_IRUGO);structmem_dev *mem_devp;/*设备结构体指针*/structcdev cdev;/*文件打开函数*/intmem_open(structinode *inode,structfile *filp)
{structmem_dev *dev;/*获取次设备号*/intnum = MINOR(inode->i_rdev);if(num >= MEMDEV_NR_DEVS)return-ENODEV;
    dev = &mem_devp[num];/*将设备描述结构指针赋值给文件私有数据指针*/filp->private_data = dev;return0; 
}/*文件释放函数*/intmem_release(structinode *inode,structfile *filp)
{return0;
}/*IO操作*/intmemdev_ioctl(structinode *inode,structfile *filp,
                 unsignedintcmd, unsignedlongarg)
{interr =0;intret =0;intioarg =0;/*检测命令的有效性*/if(_IOC_TYPE(cmd) != MEMDEV_IOC_MAGIC)return-EINVAL;if(_IOC_NR(cmd) > MEMDEV_IOC_MAXNR)return-EINVAL;/*根据命令类型，检测参数空间是否可以访问*/if(_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void*)arg, _IOC_SIZE(cmd));elseif(_IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok(VERIFY_READ, (void*)arg, _IOC_SIZE(cmd));if(err)return-EFAULT;/*根据命令，执行相应的操作*/switch(cmd) {/*打印当前设备信息*/caseMEMDEV_IOCPRINT:
          printk("<--- CMD MEMDEV_IOCPRINT Done--->\n\n");break;/*获取参数*/caseMEMDEV_IOCGETDATA: 
        ioarg =1101;
        ret = __put_user(ioarg, (int*)arg);break;/*设置参数*/caseMEMDEV_IOCSETDATA: 
        ret = __get_user(ioarg, (int*)arg);
        printk("<--- In Kernel MEMDEV_IOCSETDATA ioarg = %d --->\n\n",ioarg);break;default:return-EINVAL;
    }returnret;
}/*文件操作结构体*/staticconststructfile_operations mem_fops =
{
  .owner = THIS_MODULE,
  .open = mem_open,
  .release = mem_release,
  .ioctl = memdev_ioctl,
};/*设备驱动模块加载函数*/staticintmemdev_init(void)
{intresult;inti;
  dev_t devno = MKDEV(mem_major,0);/*静态申请设备号*/if(mem_major)
    result = register_chrdev_region(devno,2,"memdev");else/*动态分配设备号*/{
    result = alloc_chrdev_region(&devno,0,2,"memdev");
    mem_major = MAJOR(devno);
  }if(result <0)returnresult;/*初始化cdev结构*/cdev_init(&cdev, &mem_fops);
  cdev.owner = THIS_MODULE;
  cdev.ops = &mem_fops;/*注册字符设备*/cdev_add(&cdev, MKDEV(mem_major,0), MEMDEV_NR_DEVS);/*为设备描述结构分配内存*/mem_devp = kmalloc(MEMDEV_NR_DEVS *sizeof(structmem_dev), GFP_KERNEL);if(!mem_devp)/*申请失败*/{
    result =  - ENOMEM;gotofail_malloc;
  }
  memset(mem_devp,0,sizeof(structmem_dev));/*为设备分配内存*/for(i=0; i < MEMDEV_NR_DEVS; i++) 
  {
        mem_devp[i].size = MEMDEV_SIZE;
        mem_devp[i].data = kmalloc(MEMDEV_SIZE, GFP_KERNEL);
        memset(mem_devp[i].data,0, MEMDEV_SIZE);
  }return0;
  fail_malloc: 
  unregister_chrdev_region(devno,1);returnresult;
}/*模块卸载函数*/staticvoidmemdev_exit(void)
{
  cdev_del(&cdev);/*注销设备*/kfree(mem_devp);/*释放设备结构体内存*/unregister_chrdev_region(MKDEV(mem_major,0),2);/*释放设备号*/}
MODULE_AUTHOR("David Xie");
MODULE_LICENSE("GPL");
module_init(memdev_init);
module_exit(memdev_exit);![复制代码](http://common.cnblogs.com/images/copycode.gif)

**(3)app-ioctl.c**（应用程序）
![复制代码](http://common.cnblogs.com/images/copycode.gif)
\#include <stdio.h>
\#include<sys/types.h>
\#include<sys/stat.h>
\#include<fcntl.h>
\#include"memdev.h"/*包含命令定义*/intmain()
{intfd =0;intcmd;intarg =0;charBuf[4096];/*打开设备文件*/fd = open("/dev/memdev0",O_RDWR);if(fd <0)
    {
        printf("Open Dev Mem0 Error!\n");return-1;
    }/*调用命令MEMDEV_IOCPRINT*/printf("<--- Call MEMDEV_IOCPRINT --->\n");
    cmd = MEMDEV_IOCPRINT;if(ioctl(fd, cmd, &arg) <0)
        {
            printf("Call cmd MEMDEV_IOCPRINT fail\n");return-1;
    }/*调用命令MEMDEV_IOCSETDATA*/printf("<--- Call MEMDEV_IOCSETDATA --->\n");
    cmd = MEMDEV_IOCSETDATA;
    arg =2007;if(ioctl(fd, cmd, &arg) <0)
        {
            printf("Call cmd MEMDEV_IOCSETDATA fail\n");return-1;
    }/*调用命令MEMDEV_IOCGETDATA*/printf("<--- Call MEMDEV_IOCGETDATA --->\n");
    cmd = MEMDEV_IOCGETDATA;if(ioctl(fd, cmd, &arg) <0)
        {
            printf("Call cmd MEMDEV_IOCGETDATA fail\n");return-1;
    }
    printf("<--- In User Space MEMDEV_IOCGETDATA Get Data is %d --->\n\n",arg);    
    
    close(fd);return0;    
}

