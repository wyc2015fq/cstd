# 最简单的基于Flash的流媒体示例：网页播放器（HTTP，RTMP，HLS） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年02月25日 14:04:39[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：194612








=====================================================
Flash流媒体文章列表：

[最简单的基于Flash的流媒体示例：RTMP推送和接收（ActionScript）](http://blog.csdn.net/leixiaohua1020/article/details/43936141)

[最简单的基于Flash的流媒体示例：网页播放器（HTTP，RTMP，HLS）](http://blog.csdn.net/leixiaohua1020/article/details/43936415)

=====================================================





本文继续上一篇文章，记录一些基于Flash的流媒体处理的例子。本文记录一些基于Flash技术的网页播放器。基于Flash的网页播放器相比于其他网页播放器来说最大的优势就是“免插件安装”了，这一点可以很大的提高用户的体验质量。早些时候网络视频（尤其是直播）一般都使用ActiveX控件作为视频播放器，而这些控件并不普及，所以终端用户必须下载相关的插件才能收看节目，因而对很多不熟悉电脑的用户造成了很大的障碍。直到Flash网页播放器的出现，这一障碍才得到了解决。本文将会记录几个常用的网页播放器，方便以后开发测试使用。

![](https://img-blog.csdn.net/20150225124305221)


本文记录以下几种常用的网页播放器：
- rtmp_sample_player_adobe: 从Adobe Flash Media Sever提取出来的测试播放器
- rtmp_sample_player_wowza: 从Wowza服务器中提取出来的测试播放器
- rtmp_sample_player_flowplayer: 基于FlowPlayer的RTMP/HTTP播放器（添加RTMP plugin）
- rtmp_sample_player_videojs: 基于VideoJS的RTMP/HTTP播放器
- rtmp_sample_player_jwplayer: 基于JWplayer的RTMP/HTTP播放器
- hls_sample_player_flowplayer: 基于FlowPlayer的HLS播放器（添加HLS plugin）
- hls_video_player_html5: 基于HTML5的HLS/HTTP播放器
- activex_vlc_player: 基于VLC的ActiveX控件的播放器



## RTMP Sample Player Adobe

![](https://img-blog.csdn.net/20150225124702450)
RTMP Sample Player Adobe是从Adobe Flash Media Sever提取出来的测试播放器，平时测试使用非常方便，把测试的RTMP地址粘贴到下方的“Stream URL”里面，单击右侧红色的“Play Stream”就可以播放流媒体了。
RTMP Sample Player Adobe的截图如下所示。

![](https://img-blog.csdn.net/20150225124703248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




## RTMP Sample Player Wowza

![](https://img-blog.csdn.net/20150225124715261)

RTMP Sample Player Wowza是从Wowza Streaming Engine中提取出来的测试播放器，平时测试使用也比较方便，将RTMP地址贴到对应的输入框中，单击“start”就可以播放流媒体了。
RTMP Sample Player Wowza的截图如下所示。

![](https://img-blog.csdn.net/20150225124740548?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## RTMP Sample Player Flowplayer

![](https://img-blog.csdn.net/20150225124836971)
FlowPlayer官网：[http://flash.flowplayer.org/](http://flash.flowplayer.org/)
*注：FlowPlayer有两个版本：HTML5版本和Flash版本，本文中使用的是Flash版本。*

RTMP Sample Player Flowplayer是基于FlowPlayer的网页播放器。其中包含两个播放器：
- Sample Player Flowplayer：HTTP点播网页播放器
- RTMP Sample Player Flowplayer：RTMP网页播放器
FlowPlayer原生支持HTTP点播，通过RTMP Plugin扩展之后支持RTMP。

Sample Player Flowplayer是支持HTTP点播的网页播放器，代码如下所示。

```
<html>
<head>
<script type="text/javascript" src="flowplayer-3.2.8.min.js"></script>
<title>Sample Player FlowPlayer</title>
</head>


<body>	
	<h1>Sample Player FlowPlayer</h1>


	<p>Lei Xiaohua<br/>
	<a href="http://blog.csdn.net/leixiaohua1020">http://blog.csdn.net/leixiaohua1020</a><br/>
	leixiaohua1020@126.com</p>
	
	<!-- this A tag is where your Flowplayer will be placed. it can be anywhere -->
	<a  
		 href="http://video-js.zencoder.com/oceans-clip.mp4"
		 style="display:block;width:520px;height:330px"  
		 id="player"> 
	</a> 
	<!-- this will install flowplayer inside previous A- tag. -->
	<script>
	flowplayer("player", "flowplayer-3.2.8.swf");
	</script>
	
</body>
</html>
```

播放器的效果如下图所示，图中正在播放的视频文件的URL为http://video-js.zencoder.com/oceans-clip.mp4

![](https://img-blog.csdn.net/20150225124935003?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


RTMP Sample Player Flowplayer是经过RTMP Plugin扩展之后的网页播放器，代码如下所示。


```
<!DOCTYPE html>
<html>
<head>
<script type="text/javascript" src="flowplayer-3.2.8.min.js"></script>
<title>RTMP Sample Player FlowPlayer</title>
</head>


<body>	
	<h1>RTMP Sample Player FlowPlayer</h1>


	<p>Lei Xiaohua<br/>
	<a href="http://blog.csdn.net/leixiaohua1020">http://blog.csdn.net/leixiaohua1020</a><br/>
	leixiaohua1020@126.com</p>
	
	<!-- this A tag is where your Flowplayer will be placed. it can be anywhere -->
	<a  
		 href="#"
		 style="display:block;width:520px;height:330px"  
		 id="player"> 
	</a> 
	<!-- this will install flowplayer inside previous A- tag. -->
	<script>
	flowplayer("player", "flowplayer-3.2.8.swf",{ 
		clip: { 
		  url: 'hks',
		  provider: 'rtmp',
		  live: true, 
		},  
		plugins: {  
		   rtmp: {  
			 url: 'flowplayer.rtmp-3.2.8.swf',  
			 netConnectionUrl: 'rtmp://live.hkstv.hk.lxdns.com/live'
		   } 
	   } 
	});
	</script>


	<p>		
		Sample RTMP URL (Live) is "rtmp://live.hkstv.hk.lxdns.com/live/hks"
	</p>
	
</body>
</html>
```

播放器的效果如下图所示，图中正在播放的视频的URL为rtmp://live.hkstv.hk.lxdns.com/live/hks

![](https://img-blog.csdn.net/20150225125015454?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## RTMP Sample Player VideoJS

![](https://img-blog.csdn.net/20150225125040445)

Video.js官网：[http://www.videojs.com/](http://www.videojs.com/)
*注：Video.js是一个基于JavaScript的HTML5视频播放器，本身是开源的。*

RTMP Sample Player VideoJS是基于Video.js的RTMP播放器。其中包含两个播放器：
- Sample Player VideoJS：HTTP点播网页播放器
- RTMP Sample Player VideoJS：RTMP网页播放器
Sample Player VideoJS是支持HTTP点播的网页播放器，代码如下所示。

```
<!DOCTYPE html>
<html>
<head>
  <title>Sample Player Videojs</title>
  <!-- Chang URLs to wherever Video.js files will be hosted -->
  <link href="video-js.css" rel="stylesheet" type="text/css">
  <!-- video.js must be in the <head> for older IEs to work. -->
  <script src="video.js"></script>
  <!-- Unless using the CDN hosted version, update the URL to the Flash SWF -->
  <script>
    videojs.options.flash.swf = "video-js.swf";
  </script>
</head>
<body>
<h1>Sample Player Videojs</h1>
<p>Lei Xiaohua<br/>
	<a href="http://blog.csdn.net/leixiaohua1020">http://blog.csdn.net/leixiaohua1020</a><br/>
	leixiaohua1020@126.com</p>
  <video id="example_video_1" class="video-js vjs-default-skin" controls preload="none" width="640" height="264"
      poster="http://video-js.zencoder.com/oceans-clip.png"
      data-setup="{}">
    <source src="http://video-js.zencoder.com/oceans-clip.mp4" type='video/mp4' />
    <source src="http://video-js.zencoder.com/oceans-clip.webm" type='video/webm' />
    <source src="http://video-js.zencoder.com/oceans-clip.ogv" type='video/ogg' />
    <track kind="captions" src="demo.captions.vtt" srclang="en" label="English"></track>
    <track kind="subtitles" src="demo.captions.vtt" srclang="en" label="English"></track>
    <p class="vjs-no-js">To view this video please enable JavaScript, and consider upgrading to a web browser that <a href="http://videojs.com/html5-video-support/" target="_blank">supports HTML5 video</a></p>
  </video>


</body>
</html>
```

播放器的效果如下图所示，图中正在播放的视频的URL为http://video-js.zencoder.com/oceans-clip.mp4

![](https://img-blog.csdn.net/20150225125126746?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

RTMP Sample Player VideoJS是支持RTMP的网页播放器，代码如下所示。

```
<!DOCTYPE html>
<html>
<head>
  <title>RTMP Sample Player Videojs</title>
  <!-- Chang URLs to wherever Video.js files will be hosted -->
  <link href="video-js.css" rel="stylesheet" type="text/css">
  <!-- video.js must be in the <head> for older IEs to work. -->
  <script src="video.js"></script>
  <!-- Unless using the CDN hosted version, update the URL to the Flash SWF -->
  <script>
    videojs.options.flash.swf = "video-js.swf";
  </script>
</head>
<body>
<h1>RTMP Sample Player Videojs</h1>
<p>Lei Xiaohua<br/>
	<a href="http://blog.csdn.net/leixiaohua1020">http://blog.csdn.net/leixiaohua1020</a><br/>
	leixiaohua1020@126.com</p>
  <video id="example_video_1" class="video-js vjs-default-skin" controls preload="none" width="640" height="480" data-setup="{}">
    <source src="rtmp://ams.studytv.cn/livepkgr/264" type="rtmp/flv"/>
	
    <p class="vjs-no-js">To view this video please enable JavaScript, and consider upgrading to a web browser that <a href="http://videojs.com/html5-video-support/" target="_blank">supports HTML5 video</a></p>
  </video>


</body>
</html>
```

播放器的效果如下图所示，图中正在播放的视频的URL为rtmp://ams.studytv.cn/livepkgr/264

![](https://img-blog.csdn.net/20150225125147715?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## RTMP Sample Player JWPlayer

![](https://img-blog.csdn.net/20150225125303263)

JW Player官网：[http://www.jwplayer.com](http://www.jwplayer.com)

*注：最新版的JW Player似乎不能免费使用RTMP播放功能了，这里使用的是旧版的JW Player*

RTMP Sample Player JWPlayer是基于JW Player的RTMP播放器。其中包含两个播放器：
- Sample Player JWPlayer：HTTP点播网页播放器
- RTMP Sample Player JWPlayer：RTMP网页播放器
Sample Player JWPlayer是支持HTTP点播的网页播放器，代码如下所示。

```
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>Sample Player JWPlayer</title>
	<script type='text/javascript' src='jwplayer.js'></script>
</head>
<body>
	<h1>Sample Player JWPlayer</h1>
	<p>Lei Xiaohua<br/>
	<a href="http://blog.csdn.net/leixiaohua1020">http://blog.csdn.net/leixiaohua1020</a><br/>
	leixiaohua1020@126.com</p>
	
	<div id='mediaspace'>This text will be replaced</div>
	<script type='text/javascript'>
	  jwplayer('mediaspace').setup({
		'flashplayer': 'player.swf',
		'file': 'sintel.mp4',
		'controlbar': 'bottom',
		'width': '640',
		'height': '360'
	  });
	</script>
	
</body>
</html>
```

播放器的效果如下图所示，图中正在播放的视频的URL为本地的sintel.mp4


![](https://img-blog.csdn.net/20150225125412496?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

RTMP Sample Player JWPlayer是支持RTMP的网页播放器，代码如下所示。

```
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>RTMP Sample Player JWPlayer</title>
	<script type='text/javascript' src='jwplayer.js'></script>
</head>
<body>
	<h1>RTMP Sample Player JWPlayer</h1>
	<p>Lei Xiaohua<br/>
	<a href="http://blog.csdn.net/leixiaohua1020">http://blog.csdn.net/leixiaohua1020</a><br/>
	leixiaohua1020@126.com</p>
	
	<div id='mediaspace'>This text will be replaced</div>
	<script type='text/javascript'>
	  jwplayer('mediaspace').setup({
		'flashplayer': 'player.swf',
		'file': 'flv',
		'streamer': 'rtmp://wx.cnrmall.com/live',
		'controlbar': 'bottom',
		'width': '640',
		'height': '360'
	  });
	</script>
	<p>		
		Sample RTMP URL (Live) is "rtmp://wx.cnrmall.com/live/flv"
	</p>
</body>
</html>
```

播放器的效果如下图所示，图中正在播放的视频的URL为rtmp://wx.cnrmall.com/live/flv

![](https://img-blog.csdn.net/20150225125440548?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## HLS Sample Player Flowplayer
HLS Sample Player Flowplayer是增加了HLS Plugin扩展之后的FlowPlayer。增加了HLS Plugin之后，FlowPlayer增加了对HLS的支持。代码如下所示。

```
<!DOCTYPE HTML>
<html>
<head>
<title>HLS Sample Player Flowplayer</title>
<script type="text/javascript" src="flowplayer-3.2.12.min.js"></script>
<script type="text/javascript" src="flowplayer.ipad-3.2.12.min.js"></script>
</head>


<body>
<h1>HLS Sample Player Flowplayer</h1>
<p>Lei Xiaohua<br/>
	<a href="http://blog.csdn.net/leixiaohua1020">http://blog.csdn.net/leixiaohua1020</a><br/>
	leixiaohua1020@126.com</p>
<!-- player container-->
<a style="display: block; width: 660px; height: 350px;" id="flashls_vod">
</a>


<!-- Flowplayer installation and configuration -->
<script type="text/javascript">
flowplayer("flashls_vod", "flowplayer.swf", {
	// configure the required plugins
	plugins: {
		flashls: {
			url: 'flashlsFlowPlayer.swf',
		}
	},
	clip: {
		url: "http://stream.flowplayer.org/drive.m3u8",
		//url: "http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8",
		//live: true,
		urlResolvers: "flashls",
		provider: "flashls"
	}
}).ipad();
</script>
<p>


</p>


</body>
</html>
```

播放器的效果如下图所示，图中正在播放的视频的URL为http://stream.flowplayer.org/drive.m3u8

![](https://img-blog.csdn.net/20150225125539248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## HLS Video Player Html5

![](https://img-blog.csdn.net/20150225125546411)
HTML5 Video标签参考：http://www.w3school.com.cn/html5/html5_video.asp
HLS Video Player Html5是使用了HTML5的<Video>标签的网页播放器（实际上就是一张网页）。源代码如下所示。

```
<!DOCTYPE HTML>
<html>
<head>
<title>Video Player Html5</title>
</head>
<body>
<h1>Video Player Html5</h1>
<p>Lei Xiaohua<br/>
	<a href="http://blog.csdn.net/leixiaohua1020">http://blog.csdn.net/leixiaohua1020</a><br/>
	leixiaohua1020@126.com</p>
<video src="sintel.mp4" controls="controls">
your browser does not support the video tag
</video>


</body>
</html>
```

在这里需要注意，<video>标签需要浏览器支持HTML。如果浏览器不支持HTML5的话，打开网页会显示如下信息。

![](https://img-blog.csdn.net/20150225125611184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果浏览器支持HTML5，打开网页的效果如下图所示。图中正在播放的视频的URL为sintel.mp4

![](https://img-blog.csdn.net/20150225125630465?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## ActiveX VLC Player

![](https://img-blog.csdn.net/20150225125653990)

ActiveX VLC Player与上述的网页播放器有很大的不同，主要用于对比。ActiveX VLC Player是基于ActiveX控件的播放器。基于ActiveX控件意味着只有IE支持该网页播放器，而且要求本机必须首先安装VLC。
由于有几乎“万能”的VLC Media Player做后盾，该网页播放器的性能是相当强的：不仅支持上文中提到的各种播放器支持的HTTP、RTMP、HLS，而且支持MMS、RTSP以及UDP这些协议（VLC Media Player支持的协议它都支持）。
ActiveX VLC Player源代码如下所示。

```
<!DOCTYPE HTML>
<html>
<head>
<title>ActiveX VLC Player</title>
</head>
<body>
<h1>ActiveX VLC Player</h1>
<p>Lei Xiaohua<br/>
<!-- To run this demo you should install VLC first -->
	<a href="http://blog.csdn.net/leixiaohua1020">http://blog.csdn.net/leixiaohua1020</a><br/>
	leixiaohua1020@126.com</p>
<OBJECT classid="clsid:9BE31822-FDAD-461B-AD51-BE1D1C159921"
    codebase="http://downloads.videolan.org/pub/videolan/vlc/latest/win32/axvlc.cab"
       width="640" height="480" id="vlc" events="True">
 <!--<param name="Src" value="http://video-js.zencoder.com/oceans-clip.mp4" />-->
 <!--<param name="Src" value="http://live.3gv.ifeng.com/live/CCTV13.m3u8" />-->
 <!--<param name="Src" value="rtmp://live.hkstv.hk.lxdns.com/live/hks" />-->
 <!--<param name="Src" value="mmst://media.shtv.net.cn/shtv" />-->
 <param name="Src" value="rtsp://58.248.254.7:9135/live/ds-mmzh.sdp" />
 
 <param name="ShowDisplay" value="True" />
 <param name="AutoLoop" value="False" />
 <param name="AutoPlay" value="True" />
 </OBJECT>


</body>
</html>
```

播放器的效果如下图所示，图中正在播放的视频的URL为rtsp://58.248.254.7:9135/live/ds-mmzh.sdp

![](https://img-blog.csdn.net/20150225125730947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 下载

**Simplest flashmedia example**

SourceForge：[https://sourceforge.net/projects/simplestflashmediaexample/](https://sourceforge.net/projects/simplestflashmediaexample/)
Github：[https://github.com/leixiaohua1020/simplest_flashmedia_example](https://github.com/leixiaohua1020/simplest_flashmedia_example)
开源中国：[http://git.oschina.net/leixiaohua1020/simplest_flashmedia_example](http://git.oschina.net/leixiaohua1020/simplest_flashmedia_example)


CSDN下载：[http://download.csdn.net/detail/leixiaohua1020/8456499](http://download.csdn.net/detail/leixiaohua1020/8456499)



本工程包含如下基于Flash技术的流媒体的例子：
simplest_as3_rtmp_player:  最简单的RTMP播放器（基于ActionScript）
simplest_as3_rtmp_streamer:最简单的RTMP推流器（基于ActionScript）
rtmp_sample_player_adobe: 从Adobe Flash Media Sever提取出来的测试播放器
rtmp_sample_player_wowza: 从Wowza服务器中提取出来的测试播放器
rtmp_sample_player_flowplayer: 基于FlowPlayer的RTMP/HTTP播放器（添加RTMP plugin）
rtmp_sample_player_videojs: 基于VideoJS的RTMP/HTTP播放器
rtmp_sample_player_jwplayer: 基于JWplayer的RTMP/HTTP播放器
hls_sample_player_flowplayer: 基于FlowPlayer的HLS播放器（添加HLS plugin）
hls_video_player_html5: 基于HTML5的HLS/HTTP播放器
activex_vlc_player:  基于VLC的ActiveX控件的播放器

注意：某些播放器直接打开html页面是不能工作的，需要把播放器放到Web服务器上。
（例如Apache或者Nginx）








