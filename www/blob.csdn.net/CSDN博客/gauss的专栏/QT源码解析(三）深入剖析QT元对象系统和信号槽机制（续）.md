# QT源码解析(三）深入剖析QT元对象系统和信号槽机制（续） - gauss的专栏 - CSDN博客
2013年01月03日 18:17:32[gauss](https://me.csdn.net/mathlmx)阅读数：317
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
来自：http://blog.csdn.net/tingsking18/article/details/4991563
我看到过这样的论断： "signal和slot机制与GUI的事件循环完全没有关系，当所有链接到这个signal的slot执行完成之后，在emit代码行之后的代码才会被执行。" "当emit一个信号后，只有在所有与之相连的信号或槽返回后，emit才会返回。" 这2句话说的是同一个意思，但它们对吗？ 1. 这2句话是不是针对单线程的，或者说是在同一个线程中的信号和槽？ 2. 当线程A，B同时向另一个线程C的同一个槽发射信号时，如果线程C没有自己的事件循环，那些发过来的信号会排队吗，还是会丢失？ 如果有事件循环情况又如何？
 3. 接上面的问题2，假设GUI线程是D，线程C的对象存在于D，那C的槽函数是在C中还是在D中被执行的呢？ 4. 线程A在向C发射信号后，会向开头那些话所说的，在C的槽返回之后再执行emit之后的信号么？
发现，在上一篇对信号槽极致的分析还不是那么透彻。下面专门来回答一下[toto07](http://hi.csdn.net/toto07)网友的问题。
首先我们来看一段代码：
**[cpp]**[view plain](http://blog.csdn.net/tingsking18/article/details/4991563#)[copy](http://blog.csdn.net/tingsking18/article/details/4991563#)
- #include <iostream>     
- #include <qobject.h>    
- #include <QtCore>  
- using namespace std ;     
- 
- class Counter : public QObject   
- {     
-     Q_OBJECT     
-         int value ;     
- signals:     
-     void valueChanged( int val);     
- public slots:     
-     void setValue(int val)  
-     {  
-         value = val ;  
-         emit valueChanged( value ) ;  
-         cout<<"Counter emit executed"<<endl;  
-     };     
- public:     
-     Counter(int val) : value( val ){};     
-     Counter():value(0){};  
-     friend std::ostream& operator<< ( std::ostream& os, const Counter& c) {return os << c.value ; };     
- } ;    
- 
- class Counter1 : public QObject   
- {     
-     Q_OBJECT     
-         int value ;     
- 
- public slots:     
-     void slotfunc(int val)  
-     {  
-         int i  = value;  
-         value = val ;  
-         cout<<i<<"  Counter1 slot executed"<<endl;  
-     };     
- public:     
-     Counter1(int val) : value( val ){};     
-     Counter1():value(0){};  
-     friend std::ostream& operator<< ( std::ostream& os, const Counter1& c) {return os << c.value ; };     
- } ;    
- #include "main.moc"  
- int main( int argc, char *argv[] )     
- {     
-     Counter a;  
-     Counter1 b( 10 ) ;     
-     Counter1 b1( 11 ) ;  
-     cout << "We have two counters, " << a<< " and " << b << endl ;     
-     QObject::connect( &a, SIGNAL( valueChanged(int) ),&b, SLOT( slotfunc(int) ) ) ;     
-     QObject::connect( &a, SIGNAL( valueChanged(int) ),&b1, SLOT( slotfunc(int) ) ) ;     
- 
-     a.setValue( 12 ) ;     
-     cout << "We have two counters, " << a << " and " << b << endl ;     
- 
-     b.setValue( 5 ) ;     
-     cout << "We have two counters, " << a << " and " << b << endl ;     
- 
-     int i;  
-     cin>>i;  
-     return 0 ;     
- }    
在pro文件中要指定：
CONFIG += console
QT -= gui
这样，我们就没有使用gui部分。
这是生成的Makefile部分：
LIBS          = d:/qt4.4.3/lib/QtCored4.lib
很明显，我们只用了QtCored4.lib部分，并咩有使用Gui部分。所以说QT的signal、slot机制于Gui没有关系。
下面是运行结果，从运行结果很明显能看得出来：
C:/test1/debug>test1.exe
We have two counters, 0 and 10
10  Counter1 slot executed
11  Counter1 slot executed
Counter emit executed
很明显：
是slot首先执行，然后是emit后面的代码再继续执行，看起来貌似signal和slot是阻塞的。下面我们在看一下[toto07](http://hi.csdn.net/toto07)朋友说的第二种情况：
添加线程类：
**[cpp]**[view plain](http://blog.csdn.net/tingsking18/article/details/4991563#)[copy](http://blog.csdn.net/tingsking18/article/details/4991563#)
- class Producer : public QThread  
- {  
- private:  
-     Counter1* m_c;  
- public:  
-     void run()  
-     {  
-         Counter Producera;  
-         QObject::connect( &Producera, SIGNAL( valueChanged(int) ),m_c, SLOT( slotfunc(int) ) ) ;     
-         Producera.setValue( 12 ) ;  
-     };  
-     void SetCount(Counter1 *c){m_c = c;};  
- public:  
- };  
然后把main函数改成如下形式：
**[cpp]**[view plain](http://blog.csdn.net/tingsking18/article/details/4991563#)[copy](http://blog.csdn.net/tingsking18/article/details/4991563#)
- int main( int argc, char *argv[] )     
- {     
-     Counter1 b1( 11 ) ;   
- 
-     Producer producer;  
-     producer.SetCount(&b1);  
-     Producer producer1;  
-     producer1.SetCount(&b1);  
-     producer.run();  
-     producer1.run();  
-     int i;  
-     cin>>i;  
-     return 0 ;  
- }  
我们再来看一下运行结果：
C:/test1/debug>test1.exe
11  Counter1 slot executed
Counter emit executed
12  Counter1 slot executed
Counter emit executed
和上面的所有的操作在单线程中的结果有所不同，这时候是slot执行之后，直接返回，继续执行emit后面的代码。
貌似这时候结论已经出来了，单线程和多线程的执行结果不同。就如上面的测试结果一样。但是事实是这样么？
如果你仔细看了我的上一篇QT源码解析的文章：[QT源码解析(二）深入剖析QT元对象系统和信号槽机制](http://blog.csdn.net/tingsking18/archive/2009/11/12/4800828.aspx)
在src/corelib/kernel/qobject.cpp文件的void QMetaObject::activate函数中。
文章：[QT源码解析(二）深入剖析QT元对象系统和信号槽机制](http://blog.csdn.net/tingsking18/archive/2009/11/12/4800828.aspx)也对activate函数做了详细的解释，下面我再解释一下。
**[cpp]**[view plain](http://blog.csdn.net/tingsking18/article/details/4991563#)[copy](http://blog.csdn.net/tingsking18/article/details/4991563#)
- //获取当前线程ID  
- QThread * const currentThread = QThread::currentThread();  
- const int currentQThreadId = currentThread ? QThreadData::get(currentThread)->id : -1;  
- // 记录sender连接的索引  
- QVarLengthArray<int> connections;  
- for (; it != end && it.key() == sender; ++it) {  
-     connections.append(it.value());  
-     // 打上使用标记,因为可能是放在队列中  
-     list->connections[it.value()].inUse = 1;  
- }  
- //遍历所有的链接  
- for (int i = 0; i < connections.size(); ++i) {  
-     const int at = connections.constData()[connections.size() - (i + 1)];  
-     QConnectionList * const list = ::connectionList();  
-     // 得到连接  
-     QConnection &c = list->connections[at];  
-     c.inUse = 0;  
-     //判断连接是否合法  
-     if (!c.receiver || (c.signal < from_signal_index || c.signal > to_signal_index))  
-         continue;  
-     // 判断是否放到队列中  
-     // determine if this connection should be sent immediately or  
-     // put into the event queue  
-     //这里就是最重要的地方。  
-     if ((c.type == Qt::AutoConnection  
-          && (currentQThreadId != sender->d_func()->thread  
-              || c.receiver->d_func()->thread != sender->d_func()->thread))  
-         || (c.type == Qt::QueuedConnection)) {  
-          //进入队列。下一篇再详细解释这里。  
-         ::queued_activate(sender, c, argv);  
-         continue;  
-     }  
-    // 为receiver设置当前发送者  
-     const int method = c.method;  
-     QObject * const previousSender = c.receiver->d_func()->currentSender;  
-     c.receiver->d_func()->currentSender = sender;  
-     list->lock.unlock();  
-     if (qt_signal_spy_callback_set.slot_begin_callback != 0)  
-         qt_signal_spy_callback_set.slot_begin_callback(c.receiver, method, argv ? argv : empty_argv);  
- 
- #if defined(QT_NO_EXCEPTIONS)  
-     c.receiver->qt_metacall(QMetaObject::InvokeMetaMethod, method, argv ? argv : empty_argv);  
- #else  
-     try {  
-         // 直接执行slot的方法。调用receiver的方法  
-         c.receiver->qt_metacall(QMetaObject::InvokeMetaMethod, method, argv ? argv : empty_argv);  
-     } catch (...) {  
-         list->lock.lockForRead();  
-         if (c.receiver)  
-             c.receiver->d_func()->currentSender = previousSender;  
-         throw;  
-     }  
- #endif  
其中最关键的地方就是这里：
    if ((c.type == Qt::AutoConnection
         && (currentQThreadId != sender->d_func()->thread
             || c.receiver->d_func()->thread != sender->d_func()->thread))
        || (c.type == Qt::QueuedConnection))
打开QAssistant，找到：
bool QObject::connect ( const QObject * sender, const char * signal, const QObject * receiver, const char * method, Qt::ConnectionType type = Qt::AutoConnection )   [static]
看到了，原来connect是5个参数，其中第五个参数
Qt::ConnectionType type = Qt::AutoConnection是有默认值的，我们再看一下
Qt::ConnectionType
|**Constant**|**Value**|**Description**|
|----|----|----|
|Qt::DirectConnection|1|When emitted, the signal is immediately delivered to the slot.|
|Qt::QueuedConnection|2|When emitted, the signal is queued until the event loop is able to deliver it to the slot.|
|Qt::BlockingQueuedConnection|4|Same as QueuedConnection, except that the current thread blocks until the slot has been delivered. This connection type should only be used for receivers in a different thread. Note that misuse of this type can lead to dead locks in your application.|
|Qt::AutoConnection|0|If the signal is emitted from the thread in which the receiving object lives, the slot is invoked directly, as with Qt::DirectConnection; otherwise the signal is queued, as with Qt::QueuedConnection.|
Qt支持三种类型的信号-槽连接：
1，直接连接，当signal发射时，slot立即调用。此slot在发射signal的那个线程中被执行(不一定是接收对象生存的那个线程)
2，队列连接，当控制权回到对象属于的那个线程的事件循环时，slot被调用。此slot在接收对象生存的那个线程中被执行
3，自动连接(缺省)，假如信号发射与接收者在同一个线程中，其行为如直接连接，否则，其行为如队列连接。
连接类型可能通过以向connect()传递参数来指定。注意的是，当发送者与接收者生存在不同的线程中，而事件循环正运行于接收者的线程中，使用直接连接是不安全的。同样的道理，调用生存在不同的线程中的对象的函数也是不是安全的。QObject::connect()本身是线程安全的。
