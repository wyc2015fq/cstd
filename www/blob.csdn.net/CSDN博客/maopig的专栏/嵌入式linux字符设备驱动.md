# 嵌入式linux字符设备驱动 - maopig的专栏 - CSDN博客
2012年03月30日 10:42:18[maopig](https://me.csdn.net/maopig)阅读数：1113标签：[linux																[嵌入式																[struct																[makefile																[class																[module](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/maopig/article/category/850267)
1. 我们需要先调用register_chrdev_region()或 alloc_chrdev_region()来向系统申请设备号
    int register_chrdev_region( dev_t first, unsigned int count, char *name );  //函数通过已知的设备号first来注册字符设备区域。
    int alloc_chrdev_region( dev_t *dev, unsigned int firstminor,  unsigned int count, char *name );  //若需要动态分配设备号，则使用该函数，此处dev作为指针，为仅用于输出的参数，分配成功后保存已分配范围的第一个编号。firstminor通常为0，表示第一个次设备号。
2.释放设备号
    void unregister_chrdev_region( dev_t first, unsigned int count );
3.注册完字符设备区域之后
    需要使用cdev_init函数初始化cdev结构和 file_operations结构
    cdev->owner = THIS_MODULE;
    void cdev_init( struct cdev *cdev, struct file_operations *fops);  //为cdev结构指针创建内存，并将cdev->ops=fops;(**抵岸科技注：**源代码中并未见创建内存。cdev_init依次做了：cdev填充0，初始化表头，kobject成员初始化，将cdev->ops=fops)
    接着讲cdev加入到内核中    int cdev_add( struct cdev *dev, dev_t num, unsigned int count);
    完成字符设备的注册过程
    以上函数需要#include  <linux/cdev.h>
4. 自动创建字符设备节点
    下面的函数需要  #include <linux/device.h>.
    struct class *myclass;  //声明一个class结构用于创建字符设备节点
    myclass = class_create(THIS_MODULE,"myclass");  //"myclass"为类名
    device_create(myclass,NULL,devno,NULL,"leds%d",0);  //通过myclass结构创建字符设备，第二个参数是设备的parent,第三个参
    数是设备号,第四个参数是传入内核的驱动数据void *类型指针，如果没有可以设置成NULL,最后一个参数是const char* fmt, ...,就是
    格式化参数，即leds0
    完成上述过程后会在/dev中找到leds0设备
    貌似以前的老版本会用到devfs_mk_cdev来创建设备节点，但是最后发现linux-2.6.33并没有这个函数，应该是被上述函数取代
    了吧
5. 当然，还包括驱动Makefile的编写以及应用程序
    几处比较有用的参考文章：
[http://hi.baidu.com/hust_junejun/blog/item/d2dcfcedc09833d92e2e213f.html](http://hi.baidu.com/hust_junejun/blog/item/d2dcfcedc09833d92e2e213f.html)    //比较全面
[http://www.dzsc.com/data/html/2009-8-28/78412.html](http://www.dzsc.com/data/html/2009-8-28/78412.html)                                        //自动生成字符设备节点
[http://fanyihui1986.blog.163.com/blog/static/7844859200911325826783/](http://fanyihui1986.blog.163.com/blog/static/7844859200911325826783/)        //小型总结  
6.TIPS
    printk(KERN_WARNING" this is the test ");
    KERN_ALERT
    KERN_WANING
    KERN_INFO
    KERN_ERR
    等等不同优先级 宏，具体可以查阅linux设备驱动程序三，字符设备驱动一章。
至于mknod console c 5 1 的意思也很相似：
console是设备的名字 
c指字符设备 
5是该设备在major.h中定义的标记 
1是第一个子设备
mknod console c 5 1
console为设备文件名，自己随便取 
c是指字符型设备 (可选b，块设备) 
5是主设备号 /dev/devices里面记录现有的设备 
找个没有用的就可以了 
1是次设备号，当你要给两个同样的设备上一个驱动的时候就要分了，从0开始，1就是第二个了 ，对于字符设备，次设备号就表示同类型设备的不同设备。

