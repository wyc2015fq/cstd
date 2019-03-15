# 多线程互斥之Dekker算法

2017年09月29日 11:38:17 [玉名卍](https://me.csdn.net/fourierFeng) 阅读数：942



## 前言

当并发线程竞争使用同一个资源时，它们互相之间会发生冲突，我们可以把这种情况简单描述如下：两个或更多的线程在它们的运行过程中需要访问一个资源，每个进程并不知道其他线程的存在，并且每一个线程也不受其他线程的影响。每个线程都不影响它所使用的资源的状态，这类资源包括I/O设备、存储器、处理器时间和时钟。
竞争线程间没有任何信息交换，但是一个线程的执行可能会影响到竞争线程的行为。特别是如果两个线程都期望访问同一个资源，操作系统把这个资源分配给一个线程，另一个就必须等待。因此，被拒绝访问的线程速度就会变慢。一种极端情况是，被阻塞的线程永远不能访问这个资源，因此一直不能成功地终止。

## 临界区与临界资源

竞争线程面临的首先是互斥的要求。假设两个或者更多的线程需要访问一个不可共享的资源，如打印机。在执行过程中，每个线程都给该I/O设备发命令，接收状态信息，发送数据和接收数据。我们把这类资源称为临界资源，使用临界资源的那一部分代码称为程序的临界区。一次只允许有一个程序在临界区中，这一点是非常重要的。
由于不清楚详细要求，我们不能仅仅通过操作系统来理解和增强这个限制。这就需要我们程序员具备较强的处理并发的能力。
例如在打印机的例子中，我们希望任何一个线程在打印整个文件时都拥有打印机的控制权，否则在打印结果中就会穿插着来自竞争资源的打印内容。
实施互斥产生了两个额外的控制问题。一个是死锁。一个是饥饿。要解决好这两个问题，就需要一个好的互斥算法，既要使得多线程程序高效运行，又要防止死锁的发生。
假设有nn*n*个线程并发执行，每个线程包括

1. 在某些资源RaRa*R**a*上操作的临界区。
2. 不涉及访问资源RaRa*R**a*的额外代码。

因为所有的线程都需要访问同一资源RaRa*R**a*，因此保证在同一时刻只有一个线程在临界区是很重要的。为实现互斥，需要两个函数：entercriticalentercritical*e**n**t**e**r**c**r**i**t**i**c**a**l*和exitcriticalexitcritical*e**x**i**t**c**r**i**t**i**c**a**l*。每个函数的参数都是竞争使用的资源标识，如果另外一个线程在临界区中，那么任何试图进入关于同一个资源的临界区的线程都必须等待。同时要注意：临界区要尽可能的小，一层一层的封装会使得临界区变大；同时代码过于粗糙也会使临界区变大。下面是伪代码，给出了互斥机制。

```cpp
/*PROCESS*/
void p(Ra)
{
    while(true)
    {
        /*preceding code*/
        entercritical(Ra);
        /*critical section*/
        exicritical(Ra);
        /*following code*/
    }
}
```

## Dekker算法

DijkstraDijkstra*D**i**j**k**s**t**r**a*提出了两个线程互斥的算法，由德国数学家DekkerDekker*D**e**k**k**e**r*实现。它避免了线程互相谦让的问题，算法效率可以得到提高，主要用于生产者-消费者模式。下面是具体实现代码（C++）

```cpp
enum BLOCKMODEL
{
    PRODUCER,CONSUMER
};

bool queue_mutex[2];
int queue_turn;

void Dekker_in(bool mutex[2],int &turn,BLOCKMODEL type)
{
    BLOCKMODEL another_type=(BLOCKMODEL)abs(type-1);
    mutex[type]=true;

    while(mutex[another_type])
    {
        if(turn==another_type)
        {
            mutex[type]=false;
            while(turn==another_type)
            {
                Sleep(1);
            }

            mutex[type]=true;
        }
        Sleep(1);
    }
}

void Dekker_out(bool mutex[2],int &turn,BLOCKMODEL type)
{
    BLOCKMODEL another_type=(BLOCKMODEL)abs(type-1);
    turn=another_type;
    mutex[type]=false;
}


//生产者调用实例（消费者只需要把PRODUCER换成CONSUMER）

Dekker_in(queue_mutex,queue_turn,PRODUCER);

/*

临界区操作

*/
Dekker_out(queue_mutex,queue_turn,PRODUCER);
```

当然，对于互斥算法，各类编程平台都做了封装的工作，可以直接拿来用，但是要追求高效率、高鲁棒性的程序，像这样的实现代码还是很有价值的。但是一般来说DekkerDekker*D**e**k**k**e**r*算法仅能用来处理“简单”的情况，如果并发的线程多于两个，或者共享的资源较多，流程逻辑较为复杂，那还是需要更加高级的多线程互斥的算法。

### 参考文献

[1] 操作系统——精髓与设计原理 【美】William Stallings 编著 陈渝 译

转载请注明出处：<http://blog.csdn.net/fourierFeng/article/details/78132341>