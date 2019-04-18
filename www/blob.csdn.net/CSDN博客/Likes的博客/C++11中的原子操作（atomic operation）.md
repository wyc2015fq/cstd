# C++11中的原子操作（atomic operation） - Likes的博客 - CSDN博客
2018年11月06日 14:50:56[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：33标签：[C  																[atomic																[原子																[互斥](https://so.csdn.net/so/search/s.do?q=互斥&t=blog)](https://so.csdn.net/so/search/s.do?q=原子&t=blog)](https://so.csdn.net/so/search/s.do?q=atomic&t=blog)](https://so.csdn.net/so/search/s.do?q=C  &t=blog)
个人分类：[C++](https://blog.csdn.net/songchuwang1868/article/category/7898933)
所谓的原子操作，取的就是“原子是最小的、不可分割的最小个体”的意义，它表示在多个线程访问同一个全局资源的时候，能够确保所有其他的线程都不在同一时间内访问相同的资源。也就是他确保了在同一时刻只有唯一的线程对这个资源进行访问。这有点类似互斥对象对共享资源的访问的保护，但是原子操作更加接近底层，因而效率更高。
在以往的C++标准中并没有对原子操作进行规定，我们往往是使用汇编语言，或者是借助第三方的线程库，例如intel的pthread来实现。在新标准C++11，引入了原子操作的概念，并通过这个新的头文件提供了多种原子操作数据类型，例如，atomic_bool,atomic_int等等，如果我们在多个线程中对这些类型的共享资源进行操作，编译器将保证这些操作都是原子性的，也就是说，确保任意时刻只有一个线程对这个资源进行访问，编译器将保证，多个线程访问这个共享资源的正确性。从而避免了锁的使用，提高了效率。
我们还是来看一个实际的例子。假若我们要设计一个广告点击统计程序，在服务器程序中，使用多个线程模拟多个用户对广告的点击：
```cpp
#include <boost/thread/thread.hpp>
#include <atomic> 
#include <iostream>
#include <time.h>
using namespace std;
// 全局的结果数据 
long total = 0; 
// 点击函数
void click()
{
    for(int i=0; i<1000000;++i)
    {
        // 对全局数据进行无锁访问 
        total += 1;     
    }
}
 
 
int main(int argc, char* argv[])
{
    // 计时开始
    clock_t start = clock();
    // 创建100个线程模拟点击统计
    boost::thread_group threads;
    for(int i=0; i<100;++i) 
    {
        threads.create_thread(click);
    }
    threads.join_all();
    // 计时结束
    clock_t finish = clock();
    // 输出结果
    cout<<"result:"<<total<<endl;
    cout<<"duration:"<<finish -start<<"ms"<<endl;
    return 0;
}
```
从执行的结果来看，这样的方法虽然非常快，但是结果不正确
```
E:\SourceCode\MinGW>thread.exe
result:87228026
duration:528ms
```
正确结果应该是100*1000000，但是由于没有互斥的访问全局变量，多个线程可能同时读数据，修改数据，导致加的总次数减少
很自然地，我们会想到使用互斥对象来对全局共享资源的访问进行保护，于是有了下面的实现：
```cpp
long total = 0;
// 对共享资源进行保护的互斥对象
mutex m;
void click()
{
    for(int i=0; i<1000000;++i)
    {
        // 访问之前，锁定互斥对象
        m.lock();
        total += 1;
        // 访问完成后，释放互斥对象 
        m.unlock();
    }
}
```
互斥对象的使用，保证了同一时刻只有唯一的一个线程对这个共享进行访问，从执行的结果来看，互斥对象保证了结果的正确性，但是也有非常大的性能损失，从刚才的528ms变成了现在的8431，用了原来时间的10多倍的时间。这个损失够大。
```cpp
E:\SourceCode\MinGW>thread.exe
result:100000000
duration:8431ms
```
如果是在C++11之前，我们的解决方案也就到此为止了，但是，C++对性能的追求是永无止境的，他总是想尽一切办法榨干CPU的性能。在C++11中，实现了原子操作的数据类型（atomic_bool,atomic_int,atomic_long等等），对于这些原子数据类型的共享资源的访问，无需借助mutex等锁机制，也能够实现对共享资源的正确访问。
```cpp
// 引入原子数据类型的头文件
#include <atomic> 
 
// 用原子数据类型作为共享资源的数据类型
atomic_long total(0);
//long total = 0;
 
void click()
{
    for(int i=0; i<1000000;++i)
    {
        // 仅仅是数据类型的不同而以，对其的访问形式与普通数据类型的资源并无区别
        total += 1;
    }
}
```
我们来看看使用原子数据类型之后的效果如何：
```
E:\SourceCode\MinGW>thread.exe
result:100000000
duration:2105ms
```
结果正确！耗时只是使用mutex互斥对象的四分之一！也仅仅是不采用任何保护机制的时间的4倍。可以说这是一个非常不错的成绩了。
原子操作的实现跟普通数据类型类似，但是它能够在保证结果正确的前提下，提供比mutex等锁机制更好的性能，如果我们要访问的共享资源可以用原子数据类型表示，那么在多线程程序中使用这种新的等价数据类型，是一个不错的选择。
**所以在考虑互斥操作前应该先想一想是否能够使用atomic操作。**
