# I/O重定向和管道 - Orisun - 博客园







# [I/O重定向和管道](https://www.cnblogs.com/zhangchaoyang/articles/2300358.html)





## I/O重定向

重定向I/O的是shell而不是程序。

下面的例子证明了shell并不将重定向标记和文件名传递给程序。

```
#include<stdio.h>
main(int argc,char *argv[]){
    int i;
    printf("%d args:\n",argc);
    for(i=0;i<argc;i++){
        printf("%s\n",argv[i]);
    }
    fprintf(stderr,"this is message sent to stderr.\n");
}
```

$ ./listargs testing >xyz one two 2> oops

只有testing、one、two是命令的参数，>xyz和2>oops是用来作重定向的，且testing、one、two和>xyz、2>oops的位置可以任意的交换都不影响结果。

stdout上没有输出，输出全部到了xyz和oops两个文件里：

$ cat xyz

4 args:

./listargs

testing

one

two

$ cat oops

this message is sent to stderr.

最低可用文件描述符原则

每个进程都打开一组文件，这些文件被保存在一个数组中，文件描述符就是文件在此数组中的索引。当打开文件时，为此文件安排的文件描述符总是此数组中最低可用位置的索引。

因此将stdin定向到文件实现起来其实非常简单：先close(0)，再打开一个文件，这时分配给该文件的文件描述符就是0。在读写文件时我们依然使用stdin（数字0）这个文件描述符，实现是在操作一上普通文件，而非标准输入。


```
1 #include<stdio.h>
 2 #include<stdlib.h>
 3 #include<fcntl.h>
 4 #include<string.h>
 5 main(){
 6     int fd;
 7     char line[100];
 8     fgets(line,100,stdin);
 9     printf("%s",line);
10     close(0);
11     fd=open("/etc/passwd",O_RDONLY);
12     if(fd!=0){
13         fprintf(stderr,"Could not open data as fd 0\n");
14         exit(1);
15     }
16     fgets(line,100,stdin);
17     printf("%s",line);
18 }
```

$ gcc stdinredir1.c -o stdinredir1




$ ./stdinredir1

heoo chnn jdk

heoo chnn jdk

root:x:0:0:root:/root:/bin/bash

注意值为0的fd只能用来读，不能用来写，即上例中不能向/etc/passwd中写入内容，亦即16行不能用fputs。


将stdout重定向到文件

执行who>userlist发生了什么？首先要执行who命令，终端程序会fork一个子进程，然后在子进程中通过exec来执行who命令。重定向操作就发生在子进程调用exec之前。fork会复制父进程的代码和数据。exec会改变进程运行的程序，但它并不改变进程的属性和进程中所有的连接。即exec之后，进程的用户ID不变，优先级不变，文件描述符也不变。在子进程中close(1)，再打开一个文件，该文件自然就重定向到了标准输出。


```
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
main(){
    int pid;
    int fd;
    printf("About to run who into a file\n");
    if((pid=fork())==-1){   //创建子进程
        perror("perror");
        exit(1);
    }
    if(pid==0){     //在子进程中
        close(1);   //关闭文件描述符1
        fd=creat("userlist",0644);     //创建一个新文件，此时它获得文件描述任1
        execlp("who","who",NULL);       //如果本句执行成功，下面的代码就不会执行
        perror("execlp");       
        exit(1);
    }
    if(pid!=0){     //在父进程中等待子进程退出
        wait(NULL); //等待某个子进程退出
        printf("Done running who.results in userlist\n");
    }
}
```

$ ./stdoutredir
About to run who into a file
Done running who.results in userlist
$ cat userlist
orisun tty7 2011-12-24 14:21 (:0)
orisun pts/1 2011-12-24 14:44 (:0.0)





利用“最低可用文件描述符原则”来做文件重定向大部分情况下使用起来不够灵活，因为必须先关闭一个文件1，然后马上打开一个文件2，才能把文件1重定向到文件2，关闭1和打开2之间不能打开或关闭其他文件。可以使用dup2将一个文件描述符复制到另外一个文件描述符。

newfd=dup2(oldfd,newfd);       它可以任意指定oldfd和newfd



