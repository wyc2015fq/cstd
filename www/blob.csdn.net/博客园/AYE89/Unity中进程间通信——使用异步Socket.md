
# Unity中进程间通信——使用异步Socket - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[Unity中进程间通信——使用异步Socket](https://www.cnblogs.com/eniac1946/p/7569641.html)
|Posted on|2017-09-21 17:18|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7569641)|[收藏](#)
|开发Unity项目过程中，即时通信功能来完成服务器与客户端自定义的数据结构封装。
|如果要序列化和数据封装参考：|[Unity3D之C\#用Socket传数据包](https://www.oschina.net/code/snippet_658631_36293)
|蓝鸥3G封装的类
|客户端脚本ClientScript
|/// <summary>
/// Client Script.
/// Created By 蓝鸥3G 2014.08.23
/// </summary>
using UnityEngine;
using System.Collections;
public class ClientScript: MonoBehaviour {
    string msg = "";
    // Use this for initialization
    LOSocket client;
    void Start () {
        client = LOSocket.GetSocket(LOSocket.LOSocketType.CLIENT);
        client.InitClient ("127.0.0.1", 2222, ((string content) => {
            //收到服务器的回馈信息
        }));
    }
    void OnGUI() {
        msg = GUI.TextField(new Rect(0, 0, 500, 40), msg);
        if(GUI.Button(new Rect(0, 50, 100, 30), "Send"))
        {
            client.SendMessage (msg);
        }
    }
}
|服务器端脚本
|using UnityEngine;
using System.Collections;
public class ServerScript : MonoBehaviour {
    private string receive_str;
    LOSocket server;
    // Use this for initialization
    void Start () 
    {
        server = LOSocket.GetSocket(LOSocket.LOSocketType.SERVER);
        //初始化服务器
        server.InitServer((string content) => {
            receive_str = content;
        });
    }
    void OnGUI()
    {
        if (receive_str != null) 
        {
            GUILayout.Label (receive_str);
        }
    }
}
|LOSocket框架
|using UnityEngine;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Text;
//收到消息后的委托回调
public delegate void ReceiveCallBack(string content);
public class LOSocket{
    //可以创建的Socket端口类型
    public enum LOSocketType 
    {
        CLIENT = 0,
        SERVER = 1,
    }
    \#region --------取消构造器
    private LOSocket()
    {
    }
        
    \#endregion
    
    \#region --------公共代码
    //通过静态方法获取不同的端口类型
    public static LOSocket GetSocket(LOSocket.LOSocketType type)
    {
        LOSocket socket = null;

        switch (type) {
        case LOSocketType.CLIENT:
            {
                //创建一个新的客户端
                socket = new LOSocket ();
                break;
            }
        case LOSocketType.SERVER:
            {
                //获取服务端
                socket = GetServer ();
                break;
            }
        }
        return socket;
    }
    \#endregion
    \#region --------客户端部分代码
    private Socket clientSocket;
    //声明客户端收到服务端返回消息后的回调委托函数
    private ReceiveCallBack clientReceiveCallBack;
    //用来存储服务端返回的消息数据
    byte[] Buffer = new byte[1024];
    //初始化客户端Socket信息
    public void InitClient(string ip,int port,ReceiveCallBack ccb)
    {
        this.clientReceiveCallBack = ccb;
        this.clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        IPAddress address = IPAddress.Parse (ip);
        IPEndPoint ep = new IPEndPoint (address, port);
        this.clientSocket.Connect(ep);
        //开始异步等待接收服务端消息
        this.clientSocket.BeginReceive (Buffer, 0, Buffer.Length, SocketFlags.None, new System.AsyncCallback(ReceiveFromServer), this.clientSocket);
    }
    //收到服务端返回消息后的回调函数
    void ReceiveFromServer(System.IAsyncResult ar)
    {
        //获取当前正在工作的Socket对象
        Socket worker = ar.AsyncState as Socket;
        int ByteRead=0;
        try
        {
            //接收完毕消息后的字节数
            ByteRead = worker.EndReceive(ar);
        }
        catch (System.Exception ex)
        {
            this.clientReceiveCallBack (ex.ToString ());
        }
        if (ByteRead > 0)
        {
            string Content = Encoding.Default.GetString (Buffer);
            //通过回调函数将消息返回给调用者
            this.clientReceiveCallBack (Content);
        }
        //继续异步等待接受服务器的返回消息
        worker.BeginReceive(Buffer, 0, Buffer.Length, SocketFlags.None, new System.AsyncCallback(ReceiveFromServer), worker);
    }
    //客户端主动发送消息
    public void SendMessage(string message)
    {
        if (message == null)
            return;
        message += "\r\n";
        byte[] sendData = Encoding.UTF8.GetBytes (message);
        //异步发送消息请求
        clientSocket.BeginSend (sendData, 0, sendData.Length, SocketFlags.None, new System.AsyncCallback (SendToServer), clientSocket);
    }
    //发送消息结束的回调函数
    void SendToServer(System.IAsyncResult ar)
    {
        Socket worker = ar.AsyncState as Socket;
        worker.EndSend (ar);
    }
    \#endregion

    \#region -------服务器部分代码
    //服务器端收到消息的存储空间
    byte[] ReceiveBuffer = new byte[1024];
    //服务器收到消息后的回调委托
    private ReceiveCallBack callback;
    //单例模式  
    private static LOSocket serverSocket;  
    private static LOSocket GetServer() {  
        if (serverSocket == null) {  
            serverSocket = new LOSocket();  
        }  
        return serverSocket;  
    }  
    //初始化服务器信息
    public void InitServer(ReceiveCallBack cb) {
        this.callback = cb;
        // 1.
        Socket server_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        // 2.
        IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, 2222);
        // 3.
        server_socket.Bind(endPoint);
        // 4.
        server_socket.Listen(10);
        // 5.开始异步等待客户端的请求链接
        server_socket.BeginAccept (new System.AsyncCallback (Accept), server_socket);
        this.callback ("开启服务器" + endPoint.ToString());
    }
    //接受到客户端的链接请求后的回调函数
    void Accept(System.IAsyncResult ar){
        //获取正在工作的Socket对象 
        Socket socket = ar.AsyncState as Socket;  
        //存储异步操作的信息，以及用户自定义的数据  
        Socket worker = socket.EndAccept(ar);  
        SocketError error;
        //开始异步接收客户端发送消息内容
        worker.BeginReceive (ReceiveBuffer, 0, ReceiveBuffer.Length, SocketFlags.None, new System.AsyncCallback (Receive), worker);
        //继续异步等待新的客户端链接请求
        socket.BeginAccept(new System.AsyncCallback(Accept), socket);  
    }
    //服务端收到客户端的消息后的回调函数
    void Receive(System.IAsyncResult ar)
    {
        //获取正在工作的Socket对象
        Socket worker = ar.AsyncState as Socket;
        int ByteRead=0;
        try
        {
            ByteRead = worker.EndReceive(ar);
        }
        catch (System.Exception ex)
        {
            this.callback (ex.ToString ());
        }
        if (ByteRead > 0)
        {
            string Content = Encoding.Default.GetString (ReceiveBuffer);
            this.callback (Content);
        }
        //继续异步等待客户端的发送消息请求
        worker.BeginReceive(ReceiveBuffer, 0, ReceiveBuffer.Length, SocketFlags.None, new System.AsyncCallback(Receive), worker);
    }
    \#endregion
}
|\#python客户端测试代码
|\#! /usr/env/bin python  
\#codinf=utf-8  
import socket  
import time  
  
def Client():  
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
    sock.connect(('', 8000))\#链接服务器  
    while 1:  
        print 'enter something:',  
        ent = raw_input()  
        if ent == '':  
            break  
        sock.send(ent)\#发送数据给服务器  
        time.sleep(1)  
        data = sock.recv(1024)\#接收服务器发过来到数据  
        print 'echo=>',data  
    sock.close()  
  
if __name__ == '__main__':  
    Client()
|python服务器端测试代码
|\#! /usr/env/bin python  
\#coding=utf-8  
import socket  
  
\#Server  
def Server():  
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
    sock.bind(('localhost', 8000))  
    sock.listen(5)\#监听，最大链接数  
    while 1:  
        connection, address = sock.accept()\#开始接受请求,进入等待阻塞状态，直到有链接到达  
        while 1:  
            data = connection.recv(1024)\#接收客户端发过来的数据  
            if not data:  
                break  
            print data,address  
            connection.send(data)\#发送数据到客户端，即上面到connection  
        connection.close()  
  
  
  
if __name__ == '__main__':  
    Server()
|注意，这里没有对服务器的连接关闭，自己可以加上：sock.close()









|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
