# TAF server的大致组件结构 - gauss的专栏 - CSDN博客
2015年04月14日 14:32:02[gauss](https://me.csdn.net/mathlmx)阅读数：3927
转 http://blog.csdn.net/turkeyzhou/article/details/8759530
**业务与底层网络通信分离**
Server大部分主要分为两层：
- 网络接收层：负责监听端口，负责收包，编码，解码工作，负责将响应包回传给客户端。
- 业务处理层：负责接收网络接收层完整的包，如果是RPCserver，则需要根据上下文请求（由网络接收层构建）中指定的方法名以及参数调用指定服务（需要有完善的异常处理机制），得到响应包，通过网络接收层编码回传给客户端。
![](file:///C:/Users/COREYC~1.TEN/AppData/Local/Temp/enhtmlclip/Image(1).png)
![](https://img-my.csdn.net/uploads/201304/04/1365073965_6391.png)
**TAF server的大致组件结构![](file:///C:\Users\ADMINI~1\AppData\Roaming\duowan\yy\cache\image\c2\61480-~1.JPG)**
- Application：服务需要自行继承该App，代表了一个服务应用。
- EpollServer：实现底层的Epoll网络监听模型以及adapter的注册管理等工作，监听多个端口。
- Epoller:负责Epoll底层实现细节。
- BindApapter：代表一个网络端口的监听，将具体的网络底层处理委托给EpollServer，拥有自己的协议编解码器和业务处理器。
- Protocol：是一个仿函数，表示编码解码具体的协议，可自定义，挂载到Adapter上面。
- ServantHandler：负责将请求交给具体的业务逻辑servant处理，通常接收编解码后的请求包（上下文），挂载到Adapter上。
- ServantHelperManager：是一个单例类，主要用来管理servant类与名字的对应关系，管理adapterName与servant的对应关系。以供EpollServer决定将请求分发给哪一个Adapter处理。
- Servant：表示具体的服务接口，具体业务具体实现。
![](https://img-my.csdn.net/uploads/201304/04/1365073970_1302.png)
Application代表一个应用，我们自己实现的Server继承自此类，自定义初始化操作，并调用main启动，操作包括：
- 忽略PIPE信号。
- 解析应用部署时自动生成的配置文件：解析配置文件。首先根据命令行参数构建一个TC_Option，获取参数--config获取配置文件路径，保存在ServerConfig::*ConfigFile*变量中,利用TC_Config解析配置文件，并将配置加载到Application::conf的静态变量中。
- 初始化客户端部分；
- 初始化服务端部分。
- 绑定业务servant的adapter到EpollServer中。
- 初始化业务应用。
- 设置HandleGroup分组，启动线程。
- 启动业务处理线程。
- 恢复BUS连接。
- 动态加载配置文件
- 动态设置滚动日志等级
- 动态设置按天日志等级
- 查看服务状态
- 查看当前链接状态
- 查看编译的TAF版本
- 加载配置文件中的属性信息
- 查看服务支持的管理命令
- 设置染色信息
- 上报版本
- 发送心跳给node, 表示启动了
- 发送给notify表示服务启动了
- ctrl + c能够完美结束服务
- 重定向 stdin、stdout、 stderr
- 初始化完毕后, 日志再修改为异步
**void****Application::****main**(**int**argc,**char***argv[])
{
**try**
    {
TC_Common::*ignorePipe*();
//解析配置文件
        parseConfig(argc, argv);
//初始化Proxy部分
        initializeClient();
//初始化Server部分
        initializeServer();
vector<TC_EpollServer::BindAdapterPtr>
 adapters;
//绑定对象和端口
bindAdapter(adapters);
//业务应用的初始化
        initialize();
//输出所有adapter
        outAllAdapter(cout);
//设置HandleGroup分组，启动线程
**for**(size_ti
 = 0; i < adapters.size(); ++i)
        {
stringname = adapters[i]->getName();
stringgroupName = adapters[i]->getHandleGroupName();
**if**(name != groupName)
            {
TC_EpollServer::BindAdapterPtrptr
 =*_epollServer*->getBindAdapter(groupName);
**if**(!ptr)
                {
**throw**runtime_error("[TAF][adater`"+
 name +"` setHandle to group `"+ groupName +"` fail!");
                }
            }
setHandle(adapters[i]);
/*
            if (name == groupName)
            {
                setHandle(adapters[i]);
                continue;
            }
            TC_EpollServer::BindAdapterPtrptr= _epollServer->getBindAdapter(groupName);
            if (!ptr)
            {
                throw runtime_error("[TAF][adater`" + name + "` setHandle
 to group `" + groupName + "` fail!");
            }
            adapters[i]->setHandle(ptr);
            */
        }
//启动业务处理线程
*_epollServer*->startHandle();
*_epollServer*->createEpoll();
//恢复BUS连接
cout <<"begin BusConnectRecover"<<endl;
           BusConnectRecover();
cout <<"end BusConnectRecover"<<endl;
        cout <<"\n"<<outfill("[initialize
 server] ",'.')  <<" [Done]"<<endl;
        cout << OUT_LINE_LONG <<endl;
//动态加载配置文件
TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_LOAD_CONFIG,Application::cmdLoadConfig);
//动态设置滚动日志等级
TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_SET_LOG_LEVEL,Application::cmdSetLogLevel);
//动态设置按天日志等级
TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_SET_DAYLOG_LEVEL,Application::cmdEnableDayLog);
//查看服务状态
TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_VIEW_STATUS,Application::cmdViewStatus);
//查看当前链接状态
TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_CONNECTIONS,Application::cmdConnections);
//查看编译的TAF版本
TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_VIEW_VERSION,Application::cmdViewVersion);
//加载配置文件中的属性信息
TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_LOAD_PROPERTY,Application::cmdLoadProperty);
//查看服务支持的管理命令
TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_VIEW_ADMIN_COMMANDS,Application::cmdViewAdminCommands);
//设置染色信息
TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_SET_DYEING,Application::cmdSetDyeing);
TAF_ADD_ADMIN_CMD_PREFIX(TAF_CMD_CLOSE_CORE,Application::cmdCloseCoreDump);
//上报版本
TAF_REPORTVERSION(TAF_VERSION);
//发送心跳给node, 表示启动了
TAF_KEEPALIVE("");
//发送给notify表示服务启动了
TafRemoteNotify::*getInstance*()->report("restart");
//ctrl+ c能够完美结束服务
signal(SIGINT,
 sighandler);
**if**(*_conf*.get("/taf/application/server<closecout>",AppCache::*getInstance*()->get("closeCout"))
 !="0")
        {
// 重定向stdin、stdout、stderr
**int**fd
 =open("/dev/null",O_RDWR);
**if**(fd != -1)
            {
dup2(fd, 0);
dup2(fd, 1);
dup2(fd, 2);
            }
**else**
            {
close(0);
close(1);
close(2);
            }
        }
    }
