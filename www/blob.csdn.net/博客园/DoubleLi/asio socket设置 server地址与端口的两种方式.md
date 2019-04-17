# asio socket设置 server地址与端口的两种方式 - DoubleLi - 博客园






 1. 用解释器的方法, 常用来解析域名, 如

// query("www.163.com","80"), 也可以 query("www.163.com","telnet")

// echo          7/tcp
// ftp           21/tcp                 # File Transfer Protocol (Control)
// telnet        23/tcp                 # Virtual Terminal Protocol
// smtp          25/tcp                 # Simple Mail Transfer Protocol
// time          37/tcp  timeserver     # Time



  TCP::resolver resolver([iOS](http://lib.csdn.net/base/ios));


  TCP::resolver::query query("127.0.0.1", "10000");
  TCP::resolver::iterator endpoint_iterator = resolver.resolve(query);
  TCP::resolver::iterator end;
  TCP::socket socket(ios);
  socket.connect(*endpoint_iterator, error);



  2. 用简便的方法


TCP::socket socket(ios);
TCP::endpoint endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1"), 10000);
socket.connect(endpoint, error);













