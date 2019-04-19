# vc6多线程开发时导致cpu猛增(狂飙)不停的跳动的原因。 - xqhrs232的专栏 - CSDN博客
2014年02月13日 14:48:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1099
原文地址::[http://www.greensoftcode.net/techntxt/20132221113276189397](http://www.greensoftcode.net/techntxt/20132221113276189397)
相关文章
1、VC多线程编程学习笔记（一）----[http://blog.sina.com.cn/s/blog_4dbbf76f010008yy.html](http://blog.sina.com.cn/s/blog_4dbbf76f010008yy.html)
2、关于多线程中死循环的问题----[http://bbs.csdn.net/topics/360227710](http://bbs.csdn.net/topics/360227710)
3、关于多线程中死循环问题解决办法----[http://bbs.chinaunix.net/thread-955755-1-1.html](http://bbs.chinaunix.net/thread-955755-1-1.html)
- 
vc6多线程开发时导致cpu猛增(狂飙)不停的跳动的原因。
先说下我产生这种情况的程序具体代码如下
建立两个线程(这里也就介绍啦c++多线程的实现)
  1. 启动两个线程
      HANDLE m_handle1=CreateThread(NULL,0, m_fun1,NULL, 0,NULL);
 CloseHandle( m_handle1);
      HANDLE m_handle2=CreateThread(NULL,0, m_fun2,NULL, 0,NULL);
 CloseHandle( m_handle2);
  2.线程执行的函数
       线程1函数
     DWORD WINAPI Cfrm::m_fun1(LPVOID lpParameter)
       {
        while(true)  //在多线程下执行死循环
         {
          ......
         sleep(100) ; //这个很重要：使cpu有喘气的机会
         }
        }
   线程2函数
  DWORD WINAPI Cfrm::m_fun2(LPVOID lpParameter)
       {
        while(true)  //在多线程下执行死循环
         {
          if (a>b)
          {
          ......
          sleep(100) ; //这个很重要：使cpu有喘气的机会
          }         
         }
        }
3.在frm.h中
   private:
   static DWORD WINAPI Cfrm::m_fun1(LPVOID lpParameter);
   static DWORD WINAPI Cfrm::m_fun2(LPVOID lpParameter);
开始执行两个线程发现cpu一直在50%左右 我的双核cpu 这么可能呢？困扰啦我很长时间
后来经过一顿查找代码，发现m_fun2函数的 while(true) ｛｝循环可能没有给cpu任何空虚时间
当if (a>b)条件不成立时。
后来修改啦下m_fun2 为
  DWORD WINAPI Cfrm::m_fun2(LPVOID lpParameter)
       {
        while(true)  //在多线程下执行死循环
         {
          if (a>b)
          {
          ......
          }  
         sleep(100) ; //这个很重要：使cpu有喘气的机会       
         }
        }
问题得到解决。
这个问题说明我们在学校说的基础知识并不是没有用，恰恰相反而是太重要啦！
//===============================================================================================
//备注：
1》在多线程的情况下，尽量避免while(1)这样的死循环表达式，这样的死循环会造成CPU使用率飚升，其它线程卡死不工作，就算是要进行while(1)死循环，最好是能够Sleep一段时间，让CPU能够有时间重新调度线程工作
