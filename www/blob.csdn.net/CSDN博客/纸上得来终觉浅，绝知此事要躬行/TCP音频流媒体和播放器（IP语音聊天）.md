# TCP音频流媒体和播放器（IP语音聊天） - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年12月26日 17:42:33[boonya](https://me.csdn.net/boonya)阅读数：1076








原文地址：[https://www.codeproject.com/articles/482735/tcp-audio-streamer-and-player-voip](https://www.codeproject.com/articles/482735/tcp-audio-streamer-and-player-voip)



[Download source](https://www.codeproject.com/KB/audio-video/482735/Source_6.zip)
[Download Exe_6.zip](https://www.codeproject.com/KB/audio-video/482735/Exe_6.zip)


## 介绍




这是通过TCP发送和接收音频数据的专有VoIP项目。这是我的第一篇文章[播放或捕获音频声音的延伸作为组播发送和接收（RTP）](http://www.codeproject.com/Articles/394890/Play-or-Capture-Audio-Sound-Send-and-Receive-as-Mu)。此应用程序不是通过多播而是通过TCP来传输音频数据。所以你可以确定没有数据丢失，你可以通过子网和路由器转移它们。音频编解码器是U-Law。采样率可以从5000到44100选择。


服务器可以在您的本地PC上运行。您可以在运行cmd.exe的帮助下输入“ipconfig”来获取当前的IP4地址。您应该使用静态IP地址，以便可能的客户端在几天后重新连接后不必更改其设置。客户端必须连接到正在运行的服务器上配置的IP4地址和端口。服务器可以静音模式运行（无输入，无输出），只需在所有连接的客户端之间传输音频数据。


选择另一个应用程序未使用的空闲端口（不要使用80或其他保留端口）。您可以连接局域网或互联网。对于Internet聊天，您可以在路由器上配置端口转发。


**注意 ！！！**这是一个专有的项目。您不能将我的服务器或客户端用于任何其他标准化的服务器或客户端。我不使用像RTCP或SDP这样的标准。





## 背景

由于网络流量和时钟差异，您必须使用抖动缓冲器来补偿数据传输。 您可以为每个服务器设置抖动缓冲区，所有客户端将使用相同的金额。 一个Jitter-Buffer表示一个包含在TCP-Stream中的数据包。 当抖动缓冲器达到最大值的一半时，服务器开始播放。 您可以在为每个客户端显示的进度条中观看此视频。 您设置的抖动缓冲区越多，延迟时间就越多。 您可以运行TCPStreamer作为客户端或服务器。 一台服务器可以处理一个或多个客户端。


**TCPStreamer作为客户端**


![](https://img-blog.csdn.net/20171226174349069)


作为客户端运行，您可以连接到服务器实例。 选择你的麦克风，并听设备。 点击麦克风或扬声器按钮静音。 客户端连接后，扬声器组合框更改为显示传入数据值的进度条。 使用的SamplesPerSecond（Quality）取决于服务器配置。 抖动缓冲器客户端仅对传入数据的延迟很重要。





**TCPStreamer作为服务器**


![](https://img-blog.csdn.net/20171226174532886)


作为服务器运行，您可以等待一个或多个客户端。 如果需要，可以选择你的麦克风和听设备，但是你可以运行服务器而不需要听到或说服务器端，这样只有客户端才能相互通话。 每个客户都可以静音（扬声器和微型）。 IP地址必须是您的计算机的地址。 其他应用程序不能使用端口号。 抖动缓冲器值服务器端对于所有连接的客户端的延迟是重要的。 尽可能使用最低值。 服务器必须混合来自所有客户端的所有数据，所以您应该选择运行服务器的高性能工作站。 讲话的质量取决于SamplesPerSecond值。


## 使用代码

这里有以下程序集：



- TCPStreamer.exe (主应用)
- TCPClient.dll (TCP客户端包装助手)
- TCPServer.dll (TCP服务端包装助手)
- WinSound.dll (录音和播放)




我可以直接从声卡发送数据到网络。 但是我决定首先将它们放入抖动缓冲区，因为有些声音设备（特别是在笔记本电脑上）无法在相同的时间周期内获取声音数据。 使用抖动缓冲器，我确保每20毫秒发送一次数据。 但缺点是更大的延迟。 该不可配置的抖动缓冲区的数量是8，对于较低的延迟，可以减少源代码（RecordingJitterBufferCount）中的值。 但是看看你的声卡有几分钟，如果能处理的话。



//------------------------------------------------------------------------------------------////Recording datas from Soundcard and put into Jitter Buffer////------------------------------------------------------------------------------------------privatevoid OnDataReceivedFromSoundcard_Server(Byte[] data)
{
    //Split datas in smaller equal piecesint bytesPerInterval = WinSound.Utils.GetBytesPerInterval((uint)
      m_Config.SamplesPerSecondServer, 
            m_Config.BitsPerSampleServer, m_Config.ChannelsServer);
    int count = data.Length / bytesPerInterval;
    int currentPos = 0;
    for (int i = 0; i < count; i++)
    {
        //Cast to RTP packet (Linear to U-Law)
        Byte[] partBytes = new Byte[bytesPerInterval];
        Array.Copy(data, currentPos, partBytes, 0, bytesPerInterval);
        currentPos += bytesPerInterval;
        WinSound.RTPPacket rtp = ToRTPPacket(partBytes, 
           m_Config.BitsPerSampleServer, m_Config.ChannelsServer);

        //Put RTP packet into Jitter Buffer
        m_JitterBufferServerRecording.AddData(rtp);
    }
}  在创建RTP包时，大多数信息（如CSRC Count或Version）都是相同的。 每发送一次RTP包之后，我只需要增加SequenceNumber和Timestamp。 在此之前，我将线性数据转换为压缩的U-Law格式以避免网络流量。




//------------------------------------------------------------------------------------------////Creating a RTP packet from linear data////------------------------------------------------------------------------------------------private WinSound.RTPPacket ToRTPPacket(Byte[] linearData, int bitsPerSample, int channels)
{
    //Convert linear to Mulaw
    Byte[] mulaws = WinSound.Utils.LinearToMulaw(linearData, bitsPerSample, channels);

    //Create new RTP Packet
    WinSound.RTPPacket rtp = new WinSound.RTPPacket();

    //Init base values
    rtp.Data = mulaws;
    rtp.CSRCCount = m_CSRCCount;
    rtp.Extension = m_Extension;
    rtp.HeaderLength = WinSound.RTPPacket.MinHeaderLength;
    rtp.Marker = m_Marker;
    rtp.Padding = m_Padding;
    rtp.PayloadType = m_PayloadType;
    rtp.Version = m_Version;
    rtp.SourceId = m_SourceId;

    //Update RTP header with SequenceNumber and Timestamptry
    {
        rtp.SequenceNumber = Convert.ToUInt16(m_SequenceNumber);
        m_SequenceNumber++;
    }
    catch (Exception)
    {
        m_SequenceNumber = 0;
    }
    try
    {
        rtp.Timestamp = Convert.ToUInt32(m_TimeStamp);
        m_TimeStamp += mulaws.Length;
    }
    catch (Exception)
    {
        m_TimeStamp = 0;
    }

    //Readyreturn rtp;
}  
//------------------------------------------------------------------------------------------////Send sound datas (U-Law) over network////------------------------------------------------------------------------------------------privatevoid OnJitterBufferServerDataAvailable(Object sender, WinSound.RTPPacket rtp)
{
  //Convert RTP packt to bytes
  Byte[] rtpBytes = rtp.ToBytes();

  //For all clients connected
  List<NF.ServerThread> list = new List<NF.ServerThread>(m_Server.Clients);
  foreach (NF.ServerThread client in list)
  {
      //If not muteif (client.IsMute == false)
      {
        //Send
        client.Send(m_PrototolClient.ToBytes(rtpBytes));
      }
  }             
}为了通过TCP发送和接收数据，我使用一个简单的专有协议。 在每个数据块之前，我写一个32位的数据长度信息。 所以后来，当我收到数据流时，我知道如何解释数据。



![](https://img-blog.csdn.net/20171226175220809)



//------------------------------------------------------------------------------------------////Convert bytes to a proprietary protocol format////------------------------------------------------------------------------------------------public Byte[] ToBytes(Byte[] data)
{
   //Get length of the data block
   Byte[] bytesLength = BitConverter.GetBytes(data.Length);

   //Copy all together
   Byte[] allBytes = new Byte[bytesLength.Length + data.Length];
   Array.Copy(bytesLength, allBytes, bytesLength.Length);
   Array.Copy(data, 0, allBytes, bytesLength.Length, data.Length);

   //readyreturn allBytes;
} 在这种情况下，反向路径是通过网络为每个连接的客户端获取数据。 在第一步中，我必须借助自己的协议从整个流中提取数据包。




//------------------------------------------------------------------------------------------////Get datas over network////------------------------------------------------------------------------------------------privatevoid OnServerDataReceived(NF.ServerThread st, Byte[] data)
{
    //If client existingif (m_DictionaryServerDatas.ContainsKey(st))
    {
        //Get protocol 
        ServerThreadData stData = m_DictionaryServerDatas[st];
        if (stData.Protocol != null)
        {
       //Dispatch data over protocol    
           stData.Protocol.Receive_LH(st, data);
        }
    }
} 借助于长度信息，我知道一个数据包何时开始和结束。




//------------------------------------------------------------------------------------------////Get RTP datas with help of a proprietary protocol ////------------------------------------------------------------------------------------------publicvoid Receive_LH(Object sender, Byte[] data)
{
   //Add datas to buffer
   m_DataBuffer.AddRange(data);

   //Check buffer overflowif (m_DataBuffer.Count > m_MaxBufferLength)
   {
       m_DataBuffer.Clear();
   }

   //Get the length of received datas (16 Bit value)
   Byte[] bytes = m_DataBuffer.Take(4).ToArray();
   int length = (int)BitConverter.ToInt32(bytes.ToArray(), 0);

   //Check maximum lengthif (length > m_MaxBufferLength)
   {
       m_DataBuffer.Clear();
   }

   //For each complete data packet (check by the length)while (m_DataBuffer.Count >= length + 4)
   {
       //Get data
       Byte[] message = m_DataBuffer.Skip(4).Take(length).ToArray();

       //Raise eventif (DataComplete != null)
       {
           DataComplete(sender, message);
       }
       //Remove handled datas from buffer
       m_DataBuffer.RemoveRange(0, length + 4);

       //As long as complete datas are availableif (m_DataBuffer.Count >4)
       {
           //Get next length
           bytes = m_DataBuffer.Take(4).ToArray();
           length = (int)BitConverter.ToInt32(bytes.ToArray(), 0);
       }
   } 
} 在播放数据到声卡之前，我把它们放到了另一个抖动缓冲器中。 这是非常必要的，因为网络流量不规则，尤其是互联网。 抖动缓冲量越多，延迟越多。




//------------------------------------------------------------------------------------------////Put network datas into Jitter Buffer////------------------------------------------------------------------------------------------privatevoid OnProtocolDataComplete(Object sender, Byte[] bytes)
{
   //Convert bytes to RTP packet
   WinSound.RTPPacket rtp = new WinSound.RTPPacket(bytes);
  
   //When RTP packet correctif (rtp.Data != null)
   {
     //Add RTP packet to Jitter Buffer    
     JitterBuffer.AddData(rtp); 
   }          
} 最后，数据准备好播放到声卡上。 在此之前，我将U-Law数据转换回线性数据，因为声音设备只能播放线性数据。




//------------------------------------------------------------------------------------------////Play datas to soundcard////------------------------------------------------------------------------------------------privatevoid OnJitterBufferDataAvailable(Object sender, WinSound.RTPPacket rtp)
{
   //If not mutedif (IsMuteAll == false && IsMute == false)
   {
       //Convert U-Law to linear
       Byte[] linearBytes = WinSound.Utils.MuLawToLinear(rtp.Data, BitsPerSample, Channels);
       //Play to soundcard
       Player.PlayData(linearBytes, false);
   }
} 我实现了自己的抖动缓冲区作为RTP包的队列。 可以添加数据，然后通过高频率定时器功能（20 ms）进行处理。




//------------------------------------------------------------------------------------------//// Adding datas to Jitter Buffer////------------------------------------------------------------------------------------------publicvoid AddData(RTPPacket packet)
{
    //Check overflowif (m_Overflow == false)
    {
        //Check maximum sizeif (m_Buffer.Count <= m_MaxRTPPackets)
        {
        //Adding data
            m_Buffer.Enqueue(packet);
        }
        else
        {
            //Overflow
            m_Overflow = true;
        }
    }
} 抖动缓冲区每20毫秒处理一次数据。 为了得到这样一个确切的计时器，你不能使用正常的.NET定时器。 所以我使用了Win32 kernel32和Winmm库中的定时器函数。 在启动定时器之前，我将精度设置为系统可以提供的最佳值。 这可以从1到更多的毫秒不等。 Windows中不可能超过1毫秒。




[DllImport("Kernel32.dll", EntryPoint = "QueryPerformanceCounter")]publicstaticexternbool QueryPerformanceCounter(outlong lpPerformanceCount);

[DllImport("Kernel32.dll", EntryPoint = "QueryPerformanceFrequency")]
publicstaticexternbool QueryPerformanceFrequency(outlong lpFrequency);

[DllImport("winmm.dll", SetLastError = true, EntryPoint = "timeSetEvent")]
publicstaticexternUInt32 TimeSetEvent(UInt32 msDelay, UInt32 msResolution, 
       TimerEventHandler handler, refUInt32 userCtx, UInt32 eventType);

[DllImport("winmm.dll", SetLastError = true, EntryPoint = "timeKillEvent")]
publicstaticexternUInt32 TimeKillEvent(UInt32 timerId);

[DllImport("kernel32.dll", EntryPoint = "CreateTimerQueue")]
publicstaticexternIntPtr CreateTimerQueue();

[DllImport("kernel32.dll", EntryPoint = "DeleteTimerQueue")]
publicstaticexternbool DeleteTimerQueue(IntPtr TimerQueue);

[DllImport("kernel32.dll", EntryPoint = "CreateTimerQueueTimer")]
publicstaticexternbool CreateTimerQueueTimer(outIntPtr phNewTimer, IntPtr TimerQueue, 
  DelegateTimerProc Callback, IntPtr Parameter, uint DueTime, uint Period, uint Flags);

[DllImport("kernel32.dll")]
publicstaticexternbool DeleteTimerQueueTimer(IntPtr TimerQueue, 
              IntPtr Timer, IntPtr CompletionEvent);

[DllImport("winmm.dll", SetLastError = true, EntryPoint = "timeGetDevCaps")]
publicstaticextern MMRESULT TimeGetDevCaps(ref TimeCaps timeCaps, UInt32 sizeTimeCaps);

[DllImport("winmm.dll", SetLastError = true, EntryPoint = "timeBeginPeriod")]
publicstaticextern MMRESULT TimeBeginPeriod(UInt32 uPeriod);

[DllImport("winmm.dll", SetLastError = true, EntryPoint = "timeEndPeriod")]
publicstaticextern MMRESULT TimeEndPeriod(UInt32 uPeriod);抖动缓冲器设计用于在达到最大值的一半时处理数据。 溢出或下溢之后，缓冲区将尝试返回到此值。




//------------------------------------------------------------------------------------------//// Jitter Buffer Timer main function////------------------------------------------------------------------------------------------privatevoid OnTimerTick()
{
  if (DataAvailable != null)
  {
      //When datas existingif (m_Buffer.Count >0)
      {
          //When overflowif (m_Overflow)
          {
              //Wait until buffer is half of maximumif (m_Buffer.Count <= m_MaxRTPPackets / 2)
              {
                  m_Overflow = false;
              }
          }

          //When underflowif (m_Underflow)
          {
              //Wait until buffer is half of maximumif (m_Buffer.Count < m_MaxRTPPackets / 2)
              {
                  return;
              }
              else
              {
                  m_Underflow = false;
              }
          }

          //Get data and raise event
          m_LastRTPPacket = m_Buffer.Dequeue();
          DataAvailable(m_Sender, m_LastRTPPacket);
      }
      else
      {
          //No overflow
          m_Overflow = false;

          //Whenn buffer is emptyif (m_LastRTPPacket != null && m_Underflow == false)
          {
              if (m_LastRTPPacket.Data != null)
              {
                  //Underflow 
                  m_Underflow = true;
              }
          }
      }
  } 
}这个项目不使用Overheaded库或扩展，所以它可以用来学习操作声音数据和网络操作的基础知识。 随意扩展和改善您的需求。



## 历史


- 31.05.2012 - Added
- 03.05.2013 - Added duplex connections. Removed File-Player
- 09.05.2013 - Changed tip to article
- 12.12.2013 - Added Communication between all clients
- 18.12.2013 - Fixed some bugs
- 22.04.2014 - Updated. Solved possible stability problems


## 许可

本文以及任何关联的源代码和文件均在[The Code Project Open License (CPOL)](http://www.codeproject.com/info/cpol10.aspx)许可下。



==更多：为防止代码不可下载已上传一份至百度云盘[https://pan.baidu.com/s/1jIrKxwE](https://pan.baidu.com/s/1jIrKxwE)



