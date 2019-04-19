# C++多线程编程 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2016年08月18日 19:03:06[dutong0321](https://me.csdn.net/dutong0321)阅读数：516标签：[多线程																[C++																[线程																[控制																[互斥锁](https://so.csdn.net/so/search/s.do?q=互斥锁&t=blog)](https://so.csdn.net/so/search/s.do?q=控制&t=blog)](https://so.csdn.net/so/search/s.do?q=线程&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[编程世界](https://blog.csdn.net/dutong0321/article/category/6127295)
### 概述
#### OS：Raspbian
什么叫做多线程编程，大概意思就是说对多个任务同时进行控制，而且相互之间还需要协调。相信很多人和我一样在开始的时候，认为多线程编程是为了利用多核处理器，使程序运行的更快。但是，现在只要打开操作系统，那么肯定就不止一个任务在工作，所以，和多核没有什么关系。加快速度，从某种层面上来说的确有这个效果。但是，其作用的方式是不同的，有点懵？OK，我们一会再说。
在项目中，遇到了不少需要用到多线程编程的例子。有的是在C++库里需要用到，有的是在Java语言用到，包括开发安卓的过程中。像是在QT中，用到的库是QThread,在java中继承Thread类。在这里，我想用C++中的thread.h这个头文件里的函数来进行举例，这个头文件的方便移植且不需要增加其他的库。虽然，我目前实在Raspbian上面进行编程，但是可以保证在Linux平台下也可以正常运行，windows下mingw也是可以正常运行的，其他的自己试一下吧。
现在，先举几个在项目中用到多线程编程的例子吧。 
1.armLinux的开发中，经常使用中断。（什么叫中断模式，百度百科：暂时停止当前程序的执行转而执行处理新情况的程序和执行过程。简单的来说：就是当运行到某处，暂停运行，直到从硬件处获得某个信号，比如说获得键盘某个键按下，以后有机会细讲）这时候，我们不可以让主进程暂停了，因为主进程还有其他操作。有时候，用到的中断程序不止一个。那么，就需要开多个线程每个中断都处于不同的线程当中，才能检测各个中断信号。 
2.在开发安卓的过程中，有一些操作是不允许在主线程中进行操作。例如网络通信。其实也是可以理解的，网络通信大多都需要长时间操作，那么如果主线程已经在操作UI了，如果此时耗时太多在网络通信上面的话，UI的创建就会延迟，这样手机就会出现卡顿，我们的手机已经够卡的了，所以，安卓就不允许在主线程中进行网络通信。
### 多线程基本使用
创建两个线程，输出他们是第几个线程。
```cpp
#include <iostream>
// 线程相关类，在类Unix下一般都有
#include <pthread.h>
using namespace std;
// 定义线程启动的函数
void* fun1(void* args)
{
    cout << "This is first Thread!\n";
}
void* fun2(void* args)
{
    cout << "This is second Thread!\n";
}
int main()
{
    // 线程结构体
    pthread_t pt1,pt2;
    // 创建线程，线程创建成功，返回值是0
    if(pthread_create(&pt1,NULL,fun1,NULL) != 0)
    {
        cout << "Error in create first Thread!\n";
    }
    if(pthread_create(&pt2,NULL,fun2,NULL) != 0)
    {
        cout << "Error in create second Thread!\n";
    }
    //等待线程结束后，结束进程
    pthread_exit(NULL);
    return 0;
}
```
注意：在这里编译的时候，需要在IDE中设置静态编译库。所以，我就直接用命令行来进行编译。
```
g++ -o thread0 thread0.cpp -lpthread
./thread0
```
大多数情况下会先输出第一个线程所要输出的内容，但是如果多次运行的话，会发现有时候线程二的内容会在线程一的内容出来前出现。OK，现在有三个问题了： 
1.不是C++多线程编程吗，那么怎么在类里面使用呢？ 
2.怎么往线程里面传入参数呢？ 
3.如果我们必须要线程一完成以后再进行线程二，怎么办？
### 线程调用类中函数
如果线程调用类中的函数，那么必须将该函数生命为静态函数。 
静态成员函数属于静态全局区，线程可以共享该区域。
```cpp
#include <iostream>
// 线程相关类，在类Unix下一般都有
#include <pthread.h>
using namespace std;
// 定义一个类
class MyThread
{
private:
public:
    // 定义线程启动的函数，必须是静态的
    static void* fun1(void* args);
    static void* fun2(void* args);
};
void* MyThread::fun1(void* args)
{
    cout << "This is first Thread!\n";
}
void* MyThread::fun2(void* args)
{
    cout << "This is second Thread!\n";
}
int main()
{
    // 线程结构体
    pthread_t pt1,pt2;
    // 创建线程，线程创建成功，返回值是0
    if(pthread_create(&pt1,NULL,MyThread::fun1,NULL) != 0)
    {
        cout << "Error in create first Thread!\n";
    }
    if(pthread_create(&pt2,NULL,MyThread::fun2,NULL) != 0)
    {
        cout << "Error in create second Thread!\n";
    }
    //等待线程结束后，结束进程
    pthread_exit(NULL);
    return 0;
}
```
命令其实和上面一样的：
```
g++ -o thread1 thread1.cpp -lpthread
./thread1
```
### 传入参数
现在，往里面传入参数来确定分别是哪个线程来调用的。
```cpp
#include <iostream>
// 线程相关类，在类Unix下一般都有
#include <pthread.h>
#include <string>
using namespace std;
// 定义一个类
class MyThread
{
public:
    // 定义线程启动的函数，必须是静态的
    static void* fun(void* args);
};
void* MyThread::fun(void* args)
{
    cout << "This is " << *((string*) args) << " Thread!\n";
}
int main()
{
    // 线程结构体
    pthread_t pt;
    string ss1,ss2;
    // 创建线程，线程创建成功，返回值是0
    ss1 = "first";
    if(pthread_create(&pt,NULL,MyThread::fun,(void*)&ss1) != 0)
    {
        cout << "Error in create first Thread!\n";
    }
    ss2 = "second";
    if(pthread_create(&pt,NULL,MyThread::fun,(void*)&ss2) != 0)
    {
        cout << "Error in create first Thread!\n";
    }
    //等待线程结束后，结束进程
    pthread_exit(NULL);
    return 0;
}
```
编译运行命令以后不写了，同上即可。 
我们来看一下结果，结果有点奇怪。有时候是first在前，有时候second在前，这自然不用多说。但是还有一种情况是：段错误。所以，我们继续来控制一下每个线程运行的前后顺序。
### 线程控制
#### 结束某个线程或者控制线程等待某个信号量
定义两个线程，两个线程启动，在第一次输入后，结束地一个线程，在第二次输入后，开始运行线程二中的实际内容。
```cpp
#include <iostream>
#include <pthread.h>
#include <cstdio>
using namespace std;
// 定义第一个线程类
class MyThread0
{
public:
    static bool run;
    // 定义线程启动的函数，必须是静态的
    static void* fun(void* args);
};
// 定义第二个线程类
class MyThread1
{
public:
    static bool waitRun;
    // 定义线程启动的函数，必须是静态的
    static void* fun(void* args);
};
bool MyThread0::run = true;
bool MyThread1::waitRun = true;
void* MyThread0::fun(void* args)
{
    int i = 0;
    while(run)
    {
        i = 1;
        // 执行的任务
    }
    cout << i << endl;
}
void* MyThread1::fun(void* args)
{
    while(waitRun);
    // 实际执行的任务
    cout << "MyThread1 is OK!" << endl;
}
int main()
{
    pthread_t pt;
    if(pthread_create(&pt,NULL,MyThread0::fun,NULL) != 0)
    {
        cout << "Error in create first Thread!\n";
    }
    if(pthread_create(&pt,NULL,MyThread1::fun,NULL) != 0)
    {
        cout << "Error in create first Thread!\n";
    }
    // 至此两个程序已经开始运行
    getchar();
    MyThread0::run = false;
    getchar();
    MyThread1::waitRun = false;
    pthread_exit(NULL);
    return 0;
}
```
我用这个方法很长时间了，杀人越货之利器，好理解，没有额外的函数，而且无论语言都可以直接移植。但是毕竟不是个特别好的办法，而且如果两个线程共享一个数据，那么就一定要先锁定某个线程，然后准备好资源后才可以启动另外一个线程，太过麻烦。而且一直要陷在死循环里暂停程序也不像个事。（其实，我是从嵌入式里面学到的）其实，在学习操作系统/数据库原理中对于并发性操作，都是相当需要注意的，每个线程的先后都要控制好，而且需要注意数据的保护和读写，这时候我们就需要引入互斥锁机制。
#### 等待某个线程结束再开始其他线程
我们首先来看一下程序，程序中最后有个函数pthread_exit，这里的意思是等所有子线程结束后，主线程结束，也就是说到这个函数程序将会结束，以后所有的语句都不会再执行。而且，还有个问题就是等待所有线程结束，根本无法判断个别线程结束。那么，我们可不可以先等待某个线程结束，再继续运行下一步呢？
```cpp
#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;
void* change_value(void* args)
{
    cout << "This thread value is " << *((int *)args) <<  endl;
}
int main()
{
    int value1 = 1,value2 = 2;
    pthread_t pt1,pt2;
    int ret = pthread_create(&pt1, NULL, change_value, (void*)&value1);
    if(ret != 0)
    {
    cout << "pthread_create error:error_code=" << ret << endl;
    }
    //pthread_join的另外一个作用就是可以控制等待某个进程结束
    pthread_join(pt1,NULL);
    ret = pthread_create(&pt2, NULL, change_value, (void*)&value2);
    if(ret != 0)
    {
    cout << "pthread_create error:error_code=" << ret << endl;
    }
    pthread_join(pt2,NULL);
    return 0;
}
```
#### 获得线程结束信息
我们首先来看一下程序，程序中最后有个函数pthread_exit，这里的意思是等子线程结束后，主线程结束，也就是说到这个函数程序将会结束，以后所有的语句都不会再执行。OK，那么我们即使获得线程结束信息，也无法表示。那么我们吧这条语句去了，去了的话会发现有的线程还没有运行完成，程序就已经结束了，怎么解决？当然也可以估计一下时间然后等待，或者设置一个公共变量，然后在主线程中利用循环暂停，但是，我们这里用系统提供给我们的函数来进行操作。
```cpp
#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;
void* change_value(void* args)
{
    cout << "This thread value is " << *((int *)args) <<  endl;
    int result = *((int *)args) + 10;
    pthread_exit((void*)result);
}
int main()
{
    int value1 = 1,value2 = 3;
    void* result;
    pthread_t pt1,pt2;
    int ret = pthread_create(&pt1, NULL, change_value, (void*)&value1);
    if(ret != 0)
    {
        cout << "pthread_create error:error_code=" << ret << endl;
    }
    pthread_join(pt1,&result);
    cout << "The thread1's result is " << (int)result << endl;
    ret = pthread_create(&pt2, NULL, change_value, (void*)&value2);
    if(ret != 0)
    {
        cout << "pthread_create error:error_code=" << ret << endl;
    }
    pthread_join(pt2,&result);
    cout << "The thread2's result is " << (int)result << endl;
    return 0;
}
```
#### 互斥锁
互斥锁的作用是访问数据时可以保证是被一个线程所访问，并不能控制线程执行顺序的先后。
```cpp
#include <iostream>
#include <pthread.h>
using namespace std;
// 定义第一个线程类
class MyThread
{
public:
    static int flag;
    // 定义线程启动的函数，必须是静态的
    static void* fun1(void* args);
    static void* fun2(void* args);
};
int MyThread::flag = 0;
pthread_mutex_t sum_mutex;
int temp;
void* MyThread::fun1(void* args)
{
    // 上锁
    pthread_mutex_lock(&sum_mutex);
    cout << "This is " << ++flag << " Thread1!\n";
    // 解锁
    pthread_mutex_unlock(&sum_mutex);
}
void* MyThread::fun2(void* args)
{
    pthread_mutex_lock(&sum_mutex);
    cout << "This is " << ++flag << " Thread2!\n";
    pthread_mutex_unlock(&sum_mutex);
}
int main()
{
    pthread_t pt;
    if(pthread_create(&pt,NULL,MyThread::fun1,NULL) != 0)
    {
        cout << "Error in create first Thread!\n";
    }
    if(pthread_create(&pt,NULL,MyThread::fun2,NULL) != 0)
    {
        cout << "Error in create first Thread!\n";
    }
    pthread_exit(NULL);
    cout << temp << endl;
    pthread_mutex_destroy(&sum_mutex);
    return 0;
}
```
基本到此为止，多线程的一些最基本的使用就到此结束了，在使用的过程中，发现自己的很多基础知识都有很多断层，大概这就是我用到哪里学到哪里的一个大BUG。这样也好，从最基本的开始一点一点来检查。
