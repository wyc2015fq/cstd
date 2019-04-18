# 文件位置指针对read、write的影响 - Orisun - 博客园







# [文件位置指针对read、write的影响](https://www.cnblogs.com/zhangchaoyang/articles/2317480.html)





首先明确一下，同一个文件在同一个进程中可以被打开多次，只是返回的文件描述符不同。

read、write、lseek都涉及到文件指针，文件指针是跟文件描述符关联在一起，跟物理文件是分开的。

每个文件描述符都有3个指针：begin、curr、end。可以有两种方法更改curr指针：每次write或read的末位置就是curr指针的位置；使用lseek(int fd,int offset,int whence)。每次read都从curr位置开始，每次write都从begin位置开始。

看一段程序：

```
1 #include<stdio.h>
 2 #include<string.h>
 3 #include<fcntl.h>
 4 #include<unistd.h>
 5 #include<sys/types.h>
 6 #include<sys/stat.h>
 7 
 8 main(){
 9     int fd1,fd2;
10     if((fd1=open("test",O_RDWR|O_CREAT,0600))==-1)
11         perror("open fd1");
12     if((fd2=open("test",O_RDWR|O_CREAT,0600))==-1)
13         perror("open fd2");
14     printf("fd1=%d\tfd2=%d\n",fd1,fd2);
15     if(write(fd1,"123\n",4)==-1)
16         perror("write fd1");
17     if(write(fd2,"456\n",4)==-1)
18         perror("write fd2");
19     lseek(fd1,0,SEEK_SET);
20     char buf[10]={0};
21     if(read(fd2,buf,4)<0)
22         perror("read");
23     printf("read from fd2 %s\n",buf);
24     memset(buf,0x00,sizeof(buf));
25     if(read(fd1,buf,4)<0)
26         perror("read");
27     printf("read from fd1 %s\n",buf);
28     close(fd1);
29     close(fd2);
30 }
```

输出：

fd1=3 fd2=4
read from fd2 
read from fd1 456

在上面的程序中，先是通过fd1在文件开头写入"123"，调用write(fd2,"456\n",4)时还是在文件开头写入，所以把“123”覆盖了。如果把第17行换成write(fd1,"456\n",4)则不会覆盖之前写入的内容。当调用lseek(fd1,0,SEEK_SET)之后，read(fd1,buf,4)就是从文件的开头开始读，而read(fd2,buf,4)是从write(fd2,"456\n",4)结束的位置开始读，所以读到的内容为空。

fork后子进程复制了父进程的文件描述符。

```
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>

main(){
    int fd1;
    if((fd1=open("./file1",O_RDWR))==-1)
        perror("openfd1");
    pid_t pid=fork();
    if(pid==0){
        sleep(1);
        int fd2;
        if((fd2=open("./file1",O_RDWR))==-1)
            perror("openfd2");
        printf("fd1=%d\tfd2=%d\n",fd1,fd2);
        lseek(fd1,0,SEEK_SET);
        if(write(fd1,"a",1)==-1)
            perror("writefd1");
        lseek(fd2,1,SEEK_SET);
        if(write(fd2,"b",1)==-1)
            perror("writefd2");
        close(fd1);
        close(fd2);
    }
    else{
        printf("fd1=%d\n",fd1);
        lseek(fd1,0,SEEK_SET);
        if(write(fd1,"c",1)==-1)
            perror("writefd1again");
        close(fd1);
    }
}
```

上述代码相当于文件被打开了3次--所以也要close()3次。文件最终的内容是：ab。
在父进程中已经打开的文件描述符（包括套接口描述符等），在子进程中可以直接使用，每fork一次，这些描述符的引用计数就被加1,所以也要关闭多次，直到引用计数为0.












