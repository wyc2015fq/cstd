# Posix多线程编程学习笔记（六）—共享内存（3） - ljx0305的专栏 - CSDN博客
2008年08月01日 13:53:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：850
**五、共享内存区的写入和读出<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />**
上面我们介绍了mmap函数，下面我们就可以通过这些函数，把进程映射到共享内存区。
然后我们就可以通过共享内存区进行进程间通信了。
下面是共享内存区写入的例子：
/*shm_write.h写入/读出共享内存区*/
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
int main(int argc,char **argv)
{
int shm_id;
struct stat buf;
char *ptr;
if(argc!=2)
{
printf(“usage:shm_open <pathname>/n”);
exit(1);
}
shm_id=shm_open(argv[1],O_RDWR|O_CREAT,0644);/*创建共享内存区*/
ftruncate(shm_id,100);/*修改共享区大小*/
fstat(shm_id,&buf);
ptr=mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/
strcpy(ptr,”hello linux”);/*写入共享内存区*/
printf(“%s/n”,ptr);/*读出共享内存区*/
shm_unlink(argv[1]);/*删除共享内存区*/
}
下面是运行结果：
#cc –lrt –o shm_write shm_write.c
#./shm_write test
hello linux
**六、程序例子**
下面是利用pisix共享内存区实现进程间通信的例子：服务器进程读出共享内存区内容，然后清空。客户进程向共享内存区写入数据。直到用户输入“q”程序结束。程序用posix信号量实现互斥。
/*server.c服务器程序*/
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
int main(int argc,char **argv)
{
int shm_id;
char *ptr;
sem_t *sem;
if(argc!=2)
{
printf(“usage:shm_open <pathname>/n”);
exit(1);
}
shm_id=shm_open(argv[1],O_RDWR|O_CREAT,0644);/*创建共享内存区*/
ftruncate(shm_id,100);/*调整共享内存区大小*/
sem=sem_open(argv[1],O_CREAD,0644,1);/*创建信号量*/
ptr=mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/
strcpy(ptr,”/<?xml:namespace prefix = st1 ns = "urn:schemas-microsoft-com:office:smarttags" />0”);
while(1)
{
if((strcmp(ptr,”/0”))==0)/*如果为空，则等待*/
continue;
else
{
if((strcmp(ptr,”q/n”))==0)/*如果内存为q/n退出循环*/
break;
sem_wait(sem);/*申请信号量*/
printf(“server:%s”,ptr);/*输入共享内存区内容*/
strcpy(ptr,”/0”);/*清空共享内存区*/
sem_pose(sem);/*释放信号量*/
}
sem_unlink(argv[1]);/*删除信号量*/
shm_unlink(argv[1]);/*删除共享内存区*/
}
}
/*server.c服务器程序*/
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
int main(int argc,char **argv)
{
int shm_id;
char *ptr;
sem_t *sem;
if(argc!=2)
{
printf(“usage:shm_open <pathname>/n”);
exit(1);
}
shm_id=shm_open(argv[1],0);/*打开共享内存区
sem=sem_open(argv[1],0);/*打开信号量*/
ptr=mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/
while(1)
{
sem_wait(sem);/*申请信号量*/
fgets(ptr,10,stdin);/*从键盘读入数据到共享内存区*/
printf(“user:%s”,ptr);
if((strcmp(ptr,”q/n”))==0)
exit(0);
sem_pose(sem);/*释放信号量*/
sleep(1);
}
exit(0);
}
#cc –lrt –o server server.c
#cc –lrt –o user user.c
#./server test&
#./user test
输入:abc
user:abc
server:abc
输入:123
user:123
server:123
输入:q
user:q
引用： [http://blog.chinaunix.net/u/22935/showart_341896.html](http://blog.chinaunix.net/u/22935/showart_341896.html)
