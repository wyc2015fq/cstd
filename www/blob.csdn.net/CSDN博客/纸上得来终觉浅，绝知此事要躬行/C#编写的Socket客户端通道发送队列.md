# C#编写的Socket客户端通道发送队列 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月08日 15:58:10[boonya](https://me.csdn.net/boonya)阅读数：696








C#编写的Socket客户端通道发送队列：Socket、Channel、Queue这里就不多做介绍了，本篇旨在实现一个Socket管理通道的队列服务。

## 创建一个Channel



```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using SQ.Base;
using System.Net;

namespace Network
{
    public delegate void EventChannelReceive(object sender, ChannelReceiveArg arg);
    public delegate void EventChannelSend(object sender, ChannelSendArg arg);
    public delegate void EventChannelConnect(object sender, ChannelConnectArg arg);
    public delegate void EventChannelDispose(object sender, ChannelDisposeArg arg);
    public delegate void EventChannelError(object sender, ChannelErrorArg arg);

    /// <summary>
    /// 连接通道
    /// </summary>
    public abstract class Channel
    {
        #region 属性
        private ChannelQueue SendQ;
        /// <summary>
        /// 套接字
        /// </summary>
        public Socket Socket { get; protected set; }
        /// <summary>
        /// 远程端口
        /// </summary>
        public int RemotePort { get; set; }
        /// <summary>
        /// 远程IP地址
        /// </summary>
        public string RemoteHost { get; set; }
        /// <summary>
        /// 通道状态
        /// </summary>
        public ChannelState ChannelState { get; protected set; }
        /// <summary>
        /// 用户标识对象
        /// </summary>
        public object Tag { get; set; }
        /// <summary>
        /// 数据发送完成委托
        /// </summary>
        public EventChannelSend DataSend;
        /// <summary>
        /// 收到数据委托
        /// </summary>
        public EventChannelReceive DataReceive;
        /// <summary>
        /// 通道错误事件
        /// </summary>
        public event EventChannelError ChannelError;
        /// <summary>
        /// 通道断开事件
        /// </summary>
        public event EventChannelDispose ChannelDispose;
        /// <summary>
        /// 通道连接事件
        /// </summary>
        public event EventChannelConnect ChannelConnect;
        #endregion

        #region 公共方法
        /// <summary>
        /// 用已连接套接字构造通道
        /// </summary>
        /// <param name="socket">连接套接字</param>
        public Channel(Socket socket)
        {
            Socket = socket;

            IPEndPoint endPoint = Socket.RemoteEndPoint as IPEndPoint;
            RemoteHost = endPoint.Address.ToString();
            RemotePort = endPoint.Port;

            SendQ = Utils.GetSendQueue().AddChannelQueue(this);
        }

        /// <summary>
        /// 用IP地址和端口号构造通道
        /// </summary>
        /// <param name="remoteHost">远程IP地址</param>
        /// <param name="remotePort">端口号</param>
        public Channel(string remoteHost, int remotePort)
        {
            RemoteHost = remoteHost;
            RemotePort = remotePort;

            SendQ = Utils.GetSendQueue().AddChannelQueue(this);
        }

        /// <summary>
        /// 释放通道
        /// </summary>
        public virtual void Close()
        {
            SendQ.RemoveChannelQueue();
        }

        /// <summary>
        /// 连接
        /// </summary>
        public abstract void Connect();

        /// <summary>
        /// 抛送异步请求(Server类中创建通道之后调用,主动连接则不用调用)
        /// </summary>
        public abstract void StartReceiveAsync();



        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="data">数据</param>
        /// <param name="token">令牌.数据发送完成后在发送数据参数中返回</param>
        internal void Send(byte[] data, object token = null)
        {
            Send(data, 0, data.Length, token);
        }
        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="data">数据</param>
        /// <param name="offset">偏移量</param>
        /// <param name="count">长度</param>
        /// <param name="token">令牌.数据发送完成后在发送数据参数中返回</param>
        internal abstract void Send(byte[] data, int offset, int count, object token = null);


        public void SendAsync(byte[] data)
        {
            if (SendQ.DataQueue.Count > 500) SQ.Base.Log.WriteLog4("SendQ队列长度:" + SendQ.DataQueue.Count.ToString());
            SendQ.Enqueue(data);
        }
        #endregion

        #region 事件处理
        /// <summary>
        /// 通道连接事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnConnect(object sender, ChannelConnectArg arg)
        {
#if FUNCINLOG
			Log.WriteLog4("Channel.OnConnect(sender, arg) in.", LOGTYPE.DEBUG);
#endif
            if (ChannelConnect != null)
            {
                try
                {
                    ChannelConnect(sender, arg);
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Channel event error", ex);
                }
            }
#if FUNCOUTLOG
			Log.WriteLog4("Channel.OnConnect(sender, arg) out.", LOGTYPE.DEBUG);
#endif
        }

        /// <summary>
        /// 通道释放事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnDispose(object sender, ChannelDisposeArg arg)
        {
#if FUNCINLOG
			Log.WriteLog4("Channel.OnDispose(sender, arg) in.", LOGTYPE.DEBUG);
#endif
            if (ChannelDispose != null)
            {
                try
                {
                    ChannelDispose(sender, arg);
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Channel event error", ex);
                }
            }
#if FUNCOUTLOG
			Log.WriteLog4("Channel.OnDispose(sender, arg) out.", LOGTYPE.DEBUG);
#endif
        }

        /// <summary>
        /// 通道出错事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnError(object sender, ChannelErrorArg arg)
        {
#if FUNCINLOG
			Log.WriteLog4("Channel.OnError(sender, arg) in.", LOGTYPE.DEBUG);
#endif
            if (ChannelError != null)
            {
                try
                {
                    ChannelError(sender, arg);
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Channel event error", ex);
                }
            }
#if FUNCOUTLOG
			Log.WriteLog4("Channel.OnError(sender, arg) out.", LOGTYPE.DEBUG);
#endif
        }

        /// <summary>
        /// 通道收到数据事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnReceive(object sender, ChannelReceiveArg arg)
        {
#if FUNCINLOG
			Log.WriteLog4("Channel.OnReceive(sender, arg) in.", LOGTYPE.DEBUG);
#endif
            if (DataReceive != null)
            {
                try
                {
                    DataReceive?.Invoke(sender, arg);
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Channel event error", ex);
                }
            }
#if FUNCOUTLOG
			Log.WriteLog4("Channel.OnReceive(sender, arg) out.", LOGTYPE.DEBUG);
#endif
        }

        /// <summary>
        /// 通道数据发送完成事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnSend(object sender, ChannelSendArg arg)
        {
#if FUNCINLOG
			Log.WriteLog4("Channel.OnSend(sender, arg) in.", LOGTYPE.DEBUG);
#endif
            if (DataSend != null)
            {
                try
                {
                    DataSend(sender, arg);
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Channel event error", ex);
                }
            }
#if FUNCOUTLOG
			Log.WriteLog4("Channel.OnSend(sender, arg) out.", LOGTYPE.DEBUG);
#endif
        }
        #endregion
    }
}
```




