# C#FFmpeg视频采集与推送RTMP服务器代码思路整理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年12月14日 10:47:49[boonya](https://me.csdn.net/boonya)阅读数：6266








C#视频采集与推送RTMP服务器代码思路整理：在看过FFmpeg后是否认为写C#的视频流采集和推送还是一头雾水啊？深有此感。领导是C#的高手，说可以通过C或C++的代码直接复制粘贴到C#工程然后进行适配代码就可以了，因为C#使用ffmpeg的类名和变量、方法等都与C保持高度一致的，经领导这么一说C#里面只需要参考C或C++的实现就可以完成相关的操作了，这样就更容易理解了（涉及到指针问题，C#也支持）。本文旨在分析实现思路，而不提供源代码，敬请谅解！



## 服务端和客户端的职能

客户端负责视频采集，服务端负责视频转发或播放



## 整体设计视频传输架构

![](https://img-blog.csdn.net/20171221173815744)





说明：视频数据发送H264数据包给服务端，服务端获取到数据包进行编解码转成FLV格式的流，而RTMP只支持FLV格式的流。

原理：socket视频数据采集，socket 数据包发送，编解码处理，推流。


## 视频直播推流效果展示

启动顺序：程序客户端和服务端是两个项目需要分别启动，先启动服务端，再启动客户端，允许多个实例存在。

**第一幕：启动视频编解码服务端**

![](https://img-blog.csdn.net/20171214095056324)

**第二幕：启动视频采集摄像头客户端**

![](https://img-blog.csdn.net/20171214095332654)

点击开始连接到服务端。

**第三幕：服务端处理要连接的客户端并建立数据传输**

![](https://img-blog.csdn.net/20171214095514262)

选择客户端，并开始传输视频，此时会弹出摄像头访问窗口。


**第四幕：验证直播是否进行**

访问RTMP服务器状态监控地址，如：http://172.16.20.10:1990/stat

![](https://img-blog.csdn.net/20171214095801509)

**第五幕：使用ffplay进行直播流播放**

cmd进入ffmpeg的路径执行：ffplay rtmp://172.16.20.10:1935/live/S013333333333

![](https://img-blog.csdn.net/20171214100252313)

以上就是所有架构实现的效果了。


## 摄像头视频采集客户端

客户端窗体变量：



```
// socket数据包封装对象
JTData jtdata = new JTData();
// 数据分包对象
SplitData splitData = new SplitData();
// TCP通信通道
Network.TCPChannel channel;
// 标识用户手机号
string SimKey = "013333333333";
// 线程对象
Thread thVideo;
// Socket实例对象
private Socket send_sock;
private Queue<Bitmap> mQueues = new Queue<Bitmap>();
// 摄像头处理H264对象
Cam2H264 cam2H264 = new Cam2H264();

private Setting setting;
IPEndPoint iPEndPoint;
```
客户端窗体开始录像按钮处理：




```
private void btnStart_Click(object sender, EventArgs e)
        {
            channel = new Network.TCPChannel(txtServer.Text.Trim(), Convert.ToInt32(txtPort.Text.Trim()));//实例化TCP连接通道
            channel.DataReceive = Receive;
            channel.DataSend = DataSend;
            channel.ChannelConnect += new EventChannelConnect(channel_ChannelConnect);// 设置通道连接事件
            channel.Connect();
            //channel.StartReceiveAsync();


            btnStart.Enabled = false;
            btnStop.Enabled = true;
        }
```

客户端窗体TCP通道回调函数：




```
void channel_ChannelConnect(object sender, ChannelConnectArg arg)
        {
            Console.WriteLine(arg.SocketError.ToString());
            SendHeart();
        }

        private void SendHeart()
        {
            var lst = jtdata.Package(0x0002, SimKey);
            SendAnswer(lst);
        }

 #region 链路
        public void Receive(object sender, ChannelReceiveArg arg)
        {
            splitData.SplitDataBy7E(arg.Data, ((byte[] bGps) =>
            {
                if (bGps.Length > 11 && CheckHelper.CheckXOR(bGps, 0, bGps.Length - 1) == bGps[bGps.Length - 1])//效验通过
                {
                    var head = JTHeader.NewEntity(bGps);
                    JXData(bGps, head);
                }
            }));
        }

        public void JXData(byte[] bGps, JTHeader head)
        {
            switch (head.MsgId)
            {
                case 0x8001://通用应答
                    break;
                case 0x8B00://4.1.1 实时音视频传输请求
                    if (thVideo != null && thVideo.IsAlive)
                    {
                        Console.WriteLine("已经在传输");
                    }
                    else
                    {
                        BaseLineTime = DateTime.Now;
                        SerialNumber = 0;
                        LastIFrameTime = DateTime.MinValue;
                        LastFrameTime = DateTime.MinValue;
                        var req = JTRealVideoTransferRequest.NewEntity(bGps, head.HeadLen);
                        thVideo = new Thread(StartVideo);
                        thVideo.IsBackground = true;
                        iPEndPoint = new IPEndPoint(IPAddress.Parse(req.IPAddress), req.UdpPort);
                        thVideo.Start(iPEndPoint);
                    }
                    break;
                default:
                    break;
            }
        }
        public void DataSend(object sender, ChannelSendArg arg)
        {
        }

  public bool SendAnswer(JTPData data)
        {
            foreach (var item in data)
            {
                channel.Send(item.Data.ToArray());
                //return true;
            }
            return true;
        }
```
客户端窗体线程处理视频数据：




```
unsafe void StartVideo(object ep)
        {
            try
            {
                send_sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                send_sock.SendBufferSize = 1000000;



                cam2H264.Run(setting.VideoDevice, SendToServer, 0);

            }
            catch (Exception ex)
            {
            }
        }
```

具体264的处理基本上可以参考FFmpeg的C实现方式来做，下面是步骤：


![](https://img-blog.csdn.net/20171214102554236)

请参考：[https://github.com/FFmpeg/FFmpeg](https://github.com/FFmpeg/FFmpeg)


## 视频数据包转发服务端

注：实际服务端未做转码处理，已在客户端线程中处理。


服务端窗体常量：



```
// 开启本机TCP连接
Network.TCPServer ser = new Network.TCPServer("0.0.0.0", 9700);
// RTP服务端
RTPServer rtServer;
// 连接到的客户端列表
List<VideoClient> lstTCPChannel = new List<VideoClient>();
// 数据包拆分对象
JX.SplitData splitData = new JX.SplitData();
```

服务端窗体事件绑定与客户端连接自动发现：





```
#region 窗体事件
        private void frmServer_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                ser.Stop();
            }
            catch
            {
            }
            try
            {
                rtServer.Dispose();
            }
            catch
            {
            }
        }
        private void btnStart_Click(object sender, EventArgs e)
        {
            var vc = lstTCPChannel[lstBoxTcp.SelectedIndex];

            //rtServer.AddClient(vc.SimKey, "rtmp://localhost:32768/rtmplive/S" + vc.SimKey);
            rtServer.AddClient(vc.SimKey, "rtmp://172.16.20.10:1935/live/S" + vc.SimKey);// 添加直播客户端---RTMP推流服务器地址和流名称
            vc.SendVideoCtrl(new JTRealVideoTransferRequest
            {
                IPAddress = "127.0.0.1",
                UdpPort = 9700,
                Channel = 0,
                DataType = 1,
                StreamType =  StreamType.SubStream
            });
        }

        private void frmServer_Load(object sender, EventArgs e)
        {
            rtServer = new RTPServer(9700, pictureBox1);//RTP服务端图像传输对象
            rtServer.Start();
            ser.ChannelConnect += new Network.EventChannelConnect(ser_ChannelConnect);// 通道连接事件
            ser.ChannelDispose += new Network.EventChannelDispose(ser_ChannelDispose);// 通道连接断开事件
            ser.Start();

            Network.Utils.Setup(10);
        }
        #endregion
```

服务端窗体客户端连接列表控制：




```
#region 列表控制
        VideoClient AddToList(Channel channel)
        {
            var item = new VideoClient(channel, splitData);
            channel.Tag = item;
            lstTCPChannel.Add(item);
            //rtServer.AddClient(item.SimKey, "rtmp://localhost:32768/rtmplive/S" + item.SimKey);
            rtServer.AddClient(item.SimKey, "rtmp://172.16.20.10:1935/live/S" + item.SimKey);// 添加直播客户端---RTMP推流服务器地址和流名称
            lstBoxTcp.BeginInvoke(new MethodInvoker(() =>
            {
                lstBoxTcp.Items.Add(channel.RemoteHost + ":" + channel.RemotePort);
            }));

            return item;
        }

        void RemoveList(Channel channel)
        {
            var item = (VideoClient)channel.Tag;
            lstTCPChannel.Remove(item);
            rtServer.RemoveClient(item.SimKey);
            lstBoxTcp.Invoke(new MethodInvoker(() =>
            {
                lstBoxTcp.Items.Remove(channel.RemoteHost + ":" + channel.RemotePort);
            }));
        }
        #endregion
```
服务端窗体通信链路事件方法：




```
#region 链路相关
        void ser_ChannelDispose(object sender, Network.ChannelDisposeArg arg)
        {
            RemoveList(arg.Channel);
        }

        void ser_ChannelConnect(object sender, Network.ChannelConnectArg arg)
        {
            var item = AddToList(arg.Channel);
            arg.Channel.DataReceive = item.Receive;
            arg.Channel.DataSend = item.DataSend;
            arg.Channel.StartReceiveAsync();
        }
        #endregion
```


大致代码的思路都在此了。

## 需要思考的问题

想达到这样的目的：视频能够一边下载一边播放。如何编程实现服务器端的视频流信息向客户端的发送？ 客服端又如何接收并播放视频？

使用System.Net的NetStream和使用System.IO的FileStream、MediaPlayer播放插件。

（1）在服务端建立一个Socket服务，将文件分段放入缓冲区。
（2）在客户端建立一个Socket客户端，读取服务端的缓冲区内容。
（3）将读到的部分发送给MediaPlayer进行播放。

![](https://img-blog.csdn.net/20171221173851125)


上面的客户端和服务端的情况就是按照这种思路来写的，后续需要加入视频预览、播放的功能。实际上就是将H264流数据转成YUV/RGB->Bitmap前端进行展示，然后加入音频数据传输的线程等进行处理同步播放。

![](https://img-blog.csdn.net/20171222112056790)


上图是服务器端获取到的H264->YUV播放示例。






