# 记一次C++多线程编程遇到的坑 - alw2009的博客 - CSDN博客





2019年01月29日 22:49:32[traveler_zero](https://me.csdn.net/alw2009)阅读数：21








下面一段很简单的TCP sever程序， 第二次连接连进来的时候core了，你能看出原因吗？

```cpp
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <memory>
#include <list>

namespace asio = boost::asio;
namespace ip = asio::ip;
using tcp = ip::tcp;

void handle_sock(std::unique_ptr<tcp::socket>&& sock) {
    std::cout << sock.get() << " handle socket.\n";
    sock->shutdown(tcp::socket::shutdown_both);
}

int main(int argc, char *argv[])
{
    constexpr unsigned short port_num = 3333;

    try {
        tcp::endpoint endpoint(ip::address_v4::any(), port_num);

        asio::io_context ioc;
        tcp::acceptor acceptor(ioc, endpoint.protocol());

        acceptor.bind(endpoint);
        acceptor.listen();

        std::list<std::thread> threads;

        for (int i = 0; i < 5; ++i) {
            auto psock = std::make_unique<tcp::socket>(ioc);

            acceptor.accept(*psock);

            threads.push_back(std::thread{[&psock]{
                  handle_sock(std::move(psock));
               });
        }

        for(auto& thread : threads) {
            thread.join();
        }

    } catch (boost::system::system_error& error) {
        std::cerr << error.what() << std::endl;
        return error.code().value();
    }

    return 0;
}
```

错误就在起线程的方式有问题，这样的写法非常危险， 实际上改成下面这样就可以了。

```cpp
threads.push_back(std::thread{handle_sock, std::move(psock)});
```





