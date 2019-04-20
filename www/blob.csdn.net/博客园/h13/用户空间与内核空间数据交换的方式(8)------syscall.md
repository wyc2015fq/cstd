# 用户空间与内核空间数据交换的方式(8)------syscall - h13 - 博客园
系统调用是内核提供给应用程序的接口，应用对底层硬件的操作大部分都是通过调用系统调用来完成的，例如得到和设置系统时间，就需要分别调用 gettimeofday 和 settimeofday 来实现。事实上，所有的系统调用都涉及到内核与应用之间的数据交换，如文件系统操作函数 read 和 write，设置和读取网络协议栈的 setsockopt 和 getsockopt。本节并不是讲解如何增加新的系统调用，而是讲解如何利用现有系统调用来实现用户的数据传输需求。
一般地，用户可以建立一个伪设备来作为应用与内核之间进行数据交换的渠道，最通常的做法是使用伪字符设备，具体实现方法是：
1．定义对字符设备进行操作的必要函数并设置结构 struct file_operations
结构 struct file_operations 非常大，对于一般的数据交换需求，只定义 open, read, write, ioctl, mmap 和 release 函数就足够了，它们实际上对应于用户态的文件系统操作函数 open, read, write, ioctl, mmap 和 close。这些函数的原型示例如下：
```
ssize_t exam_read (struct file * file, char __user * buf, size_t count, loff_t * ppos)
{
…
}
ssize_t exam_write(struct file * file, const char __user * buf, size_t count, loff_t * ppos)
{
…
}
int exam_ioctl(struct inode * inode, struct file * file, unsigned int cmd, unsigned long argv)
{
…
}
int exam_mmap(struct file *, struct vm_area_struct *)
{
…
}
int exam_open(struct inode * inode, struct file * file)
{
…
}
int exam_release(struct inode * inode, struct file * file)
{
…
}
```
在定义了这些操作函数后需要定义并设置结构struct file_operations
```
struct file_operations exam_file_ops = {
        .owner = THIS_MODULE,
        .read = exam_read,
        .write = exam_write,
        .ioctl = exam_ioctl,
        .mmap = exam_mmap,
        .open = exam_open,
        .release = exam_release,
};
```
2. 注册定义的伪字符设备并把它和上面的 struct file_operations 关联起来：
int exam_char_dev_major;
exam_char_dev_major = register_chrdev(0, "exam_char_dev", &exam_file_ops);
注意，函数 register_chrdev 的第一个参数如果为 0，表示由内核来确定该注册伪字符设备的主设备号，这是该函数的返回为实际分配的主设备号，如果返回小于 0，表示注册失败。因此，用户在使用该函数时必须判断返回值以便处理失败情况。为了使用该函数必须包含头文件 linux/fs.h。
在源代码包中给出了一个使用这种方式实现用户态与内核态数据交换的典型例子，它包含了三个文件：头文件 syscall-exam.h 定义了 ioctl 命令，.c 文件 syscall-exam-user.c为用户态应用，它通过文件系统操作函数 mmap 和 ioctl 来与内核态模块交换数据，.c 文件 syscall-exam-kern.c 为内核模块，它实现了一个伪字符设备，以便与用户态应用进行数据交换。为了正确运行应用程序 syscall-exam-user，需要在插入模块 syscall-exam-kern 后创建该实现的伪字符设备，用户可以使用下面命令来正确创建设备：
$ mknod /dev/mychrdev c `dmesg | grep "char device mychrdev" | sed 's/.*major is //g'` 0
然后用户可以通过 cat 来读写 /dev/mychrdev，应用程序 syscall-exam-user则使用 mmap 来读数据并使用 ioctl 来得到该字符设备的信息以及裁减数据内容，它只是示例如何使用现有的系统调用来实现用户需要的数据交互操作。
下面是作者运行该模块的结果示例：
```
$ insmod ./syscall-exam-kern.ko
char device mychrdev is registered, major is 254
$ mknod /dev/mychrdev c `dmesg | grep "char device mychrdev" | sed 's/.*major is //g'` 0
$ cat /dev/mychrdev
$ echo "abcdefghijklmnopqrstuvwxyz" > /dev/mychrdev
$ cat /dev/mychrdev
abcdefghijklmnopqrstuvwxyz
$ ./syscall-exam-user
User process: syscall-exam-us(1433)
Available space: 65509 bytes
Data len: 27 bytes
Offset in physical: cc0 bytes
mychrdev content by mmap:
abcdefghijklmnopqrstuvwxyz
$ cat /dev/mychrdev
abcde
$
```
 示例：
