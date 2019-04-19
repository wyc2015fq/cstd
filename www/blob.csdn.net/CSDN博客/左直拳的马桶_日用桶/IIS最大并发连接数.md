# IIS最大并发连接数 - 左直拳的马桶_日用桶 - CSDN博客
2014年08月06日 22:24:28[左直拳](https://me.csdn.net/leftfist)阅读数：15376
个人分类：[IIS](https://blog.csdn.net/leftfist/article/category/2459577)
**最大并发连接数 = 队列长度 + 工作线程数**
【工作线程数】
IIS实际可以第一时间处理的请求数。比如，工作线程数 = 100，一万个连接请求同时涌过来，那么只有100个可以被处理，其余9900个进入等待队列等待，或者直接以503错误（服务器不可用）返回。
工作线程可以通过Machine.config的maxWorkerThreads修改
【队列长度】
除了IIS工作线程数可以第一时间进行处理外，其余的请求可以进入等待队列进行等待的数量。比如：
工作线程数 = 100
队列长度 = 5000
那么一万个连接请求同时涌过来，将有100个第一时间被处理，5000个等待，4900个以503返回。
这个可以在应用程序池-->ASP.NET v4.0 Classic-->高级设置-->队列长度   里修改
【最大并发连接数】
工作线程数 = 100
队列长度 = 5000
最大并发连接数 = 5000
那么一万个连接请求同时涌过来，将有100个第一时间被处理，4900个进入队列等待，5000个以503返回。
假如最大并发连接数=10000，则
那么一万个连接请求同时涌过来，将有100个第一时间被处理，5000个进入队列等待，4900个以503返回。
这个可以在IIS-->网站(要修改的网站)-->高级设置-->最大并发连接数    里修改
【web园】
IIS是一个网站对应一个应用程序池，一个应用程序池默认一个工作进程。一个工作进程有默认的工作线程数量（工作线程可以通过Machine.config的maxWorkerThreads修改，并跟CPU是几核有关系)，我们可以通过设置应用程序池的最大工作进程为2，那这样就成了web园。相应的工作线程也随工作进程的增加也加倍，但是要考虑cpu和内存的会不会消耗过大。web园是直接的表现就是通过任务管理器里可以看到两个w3pw.exe进程。
参考文章：
[http://www.cnblogs.com/leotsai/p/understanding-iis-multithreading-system.html](http://www.cnblogs.com/leotsai/p/understanding-iis-multithreading-system.html)
[http://www.cnblogs.com/SALIN/archive/2013/03/28/2986704.html](http://www.cnblogs.com/SALIN/archive/2013/03/28/2986704.html)
