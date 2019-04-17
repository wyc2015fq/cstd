# Windows平台下的读写锁 - tifentan的专栏 - CSDN博客





2018年01月02日 09:45:36[露蛇](https://me.csdn.net/tifentan)阅读数：511








[Windows平台下的读写锁](http://blog.sina.com.cn/s/blog_aec526470101djux.html#cmt_59BB3DD9-73EC5B12-74BC36B8-8F3-88B)

简单介绍Windows平台下的读写锁以及实现.

背景介绍 

Windows在Vista 和 Server2008以后才开始提供读写锁API,即SRW系列函数(InitializeSRWLock, AcquireSRWLockShared, AcquireSRWLockExclusive等).考虑到目前Windows XP的装机量,只能自己实现一个读写锁了.

读写锁的目的和要求 

读写锁的最基本目的是读锁可以共享,写锁必须独占.另外,我认为还有两点需要特别考虑: 

1. 如果有写锁请求在等待,则新的读锁请求也应该等待.否则程序可能永远也没有机会获得写锁. 

2. 一旦写锁被释放,所有在等待中的读锁和写锁请求应该可以公平竞争,而不管请求的先后,只要之前已经在等待就应该有获得锁的机会. 

如果说第一点还可以再斟酌一下的话,第二点应该是必须要保证的.
读写锁的实现 

总体思路比较简单:读写锁内部维护一些状态值,用临界端 CRITICAL_SECTION保护这些状态值访问的原子性和事件对象配合实现等待.

进入锁的情况:

1.如果当前锁状态为空闲,则不管写锁还是读锁请求都允许进入,并设置相应的状态值. 

2.如果当前锁状态是读,新的写锁请求需要等待事件通知,并把写锁等待计数加一. 

3.如果当前锁状态是读,新的读锁请求: 

3.1 如果没有写锁请求在等待,则允许读锁进入,并把读锁计数加一. 

3.2 如果有写锁请求正在等待,则等待事件通知,并读锁等待计数加一(这样做的目的如上文所述,要使写锁有机会进入). 

4.如果当前锁状态为写,则不管读锁请求还是写锁请求,都等待事件通知并分别给读锁等待计数或者写锁等待计数加一.
解锁的情况: 

如果锁释放时,读锁等待计数或者写锁等待计数不为0,则触发事件对象.

我使用手动事件对象,这样的话一旦锁被释放,所有正在等待的锁请求都将被激活,然后重新以竞争临界段的方式竞争锁进入权以保证公平性.不管等待请求时的先后,只要是锁释放前进入等待状态则锁一旦释放获得进入权的机会是均等的.

后记 

我在实现读写锁之前,用Google搜索过,找到的几种Windows读写锁实现都不甚理想,主要问题如下: 

1. 等待事件对象时占有了互斥量,这是一个非常不好的设计,这样的话其他锁请求将被阻塞在临界段外部,只有一个锁请求阻塞在Wait函数,对其他锁请求是不公平的. 

2. 没考虑我开始提的目的2,即写锁可能永远都没机会进入.我不知道这个算不算缺点,但是自少在我的应用中,这种情况是不允许出现的,所以我重新设计过一个读写锁.
实现过程中一个让我很纠结的问题是:到底用自动事件还是手动事件. 用手动事件的好处是一旦触发,所有等待中的请求都被激活然后重新竞争,逻辑简单明了.缺点是可能会有很多冗余操作,比如有若干写锁还若干读锁请求正在等待进入,一旦锁释放,虽然全部请求(线程)都被激活,但是肯定只有一个请求能够进入,竞争失败的请求测试一下条件后继续挂起.如果使用自动事件,只有一个锁请求线程会被唤醒(Wait函数的特点,被唤醒的那个线程等同于已经竞争成功),似乎效率更高一些.获得进入权的锁请求再根据等待的情况决定是否继续触发事件对象:如果还有读请求和写请求在等待,则不触发;如果只有读请求在等待,则再触发一次以使其他读请求可以进入.考虑再三,我还是决定采用手动事件,毕竟在读锁的数量远大于写锁数量的情况下(这也是读写锁比较常见的场景)速度更快一些(不需要等待多次事件).

附录1 - C++代码

[cpp] view plaincopyprint?

```
#define RWLOCK_IDLE 0    
#define RWLOCK_R 0x01    
#define RWLOCK_W 0x02    

class RWLock  
{  
private:  
    int _st;   
    int _rlockCount;   
    int _rwaitingCount;   
    int _wwaitingCount;   
    HANDLE _ev;   
    //HANDLE _stLock;     
    CRITICAL_SECTION _stLock;  

public:  
    RWLock(void);  
    ~RWLock(void);  
    void rlock();  
    void wlock();  
    void unlock();  
};  

RWLock::RWLock(void)  
    : _rlockCount(0),  
    _st(RWLOCK_IDLE),  
    _rwaitingCount(0),  
    _wwaitingCount(0)  
{  
    //_stLock = CreateMutex(NULL, FALSE, NULL);   
    //assert(_stLock != INVALID_HANDLE_VALUE);   
    InitializeCriticalSection(&_stLock);  


    _ev = CreateEvent(NULL, TRUE, FALSE, NULL);  
    assert(_ev != INVALID_HANDLE_VALUE);  
}  


RWLock::~RWLock(void)  
{  
    //CloseHandle(_stLock);   
    DeleteCriticalSection(&_stLock);  
    CloseHandle(_ev);  
}  

void RWLock::rlock()  
{  
    bool isWaitReturn = false;  
    while(1)  
    {  
        //WaitForSingleObject(_stLock, INFINITE);   
        EnterCriticalSection(&_stLock);  
        if(isWaitReturn)  
        {  

            --_rwaitingCount;  
        }  

        if(_st == RWLOCK_IDLE)  
        {  

            _st = RWLOCK_R;  
            _rlockCount++;  
            //ReleaseMutex(_stLock);   
            LeaveCriticalSection(&_stLock);  
            break;  
        }  
        else if( _st == RWLOCK_R)  
        {  
            if(_wwaitingCount > 0)  
            {  

                ++_rwaitingCount;  
                ResetEvent(_ev);  
                //SignalObjectAndWait(_stLock, _ev, INFINITE, FALSE);   
                LeaveCriticalSection(&_stLock);  


                WaitForSingleObject(_ev, INFINITE);  


                isWaitReturn = true;  
            }  
            else  
            {     

                ++_rlockCount;  
                //ReleaseMutex(_stLock);   
                LeaveCriticalSection(&_stLock);  
                break;  
            }  
        }  
        else if(_st == RWLOCK_W)  
        {  

            ++_rwaitingCount;  
            ResetEvent(_ev);  
            //SignalObjectAndWait(_stLock, _ev, INFINITE, FALSE);   
            LeaveCriticalSection(&_stLock);  
            WaitForSingleObject(_ev, INFINITE);  


            isWaitReturn = true;  
        }  
        else  
        {  
            assert(0);  
            break;  
        }  
    }  
}  

void RWLock::wlock()  
{  
    bool isWaitReturn = false;  

    while(1)  
    {  
        //WaitForSingleObject(_stLock, INFINITE);   
        EnterCriticalSection(&_stLock);  

        if(isWaitReturn) --_wwaitingCount;  

        if(_st == RWLOCK_IDLE)  
        {  
            _st = RWLOCK_W;  
            //ReleaseMutex(_stLock);   
            LeaveCriticalSection(&_stLock);  
            break;  
        }  
        else  
        {  
            ++_wwaitingCount;  
            ResetEvent(_ev);  
            //SignalObjectAndWait(_stLock, _ev, INFINITE, FALSE);   
            LeaveCriticalSection(&_stLock);  
            WaitForSingleObject(_ev, INFINITE);  

            isWaitReturn = true;  
        }  
    }  
}  

void RWLock::unlock()  
{  
    //WaitForSingleObject(_stLock, INFINITE);   
    EnterCriticalSection(&_stLock);  
    if(_rlockCount > 0)  
    {  

        --_rlockCount;  

        if( 0 == _rlockCount)  
        {  
            _st = RWLOCK_IDLE;  


            if( _wwaitingCount > 0 || _rwaitingCount > 0 )  
            {  

                SetEvent(_ev);  
            }  
            else  
            {  

            }  
        }  
        else  
        {  

        }  
    }  
    else  
    {  
        _st = RWLOCK_IDLE;  


        if( _wwaitingCount > 0 || _rwaitingCount > 0 )  
        {  

            SetEvent(_ev);  
        }  
        else  
        {  

        }  
    }  
    //ReleaseMutex(_stLock);   
    LeaveCriticalSection(&_stLock);  
}
```






