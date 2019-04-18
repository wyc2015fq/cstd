# Posix多线程编程学习笔记（六）—共享内存（2） - ljx0305的专栏 - CSDN博客
2008年08月01日 13:51:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1041
**三、posix****共享内存函数<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />**
posix共享内存区涉及两个步骤：
1、指定一个名字参数调用shm_open,以创建一个新的共享内存区对象或打开一个以存在的共享内存区对象。
2、调用mmap把这个共享内存区映射到调用进程的地址空间。传递给shm_open的名字参数随后由希望共享该内存区的任何其他进程使用。
5．
|名称:：|shm_open|
|----|----|
|功能：|打开或创建一个共享内存区|
|头文件：|#include <sys/mman.h>|
|函数原形：|int shm_open(const char *name,int oflag,mode_t mode);|
|参数：|name共享内存区的名字cflag标志位mode权限位|
|返回值：|成功返回0，出错返回-1|

oflag参数必须含有O_RDONLY和O_RDWR标志，还可以指定如下标志：O_CREAT,O_EXCL或O_TRUNC.
mode参数指定权限位，它指定O_CREAT标志的前提下使用。
shm_open的返回值是一个整数描述字，它随后用作mmap的第五个参数。
6．
|名称:：|shm_unlink|
|----|----|
|功能：|删除一个共享内存区|
|头文件：|#include <sys/mman.h>|
|函数原形：|int shm_unlink(const char *name);|
|参数：|name共享内存区的名字|
|返回值：|成功返回0，出错返回-1|
shm_unlink函数删除一个共享内存区对象的名字，删除一个名字仅仅防止后续的open,mq_open或sem_open调用取得成功。
下面是创建一个共享内存区的例子：
/*shm_open.c创建共享内存区*/
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
int main(int argc,char **argv)
{
int shm_id;
if(argc!=2)
{
printf(“usage:shm_open <pathname>/n”);
exit(1);
}
shm_id=shm_open(argv[1],O_RDWR|O_CREAT,0644);
printf(“shmid:%d/n”,shm_id);
shm_unlink(argv[1]);
}
下面是运行结果，注意编译程序我们要加上“-lrt”参数。
#cc –lrt –o shm_open shm_open.c
#./shm_open test
shm_id:3
**四、ftruncate****和fstat****函数**
普通文件或共享内存区对象的大小都可以通过调用ftruncate修改。
7．
|名称:：|ftruncate|
|----|----|
|功能：|调整文件或共享内存区大小|
|头文件：|#include <unistd.h>|
|函数原形：|int ftruncate(int fd,off_t length);|
|参数：|fd描述符length大小|
|返回值：|成功返回0，出错返回-1|

当打开一个已存在的共享内存区对象时，我们可调用fstat来获取有关该对象的信息。
8．
|名称:：|fstat|
|----|----|
|功能：|获得文件或共享内存区的信息|
|头文件：|#include <unistd.h>#include <sys/types.h>#include <sys/stat.h>|
|函数原形：|int stat(const char *file_name,struct stat *buf);|
|参数：|file_name文件名bufstat结构|
|返回值：|成功返回0，出错返回-1|

对于普通文件stat结构可以获得12个以上的成员信息，然而当fd指代一个共享内存区对象时，只有四个成员含有信息。
struct stat{
mode_t st_mode;
uid_t st_uid;
gid_t st_gid;
off_t st_size;
};
/*shm_show.c显示共享区信息*/
#include <unistd.h>
#include <sys/type.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
int main(int argc,char **argv)
{
int shm_id;
struct stat buf;
if(argc!=2)
{
printf(“usage:shm_open <pathname>/n”);
exit(1);
}
shm_id=shm_open(argv[1],O_RDWR|O_CREAT,0644);/*创建共享内存*/
ftruncate(shm_id,100);/*修改共享内存的打开*/
fstat(shm_id,&buf); /*把共享内存的信息记录到buf中*/
printf(“uid_t:%d/n”,buf.st_uid); /*共享内存区所有者ID*/
printf(“git_t:%d/n”,buf.st_gid); /*共享内存区所有者组ID*/
printf(“size :%d/n”,buf.st_size); /*共享内存区大小*/
}
下面是运行结果：
#cc –lrt –o shm_show shm_show.c
#./shm_show test
uid_t:0
git_t:0
size:100
引用： [http://blog.chinaunix.net/u/22935/showart_341893.html](http://blog.chinaunix.net/u/22935/showart_341893.html)
