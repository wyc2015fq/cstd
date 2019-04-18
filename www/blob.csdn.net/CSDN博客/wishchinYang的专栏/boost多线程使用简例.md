# boost多线程使用简例 - wishchinYang的专栏 - CSDN博客
2013年10月20日 14:27:50[wishchin](https://me.csdn.net/wishchin)阅读数：699
原文链接：[http://www.cppblog.com/toMyself/archive/2010/09/22/127347.html](http://www.cppblog.com/toMyself/archive/2010/09/22/127347.html)
## [C++ Boost Thread 编程指南](http://www.cnblogs.com/chengmin/archive/2011/12/29/2306416.html)
### 转自cnblog: [http://www.cnblogs.com/chengmin/archive/2011/12/29/2306416.html](http://www.cnblogs.com/chengmin/archive/2011/12/29/2306416.html)
### [Boost::Thread使用示例](http://blog.csdn.net/zhuxiaoyang2000/article/details/6588031)
转自： [http://blog.csdn.net/zhuxiaoyang2000/article/details/6588031](http://blog.csdn.net/zhuxiaoyang2000/article/details/6588031)
罗素实验室的分析：
转自：  [http://www.rosoo.net/a/200912/8082.html](http://www.rosoo.net/a/200912/8082.html)
boost锁的概述：
原文链接：[http://blog.csdn.net/hbhhww/article/details/7416170](http://blog.csdn.net/hbhhww/article/details/7416170)
Boost多线程编程
背景
•  今天互联网应用服务程序普遍使用多线程来提高与多客户链接时的效率；为了达到最大的吞吐量，事务服务器在单独的线程上运行服务程序；
    GUI应用程序将那些费时，复杂的处理以线程的形式单独运行，以此来保证用户界面能够及时响应用户的操作。这样使用多线程的例子还有很多。
•  跨平台
最近在做一个消息中间件里面涉及到多线程编程，由于跨平台的原因我采用了boost线程库。在创建线程时遇到了几种线程创建方式现总结如下：  
  首先看看boost::thread的构造函数吧，boost::thread有两个构造函数： 
（1）thread()：构造一个表示当前执行线程的线程对象； 
（2）explicit thread(const boost::function0<void>& threadfunc)： 
     boost::function0<void>可以简单看为：一个无返回(返回void)，无参数的函数。这里的函数也可以是类重载operator()构成的函数；该构造函数传入的是函数对象而并非是函数指针，这样一个具有一般函数特性的类也能作为参数传入，在下面有例子。 
第一种方式：最简单方法 
```cpp
#include <boost/thread/thread.hpp> 
#include <iostream> 
  
void hello() 
{ 
        std::cout << 
        "Hello world, I''m a thread!" 
        << std::endl; 
} 
int main(int argc, char* argv[]) 
{ 
        boost::thread thrd(&hello); 
        thrd.join(); 
        return 0; 
}
```
第二种方式：复杂类型对象作为参数来创建线程：
```cpp
#include <boost/thread/thread.hpp> 
#include <boost/thread/mutex.hpp> 
#include <iostream> 
  
boost::mutex io_mutex; 
struct count 
{ 
        count(int id) : id(id) { }       
        void operator()() 
        { 
                for (int i = 0; i < 10; ++i) 
                { 
                        boost::mutex::scoped_lock 
                        lock(io_mutex); 
                        std::cout << id << ": " 
                        << i << std::endl; 
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
第三种方式：在类内部创建线程； 
（1）类内部静态方法启动线程 
```cpp
#include <boost/thread/thread.hpp>
#include <iostream> 
class HelloWorld
{
public:
 static void hello()
 {
      std::cout <<
      "Hello world, I''m a thread!"
      << std::endl;
 }
 static void start()
 {
  
    boost::thread thrd( hello );
    thrd.join();
 }
 
}; 
int main(int argc, char* argv[])
{
    HelloWorld::start();
    return 0;
}
```
在这里start()和hello()方法都必须是static方法。 
（2）如果要求start()和hello()方法不能是静态方法则采用下面的方法创建线程： 
```cpp
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <iostream> 
class HelloWorld
{
public:
 void hello()
 {
    std::cout <<
    "Hello world, I''m a thread!"
    << std::endl;
 }
 void start()
 {
  boost::function0< void> f =  boost::bind(&HelloWorld::hello,this);
  boost::thread thrd( f );
  thrd.join();
 }
 
}; 
int main(int argc, char* argv[])
{
 HelloWorld hello;
 hello.start();
 return 0;
}
```
（3）在Singleton模式内部创建线程： 
```cpp
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <iostream> 
class HelloWorld
{
public:
 void hello()
 {
    std::cout <<
    "Hello world, I''m a thread!"
    << std::endl;
 }
 static void start()
 {
  boost::thread thrd( boost::bind  
                   (&HelloWorld::hello,&HelloWorld::getInstance() ) ) ;
  thrd.join();
 }
 static HelloWorld& getInstance()
 {
  if ( !instance )
      instance = new HelloWorld;
  return *instance;
 }
private: 
 HelloWorld(){}
 static HelloWorld* instance;
 
}; 
HelloWorld* HelloWorld::instance = 0; 
int main(int argc, char* argv[])
{
 HelloWorld::start();
 return 0;
}
```
第四种方法：用类内部函数在类外部创建线程； 
```cpp
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream> 
class HelloWorld
{
public:
 void hello(const std::string& str)
 {
        std::cout <<str<< std::endl;
 }
}; 
  
int main(int argc, char* argv[])
{ 
 HelloWorld obj;
 boost::thread thrd( boost::bind(&HelloWorld::hello,&obj,"Hello 
                               world, I''m a thread!" ) ) ;
 thrd.join();
 return 0;
}
```
如果线程需要绑定的函数有参数则需要使用boost::bind。比如想使用 boost::thread创建一个线程来执行函数：void f(int i)，如果这样写：boost::thread thrd(f)是不对的，因为thread构造函数声明接受的是一个没有参数且返回类型为void的型别，而且不提供参数i的值f也无法运行，这时就可以写：boost::thread thrd(boost::bind(f,1))。涉及到有参函数的绑定问题基本上都是boost::thread、boost::function、boost::bind结合起来使用
