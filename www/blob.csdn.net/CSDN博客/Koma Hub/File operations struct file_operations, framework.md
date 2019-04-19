# File operations: struct file_operations, framework - Koma Hub - CSDN博客
2019年02月28日 22:38:18[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：28
**Table of Contents**
[From the syscall to your driver](#From%20the%20syscall%20to%20your%20driver)
[In fs/read_write.c](#In%20fs%2Fread_write.c)
[ioctl mechanism](#ioctl%20mechanism)
[Use the ioctl() system call.](#Use%20the%20ioctl()%20system%20call.)
[Example of frameworks](#Example%20of%20frameworks)
```cpp
struct file_operations {
    struct module *owner;
    loff_t (*llseek) (struct file *, loff_t, int);
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
    ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
    int (*readdir) (struct file *, void *, filldir_t);
    unsigned int (*poll) (struct file *, struct poll_table_struct *);
    int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
    int (*mmap) (struct file *, struct vm_area_struct *);
    int (*open) (struct inode *, struct file *);
    int (*flush) (struct file *, fl_owner_t id);
    int (*release) (struct inode *, struct file *);
    int (*fsync) (struct file *, struct dentry *, int datasync);
    int (*fasync) (int, struct file *, int);
    int (*flock) (struct file *, int, struct file_lock *);
    [...]
};
```
Implement the read() and write() operations, and instantiate the file operations structure.
```cpp
static ssize_t demo_write(struct file *f, const char __user *buf,
                            size_t len, loff_t *off)
{
    [...]
}
static ssize_t demo_read(struct file *f, char __user *buf,
                            size_t len, loff_t *off)
{
    [...]
}
static struct file_operations demo_fops =
{
    .owner = THIS_MODULE,
    .read = acme_read,
    .write = acme_write
};
```
Register and unregister the driver to the kernel using register_chrdev_region/unregister_chrdev_region and cdev_add/cdev_del.
```cpp
static dev_t demo_dev = MKDEV(202,128);
static struct cdev demo_cdev;
static int __init demo_init(void)
{
    register_chrdev_region(demo_dev, 1, \demo");
    cdev_init(&demo_cdev, &demo_fops);
    cdev_add(&demo_cdev, demo_dev, demo_count);
}
static void __exit demo_exit(void)
{
    cdev_del(&demo_cdev);
    unregister_chrdev_region(demo_dev, 1);
    iounmap(demo_buf);
}
module_init(demo_init);
module_exit(demo_exit);
```
1. Making it accessible to userspace application by creating a device node:
`$ mknod /dev/demo c 202 128`
2. Using normal the normal le API :
```cpp
fd = open("/dev/demo", O_RDWR);
ret = read(fd, buf, bufsize);
ret = write(fd, buf, bufsize);
```
# From the syscall to your driver
## In fs/read_write.c
```cpp
SYSCALL_DEFINE3(read, unsigned int, fd, char __user *, buf, size_t, count)
{
    struct file *file;
    ssize_t ret = -EBADF;
    int fput_needed;
    file = fget_light(fd, &fput_needed);
    if (file) {
        loff_t pos = file_pos_read(file);
        ret = vfs_read(file, buf, count, &pos);
        file_pos_write(file, pos);
        fput_light(file, fput_needed);
    }
    return ret;
}
ssize_t vfs_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    ssize_t ret;
    if (!(file->f_mode & FMODE_READ))
        return -EBADF;
    if (!file->f_op || (!file->f_op->read && !file->f_op->aio_read))
        return -EINVAL;
    if (unlikely(!access_ok(VERIFY_WRITE, buf, count)))
        return -EFAULT;
    ret = rw_verify_area(READ, file, pos, count);
    if (ret >= 0) {
        count = ret;
        if (file->f_op->read)
            ret = file->f_op->read(file, buf, count, pos);
        else
            ret = do_sync_read(file, buf, count, pos);
        if (ret > 0) {
            fsnotify_access(file->f_path.dentry);
            add_rchar(current, ret);
        }
        inc_syscr(current);
    }
    return ret;
}
```
# ioctl mechanism
The file_operations set of operations, while being suffcient for regular les, isn't suffcient as an API to the wide range of character and block devices
Device-specic operations such as changing the speed of a serial port, setting the volume on a soundcard, conguring video-related parameters on a framebuffer are not handled by the file_operations
One of the operations, ioctl() allows to extend the capabilities of a driver with driver-specic operations I In userspace:
```cpp
int ioctl(int d, int request, ...);
```
- d, the le descriptor
- request, a driver-specic integer identifying the operation
- ..., zero or one argument.
In kernel space:
```cpp
int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
```
Implement the demo_ioctl() operation and reference it in the file_operations structure:
```cpp
static int demo_ioctl(struct inode *inode,
                        struct file *file,
                        unsigned int cmd,
                        unsigned long arg)
{
    char __user *argp = (char __user *)arg;
    switch (cmd) {
        case DEMO_CMD1:
            /* Something */
            return 0;
        default:
            return -ENOTTY;
    }
}
static const struct file_operations demo_fops =
{
    [...]
    .ioctl = demo_ioctl,
    [...]
};
```
## Use the ioctl() system call.
```cpp
int fd, val;
fd = open("/dev/demo", O_RDWR);
ioctl(fd, DEMO_CMD1, & val);
```
- Most device drivers are not directly implemented as character devices or block devices
- They are implemented under a **framework**, specic to a device type (framebuer, V4L, serial, etc.)
- The framework allows to factorize the common parts of drivers for the same type of devices
- From userspace, they are still seen as normal character devices
- The framework allows to provide a coherent userspace interface (ioctl numbering and semantic, etc.) for every type of device, regardless of the driver
# Example of frameworks
![](https://img-blog.csdnimg.cn/20190228223717352.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
