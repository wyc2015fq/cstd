# C# Dictionary多线程安全访问问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月01日 17:12:49[boonya](https://me.csdn.net/boonya)阅读数：1844








Dictionary是非线程安全的类型，操作的时候需要对其进行线程安全处理，最简单的方式就是加锁(lock)。

数据变量：

`private static Dictionary<string, VirtualVideoChannel> m_list_video_channel_all = new Dictionary<string, VirtualVideoChannel>();`

加锁代码：


```
/// <summary>
        /// 创建设备通道连接
        /// </summary>
        /// <param name="sim"></param>
        private void CreateNewChannelConnection(string sim)
        {
            var m_tcp_order = new Network.TCPChannel(txtServer.Text.Trim(), Convert.ToInt32(numPort.Value));
            // 设置Tag为SIM卡号
            m_tcp_order.Tag = sim;
            m_tcp_order.DataReceive = Receive;
            m_tcp_order.DataSend = DataSend;
            m_tcp_order.ChannelConnect += channel_ChannelConnect;
            m_tcp_order.Connect();
            // 映射SIM号和连接对象
            lock (m_sim_dictionary)
            {
                if (m_sim_dictionary.ContainsKey(sim))
                {
                    TCPChannel tcpChannel;
                    m_sim_dictionary.TryGetValue(sim, out tcpChannel);
                    if (tcpChannel != null)
                    {
                        tcpChannel.Close();
                        m_sim_dictionary.Remove(sim);
                    }
                }
                m_sim_dictionary.Add(sim, m_tcp_order);
            }
        }
```
同样的，List也不是线程安全的，同样都需要如此处理，当然可以换成别的方式来做。



