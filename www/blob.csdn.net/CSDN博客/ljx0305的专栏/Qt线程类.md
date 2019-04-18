# Qt线程类 - ljx0305的专栏 - CSDN博客
2013年01月09日 17:00:15[ljx0305](https://me.csdn.net/ljx0305)阅读数：693
**Qt线程类**
Qt 包含下面一些线程相关的类：
QThread 提供了开始一个新线程的方法
QThreadStorage 提供逐线程数据存储
QMutex  提供相互排斥的锁，或互斥量
QMutexLocker 是一个便利类，它可以自动对QMutex加锁与解锁
QReadWriterLock 提供了一个可以同时读操作的锁
QReadLocker与QWriteLocker 是便利类，它自动对QReadWriteLock加锁与解锁
QSemaphore 提供了一个整型信号量，是互斥量的泛化
QWaitCondition 提供了一种方法，使得线程可以在被另外线程唤醒之前一直休眠。
Qt线程的创建
   Qt线程中有一个公共的抽象类，所有的线程都是从这个QThread抽象类中派生的，要实现QThread中的纯虚函数run(),run()函数是通过start()函数来实现调用的。 
```
1 class MyThread : public QThread {
 2     public:
 3         virtual void run();
 4     };
 5 
 6     void MyThread::run()
 7     {
 8         for( int count = 0; count < 20; count++ ) {
 9             sleep( 1 );
10             qDebug( "Ping!" );
11         }
12     }
13 
14     int main()
15     {
16         MyThread a;
17         MyThread b;
18 
19         a.start();//自动调用run(),否则即使该线程创建，也是一开始就挂起
20         b.start();
21         //要等待线程a,b都退出
22         a.wait();
23         b.wait();
24     }
25
```
Qt线程同步
**1.QMutex**
[**QMutex**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qmutex.html#QMutex) ( bool recursive = FALSE )
virtual [**~QMutex**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qmutex.html#~QMutex) ()
void [**lock**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qmutex.html#lock) ()  //试图锁定互斥量。如果另一个线程已经锁定这个互斥量，那么这次调用将*阻塞*直到那个线程把它解锁。
void [**unlock**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qmutex.html#unlock) ()
bool [**locked**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qmutex.html#locked) ()
bool [**tryLock**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qmutex.html#tryLock) () //如果另一个进程已经锁定了这个互斥量，这个函数返回假，而不是一直等到这个锁可用为止，比如，它不是阻塞的。
```
1 //Qt
 2   QMutex mutex;
 3   void someMethod()
 4   {
 5      mutex.lock();
 6      qDebug("Hello");
 7      qDebug("World");
 8      mutex.unlock();
 9   }
10   
11 //用Java的术语，这段代码应该是：
12   void someMethod()
13   {
14      synchronized {
15        qDebug("Hello");
16        qDebug("World");
17      }
18   }
```
     不过在Qt中我们可用通过另一个类来简化这种应用，因为如果使用QMutex.lock()而没有对应的使用QMutex.unlcok()的话
 就会造成死锁，别的线程永远也得不到接触该mutex锁住的共享资源的机会。尽管可以不使用lock()而使用tryLock(timeout)
来避免因为死等而造成的死锁( tryLock(负值)==lock()),但是还是很有可能造成错误。
   对于上述的情况MFC中用CSingleLock 或 MultiLock，Boost中用boost::mutex::scoped_lock来进行解决，而在Qt中用
QMutexLocker来进行解决。下面是没有采用QMutexLocker的例子和采用QMutexLocker的方案。
 2.QMutexLocker
this complex function locks a [QMutex](http://www.cnblogs.com/NeuqUstcIim/admin/qmutex.html) upon entering
 the function and unlocks the mutex at all the exit points
```
1 int complexFunction(int flag)
 2  {
 3      mutex.lock();
 4 
 5      int retVal = 0;
 6 
 7      switch (flag) {
 8      case 0:
 9      case 1:
10          mutex.unlock();
11          return moreComplexFunction(flag);
12      case 2:
13          {
14              int status = anotherFunction();
15              if (status < 0) {
16                  mutex.unlock();
17                  return -2;
18              }
19              retVal = status + flag;
20          }
21          break;
22      default:
23          if (flag > 10) {
24              mutex.unlock();
25              return -1;
26          }
27          break;
28      }
29 
30      mutex.unlock();
31      return retVal;
32  }
```
*This example increases the likelihood that errors will occur.Using QMutexLocker greatly simplifies the code, and makes it more readable:*
```
1 int complexFunction(int flag)
 2  {
 3      QMutexLocker locker(&mutex);
 4 
 5      int retVal = 0;
 6 
 7      switch (flag) {
 8      case 0:
 9      case 1:
10          return moreComplexFunction(flag);
11      case 2:
12          {
13              int status = anotherFunction();
14              if (status < 0)
15                  return -2;
16              retVal = status + flag;
17          }
18          break;
19      default:
20          if (flag > 10)
21              return -1;
22          break;
23      }
24 
25      return retVal;
26  }
```
*Now, the mutex will always be unlocked when the QMutexLocker object is destroyed (when the function returns since locker is an auto variable)*.即使在抛出异常的情况下也可以使用。
**3. QReadWriteLock**
  用mutex进行线程同步有一个问题就是mutex只允许某个时刻只允许一个线程对共享资源进行访问，如果同时有多个线程对共享
资源进行读访问，而只有一个写操作线程，那么在这种情况下如果采用mutex就成为程序运行性能的瓶颈了。在这种情况下Qt下采用
QReadWriteLock来实现多个线程读，一个线程写。写线程执行的时候会阻塞所有的读线程，而读线程之间的运行不需要进行同步。
```
1 MyData data;
 2 QReadWriteLock lock;
 3 void ReaderThread::run()
 4 {
 5     ![](http://www.cnblogs.com/Images/dot.gif)
 6     lock.lockForRead();
 7     access_data_without_modifying_it(&data);
 8     lock.unlock();
 9     ![](http://www.cnblogs.com/Images/dot.gif)
10 }
11 void WriterThread::run()
12 {
13     ![](http://www.cnblogs.com/Images/dot.gif)
14     lock.lockForWrite();
15     modify_data(&data);
16     lock.unlock();
17     ![](http://www.cnblogs.com/Images/dot.gif)
18 }
19 
20
```
   QReadWriterLock 与QMutex相似，除了它对 "read","write"访问进行区别对待。它使得多个读者可以共时访问数据。使用QReadWriteLock而不是QMutex，可以使得多线程程序更具有并发性。
**4.QReadLocker和QWriteLocker**
    对于QMutex有QMutexLocker来简化使用，而对于QReadWriteLock有 QReadLocker和QWriteLocker。
*Here's an example that uses QReadLocker to lock and unlock a read-write lock for reading:  *
```
QReadWriteLock lock;
 QByteArray readData()
 {
     QReadLocker locker(&lock);
     ![](http://www.cnblogs.com/Images/dot.gif)
     return data;
 }
```
It is equivalent to the following code:
```
QReadWriteLock lock;
 QByteArray readData()
 {
    ** lock.lockForRead();**
     ![](http://www.cnblogs.com/Images/dot.gif)
     **lock.unlock();**
     return data;
 }
```
**5.QSemaphore**
    QSemaphore 是QMutex的一般化，它可以保护一定数量的相同资源，与此相对，一个mutex只保护一个资源。下面例子中，使用QSemaphore来控制对**环状缓冲区**的访问，此缓冲区被生产者线程和消费者线程共享。生产者不断向缓冲写入数据直到**缓冲末端**，消费者从缓冲不断从**缓冲头部**读取数据。
信号量比互斥量有更好的并发性，假如我们用互斥量来控制对缓冲的访问，那么生产者，消费者不能同时访问缓冲。然而，我们知道在同一时刻，不同线程访问缓冲的不同部分并没有什么危害。
      QSemaphore semaphore(1); |     QMutex mutex;
      Qsemaphore.acquire();         |     Qmutex.lock();
      Qsemaphore.release();         |     Qmutex.unlock();
Public Functions
- [QSemaphore](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#QSemaphore) ( int n = 0 )
- [~QSemaphore](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#dtor.QSemaphore) ()
- void [acquire](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#acquire) ( int n = 1 )
- int [available](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#available) () const
- void [release](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#release) ( int n = 1 )
- bool [tryAcquire](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#tryAcquire) ( int n = 1 )
- bool [tryAcquire](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#tryAcquire-2) ( int n, int timeout )
 Semaphores support two fundamental operations, [acquire](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#acquire)() and [release](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#release)():
- acquire(n) tries to acquire n resources. If there aren't that many resources available, the call will block
 until this is the case.
- release(n) releases n resources.
- [](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#tryAcquire)[tryAcquire](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#tryAcquire)() returns immediately
 if it cannot acquire the resources
- [](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#available)[available](http://www.cnblogs.com/NeuqUstcIim/admin/qsemaphore.html#available)() returns the
 number of available resources at any time.
** Example:**
```
QSemaphore sem(5);      // sem.available() == 5
 sem.acquire(3);         // sem.available() == 2
 sem.acquire(2);         // sem.available() == 0
 sem.release(5);         // sem.available() == 5
 sem.release(5);         // sem.available() == 10
 sem.tryAcquire(1);      // sem.available() == 9, returns true
 sem.tryAcquire(250);    // sem.available() == 9, returns false
```
     生产者线程写数据到buffer直到缓冲末端，然后重新从buffer的头部开始写。
     显然producer线程和consumer线程是需要进行同步的，*If the producer generates the data too fast, it will overwrite data that the consumer hasn't yet read; if the consumer reads the data too fast, it will pass the producer and read garbage.*
A crude way to solve this problem is to have the producer fill
 the buffer, then wait until the consumer has read the entire buffer, and so on.显然这样做效率是比较低的。
```
1  const int DataSize = 100000;
 2  const int BufferSize = 8192;
 3  char buffer[BufferSize];
 4 
 5  //When the application starts, the reader thread will start  
    //acquiring "free" bytes and convert them into "used" bytes
 
 6  QSemaphore freeBytes(BufferSize);   //producer线程在此区域写入数据，初始资源数量为BufferSize
 7  QSemaphore usedBytes;                  //consumer线程读取此区域的数据,初始资源数量为0
 8 
 9 
10 //For this example, each byte counts as one resource.
11 //In a real-world application, we would probably operate on larger
    //units (for example, 64 or 256 bytes at a time) 
 
 
12  class Producer : public QThread
13  {
14  public:
15      void run();
16  };
17 //生产者每acquire一次就，使用掉Buffer个资源中的一个，而写入的字符存入到buffer数组中
 
    //从而消费者可用读取字符，从而消费者获取一个资源 
18  void Producer::run()
19  {
20      //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
21      for (int i = 0; i < DataSize; ++i) {
22          ***freeBytes.acquire***();
23          buffer[i % BufferSize] = "ACGT"[(int)qrand() % 4];
24          ***usedBytes.release***();
25      }
26  }
27 
28  class Consumer : public QThread
29  {
30  public:
31      void run();
32  };
33 
34  void Consumer::run()
35  {
36      for (int i = 0; i < DataSize; ++i) {
37          ***usedBytes.acquire***();
38          fprintf(stderr, "%c", buffer[i % BufferSize]);
39          ***freeBytes.release***();
40      }
41      fprintf(stderr, "\n");
42  }
43 //Finally, in main(), we start the producer and consumer threads. 
    //What happens then is that the producer converts some "free" space
    //into "used" space, and the consumer can then convert it back to //"free" space.
46  int main(int argc, char *argv[])
47  {
48      QCoreApplication app(argc, argv);
 
 
49      Producer producer;
50      Consumer consumer;
 
 
51      producer.start();
52      consumer.start();
 
 
53      producer.wait();
54      consumer.wait();
 
 
55      return 0;
56  }
```
**producer的run函数：**
   当producer线程执行run函数，如果buffer中已经满了，而没有consumer线程没有读，这样producer就不能再往buffer
中写字符。此时在 ***freeBytes.acquire***处就阻塞直到consumer线程读(consume）数据。一旦producer获取到一个字节(资源）
就写如一个随机的字符，并调用 ***usedBytes.release***从而consumer线程获取一个资源可以读一个字节的数据了。
**consumer的run函数：**
    当consumer线程执行run函数，如果buffer中没有数据，就是资源=0，则consumer线程在此处阻塞。直到producer线程执行
写操作，写入一个字节，并执行***usedBytes.release***从而使得consumer线程的可用资源数=1。则consumer线程从阻塞状态中退出，
并将 ***usedBytes***资源数-1，当前资源数=0。
**6.QWaitCondition**
- [**QWaitCondition**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qwaitcondition.html#QWaitCondition) ()
- virtual [**~QWaitCondition**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qwaitcondition.html#~QWaitCondition) ()
- bool [**wait**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qwaitcondition.html#wait-2) ( QMutex * mutex, unsigned long time = ULONG_MAX )
- void [**wakeOne**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qwaitcondition.html#wakeOne) ()
- 
void [**wakeAll**](http://www.qtopia.org.cn/doc/qiliang.net/qt/qwaitcondition.html#wakeAll) ()
**Public function:**
**bool QWaitCondition::wait ( [QMutex](http://www.qtopia.org.cn/doc/qiliang.net/qt/qmutex.html) * mutex, unsigned long time = ULONG_MAX )**
 1) 释放锁定的mutex
 2) 在线程对象上等待
mutex必须由调用线程进行初锁定 。注意调用wait的话，会自动调用unlock解锁之前锁住的资源，不然会造成死锁。
线程1等待线程2来改变共享资源，从而达到一定的条件然后发出信号，使得线程1从wait中的阻塞状态中被唤醒。
但是线程2想改变资源，却无法办到，因为线程1调用lock之后就在wait中blocking，了但是没有及时的unlock，那么这就
构成了死锁的条件。所以说wait函数除了使调用线程切换到内核态之外，还自动unlock(&mutex) 
***mutex*将被解锁，并且调用线程将会阻塞，直到下列条件之一满足时才醒来：**
- 另一个线程使用[wakeOne](http://www.qtopia.org.cn/doc/qiliang.net/qt/qwaitcondition.html#wakeOne)()或[wakeAll](http://www.qtopia.org.cn/doc/qiliang.net/qt/qwaitcondition.html#wakeAll)()传输信号给它。在这种情况下，这个函数将返回真。
- *time*毫秒过去了。如果*time*为ULONG_MAX（默认值），那么这个等待将永远不会超时（这个事件必须被传输）。如果等待的事件超时，这个函数将会返回假互斥量将以同样的锁定状态返回。这个函数提供的是允许从锁定状态到等待状态的原子转换。 
**void QWaitCondition::wakeAll ()**
这将会唤醒所有等待QWaitCondition的线程。这些线程被唤醒的顺序依赖于操组系统的调度策略，并且不能被控制或预知。
**void  QWaitCondition::wakeOne ()**
这将会唤醒所有等待QWaitCondition的线程中的一个线程。这个被唤醒的线程依赖于操组系统的调度策略，并且不能被控制或预知。

 假定每次用户按下一个键，我们有三个任务要同时执行，每个任务都可以放到一个线程中，每个线程的run()都应该是这样：
```
QWaitCondition key_pressed;
  for (;;) {
     key_pressed.wait(); // 这是一个QWaitCondition全局变量
     // 键被按下，做一些有趣的事
     do_something();
  }
```
 或是这样：
```
forever {
     mutex.lock();
     keyPressed.wait(&mutex);
     do_something();
     mutex.unlock();
 }
```
第四个线程回去读键按下并且每当它接收到一个的时候唤醒其它三个线程，就像这样：
```
QWaitCondition key_pressed;
  for (;;) {
     getchar();
     // 在key_pressed中导致引起任何一个线程。wait()将会从这个方法中返回并继续执行
     key_pressed.wakeAll();
  }
```
    注意这三个线程被唤醒的顺序是未定义的，并且当键被按下时，**这些线程中的一个或多个还在do_something()，它们将不会被唤醒（因为它们现在没有等待条件变量）**并且这个任务也就不会针对这次按键执行操作。这种情况是可以避免得，比如，就像下面这样做：
```
1  QMutex mymutex;
 2   QWaitCondition key_pressed;
 3   int mycount=0;
 4 
 5   //Worker线程代码
 6   for (;;) {
 7      key_pressed.wait(); // 这是一个QWaitCondition全局变量
 
         //keyPressed.wait(&mutex);
 8      mymutex.lock();
 9      **mycount****++**;
10      mymutex.unlock();
 
 
11      **do_something**();
 
 
12      mymutex.lock();
13      **mycount****--**;
14      mymutex.unlock();
15   }
16 
17   // 读取按键线程代码
18   for (;;) {
19      getchar();
20      mymutex.lock();
21      // 睡眠，直到没有忙碌的工作线程才醒来。count==0说明没有Worker线程在do
 something
22      while( count > 0 ) {
23        mymutex.unlock();
24        sleep( 1 );
25        mymutex.lock();
26      }
27      mymutex.unlock();
28      key_pressed.wake**All**();
29   }
30
```
 应用条件变量对前面用信号量进行保护的环状缓冲区的例子进行改进：
   下面的例子中：
     1）生产者首先必须检查缓冲是否已满(numUsedBytes==BufferSize)，如果是，线程停下来等待bufferNotFull条件。如果不是，在缓冲中生产数据，增加numUsedBytes,激活条件 bufferNotEmpty。
     2）使用mutex来保护对numUsedBytes的访问。
     另外，QWaitCondition::wait()接收一个mutex作为参数，这个mutex应该被调用线程初始化为锁定状态。在线程进入休眠状态之前，mutex会被解锁。而当线程被唤醒时，mutex会再次处于锁定状态。
     而且，从锁定状态到等待状态的转换是原子操作，这阻止了竞争条件的产生。当程序开始运行时，只有生产者可以工作。消费者被阻塞等待bufferNotEmpty条件，一旦生产者在缓冲中放入一个字节，bufferNotEmpty条件被激发，消费者线程于是被唤醒。
```
1 const int DataSize = 100000;
 2  const int BufferSize = 8192;
 3  char buffer[BufferSize];
 4 
 5  QWaitCondition bufferNotEmpty;
 6  QWaitCondition bufferNotFull;
 7  QMutex mutex;
 
 8  int numUsedBytes = 0;
 9 
10  class Producer : public QThread
11  {
12  public:
13      void run();
14  };
15 
16  void Producer::run()
17  {
18      qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
19 
20      for (int i = 0; i < DataSize; ++i) {
21          mutex.lock();
             //producer线程首先检查缓冲区是否已满 
22          if (numUsedBytes == BufferSize)//缓冲区已满，等待consumer来减少numUsedBytes
                 // bufferNotFull.wait(&mutex)先调用mutex.unlock()然后收到信号时调用mutex.lock()
23              bufferNotFull.wait(&mutex);//缓冲区已满等待bufferNotFull的条件变量成立变为有信号
24          mutex.unlock();
25        
26          buffer[i % BufferSize] = "ACGT"[(int)qrand() % 4];
27 
28          mutex.lock();
29          ++numUsedBytes; //producer用掉一个Bytes，表示producer写入buffer中的字节数
30          bufferNotEmpty.wakeAll();
31          mutex.unlock();
32      }
33  }
34 
35  class Consumer : public QThread
36  {
37  public:
38      void run();
39  };
40 
41  void Consumer::run()
42  {
43      for (int i = 0; i < DataSize; ++i) {
44          mutex.lock();
45          if (numUsedBytes == 0)
46              bufferNotEmpty.wait(&mutex);
47          mutex.unlock();
48 
49          fprintf(stderr, "%c", buffer[i % BufferSize]);
50 
51          mutex.lock();
52          --numUsedBytes;
53          bufferNotFull.wakeAll();
54          mutex.unlock();
55      }
56      fprintf(stderr, "\n");
57  }
58 
59  int main(int argc, char *argv[])
60  {
61      QCoreApplication app(argc, argv);
62      Producer producer;
63      Consumer consumer;
64      producer.start();
65      consumer.start();
66      producer.wait();
67      consumer.wait();
68      return 0;
69  }
```
 另外一个例子：
```
1     #include <qapplication.h>
 2     #include <qpushbutton.h>
 3 
 4     // 全局条件变量
 5     QWaitCondition mycond;
 6 
 7     // Worker类实现
 8     class Worker : public QPushButton, public QThread
 9     {
10         Q_OBJECT
11 
12     public:
13         Worker(QWidget *parent = 0, const char *name = 0)
14             : QPushButton(parent, name)
15         {
16             setText("Start Working");
17 
18             // 连接从QPushButton继承来的信号和我们的slotClicked()方法
19             connect(this, SIGNAL(clicked()), SLOT(slotClicked()));
20 
21             // 调用从QThread继承来的start()方法……这将立即开始线程的执行
22             QThread::start();
23         }
24 
25     public slots:
26         void slotClicked()
27         {
28             // 唤醒等待这个条件变量的一个线程
29             **mycond.wakeOne**();
30         }
31 
32     protected:
33         void run()
34         {
35             // 这个方法将被新创建的线程调用……
36 
37             while ( TRUE ) {
38                 // 锁定应用程序互斥锁，并且设置窗口标题来表明我们正在等待开始工作
39                 qApp->lock();
40                 setCaption( "Waiting" );
41                 qApp->unlock();
42 
43                 // 等待直到我们被告知可以继续
44                 **mycond.wait**();
45 
46                 // 如果我们到了这里，我们已经被另一个线程唤醒……让我们来设置标题来表明我们正在工作
47                 qApp->lock();
48                 setCaption( "Working!" );
49                 qApp->unlock();
50 
51                 // 这可能会占用一些时间，几秒、几分钟或者几小时等等，因为这个一个和GUI线程分开的线程，在处理事件时，GUI线程不会停下来……
52                 do_complicated_thing();
53             }
54         }
55     };
56 
57     // 主线程——所有的GUI事件都由这个线程处理。
58     int main( int argc, char **argv )
59     {
60         QApplication app( argc, argv );
61 
62         // 创建一个worker……当我们这样做的时候，这个worker将在一个线程中运行
63         Worker firstworker( 0, "worker" );
64 
65         app.setMainWidget( &worker );
66         worker.show();
67 
68         return app.exec();
69     }
70
```
**7.线程安全类**

**非线程安全类**
    这个类不是线程安全的，因为假如多个线程都试图修改数据成员 n,结果未定义。这是因为c++中的++和--操作符不是原子操作。实际上，它们会被扩展为三个机器指令：
1，把变量值装入寄存器
2，增加或减少寄存器中的值
3，把寄存器中的值写回内存
    假如线程A与B同时装载变量的旧值，在寄存器中增值，回写。他们写操作重叠了，导致变量值仅增加了一次。很明显，访问应该串行化：A执行123步骤时不应被打断。
```
class Counter
{
  public:
      Counter() {n=0;}
      void increment() {++n;}
      void decrement() {--n;}
      int value() const {return n;}
 private:
      int n;
};
```
**线程安全类**
```
class Counter
 {
 public:
     Counter() { n = 0; }
     void increment() { QMutexLocker locker(&mutex); ++n; }
     void decrement() { QMutexLocker locker(&mutex); --n; }
     int value() const { QMutexLocker locker(&mutex); return n; }
 private:
     mutable QMutex mutex;
     int n;
 };
```
   QMutexLocker类在构造函数中自动对mutex进行加锁，在析构函数中进行解锁。随便一提的是，mutex使用了mutable关键字来修饰，因为我们在value()函数中对mutex进行加锁与解锁操作，而value()是一个const函数。
引用：http://www.cnblogs.com/NeuqUstcIim/archive/2008/08/02/1258871.html
