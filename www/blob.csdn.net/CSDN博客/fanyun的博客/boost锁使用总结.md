# boost锁使用总结 - fanyun的博客 - CSDN博客
2016年05月19日 13:29:57[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：9386
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
 boost锁的概述：
boost库中提供了mutex类与lock类，通过组合可以轻易的构建读写锁与互斥锁。
        举个通俗的例子，如果共享资源是一个自动锁住的房间，互斥体是钥匙，进入房间必须取钥匙，离开房间应该还钥匙。这就对应着互斥体的lock（取钥匙）和unlock（还钥匙）。
动态分配内存存在类似的情况。如果忘记delete，会导致内存泄漏。它是如何解决的？在栈上分配对象，要一个特点，那就是离开作用域后，对象肯定要调用析构方法。利用这个特点，可以使用对象对指针封装，在对象的析构方法中进行delete，就保证了一定会执行delete。这就是智能指针。因为智能指针，不是针对一个特定类型的指针，因此把智能指针设计为类的模版，根据模版实参特例化一个模板类。同样道理，也可以使用相同技术，对互斥体封装。这就是
 lock类模版。在lock的构造方法调用互斥体的lock方法，在lock的析构方法调用互斥体的unlock方法。mutex是对象类，lock是模板类。
** mutex对象类：**
      mutex类主要有两种：boost::mutex，boost::shared_mutex，其中mutex有lock和unlock方法，shared_mutex除了提供lock和unlock方法外，还有shared_lock和shared_unlock方法。因此，boost::mutex为独占互斥类，boost::shared_mutex为共享互斥类。
**lock模板类：**
      boost::unique_lock<T>，boost::shared_lock<T>，其中unique_lock为独占锁，shared_lock为共享锁。unique_lock<T>中的T可以为mutex类中的任意一种，如果为shared_mutex，那么boost::unique_lock<boost::shared_mutex>类的对象构造函数构造时，会自动调用shared_mutex的shared_lock方法，析构函数里，会自动调用shared_mutex的shared_unlock方法。如果是boost::
 unique_lock<boost::mutex>，则分别自动调用lock和unlock方法。
boost::shared_lock<T>中的T只能是shared_mutex类。
**读写锁的实现：**
typedefboost::shared_lock<boost::shared_mutex> readLock;
typedef boost::unique_lock<boost::shared_mutex> writeLock;
boost::shared_mutex rwmutex;
void readOnly( )
{
       readLock  rdlock( rwmutex );
       /// do something
}
void writeOnly( )
{
       writeLock  wtlock( rwmutex );
       /// do something
}
      对同一个rwmutex，线程可以同时有多个readLock，这些readLock会阻塞任意一个企图获得writeLock的线程，直到所有的readLock对象都析构。如果writeLock首先获得了rwmutex，那么它会阻塞任意一个企图在rwmutex上获得readLock或者writeLock的线程。boost::shared_lock使用要小心，千万不要同一个线程多次进入。
**互斥锁的实现：**
typedef boost::unique_lock<boost::mutex> exclusiveLock;
**递归式的互斥量：**
      boost::recursive_mutex提供一个递归式的互斥量。对于一个实例最多允许一个线程拥有其锁定，如果一个线程已经锁定一个boost::recursive_mutex实例，那么这个线程可以多次锁定这个实例。
boost::mutex::scoped_lock
boost::mutexio_mutex;
void foo( )
{
       {
               boost::mutex::scoped_lock lock( io_mutex);         /// 锁定
       } /// 解锁
}
