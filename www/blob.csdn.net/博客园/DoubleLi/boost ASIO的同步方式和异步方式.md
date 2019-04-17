# boost::ASIO的同步方式和异步方式 - DoubleLi - 博客园






http://blog.csdn.net/zhuky/article/details/5364574

http://blog.csdn.net/zhuky/article/details/5364685



Boost.Asio是一个跨平台的网络及底层IO的C++编程库，它使用现代C++手法实现了统一的异步调用模型。

头文件
#include <boost/asio.hpp>
名空间
using namespace boost::asio;
ASIO库能够使用TCP、UDP、ICMP、串口来发送/接收数据，下面先介绍TCP协议的读写操作

对于读写方式，ASIO支持同步和异步两种方式，首先登场的是同步方式，下面请同步方式自我介绍一下：


大家好！我是同步方式！

我的主要特点就是执着！所有的操作都要完成或出错才会返回，不过偶的执着被大家称之为阻塞，实在是郁闷~~（场下一片嘘声），其实这样 也是有好处的，比如逻辑清晰，编程比较容易。

在服务器端，我会做个socket交给acceptor对象，让它一直等客户端连进来，连上以后再通过这个socket与客户端通信， 而所有的通信都是以阻塞方式进行的，读完或写完才会返回。

在客户端也一样，这时我会拿着socket去连接服务器，当然也是连上或出错了才返回，最后也是以阻塞的方式和服务器通信。

有人认为同步方式没有异步方式高效，其实这是片面的理解。在单线程的情况下可能确实如此，我不能利用耗时的网络操作这段时间做别的事 情，不是好的统筹方法。不过这个问题可以通过多线程来避免，比如在服务器端让其中一个线程负责等待客户端连接，连接进来后把socket交给另外的线程去 和客户端通信，这样与一个客户端通信的同时也能接受其它客户端的连接，主线程也完全被解放了出来。

我的介绍就有这里，谢谢大家！


好，感谢同步方式的自我介绍，现在放出同步方式的演示代码(起立鼓掌!):

#### 服务器端


- #include <iostream>
- #include <boost/asio.hpp>
- using namespace boost::asio;
- int main(int argc, char* argv[])
- {
- // 所有asio类都需要io_service对象
-      io_service iosev;
-      ip::tcp::acceptor acceptor(iosev,
-         ip::tcp::endpoint(ip::tcp::v4(), 1000));
- for(;;)
-      {
- // socket对象
-          ip::tcp::socket socket(iosev);
- // 等待直到客户端连接进来
-          acceptor.accept(socket);
- // 显示连接进来的客户端
-          std::cout << socket.remote_endpoint().address() << std::endl;
- // 向客户端发送hello world!
-          boost::system::error_code ec;
-          socket.write_some(buffer("hello world!"), ec);
- // 如果出错，打印出错信息
- if(ec)
-          {
-              std::cout <<
-                 boost::system::system_error(ec).what() << std::endl;
- break;
-          }
- // 与当前客户交互完成后循环继续等待下一客户连接
-      }
- return 0;
- }



#### 客户端


- #include <iostream>
- #include <boost/asio.hpp>
- using namespace boost::asio;
- int main(int argc, char* argv[])
- {
- // 所有asio类都需要io_service对象
-      io_service iosev;
- // socket对象
-      ip::tcp::socket socket(iosev);
- // 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
-      ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"), 1000);
- // 连接服务器
-      boost::system::error_code ec;
-      socket.connect(ep,ec);
- // 如果出错，打印出错信息
- if(ec)
-      {
-          std::cout << boost::system::system_error(ec).what() << std::endl;
- return -1;
-      }
- // 接收数据
- char buf[100];
- size_t len=socket.read_some(buffer(buf), ec);
-      std::cout.write(buf, len);
- return 0;
- }



从演示代码可以得知
- ASIO的TCP协议通过boost::asio::ip名 空间下的tcp类进行通信。
- IP地址（address,address_v4,address_v6）、 端口号和协议版本组成一个端点（tcp:: endpoint）。用于在服务器端生成tcp::acceptor对 象，并在指定端口上等待连接；或者在客户端连接到指定地址的服务器上。
- socket是 服务器与客户端通信的桥梁，连接成功后所有的读写都是通过socket对 象实现的，当socket析 构后，连接自动断 开。
- ASIO读写所用的缓冲区用buffer函 数生成，这个函数生成的是一个ASIO内部使用的缓冲区类，它能把数组、指针（同时指定大 小）、std::vector、std::string、boost::array包装成缓冲区类。
- ASIO中的函数、类方法都接受一个boost::system::error_code类 型的数据，用于提供出错码。它可以转换成bool测试是否出错，并通过boost::system::system_error类 获得详细的出错信息。另外，也可以不向ASIO的函数或方法提供 boost::system::error_code，这时如果出错的话就会直 接抛出异常，异常类型就是boost::system:: system_error(它是从std::runtime_error继承的)。

