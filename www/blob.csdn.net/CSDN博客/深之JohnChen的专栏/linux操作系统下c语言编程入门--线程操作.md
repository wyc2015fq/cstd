# linux操作系统下c语言编程入门--线程操作  - 深之JohnChen的专栏 - CSDN博客

2007年02月28日 18:16:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1840


 linux操作系统下c语言编程入门--线程操作 

前言:Linux下线程的创建
介绍在Linux下线程的创建和基本的使用. Linux下的线程是一个非常复杂的问题,由
于我对线程的学习不时很好,我在这里只是简单的介绍线程的创建和基本的使用,关于线
程的高级使用(如线程的属性,线程的互斥,线程的同步等等问题)可以参考我后面给出的
资料. 现在关于线程的资料在网络上可以找到许多英文资料,后面我罗列了许多链接,对
线程的高级属性感兴趣的话可以参考一下. 等到我对线程的了解比较深刻的时候,我回来完成这篇文章.如果您对线程了解的详尽我也非常高兴能够由您来完善.
先介绍什么是线程.我们编写的程序大多数可以看成是单线程的.就是程序是按照一定的
顺序来执行.如果我们使用线程的话,程序就会在我们创建线成的地方分叉,变成两个"程
序"在执行.粗略的看来好象和子进程差不多的,其实不然.子进程是通过拷贝父进程的地
址空间来执行的.而线程是通过共享程序代码来执行的,讲的通俗一点就是线程的相同的
代码会被执行几次.使用线程的好处是可以节省资源,由于线程是通过共享代码的,所以没
有进程调度那么复杂.
线程的创建和使用
线程的创建是用下面的几个函数来实现的.
#include <pthread.h>
int pthread_create(pthread_t *thread,pthread_attr_t *attr,
void *(*start_routine)(void *),void *arg);
void pthread_exit(void *retval);
int pthread_join(pthread *thread,void **thread_return);
pthread_create创建一个线程,thread是用来表明创建线程的ID,attr指出线程创建时候
的属性,我们用NULL来表明使用缺省属性.start_routine函数指针是线程创建成功后开始
执行的函数,arg是这个函数的唯一一个参数.表明传递给start_routine的参数. pthrea
d_exit函数和exit函数类似用来退出线程.这个函数结束线程,释放函数的资源,并在最后
阻塞,直到其他线程使用pthread_join函数等待它.然后将*retval的值传递给**thread_
return.由于这个函数释放所以的函数资源,所以retval不能够指向函数的局部变量. pt
hread_join和wait调用一样用来等待指定的线程. 下面我们使用一个实例来解释一下使
用方法.在实践中,我们经常要备份一些文件.下面这个程序可以实现当前目录下的所有文
件备份.备份后的后缀名为bak
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#define BUFFER 512
struct copy_file {
int infile;
int outfile;
};
void *copy(void *arg)
{
int infile,outfile;
int bytes_read,bytes_write,*bytes_copy_p;
char buffer[BUFFER],*buffer_p;
struct copy_file *file=(struct copy_file *)arg;
infile=file->infile;
outfile=file->outfile;
/* 因为线程退出时,所有的变量空间都要被释放,所以我们只好自己分配内存了 */
if((bytes_copy_p=(int *)malloc(sizeof(int)))==NULL) pthread_exit(NULL);
bytes_read=bytes_write=0;
*bytes_copy_p=0;
/* 还记得怎么拷贝文件吗 */
while((bytes_read=read(infile,buffer,BUFFER))!=0)
{
if((bytes_read==-1)&&(errno!=EINTR))break;
else if(bytes_read>0)
{
buffer_p=buffer;
while((bytes_write=write(outfile,buffer_p,bytes_read))!=0)
{
if((bytes_write==-1)&&(errno!=EINTR))break;
else if(bytes_write==bytes_read)break;
else if(bytes_write>0)
{
buffer_p+=bytes_write;
bytes_read-=bytes_write;
}
}
if(bytes_write==-1)break;
*bytes_copy_p+=bytes_read;
}
} 
close(infile);
close(outfile);
pthread_exit(bytes_copy_p);
}
int main(int argc,char **argv)
{
pthread_t *thread;
struct copy_file *file;
int byte_copy,*byte_copy_p,num,i,j;
char filename[BUFFER];
struct dirent **namelist;
struct stat filestat;
/* 得到当前路径下面所有的文件(包含目录)的个数 */
if((num=scandir(".",&namelist,0,alphasort))<0)
{
fprintf(stderr,"Get File Num Error:%s/n/a",strerror(errno));
exit(1);
}
/* 给线程分配空间,其实没有必要这么多的 */
if(((thread=(pthread_t *)malloc(sizeof(pthread_t)*num))==NULL)||
((file=(struct copy_file *)malloc(sizeof(struct copy_file)*num))==NULL)
)
{
fprintf(stderr,"Out Of Memory!/n/a");
exit(1);
} 

for(i=0,j=0;i<num;i++)
{
memset(filename,'/0',BUFFER);
strcpy(filename,namelist[i]->d_name);
if(stat(filename,&filestat)==-1)
{
fprintf(stderr,"Get File Information:%s/n/a",strerror(errno));
exit(1);
}
/* 我们忽略目录 */
if(!S_ISREG(filestat.st_mode))continue;
if((file[j].infile=open(filename,O_RDONLY))<0)
{
fprintf(stderr,"Open %s Error:%s/n/a",filename,strerror(errno));
continue;
}
strcat(filename,".bak");
if((file[j].outfile=open(filename,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))
<0)
{
fprintf(stderr,"Creat %s Error:%s/n/a",filename,strerror(errno
));
continue;
}
/* 创建线程,进行文件拷贝 */
if(pthread_create(&thread[j],NULL,copy,(void *)&file[j])!=0)
fprintf(stderr,"Create Thread[%d] Error:%s/n/a",i,strerror(errno));
j++;
} 
byte_copy=0;
for(i=0;i<j;i++)
{
/* 等待线程结束 */
if(pthread_join(thread[i],(void **)&byte_copy_p)!=0)
fprintf(stderr,"Thread[%d] Join Error:%s/n/a",
i,strerror(errno));
else
{
if(bytes_copy_p==NULL)continue;
printf("Thread[%d] Copy %d bytes/n/a",i,*byte_copy_p);
byte_copy+=*byte_copy_p;
/* 释放我们在copy函数里面创建的内存 */ 
free(byte_copy_p); 
}
}
printf("Total Copy Bytes %d/n/a",byte_copy);
free(thread);
free(file);
exit(0);
} 
线程的介绍就到这里

