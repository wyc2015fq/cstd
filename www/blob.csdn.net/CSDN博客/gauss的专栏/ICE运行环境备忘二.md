# ICE运行环境备忘二 - gauss的专栏 - CSDN博客
2014年04月25日 22:41:46[gauss](https://me.csdn.net/mathlmx)阅读数：485
1、需要理解ICE运行环境对客户端请求的调用分派语义，即当一个请求到来时，如何查找到对应到servant服务对象进行消息处理
   A、在ASM 中查找该标识。如果ASM 有这样一个条目，就把请求分派到对应的servant。
   B、如果到来的对象标识的范畴不是空的，就查找针对该范畴注册的servant定位器。如果有这样的servant 定位器，就调用这个定位器的locate，
      如果locate 返回一个servant，就把请求分派到该servant，然后调用finished ；而如果locate 调用返回null，就在客户端引发
      ObjectNotExistException。
   C、如果到来的对象标识的范畴是空的，或者在找不到针对该范畴的servant 定位器，就去查找缺省的servant 定位器（也就是，针对空范畴
      注册的servant 定位器）。如果有缺省的servant 定位器，就像上一步那样分派请求。
   D、如果上述操作均找不到对应的servant，则在客户端ObjectNotExistException。
2、通过如下的方法在一个Objectadapter中操作servant的定位器：
    virtual void addServantLocator(const ServantLocatorPtr&, const std::string&);  // 指定范畴注册定位器，如果范畴为空，则是默认定位器
    virtual ServantLocatorPtr removeServantLocator(const std::string&);            // 移除定位器，该定位器上不会再有新请求，老请求继续
    virtual ServantLocatorPtr findServantLocator(const std::string&) const;        // 指定范畴查找定位器
3、Servant定位器的实现样例：
class OCCIServantLocator : public Ice::ServantLocator
{
public:
virtual Ice::ObjectPtr locate(const Ice::Current&, Ice::LocalObjectPtr&);   
virtual void finished(const Ice::Current&, const Ice::ObjectPtr&, const Ice::LocalObjectPtr&);
virtual void deactivate(const std::string&);
};
注意：
A、ICE调用定位器的locate和finished方法是在同一个线程中进行的。（AMD方法除外）
B、如果locate返回了一个servant，那么在servant的操作请求完成时，一定会调用finished方法以方便资源回收
C、可以再locate方法中返回一个cookie对象，那么在finished方法调用时，该cookie会发送给finished方法
D、只有当communicator或者objectadapter对象关闭时，才会调用定位器的deactivate方法
E、如果在定位器对象中存在一些临界资源修改，需要对这些临界资源进行同步互斥保护
F、可以再locate和finished方法中抛出异常，这些异常会抛送给客户端
4、为每个服务操作申明实例化一个服务处理对象时最容易理解的普通做法，但是在一个受限资源系统上是不可接受的。为了解决系统启动时就创建多个
   servant实例所带来的缺陷(可能有些servant压根就不会使用、启动过程中实例化可能导致比较慢的启动)，可以通过servant定位器来实现延迟实例化
   和按需实例化。
Ice::ObjectPtr MyServantLocator::locate(const Ice::Current & c,Ice::LocalObjectPtr &)
{
   IceUtil::Mutex::Lock lock(_m);
   // Check if we have instantiated a servant already.
   Ice::ObjectPtr servant = c.adapter.identityToServant(c.id);
   if(!servant) { // We don't have a servant already
      // Instantiate a servant.
      servant = new ServantI(c.id.name);
      // Add the servant to the ASM.
      c.adapter->add(servant, c.id);
   }
   return servant;
}
4、默认服务处理对象。默认服务处理对象存在的理由时，要为每类请求创建一个对应的servant实例在受限资源系统上是不可接受的。
   那么使用默认的服务处理对象，让所有请求都用一个实例化对象来处理时一种不错的选择，通过如下的方法设置默认服务处理对象：
   virtual void addDefaultServant(const ObjectPtr&, const std::string&);
   在objectadapter上指定特属范畴的默认服务处理对象。
   那么为了能够区分每个服务调用请求，一种常用的做法是：在服务处理函数中通过current.id.name来区分
   每个servant需要有一个ice_Ping方法来判断servant是否还存活，由于默认处理对象合并了多数servant的服务调用，因此需要显示
   的实现ice_Ping方法来判断current.id.name是否还存活。
5、采用servant定位器来实现servant的按需实例化是一种不错的改进，但是这样还是存在一个问题，就是庞大的servant实例往往在资源受限系统上
   得不到满足，为此，ICE提供了一种改进的servant定位器"逐出器（evictor）"
   逐出器是一种特殊的servant定位器，除了实现servant定位器的虚拟接口locate和finished之外，还实现了在内部管理了一个所谓的ASM，
   而不是用Objectadapter上的默认ASM来存储ASM，同时在内部的ASM上实现了LRU的淘汰算法。
   实现一个逐出器的方法如下：
class MyEvictor : public Ice::EvictorBase
{
public:
virtual Ice::ObjectPtr add(const Ice::Current&, Ice::LocalObjectPtr&);
virtual void evict(const Ice::ObjectPtr&, const Ice::LocalObjectPtr&);
};
注意：
A、EvictorBase的回调方法add、evict和ServantLocator的回调方法locate、finished功能类似
B、注意不要试图在add方法中做类似locate方法中添加servant到ASM中
C、add和evict方法都支持cookie参数的传递，可以传递用户指定的上下文信息
D、在类似如数据库代理的服务系统中，要为每条记录创建一个servant对象不太现实，使用默认对象又不太高效，使用Evictor是一个不错的选择
转载:http://blog.csdn.net/followshake/article/details/7392424
