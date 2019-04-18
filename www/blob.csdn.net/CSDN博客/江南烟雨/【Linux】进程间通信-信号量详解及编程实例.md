# 【Linux】进程间通信-信号量详解及编程实例 - 江南烟雨 - CSDN博客
2013年01月07日 10:58:07[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：19597
前面一篇文章[线程同步之信号量同步](http://blog.csdn.net/xiajun07061225/article/details/8467853) 讲的是线程之间的信号量，这篇讲的更加具有通用性，能够实现进程之间的同步。
## 信号量概述
### 信号量定义：
它是一个特殊变量，只允许对它进行等待和发送信号这两种操作。
- P（信号量变量sv）：等待。如果sv大于0，减小sv。如果sv为0，挂起这个进程的执行。
- V（信号量变量sv）：发送信号。如果有进程被挂起等待sv，使其恢复执行。如果没有进行被挂起等待sv，增加sv。
### PV操作伪代码
```cpp
semaphore sem_id = 1;
loop{
    P(sem_id);
   临界区代码；
    V(sem_id);
     非临界区代码;
}
```
## 信号量机制及相关接口
Linux系统中的信号量接口经过了精心设计，提供了比通常所需更多的机制。所有的Linux信号量函数都是针对成组的通用信号量进行操作，而不只是针对一个二进制信号量。但是在绝大多数情况下，使用一个单个信号量就足够了，所以在这里只讨论单个信号量的使用。
### semget函数
用于创建一个新的信号量或者是取得一个已有的信号量的键。
所需包含的头文件：
```cpp
#include <sys/sem.h>
```
它通常依赖于另两个头文件：
```cpp
#include <sys/types.h>
#include <sys/ipc.h>
```
一般情况下，这两个头文件都会被它自动包含。
**功能描述**
　　函数原型：int semget(key_t key,int nsems,int semflg);
　　功能描述
　　获取与某个键关联的信号量集标识。信号量集被建立的情况有两种：
　　1.如果键的值是IPC_PRIVATE。
　　2.或者键的值不是IPC_PRIVATE，并且键所对应的信号量集不存在，同时标志中指定IPC_CREAT。
　　当调用semget创建一个信号量时，他的相应的semid_ds结构被初始化。ipc_perm中各个量被设置为相应
　　值：
　　sem_nsems被设置为nsems所示的值；
　　sem_otime被设置为0；
　　sem_ctime被设置为当前时间
**参数解释**：
       key：所创建或打开信号量集的键值。需要是唯一的非零整数。
　　nsems：创建的信号量集中的信号量的个数，该参数只在创建信号量集时有效。几乎总是取值为1.
　　flag：调用函数的操作类型，也可用于设置信号量集的访问权限，两者通过or表示
**返回值说明：**
　　如果成功，则返回信号量集的IPC标识符（一个正数）。
　　如果失败，则返回-1，errno被设定成以下的某个值
　　EACCES：没有访问该信号量集的权限
　　EEXIST：信号量集已经存在，无法创建
　　EINVAL：参数nsems的值小于0或者大于该信号量集的限制；或者是该key关联的信号量集已存在，并且nsems
　　大于该信号量集的信号量数
　　ENOENT：信号量集不存在，同时没有使用IPC_CREAT
　　ENOMEM ：没有足够的内存创建新的信号量集
　　ENOSPC：超出系统限制
### semop函数
用于改变信号量的值。
```cpp
#include <sys/sem.h>
int semop( int semid, struct sembuf semoparray[], size_t nops );
```
**参数解释：**
参数semid是一个通过semget函数返回的一个信号量标识符
参数nops标明了参数semoparray所指向数组中的元素个数
参数semoparray是一个struct sembuf结构类型的数组指针，
结构sembuf来说明所要执行的操作，其定义如下：
```cpp
struct sembuf{
　　unsigned short sem_num;
　　short sem_op;
　　short sem_flg;
　　}
```
在sembuf结构中，
sem_num是相对应的信号量集中的某一个资源，所以其值是一个从0到相应的信号量集的资源总数（ipc_perm.sem_nsems）之间的整数。除非使用一组讯号了，否则它的取值一般为0.
sem_op的值是一个整数，是信号量在一次操作中需要改变的数值（可以是非1的数值）。通常只会用到两个值：1----P操作，-1---V操作。
sem_flg说明函数semop的行为。通常被设置为SEM_UNDO。它将使得操作系统跟着当前进程对这个信号量的修改情况，如果这个进程在没有释放该信号量的情况下终止，操作系统将自动释放该进程持有的信号量。
注意：
semop调用的一切动作都是一次性完成的，这是为了避免出现因使用多个信号量而可能发生的竞争现象。
### semctl函数
**原型：**
```cpp
int semctl(int semid,int semnum,int cmd,union semun);
```
**返回值：**
       如果成功，则为一个正数。
　　如果失败，则为-1：
errno=EACCESS(权限不够)
　　EFAULT(arg指向的地址无效)
　　EIDRM(信号量集已经删除)
　　EINVAL(信号量集不存在，或者semid无效)
　　EPERM(EUID没有cmd的权利)
　　ERANGE(信号量值超出范围) 
**参数解释：**
sem_id是由semget返回的信号量标识符。
sem_num与前面一个函数相同。
cnd：表示将要采取的动作。最常用的两个值如下：
- SETVAL：用来把信号量初始化为一个已知的值。这个值通过union semun中的val成员设置。其作用是在信号量第一次使用之前对它进行设置。
- IPC_RMID：用于删除一个无需继续使用的信号量标志符。
semun联合结构的定义：
```cpp
semun是在linux/sem.h中定义的：
　　/*arg for semctl systemcalls.*/
　　union semun{
　　int val;/*value for SETVAL*/
　　struct semid_ds *buf;/*buffer for IPC_STAT&IPC_SET*/
　　ushort *array;/*array for GETALL&SETALL*/
　　struct seminfo *__buf;/*buffer for IPC_INFO*/
　　void *__pad;
```
## 信号量的使用
虽然上述函数调用看似很复杂，但是我们可以用这些接口来创建一个简单的PV类型的接口，然后用这个简单的接口来进行信号量相关操作。
下面的程序使用上述接口实现了P、V操组以及设置信号量、删除信号量的操作。
然后利用这些新的函数接口实现了两个程序实例访问临界区的功能。
在这里同时访问临界区的是一个程序的两个不同实例，并且使用参数个数的不同来进行区别。其中一个需要完成信号量的创建及其删除的额外操作。
两个程序在进入临界区和离开临界区的时候分别都会输出两个不同的字符，以此来进行区分。可以发现，两个不同的字符是成对出现的。因为同一时刻只有一个进程可以进入临界区。
完整代码：
```cpp
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>//包含信号量定义的头文件
//联合类型semun定义
union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};
//函数声明
//函数：设置信号量的值
static int set_semvalue(void);
//函数：删除信号量
static void del_semvalue(void);
//函数：信号量P操作
static int semaphore_p(void);
//函数：信号量V操作
static int semaphore_v(void);
static int sem_id;//信号量ID
int main(int argc,char *argv[])
{
	int i;
	int pause_time;
	char op_char = 'O';
	srand((unsigned int)getpid());
	//创建一个新的信号量或者是取得一个已有信号量的键
	sem_id = semget((key_t)1234,1,0666 | IPC_CREAT);
	//如果参数数量大于1，则这个程序负责创建信号和删除信号量
	if(argc > 1)
	{
	    if(!set_semvalue())
	    {
	    	fprintf(stderr,"failed to initialize semaphore\n");
		exit(EXIT_FAILURE);
	    }
	    op_char = 'X';//对进程进行标记
	    sleep(5);
	}
	//循环：访问临界区
	for(i = 0;i < 10;++i)
	{
	    //P操作，尝试进入缓冲区
	    if(!semaphore_p())
		exit(EXIT_FAILURE);
	    printf("%c",op_char);
	    fflush(stdout);//刷新标准输出缓冲区，把输出缓冲区里的东西打印到标准输出设备上
	    pause_time = rand() % 3;
	    sleep(pause_time);
	    printf("%c",op_char);
	    fflush(stdout);
	    //V操作，尝试离开缓冲区
	    if(!semaphore_v())
		exit(EXIT_FAILURE);
	    pause_time = rand() % 2;
	    sleep(pause_time);
	}
	printf("\n %d - finished \n",getpid());
	if(argc > 1)
	{
		sleep(10);
		del_semvalue();//删除信号量
	}
}
//函数：设置信号量的值
static int set_semvalue(void)
{
	union semun sem_union;
	sem_union.val = 1;
	if(semctl(sem_id,0,SETVAL,sem_union))
	    return 0;
	return 1;
}
//函数：删除信号量
static void del_semvalue(void)
{
	union semun sem_union;
	if(semctl(sem_id,0,IPC_RMID,sem_union))
		fprintf(stderr,"Failed to delete semaphore\n");
}
//函数：信号量P操作：对信号量进行减一操作
static int semaphore_p(void)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;//信号量编号
	sem_b.sem_op = -1;//P操作	
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id,&sem_b,1) == -1)
	{
	    fprintf(stderr,"semaphore_p failed\n");
	    return 0;
	}
	return 1;
}
//函数：信号量V操作：对信号量进行加一操作
static int semaphore_v(void)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;//信号量编号
	sem_b.sem_op = 1;//V操作	
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id,&sem_b,1) == -1)
	{
	    fprintf(stderr,"semaphore_v failed\n");
	    return 0;
	}
	return 1;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201301/07/1357527723_3138.png)
如需转载，请注明出处：[http://blog.csdn.net/xiajun07061225/article/details/8475738](http://blog.csdn.net/xiajun07061225/article/details/8475738)
