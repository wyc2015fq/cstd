# 使用FFmpeg命令行进行UDP、RTP推流（H264、TS），ffplay接收 - 爱写代码的自由人 - CSDN博客





2019年01月31日 10:43:56[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：867








我们在开发网络程序时经常用到UDP或RTP来发送和接收流媒体，而开发程序完毕需要搭建一个环境测试，这时候可能你需要一个推流端或接收端。对于推流端，我们可以借助FFmpeg工具轻松完成该功能，只需要敲一条命令后就可以实现发流，并且支持多种网络协议（UDP/RTP/RTSP/RTMP）。而接收端我们可以使用ffplay，这个程序也是在FFmpeg工具包的Bin目录里面。大家可以根据自己需要使用这两个工具进行推流或接收，下面就以传输协议UDP、RTP为基础，介绍几种最常见的推流场景下两个工具的用法。

**1. 使用RTP发送H264**

FFmpeg的推流命令：
`ffmpeg -re -i d:\videos\1080P.264 -vcodec copy -f rtp rtp://127.0.0.1:1234`
该命令行实现读取一个H264文件，以源文件固有帧率发送数据（加上-re参数），输出流协议为rtp。

ffplay接收端的命令：
`ffplay -protocol_whitelist "file,udp,rtp" -i rtp://127.0.0.1:1234`
**注意：ffplay的RTP协议默认是不启用的，需要添加-protocol_whitelist参数把RTP协议加入白名单。但是，执行了上面的命令可能会出错，因为ffplay使用RTP协议接收需要一个SDP文件，该文件定义了输入流的格式信息以及接收端的IP和端口号。**

我们可以让FFmpeg在执行推流输出时导出一个SDP，命令行如下：
`ffmpeg -re -i d:\videos\1080P.264 -vcodec copy -f rtp rtp://127.0.0.1:1234>test_rtp_h264.sdp`
下面是一个SDP文件的例子：

```
SDP:
v=0
o=- 0 0 IN IP4 127.0.0.1
s=No Name
c=IN IP4 127.0.0.1
t=0 0
a=tool:libavformat 56.15.102
m=video 1234 RTP/AVP 96
a=rtpmap:96 H264/90000
```

（SDP文件的信息说明：传输协议：RTP/AVP， 接收端IP：127.0.0.1，  接收端口号：1234 ，  PayloadType：96 ，视频格式：H264 ）

以上面SDP文件为输入让ffplay打开流，命令行是：
`ffplay -protocol_whitelist "file,udp,rtp" -i test_rtp_h264.sdp`
**2. 使用UDP发送TS流**

FFmpeg的推流命令：
`ffmpeg -re -i d:\videos\1080P.264 -vcodec copy -f mpegts udp://127.0.0.1:1234`
ffplay的接收命令：
`ffplay -protocol_whitelist "file,udp,rtp" -i udp://127.0.0.1:1234`
**3. 使用RTP发送TS流**

很多人以为这种情况跟上面差不多，使用如下的推流命令（错误）：
`ffmpeg -re -i d:\videos\1080P.264 -vcodec copy -f mpegts rtp://127.0.0.1:1234`
但是实际上不行，我开始也使用该条命令，试过好多次老是失败，失败的原因是：发送的包缺少RTP头，输出的协议有错误。实际上，正确的命令是：
`ffmpeg -re -i d:\videos\1080P.264 -vcodec copy -f rtp_mpegts rtp://127.0.0.1:1234`
ffplay接收的命令：
`ffplay -protocol_whitelist "file,udp,rtp" -i rtp://127.0.0.1:1234`
或者打开一个SDP文件，SDP文件对TS流的写法：

```
SDP:
v=0
o=- 0 0 IN IP4 127.0.0.1
s=No Name
c=IN IP4 127.0.0.1
t=0 0
m=video 1234 RTP/AVP 33
a=rtpmap:33 MP2T/90000
```

（注意：FFmpeg发送TS流的RTP PayloadType不是96而是33）

还有一个问题：**早期的ffmpeg版本是不支持rtp_mpegts输出协议的，你可以使用命令ffmpeg -formats查看ffmpeg是否支持这种输出协议。**我使用FFmpeg的版本是ffmpeg-20180209是支持这种协议的。

**4. 使用RTP分别发送音频流和视频流**

FFmpeg命令：
`ffmpeg  -re -i <media_file> -an -vcodec copy -f rtp rtp://<IP>:5004 -vn -acodec copy -f rtp rtp://<IP>:5005 > test.sdp`
FFplay接收的SDP文件：

```
SDP:
v=2 
m=video 5004 RTP/AVP 96
a=rtpmap:96 H264
t=0 0 
a=framerate:25
c=IN IP4 192.168.0.100


m=audio 5005 RTP/AVP 97
a=rtpmap:97 PCM/8000/1
a=framerate:25
c=IN IP4 192.168.0.100
```





