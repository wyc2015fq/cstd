# [Asio] Using Asio with C++11 - Pangda NoSilly - CSDN博客

2018年12月24日 17:37:17[hepangda](https://me.csdn.net/hepangda)阅读数：203


# Using Asio with C++11

这篇文章既作为Asio网络库的一个介绍，也作为Asio网络库与C++11进行协同实现和使用的一个简单的概览。

本文主要介绍的不是跟随Boost发布的Asio库，而是Asio独立于Boost的一个版本，我们所介绍的这个版本的Asio有以下几个目标：
- 在库接口上使用C++11语言和标准库特性。
- 证明这个库可以在仅仅使用C++11标准库和操作系统提供的设施下实现

你可以在这里找到这个版本的Asio源代码：
[http://github.com/chriskohlhoff/asio/tree/cpp11-only](http://github.com/chriskohlhoff/asio/tree/cpp11-only)

## 使用I/O流来解决简单情况

在很多应用程序里，网络环节不是其核心特性，也不把网络环节看做应用开发者的核心竞争力。那么，为了迎合这些情况，Asio提供了一个TCP套接字的高阶抽象的接口，它是根据大家很熟悉的C++输入输出流(iostream)来设计的。

用这个方法来使用这个库的话，创建一个流对象将异常简单，简单到仅仅需要提供远程主机的一些细节，下面就是一个构建TCP连接的例子：

```
tcp::iostream s("www.boost.org", "http");
```

在构建完流对象之后，取决于你想要当主机在多长时间内无响应时放弃，你可以通过以下方式来设置超时时间，我们以60秒超时为例：

```
s.expires_from_now(std::chrono::seconds(60));
```

然后，你可以发送数据，并且如果需要也可以选择接受数据。比如在这个例子中，我们发送了一个HTTP请求：

```
s << "GET / HTTP/1.0\r0.2cm\n";
s << "Host: www.boost.org\r\n";
s << "Accept: */*\r\n";
s << "Connection: close\r\n\r\n";
```

然后我们接受数据并处理这些响应，在本例中，我们接收并打印收到的信息:

```
std::string header;
while (std::getline(s, header) && header != "\r")
  std::cout << header << "\n";
std::cout << s.rdbuf();
```

如果在任何时候出现了错误，那么，你可以使用`tcp::iostream`的成员函数`error()`来确定出现错误的原因。

```
if (!s) {
  std::cout << "Socket error: " << s.error().message() << "\n";
  return 1;
}
```

## 理解同步操作

**同步操作**(synchronous operations)是指那些直到其对应的系统调用完成为止，不会将控制权返还给调用者的函数。在基于Asio的程序中，同步操作一般用于以下两类情况：
- 不在意超时的简单程序，或是需要依赖底层操作系统提供的超时行为的情况
- 需要对系统调用进行细粒度的控制，并且已经了解到同步操作是否将阻塞的情况

Asio既可以对I/O对象（比如：套接字）进行同步操作，也可以进行异步操作。然而，同步操作可以更简单化的叙述Asio的各个部分、你的程序、以及他们如何协同工作。接下来，作为一个介绍性质的例子，让我们看一看，当你在套接字进行同步的connect操作时，将发生什么：

首先，你的程序需要有至少一个`io_service`对象，`io_service`对象代表了你的程序与操作系统的I/O服务的连接：

```
asio::io_service io_service;
```

为了进行I/O操作，你的程序需要一个I/O对象，比如说，一个TCP套接字：

```
tcp::socket socket(io_service);
```

当进行一个同步connect操作时，将发生下面一系列事件：

![[图1]](https://img-blog.csdnimg.cn/20181224173627584.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh,size_16,color_FFFFFF,t_70)
- 你的程序通过I/O对象的调用**发起**(initiate)一次同步connect操作：

```
socket.connect(server_endpoint);
```
- I/O对象将请求转发给`io_service`；
- `io_service`对象通过操作系统，进行connect操作；
- 操作系统返回操作结果给`io_service`对象；
- `io_service`对象将操作产生的错误结果翻译为一个`std::error_code`类型的对象，然后这个结果被转发回I/O对象。
- 若操作失败，那么I/O对象将抛出一个类型为`std::system_error`的异常。若在发起该操作时，传入一个`std::error_code`类型的参数：

```
std::error_code ec;
socket.connect(server_endpoint, ec);
```

那么，这个`std::error_code`类型的参数（下面例子中的`ec`）将被设为该操作结果所对应的错误码，并且不抛出异常。

## 理解异步操作

**异步操作**(asynchronous operations)将不阻塞调用者，而是当对应的系统调用完成时，给程序以通知。大部分基于Asio的程序，都使用异步操作。

当进行一个异步connect操作时，将发生下面的一系列事件：

![[图2]](https://img-blog.csdnimg.cn/20181224173619148.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh,size_16,color_FFFFFF,t_70)
- 你的程序通过I/O对象**发起**(initiate)一个异步connect操作：

```
socket.async_connect(server_endpoint, your_completion_handler);
```

`async_connect()`函数是一个**发起函数**(initiating function)。在Asio中发起函数的名字前面都有`async_`。一个发起函数的最后一个参数应当是一个函数对象，我们称之为**完成处理程序**(completion handler)。在本例中，`your_completion_handler`就是完成处理程序。完成处理程序应当是一个具有以下签名形式的函数或函数对象：

```
void your_completion_handler(const std::error_code& ec);
```

完成处理程序的确切签名形式取决于你所进行的异步操作，每个异步操作所需的完成处理程序的签名形式可以在Asio的参考文档中查阅到。
- I/O对象将请求转发给`io_service`。
- `io_service`通知操作系统，应当开始一个异步的connect操作。

过去一段时间之后。（若是同步操作的情况，这里将等待连接操作，直到其进行完毕。）在这段时间中，理论上由操作系统负责异步操作，我们将之称为**未完成的工作**(outstanding work)。

![[图3]](https://img-blog.csdnimg.cn/20181224173606792.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh,size_16,color_FFFFFF,t_70)
- 
操作系统将connect操作的结果放置在队列中指示操作已经完成，等待被`io_service`接收。

- 
你的程序必须调用`io_service::run()`函数（或者`io_service`下其他类似的成员函数之一），才能得到异步操作的结果。当在有**未完成的工作**(outstanding work)时调用`io_service::run()`函数，会阻塞调用者。一般而言，你会在启动你的第一个异步操作之后立即调用这个函数。

- 
调用`io_service`类的`run()`函数时，`io_service`逐个地从结果队列中取出异步操作的结果，将之翻译为一个`std::error_code`类，并把它传递给你的**完成处理程序**(completion handler)。

## 链接异步操作

一个异步操作所关联的**完成处理程序**(completion handler)被调用并返回之前，它都将被认为是一个**未完成的工作**(outstanding work)。而且，**完成处理程序**可以依次调用其他的**发起函数**(initiating function)，从而创建更多**未完成的工作**。

接下来，让我们通过这样一个例子来理解异步操作的“链接”：一个套接字进行了异步connect操作之后紧接着其他的I/O操作：

```
socket.async_connect(server_endpoint, [&](std::error_code ec) {
 if (!ec) {
  socket.async_read_some(asio::buffer(data), [&](std::error_code ec, std::size_t length) {
    if (!ec) {
      async_write(socket, asio::buffer(data, length), [&](std::error_code ec, std::size_t length) {
        // ...
      });
    }
  });
 }
});
```

在这个例子中，异步connect操作的**完成处理程序**（此处表现为C++11的新特性——lambda表达式）发起了一个异步read操作，正如上面所说的，这等同于connect操作创建了一个新的**未完成的工作**(outstanding work)。而且，read操作的**完成处理程序**也创建了一个“**工作**”——一个异步write操作。这三个操作构成了一个“异步操作链”，在这条链上的所有操作都完成之前，`io_service::run()`函数都不会返回（当然也不会解除该函数带来的阻塞）。

当然，以上仅仅使我们为了方便叙述的一个简单的例子，在真正的程序中，这些“异步操作链”一般都是很长的，而且可能包含很多的循环和分支。在这些程序中，`io_service::run()`函数可能是无限执行下去的。

## 处理错误

Asio认为，异常不总是处理错误的正确方法。例如，在网络编程中，可能会遇到这样一些错误：
- 无法连接到远程IP地址
- 网络连接中断了
- 尝试打开一个IPv6套接字，但没有可用的IPv6网络接口

当然，这些可能是一些“异常情况”，但是他们同样可以作为正常控制流程的一部分所处理。特别是，如果你可以预计以上情况的发生，并想以这些方案解决它们：
- 这个IP地址是某个主机所对应的IP地址列表中的一个，你想要尝试连接列表中下一个地址
- 网络状况不可靠，除非失败了n次，否则重新建立连接
- 退而求其次地使用IPv4套接字

我们可以看出，一个“错误”是否是一个“异常”取决于程序的上下文。如果我们没有计划去处理这些情况，那么它们就是一个“异常”，如果我们早就预计到了这一情况，并具有解决方案，那么就不应当是一个“异常”。

此外，在某些领域的应用中，受限于代码大小和性能限制，不愿意或不能使用异常。所以，在Asio中，所有的同步操作提供了两种重载形式，一种是会抛出异常的重载形式：

```
socket.connect(server_endpoint); // 发生错误时抛出std::system_error异常
```

而另一种则是不抛出异常的重载形式：

```
std::error_code ec;
socket.connect(server_endpoint, ec); // 设定ec变量来表示异常
```

出于类似的原因，Asio并不将发生错误和不发生错误的**完成处理程序**(completion handler)分开作为不同的两个参数。因为如果这样做的话，会在异步操作链中产生分支，并且可能会和程序关于什么是一个“错误”的想法不一致。

## 管理对象的生命周期

当我们选择并使用异步操作时，会遇到一个使用同步操作遇不到的挑战——对象生命周期的管理问题。Asio并没有明确的设施来支持对象生命周期管理，相反的，这需要我们的程序来自行控制。根据**发起函数**(initiating function)声明形式的不同，有这样一些规则：
- 
若参数是：值对象、常引用和右值引用

那么这些参数将根据库实现的要求复制或移动它们，直到不再需要为止。举个例子来说，库实现将维持完成处理者对象的一份拷贝，直到**完成处理程序**被调用为止。

- 
若参数是：非const引用，this指针

那么程序将有责任保证直到该异步操作完成时，该对象仍然有效。

对于这种情况，由于**完成处理程序**的生命周期很容易搞清楚，所以，很多Asio程序采用的方法是都是将该对象的生命周期与**完成处理程序**的生命周期关联起来。我们可以通过使用`std::shared_ptr<>`和`std::enable_shared_from_this<>`来实现，如下面这个例子，就保证了在异步操作`async_read_some`结束之前，由`std::shared_ptr<>`管理的当前`connection`不会失效：

```
class connection : std::enable_shared_from_this<connection> {
  tcp::socket socket_;
  vector<unsigned char> data_;
  // ...
  void start_read() {
    socket_.async_read_some(socket_, asio::buffer(data_), 
                            std::bind(&connection::handle_read, shared_from_this(), _1, _2));
  }
  // ...
};
```

使用C++11的这个方法，可以在易用性和性能之间达成一个完美的平衡。Asio也能通过`std::move`传递参数给**完成处理程序**来最小化`std::shared_ptr<>`带来的引用计数的开销（的引用计数开销）。由于程序通常来说都由异步操作链构成，指针的所有权在链中转移，那么引用计数仅仅在链的开始和结束时被更新。

然而，一些程序需要对对象的生命周期、内存使用和执行效率进行细粒度的控制。由于Asio将对象生命周期放在程序的控制下，因而这种需求也能得到满足。举个例子，有些程序需要在程序开始时，创建所有的对象，那么在这时，**完成处理程序**就与对象生命周期的管理无关，而可能只是进行普通的复制操作而已。

## 优化内存分配

很多异步操作需要使用一个对象去存储与操作相关联的状态。比如说，在一个Windows程序中需要给Win32 API传递一个派生自OVERLAPPED结构体的对象。

一般来说，Asio将为它使用`::operator new()`来分配一块空间。然而，我们使用的“异步操作链”给予了我们一个优化思路——异步操作链上的每个异步操作只持有和它所对应的某块部分的内存，并且，具有相同意义的内存区域可以在链中的被重复利用。这样，我们就不必不断地进行内存分配。

这个自定义内存分配的关键之处是异步操作的**完成处理程序**(completion handler)。**完成处理程序**标识了正在执行操作中较大的上下文。通过传递**完成处理程序**给**发起函数**(initiating function)，Asio可以在通知系统开始一个异步操作之前分配好必要的内存。

## 处理并发

一个网络程序的实现通常涉及到多个异步操作链的协调工作。举个例子说，一个操作链可能处理消息发送，另一个处理消息接受，第三个可能来实现应用层的超时。所有这些链都需要来访问共同的变量，比如说套接字、定时器和缓冲区。除此之外，这些异步操作可能无限期的继续下去。

异步操作提供了一种不需要考虑线程复杂性和开销的实现并发的方法。Asio的接口支持一系列的线程方法，以下是一些例子：

### 单线程设计

由于Asio保证**完成处理程序**只在`io_service::run()`函数中被调用。因此，通过只在一个线程中调用`io_service::run()`函数，程序可以避免**完成处理程序**的并行执行。

我们推荐大多数的程序都从这个原型开始设计，因为这种设计不需要显式的同步机制。但是，使用这种设计时，你必须保持**完成处理程序**的简短，并且其中不能包含任何阻塞操作（否则其他操作将无法执行）。

### 单线程+线程池设计

这种方法是上面的变体，我们仍然只有一个线程执行`io_service::run()`了来实现程序逻辑，但长时间运行或者将阻塞的任务将被传递到后台线程中，当其完成后，将结果发回执行`io_service::run()`的线程。

由于这里没有消息的共享，程序可以确保对象不会在`io_service::run()`线程和任何后台工作线程中被共享。因此，这种方法同样不需要显式的同步机制。

### 多线程+每个线程一个io_service

在这种设计方法中，I/O对象们被赋予某一个运行在单线程中的io_service，不同的对象仅仅通过消息传递来交流。这种设计比起可以更高效的利用多核CPU，并且限制了竞争源。这种设计方法同样的不需要显式的同步机制，但是，仍然需要保持**完成处理程序**的简短和无阻塞。

### 多线程+共用同一个io_service

多个线程组成一个线程池来共同使用同一个`io_service`，它的`run()`函数可能被线程池中的任何一个线程使用。通过这种实现方式，我们可以用任意方式跨线程地分发可用的工作。

由于**完成处理程序**可能被任何线程调用，因此，除非程序是简易平凡，或由单个异步操作链构成的。否则则需要某种形式的同步机制。Asio提供了`io_service::strand`类来完成这个目标。

处于同一个`strand`上的**完成处理程序**将被保证不会同时执行。我们仍以三个异步操作链（处理发送、消息接受、应用层超时）为例，在使用这种技术时，每一个`strand`上异步操作链的**完成处理程序**将会被顺序地执行。当然，其他`strand`上异步操作链仍然可以利用线程池中的其他线程。这种实现与互斥锁不同，如果一个`strand`正在被使用，执行`run()`函数将不会阻塞调用线程去等待该`strand`就绪，而是去执行其他`strand`上已经就绪的**完成处理程序**来保证线程处于忙碌状态。

与自定义内存分配机制一样，`strand`的同步机制是与**完成处理程序**相“关联”的。 也就是说，由**完成处理程序**去识别正在执行操作的较大上下文。 这样的自定义调用“关联”使得同步机制扩展到我们将在下面介绍的基于“组合操作”的抽象。

## 权责转移：开发高效的抽象

Asio的设计目标之一就是创建更高层级的抽象，而我们创建抽象最主要的机制就是异步操作的组合，在Asio的术语中，我们简单的将之称为“组合操作”。

我们假设有这样一个用户定义的异步操作：该操作实现从一个套接字读取数据并把读取到的数据写入另一个套接字，也就是在两个套接字之间转发数据，假设我们将**发起函数**(initiating function)定义为：

```
template <typename Handler>
void async_transfer(tcp::socket& socket1, tcp::socket& socket2,
                    std::array<unsigned char, 1024>& working_buffer, Handler handler);
```

这个函数将根据两个底层异步操作实现：从socket1读取和写入socket2。这些操作中的每一个都有一个**中间完成处理程序**(intermediate completion handler)，它们之间的关系如下图所示：

![[图4]](https://img-blog.csdnimg.cn/2018122417352388.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh,size_16,color_FFFFFF,t_70)

这些**中间处理程序**可以通过自定义内存分配、自定义调用“关联”来进行“权责转移”，只是去调用用户定义的**完成处理程序**。通过这种方式，组合操作把对内存分配和同步机制的选择，转交给了使用这种抽象的用户手中。这样，用户就可以在易用性和效率之间进行权衡，如果用户不需要显式的同步机制，那么程序将不会受到同步机制的拖累。

Asio提供了许多开箱即用的“组合操作”，例如非成员函数版本的async_connect()，async_read()，async_write()和async_read_until()。密切相关的“组合操作”也可以分组存在于对象中，就像Asio中的`buffered_stream<>`和`ssl::stream<>`一样。

## 规模和拓展性

考虑到Asio库的大小，在WG21之前，Asio仅仅实现了关于TCP、UDP、缓冲区和定时器的网络编程。但是，Asio的接口允许用户通过多种机制来实现用户和实现者的可拓展性。下面是其中的一些例子：

### 额外的I/O服务

`io_service`类实现了一组由服务类型索引的，可拓展、类型安全且多态的I/O服务。I/O服务是为了管理与I/O对象所代表的操作系统的逻辑接口而存在的，特别地，有一些资源是在同一类型的I/O对象之间共享的。例如，定时器可以根据单个定时器队列来实现。我们由I/O服务来管理这些共享的资源，这样，如果我们不使用它们，那么我们就不必为之产生开销。

Asio实现了一些额外的I/O服务，通过这些服务我们可以对特定操作系统特定功能的访问。比如：
- [Windows限定] 在**句柄**(HANDLE)上执行**重叠I/O**(overlapped I/O)
- [Windows限定] 等待事件，进程和线程等内核对象
- [POSIX限定] 流式文件的文件描述符
- 通过`signal()`或POSIX的`sigaction()`进行安全集成的信号处理服务

Asio可能在不影响库现有用户的前提下添加新的I/O服务。

### 套接字类型需求

虽然Asio的提案仅限于TCP和UDP套接字，但是，Asio的接口设计是基于**类型需求**(type requirement)的，比如说Asio中的Protocol和Endpoint。这样的设计使得库可以与其他类型的套接字一起使用。Asio本身已经根据这些类型需求，添加了对ICMP和Unix域套接字的支持。

### 流类型需求

Asio库为同步和异步面向流的I/O定义了几种**类型需求**。这些**类型要求**在Asio中，由TCP套接字实现、`ssl::stream<>`和`buffered_stream<>`等抽象来实现。通过实现这些**类型需求**，一个类将可以与async_read()，async_read_until()和async_write()等组合操作一起使用。**类型需求**也适用于更高级别的抽象，例如对HTTP进行异步包装。

## 原文地址

[http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3388.pdf](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3388.pdf)