## 创建一个TCPChannel

TCPChannel继承自Channel.




```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using SQ.Base;

namespace Network
{
    /// <summary>
    /// TCP通道
    /// </summary>
    /// <remarks>内部方法注释请参照基类</remarks>
    public class TCPChannel : Channel
    {
        /// <summary>
        /// 已启动接收
        /// </summary>
        private bool IsStartReceive;
        #region 属性
        public bool Connected
        {
            get
            {
                if (Socket != null && Socket.Connected)
                    return true;
                else
                    return false;
            }
        }
        #endregion

        #region 公共方法
        /// <summary>
        /// 使用远程IP地址和端口号创建TCP通道
        /// </summary>
        /// <param name="remoteHost"></param>
        /// <param name="remotePort"></param>
        public TCPChannel(string remoteHost, int remotePort)
            : base(remoteHost, remotePort)
        {
        }

        /// <summary>
        /// 使用已连接套接字创建TCP通道
        /// </summary>
        /// <param name="socket">已连接套接字</param>
        public TCPChannel(Socket socket)
            : base(socket)
        {
        }

        public override void StartReceiveAsync()
        {
            if (IsStartReceive)
            {
                return;
            }
            IsStartReceive = true;

            SocketAsyncEventArgs e = Managers.SocketArgManager.Allocate(true);
            e.Completed += IO_Completed;
            DateTime dt = DateTime.Now;
            while (!Connected)
            {
                if (SQ.Base.DateTimeHelper.DiffNowSec(dt) > 10)
                {
                    e.Completed -= IO_Completed;
                    Managers.SocketArgManager.Free(e, true);
                    e.SocketError = SocketError.ConnectionAborted;
                    //如果10秒都未连接成功
                    throw new Exception("等待连接超时！");
                }
                else
                {
                    Thread.Sleep(100);
                }
            }

            while (!Socket.ReceiveAsync(e))//如果同步接收完成，直接执行，不用等待IO_Completed
            {
                if (e.SocketError == SocketError.Success && e.BytesTransferred > 0)
                {
                    byte[] temp = new byte[e.BytesTransferred];
                    Array.Copy(e.Buffer, e.Offset, temp, 0, e.BytesTransferred);
                    OnReceive(this, new ChannelReceiveArg(temp));
                }
                else if (e.BytesTransferred == 0)//通道断开
                {
                    e.Completed -= IO_Completed;
                    Managers.SocketArgManager.Free(e, true);

                    OnDispose(this, new ChannelDisposeArg(this, e.SocketError));
                    break;
                }
                else//通道出错
                {
                    e.Completed -= IO_Completed;
                    Managers.SocketArgManager.Free(e, true);

                    OnError(this, new ChannelErrorArg(this, e.SocketError));
                    break;
                }
            }

        }

        public override void Connect()
        {
#if FUNCINLOG
			Log.WriteLog4("TCPChannel.Connect() in.", LOGTYPE.DEBUG);
#endif
            Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            SocketAsyncEventArgs e = Managers.SocketArgManager.Allocate(false);
            e.RemoteEndPoint = Utils.GetEndPoint(RemoteHost, RemotePort);
            //new IPEndPoint(IPAddress.Parse(RemoteHost), RemotePort);
            e.Completed += IO_Completed;

            IsStartReceive = false;

            if (!Socket.ConnectAsync(e))
            {
                OnConnect(e);
            }
#if FUNCOUTLOG
			Log.WriteLog4("TCPChannel.Connect() out.", LOGTYPE.DEBUG);
#endif
        }

        //public override void Connect(int timeout) { 
        public void Send(byte[] data)
        {
            Send(data, 0, data.Length);
        }
        //}
        /// <summary>
        /// 是否存在乱序问题?发送完成事件肯定不能保证顺序
        /// </summary>
        /// <param name="data"></param>
        /// <param name="offset"></param>
        /// <param name="count"></param>
        /// <param name="token"></param>
        internal override void Send(byte[] data, int offset, int count, object token = null)
        {
            if (data == null)
            {
                Log.WriteLog4("[Send]data=null", null, LOGTYPE.INFO);
                return;
            }
            if (data != null && data.Length == 0)
            {
                Log.WriteLog4("[Send]data.Length == 0", null, LOGTYPE.INFO);
                return;
            }
            DateTime dtTag = DateTime.Now;
            if (Connected)
            {
                SocketAsyncEventArgs e = Managers.SocketArgManager.Allocate(false);
                e.Completed += IO_Completed;
                e.SetBuffer(data, offset, count);
                e.UserToken = token;
                if (!Socket.SendAsync(e))
                {
                    OnSend(e);
                }
            }
            int a = (DateTime.Now - dtTag).Milliseconds;
            if ((DateTime.Now.Millisecond % 1000) < 10 && a > 0) SQ.Base.Log.WriteLog4("处理[Send]耗时(Milliseconds):" + a.ToString() + " __ data.Length = " + data.Length.ToString(), LOGTYPE.INFO);
        }

        public override void Close()
        {
            if (Connected)
            {
                IsStartReceive = false;
                Socket.Close();
                Socket.Dispose();
                base.Close();
            }
        }
        #endregion

        #region 异步请求结果处理
        /// <summary>
        /// 发送数据异步请求结果处理
        /// </summary>
        /// <param name="e"></param>
        private void OnSend(SocketAsyncEventArgs e)
        {
#if FUNCINLOG
			Log.WriteLog4("TCPChannel.OnSend(e) in.", LOGTYPE.DEBUG);
#endif
            var arg = new ChannelSendArg(e.UserToken, e.Buffer, e.Offset, e.Count);
            e.SetBuffer(null, 0, 0);
            e.Completed -= IO_Completed;
            Managers.SocketArgManager.Free(e, false);

            OnSend(this, arg);
#if FUNCOUTLOG
			Log.WriteLog4("TCPChannel.OnSend(e) out.", LOGTYPE.DEBUG);
#endif
        }

        /// <summary>
        /// 接收数据异步请求结果处理
        /// </summary>
        /// <param name="e"></param>
        private void OnReceive(SocketAsyncEventArgs e)
        {
#if FUNCINLOG
			Log.WriteLog4("TCPChannel.OnReceive(e) in.", LOGTYPE.DEBUG);
#endif
            if (e.SocketError == SocketError.Success && e.BytesTransferred > 0)//收到数据
            {
                byte[] temp = new byte[e.BytesTransferred];
                Array.Copy(e.Buffer, e.Offset, temp, 0, e.BytesTransferred);
                OnReceive(this, new ChannelReceiveArg(temp));
                bool flag = true;//防止回收两次造成e重用的问题
                while (Connected && !Socket.ReceiveAsync(e))//如果同步接收完成，直接执行，不用等待下一次IO_Completed
                {
                    if (e.SocketError == SocketError.Success && e.BytesTransferred > 0)
                    {
                        temp = new byte[e.BytesTransferred];
                        Array.Copy(e.Buffer, e.Offset, temp, 0, e.BytesTransferred);

                        OnReceive(this, new ChannelReceiveArg(temp));
                    }
                    else if (e.BytesTransferred == 0)//通道断开
                    {
                        flag = false;
                        e.Completed -= IO_Completed;
                        Managers.SocketArgManager.Free(e, true);

                        OnDispose(this, new ChannelDisposeArg(this, e.SocketError));
                        //break;//无需break，下次循环Connected 为false
                    }
                    else//通道出错
                    {
                        flag = false;
                        e.Completed -= IO_Completed;
                        Managers.SocketArgManager.Free(e, true);

                        OnError(this, new ChannelErrorArg(this, e.SocketError));
                        //break;//无需break，下次循环Connected 为false
                    }
                }

                if (!Connected && flag)
                {
                    e.Completed -= IO_Completed;
                    Managers.SocketArgManager.Free(e, true);

                    OnDispose(this, new ChannelDisposeArg(this, e.SocketError));
                }
            }
            else if (e.BytesTransferred == 0)//通道断开
            {
                e.Completed -= IO_Completed;
                Managers.SocketArgManager.Free(e, true);

                OnDispose(this, new ChannelDisposeArg(this, e.SocketError));
            }
            else//通道出错
            {
                e.Completed -= IO_Completed;
                Managers.SocketArgManager.Free(e, true);

                OnError(this, new ChannelErrorArg(this, e.SocketError));
            }
#if FUNCOUTLOG
			Log.WriteLog4("TCPChannel.OnReceive(e) out.", LOGTYPE.DEBUG);
#endif
        }

        /// <summary>
        /// 通道连接异步请求结果处理(通过IP地址和端口号创建通道时，通道连接返回后触发)
        /// </summary>
        /// <param name="e"></param>
        private void OnConnect(SocketAsyncEventArgs e)
        {
#if FUNCINLOG
			Log.WriteLog4("TCPChannel.OnConnect(e) in.", LOGTYPE.DEBUG);
#endif
            if (e.SocketError == SocketError.Success)
            {
                StartReceiveAsync();
            }

            OnConnect(this, new ChannelConnectArg(this, e.SocketError));

            e.Completed -= IO_Completed;
            Managers.SocketArgManager.Free(e, false);

#if FUNCOUTLOG
			Log.WriteLog4("TCPChannel.OnConnect(e) out.", LOGTYPE.DEBUG);
#endif
        }
        #endregion

        #region 内部方法
        /// <summary>
        /// 异步请求回调处理方法
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void IO_Completed(object sender, SocketAsyncEventArgs e)
        {
#if FUNCINLOG
			Log.WriteLog4("TCPChannel.IO_Completed(sender, e) in.", LOGTYPE.DEBUG);
#endif
            try
            {
                switch (e.LastOperation)
                {
                    case SocketAsyncOperation.Connect:
                        OnConnect(e);
                        break;
                    case SocketAsyncOperation.Receive:
                        OnReceive(e);
                        break;
                    case SocketAsyncOperation.Send:
                        OnSend(e);
                        break;
                    default:
                        e.SetBuffer(null, 0, 0);
                        e.Completed -= IO_Completed;
                        Managers.SocketArgManager.Free(e, false);
                        break;
                }
            }
            catch (Exception ex)
            {
                Log.WriteLog4Ex("TCPChannel.IO_Completed(sender, e) error", ex);
            }
#if FUNCOUTLOG
			Log.WriteLog4("TCPChannel.IO_Completed(sender, e) out.", LOGTYPE.DEBUG);
#endif
        }
        #endregion
    }
}
```