```
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
main(){
    int fd;
    int newfd;
    char line[100];
    fgets(line,100,stdin);
    printf("%s",line);
    fd=open("/etc/passwd",O_RDONLY);
    newfd=dup2(fd,0);      //将标准输入重定向到文件
    if(newfd!=0){
        fprintf(stderr,"Could not duplicate fd 0\n");
        exit(1);
    }
    close(fd);
    fgets(line,100,stdin);
    printf("%s",line);
}
```



$ ./stdinredir2

a s d f g

a s d f g

root:x:0:0:root:/root:/bin/bash


## 管道

想想who|sort是怎么实现的。who把输出送给stdout，sort从stdin中读入数据，那也就是说who的stdout和sort的stdin连成了一个。

#include<unistd.h>

result=pipe(int array[2])

array[0]是读端的文件描述符，array[1]是写端的文件描述符。

pipe调用首先获得两个“最低可用文件描述符”，赋给array[0]和array[1]，然后再把这两个文件描述符连接起来。

```
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#define BUFSIZE 20
main(){
    int len,i,apipe[2];     //apipe数组中存储两个文件的描述符
    char buf[BUFSIZE];
    if(pipe(apipe)==-1){
        perror("could not make pipe");
        exit(1);
    }
    printf("Got a pipe,it's file descriptions are:%d %d\n",apipe[0],apipe[1]);
    while(fgets(buf,BUFSIZE,stdin)){    //从标准输入读入数据，放到缓冲区
        len=strlen(buf);
        if(write(apipe[1],buf,len)!=len){   //向apipe[1](即管道写端)写入数据
            perror("write");
            break;
        }
        for(i=0;i<len;i++)      //清理缓冲区
            buf[i]='X';
        len=read(apipe[0],buf,BUFSIZE);     //从apipe[0](即管道读端)读数据
        if(len==-1){
            perror("read");
            break;
        }
        if(write(1,buf,len)!=len){      //把从管道读出的数据再写到标准输出
            perror("write");
            break;
        }
    }
}
```

$ ./pipedemo
Got a pipe,it's file descriptions are:3 4
a bird!
a bird!
two plane!
two plane!
^\Quit (core dumped)

使用管道实现父子进程之间的通信

其基本原理是这样的：假如原先在父进程中文件描述符3和4通过管道1连接起来（3是读端，4是写端），则fork创建子进程后，子进程中的文件描述符3和4也通过管道1连接起来（3是读端，4是写端）。这样一来，在父进程通过文件描述符4向管道写入内容后，在子进程中就可以通过文件描述符3从管道中读出数据（当然在父进程中也可以通过文件描述符3从管道中读出数据）。

```
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#define BUFSIZE 100
#define CHILD_MESS "I want a cookie\n"
#define PAR_MESS "testing..\n"
#define oops(m,x) {perror(m);exit(x);}      //还可以这样宏定义语句块
main(){
    int pipefd[2];
    int len;
    char buf[BUFSIZE];
    int read_len;
    if(pipe(pipefd)==-1)
        oops("pipe",1);
    switch(fork()){
        case -1:
            oops("fork",2);
        case 0:         //子进程中写管道写入内容
            len=strlen(CHILD_MESS);
            while(1){
                if(write(pipefd[1],CHILD_MESS,len)!=len)
                    oops("write",3);
                sleep(1);   //让给父进程占用CPU
            }
        default:        //在父进程中，先向管道写，再从管道读（连子进程向管道写的内容也读出来了）
            sleep(1);    //让子进程先执行
            len=strlen(PAR_MESS);
            if(write(pipefd[1],PAR_MESS,len)!=len)
                oops("write",4);
            read_len=read(pipefd[0],buf,BUFSIZE);
            if(read_len<=0)
                break;
            write(1,buf,read_len);      //把从管道读到的内容输出到标准输出
    }
}
```

$ ./pipedemo2
I want a cookie
testing..

管道是一个队列，当从管道读走数据后，数据就不存在了。如果两个进程都对管道进行读操作，当一个进程读走一些数据后，另一个进程读到的是后面的内容。













