# C#编写的Socket基于通道的服务端 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月08日 16:41:09[boonya](https://me.csdn.net/boonya)阅读数：201








本文是继[C#编写的Socket客户端通道发送队列](http://blog.csdn.net/boonya/article/details/79003347)的续集。有客户端还需要服务端，服务端也要采用通道的方式来创建。

##  连接服务类基类



```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using SQ.Base;

namespace Network
{
    /// <summary>
    /// 连接服务类基类
    /// </summary>
    public abstract class Server
    {
        #region 属性
        /// <summary>
        /// 通道连接事件
        /// </summary>
        public virtual event EventChannelConnect ChannelConnect;
        /// <summary>
        /// 通道断开事件
        /// </summary>
        public virtual event EventChannelDispose ChannelDispose;
        /// <summary>
        /// 通道错误事件
        /// </summary>
        public virtual event EventChannelError ChannelError;
        /// <summary>
        /// 获取本地套接字
        /// </summary>
        public Socket LocalSocket { get; protected set; }
        /// <summary>
        /// 获取服务器本地监听IP地址
        /// </summary>
        public string LocalHost { get; protected set; }
        /// <summary>
        /// 获取服务器本地绑定或监听端口
        /// </summary>
        public int LocalPort { get; protected set; }
        /// <summary>
        /// 获取连接客户端管理实例
        /// </summary>
        public ListLocker<Channel> Channels { get; protected set; }
        /// <summary>
        /// 用户标识对象
        /// </summary>
        public object Tag { get; set; }
        #endregion

        #region 事件处理
        /// <summary>
        /// 通道连接事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnChannelConnect(object sender, ChannelConnectArg arg)
        {
#if FUNCINLOG
			Log.WriteLog4("TCPServer.OnChannelConnect(sender, arg) in.", LOGTYPE.DEBUG);
#endif
            if (ChannelConnect != null)
            {
                try
                {
                    ChannelConnect(sender, arg);
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Server event error", ex);
                    if (ex.Message == "等待连接超时！")
                    {
                        arg.SocketError = SocketError.SocketError;
                    }
                }
            }

            if (arg.SocketError == SocketError.Success)
            {
                try
                {
                    Channels.Enter();
                    Channels.Add(arg.Channel);
                }
                finally
                {
                    Channels.Exit();
                }
            }
#if FUNCOUTLOG
			Log.WriteLog4("TCPServer.OnChannelConnect(sender, arg) out.", LOGTYPE.DEBUG);
#endif
        }

        /// <summary>
        /// 通道释放事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnChannelDispose(object sender, ChannelDisposeArg arg)
        {
#if FUNCINLOG
			Log.WriteLog4("TCPServer.OnChannelDispose(sender, arg) in.", LOGTYPE.DEBUG);
#endif
            if (ChannelDispose != null)
            {
                try
                {
                    ChannelDispose(this, arg);
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Server event error", ex);
                }
            }
            try
            {
                Channels.Enter();
                Channels.Remove(arg.Channel);
            }
            finally
            {
                Channels.Exit();
            }
#if FUNCOUTLOG
			Log.WriteLog4("TCPServer.OnChannelDispose(sender, arg) out.", LOGTYPE.DEBUG);
#endif
        }

        /// <summary>
        /// 通道异常事件处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="arg"></param>
        protected virtual void OnChannelError(object sender, ChannelErrorArg arg)
        {
#if FUNCINLOG
			Log.WriteLog4("TCPServer.OnChannelError(sender, arg) in.", LOGTYPE.DEBUG);
#endif
            if (ChannelError != null)
            {
                try
                {
                    ChannelError(this, arg);
                }
                catch (Exception ex)
                {
                    Log.WriteLog4Ex("Network Server event error", ex);
                }
            }
            try
            {
                Channels.Enter();
                Channels.Remove(arg.Channel);
            }
            finally
            {
                Channels.Exit();
            }
#if FUNCOUTLOG
			Log.WriteLog4("TCPServer.OnChannelError(sender, arg) out.", LOGTYPE.DEBUG);
#endif
        }
        #endregion

        #region 公共方法
        /// <summary>
        /// 构造方法
        /// </summary>
        public Server()
        {
            Channels = new ListLocker<Channel>();
            LocalSocket = null;
        }

        /// <summary>
        /// 开启服务器
        /// </summary>
        /// <returns>成功返回true,否则返回false</returns>
        public virtual bool Start()
        {
            return true;
        }

        /// <summary>
        /// 停止服务器
        /// </summary>
        public virtual void Stop()
        {
            Channel[] channels;
            try
            {
                Channels.Enter();
                channels = Channels.ToArray();
                Channels.Clear();
            }
            finally
            {
                Channels.Exit();
            }

            foreach (var channel in channels)
            {
                channel.Close();
            }
        }
        #endregion

        #region 内部方法
        /// <summary>
        /// 通过连接套接字创建通道
        /// </summary>
        /// <param name="socket">连接套接字</param>
        /// <param name="channel">子类已创建通道.如果非空,则基类在现有通道实例上处理</param>
        /// <returns>创建的通道实例.非空进创建成功,否则失败</returns>
        protected virtual Channel CreateChannelBySocket(Socket socket, Channel channel = null)
        {
            if (channel == null)
                return null;
            else
            {
                channel.ChannelDispose += OnChannelDispose;
                channel.ChannelError += OnChannelError;
                return channel;
            }
        }
        #endregion
    }
}
```




## TCP 连接服务类

TCPServer 继承自Server类：



```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using SQ.Base;

namespace Network
{
	/// <summary>
	/// TCP通道服务
	/// </summary>
	/// <remarks>内部方法注释请参照基类</remarks>
	public class TCPServer : Server
	{
		#region 公共方法
		/// <summary>
		/// TCP通道服务构造函数
		/// </summary>
		/// <param name="localHost">本地绑定网卡地址</param>
		/// <param name="localPort">本地端口号</param>
		public TCPServer(string localHost, int localPort)
		{
			LocalHost = localHost;
			LocalPort = localPort;
		}

		/// <summary>
		/// 开启服务器
		/// </summary>
		/// <returns>成功返回true,否则返回false</returns>
		public override bool Start()
		{
			try
			{
				if (LocalSocket == null)
				{
					LocalSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
					LocalSocket.Bind(new IPEndPoint(IPAddress.Parse(LocalHost), LocalPort));
					LocalSocket.Listen(64);

					SocketAsyncEventArgs e = Managers.SocketArgManager.Allocate(false);
					e.Completed += IO_Completed;
					if (LocalSocket != null && !LocalSocket.AcceptAsync(e))
					{
						OnConnect(e);
					}
					return base.Start();
				}
				else
					return false;
			}
			catch (Exception ex)
			{
				Log.WriteLog4Ex("Server启动失败", ex);
				return false;
			}
		}

		/// <summary>
		/// 停止服务器
		/// </summary>
		public override void Stop()
		{
			try
			{
				if (LocalSocket != null)
				{
					LocalSocket.Close();
					LocalSocket.Dispose();
					LocalSocket = null;

					base.Stop();
				}
				else
					Log.WriteLog4("Server停止失败.服务已停止.", LOGTYPE.WARN);
			}
			catch (Exception ex)
			{
				Log.WriteLog4Ex("Server停止失败", ex);
			}
		}
		#endregion

		#region 异步请求结果处理
		/// <summary>
		/// 通道连接IO完成时调用
		/// </summary>
		/// <param name="e"></param>
		private void OnConnect(SocketAsyncEventArgs e)
		{
#if FUNCINLOG
			Log.WriteLog4("TCPServer.OnConnect(e) in.", LOGTYPE.DEBUG);
#endif
			Channel channel = null;
			if (e.SocketError == SocketError.Success)
				channel = CreateChannelBySocket(e.AcceptSocket);
			e.AcceptSocket = null;
			OnChannelConnect(this, new ChannelConnectArg(channel, e.SocketError));

			while (LocalSocket != null && !LocalSocket.AcceptAsync(e))
			{
				channel = null;
				if (e.SocketError == SocketError.Success)
					channel = CreateChannelBySocket(e.AcceptSocket);
				e.AcceptSocket = null;
				OnChannelConnect(this, new ChannelConnectArg(channel, e.SocketError));
			}
#if FUNCOUTLOG
			Log.WriteLog4("TCPServer.OnConnect(e) out.", LOGTYPE.DEBUG);
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
			Log.WriteLog4("TCPServer.IO_Completed(sender, e) in.", LOGTYPE.DEBUG);
#endif
			try
			{
				switch (e.LastOperation)
				{
					case SocketAsyncOperation.Accept:
						OnConnect(e);
						break;
				}
			}
			catch (Exception ex)
			{
				Log.WriteLog4Ex("TCPServer.IO_Completed(sender, e) error", ex);
			}
#if FUNCOUTLOG
			Log.WriteLog4("TCPServer.IO_Completed(sender, e) out.", LOGTYPE.DEBUG);
#endif
		}

		/// <summary>
		/// 通过连接套接字创建通道
		/// </summary>
		/// <param name="socket">连接套接字</param>
		/// <param name="channel">子类已创建通道.如果非空,则基类在现有通道实例上处理</param>
		/// <returns>创建的通道实例.非空进创建成功,否则失败</returns>
		protected override Channel CreateChannelBySocket(Socket socket, Channel channel = null)
		{
#if FUNCINLOG
			Log.WriteLog4("TCPServer.CreateChannelBySocket(socket) in.", LOGTYPE.DEBUG);
#endif
			if (channel == null)
				channel = new TCPChannel(socket);
#if FUNCOUTLOG
			Log.WriteLog4("TCPServer.CreateChannelBySocket(socket) out.", LOGTYPE.DEBUG);
#endif
			return base.CreateChannelBySocket(socket, channel);
		}
		#endregion
	}
}
```




## 如何使用初始化服务端

建立一个本地服务端连接：


` Network.TCPServer ser = new Network.TCPServer("0.0.0.0", 9999);`
启动Socket连接：




```
ser.ChannelConnect += new Network.EventChannelConnect(ser_ChannelConnect);
 ser.ChannelDispose += new Network.EventChannelDispose(ser_ChannelDispose);
 ser.Start();
```

要保证服务端不出问题，还需要实现通道的连接事件和释放事件方法，此处不提。










