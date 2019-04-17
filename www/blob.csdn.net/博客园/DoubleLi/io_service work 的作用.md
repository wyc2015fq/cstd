# io_service work 的作用 - DoubleLi - 博客园






当有任务的时候，run函数会一直阻塞；但当没有任务了，run函数会返回，所有异步操作终止。

    客户端程序中，如果我想连接断开后重连，由于连接断开了，run会返回，当再次重连的时候，由于run返回了，即使连接成功了，也不会调用aysnc_connect绑定的回调函数。

 解决方法：

1，在再次重连的时候，要重新调用run函数，在调用的前一定要调用io_service::reset。以便io_service::run重用。



     boost::asio::io_service io_service_;

   io_service_.reset();

   io_service_.run();



2，用boost::asio::io_service::work。

     boost::asio::io_service io_service_;

     boost::asio::io_service::work work(io_service_); 

     io_service_.run();



  这样即使没有任务，run也不会返回了。









