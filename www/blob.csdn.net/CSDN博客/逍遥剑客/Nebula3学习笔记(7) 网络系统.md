# Nebula3学习笔记(7): 网络系统 - 逍遥剑客 - CSDN博客
2008年07月10日 23:01:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2298
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
  Nebula3的网络子系统提供了基于TCP协议的简单C/S通信模式. 它并没有打算做成大厅,会话管理还有玩家数据同步的面向游戏的高级通信. 这些以后会在更高层的Nebula3子系统中出现. 
**使用****IP****地址**
  一个IpAddress对象通过主机名字或TCP/IP地址加一个端口号定义了一个通信端点. IpAddress对象可以通过多数方式建立: 
 1: // 从 TCP/IP 地址和端口号:
 2: IpAddress ipAddr**(**"192.168.0.2"**,**1234**);**
 3: 
 4: // 从主机名和端口号:
 5: IpAddress ipAddr**(**"www.radonlabs.de"**,**1234**);**
 6: 
 7: // 从本机(127.0.0.1) 和端口号:
 8: IpAddress ipAddr**(**"localhost"**,**1234**);**
 9: 
10: // 从"any" 地址 (0.0.0.0) 和端口号:
11: IpAddress ipAddr**(**"any"**,**1234**);**
12: 
13: // 从广播地址 (255.255.255.255) 和端口号:
14: IpAddress ipAddr**(**"broadcast"**,**1234**);**
15: 
16: // 从主机的第一个合法网络适配器的地址和端口号
17: IpAddress ipAddr**(**"self"**,**1234**);**
18: 
19: // 从主机的第一个连接到互联网的网络适配器的地址和端口号:
20: IpAddress ipAddr**(**"insetself"**,**1234**);**
21: 
22: // 从一个定义了主机名的URI和端口号:
23: IpAddress ipAddr**(**IO::URI**(**"http://www.radonlabs.de:2100"**));**
  一个IpAddress对象可以用于从主机名查找TCP/IP地址: 
