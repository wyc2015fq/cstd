# 光电视频流在html5中展示 - 左直拳的马桶_日用桶 - CSDN博客
2019年01月30日 19:12:05[左直拳](https://me.csdn.net/leftfist)阅读数：114
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
光电，其实是一种类似摄像头的设备，有点怪异，呵？它比一般的摄像头看得清，看得远，还有红外功能。
如何在页面中展示它？
三个步骤：
> 
1、将光电视频流转码
2、网站要支持转码后的视频文件
2、HTML5中播放视频文件
边转边播放。
**一、将光电视频流转码**
为什么要转码？
当然是不转的话，浏览器无法识别咯。
转成啥？怎么转？
转成hls。用一个ffmepg的东东转。
具体步骤：
1、下载一个ffmpeg，解压，设置环境变量path
下载：[http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/)
下载并解压FFmpeg文件夹，配置环境变量：在“Path”变量原有变量值内容上加上d:\ffmpeg\bin，验证：ffmpeg -version 出现版本号则成功。
2、运行命令，视频流转换格式并保存在硬盘
```
ffmpeg -rtsp_transport tcp -i "rtsp://账号:密码@IP:端口/路径" -c copy -f hls -hls_time 2.0 -hls_list_size 0 -hls_wrap 15 D:/soft/nginx/html/hls/test.m3u8
```
上述命令中，`-rtsp_transport tcp` 这个比较关键，因为下载的这个版本似乎不支持udp，要改用tcp。参考文章中没有给出该参数。而且这个命令似乎要写在前面，紧跟ffmpeg命令才有效。
其他参数：
-hls_time:每个片段时长
同时，这个命令好像是长期运行的，如果不喊停，理论上会一直运行下去。不过应该也不会导致硬盘撑爆，因为这里指定了15块？
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190130183204432.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
ffmpeg有许多参数，可以用`ffmpeg -help`来查看。
扫盲：
HTTP Live Streaming（HLS）是苹果公司(Apple Inc.)实现的基于HTTP的流媒体传输协议，可实现流媒体的直播和点播，主要应用在iOS系统，为iOS设备（如iPhone、iPad）提供音视频直播和点播方案，它的工作原理是把整个流分成一个个小的基于HTTP的文件来下载，每次只下载一些，在开始一个流媒体会话时，客户端会下载一个包含元数据的extended M3U (m3u8) playlist文件，用于播放可用的媒体流。
**二、网站要支持这个视频格式**
转码，视频流生成了视频文件：***.m3u8。
参考文章中用nginx来做WEB服务器。我们就试试这个nginx。大名鼎鼎，如雷贯耳的nginx。哎，它不是用于做反向代理吗？好像在这里，我们只用来做WEB服务器。
```
nginx.exe -c D:\soft\nginx\conf\nginx.conf
```
按照参考文章的指导去做。但有一些坑需要提醒一下：
1、nginx不能位于有中文的路径里
2、运行nginx，要用管理员身份运行
**三、HTML5中播放**
转码，视频流生成了视频文件：***.m3u8。html5也并不天然支持这种格式。还要一些JS插件的支持。
下载[hls.js](https://github.com/video-dev/hls.js)
```
<!DOCTYPE html>
<html>
<head>
<script src="dist/hls.js"></script>
</head>
<body>
      <center>
          <video height="600" id="video" controls></video>
      </center>
      <script>
        if(Hls.isSupported()) {
          var video = document.getElementById('video');
          var hls = new Hls();
          hls.loadSource('http://192.168.0.98:20000/hls/test.m3u8');
          hls.attachMedia(video);
          hls.on(Hls.Events.MANIFEST_PARSED,function() {
            video.play();
        });
       }
       // hls.js is not supported on platforms that do not have Media Source Extensions (MSE) enabled.
       // When the browser has built-in HLS support (check using `canPlayType`), we can provide an HLS manifest (i.e. .m3u8 URL) directly to the video element throught the `src` property.
       // This is using the built-in support of the plain video element, without using hls.js.
        else if (video.canPlayType('application/vnd.apple.mpegurl')) {
          video.src = 'http://192.168.0.98:20000/hls/test.m3u8';
          video.addEventListener('canplay',function() {
            video.play();
          });
        }
      </script>
</body>
</html>
```
你是光，你是电，你是唯一的神话，我只爱你、you are my super star。
参考文章：
[ffmpeg+nginx+video实现rtsp流转hls流，通过H5查看监控视频](https://blog.csdn.net/kunzai6/article/details/76975600)
[有支持M3U8格式的HTML5播放器吗？](https://www.zhihu.com/question/21087379)
