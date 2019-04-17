# 用vlc搭建简单流媒体服务器（UDP和TCP方式） - DoubleLi - 博客园







这段时间用到了流媒体数据传输的TCP和UDP方式，感觉vlc可以做这方面的demo，这里总结下，也方便我以后查阅。

# 简介

VLC主页：[http://www.videolan.org](http://www.videolan.org/)

VLC不仅仅是个播放器，也可以作为流媒体服务器使用。这个网上有相关的资料，我就不多说了。

声明下本文用的VLC版本：2.0.3

# 用VLC搭建基于UDP的流媒体服务器

流媒体服务器大多数是基于UDP的，这个在VLC中也有好几种实现，我这里只列出我用到的几个。

## 1、UDP（legacy）传统模式

**服务端操作**

打开“媒体——流”选项，如图所示：

![](https://pic002.cnblogs.com/images/2012/300959/2012090912022114.png)

点击“添加”按钮选择流媒体文件，然后点击“串流”按钮

![](https://pic002.cnblogs.com/images/2012/300959/2012090912030164.png)

点击“下一个”后，在“目标设置”界面选择“UDP (legacy)”，然后点击添加按钮

![](https://pic002.cnblogs.com/images/2012/300959/2012090912035189.png)

填上目标ip地址，并在“转码选项”中选择相应的编码类型（这个视情况而定）

![](https://pic002.cnblogs.com/images/2012/300959/2012090912042363.png)

可以点击“下一个”进行相关设置，也可以点击“串流”来完成服务端的设定。这里我点击“串流”完成服务端的设定。

**客户端操作**

点击“媒体——打开网络串流”，在弹出的框中输入“udp:[//@1234](mailto://1234)”，点击“播放”按钮即可。

![](https://pic002.cnblogs.com/images/2012/300959/2012090912045710.png)

通过界面操作比较简单，但如果经常进行此类工作的话，脚本是比较靠谱的。

**命令行操作方式**

服务端（172.16.1.1）：

```
vlc -vvv sample1.avi --sout udp:172.16.1.100:1234 --ttl 10
```

客户端（172.16.1.100）：

```
vlc udp://@:1234
```

## 2、RTP方式

虽然有基于TCP的RTP，但抓包发现，VLC的RTP是基于UDP的。



![](https://pic002.cnblogs.com/images/2012/300959/2012090912060951.png)

**服务端操作**

前几步和“UDP（legacy）传统模式”类似，这里不再赘述。主要是在目标里面选择“RTP / MPEG Transport Stream”这个选项，如图所示：

![](https://pic002.cnblogs.com/images/2012/300959/2012090912063327.png)

填IP，选端口：

![](https://pic002.cnblogs.com/images/2012/300959/2012090912070221.png)

点击“串流”按钮即可完成服务端操作。

**客户端操作**

![](https://pic002.cnblogs.com/images/2012/300959/2012090912072795.png)

点击“播放”按钮即可。

**命令行操作方式**

服务端（172.16.1.1）：

```
vlc -vvv sample1.avi --sout "#transcode{vcodec=h264,vb=0,scale=0,acodec=mpga,ab=128,channels=2,samplerate=44100}:rtp{dst=172.16.1.100,port=5004,mux=ts,ttl=10}"
```

客户端（172.16.1.100）：

```
vlc rtp://@:5004
```

## 3、RTSP方式

RTSP方式是通过RTP进行流媒体数据的传输的，VLC的实现也是基于UDP的。这种方式网上的参考资料比较多，我就不截图了，直接叙述命令行方式的操作。

**命令行操作方式**

服务端（172.16.1.1）：

```
vlc -vvv sample1.avi --sout "#transcode{vcodec=h264,vb=0,scale=0,acodec=mpga,ab=128,channels=2,samplerate=44100}:rtp{sdp=rtsp://:8554/test}"
```

客户端（172.16.1.100）：

```
vlc rtsp://172.16.1.1:8554/test
```

# 用VLC搭建基于TCP的流媒体服务器

VLC的http方式搭建的流媒体服务器是用TCP方式进行数据传输的，这里就以这个为例子了。

## HTTP方式

这个的界面操作和RTSP的类似，就不赘述。

**命令行操作方式**

服务端（172.16.1.1）：

```
vlc -vvv sample1.avi --sout "#transcode{vcodec=h264,vb=0,scale=0,acodec=mpga,ab=128,channels=2,samplerate=44100}:http{mux=ffmpeg{mux=flv},dst=:8080/test}"
```

客户端（172.16.1.100）：

```
vlc http://172.16.1.1:8080/test
```

 好，就这些了，希望对你有帮助。


- E-Mail : [Mike_Zhang@live.com](mailto:Mike_Zhang@live.com)
- github ：[https://github.com/mike-zhang](https://github.com/mike-zhang)
- QQ群 : 550747085










