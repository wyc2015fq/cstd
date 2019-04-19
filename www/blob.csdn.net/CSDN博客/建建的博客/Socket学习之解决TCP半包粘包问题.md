# Socket学习之解决TCP半包粘包问题 - 建建的博客 - CSDN博客
2017年02月06日 08:58:25[纪建](https://me.csdn.net/u013898698)阅读数：4055
### 问题产生
TCP是一个数据流协议，所以TCP发送的数据包大小是不可控制的，这时候就会出现粘包和半包的现象，下面这张图是我从网上找的，描述很形象
![](https://img-blog.csdn.net/20141215205044468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
1. 情况1，Data1和Data2都分开发送到了Server端，没有产生粘包和拆包的情况。
2. 情况2，Data1和Data2数据粘在了一起，打成了一个大的包发送到Server端，这个情况就是粘包。
3. 情况3，Data2被分离成Data2_1和Data2_2，并且Data2_1在Data1之前到达了服务端，这种情况就产生了拆包。
由于网络的复杂性，可能数据会被分离成N多个复杂的拆包/粘包的情况，所以在做TCP服务器的时候就需要首先解决拆包/粘包的问题。
### TCP粘包和拆包产生的原因
1. 应用程序写入数据的字节大小大于套接字发送缓冲区的大小
2. 进行MSS大小的TCP分段。MSS是最大报文段长度的缩写。MSS是TCP报文段中的数据字段的最大长度。数据字段加上TCP首部才等于整个的TCP报文段。所以MSS并不是TCP报文段的最大长度，而是：MSS=TCP报文段长度-TCP首部长度
3. 以太网的payload大于MTU进行IP分片。MTU指：一种通信协议的某一层上面所能通过的最[大数据](http://lib.csdn.net/base/hadoop)包大小。如果IP层有一个数据包要传，而且数据的长度比链路层的MTU大，那么IP层就会进行分片，把数据包分成托干片，让每一片都不超过MTU。注意，IP分片可以发生在原始发送端主机上，也可以发生在中间路由器上。
### TCP粘包和拆包的解决策略
1. 消息定长。例如100字节。
2. 在包尾部增加回车或者空格符等特殊字符进行分割，典型的如FTP协议
3. 将消息分为消息头和消息尾。
   我使用方法3解决数据包粘包和半包的情况
  数据包分为
  包头+包体
  包头
/// 网络数据包包头  
struct NetPacketHeader  
{  
unsigned short      wDataSize;  ///< 数据包大小，包含封包头和封包数据大小  
unsigned short      wOpcode;    ///< 操作码  
};
包体
struct NetPacket  
{  
NetPacketHeader     Header;                         ///< 包头  
unsigned char       Data[NET_PACKET_DATA_SIZE];     ///< 数据  
};    
/// 网络操作码  
enum eNetOpcode  
{  
NET_TEST1           = 1,  
};  
/// 测试1的网络数据包定义  
struct NetPacket_Test1  
{  
int     nIndex;  
char name[20];  
char sex[20];  
int age;  
char    arrMessage[512];  
};  
封包方法
bool TCPServer::SendData( unsigned short nOpcode, const char* pDataBuffer, const unsigned int& nDataSize )  
{  
NetPacketHeader* pHead = (NetPacketHeader*) m_cbSendBuf;  
pHead->wOpcode = nOpcode;//操作码  
// 数据封包  
if ( (nDataSize > 0) && (pDataBuffer != 0) )  
{  
CopyMemory(pHead+1, pDataBuffer, nDataSize);   
}  
// 发送消息  
const unsigned short nSendSize = nDataSize + sizeof(NetPacketHeader);//包的大小事发送数据的大小加上包头大小  
pHead->wDataSize = nSendSize;//包大小  
int ret = ::send(mAcceptSocket, m_cbSendBuf, nSendSize, 0); 
return (ret > 0) ? true : false;  
}  
拆包策略
//TCP会存在有时候发送半包的情况，所以事先要检测接受的数据是否大于包头的长度，如果大于的话就接受并解析包头，包头的大小是固定的
int nRecvSize = ::recv(mServerSocket,  
m_cbRecvBuf+m_nRecvSize,   
sizeof(m_cbRecvBuf)-m_nRecvSize, 0);  
// 保存已经接收数据的大小  
m_nRecvSize += nRecvSize;  
// 接收到的数据够不够一个包头的长度  
while (m_nRecvSize >= sizeof(NetPacketHeader))//已经收到一个完整的包，如果没用收到一个完整的包，此处循环不执行，继续下一轮循环  
// 读取包头  
NetPacketHeader* pHead = (NetPacketHeader*) (m_cbRecvBuf);  
const unsigned short nPacketSize = pHead->wDataSize;  
// 判断是否已接收到足够一个完整包的数据  
if (m_nRecvSize < nPacketSize)  
{  
// 还不够拼凑出一个完整包  
break;  
}  
// 拷贝到数据缓存  
CopyMemory(m_cbDataBuf, m_cbRecvBuf, nPacketSize);  
// 从接收缓存移除  
MoveMemory(m_cbRecvBuf, m_cbRecvBuf+nPacketSize, m_nRecvSize);  
m_nRecvSize -= nPacketSize;  
// 解密数据，以下省略一万字  
// ...  
// 分派数据包，让应用层进行逻辑处理  
pHead = (NetPacketHeader*) (m_cbDataBuf);  
const unsigned short nDataSize = nPacketSize - (unsigned short)sizeof(NetPacketHeader);  
OnNetMessage(pHead->wOpcode, m_cbDataBuf+sizeof(NetPacketHeader), nDataSize);  
以上就是解决简单的解决数据包粘包和半包的方法，结尾应该加上一个结尾符。