1: IpAddress ipAddr**(**"www.radonlabs.de"**,**0**);**
2: String numericalAddr = ipAddr**.**GetHostAddr**();**
**建立一个客户端****/****服务器系统**
  网络子系统用TcpServer和TcpClient类实现了一个易用的基于TCP协议的C/S系统. 一个TcpServer可以为任意数量的TcpClient服务. 
  建立一个服务器可以这么做: 
1: using namespace Net**;**
2: 
3: Ptr<TcpServer> tcpServer = TcpServer::Create**();**
4: tcpServer->SetAddress**(**IpAddress**(**"any"**,**2352**));**
5: **if****(**tcpServer->Open**())**
6: **{**
7: // TcpServer successfully opened
8: **}**
  这样会建立一个在2352端口监听客户端连接请求的服务器. 
  为了跟TcpServer通信, 需要在客户端建立一个TcpClient对象: 
1: using namespace Net**;**
2: 
3: Ptr<TcpClient> tcpClient = TcpClient::Create**();**
4: tcpClient->SetBlocking**(**false**);**
5: tcpClient->SetAddress**(**IpAddress**(**"localhost"**,**2352**));**
6: TcpClient::Result res = tcpClient->Connect**();**
  这里假设服务端和客户端运行在同一台机器上(因为客户端连接到了”localhost”). 
  像上面那样非阻塞的情况, Connect()方法不是返回TcpClient::Success(这意味着连接建立好了)就是TcpClient::Connecting, 如果这样的话, 应用程序需要继续调用Connect()方法. 如果连接错误, 会返回一个TcpClient::Error的返回值. 
  如果是阻塞的, Connect()方法直到连接建立(结果是TcpClient::Success)或发生错误才会返回. 
**  注意**:一个交互式应用程序不应该在网络通信时阻塞, 而应不断地为用户提供反馈. 
  一旦连接建立, 服务端会为每个客户机建立一个TcpClientConnection对象. TcpClientConnection在服务器上表示客户机, 并且负责从客户机收发数据. 
  要进行接收和发送数据的话, 需使用IO::Stream对象. 在通信流上连接IO::StreamReader和IO::StreamWriter对象后, 从流中编码和解码数据是一件非常容易的事情. 
**  注意**:发送数据并不是即时的, 而是在Send()方法被调用之前会一直保存在发送流当中. 
  要客户端给服务器发送一些文本数据话, 只要从发送流获取一个指针, 向其中写入数据后调用Send()方法就可以了: 
 1: using namespace Net**;**
 2: using namespace IO**;**
 3: 
 4: // obtain pointer to client's send stream and attach a TextWriter
 5: **const** Ptr<Stream>& sendStream = tcpClient->GetSendStream**();**
 6: Ptr<TextWriter> textWriter = TextWriter::Create**();**
 7: textWriter->SetStream**(**sendStream**);**
 8: textWriter->Open**())**
 9: textWriter->WriteString**(**"Hello Server"**);**
10: textWriter->Close**();**
11: 
12: // send off the data to the server
13: **if****(****this**->tcpClient->Send**())**
14: **{**
15: // data has been sent
16: **}**
  在服务器端接收客户端数据, 应用程序需要要频繁地(每帧一次)缓存带有客户羰数据的TcpClientConnection. 可能不只一个TcpClientConnection在等待处理, 因此处理循环应该像这样: 
 1: // get array of client connections which received data since the last time
 2: Array<Ptr<TcpClientConnection>> recvConns = tcpServer->Recv**();**
 3: IndexT i**;**
 4: **for****(**i =0**;** i < recvConns**.**Size**();** i++**)**
 5: **{**
 6: // get receive stream from current connection, attach a text reader and read content
 7:      Ptr<TextReader> textReader = TextReader::Create**();**
 8:      textReader->SetStream**(**recvConns**[**i**]**->GetRecvStream**());**
 9:      textReader->Open**();**
10:      String str = textReader->ReadString**();**
11:      textReader->Close**();**
12: 
13: // process received string and send response back to client
14: // create a TextWriter and attach it to the send stream of the client connection
15:      Ptr<TextWriter> textWriter = TextWriter::Create**();**
16:      textWriter->SetStream**(**recvConns**[**i**]**->GetSendStream**());**
17:      textWriter->Open**();**
18:      textWriter->WriteString**(**"Hello Client"**);**
19:      textWriter->Close**();**
20: 
21: // finally send the response back to the client
22:      recvConns**[**i**]**->Send**();**
23: **}**
  在客户端获得服务器的应答, 调用TcpClient::Recv()方法会在数据到达之前一直阻塞(在阻塞模式下), 或者立即返回(在非阻塞模式下), 并在有服务器数据时返回true: 
 1: // check if data is available from the server
 2: **if****(**tcpClient->Recv**())**
 3: **{**
 4: // yep, data is available, get the recv stream and read the data from it
 5: **const** Ptr<Stream>& recvStream = tcpClient->GetRecvStream**();**
 6:      Ptr<TextReader> textReader = TextReader::Create**();**
 7:      textReader->SetStream**(**recvStream**);**
 8:      textReader->Open**();**
 9:      String responseString = textReader->ReadString**();**
10:      n_printf**(**"The server said: %s/n"**,** responseString**.**AsCharPtr**());**
11:      textReader->Close**();**
12: **}**
  客户端也应该通过调用IsConnected()访求检查连接是否有效. 如果因为某些原因使连接断开, 这个方法会返回false. 
**  注意**:
TcpServer和TcpClient并没有为能够跟不相关的客户端和服务器端而实现一个潜在的通信协议(例如, 一个TcpServer可以跟标准的Web浏览器客户端一起工作, 还有一个TcpClient类可以跟一个标准的HTTP服务器通信). 
  现实世界的情况是, 一个应用程序应该实现自己的健壮的通信协议, 它至少会编码负载数据的长度. 如果负载比最大包大小还要大, 数据会以多个包发送并在客户端接收. 客户端应该把数据解码成一个完整的消息, 否则需要等待消息的数据接收完毕. 
**字节次序问题**
  服务器和客户端可能运行在不同字节次序的的CPU上. 如果二进制数据通过网络发送, 数据必需转换成两个客户端都一致的”网络字节顺序”. Nebula3在IO::BinaryReader和IO::BinaryWriter类中提供字节顺序的自动转换. 只需要简单地调用下面的方法在网络通信流上读写就可以了: 
1: binaryReader->SetStreamByteOrder**(**System::ByteOrder::Network**);**
2: binaryWriter->SetStreamByteOrder**(**System::ByteOrder::Network**);**
**Socket****类**
  网络子系统提供了一个把传统socket函数包装成C++接口的Socket类. 一般情况下应用程序不直接使用Socket类, 而是使用更高级的像TcpServer这样的类. 但也不是不可能在有的时候直接使用socket函数比Socket类更方便. 