**catch**(exception&ex)
    {
TafRemoteNotify::*getInstance*()->report("exit:
 "+string(ex.what()));
cout<<"[main exception]:"<<
 ex.what() <<endl;
*terminate*();
    }
//初始化完毕后, 日志再修改为异步
TafRollLogger::*getInstance*()->sync(**false**);
}
接下来，我们分析其中的一些关键步骤。
(1)忽略PIPE信号。为何需要忽略PIPE信号呢？对一个对端已经关闭的socket调用两次write, 第二次将会生成SIGPIPE信号, 该信号默认结束进程.具体的分析可以结合TCP的"四次握手"关闭. TCP是全双工的信道, 可以看作两条单工信道, TCP连接两端的两个端点各负责一条. 当对端调用close时,
 虽然本意是关闭整个两条信道, 但本端只是收到FIN包. 按照TCP协议的语义, 表示对端只是关闭了其所负责的那一条单工信道, 仍然可以继续接收数据. 也就是说, 因为TCP协议的限制, 一个端点无法获知对端的socket是调用了close还是shutdown.对一个已经收到FIN包的socket调用read方法, 如果接收缓冲已空, 则返回0, 这就是常说的表示连接关闭. 但第一次对其调用write方法时, 如果发送缓冲没问题, 会返回正确写入(发送).
 但发送的报文会导致对端发送RST报文, 因为对端的socket已经调用了close, 完全关闭, 既不发送, 也不接收数据. 所以, 第二次调用write方法(假设在收到RST之后), 会生成SIGPIPE信号, 导致进程退出.为了避免进程退出, 可以捕获SIGPIPE信号, 或者忽略它, 给它设置SIG_IGN信号处理函数:
signal(SIGPIPE, SIG_IGN);
这样, 第二次调用write方法时, 会返回-1, 同时errno置为SIGPIPE. 程序便能知道对端已经关闭.
在linux下写socket的程序的时候，如果尝试send到一个disconnected socket上，就会让底层抛出一个SIGPIPE信号。
这个信号的缺省处理方法是退出进程，大多数时候这都不是我们期望的。因此我们需要重载这个信号的处理方法。调用以下代码，即可安全的屏蔽SIGPIPE：
signal （SIGPIPE， SIG_IGN）；
对于产生信号，我们可以在产生信号前利用方法 signal(int signum, sighandler_t handler) 设置信号的处理。如果没有调用此方法，系统就会调用默认处理方法：中止程序，显示提示信息(就是我们经常遇到的问题)。我们可以调用系统的处理方法，也可以自定义处理方法。 
系统里边定义了三种处理方法： 
- SIG_DFL信号专用的默认动作:
  (a)如果默认动作是暂停线程，则该线程的执行被暂时挂起。当线程暂停期间，发送给线程的任何附加信号都不交付，直到该线程开始执行，但是SIGKILL除外。
  (b)把挂起信号的信号动作设置成SIG_DFL，且其默认动作是忽略信号 (SIGCHLD)。
- SIG_IGN忽略信号
  (a)该信号的交付对线程没有影响
  (b)系统不允许把SIGKILL或SIGTOP信号的动作设置为SIG_DFL
- SIG_ERR   
**void****TC_Common::****ignorePipe**()
{
**struct**sigactionsig;
    sig.sa_handler=SIG_IGN;
    sig.sa_flags= 0;
sigemptyset(&sig.sa_mask);
sigaction(SIGPIPE,&sig,NULL);
}
(3)中是获取配置文件的default属性，利用CommunicatorFactory构建communicator，初始化通信器。
(4)中将配置文件的该APP的信息都设置到ServerConfig的静态成员属性中。将ServerConfig::LocalIP属性设置为本地所有IP地址的第一个非127.0.0.1回环地址以外的值。
初始化是否防空连接以及空连接超时等选项/taf/application/server<emptyconcheck>，/taf/application/server<emptyconntimeout>。默认为false和3
缓存应用的数据文件，数据文件路径为/taf/application/server<datapath>/${servername}.tafdat路，将文件内容缓存到AppCache的_tFileCache变量中。并且设置本地日志级别，初始化到LogServer代理，初始化到配置中心代理，初始化到Node的代理，构造一个EpollServer,这个很重要。将AdminServant的管理Servant对应的Adapter绑定到EpollServer上。EndPoint是在配置文件的/taf/application/server<local>指定的。该Adapter的handle线程只有1.将Adapter绑定到EpollServer中。
**if**(!ServerConfig::*Local*.empty())
    {
ServantHelperManager::*getInstance*()->addServant<AdminServant>("AdminObj");
ServantHelperManager::*getInstance*()->setAdapterServant("AdminAdapter","AdminObj");
TC_EpollServer::BindAdapterPtrlsPtr
 =**new**TC_EpollServer::BindAdapter(*_epollServer*.get());
        lsPtr->setName("AdminAdapter");
        lsPtr->setEndpoint(ServerConfig::*Local*);
        lsPtr->setMaxConns(TC_EpollServer::BindAdapter::*DEFAULT_MAX_CONN*);
        lsPtr->setQueueCapacity(TC_EpollServer::BindAdapter::*DEFAULT_QUEUE_CAP*);
        lsPtr->setQueueTimeout(TC_EpollServer::BindAdapter::*DEFAULT_QUEUE_TIMEOUT*);
        lsPtr->setProtocolName("taf");
        lsPtr->setProtocol(AppProtocol::*parse*);
        lsPtr->setHandleGroupName("AdminAdapter");
        lsPtr->setHandleNum(1);
        lsPtr->setHandle<ServantHandle>();
*_epollServer*->bind(lsPtr);
    }
采用的协议为AppProtocol::parse,采用的处理器为默认的Servanthandler。Servant对应的Obj名称为AdminObj，Adapter的名称为AdminAdapter。其中缺省的数据值为
**enum**
        {
*DEFAULT_QUEUE_CAP*      
 = 10*1024,   /**流量*/
*MIN_QUEUE_TIMEOUT*      
 = 3*1000,    /**队列最小超时时间(ms)*/
*DEFAULT_MAX_CONN*       
 = 1024,      /**缺省最大连接数*/
*DEFAULT_QUEUE_TIMEOUT*  
 = 60*1000,   /**缺省的队列超时时间(ms)*/
        };
