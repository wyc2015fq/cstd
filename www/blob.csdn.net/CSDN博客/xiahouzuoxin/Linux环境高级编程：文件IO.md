# Linux环境高级编程：文件IO - xiahouzuoxin - CSDN博客





2012年10月14日 21:26:02[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2247








        这里提到的文件IO操作指不带缓存的IO操作，也就是说这里提到的函数的实现都是通过系统调用实现的，而不是直接对IO端口操作实现的。




1、文件描述符（File Discriptor，fd）——Linux文件的身份证号码

        文件描述符是一个非负的整数，当打开一个文件或创建一个文件时，内核向进程返回一个标识该文件的整数，即fd。注意，若文件不被创建打开（即没有载入进程），是没有文件描述符可言的。

        必需明确一个概念：LInux中的一切即是文件。因此对于特殊的文件有特殊的文件描述符。


        特殊文件描述符：0——标准输入，1——标准输出，2——标准错误输出。





2、open函数

#include <fcntl.h>


int open(const char *pathname, int oflag, mode_t mode);

返回值：文件描述符fd


oflag可选值：

（1）不能进行或运算的选项


O_RDONLY

O_WRONLY

O_RDWR

（2）可进行或运算选项

O_APPEND：追加

O_CREAT：若文件不存在则创建，只有此时mode参数有效

O_EXCL：如果同时指定了O_CREAT，而文件又存在，则出错

O_TRUNC

O_NOCTTY

O_NONBLOCK：FIFO/快特殊文件/字符文件 中使用

O_SYNC：使每次write操作都等待到物理IO操作完成，使用该项，可能耗时更长





注：

另有creat函数可创建文件，int creat(const char *pathname, mode_t mode)


等效于open(pathname, O_WRONLY|OCREAT|O_TRUNC,  mode)





3、read 函数

#include <unistd.h>


ssize_t read(int fd, void *buff, size_t nbytes);

返回值：成功读取字节数

fd：打开文件时的文件描述符




4、write函数

#include <unistd.h>


ssize_t write(int fd, void *buff, size_t nbytes);

返回值：成功写入的字节数

fd：打开文件时的文件描述符




5、lseek函数——操纵文件偏移量

#include <sys/types.h>


#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence);

返回值：若执行成功，返回新的文件位移量；如果fd为FIFO或管道文件，则返回-1，并保存errno=EPIPE


whence取值：

SEEK_SET：offset为相对文件头的偏移量

SEEK_CUR：offset为相对当前位置的偏移量

SEEK_END：offset为相对文件尾的偏移量




6、fcntl函数——读取修改文件属性

#include <sys/types.h>

#include <unistd.h>

#include <fcntl.h>

int fcntl(int fd, int cmd, .../* int arg */);

返回值：失败则返回-1

fcntl函数的5种功能：

（1）复制一个现存的描述符（cmd=F_DUPFD）

（2）获得/设置文件描述符标记（cmd=F_GETFD/F_SETFD）

（3）获得/设置文件状态标记（cmd=F_GETFL/FSETFL）——open函数中的oflag参数

（4）获得/设置异步IO权限（cmd=F_GETOWN/F_SETOWN）

（5）获得/设置记录锁（cmd=F_GETLK/F_SETLK/F_SETLKW）




7、打开文件的内核数据结构

每个**打开文件**都在进程表项中文件描述符表（文件的fd、指向文件表项的指针）；文件表项包括文件状态标志、文件当前偏移量、指向该文件v节点表项的指针；v节点表项包含了文件的v节点信息，i节点信息和文件长度等信息。

![](https://img-my.csdn.net/uploads/201210/14/1350219826_4529.png)




了解打开文件的内核数据结构对了解文件共享很有帮助。

lseek函数操作的文件偏移量（offset）应该是在文件表当中。

2个进程同时打开同一个文件，则同一个文件会对应2个不同的文件描述符，文件描述符是与进程相关联的。




8、程序实例

（1）文件读写——实例1




```cpp
18 #include "apue.h"
 19 
 20 #define BUFFERSIZE          8192
 21 
 22 int main(void)
 23 {
 24         int n;
 25         char buf[BUFFERSIZE];
 26 
 27         /* 从标准输入读数据，写入到标准输出 */
 28         while( (n = read(STDIN_FILENO, buf, BUFFERSIZE)) > 0 )
 29         {
 30                 if(write(STDOUT_FILENO, buf, n) != n)
 31                         err_sys("write error");
 32         }
 33 
 34         if(n < 0)
 35         {
 36                 err_sys("read error");
 37         }
 38 
 39         /* 考虑到进程关闭前会自动关闭文件，所以没有手动关闭文件 */
 40 
 41         return 0;
 42 }
```


程序功能：从标准输入端（对应文件描述符=STDIN_FILENO）读入数据，输出到标准输出端（对应文件描述符=STDOUT_FILENO）。

因为现在标准输入端为键盘，标准输出端为显示器，所以实验结果就是在终端输入什么就输出什么。



```
[monkeyzx@CentOS File]$ ./p3_3
xiahouzuoxin hello,file............
xiahouzuoxin hello,file............
second time
second time
third time
third time
^C
[monkeyzx@CentOS File]$
```





（2）fcntl函数读取oflags——实例2



```cpp
18 #include <sys/types.h>
 19 #include <fcntl.h>
 20 #include "apue.h"
 21 
 22 int main(int argc, char *argv[])
 23 {
 24         int accmode, val;
 25 
 26         if(argc != 2)
 27                 err_quit("Usage: p3_4 <descriptor#>");
 28 
 29         if( (val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0 )
 30                 err_sys("fcntl error for fd %d", atoi(argv[1]));
 31 
 32         accmode = val & O_ACCMODE;    /* O_ACCMODE=0x03,用于取出低2位 */
 33         if     (accmode == O_RDONLY) printf("read only!");
 34         else if(accmode == O_WRONLY) printf("write only!");
 35         else if(accmode == O_RDWR) printf("read write!");
 36         else err_dump("unknown access mode!");
 37 
 38         if(val & O_APPEND) printf(", append");
 39         if(val & O_NONBLOCK) printf(", nonblocking");
 40 #if !defined(_POSIX_SOURCE) && defined(O_SYNC)
 41         if(val & O_SYNC) printf(", synchronous writes");
 42 #endif
 43         putchar('\n');
 44 
 45         return 0;
 46 }
```




程序功能：对于指定的文件描述符打印文件描述标志，注意程序中没有使用打开文件操作，实验时使用交互的打开文件操作。



```
[monkeyzx@CentOS File]$ ./p3_4 0 < temp.foo
read only!
[monkeyzx@CentOS File]$ ./p3_4 1
read write!
[monkeyzx@CentOS File]$ ./p3_4 2 2>>temp.foo
write only!, append
[monkeyzx@CentOS File]$
```




（3）fcntl函数标志设置——实例3



```cpp
18 #include <fcntl.h>
 19 #include "apue.h"
 20 
 21 #define CLR_FLAG      (0)
 22 #define SET_FLAG      (1)
 23 
 24 void opt_fl(int fd, int flags, int opt)
 25 {
 26         int val;
 27 
 28         if( (val = fcntl(fd, F_GETFL, 0)) < 0 )
 29                 err_sys("fcntl F_GETFL error");
 30 
 31         switch(opt)
 32         {
 33                 case CLR_FLAG: val &= ~flags; break;
 34                 case SET_FLAG: val |= flags; break;
 35                 default:break;
 36         }
 37 
 38         if( fcntl(fd, F_SETFL, val) < 0 )
 39         {
 40                 err_sys("fcntl F_SETFL error");
 41         }
 42 }
 43 
 44 int main(int argc, char *argv[])
 45 {
 46         int fd;
 47 
 48         if(argc != 2)
 49                 err_quit("Usage: ./p3_4 <filename>");
 50 
 51         if( (fd=open(argv[1], O_RDWR)) < 0 )
 52                 err_sys("open %s error", argv[1]);
 53 
 54         opt_fl(fd, O_RDONLY, SET_FLAG);
 55 
 56         exit(0);
 57 }
```

opt_fl函数功能：对于一个文件描述符，打开或清楚一个或多个文件状态标志。





注：

（1）对于几个程序例子，都没有使用close函数将文件关闭的原因是——当进程执行结束时会自动将文件关闭，因此可以不必手动添加。但标准C的fclose函数则不能省略。

（2）在进行文件IO程序的测试时，可以使用交互式的SHELL完成打开文件的操作，例如实例2

（3）程序中包含的头文件apue.h为参考书籍中为简化代码提供的头文件




【参考书籍】《UNIX环境高级编程》




