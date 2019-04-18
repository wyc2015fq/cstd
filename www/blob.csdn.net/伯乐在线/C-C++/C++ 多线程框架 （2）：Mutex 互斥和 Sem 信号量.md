# C++ 多线程框架 （2）：Mutex 互斥和 Sem 信号量 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [吳YH堅](http://www.jobbole.com/members/wuyinghao) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
互斥和信号量是多线程编程的两个基础，其原理就不详细说了，大家去看看操作系统的书或者网上查查吧。
对于互斥的实现，无论什么操作系统都离不开三个步骤
1.初始化互斥锁
2.锁操作
3.解锁操作
对于不同的系统只是实现的函数有一些不同而已，但是功能其实都大同小异，在锁操作和解锁操作的时候大部分系统都有超时机制在里面，来保证不会一直锁在某个地方，我们为了框架简单，没有设置超时，进行锁操作的时候如果得不到锁，将一直等待在那里。
Mutex的基类我们描述如下


```
class CMutex  
{  
    public:  
        CMutex(const char *pName = NULL);  //初始化锁  
        ~CMutex();  
        virtual bool Lock()=0;       //锁操作，纯虚函数  
        virtual bool UnLock()=0;  //解锁操作，纯虚函数  
        const char * getName(void) const {  
                return mutex_name;  
            }  
    protected:  
        char       *mutex_name;  //锁名字  
};
```
对于每个系统的实现，都需要完成初始化，锁操作，解锁操作三个部分，在linux下，这三个操作都很简单，就不在这里贴代码了。
同样，对于信号量Sem，每个系统的实现也大同小异，无非就是
1.初始化信号量
2.发送信号量（信号量+1）
3.接收信号量（信号量-1）
Sem基类描述如下


```
class CCountingSem  
{  
    public:  
        typedef enum {  
                 kTimeout,  
             kForever  
            }Mode;                       
        CCountingSem();     //初始化信号量  
        ~CCountingSem();  
         virtual bool                Get() = 0;  //接收信号量  
             virtual bool                Post(void) = 0;   //发送信号量  
};
```
同样，具体实现就不贴代码了。
当然，对于一个满足设计模式的系统，新建互斥锁和信号量的时候当然不能直接new这些类啦，必然还要通过简单工程来返回，在COperatingSystemFactory类中添加newMutex和newCountingSem方法，通过COperatingSystemFactory对操作系统的判断来返回相应的实体。


```
class COperatingSystemFactory  
{  
    public:  
        static COperatingSystem *newOperatingSystem();  
        static CCountingSem  *newCountingSem(unsigned int init=0); //参数是信号量的初始值，一般为0  
        static CMutex           *newMutex(const char *pName=NULL);  
};
```
好了，有了互斥和信号量，怎么用呢，在main函数中，我们可以先申请好互斥锁和信号量，如果我们启了很多线程，如果某几个之间需要互斥锁，那我们将申请好的互斥锁赋值给相应的线程，就可以直接使用了。至于各个线程类，是你自己写的，只是继承自CThread而已，里面的成员变量怎么和main中申请的互斥锁关联，这就不用说了吧，你把它设置成public赋值也行，设置从private用函数set也行，一切看你啦。
有了互斥锁和信号量，下面就可以起消息队列了，有了消息队列，一个最简单的多线程模型也就完成了。
github地址：
https://github.com/wyh267/Cplusplus_Thread_Lib
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/09/db956e69f586b9e6b487bc448ef34dcb.jpeg)
