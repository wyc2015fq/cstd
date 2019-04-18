# socket服务器模型 - ljx0305的专栏 - CSDN博客
2008年06月05日 21:50:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：936
多线程有三种模式：   
  1、一个请求一个线程。   
  2、一个连接一个线程。   
  3、预先开一个线程池队列，等待客户机的连接。 
我看了一下Apache1.3.7的源代码,主要是看了一下其中SOCKET网络服务的结构,其中使用select   +   thread   pool的过程大体如下:   
  首先创建一个全局的job_list链表,用于存放使用select方式(accept)得到的新的socket句柄.   
    然后再创建一个工作者线程池,每个工作者线程都运行一个叫child_sub_mail()的线程安全函数,这个函数调用remove_job(),进入阻塞方式,直到从job_list得到一个已经accepted的新的socket句柄,接下来就循环处理这个socket上的读写请求直到这个socket断开,然后再调用remove_job()来处理下一个socket连接.remove_job()同时把该socket从job_list链表里删除。   
      在主进程中,当一个新的socket连接被建立,apache就调用add_job()把新的socket句柄加入job_list.这样就会被work_thread   pool里面空闲的work_thread   
  马上从job_list发现并接管对该socket的所有I/O请求。   
  当然还有线程安全退出和对socket异常错误的处理，apache都处理的相当的好。   
  不妥之处欢迎指正，如果有详细的分析就更好。当然apache的这种模式也不是十全十美的。比如work_thread_pool中应当有多少个线程？能不能根据软硬件环境自动调整线程池的大小（工作者线程数目）？使空闲的线程数不至于过多，也不至于当一个accepted   socket加入job_list后得不到及时的处理。如果使用select+process(fork)方式在大型机环境下性能是不是要好于上面的模式？
