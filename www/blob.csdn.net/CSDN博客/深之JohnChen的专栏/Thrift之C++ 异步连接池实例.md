# Thrift之C++ 异步连接池实例 - 深之JohnChen的专栏 - CSDN博客

2017年09月16日 18:15:49[byxdaz](https://me.csdn.net/byxdaz)阅读数：984


```cpp
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

int port = 9090;
shared_ptr<serDemoHandler> handler(new serDemoHandler());
shared_ptr<TProcessor> processor(new serDemoProcessor(handler));
shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(10);//指定10个线程数
shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
threadManager->threadFactory(threadFactory);
threadManager->start();
TThreadPoolServer server(processor,serverTransport,transportFactory,protocolFactory,threadManager);//使用线程池
//TNonblockingServer server(processor,protocolFactory,port);//不使用线程池

server.serve();
```

Windows下将PosixThreadFactory换成PlatformThreadFactory

```cpp
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/BoostThreadFactory.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

int port = 9090;
shared_ptr<TestHandler> handler(new TestHandler());
shared_ptr<TProcessor> processor(new TestProcessor(handler));
shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
try
{
		shared_ptr<ThreadManager> threadManager =ThreadManager::newSimpleThreadManager(10);
		boost::shared_ptr<PlatformThreadFactory> threadFactory = boost::shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory());
		threadManager->threadFactory(threadFactory);
		threadManager->start();
		//TThreadPoolServer server(processor,serverTransport,transportFactory,protocolFactory,threadManager);//使用线程池
		TNonblockingServer server(processor,protocolFactory,port);

		printf("Starting the server...\n");
		server.serve();
		printf("done.\n");
}
catch (...)
{
	
}
```

