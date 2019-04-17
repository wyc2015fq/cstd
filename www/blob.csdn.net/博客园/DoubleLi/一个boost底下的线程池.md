# 一个boost底下的线程池 - DoubleLi - 博客园






Boost的thread库中目前并没有提供线程池，我在sorceforge上找了一个用boost编写的线程池。该线程池和boost结合的比较好，并且提供了多种任务执行策略，使用也非常简单。

### 下载地址：

[http://threadpool.sourceforge.net/](http://threadpool.sourceforge.net/)

### 使用threadpool：

这个线程池不需要编译，只要在项目中包含其头文件就可以了。

### 一个简单的例子：

#include <iostream>
#include "threadpool.hpp"

using namespace std;
using namespace boost::threadpool;

// Some example tasks
void first_task()
{
    cout << "first task is running\n" ;
}

void second_task()
{
    cout << "second task is running\n" ;
}

void task_with_parameter(int value)
{
    cout << "task_with_parameter(" << value << ")\n";
}

int main(int argc,char *argv[])
{
    // Create fifo thread pool container with two threads.
    pool tp(2);

    // Add some tasks to the pool.
    tp.schedule(&first_task);
    tp.schedule(&second_task); 
    tp.schedule(boost::bind(task_with_parameter, 4));

    // Wait until all tasks are finished.
    tp.wait();

    // Now all tasks are finished!    
    return(0);
}

### 任务返回值的获取：

一般异步调用中，返回值的获取有同步获取和异步获取两种形式。

**同步获取返回值：**

int task_int_23()
{
    cout<<"task_int_23()\n";
    return 23;
}

future<int> res = schedule(tp, &task_int_23);
res.wait();

cout<<"get res value:"<<res.get()<<endl;

**异步获取返回值：**

不知道是设计者就不打算使用异步回调获取返回值还是我看的不够仔细，异步获取返回值的方式还真没有找着，只好自己简单的写了一个回调的仿函数来实现异步返回值的获取。

//R为任务函数的返回值类型
template<class R>
class callback_task
{
    typedef boost::function<void (R)> callback;
    typedef boost::function<R ()> function;

private:
    callback c_;
    function f_;

public:
    //F: 任务执行函数 C：结果回调函数
    template<class F,class C>
    callback_task(F f,C c)
    {
        f_ = f;
        c_ = c;
    }

    void operator()()
    {
        c_(f_());
    }
};

通过这个对象可以很容易的实现异步结果的回调。

//task_int_23的结果回调函数 
void callback(int k)
{
    cout<<"get callback value:"<<k<<endl;
}

//通过回调的形式获取任务的返回值 
tp.schedule(callback_task<int>(&task_int_23,&callback));

### 执行效率：

这个线程池的效率还没有怎么测试过，目前还没有应用到对性能要求比较高的地方，有时间测试一下。









