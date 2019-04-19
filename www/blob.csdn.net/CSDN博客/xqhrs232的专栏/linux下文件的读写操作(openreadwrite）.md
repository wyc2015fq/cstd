# linux下文件的读写操作(openreadwrite） - xqhrs232的专栏 - CSDN博客
2016年12月30日 11:22:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：363
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.2cto.com/os/201403/285837.html](http://www.2cto.com/os/201403/285837.html)
**open（打开文件）**
相关函数 read，write，fcntl，close，link，stat，umask，unlink，fopen 
表头文件 #include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h> 
定义函数 int open( const char * pathname, int flags);
int open( const char * pathname,int flags, mode_t mode);
函数说明 参数pathname 指向欲打开的文件路径字符串。下列是参数flags 所能使用的旗标:
O_RDONLY 以只读方式打开文件
O_WRONLY 以只写方式打开文件
O_RDWR 以可读写方式打开文件。上述三种旗标是互斥的，也就是不可同时使用，但可与下列的旗标利用OR(|)运算符组合。
O_CREAT 若欲打开的文件不存在则自动建立该文件。
O_EXCL 如果O_CREAT 也被设置，此指令会去检查文件是否存在。文件若不存在则建立该文件，否则将导致打开文件错误。此外，若O_CREAT与O_EXCL同时设置，并且欲打开的文件为符号连接，则会打开文件失败。
O_NOCTTY 如果欲打开的文件为终端机设备时，则不会将该终端机当成进程控制终端机。
O_TRUNC 若文件存在并且以可写的方式打开时，此旗标会令文件长度清为0，而原来存于该文件的资料也会消失。
O_APPEND 当读写文件时会从文件尾开始移动，也就是所写入的数据会以附加的方式加入到文件后面。
O_NONBLOCK 以不可阻断的方式打开文件，也就是无论有无数据读取或等待，都会立即返回进程之中。
O_NDELAY 同O_NONBLOCK。
O_SYNC 以同步的方式打开文件。
O_NOFOLLOW 如果参数pathname 所指的文件为一符号连接，则会令打开文件失败。
O_DIRECTORY 如果参数pathname 所指的文件并非为一目录，则会令打开文件失败。
此为[Linux](http://www.2cto.com/os/linux/)2.2以后特有的旗标，以避免一些[](http://www.2cto.com/Article/xitong/)[系统](http://www.2cto.com/os/)安全问题。参数mode
 则有下列数种组合，只有在建立新文件时才会生效，此外真正建文件时的权限会受到umask值所影响，因此该文件权限应该为（mode-umaks）。
S_IRWXU00700 权限，代表该文件所有者具有可读、可写及可执行的权限。
S_IRUSR 或S_IREAD，00400权限，代表该文件所有者具有可读取的权限。
S_IWUSR 或S_IWRITE，00200 权限，代表该文件所有者具有可写入的权限。
S_IXUSR 或S_IEXEC，00100 权限，代表该文件所有者具有可执行的权限。
S_IRWXG 00070权限，代表该文件用户组具有可读、可写及可执行的权限。
S_IRGRP 00040 权限，代表该文件用户组具有可读的权限。
S_IWGRP 00020权限，代表该文件用户组具有可写入的权限。
S_IXGRP 00010 权限，代表该文件用户组具有可执行的权限。
S_IRWXO 00007权限，代表其他用户具有可读、可写及可执行的权限。
S_IROTH 00004 权限，代表其他用户具有可读的权限
S_IWOTH 00002权限，代表其他用户具有可写入的权限。
S_IXOTH 00001 权限，代表其他用户具有可执行的权限。
返回值 若所有欲核查的权限都通过了检查则返回0 值，表示成功，只要有一个权限被禁止则返回-1。
**read（由已打开的文件读取数据）**
相关函数 readdir，write，fcntl，close，lseek，readlink，fread 
表头文件 #include<unistd.h> 
定义函数 ssize_t read(int fd,void * buf ,size_t count); 
函数说明 read()会把参数fd 所指的文件传送count个字节到buf指针所指的内存中。若参数count为0，则read()不会有作用并返回0。返回值为实际读取到的字节数，如果返回0，表示已到达文件尾或是无可读取的数据，此外文件读写位置会随读取到的字节移动。
附加说明 如果顺利read()会返回实际读到的字节数，最好能将返回值与参数count 作比较，若返回的字节数比要求读取的字节数少，则有可能读到了文件尾、从管道(pipe)或终端机读取，或者是read()被信号中断了读取动作。当有错误发生时则返回-1，错误代码存入errno中，而文件读写位置则无法预期。
错误代码 EINTR 此调用被信号所中断。
EAGAIN 当使用不可阻断I/O 时（O_NONBLOCK），若无数据可读取则返回此值。
EBADF 参数fd 非有效的文件描述词，或该文件已关闭。
范例 参考open（）。
**sync（将缓冲区数据写回磁盘）**
相关函数 fsync 
表头文件 #include<unistd.h> 
定义函数 int sync(void) 
函数说明 sync()负责将系统缓冲区数据写回磁盘，以确保数据同步。 
返回值 返回0。
**write（将数据写入已打开的文件内）**
相关函数 open，read，fcntl，close，lseek，sync，fsync，fwrite
表头文件 #include<unistd.h> 
定义函数 ssize_t write (int fd,const void * buf,size_t count); 
函数说明 write()会把参数buf所指的内存写入count个字节到参数fd所指的文件内。当然，文件读写位置也会随之移动。 
返回值 如果顺利write()会返回实际写入的字节数。当有错误发生时则返回-1，错误代码存入errno中。 
错误代码 EINTR 此调用被信号所中断。
EAGAIN 当使用不可阻断I/O 时（O_NONBLOCK），若无数据可读取则返回此值。
EADF 参数fd非有效的文件描述词，或该文件已关闭。 
范例 请参考open（）。
**lseek用于文件位置定位**
int fd = open(file_name,O_RDONLY);
if (fd<0) return -1;
long fsize = lseek(fd,0L,SEEK_END);
close(fd);
lseek用于文件位置定位
函数原形：off_t lseek(int fildes, off_t offset, int whence);
fildes,表示打开的文件描述符
offset,表示操作需要移动的相对量
whence,标示文件移动的方向
其取值有如下三种情况：
lseek(int fildes, off_t offset, SEEK_SET);
返回值即为文件开头起始之后的offset位置，seek的起始位置为文件头
lseek(int fildes, off_t offset, SEEK_CUR);
返回值即为文件当前偏移量+offset的值，seek的起始位置为当前位置
lseek(int fildes, off_t offset, SEEK_END);
返回值即为文件大小+offset，seed的起始位置为文件末尾
[offset 可正可负，表示相对位置的前后关系]
偏移量offset已字节为单元。在成功调用情况下的返回值表示相对于文件头的文件读取偏移量
在调用失败情况下，将返回-1
lseek将当前的文件偏移量记录在内核之中, 而并不会引起任何实际的I/O操作,之后的文件读/写操作将在该偏移量上执行.对与文件偏移量大于文件当前长度情况下, 对该文件的写操作将导致在文件中形成一段空洞,即那段没有写过字节的位移段被读为0..
所以,比较高效的求文件大小,就如前程序片断所示：
long fsize = lseek(fd,0L,SEEK_END);
ｏｆｆsize参数=0;
whence = SEEK_END;
返回值即为文件大小.
测试代码：
```java
root@wl-MS-7673:/home/wl/桌面/c++# cat -n file_copy.cpp 
     1	
     2	#include <stdio.h>
     3	#include <string.h>
     4	#include <stdlib.h>
     5	#include <sys/types.h>
     6	#include <sys/stat.h>
     7	#include <unistd.h>
     8	#include <fcntl.h>
     9	#define BUFFER_SIZE 1024
    10	
    11	/*
    12	 * 程序入口
    13	 * */
    14	int main(int argc,char **argv)
    15	{
    16	    int from_fd, to_fd;
    17	    long file_len=0;
    18	    int ret=1;
    19	    char buffer[BUFFER_SIZE];
    20	    char *ptr;
    21	
    22	    /*判断入参*/
    23	    if(argc!=3)
    24	    {
    25	        printf("Usage:%s fromfile tofile\n",argv[0]);
    26	        exit(1);
    27	    } 
    28	
    29	    /*打开源文件*/ 
    30	    if((from_fd=open(argv[1], O_RDONLY|O_CREAT))==-1) 
    31	    { 
    32	        printf("Open %s Error\n",argv[1]); 
    33	        exit(1); 
    34	    } 
    35	
    36	    /*创建目的文件*/ 
    37	    if((to_fd=open(argv[2], O_WRONLY|O_CREAT))==-1) 
    38	    { 
    39	        printf("Open %s Error\n",argv[2]); 
    40	        exit(1); 
    41	    } 
    42	
    43	    /*测得文件大小*/
    44	    file_len= lseek(from_fd,0L,SEEK_END);
    45	    lseek(from_fd,0L,SEEK_SET);
    46	    printf("form file size is %d\n",file_len);
    47	
    48	    /*进行文件拷贝*/
    49	    while(ret) 
    50	    { 
    51	        ret= read(from_fd, buffer, BUFFER_SIZE);
    52	        if(ret==-1)
    53	        {
    54	            printf("read Error\n"); 
    55	            exit(1);		
    56	        }
    57	        write(to_fd, buffer, ret);
    58	        file_len-=ret;
    59	        bzero(buffer,BUFFER_SIZE);
    60	    } 
    61	    printf("there are %d byte(s) data left without copy\n", file_len);
    62	    close(from_fd); 
    63	    close(to_fd); 
    64	    exit(0); 
    65	}
    66	
    67	
root@wl-MS-7673:/home/wl/桌面/c++# g++ file_copy.cpp -o file_copy
file_copy.cpp: 在函数‘int main(int, char**)’中:
file_copy.cpp:46:45: 警告： 格式 ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘long int’ [-Wformat]
file_copy.cpp:61:69: 警告： 格式 ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘long int’ [-Wformat]
root@wl-MS-7673:/home/wl/桌面/c++# vim file_copy.cpp
root@wl-MS-7673:/home/wl/桌面/c++# ./file_copy  file_copy.cpp  test.cpp
form file size is 1348
there are 0 byte(s) data left without copy
root@wl-MS-7673:/home/wl/桌面/c++# diff file_copy.cpp test.cpp 
root@wl-MS-7673:/home/wl/桌面/c++#
```
