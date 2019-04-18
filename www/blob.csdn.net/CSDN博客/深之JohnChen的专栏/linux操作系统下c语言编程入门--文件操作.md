# linux操作系统下c语言编程入门--文件操作 - 深之JohnChen的专栏 - CSDN博客

2007年02月28日 18:00:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3031


 linux操作系统下c语言编程入门--文件操作

前言:
我们在这一节将要讨论linux下文件操作的各个函数.
文件的创建和读写
文件的各个属性
目录文件的操作
管道文件
----------------------------------------------------------------------------
----
1。文件的创建和读写
我假设你已经知道了标准级的文件操作的各个函数(fopen,fread,fwrite等等).当然
如果你不清楚的话也不要着急.我们讨论的系统级的文件操作实际上是为标准级文件操作
服务的.
当我们需要打开一个文件进行读写操作的时候,我们可以使用系统调用函数open.使用完
成以后我们调用另外一个close函数进行关闭操作.
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int open(const char *pathname,int flags);
int open(const char *pathname,int flags,mode_t mode);
int close(int fd);
open函数有两个形式.其中pathname是我们要打开的文件名(包含路径名称,缺省是认为在
当前路径下面).flags可以去下面的一个值或者是几个值的组合.
O_RDONLY:以只读的方式打开文件.
O_WRONLY:以只写的方式打开文件.
O_RDWR:以读写的方式打开文件.
O_APPEND:以追加的方式打开文件.
O_CREAT:创建一个文件.
O_EXEC:如果使用了O_CREAT而且文件已经存在,就会发生一个错误.
O_NOBLOCK:以非阻塞的方式打开一个文件.
O_TRUNC:如果文件已经存在,则删除文件的内容.
前面三个标志只能使用任意的一个.如果使用了O_CREATE标志,那么我们要使用open的第
二种形式.还要指定mode标志,用来表示文件的访问权限.mode可以是以下情况的组合.
-----------------------------------------------------------------
S_IRUSR 用户可以读 S_IWUSR 用户可以写
S_IXUSR 用户可以执行 S_IRWXU 用户可以读写执行
-----------------------------------------------------------------
S_IRGRP 组可以读 S_IWGRP 组可以写
S_IXGRP 组可以执行 S_IRWXG 组可以读写执行
-----------------------------------------------------------------
S_IROTH 其他人可以读 S_IWOTH 其他人可以写
S_IXOTH 其他人可以执行 S_IRWXO 其他人可以读写执行
-----------------------------------------------------------------
S_ISUID 设置用户执行ID S_ISGID 设置组的执行ID
-----------------------------------------------------------------
我们也可以用数字来代表各个位的标志.Linux总共用5个数字来表示文件的各种权限.
00000.第一位表示设置用户ID.第二位表示设置组ID,第三位表示用户自己的权限位,第四
位表示组的权限,最后一位表示其他人的权限.
每个数字可以取1(执行权限),2(写权限),4(读权限),0(什么也没有)或者是这几个值的和
.. 
比如我们要创建一个用户读写执行,组没有权限,其他人读执行的文件.设置用户ID位那么
我们可以使用的模式是--1(设置用户ID)0(组没有设置)7(1+2+4)0(没有权限,使用缺省)
5(1+4)即10705:
open("temp",O_CREAT,10705);
如果我们打开文件成功,open会返回一个文件描述符.我们以后对文件的所有操作就可以
对这个文件描述符进行操作了.
当我们操作完成以后,我们要关闭文件了,只要调用close就可以了,其中fd是我们要关闭
的文件描述符.
文件打开了以后,我们就要对文件进行读写了.我们可以调用函数read和write进行文件的读写.
#include <unistd.h>
ssize_t read(int fd, void *buffer,size_t count);
ssize_t write(int fd, const void *buffer,size_t count);
fd是我们要进行读写操作的文件描述符,buffer是我们要写入文件内容或读出文件内容的
内存地址.count是我们要读写的字节数.
对于普通的文件read从指定的文件(fd)中读取count字节到buffer缓冲区中(记住我们必
须提供一个足够大的缓冲区),同时返回count.
如果read读到了文件的结尾或者被一个信号所中断,返回值会小于count.如果是由信号中
断引起返回,而且没有返回数据,read会返回-1,且设置errno为EINTR.当程序读到了文件
结尾的时候,read会返回0.
write从buffer中写count字节到文件fd中,成功时返回实际所写的字节数.
下面我们学习一个实例,这个实例用来拷贝文件.
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#define BUFFER_SIZE 1024
int main(int argc,char **argv)
{
int from_fd,to_fd;
int bytes_read,bytes_write;
char buffer[BUFFER_SIZE];
char *ptr;
if(argc!=3)
{
fprintf(stderr,"Usage:%s fromfile tofile/n/a",argv[0]);
exit(1);
}
/* 打开源文件 */
if((from_fd=open(argv[1],O_RDONLY))==-1)
{
fprintf(stderr,"Open %s Error:%s/n",argv[1],strerror(errno));
exit(1);
}
/* 创建目的文件 */
if((to_fd=open(argv[2],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1)
{
fprintf(stderr,"Open %s Error:%s/n",argv[2],strerror(errno));
exit(1);
}
/* 以下代码是一个经典的拷贝文件的代码 */
while(bytes_read=read(from_fd,buffer,BUFFER_SIZE))
{
/* 一个致命的错误发生了 */
if((bytes_read==-1)&&(errno!=EINTR)) break;
else if(bytes_read>0)
{
ptr=buffer;
while(bytes_write=write(to_fd,ptr,bytes_read))
{
/* 一个致命错误发生了 */
if((bytes_write==-1)&&(errno!=EINTR))break;
/* 写完了所有读的字节 */
else if(bytes_write==bytes_read) break;
/* 只写了一部分,继续写 */
else if(bytes_write>0)
{
ptr+=bytes_write;
bytes_read-=bytes_write;
}
}
/* 写的时候发生的致命错误 */
if(bytes_write==-1)break;
}
}
close(from_fd);
close(to_fd);
exit(0);
}
2。文件的各个属性
文件具有各种各样的属性,除了我们上面所知道的文件权限以外,文件还有创建时间
,大小等等属性.
有时侯我们要判断文件是否可以进行某种操作(读,写等等).这个时候我们可以使用acce
ss函数.
#include <unistd.h>

int access(const char *pathname,int mode);
pathname:是文件名称,mode是我们要判断的属性.可以取以下值或者是他们的组合.
R_OK文件可以读,W_OK文件可以写,X_OK文件可以执行,F_OK文件存在.当我们测试成功时
,函数返回0,否则如果有一个条件不符时,返回-1.
如果我们要获得文件的其他属性,我们可以使用函数stat或者fstat.
#include <sys/stat.h>
#include <unistd.h>
int stat(const char *file_name,struct stat *buf);
int fstat(int filedes,struct stat *buf);
struct stat {
dev_t st_dev; /* 设备 */
ino_t st_ino; /* 节点 */ 
mode_t st_mode; /* 模式 */
nlink_t st_nlink; /* 硬连接 */
uid_t st_uid; /* 用户ID */
gid_t st_gid; /* 组ID */
dev_t st_rdev; /* 设备类型 */
off_t st_off; /* 文件字节数 */
unsigned long st_blksize; /* 块大小 */
unsigned long st_blocks; /* 块数 */
time_t st_atime; /* 最后一次访问时间 */
time_t st_mtime; /* 最后一次修改时间 */
time_t st_ctime; /* 最后一次改变时间(指属性) */
};
stat用来判断没有打开的文件,而fstat用来判断打开的文件.我们使用最多的属性是st_
mode.通过着属性我们可以判断给定的文件是一个普通文件还是一个目录,连接等等.可以
使用下面几个宏来判断.
S_ISLNK(st_mode):是否是一个连接.S_ISREG是否是一个常规文件.S_ISDIR是否是一个目
录S_ISCHR是否是一个字符设备.S_ISBLK是否是一个块设备S_ISFIFO是否 是一个FIFO文
件.S_ISSOCK是否是一个SOCKET文件. 我们会在下面说明如何使用这几个宏的.
3。目录文件的操作
在我们编写程序的时候，有时候会要得到我们当前的工作路径。C库函数提供了get
cwd来解决这个问题。
#include <unistd.h>

char *getcwd(char *buffer,size_t size);
我们提供一个size大小的buffer,getcwd会把我们当前的路径考到buffer中.如果buffer
太小,函数会返回-1和一个错误号.
[Linux](http://os.rdxx.com/Linux/)提供了大量的目录操作函数,我们学习几个比较简单和常用的函数.
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int mkdir(const char *path,mode_t mode);
DIR *opendir(const char *path);
struct dirent *readdir(DIR *dir);
void rewinddir(DIR *dir);
off_t telldir(DIR *dir);
void seekdir(DIR *dir,off_t off);
int closedir(DIR *dir);
struct dirent {
long d_ino;
off_t d_off;
unsigned short d_reclen;
char d_name[NAME_MAX+1]; /* 文件名称 */
mkdir很容易就是我们创建一个目录,opendir打开一个目录为以后读做准备.readdir读一
个打开的目录.rewinddir是用来重读目录的和我们学的rewind函数一样.closedir是关闭
一个目录.telldir和seekdir类似与ftee和fseek函数.
下面我们开发一个小程序,这个程序有一个参数.如果这个参数是一个文件名,我们输出这
个文件的大小和最后修改的时间,如果是一个目录我们输出这个目录下所有文件的大小和
修改时间.
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
static int get_file_size_time(const char *filename)
{
struct stat statbuf;
if(stat(filename,&statbuf)==-1)
{
printf("Get stat on %s Error:%s/n",
filename,strerror(errno));
return(-1);
}
if(S_ISDIR(statbuf.st_mode))return(1);
if(S_ISREG(statbuf.st_mode))
printf("%s size:%ld bytes/tmodified at %s",
filename,statbuf.st_size,ctime(&statbuf.st_mtime)); 

return(0);
}
int main(int argc,char **argv)
{
DIR *dirp;
struct dirent *direntp;
int stats;
if(argc!=2)
{
printf("Usage:%s filename/n/a",argv[0]);
exit(1);
}
if(((stats=get_file_size_time(argv[1]))==0)||(stats==-1))exit(1);
if((dirp=opendir(argv[1]))==NULL)
{
printf("Open Directory %s Error:%s/n",
argv[1],strerror(errno));
exit(1);
}
while((direntp=readdir(dirp))!=NULL)
if(get_file_size_time(direntp-<d_name)==-1)break;
closedir(dirp);
exit(1);
}
4。管道文件
[Linux](http://os.rdxx.com/Linux/)提供了许多的过滤和重定向程序,比如more cat
等等.还提供了< > | <<等等重定向操作符.在这些过滤和重 定向程序当中,都用到了管
道这种特殊的文件.系统调用pipe可以创建一个管道.
#include<unistd.h>

int pipe(int fildes[2]);
pipe调用可以创建一个管道(通信缓冲区).当调用成功时,我们可以访问文件描述符fild
es[0],fildes[1].其中fildes[0]是用来读的文件描述符,而fildes[1]是用来写的文件描
述符.
在实际使用中我们是通过创建一个子进程,然后一个进程写,一个进程读来使用的.
关于进程通信的详细情况请查看进程通信
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER 255
int main(int argc,char **argv)
{
char buffer[BUFFER+1];
int fd[2];
if(argc!=2)
{
fprintf(stderr,"Usage:%s string/n/a",argv[0]);
exit(1);
}
if(pipe(fd)!=0)
{
fprintf(stderr,"Pipe Error:%s/n/a",strerror(errno));
exit(1);
}
if(fork()==0)
{
close(fd[0]);
printf("Child[%d] Write to pipe/n/a",getpid());
snprintf(buffer,BUFFER,"%s",argv[1]);
write(fd[1],buffer,strlen(buffer));
printf("Child[%d] Quit/n/a",getpid());
exit(0);
}
else
{
close(fd[1]);
printf("Parent[%d] Read from pipe/n/a",getpid());
memset(buffer,'/0',BUFFER+1);
read(fd[0],buffer,BUFFER);
printf("Parent[%d] Read:%s/n",getpid(),buffer);
exit(1);
}
}
为了实现重定向操作,我们需要调用另外一个函数dup2.
#include <unistd.h>

int dup2(int oldfd,int newfd);
dup2将用oldfd文件描述符来代替newfd文件描述符,同时关闭newfd文件描述符.也就是说
,
所有向newfd操作都转到oldfd上面.下面我们学习一个例子,这个例子将标准输出重定向
到一个文件.
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#define BUFFER_SIZE 1024
int main(int argc,char **argv)
{
int fd;
char buffer[BUFFER_SIZE];
if(argc!=2)
{
fprintf(stderr,"Usage:%s outfilename/n/a",argv[0]);
exit(1);
}
if((fd=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR))==-1)
{
fprintf(stderr,"Open %s Error:%s/n/a",argv[1],strerror(errno));
exit(1);
}
if(dup2(fd,STDOUT_FILENO)==-1)
{
fprintf(stderr,"Redirect Standard Out Error:%s/n/a",strerror(errno));
exit(1);
}
fprintf(stderr,"Now,please input string");
fprintf(stderr,"(To quit use CTRL+D)/n");
while(1)
{
fgets(buffer,BUFFER_SIZE,stdin);
if(feof(stdin))break;
write(STDOUT_FILENO,buffer,strlen(buffer));
}
exit(0);
}
好了,文件一章我们就暂时先讨论到这里,学习好了文件的操作我们其实已经可以写出一
些比较有用的程序了.我们可以编写一个实现例如dir,mkdir,cp,mv等等常用的文件操作
命令了.

