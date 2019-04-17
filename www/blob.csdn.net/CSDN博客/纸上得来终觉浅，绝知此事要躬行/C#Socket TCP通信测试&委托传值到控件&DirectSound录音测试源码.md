# C#Socket TCP通信测试&委托传值到控件&DirectSound录音测试源码 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年12月28日 09:49:12[boonya](https://me.csdn.net/boonya)阅读数：167








C#里面Socket有异步和同步之分，可参考:[https://docs.microsoft.com/en-us/dotnet/framework/network-programming/socket-code-examples](https://docs.microsoft.com/en-us/dotnet/framework/network-programming/socket-code-examples)进行学习。网络上很多有关DirectSound的Socket声音采集示例，不过都是单独的一个工具类（如：DirectSoundCapture），花了点时间实现了Socket的调用，顺便总结分享一下。下图为测试示例：

![](https://img-blog.csdn.net/20171228095830813)


## Socket客户端



```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
/// <summary>
/// 参考:https://docs.microsoft.com/en-us/dotnet/framework/network-programming/socket-code-examples
/// </summary>
namespace DirectsoundTest
{
    class SocketClient
    {
        /// <summary>
        /// 启动客户端socket连接
        /// </summary>
        public static void StartClient(string data)
        {
            // Data buffer for incoming data.  
            byte[] bytes = new byte[1024];

            // Connect to a remote device.  
            try
            {
                // Establish the remote endpoint for the socket.  
                // This example uses port 11000 on the local computer.  
                IPHostEntry ipHostInfo = Dns.GetHostEntry(Dns.GetHostName());
                IPAddress ipAddress = ipHostInfo.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, 11000);

                // Create a TCP/IP  socket.  
                Socket sender = new Socket(ipAddress.AddressFamily,SocketType.Stream, ProtocolType.Tcp);

                // Connect the socket to the remote endpoint. Catch any errors.  
                try
                {
                    sender.Connect(remoteEP);

                    Console.WriteLine("Socket connected to {0}",
                        sender.RemoteEndPoint.ToString());

                    // Encode the data string into a byte array.  
                    byte[] msg = Encoding.UTF8.GetBytes(data+"<EOF>");

                    // Send the data through the socket.  
                    int bytesSent = sender.Send(msg);

                    // Receive the response from the remote device.  
                    int bytesRec = sender.Receive(bytes);
                    Console.WriteLine("Echoed test = {0}",
                        Encoding.UTF8.GetString(bytes, 0, bytesRec));

                    // Release the socket.  
                    sender.Shutdown(SocketShutdown.Both);
                    sender.Close();

                }
                catch (ArgumentNullException ane)
                {
                    Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
                }
                catch (SocketException se)
                {
                    Console.WriteLine("SocketException : {0}", se.ToString());
                }
                catch (Exception e)
                {
                    Console.WriteLine("Unexpected exception : {0}", e.ToString());
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

    }
}
```





## Socket服务端



```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
/// <summary>
/// 参考:https://docs.microsoft.com/en-us/dotnet/framework/network-programming/socket-code-examples
/// </summary>
namespace DirectsoundTest
{
    class SocketServer
    {
        // Incoming data from the client.  
        public static string data = null;

        /// <summary>
        /// 启动服务端socket监听
        /// </summary>
        public static void StartListening()
        {
            // Data buffer for incoming data.  
            byte[] bytes = new Byte[1024];

            // Establish the local endpoint for the socket.  
            // Dns.GetHostName returns the name of the   
            // host running the application.  
            IPHostEntry ipHostInfo = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress ipAddress = ipHostInfo.AddressList[0];
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 11000);

            // Create a TCP/IP socket.  
            Socket listener = new Socket(ipAddress.AddressFamily,SocketType.Stream, ProtocolType.Tcp);

            // Bind the socket to the local endpoint and   
            // listen for incoming connections.  
            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(10);

                // Start listening for connections.  
                while (true)
                {
                    Console.WriteLine("Waiting for a connection...");
                    // Program is suspended while waiting for an incoming connection.  
                    Socket handler = listener.Accept();
                    data = null;

                    // An incoming connection needs to be processed.  
                    while (true)
                    {
                        bytes = new byte[1024];
                        int bytesRec = handler.Receive(bytes);
                        data += Encoding.UTF8.GetString(bytes, 0, bytesRec);
                        if (data.IndexOf("<EOF>") > -1)
                        {
                            break;
                        }
                    }

                    // Show the data on the console.  
                    Console.WriteLine("Text received : {0}", data);

                    // Echo the data back to the client.  
                    byte[] msg = Encoding.UTF8.GetBytes(data);

                    handler.Send(msg);
                    handler.Shutdown(SocketShutdown.Both);
                    handler.Close();
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }

            Console.WriteLine("\nPress ENTER to continue...");
            Console.Read();

        }

    }
}
```





## 窗体&委托传值



```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace DirectsoundTest
{
    public partial class FrmSocket : Form
    {
        public FrmSocket()
        {
            InitializeComponent();
        }


        /// <summary>
        /// 启动服务端socket连接
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnBindSocketServer_Click(object sender, EventArgs e)
        {
            btnBindSocketServer.Enabled = false;
            Thread thread = new Thread(() => {
                SocketServer.StartListening();
            });
            thread.IsBackground = true;
            thread.Start();


        }


        // 委托设置值

        delegate void setDataToListView(string data);
        private void SetListViewData(string data)
        {
            if (this.listViewData.InvokeRequired)
            {
                setDataToListView stcb = new setDataToListView(SetListViewData);
                this.Invoke(stcb, new object[] { data });
            }
            else
            {
                this.listViewData.Items.Add(data);


            }
        }


        /// <summary>
        /// 客户端发送socket连接数据
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnSocketClient_Click(object sender, EventArgs e)
        {
            // 发送UTF8文字
            byte[] buffer = Encoding.UTF8.GetBytes(this.textBox.Text.ToString());
            string data = Encoding.UTF8.GetString(buffer);
            Thread thread = new Thread(() => {
                SocketClient.StartClient(data);
                SetListViewData(data);//委托设置控件的值
            });
            thread.IsBackground = true;
            thread.Start();


        }


        /// <summary>
        /// 打开服务端
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void toolStripMenuItemServer_Click(object sender, EventArgs e)
        {
            if (!Setting.SERVER_BINDED)
            {
                FrmServer frmServer = new FrmServer();
                frmServer.Show();
                Setting.SERVER_BINDED = true;
            }
            else {
                MessageBox.Show("服务端已注册");
            }
        }


        /// <summary>
        /// 打开客户端
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void toolStripMenuItemClient_Click(object sender, EventArgs e)
        {
            FrmClient frmClient = new FrmClient();
            frmClient.Show();
        }
    }
}
```

**委托写法-实现跨线程修改ListView控件值：**




```
// 委托设置值

        delegate void setDataToListView(string data);
        private void SetListViewData(string data)
        {
            if (this.listViewData.InvokeRequired)
            {
                setDataToListView stcb = new setDataToListView(SetListViewData);
                this.Invoke(stcb, new object[] { data });
            }
            else
            {
                this.listViewData.Items.Add(data);

            }
        }
```


## DirectSound录音测试源码

Socket例子和DirectSound录音代码都在Github上可以找到。

![](https://img-blog.csdn.net/20171228094523277)




Github地址：[https://github.com/BoonyaCSharp-ASP/DirectSoundTest](https://github.com/BoonyaCSharp-ASP/DirectSoundTest)






