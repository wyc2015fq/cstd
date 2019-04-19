# C#高性能 TCP 服务的多种实现方式 - 文章 - 伯乐在线
原文出处： [Dennis Gao](http://www.cnblogs.com/gaochundong/p/csharp_tcp_service_models.html)
本篇文章的主旨是使用 .NET/C# 实现 TCP 高性能服务的不同方式，包括但不限于如下内容：
- [APM 方式，即 Asynchronous Programming Model](#apm)
- [TAP 方式，即 Task-based Asynchronous Pattern](#tap)
- [SAEA 方式，即 SocketAsyncEventArgs](#saea)
- [RIO 方式，即 Registered I/O](#rio)
在 .NET/C# 中对于 Socket 的支持均是基于 [Windows I/O Completion Ports](https://msdn.microsoft.com/en-us/library/windows/desktop/aa365198(v=vs.85).aspx) 完成端口技术的封装，通过不同的 Non-Blocking 封装结构来满足不同的编程需求。以上方式均已在 [Cowboy.Sockets](https://github.com/gaochundong/Cowboy) 中有完整实现，并且 APM 和 TAP 方式已经在实际项目中应用。[Cowboy.Sockets](https://github.com/gaochundong/Cowboy) 还在不断的进化和完善中，如有任何问题请及时指正。
虽然有这么多种实现方式，但抽象的看，它们是一样一样的，用两个 Loop 即可描述：**Accept Loop** 和 **Read Loop**，如下图所示。（这里提及的 “**Loop**” 指的是一种**循环方式**，而非特指 while/for 等关键字。）
![](http://jbcdn2.b0.upaiyun.com/2016/02/13ea968e48d75698f3f7c2e023cc8cae.png)
- 在任何 TCP Server 的实现中，一定存在一个 Accept Socket Loop，用于接收 Client 端的 Connect 请求以建立 TCP Connection。
- 在任何 TCP Server 的实现中，一定存在一个 Read Socket Loop，用于接收 Client 端 Write 过来的数据。
如果 Accept 循环阻塞，则会导致无法快速的建立连接，服务端 Pending Backlog 满，进而导致 Client 端收到 Connect Timeout 的异常。如果 Read 循环阻塞，则显然会导致无法及时收到 Client 端发过来的数据，进而导致 Client 端 Send Buffer 满，无法再发送数据。
从实现细节的角度看，能够导致服务阻塞的位置可能在：
- Accept 到新的 Socket，构建新的 Connection 需要分配各种资源，分配资源慢；
- Accept 到新的 Socket，没有及时触发下一次 Accept；
- Read 到新的 Buffer，判定 Payload 消息长度，判定过程长；
- Read 到新的 Buffer，发现 Payload 还没有收全，继续 Read，则可能会导致一次 Buffer Copy；
- Payload 接收完毕，进行 Serialization 转成可识别的 Protocol Message，序列化慢；
- 由 Business Module 来处理相应的 Protocol Message，处理过程慢；
1-2 涉及到 Accept 过程和 Connection 的建立过程，3-4 涉及到 ReceiveBuffer 的处理过程，5-6 涉及到应用逻辑侧的实现。
Java 中著名的 Netty 网络库从 4.0 版本开始对于 Buffer 部分做了全新的尝试，采用了名叫 [ByteBuf](https://github.com/netty/netty/wiki/New-and-noteworthy-in-4.0) 的设计，实现 Buffer Zero Copy 以减少高并发条件下 Buffer 拷贝带来的性能损失和 GC 压力。[DotNetty](https://github.com/Azure/DotNetty)，[Orleans](https://github.com/dotnet/orleans) ，[Helios](https://github.com/helios-io/helios) 等项目正在尝试在 C# 中进行类似的 ByteBuf 的实现。
## APM 方式：TcpSocketServer
[TcpSocketServer](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/APM/TcpSocketServer.cs) 的实现是基于 .NET Framework 自带的 [TcpListener](https://msdn.microsoft.com/en-us/library/system.net.sockets.tcplistener(v=vs.110).aspx) 和 [TcpClient](https://msdn.microsoft.com/en-us/library/system.net.sockets.tcpclient(v=vs.110).aspx) 的更进一步的封装，采用基于 APM 的 BeginXXX 和 EndXXX 接口实现。
TcpSocketServer 中的 Accept Loop 指的就是，
- BeginAccept -> EndAccept-> BeginAccept -> EndAccept -> BeginAccept -> …
每一个建立成功的 Connection 由 [TcpSocketSession](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/APM/TcpSocketSession.cs) 来处理，所以 TcpSocketSession 中会包含 Read Loop，
- BeginRead -> EndRead -> BeginRead -> EndRead -> BeginRead -> …
TcpSocketServer 通过暴露 Event 来实现 Connection 的建立与断开和数据接收的通知。

C#
```
event EventHandler ClientConnected;
  event EventHandler ClientDisconnected;
  event EventHandler ClientDataReceived;
```
使用也是简单直接，直接订阅事件通知。

C#
```
private static void StartServer()
  {
      _server = new TcpSocketServer(22222);
      _server.ClientConnected += server_ClientConnected;
      _server.ClientDisconnected += server_ClientDisconnected;
      _server.ClientDataReceived += server_ClientDataReceived;
      _server.Listen();
  }
  static void server_ClientConnected(object sender, TcpClientConnectedEventArgs e)
  {
      Console.WriteLine(string.Format("TCP client {0} has connected {1}.", e.Session.RemoteEndPoint, e.Session));
  }
  static void server_ClientDisconnected(object sender, TcpClientDisconnectedEventArgs e)
  {
      Console.WriteLine(string.Format("TCP client {0} has disconnected.", e.Session));
  }
  static void server_ClientDataReceived(object sender, TcpClientDataReceivedEventArgs e)
  {
      var text = Encoding.UTF8.GetString(e.Data, e.DataOffset, e.DataLength);
      Console.Write(string.Format("Client : {0} {1} --> ", e.Session.RemoteEndPoint, e.Session));
      Console.WriteLine(string.Format("{0}", text));
      _server.Broadcast(Encoding.UTF8.GetBytes(text));
  }
```
## TAP 方式：AsyncTcpSocketServer
[AsyncTcpSocketServer](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/TAP/AsyncTcpSocketServer.cs) 的实现是基于 .NET Framework 自带的 [TcpListener](https://msdn.microsoft.com/en-us/library/system.net.sockets.tcplistener(v=vs.110).aspx) 和 [TcpClient](https://msdn.microsoft.com/en-us/library/system.net.sockets.tcpclient(v=vs.110).aspx) 的更进一步的封装，采用基于 TAP 的 async/await 的 XXXAsync 接口实现。
然而，实际上 [XXXAsync](http://referencesource.microsoft.com/#System/net/System/Net/Sockets/TCPListener.cs) 并没有创建什么神奇的效果，其内部实现只是将 APM 的方法转换成了 TAP 的调用方式。

C#
```
//************* Task-based async public methods *************************
  [HostProtection(ExternalThreading = true)]
  public Task AcceptSocketAsync()
  {
      return Task.Factory.FromAsync(BeginAcceptSocket, EndAcceptSocket, null);
  }
  [HostProtection(ExternalThreading = true)]
  public Task AcceptTcpClientAsync()
  {
      return Task.Factory.FromAsync(BeginAcceptTcpClient, EndAcceptTcpClient, null);
  }
```
[AsyncTcpSocketServer](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/TAP/AsyncTcpSocketServer.cs) 中的 Accept Loop 指的就是，

C#
```
while (IsListening)
  {
      var tcpClient = await _listener.AcceptTcpClientAsync();
  }
```
每一个建立成功的 Connection 由 [Async](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/TAP/AsyncTcpSocketSession.cs)[TcpSocketSession](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/TAP/AsyncTcpSocketSession.cs) 来处理，所以 AsyncTcpSocketSession 中会包含 Read Loop，

C#
```
while (State == TcpSocketConnectionState.Connected)
  {
      int receiveCount = await _stream.ReadAsync(_receiveBuffer, 0, _receiveBuffer.Length);
  }
```
为了将 async/await 异步到底，[AsyncTcpSocketServer](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/TAP/AsyncTcpSocketServer.cs) 所暴露的接口也同样是 Awaitable 的。

C#
```
public interface IAsyncTcpSocketServerMessageDispatcher
  {
      Task OnSessionStarted(AsyncTcpSocketSession session);
      Task OnSessionDataReceived(AsyncTcpSocketSession session, byte[] data, int offset, int count);
      Task OnSessionClosed(AsyncTcpSocketSession session);
  }
```
使用时仅需将一个实现了该接口的对象注入到 AsyncTcpSocketServer 的构造函数中即可。

C#
```
public class SimpleMessageDispatcher : IAsyncTcpSocketServerMessageDispatcher
  {
      public async Task OnSessionStarted(AsyncTcpSocketSession session)
      {
          Console.WriteLine(string.Format("TCP session {0} has connected {1}.", session.RemoteEndPoint, session));
          await Task.CompletedTask;
      }
      public async Task OnSessionDataReceived(AsyncTcpSocketSession session, byte[] data, int offset, int count)
      {
          var text = Encoding.UTF8.GetString(data, offset, count);
          Console.Write(string.Format("Client : {0} --> ", session.RemoteEndPoint));
          Console.WriteLine(string.Format("{0}", text));
          await session.SendAsync(Encoding.UTF8.GetBytes(text));
      }
      public async Task OnSessionClosed(AsyncTcpSocketSession session)
      {
          Console.WriteLine(string.Format("TCP session {0} has disconnected.", session));
          await Task.CompletedTask;
      }
  }
```
当然，对于接口的实现也不是强制了，也可以在构造函数中直接注入方法的实现。

C#
```
public AsyncTcpSocketServer(
      IPEndPoint listenedEndPoint,
      Funcbyte[], int, int, Task> onSessionDataReceived = null,
      Func onSessionStarted = null,
      Func onSessionClosed = null,
      AsyncTcpSocketServerConfiguration configuration = null)
  {}
```
## SAEA 方式：TcpSocketSaeaServer
SAEA 是 [SocketAsyncEventArgs](https://msdn.microsoft.com/en-us/library/system.net.sockets.socketasynceventargs(v=vs.110).aspx) 的简写。SocketAsyncEventArgs 是 [.NET Framework 3.5](https://msdn.microsoft.com/en-us/library/bb968780(v=vs.110).aspx) 开始支持的一种支持高性能 Socket 通信的实现。SocketAsyncEventArgs 相比于 APM 方式的主要优点可以描述如下：
> 
The main feature of these enhancements is the **avoidance of the repeated allocation and synchronization of objects** during high-volume asynchronous socket I/O. The Begin/End design pattern currently implemented by the Socket class for asynchronous socket I/O requires a System.IAsyncResult object be allocated for each asynchronous socket operation.
也就是说，优点就是无需为每次调用都生成 IAsyncResult 等对象，向原生 Socket 更靠近一些。
使用 SocketAsyncEventArgs 的[推荐步骤](https://msdn.microsoft.com/en-us/library/bb968780(v=vs.110).aspx)如下：
- Allocate a new SocketAsyncEventArgs context object, or get a free one from an application pool.
- Set properties on the context object to the operation about to be performed (the callback delegate method and data buffer, for example).
- Call the appropriate socket method (xxxAsync) to initiate the asynchronous operation.
- If the asynchronous socket method (xxxAsync) returns true in the callback, query the context properties for completion status.
- If the asynchronous socket method (xxxAsync) returns false in the callback, the operation completed synchronously. The context properties may be queried for the operation result.
- Reuse the context for another operation, put it back in the pool, or discard it.
重点在于池化（Pooling），池化的目的就是为了重用和减少运行时分配和垃圾回收的压力。
[TcpSocketSaeaServer](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/SAEA/TcpSocketSaeaServer.cs) 即是对 SocketAsyncEventArgs 的应用和封装，并实现了 Pooling 技术。[TcpSocketSaeaServer](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/SAEA/TcpSocketSaeaServer.cs) 中的重点是 SaeaAwaitable 类，SaeaAwaitable 中内置了一个 SocketAsyncEventArgs，并通过 GetAwaiter 返回 SaeaAwaiter 来支持 async/await 操作。同时，通过 SaeaExtensions 扩展方法对来扩展 SocketAsyncEventArgs 的 Awaitable 实现。

C#
```
public static SaeaAwaitable AcceptAsync(this Socket socket, SaeaAwaitable awaitable)
  public static SaeaAwaitable ConnectAsync(this Socket socket, SaeaAwaitable awaitable)
  public static SaeaAwaitable DisonnectAsync(this Socket socket, SaeaAwaitable awaitable)
  public static SaeaAwaitable ReceiveAsync(this Socket socket, SaeaAwaitable awaitable)
  public static SaeaAwaitable SendAsync(this Socket socket, SaeaAwaitable awaitable)
```
SaeaPool 则是一个 QueuedObjectPool 的衍生实现，用于池化 SaeaAwaitable 实例。同时，为了减少 TcpSocketSaeaSession 的构建过程，也实现了 SessionPool 即 QueuedObjectPool。
[TcpSocketSaeaServer](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/SAEA/TcpSocketSaeaServer.cs) 中的 Accept Loop 指的就是，

C#
```
while (IsListening)
  {
      var saea = _acceptSaeaPool.Take();
      var socketError = await _listener.AcceptAsync(saea);
      if (socketError == SocketError.Success)
      {
          var acceptedSocket = saea.Saea.AcceptSocket;
      }
      _acceptSaeaPool.Return(saea);
  }
```
每一个建立成功的 Connection 由 [TcpSocketSaeaSession](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/SAEA/TcpSocketSaeaSession.cs) 来处理，所以 TcpSocketSaeaSession 中会包含 Read Loop，

C#
```
var saea = _saeaPool.Take();
  saea.Saea.SetBuffer(_receiveBuffer, 0, _receiveBuffer.Length);
  while (State == TcpSocketConnectionState.Connected)
  {
      saea.Saea.SetBuffer(0, _receiveBuffer.Length);
      var socketError = await _socket.ReceiveAsync(saea);
      if (socketError != SocketError.Success)
          break;
      var receiveCount = saea.Saea.BytesTransferred;
      if (receiveCount == 0)
          break;
  }
```
同样，[TcpSocketSaeaServer](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/SAEA/TcpSocketSaeaServer.cs) 对外所暴露的接口也同样是 Awaitable 的。

C#
```
public interface ITcpSocketSaeaServerMessageDispatcher
  {
      Task OnSessionStarted(TcpSocketSaeaSession session);
      Task OnSessionDataReceived(TcpSocketSaeaSession session, byte[] data, int offset, int count);
      Task OnSessionClosed(TcpSocketSaeaSession session);
  }
```
使用起来也是简单直接：

C#
```
public class SimpleMessageDispatcher : ITcpSocketSaeaServerMessageDispatcher
  {
      public async Task OnSessionStarted(TcpSocketSaeaSession session)
      {
          Console.WriteLine(string.Format("TCP session {0} has connected {1}.", session.RemoteEndPoint, session));
          await Task.CompletedTask;
      }
      public async Task OnSessionDataReceived(TcpSocketSaeaSession session, byte[] data, int offset, int count)
      {
          var text = Encoding.UTF8.GetString(data, offset, count);
          Console.Write(string.Format("Client : {0} --> ", session.RemoteEndPoint));
          Console.WriteLine(string.Format("{0}", text));
          await session.SendAsync(Encoding.UTF8.GetBytes(text));
      }
      public async Task OnSessionClosed(TcpSocketSaeaSession session)
      {
          Console.WriteLine(string.Format("TCP session {0} has disconnected.", session));
          await Task.CompletedTask;
      }
  }
```
## RIO 方式：TcpSocketRioServer
从 Windows 8.1 / Windows Server 2012 R2 开始，微软推出了 [Registered I/O Networking Extensions](https://msdn.microsoft.com/library/windows/desktop/ms740642(v=vs.85).aspx) 来支持高性能 Socket 服务的实现，简称 RIO。
*The following functions are supported for Windows Store apps on Windows 8.1, Windows Server 2012 R2, and later. Microsoft Visual Studio 2013 Update 3 or later is required for Windows Store apps.*
- *RIOCloseCompletionQueue*
- *RIOCreateCompletionQueue*
- *RIOCreateRequestQueue*
- *RIODequeueCompletion*
- *RIODeregisterBuffer*
- *RIONotify*
- *RIOReceive*
- *RIOReceiveEx*
- *RIORegisterBuffer*
- *RIOResizeCompletionQueue*
- *RIOResizeRequestQueue*
- *RIOSend*
- *RIOSendEx*
到目前为止，.NET Framework 还没有推出对 RIO 的支持，所以若想在 C# 中实现 RIO 则只能通过 P/Invoke 方式，[RioSharp](https://github.com/aL3891/RioSharp) 是开源项目中的一个比较完整的实现。
[Cowboy.Sockets](https://github.com/gaochundong/Cowboy) 直接引用了 [RioSharp](https://github.com/aL3891/RioSharp) 的源代码，放置在 **Cowboy.Sockets.Experimental** 名空间下，以供实验和测试使用。
同样，通过 [TcpSocketRioServer](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/RIO/TcpSocketRioServer.cs) 来实现 Accept Loop，

C#
```
_listener.OnAccepted = (acceptedSocket) =>
{
    Task.Run(async () =>
    {
        await Process(acceptedSocket);
    })
    .Forget();
};
```
通过 [TcpSocketRioSession](https://github.com/gaochundong/Cowboy/blob/master/Cowboy/Cowboy.Sockets/Tcp/Server/RIO/TcpSocketRioSession.cs) 来处理 Read Loop，

C#
```
while (State == TcpSocketConnectionState.Connected)
  {
      int receiveCount = await _stream.ReadAsync(_receiveBuffer, 0, _receiveBuffer.Length);
      if (receiveCount == 0)
          break;
  }
```
测试代码一如既往的类似：

C#
```
public class SimpleMessageDispatcher : ITcpSocketRioServerMessageDispatcher
  {
      public async Task OnSessionStarted(TcpSocketRioSession session)
      {
          //Console.WriteLine(string.Format("TCP session {0} has connected {1}.", session.RemoteEndPoint, session));
          Console.WriteLine(string.Format("TCP session has connected {0}.", session));
          await Task.CompletedTask;
      }
      public async Task OnSessionDataReceived(TcpSocketRioSession session, byte[] data, int offset, int count)
      {
          var text = Encoding.UTF8.GetString(data, offset, count);
          //Console.Write(string.Format("Client : {0} --> ", session.RemoteEndPoint));
          Console.Write(string.Format("Client : --> "));
          Console.WriteLine(string.Format("{0}", text));
          await session.SendAsync(Encoding.UTF8.GetBytes(text));
      }
      public async Task OnSessionClosed(TcpSocketRioSession session)
      {
          Console.WriteLine(string.Format("TCP session {0} has disconnected.", session));
          await Task.CompletedTask;
      }
  }
```
## 参考资料
- [Asynchronous Programming Model (APM)](https://msdn.microsoft.com/en-us/library/ms228963(v=vs.110).aspx)
- [Task-based Asynchronous Pattern (TAP)](https://msdn.microsoft.com/en-us/library/hh873175(v=vs.110).aspx)
- [Event-based Asynchronous Pattern (EAP)](https://msdn.microsoft.com/en-us/library/ms228969(v=vs.110).aspx)
- [SocketAsyncEventArgs](https://msdn.microsoft.com/en-us/library/system.net.sockets.socketasynceventargs(v=vs.110).aspx)
- [Registered I/O](https://msdn.microsoft.com/library/windows/desktop/hh437226(v=vs.85).aspx)
- [Netty: Reference counted objects](https://github.com/netty/netty/wiki/Reference-counted-objects)
- [Socket Performance Enhancements in Version 3.5](https://msdn.microsoft.com/en-us/library/bb968780(v=vs.110).aspx)
- [What’s New for Windows Sockets for Windows 8.1 and Windows Server 2012 R2](https://msdn.microsoft.com/library/windows/desktop/ms740642(v=vs.85).aspx)
- [RIO_EXTENSION_FUNCTION_TABLE structure](https://msdn.microsoft.com/library/windows/desktop/hh437226(v=vs.85).aspx)
- [Windows 8 Registered I/O Networking Extensions](http://www.serverframework.com/asynchronousevents/2011/10/windows-8-registered-io-networking-extensions.html)
