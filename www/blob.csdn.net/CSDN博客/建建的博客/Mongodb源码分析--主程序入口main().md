# Mongodb源码分析--主程序入口main() - 建建的博客 - CSDN博客
2017年03月07日 14:11:14[纪建](https://me.csdn.net/u013898698)阅读数：275
      作为这个系列的开篇，本人特此声明，因为本人技术功力有限，且对mongodb源码目前也在研究探索中，可能会对mongodb内部某些实现机制及原作者的意图领会不够精确，因此错误再所难免，希望大家批评指正。另外本文所使用的mongodb源码为1.8 rc1，同时如果有条件的话，大家可以安装vs2010，用C++来编译调试mongodb源码，以便通过运行过程中的数据和流程来验证自己的判断。
      VS2010 C++下编译调试MongoDB源码
     http://www.cnblogs.com/daizhj/archive/2011/03/07/1973764.html
      好了，开始今天的正文吧。
    为了理解mongodb整体的运行机制，首先我们需要对其主要运行流程有一个大概的理解，而主入口函数main无疑是最佳突破口。首先我们在VS2010中打开db.sln文件，并打开db.cpp文件，找到主入口函数（位于文件613行），如下：
```
int main(int argc, char* argv[]) {
    static StaticObserver staticObserver;
    getcurns = ourgetns;
    po::options_description general_options("General options");//常规选项
#if defined(_WIN32)
    po::options_description windows_scm_options("Windows Service Control Manager options");//windows服务控制管理选项仅限windows平台
#endif
    po::options_description replication_options("Replication options");//Replication选项
    po::options_description ms_options("Master/slave options");//主从选项
    po::options_description rs_options("Replica set options");//Replica设置选项
    po::options_description sharding_options("Sharding options");//数据分片选项
    po::options_description visible_options("Allowed options");//可见选项
    po::options_description hidden_options("Hidden options");//隐藏选项
    po::positional_options_description positional_options;
    general_options.add_options()
    ("auth", "run with security")
    ("cpu", "periodically show cpu and iowait utilization")
    ("dbpath", po::value<string>() , "directory for datafiles")
    ("diaglog", po::value<int>(), "0=off 1=W 2=R 3=both 7=W+some reads")
    ("directoryperdb", "each database will be stored in a separate directory")
    .....
    该方法的开头代码（上面）主要是绑定一个配置操作选项的说明，包括命令行模式下的参数说明，因为内容较长，这里就不做过多描述了，需要说明options_description的是这些内容被放到了boost库（一个C++开源库）的options_description对象中，其类型结构可以理解为key/value模式，主要用于记录一系列的选项描述（符）信息，以便于通过名称查询相应选项信息。同时mongodb将选项大致归为8类，如上所述。
    接下说看一下其初始化时命令行参数的操作，如下：
   
```
if( argc == 1 )
        cout << dbExecCommand << " --help for help and startup options" << endl;
    {
        po::variables_map params;
        string error_message = arg_error_check(argc, argv);
        if (error_message != "") {
            cout << error_message << endl << endl;
            show_help_text(visible_options);
            return 0;
        }
        if ( ! CmdLine::store( argc , argv , visible_options , hidden_options , positional_options , params ) )
            return 0;
     上面方法对main主函数参数argc，argv及上面的那些选项实例进行存储并以此绑定到params实例上，因为接下来会通过params来设置cmdLine对象（CmdLine类型），并最终以该对象做为最终在mongodb内部标记相应启动命令参数信息的对象。形如：
  
```
if (params.count("version")) {
            cout << mongodVersion() << endl;
            printGitVersion();
            return 0;
        }
        if ( params.count( "dbpath" ) ) {
            dbpath = params["dbpath"].as<string>();
            if ( params.count( "fork" ) && dbpath[0] != '/' ) {
                // we need to change dbpath if we fork since we change
                // cwd to "/"
                // fork only exists on *nix
                // so '/' is safe
                dbpath = cmdLine.cwd + "/" + dbpath;
            }
        }
        else {
            dbpath = "d:/data/db/";//我在此处改了源码
        }
    
        if ( params.count("directoryperdb")) {
            directoryperdb = true;
        }
        if (params.count("cpu")) {
            cmdLine.cpu = true;
        }
        ......
    当搜集到足够的启动信息（参数）后，mongodb开启执行下面两行代码：
```
Module::configAll( params );
```
    上面用params来配置加载的模块信息，而就目前而言，mongodb中的模块有两个：其类模式和MMS模块，后者是当mongodb监视服务有效情况下，以后台线程方式（BackgroundJob）运行的程序，类定义如下：
```
/** Mongo Monitoring Service
        if enabled, this runs in the background ands pings mss
    */
    class MMS : public BackgroundJob , Module {
    ....
    }
```
      因为相关代码比较简单，这里就不多作说明了，如果大家感兴趣的话，以后会专门写一篇介绍Module，BackgroundJob的文章 。
     回到正文，模块实始化完成了，就会运行如下代码：
```
dataFileSync.go();
```
    这里要说明的是dataFileSync类也派生自BackgroundJob类，而BackgroundJob的功能就是生成一个后台线程并执行相应任务。而当前dataFileSync的任务就是在一段时间后（cmdLine.syncdelay）将内存中的数据flash到磁盘上（因为mongodb使用mmap方式将数据先放入内存中），代码如下：
```
class DataFileSync : public BackgroundJob {
  ......
  void run() {
            if( cmdLine.syncdelay == 0 )
                log() << "warning: --syncdelay 0 is not recommended and can have strange performance" << endl;
            else if( cmdLine.syncdelay == 1 )
                log() << "--syncdelay 1" << endl;
            else if( cmdLine.syncdelay != 60 )
                log(1) << "--syncdelay " << cmdLine.syncdelay << endl;
            int time_flushing = 0;
            while ( ! inShutdown() ) {
                flushDiagLog();
                if ( cmdLine.syncdelay == 0 ) {
                    // in case at some point we add an option to change at runtime
                    sleepsecs(5);
                    continue;
                }
                sleepmillis( (long long) std::max(0.0, (cmdLine.syncdelay * 1000) - time_flushing) );
                if ( inShutdown() ) {
                    // occasional issue trying to flush during shutdown when sleep interrupted
                    break;
                }
                Date_t start = jsTime();
                int numFiles = MemoryMappedFile::flushAll( true );//使用系统提供的内存映射文件方法
                time_flushing = (int) (jsTime() - start);
                globalFlushCounters.flushed(time_flushing);
                log(1) << "flushing mmap took " << time_flushing << "ms " << " for " << numFiles << " files" << endl;
            }
        }
......
     main主函数完成上面方法后，就会启动侦听方法，开始侦听客户端的链接请求，如下：
```
initAndListen(cmdLine.port, appsrvPath);
```
     该侦听方法会最终调用db.cpp (467行)的如下方法，我们来看一下该方法做了些什么：
```
void _initAndListen(int listenPort, const char *appserverLoc = NULL) {
```
     首先是初始化一个名称“initandlisten”线程用于侦听客户端传来的操作信息(可能有误)：
```
Client::initThread("initandlisten");
```
     接着判断当前系统是32或64位系统？并获取当前进程ID并输出进程ID及数据库路径，端口信息以及当前mongodb及系统信息（这些信息也就是我们在命令行下经常看到的启动mongodb信息）
```
bool is32bit = sizeof(int*) == 4;
    {
#if !defined(_WIN32)
            pid_t pid = getpid();
#else
            DWORD pid=GetCurrentProcessId();
#endif
        Nullstream& l = log();
        l << "MongoDB starting : pid=" << pid << " port=" << cmdLine.port << " dbpath=" << dbpath;
        if( replSettings.master ) l << " master=" << replSettings.master;
        if( replSettings.slave )  l << " slave=" << (int) replSettings.slave;
        l << ( is32bit ? " 32" : " 64" ) << "-bit " << endl;
    }
    DEV log() << "_DEBUG build (which is slower)" << endl;
    show_warnings();
    log() << mongodVersion() << endl;
    printGitVersion();
    printSysInfo();
    完成这一步之后，接下来mongodb就会对相应路径下的数据文件进行检查，如出现文件错误（文件不存在等）：
```
stringstream ss;
    ss << "dbpath (" << dbpath << ") does not exist";
    uassert( 10296 ,  ss.str().c_str(), boost::filesystem::exists( dbpath ) );
    
    stringstream ss;
    ss << "repairpath (" << repairpath << ") does not exist";
    uassert( 12590 ,  ss.str().c_str(), boost::filesystem::exists( repairpath ) );
        
   同时使用"路径锁"方式来移除指定路径下的临时文件夹信息，如下：
```
acquirePathLock();
   remove_all( dbpath + "/_tmp/" );
```
   接着，mongodb还会启动持久化功能，该功能貌似是1.7版本后引入到系统中的，主要用于解决因系统宕机时，内存中的数据未写入磁盘而造成的数据丢失。其机制主要是通过log方式定时将操作日志（如cud操作等）记录到db的journal文件夹下，这样当系统再次重启时从该文件夹下恢复丢失的（内存）数据。有关这部分内容我会专门写文章加以介绍。
```
dur::startup();
     if( cmdLine.durOptions & CmdLine::DurRecoverOnly )
         return;
```
     注：其命令行枚举定义如下
```
enum { // bits to be ORed
            DurDumpJournal = 1,   // dump diagnostics on the journal during recovery
            DurScanOnly = 2,      // don't do any real work, just scan and dump if dump specified
            DurRecoverOnly = 4,   // terminate after recovery step
            DurParanoid = 8,      // paranoid mode enables extra checks
            DurAlwaysCommit = 16  // do a group commit every time the writelock is released
        };
     int durOptions;          // --durOptions <n> for debugging
   完成这一步之后，系统还会初始化脚本引擎，因为mongodb支持脚本语法做为其操作数据库的语言，如下：
```
if ( scriptingEnabled ) {
            ScriptEngine::setup();
            globalScriptEngine->setCheckInterruptCallback( jsInterruptCallback );
            globalScriptEngine->setGetInterruptSpecCallback( jsGetInterruptSpecCallback );
   }
```
   当这些主要工作做完之后，最后系统会调用下面方法正式启动侦听操作：
```
void listen(int port) {
        log() << "waiting for connections on port " << port << endl;
        OurListener l(cmdLine.bind_ip, port);
        l.setAsTimeTracker();
        startReplication();
        if ( !noHttpInterface )
            boost::thread web( boost::bind(&webServerThread, new RestAdminAccess() /* takes ownership */));
#if(TESTEXHAUST)
        boost::thread thr(testExhaust);
#endif
        l.initAndListen();
    }
    注意上面的OurListener类其initAndListen()方法位于message.cpp中，因为mongodb采用message相关类来封装c/s双在的数据和操作：
```
void Listener::initAndListen() {
     checkTicketNumbers();
        vector<SockAddr> mine = ipToAddrs(_ip.c_str(), _port);
        vector<int> socks;
        SOCKET maxfd = 0; // needed for select()
        for (vector<SockAddr>::iterator it=mine.begin(), end=mine.end(); it != end; ++it) {
            SockAddr& me = *it;
            SOCKET sock = ::socket(me.getType(), SOCK_STREAM, 0);
            if ( sock == INVALID_SOCKET ) {
                log() << "ERROR: listen(): invalid socket? " << errnoWithDescription() << endl;
            }
            if (me.getType() == AF_UNIX) {
#if !defined(_WIN32)
                if (unlink(me.getAddr().c_str()) == -1) {
                    int x = errno;
                    if (x != ENOENT) {
                        log() << "couldn't unlink socket file " << me << errnoWithDescription(x) << " skipping" << endl;
                        continue;
                    }
                }
#endif
            }
            else if (me.getType() == AF_INET6) {
                // IPv6 can also accept IPv4 connections as mapped addresses (::ffff:127.0.0.1)
                // That causes a conflict if we don't do set it to IPV6_ONLY
                const int one = 1;
                setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, (const char*) &one, sizeof(one));
            }
            prebindOptions( sock );
            if ( ::bind(sock, me.raw(), me.addressSize) != 0 ) {
                int x = errno;
                log() << "listen(): bind() failed " << errnoWithDescription(x) << " for socket: " << me.toString() << endl;
                if ( x == EADDRINUSE )
                    log() << "  addr already in use" << endl;
                closesocket(sock);
                return;
            }
#if !defined(_WIN32)
            if (me.getType() == AF_UNIX) {
                if (chmod(me.getAddr().c_str(), 0777) == -1) {
                    log() << "couldn't chmod socket file " << me << errnoWithDescription() << endl;
                }
                ListeningSockets::get()->addPath( me.getAddr() );
            }
#endif
            if ( ::listen(sock, 128) != 0 ) {
                log() << "listen(): listen() failed " << errnoWithDescription() << endl;
                closesocket(sock);
                return;
            }
            ListeningSockets::get()->add( sock );
            socks.push_back(sock);
            if (sock > maxfd)
                maxfd = sock;
        }
        static long connNumber = 0;
        struct timeval maxSelectTime;
        while ( ! inShutdown() ) {
            fd_set fds[1];
            FD_ZERO(fds);
            for (vector<int>::iterator it=socks.begin(), end=socks.end(); it != end; ++it) {
                FD_SET(*it, fds);
            }
            maxSelectTime.tv_sec = 0;
            maxSelectTime.tv_usec = 10000;
            const int ret = select(maxfd+1, fds, NULL, NULL, &maxSelectTime);
            if (ret == 0) {
#if defined(__linux__)
                _elapsedTime += ( 10000 - maxSelectTime.tv_usec ) / 1000;
#else
                _elapsedTime += 10;
#endif
                continue;
            }
            _elapsedTime += ret; // assume 1ms to grab connection. very rough
            if (ret < 0) {
                int x = errno;
#ifdef EINTR
                if ( x == EINTR ) {
                    log() << "select() signal caught, continuing" << endl;
                    continue;
                }
#endif
                if ( ! inShutdown() )
                    log() << "select() failure: ret=" << ret << " " << errnoWithDescription(x) << endl;
                return;
            }
            for (vector<int>::iterator it=socks.begin(), end=socks.end(); it != end; ++it) {
                if (! (FD_ISSET(*it, fds)))
                    continue;
                SockAddr from;
                int s = accept(*it, from.raw(), &from.addressSize);
                if ( s < 0 ) {
                    int x = errno; // so no global issues
                    if ( x == ECONNABORTED || x == EBADF ) {
                        log() << "Listener on port " << _port << " aborted" << endl;
                        return;
                    }
                    if ( x == 0 && inShutdown() ) {
                        return;   // socket closed
                    }
                    if( !inShutdown() )
                        log() << "Listener: accept() returns " << s << " " << errnoWithDescription(x) << endl;
                    continue;
                }
                if (from.getType() != AF_UNIX)
                    disableNagle(s);
                if ( _logConnect && ! cmdLine.quiet )
                    log() << "connection accepted from " << from.toString() << " #" << ++connNumber << endl;
                accepted(s, from);
            }
        }
    }
    上面方法基本上就是一个无限循环（ while ( ! inShutdown() ) ）的侦听服务端，它调用操作系统的底层socket api接口，并将侦听到的结果使用accepted（）方法进行接收。这里要注意的是因为最终我们使用的是OurListener进行的侦听，所以最终系统会调用OurListener所实现的虚（virtual）方法，如下：
```
class OurListener : public Listener {
    public:
        OurListener(const string &ip, int p) : Listener(ip, p) { }
        virtual void accepted(MessagingPort *mp) {
            if ( ! connTicketHolder.tryAcquire() ) {
                log() << "connection refused because too many open connections: " << connTicketHolder.used() << " of " << connTicketHolder.outof() << endl;
                // TODO: would be nice if we notified them...
                mp->shutdown();
                delete mp;
                return;
            }
            try {
                boost::thread thr(boost::bind(&connThread,mp));
            }
            catch ( boost::thread_resource_error& ) {
                log() << "can't create new thread, closing connection" << endl;
                mp->shutdown();
                delete mp;
            }
            catch ( ... ) {
                log() << "unkonwn exception starting connThread" << endl;
                mp->shutdown();
                delete mp;
            }
        }
    };
    上面方法中的try{}语句中包含的是boost库中的thread方法，其主要提供了跨操作系统的线程创建方式及相关并行操作（相关信息参数boost官方网站），我们这里只要知道，通过该语句，我们最终用一个线程来运行connThread方法及其所需参数mp即可。下面看一下connThread方法的代码：
 
```
void connThread( MessagingPort * inPort ) {
        TicketHolderReleaser connTicketReleaser( &connTicketHolder );
        /* todo: move to Client object */
        LastError *le = new LastError();
        lastError.reset(le);
        inPort->_logLevel = 1;
        auto_ptr<MessagingPort> dbMsgPort( inPort );
        Client& c = Client::initThread("conn", inPort);
        try {
            c.getAuthenticationInfo()->isLocalHost = dbMsgPort->farEnd.isLocalHost();
            Message m;
            while ( 1 ) {
                inPort->clearCounters();
                if ( !dbMsgPort->recv(m) ) {
                    if( !cmdLine.quiet )
                        log() << "end connection " << dbMsgPort->farEnd.toString() << endl;
                    dbMsgPort->shutdown();
                    break;
                }
sendmore:
                if ( inShutdown() ) {
                    log() << "got request after shutdown()" << endl;
                    break;
                }
                lastError.startRequest( m , le );
                DbResponse dbresponse;
                assembleResponse( m, dbresponse, dbMsgPort->farEnd );
                if ( dbresponse.response ) {
                    dbMsgPort->reply(m, *dbresponse.response, dbresponse.responseTo);
                    if( dbresponse.exhaust ) {
                        ...出现问题时
                    }
                }
                networkCounter.hit( inPort->getBytesIn() , inPort->getBytesOut() );
                m.reset();
            }
        }
        ......
        // thread ending...
        {
            Client * c = currentClient.get();
            if( c ) c->shutdown();
        }
        globalScriptEngine->threadDone();
    }
    上面代码主要工作就是不断循环[while ( 1 )]获取当前客户端发来的信息（上面已封装成了message）并将其信息进行分析，并根据相应操作标志位确定当前操作是CRUD或构建索引等[assembleResponse()]，如果一些正常，则向客户端发送应答信息：
```
void connThread( MessagingPort * inPort ) {
        TicketHolderReleaser connTicketReleaser( &connTicketHolder );
        /* todo: move to Client object */
        LastError *le = new LastError();
        lastError.reset(le);
        inPort->_logLevel = 1;
        auto_ptr<MessagingPort> dbMsgPort( inPort );
        Client& c = Client::initThread("conn", inPort);
        try {
            c.getAuthenticationInfo()->isLocalHost = dbMsgPort->farEnd.isLocalHost();
            Message m;
            while ( 1 ) {
                inPort->clearCounters();
                if ( !dbMsgPort->recv(m) ) {
                    if( !cmdLine.quiet )
                        log() << "end connection " << dbMsgPort->farEnd.toString() << endl;
                    dbMsgPort->shutdown();
                    break;
                }
sendmore:
                if ( inShutdown() ) {
                    log() << "got request after shutdown()" << endl;
                    break;
                }
                lastError.startRequest( m , le );
                DbResponse dbresponse;
                assembleResponse( m, dbresponse, dbMsgPort->farEnd );
                if ( dbresponse.response ) {
                    dbMsgPort->reply(m, *dbresponse.response, dbresponse.responseTo);
                    if( dbresponse.exhaust ) {
                        ...出现问题时
                    }
                }
                networkCounter.hit( inPort->getBytesIn() , inPort->getBytesOut() );
                m.reset();
            }
        }
        ......
        // thread ending...
        {
            Client * c = currentClient.get();
            if( c ) c->shutdown();
        }
        globalScriptEngine->threadDone();
    }
  
    运行到这里，main函数的使命就完成了，本来想用一张时序图来大致回顾一下，只有等有时间再补充了。
    好了，今天的内容到这里就告一段落了，在接下来的文章中，将会介绍客户端发起查询操作时，Mongodb的执行流程和运行机制。
    原文链接:[http://www.cnblogs.com/daizhj/archive/2011/03/17/1987311.html](http://www.cnblogs.com/daizhj/archive/2011/03/17/1987311.html)
    作者: daizhj, 代震军    
    微博:http://t.sina.com.cn/daizhj
   
Tags: mongodb,c++,source code
Tags: mongodb,c++,source code
```
```
```
```
```
```
```
```
```
```
```
```
