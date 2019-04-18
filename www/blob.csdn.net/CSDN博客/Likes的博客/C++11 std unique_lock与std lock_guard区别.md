# C++11 std::unique_lock与std::lock_guard区别 - Likes的博客 - CSDN博客
2018年12月07日 14:41:22[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：13标签：[thread																[unique_lock																[lock_guard																[condition_variable																[wait](https://so.csdn.net/so/search/s.do?q=wait&t=blog)](https://so.csdn.net/so/search/s.do?q=condition_variable&t=blog)](https://so.csdn.net/so/search/s.do?q=lock_guard&t=blog)](https://so.csdn.net/so/search/s.do?q=unique_lock&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)
个人分类：[C++](https://blog.csdn.net/songchuwang1868/article/category/7898933)
C++多线程编程中通常会对共享的数据进行写保护，以防止多线程在对共享数据成员进行读写时造成资源争抢导致程序出现未定义的行为。通常的做法是在修改共享数据成员的时候进行加锁--mutex。在使用锁的时候通常是在对共享数据进行修改之前进行lock操作，在写完之后再进行unlock操作，进场会出现由于疏忽导致由于lock之后在离开共享成员操作区域时忘记unlock，导致死锁。
针对以上的问题，C++11中引入了std::unique_lock与std::lock_guard两种数据结构。通过对lock和unlock进行一次薄的封装，实现自动unlock的功能。
```cpp
std::mutex mut;
 
void insert_data()
{
       std::lock_guard<std::mutex> lk(mut);
       queue.push_back(data);
}
 
void process_data()
{
       std::unqiue_lock<std::mutex> lk(mut);
       queue.pop();
}
```
std::unique_lock 与std::lock_guard都能实现自动加锁与解锁功能，但是std::unique_lock要比std::lock_guard更灵活，但是更灵活的代价是占用空间相对更大一点且相对更慢一点。
此外需要强调一点，**condition_variable中的wait只接收unique_lock**
通过实现一个线程安全的队列来说明两者之间的差别。
```cpp
template <typename T>
class ThreadSafeQueue{
public:
         void Insert(T value);
         void Popup(T &value);
         bool Empety();
 
private:
       mutable std::mutex mut_;
       std::queue<T> que_;
       std::condition_variable cond_;
};
template <typename T>
void ThreadSafeQueue::Insert(T value){
    std::lock_guard<std::mutex> lk(mut_);
    que_.push_back(value);
    cond_.notify_one();
}
 
 
template <typename T>
void ThreadSafeQueue::Popup(T &value){
    std::unique_lock<std::mutex> lk(mut_);
    cond_.wait(lk, [this]{return !que_.empety();});
    value = que_.front();
    que_.pop();
}
 
 
template <typename T>
bool ThreadSafeQueue::Empty() const{
        std::lock_guard<std::mutex> lk(mut_);
        return que_.empty();
}
```
上面代码只实现了关键的几个函数，并使用了C++11新引入的condition_variable条件变量。从Popup与Inert两个函数看std::unique_lock相对std::lock_guard更灵活的地方在于在等待中的线程如果在等待期间需要解锁mutex，并在之后重新将其锁定。而std::lock_guard却不具备这样的功能。
上面代码中
```cpp
cond_.wait(lk, [this]{return !Empety();});
```
可能会比较难以理解，
```cpp
[this]{return !Empety();}
```
是C++11新引入的功能，lambda表达式，是一种匿名函数。方括号内表示捕获变量。
- 当lambda表达式返回true时（即queue不为空），本质上不会进入wait阻塞
- 当lambda表达式返回false时，进入wait阻塞。
```cpp
//wait(lock,pred)等价于
while (!pred()) {
    wait(lock);
}
```
还存在另一种读写锁，但是并没有引入C++11，但是boost库提供了对应的实现。读写锁主要适合在于共享数据更新频率较低，但是读取共享数据频率较高的场合。
