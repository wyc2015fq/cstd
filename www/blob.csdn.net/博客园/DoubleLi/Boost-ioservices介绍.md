# Boost-ioservices介绍 - DoubleLi - 博客园






IO模型

io_service对象是asio框架中的调度器，所有异步io事件都是通过它来分发处理的（io对象的构造函数中都需要传入一个io_service对象）。

    asio::io_service io_service;
    asio::ip::tcp::socket socket(io_service);

在asio框架中，同步的io主要流程如下：

![](https://images0.cnblogs.com/blog/12132/201302/03173754-add23ebf90fd4953bde0a13c0a7ec9ac.jpg)
- 
应用程序调用IO对象成员函数执行IO操作

- 
IO对象向io_service 提出请求.

- 
io_service 调用操作系统的功能执行连接操作.

- 
操作系统向io_service 返回执行结果.

- 
io_service将错误的操作结果翻译为boost::system::error_code类型，再传递给IO对象.

- 
如果操作失败,IO对象抛出boost::system::system_error类型的异常.


而异步IO的处理流程则有些不同：

![](https://images0.cnblogs.com/blog/12132/201302/03173755-527f5fe81eec4958b5fea912ef5df7ce.jpg)
- 
应用程序调用IO对象成员函数执行IO操作

- 
IO对象请求io_service的服务

- 
io_service 通知操作系统其需要开始一个异步连接.

- 
操作系统指示连接操作完成, io_service从队列中获取操作结果

- 
应用程序必须调用io_service::run()以便于接收结果

- 
调用io_service::run()后,io_service返回一个操作结果,并将其翻译为error_code,传递到事件回调函数中


io_service对象

io_service对象主要有两个方法——post和run：
- 
post用于发布io事件，如timer，socket读写等，一般由asio框架相应对象调用，无需我们显式调用。

- 
run用于监听io事件响应，并执行响应回调，对于异步io操作需要在代码中显式调用，对于同步io操作则由io对象隐式调用（并不是run函数，不过也是等待io事件）。


可见，io_service提供的是一个生产者消费者模型。在异步io操作中需要我们手动控制消费者，调用run函数，它的基本工作模式如下：
- 
等待io事件响应，如果所有io事件响应完成则退出

- 
等待到io事件响应后，执行其对应的回调

- 
继续等待下一个io事件，重复1-2


从中可以看出，io_service是一个工作队列的模型。在使用过程中一般有如下几个需要注意的地方：

1. run函数在io事件完成后会退出，导致后续基于该对象的异步io任务无法执行

由于io_service并不会主动常见调度线程，需要我们手动分配，常见的方式是给其分配一个线程，然后执行run函数。但run函数在io事件完成后会退出，线程会终止，后续基于该对象的异步io任务无法得到调度。

解决这个问题的方法是通过一个asio::io_service::work对象来守护io_service。这样，即使所有io任务都执行完成，也不会退出，继续等待新的io任务。

    boost::asio::io_service io;
    boost::asio::io_service::work work(io);
    io.run();

2. 回调在run函数的线程中同步执行，当回调处理时间较长时阻塞后续io响应

解决这个问题的方法有两种：1. 启动多线程执行run函数（run函数是线程安全的），2. 新启动一个线程（或通过线程池）来执行回调函数。一般来讲，如果回调处理事件不是特别短，应该使用在线程池中处理回调的方式。

3. 回调在run函数的线程中同步执行，io事件较多的时候得不到及时响应

这个其实是性能问题了，在多核cpu上可以通过在多个线程中执行run函数来解决这一问题。这种方式也只能充分利用cpu性能，本身性能问题就不是光靠软件就能解决的。

.net中的异步io调度方式

和io_service这种手动控制的方式比起来，.net则是纯粹的自动档了。IO调度由CLR托管了，无需手动控制。回调也是在线程池中执行，无需担心影响后续IO响应。

正是由于CLR的托管，在.net 的异步IO框架中，就没有类似io_service的调度对象存在，这也符合.net的一贯简洁做法。









