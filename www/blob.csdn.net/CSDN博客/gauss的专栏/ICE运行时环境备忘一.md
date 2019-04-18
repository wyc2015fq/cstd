# ICE运行时环境备忘一 - gauss的专栏 - CSDN博客
2014年04月25日 22:29:15[gauss](https://me.csdn.net/mathlmx)阅读数：306
转:http://summer85.iteye.com/blog/1601232
1、ICE运行环境的唯一句柄Communicator对象，其用来管理ICE运行环境下所有对象资源，包含但不限于：
   A、客户端线程池，主要用来进行AMI方法的回调接口调用，防止环形调用产生的死锁
   B、服务器端的线程池，主要用来接收和处理客户端的接口调用请求
   C、ICE运行环境对应的属性集合
   D、对象工厂，主要用来根据实例化接口对象
   E、日志对象和统计对象，其实现日志接口和统计接口用以实现ICE运行过程中的日志记录和网络传输统计
   F、缺省的路由器和定位器
   G、插件管理对象，用来管理ICE运行环境下的插件对象
   H、对象适配器ObjectAdapter，用以维护对象标示和服务对象之间的映射关系
2、Communicator对象的定义类似如下：
class CommunicatorI : public Communicator
{
public:
virtual void destroy();                   // 销毁通信器以及其所管理的ICE运行资源
virtual void shutdown();                  // 关闭通信器，不再接收客户端请求，但是正在执行的请求会执行完成
virtual void waitForShutdown();           // 等待当前通信器结束，意味着正在执行的所有请求都执行完毕
virtual bool isShutdown() const;          // 判断通信器是否已经关闭
virtual ObjectPrx stringToProxy(const std::string&) const;    // 根据对象标示获取对象的代理
virtual std::string proxyToString(const ObjectPrx&) const;    // 根据对象代理获取对象的标示
virtual ObjectPrx propertyToProxy(const std::string&) const;  // 根据给定的属性名称找到对应值的代理对象
virtual PropertyDict proxyToProperty(const ObjectPrx&, const std::string&) const;  // 返回代理对象的属性集
virtual Identity stringToIdentity(const std::string&) const;  // 将字符串标示转换成唯一ID
virtual std::string identityToString(const Identity&) const;  // 将唯一ID转换成字符串
virtual ObjectAdapterPtr createObjectAdapter(const std::string&);  // 通过名字创建对象适配器，端口在配置文件中配置(name.Endpoints)
virtual ObjectAdapterPtr createObjectAdapterWithEndpoints(const std::string&, const std::string&); //通过名称和端口创建适配器
virtual ObjectAdapterPtr createObjectAdapterWithRouter(const std::string&, const RouterPrx&);  // 通过名字和路由器来创建适配器
virtual PropertiesPtr getProperties() const;    // 获取通信器的属性集合
virtual LoggerPtr getLogger() const;            // 获取日志接口对象
virtual StatsPtr getStats() const;              // 获取统计接口对象
};
3、通信器Communicator的创建和初始化方法：
    Ice::CommunicatorPtr ic = Ice::Initialize(argc, argv, initdata);       // 通过命令行参数创建，会解析命令行中ICE相关属性
Ice::CommunicatorPtr ic = Ice::Initialize(initdata);                   // 通过初始化对象创建
Ice::InitializationData initdata;                                      // 构建初始化对象
initdata.properties = Ice::createProperties(args);                     // 设置初始化对象的属性集合
initdata.logger = new MyLoggerI;                                       // 设置对应的日志接口
initdata.stats = new MyStatsI;                                         // 设置对应的统计接口
4、对象适配器是ICE运行环境用来维护ASM的对象，管理ICE运行环境Activate Servant Map
   每个对象适配器有一个和多个接入端点可以接入
   每个对象适配器默认采用communicator的服务器线程进行消息处理，当然也可以通过指定<adapter-name>.ThreadPool.Size来指定自己的线程池
   可以通过Communicator中如下的方法创建一个ObjectAdapter对象：
   createObjectAdapter、createObjectAdapterWithEndpoints、createObjectAdapterWithRouter   
   Adapter对象提供了getName方法获取对象名称、getCommunicator方法获取其所在的通信器对象
   对象适配器是一个有状态的对象，其包含状态包括：Holding(初始状态，不接收请求)、Active(激活状态，正常处理)、Inactive(去激活状态，对象即将销毁)
   对象适配器提供了一系列的API来管理其状态转换：activate、hold、deactivate、waitForHold、waitForDeactivate、destory   
   对象适配器均有一个地址与其对应，类似如："tcp -h 10.0.0.1 -p 35840 -t 5000 -z:tcp -h 10.0.0.2 -p 35840 -t 5000 -z" (绑定两个地址，超时5s，支持压缩)
   可以调用objectadapter的getEndpoints返回所绑定的地址信息
5、Servant对象时真正的接口服务对象，要想提供服务，需要将Servant对象加入到某一个objectAdapter中，并指定一个对象标示
    virtual ObjectPrx add(const ObjectPtr&, const Identity&);      // 指定ID添加对象，一般调用该方法
    virtual ObjectPrx addWithUUID(const ObjectPtr&);               // 自动生成对象标示来添加对象，仅仅临时的服务对象调用该方法添加
virtual ObjectPtr remove(const Identity&);                     // 从objectadapter的ASM中移除一个服务对象
    virtual ObjectPrx createProxy(const Identity&) const;          // 创建servant对应的代理对象
    virtual ObjectPrx createDirectProxy(const Identity&) const;    // 创建servant的直接代理对象
    virtual ObjectPrx createIndirectProxy(const Identity&) const;  // 创建servant的间接代理对象
6、在生成的ICE接口方法代码中，每个映射方法的后面都携带了一个current参数，Current的定义如下：
   module Ice 
   {
        local dictionary<string, string> Context;
        enum OperationMode { Normal, \Idempotent };
        local struct Current 
   {
            ObjectAdapter adapter;    // 服务器的对象适配器，可以通过它再调用getCommunicator得到通信器
Connection con;           // 连接对象
            Identity id;              // 服务对象标示
            string facet;             // 
            string operation;         // 操作的名称
            OperationMode mode;       // 操作模式
            Context ctx;              // 操作上下文属性
int requestId;            // 请求ID
        };
    };