头文件 syscall-exam.h：
```
//header: syscall-exam.h
#ifndef _SYSCALL_EXAM_H
#define _SYSCALL_EXAM_H
#include <linux/ioctl.h>
#undef TASK_COMM_LEN
#define TASK_COMM_LEN 16
typedef struct mychrdev_info {
        pid_t user_pid;
        char user_name[TASK_COMM_LEN];
        unsigned int available_len;
        unsigned int len;
        unsigned long offset_in_ppage;
} mydev_info_t;
struct mychrdev_window {
        unsigned int head;
        unsigned int tail;
};
#define MYCHRDEV_IOCTL_BASE 'm'
#define MYCHRDEV_IOR(nr, size) _IOR(MYCHRDEV_IOCTL_BASE, nr, size)
#define MYCHRDEV_IOW(nr, size) _IOW(MYCHRDEV_IOCTL_BASE, nr, size)
#define MYCHRDEV_IOCTL_GET_INFO MYCHRDEV_IOR(0x01,mydev_info_t)
#define MYCHRDEV_IOCTL_SET_TRUNCATE MYCHRDEV_IOW(0x02,int)
#endif
```
内核模块源码 syscall-exam-kern.c：
```
//kernel module: syscall-exam-kern.c
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/mm.h>
#include "syscall-exam.h"
#define MYCHRDEV_MAX_MINOR 4
#define MYCHRDEV_CAPACITY 65536
struct mychrdev_data {
        char buf[MYCHRDEV_CAPACITY];
        unsigned int headptr;
        unsigned int tailptr;
};
struct mychrdev_data * mydata[MYCHRDEV_MAX_MINOR];
static atomic_t mychrdev_use_stats[MYCHRDEV_MAX_MINOR];
static int mychrdev_major;
struct mychrdev_private {
        pid_t user_pid;
        char user_name[TASK_COMM_LEN];
        int minor;
        struct mychrdev_data * data;
#define headptr data->headptr
#define tailptr data->tailptr
#define buffer data->buf
};
ssize_t mychrdev_read(struct file * file, char __user * buf, size_t count, loff_t * ppos)
{
        int len;
        struct mychrdev_private * myprivate  = (struct mychrdev_private *)file->private_data;
        len = (int)(myprivate->tailptr - myprivate->headptr);
        if (*ppos >= len) {
                return 0;
        }
        if (*ppos + count > len) {
                count = len - *ppos;
        }
        if (copy_to_user(buf, myprivate->buffer + myprivate->headptr + *ppos, count)) {
                return -EFAULT;
        }
        *ppos += count;
        return count;
}
ssize_t mychrdev_write(struct file * file, const char __user * buf, size_t count, loff_t * ppos)
{
        int leftlen;
        struct mychrdev_private * myprivate  = (struct mychrdev_private *)file->private_data;
        leftlen = (MYCHRDEV_CAPACITY - myprivate->tailptr);
        if (* ppos >= MYCHRDEV_CAPACITY) {
                return -ENOBUFS;
        }
        if (*ppos + count > leftlen) {
                count = leftlen - *ppos;
        }
        if (copy_from_user(myprivate->buffer + myprivate->headptr + *ppos, buf, count)) {
                return -EFAULT;
        }
        *ppos += count;
        myprivate->tailptr += count;
        return count;;
}
int mychrdev_ioctl(struct inode * inode, struct file * file, unsigned int cmd, unsigned long argp)
{
        struct mychrdev_private * myprivate = (struct mychrdev_private *)file->private_data;
        mydev_info_t a;
        struct mychrdev_window window;
        switch(cmd) {
                case MYCHRDEV_IOCTL_GET_INFO:
                        a.user_pid = myprivate->user_pid;
                        memcpy(a.user_name, myprivate->user_name, strlen(myprivate->user_name));
                        a.available_len = MYCHRDEV_CAPACITY - myprivate->tailptr;
                        a.len = myprivate->tailptr - myprivate->headptr;
                        a.offset_in_ppage = __pa(myprivate) & 0x00000fff;
                        if (copy_to_user((void *)argp, (void *)&a, sizeof(a))) {
                                return -EFAULT;
                        }
                        break;
                case MYCHRDEV_IOCTL_SET_TRUNCATE:
                        if (copy_from_user(&window, (void *)argp, sizeof(window))) {
                                return -EFAULT;
                        }
                        if (window.head < myprivate->headptr) {
                                return -EINVAL;
                        }
                        if (window.tail > myprivate->tailptr) {
                                return -EINVAL;
                        }
                        myprivate->headptr = window.head;
                        myprivate->tailptr = window.tail;
                        break;
                default:
                        return -EINVAL;
        }
        return 0;
}
int mychrdev_open(struct inode * inode, struct file * file)
{
        struct mychrdev_private * myprivate = NULL;
        int minor;
        if (current->euid != 0) {
                return -EPERM;
        }
        minor = MINOR(inode->i_rdev);
        if (atomic_read(&mychrdev_use_stats[minor])) {
                return -EBUSY;
        }
        else {
                atomic_inc(&mychrdev_use_stats[minor]);
        }
        myprivate = (struct mychrdev_private *)kmalloc(sizeof(struct mychrdev_private), GFP_KERNEL);
        if (myprivate == NULL) {
                return -ENOMEM;
        }
        myprivate->user_pid = current->pid;
        sprintf(myprivate->user_name, "%s", current->comm);
        myprivate->minor = minor;
        myprivate->data = mydata[minor];
        file->private_data = (void *)myprivate;
        return 0;
}
int mychrdev_mmap(struct file * file, struct vm_area_struct * vma)
{
        unsigned long pfn;
        struct mychrdev_private * myprivate  = (struct mychrdev_private *)file->private_data;
        /* Turn a kernel-virtual address into a physical page frame */
        pfn = __pa(&(mydata[myprivate->minor]->buf)) >> PAGE_SHIFT;
        if (!pfn_valid(pfn))
                return -EIO;
        vma->vm_flags |= VM_RESERVED;
        vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
        /* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
        if (remap_pfn_range(vma,
                            vma->vm_start,
                            pfn,
                            vma->vm_end - vma->vm_start,
                            vma->vm_page_prot))
                return -EAGAIN;
        return 0;
}
int mychrdev_release(struct inode * inode, struct file * file)
{
        atomic_dec(&mychrdev_use_stats[MINOR(inode->i_rdev)]);
        kfree(((struct mychrdev_private *)(file->private_data))->data);
        kfree(file->private_data);
        return 0;
}
loff_t mychrdev_llseek(struct file * file, loff_t offset, int seek_flags)
{
        struct mychrdev_private * myprivate = (struct mychrdev_private *)file->private_data;
        int len = myprivate->tailptr - myprivate->headptr;
        switch (seek_flags) {
                case 0:
                        if ((offset > len)
                                || (offset < 0)) {
                                return -EINVAL;
                        }
                case 1:
                        if ((offset + file->f_pos < 0)
                                || (offset + file->f_pos > len)) {
                                return -EINVAL;
                        }
                        offset += file->f_pos;
                case 2:
                        if ((offset > 0)
                                || (-offset > len)) {
                                return -EINVAL;
                        }
                        offset += len;
                        break;
                default:
                        return -EINVAL;
        }
        if ((offset >= 0) && (offset <= len)) {
                file->f_pos = offset;
                file->f_version = 0;
                return offset;
        }
        else {
                return -EINVAL;
        }
}
struct file_operations mychrdev_fops = {
        .owner = THIS_MODULE,
        .read = mychrdev_read,
        .write = mychrdev_write,
        .ioctl = mychrdev_ioctl,
        .open = mychrdev_open,
        .llseek = mychrdev_llseek,
        .release = mychrdev_release,
        .mmap = mychrdev_mmap,
};
static int __init mychardev_init(void)
{
        int i;
        for (i=0;i<MYCHRDEV_MAX_MINOR;i++) {
                atomic_set(&mychrdev_use_stats[i], 0);
                mydata[i] = NULL;
                mydata[i] = 
                    (struct mychrdev_data *)kmalloc(sizeof(struct mychrdev_data), GFP_KERNEL);
                if (mydata[i] == NULL) {
                        return -ENOMEM;
                }
                memset(mydata[i], 0, sizeof(struct mychrdev_data));
        }
        mychrdev_major = register_chrdev(0, "mychrdev", &mychrdev_fops);
        if (mychrdev_major <= 0) {
                printk("Fail to register char device mychrdev.\n");
                return -1;
        }
        printk("char device mychrdev is registered, major is %d\n", mychrdev_major);
        return 0;
}
static void __exit mychardev_remove(void)
{
        unregister_chrdev(mychrdev_major, NULL);
}
module_init(mychardev_init);
module_exit(mychardev_remove);
MODULE_LICENSE("GPL");
```
用户程序 syscall-exam-user.c：
```
//application: syscall-exam-user.c
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "syscall-exam.h"
int main(void)
{
        int fd;
        mydev_info_t mydev_info;
        struct mychrdev_window truncate_window;
        char * mmap_ptr = NULL;
        int i;
        fd = open("/dev/mychrdev", O_RDWR);
        if (fd < 0) {
                perror("open:");
                exit(-1);
        }
        ioctl(fd, MYCHRDEV_IOCTL_GET_INFO, &mydev_info);
        printf("User process: %s(%d)\n", mydev_info.user_name, mydev_info.user_pid);
        printf("Available space: %d bytes\n", mydev_info.available_len);
        printf("Data len: %d bytes\n", mydev_info.len);
        printf("Offset in physical: %lx bytes\n", mydev_info.offset_in_ppage);
        mmap_ptr = mmap(NULL, 65536, PROT_READ, MAP_PRIVATE, fd, 0);
        if ((int) mmap_ptr == -1) {
                perror("mmap:");
                close(fd);
                exit(-1);
        }
        printf("mychrdev content by mmap:\n");
        printf("%s\n", mmap_ptr);
        munmap(mmap_ptr, 65536);
        truncate_window.head = 0;
        truncate_window.tail = 5;
        ioctl(fd, MYCHRDEV_IOCTL_SET_TRUNCATE, &truncate_window);
        close(fd);
}
```
