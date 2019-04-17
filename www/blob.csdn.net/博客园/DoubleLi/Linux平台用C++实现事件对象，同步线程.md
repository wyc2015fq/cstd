# Linux平台用C++实现事件对象，同步线程 - DoubleLi - 博客园






前文在Win32平台上用C++实现了事件对象Event，对线程进行同步，以达到期望目的。这次在[Linux](http://lib.csdn.net/base/linux)平台上实现与之类似的事件对象。与其相关的一组API包括：pthread_mutex_init，pthread_cond_init，pthread_mutex_lock，pthread_cond_wait，pthread_mutex_unlock，pthread_cond_broadcast，pthread_cond_timedwait，pthread_cond_destroy，pthread_mutex_destroy。这些API的说明可以在这里找到：[http://www.9linux.com/](http://www.9linux.com/)。下边，是封装的事件对象类，以及[测试](http://lib.csdn.net/base/softwaretest)代码。使用VS2005编辑，在虚拟机 Fedora 13中编译，测试通过。

MyEvent.h



**[cpp]**[view plain](http://blog.csdn.net/chexlong/article/details/7080537#)[copy](http://blog.csdn.net/chexlong/article/details/7080537#)



- #ifndef My_Event_Header  
- #define My_Event_Header  
- 
- #include <iostream>  
- #include <pthread.h>  
- #include <errno.h>  
- 
- using namespace std;  
- 
- //---------------------------------------------------------------  
- 
- class CEventImpl  
- {  
- protected:  
- 
- /* 
-      动态方式初始化互斥锁,初始化状态变量m_cond 
-     `bAutoReset  true   人工重置 
-                  false  自动重置 
-     */  
-     CEventImpl(bool manualReset);         
- 
- /* 
-      注销互斥锁,注销状态变量m_cond 
-     */  
-     ~CEventImpl();  
- 
- /* 
-      将当前事件对象设置为有信号状态 
-      若自动重置，则等待该事件对象的所有线程只有一个可被调度 
-      若人工重置，则等待该事件对象的所有线程变为可被调度 
-     */  
- void SetImpl();  
- 
- /* 
-      以当前事件对象，阻塞线程，将其永远挂起 
-      直到事件对象被设置为有信号状态 
-     */  
- bool WaitImpl();  
- 
- /* 
-      以当前事件对象，阻塞线程，将其挂起指定时间间隔 
-      之后线程自动恢复可调度 
-     */  
- bool WaitImpl(long milliseconds);  
- 
- /* 
-      将当前事件对象设置为无信号状态 
-     */  
- void ResetImpl();  
- 
- private:  
- bool            m_manual;  
- volatile bool   m_state;  
-     pthread_mutex_t m_mutex;  
-     pthread_cond_t  m_cond;  
- };  
- 
- inline void CEventImpl::SetImpl()  
- {  
- if (pthread_mutex_lock(&m_mutex))     
-         cout<<"cannot signal event (lock)"<<endl;  
- 
- //设置状态变量为true，对应有信号  
-     m_state = true;  
- 
- //cout<<"CEventImpl::SetImpl m_state = "<<m_state<<endl;  
- 
- //重新激活所有在等待m_cond变量的线程  
- if (pthread_cond_broadcast(&m_cond))  
-     {  
-         pthread_mutex_unlock(&m_mutex);  
-         cout<<"cannot signal event"<<endl;  
-     }  
-     pthread_mutex_unlock(&m_mutex);  
- }  
- 
- inline void CEventImpl::ResetImpl()  
- {  
- if (pthread_mutex_lock(&m_mutex))     
-         cout<<"cannot reset event"<<endl;  
- 
- //设置状态变量为false，对应无信号  
-     m_state = false;  
- 
- //cout<<"CEventImpl::ResetImpl m_state = "<<m_state<<endl;  
- 
-     pthread_mutex_unlock(&m_mutex);  
- }  
- 
- //---------------------------------------------------------------  
- 
- class CMyEvent: private CEventImpl  
- {  
- public:  
-     CMyEvent(bool bManualReset = true);  
-     ~CMyEvent();  
- 
- void Set();  
- bool Wait();  
- bool Wait(long milliseconds);  
- bool TryWait(long milliseconds);  
- void Reset();  
- 
- private:  
-     CMyEvent(const CMyEvent&);  
-     CMyEvent& operator = (const CMyEvent&);  
- };  
- 
- 
- inline void CMyEvent::Set()  
- {  
-     SetImpl();  
- }  
- 
- inline bool CMyEvent::Wait()  
- {  
- return WaitImpl();  
- }  
- 
- inline bool CMyEvent::Wait(long milliseconds)  
- {  
- if (!WaitImpl(milliseconds))  
-     {  
-         cout<<"time out"<<endl;  
- return false;  
-     }  
- else  
-     {  
- return true;  
-     }  
- }  
- 
- inline bool CMyEvent::TryWait(long milliseconds)  
- {  
- return WaitImpl(milliseconds);  
- }  
- 
- inline void CMyEvent::Reset()  
- {  
-     ResetImpl();  
- }  
- 
- #endif  



MyEvent.cpp



**[cpp]**[view plain](http://blog.csdn.net/chexlong/article/details/7080537#)[copy](http://blog.csdn.net/chexlong/article/details/7080537#)



- #include "MyEvent.h"  
- #include <sys/time.h>  
- 
- CEventImpl::CEventImpl(bool manualReset): m_manual(manualReset), m_state(false)  
- {  
- if (pthread_mutex_init(&m_mutex, NULL))  
-         cout<<"cannot create event (mutex)"<<endl;  
- if (pthread_cond_init(&m_cond, NULL))  
-         cout<<"cannot create event (condition)"<<endl;  
- }  
- 
- CEventImpl::~CEventImpl()  
- {  
-     pthread_cond_destroy(&m_cond);  
-     pthread_mutex_destroy(&m_mutex);  
- }  
- 
- bool CEventImpl::WaitImpl()  
- {  
- if (pthread_mutex_lock(&m_mutex))  
-     {  
-         cout<<"wait for event failed (lock)"<<endl;   
- return false;  
-     }  
- while (!m_state)   
-     {  
- //cout<<"CEventImpl::WaitImpl while m_state = "<<m_state<<endl;  
- 
- //对互斥体进行原子的解锁工作,然后等待状态信号  
- if (pthread_cond_wait(&m_cond, &m_mutex))  
-         {  
-             pthread_mutex_unlock(&m_mutex);  
-             cout<<"wait for event failed"<<endl;  
- return false;  
-         }  
-     }  
- if (m_manual)  
-         m_state = false;  
-     pthread_mutex_unlock(&m_mutex);  
- 
- //cout<<"CEventImpl::WaitImpl end m_state = "<<m_state<<endl;  
- 
- return true;  
- }  
- 
- bool CEventImpl::WaitImpl(long milliseconds)  
- {  
- int rc = 0;  
- struct timespec abstime;  
- struct timeval tv;  
-     gettimeofday(&tv, NULL);  
-     abstime.tv_sec  = tv.tv_sec + milliseconds / 1000;  
-     abstime.tv_nsec = tv.tv_usec*1000 + (milliseconds % 1000)*1000000;  
- if (abstime.tv_nsec >= 1000000000)  
-     {  
-         abstime.tv_nsec -= 1000000000;  
-         abstime.tv_sec++;  
-     }  
- 
- if (pthread_mutex_lock(&m_mutex) != 0)  
-     {  
-         cout<<"wait for event failed (lock)"<<endl;   
- return false;  
-     }  
- while (!m_state)   
-     {  
- //自动释放互斥体并且等待m_cond状态,并且限制了最大的等待时间  
- if ((rc = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime)))  
-         {  
- if (rc == ETIMEDOUT) break;  
-             pthread_mutex_unlock(&m_mutex);  
-             cout<<"cannot wait for event"<<endl;  
- return false;  
-         }  
-     }  
- if (rc == 0 && m_manual)   
-         m_state = false;  
-     pthread_mutex_unlock(&m_mutex);  
- return rc == 0;  
- }  
- 
- CMyEvent::CMyEvent(bool bManualReset): CEventImpl(bManualReset)  
- {  
- }  
- 
- CMyEvent::~CMyEvent()  
- {  
- }  



    下边是测试代码



**[cpp]**[view plain](http://blog.csdn.net/chexlong/article/details/7080537#)[copy](http://blog.csdn.net/chexlong/article/details/7080537#)



- // pthread_event.cpp : 定义控制台应用程序的入口点。  
- //  
- 
- #include <unistd.h>  
- #include "MyEvent.h"  
- 
- #define PRINT_TIMES 10  
- 
- //创建一个人工自动重置事件对象  
- CMyEvent g_myEvent;  
- int g_iNum = 0;  
- 
- 
- //线程函数1  
- void * ThreadProc1(void *pParam)  
- {  
- for (int i = 0; i < PRINT_TIMES; i++)  
-     {  
-         g_iNum++;  
-         cout<<"ThreadProc1 do print, Num = "<<g_iNum<<endl;  
- 
- //设置事件为有信号状态  
-         g_myEvent.Set();  
- 
-         sleep(1);  
-     }  
- 
- return (void *)0;  
- }  
- 
- //线程函数2  
- void * ThreadProc2(void *pParam)  
- {  
- bool bRet = false;  
- while ( 1 )  
-     {  
- if ( g_iNum >= PRINT_TIMES )  
-         {  
- break;  
-         }  
- 
- //以当前事件对象阻塞本线程，将其挂起  
-         bRet = g_myEvent.Wait();  
- if ( bRet )  
-         {  
-             cout<<"ThreadProc2 do print, Num = "<<g_iNum<<endl;  
- 
- //设置事件为无信号状态  
-             g_myEvent.Reset();  
-         }  
- else  
-         {  
-             cout<<"ThreadProc2 system exception"<<endl;  
-         }  
-     }  
- 
- return (void *)0;  
- }  
- 
- 
- int main(int argc, char* argv[])  
- {  
-     pthread_t thread1,thread2;  
-     pthread_attr_t attr1,attr2;  
- 
- 
- //创建两个工作线程  
-     pthread_attr_init(&attr1);  
-     pthread_attr_setdetachstate(&attr1,PTHREAD_CREATE_JOINABLE);  
- if (pthread_create(&thread1,&attr1, ThreadProc1,NULL) == -1)  
-     {  
-         cout<<"Thread 1: create failed"<<endl;  
-     }  
-     pthread_attr_init(&attr2);  
-     pthread_attr_setdetachstate(&attr2,PTHREAD_CREATE_JOINABLE);  
- if (pthread_create(&thread2,&attr2, ThreadProc2,NULL) == -1)  
-     {  
-         cout<<"Thread 2: create failed"<<endl;  
-     }  
- 
- //等待线程结束  
- void *result;  
-     pthread_join(thread1,&result);  
-     pthread_join(thread2,&result);  
- 
- //关闭线程，释放资源  
-     pthread_attr_destroy(&attr1);  
-     pthread_attr_destroy(&attr2);  
- 
- int iWait;  
-     cin>>iWait;  
- 
- return 0;  
- }  



    编译，运行。可以看到，与Win32平台上的测试结果相同，好神奇！

![](http://hi.csdn.net/attachment/201112/17/0_13241222105LX5.gif)

from:[http://blog.csdn.net/chexlong/article/details/7080537](http://blog.csdn.net/chexlong/article/details/7080537)









