# LDD3源码分析之llseek分析 - 一世豁然的专栏 - CSDN博客





2014年12月18日 16:24:26[一世豁然](https://me.csdn.net/Explorer_day)阅读数：676








本博客转载于：[http://blog.csdn.net/liuhaoyutz/article/details/7402857](http://blog.csdn.net/liuhaoyutz/article/details/7402857)[作者：刘昊昱] 






本文分析LDD3第6章的llseek函数。一、用户空间的lseek函数要理解驱动中llseek函数的实现，必须先清楚对应的用户空间中lseek函数的用法，lseek函数函数原型如下：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7402857#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7402857#)

- off_t lseek(int fd, off_t offset, int whence);  

第一个参数fd是要操作的文件描述符。第二个参数指定文件操作指针的偏移量。注意，文件的读和写使用的是同一个文件操作指针。第三个参数指定移动文件操作指针的参考点。这个参数通常取值为以下宏：SEEK_SET：表示相对文件起始位置。SEEK_CUR：表示相对文件操作指针当前位置。SEEK_END：表示相对文件结束位置。下面先来看一个用户空间测试程序llseek_test.c的实现，这个程序用来测试scull的定位功能，其代码如下：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7402857#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7402857#)

-  1#include <stdio.h>  
-  2#include <unistd.h>  
-  3#include <fcntl.h>  
-  4#include <string.h>  
-  5#include <sys/types.h>  
-  6#include <sys/stat.h>  
-  7  
-  8#define BUF_SIZE 50  
-  9#define DEVICE_FILE "/dev/scull"
- 10  
- 11int main(int argc, char *argv[])  
- 12{  
- 13    int fd;  
- 14    int num;  
- 15    char buf[BUF_SIZE];  
- 16  
- 17    fd = open(DEVICE_FILE, O_RDWR);  
- 18    if(fd < 0)  
- 19    {  
- 20        printf("open scull error!\n");  
- 21        return -1;  
- 22    }  
- 23  
- 24    memset(buf, 0, BUF_SIZE);  
- 25    num = read(fd, buf, BUF_SIZE);  
- 26    buf[num] = 0;  
- 27    printf("%s\n", buf);  
- 28  
- 29    lseek(fd, 2, SEEK_SET);  
- 30    write(fd, "aa", 2);  
- 31    num = read(fd, buf, BUF_SIZE);  
- 32    buf[num] = 0;  
- 33    printf("%s\n", buf);  
- 34  
- 35    lseek(fd, 2, SEEK_SET);  
- 36    num = read(fd, buf, BUF_SIZE);  
- 37    buf[num] = 0;  
- 38    printf("%s\n", buf);  
- 39  
- 40    lseek(fd, 0, SEEK_SET);  
- 41    lseek(fd, 2, SEEK_CUR);  
- 42    num = read(fd, buf, BUF_SIZE);  
- 43    buf[num] = 0;  
- 44    printf("%s\n", buf);  
- 45  
- 46    lseek(fd, 0, SEEK_SET);  
- 47    lseek(fd, 0, SEEK_END);  
- 48    memset(buf, 0, BUF_SIZE);  
- 49    printf("read return value is %d.\n", read(fd, buf, BUF_SIZE));  
- 50  
- 51    return 0;  
- 52}  

这个程序很简单，主要关注一下lseek函数是怎样移动文件操作指针的。第29行，使用SEEK_SET宏，将文件操作指针移动到文件起始位置加上2个字节处。第30行，写入两个字符’a’。第41行，使用SEEK_CUR宏，将文件操作指针移动到文件操作指针当前位置加上2个字节处。第47行，使用SEEK_END宏，将文件操作指针移动到文件结束处。第49行，打印read的返回值，当文件操作指针在文件结束处时，read返回0。下图是使用llseek_test测试scull设备的定位功能的过程：这里需要说明的一点是，从上面的输出信息可以看出，对文件的read和write操作使用的是同一个文件操作指针。二、驱动程序中llseek函数的实现用户空间的lseek函数的定位功能在驱动程序中是由llseek函数实现的。注意，要完成对文件的定位操作，还需要read、write函数的配合，读写完成后必须更新文件操作指针位置。即使驱动程序中没有实现llseek函数，有某些情况下，设备也是可以完成定位操作的，内核通过修改filp->f_pos来执行定位，filp->f_pos是文件的当前读写位置。但是，如果定位操作需要涉及设备的物理操作，就必须实现llseek函数了。scull设备的llseek函数代码如下：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7402857#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7402857#)

- 523loff_t scull_llseek(struct file *filp, loff_t off, int whence)  
- 524{  
- 525    struct scull_dev *dev = filp->private_data;  
- 526    loff_t newpos;  
- 527  
- 528    switch(whence) {  
- 529      case 0: /* SEEK_SET */
- 530        newpos = off;  
- 531        break;  
- 532  
- 533      case 1: /* SEEK_CUR */
- 534        newpos = filp->f_pos + off;  
- 535        break;  
- 536  
- 537      case 2: /* SEEK_END */
- 538        newpos = dev->size + off;  
- 539        break;  
- 540  
- 541      default: /* can't happen */
- 542        return -EINVAL;  
- 543    }  
- 544    if (newpos < 0) return -EINVAL;  
- 545    filp->f_pos = newpos;  
- 546    return newpos;  
- 547}  

这里唯一与设备相关的操作就是第538行，取得设备文件的大小。同时，我们在前面的文章中分析过，scull的read和write函数读写文件后，总是更新文件操作指针的位置，定位功能需要llseek与read、write的配合。对于某些设备文件来说，定位功能是没有意义的，例如键盘。对于这些设备，我们不能简单地不实现llseek函数，因为默认方法是允许通过filp->f_pos定位的。我们应该在我们的open函数中调用nonseekable_open，通知内核设备不支持llseek。该函数的函数原型如下：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7402857#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7402857#)

- int nonseekable_open(struct inode *inode; struct file *filp);  

另外，为完整起见，我们还应该将file_operations结构中的llseek方法设置为特殊的辅助函数no_llseek。