## 发送队列SendQueue






```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace Network
{
    public class SendQueue
    {
        public Dictionary<Channel, ChannelQueue> dit = new Dictionary<Channel, ChannelQueue>();

        public void SendAll()
        {
            var arr = dit.Values.ToArray();
            foreach (var item in arr)
            {
                item.SendAll();
            }
        }
        public ChannelQueue AddChannelQueue(Channel cl)
        {
            var cq = new ChannelQueue()
            {
                Cl = cl,
                SQU = this
            };
            dit[cl] = cq;
            return cq;
        }

        public void RemoveChannelQueue(Channel cl)
        {
            if (dit.ContainsKey(cl))
            {
                dit.Remove(cl);
            }
        }
        public void RunSend()
        {
            try
            {
                while (true)
                {
                    SendAll();
                    Thread.Sleep(100);
                }
            }
            catch (Exception ex)
            {

            }
        }
    }
    public class ChannelQueue
    {
        public SendQueue SQU;
        public Channel Cl;
        public Queue<byte[]> DataQueue = new Queue<byte[]>();
        public void Enqueue(byte[] bts)
        {
            lock (this)
            {
                DataQueue.Enqueue(bts);
            }
        }

        public void SendAll()
        {
            lock (this)
            {
                List<byte> bts = new List<byte>();
                while (DataQueue.Count > 0)
                {
                    bts.AddRange(DataQueue.Dequeue());
                }
                if (bts.Count>0) Cl.Send(bts.ToArray());
            }
        }
        public void RemoveChannelQueue()
        {
            SQU.RemoveChannelQueue(Cl);
        }
    }
}
```



