# QUIC学习总结

2018年11月08日 17:22:45 [SADHELLO](https://me.csdn.net/m0_37587447) 阅读数：814



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/m0_37587447/article/details/83864116

**Quic项目介绍**
对于Quic的基本下载和启动使用参考 <https://www.jianshu.com/p/f17b4a015e27> 。
Quic详细介绍 <https://max.book118.com/html/2017/0202/87965079.shtm>
Quic官方文档 <https://github.com/google/proto-quic>
代码下载地址 <https://github.com/google/proto-quic>

**Quic代码介绍**
quic基本实现代码位于 proto-quic-master\src\net\quic\core
quic实现的C/S端代码位于proto-quic-master\src\net\tools\quic
quic阻塞算法控制代码位于 proto-quic-master\src\net\quic\core\congestion_control

文件介绍：

1. quic_connection类文件

   主要编写QuicConnection类，该类是quic服务端和客户端的处理框架，它提供SendStreamData方法用来发送流数据，被QuicSession调用。 它使用 QuicPacketGenerator 来创建 Quic帧。 而QuicPacketGenerator会被QuicConnection的OnSerializedPacket 方法调用。 最后帧则会被QuicPacketWriter写入连接中。

2. quic_session类文件
   主要编写QuicSession类，QuicSession类是一个基础类，当一个具体会话类被创建时将被继承。 它主要将传入的数据发送到正确的Quic流。 拥有QuicConnection类，用于在线上发送数据。 因此它代表一个quic连接，由多个流组成，并抽象出真实的网络连接。 quic流会被WritevData 方法用来发送数据。 反过来QuicConnection类会调用QuicConnectionVisitorInterface 方法来通知会话新的数据包和对连接的更改。

3. quic_stream类文件
   主要编写QuicStream类，它定义了quic流类需要满足的接口。 它还实现了流的基本逻辑，如流控制，帧排序，处理流连接重置或关闭和缓冲数据写入，用户根据stream类来进行数据的抛出和写入。stream流有QuicDataStream类，其实现了传输SPDY请求的quic流。 它要求在会话管理的专用报头流中发送不同的报头。 报头通过OnStreamHeaders, OnStreamHeadersPriority 和 OnStreamHeadersComplete发送给它。 初始化时会阻塞QuicStreamSequencer 直到所有报头被接收。QuicStreamSequencer类用来缓冲帧数据直到它们可以传递到下一层为止。 其中包括检查重复帧，排序帧，以便数据有序，并检查错误情况。QuicHeadersStream类用来处理SPDY报头

4. quic_packet类文件
   主要编写QuicPacket类，如QuicPacketCreator类是QuicConnection 用来创建和发送包的类。 它使用 QuicPacketCreator来构建帧和包。 当一个包被创建完毕，它将调用OnSerializedPacket 给它的调用者。QuicReceivedPacket类用来处理接收包。QuicPacketWriter类接口定义了通过QuicConnection发送数据包的方法。
   它还定义了一些方法来判断套接字是否被阻塞。QuicPacketGenerator::DelegateInterface类定义了当新数据包可用时 QuicPacketGenerator调用的接口。 它通过QuicConnection实现。

5. quic_framer类文件
   主要编写QuicFramer类,用来解析和构建QUIC包。 通过 ProcessPacket方法来接收数据，并调用QuicFrameVisitorInterface 接口来通知QuicConnection 接收到新包。QuicFrameVisitorInterface类定义了QuicFrame处理新QUIC数据包的方法，它通过QuicConnection实现。
   跟据以下几种类型来判断为什么类型的包，来进行相应的处理

   ```
   enum QuicFrameType {
     PADDING_FRAME = 0,
     RST_STREAM_FRAME = 1,
     CONNECTION_CLOSE_FRAME = 2,
     GOAWAY_FRAME = 3,
     WINDOW_UPDATE_FRAME = 4,
     BLOCKED_FRAME = 5,
     STOP_WAITING_FRAME = 6,
     PING_FRAME = 7,
     STREAM_FRAME,
     ACK_FRAME,
     MTU_DISCOVERY_FRAME,
     NUM_FRAME_TYPES
   };
   PADDING_FRAME：为填充字节帧，接收到这个包时会将包剩余部分填充字节。
   RST_STREAM_FRAME：当由流的创建者发送时，表示创建者希望关闭流，当由接收者发送时，表示发生错误或者不想接收流，因此流应该被关闭。
   CONNECTION_CLOSE_FRAME：连接关闭。
   GOAWAY_FRAME：表示流应该被停止使用，因为之后将会被关闭，在使用的流将被继续处理，但是发送者不会在接收流。
   WINDOW_UPDATE_FRAME：用于通知对端流量控制端口接收窗口大小的更新。
   BLOCKED_FRAME：表示已经准备好发送数据且有数据要发送，但是被阻塞了。
   STOP_WAITING_FRAME：通知对端不需要等待包号小于特定值的包。
   PING_FRAME：用来验证对端是否保持活跃，且连接是否正常。
   STREAM_FRAME：用于发送数据。
   ACK_FRAME：通知对端哪些包被接收到了。
   123456789101112131415161718192021222324
   ```

**Quic服务端客户端发送数据流程解析**
根据quic C/S端代码，其服务端和客户端代码的main（）文件为server_bin.cc和client_bin.cc等文件。

client.main（） 函数基本实现步骤：
创建 QuicClient 类client，调用client.Initialize() 进行初始化。

```
Initialize()实现： 
{ 
定义流窗口大小 
定义session窗口大小 
设置epoll_server超时时间 
调用CreateUDPSocket()创建UDP套接字 
注册epoll时间回调函数 
} 
12345678
```

之后调用client.Connect()进行对话的连接

```
Connect()实现 
{ 
写数据类PacketWrite类创建 
创建session类 
初始化session，InitializeSession() 
WaitForEvent 
}
1234567
```

调用 client.CreateClientStream()与session中创建一个stream类流，用于发送数据。
调用stream->WriteStringPiece() 来进行数据的发送。
调用client.WaitForEvents() 等待事件。
调用stream->CloseConnection(net::QUIC_NO_ERROR);来关闭连接。
调用client.Disconnect()关闭client。

服务端与client端基本类似。

**发送和接收：**
发送：
最外层的发送数据接口为调用stream流的WriteOrBufferData(body, fin, nullptr)方法其中body是要发的数据，fin是标识是否是改流的最后一个数据。之后会在流中进行相应的判断和处理，如流上是否有足够的空间来发送这个数据，发送窗口大小是否合适，是否阻塞等。如果判断可以进行发送之后便会调用session类的方法WritevData（）。
在session类会调用connection类的SendStreamData方法发送数据，并根据实际发送的数据更新相应stream流的数据消费的数值。
在connection类会调用PacketGenerator类的ConsumeData方法来发送数据。其中会根据包来进行ack的绑定。之后就是在底层进行一系列处理，因为理解有限，不能详细介绍。
之后会返回connection类，根据消息队列情况调用WritePacket（）进行socket上包的写入，该方法实现于PacketWriter类。

接收：
当Server端创建好之后循环调用StartReading()，进行接收包，根据synchronous_read_count_ 来判断是否是CHLO包。

```
void QuicSimpleServer::StartReading() {
  if (synchronous_read_count_ == 0) {
    // Only process buffered packets once per message loop.
    dispatcher_->ProcessBufferedChlos(kNumSessionsToCreatePerSocketEvent);
  }
...
  int result = socket_->RecvFrom(
      read_buffer_.get(), read_buffer_->size(), &client_address_,
      base::Bind(&QuicSimpleServer::OnReadComplete, base::Unretained(this)));
...
OnReadComplete(result);
}
123456789101112
```

OnReadComplete(）中会调用dispatcher的处理包方法

```
void QuicSimpleServer::OnReadComplete(int result) {
...
  dispatcher_->ProcessPacket(
      QuicSocketAddress(QuicSocketAddressImpl(server_address_)),
      QuicSocketAddress(QuicSocketAddressImpl(client_address_)), packet);

  StartReading();
}
void QuicDispatcher::ProcessPacket(const QuicSocketAddress& server_address,
                                   const QuicSocketAddress& client_address,
                                   const QuicReceivedPacket& packet) {
...
  framer_.ProcessPacket(packet);
...
}
123456789101112131415
```

跳转到Framer类的处理方法

```
bool QuicFramer::ProcessPacket(const QuicEncryptedPacket& packet) {
...
  if (!visitor_->OnUnauthenticatedPublicHeader(public_header)) {
    // The visitor suppresses further processing of the packet.
    return true;
  }
...
}
12345678
```

visitor_指向dispatch类，跳转到

```
QuicDispatcher::OnUnauthenticatedPublicHeader（）{
...
  QuicConnectionId connection_id = header.connection_id;
  SessionMap::iterator it = session_map_.find(connection_id);
  if (it != session_map_.end()) {
    DCHECK(!buffered_packets_.HasBufferedPackets(connection_id));
    it->second->ProcessUdpPacket(current_server_address_,
                                 current_client_address_, *current_packet_);
    return false;
  }
...
}
123456789101112
```

当包头的connection_id 能在session_map里找到时，直接调用connection的ProcessUdpPacket处理，server端的session_map维护在dispatch类里，创建session类都会记录下来。

之后经过处理跳转到Framer类的ProcessFrameData（）方法里，其中对stream Framer 和ACK Framer 分别进行了处理
如果是stream包，则对其进行解析后会调用OnStreamFrame()抛到上层

```
if (!ProcessStreamFrame(reader, frame_type, &frame)) {
          return RaiseError(QUIC_INVALID_STREAM_DATA);
        }
        if (!visitor_->OnStreamFrame(frame)) {
          QUIC_DVLOG(1) << ENDPOINT
                        << "Visitor asked to stop further processing.";
          // Returning true since there was no parsing error.
          return true;
        }
}
12345678910
```

visitor_在Framer类里，由创建connection类时初始化，指向connection类，在到connection类里调用visitor_->OnStreamFrame(），visitor_指向session类，在由session类抛到stream类的OnDataAvailable()将数据进行处理，注意基础stream类里没有实现OnDataAvailable()的方法，需要编写，下面是官方tools文件里的处理。

```
OnDataAvailable() {
  while (HasBytesToRead()) {
    struct iovec iov;
    if (GetReadableRegions(&iov, 1) == 0) {
      // No more data to read.
      break;
    }
    QUIC_DVLOG(1) << "Stream " << id() << " processed " << iov.iov_len
                  << " bytes.";
    body_.append(static_cast<char*>(iov.iov_base), iov.iov_len);

    if (content_length_ >= 0 &&
        body_.size() > static_cast<uint64_t>(content_length_)) {
      QUIC_DVLOG(1) << "Body size (" << body_.size() << ") > content length ("
                    << content_length_ << ").";
      SendErrorResponse();
      return;
    }
    MarkConsumed(iov.iov_len);
  }
  if (!sequencer()->IsClosed()) {
    sequencer()->SetUnblocked();
    return;
  }

  // If the sequencer is closed, then all the body, including the fin, has been
  // consumed.
  OnFinRead();

  if (write_side_closed() || fin_buffered()) {
    return;
  }
}
123456789101112131415161718192021222324252627282930313233
```

如果是ACK包，则会对ack进行处理，并进行拥塞算法的运算。

```
    if (!ProcessAckFrame(reader, frame_type, &frame)) {
      return RaiseError(QUIC_INVALID_ACK_DATA);
    }
    if (!visitor_->OnAckFrame(frame)) {
      QUIC_DVLOG(1) << ENDPOINT
                    << "Visitor asked to stop further processing.";
      // Returning true since there was no parsing error.
      return true;
    }
123456789
```

OnAckFrame（）跳转到connection类的OnAckFrame，其中调用QuicSentPacketManager类OnIncomingAck（）方法，其中进行了rtt和带宽的更新，并对丢包进行判断。

之后根据理解对代码进行了简化，去掉了客户端和服务端的spdy和http验证，只进行数据的收发，具体代码和使用方法：
<https://github.com/IFrestart/Proto-quic>