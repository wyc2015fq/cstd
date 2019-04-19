# 也说Socket - 毛小亮 - 博客园
# [也说Socket](https://www.cnblogs.com/xianerwonder/p/6524628.html)
网上有大量socket相关文章，茫茫多，大多交代不清，最近自我整理了一下socket相关知识，附加了大量**代码注释**，先看效果。
![](https://images2015.cnblogs.com/blog/688944/201703/688944-20170309105441094-930997657.gif)

上代码，**客户端**：
```
1         Socket socket1 = null;//一个全局的socket对象
  2 
  3         private void btnConnect_Click(object sender, EventArgs e)
  4         {
  5             //获得服务器ip和端口号
  6             IPAddress ip = IPAddress.Parse(textBox1.Text);
  7             int port =Convert.ToInt32( textBox2.Text);
  8 
  9             //构造客户端socket，设置为tcp协议模式
 10             socket1 = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
 11 
 12             //连接
 13             socket1.Connect(ip,  port);
 14             if (socket1.Connected)//如果连接成功
 15             {
 16                 textBox3.Text += "连接成功\r\n";
 17                 SendMsg(socket1, "发成功");
 18 
 19                 string recMsg= "";
 20                 //接收
 21                 ReceiveMsg(socket1, ref recMsg);
 22                 textBox3.Text +=recMsg;//文本显示
 23             }
 24             else
 25             {
 26                 textBox3.Text += "连接不成功\r\n";
 27             }
 28         }
 29 
 30         private void btnSendMessage_Click(object sender, EventArgs e)
 31         {
 32             if (socket1.Connected)
 33             {
 34                 string sendMsg = textBox4.Text;
 35 
 36                 //依据长度判断是否发送
 37                 if (sendMsg.Length <= 0)
 38                 {
 39                     return;
 40                 }
 41 
 42                 //拿到发送是否成功的标记
 43                 bool isSendSuccess = SendMsg(socket1, textBox4.Text);
 44 
 45                 string recMsg = "";
 46                 bool isReceiveSuccess = ReceiveMsg(socket1, ref recMsg);
 47                 textBox3.Text += recMsg;
 48 
 49                 if (!(isSendSuccess&&isReceiveSuccess))
 50                 {
 51                     textBox3.Text += "连接已断开，请重连";
 52                 }
 53             }
 54         }
 55 
 56         /// <summary>
 57         /// 发送内容，并显示
 58         /// </summary>
 59         /// <param name="socket">发送内容的socket</param>
 60         /// <param name="msg">输入字符串</param>
 61         /// <returns>发送是否成功</returns>
 62         private bool SendMsg(Socket socket, string msg)
 63         {
 64             try {
 65                 socket.Send(Encoding.UTF8.GetBytes(msg));
 66                 return true;
 67             }
 68             catch (Exception e)
 69             {
 70                 socket.Close();
 71                 return false;
 72             }
 73         }
 74 
 75         /// <summary>
 76         /// 接收内容，并显示
 77         /// </summary>
 78         /// <param name="socket">接受内容的socket</param>
 79         /// <param name="msg">输出字符串</param>
 80         /// <returns>接收是否成功</returns>
 81         private static bool ReceiveMsg(Socket socket ,ref string  msg)
 82         {
 83             try
 84             {
 85                 int recNumber;//接收到的字节数量
 86                 byte[] recBytes = new byte[1024];//缓冲字节数组
 87                 recNumber = socket.Receive(recBytes);//接收行为
 88                 byte[] recBytesForShort = new byte[recNumber];//byteArrayForDestination
 89                 Array.Copy(recBytes, recBytesForShort, recNumber);//拷贝
 90 
 91                 //显示文本构造
 92                 msg += (Encoding.UTF8.GetString(recBytesForShort) + "\r\n");
 93 
 94                 return true;
 95             }
 96             catch (Exception e)
 97             {
 98                 socket.Close();
 99 
100                 return false;
101             }
102 
103         }    
客户端
```
**服务端**：
```
1         static void Main(string[] args)
  2         {
  3             //设置监听的ip和端口号
  4             IPAddress ip = IPAddress.Parse("192.168.120.56");
  5             IPEndPoint ipEP = new IPEndPoint(ip, 8088);
  6 
  7             //迎宾socket
  8             Socket serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
  9             //绑定
 10             serverSocket.Bind(ipEP);
 11             //监听
 12             serverSocket.Listen(10);
 13             Console.WriteLine("监听已开\r\n");
 14 
 15             while (true)
 16             {
 17                 //这里会阻塞当前线程，以等待连接
 18                 Socket socketTemp = serverSocket.Accept();
 19 
 20                 //开启其他线程处理数据
 21                 ParameterizedThreadStart pts = new ParameterizedThreadStart(ReceiveAndSend);//这里传入的方法必须是静态
 22                 new Thread(pts).Start(socketTemp);
 23             }
 24 
 25         }
 26 
 27         /// <summary>
 28         /// socket循环读取和发送的方法，建议用于多线程异步处理
 29         /// </summary>
 30         /// <param name="socketTemp">单用户Socket对象</param>
 31         private static void ReceiveAndSend(object socketTemp)
 32         {
 33             Socket socket = (Socket)socketTemp;
 34             IPEndPoint ipep = ((IPEndPoint)(socket.RemoteEndPoint));
 35             string ip = ipep.Address.ToString();//获得对方ip
 36             string port = ipep.Port.ToString();//获得对方端口
 37             byte[] buffer = new byte[1024];
 38 
 39             while (true)//循环接收和发送
 40             {
 41                 //接收并显示
 42                 string result="";
 43                 bool isReceiveMsgSuccess = ReceiveMsg(socket, ref result);
 44                 if (isReceiveMsgSuccess)
 45                 {
 46                     Console.WriteLine(DateTime.Now.ToLongDateString()+" "+ DateTime.Now.ToLongTimeString());
 47                     Console.WriteLine(ip + " _ " + port + "：" + result);
 48                 }
 49 
 50                 //回发
 51                 string sendBackStr = "服务端已接收:" + result;
 52                 SendMsg(socket, sendBackStr);
 53 
 54                 //socket.Shutdown(SocketShutdown.Both);
 55                 //socket.Close();
 56                 //socket.Dispose();
 57             }
 58         }
 59 
 60         /// <summary>
 61         /// 接收内容（如果失败，会释放socket）
 62         /// </summary>
 63         /// <param name="socket">接受内容的socket</param>
 64         /// <param name="msg">输出字符串</param>
 65         /// <returns>接收是否成功</returns>
 66         private static bool ReceiveMsg(Socket socket,ref string msg)
 67         {
 68             try
 69             {
 70                 int recNumber;//接收到的字节数量
 71                 byte[] recBytes = new byte[1024];//缓冲字节数组
 72                 recNumber = socket.Receive(recBytes);//接收行为
 73                 byte[] recBytesForShort = new byte[recNumber];
 74                 Array.Copy(recBytes, recBytesForShort, recNumber);//拷贝
 75 
 76                 //显示
 77                 msg += Encoding.UTF8.GetString(recBytesForShort) + "\r\n";
 78 
 79                 return true;
 80             }
 81             catch (Exception e)
 82             {
 83                 socket.Close();
 84 
 85                 return false;
 86             }
 87 
 88         }
 89 
 90         /// <summary>
 91         /// 发送内容（如果失败，会释放socket）
 92         /// </summary>
 93         /// <param name="socket">发送内容的socket</param>
 94         /// <param name="msg">输入字符串</param>
 95         /// <returns>接收是否成功</returns>
 96         private static bool SendMsg(Socket socket, string msg)
 97         {
 98             try
 99             {
100                 socket.Send(Encoding.UTF8.GetBytes(msg));
101                 return true;
102             }
103             catch (Exception e)
104             {
105                 socket.Close();
106                 return false;
107             }
108         }    
服务端
```
需要源码请移步：[客户端](http://download.csdn.net/detail/mm190835773/9775068)、[服务端](http://download.csdn.net/detail/mm190835773/9775063)

