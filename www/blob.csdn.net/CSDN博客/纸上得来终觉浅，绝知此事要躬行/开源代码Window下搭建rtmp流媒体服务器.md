# 开源代码Window下搭建rtmp流媒体服务器 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月22日 15:42:30[boonya](https://me.csdn.net/boonya)阅读数：10647








有这样需求，将摄像头视屏发布出去，并且需要在web网页上通过Flash播放器播放。首先Flash被我所知道的所有浏览器所支持。如果是Activex控件。还需要被用户额外安装，而且一不小心还会出问题。但是Flash player默认是被安装的。而且是比较稳定的。虽然开源的VLC播放器所提供的OCX相当不错。可是个人还是觉得没有Flash直接播放方便。flash支持的视频格式有限，像mp4等格式都不会被flash
 player直接支持。所以就有了选择RTMP协议的视频流。支持点播和时间查询播放。那么下面我们就在Windows下搭建一个RTMP视频流发布程序。我选择的都是开源代码编译出的程序去搭建。免费，可定制。：）ffmpeg+ CRtmpserver

### **准备**

             下载ffmpeg. 这个支持很多很多的格式视频http://ffmpeg.org/download.html。进入下载，可以下载源码和Windows 平台下的Release版。然后下载crtmpserver.http://www.rtmpd.com/  这是官网。，但是官网不提供Windows下的编译好的程序。http://www.rtmpd.com/resources/里面能看到源码下载地址，

![](http://www.iyunv.com/data/attachment/forum/201401/23/090011vxsbh0ydjnnrn22g.jpg)

需要我们自己下载源代码编译。可以看到是SVN进行管理的。进入Wiki。可以看到详细的说明文档。它是在linux下编写完成（很多好的开源代码都是在linux下编写完成，所以我们还是非常有必要学习学习linux）。我们只能搭建交叉编译环境，编译出Windows下可执行程序。我喜欢Ubuntu下编译。交叉编译环境如何搭建。直接百度。很多资料可以参考。这样两个重要的开源工具下载准备完成。当然我们需要一个好的flash播放器。当然流媒体发布服务器开源工具还有比较好的。比如Red5.这是java写的，感兴趣的可以进去看看。

### 开始发布视频

首先我们可以直接将视频文件发布到media目录中


![](http://www.iyunv.com/data/attachment/forum/201401/23/090011np2z1osvnb82gok1.jpg)

可以看到media目录中我放入了几个视频文件：

![](http://www.iyunv.com/data/attachment/forum/201401/23/090012fvczh7sc53zw6lr5.jpg)

这样我们就可以以rtmp流的形式访问视频了；首先得启动服务。直接用批处理启动。run.bat.

![](http://www.iyunv.com/data/attachment/forum/201401/23/090012zglzazbu55alalgl.jpg)

这样就实现了rtmp静态视频发布rtmp流给flash player播放。注意我们这里视频放在media文件夹中这个是由crtmpserver的配置文件中配置的。它的配置文件是lua脚本写的。

![](http://www.iyunv.com/data/attachment/forum/201401/23/090012zfzb6tb7tfx6sh76.jpg)

我的就是配置在media中的。

现在我通过ffmpeg将流发布给crtmpserver对外发布。

我写了一个批处理直接运行。批处理内容是：

![](http://www.iyunv.com/data/attachment/forum/201401/23/090013udok7ididtckm8sd.jpg)

我将2.mp4视频发布成编码成H264以rtmp格式发布出去。

![](http://www.iyunv.com/data/attachment/forum/201401/23/090013gcoav00msfk0sc3o.jpg)
![](http://www.iyunv.com/data/attachment/forum/201401/23/090014cmd1pdozc8zaky9l.jpg)
这样这个2.mp4就被发布出去了。


**后记：**

      虽然都是以视频文件发布rtmp流发布的。但是这只是简单的搭建实例。我们可以将他用到很多地方。我后面打算通过它直接将摄像头的视频流编码成H264发布出去rtmp流供flash player播放。然后远程查看。同时我打算利用Drectshow操作电脑摄像头实现视频即时一对一或者一对多聊天。并且存储服务器视频文件供历史聊天视频查询。这也是我选择开源代码搭建的原因之一。

文章来源：[http://www.iyunv.com/thread-14530-1-1.html](http://www.iyunv.com/thread-14530-1-1.html)


### 参考阅读

网络视频：[http://blog.csdn.net/xdwyyan/article/category/2752791](http://blog.csdn.net/xdwyyan/article/category/2752791)




