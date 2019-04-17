# boost的asio接收单路大数据量udp包的方法 - DoubleLi - 博客园






开发windows客户端接收RTP视频流，当h264视频达到1080P 60fps的时候，按包来调用recvfrom的函数压力比较大，存在丢包的问题，windows的*完成端口*的性能效果当然可以解决这个问题，而boost的asio在windows上是基于完成端口来开发的，所以采用boost的asio和环形缓冲区的方法，可以解决接收单路大数据量udp包中丢包的问题。

    需要引入的头文件为：





**[cpp]**[view plain](http://blog.csdn.net/dong_beijing/article/details/62884324#)[copy](http://blog.csdn.net/dong_beijing/article/details/62884324#)



- #include "CircledBuffer.h"  
- #include <iostream>  
- #include <boost/asio.hpp>  
- #include <boost/bind.hpp>  


其中CircledBuffer.h是自定义的缓冲区的类，之后会有介绍，boost的两个文件是asio必需的两个文件。



    需要定义的全局变量为：





**[cpp]**[view plain](http://blog.csdn.net/dong_beijing/article/details/62884324#)[copy](http://blog.csdn.net/dong_beijing/article/details/62884324#)



- using boost::asio::ip::udp;  
- boost::asio::io_service service;  
- boost::asio::ip::udp::socket sock(service);  
- boost::asio::ip::udp::endpoint sender_ep;  
- CircledBuffer readBuffer;  
- PacketBuffer* packet;  


其中io_service是用来标示启动的，后面会调用run。sock和endpoint类似于描述符和sockaddr_in的关系。CircledBuffer和PacketBuffer*，是自定义缓冲区。





主函数为：





**[cpp]**[view plain](http://blog.csdn.net/dong_beijing/article/details/62884324#)[copy](http://blog.csdn.net/dong_beijing/article/details/62884324#)



- int main(int argc, char* argv[]) {  
-     boost::asio::ip::udp::endpoint ep( boost::asio::ip::address::from_string("192.168.1.206"),  
-         9002);  
-     sock.open(ep.protocol());  
-     sock.set_option(boost::asio::ip::udp::socket::reuse_address(true));  
-     boost::asio::socket_base::receive_buffer_size recv_option(8*65534);  
-     sock.set_option(recv_option);  
-     sock.bind(ep);  
-     packet = readBuffer.GetLast();  
-     sock.async_receive_from(boost::asio::buffer(packet->data, packet->bufferSize), sender_ep, &on_read);  
-     service.run();  
- }  



初始化ep和sock，其中udp接收的数量比较大的话，需要设定receive_buffer_size，然后bind，设置接受buffer为packet。



介绍一下async_receive_from函数，它有三个参数，分别为接收的buffer，远端的ep，注意与本端的ep不同，远端的ep不用初始化设置，再就是buffer收满后的回调函数。



回调函数的内容是：





**[cpp]**[view plain](http://blog.csdn.net/dong_beijing/article/details/62884324#)[copy](http://blog.csdn.net/dong_beijing/article/details/62884324#)



- void on_read(const boost::system::error_code & err, std::size_t  
-              read_bytes) {  
-         std::cout << "read: " << read_bytes << std::endl;  
-         readBuffer.MoveNext();  
-         packet= readBuffer.GetLast();  
-         sock.async_receive_from(boost::asio::buffer(packet->data, packet->bufferSize), sender_ep, &on_read);  
- }  



与main函数的接收部分一致，这里用了不断的自身回调，来实现while recvfrom的功能。



补充说一句，用申请好的CircledBuffer，便于后期的多线程或者异步strand的处理，而不阻塞接收。

缓冲区类的代码：

头文件：





**[cpp]**[view plain](http://blog.csdn.net/dong_beijing/article/details/62884324#)[copy](http://blog.csdn.net/dong_beijing/article/details/62884324#)



- #ifndef CIRCLED_BUFFER_H  
- #define CIRCLED_BUFFER_H  
- 
- #include <memory.h>  
- #include <boost/atomic.hpp>  
- #define CIRCLED_BUFFER_SIZE 300  
- #define BUFFER_SIZE 2000  
- 
- struct PacketBuffer  
- {  
-     PacketBuffer(){bufferSize=BUFFER_SIZE;dataSize=0;}   
-     unsigned int bufferSize;  
-     unsigned int dataSize;  
- char data[BUFFER_SIZE];  
- 
-     PacketBuffer& operator=(PacketBuffer& other)  
-     {  
-         memcpy(data,other.data,other.dataSize);  
-         dataSize = other.dataSize;  
-         bufferSize = other.bufferSize;  
- return *this;  
-     }  
- };  
- 
- class CircledBuffer  
- {  
- public:  
-     CircledBuffer(unsigned int bufSize=CIRCLED_BUFFER_SIZE);  
- public:  
-     ~CircledBuffer(void);  
-     PacketBuffer* GetAt(unsigned int idx){return &packets[idx];}  
-     PacketBuffer* GetLast()  
-     {         
- return GetAt(writeIndex.load(boost::memory_order_consume));  
-     };  
- void MoveNext()  
-     {  
-         unsigned int idx = writeIndex.load(boost::memory_order_relaxed);  
-         writeIndex.store((idx+1)%bufferSize,boost::memory_order_release);  
-     };  
-     unsigned int GetLastIndex(){return writeIndex.load(boost::memory_order_consume);};  
-     unsigned int GetSize(){return bufferSize;};  
- protected:  
-     boost::atomic<unsigned int> writeIndex;  
-     unsigned int bufferSize;  
-     PacketBuffer* packets;  
- };  
- #endif  



缓冲区类的构造函数与析构函数







**[cpp]**[view plain](http://blog.csdn.net/dong_beijing/article/details/62884324#)[copy](http://blog.csdn.net/dong_beijing/article/details/62884324#)



- #include "CircledBuffer.h"  
- 
- CircledBuffer::CircledBuffer(unsigned int bufSize)  
- :bufferSize(bufSize),  
- writeIndex(0)  
- {  
-     packets = new PacketBuffer[bufSize];  
- }  
- 
- CircledBuffer::~CircledBuffer(void)  
- {  
- delete []packets;  
- }  



[源代码下载链接](http://download.csdn.net/detail/dong_beijing/9784310)











