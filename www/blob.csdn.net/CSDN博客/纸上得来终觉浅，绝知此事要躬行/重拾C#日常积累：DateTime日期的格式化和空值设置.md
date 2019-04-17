# 重拾C#日常积累：DateTime日期的格式化和空值设置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月13日 15:32:16[boonya](https://me.csdn.net/boonya)阅读数：652











C#时间格式化不同于Java，其格式化步骤偏复杂，[C#DateTime API](http://www1.cs.columbia.edu/~lok/csharp/refdocs/System/types/DateTime.html)。



 DateTime取空值类似三元表达式的用法：为了实现Nullable数据类型转换成non-Nullable型数据，就有了一个这样的操作符”？？（两个问号）“，双问号操作符意思是取所赋值??左边的，如果左边为null，取所赋值??右边的.

比如int y = x ?? -1 如果x为空，那么y的值为-1.

**示例代码如下：**

```
/// <summary>
        /// 平台下发远程录像回放请求(808协议:0x9201):此处成功与否仅表示指令是否成功发出
        /// </summary>
        /// <param name="IPAddress">服务器IP地址</param>
        /// <param name="TcpPort">服务器视频通道监听端口号(TCP)（不使用TCP传输时置0）</param>
        /// <param name="UdpPort">服务器视频通道监听端口号(UDP)（不使用UDP传输时置0）</param>
        /// <param name="Channel">逻辑通道号</param>
        /// <param name="MediaType">音视频类型（0：音视频，1：音频，2：视频，3：视频或音视频）</param>
        /// <param name="StreamType">码流类型（0：主码流或子码流，1：主码流，2：子码流；如果此通道只传输音频，此字段置0）</param>
        /// <param name="StorageType">存储器类型（0：主存储器或灾备存储器，1：主存储器，2：灾备存储器）</param>
        /// <param name="PlaybackMode">回放方式（0：正常回放，1：快进回放，2：关键帧快退回放，3：关键帧播放，4：单帧上传）</param>
        /// <param name="Multiple">快进或快退倍数（0：无效，1：1倍，2：2倍，3：4倍，4：8倍，5：16倍）</param>
        /// <param name="StartTime">开始时间（YYYY-MM-DD HH:MM:SS，回放方式为4时，该字段表示单帧上传时间）</param>
        /// <param name="EndTime">结束时间（YYYY-MM-DD HH:MM:SS，为0表示一直码回放，回放方式为4时，该字段无效）</param>
        /// <param name="IsAlwaysPlayback">是否一直回放</param>
        /// <returns>指令下发状态是否成功（bool）,而非设备响应的结果是否成功，设备响应的结果从Redis中取到后异步发给ocx通过事件的方式响应前端页面</returns>
        public bool SendOrderToRequestPlaybackRemoteVideos(string IPAddress, int TcpPort, int UdpPort, byte Channel, byte MediaType, byte StreamType, byte StorageType, byte PlaybackMode, byte Multiple, string StartTime, string EndTime, bool IsAlwaysPlayback)
        {
            if (cheji==null||string.IsNullOrEmpty(IPAddress)|| string.IsNullOrEmpty(StartTime) || string.IsNullOrEmpty(EndTime))
            {
                return false;
            }
            DateTime? StartTime_=null, EndTime_=null;
            try
            {
                StartTime= Convert.ToDateTime(StartTime).ToString("yy-MM-dd-HH-mm-ss");
                StartTime_= DateTime.ParseExact(StartTime, "yy-MM-dd-HH-mm-ss", System.Globalization.CultureInfo.CurrentCulture);

                if (!StartTime.Equals("0"))
                {
                    EndTime = Convert.ToDateTime(EndTime).ToString("yy-MM-dd-HH-mm-ss");
                    EndTime_ = DateTime.ParseExact(EndTime, "yy-MM-dd-HH-mm-ss", System.Globalization.CultureInfo.CurrentCulture);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("DateTime转换错误，请检查格式是否满足DateTime的要求:" + ex.Message);
                return false;
            }
            return commander.Send0x9201( IPAddress,  (UInt16)TcpPort, (UInt16)UdpPort,  Channel,  (AudioVideoFlag)MediaType,  (StreamType)StreamType,  (MemoryType)StorageType,  PlaybackMode,  Multiple, (DateTime)StartTime_,(DateTime)EndTime_,  IsAlwaysPlayback);
        }
```




