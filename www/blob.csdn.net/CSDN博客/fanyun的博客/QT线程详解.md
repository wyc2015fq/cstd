# Qt线程详解 - fanyun的博客 - CSDN博客
2017年06月30日 20:03:00[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2475
线程与进程有什么区别呢？
         举个简单的例子，当你在LOL的时候，在相同的桌面上可能有一个播放器正在播放你最喜欢的歌曲。这是一个两个进程并行工作的例子：一个进程运行LOL程序;另一个进程运行一个媒体播放器。这种情况最适合用多任务这个词来描述。进一步观察播放器，你会发现在这个进程内，又存在并行的工作。当播放器向音频驱动发送音乐数据的时候，用户界面上与之相关的歌词信息不断地进行更新。这就是单个进程内的并行线程。应用程序可以使用多线程技术来提高程序性能，改善用户体验。尤其是在多核处理器中，多个线程可以在多个CPU上同时运行，能够有效提高程序的运行速度。如果一个应用程序需要处理一些耗时的数据处理计算，应该用一个独立的线程来做这些运算，而用另外一个应用程序来处理和用户的交互。这样，用户可以提前中断或者暂停数据处理工作，或者可以做些和数据计算无关的工作，提高用户的工作效率。
        那么，线程的并行性是如何实现的呢？在单核CPU计算机上，并行工作类似在电影院中不停移动图像产生的一种假象。对于进程而言，在很短的时间内中断占有处理器的进程就形成了这种假象。然而，处理器迁移到下一个进程。为了在不同进程之间进行切换，当前程序计算器被保存，下一个程序计算器被加载进来。这还不够，相关寄存器以及一些体系结构和操作系统特定的数据也要进行保存和重新加载。
        就像一个CPU可以支撑两个或多个进程一样，同样也可以让CPU在单个进程内运行不同的代码片段。当一个进程启动时，它问题执行一个代码片断从而该进程就被认为是拥有了一个线程。但是，该程序可以会决定启动第二个线程。这样，在一个进程内部，两个不同的代码序列就需要被同步处理。通过不停地保存当前线程的程序计数器和相关寄存器，同时加载下一个线程的程序计数器和相关寄存器，就可以在单核CPU上实现并行。在不同活跃线程之间的切换不需要这些线程之间的任何协作。当切换到下一个线程时，当前线程可能处于任一种状态。
       当前CPU设计的趋势是拥有多个核。一个典型的单线程应用程序只能利用一个核。但是，一个多线程程序可被分配给多个核，便得程序以一种完全并行的方式运行。这样，将一个任务分配给多个线程使得程序在多核CPU计算机上的运行速度比传统的单核CPU计算机上的运行速度快很多。何为线程 、GUI线程和工作者线程 ？数据的同步访问何时使用线程，何时不应使用线程？该使用哪种Qt线程技术？ Qt 线程基础 QObject和Threads
 保护数据的完整性！处理异步执行示例和 深入研究更多参考GUI线程和工作者线程GUI线程和工作者线程。
       如上所述，每个程序启动后就会拥有一个线程。该线程称为”主线程”（在Qt应用程序中也叫”GUI线程”）。Qt GUI必须运行在此线程上。所有的图形元件和几个相关的类，如QPixmap，不能工作于非主线程中。非主线程通常称为”工作者线程”，因为它主要处理从主线程中卸下的一些工作。如数据的同步访问。
       每个线程都有自己的栈，这意味着每个线程都拥有自己的调用历史和本地变量。不同于进程，同一进程下的线程之间共享相同的地址空间。下图1-1显示了内存中的线程块图。非活跃线程的程序计数器和相关寄存器通常保存在内核空间中。对每个线程来说，存在一个共享的代码片段和一个单独的栈。
![](https://img-blog.csdn.net/20170613222652403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                           图 1-1 
        如果两个线程拥有一个指向相同对象的指针，那么两个线程可以同时去访问该对象，这可以破坏该对象的完整性。很容易想象的情形是一个对象的两个方法同时执行可能会出错。
        有时，从不同线程中访问一个对象是不可避免的。例如，当位于不同线程中的许多对象之间需要进行通信时。由于线程之间使用相同的地址空间，线程之间进行数据交换要比进程之间进行数据交换快得多。数据不需要序列化然后拷贝。线程之间传递指针是允许的，但是必须严格协调哪些线程使用哪些指针。禁止在同一对象上执行同步操作。有一些方法可以实现这种要求，下面描述其中的一些方法。
       那么，怎样做才安全呢？在一个线程中创建的所有对象在线程内部使用是安全的，前提条件是其他线程没有引用该线程中创建的一些对象且这些对象与其他的线程之间没有隐性耦合关系。当数据作为静态成员变量，单例或全局数据方式共享时，这种隐性耦合是可能发生的。
       使用线程基本上，对线程来讲，有两种使用情形:
       将一些处理时间较长或阻塞的任务移交给其他的线程，从而保证GUI线程或其他对时间敏感的线程保持良好的反应速度。何时不应使用线程。开发者在使用线程时必须特意小心。启动其他线程很容易，但很难保证所有共享的数据仍然是一致的。这些问题通常很难找到，因为它们可以在某个时候仅显示一次或仅在某种硬件配置下出现。在创建线程解决某些问题之前，如下的一些方法也应该考虑一下。
![](https://img-blog.csdn.net/20170613222825291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         一般来讲，建议只使用安全的且已被验证过的路径，避免引入线程概念。 QtConcurrent提供了一种简易的接口，来将工作分配到所有的处理器的核上。线程相关代码已经完全隐藏在QtConcurrent 框架中，因此，开发者不需要关注这些细节。但是, QtConcurrent 不能用于那么需要与运行中的线程进行通信的情形，且它也不能用于处理阻塞操作。
          该使用哪种 Qt 线程技术?
         有时，我们不仅仅只是在另一个线程中运行一个方法。可能需要位于其他线程中的某个对象为GUI线程提供服务。也许，你想其他的线程一直保持活跃状态去不停地轮询硬件端口并在一个需要关注的事件发生时发送一个信号给GUI线程。Qt提供了不同的解决方案来开发多线程应用程序。正确的解决方案取决于新线程的目的以及它的生命周期。
![](https://img-blog.csdn.net/20170613222933250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
              QThread 是对本地平台线程的一个非常好的跨平台抽象。启动一个线程非常简单。让我们看一段代码，它产生另一个线程，该线程打印data，然后退出。
```cpp
// Userthread.h  
class UserThread : public QThread 
{ 
    Q_OBJECT 
private: 
    void run(); 
};
```
              我们从QThread 中派生一个类并重载run()方法。
```cpp
// Userthread.cpp  
 void UserThread::run() 
 { 
      qDebug() <<"data from worker thread " << thread()->currentThreadId(); 
 }
```
           run方法中包含的代码会运行于一个单独的线程。在本例中，一条包含线程ID的信号将会被输出来。QThread::start() 会在另一个线程中调用该方法。
```cpp
int main(int argc, char *argv[]) 
 { 
     QCoreApplication app(argc, argv); 
     Userthread  thread; 
     thread.start(); 
     qDebug() <<"data from GUI thread " <<app.thread()->currentThreadId(); 
     thread.wait();  
     return 0; 
 }
```
         为了启动该线程，我们的线程对象必须被初始化。start() 方法创建了一个新的线程并在新线程中调用重载的run() 方法。在 start() 被调用后，有两个程序计数器走过程序代码。主函数启动，且仅有一个GUI线程运行，它停止时也只有一个GUI线程运行。当另一个线程仍然忙碌时退出程序是一种编程错误，因此， wait方法被调用用来阻塞调用的线程直到run()方法执行完毕。
 下面是运行代码的结果：
**           data from worker thread 0x3b4c**
**           data from GUI thread  0x3598**
QObject 和线程:
         一个 QObject 通常被认为有线程亲和力 或换句话说, 它位于某个线程中。这意味着，在创建的时候， QObject保存了一个指向当前线程的指针。当一个事件利用 postEvent()发出时，该信息就变得有关了。该事件将会被放于对应线程的事件循环中。如果QObject位于的线程没有事件循环，那么事件就不会被传递。
为了启动一个事件循环，exec()必须在 run()里面调用. 线程亲和力可使用moveToThread()来改变。如上所述，开发者从其他线程中调用对象的方法时必须非常小心。线程亲和力并没有改变这种状况。Qt文档标记了几个方法是线程安全的。 postEvent() 是一个很明显的例子。一个线程安全的方法可以在不同的线程中同时被调用。
在没有并行访问方法的情况下，在其他线程中调用对象的非线程安全的方法时可能运行了几千次后才会出现一个并发访问，造成不可预料的行为。编写测试代码并不能完全的保证线程的正确性，但仍然很重要。在Linux中，Valgrind和Helgrind可以侦测线程错误。
 QThread 细节非常有意思:
·        QThread 并不位于新线程 run()执行的位置中。它位于旧线程中。
·        大部分QThread 的方法是线程的控制接口中，并在旧线程中调用。不要使用moveToThread()将这些接口移到新创建的线程中，例如，调用moveToThread(this) 被认为是一种坏的实践。
·        exec()和静态方法usleep(),msleep(), sleep()应在新创建的线程中调用。
         其他的一些定义在QThread  子类中的成员可以在新旧线程中访问。开发者负责协调这些访问。 一种典型的策略是在调用start() 前设置这些成员。一旦工作者线程运行起来，主线程不应当再修改这些成员。当工作者线程停止后，主线程又可以访问些额外的成员。这是一种在线程启动前和停止后传递参数的方便的策略。
         一个 QObject's 父类必须位于相同的线程中。对于run()方法中创建的对象，在这有一个非常惊人的结果。
```cpp
void Userthread::run() 
{ 
     QObject *object1 = new QObject(this);  //error, parent must bein the same thread  
     QObject object2;  // OK  
     QShared Pointer<QObject> object3(new QObject); // OK  
}
```
        使用一个互斥量 来保护数据的完整性。
         一个互斥量是一中且具有lock()和 unlock() 方法的对象，并记住它是否被锁住。互斥量可在多个线程中访问。如果互斥量没有被锁定， lock() 会立即返回。下一个从其他线程的调用会发现互斥量已经处于锁定状态，然后，lock() 会阻塞线程直到其他线程调用 unlock()。该功能可保证一个代码段在同一时间仅能被一个线程执行。
      下面代码显示了怎样使用一个互斥量来确保一个方法是线程安全的。
```cpp
void Worker::work() 
 { 
    this->mutex.lock();  // firstthread can pass, other threads will be blocked here  
     doWork(); 
    this->mutex.unlock(); 
 }
```
         如果一个线程不能解锁一个互斥量会发生什么情况呢？结果是应用程序会僵死。在上面的例子中，可以会抛出异常且永远不会到达mutex.unlock() 。为了防止这种情况，应该使用 QMutexLocker 。
```cpp
void Worker::work() 
{ 
    QMutexLockerlocker(&mutex);  // Locks the mutexand unlocks when locker exits the scope  
    doWork(); 
}
```
         这看上去很简单，但互斥会引入新的问题：死锁。当一个线程等待一个互斥量变为解锁，但是该互斥量仍然处于锁定状态，因为占有该互斥量的线程在等待第一个线程解锁该互斥量。结果是一个僵死的应用程序。互斥量用于保证一个方法是线程安全的。大部分Qt方法不是线程安全的，因为当使用互斥量时总是有些性能损失。
         在一个方法中并不总是能够加锁和解锁一个互斥量。有时，锁定的范围跨越了数个调用。例如，利用迭代器修改一个容器时需要几个调用组成的序列，这个序列不能被其他线程中断。在这种情况下，利用外部锁就可以保证这个调用序列是被锁定的。利用一个外部锁，锁定的时间可以根据操作的需要进行调整。不好之处是外部锁帮助锁定，但不能强制执行它，因为对象的使用者可能忘记使用它。
         使用事件循环来防止数据崩溃
         Qt的事件循环对线程间通信是一个非常有价值的工具。每个线程可以拥有自己的事件循环。调用另一个线程中的槽的安全方法就是将此调用放在该线程的事件循环中。这确保了目标对象在启动另一方法前完成了当前正在执行的方法。那么，怎样将一个方法调用放到一个事件循环中呢？Qt有两种方式。一种方式是通过queued信号-槽连接;另一种方式就是利用QCoreApplication::postEvent()发送一个事件。一个queued
 信号-槽连接是一种异步执行的信号槽连接。内部实现是基于发送的事件。信号的参数放置到事件循环中，信号方法会立即返回。
         连接的槽执行的时间取决于事件循环中的基于事件。通过事件循环通信消除了使用互斥量面临的死锁问题。这就是为什么我们建议使用事件循环而不是使用互斥量锁定一个对象。
        处理异步执行
         一种获得工作者线程结果的方式是等待该线程停止。然而，在许多情况下，阻塞的等待是不可接受的。另一种方式是通过发送的事件或queued信号和槽来获得异步结果。这产生了一些开销，因为一个操作的结果并不是出现在下一个代码行，而是在一个位于其他地方的槽中。Qt开发者习惯了这种异步行为
该手册提供了一些例子，演示了在Qt中使用线程的三种基本方法。另外两个例子演示了怎样与一个运行中的线程进行通信以及一个 QObject 可被置于另一个线程中，为主线程提供服务。
       使用 QThread 使用如上所示。
       使用全局的QThreadPool；
        使用 QtConcurrent；
        与GUI线程进行通信；
        在另一个线程的常驻对象为主线程提供服务；
 如下 的例子可以单独地进行编译和运行。
例 1: 使用Thread Pool
        不停地创建和销毁线程非常耗时，可以使用一个线程池。线程池可以存取线程和获取线程。我们可以使用全局线程池写一个与上面相同的"hello thread" 程序 。我们从QRunnable派生出一个类。在另一个线程中运行的代码必须放在重载的QRunnable::run()方法中。
//Userthreadpool/main.cpp  
```cpp
class Work : publicQRunnable 
 { 
 public: 
     void run() 
     { 
         qDebug() <<"data from thread " << QThread::currentThread(); 
     } 
 }; 
 
 int main(int argc, char*argv[]) 
 { 
     QCoreApplication app(argc, argv); 
     Work work; 
    work.setAutoDelete(false); 
     QThreadPool *threadPool =QThreadPool::globalInstance(); 
    threadPool->start(&work); 
     qDebug() <<"data from GUI thread " << QThread::currentThread(); 
    threadPool->waitForDone(); 
     return 0; 
 }
```
         在main()中， 我们实例化了Work, 定位于全局的线程池，使用QThreadPool::start()方法。现在，线程池在另一个线程中运行我们的工作。 使用线程池有一些性能上的优势，因为线程在它们结束后没有被销毁，它们被保留在线程池中，等待之后再次被使用。
例 2: 使用QtConcurrent
// Userconcurrent/main.cpp   
```cpp
void Setdata() 
{ 
    qDebug() <<"data from thread " << QThread::currentThread(); 
} 
 
int main(int argc, char *argv[]) 
{ 
    QCoreApplication app(argc,argv); 
    QFuture<void> future= QtConcurrent::run(data); 
    qDebug() <<"data from GUI thread " << QThread::currentThread(); 
   future.waitForFinished(); 
    return 0; 
}
```
          我们写一个全局的函数hello()来实现工作者代码。QtConcurrent::run()用于在另一个线程中运行该函数。该结果是QFuture。 QFuture 提供了一个方法叫waitForFinished(), 它阻塞主线程直到计算完成。当所需的数据位于容器中时，QtConcurrent才显示它真正的威力。 QtConcurrent 提供了一些函数能并行地处理这些已经成为容器里元素的一些数据。使用QtConcurrent非常类似于应用一个STL算法到某个STL容器类。QtConcurrent
 Map是一个非常简短且清晰的例子，它演示了容器中的图片怎么被扩散到所有核中去处理。对于每个阻塞函数，都同时存在一个非阻塞，异步型函数。异步地获取结果是通过QFuture 和QFutureWatcher来实现的。
例3.电子表
        我们想创建一个电子表应用程序。该应用程序有一个GUI和一个工作者线程。工作者线程每10毫秒检查一下当前的时间。如果格式化的时间发生了变化，该结果会发送给显示时间的GUI线程当中。
         当然， 这是一种过度复杂的方式来设计一个电子表，事实上，一个独立的线程没必要。使用计时器会更好。本例子纯粹是用于教学目的的，演示了从工作者线程向GUI线程进行通信。注意，这种通信方式非常容易，我们仅需要添加一个信号给QThread，然后构建一个queued 信号/槽连接到主线程中。从GUI到工作者线程的方式在下一个例子中演示。
```cpp
int main(int argc, char *argv[]) 
{ 
    QApplication app(argc,argv); 
    // build gui  
    QWidget widget; 
    QLabel *label = newQLabel; 
    QHBoxLayout *layout = newQHBoxLayout(&widget); 
   layout->addWidget(label); 
   widget.setWindowTitle("clock"); 
 
    //instantiate threadobject  
    ClockThreadclockThread; 
   QObject::connect(&clockThread, SIGNAL(sendTime(QString)), label,SLOT(setText(QString)), Qt::QueuedConnection); 
    clockThread.start(); 
    widget.show(); 
    app.exec(); 
    clockThread.quit(); 
    clockThread.wait(); 
    return 0; 
}
```
       将clockThread 与标签连接起来。连接必须是一个queued 信号-槽连接，因为我们想将调用放到事件循环当中。
```cpp
// clock/clockthread.h  
 class ClockThread : publicQThread 
 { 
     Q_OBJECT 
 signals: 
     void sendTime(QStringtime); 
 private: 
     void run(); 
     QString m_lastTime; 
 private slots: 
     void timerHit(); 
 
 };
```
       从 QThread 派生出一个类，并声明sendTime()信号。
```cpp
// clock/clockthread.cpp  
void ClockThread::run() 
{ 
    QTimer timer; 
    connect(&timer,SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection); 
   timer.setInterval(10); 
    timer.start();   // puts one event in the threads eventqueue  
    exec(); 
    timer.stop(); 
} 
 
void ClockThread::timerHit() 
{ 
    QString newTime=QDateTime::currentDateTime().toString("ddd MMMM d yy,hh:mm:ss"); 
    if(m_lastTime != newTime){ 
        m_lastTime =newTime; 
        emit sendTime(newTime); 
    } 
}
```
        该例子中最值得关注的部分是计时器通过一个直接连接与它的槽相连。默认的连接会产生一个queued 信号-槽连接，因为被连接的对象位于不同的线程。记住，QThread并不位于它创建的线程中。但是，从工作者线程中访问ClockThread::timerHit() 仍然是安全的，因为ClockThread::timerHit()是私有的，且只处理私有变量。QDateTime::currentDateTime()
 在Qt文档中并未标记为线程安全的，但是在此例子中，我们可以放心使用，因为我们知道访方法没有会其他的线程中使用。
例 4: 常驻线程
        该例子演示了位于工作者线程中的一个QObject接受来自GUI线程的请求，利用一个计时器进行轮询，并不时地将结果返回给GUI线程。实现的工作包括轮询必须实现在一个从QObject派生出的类中。在如下代码中，我们已称该类为 WorkerObject。线程相关的代码已经隐藏在称为Thread类中，派生自QThread. Thread有两个额外的公共成员。launchWorker() 获取工作者对象并将其移到另一个开启了事件循环的线程中。该调用阻塞一小会，直到创建操作完成，使得工作者对象可以在下一行被再次使用。Thread
 类的代码短但有点复杂，因此我们只显示怎样使用该类。
```cpp
// movedobject/main.cpp  
 int main(int argc, char*argv[]) 
 { 
     QCoreApplicationapp(argc, argv); 
     Thread thread; 
     qDebug() <<"main thread ID: " << app.thread()->currentThreadId(); 
     WorkerObject *worker =new WorkerObject; 
    thread.launchWorker(worker); 
    QMetaObject::invokeMethod(worker, "doWork",Qt::QueuedConnection); 
    QMetaObject::invokeMethod(worker, "startPolling",  Qt::QueuedConnection,  Q_ARG(int, 500)); 
     //let application produceoutput for 3 seconds and quit  
     QTimer::singleShot(3000,&app, SLOT(quit())); 
     app.exec(); 
     thread.stop(); 
     thread.wait(); 
     delete worker; 
     return 0; 
 }
```
         QMetaObject::invokeMethod()通过事件循环调用槽。worker对象的方法不应该在对象被移动到另一个线程中直接调用。我们让工作者线程执行一个工作和轮询，并使用一个计时器在3秒后关闭该应用程序。关闭worker需要当心。我们调用 Thread::stop() 退出事件循环。我们等待线程停止，当线程停止后，我们删除worker。

