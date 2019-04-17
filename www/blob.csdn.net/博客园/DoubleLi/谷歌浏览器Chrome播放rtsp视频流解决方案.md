# 谷歌浏览器Chrome播放rtsp视频流解决方案 - DoubleLi - 博客园






代码中的具体参数含义，大家可以移步这里：

[https://wiki.videolan.org/Documentation:WebPlugin/](https://wiki.videolan.org/Documentation:WebPlugin/)



找半天，[HTML5](http://lib.csdn.net/base/html5)的可以支持RTMP 但是无法播放RTSP，flash也止步于RTMP，最后同事推荐了个开源的好东东 VLC ，请教谷歌大神之后，这货果然可以用来让各浏览器（IE activex方式，谷歌、FF）实现播放RTSP视频流，那真是极好的~~ 废话不多说了附上参考文档和案例，大家自己看吧。

视频是用的海康网络摄像头（支持RTSP，标准H.264 RTP封装的设备），可以通过 rtsp://admin:12345@192.0.0.64:81/h264/ch1/main/av_stream  对摄像头进行读取RSTP流。在谷歌浏览器下实现web显示实时监控画面步骤：

1. 下载vlc -  http://www.videolan.org/vlc/  开源的好东西，并安装（本人[测试](http://lib.csdn.net/base/softwaretest)系统为WIN8.1 64bit 下载32bit的就可以，XP也没问题）；

2. 安装，并勾选Mozila支持；

3. 实时播放代码如下

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

<object type='application/x-vlc-plugin' pluginspage="http://www.videolan.org/" id='vlc' events='false' width="720" height="410">
    <param name='mrl' value='rtsp://admin:12345@192.168.10.235:554/h264/ch1/main/av_stream' />
    <param name='volume' value='50' />
    <param name='autoplay' value='true' />
    <param name='loop' value='false' />
    <param name='fullscreen' value='false' />
    <param name='controls' value='false' />
</object>

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

代码中的具体参数含义，大家可以移步这里：

[https://wiki.videolan.org/Documentation:WebPlugin/](https://wiki.videolan.org/Documentation:WebPlugin/)

说明文档很详细，可以使用[js](http://lib.csdn.net/base/javascript)进行各种操作，修改视频地址、建立播放列表、暂停、播放、音量控制等。









