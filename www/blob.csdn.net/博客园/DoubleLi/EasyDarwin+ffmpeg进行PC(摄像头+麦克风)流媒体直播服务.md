# EasyDarwin+ffmpeg进行PC(摄像头+麦克风)流媒体直播服务 - DoubleLi - 博客园






上一回我们描述了用EasyDarwin+ffmpeg进行摄像机直播的过程：[ffmpeg推送,EasyDarwin转发,vlc播放 实现整个RTSP直播](http://www.easydarwin.org/article/EasyDarwin/30.html)

我们再进行一个方面的描述，那就是pc摄像头+麦克风进行主播过程：

第一步》列出我们本机的设备：ffmpeg -list_devices true -f dshow -i dummy

![](http://img.blog.csdn.net/20150717161507986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



第二步》ffmpeg编码推送到EasyDarwin：ffmpeg -f dshow -i video="Integrated Camera":audio="麦克风 (Realtek High Definition Au" -vcodec libx264 -acodec libvo_aacenc -preset:v ultrafast -tune:v zerolatency -f rtsp rtsp://127.0.0.1/live.sdp

![](http://img.blog.csdn.net/20150717161804788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



第三步》查看直播：rtsp://127.0.0.1/live.sdp

![](http://img.blog.csdn.net/20150717162107949?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



------------------------------------------------------------









