# pthread_cond_wait的思考 - Orisun - 博客园







# [pthread_cond_wait的思考](https://www.cnblogs.com/zhangchaoyang/articles/2302085.html)





java中有wait()、notify()、notifyall()，Linux C中有一组函数和它们是一一对应的，实现完全相同的功能。

```
int  pthread_cond_wait(pthread_cond_t  *cond,pthread_mutex_t *mutex);
int pthread_cond_signal(pthread_cond_t * cond);
int pthread_cond_broadcast(pthread_cond_t * cond);
```

要实现java中的synchonized，在Linux C中可以使用：

```
pthread_mutex_lock(pthread_mutex_t *);
pthread_mutex_unlock(pthread_mutex_t *);
```

在java中我们会这样用：

```
class Syncrimitive implements Watcher{
    Integer mutex=new Integer(-1);
    @Override
    //只要有create或delete发生，都会调用process，都会调用mutex.notify()。
    synchronized public void process(WatchedEvent event){
        synchronized(mutex){
            mutex.notify();
        }
    }
    class Barrier extends SyncPrimitive{
        boolean enter(){
            while(true){
                synchronized(mutex){
                    List<String> list=zjp.getChildren(root,true);
                    if(list.size()<size){
                        mutex.wait();
                    }
                    else{
                        return true;
                    }
                }
            }
        }
    }
}
```

wait()、notify()、notifyall()只能在synchronized块或方法中被调用，否则会抛异常。

wait()不仅是让出了CPU的使用权，而且把持有的锁释放了。

分析上面的java代码：在调用mutex.wait()之前的若干步我们锁住了mutex；调用mutex.wait()让出CPU的使用权，并且释放了锁；调用mutex.notify()之前先获得mutex上的锁，调用notify之后退出了synchronized块，即立刻释放了锁；这时wait()被唤醒，它首先获得mutex上的锁，再执行下面的语句。

int  pthread_cond_wait(pthread_cond_t  *cond,pthread_mutex_t *mutex);

pthread_cond_wait总和一个互斥锁结合使用。在调用pthread_cond_wait前要先获取锁。pthread_cond_wait函数执行时先自动释放指定的锁，然后等待条件变量的变化。在函数调用返回之前，自动将指定的互斥量重新锁住。

int pthread_cond_signal(pthread_cond_t * cond);

pthread_cond_signal通过条件变量cond发送消息，若多个消息在等待，它只唤醒一个。pthread_cond_broadcast可以唤醒所有。调用pthread_cond_signal后要立刻释放互斥锁，因为pthread_cond_wait的最后一步是要将指定的互斥量重新锁住，如果pthread_cond_signal之后没有释放互斥锁，pthread_cond_wait仍然要阻塞。

问题：一个大班有两个小班，一天老师给这个大班上课，他想统计一下有多少人出勤。如果仅仅是为了得到总的出勤人数，那创建两个线程分别统计两个小班的人数，等2个线程都退出后，主线程就可以知道总出勤人数了。但是老师还想在任一个班级统计出人数后立刻把该数字宣布出来。

**方案1**：建立一个公共容器开始为空，当任一个小班统计出人数后把人数放入容器中。主线程循环检查容器是否为空，如果不为空就从容器中读出数据，然后把容器置空。这里的容器是个公共变量，任意一个线程在对它进行读写操作之前要先获得互斥锁，操作完之后释放互斥锁。

```
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<ctype.h>
#include<time.h>
struct arg_set{     //线程函数的参数结构体
    char *fname;
    int count;
};
struct arg_set *mailbox;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;     //互斥变量
pthread_cond_t flag=PTHREAD_COND_INITIALIZER;       //条件变量
main(int argc,char *argv[]){
    clock_t clk=clock();
    pthread_t t1,t2;
    struct arg_set args1,args2;
    void *count_words(void*);       //线程执行的函数
    int reports_in=0;
    int total_words=0;
    if(argc!=3){
        printf("usage:%s file1 file2\n",argv[0]);
        exit(1);
    } 
    args1.fname=argv[1];
    args1.count=0;
    pthread_create(&t1,NULL,count_words,(void*)&args1);
    args2.fname=argv[2];
    args2.count=0;
    pthread_create(&t2,NULL,count_words,(void*)&args2);
    while(reports_in<2){    //需要两个线程都报告结果
        if(mailbox==NULL)　　　　//循环查看容器是否为空
            continue;
        printf("main:waiting to get lock\n");
        pthread_mutex_lock(&lock);  //主线程（线程0）要从mailbox(容器)里面读数据，所以要先上锁
        printf("main:wow!I have the lock\n");
        printf("%-7d%s\n",mailbox->count,mailbox->fname);       //从mailbox中读出数据
        total_words+=mailbox->count;
        printf("main:unlocking box\n");
        pthread_mutex_unlock(&lock);
        if(mailbox==&args1)
            pthread_join(t1,NULL);
        if(mailbox==&args2)
            pthread_join(t2,NULL);
        mailbox=NULL;
        reports_in++;
    }
    printf("%-7dtotal words\n",total_words);
    printf("%ld\n",(clock()-clk));
}
void *count_words(void *a){
    struct arg_set *args=a;
    FILE *fp;
    int c,prev='\0';
    if((fp=fopen(args->fname,"r"))!=NULL){
        while((c=getc(fp))!=EOF){
            if(!isalnum(c) && isalnum(prev))
                args->count++;
            prev=c;
        }
        fclose(fp);
    }
    else{
        perror(args->fname);
    }
    printf("count:waiting to get lock\n");
    pthread_mutex_lock(&lock);
    printf("count:have lock,storing data\n");
    mailbox=args;
    printf("count:unlocking box\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}
```

