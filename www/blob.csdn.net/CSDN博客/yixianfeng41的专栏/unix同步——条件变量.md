# unix同步——条件变量 - yixianfeng41的专栏 - CSDN博客
2016年01月04日 19:51:45[第2梦](https://me.csdn.net/yixianfeng41)阅读数：1632
一。互斥量和条件变量简介
       互斥量(mutex)从本质上说是一把锁，在访问共享资源前对互斥量进行加锁，在访问完成后释放互斥量上的锁。对互斥量进行加锁以后，任何其他试图再次对互斥锁加锁的线程将会阻塞直到当前线程释放该互斥锁。如果释放互斥锁时有多个线程阻塞，所有在该互斥锁上的阻塞线程都会变成可运行状态，第一个变为运行状态的线程可以对互斥锁加锁，其他线程将会看到互斥锁依然被锁住，只能回去再次等待它重新变为可用。
       条件变量(cond)是在多线程程序中用来实现"等待-->唤醒"逻辑常用的方法。条件变量利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；另一个线程使“条件成立”。为了防止竞争，条件变量的使用总是和一个互斥锁结合在一起。线程在改变条件状态前必须首先锁住互斥量，函数pthread_cond_wait把自己放到等待条件的线程列表上，然后对互斥锁解锁(这两个操作是原子操作)。在函数返回时，互斥量再次被锁住。
二。为什么存在条件变量
       首先，举个例子：在应用程序中有连个线程thread1，thread2，thread3和thread4，有一个int类型的全局变量iCount。iCount初始化为0，thread1和thread2的功能是对iCount的加1，thread3的功能是对iCount的值减1，而thread4的功能是当iCount的值大于等于100时，打印提示信息并重置iCount=0。
       如果使用互斥量，线程代码大概应是下面的样子：
       thread1/2：
       while (1)
       {
             pthread_mutex_lock(&mutex);
             iCount++;
             pthread_mutex_unlock(&mutex);
       }
       thread4:
       while(1)
       {
             pthead_mutex_lock(&mutex);
             if (100 <= iCount)
             {
                   printf("iCount >= 100\r\n");
                   iCount = 0;
                   pthread_mutex_unlock(&mutex);
             }
             else
             {
                   pthread_mutex_unlock(&mutex);
             }
       }
       在上面代码中由于thread4并不知道什么时候iCount会大于等于100，所以就会一直在循环判断，但是每次判断都要加锁、解锁(即使本次并没有修改iCount)。这就带来了问题一，CPU浪费严重。所以在代码中添加了sleep(),这样让每次判断都休眠一定时间。但这由带来的第二个问题，如果sleep()的时间比较长，导致thread4处理不够及时，等iCount到了很大的值时才重置。对于上面的两个问题，可以使用条件变量来解决。
      首先看一下使用条件变量后，线程代码大概的样子：
      thread1/2:
       while(1)
       {
               pthread_mutex_lock(&mutex);
               iCount++;
               pthread_mutex_unlock(&mutex);
               if (iCount >= 100)
               {
                      pthread_cond_signal(&cond);
               }
       }         
       thread4:
       while (1)
       {
              pthread_mutex_lock(&mutex);
              while(iCount < 100)
              {
                     pthread_cond_wait(&cond, &mutex);
              }
              printf("iCount >= 100\r\n");
              iCount = 0;
              pthread_mutex_unlock(&mutex);
       }
       从上面的代码可以看出thread4中，当iCount < 100时，会调用pthread_cond_wait。而pthread_cond_wait在上面应经讲到它会释放mutex，然后等待条件变为真返回。当返回时会再次锁住mutex。因为pthread_cond_wait会等待，从而不用一直的轮询，减少CPU的浪费。在thread1和thread2中的函数pthread_cond_signal会唤醒等待cond的线程（即thread4），这样当iCount一到大于等于100就会去唤醒thread4。从而不致出现iCount很大了，thread4才去处理。
      需要注意的一点是在thread4中使用的while (iCount < 100),而不是if (iCount < 100)。这是因为在pthread_cond_singal()和pthread_cond_wait()返回之间有时间差，假如在时间差内，thread3又将iCount减到了100以下了，那么thread4就需要在等待条件为真了。
