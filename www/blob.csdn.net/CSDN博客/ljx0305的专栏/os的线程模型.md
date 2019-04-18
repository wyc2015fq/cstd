# os的线程模型 - ljx0305的专栏 - CSDN博客
2008年06月05日 21:36:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：575标签：[os																[solaris																[freebsd																[thread																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=freebsd&t=blog)](https://so.csdn.net/so/search/s.do?q=solaris&t=blog)](https://so.csdn.net/so/search/s.do?q=os&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
                os的线程模型有3种     用户态,   核心态     混合态   ,   
  用户态就是   纯粹的用户级线程,对于内核是透明的,这样的好处是上下文切换的时候很快.   
  但是不好的就是一个线程阻塞会阻塞整个进程.   
  核心态也就是所谓的轻量级进程.单个线程阻塞不会阻塞整个进程.是基本的调度单位.   
  混合态有2种   1:1   比如linux种使用的native   thread   .   solaris   ,freebsd   的   是M;N的   
    基本的想法就是一个进程里面有核心态线程也有用户态线程,   线程还可以在两种状态下切换.   
  不过这个需要在内存管理,进程管理方面坐更多的考虑.   solaris的比较成熟可以参考            
