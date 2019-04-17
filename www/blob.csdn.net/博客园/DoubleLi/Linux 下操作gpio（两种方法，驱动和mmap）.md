# Linux 下操作gpio（两种方法，驱动和mmap） - DoubleLi - 博客园






目前我所知道的在linux下操作GPIO有两种方法: 

1．  编写驱动,这当然要熟悉linux下驱动的编写方法和技巧，在驱动里可以使用ioremap函数获得GPIO物理基地址指针,然后使用这个指针根据ioctl命令进行GPIO寄存器的读写，并把结果回送到应用层。这里提供一点程序片断供大家参考:



**[cpp]**[view plain](http://blog.csdn.net/wangrunmin/article/details/6671603)[copy](http://blog.csdn.net/wangrunmin/article/details/6671603)



- int  init_module(void){   
- 
-         printk(KERN_ALERT "ioctl load.\r\n");   
- 
-     register_chrdev(254,"ioreg",&fops);   
- 
-         stb_gpio = (STBX25XX_GPIO_REG *)ioremap(GPIO_BASE,GPIO_LEN);   
- 
- if(stb_gpio == NULL){   
- 
-             printk(KERN_ALERT "can''t get io base.\r\n");   
- 
- return -1;   
- 
-         }   
- 
- return 0;   
- 
- }   
- 
- int io_ioctl (struct inode *inode, struct file *filp,unsigned int cmd, unsigned long arg){  
- 
-     unsigned long uGpio;   
- 
-     printk(KERN_ALERT "io_ioctl cmd=%04x,arg=%04x.\r\n",cmd,(int)arg);   
- 
- switch(cmd){   
- 
- case SET_IO_DIR:{   
- 
-              printk(KERN_ALERT "SET_IO_DIR\r\n");   
- 
- break;   
- 
-         }   
- 
- case SET_IO_VALUE:{   
- 
-              printk(KERN_ALERT "SET_IO_VALUE\r\n");   
- 
- break;   
- 
-         }   
- 
- case GET_IO_VALUE:{   
- 
-              printk(KERN_ALERT "GET_IO_VALUE\r\n");   
- 
-              uGpio = stb_gpio->GPI;   
- 
-              printk(KERN_ALERT "GPIO = %08x",(int)uGpio);   
- 
-              copy_to_user((void *)arg,(const void *) &uGpio,sizeof(uGpio));   
- 
- break;   
- 
-         }   
- 
- case GET_IO_DIR:{   
- 
-              printk(KERN_ALERT "GET_IO_DIR\r\n");   
- 
- break;   
- 
-         }   
- 
-     }   
- 
- return 0;   
- 
- }   
- 
- 






2．  在应用层使用mmap函数在应用层获得GPIO物理基地址对应的虚拟地址指针,然后使用这个指针来读写GPIO寄存器,这里提供一点程序片断供大家参考: 





**[cpp]**[view plain](http://blog.csdn.net/wangrunmin/article/details/6671603)[copy](http://blog.csdn.net/wangrunmin/article/details/6671603)



- char dev_name[] = "/dev/mem";   
- 
-         GPIO_REGISTER  *gpio_base;   
- 
-         fd  =  open(dev_name,O_RDWR);   
- 
- if(fd<0){   
- 
-                printf("open %s is error\n",dev_name);   
- 
- return -1 ;   
- 
-         }   
- 
-         gpio_base = (GPIO_REGISTER *)mmap( 0, 0x32, PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0x40060000 );  
- 
- if(gpio_base == NULL){   
- 
-                 printf("gpio base mmap is error\n");   
- 
-                 close(fd);   
- 
- return -1;   
- 
-         }   
- 
-         gpio_base->or  = (gpio_base->or & 0x7fffffff);   





**mmap**

功能描述： 
mmap将一个文件或者其它对象映射进内存。文件被映射到多个页上，如果文件的大小不是所有页的大小之和，最后一个页不被使用的空间将会清零。munmap执行相反的操作，删除特定地址区域的对象映射。
基于文件的映射，在mmap和munmap执行过程的任何时刻，被映射文件的st_atime可能被更新。如果st_atime字段在前述的情况下没有得到更新，首次对映射区的第一个页索引时会更新该字段的值。用PROT_WRITE 和 MAP_SHARED标志建立起来的文件映射，其st_ctime 和 st_mtime
在对映射区写入之后，但在msync()通过MS_SYNC 和 MS_ASYNC两个标志调用之前会被更新。

用法：  
#include <sys/mman.h>
void *mmap(void *start, size_t length, int prot, int flags,
    int fd, off_t offset);
int munmap(void *start, size_t length); 

参数：   
start：映射区的开始地址。
length：映射区的长度。
prot：期望的内存保护标志，不能与文件的打开模式冲突。是以下的某个值，可以通过or运算合理地组合在一起
PROT_EXEC //页内容可以被执行
PROT_READ  //页内容可以被读取
PROT_WRITE //页可以被写入
PROT_NONE  //页不可访问
flags：指定映射对象的类型，映射选项和映射页是否可以共享。它的值可以是一个或者多个以下位的组合体
MAP_FIXED //使用指定的映射起始地址，如果由start和len参数指定的内存区重叠于现存的映射空间，重叠部分将会被丢弃。如果指定的起始地址不可用，操作将会失败。并且起始地址必须落在页的边界上。
MAP_SHARED //与其它所有映射这个对象的进程共享映射空间。对共享区的写入，相当于输出到文件。直到msync()或者munmap()被调用，文件实际上不会被更新。
MAP_PRIVATE //建立一个写入时拷贝的私有映射。内存区域的写入不会影响到原文件。这个标志和以上标志是互斥的，只能使用其中一个。
MAP_DENYWRITE //这个标志被忽略。
MAP_EXECUTABLE //同上
MAP_NORESERVE //不要为这个映射保留交换空间。当交换空间被保留，对映射区修改的可能会得到保证。当交换空间不被保留，同时内存不足，对映射区的修改会引起段违例信号。
MAP_LOCKED //锁定映射区的页面，从而防止页面被交换出内存。
MAP_GROWSDOWN //用于堆栈，告诉内核VM系统，映射区可以向下扩展。
MAP_ANONYMOUS //匿名映射，映射区不与任何文件关联。
MAP_ANON //MAP_ANONYMOUS的别称，不再被使用。
MAP_FILE //兼容标志，被忽略。
MAP_32BIT //将映射区放在进程地址空间的低2GB，MAP_FIXED指定时会被忽略。当前这个标志只在x86-64平台上得到支持。
MAP_POPULATE //为文件映射通过预读的方式准备好页表。随后对映射区的访问不会被页违例阻塞。
MAP_NONBLOCK //仅和MAP_POPULATE一起使用时才有意义。不执行预读，只为已存在于内存中的页面建立页表入口。
fd：有效的文件描述词。如果MAP_ANONYMOUS被设定，为了兼容问题，其值应为-1。
offset：被映射对象内容的起点。


返回说明：   
成功执行时，mmap()返回被映射区的指针，munmap()返回0。失败时，mmap()返回MAP_FAILED[其值为(void *)-1]，munmap返回-1。errno被设为以下的某个值   
EACCES：访问出错
EAGAIN：文件已被锁定，或者太多的内存已被锁定
EBADF：fd不是有效的文件描述词
EINVAL：一个或者多个参数无效
ENFILE：已达到系统对打开文件的限制
ENODEV：指定文件所在的文件系统不支持内存映射
ENOMEM：内存不足，或者进程已超出最大内存映射数量
EPERM：权能不足，操作不允许
ETXTBSY：已写的方式打开文件，同时指定MAP_DENYWRITE标志
SIGSEGV：试着向只读区写入
SIGBUS：试着访问不属于进程的内存区









