# boot asio 非阻塞同步编程---非阻塞的accept和receive。 - DoubleLi - 博客园






boot asio 非阻塞同步编程---非阻塞的accept和receive。

客户端编程：





**[cpp]**[view plain](http://blog.csdn.net/seulww/article/details/23450513#)[copy](http://blog.csdn.net/seulww/article/details/23450513#)



- #include<boost/timer.hpp>   
- #include <iostream>  
- #include <boost/asio.hpp>  
- #include <stdlib.h>  
- 
- using namespace boost::asio;  
- using namespace std;  
- 
- #define RECEIVE_BUF_SIZE 100  
- 
- #define RECEIVE_BYTE_NUM 30  
- 
- int readMaxBytesInTime(ip::tcp::socket & socket,char * strBuf,int nMaxBytes,int nMilSec)  
- {  
-     boost::timer t;  
- int nTotalRec = 0;  
- int nLeftBytes = nMaxBytes - nTotalRec;  
- while(1)  
-     {  
-         boost::system::error_code ec;  
- char buf[RECEIVE_BUF_SIZE];  
- 
- int nWantBytes = 0;  
- if(nLeftBytes < RECEIVE_BUF_SIZE)  
-         {  
-             nWantBytes = nLeftBytes;  
-         }  
- else  
-         {  
-             nWantBytes = RECEIVE_BUF_SIZE;  
-         }  
- 
- size_t len=socket.read_some(buffer(buf,nWantBytes), ec);  
- if(len>0)  
-         {  
-             memcpy(strBuf + nTotalRec,buf,len);  
-             nTotalRec += len;  
-             nLeftBytes -= len;  
- 
- if(nLeftBytes <= 0)  
- break;  
- else  
- continue;  
-         }  
- else  
-         {  
- if(t.elapsed()*1000 < nMilSec)  
-             {  
-                 Sleep(0);  
- continue;  
-             }  
- else  
- break;  
-         }  
-     }  
- return nTotalRec;  
- }  
- 
- int main(int argc, char* argv[])  
- {  
- 
- // 所有asio类都需要io_service对象  
-     io_service iosev;  
- // socket对象  
-     ip::tcp::socket socket(iosev);  
-     socket.open(boost::asio::ip::tcp::v4());  
-     socket.io_control(boost::asio::ip::tcp::socket::non_blocking_io(true));  
- // 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接  
-     ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"), 1000);  
- // 连接服务器  
-     boost::system::error_code ec;  
-     boost::timer t;  
-     socket.connect(ep,ec);  
-     cout<< t.elapsed()<<"s"<<endl;   
-     system("PAUSE");  
- 
- // 如果出错，打印出错信息  
- if(ec)  
-     {  
-         std::cout << boost::system::system_error(ec).what() << std::endl;  
- return -1;  
-     }  
- // 接收数据  
- char buf[RECEIVE_BYTE_NUM];  
- int len = readMaxBytesInTime(socket,buf,RECEIVE_BYTE_NUM,1000);  
- 
-     std::cout<<"接收字节数:"<<len<<std::endl;  
-     std::cout.write(buf, len);  
- 
-     system("PAUSE");  
- 
- return 0;  
- }  


服务器端编程：





**[cpp]**[view plain](http://blog.csdn.net/seulww/article/details/23450513#)[copy](http://blog.csdn.net/seulww/article/details/23450513#)



- #include <iostream>  
- #include <boost/asio.hpp>  
- 
- #include <stdlib.h>  
- 
- int main(int argc, char* argv[])  
- {  
- using namespace boost::asio;  
- // 所有asio类都需要io_service对象  
-     io_service iosev;  
-     ip::tcp::acceptor acceptor(iosev);  
-     acceptor.open(boost::asio::ip::tcp::v4());  
-     acceptor.io_control(boost::asio::ip::tcp::socket::non_blocking_io(true));  
- // 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接  
-     ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"), 1000);  
-     acceptor.bind(ep);  
-     acceptor.listen();  
- for(;;)  
-     {  
-         boost::system::error_code ec;  
- // socket对象  
-         ip::tcp::socket socket(iosev);  
- 
- // 等待直到客户端连接进来  
- while (1)  
-         {  
-             acceptor.accept(socket,ec);  
- if(ec)  
-             {  
-                 std::cout <<  
-                     boost::system::system_error(ec).what() << std::endl;  
-                 Sleep(10);  
-             }  
- else  
- break;  
-         }  
- 
-         system("PAUSE");  
- 
- // 显示连接进来的客户端  
-         std::cout << socket.remote_endpoint().address() << std::endl;  
- 
- // 向客户端发送hello world!  
- char * str = "hello world!hello world!";  
-         socket.write_some(buffer(str,20), ec);  
- 
- // 如果出错，打印出错信息  
- if(ec)  
-         {  
-             std::cout <<  
-                 boost::system::system_error(ec).what() << std::endl;  
- break;  
-         }  
- // 与当前客户交互完成后循环继续等待下一客户连接  
-     }  
- return 0;  
- }  










