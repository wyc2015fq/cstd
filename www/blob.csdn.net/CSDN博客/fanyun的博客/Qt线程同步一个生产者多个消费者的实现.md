# Qt线程同步一个生产者多个消费者的实现 - fanyun的博客 - CSDN博客
2018年02月25日 22:13:30[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：645
        根据QMutex和QSemaphore的特点，我们可以实现一个生成者对应多个消费者的的生产者模型。
可参考：
[http://blog.csdn.net/fanyun_01/article/details/79354222](http://blog.csdn.net/fanyun_01/article/details/79354222)
[http://blog.csdn.net/fanyun_01/article/details/79354106](http://blog.csdn.net/fanyun_01/article/details/79354106)
       互斥量可以锁定一次，而信号量可以在设置上限大小的情况下，获取多次，可以用来保护一定数量的同种资源。在使用acquire函数跨线程获取n个资源。release(n)可以释放n个资源。当没有足够的资源时，调用者将被阻塞直到有足够的资源可用。
```cpp
#include <QCoreApplication>
#include <QSemaphore>
#include <QThread>
#include <QMutex>
#include <iostream>
using namespace std;
QSemaphore vacancy(10);      //资源上限
QSemaphore produce(0);       //产品数量
QMutex mutex;                //互斥锁
int buffer[5];               //缓冲区可以放5个产品
int numtaken=30;
int takei=0;
class Producer:public QThread
{
    public:
    void run();
};
//生产者线程
void Producer::run()
{
    for(int i=0;i<30;i++)    //生产30次产品
    {
        vacancy.acquire();
        buffer[i%5]=i;
        printf("produced %d\n",i);
        produce.release();
    }
}
class Consumer:public QThread
{
    public:
    void run();
};
void Consumer::run()         //消费者线程
{
    while(numtaken>1)
    {
        produce.acquire();
        mutex.lock();        //从缓冲区取出一个产品,多个消费者,不能同时取出,故用了互斥锁
        printf("thread %ul take %d from buffer[%d] \n",currentThreadId(),buffer[takei%5],takei%5);
        takei++;
        numtaken--;
        mutex.unlock();
        vacancy.release();
    }
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Producer producer;
    Consumer consumerA;
    Consumer consumerB;
    Consumer consumerC;
    producer.start();
    consumerA.start();
    consumerB.start();
    consumerC.start();
    producer.wait();
    consumerA.wait();
    consumerB.wait();
    consumerC.wait();
    return 0;
}
```
执行结果如图1所示：
![](https://img-blog.csdn.net/20180223171427129)
