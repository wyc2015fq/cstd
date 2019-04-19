# 使用ffmpeg循环推流(循环读取视频文件)推送EasyDSS RTMP流媒体服务器的方法 - xcyl的口袋 - CSDN博客
2017年07月08日 00:36:51[xcyl](https://me.csdn.net/cai6811376)阅读数：15062
## 需求
在做EasyDSS开发时，总是在测试推流效果。
当然，可以使用[libEasyRTMP](https://github.com/EasyDarwin/EasyRTMP)进行推流测试。
有时候，我们想使用[OBS](http://blog.csdn.net/xiejiashu/article/details/68483758)进行推流测试，也可以，这很好。
以及其他RTMP推流工具。
但是，别忘了，还有ffmpeg这个神器。ffmpeg可以获取各种视频流，并推送给EasyDSS RTMP流媒体服务器。
比如，拉取摄像机RTSP流，推送给EasyDSS，命令在此
` ffmpeg -re -i rtsp://username:password@ip:port/xxxxxxx -vcodec copy -acodec copy -f flv -y rtmp://easydssip:easydssport/live/test`
当然，这个命令限于摄像机提供的就为H.264+AAC的码流，若不是
则将`-vcodec copy`改为`-vcodec libx264`，`-acodec copy`改为`-acodec aac`
若是，我们想用ffmpeg读取文件进行RTMP推送呢，当然，您可以选择OBS，但是，毕竟，ffmpeg比OBS更轻量，命令在此
` ffmpeg -re -i test.h264 -vcodec copy -acodec copy -f flv -y rtmp://easydssip:easydssport/live/test`
-vcodec和-acodec按需更改同拉取RTSP流。
这样可以满足我们推送文件到EasyDSS的愿望。
但是。。。。。
如果我想一直循环读取文件，做个长测呢？
## 解决
当然有解决办法
` ffmpeg -re  -stream_loop -1 -i test.h264 -vcodec copy -acodec copy -f flv -y rtmp://easydssip:easydssport/live/test`
加了一个`-stream_loop -1`
这是干啥的？
![这里写图片描述](https://img-blog.csdn.net/20170708003320838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
[https://ffmpeg.org/ffmpeg-all.html#Main-options](https://ffmpeg.org/ffmpeg-all.html#Main-options)
我们明白了此选项可以指定循环读取视频源的次数，-1为无限循环
就此解决。
## EasyDSS
EasyDSS商用流媒体服务器提供一站式的转码、点播、直播、时移回放服务，极大地简化了开发和集成的工作，并且EasyDSS支持多种特性，完全能够满足企业视频信息化建设方面的需求： 
①多屏播放：支持Flash、HTML5播放，兼容Windows、Android、iOS、Mac等操作系统。 
②自由组合：EasyDSS软件产品之间无缝对接，也可将EasyDSS流媒体服务器软件与其他第三方平台对接，组合灵活自由。 
③支持云架构：支持阿里云、腾讯云、华为云、青云、七牛云存储等各大云服务商，支持云架构，部署更灵活。 
④与CDN无缝对接：EasyDSS软件产品支持与网宿、帝联、蓝汛等CDN无缝对接。 
⑤二次开发简单：提供编程语言无关化的RESTful API接口，只要了解JS、HTML、JAVA、ASP.NET、PHP、ASP等开发语言中的任意一种，就能运用EasyDSS提供的RESTful API进行二次开发。 
⑥简单易用：图形化操作，模块清晰，流程简单，极易上手。 
[EasyDarwin如何支持点播和RTMP/HLS直播？EasyDSS！](http://blog.csdn.net/xiejiashu/article/details/73253903)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：[587254841](https://jq.qq.com/?_wv=1027&k=4ASE72a)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
