# boost asio中io_service类的几种使用 - DoubleLi - 博客园






io_service类

你应该已经发现大部分使用Boost.Asio编写的代码都会使用几个io_service的实例。io_service是这个库里面
最重要的类；它负责和操作系统打交道，等待所有异步操作的结束，然后为每一个异步操作调用其完成处
理程序。
如果你选择用同步的方式来创建你的应用，你则不需要考虑我将在这一节向你展示的东西。 你有多种不同
的方式来使用io_service。在下面的例子中，我们有3个异步操作，2个socket连接操作和一个计时器等待操
作：
有一个io_service实例和一个处理线程的单线程例子：
io_service service; // 所有socket操作都由service来处理
ip::tcp::socket sock1(service); // all the socket operations are handled by service
ip::tcp::socket sock2(service);

sock1.asyncconnect( ep, connect_handler);
sock2.async_connect( ep, connect_handler);
deadline_timer t(service, boost::posixtime::seconds(5));
t.async_wait(timeout_handler);
service.run();
有一个io_service实例和多个处理线程的多线程例子：
io_service service;
ip::tcp::socket sock1(service);
ip::tcp::socket sock2(service);
sock1.asyncconnect( ep, connect_handler);
sock2.async_connect( ep, connect_handler);
deadline_timer t(service, boost::posixtime::seconds(5));
t.async_wait(timeout_handler);
for ( int i = 0; i < 5; ++i)
boost::thread( run_service);
void run_service()
{
service.run();
}
有多个io_service实例和多个处理线程的多线程例子：
io_service service[2];
ip::tcp::socket sock1(service[0]);
ip::tcp::socket sock2(service[1]);
sock1.asyncconnect( ep, connect_handler);
sock2.async_connect( ep, connect_handler);
deadline_timer t(service[0], boost::posixtime::seconds(5));
t.async_wait(timeout_handler);
for ( int i = 0; i < 2; ++i)
boost::thread( boost::bind(run_service, i));

void run_service(int idx)
{
service[idx].run();
}
首先，要注意你不能拥有多个io_service实例却只有一个线程。下面的代码片段没有任何意义：
for ( int i = 0; i < 2; ++i)
service[i].run();
上面的代码片段没有意义是因为service[1].run()需要service[0].run()先结束。因此，所有由service[1]处理的
异步操作都需要等待，这显然不是一个好主意。
在前面的3个方案中，我们在等待3个异步操作结束。为了解释它们之间的不同点，我们假设：过一会操作1
完成，然后接着操作2完成。同时我们假设每一个完成处理程序需要1秒钟来完成执行。
在第一个例子中，我们在一个线程中等待三个操作全部完成，第1个操作一完成，我们就调用它的完成处理
程序。尽管操作2紧接着完成了，但是操作2的完成处理程序需要在1秒钟后，也就是操作1的完成处理程序
完成时才会被调用。
第二个例子，我们在两个线程中等待3个异步操作结束。当操作1完成时，我们在第1个线程中调用它的完成
处理程序。当操作2完成时，紧接着，我们就在第2个线程中调用它的完成处理程序（当线程1在忙着响应操
作1的处理程序时，线程2空闲着并且可以回应任何新进来的操作）。
在第三个例子中，因为操作1是sock1的connect，操作2是sock2的connect，所以应用程序会表现得像第二
个例子一样。线程1会处理sock1 connect操作的完成处理程序，线程2会处理sock2的connect操作的完成处
理程序。然而，如果sock1的connect操作是操作1，deadline_timer t的超时操作是操作2，线程1会结束正
在处理的sock1 connect操作的完成处理程序。因而，deadline_timer t的超时操作必须等sock1 connect操
作的完成处理程序结束（等待1秒钟），因为线程1要处理sock1的连接处理程序和t的超时处理程序。

下面是你需要从前面的例子中学到的：
第一种情况是非常基础的应用程序。因为是串行的方式，所以当几个处理程序需要被同时调用时，你
通常会遇到瓶颈。如果一个处理程序需要花费很长的时间来执行，所有随后的处理程序都不得不等
待。
第二种情况是比较适用的应用程序。他是非常强壮的——如果几个处理程序被同时调用了（这是有可
能的），它们会在各自的线程里面被调用。唯一的瓶颈就是所有的处理线程都很忙的同时又有新的处
理程序被调用。然而，这是有快速的解决方式的，增加处理线程的数目即可。
第三种情况是最复杂和最难理解的。你只有在第二种情况不能满足需求时才使用它。这种情况一般就
是当你有成千上万实时（socket）连接时。你可以认为每一个处理线程（运行io_service::run()的线
程）有它自己的select/epoll循环；它等待任意一个socket连接，然后等待一个读写操作，当它发现这
种操作时，就执行。大部分情况下，你不需要担心什么，唯一你需要担心的就是当你监控的socket数
目以指数级的方式增长时（超过1000个的socket）。在那种情况下，有多个select/epoll循环会增加应
用的响应时间。
如果你觉得你的应用程序可能需要转换到第三种模式，请确保监听操作的这段代码（调用io_service::run()
的代码）和应用程序其他部分是隔离的，这样你就可以很轻松地对其进行更改。









