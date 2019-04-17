# 条件变量(Condition Variable)详解 - DoubleLi - 博客园






条件变量(Condtion Variable)是在多线程程序中用来实现“等待->唤醒”逻辑常用的方法。举个简单的例子，应用程序A中包含两个线程t1和t2。t1需要在bool变量test_cond为true时才能继续执行，而test_cond的值是由t2来改变的，这种情况下，如何来写程序呢？可供选择的方案有两种：
- 第一种是t1定时的去轮询变量test_cond，如果test_cond为false，则继续休眠；如果test_cond为true，则开始执行。
- 第二种就是上面提到的条件变量，t1在test_cond为false时调用cond_wait进行等待，t2在改变test_cond的值后，调用cond_signal，唤醒在等待中的t1，告诉t1 test_cond的值变了，这样t1便可继续往下执行。

      很明显，上面两种方案中，第二种方案是比较优的。在第一种方案中，在每次轮询时，如果t1休眠的时间比较短，会导致cpu浪费很厉害；如果t1休眠的时间比较长，又会导致应用逻辑处理不够及时，致使应用程序性能下降。第二种方案就是为了解决轮询的弊端而生的。然而条件变量在使用的过程中，比较容易出错，如何用得不正确的话，会适得其反的，接下来，我将详细分析如何来使用条件变量，希望能够给在使用条件变量过程中遇到问题的朋友有所帮助。
      在开始介绍之前，需要说明一下，在接下来的介绍中，需要用到互斥锁和条件变量相关的内容，在这里我以[Linux](http://lib.csdn.net/base/linux)下的pthread_mutex_t为互斥锁类型，pthread_cond_t为条件变量类型来进行介绍，对pthread不熟的朋友，可以参考一下[linux](http://lib.csdn.net/base/linux)下的manual。
      1. 下面是把刚开始举的例子翻译后的程序：

|123456789101112131415161718192021|pthread_mutex_t mutex;///< 互斥锁pthread_cond_t  cond;///< 条件变量bool test_cond =false;/// TODO 初始化mutex和cond/// thread 1:pthread_mutex_lock(&mutex);///< 1while(!test_cond){    pthread_cond_wait(&cond,&mutex);///< 2,3}pthread_mutex_unlock(&mutex);///< 4RunThread1Func();/// thread 2:pthread_mutex_lock(&mutex);///< 5test_cond =true;pthread_cond_signal(&cond);pthread_mutex_unlock(&mutex);///< 6/// TODO 销毁mutex和cond|
|----|----|


      通过上面的例子，下面我来介绍一下条件变量在使用过程中需要注意的几点（也是比较容易出错的）：
      （1）条件变量的使用过程中，最为关键的一点是互斥锁的使用。细心的朋友应该发现了，我在上面的例子中标了1、2、3、4、5、6个标号。在这里1、4、5、6都是正常的lock/unlock，2、3是需要特别说明的。2是进入pthread_cond_wait后的，pthread_cond_wait调的pthread_mutex_unlock，这样做的目的是为了保证在thread1阻塞wait后，thread2获取同一把锁mutex的时候，能够正常获取（即5，6）。3是thread1被唤醒后，要退出pthead_cond_wait之前，pthread_cond_wait调的pthread_mutex_lock，这样做的目的是为了把mutex的控制权还给调用pthread_cond_wait的线程（即thread1）。整理一下基本的时序为：

|123|thread 1lock->thread 1 wait-> thread 1 unlock(in wait)->thread 2lock->thread 2 signal->thread 2 unlock->thread 1lock(in wait)->thread 1 unlock|
|----|----|


      （2）条件变量使用的过程中，通常会加一个bool或者int的值test_cond来配合使用。这里需要注意的一点是一定要在signal之前来改变test_cond，这样才能保证wait的线程被唤醒后，能够取到正确的test_cond的值，否则后果是不可预测的。










