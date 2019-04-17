# 【面经笔记】线程、多线程 - 死锁、读写、消费者问题、TLS【续2】 - xiaxzhou的博客 - CSDN博客





2017年07月23日 20:30:17[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：406








### **死锁：**
- 
定义：一组相互竞争资源或进行通信的进程间的**永久阻塞。**

- 
死锁产生的原因： 

（1） 因为系统资源不足。 

（2） 进程运行推进的顺序不合适。 

（3） 资源分配不当等。- 
死锁条件：**互斥、占有并等待、不可剥夺**、（循环等待）

- 
死锁**预防**：解除死锁的必要条件，排除发生死锁的可能性。
- 
互斥：不可禁止

- 
占有并等待：要求进程一次性请求所有资源，阻塞直到所有请求满足

- 
不可剥夺：进程若进一步请求被拒绝，则需释放最初占有的资源；优先级高的进程可以抢占优先级低的进程资源。


- 
死锁**避免**：允许三个发生死锁的条件，但是通过明智的策略，确保不会发生死锁。
- 
**进程启动拒绝**：如果一个进程的请求会导致死锁，则不启动这个进程： 

对每种资源，都需要满足： 
**新进程及已有进程对此项资源的最大需求之和小于系统可分配资源数**- 
**资源分配拒绝**：如果一个进程增加的资源请求会导致死锁，则不允许资源分配：银行家算法


#### **银行家算法：**

> 
参考：[http://blog.csdn.net/leo115/article/details/8039589](http://blog.csdn.net/leo115/article/details/8039589)

链接中博文指出算法核心是：保证银行家的余额至少不小于一个客户的限额 

  这种判断安全的方法必须和进程启动拒绝判断**一起使用**才可以避免死锁。 

  如：系统资源10；A、B、C三个进程分别最大需求：8、 8、 2 。 

  A请求4 、 B请求 4 。 

  而此时 A还需要4且B还需要4才能结束，则发生死锁。 

  但如果有进程启动拒绝，则B进程是不会被启动的。
**安全状态**：至少有一个**资源分配序列**不会导致死锁，所有进程都能运行直到结束。

**安全状态的判断：每次找一个可满足其最大资源需求的进程，回收其资源；直到所有进程回收。中途若找不到可满足最大资源需求的进程，则说明不安全。**

若已有进程启动拒绝判断，只需剩余资源可以满足某一个进程的最大需求，则为安全状态。

当进程请求资源时，**假设同意该请求**，改变了系统状态，然后确认其结果是否**还是安全状态**。若是，则同意请求；否则，拒绝请求。

### 生产者消费者问题：
- 任何时候只有一个主体可以访问缓冲区
- 缓冲区大小有限 ：N

```cpp
#include <pthread.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <semaphore.h>
#include <iostream>
#pragma comment(lib,"pthreadVC2.lib") 

using namespace::std;

#define CONSUMERNUM 3
#define BUFFERSIZE 10

vector<int> data;

sem_t sem_buffer;//信号量：防止缓冲区溢出
sem_t sem_product;//信号量：防止消费者消费空
pthread_mutex_t mutex;//互斥量：限制访问缓冲区线程数

void * consume_func(void * arg)
{
    int id = (int)arg;
    while (1)
    {
        sem_wait(&sem_product);
        pthread_mutex_lock(&mutex);
        cout << "线程"<<id <<"消费： "<<*data.begin() << endl;
        data.erase(data.begin());
        sem_post(&sem_buffer);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
void * product_func(void * arg)
{
    int i(0);
    while (1)
    {
        sem_wait(&sem_buffer);
        pthread_mutex_lock(&mutex);
        data.push_back(i++);
        cout <<"线程生产: "<< data.back() << endl;
        sem_post(&sem_product);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main()
{
    pthread_mutex_init(&mutex, 0);
    sem_init(&sem_buffer, 0, 10);
    sem_init(&sem_product, 0, 0);
    pthread_t consumer[CONSUMERNUM], productor;

    for (auto i = 0; i < CONSUMERNUM;++i)
        pthread_create(&consumer[i], NULL, consume_func, (void *)i);
    pthread_create(&productor, NULL, product_func, NULL);

    void * res;
    for (auto i = 0; i < CONSUMERNUM;++i)
        pthread_join(consumer[i], &res);
    pthread_join(productor, &res);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_buffer);
    sem_destroy(&sem_product);
    return 0;
}
```

### 读者-写者问题 ：

> 
生产者-消费者问题能不能看成一个写进程，一个读进程的读者-写者问题呢？


不能，生产者不仅是一个写进程，还必须读取队列指针，决定下一项往哪里写，还必须确定缓冲区是否已满，消费者不仅仅是读取数据，还必须调整队列指针。

直接使用读写锁：

```
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <semaphore.h>
#include <iostream>
#pragma comment(lib,"pthreadVC2.lib") 

using namespace::std;

pthread_rwlock_t rwlock;

pthread_t readerM, readerN, writerA, writerB;

int data(1);//共享资源

void* readM(void * arg)
{
    while (1)
    {
        pthread_rwlock_rdlock(&rwlock);
        cout << "读者M 读取数据：" << data << endl;
        pthread_rwlock_unlock(&rwlock);
        Sleep(1200);
    }
    return NULL;
}

void* readN(void * arg)
{
    while (1)
    {
        pthread_rwlock_rdlock(&rwlock);
        cout << "读者N 读取数据：" << data << endl;
        pthread_rwlock_unlock(&rwlock);
        Sleep(1200);
    }
    return NULL;
}

void* writeA(void * arg)
{
    while (1)
    {
        pthread_rwlock_wrlock(&rwlock);
        data++;
        cout << "写者A 写入数据：" << data << endl;
        pthread_rwlock_unlock(&rwlock);
        Sleep(2000);
    }
    return NULL;
}

void* writeB(void * arg)
{
    while (1)
    {
        pthread_rwlock_wrlock(&rwlock);
        data++;
        cout << "写者B 写入数据：" << data << endl;
        pthread_rwlock_unlock(&rwlock);
        Sleep(2000);

    }
    return NULL;
}

int main()
{
    pthread_rwlock_init(&rwlock,NULL);

    pthread_create(&readerM, NULL, readM, NULL);
    pthread_create(&readerN, NULL, readN, NULL);
    pthread_create(&writerA, NULL, writeA, NULL);
    pthread_create(&writerB, NULL, writeB, NULL);

    void * res;
    pthread_join(readerM,&res);
    pthread_join(readerN,&res);
    pthread_join(writerA,&res);
    pthread_join(writerB,&res);

    pthread_rwlock_destroy(&rwlock);

    return 0;
}
```

### **volatile :**

volatile 关键字是一种类型修饰符，用它声明的类型变量表示可以被某些编译器未知的因素更改，比如：操作系统、硬件或者其它线程等。遇到这个关键字声明的变量，编译器对访问该变量的代码就不再进行优化，从而可以提供对特殊地址的稳定访问。声明时语法：int volatile vInt; 

当要求使用 volatile 声明的变量的值的时候，系统总是重新从它所在的内存读取数据，即使它前面的指令刚刚从该处读取过数据。而且读取的数据立刻被保存。

有些变量是用 volatile 关键字声明的。当两个线程都要用到某一个变量且该变量的值会被改变时，应该用 volatile 声明，该关键字的作用是防止优化编译器把变量从内存装入  CPU 寄存器中。如果变量被装入寄存器，那么两个线程有可能一个使用内存中的变量，一个使用寄存器中的变量，这会造成程序的错误执行。volatile 的意思是让编译器每次操作该变量时一定要从内存中真正取出，而不是使用已经存在寄存器中的值

### C++类中线程callback函数怎么使用自己类的成员

首先回调函数得定义成static，然后把this指针传入

```cpp
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib,"pthreadVC2.lib") 

using namespace::std;

class my_class
{
public:
    static void *thrd_func(void* p_)
    {
        my_class* p = (my_class*)p_;
        cout << p->value << endl;
        return NULL;
    }
    my_class(int v) :value(v){};
private:
    int value;
};

int main()
{
    pthread_t class_thrd;
    my_class tmp_class(3);
    pthread_create(&class_thrd, NULL, my_class::thrd_func, (void*)&tmp_class);

    void *res;
    pthread_join(class_thrd,&res);
    return 0;
}
```

### 线程局部存储：

如果需要在一个线程内部的各个函数调用都能访问、但其它线程不能访问的变量（被称为static memory local to a thread 线程局部静态变量），就需要新的机制来实现。这就是TLS。

要实现线程局部存储并不难，最简单的办法就是建立一个全局表，通过当前线程ID去查询相应的数据，因为各个线程的ID不同，查到的数据自然也不同了。但Windows系统采用了每个线程建线程专享的索引表，表的条目为线程局部存储的地址。在线程执行的任何代码处，都可以查询本线程的这个索引表获得要访问的线程局部存储的地址。

windows下：
- 动态TLS

TlsAlloc() 

TlsSetValue() 

TlsGetValue() 

TlsFree()- 静态TLS：

win32 声明线程局部静态变量：
`__declspec( thread ) int var_name;`
静态链接时，会将所有模块的.tls段加在一起 

动态链接时，会动态扩大缩减应用程序的.tls段

无锁编程：  

有锁编程的问题与解决方法：阻塞（效率下降）、死锁、优先级反转问题  

无锁编程解决单生产者多消费者问题和多生产者多消费者问题  

线程安全的接口











