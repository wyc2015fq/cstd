# C#中Socket通信用法实例详解 - fighting！！！ - CSDN博客
2017年11月02日 22:22:10[dujiahei](https://me.csdn.net/dujiahei)阅读数：590
C#中Socket通信用法实例详解_C#教程_脚本之家  http://www.jb51.net/article/72313.htm
这篇文章主要介绍了C#中Socket通信用法,以实例形式较为详细的分析了UDP及TCP两种通信方式的具体实现技巧,具有一定参考借鉴价值,需要的朋友可以参考下.
本文实例讲述了C#中Socket通信用法。分享给大家供大家参考。具体如下：
一、UDP方式：
服务器端代码:
`static`
```
void
```
`Main(``string``[] args)`
`{`
`  ``int``recv;`
`  ``byte`
```
[] data =
```
`new``byte``[1024];`
`  `
```
IPEndPoint ipep =
```
`new``IPEndPoint(IPAddress.Any, 9050);``//定义一网络端点`
`  `
```
Socket newsock =
```
`new``Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);``//定义一个Socket`
`  ``newsock.Bind(ipep);``//Socket与本地的一个终结点相关联`
`  ``Console.WriteLine(``"Waiting for a client.."``);`
`  `
```
IPEndPoint sender =
```
`new``IPEndPoint(IPAddress.Any, 0);``//定义要发送的计算机的地址`
`  ``EndPoint Remote = (EndPoint)(sender);``//`
`  `
```
recv = newsock.ReceiveFrom(data,
```
`ref``Remote);`
```
//接受数据
```
`  ``Console.WriteLine(``"Message received from{0}:"``, Remote.ToString());`
`  ``Console.WriteLine(Encoding.ASCII.GetString(data, 0, recv));`
`  ``string``welcome = ``"Welcome to my test server!"``;`
`  ``data = Encoding.ASCII.GetBytes(welcome);`
`  ``newsock.SendTo(data, data.Length, SocketFlags.None, Remote);`
`  ``while``(``true``)`
`  ``{`
`    `
```
data =
```
`new``byte``[1024];`
`    `
```
recv = newsock.ReceiveFrom(data,
```
`ref``Remote);`
`    ``Console.WriteLine(Encoding.ASCII.GetString(data, 0, recv));`
`    ``newsock.SendTo(data, recv, SocketFlags.None, Remote);`
`  ``}`
`}`
客户端代码:
`void`
```
MainInfo()
```
`{`
`  ``byte`
```
[] data =
```
`new``byte``[1024];``//定义一个数组用来做数据的缓冲区`
`  ``string``input, stringData;`
`  `
```
IPEndPoint ipep =
```
`new``IPEndPoint(IPAddress.Parse(``"192.168.1.21"``), 9050);`
`  `
```
Socket server =
```
`new``Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);`
`  ``string``welcome = ``"Hello,are you there?"``;`
`  ``data = Encoding.ASCII.GetBytes(welcome);`
`  ``server.SendTo(data, data.Length, SocketFlags.None, ipep);``//将数据发送到指定的终结点`
`  `
```
IPEndPoint sender =
```
`new``IPEndPoint(IPAddress.Any, 0);`
`  ``EndPoint Remote = (EndPoint)sender;`
`  `
```
data =
```
`new``byte``[1024];`
`  ``int``recv = server.ReceiveFrom(data, ``ref``Remote);``//接受来自服务器的数据`
`  ``Console.WriteLine(``"Message received from{0}:"``, Remote.ToString());`
`  ``Console.WriteLine(Encoding.ASCII.GetString(data, 0, recv));`
`  ``while``(``true``)``//读取数据`
`  ``{`
`    ``input = richTextBox1.Text;``//从键盘读取数据`
`    ``if``(input == ``"text"``)``//结束标记`
`    ``{`
`      ``break``;`
`    ``}`
`    ``server.SendTo(Encoding.ASCII.GetBytes(input), Remote);``//将数据发送到指定的终结点Remote`
`    `
```
data =
```
`new``byte``[1024];`
`    `
```
recv = server.ReceiveFrom(data,
```
`ref``Remote);``//从Remote接受数据`
`    ``stringData = Encoding.ASCII.GetString(data, 0, recv);`
`    ``Console.WriteLine(stringData);`
`  ``}`
`  ``Console.WriteLine(``"Stopping client"``);`
`  ``server.Close();`
`}`
二、TCP方式:
服务器端代码:
```
Socket serverSocket =
```
`null``;`
```
Thread clientThread =
```
`null``;`
```
Socket clientSocket =
```
`null``;`
`Thread thread = ``null``;`
`IPAddress ips = ``null``;`
```
PEndPoint ipep =
```
`null``;`
`void`
```
ServerStart()
```
`{`
`  ``ips = Dns.GetHostAddresses(Dns.GetHostName())[0];`
`  `
```
//创建IPEndPoint实例
```
`  `
```
ipep =
```
`new``IPEndPoint(ips, 9050);`
`  `
```
//创建一个套接字
```
`  `
```
serverSocket =
```
`new``Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);`
`  `
```
serverSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress,
```
`true``);`
`  `
```
//将所创建的套接字与IPEndPoint绑定
```
`  ``serverSocket.Bind(ipep);`
`  `
```
//设置套接字为收听模式
```
`  ``serverSocket.Listen(20);`
`  ``while``(listenalive)`
`  ``{`
`    ``try`
`    ``{`
`      `
```
//在套接字上接收接入的连接
```
`      ``clientSocket = serverSocket.Accept();`
`      `
```
clientThread =
```
`new``Thread(``new``ParameterizedThreadStart(ReceiveData));`
`      ``clientThread.Start(clientSocket);`
`    ``}`
`    ``catch``(Exception ex)`
`    ``{`
`      ``WriteErrorLog(ex.Message);`
`      ``serverSocket.Close();`
`      `
```
serverSocket =
```
`null``;`
`    ``}`
`  ``}`
`}`
`static``void``ReceiveData(``object``obj)`
`{`
`  ``bool``keepalive = ``true``;`
`  `
```
Socket s = obj
```
`as``Socket;`
`  `
```
Byte[] buffer =
```
`new``Byte[1024];`
`  `
```
//根据收听到的客户端套接字向客户端发送信息
```
`  ``IPEndPoint clientep = (IPEndPoint)s.RemoteEndPoint;`
`  ``Console.WriteLine(``"客户端ip:"``+ clientep.Address + ``" 端口:"``+ clientep.Port);`
`  ``string``welcome = ``"连接服务器成功"``;`
`  ``buffer = System.Text.Encoding.Unicode.GetBytes(welcome);`
`  ``//向客户端发送“连接服务器成功”消息`
`  ``s.Send(buffer, buffer.Length, SocketFlags.None);`
`  `
```
buffer =
```
`new``Byte[1024];`
`  ``int``bufLen = 0;`
`  ``string``content = ``string``.Empty;`
`  ``while``(``true``)`
`  ``{`
`    `
```
//在套接字上接收客户端发送的信息
```
`    ``bufLen = 0;`
`    ``try`
`    ``{`
`      ``bufLen = s.Receive(buffer);`
`      ``if``(bufLen == 0)`
`      ``{`
`        ``break``;`
`      ``}`
`      ``content += System.Text.Encoding.Unicode.GetString(buffer, 0, bufLen);`
`    ``}`
`    ``catch``(Exception ex)`
`    ``{`
`      ``break``; ;`
`    ``}`
`  ``}`
`  ``Send(s, content);`
`  `
```
s =
```
`null``;`
`  `
```
buffer =
```
`null``;`
`  `
```
clientep =
```
`null``;`
`  ``Thread.CurrentThread.Abort();`
`}`
客户端代码:
`void`
```
Send(
```
`string`
```
content)
```
`{`
`  ``byte`
```
[] data =
```
`new``byte``[1024];`
`  `
```
newclient =
```
`new``System.Net.Sockets.Socket(System.Net.Sockets.AddressFamily.InterNetwork, System.Net.Sockets.SocketType.Stream, System.Net.Sockets.ProtocolType.Tcp);`
`  `
```
ie =
```
`new``System.Net.IPEndPoint(System.Net.IPAddress.Parse(ipadd), port);``//服务器的IP和端口`
`  ``try`
`  ``{`
`    ``//因为客户端只是用来向特定的服务器发送信息，所以不需要绑定本机的IP和端口。不需要监听。`
`    ``newclient.Connect(ie);`
`  ``}`
`  ``catch``(System.Net.Sockets.SocketException e)`
`  ``{`
`    ``Console.WriteLine(e.ToString());`
`    ``return``;`
`  ``}`
`  ``int``recv = newclient.Receive(data);`
`  ``//连接服务器成功`
`  ``string``stringdata = System.Text.Encoding.Unicode.GetString(data, 0, recv);`
`  ``if``(stringdata == ``"连接服务器成功"``)`
`  ``{`
`    ``newclient.Send(System.Text.Encoding.Unicode.GetBytes(content));`
`    ``newclient.Shutdown(System.Net.Sockets.SocketShutdown.Send);`
`    `
```
data =
```
`new``byte``[1024];`
`    ``recv = newclient.Receive(data);`
`    ``string`
```
result = System.Text.Encoding.Unicode.GetString(data, 0, recv);
```
`    ``newclient.Shutdown(System.Net.Sockets.SocketShutdown.Receive);`
`    ``newclient.Close();`
`    ``MessageBox.Show(result);`
`  ``}`
`  ``else`
`  ``{`
`    ``MessageBox.Show(``"连接服务器失败"`
```
,
```
`"友情提示"``);`
`  ``}`
