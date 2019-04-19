# Linux 内核驱动中对文件的读写 - 文章 - 伯乐在线
原文出处： [niuyimail](http://blog.chinaunix.net/uid-13059007-id-5766941.html)
有时候需要在[Linux](http://www.chinabyte.com/keyword/Linux/) kernel–大多是在需要调试的驱动程序–中读写文件数据。在kernel中操作文件没有标准库可用，需要利用kernel的一些函数，这些函数主 要有： filp_open() filp_close(), vfs_read() vfs_write()，set_fs()，get_fs()等，这些函数在linux/fs.h和asm/uaccess.h头文件中声明。下面介绍主要步骤
### 1. 打开文件
filp_open()在kernel中可以打开文件，其原形如下：
strcut file* filp_open(const char* filename, int open_mode, int mode);
该函数返回strcut file*结构指针，供后继函数操作使用，该返回值用IS_ERR()来检验其有效性。
#### 参数说明
filename： 表明要打开或创建文件的名称(包括路径部分)。在内核中打开的文件时需要注意打开的时机，很容易出现需要打开文件的驱动很早就加载并打开文件，但需要打开的文件所在设备还不有挂载到文件系统中，而导致打开失败。
open_mode： 文件的打开方式，其取值与标准库中的open相应参数类似，可以取O_CREAT,O_RDWR,O_RDONLY等。
mode： 创建文件时使用，设置创建文件的读写权限，其它情况可以匆略设为0
### 2. 读写文件
kernel中文件的读写操作可以使用vfs_read()和vfs_write，在使用这两个函数前需要说明一下get_fs()和 set_fs()这两个函数。
vfs_read() vfs_write()两函数的原形如下：
ssize_t vfs_read(struct file* filp, char __user* buffer, size_t len, loff_t* pos);
ssize_t vfs_write(struct file* filp, const char __user* buffer, size_t len, loff_t* pos);
注意这两个函数的第二个参数buffer，前面都有__user修饰符，这就要求这两个buffer指针都应该指向用户空间的内存，如果对该参数传 递kernel空间的指针，这两个函数都会返回失败-EFAULT。但在Kernel中，我们一般不容易生成用户空间的指针，或者不方便独立使用用户空间 内存。要使这两个读写函数使用kernel空间的buffer指针也能正确工作，需要使用set_fs()函数或宏(set_fs()可能是宏定义)，如 果为函数，其原形如下：
void set_fs(mm_segment_t fs);
该函数的作用是改变kernel对内存地址检查的处理方式，其实该函数的参数fs只有两个取值：USER_DS，KERNEL_DS，分别代表 用户空间和内核空间，默认情况下，kernel取值为USER_DS，即对用户空间地址检查并做变换。那么要在这种对内存地址做检查变换的函数中使用内核 空间地址，就需要使用set_fs(KERNEL_DS)进行设置。get_fs()一般也可能是宏定义，它的作用是取得当前的设置，这两个函数的一般用 法为：
mm_segment_t old_fs;
old_fs = get_fs();
set_fs(KERNEL_DS);
…… //与内存有关的操作
set_fs(old_fs);
还有一些其它的内核函数也有用__user修饰的参数，在kernel中需要用kernel空间的内存代替时，都可以使用类似办法。
使用vfs_read()和vfs_write()最后需要注意的一点是最后的参数loff_t * pos，pos所指向的值要初始化，表明从文件的什么地方开始读写。
### 3. 关闭读写文件
int filp_close(struct file*filp, fl_owner_t id);
该函数的使用很简单，第二个参数一般传递NULL值，也有用current->files作为实参的。
使用以上函数的其它注意点：
1. 其实Linux Kernel组成员不赞成在kernel中独立的读写文件(这样做可能会影响到策略和安全问题)，对内核需要的文件内容，最好由应用层配合完成。
2. 在可加载的kernel module中使用这种方式读写文件可能使[模块](http://www.chinabyte.com/keyword/%E6%A8%A1%E5%9D%97/)加载失败，原因是内核可能没有EXPORT你所需要的所有这些函数。
3. 分析以上某些函数的参数可以看出，这些函数的正确运行需要依赖于进程环境，因此，有些函数不能在中断的handle或Kernel中不属于任可进程的代码 中执行，否则可能出现崩溃，要避免这种情况发生，可以在kernel中创建内核线程，将这些函数放在线程环境下执行(创建内核线程的方式请参数 kernel_thread()函数)。
在VFS的支持下，用户态进程读写 任何类型的文件系统都可以使用read和write着两个系统调用，但是在linux内核中没有这样的系统调用我们如何操作文件呢？我们知道read和 write在进入内核态之后，实际执行的是sys_read和sys_write，但是查看内核源代码，发现这些操作文件的函数都没有导出(使用 EXPORT_SYMBOL导出)，也就是说在内核模块中是不能使用的，那如何是好？
通过查看sys_open的源码我 们发现，其主要使用了do_filp_open()函数，该函数在fs/namei.c中，而在改文件中，filp_open函数也是调用了 do_filp_open函数，并且接口和sys_open函数极为相似，调用参数也和sys_open一样，并且使用EXPORT_SYMBOL导出 了，所以我们猜想该函数可以打开文件，功能和open一样。使用同样的查找方法，我们找出了一组在内核中操作文件的函数，如下：
|功能|函数原型|
|----|----|
|打开文件|**struct** file *filp_open(**const****char** *filename, **int** flags, **int** mode)|
|读取文件|**ssize_t** vfs_read(**struct** file *file, **char** __user *buf, **size_t** count, loff_t *pos)|
|写文件|**ssize_t** vfs_write(**struct** file *file, **const****char** __user *buf, **size_t** count, loff_t *pos)|
|关闭文件|**int** filp_close(**struct** file *filp, fl_owner_t id)|
我们注意到在vfs_read和vfs_write函数中，其参数buf指向的用户空间的内存地址，如果我们直接使用内核空间的指针，则会返回-EFALUT。所以我们需要使用
set_fs()和get_fs()宏来改变内核对内存地址检查的处理方式，所以在内核空间对文件的读写流程为：


```
mm_segment_t fs = get_fs();
set_fs(KERNEL_FS);
//vfs_write();
vfs_read();
set_fs(fs);
```
下面为一个在内核中对文件操作的例子：


```
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
static char buf[] = "你好";
static char buf1[10];
 
int __init hello_init(void)
{
    struct file *fp;
    mm_segment_t fs;
    loff_t pos;
    printk("hello enter\n");
    fp = filp_open("/home/niutao/kernel_file", O_RDWR | O_CREAT, 0644);
    if (IS_ERR(fp)) {
        printk("create file error\n");
        return -1;
    }
    fs = get_fs();
    set_fs(KERNEL_DS);
    pos = 0;
    vfs_write(fp, buf, sizeof(buf), &pos);
    pos = 0;
    vfs_read(fp, buf1, sizeof(buf), &pos);
    printk("read: %s\n", buf1);
    filp_close(fp, NULL);
    set_fs(fs);
    return 0;
}
void __exit hello_exit(void)
{
    printk("hello exit\n");
}
 
module_init(hello_init);
module_exit(hello_exit);
 
MODULE_LICENSE("GPL");
```
