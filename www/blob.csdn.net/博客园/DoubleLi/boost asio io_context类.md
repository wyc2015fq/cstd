# boost::asio::io_context类 - DoubleLi - 博客园






```cpp
//有个疑惑： 向io_context对象中提交的任务只能被顺序化的执行.
//下面这个构造函数表明可以运行多线程啊。。。。。

  /**
   * Construct with a hint about the required level of concurrency.
   *
   * @param concurrency_hint A suggestion to the implementation on how many
   * threads it should allow to run simultaneously.
   */
  BOOST_ASIO_DECL explicit io_context(int concurrency_hint);
```

### io_context类为异步I/O对象提供核心功能，对象包括:
- boost::asio::ip::tcp::socket
- boost::asio::ip::tcp::acceptor
- boost::asio::ip::udp::socket
- boost::asio::deadline_timer

```cpp
boost::asio::io_context io_context;
...
for (;;)
{
  try
  {
    io_context.run();
    break; // run() exited normally
  }
  catch (my_exception& e)
  {
    // Deal with exception as appropriate.
  }
}
```

### 使用以下函数向io_context提交任务
- boost::asio::dispatch
- boost::asio::post
- boost::asio::defer

```cpp
void my_task()
{
  ...
}

...

boost::asio::io_context io_context;

// Submit a function to the io_context.
boost::asio::post(io_context, my_task);

// Submit a lambda object to the io_context.
boost::asio::post(io_context,
    []()
    {
      ...
    });

// Run the io_context until it runs out of work.
// Run the io_context until it runs out of work.
// Run the io_context until it runs out of work.
io_context.run(); 

io_context.restart();
boost::asio::post(io_context, my_task);

io_context.run();
```

### 如果希望在调用run()函数后即使做完任务后也不要run()函数返回，则可使用以下类实现

boost::asio::executor_work_guard

```cpp
asio::io_context io_context;

asio::executor_work_guard<asio::io_context::executor_type> 
    work = asio::make_work_guard(io_context);

......

work.reset(); // Allow run() to exit. @endcode
```

### 成员函数

#### 1. count_type run();

> - 调用 run() 函数后程序将被阻塞到任务被完成同时没用其他任务派遣，或者直到`io_context`调用 stop() 函数停止为止
- 多线程中可以调用 run() 函数来开启一个线程池，`io_context`可以在线程池中执行处理程序。在池中等待的所有线程都是等效的， `io_context`可以选择其中的任何一个线程来调用处理程序。
- 在 run() 函数正常退出后立即调用 `run()`、 `run_one()`、`poll()` 或 `poll_one()`函数将会立即返回，除非在调用这些函数前调用`restart()`函数。
- 返回被处理的程序的数量 count_type


#### 2. std::size_t run_for(const chrono::duration<Rep, Period>& rel_time);

> - 在一定时间内处理事件循环，阻塞到任务被完成同时没用其他任务派遣，或者直到`io_context`调用 stop() 函数停止 或 超时 为止
- `rel_time` : 表示时间段


#### 3. std::size_t run_until(const chrono::time_point<Clock, Duration>& abs_time);

> 
`abs_time` : 阻塞到哪个时间点


#### 4. count_type run_one();

> - 最多处理一个任务， 处理完就退出 或 io_context被停止
- 函数正常退出后立即调用 run()、 run_one()、poll() 或 poll_one() 函数将会立即返回，除非在调用这些函数前调用 restart() 函数。
`count_type` : 返回 **0** 表示io_conutext被停止


#### 5. std::size_t run_one_for(const chrono::duration<Rep, Period>& rel_time);

#### 6. std::size_t run_one_until(const chrono::time_point<Clock, Duration>& abs_time);

#### 7. count_type poll();

> - 以非阻塞方式处理任务
- 返回处理的任务数量


#### 8. count_type poll_one();

> - 以非阻塞方式处理任务，最多处理一个
- 返回 **0** 表示io_context被终止


#### 9. void stop();

> 
此函数将终止io_context对象的事件处理，此函数不阻塞，而只是向io_context发出停止信号。它的run()或run_one()成员函数的所有调用都应该尽快返回。对run()、run_one()、poll()或poll_one()的后续调用将立即返回，直到调用restart()。


#### 10. bool stopped() const;

> 
判断io_context对象的事件处理是否被终止


#### 11. void restart();

> 
重新启动io_context，为后续调用 run() 做准备。必须在run()、run_one()、poll()或poll_one()函数的第二次或更高的调用集之前调用此函数。










