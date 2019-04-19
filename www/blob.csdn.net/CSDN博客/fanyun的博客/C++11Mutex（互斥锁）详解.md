# C++11Mutex（互斥锁）详解 - fanyun的博客 - CSDN博客
2018年02月28日 22:25:37[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4285
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
多个线程访问同一资源时，为了保证数据的一致性，最简单的方式就是使用 mutex（互斥锁）。
（1）.直接操作 mutex，即直接调用 mutex 的 lock / unlock 函数。此例顺带使用了 boost::thread_group 来创建一组线程。
```cpp
#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
boost::mutex mutex;
int count = 0;
void Counter() {
  mutex.lock();
  int i = ++count;
  std::cout << "count == " << i << std::endl;
  // 前面代码如有异常，unlock 就调不到了。
  mutex.unlock();
}
int main() {
  // 创建一组线程。
  boost::thread_group threads;
  for (int i = 0; i < 4; ++i) {
    threads.create_thread(&Counter);
  }
  // 等待所有线程结束。
  threads.join_all();
  return 0;
}
```
（2）.使用 lock_guard 自动加锁、解锁。原理是 RAII，和智能指针类似
```cpp
#include <iostream>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
boost::mutex mutex;
int count = 0;
void Counter() {
  // lock_guard 在构造函数里加锁，在析构函数里解锁。
  boost::lock_guard<boost::mutex> lock(mutex);
  int i = ++count;
  std::cout << "count == " << i << std::endl;
}
int main() {
  boost::thread_group threads;
  for (int i = 0; i < 4; ++i) {
    threads.create_thread(&Counter);
  }
  threads.join_all();
  return 0;
}
```
（3）.使用 unique_lock 自动加锁、解锁。
unique_lock 与 lock_guard 原理相同，但是提供了更多功能（比如可以结合条件变量使用）。注意：mutex::scoped_lock 其实就是 unique_lock<mutex> 的 typedef。
```cpp
#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
boost::mutex mutex;
int count = 0;
void Counter() {
  boost::unique_lock<boost::mutex> lock(mutex);
  int i = ++count;
  std::cout << "count == " << i << std::endl;
}
int main() {
  boost::thread_group threads;
  for (int i = 0; i < 4; ++i) {
    threads.create_thread(&Counter);
  }
  threads.join_all();
  return 0;
}
```
