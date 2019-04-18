# linux操作系统下c语言编程入门--消息管理  - 深之JohnChen的专栏 - CSDN博客

2007年02月28日 18:13:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1728


linux操作系统下c语言编程入门--消息管理 

 前言:Linux下的进程通信(IPC)
Linux下的进程通信(IPC)
POSIX无名信号量
System V信号量
System V消息队列
System V共享内存
1。POSIX无名信号量 如果你学习过[操作系统](http://os.rdxx.com/),那么肯定熟悉PV操作了.PV操作是原子
操作.也就是操作是不可以中断的,在一定的时间内,只能够有一个进程的代码在CPU上面
执行.在系统当中,有时候为了顺利的使用和保护共享资源,大家提出了信号的概念. 假设
我们要使用一台打印机,如果在同一时刻有两个进程在向打印机输出,那么最终的结果会
是什么呢.为了处理这种情况,POSIX标准提出了有名信号量和无名信号量的概念,由于Li
nux只实现了无名信号量,我们在这里就只是介绍无名信号量了. 信号量的使用主要是用
来保护共享资源,使的资源在一个时刻只有一个进程所拥有.为此我们可以使用一个信号灯.当信号灯的值为某个值的时候,就表明此时资源不可以使用.否则就表>示可以使用. 
为了提供效率,系统提供了下面几个函数
POSIX的无名信号量的函数有以下几个:
#include <semaphore.h>
int sem_init(sem_t *sem,int pshared,unsigned int value);
int sem_destroy(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_getvalue(sem_t *sem);
sem_init创建一个信号灯,并初始化其值为value.pshared决定了信号量能否在几个进程
间共享.由于目前[Linux](http://os.rdxx.com/Linux/)还没有实现进程间共享信号灯,所以这个值只能够取0. sem_dest
roy是用来删除信号灯的.sem_wait调用将阻塞进程,直到信号灯的值大于0.这个函数返回
的时候自动的将信号灯的值的件一.sem_post和sem_wait相反,是将信号灯的内容加一同
时发出信号唤醒等待的进程..sem_trywait和sem_wait相同,不过不阻塞的,当信号灯的值
为0的时候返回EAGAIN,表示以后重试.sem_getvalue得到信号灯的值.
由于Linux不支持,我们没有办法用源程序解释了.
这几个函数的使用相当简单的.比如我们有一个程序要向一个系统打印机打印两页.我们
首先创建一个信号灯,并使其初始值为1,表示我们有一个资源可用.然后一个进程调用se
m_wait由于这个时候信号灯的值为1,所以这个函数返回,打印机开始打印了,同时信号灯
的值为0 了. 如果第二个进程要打印,调用sem_wait时候,由于信号灯的值为0,资源不可
用,于是被阻塞了.当第一个进程打印完成以后,调用sem_post信号灯的值为1了,这个时候
系统通知第二个进程,于是第二个进程的sem_wait返回.第二个进程开始打印了.
不过我们可以使用线程来解决这个问题的.我们会在后面解释什么是线程的.编译包含上
面这几个函数的程序要加上 -lrt选贤,以连接librt.so库
2。System V信号量 为了解决上面哪个问题,我们也可以使用System V信号量.很幸运的
是Linux实现了System V信号量.这样我们就可以用实例来解释了. System V信号量的函
数主要有下面几个.
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
key_t ftok(char *pathname,char proj);
int semget(key_t key,int nsems,int semflg);
int semctl(int semid,int semnum,int cmd,union semun arg);
int semop(int semid,struct sembuf *spos,int nspos);
struct sembuf {
short sem_num; /* 使用那一个信号 */
short sem_op; /* 进行什么操作 */
short sem_flg; /* 操作的标志 */
};
ftok函数是根据pathname和proj来创建一个关键字.semget创建一个信号量.成功时返回
信号的ID,key是一个关键字,可以是用ftok创建的也可以是IPC_PRIVATE表明由系统选用
一个关键字. nsems表明我们创建的信号个数.semflg是创建的权限标志,和我们创建一个
文件的标志相同.
semctl对信号量进行一系列的控制.semid是要操作的信号标志,semnum是信号的个数,cm
d是操作的命令.经常用的两个值是:SETVAL(设置信号量的值)和IPC_RMID(删除信号灯).
arg是一个给cmd的参数.
semop是对信号进行操作的函数.semid是信号标志,spos是一个操作数组表明要进行什么
操作,nspos表明数组的个数. 如果sem_op大于0,那么操作将sem_op加入到信号量的值中
,并唤醒等待信号增加的进程. 如果为0,当信号量的值是0的时候,函数返回,否则阻塞直
到信号量的值为0. 如果小于0,函数判断信号量的值加上这个负值.如果结果为0唤醒等待信号量为0的进程,如果小与0函数阻塞.如果大于0,那么从信号量里面减去这个值并返回
.. 
下面我们一以一个实例来说明这几个函数的使用方法.这个程序用标准错误输出来代替我
们用的打印机.
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define PERMS S_IRUSR|S_IWUSR
void init_semaphore_struct(struct sembuf *sem,int semnum,
int semop,int semflg)
{
/* 初始话信号灯结构 */
sem->sem_num=semnum;
sem->sem_op=semop;
sem->sem_flg=semflg;
}
int del_semaphore(int semid)
{
/* 信号灯并不随程序的结束而被删除,如果我们没删除的话(将1改为0)
可以用ipcs命令查看到信号灯,用ipcrm可以删除信号灯的
*/
#if 1
return semctl(semid,0,IPC_RMID);
#endif
}
int main(int argc,char **argv)
{
char buffer[MAX_CANON],*c;
int i,n;
int semid,semop_ret,status;
pid_t childpid;
struct sembuf semwait,semsignal;
if((argc!=2)||((n=atoi(argv[1]))<1))
{
fprintf(stderr,"Usage:%s number/n/a",argv[0]);
exit(1);
}
/* 使用IPC_PRIVATE 表示由系统选择一个关键字来创建 */
/* 创建以后信号灯的初始值为0 */
if((semid=semget(IPC_PRIVATE,1,PERMS))==-1)
{
fprintf(stderr,"[%d]:Acess Semaphore Error:%s/n/a",
getpid(),strerror(errno));
exit(1);
}
/* semwait是要求资源的操作(-1) */
init_semaphore_struct(&semwait,0,-1,0);
/* semsignal是释放资源的操作(+1) */
init_semaphore_struct(&semsignal,0,1,0);
/* 开始的时候有一个系统资源(一个标准错误输出) */
if(semop(semid,&semsignal,1)==-1)
{
fprintf(stderr,"[%d]:Increment Semaphore Error:%s/n/a",
getpid(),strerror(errno));
if(del_semaphore(semid)==-1)
fprintf(stderr,"[%d]:Destroy Semaphore Error:%s/n/a",
getpid(),strerror(errno));
exit(1);
}
/* 创建一个进程链 */
for(i=0;i<n;i++)
if(childpid=fork()) break;
sprintf(buffer,"[i=%d]-->[Process=%d]-->[Parent=%d]-->[Child=%d]/n",
i,getpid(),getppid(),childpid);
c=buffer;
/* 这里要求资源,进入原子操作 */
while(((semop_ret=semop(semid,&semwait,1))==-1)&&(errno==EINTR));
if(semop_ret==-1)
{
fprintf(stderr,"[%d]:Decrement Semaphore Error:%s/n/a",
getpid(),strerror(errno));
}
else
{
while(*c!='/0')fputc(*c++,stderr);
/* 原子操作完成,赶快释放资源 */
while(((semop_ret=semop(semid,&semsignal,1))==-1)&&(errno==EINTR));
if(semop_ret==-1)
fprintf(stderr,"[%d]:Increment Semaphore Error:%s/n/a",
getpid(),strerror(errno));
}
/* 不能够在其他进程反问信号灯的时候,我们删除了信号灯 */
while((wait(&status)==-1)&&(errno==EINTR));
/* 信号灯只能够被删除一次的 */
if(i==1)
if(del_semaphore(semid)==-1)
fprintf(stderr,"[%d]:Destroy Semaphore Error:%s/n/a",
getpid(),strerror(errno));
exit(0);
} 
信号灯的主要用途是保护临界资源(在一个时刻只被一个进程所拥有).
3。SystemV消息队列 为了便于进程之间通信,我们可以使用管道通信 SystemV也提供了
一些函数来实现进程的通信.这就是消息队列.
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgget(key_t key,int msgflg);
int msgsnd(int msgid,struct msgbuf *msgp,int msgsz,int msgflg);
int msgrcv(int msgid,struct msgbuf *msgp,int msgsz,
long msgtype,int msgflg);
int msgctl(Int msgid,int cmd,struct msqid_ds *buf);

struct msgbuf {
long msgtype; /* 消息类型 */
....... /* 其他数据类型 */
}
msgget函数和semget一样,返回一个消息队列的标志.msgctl和semctl是对消息进行控制
.. msgsnd和msgrcv函数是用来进行消息通讯的.msgid是接受或者发送的消息队列标志. 
msgp是接受或者发送的内容.msgsz是消息的大小. 结构msgbuf包含的内容是至少有一个
为msgtype.其他的成分是用户定义的.对于发送函数msgflg指出缓冲区用完时候的操作.
接受函数指出无消息时候的处理.一般为0. 接收函数msgtype指出接收消息时候的操作.

如果msgtype=0,接收消息队列的第一个消息.大于0接收队列中消息类型等于这个值的第
一个消息.小于0接收消息队列中小于或者等于msgtype绝对值的所有消息中的最小一个消
息. 我们以一个实例来解释进程通信.下面这个程序有server和client组成.先运行服务
端后运行客户端.
服务端 server.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#define MSG_FILE "server.c"
#define BUFFER 255
#define PERM S_IRUSR|S_IWUSR
struct msgtype {
long mtype;
char buffer[BUFFER+1];
};
int main()
{
struct msgtype msg;
key_t key;
int msgid;
if((key=ftok(MSG_FILE,'a'))==-1)
{
fprintf(stderr,"Creat Key Error:%s/a/n",strerror(errno));
exit(1);
}
if((msgid=msgget(key,PERM|IPC_CREAT|IPC_EXCL))==-1)
{
fprintf(stderr,"Creat Message Error:%s/a/n",strerror(errno));
exit(1);
}
while(1)
{
msgrcv(msgid,&msg,sizeof(struct msgtype),1,0);
fprintf(stderr,"Server Receive:%s/n",msg.buffer);
msg.mtype=2;
msgsnd(msgid,&msg,sizeof(struct msgtype),0);
}
exit(0);
} 
----------------------------------------------------------------------------
----
客户端(client.c)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#define MSG_FILE "server.c"
#define BUFFER 255
#define PERM S_IRUSR|S_IWUSR
struct msgtype {
long mtype;
char buffer[BUFFER+1];
};
int main(int argc,char **argv)
{
struct msgtype msg;
key_t key;
int msgid;
if(argc!=2)
{
fprintf(stderr,"Usage:%s string/n/a",argv[0]);
exit(1);
}
if((key=ftok(MSG_FILE,'a'))==-1)
{
fprintf(stderr,"Creat Key Error:%s/a/n",strerror(errno));
exit(1);
}
if((msgid=msgget(key,PERM))==-1)
{
fprintf(stderr,"Creat Message Error:%s/a/n",strerror(errno));
exit(1);
}
msg.mtype=1;
strncpy(msg.buffer,argv[1],BUFFER);
msgsnd(msgid,&msg,sizeof(struct msgtype),0);
memset(&msg,'/0',sizeof(struct msgtype));
msgrcv(msgid,&msg,sizeof(struct msgtype),2,0);
fprintf(stderr,"Client receive:%s/n",msg.buffer);
exit(0);
} 
注意服务端创建的消息队列最后没有删除,我们要使用ipcrm命令来删除的.
4。SystemV共享内存 还有一个进程通信的方法是使用共享内存.SystemV提供了以下几个
函数以实现共享内存.
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int shmget(key_t key,int size,int shmflg);
void *shmat(int shmid,const void *shmaddr,int shmflg);
int shmdt(const void *shmaddr);
int shmctl(int shmid,int cmd,struct shmid_ds *buf);
shmget和shmctl没有什么好解释的.size是共享内存的大小. shmat是用来连接共享内存
的.shmdt是用来断开共享内存的.不要被共享内存词语吓倒,共享内存其实很容易实现和
使用的.shmaddr,shmflg我们只要用0代替就可以了.在使用一个共享内存之前我们调用s
hmat得到共享内存的开始地址,使用结束以后我们使用shmdt断开这个内存.
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define PERM S_IRUSR|S_IWUSR
int main(int argc,char **argv)
{
int shmid;
char *p_addr,*c_addr;
if(argc!=2)
{
fprintf(stderr,"Usage:%s/n/a",argv[0]);
exit(1);
}
if((shmid=shmget(IPC_PRIVATE,1024,PERM))==-1)
{
fprintf(stderr,"Create Share Memory Error:%s/n/a",strerror(errno));
exit(1);
}
if(fork())
{
p_addr=shmat(shmid,0,0);
memset(p_addr,'/0',1024);
strncpy(p_addr,argv[1],1024);
exit(0);
}
else
{
c_addr=shmat(shmid,0,0);
printf("Client get %s",c_addr);
exit(0);
}
}
这个程序是父进程将参数写入到共享内存,然后子进程把内容读出来.最后我们要使用ip
crm释放资源的.先用ipcs找出ID然后用ipcrm shm ID删除.
后记:
进程通信(IPC)是网络程序的基础,在很多的网络程序当中会大量的使用进程通信的概念
和知识.其实进程通信是一件非常复杂的事情,我在这里只是简单的介绍了一下.如果你想
学习进程通信的详细知识,最好的办法是自己不断的写程序和看联机手册.现在网络上有
了很多的知识可以去参考.可惜我看到的很多都是英文编写的.如果你找到了有中文的版
本请尽快告诉我.谢谢! 

