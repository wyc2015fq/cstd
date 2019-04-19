# 蜕变成蝶：Linux设备驱动之字符设备驱动 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
**一、linux系统将设备**分为3类：字符设备、块设备、网络设备。使用驱动程序：
![](http://images.cnitblog.com/blog2015/563126/201504/150811473704397.jpg)
- **字符设备**：是指只能一个字节一个字节读写的设备，不能随机读取设备内存中的某一数据，读取数据需要按照先后数据。字符设备是面向流的设备，常见的字符设备有鼠标、键盘、串口、控制台和LED设备等。
- **块设备**：是指可以从设备的任意位置读取一定长度数据的设备。块设备包括硬盘、磁盘、U盘和SD卡等。
每一个字符设备或块设备都在/dev目录下对应一个设备文件。linux用户程序通过设备文件（或称设备节点）来使用驱动程序操作字符设备和块设备。
**二、字符设备、字符设备驱动与用户空间访问该设备的程序三者之间的关系。**
**![](http://images.cnitblog.com/blog2015/563126/201504/150813012455928.png)**
如图，在Linux内核中使用cdev结构体来描述字符设备，通过其成员dev_t来定义设备号（分为主、次设备号）以确定字符设备的唯一性。通过其成员file_operations来定义字符设备驱动提供给VFS的接口函数，如常见的open()、read()、write()等。
在Linux字符设备驱动中，模块加载函数通过register_chrdev_region( ) 或alloc_chrdev_region( )来静态或者动态获取设备号，通过cdev_init( )建立cdev与file_operations之间的连接，通过cdev_add( )向系统添加一个cdev以完成注册。模块卸载函数通过cdev_del( )来注销cdev，通过unregister_chrdev_region( )来释放设备号。
用户空间访问该设备的程序通过Linux系统调用，如open( )、read( )、write( )，来“调用”file_operations来定义字符设备驱动提供给VFS的接口函数。
**三、字符设备驱动模型**
**![](http://images.cnitblog.com/blog2015/563126/201504/150815039953294.png)**
**1. 驱动初始化**
**1.1. 分配cdev**
在2.6的内核中使用cdev结构体来描述字符设备，在驱动中分配cdev,主要是分配一个cdev结构体与申请设备号，以按键驱动为例：

C
```
/*……*/
/* 分配cdev*/
struct cdev btn_cdev;
/*……*/
/* 1.1 申请设备号*/
    if(major){
        //静态
        dev_id = MKDEV(major, 0);
        register_chrdev_region(dev_id, 1, "button");
    } else {
        //动态
        alloc_chardev_region(&dev_id, 0, 1, "button");
        major = MAJOR(dev_id);
    }
/*……*/
```
从上面的代码可以看出，申请设备号有动静之分，其实设备号还有主次之分。
在Linux中以主设备号用来标识与设备文件相连的驱动程序。次编号被驱动程序用来辨别操作的是哪个设备。cdev 结构体的 dev_t 成员定义了设备号，为 32 位，其中高 12 位为主设备号，低20 位为次设备号。
设备号的获得与生成：
获得：主设备号：MAJOR(dev_t dev);
次设备号：MINOR(dev_t dev);
生成：MKDEV(int major,int minor);
设备号申请的动静之分：
静态：

C
```
int register_chrdev_region(dev_t from, unsigned count, const char *name)；
/*功能：申请使用从from开始的count 个设备号(主设备号不变，次设备号增加）*/
```
静态申请相对较简单，但是一旦驱动被广泛使用,这个随机选定的主设备号可能会导致设备号冲突，而使驱动程序无法注册。
动态：

C
```
int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,const char *name)；
/*功能：请求内核动态分配count个设备号，且次设备号从baseminor开始。*/
```
动态申请简单，易于驱动推广，但是无法在安装驱动前创建设备文件（因为安装前还没有分配到主设备号）。
**1.2. 初始化cdev**


C
```
void cdev_init(struct cdev *, struct file_operations *); 
cdev_init()函数用于初始化 cdev 的成员，并建立 cdev 和 file_operations 之间的连接。
```
**1.3. 注册cdev**


C
```
int cdev_add(struct cdev *, dev_t, unsigned);
cdev_add()函数向系统添加一个 cdev，完成字符设备的注册。
```
**1.4. 硬件初始化**
硬件初始化主要是硬件资源的申请与配置，以TQ210的按键驱动为例：

C
```
/* 1.4 硬件初始化*/
//申请GPIO资源
gpio_request(S5PV210_GPH0(0), "GPH0_0");
//配置输入
gpio_direction_input(S5PV210_GPH0(0));
```
**  2.实现设备操作**
用户空间的程序以访问文件的形式访问字符设备，通常进行open、read、write、close等系统调用。而这些系统调用的最终落实则是file_operations结构体中成员函数，它们是字符设备驱动与内核的接口。以TQ210的按键驱动为例：

C
```
/*设备操作集合*/
static struct file_operations btn_fops = {
    .owner = THIS_MODULE,
    .open = button_open,
    .release = button_close,
    .read = button_read
};
```
上面代码中的button_open、button_close、button_read是要在驱动中自己实现的。file_operations结构体成员函数有很多个，下面就选几个常见的来展示：
**2.1. open（）函数**
原型：

C
```
int(*open)(struct inode *, struct file*); 
/*打开*/
```
** 2.2. read( )函数**
原型：

C
```
ssize_t(*read)(struct file *, char __user*, size_t, loff_t*); 
/*用来从设备中读取数据，成功时函数返回读取的字节数，出错时返回一个负值*/
```
**2.3. write( )函数**
原型：

C
```
ssize_t(*write)(struct file *, const char__user *, size_t, loff_t*);
/*向设备发送数据，成功时该函数返回写入的字节数。如果此函数未被实现，
当用户进行write()系统调用时，将得到-EINVAL返回值*/
```
**2.4. close( )函数**
原型：


C
```
int(*release)(struct inode *, struct file*); 
/*关闭*/
```
**2.5. 补充说明**
1. 在Linux字符设备驱动程序设计中，有3种非常重要的数据结构：struct file、struct inode、struct file_operations。
struct file 代表一个打开的文件。系统中每个打开的文件在内核空间都有一个关联的struct file。它由内核在打开文件时创建, 在文件关闭后释放。其成员loff_t f_pos 表示文件读写位置。
struct inode 用来记录文件的物理上的信息。因此,它和代表打开文件的file结构是不同的。一个文件可以对应多个file结构,但只有一个inode结构。其成员dev_t i_rdev表示设备号。
struct file_operations 一个函数指针的集合，定义能在设备上进行的操作。结构中的成员指向驱动中的函数,这些函数实现一个特别的操作, 对于不支持的操作保留为NULL。
2. 在read( )和write( )中的buff 参数是用户空间指针。因此,它不能被内核代码直接引用，因为用户空间指针在内核空间时可能根本是无效的——没有那个地址的映射。因此，内核提供了专门的函数用于访问用户空间的指针：

C
```
unsigned long copy_from_user(void *to, const void __user *from, unsigned long count);
unsigned long copy_to_user(void __user *to, const void *from, unsigned long count);
```
**3. 驱动注销**
**3.1. 删除cdev**
在字符设备驱动模块卸载函数中通过cdev_del()函数向系统删除一个cdev，完成字符设备的注销。

C
```
/*原型：*/
void cdev_del(struct cdev *);
/*例：*/
cdev_del(&btn_cdev);
```
**3.2. 释放设备号**
在调用cdev_del()函数从系统注销字符设备之后，unregister_chrdev_region()应该被调用以释放原先申请的设备号。

C
```
/*原型：*/
void unregister_chrdev_region(dev_t from, unsigned count);
/*例：*/
unregister_chrdev_region(MKDEV(major, 0), 1);
```
**四、字符设备驱动程序基础:**
**4.1 cdev结构体**
在Linux2.6 内核中，使用cdev结构体来描述一个字符设备，cdev结构体的定义如下：


C
```
struct cdev {
      struct kobject kobj;
      struct module *owner;  /*通常为THIS_MODULE*/
      struct file_operations *ops; /*在cdev_init()这个函数里面与cdev结构联系起来*/
      struct  list_head list;
      dev_t  dev;  /*设备号*/
      unsigned int count;
}；
```
cdev 结构体的dev_t 成员定义了设备号，为32位，其中12位是主设备号，20位是次设备号，我们只需使用二个简单的宏就可以从dev_t 中获取主设备号和次设备号：
MAJOR(dev_t dev)
MINOR(dev_t dev)
相反地，可以通过主次设备号来生成dev_t：
MKDEV(int major,int minor)
**4.2 Linux 2.6内核提供一组函数用于操作cdev 结构体：**


C
```
1：void cdev_init(struct cdev*,struct file_operations *);
2：struct cdev *cdev_alloc(void);
3：int cdev_add(struct cdev *,dev_t,unsigned);
4：void cdev_del(struct cdev *);
```
其中（1）用于初始化cdev结构体，并建立cdev与file_operations 之间的连接。（2）用于动态分配一个cdev结构，（3）向内核注册一个cdev结构，（4）向内核注销一个cdev结构
**4.3  Linux 2.6内核分配和释放设备号**
在调用cdev_add()函数向系统注册字符设备之前，首先应向系统申请设备号，有二种方法申请设备号，一种是静态申请设备号：
5：int register_chrdev_region(dev_t from,unsigned count,const char *name)
另一种是动态申请设备号：
6：int alloc_chrdev_region(dev_t *dev,unsigned baseminor,unsigned count,const char *name);
其中，静态申请是已知起始设备号的情况，如先使用cat /proc/devices 命令查得哪个设备号未事先使用（不推荐使用静态申请）；动态申请是由系统自动分配，只需设置major = 0即可。
相反地，在调用cdev_del()函数从系统中注销字符设备之后，应该向系统申请释放原先申请的设备号，使用：
7：void unregister_chrdev_region(dev_t from,unsigned count);
**4.4 cdev结构的file_operations结构体**
这个结构体是字符设备当中最重要的结构体之一，file_operations 结构体中的成员函数指针是字符设备驱动程序设计的主体内容，这些函数实际上在应用程序进行Linux 的 open()、read()、write()、close()、seek()、ioctl()等系统调用时最终被调用。


C
```
struct file_operations {
/*拥有该结构的模块计数，一般为THIS_MODULE*/
 struct module *owner;
/*用于修改文件当前的读写位置*/
 loff_t (*llseek) (struct file *, loff_t, int);
/*从设备中同步读取数据*/
 ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
/*向设备中写数据*/
 ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
 ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
 ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
 int (*readdir) (struct file *, void *, filldir_t);
/*轮询函数，判断目前是否可以进行非阻塞的读取或写入*/
 unsigned int (*poll) (struct file *, struct poll_table_struct *);
/*执行设备的I/O命令*/
 int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
 long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
 long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
/*用于请求将设备内存映射到进程地址空间*/
 int (*mmap) (struct file *, struct vm_area_struct *);
/*打开设备文件*/
 int (*open) (struct inode *, struct file *);
 int (*flush) (struct file *, fl_owner_t id);
/*关闭设备文件*/
 int (*release) (struct inode *, struct file *);
 int (*fsync) (struct file *, struct dentry *, int datasync);
 int (*aio_fsync) (struct kiocb *, int datasync);
 int (*fasync) (int, struct file *, int);
 int (*lock) (struct file *, int, struct file_lock *);
 ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
 unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
 int (*check_flags)(int);
 int (*flock) (struct file *, int, struct file_lock *);
 ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
 ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
 int (*setlease)(struct file *, long, struct file_lock **);
};
```
**4.5 file结构**
file  结构代表一个打开的文件，它的特点是一个文件可以对应多个file结构。它由内核再open时创建，并传递给在该文件上操作的所有函数，直到最后close函数，在文件的所有实例都被关闭之后，内核才释放这个数据结构。
在内核源代码中，指向 struct file 的指针通常比称为filp，file结构有以下几个重要的成员：


C
```
struct file{
mode_t   fmode; /*文件模式，如FMODE_READ，FMODE_WRITE*/
......
loff_t   f_pos;  /*loff_t 是一个64位的数，需要时，须强制转换为32位*/
unsigned int f_flags;  /*文件标志，如：O_NONBLOCK*/
struct  file_operations  *f_op;
void  *private_data;  /*非常重要，用于存放转换后的设备描述结构指针*/
.......
};
```
**4.6 inode 结构**
内核用inode 结构在内部表示文件，它是实实在在的表示物理硬件上的某一个文件，且一个文件仅有一个inode与之对应，同样它有二个比较重要的成员：


C
```
struct inode{
dev_t  i_rdev;            /*设备编号*/
struct cdev *i_cdev;  /*cdev 是表示字符设备的内核的内部结构*/
};
可以从inode中获取主次设备号，使用下面二个宏：
/*驱动工程师一般不关心这二个宏*/
unsigned int imajor(struct inode *inode);
unsigned int iminor(struct inode *inode);
```
**4.7字符设备驱动模块加载与卸载函数**
在字符设备驱动模块加载函数中应该实现设备号的申请和cdev 结构的注册，而在卸载函数中应该实现设备号的释放与cdev结构的注销。
我们一般习惯将cdev内嵌到另外一个设备相关的结构体里面，该设备包含所涉及的cdev、私有数据及信号量等等信息。常见的设备结构体、模块加载函数、模块卸载函数形式如下：


C
```
/*设备结构体*/
struct  xxx_dev{
      struct   cdev   cdev;
      char *data;
      struct semaphore sem;
      ......
};
/*模块加载函数*/
static int   __init  xxx_init(void)
{
      .......
      初始化cdev结构;
      申请设备号；
      注册设备号；
       申请分配设备结构体的内存；  /*非必须*/
}
/*模块卸载函数*/
static void  __exit   xxx_exit(void)
{
       .......
       释放原先申请的设备号；
       释放原先申请的内存；
       注销cdev设备；
}
```
**4.8字符设备驱动的 file_operations 结构体重成员函数**


C
```
/*读设备*/
ssize_t   xxx_read(struct file *filp,  char __user *buf,  size_t  count,  loff_t *f_pos)
{
        ......
        使用filp->private_data获取设备结构体指针；
        分析和获取有效的长度；
        /*内核空间到用户空间的数据传递*/
        copy_to_user(void __user *to,  const void *from,  unsigned long count);
        ......
}
/*写设备*/
ssize_t   xxx_write(struct file *filp,  const char  __user *buf,  size_t  count,  loff_t *f_pos)
{
        ......
        使用filp->private_data获取设备结构体指针；
        分析和获取有效的长度；
        /*用户空间到内核空间的数据传递*/
        copy_from_user(void *to,  const  void   __user *from,  unsigned long count);
        ......
}
/*ioctl函数*/
static int xxx_ioctl(struct inode *inode,struct file *filp,unsigned int cmd,unsigned long arg)
{
      ......
      switch(cmd){
           case  xxx_CMD1:
                        ......
                        break;
           case  xxx_CMD2:
                       .......
                      break;
           default:
                      return -ENOTTY;  /*不能支持的命令*/
      }
      return 0;
}
```
**4.9、字符设备驱动文件操作结构体模板**


C
```
struct file_operations xxx_fops = {
      .owner = THIS_MODULE,
      .open = xxx_open,
      .read = xxx_read,
     .write = xxx_write,
     .close = xxx_release,
     .ioctl = xxx_ioctl,
     .lseek = xxx_llseek,
};
上面的写法需要注意二点，一：结构体成员之间是以逗号分开的而不是分号，结构体字段结束时最后应加上分号。
```
**五、字符设备驱动小结：**
字符设备是3大类设备（字符设备、块设备、网络设备）中较简单的一类设备，其驱动程序中完成的主要工作是初始化、添加和删除cdev结构体，申请和释放设备号，以及填充file_operation结构体中操作函数，并实现file_operations结构体中的read()、write()、ioctl()等重要函数。如图所示为cdev结构体、file_operations和用户空间调用驱动的关系。
**![](http://images.cnitblog.com/blog2015/563126/201504/150848455892220.jpg)**
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