$ ./lock 123 456
count:waiting to get lock
count:have lock,storing data
count:unlocking box
main:waiting to get lock
main:wow!I have the lock
11116 123
main:unlocking box
count:waiting to get lock
count:have lock,storing data
count:unlocking box
main:waiting to get lock
main:wow!I have the lock
11116 456
main:unlocking box
22232 total words
50000

方案1有两个不足：
- 主线程循环查看容器是否为空，这个操作本身是要占用CPU时间的。如果主线程使用pthread_cond_wait（请看方案2），它进入阻塞状态后就不占用CPU时间了，等条件变量到达才时唤醒主线程。我们的方案2确实比方案1使用的时间短。
当一个进程发现临界区上锁后，它会通过循环语句为断检测锁的状态，直到临界区解锁，这种锁叫做自旋锁，这种机制叫做忙等待。 
- 主线程在最后一次查看容器为空和第一次发现容器不为空之间可能发生了很多事情：比如线程1向容器中放入数据后线程2也向容器中放入了数据，覆盖了之前线程1的数据。

**方案2**：主线程调用pthread_cond_wait进入条件等待，任意一个子线程完成人数统计后就调用pthread_cond_signal把主线程唤醒。

```
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<ctype.h>
#include<time.h>
struct arg_set{     //线程函数的参数结构体
    char *fname;
    int count;
};
struct arg_set *mailbox;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;     //互斥变量
pthread_cond_t flag=PTHREAD_COND_INITIALIZER;       //条件变量
main(int argc,char *argv[]){
    clock_t clk=clock();
    pthread_t t1,t2;
    struct arg_set args1,args2;
    void *count_words(void*);       //线程执行的函数
    int reports_in=0;
    int total_words=0;
    if(argc!=3){
        printf("usage:%s file1 file2\n",argv[0]);
        exit(1);
    }
    pthread_mutex_lock(&lock);  //主线程（线程0）要从mailbox(容器)里面读数据，所以要先上锁
    args1.fname=argv[1];
    args1.count=0;
    pthread_create(&t1,NULL,count_words,(void*)&args1);
    args2.fname=argv[2];
    args2.count=0;
    pthread_create(&t2,NULL,count_words,(void*)&args2);
    while(reports_in<2){    //需要两个线程都报告结果
        printf("main:waiting for flag to go up\n");
        pthread_cond_wait(&flag,&lock);     //由于主线程要等2个从线程都统计完单词后才给出最后结果，所以进入条件等待
        printf("main:wow!flag was raised,I have the lock\n");
        printf("%-7d%s\n",mailbox->count,mailbox->fname);       //从mailbox中读出数据
        total_words+=mailbox->count;
        if(mailbox==&args1)
            pthread_join(t1,NULL);
        if(mailbox==&args2)
            pthread_join(t2,NULL);
        reports_in++;
    }
    printf("%-7dtotal words\n",total_words);
    printf("%ld\n",(clock()-clk));
}
void *count_words(void *a){
    struct arg_set *args=a;
    FILE *fp;
    int c,prev='\0';
    if((fp=fopen(args->fname,"r"))!=NULL){
        while((c=getc(fp))!=EOF){
            if(!isalnum(c) && isalnum(prev))
                args->count++;
            prev=c;
        }
        fclose(fp);
    }
    else{
        perror(args->fname);
    }
    printf("count:waiting to get lock\n");
    pthread_mutex_lock(&lock);
    printf("count:have lock,storing data\n");
    mailbox=args;
    printf("count:raising flag\n");
    pthread_cond_signal(&flag);
    printf("count:unlocking box\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}
```

$ ./cond 123 456
main:waiting for flag to go up
count:waiting to get lock
count:have lock,storing data
count:raising flag
count:unlocking box
main:wow!flag was raised,I have the lock
11116 123
main:waiting for flag to go up
count:waiting to get lock
count:have lock,storing data
count:raising flag
count:unlocking box
main:wow!flag was raised,I have the lock
11116 456
22232 total words
40000












