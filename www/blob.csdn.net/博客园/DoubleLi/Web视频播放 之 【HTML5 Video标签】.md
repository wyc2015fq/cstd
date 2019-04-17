# Web视频播放 之 【HTML5 Video标签】 - DoubleLi - 博客园






## **一、说明**

[HTML5](http://lib.csdn.net/base/html5)中引入了video标签用于方便的在浏览器中播放视频，不在需要对flashPlayer进行依赖，更加轻量级。但在浏览器兼容、视频协议支持方面还有一些需要注意的问题。

## **二、浏览器兼容**

> 
[html5](http://lib.csdn.net/base/html5) Video标签目前**IE9+ 及 其他主流浏览器**都能够支持。

对于IE8及以下的IE浏览器，可考虑引入[**html5扩展支持js**](http://www.runoob.com/html/html5-browsers.html)来尝试解决，未亲测。


## **三、视频协议支持**

支 持：Ogg、MPEG4、WebM 
不支持：rtmp、rtsp（如果进行转码，切成HLS可以播放，但是即使在局域网内也有3秒左右的时延）

> 
**注：更详细的视频格式支持希望有知情的朋友告知，多谢！**


## **四、DEMO**

> 
**更多的属性、方法和事件参考本文参考资料中的API列表；**


```
<!-- 定义一个video标签：
    autoplay：自动播放；
    controls：显示视频控件;
    src:指定视频源;
    width,height:制定视频的显示尺寸；
 -->
<video id="my_video" autoplay controls      src="https://media.w3.org/2010/05/sintel/trailer.mp4" width="500" height="500">
</video>

<script type="text/javascript">
    //获取视频对象
    var myVideo=document.getElementById("my_video");

    //播放视频
    function play(){
        myVideo.play();
    }
    //暂停视频
    function pause(){
        myVideo.pause();
    }
</script>
```
- 

## **五、参考资料**
- HTML5 Video详细API：[https://msdn.microsoft.com/zh-cn/library/hh772500(v=vs.85).aspx](https://msdn.microsoft.com/zh-cn/library/hh772500(v=vs.85).aspx)
- HTML5 Video详细功能DEMO：[http://www.w3.org/2010/05/video/mediaevents.html](http://www.w3.org/2010/05/video/mediaevents.html)