(5)中主要将配置文件中的业务Servant以及参数配置生成Adapter绑定到EpollServer中。主要是几点值得注意
Servant的Obj名称必须符合App.ServerName.的格式，ServantHelperManager会把Servant的Obj名称与AdapterName对应管理起来，而Obj与Servant类对应起来，需要Application的initialize的addServant<XXXXServant>来做。另外不同于AdminServant绑定在LocalIP的EndPoint上，这里需要在Servant中指定EndPoint。如果该Servant的配置中指定Protocol为taf，则使用AppProtocol::parse来进行协议解析。此处并没有设置handler。将各种Adapter绑定到EpollServer中。并且Application::adapters会维护除了AdminAdapter以外的所有业务Adapter。
**void****Application::bindAdapter**(vector<TC_EpollServer::BindAdapterPtr>&
 adapters)
{
stringsPrefix =ServerConfig::*Application*+"."+ServerConfig::*ServerName*+".";
vector<string>adapterName;
map<string,ServantHandle*>
 servantHandles;
**if**(*_conf*.getDomainVector("/taf/application/server",adapterName))
    {
**for**(size_ti
 = 0; i <adapterName.size(); i++)
        {
stringservant =*_conf*.get("/taf/application/server/"+adapterName[i]
 +"<servant>");
checkServantNameValid(servant, sPrefix);
//TC_EpollServer::BindAdapterPtr bindAdapter = bind("/taf/application/server",
 adapterName[i], servant);
ServantHelperManager::*getInstance*()->setAdapterServant(adapterName[i],
 servant);
TC_EpollServer::BindAdapterPtrbindAdapter
 =**new**TC_EpollServer::BindAdapter(*_epollServer*.get());
stringsLastPath ="/taf/application/server/"+adapterName[i];
            bindAdapter->setName(adapterName[i]);
            bindAdapter->setEndpoint(*_conf*[sLastPath
 +"<endpoint>"]);
            bindAdapter->setMaxConns(TC_Common::strto<**int**>(*_conf*.get(sLastPath
 +"<maxconns>","128")));
            bindAdapter->setOrder(parseOrder(*_conf*.get(sLastPath
 +"<order>","allow,deny")));
            bindAdapter->setAllow(TC_Common::sepstr<string>(*_conf*[sLastPath
 +"<allow>"],";,",**false**));
            bindAdapter->setDeny(TC_Common::sepstr<string>(*_conf*.get(sLastPath
 +"<deny>",""),";,",**false**));
            bindAdapter->setQueueCapacity(TC_Common::strto<**int**>(*_conf*.get(sLastPath
 +"<queuecap>","1024")));
            bindAdapter->setQueueTimeout(TC_Common::strto<**int**>(*_conf*.get(sLastPath
 +"<queuetimeout>","10000")));
            bindAdapter->setProtocolName(*_conf*.get(sLastPath
 +"<protocol>","taf"));
**if**(bindAdapter->isTafProtocol())
            {
                bindAdapter->setProtocol(AppProtocol::*parse*);
            }
            bindAdapter->setHandleGroupName(*_conf*.get(sLastPath
 +"<handlegroup>",adapterName[i]));
            bindAdapter->setHandleNum(TC_Common::strto<**int**>(*_conf*.get(sLastPath
 +"<threads>","0")));
            bindAdapter->setShm(TC_Common::strto<**int**>(*_conf*.get(sLastPath
 +"<shmkey>","0")),TC_Common::strto<**int**>(*_conf*.get(sLastPath
 +"<shmcap>","0")));
            bindAdapter->setBusCommuPath(*_conf*.get(sLastPath
 +"<buscommupath>",ServerConfig::*DataPath*));
            bindAdapter->setBusCommuSize(TC_Common::strto<size_t>(*_conf*.get(sLastPath
 +"<buscommucap>","1051648")));
            bindAdapter->setBusCommuEnable(*_conf*.get(sLastPath
 +"<buscommuenable>","N")
 =="Y");
            bindAdapter->setLoadFactor(TC_Common::strto<size_t>(*_conf*.get("/taf/application/server<loadfactor>","100")));
            bindAdapter->initialize();
*_epollServer*->bind(bindAdapter);
            adapters.push_back(bindAdapter);
//队列取平均值
**if**(!*_communicator*->getProperty("property").empty())
            {
*_communicator*->getStatReport()->createPropertyReport(bindAdapter->getName()
 +".queue",PropertyReport::avg());
*_communicator*->getStatReport()->createPropertyReport(bindAdapter->getName()
 +".connectRate",PropertyReport::avg());
            }
        }
    }
}
如果使用了共享内存队列
bindAdapter->setShm(TC_Common::strto<**int**>(*_conf*.get(sLastPath
 + "<shmkey>", "0")), TC_Common::strto<**int**>(*_conf*.get(sLastPath
 +  "<shmcap>", "0")));
**void****TC_EpollServer::BindAdapter::****setShm**(**int**iKey,**int**iSize)
{
**if**(iKey
 > 0 && iSize > 0)
    {
_shmQueue.setShmKey(**static_cast**<key_t>(iKey));
_shmQueue.setShmSize(**static_cast**<size_t>(iSize));
_bUseShm=**true**;
    }
}
则在Adapter的initialize方法中，会对共享内存队列进行初始化。
**void****TC_EpollServer::BindAdapter::****initialize**()
{
**if**(_bUseShm)
    {
_shmQueue.initialize();
    }
}

