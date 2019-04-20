# linux消息队列操作(转) - h13 - 博客园
对消息队列的操作无非有下面三种类型：
1、 打开或创建消息队列
消息队列的内核持续性要求每个消息队列都在系统范围内对应唯一的键值，所以，要获得一个消息队列的描述字，只需提供该消息队列的键值即可；
注：消息队列描述字是由在系统范围内唯一的键值生成的，而键值可以看作对应系统内的一条路经。
2、 读写操作
消息读写操作非常简单，对开发人员来说，每个消息都类似如下的[数据](http://www.3800hk.com/)结构：
```
struct msgbuf{
long mtype;
char mtext[1];
};
```
mtype成员代表消息类型，从消息队列中读取消息的一个重要依据就是消息的类型；mtext是消息内容，当然长度不一定为1。因此，对于发送消息来说，首先预置一个msgbuf缓冲区并写入消息类型和内容，调用相应的发送函数即可；对读取消息来说，首先分配这样一个msgbuf缓冲区，然后把消息读入该缓冲区即可。
3、 获得或设置消息队列属性：
消息队列的信息基本上都保存在消息队列头中，因此，可以分配一个类似于消息队列头的结构(struct msqid_ds，见[附录 2](http://www-900.ibm.com/developerWorks/cn/linux/l-ipc/part3/index.shtml#listing2))，来返回消息队列的属性；同样可以设置该[数据](http://www.3800hk.com/)结构。
![](http://www-900.ibm.com/developerWorks/cn/linux/l-ipc/part3/fig2.gif)
消息队列API
**1、文件名到键值**
```
#include <sys/types.h>
#include <sys/ipc.h>
key_t ftok (char*pathname, char proj)；
```
它返回与路径pathname相对应的一个键值。该函数不直接对消息队列操作，但在调用ipc(MSGGET,…)或msgget()来获得消息队列描述字前，往往要调用该函数。典型的调用代码是：
```
key=ftok(path_ptr, 'a');
    ipc_id=ipc(MSGGET, (int)key, flags,0,NULL,0);
    …
```
**2.系统V消息队列API**
系统V消息队列API共有四个，使用时需要包括几个头文件：
```
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
```
**1）int msgget(key_t key, int msgflg)**
参数key是一个键值，由ftok获得；msgflg参数是一些标志位。该调用返回与健值key相对应的消息队列描述字。
在以下两种情况下，该调用将创建一个新的消息队列：
- 如果没有消息队列与健值key相对应，并且msgflg中包含了IPC_CREAT标志位； 
- key参数为IPC_PRIVATE； 
参数msgflg可以为以下：IPC_CREAT、IPC_EXCL、IPC_NOWAIT或三者的或结果。
**调用返回：**成功返回消息队列描述字，否则返回-1。
注：参数key设置成常数IPC_PRIVATE并不意味着其他进程不能访问该消息队列，只意味着即将创建新的消息队列。
**2）int msgrcv(int msqid, struct msgbuf *msgp, int msgsz, long msgtyp, int msgflg);**
该系统调用从msgid代表的消息队列中读取一个消息，并把消息存储在msgp指向的msgbuf结构中。
msqid为消息队列描述字；消息返回后存储在msgp指向的地址，msgsz指定msgbuf的mtext成员的长度（即消息内容的长度），msgtyp为请求读取的消息类型；读消息标志msgflg可以为以下几个常值的或：
- IPC_NOWAIT 如果没有满足条件的消息，调用立即返回，此时，errno=ENOMSG 
- IPC_EXCEPT 与msgtyp>0配合使用，返回队列中第一个类型不为msgtyp的消息 
- IPC_NOERROR 如果队列中满足条件的消息内容大于所请求的msgsz字节，则把该消息截断，截断部分将丢失。 
msgrcv手册中详细给出了消息类型取不同值时(>0; <0; =0)，调用将返回消息队列中的哪个消息。
msgrcv()解除阻塞的条件有三个：
- 消息队列中有了满足条件的消息； 
- msqid代表的消息队列被删除； 
- 调用msgrcv（）的进程被信号中断； 
**调用返回：**成功返回读出消息的实际字节数，否则返回-1。
**3）int msgsnd(int msqid, struct msgbuf *msgp, int msgsz, int msgflg);**
向msgid代表的消息队列发送一个消息，即将发送的消息存储在msgp指向的msgbuf结构中，消息的大小由msgze指定。
对发送消息来说，有意义的msgflg标志为IPC_NOWAIT，指明在消息队列没有足够空间容纳要发送的消息时，msgsnd是否等待。造成msgsnd()等待的条件有两种：
- 当前消息的大小与当前消息队列中的字节数之和超过了消息队列的总容量； 
- 当前消息队列的消息数（单位"个"）不小于消息队列的总容量（单位"字节数"），此时，虽然消息队列中的消息数目很多，但基本上都只有一个字节。 
msgsnd()解除阻塞的条件有三个： 
- 不满足上述两个条件，即消息队列中有容纳该消息的空间； 
- msqid代表的消息队列被删除； 
- 调用msgsnd（）的进程被信号中断； 
**调用返回：**成功返回0，否则返回-1。
**4）int msgctl(int msqid, int cmd, struct msqid_ds *buf);**
该系统调用对由msqid标识的消息队列执行cmd操作，共有三种cmd操作：IPC_STAT、IPC_SET 、IPC_RMID。
- IPC_STAT：该命令用来获取消息队列信息，返回的信息存贮在buf指向的msqid结构中； 
- IPC_SET：该命令用来设置消息队列的属性，要设置的属性存储在buf指向的msqid结构中；可设置属性包括：msg_perm.uid、msg_perm.gid、msg_perm.mode以及msg_qbytes，同时，也影响msg_ctime成员。 
- IPC_RMID：删除msqid标识的消息队列； 
**调用返回：**成功返回0，否则返回-1。
----------------------------------------------------------------------------------------------------------------------
/*msgserver.c*/
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#define   MSG_FILE "msgserver.c" 
#define   BUFFER 255 
#define   PERM S_IRUSR|S_IWUSR 
/* 服务端创建的消息队列最后没有删除,我们要使用ipcrm命令来删除的 */
/* ipcrm -q <msqid> */
struct msgtype { 
    long mtype; 
    char buffer[BUFFER+1]; 
}; 
int main() 
{ 
    struct msgtype msg; 
    key_t key; 
    int msgid; 
    if((key=ftok(MSG_FILE,'a'))==-1) 
    { 
        fprintf(stderr,"Creat Key Error:%s/n", strerror(errno)); 
        exit(1); 
    } 
    if((msgid=msgget(key, PERM|IPC_CREAT|IPC_EXCL))==-1) 
    {
        fprintf(stderr, "Creat Message Error:%s/n", strerror(errno)); 
        exit(1);
    } 
    printf("msqid = %d/n", msgid);
    while(1)
    { 
        msgrcv(msgid, &msg, sizeof(struct msgtype), 1, 0); 
        fprintf(stderr,"Server Receive:%s/n", msg.buffer); 
        msg.mtype = 2; 
        msgsnd(msgid, &msg, sizeof(struct msgtype), 0); 
    } 
    exit(0); 
} 
/* msgclient.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#define   MSG_FILE "msgserver.c" 
#define   BUFFER 255 
#define   PERM S_IRUSR|S_IWUSR 
struct msgtype { 
    long mtype; 
    char buffer[BUFFER+1]; 
}; 
int main(int argc, char **argv) 
{ 
    struct msgtype msg; 
    key_t key; 
    int msgid; 
    if(argc != 2) 
    { 
        fprintf(stderr,"Usage:%s string/n", argv[0]); 
        exit(1); 
    } 
    if((key=ftok(MSG_FILE,'a'))==-1) 
    { 
        fprintf(stderr,"Creat Key Error:%s/n", strerror(errno)); 
        exit(1); 
    } 
    if((msgid=msgget(key, PERM))==-1) 
    { 
        fprintf(stderr,"Creat Message  Error:%s/n", strerror(errno)); 
        exit(1); 
    } 
    msg.mtype = 1; 
    strncpy(msg.buffer, argv[1], BUFFER); 
    msgsnd(msgid, &msg, sizeof(struct msgtype), 0);  
    memset(&msg, '/0', sizeof(struct msgtype)); 
    msgrcv(msgid, &msg, sizeof(struct msgtype), 2, 0); 
    fprintf(stderr, "Client receive:%s/n", msg.buffer); 
    exit(0);
}   
[http://blog.csdn.net/zhsp1029/article/details/2171462](http://blog.csdn.net/zhsp1029/article/details/2171462)
