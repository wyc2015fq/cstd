# 用现代 C++ 写一个高性能的服务器 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [袁欣](http://www.jobbole.com/members/yuan12xin) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[James Perry](https://medium.com/hacker-daily/writing-high-performance-servers-in-modern-c-7cd00926828)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
首先感谢大家对上一篇博文《[用 C++ 开启技术创业之旅](http://blog.jobbole.com/104468/)》的反馈。在上篇文章中我提到我曾在一天内就凭借 Facebook 的 Wangle 搭建起一个数据库引擎的原型，在这里我会解释我是如何做到的。到本文最后，你将可以用 [Wangle](https://github.com/facebook/wangle) 编写出一个高性能的C++服务器。本文也将作为教程整合进 Wangle 的 ReadMe.md 中。
我将展示如何使用现代C++编写一个Echo服务器，相当于分布式系统开发中的“Hello World”。这个服务器会将接收的消息直接返回。我们同时需要一个可以向我们的服务器发动消息的客户端，在这里可以发现客户端的[源码](https://github.com/facebook/wangle/tree/master/wangle/example/echo)。
Wangle是一个用来搭建事件驱动的现代异步C++服务的C/S应用框架。Wangle最基本的抽象概念就是Pipeline(管线)。能够理解这种抽象，将会很容易写出各种复杂的现代C++服务，另一个重要的概念是Service(服务)，其可以看作一种更高级的Pipeline，不过超出了本文我们关注的范畴。
### PipeLine
pipeline 是 Wangle 中最重要也是最强大的抽象，可以让用户在定制 request 和 response 的实现时拥有很大的自由。一个pipeline就是一系列request/response控制程序的嵌套。我试图寻找一个真实世界中pipeline的类比，唯一我能想到的就是现实世界工厂中的生产线。一条生产线工作在一种顺序模式下，所有的工人取得一个物体，并且只添加一种修改，再将其发送给上游的工人直到整个产品制造完成。这可能不是一个特别好的比喻,因为流水线上产品的流动是单向的，而一个pipeline能控制反方向的数据流动－－就好像将成品分解成原材料。
一个Wangle handler可以同时掌控上游和下游的两个方向的数据流动。当你把所有的handler连接在一起，就可以用一种灵活的方式将原始数据组装为想要的数据类型或者将已有的数据拆分。
在我们的服务器的pipeline中大致将会有下面几种handler:
1.Handler 1 (下文的上游下游是指对一同个handler而言，根据其在pipeline中的位置不同，输入输出相反) 上游：将从socket中接收的二进制数据流写入一个零拷贝(zero-copy,指省略了Applicaion context和Kernel context之间的上下文切换，避免了CPU对Buffer的冗余拷贝，直接在Kernel级别进行数据传输的技术，详情请参阅维基百科)的字节缓存中，发送给handler2
下游：接收一个零拷贝的字节缓存，将其内容写入socket中
2.Handler2 上游：接收handler1的缓存对象，解码为一个string对象传递给handler3 下游：接收handler3的string对象，将其转码为一个零拷贝的字节缓存，发送给handler1
3.Handler3 上游：接收handler2中的string对象，再向下发送至pipeline等待写回客户端。string会发回handler2 下游：接收上游的string对象，传递给handler2
需要注意的一点是，每一个handler应当只做一件事并且只有一件，如果你有一个handler里做了多项任务,比如从二进制流离直接解码出string，那么你需要学会将它拆分。这对提升代码的可维护性和扩展性非常重要。
另外，没错，handler不是线程安全的,所以不要轻易的在其中使用任何没有经过mutex，atomic lock保护的数据，如果你确实需要一个线程安全的环境，Folly提供了一种免于加锁的数据结构, Folly依赖于Wangle，你可以很容易的在项目中引入并使用它。
如果你还不是很明白所有的步骤，不用着急，在看到下面的具体实现时你会更加清楚。
### Echo Server
下面我会展示服务器的具体实现。我假定您已经安装好Wangle。需要注意的是截至目前Wangle还不能在Mac OS上安装，我建议您可以安装虚拟机，使用Ubuntu来安装Wangle。
这就是echo handler：接收一个string，打印到stdout中，再发送回pipeline。要注意write语句中的定界符不可以省略，因为pipeline会按照字节解码。

C++
```
// the main logic of our echo server; receives a string and writes it straight
// back
class EchoHandler : public HandlerAdapter {
 public:
  virtual void read(Context* ctx, std::string msg) override {
    std::cout << "handling " << msg << std::endl;
    write(ctx, msg + "rn");
  }
};
```
Echohandler其实是我们pipeline的最后一个handler，现在我们需要创建一个PipelineFactory来控制所有的request和response。

C++
```
// where we define the chain of handlers for each messeage received
class EchoPipelineFactory : public PipelineFactory {
 public:
  EchoPipeline::Ptr newPipeline(std::shared_ptr sock) {
    auto pipeline = EchoPipeline::create();
    pipeline->addBack(AsyncSocketHandler(sock));
    pipeline->addBack(LineBasedFrameDecoder(8192));
    pipeline->addBack(StringCodec());
    pipeline->addBack(EchoHandler());
    pipeline->finalize();
    return pipeline;
  }
};
```
pipeline中每一个handler的插入顺序都需要严格注意，因为它们是按照先后排序的，此处我们有4个handler
1.AsyncSocketHandler: 上游：读取scoket中的二进制流转换成零拷贝字节缓存 下游：将字节缓存内容写入底层socket 2. LineBasedFrameDecoder: 上游：接收字节缓存，按行分割数据 下游：将字节缓存发送给AsyncSocketHandler 3. StringCodec: 上游：接收字节缓存，解码为std:string传递给EchoHandler 下游：接收std:string, 编码为字节缓存,传递给LineBasedFrameDecoder 4. EchoHandler: 上游：接收std:string对象，将其写入pipeline－将消息返回给Echohandler。 下游：接收一个std:string对象，转发给StringCodec Handler。 现在我们所需要做的就是将pipeline factory关联到ServerBootstrap，绑定一个端口，这样我们已经完成了 基本上所有的工作。

C++
```
#include <gflags/gflags.h>
#include <wangle/bootstrap/ServerBootstrap.h>
#include <wangle/channel/AsyncSocketHandler.h>
#include <wangle/codec/LineBasedFrameDecoder.h>
#include <wangle/codec/StringCodec.h>
using namespace folly;
using namespace wangle;
DEFINE_int32(port, 8080, "echo server port");
typedef Pipeline<IOBufQueue&, std::string> EchoPipeline;
// the main logic of our echo server; receives a string and writes it straight
// back
class EchoHandler : public HandlerAdapter<std::string> {
 public:
  virtual void read(Context* ctx, std::string msg) override {
    std::cout << "handling " << msg << std::endl;
    write(ctx, msg + "\r\n");
  }
};
// where we define the chain of handlers for each messeage received
class EchoPipelineFactory : public PipelineFactory<EchoPipeline> {
 public:
  EchoPipeline::Ptr newPipeline(std::shared_ptr<AsyncTransportWrapper> sock) {
    auto pipeline = EchoPipeline::create();
    pipeline->addBack(AsyncSocketHandler(sock));
    pipeline->addBack(LineBasedFrameDecoder(8192));
    pipeline->addBack(StringCodec());
    pipeline->addBack(EchoHandler());
    pipeline->finalize();
    return pipeline;
  }
};
int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  ServerBootstrap<EchoPipeline> server;
  server.childPipeline(std::make_shared<EchoPipelineFactory>());
  server.bind(FLAGS_port);
  server.waitForStop();
  return 0;
}
```
至此我们一共只写了48行代码就完成了一个高性能的异步C++服务器。
### Echo Client
echo客户端的实现与我们的服务端非常类似:

C++
```
// the handler for receiving messages back from the server
class EchoHandler : public HandlerAdapter {
 public:
  virtual void read(Context* ctx, std::string msg) override {
    std::cout << "received back: " << msg;
  }
  virtual void readException(Context* ctx, exception_wrapper e) override {
    std::cout << exceptionStr(e) << std::endl;
    close(ctx);
  }
  virtual void readEOF(Context* ctx) override {
    std::cout << "EOF received :(" << std::endl;
    close(ctx);
  }
};
```
注意我们重载了readException和readEOF两个方法，还有其他一些方法可以被重载。如果你需要控制某个特别的事件，只需要重载对应的虚函数即可。
这是客户端的pipeline factory的实现，与我们的服务端结构基本一致，只有EventBaseHandler这个handler在服务端代码中不曾出现，它可以确保我们可以从任意一个线程写入数据。

C++
```
// the handler for receiving messages back from the server
class EchoHandler : public HandlerAdapter {
 public:
  virtual void read(Context* ctx, std::string msg) override {
    std::cout << "received back: " << msg;
  }
  virtual void readException(Context* ctx, exception_wrapper e) override {
    std::cout << exceptionStr(e) << std::endl;
    close(ctx);
  }
  virtual void readEOF(Context* ctx) override {
    std::cout << "EOF received :(" << std::endl;
    close(ctx);
  }
};
```
客户端所有的代码如下图所示

C++
```
#include <gflags/gflags.h>
#include 
#include <wangle/bootstrap/ClientBootstrap.h>
#include <wangle/channel/AsyncSocketHandler.h>
#include <wangle/channel/EventBaseHandler.h>
#include <wangle/codec/LineBasedFrameDecoder.h>
#include <wangle/codec/StringCodec.h>
using namespace folly;
using namespace wangle;
DEFINE_int32(port, 8080, "echo server port");
DEFINE_string(host, "::1", "echo server address");
typedef Pipeline<folly::IOBufQueue&, std::string> EchoPipeline;
// the handler for receiving messages back from the server
class EchoHandler : public HandlerAdapter {
 public:
  virtual void read(Context* ctx, std::string msg) override {
    std::cout << "received back: " << msg;
  }
  virtual void readException(Context* ctx, exception_wrapper e) override {
    std::cout << exceptionStr(e) << std::endl;
    close(ctx);
  }
  virtual void readEOF(Context* ctx) override {
    std::cout << "EOF received :(" << std::endl;
    close(ctx);
  }
};
// chains the handlers together to define the response pipeline
class EchoPipelineFactory : public PipelineFactory {
 public:
  EchoPipeline::Ptr newPipeline(std::shared_ptr sock) {
    auto pipeline = EchoPipeline::create();
    pipeline->addBack(AsyncSocketHandler(sock));
    pipeline->addBack(
        EventBaseHandler()); // ensure we can write from any thread
    pipeline->addBack(LineBasedFrameDecoder(8192, false));
    pipeline->addBack(StringCodec());
    pipeline->addBack(EchoHandler());
    pipeline->finalize();
    return pipeline;
  }
};
int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  ClientBootstrap client;
  client.group(std::make_shared(1));
  client.pipelineFactory(std::make_shared());
  auto pipeline = client.connect(SocketAddress(FLAGS_host, FLAGS_port)).get();
  try {
    while (true) {
      std::string line;
      std::getline(std::cin, line);
      if (line == "") {
        break;
      }
      pipeline->write(line + "rn").get();
      if (line == "bye") {
        pipeline->close();
        break;
      }
    }
  } catch (const std::exception& e) {
    std::cout << exceptionStr(e) << std::endl;
  }
  return 0;
}
```
程序用一个While循环不断监测用户的输入,并且依靠调用.get() 来同步等待一直到请求被响应。
### 总结
本文我展示了如何用Wangle来编写一个简易的高性能C++服务器。您应该已经掌握了Wangle的一些基本知识，并且有信心写出自己的C++服务器。我建议您深入了解Wangle中的Service概念，它会有助您开发出更加强大的服务器。
