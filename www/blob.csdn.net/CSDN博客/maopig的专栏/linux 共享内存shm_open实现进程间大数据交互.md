# linux 共享内存shm_open实现进程间大数据交互 - maopig的专栏 - CSDN博客
2013年11月24日 16:17:16[maopig](https://me.csdn.net/maopig)阅读数：11339

read.c 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
/*
int shm_open(const char *name, int oflag, mode_t mode);
//创建或打开一个共享内存,成功返回一个整数的文件描述符，错误返回-1。
1.name:共享内存区的名字；
2.标志位；open的标志一样
3.权限位
int shm_unlink(const char *name);
编译时要加库文件-lrt
*/
#define SHMNAME "shm_ram"
#define OPEN_FLAG O_RDWR|O_CREAT
#define OPEN_MODE 00777
#define FILE_SIZE 4096*4
int main(void)
{
    int ret = -1;
    int fd = -1;
    char buf[4096] = {0};
    void* add_r = NULL;
    //创建或者打开一个共享内存
    fd = shm_open(SHMNAME, OPEN_FLAG, OPEN_MODE);
    if(-1 == (ret = fd))
    {
        perror("shm  failed: ");
        goto _OUT;
    }
        //调整确定文件共享内存的空间
    ret = ftruncate(fd, FILE_SIZE);
    if(-1 == ret)
    {
        perror("ftruncate faile: ");
        goto _OUT;
    }
    //映射目标文件的存储区
   add_r = mmap(NULL, FILE_SIZE, PROT_READ, MAP_SHARED, fd, SEEK_SET);
    if(NULL == add_r)
    {
        perror("mmap add_r failed: ");
        goto _OUT;
    }
    //memcpy 内存共享 写入内容
memcpy(buf, add_r, sizeof(buf));
    printf("buf = %s\n", buf);
    //取消映射
    ret = munmap(add_r, FILE_SIZE);
    if(-1 == ret)
    {
        perror("munmap add_r faile: ");
        goto _OUT;
    }
    //删除内存共享
  shm_unlink(SHMNAME);
    if(-1 == ret)
    {
        perror("shm_unlink faile: ");
        goto _OUT;
    }
_OUT:    
    return ret;
}
write.c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
/*
int shm_open(const char *name, int oflag, mode_t mode);
//创建或打开一个共享内存,成功返回一个整数的文件描述符，错误返回-1。
1.name:共享内存区的名字；
2.标志位；open的标志一样
3.权限位
int shm_unlink(const char *name);
编译时要加库文件-lrt
*/
#define SHMNAME "shm_ram"
#define OPEN_FLAG O_RDWR|O_CREAT
#define OPEN_MODE 00777
#define FILE_SIZE 4096*4
int main(void)
{
    int ret = -1;
    int fd = -1;
    void* add_w = NULL;
    //创建或者打开一个共享内存
    fd = shm_open(SHMNAME, OPEN_FLAG, OPEN_MODE);
    if(-1 == (ret = fd))
    {
        perror("shm  failed: ");
        goto _OUT;
    }
    //调整确定文件共享内存的空间
    ret = ftruncate(fd, FILE_SIZE);
    if(-1 == ret)
    {
        perror("ftruncate faile: ");
        goto _OUT;
    }
    //映射目标文件的存储区
add_w = mmap(NULL, FILE_SIZE, PROT_WRITE, MAP_SHARED, fd, SEEK_SET);
    if(NULL == add_w)
    {
        perror("mmap src failed: ");
        goto _OUT;
    }
    //memcpy 内存共享 写入内容
   memcpy(add_w, "howaylee", sizeof("howaylee"));
    //取消映射
    ret = munmap(add_w, FILE_SIZE);
    if(-1 == ret)
    {
        perror("munmap add_w faile: ");
        goto _OUT;
    }
    //删除内存共享
    /*shm_unlink(SHMNAME);
    if(-1 == ret)
    {
        perror("shm_unlink faile: ");
        goto _OUT;
    }*/
_OUT:    
    return ret;
}
```
1 /*shm_write.c写入/读出共享内存区*/
 2 #include <stdio.h>
 3 #include <string.h>
 4 #include <stdlib.h>
 5 #include <sys/stat.h>
 6 #include <fcntl.h>
 7 #include <sys/mman.h>
 8 #include <unistd.h>
 9 
10 int main(int argc,char **argv)
11 {
12     int shm_id;
13     struct stat buf;
14     char *ptr;
15 
16     if(argc!=2)
17     {
18         printf("usage:shm_open <pathname>\n");
19         exit(1);
20     }
21     shm_id=shm_open(argv[1],O_RDWR|O_CREAT,0644);/*创建共享内存区*/
22     ftruncate(shm_id,100);/*修改共享区大小*/
23     fstat(shm_id,&buf);
24     ptr=mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/
25     strcpy(ptr,"hello linux");/*写入共享内存区*/
26     printf("%s\n",ptr);/*读出共享内存区*/
27     shm_unlink(argv[1]);/*删除共享内存区*/
28 }
[](http://www.cnblogs.com/polestar/archive/2012/04/23/2466022.html)
```
编译运行：
```
1 root@linux:/mnt/hgfs/C_libary# gcc **-lrt** -o shm_write shm_write.c
2 root@linux:/mnt/hgfs/C_libary# ./shm_write test
3 hello linux
4 root@linux:/mnt/hgfs/C_libary#
```
```
1 ftruncate()函数 3 功能：    调整文件或共享内存区大小
 4 头文件：    #include <unistd.h>
 5 函数原形：    int ftruncate(int fd,off_t length);
 6 参数：    
 7     fd          描述符
 8     length       大小
 9 返回值：    成功返回0，出错返回-1
10 
11 当打开一个已存在的共享内存区对象时，我们可调用fstat来获取有关该对象的信息
12 
13 fstat()函数
14 功能：    获得文件或共享内存区的信息
15 头文件：    #include <unistd.h>
16 #include <sys/types.h>
17 #include <sys/stat.h>
18 函数原形：    int stat(const char *file_name,struct stat *buf);
19 参数：    
20 file_name          文件名
21 buf               stat结构
22 返回值：    成功返回0，出错返回-1
23 
24 对于普通文件stat结构可以获得12个以上的成员信息，然而当fd指代一个共享内存区对象时，只有四个成员含有信息。
25 struct stat{
26     mode_t st_mode;
27     uid_t st_uid;
28     gid_t st_gid;
29     off_t st_size;
30 };
[](http://www.cnblogs.com/polestar/archive/2012/04/23/2466022.html)
```
示例代码：
```
[](http://www.cnblogs.com/polestar/archive/2012/04/23/2466022.html)
 1 /*shm_show.c显示共享区信息*/
 2 #include <stdio.h>
 3 #include <stdlib.h>
 4 #include <unistd.h>
 5 #include <sys/stat.h>
 6 #include <sys/types.h>
 7 #include <fcntl.h>
 8 #include <sys/mman.h>
 9 
10 int main(int argc,char **argv)
11 {
12     int shm_id;
13         struct stat buf;
14 
15         if(argc!=2)
16         {
17             printf("usage:shm_open <pathname>\n");
18             exit(1);
19         }
20         shm_id=shm_open(argv[1],O_RDWR|O_CREAT,0644);/*创建共享内存*/
21         printf("size :%d\n",buf.st_size); /*修改前共享内存区大小*/
22         ftruncate(shm_id,100);/*修改共享内存的大小*/
23         fstat(shm_id,&buf); /*把共享内存的信息记录到buf中*/
24         printf("uid_t:%d\n",buf.st_uid); /*共享内存区所有者ID*/
25         printf("git_t:%d\n",buf.st_gid); /*共享内存区所有者组ID*/
26         printf("size :%d\n",buf.st_size); /*修改后共享内存区大小*/
27 }
```
客户服务段两进程通信实例：
服务端
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
int main(int argc,char **argv)
{
    int shm_id;
    char *ptr;
    sem_t *sem;
    if (argc!=2)
    {
        printf("usage:shm_open<pathname>\n");
        return -1;
    }
    shm_id=shm_open(argv[1],O_RDWR|O_CREAT,0644);/*第一步：创建共享内存区*/
    if (shm_id==-1)
    {
        printf( "open shared memory error.errno=%d,desc=%s.\n", errno, strerror(errno));
        return -1;
    }
    ftruncate(shm_id,100);/*第二步：调整共享内存区大小，shmid问shm_open的返回值*/
    sem=sem_open(argv[1],O_CREAT,0644,0);/*创建信号量*/
    if (sem==SEM_FAILED)
    {
        printf( "open semaphore error.errno=%d,desc=%s.\n", errno, strerror(errno));
        return -1;
    }
    ptr=mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,**shm_id**,0);/***第三**步：连接共享内存区*/
    strcpy(ptr,"\0");
    sem_wait(sem);/***第四步**：申请信号量*/
    printf("server : %s\n",ptr);/*输入共享内存区内容*/
    strcpy(ptr,"\0");/*清空共享内存区*/
    sem_unlink(argv[1]);/***第五步**：删除信号量*/
    shm_unlink(argv[1]);/***第六步**：删除共享内存区*/
    return 0;
}
客户端：
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
int main(int argc,char **argv)
{
    int shm_id;
    char *ptr;
    sem_t *sem;
    if (argc!=2)
    {
        printf("usage:shm_open <pathname>\n");
        return -1;
    }
    shm_id=shm_open(argv[1],O_RDWR,0);/***第一**步：打开共享内存区*/
    if (shm_id==-1)
    {
        printf( "open shared memory error.errno=%d,desc=%s.\n", errno, strerror(errno));
        return -1;
    }
    else
    {
        printf( "open shared memory ok.\n");
    }
    sem=sem_open(argv[1],0);/*打开信号量*/
    if (sem==SEM_FAILED)
    {
        printf( "open semaphore error.errno=%d,desc=%s.\n", errno, strerror(errno));
        return -1;
    }
    else
    {
        printf( "open semaphore ok.\n");
    }
    ptr=mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/
    fgets(ptr,10,stdin);/*从键盘读入数据到共享内存区*/
    printf("user : %s",ptr);
sem_post(sem);/*释放信号量*/
    return 0;
}
