# ioctl之FIONREAD - maopig的专栏 - CSDN博客
2014年07月01日 14:46:22[maopig](https://me.csdn.net/maopig)阅读数：1694
在学习ioctl 时常常跟 read, write 混淆。其实 ioctl 是用来设置硬件控制寄存器，或者读取硬件状态寄存器的数值之类的。
而read,write 是把数据丢入缓冲区，硬件的驱动从缓冲区读取数据一个个发送或者把接收的数据送入缓冲区。
 ioctl(keyFd, FIONREAD, &b)
得到缓冲区里有多少字节要被读取，然后将字节数放入b里面。
接下来就可以用read了。
read(keyFd, &b, sizeof(b))
这两个可以用在按键控制上，先是检测按键是否被按下，如果被按下就放在B里，然后user 在读取按键对应数值。
**Listing - Getting the number of bytes in the input buffer. 清单 - 读取串行端口输入缓冲区中的字节数 #include <unistd.h> #include <termios.h> int fd; int bytes; ioctl(fd, FIONREAD, &bytes);  **
**eg:**
**#include<stdio.h>**
**#include<stdlib.h>**
**#include<sys/ioctl.h>**
**#include<errno.h>**
**int kbhit(){**
**  int i;**
**  if(ioctl(0,FIONREAD,&i)<0){**
**        printf("ioctl failed, error=%d\n ",errno);**
**        exit(1);**
**  }**
**  return i;**
**}**
**main(){**
**int i=0;**
**int c=' ';**
**system("stty raw -echo" );**
**printf("enter 'q' to quit \n" );**
**for(;c!='q';++i){**
**  if(kbhit()){**
**    c=getchar();**
**    printf("\n got %c, on iteration %d",c,i);**
**  }**
**}**
**system("stty cooked echo" );**
**return 0;**
**}**
