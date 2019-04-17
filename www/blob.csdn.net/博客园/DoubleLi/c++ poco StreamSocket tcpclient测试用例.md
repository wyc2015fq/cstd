# c++ poco StreamSocket tcpclient测试用例 - DoubleLi - 博客园






#### 1.代码

```cpp
#include <iostream>
#include "Poco/Net/Socket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"

using Poco::Net::Socket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Net::NetException;
using Poco::Net::ConnectionRefusedException;
using Poco::Net::InvalidSocketException;
using Poco::Timespan;
using Poco::TimeoutException;
using Poco::IOException;

const int RECV_BUF_SIZE = 64*1024; 
const int SEND_BUF_SIZE = 64*1024;

int main(int argc,char * argv[])
{
    int n=0;
    char buffer[1024]={"\0"};
    SocketAddress sa("127.0.0.1",5000);
    StreamSocket ss;

    Timespan timeout(2000000);
    try
    {
        ss.connect(sa,timeout);
    }
    catch (ConnectionRefusedException&)
    {
        std::cout<<"connect refuse"<<std::endl;
    }
    catch (NetException&)
    {   
        std::cout<<"net exception"<<std::endl;
    }
    catch (TimeoutException&)
    {
        std::cout<<"connect time out"<<std::endl;
    }

    //setopt timeout
    Timespan timeout3(5000000);
    ss.setReceiveTimeout(timeout3); //retn void
    Timespan timeout4(5000000);
    ss.setSendTimeout(timeout4); //retn void
    Timespan timeout0 = ss.getReceiveTimeout();
    Timespan timeout1 = ss.getSendTimeout();
    std::cout<<"Recv Timeout : "<<timeout0.totalMicroseconds()<<std::endl;  
    std::cout<<"Send Timeout : "<<timeout1.totalMicroseconds()<<std::endl;  

    //setopt bufsize
    ss.setReceiveBufferSize(RECV_BUF_SIZE); //retn void 
    ss.setSendBufferSize(SEND_BUF_SIZE); //retn void 
    int recv_len=ss.getReceiveBufferSize();
    int send_len=ss.getSendBufferSize();
    std::cout<<"recv buf size : "<<recv_len<<std::endl;
    std::cout<<"send buf size : "<<send_len<<std::endl;

    //setopt nodelay
    ss.setNoDelay(true); //retn void 

    try
    {
        n = ss.sendBytes("hello", 5); //block
        std::cout<<"write length : "<<n<<std::endl;
    }
    catch (TimeoutException&)
    {
        std::cout<<"send time out"<<std::endl;
    }
    catch (InvalidSocketException&)
    {
        std::cout<<"invalid socket exception"<<std::endl;
    }
    catch (IOException&)
    {
        std::cout<<"write io exception"<<std::endl;
    }

    while(1)
    {
        try
        {   
            if(ss.available())
            {   
                n=0;
                memset(buffer,0,sizeof(buffer));
                n = ss.receiveBytes(buffer,sizeof(buffer)); //block
                std::cout<<"recv length : "<<n<<","<<"value : "<<buffer<<std::endl;
            }
        }
        catch (TimeoutException&)
        {
            std::cout<<"recv time out"<<std::endl;
        }
        catch (InvalidSocketException&)
        {
            std::cout<<"invalid socket exception"<<std::endl;
        }
    }

    //Socket::poll有select poll epoll 三种模式，编译Poco库时确定。
    /*
    Timespan timer(2000000);
    Socket::SocketList readList;
    Socket::SocketList writeList;
    Socket::SocketList exceptList;
    readList.push_back(ss);
    while(1)
    {
        if(ss.poll(timer, Socket::SELECT_READ))
        {
            std::cout<<"he number of bytes available that can be read : "<<ss.available()<<std::endl;
            memset(buffer,'\0',sizeof(buffer));
            n = ss.receiveBytes(buffer,ss.available());
            std::cout<<"recv length : "<<n<<","<<"value : "<<buffer<<std::endl;
        }
    }   
    */

    ss.close();

    return 0;
}
```



#### 2.编译指令
`g++ myStreamSocket.cpp -o mysocket -lPocoNet -lPocoFoundation`- 1

#### 3.运行截图

![这里写图片描述](http://img.blog.csdn.net/20170430215117472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHpfb2Jq/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)