(6)初始化业务应用中，会执行我们重载的Application的initialize方法，主要包括addServant<XXServant>往ServantHelperManager通过模版泛型指定Servant类型来建立起Obj与ServantCreationPolicy对应关系。
(7)中设置HandleGroup分组，启动线程。在(5)中我们为业务线程并没有指定Handler，这里统一指定，我们获取adapter配置中groupName，该groupName必须有对应的AdpaterName的Adapter存在，为每个Adapter设置Handler为ServantHandler。
**for**(size_ti
 = 0; i <adapters.size(); ++i)
        {
stringname =adapters[i]->getName();
stringgroupName =adapters[i]->getHandleGroupName();
**if**(name
 != groupName)
            {
TC_EpollServer::BindAdapterPtrptr
 =*_epollServer*->getBindAdapter(groupName);
**if**(!ptr)
                {
**throw**runtime_error("[TAF][adater`"+
 name +"` setHandle to group `"+ groupName +"`
 fail!");
                }
            }
setHandle(adapters[i]);
**void****Application::setHandle**(TC_EpollServer::BindAdapterPtr&adapter)
{
adapter->setHandle<ServantHandle>();
}
而Adapter调用setHandler方法，又会连同handleGroupName与handleNum一齐委托给EpollServer的setHandleGroup方法，EpollServer会维护一个groupName与HandlerGroup的KV map。每次当有新的HandleGroupName时，会构建HandleGroup挂载到相应的Adapter上去，并将至放入EpollServer的map中管理起来，并生成对应数目的Handler(此处为ServantHander)放入该HandleGroup中。每个handler都是继承线程的。所以其实HanderGroup就是一个线程池。如果该name的HandleGroup已经存在了，则将adapter将如到该HandleGroup中来，这个Adapter与其他Adapter共享一个HandleGroup（此处的疑问，就是handler的数目并不累加）。
**struct**HandleGroup: **public**TC_HandleBase
    {
stringname;
TC_ThreadLockmonitor;
vector<HandlePtr>           handles;
map<string, BindAdapterPtr> adapters;
    };
**template**<**class****T**>**void****setHandleGroup**(**const**string&
 groupName,int32_thandleNum,BindAdapterPtradapter)
    {
map<string,HandleGroupPtr>::iteratorit
 =_handleGroups.find(groupName);
**if**(it
 ==_handleGroups.end())
        {
HandleGroupPtrhg
 =**new**HandleGroup();
            hg->name=
 groupName;
            adapter->_handleGroup=
 hg;
**for**(int32_ti
 = 0; i < handleNum; ++i)
            {
HandlePtrhandle
 =**new****T**();
                handle->setEpollServer(**this**);
                handle->setHandleGroup(hg);
                hg->handles.push_back(handle);
            }
_handleGroups[groupName]
 = hg;
            it =_handleGroups.find(groupName);
        }
        it->second->adapters[adapter->getName()]
 = adapter;
        adapter->_handleGroup=
 it->second;
    }

(8)委托EpollServer启动业务处理线程*_epollServer*->startHandle()首先判断并且设置_handleStarted状态，避免重复启动handler线程。遍历当前EpollServer中的所有HandleGroup，将HandleGroup中的每一个Handler都start起来。
**void****TC_EpollServer::****startHandle**()
{
**if**(!_handleStarted)
    {
_handleStarted=**true**;
map<string,HandleGroupPtr>::iteratorit;
**for**(it
 =_handleGroups.begin();
 it !=_handleGroups.end();
 ++it)
        {
vector<HandlePtr>&
 hds = it->second->handles;
**for**(uint32_ti
 = 0; i < hds.size(); ++i)
            {  
**if**(!hds[i]->isAlive())
                {
                    hds[i]->start();
                }
            }
        }
    }
}
ServantHandler继承与Handle，Handle的run如下：
**void****TC_EpollServer::Handle::run**()
{
    initialize();
handleImp();
}
首先会实现我们业务Servant的initialize方法。所以Servant的initialize方法是我们配置了多少个handleNum就会执行多少次。并且会执行一系列的handle方法，我们后续分析ServantHandler的时候进一步详细分析。

然后启动Epoll，开始监听所有Adapter的ServerSock：*_epollServer*->createEpoll();

在Application的waitForShutdown方法中：
**void****Application::waitForShutdown**()
{
*_epollServer*->waitForShutdown();
    destroyApp();
TafRemoteNotify::*getInstance*()->report("stop",**true**);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
初始化EpollServer，初始化工作如下：
- 设置一系列的状态
- 标记未构建底层Epoller。
- 初始化conn list。
- 不妨空连接攻击。
- 默认空连接超时时间2000
- udp连接时接收包缓存大小,针对所有udp接收缓存有效64K
- 初始化Socket管道(用于关闭服务)
- 初始化Socket管道(用于通知有数据需要发送就
**TC_EpollServer::****TC_EpollServer**()
:_bTerminate(**false**)
,_handleStarted(**false**)
,_createEpoll(**false**)
,_list(**this**)
,_pLocalLogger(NULL)
,_bEmptyConnAttackCheck(**false**)
,_iEmptyCheckTimeout(*MIN_EMPTY_CONN_TIMEOUT*)
,_nUdpRecvBufferSize(*DEFAULT_RECV_BUFFERSIZE*)
{
_shutdown.createSocket();
_notify.createSocket();
}
EpollServer的绑定Adapter，我们可以把Adapter绑定到该EpollServer中，每个EpollServer都包括一个BindAdapter的Map，用于维护与管理。每次绑定新的Adapter的时候，都会根据AdapterName来进行冲突检测，保证Adapter的唯一性。如果存在重名，则会抛出异常，然后根据Adapter中指定的EndPoint绑定到该Adapter包含的TC_Socket上面去。绑定后listeners将保存该ServerSock的描述符与Adapter的KV关系。
**int****TC_EpollServer::****bind**(BindAdapterPtr&lsPtr)
{
map<**int**,BindAdapterPtr>::iteratorit
 =_listeners.begin();
**while**(it
 !=_listeners.end())
    {
**if**(it->second->getName()
 == lsPtr->getName())
        {
**throw**TC_Exception("bind
 name '"+ lsPtr->getName() +"' conflicts.");
        }
        ++it;
    }
**const**TC_Endpoint&ep
 = lsPtr->getEndpoint();
TC_Socket&
 s = lsPtr->getSocket();
    bind(ep, s);
_listeners[s.getfd()]
 = lsPtr;
**return**s.getfd();
}
创建响应EndPoint的ServerSock的时候，会对ServerSock做一些响应的设置，如果是TCP的话，listenerSize设置为1024，sockopt会设置为alive连接，关闭Nagle实现tcpnodelay，以及SO_LINGER和非阻塞。
**void****TC_EpollServer::bind**(**const**TC_Endpoint&ep,TC_Socket&s)
{
**int**type = ep.isUnixLocal()?AF_LOCAL:AF_INET;
**if**(ep.isTcp())
    {
        s.createSocket(SOCK_STREAM,
 type);
    }
**else**
    {
        s.createSocket(SOCK_DGRAM,
 type);
    }
//设置网络qos的dscp标志
**if**(0 != ep.getQos())
    {
**int**iQos=ep.getQos();
        s.setSockOpt(IP_TOS,&iQos,**sizeof**(iQos),SOL_IP);
    }
**if**(ep.isUnixLocal())
    {
        s.bind(ep.getHost().c_str());
    }
**else**
    {
        s.bind(ep.getHost(), ep.getPort());
    }
**if**(ep.isTcp() && !ep.isUnixLocal())
    {
        s.listen(1024);
        s.setKeepAlive();
        s.setTcpNoDelay();
//不要设置close wait否则http服务回包主动关闭连接会有问题
        s.setNoCloseWait();
    }
    s.setblock(**false**);
}
EpollServer启动业务线程，首先判断——handlerStarted的状态并设置，避免重启启动，遍历
**void****TC_EpollServer::****startHandle**()
{
**if**(!_handleStarted)
    {
_handleStarted=**true**;
map<string,HandleGroupPtr>::iteratorit;
**for**(it =_handleGroups.begin();
 it !=_handleGroups.end(); ++it)
        {
vector<HandlePtr>&
 hds = it->second->handles;
**for**(uint32_ti
 = 0; i < hds.size(); ++i)
            {
**if**(!hds[i]->isAlive())
                {
                    hds[i]->start();
                }
            }
        }
    }
}

EpollServer负责建立对应的Epoller底层处理器，默认处理链接数目为10240最大的连接数目。
- 将shutdown与notify的套接字加入到Epoll中。
- 遍历注册到EpollServer中的BindAdapter。
- 根据所有Adapter设置的MaxConn的累加计算出系统可以总共可以管理的最大连接数目(如果是UDP，则算一个连接),并初始化连接队列ConnectionList。
- 如果存在UDP连接，则找出相应的Adapter，并建立Connection。
**void****TC_EpollServer::createEpoll**()
{
**if**(!_createEpoll)
     {
_createEpoll=**true**;
//创建epoll
_epoller.create(10240);
_epoller.add(_shutdown.getfd(),
 H64(*ET_CLOSE*),EPOLLIN);
_epoller.add(_notify.getfd(),
 H64(*ET_NOTIFY*),EPOLLIN);
size_tmaxAllConn   = 0;
**bool**  
 hasUdp       =**false**;
//监听socket
map<**int**,BindAdapterPtr>::iteratorit
 =_listeners.begin();
**while**(it
 !=_listeners.end())
           {
**if**(it->second->getEndpoint().isTcp())
                {
//获取最大连接数
                     maxAllConn += it->second->getMaxConns();
_epoller.add(it->first,
 H64(*ET_LISTEN*) | it->first,EPOLLIN);
                }
**else**
                {
                     maxAllConn++;
                     hasUdp =**true**;
                }
                ++it;
           }
//初始化连接管理链表
_list.init(maxAllConn);
**if**(hasUdp)
           {
//监听socket
map<**int**,BindAdapterPtr>::iteratorit
 =_listeners.begin();
**while**(it
 !=_listeners.end())
                {
**if**(!it->second->getEndpoint().isTcp())
                     {
Connection*cPtr
 =**new**Connection(it->second.get(),
 it->first);
//udp分配接收buffer
                          cPtr->setRecvBuffer(_nUdpRecvBufferSize);
                           addUdpConnection(cPtr);
                     }
                     ++it;
                }
           }
     }
}
**void****TC_EpollServer::****ConnectionList****::init**(uint32_tsize)
{
_lastTimeoutTime=TC_TimeProvider::*getInstance*()->getNow();
_total=
 size;
_free_size 
 = 0;
//初始化链接链表
**if**(_vConn)**delete**[]_vConn;
//分配total+1个空间(多分配一个空间, 第一个空间其实无效)
_vConn=**new**list_data[_total+1];
_iConnectionMagic  
 = ((uint32_t)_lastTimeoutTime)
 << 20;
//free从1开始分配, 这个值为uid,
 0保留为管道用,epollwait根据0判断是否是管道消息
**for**(uint32_ti
 = 1; i <=_total; i++)
    {
_vConn[i].first=NULL;
_free.push_back(i);
        ++_free_size;
    }
}
我们具体看一下连接是如何被listen，accept，以及具体到read,write，close等过程的。在EpollServer构建的时候，会创建两个SOCK
_shutdown.createSocket(AF_INET,SOCK_STREAM);
_notify.createSocket(AF_INET,SOCK_STREAM);
注册感兴趣的EPOLLIN，表示接收信号。
_epoller.add(_shutdown.getfd(),
 H64(*ET_CLOSE*), EPOLLIN);
_epoller.add(_notify.getfd(),
 H64(*ET_NOTIFY*), EPOLLIN);
H64表示把一个int32为的数据转化为int64位，然后把低端32位移到高端32位。
H64(x) (((uint64_t)x) << 32)
然后EpollServer会遍历listenter中的所有fd
_epoller.add(it->first,
 H64(*ET_LISTEN*)
 | it->first,EPOLLIN);
可见epoll_event data声明如下：
           typedef union epoll_data {
               void *ptr;
               int fd;
               __uint32_t u32;
               __uint64_t u64;
           } epoll_data_t;
           struct epoll_event {
               __uint32_t events;      /* Epoll events */
               epoll_data_t data;      /* User data variable */
           };
我们这里选择在把u64的高端32位设定为事件类型
**enum**
    {
*ET_LISTEN*=
 1,
*ET_CLOSE* 
 = 2,
*ET_NOTIFY*=
 3,
*ET_NET*   
 = 0,
    };
**进入轮询响应请求阶段**
最后系统会调用EpollServer会waitShutdown,进行Epollwait操作。经典的死循环，每次EpollWait之前都进行超时检测，调用Epoller进行wait获取有事件激活的sock（2s超时），返回事件激活的sock的数目，并进行遍历得到epoll_event，根据epoll_event中的data，得到该事件的关联信息，进行处理。
**void****TC_EpollServer::****waitForShutdown**()
{
    startHandle();
    createEpoll();
//循环监听网路连接请求
**while**(!_bTerminate)
    {
_list.checkTimeout(TC_TimeProvider::*getInstance*()->getNow());
   
**int**iEvNum
 =_epoller.wait(2000);
**for**(**int**i
 = 0; i < iEvNum; ++i)
        {
**try**
            {
**const**epoll_event&ev
 =_epoller.get(i);
uint32_th
 = ev.data.u64>> 32;
**switch**(h)
                {
**case***ET_LISTEN*:
                    {
//监听端口有请求
map<**int**,BindAdapterPtr>::const_iteratorit
 =_listeners.find(ev.data.u32);
**if**(
 it !=_listeners.end())
                        {
**if**(ev.events&EPOLLIN)
                            {
**bool**ret;
**do**
                                {
                                    ret =accept(ev.data.u32);
                                }**while**(ret);
                            }
                        }
                    }
**break**;
**case***ET_CLOSE*:
//关闭请求
**break**;
**case***ET_NOTIFY*:
//发送通知
                    processPipe();
**break**;
**case***ET_NET*:
//网络请求
                    processNet(ev);
**break**;
**default**:
                    assert(**true**);
                }
            }
**catch**(exception&ex)
            {
error("waitForShutdown
 exception:"+string(ex.what()));
            }
        }
    }
    stopThread();
}
把事件类型取出来uint32_t h
 = ev.data.u64>> 32
因为epoll_data_t是一个union，所以直接ev.data.u32可以得到listen的sock
 fd。
**监听端口有反应？客户端来了？**
当listener的接口获取到客户端的握手请求的时候，会accept调用来建立与客户端的连接。在连接建立的时候，会进行白名单的判断，如果不在白名单中，则会断开连接。如果超出了当前Adapter允许的最大连接数，也会断掉。如果连接建立成功，将Adapter中设置的超时时间拿出来，为客户端构建一个新的Connection对象，超时时间默认不短于2S。
**bool****TC_EpollServer::accept**(**int**fd)
{
**struct**sockaddr_instSockAddr;
socklen_tiSockAddrSize =**sizeof**(sockaddr_in);
TC_Socketcs;
    cs.setOwner(**false**);
//接收连接
TC_Sockets;
    s.init(fd,**false**,AF_INET);
**int**iRetCode
 = s.accept(cs, (**struct**sockaddr*)
 &stSockAddr, iSockAddrSize);
**if**(iRetCode
 > 0)
    {
string  ip;
uint16_tport;
**char**sAddr[INET_ADDRSTRLEN]
 ="\0";
**struct**sockaddr_in*p
 = (**struct**sockaddr_in*)&stSockAddr;
inet_ntop(AF_INET,
 &p->sin_addr, sAddr,**sizeof**(sAddr));
        ip      = sAddr;
        port    =ntohs(p->sin_port);
debug("accept
 ["+ ip +":"+TC_Common::tostr(port)
 +"] ["+TC_Common::tostr(cs.getfd())
 +"]incomming");
**if**(!_listeners[fd]->isIpAllow(ip))
        {
debug("accept
 ["+ ip +":"+TC_Common::tostr(port)
 +"] ["+TC_Common::tostr(cs.getfd())
 +"] not allowed");
            cs.close();
**return****true**;
        }
**if**(_listeners[fd]->isLimitMaxConnection())
        {
            cs.close();
debug("accept
 ["+ ip +":"+TC_Common::tostr(port)
 +"]["+TC_Common::tostr(cs.getfd())
 +"] beyond max connection:"+TC_Common::tostr(_listeners[fd]->getMaxConns()));
**return****true**;
        }
        cs.setblock(**false**);
        cs.setKeepAlive();
        cs.setTcpNoDelay();
        cs.setCloseWaitDefault();
//设置网络qos的dscp标志
**if**(0
 !=_listeners[fd]->getEndpoint().getQos())
        {
**int**iQos=_listeners[fd]->getEndpoint().getQos();
            cs.setSockOpt(IP_TOS,&iQos,**sizeof**(iQos),SOL_IP);
info("setSockOpt
 ["+ ip +":"+TC_Common::tostr(port)
 +"]["+TC_Common::tostr(cs.getfd())
 +"] listen serverqosis:"+TC_Common::tostr(iQos));
        }
**int**timeout
 =_listeners[fd]->getEndpoint().getTimeout()/1000;
Connection*cPtr
 =**new**Connection(_listeners[fd].get(),
 fd, (timeout < 2 ? 2 : timeout), cs.getfd(), ip, port);
        addTcpConnection(cPtr);
//过滤连接首个数据包包头
        cPtr->setHeaderFilterLen(_listeners[fd]->getHeaderFilterLen());
**return****true**;
    }
**else**
    {
//直到发生EAGAIN才不继续accept
**if**(errno==EAGAIN)
        {
**return****false**;
        }
**return****true**;
    }
**return****true**;
}
在构建好Connection后，会将Connection放入到EpollServer统一的ConnectionList中进行管理。另外该Adapter的当前连接数目会+1；并且该连接会注册到Epoll，读写时间都将被注册。该Connection的ID会当作epoll_event的data被放入。因为Connection本身ID为int32，所以连接类型为NET=0;
**void****TC_EpollServer::addTcpConnection**(TC_EpollServer::Connection*cPtr)
{
uint32_tuid
 =_list.getUniqId();
    cPtr->init(uid);
_list.add(cPtr,
 cPtr->getTimeout() +TC_TimeProvider::*getInstance*()->getNow());
    cPtr->getBindAdapter()->increaseNowConnection();
//注意epolladd必须放在最后,
 否则可能导致执行完, 才调用上面语句
_epoller.add(cPtr->getfd(),
 cPtr->getId(),EPOLLIN|EPOLLOUT);
}
上述步骤将处理了反复接受accept事件，accept设置为了unblock,所以当没有新的握手请求了，反复将边缘触发的事件处理完后停止接收。
**客户端和服务器果断开始基情燃烧**
如果是普通的客户端请求的处理，则需要处理NET类型的事件。如果发生错误，直接断掉请求。
**void****TC_EpollServer::****processNet**(**const**epoll_event&ev)
{
uint32_tuid
 = ev.data.u32;
Connection*cPtr
 = getConnectionPtr(uid);
**if**(!cPtr)
    {
debug("NetThread::processNet
 connection["+TC_Common::tostr(uid)
 +"] not exists.");
**return**;
    }
**if**((ev.events
 & EPOLLERR||ev.events & EPOLLHUP) && cPtr->getType() !=Connection::*EM_BUS*)
    {
        delConnection(cPtr);
**return**;
    }
**if**(ev.events&EPOLLIN)              //有数据需要读取
    {
recv_queue::queue_typevRecvData;
**int**ret
 =recvBuffer(cPtr, vRecvData);
**if**(ret
 < 0)
        {
            delConnection(cPtr);
**return**;
        }
**if**(!vRecvData.empty())
        {
            cPtr->insertRecvQueue(vRecvData);
        }
    }
**if**(ev.events&EPOLLOUT)             //有数据需要发送
    {
**int**ret
 = sendBuffer(cPtr);
**if**(ret
 < 0)
        {
            delConnection(cPtr);
**return**;
        }
    }
_list.refresh(uid,
 cPtr->getTimeout() +TC_TimeProvider::*getInstance*()->getNow());
}
如果有数据要读，代表客户端发包请求，调用Connector进行数据读取，以及数据包的解析。最后将得到的完整的数据包插入到该Adapter统一的接受队列中来。
如果有数据需要写，表示有rsp发送到客户端去：会调用该Connector将sendBuff的数据都发送到客户端去。当每次buffer遗留的数据太多了（超过了8192），则把sendBuff和_sendPos都复位一下。这样避免总是copy
 buff，也避免因为buffer无限增大。
**int****TC_EpollServer::Connection::****send**(**const**string&
 buffer,**const**string&ip,uint16_tport)
{
**if**(_lfd==
 -1)
     {
**int**iRet
 =_sock.sendto((**const****void***)
 buffer.c_str(), buffer.length(), ip, port, 0);
**if**(iRet
 < 0)
           {
_pBindAdapter->getEpollServer()->error("[TC_EpollServer::Connection]
 send ["+_ip+":"+TC_Common::tostr(_port)
 +"] error");
**return**-1;
           }
**return**0;
     }
_sendbuffer+=
 buffer;
size_tpos = 0;
size_tsendLen =_sendbuffer.length()
 -_sendPos;
**const****char***sendBegin
 =_sendbuffer.c_str()
 +_sendPos;
**while**(pos
 < sendLen )
    {
**int**iBytesSent
 = 0;
        iBytesSent =write(_sock.getfd(),
 (**const****void***)(sendBegin+
 pos), sendLen - pos);
**if**(iBytesSent
 < 0)
        {
**if**(errno==EAGAIN)
            {
**break**;
            }
**else**
            {
_pBindAdapter->getEpollServer()->debug("send
 ["+_ip+":"+TC_Common::tostr(_port)
 +"] close connection by peer.");
**return**-1;
            }
        }
        pos += iBytesSent;
//发送的数据小于需要发送的,break, 内核会再通知你的
**if**(pos
 < sendLen)
        {
**break**;
        }
    }
**if**(pos
 > 0)
    {
**if**(_sendbuffer.length()
 > 8192)
           {
_sendbuffer=_sendbuffer.substr(_sendPos+
 pos);
_sendPos=
 0;
           }
**else**
           {
_sendPos+=
 pos;
           }
    }
//需要关闭链接
**if**(_bClose&&
 ( (_sendbuffer.length()
 -_sendPos) == 0 ) )
    {
_pBindAdapter->getEpollServer()->debug("send
 ["+_ip+":"+TC_Common::tostr(_port)
 +"] close connection by user.");
_sendbuffer.clear();
_sendPos=
 0;
**return**-1;
    }
**return**0;
}
**下面完了后该上面了，**关于逻辑层的处理
我们来看一下业务线程都是如何处理这些我们收到的完整的数据包的。我们已经知道handleGroup里面有很多Handle线程，并且关联了一个或者多个Adapter，他会如何处理这些Adapter中收到的包的呢？每个Handle都会在线程run方法中调用handleImp方法，startHandle是一个钩子函数，在处理业务逻辑之前处理回调，当该handleGroup所有挂载的Adapter都没有接受数据的时候会挂起线程等待通知，
- 发送心跳
- 业务处理附加的自有消息遍历所有的Servant处理doCustomMessage。
- 处理各个Adapter的回包。
**void****TC_EpollServer::Handle::handleImp**()
{
    startHandle();
**while**(!getEpollServer()->isTerminate())
    {
        {
TC_ThreadLock::Locklock(_handleGroup->monitor);
**if**(allAdapterIsEmpty()
 && allFilterIsEmpty())
            {
_handleGroup->monitor.timedWait(_iWaitTime);
            }
        }
//上报心跳
        heartbeat();
//为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
        handleAsyncResponse();
        handleCustomMessage(**true**);
tagRecvData*
 recv =NULL;
map<string,BindAdapterPtr>&
 adapters =_handleGroup->adapters;
**for**(map<string,BindAdapterPtr>::iteratorit
 = adapters.begin(); it != adapters.end(); ++it)
        {
BindAdapterPtr&
 adapter = it->second;
**try**
            {
**while**(adapter->waitForRecvQueue(recv,
 0))
                {
//上报心跳
                    heartbeat();
//为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
                     handleAsyncResponse();
tagRecvData&
 stRecvData = *recv;
time_tnow =TC_TimeProvider::*getInstance*()->getNow();
                    stRecvData.adapter=
 adapter;
//数据已超载 overload
**if**(stRecvData.isOverload)
                    {
                        handleOverload(stRecvData);
                    }
//关闭连接的通知消息
**else****if**(stRecvData.isClosed)
                    {
                        handleClose(stRecvData);
                    }
//数据在队列中已经超时了
**else****if**(
 (now - stRecvData.recvTimeStamp)*1000 > adapter->getQueueTimeout())
                    {
                        handleTimeout(stRecvData);
                    }
**else**
                    {
                        handle(stRecvData);
                    }
                            handleCustomMessage(**false**);
**delete**recv;
                           recv =NULL;
                }
            }
**catch**(exception&ex)
            {
**if**(recv)
                     {
                           close(recv->uid);
**delete**recv;
                           recv =NULL;
                     }
                getEpollServer()->error("[Handle::handleImp]
 error:"+string(ex.what()));
            }
**catch**(...)
            {
**if**(recv)
                     {
                           close(recv->uid);
**delete**recv;
                           recv =NULL;
                     }
                getEpollServer()->error("[Handle::handleImp]
 unknown error");
            }
        }
    }
    stopHandle();
}
主要是handle(stRecvData)方法，下面是ServantHandler的实现
**void****ServantHandle::****handle**(**const**TC_EpollServer::tagRecvData&stRecvData)
{
JceCurrentPtrcurrent
 = createCurrent(stRecvData);
**if**(!current)**return**;
**if**(current->getBindAdapter()->isTafProtocol())
    {
        handleTafProtocol(current);
    }
**else**
    {
        handleNoTafProtocol(current);
    }
}
将tagRecvData序列化成为请求上下文，如果是taf协议的话，会把resvData中buffer的数据解析成为一个RequestPacket的通用请求包，并把当前Adapter的AdapterName设置到该RequestPackage的sServantName中。
JceCurrentPtr**ServantHandle::createCurrent**(**const**TC_EpollServer::tagRecvData&stRecvData)
{
JceCurrentPtrcurrent
 =**new**JceCurrent(**this**);
**try**
    {
        current->initialize(stRecvData);
    }
**catch**(JceDecodeException&ex)
    {
        LOG->error() <<"[TAF]ServantHandle::handle request protocol decode
 error:"<< ex.what() <<endl;
        close(stRecvData.uid);
**return**NULL;
    }
//只有TAF协议才处理
**if**(current->getBindAdapter()->isTafProtocol())
    {
time_tnow =TC_TimeProvider::*getInstance*()->getNow();
//数据在队列中的时间超过了客户端等待的时间(TAF协议)
**if**(current->_request.iTimeout>
 0 && (now - stRecvData.recvTimeStamp-1 )*1000 > current->_request.iTimeout)
        {
            LOG->error() <<"[TAF]ServantHandle::handle queue timeout:"
                         << current->_request.sServantName<<"|"
                         << current->_request.sFuncName<<"|"
                         << stRecvData.recvTimeStamp<<"|"
                         << stRecvData.adapter->getQueueTimeout()
 <<"|"
                         << current->_request.iTimeout<<"|"
                         << now <<"|"<<
 stRecvData.ip<<":"<<
 stRecvData.port<<endl;
            current->sendResponse(JCESERVERQUEUETIMEOUT);
**return**NULL;
        }
    }
**return**current;
}
如果是taf协议的话，resvData buff中的数据会解析为通用的RequestPackage
**void****readFrom**(taf::JceInputStream<**ReaderT**>&
 _is)
{
            _is.read(iVersion,
 1,**true**);
            _is.read(cPacketType,
 2,**true**);
            _is.read(iMessageType,
 3,**true**);
            _is.read(iRequestId,
 4,**true**);
            _is.read(sServantName,
 5,**true**);
            _is.read(sFuncName,
 6,**true**);
            _is.read(sBuffer,
 7,**true**);
            _is.read(iTimeout,
 8,**true**);
            _is.read(context,
 9,**true**);
            _is.read(status,
 10,**true**);
}
如果不是taf协议，则把buffer的数据原封不动直接丢给RequestPackage
_request.sBuffer.resize(stRecvData.buffer.length());
::memcpy(&_request.sBuffer[0],
 stRecvData.buffer.c_str(),
 stRecvData.buffer.length());
最终如果是taf协议，调用ServantHandle的**void****ServantHandle::****handleTafProtocol**(**const**JceCurrentPtr ¤t);
如果协议不是taf，调用ServantHandle的**void****ServantHandle::****handleNoTafProtocol**(**const**JceCurrentPtr ¤t);
这两个都会要求Servant进行Dispatch处理，接下来了解一下Servant，在Servant的dispatch中，需要根据请求的元信息和业务Servant自动生成的OnDispatch方法进行方法分发（JAVA可用反射实现），并且将Rsp输出。在这个方法中，可以进行一些系统的元方法处理，如taf_ping。如果是taf协议，直接调用了自动生成的onDispatch，如果是自定义协议，则需要调用doRequest方法，current的buffer中没有经过协议解码的数据，默认实现为返回-1，rsp字节为空。
**int****Servant::dispatch**(JceCurrentPtrcurrent,vector<**char**>
 &buffer)
{
**int**ret
 = JCESERVERUNKNOWNERR;
**if**(current->getFuncName()
 =="taf_ping")
    {
        TLOGINFO("[TAF][Servant::dispatch] taf_pingokfrom
 ["<< current->getIp() <<":"<<
 current->getPort() <<"]"<<endl);
        ret = JCESERVERSUCCESS;
    }
**else****if**(current->getFuncName()
 =="taf_bus")
     {
TC_LockT<TC_ThreadRecMutex>
 lock(***this**);
           ret =doBus(current, buffer);
     }
**else****if**(!current->getBindAdapter()->isTafProtocol())
    {
TC_LockT<TC_ThreadRecMutex>
 lock(***this**);
        ret =doRequest(current, buffer);
    }
**else**
    {
TC_LockT<TC_ThreadRecMutex>
 lock(***this**);
        ret =onDispatch(current, buffer);
    }
**return**ret;
}
一下是MobWIN广告Servant自动生成的一个onDispatch方法：
**int****AdService::****onDispatch**(taf::JceCurrentPtr_current, vector<**char**>
 &_sResponseBuffer)
    {
pair<string*, string*>
 r = equal_range(__MobWin__AdService_all, __MobWin__AdService_all+2, _current->getFuncName());
**if**(r.first ==
 r.second) **return**taf::JCESERVERNOFUNCERR;
**switch**(r.first -
 __MobWin__AdService_all)
        {
**case** 0:
            {
taf::JceInputStream<taf::BufferReader> _is;
_is.setBuffer(_current->getRequestBuffer());
                MobWin::AdRequest4App req;
                MobWin::AdResponse rsp;
**if** (_current->getRequestVersion()
 == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                {
UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
tafAttr.setVersion(_current->getRequestVersion());
tafAttr.decode(_current->getRequestBuffer());
tafAttr.get("req",
 req);
tafAttr.getByDefault("rsp",
 rsp, rsp);
                }
**else**
                {
_is.read(req,
 1, **true**);
_is.read(rsp,
 2, **false**);
                }
taf::Int32 _ret = getAdContents(req,rsp,
 _current);
**if**(_current->isResponse())
                {
**if** (_current->getRequestVersion()
 == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                    {
UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
tafAttr.setVersion(_current->getRequestVersion());
tafAttr.put("",
 _ret);
tafAttr.put("rsp",
 rsp);
tafAttr.encode(_sResponseBuffer);
                    }
**else**
                    {
taf::JceOutputStream<taf::BufferWriter> _os;
_os.write(_ret,
 0);
_os.write(rsp,
 2);
_os.swap(_sResponseBuffer);
                    }
                }
**return**taf::JCESERVERSUCCESS;
            }
**case** 1:
            {
taf::JceInputStream<taf::BufferReader> _is;
_is.setBuffer(_current->getRequestBuffer());
                std::string inMsg;
                std::string rtnMsg;
**if** (_current->getRequestVersion()
 == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                {
UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
tafAttr.setVersion(_current->getRequestVersion());
tafAttr.decode(_current->getRequestBuffer());
tafAttr.get("inMsg",
 inMsg);
tafAttr.getByDefault("rtnMsg",
 rtnMsg, rtnMsg);
                }
**else**
                {
_is.read(inMsg,
 1, **true**);
_is.read(rtnMsg,
 2, **false**);
                }
taf::Int32 _ret = testService(inMsg,rtnMsg,
 _current);
**if**(_current->isResponse())
                {
**if** (_current->getRequestVersion()
 == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                    {
UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
tafAttr.setVersion(_current->getRequestVersion());
tafAttr.put("",
 _ret);
tafAttr.put("rtnMsg",
 rtnMsg);
tafAttr.encode(_sResponseBuffer);
                    }
**else**
                    {
taf::JceOutputStream<taf::BufferWriter> _os;
_os.write(_ret,
 0);
_os.write(rtnMsg,
 2);
_os.swap(_sResponseBuffer);
                    }
                }
**return**taf::JCESERVERSUCCESS;
            }
        }
**return**taf::JCESERVERNOFUNCERR;
    }
业务Request从JCEInputStream的解析，以及Rsp的编码，都是根据JCE文件自动生成的。这些留待分析JCE编码的时候再分析。
**该把结果发射给客户端啦！**
如果是同步请求，则必须把结果返还给客户端
**void****JceCurrent::sendResponse**(**int** iRet, **const**vector<**char**>&
 buffer, **const**map<string, string>&
 status, **const**string &
 sResultDesc)
{
_iRet =
 iRet;
//单向调用不需要返回
**if** (_request.cPacketType ==
 JCEONEWAY)
    {
**return**;
    }
JceOutputStream<BufferWriter>
 os;
**if** (_request.iVersion !=
 WUPVERSION && _request.iVersion !=
 WUPVERSION2)
    {
ResponsePacket response;
        response.iRequestId    
 = _request.iRequestId;
        response.iMessageType  
 = _request.iMessageType;
        response.cPacketType   
 = JCENORMAL;
        response.iVersion      
 = JCEVERSION;
        response.status        
 = status;
        response.sBuffer       
 = buffer;
        response.sResultDesc   
 = sResultDesc;
        response.iRet          
 = iRet;
**if**(IS_MSG_TYPE(_request.iMessageType,
 taf::JCEMESSAGETYPELOADED))
        {
/**
             * 如果业务已经设置负载值，则取业务的负载值,否则取服务端队列大小
             * 负载值加上调节因子，为了提高客户端的负载均衡性。
             */
**int** iLoaded
 = 1;
**if**(_bSetLoaded)
            {
                iLoaded = _iLoaded >
 0?_iLoaded:1;
            }
**else**
            {
                iLoaded = _pBindAdapter->getRecvBufferSize()
 + _pBindAdapter->getLoadFactor();
            }
            response.status[ServantProxy::*STATUS_LOADED_VALUE*]
 = TC_Common::tostr(iLoaded);
        }
           TLOGINFO("[TAF]JceCurrent::sendResponse :"
                        << response.iMessageType << "|"
                        << _request.sServantName << "|"
                        << _request.sFuncName << "|"
                        << response.iRequestId << endl);
        response.writeTo(os);
    }
**else**
    {
//wup回应包用请求包的结构
RequestPacket response;
        response.iRequestId    
 = _request.iRequestId;
        response.iMessageType  
 = _request.iMessageType;
        response.cPacketType   
 = JCENORMAL;
        response.iVersion      
 = _request.iVersion;
        response.status        
 = status;
        response.sBuffer       
 = buffer;
        response.sServantName  
 = _request.sServantName;
        response.sFuncName     
 = _request.sFuncName;
//异常的情况下buffer可能为空，要保证有一个空UniAttribute的编码内容
**if**(response.sBuffer.size()
 == 0)
        {
            wup::UniAttribute<>
 tafAttr;
                tafAttr.setVersion(_request.iVersion);
            tafAttr.encode(response.sBuffer);
        }
//iRet为0时,不记录在status里面,和wup客户端协议一致,节省空间
**if**(iRet
 != 0)
        {
            response.status[ServantProxy::*STATUS_RESULT_CODE*]
 = TC_Common::tostr(iRet);
        }
//sResultDesc为空时,不记录在status里面,和wup客户端协议一致,节省空间
**if**(!sResultDesc.empty())
        {
            response.status[ServantProxy::*STATUS_RESULT_DESC*]
 = sResultDesc;
        }
           TLOGINFO("[TAF]JceCurrent::sendResponse :"
                        << response.iMessageType << "|"
                        << response.sServantName << "|"
                        << response.sFuncName << "|"
                        << response.iRequestId << endl);
        response.writeTo(os);
    }
    taf::Int32 iHeaderLen
 = htonl(**sizeof**(taf::Int32)
 + os.getLength());
strings = "";
    s.append((**const****char***)&iHeaderLen, **sizeof**(taf::Int32));
    s.append(os.getBuffer(), os.getLength());
_pServantHandle->sendResponse(_uid,
 s, _ip, _port);
}
最后会调用EpollServer的send方法
**void****TC_EpollServer::****send**(uint32_tuid, **const**string&s, **const**string &ip, uint16_t port)
{
**if**(_bTerminate)
    {
**return**;
    }
tagSendData*
 send = **new** tagSendData();
    send->uid =
 uid;
    send->cmd = 's';
    send->buffer =
 s;
    send->ip =
 ip;
    send->port =
 port;
_sbuffer.push_back(send);
//通知epoll响应,
 有数据要发送
_epoller.mod(_notify.getfd(),
 H64(*ET_NOTIFY*), EPOLLOUT);
}
EpollServer会构造tagSendData包，并放入自己的回包队列中（EpollServer所有）。并且通知notify描述符（write只要注册了该事件，写缓存区符合低潮lowat的阀值就可以满足激活条件），可写。下面是notify类型的事件处理
**void****TC_EpollServer::****processPipe**()
{
send_queue::queue_typedeSendData;
_sbuffer.swap(deSendData);
send_queue::queue_type::iteratorit
 = deSendData.begin();
send_queue::queue_type::iteratoritEnd
 = deSendData.end();
**while**(it
 != itEnd)
    {
**switch**((*it)->cmd)
        {
**case**'c':
            {
Connection *cPtr
 = getConnectionPtr((*it)->uid);
**if**(cPtr)
                {
**if**(cPtr->setClose())
                    {
                        delConnection(cPtr);
                    }
                }
**break**;
            }
**case**'s':
            {
Connection *cPtr
 = getConnectionPtr((*it)->uid);
**if**(cPtr)
                {
**int** ret
 = sendBuffer(cPtr, (*it)->buffer, (*it)->ip,
 (*it)->port);
**if**(ret
 < 0)
                    {
                        delConnection(cPtr);
                    }
                }
**break**;
            }
**default**:
            assert(**false**);
        }
**delete** (*it);
        ++it;
    }
}
把EpollServer中的回包交换出来（EpollServer的清空，拿出来的继续处理），需要回包的继续调用Connection将数据发送到客户端去。
**int****TC_EpollServer::****sendBuffer**(TC_EpollServer::Connection*cPtr, **const**string &buffer, **const**string &ip, uint16_tport)
{
**return** cPtr->send(buffer,
 ip, port);
}
如果应用终止（_bTerminate）最后关闭线程，遍历EpollServer中的所有handleGroup。
**void****TC_EpollServer::stopThread**()
{
map<string,HandleGroupPtr>::iteratorit;
**for**(it
 =_handleGroups.begin();
 it !=_handleGroups.end();
 ++it)
    {
        {
TC_ThreadLock::Locklock(it->second->monitor);
            it->second->monitor.notifyAll();
        }
vector<HandlePtr>&
 hds = it->second->handles;
**for**(uint32_ti
 = 0; i < hds.size(); ++i)
        {
**if**(hds[i]->isAlive())
            {
                hds[i]->getThreadControl().join();
            }
        }
    }
}

