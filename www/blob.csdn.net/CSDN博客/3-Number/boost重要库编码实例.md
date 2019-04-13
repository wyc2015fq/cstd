
# boost重要库编码实例 - 3-Number - CSDN博客


2017年12月12日 10:15:34[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：345


### 0x01 缘由
最近几周没有再继续学习新的知识点，而是准备一些面试题。大家懂的。。。。

### 0x02 boost::shared_ptr 智能指针
介绍请看另外一篇博文：[http://blog.csdn.net/pangyemeng/article/details/72842566](http://blog.csdn.net/pangyemeng/article/details/72842566)
example1：
```python
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>
class Shared
{
    public:
        Shared()
        {
            std::cout << "ctor() called" << std::endl;
        }
        Shared(const Shared & other)
        {
            std::cout << "copy ctor() called" << std::endl;
        }
        ~Shared()
        {
            std::cout << "dtor() called" << std::endl;
        }
        Shared & operator =(const Shared & other)
        {
            std::cout << "operator = called" << std::endl;
        }
};
int main()
{
    typedef boost::shared_ptr<Shared> SharedSP;
    typedef std::vector<SharedSP> VShared;
    VShared v;
    v.push_back(SharedSP(new Shared()));
    v.push_back(SharedSP(new Shared()));
}
```
**编译：**g++ -o test2 test2.cpp
**运行结果：**
[root@Huawei test]\# ./test2
ctor() called
ctor() called
dtor() called
dtor() called
### 0x03 boost::any 特殊容器
介绍请看另一篇文章：[http://blog.csdn.net/pangyemeng/article/details/72896889](http://blog.csdn.net/pangyemeng/article/details/72896889)
example2:
```python
#include <boost/any.hpp>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
using namespace boost;
void test_any()
{
    typedef vector<boost::any> many;
    many a;
    a.push_back(2.3);
    a.push_back(string("test"));
    for (unsigned int i = 0; i < a.size(); ++i)
    {
        cout << a[i].type().name() << endl;
        try
        {
            int result = any_cast<int> (a[i]);
            cout << result << endl;
        }
        catch (boost::bad_any_cast & ex)
        {
            cout << "cast error:" << ex.what() << endl;
        }
    }
}
int main()
{
    test_any();
    return 0;
}
```
**编译：**g++ -o test4 test4.cpp
**运行结果：**
[root@Huawei test]\# ./test4
d
cast error:boost::bad_any_cast: failed conversion using boost::any_cast
Ss
cast error:boost::bad_any_cast: failed conversion using boost::any_cast
### 0x04 boost::thread 线程库
boost::thread类代表一个执行线程(a thread of execution)，就像std::fstream类代表一个文件一样。缺省构造函数创建一个代表当前执行线程的一个实例。重载构造函数则有一个函数对象作为参数，该函数对象没有实参(argument)，也无返回值。重载的构造函数创建一个新的线程，然后调用函数对象。
由于Boost.Threads采用了函数对象，而不是函数指针，因此函数对象携带线程要用到的数据是完全可以的。这种方法更加灵活，而且类型安全。如果再和相关的函数库结合起来，比如Boost.Bind，这种方式可以让你轻松地将任意多的数据传递给新创建的线程。
example3:
```python
#include <boost/thread/thread.hpp>
#include <stdio.h>
#include <iostream>
void hello()
{
    std::cout << "Hello world, I'm a thread!" << std::endl;
}
int main()
{
    boost::thread thrd(&hello);
    thrd.join();
    return 0;
}
```
编译： g++ -o test5 test5.cpp -lpthread -lboost_thread
运行结果：
[root@Huawei test]\# ./test5
Hello world, I'm a thread!
有线程就要想到线程同步：
Boost.Threads支持两大类型的mutex：简单mutex和递归mutex。
一个线程有3种可能方法来锁定mutex：
1. 等待并试图对mutex加锁，直到没有其他线程锁定mutex；
2. 试图对mutex加锁，并立即返回，如果其他线程锁定了mutex；
3. 等待并试图对mutex加锁，直到没有其他线程锁定mutex或者直到规定的时间已过。
Boost.Threads允许你挑选最有效率的mutex。为此，Boost.Threads提供了6中类型的mutex，效率由高到低排列：boost::mutex，boost::try_mutex，boost::timed_mutex，boost::recursive_mutex，boost::recursive_try_mutex和boost::recursive_timed_mutex。
Boost.Threads提供Scoped Lock模式，防止死鎖。要构建一个这种类型的锁，需要传递一个mutex引用，构造函数将锁定mutex，析构函数将解锁mutex
example4:
```python
#include <boost/thread/thread.hpp>
#include <stdio.h>
#include <iostream>
boost::mutex io_mutex;
struct count
{
        count(int id) :
            id(id)
        {
        }
        void operator()()
        {
            for (int i = 0; i < 10; ++i)
            {
                boost::mutex::scoped_lock lock(io_mutex);
                std::cout << id << ": " << i << std::endl;
            }
        }
        int id;
};
int main(int argc, char* argv[])
{
    boost::thread thrd1(count(1));
    boost::thread thrd2(count(2));
    thrd1.join();
    thrd2.join();
    return 0;
}
```
**编译：**g++ -o test6 test6.cpp -lpthread -lboost_thread
**运行结果：**
[root@Huawei test]\# ./test6
2: 0
2: 1
1: 0
1: 1
1: 2
1: 3
1: 4
1: 5
1: 6
1: 7
1: 8
1: 9
2: 2
2: 3
2: 4
2: 5
2: 6
2: 7
2: 8
2: 9
### 0x05 boost::bind boost::function
介绍请看另外一篇博文：[http://blog.csdn.net/pangyemeng/article/details/72865413](http://blog.csdn.net/pangyemeng/article/details/72865413)
example5:
```python
#include <boost/thread/thread.hpp>
#include <stdio.h>
#include <iostream>
boost::mutex io_mutex;
void count(int id)
{
    for (int i = 0; i < 10; ++i)
    {
        boost::mutex::scoped_lock lock(io_mutex);
        std::cout << id << ": " << i << std::endl;
    }
}
int main(int argc, char* argv[])
{
    boost::thread thrd1(boost::bind(&count, 1)); // 有无&符号均可
    boost::thread thrd2(boost::bind(&count, 2)); // 有无&符号均可
    thrd1.join();
    thrd2.join();
    return 0;
}
```
编译： g++ -o test7 test7.cpp -lpthread -lboost_thread
运行结果：
[root@Huawei test]\# ./test7
1: 0
1: 1
1: 2
1: 3
1: 4
1: 5
1: 6
1: 7
1: 8
1: 9
2: 0
2: 1
2: 2
2: 3
2: 4
2: 5
2: 6
2: 7
2: 8
### 2: 9
### 0x06 总结
对于boost的库的实践，在ise框架中使用了其他boost库，并发编程库中的特点，在c++11中也有类似做法。

