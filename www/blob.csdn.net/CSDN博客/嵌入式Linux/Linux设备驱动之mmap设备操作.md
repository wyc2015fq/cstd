
# Linux设备驱动之mmap设备操作 - 嵌入式Linux - CSDN博客

2015年11月19日 14:37:58[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：623


**1.mmap系统调用**
void*mmap(void*addr, size_t len,intprot,intflags,intfd, off_t offset);
功能：负责把文件内容映射到进程的虚拟地址空间，通过对这段内存的读取和修改来实现对文件的读取和修改，而不需要再调用read和write；
参数：addr：映射的起始地址，设为NULL由系统指定；
len：映射到内存的文件长度；
prot：期望的内存保护标志，不能与文件的打开模式冲突。PROT_EXEC,PROT_READ,PROT_WRITE等；
flags：指定映射对象的类型，映射选项和映射页是否可以共享。MAP_SHARED，MAP_PRIVATE等；
fd：由open返回的文件描述符，代表要映射的文件；
offset：开始映射的文件的偏移。
返回值：成功执行时，mmap()返回被映射区的指针。失败时，mmap()返回MAP_FAILED。
**mmap映射图：**
![](http://pic002.cnblogs.com/images/2011/341608/2011120820050810.jpg)

**2.解除映射：**
int munmap(void *start, size_t length);
**3.虚拟内存区域：**
虚拟内存区域是进程的虚拟地址空间中的一个同质区间，即具有同样特性的连续地址范围。一个进程的内存映象由下面几个部分组成：程序代码、数据、BSS和栈区域，以及内存映射的区域。
linux内核使用vm_area_struct结构来描述虚拟内存区。其主要成员：
unsignedlongvm_start;/*Our start address within vm_mm.*/unsignedlongvm_end;/*The first byte after our end address within vm_mm.*/unsignedlongvm_flags;/*Flags, see mm.h. 该区域的标记。如VM_IO（该VMA标记为内存映射的IO区域,会阻止系统将该区域包含在进程的存放转存中）和VM_RESERVED（标志内存区域不能被换出）。*/
**4.mmap设备操作：**
映射一个设备是指把用户空间的一段地址（虚拟地址区间）**关联**到设备内存上，当程序读写这段用户空间的地址时，它实际上是在访问设备。
mmap方法是file_operations结构的成员，在mmap系统调用的发出时被调用。在此之前，内核已经完成了很多工作。
mmap设备方法所需要做的就是建立虚拟地址到物理地址的页表（虚拟地址和设备的物理地址的**关联**通过页表）。
staticintmmap(structfile *file,structvm_area_struct *vma)；
mmap如何完成页表的建立？（两种方法）
（1）使用remap_pfn_range一次建立所有页表。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
intremap_pfn_range(structvm_area_struct *vma, unsignedlongaddr, unsignedlongpfn, unsignedlongsize, pgprot_t prot)；/**
* remap_pfn_range - remap kernel memory to userspace
* @vma: user vma to map to：内核找到的虚拟地址区间
* @addr: target user address to start at：要关联的虚拟地址
* @pfn: physical address of kernel memory：要关联的设备的物理地址，也即要映射的物理地址所在的物理帧号，可将物理地址>>PAGE_SHIFT
* @size: size of map area
* @prot: page protection flags for this mapping
*
* Note: this is only safe if the mm semaphore is held when called.*/![复制代码](http://common.cnblogs.com/images/copycode.gif)

（2）使用nopage VMA方法每次建立一个页表；
**5.源码分析：**
**（1）memdev.h**
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)View
 Code
/*mem设备描述结构体*/structmem_dev                                     
{char*data;                      
  unsignedlongsize;       
};\#endif/* _MEMDEV_H_ */![复制代码](http://common.cnblogs.com/images/copycode.gif)

**（2）memdev.c**
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)View Code
staticintmem_major = MEMDEV_MAJOR;
module_param(mem_major,int, S_IRUGO);structmem_dev *mem_devp;/*设备结构体指针*/structcdev cdev;/*文件打开函数*/intmem_open(structinode *inode,structfile *filp)
{structmem_dev *dev;/*获取次设备号*/intnum = MINOR(inode->i_rdev);if(num >= MEMDEV_NR_DEVS)return-ENODEV;
    dev = &mem_devp[num];/*将设备描述结构指针赋值给文件私有数据指针*/filp->private_data = dev;return0; 
}/*文件释放函数*/intmem_release(structinode *inode,structfile *filp)
{return0;
}staticintmemdev_mmap(structfile*filp,structvm_area_struct *vma)
{structmem_dev *dev = filp->private_data;/*获得设备结构体指针*/vma->vm_flags |= VM_IO;
      vma->vm_flags |= VM_RESERVED;if(remap_pfn_range(vma,vma->vm_start,virt_to_phys(dev->data)>>PAGE_SHIFT, vma->vm_end - vma->vm_start, vma->vm_page_prot))return-EAGAIN;return0;
}/*文件操作结构体*/staticconststructfile_operations mem_fops =
{
  .owner = THIS_MODULE,
  .open = mem_open,
  .release = mem_release,
  .mmap = memdev_mmap,
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

**（3）app-mmap.c**
![复制代码](http://common.cnblogs.com/images/copycode.gif)
\#include <stdio.h>
\#include<sys/types.h>
\#include<sys/stat.h>
\#include<fcntl.h>
\#include<unistd.h>
\#include<sys/mman.h>intmain()
{intfd;char*start;//char buf[100];char*buf;/*打开文件*/fd = open("/dev/memdev0",O_RDWR);
        
    buf = (char*)malloc(100);
    memset(buf,0,100);
    start=mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);/*读出数据*/strcpy(buf,start);
    sleep (1);
    printf("buf 1 = %s\n",buf);/*写入数据*/strcpy(start,"Buf Is Not Null!");
    
    memset(buf,0,100);
    strcpy(buf,start);
    sleep (1);
    printf("buf 2 = %s\n",buf);
       
    munmap(start,100);/*解除映射*/free(buf);
    close(fd);return0;    
}![复制代码](http://common.cnblogs.com/images/copycode.gif)

测试步骤：
（1）编译安装内核模块：insmod memdev.ko
（2）查看设备名、主设备号：cat /proc/devices
（3）手工创建设备节点：mknod/dev/memdev0  c  ***  0
查看设备文件是否存在：ls -l /dev/* | grep memdev
（4）编译下载运行应用程序：./app-mmap
结果：buf 1 =
buf 2 =Buf Is Not Null!

总结：mmap设备方法实现将用户空间的一段内存关联到设备内存上，对用户空间的读写就相当于对字符设备的读写；不是所有的设备都能进行mmap抽象，比如像串口和其他面向流的设备就不能做mmap抽象

