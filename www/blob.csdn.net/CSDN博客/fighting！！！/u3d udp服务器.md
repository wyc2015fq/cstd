# u3d udp服务器 - fighting！！！ - CSDN博客
2017年08月15日 09:18:30[dujiahei](https://me.csdn.net/dujiahei)阅读数：152标签：[unity																[udp](https://so.csdn.net/so/search/s.do?q=udp&t=blog)](https://so.csdn.net/so/search/s.do?q=unity&t=blog)
个人分类：[unity学习](https://blog.csdn.net/dujiahei/article/category/6975775)
u3d udp服务器 - c_dragon - 博客园 http://www.cnblogs.com/dragon2012/p/4244066.html
`using``UnityEngine;`
`using``System.Collections;`
`using``System.Net;`
`using``System.Net.Sockets;`
`using``System.Text;`
`using``System;`
`using``System.Threading;`
`public``class`
```
udpServer
 :MonoBehaviour
```
`{`
`    ``private`
```
Socket
 newsock;
```
`//定义一个socket变量`
`    `
```
IPEndPoint
 ip;
```
`//定义一个IP地址和端口号`
`    ``int``recv;``//定义一个接受值的变量`
`    ``byte`
```
[]
 data =
```
`new``byte``[1024];``//定义一个二进制的数组用来获取客户端发过来的数据包`
`    ``string``mydata;`
`    ``void``Start()`
`    ``{`
`        ``//得到本机IP，设置TCP端口号        `
`        `
```
ip
 =
```
`new`
```
IPEndPoint(IPAddress.Any,
 12345);
```
`//设置自身的IP和端口号，在这里IPAddress.Any是自动获取本机IP`
`        `
```
newsock
 =
```
`new`
```
Socket(AddressFamily.InterNetwork,
 SocketType.Dgram, ProtocolType.Udp);
```
`//实例化socket对象设置寻址方案为internetwork（IP版本的4存放）,设置Soket的类型，为Dgram（支持数据报形式的数据），设置协议的类型，为UDP`
`        ``//绑定网络地址`
`        ``newsock.Bind(ip);``//绑定IP`
`        ``Debug.Log(`
```
"This
 is a Server,host name is "
```
```
+
 Dns.GetHostName());
```
`//输出本地的名字`
`        ``Debug.Log(`
```
"Waiting
 for a client"
```
`);`
`        ``//BeginReceives();`
`        `
```
Thread
 test =
```
`new``Thread(BeginListening);``//定义一个子线程`
`        ``test.Start();``//子线程开始   `
`    ``}`
`    ``void``BeginListening()`
`    ``{`
`        `
```
IPEndPoint
 sender =
```
`new`
```
IPEndPoint(IPAddress.Any,
 0);
```
`//实例化一个网络端点，设置为IPAddress.Any为自动获取跟我通讯的IP，0代表所有的地址都可以`
`        `
```
EndPoint
 Remote = (EndPoint)(sender);
```
`//实例化一个地址结束点来标识网络路径`
`      `
```
// 
 Debug.Log(Encoding.ASCII.GetString(data, 0, recv));//输出二进制转换为string类型用来测试
```
`        ``while``(``true``)`
`        ``{`
`            `
```
data
 =
```
`new``byte``[1024];``//实例化data`
`            `
```
recv
 = newsock.ReceiveFrom(data,
```
`ref``Remote);``//将数据包接收到的数据放入缓存点，并存储终节点`
`            `
```
//Debug.Log(Encoding.ASCII.GetString(data,
 0, recv));
```
`            `
```
mydata
 = Encoding.ASCII.GetString(data, 0, recv);
```
`            ``Debug.Log(mydata);`
`            `
```
//
 newsock.SendTo(Encoding.ASCII.GetBytes(mydata),mydata.Length,SocketFlags.None,Remote);
```
`        ``}`
`    ``}`
`    ``string`
```
input
 =
```
`""``;`
`    ``void``SendMessage(``string``message)`
`    ``{`
`        ``byte`
```
[]
 data =
```
`new``byte``[1024];`
`        ``Debug.Log(`
```
"This
 is a client,host name is"
```
```
+
 Dns.GetHostName());
```
`        `
```
//IPEndPoint
 sender = new IPEndPoint(IPAddress.Any, 0);//实例化一个网络端点，设置为IPAddress.Any为自动获取跟我通讯的IP，0代表所有的地址都可以
```
`        `
```
EndPoint
 Remote = (EndPoint)(ip);
```
`//实例化一个地址结束点来标识网络路径`
`        `
```
Socket
 server =
```
`new`
```
Socket(AddressFamily.InterNetwork,
 SocketType.Dgram, ProtocolType.Udp);
```
`        `
```
//string
 welcome = "你好";
```
`        `
```
//data
 = Encoding.ASCII.GetBytes(welcome);
```
`        `
```
ip
 =
```
`new``IPEndPoint(IPAddress.Parse(``"192.168.1.120"`
```
),
 12346);
```
`       `
```
//
 server.SendTo(data, data.Length, SocketFlags.None, ip);
```
`       `
```
//
 data = new byte[1024];
```
`        `
```
server.SendTo(Encoding.ASCII.GetBytes(message),
 ip);
```
`       `
```
//
 data = new byte[1024];
```
`        ``Debug.Log(`
```
"Stopping
 Client."
```
`);`
`        ``server.Close();`
`    ``}`
`    ``void``OnGUI()`
`    ``{`
`        `
```
//input
 = GUILayout.TextField(input);
```
`        `
```
input
 = GUI.TextField(
```
`new`
```
Rect(100,
 50, 60, 30), input);
```
`        ``if``(GUI.Button(``new`
```
Rect(100,
 90, 60, 30),
```
`"Send"``))`
`        ``{`
`            ``SendMessage(input);`
`        ``}`
`    ``}`
`    ``void``OnApplicationQuit()`
`    ``{`
`        ``newsock.Close();`
`    ``}`
`}`

```
`using``System;`
`using``UnityEngine;`
`using``System.Collections.Generic;`
`using``System.Linq;`
`using``System.Net;`
`using``System.Net.Sockets;`
`using``System.Text;`
`using``System.Threading;`
`public``struct``mystructs`
`{`
`    ``public``int``Age; ``//获取结构里的`
`    ``public``string``name;`
`    ``public``float``Height;`
`    ``public``mystructs(``int``myage, ``string``myname, ``float``myheight)`
`    ``{`
`        `
```
Age
 = myage;
```
`        `
```
name
 = myname;
```
`        `
```
Height
 = myheight;
```
`    ``}`
`}`
`public``class`
```
udpMystruct
 : MonoBehaviour
```
`{`
`    ``int``Age;`
`    ``string``Name;``//获取`
`    ``float``Height;`
`    ``private`
```
Socket
 newsock;
```
`//定义一个socket变量`
`    `
```
IPEndPoint
 ip;
```
`//定义一个IP地址和端口号`
`    ``int``recv;``//定义一个接受值的变量`
`    ``byte`
```
[]
 data =
```
`new``byte``[1024];``//定义一个二进制的数组用来获取客户端发过来的数据包`
`    ``byte`
```
[]
 bytedate =
```
`new``byte``[1024];`
`    ``string``mydata;`
`    ``void``Start()`
`    ``{`
`        `
```
mystructs
 my;
```
```
//将结构初始化 
 付给另一个
```
`        `
```
Age
 = my.Age = 17;
```
`        `
```
Name
 = my.name =
```
```
" 
 haha  "
```
`;`
`        `
```
Height
 = my.Height = 180.50f;
```
`        `
```
Debug.Log(Age
 + Name + Height);
```
`        ``byte`
```
[]
 agebyte =
```
`new``byte``[1024];   ``//定义二进制数组`
`        ``byte`
```
[]
 namebyte =
```
`new``byte``[1024];`
`        ``byte`
```
[]
 heightbyte =
```
`new``byte``[1024];`
`        `
```
agebyte
 = Encoding.ASCII.GetBytes(Age.ToString());
```
`//转换成二进制的`
`        `
```
namebyte
 = Encoding.ASCII.GetBytes(Name);
```
`        `
```
heightbyte
 = Encoding.ASCII.GetBytes(Height.ToString());
```
`        `
```
bytedate
 =
```
`new``byte`
```
[agebyte.Length
 + namebyte.Length + heightbyte.Length];
```
`//byte`
`        `
```
agebyte.CopyTo(bytedate,
 0);
```
```
//把值
 拷贝 给bytedate  索引
```
`        `
```
namebyte.CopyTo(bytedate,
 agebyte.Length);
```
`//agebyte索引开始`
`        `
```
heightbyte.CopyTo(bytedate,
 namebyte.Length);
```
`        `
```
//string
 sss = Encoding.ASCII.GetString(data);
```
`        `
```
// 
 Debug.Log(sss);
```
`        ``//得到本机IP，设置TCP端口号        `
`        `
```
ip
 =
```
`new``IPEndPoint(IPAddress.Parse(``"192.168.1.120"`
```
),
 12346);
```
`//设置自身的IP和端口号，在这里IPAddress.Any是自动获取本机IP`
`        `
```
newsock
 =
```
`new`
```
Socket(AddressFamily.InterNetwork,
 SocketType.Dgram, ProtocolType.Udp);
```
`//实例化socket对象设置寻址方案为internetwork（IP版本的4存放）,设置Soket的类型，为Dgram（支持数据报形式的数据），设置协议的类型，为UDP`
`        ``//绑定网络地址`
`        ``newsock.Bind(ip);``//绑定IP`
`        ``Debug.Log(`
```
"This
 is a Server,host name is  "
```
```
+
 Dns.GetHostName());
```
`//输出本地的名字`
`        ``Debug.Log(`
```
"Waiting
 for a client"
```
`);`
`        `
```
Thread
 test =
```
`new``Thread(BeginListening);``//定义一个子线程`
`        ``test.Start();``//子线程开始   `
`    ``}`
`    ``void``BeginListening()`
`    ``{`
`        `
```
//     
 IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);//实例化一个网络端点，设置为IPAddress.Any为自动获取跟我通讯的IP，0代表所有的地址都可以
```
`        `
```
EndPoint
 Remote = (EndPoint)(ip);
```
`//实例化一个地址结束点来标识网络路径`
`        `
```
Debug.Log(Encoding.ASCII.GetString(data,
 0, recv));
```
`//输出二进制转换为string类型用来测试`
`        ``while``(``true``)`
`        ``{`
`            `
```
data
 =
```
`new``byte``[1024];``//实例化data`
`            `
```
recv
 = newsock.ReceiveFrom(data,
```
`ref``Remote);``//将数据包接收到的数据放入缓存点，并存储终节点`
`            `
```
//Debug.Log(Encoding.ASCII.GetString(data,
 0, recv));
```
`            `
```
mydata
 = Encoding.ASCII.GetString(data, 0, recv);
```
`            ``Debug.Log(mydata);`
`            `
```
//
 newsock.SendTo(Encoding.ASCII.GetBytes(mydata),mydata.Length,SocketFlags.None,Remote);
```
`        ``}`
`    ``}`
`    ``string`
```
input
 =
```
`""``;`
`    ``void``SendMessage(``string``message)`
`    ``{`
`        ``byte`
```
[]
 data =
```
`new``byte``[1024];`
`        ``byte`
```
[]
 messbyte =
```
`new``byte``[1024];`
`        ``Debug.Log(`
```
"This
 is a client,host name is  "
```
```
+
 Dns.GetHostName());
```
`        `
```
ip
 =
```
`new``IPEndPoint(IPAddress.Parse(``"192.168.1.120"`
```
),
 12345);
```
`        `
```
Socket
 server =
```
`new`
```
Socket(AddressFamily.InterNetwork,
 SocketType.Dgram, ProtocolType.Udp);
```
`        `
```
EndPoint
 ep = (EndPoint)ip;
```
`        `
```
//string
 welcome = "你好";
```
`        `
```
//data
 = Encoding.ASCII.GetBytes(welcome);
```
`        `
```
server.SendTo(data,
 data.Length, SocketFlags.None, ep);
```
`        `
```
messbyte
 = Encoding.ASCII.GetBytes(message);
```
`        `
```
data
 =
```
`new``byte`
```
[bytedate.Length
 + messbyte.Length];
```
`        `
```
bytedate.CopyTo(data,
 0);
```
`        `
```
messbyte.CopyTo(data,
 bytedate.Length);
```
`        `
```
server.SendTo(data,
 ep);
```
`        `
```
data
 =
```
`new``byte``[1024];`
`        ``Debug.Log(`
```
"Stopping
 Client."
```
`);`
`        ``server.Close();`
`    ``}`
`    ``void``OnGUI()`
`    ``{`
`        `
```
input
 = GUILayout.TextField(input);
```
`        ``if``(GUILayout.Button(``"发送"``))`
`        ``{`
`            ``SendMessage(input);`
`        ``}`
`    ``}`
`    ``void``OnApplicationQuit()`
`    ``{`
`        ``newsock.Close();`
`    ``}`
```
