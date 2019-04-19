# EasyDSS RTMP流媒体服务器搭建直播集群 - xcyl的口袋 - CSDN博客
2017年07月09日 22:48:04[xcyl](https://me.csdn.net/cai6811376)阅读数：2409
前段时间，我们的EasyDSS流媒体的一个客户提出一个需求：
> 
搭建一个讲座的直播，提供给普通用户免费观看，持续几天，每天上午和下午有几场讲座同时直播，用户量巨大。
我们决定不适用CDN，完全用EasyDSS搭建集群提供直播服务。
EasyDSS支持接收RTMP推流，并可以同步输出RTMP、HLS。由于客户在APP使用，并对延时要求不高，故推荐使用HLS直播。
EasyDSS支持RTMP转推功能，可以将推送断推送的音视频流转推送给其他的RTMP流媒体服务器，这样我们就可以使用一个EasyDSS接收直播端的推流，并转发给集群的其他EasyDSS流媒体服务器。
![这里写图片描述](https://img-blog.csdn.net/20170709223251061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样，集群中的所有EasyDSS都可以提供此直播流的分发服务，并同步进行HLS切片。
接着，在入口端提供一个http的负载均衡服务器，就可以进行HLS直播集群。
![这里写图片描述](https://img-blog.csdn.net/20170709224130344?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170709230336066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170709230357052?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170709230419729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170709230436462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170709230457287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## EasyDSS
EasyDSS商用流媒体服务器提供一站式的转码、点播、直播、时移回放服务，极大地简化了开发和集成的工作，并且EasyDSS支持多种特性，完全能够满足企业视频信息化建设方面的需求：
①多屏播放：支持Flash、HTML5播放，兼容Windows、Android、iOS、Mac等操作系统。  
②自由组合：EasyDSS软件产品之间无缝对接，也可将EasyDSS流媒体服务器软件与其他第三方平台对接，组合灵活自由。  
③支持云架构：支持阿里云、腾讯云、华为云、青云、七牛云存储等各大云服务商，支持云架构，部署更灵活。  
④与CDN无缝对接：EasyDSS软件产品支持与网宿、帝联、蓝汛等CDN无缝对接。  
⑤二次开发简单：提供编程语言无关化的RESTful API接口，只要了解js、HTML、Java、ASP.NET、PHP、ASP等开发语言中的任意一种，就能运用EasyDSS提供的RESTful API进行二次开发。  
⑥简单易用：图形化操作，模块清晰，流程简单，极易上手。
[EasyDarwin如何支持点播和RTMP/HLS直播？EasyDSS！](http://blog.csdn.net/xiejiashu/article/details/73253903)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：[587254841](https://jq.qq.com/?_wv=1027&k=4ASE72a)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