## 如何使用发送队列

通过工具类Utils来初始化队列和依次获取队列。




```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;

namespace Network
{
    public class Utils
    {
        static int p;
        static SendQueue[] SQS;
        static bool Setuped = false;
        public static void Initialize()
        {
            if (SQS == null || !Setuped)
            {
                Setup();
            }
        }

        public static void Setup(int sendThread = 10)
        {
            SQS = new Network.SendQueue[sendThread];
            for (int i = 0; i < sendThread; i++)
            {
                SQS[i] = new SendQueue();

                Thread th = new Thread(SQS[i].RunSend);
                th.IsBackground = true;
                th.Name = "RunSend" + i;
                th.Start();
            }


            Setuped = true;


        }



        public static SendQueue GetSendQueue()
        {
            lock (SQS)
            {
                p++;
                if (p >= SQS.Length)
                {
                    p = 0;
                }
                return SQS[p];
            }
        }

        static System.Text.RegularExpressions.Regex reg = new System.Text.RegularExpressions.Regex(@"\d+\.\d+\.\d+\.\d+");
        public static EndPoint GetEndPoint(string Host, int Port)
        {
            if (reg.IsMatch(Host))
            {
                return new IPEndPoint(IPAddress.Parse(Host), Port);
            }
            else
            {
                return new DnsEndPoint(Host, Port);

            }


        }
    }
}
```
初始化使用队列：



