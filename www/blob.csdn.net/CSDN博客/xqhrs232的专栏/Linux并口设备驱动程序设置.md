# Linux并口设备驱动程序设置 - xqhrs232的专栏 - CSDN博客
2016年05月10日 11:04:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：582
原文地址::[http://blog.csdn.net/bg2bkk/article/details/8946424](http://blog.csdn.net/bg2bkk/article/details/8946424)
参考地址：[http://oss.org.cn/kernel-book/ldd3/ch09s03.html](http://oss.org.cn/kernel-book/ldd3/ch09s03.html)
![并口的管脚](http://oss.org.cn/kernel-book/ldd3/images/snagitldd3/ldd3-9-1.png)
13脚接led的正输入，25脚接led的负极
驱动代码：
parport_drv.c
**[cpp]**[view
 plain](http://blog.csdn.net/bg2bkk/article/details/8946424#)[copy](http://blog.csdn.net/bg2bkk/article/details/8946424#)
- #include<linux init="" h="">
- #include<linux module="" h="">
- #include<linux kernel="" h="">
- #include<linux fs="" h="">
- #include<linux cdev="" h="">
- #include<linux types="" h="">
- #include<linux uaccess="" h="">
- 
- #include"parport_drv.h"
- 
- #define Drv_major 240
- #define Drv_name  "parport_drv"
- #define Drv_read_addr 0x379
- #define Drv_write_addr 0x378
- 
- int parport_open(struct inode *inode, struct file *filp)  
- {  
-     printk(KERN_ALERT "open the parport_dev\n");  
- return 0;  
- }  
- 
- ssize_t parport_write(struct file *filp, constchar *buf, size_t count, loff_t *f_ops)  
- {  
-     unsigned char status;  
- int loop;  
- for(loop = 0; loop < count; loop++)  
-     {  
-         get_user(status, (char *)buf);  
-         outb(status, Drv_write_addr);  
-     }  
- return count;  
- }  
- 
- ssize_t parport_read(struct file *filp, char *buf, size_t count, loff_t *f_ops)  
- {  
-     unsigned char status;  
- int loop;  
- for(loop = 0; loop < count; loop++)  
-     {  
-         status = inb(Drv_read_addr);  
-         put_user(status, (char *) &buf[loop]);  
-     }  
- return count;  
- }  
- 
- long    parport_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)  
- {  
- int loop;  
- struct dat data;  
- switch(cmd)  
-     {  
- case PARPORT_WRITE:  
- //          outb(status, Drv_write_addr);
-             copy_from_user(&data, (struct dat *)arg, sizeof(data));           
-             printk(KERN_ALERT "out put %d\n",data.loop);  
- for(loop = 0; loop < data.loop; loop ++)  
-             {  
-                 printk(KERN_ALERT "the %dth loop, write %d\n",loop,data.buf[loop]);  
-                 outb(data.buf[loop], Drv_write_addr);  
-                 wmb();  
-             }  
- break;  
- 
- case PARPORT_CLOSE:  
-             outb(0x00, Drv_write_addr);  
-             wmb();  
- break;  
-     }  
- return 0;  
- }  
- 
- int parport_release(struct inode *inode, struct file *filp)  
- {  
-     printk(KERN_ALERT "close the module parport_dev\n");  
- return 0;  
- }  
- 
- struct file_operations parport_fops = {  
-     .owner  =   THIS_MODULE,  
-     .write  =   parport_write,  
-     .read   =   parport_read,  
-     .open   =   parport_open,  
-     .unlocked_ioctl =   parport_ioctl,  
-     .release=   parport_release,  
- };  
- 
- int parport_init(void)  
- {  
- int result;  
- 
-     result = register_chrdev(Drv_major, Drv_name, &parport_fops);  
- if(result < 0)  
- return result;  
-     printk(KERN_ALERT "hello the module parport_dev\n");  
- 
- return 0;  
- }  
- 
- void parport_exit(void)  
- {  
-     printk(KERN_ALERT "exit the module parport_drv\n");  
-     unregister_chrdev(Drv_major, Drv_name);  
- 
- }  
- 
- module_init(parport_init);  
- module_exit(parport_exit);  
- </linux></linux></linux></linux></linux></linux></linux>  
**[cpp]**[view
 plain](http://blog.csdn.net/bg2bkk/article/details/8946424#)[copy](http://blog.csdn.net/bg2bkk/article/details/8946424#)
- <linux init="" h=""><linux module="" h=""><linux kernel="" h=""><linux fs="" h=""><linux cdev="" h=""><linux types="" h=""><linux uaccess="" h="">  
- </linux></linux></linux></linux></linux></linux></linux>  
**[cpp]**[view
 plain](http://blog.csdn.net/bg2bkk/article/details/8946424#)[copy](http://blog.csdn.net/bg2bkk/article/details/8946424#)
- <linux init="" h=""><linux module="" h=""><linux kernel="" h=""><linux fs="" h=""><linux cdev="" h=""><linux types="" h=""><linux uaccess="" h="">parport_drv.h  
- </linux></linux></linux></linux></linux></linux></linux>  
**[cpp]**[view
 plain](http://blog.csdn.net/bg2bkk/article/details/8946424#)[copy](http://blog.csdn.net/bg2bkk/article/details/8946424#)
- #ifndef<span style="white-space:pre">   </span>_PARPORT_DRV_H
- #define _PARPORT_DRV_H
- 
- 
- #define<span style="white-space:pre">   </span>PARPORT_WRITE<span style="white-space:pre">  </span>1
- #define<span style="white-space:pre">   </span>PARPORT_CLOSE<span style="white-space:pre">  </span>2
- 
- 
- struct dat{  
- <span style="white-space:pre">  </span>int loop;  
- <span style="white-space:pre">  </span>unsigned char *buf;  
- };  
- 
- 
- #endif
测试代码par_test.c
**[cpp]**[view
 plain](http://blog.csdn.net/bg2bkk/article/details/8946424#)[copy](http://blog.csdn.net/bg2bkk/article/details/8946424#)
- #include <stdio h="">
- #include <stdlib h="">
- #include <sys types="" h="">
- #include <sys stat="" h="">
- #include <fcntl h="">
- #include "parport_drv.h"
- #define DEVICE_NAME "/dev/parport_dev"
- 
- int main()  
- {  
- int fd;  
- char buf[128];  
- int loop;  
- 
-     fd = open(DEVICE_NAME, O_RDWR | O_NDELAY);  
- 
- if(fd < 0)  
-     {  
-         perror("open device");  
-         exit(1);  
-     }  
- else
-     {  
- //      printf("waiting for input ...\n");
- //
- //      while(1)
- //      {
- //          if(read(fd, buf, 1) == 1)
- //          {
- //              printf("read data [%2X]\n", buf[0] & 0xFF);
- //              if(!(buf[0] & 0x10))
- //                  break;
- //          }
- //          sleep(1);
- //      }
- //      printf("input ok ... \n");
- //      printf("led begins to flash ... \n");
- //      for(loop = 0; loop < 5; loop++)
- //      {
- //          buf[0] = 0xff;
- //          write(fd, buf, 1);
- //          sleep(1);
- //          buf[0] = 0x00;
- //          write(fd, buf, 1);
- //          sleep(1);
- //      }
- int i;  
- int arg=0x99;  
-         unsigned char buf[255];  
- struct dat da;  
-         da.loop = 4;  
-         da.buf = (unsigned char *)malloc(5 * sizeof(unsigned char));  
- for(i = 0;i< da.loop; i++)  
-             da.buf[i] = i*2+1;  
- for(i=0;i<da.loop;i++)  
-             printf("test:%d\n", da.buf[i]);  
-         ioctl(fd, PARPORT_WRITE,&da);  
-         sleep(1);  
-         ioctl(fd, PARPORT_CLOSE);  
-         sleep(1);  
-         close(fd);  
-     }  
- 
- return 0;  
- }  
-     </fcntl></sys></sys></stdlib></stdio>  
1 load_rdwrdev
**[cpp]**[view
 plain](http://blog.csdn.net/bg2bkk/article/details/8946424#)[copy](http://blog.csdn.net/bg2bkk/article/details/8946424#)
- #!/bin/sh
- 
- insmod parport_drv.ko  
- mknod /dev/parport_dev c 240 0  
2 unload_rdwrdev
**[cpp]**[view
 plain](http://blog.csdn.net/bg2bkk/article/details/8946424#)[copy](http://blog.csdn.net/bg2bkk/article/details/8946424#)
- #!/bin/sh
- 
- rmmod parport_drv.ko  
- rm /dev/parport_dev  
[](http://blog.csdn.net/bg2bkk/article/details/8946424#)[](http://blog.csdn.net/bg2bkk/article/details/8946424#)[](http://blog.csdn.net/bg2bkk/article/details/8946424#)[](http://blog.csdn.net/bg2bkk/article/details/8946424#)[](http://blog.csdn.net/bg2bkk/article/details/8946424#)[](http://blog.csdn.net/bg2bkk/article/details/8946424#)
- 
顶

