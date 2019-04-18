# VLC接收RTP流的SDP文件写法 - 爱写代码的自由人 - CSDN博客





2019年01月31日 11:28:28[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：142








接收RTP流我们经常用VLC来进行测试，但是对于RTP，如果直接在VLC的网络协议框输入RTP地址来播放是播放不了的，如下图，会提示"SDP required"的错误信息。

![](https://img-blog.csdnimg.cn/20190131110411238.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pob3Vib3RvbmcyMDEy,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190131113529569.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pob3Vib3RvbmcyMDEy,size_16,color_FFFFFF,t_70)

因为VLC打开RTP流需要一个SDP文件，SDP文件定义了流的一些信息比如视频编码格式，帧率，接收端的IP、端口号等。关于SDP文件的格式网上有详细的介绍，这里就不作详细说明了。反正，打开一个流，播放器需要根据SDP文件描述的信息知道如何播放输入源。

在使用RTP传输的场合，我们封装的流媒体数据一般是H264，PS，TS，而这几种格式的SDP文件的写法都是不同的。

1. 输入流是H264的SDP文件例子：

```
v=0
m=video 1234 RTP/AVP 96
a=rtpmap:96 H264/90000
c=IN IP4 127.0.0.1
```

2. 输入流是PS的SDP文件例子：

```
s=MPEG Program Stream, streamed by XXXX  
m=video 1234 RTP/AVP 96  
a=rtpmap:96 MP2P/90000
c=IN IP4 127.0.0.1
```

3.输入流是TS的SDP文件例子：

```
s=MPEG Transport Stream, streamed by XXXX  
m=video 1234 RTP/AVP 96  
a=rtpmap:96 MP2T/90000
c=IN IP4 127.0.0.1
```

参数说明：

s=参数：对流名称的说明，可以省略

m=参数后跟的信息：video <接收端口号>  RTP/AVP  <RTP负载类型>

a=参数跟的信息：rtpmap: <RTP负载类型> <数据的封装格式>/<时间戳的时钟频率>

c=参数跟的信息：IN  IP4 <接收端IP地址>





