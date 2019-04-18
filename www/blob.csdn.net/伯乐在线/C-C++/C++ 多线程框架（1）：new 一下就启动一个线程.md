# C++ 多线程框架（1）：new 一下就启动一个线程 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [吳YH堅](http://www.jobbole.com/members/wuyinghao) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
几年前写过一个C++的多线程框架，虽然写完了，但是人一懒做了一次说明以后就没影了，最近把代码整理了一下，准备发到github上，在这里，再把这个框架总结一下吧。
多线程一直是编程中常见的问题，特别是在Linux的c++上，多线程的封装一直不是很好，当然，有很多第三方库可以用，比如boost之类的，但是我们有时候并不需要那么庞大的库，只需要一个轻量级的线程框架就行了，于是自己编了一个，目前只在Linux下用了，但是设计的时候是按照多平台来编的，如果你有需要，可以自己添加一些个类，把他变成一个windows平台获取其他平台的，比如eCos,Vxworks等等。。
对于多线程，我们需要的是把操作系统底层封装起来，让用户编写程序的时候更多的关注他的代码逻辑而不是线程之间的逻辑，最好是new一个类以后，就启动了一个线程，线程之间的通信也有相应的类封装起来，只要调用就行了。
根据这些，我们定义了一组基类，来封装各种多线程的接口
操作系统基类，该类主要定义了createThread函数来创建线程，该函数是纯虚函数，继承自它的类需要根据平台实现其功能


```
class COperatingSystem  
{  
    public:  
        COperatingSystem();  
        ~COperatingSystem();  
  
        virtual  bool createThread(CThread *mThread,unsigned long stack_size=8*1024)=0;  
        virtual void  sleepSec(unsigned long sec)=0;  
  
    protected:  
        CThread     *p_thread;  
};
```
线程基类，定义了threadEntry来作为线程的入口，initializeThread来初始化线程，子类可以初始化不同的成员变量，mainLoop是纯虚函数，为线程的主函数，一般是一个while循环，子类必须实现该虚函数。


```
class CThread  
{  
    public:  
        CThread(const char *m_thread_name);  
        ~CThread();  
         void threadEntry(CCountingSem *pSemaphore);  
  
    protected:  
        virtual bool initializeThread();  
        virtual void mainLoop()=0;  
  
        COperatingSystem        *p_opration_system;  
        char        *p_thread_name;  
  
};
```
为了平台的无关性，使用了简单工厂模式，用来根据不同的平台返回不同的操作系统类，信号量类和互斥类。


```
class COperatingSystemFactory  
{  
    public:  
        static COperatingSystem *newOperatingSystem();  
        static CCountingSem  *newCountingSem(unsigned int init);  
        static CMutex           *newMutex(const char *pName=NULL);  
  
};
```
信号量基类，纯虚函数定义了get和post信号量方法，子类必须根据系统类型进行不同的实现


```
class CCountingSem  
{  
    public:  
        CCountingSem();  
        ~CCountingSem();  
         virtual bool                Get(Mode mode = kForever, unsigned long timeoutMS = 0) = 0;  
             virtual bool                Post(void) = 0;  
  
};
```
互斥基类，纯虚函数定义了lock和unlock两个方法，同样，子类必须根据系统类型进行不同的实现


```
class CMutex  
{  
    public:  
        CMutex(const char *pName = NULL);  
        ~CMutex();  
        virtual bool Lock()=0;  
        virtual bool UnLock()=0;  
  
    protected:  
        char       *mutex_name;  
};
```
还有一个重头是msgQueue类，下次说。
有了这些个基础类以后，我们就可以开始了。
我们希望的结果是
用户，也就是程序员啦，从CThread继承一个自己的线程类，比如CTestThread，然后实现mainLoop方法，这样，一个不考虑通信的线程就编写完了，然后我只需要在main.cpp中new一下这个CTestThread，那么线程就启动了，没有其他任何繁琐的操作。
要实现这样的功能，上面那些个类需要怎么样的组合调用呢？
首先，因为是在Linux下，所以所有的基类都要派生出Linux对应的子类（CThread不需要，因为是用户编写的，COperatingSystemFactory也不需要，因为是抽象工厂），所以，我们在Linux下创建了CLinuxMutex,CLinuxOperratingSystem,CLinuxCountingSem三个子类，并且在这些个子类中实现了基类中的纯虚函数。
接着，我们new 一个 CTestThread 后，需要通过COperatingSystemFactory的newOperatingSystem 生成一个CLinuxOperratingSystem，然后CLinuxOperratingSystem调用createThread产生一个线程函数，然后把CTestThread的mainLoop绑定到这个线程函数中。
对，就这么简单
在github中下载了所有的文件以后，你只需要编写你自己的线程类，如：


```
class TestThread:public CThread  
{  
    public:  
        TestThread(const char *m_name);  
        ~TestThread();  
        virtual void mainLoop();  
};  
//然后实现mainLoop方法：  
void TestThread::mainLoop()  
{  
    while(1)  
        {  
            printf("%s :hello world\n",p_thread_name);  
              
        }  
}
```
然后在main.cpp中，调用一句话new一下这个类：


```
TestThread *a=new TestThread("Thread A");
```
OK，一切搞定，现在运行，就能不停的打出hello world了。
同样，你也可以new多个实例
如果想要其他功能的线程，你再从CThread派生一个其他类就行了，很简单吧。
稍微复杂一点的是线程通信，下次说。
代码还没有整理完成，等整理完成了一并传到github上，大概还需要两三天时间吧。
github地址：
https://github.com/wyh267/Cplusplus_Thread_Lib
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/09/db956e69f586b9e6b487bc448ef34dcb.jpeg)
