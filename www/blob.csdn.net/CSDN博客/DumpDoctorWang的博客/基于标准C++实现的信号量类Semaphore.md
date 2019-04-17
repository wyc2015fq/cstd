# 基于标准C++实现的信号量类Semaphore - DumpDoctorWang的博客 - CSDN博客





2018年03月28日 18:07:58[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：700








## 一、缘起

在一个项目中，需要使用到信号量这个东东来同步我的线程，C++已经提供了标准的信号量头文件semaphore.h，那我为什么要重新实现一个呢？因为原来的信号量机制是C style的，信号量一多，整个程序就会非常的混乱，因此我基于C++的mutex和condition_variable来实现了一个自定义的信号量类。

## 二、实现

[1、信号量机制简介](https://www.cnblogs.com/han-bing/p/6166391.html)

2、实现

```cpp
/**
 * semaphore类似于一个信号量类，利用mutex和condition_variable来实现
 */
#include <mutex>
#include <condition_variable>
#include <string>
#include <cassert>
#include <chrono>
#include <iostream>

namespace SEM {
    class Semaphore {
    private:
        int _count;                             //等待线程数量
        std::mutex _mutex;                      //互斥锁
        std::condition_variable _condition;     //条件变量
        std::string _name;                      //信号量名字

    public:
        explicit Semaphore(std::string name, int value = 0) :
                _name(std::move(name)), _count(value) {}

        /**
         * 相当于信号量机制里面的P操作.
         * _count大于0(有资源)时,函数会立即返回,否则会阻塞调用此函数的线程.
         */
        void wait() {
            std::unique_lock<std::mutex> lock(_mutex);
            //std::cout<<_name+":"<<_count<<std::endl;
            if (--_count < 0) { // 资源不够
                _condition.wait(lock); // 阻塞
            }
        }

        /**
         * 相当于信号量机制里面的P操作.
         * _count大于0(有资源)时,函数会立即返回,否则会阻塞调用此函数的线程.
         * 但如果等待时间超过seconds指定的值，会唤醒所有阻塞线程.
         * @param ms 等待时间(ms)
         */
        void wait(int ms) {
            assert(ms > 0);
            std::unique_lock<std::mutex> lock(_mutex);
            //std::cout<<_name+":"<<_count<<std::endl;
            if (--_count < 0) {                            // 资源不够
                std::cv_status status
                        = _condition.wait_for(lock, std::chrono::milliseconds(ms));
                if (status == std::cv_status::timeout)   // 超时
                {
                    std::cout << _name + ":timeout" << std::endl;
                    _count = 0;
                    _condition.notify_all();//唤醒所有阻塞线程
                }
            }
        }

        /**
         * 如果有阻塞的线程,则随机唤醒一个线程，相当于信号量机制里面的V操作.否则
         * 立即返回.
         */
        void signal() {
            std::lock_guard<std::mutex> lock(_mutex);
            if (++_count <= 0) {
                _condition.notify_one();
            }
        }

        /**
         * 如果有线程阻塞,唤醒阻塞的所有线程;否则立即返回.
         */
        void signalAll() {
            std::lock_guard<std::mutex> lock(_mutex);
            while (++_count <= 0) {
                _condition.notify_one();
            }
            _count = 0;
        }

        /**
         * 返回这个信号量的名字
         * @return 名字
         */
        std::string name(){
            return _name;
        }

        /**
         * 重载<<,输出信号量的信息
         * @param out 输出流
         * @param sem 信号量
         * @return 输出流
         */
        friend std::ostream &operator<<(std::ostream &out,Semaphore &sem);
    };

    std::ostream &operator<<(std::ostream &out, Semaphore &sem) {
        out<<"Semaphore:"<<sem._name<<",Waiting threads count:"<<sem._count<<std::endl;
        return out;
    }
};
```



## 三、使用

用信号量实现一个简单的生产者消费者模型.

```cpp
#include <iostream>
#include <mutex>
#include <list>
#include <random>
#include "Semaphore.h"
#include <unistd.h>
#include <thread>

using namespace std;

class Goods{
public:
    explicit Goods(int ii):id(ii){
    }
    int id;
};//产品

SEM::Semaphore *g_sem;

list<Goods> g_goods;//商品货架

mutex g_mutex;

void producer(){
    default_random_engine e(static_cast<unsigned long>(time(0)));
    uniform_int_distribution<unsigned> u(0, 10);
    int i=0;
    while (++i<5) {
        //休眠一段随机时间,代表生产过程
        sleep(u(e));
        //生产
        g_mutex.lock();
        Goods good(u(e));
        g_goods.emplace_back(good);
        cout<<"生产产品:"<<good.id<<endl;
        g_mutex.unlock();
        //唤醒一个阻塞的消费者
        g_sem->signal();
    }
}

void costumer(){
    int i=0;
    while (++i<5) {
        g_sem->wait();//有资源会立即返回,没有资源则会等待
        //消费
        g_mutex.lock();
        Goods good = g_goods.front();
        cout<<"消费产品:"<<good.id<<endl;
        g_goods.pop_front();
        g_mutex.unlock();
    }
}
int main() {
    g_sem = new SEM::Semaphore("sem_name",0);
    thread producer_t(producer);
    thread costumer_t(costumer);
    producer_t.join();
    costumer_t.join();
    delete g_sem;
    return 0;
}
```



最后安利一波我自己写的多线程编程模型,非常简单好用!



[https://github.com/Mannix1994/ThreadPool](https://github.com/Mannix1994/ThreadPool)