嗯？异步方式好像有点坐不住了，那就请异步方式上场，大家欢迎...


大家好，我是异步方式

和同步方式不同，我从来不花时间去等那些龟速的IO操作，我只是向系统说一声要做什么，然后就可以做其它事去了。如果系统完成了操作， 系统就会通过我之前给它的回调对象来通知我。

在ASIO库中，异步方式的函数或方法名称前面都有“async_” 前缀，函数参数里会要求放一个回调函数（或仿函数）。异步操作执行 后不管有没有完成都会立即返回，这时可以做一些其它事，直到回调函数（或仿函数）被调用，说明异步操作已经完成。

在ASIO中很多回调函数都只接受一个boost::system::error_code参数，在实际使用时肯定是不够的，所以一般 使用仿函数携带一堆相关数据作为回调，或者使用boost::bind来绑定一堆数据。

另外要注意的是，只有io_service类的run()方法运行之后回调对象才会被调用，否则即使系统已经完成了异步操作也不会有任 务动作。

好了，就介绍到这里，下面是我带来的异步方式TCP Helloworld服务器端:




- #include <iostream>
- #include <string>
- #include <boost/asio.hpp>
- #include <boost/bind.hpp>
- #include <boost/smart_ptr.hpp>
- using namespace boost::asio;
- using boost::system::error_code;
- using ip::tcp;
- struct CHelloWorld_Service{
-      CHelloWorld_Service(io_service &iosev)
-          :m_iosev(iosev),m_acceptor(iosev, tcp::endpoint(tcp::v4(), 1000))
-      {
-      }
- void start()
-      {
- // 开始等待连接（非阻塞）
-          boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
- // 触发的事件只有error_code参数，所以用boost::bind把socket绑定进去
-          m_acceptor.async_accept(*psocket,
-              boost::bind(&CHelloWorld_Service::accept_handler,this, psocket, _1)
-              );
-      }
- // 有客户端连接时accept_handler触发
- void accept_handler(boost::shared_ptr<tcp::socket> psocket, error_code ec)
-      {
- if(ec) return;
- // 继续等待连接
-          start();
- // 显示远程IP
-          std::cout << psocket->remote_endpoint().address() << std::endl;
- // 发送信息(非阻塞)
-          boost::shared_ptr<std::string> pstr(new std::string("hello async world!"));
-          psocket->async_write_some(buffer(*pstr),
-              boost::bind(&CHelloWorld_Service::write_handler, this, pstr, _1, _2)
-              );
-      }
- // 异步写操作完成后write_handler触发
- void write_handler(boost::shared_ptr<std::string> pstr,
-          error_code ec, size_t bytes_transferred)
-      {
- if(ec)
-              std::cout<< "发送失败!" << std::endl;
- else
-              std::cout<< *pstr << " 已发送" << std::endl;
-      }
- private:
-      io_service &m_iosev;
-      ip::tcp::acceptor m_acceptor;
- };
- int main(int argc, char* argv[])
- {
-      io_service iosev;
-      CHelloWorld_Service sev(iosev);
- // 开始等待连接
-      sev.start();
-      iosev.run();
- return 0;
- }



在这个例子中，首先调用sev.start()开 始接受客户端连接。由于async_accept调 用后立即返回，start()方 法 也就马上完成了。sev.start()在 瞬间返回后iosev.run()开 始执行，iosev.run()方法是一个循环，负责分发异步回调事件，只 有所有异步操作全部完成才会返回。

这里有个问题，就是要保证start()方法中m_acceptor.async_accept操 作所用的tcp::socket对 象 在整个异步操作期间保持有效(不 然系统底层异步操作了一半突然发现tcp::socket没了，不是拿人家开涮嘛-_-!!!)，而且客户端连接进来后这个tcp::socket对象还 有用呢。这里的解决办法是使用一个带计数的智能指针boost::shared_ptr<tcp:: socket>，并把这个指针作为参数绑定到回调函数上。

一旦有客户连接，我们在start()里给的回调函数accept_handler就会被 调用，首先调用start()继续异步等待其 它客户端的连接，然后使用绑定进来的tcp::socket对象与当前客户端通信。

发送数据也使用了异步方式(async_write_some)， 同样要保证在整个异步发送期间缓冲区的有效性，所以也用boost::bind绑定了boost::shared_ptr<std:: string>。

对于客户端也一样，在connect和read_some方法前加一个async_前缀，然后加入回调即可，大家自己练习写一写。









