# H264 NALU 使用PS封装 RTP发送 - DoubleLi - 博客园






最近由于项目平台需求，要将H264 NALU封装为PS再用RTP发送，PS封装按照ISO DEC-13818-1标准。一个PS包包含PS Header, PES Header, PS system header, PS system map等。


针对H264做如下PS封装：

1、每个IDR NALU前一般都会包含SPS、PPS等NALU，因此将SPS、PPS、IDR 的NALU封装为一个PS包，包括ps头，然后加上PS system header，PS system map，PES header+h264 raw data。所以一个IDR NALU PS包由外到内顺序是：PS header | PS system header | PS system Map | PES header |  h264 raw data。

2、对于其它非关键帧的PS包，就简单多了，直接加上PS头和PES头就可以了。顺序为：PS header | PES header | h264 raw data。

还有就是用rtp协议发送时，如果一个PS包大于MTU时，会有分包问题。只需要简单的分包发送就可以了，但是同一个PS包的时间戳是相同的，需要注意。



以上是对只有视频video的情况，如果要把音频Audio也打包进PS封装，也可以。当有音频数据时，将数据加上PES header 放到视频PES后就可以了。顺序如下：

PS包=PS头|PES(video)|PES(audio)，再用RTP封装发送就可以了。



以上是H264 PS封装的大致流程。PS一些头的定义，在我的下载中，有需要的可以参考。

或者，加QQ群（219128816）一起探讨流媒体方面的开发。










