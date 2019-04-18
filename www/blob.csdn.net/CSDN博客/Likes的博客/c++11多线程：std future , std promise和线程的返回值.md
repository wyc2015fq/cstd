# c++11多线程：std::future , std::promise和线程的返回值 - Likes的博客 - CSDN博客
2018年11月05日 19:14:26[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：43
[https://blog.csdn.net/lijinqi1987/article/details/78507623](https://blog.csdn.net/lijinqi1987/article/details/78507623)
std::future对象可以和asych，std::packaged_task，std::promise一起使用。这篇文章集中讨论std::future和std::promise。
我们经常会遇到需要得到线程返回结果的情况，现在的问题是我们如何实现。
举个例子：
假设在程序中，我们创建了一个压缩给定文件夹的线程，并且我们希望该线程能够返回新的zip文件的名称和大小。
有两种实现方式：
### 1、老方法：使用指针在线程间共享数据
传递一个指针到新的线程中，该线程将在其中设置数据。直到主线程继续等待使用条件变量。当新线程设置数据并通知条件变量时，主线程将唤醒并从该指针处获取数据。
为了实现这一简单功能，我们使用了一个条件变量、一个mutex锁和一个指针，来实现捕获返回值。
如果我们想要该线程在不同的时间点返回3个不同的值，问题会变得更加复杂，有没有一种简单的方法来从线程处获取返回值呢？
答案是使用std::future
### 2、c++11的方法：使用std::future 和 std::promise
std::future是一个类模板(class template)，其对象存储未来的值，
到底什么是未来的值呢？
事实上，一个std::future对象在内部存储一个将来会被赋值的值，并提供了一个访问该值的机制，通过get()成员函数实现。但如果有人视图在get()函数可用之前通过它来访问相关的值，那么get()函数将会阻塞，直到该值可用。
std::promise也是一个类模板，其对象有可能在将来对值进行赋值，每个std::promise对象有一个对应的std::future对象，一旦由std::promise对象设置，std::future将会对其赋值。
std::promise对象与其管理的std::future对象共享数据。
## 逐步解析
在线程1中创建一个std::promise对象
```cpp
std::promise<int> promiseObj;
```
目前为止，该promise对象没有任何管理的值，但它承诺肯定会有人对其进行赋值，一旦被赋值，就可以通过其管理的std::future对象来获取该值。
但是，假设线程1创建了该promise对象并将其传给线程2，那么线程1怎样知道线程2什么时候会对promise对象进行赋值呢？
答案是使用std::future对象
每个std::promise对象都有个对应的std::future对象，其他人可以通过它来获取promise设置的值。
所以，线程1将会创建std::promise对象，然后在将其传递给线程2之前从它那里获取std::future对象。
```cpp
std::future<int> futureObj = promiseObj.get_future();
```
现在，线程1将promiseObj传递给线程2.
那么线程1将会获取到线程2通过std::future的get函数设置在std::promise中的值，
```cpp
int val = futureObj.get();
```
但是如果线程2还没有对该值进行设置，那么这个调用将会阻塞，直到线程2在promise对象中对该值进行设置。
```cpp
promiseObj.set_value(45);
```
查看下图中的完整流程
看一个完整的std::future和std::promise的例子：
```cpp
#include <iostream>
#include <thread>
#include <future>
 
void initiazer(std::promise<int>* promObj){
    std::cout<<"Inside Thread"<<std::endl;
    promObj->set_value(35);
}
 
int main(){
    std::promise<int> promiseObj;
    std::future<int> futureObj = promiseObj.get_future();
    std::thread th(initiazer, &promiseObj);
    std::cout<<futureObj.get()<<std::endl;
    th.join();
 
    return 0;
}
```
如果std::promise对象在赋值之前被销毁，那么管理的std::future对象上的get()调用将会抛出异常。
除此之外，如果想要线程在不同时间点返回多个值，只需要在线程中传输多个std::promise对象，并从相关的多个std::futur对象中获取多个返回值。