` Network.Utils.Setup(2);`
创建Socket通道连接：




```
channel = new Network.TCPChannel(txtServer.Text.Trim(), Convert.ToInt32(txtPort.Text.Trim()));
            channel.DataReceive = Receive;
            channel.DataSend = DataSend;
            channel.ChannelConnect += new EventChannelConnect(channel_ChannelConnect);
            channel.Connect();
```

Socket设置新增通道队列的代码（Channel初始化调用的是父类的构造，即Channel类构造）：




```
/// <summary>
        /// 用已连接套接字构造通道
        /// </summary>
        /// <param name="socket">连接套接字</param>
        public Channel(Socket socket)
        {
            Socket = socket;

            IPEndPoint endPoint = Socket.RemoteEndPoint as IPEndPoint;
            RemoteHost = endPoint.Address.ToString();
            RemotePort = endPoint.Port;

            SendQ = Utils.GetSendQueue().AddChannelQueue(this);
        }
```
此时已经将当前的Channel对象加入到指定大小的队列之中。


## 使用通道来构造客户端

Client类



```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SQ.Base;
using System.Threading;

namespace Network
{
    #region 委托定义
    public delegate void EventClientConnect(object sender, ClientConnectArg arg);
    public delegate void EventClientReceive(object sender, ClientReceiveArg arg);
    public delegate void EventClientSend(object sender, ClientSendArg arg);
    public delegate void EventClientlDispose(object sender, ClientDisposeArg arg);
    public delegate void EventClientError(object sender, ClientErrorArg arg);
    #endregion

    public abstract class Client
    {
        private AutoResetEvent connWaiter = new AutoResetEvent(false);

        #region 属性
        /// <summary>
        /// 客户端连接通道
        /// </summary>
        public Channel Channel { get; protected set; }
        /// <summary>
        /// 用户令牌
        /// </summary>
        public object Tag { get; set; }
        /// <summary>
        /// 日志打印标志
        /// </summary>
        public bool LogFocus = false;
        /// <summary>
        /// Client计数器
        /// </summary>
        public ClientCounter ClientCounter = new ClientCounter();
        /// <summary>
        /// 客户端连接事件
        /// </summary>
        public event EventClientConnect ClientConnect;
        /// <summary>
        /// 客户端释放事件
        /// </summary>
        public event EventClientlDispose ClientDispose;
        /// <summary>
        /// 客户端异常事件
        /// </summary>
        public event EventClientError ClientError;
        /// <summary>
        /// 客户端收到数据事件
        /// </summary>
        public EventClientReceive ObjectReceive;
        /// <summary>
        /// 客户端数据发送完成事件
        /// </summary>
        public EventClientSend ObjectSend;
        #endregion

        #region 事件处理
        /// <summary>
        /// 客户端连接事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnConnect(object sender, ChannelConnectArg arg)
        {
            connWaiter.Set();
            if (ClientConnect != null)
            {
                try
                {
                    ClientConnect(this, new ClientConnectArg(this));
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Client event error", ex);
                }
            }
        }

        /// <summary>
        /// 客户端释放事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnDispose(object sender, ChannelDisposeArg arg)
        {
            if (ClientDispose != null)
            {
                try
                {
                    ClientDispose(this, new ClientDisposeArg(this));
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Client event error", ex);
                }
            }
        }

        /// <summary>
        /// 客户端错误事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnError(object sender, ChannelErrorArg arg)
        {
            if (ClientError != null)
            {
                try
                {
                    ClientError(this, new ClientErrorArg(this));
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Client event error", ex);
                }
            }
        }

        /// <summary>
        /// 客户端收到数据事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnReceive(object sender, ChannelReceiveArg arg)
        {
            ClientCounter.RefreshReceiveCount();
            if (ClientCounter.ReceiveCountSum < 1 || (LogFocus && arg.Data != null)) Log.WriteLog4("[OnReceive]source=" + Channel.RemoteHost + ":" + Channel.RemotePort.ToString() + ",arg.Data=", arg.Data, LOGTYPE.INFO);
            if (ObjectReceive != null)
            {
                try
                {
                    ObjectReceive(this, new ClientReceiveArg(arg.Data));
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Client event error", ex);
                }
            }
        }

        /// <summary>
        /// 客户端数据发送完成事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnSend(object sender, ChannelSendArg arg)
        {
            bool boolExcuteObjectSend = false;
            if (ObjectSend != null)
            {
                try
                {
                    ObjectSend(this, new ClientSendArg(arg.Token));
                    boolExcuteObjectSend = true;
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Client event error", ex);
                }
            }
            if (LogFocus && arg.Buffer != null) Log.WriteLog4("[OnSend]ExcuteObjectSend=" + boolExcuteObjectSend.ToString() + " __ 发往" + (sender as Channel).RemoteHost + ":" + (sender as Channel).RemotePort.ToString() + "的数据：", arg.Buffer, LOGTYPE.INFO);
            ClientCounter.RefreshSentCount();
            //if ((sender as Channel).RemoteHost == "222.134.129.230" || (sender as Channel).RemoteHost == "192.168.121.1" || (sender as Channel).RemoteHost == "192.168.122.1" || (sender as Channel).RemoteHost == "10.10.11.177")
        }
        #endregion

        #region 公共方法
        /// <summary>
        /// 通道通道构造客户端
        /// </summary>
        /// <param name="channel">通道</param>
        public Client(Channel channel)
        {
            Channel = channel;
            Channel.ChannelConnect += OnConnect;
            Channel.ChannelDispose += OnDispose;
            Channel.ChannelError += OnError;
            Channel.DataReceive += OnReceive;
            Channel.DataSend += OnSend;
        }

        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="obj"></param>
        /// <param name="token"></param>
        public virtual void Send(byte[] data)
        {
            if (data != null)
            {
                //Channel.SendAsync(data);
                Channel.Send(data);
                ClientCounter.RefreshSendCount();
                if (ClientCounter.SendCountSum < 1 || (LogFocus && data != null)) Log.WriteLog4("[Send]target=" + Channel.RemoteHost + ":" + Channel.RemotePort.ToString() + ",data=", data, LOGTYPE.INFO);
            }
        }

        /// <summary>
        /// 关闭客户端
        /// </summary>
        public virtual void Close()
        {
            Channel.Close();
        }

        public bool Connect(int _timeout)
        {
            try
            {
                bool result = false;
                Channel.Connect();
                if (connWaiter.WaitOne(_timeout))
                {
                    result = true;
                }
                else
                {
                    result = false;
                }
                return result;
            }
            catch (Exception e)
            {
                return false;
            }
        }
        #endregion
    }

}
```
其构造方法就可以通过Channel来实现。







