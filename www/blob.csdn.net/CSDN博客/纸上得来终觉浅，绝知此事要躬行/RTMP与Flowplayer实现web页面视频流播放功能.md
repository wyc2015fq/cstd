# RTMP与Flowplayer实现web页面视频流播放功能 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年11月29日 14:09:29[boonya](https://me.csdn.net/boonya)阅读数：5225








官网地址：[https://flowplayer.com/](https://flowplayer.com/)

基于RTMP实时消息通信协议与Flowplayer的示例：



```
<!-- widescreen container, 560x240 (clip dimensions) * 1.15, center splash -->
<div id="wowza" style="width:644px;height:276px;margin:0 auto;text-align:center">
    <img src="/media/img/player/splash_black.jpg"
         height="276" width="548" style="cursor:pointer" />
</div>
```


```
$f("wowza", "http://releases.flowplayer.org/swf/flowplayer-3.2.18.swf", {
 
    clip: {
        url: 'mp4:vod/demo.flowplayer/buffalo_soldiers.mp4',
        scaling: 'fit',
        // configure clip to use hddn as our provider, referring to our rtmp plugin
        provider: 'hddn'
    },
 
    // streaming plugins are configured under the plugins node
    plugins: {
 
        // here is our rtmp plugin configuration
        hddn: {
            url: "flowplayer.rtmp-3.2.13.swf",
 
            // netConnectionUrl defines where the streams are found
            netConnectionUrl: 'rtmp://r.demo.flowplayer.netdna-cdn.com/play'
        }
    },
    canvas: {
        backgroundGradient: 'none'
    }
});
```
示例参考地址：[http://flash.flowplayer.org/plugins/streaming/rtmp.html](http://flash.flowplayer.org/plugins/streaming/rtmp.html)


=============更多参考内容：=================================

[最简单的基于Flash的流媒体示例：网页播放器（HTTP，RTMP，HLS）](http://blog.csdn.net/wishfly/article/details/44534139)


[Flowplayer基于视频流的免费web视频播放器](http://blog.csdn.net/u010989191/article/details/51346604)

[基于RTMP协议的Flash流媒体网页直播播放器](http://blog.csdn.net/aoshilang2249/article/details/44621961)


[最简单的基于Flash的流媒体示例：RTMP推送和接收（ActionScript）](http://blog.csdn.net/leixiaohua1020/article/details/43936141)


[Flash视频播放器flowplayer的使用](https://www.cnblogs.com/modou/articles/1982838.html)

[FlowPlayer 参数说明](https://www.cnblogs.com/microstep/p/4972997.html)


[videoJS/ckPlayer开源播放器二次开发(播放rtmp、hls直播流及普通视频)](https://www.2cto.com/kf/201607/526575.html)




