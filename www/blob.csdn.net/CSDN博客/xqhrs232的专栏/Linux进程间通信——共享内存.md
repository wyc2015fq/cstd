# Linux进程间通信——共享内存 - xqhrs232的专栏 - CSDN博客
2011年01月07日 23:15:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：758标签：[linux																[struct																[存储																[delete																[cmd																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.cnblogs.com/feisky/archive/2010/03/24/1693488.html](http://www.cnblogs.com/feisky/archive/2010/03/24/1693488.html)
共享内存是运行在同一台机器上的进程间通信最快的方式，因为数据不需要在不同的进程间复制。通常由一个进程创建一块共享内存区，其余进程对这块内存区进行读写。共享内存往往与其它通信机制，如信号量结合使用，来达到进程间的同步及互斥。
首先要用的函数是shmget，它获得一个共享存储标识符。 
＃i nclude <sys/types.h> 
        ＃i nclude <sys/ipc.h> 
        ＃i nclude <sys/shm.h> 
        int shmget(key_t key, int size, int flag); 
这个函数有点类似大家熟悉的malloc函数，系统按照请求分配size大小的内存用作共享内存。Linux系统内核中每个IPC结构都有的一个非负整数的标识符，这样对一个消息队列发送消息时只要引用标识符就可以了。这个标识符是内核由IPC结构的关键字得到的，这个关键字，就是上面第一个函数的key。数据类型key_t是在头文件sys/types.h中定义的，它是一个长整形的数据。在我们后面的章节中，还会碰到这个关键字。
当共享内存创建后，其余进程可以调用shmat（）将其连接到自身的地址空间中。 
void *shmat(int shmid, void *addr, int flag); 
shmid为shmget函数返回的共享存储标识符，addr和flag参数决定了以什么方式来确定连接的地址，函数的返回值即是该进程数据段所连接的实际地址，进程可以对此进程进行读写操作。
使用共享存储来实现进程间通信的注意点是对数据存取的同步，必须确保当一个进程去读取数据时，它所想要的数据已经写好了。通常，信号量被要来实现对共享存储数据存取的同步，另外，可以通过使用shmctl函数设置共享存储内存的某些标志位如SHM_LOCK、SHM_UNLOCK等来实现。
共享内存机制使得两个不相关进程可以读取和修改同一段内存从而实现数据共享。主要函数定义: 
#include <sys/shm.h> 
void *shmat(int shm_id, const void *shm_addr, int shmflg); 
int shmctl(int shm_id, int cmd, struct shmid_ds *buf); 
int shmdt(const void *shm_addr); 
int shmget(key_t key, size_t size, int shmflag); 
C代码  
- /*server.c:向共享内存中写入People*/ 
- #include <stdio.h> 
- #include <sys/types.h> 
- #include <sys/ipc.h> 
- #include <sys/sem.h> 
- int main()   
- {   
- struct People{   
- char name[10];   
- int age;   
-     };   
- int semid;   
- int shmid;   
-     key_t semkey;   
-     key_t shmkey;   
-     semkey=ftok("server.c",0);   
-     shmkey=ftok("client.c",0);   
- /*创建共享内存和信号量的IPC*/ 
-     semid=semget(semkey,1,0666|IPC_CREAT);   
- if(semid==-1)   
-     printf("creat sem is fail/n");   
-     shmid=shmget(shmkey,1024,0666|IPC_CREAT);   
- if(shmid==-1)   
-     printf("creat shm is fail/n");   
- /*设置信号量的初始值，就是资源个数*/ 
- union semun{   
- int val;   
- struct semid_ds *buf;   
-         ushort *array;   
-     }sem_u;   
-     sem_u.val=1;   
-     semctl(semid,0,SETVAL,sem_u);   
- /*将共享内存映射到当前进程的地址中，之后直接对进程中的地址addr操作就是对共享内存操作*/ 
- struct People * addr;   
-     addr=(struct People*)shmat(shmid,0,0);   
- if(addr==(struct People*)-1)   
-     printf("shm shmat is fail/n");   
- /*信号量的P操作*/ 
- void p()   
-     {   
- struct sembuf sem_p;   
-         sem_p.sem_num=0;   
-         sem_p.sem_op=-1;   
- if(semop(semid,&sem_p,1)==-1)   
-         printf("p operation is fail/n");           
-     }   
- /*信号量的V操作*/ 
- void v()   
-     {   
- struct sembuf sem_v;   
-         sem_v.sem_num=0;   
-         sem_v.sem_op=1;   
- if(semop(semid,&sem_v,1)==-1)   
-         printf("v operation is fail/n");   
-     }   
- /*向共享内存写入数据*/ 
-     p();   
-     strcpy((*addr).name,"xiaoming");   
- /*注意：①此处只能给指针指向的地址直接赋值，不能在定义一个  struct People people_1;addr=&people_1;因为addr在addr=(struct People*)shmat(shmid,0,0);时,已经由系统自动分配了一个地址，这个地址与共享内存相关联，所以不能改变这个指针的指向，否则他将不指向共享内存，无法完成通信了。
- 注意：②给字符数组赋值的方法。刚才太虎了。。*/ 
-     (*addr).age=10;   
-     v();   
- /*将共享内存与当前进程断开*/ 
- if(shmdt(addr)==-1)   
-     printf("shmdt is fail/n");     
- }  
C代码  
- /*client.c:从共享内存中读出People*/ 
- #include <stdio.h> 
- #include <sys/types.h> 
- #include <sys/ipc.h> 
- #include <sys/sem.h> 
- int main()   
- {   
- int semid;   
- int shmid;   
-     key_t semkey;   
-     key_t shmkey;   
-     semkey=ftok("server.c",0);   
-     shmkey=ftok("client.c",0);   
- struct People{   
- char name[10];   
- int age;   
-     };   
- /*读取共享内存和信号量的IPC*/ 
-     semid=semget(semkey,0,0666);   
- if(semid==-1)   
-     printf("creat sem is fail/n");   
-     shmid=shmget(shmkey,0,0666);   
- if(shmid==-1)   
-     printf("creat shm is fail/n");   
- /*将共享内存映射到当前进程的地址中，之后直接对进程中的地址addr操作就是对共享内存操作*/ 
- struct People * addr;   
-     addr=(struct People*)shmat(shmid,0,0);   
- if(addr==(struct People*)-1)   
-     printf("shm shmat is fail/n");   
- /*信号量的P操作*/ 
- void p()   
-     {   
- struct sembuf sem_p;   
-         sem_p.sem_num=0;   
-         sem_p.sem_op=-1;   
- if(semop(semid,&sem_p,1)==-1)   
-         printf("p operation is fail/n");           
-     }   
- /*信号量的V操作*/ 
- void v()   
-     {   
- struct sembuf sem_v;   
-         sem_v.sem_num=0;   
-         sem_v.sem_op=1;   
- if(semop(semid,&sem_v,1)==-1)   
-         printf("v operation is fail/n");   
-     }   
- /*从共享内存读出数据*/ 
-     p();   
-     printf("name:%s/n",addr->name);   
-     printf("age:%d/n",addr->age);   
-     v();   
- /*将共享内存与当前进程断开*/ 
- if(shmdt(addr)==-1)   
-     printf("shmdt is fail/n");   
- /*IPC必须显示删除。否则会一直留存在系统中*/ 
- if(semctl(semid,0,IPC_RMID,0)==-1)   
-     printf("semctl delete error/n");   
- if(shmctl(shmid,IPC_RMID,NULL)==-1)   
-     printf("shmctl delete error/n");   
- }  

