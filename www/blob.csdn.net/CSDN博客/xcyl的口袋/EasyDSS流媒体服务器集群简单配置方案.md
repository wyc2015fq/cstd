# EasyDSS流媒体服务器集群简单配置方案 - xcyl的口袋 - CSDN博客
2018年03月14日 17:15:08[xcyl](https://me.csdn.net/cai6811376)阅读数：386
# EasyDSS集群简单配置方案
## 1. 说明
EasyDSS支持配置RTMP拉流转发和RTMP推流转发。在做EasyDSS集群时，我们可以配置一台EasyDSS为接收推流的服务器，并配置推流转发至其他的EasyDSS服务器用于流分发，当然，接收推流的EasyDSS也可用于流分发。
这样流分发服务器提供的播放地址都相同，只需要提供一台管理服务用于分配播放地址即可。
![](https://img-blog.csdn.net/20180314171314805?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2NhaTY4MTEzNzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2. 配置
推流配置
```
rtmp {
    server {
        #rtmp监听端口，默认10085
        listen 10085;
        #自动推流配置
        application live {
            live on;
            #rtmp地址为推流地址，name指定rtmp的name
            #当EasyDSS接收app为live，name为stream1的rtmp流时
            #可以同步转发到指定的rtmp推流地址
            push rtmp://ip:port/app/stream1 name=stream1;
            #当EasyDSS接收app为live的推流时
            #可以同步转发到指定的rtmp推流地址
            #rtmp流的name将自动添加到转发地址进行转发
            #如：接收推流为/live/stream
            #则会转发到rtmp://ip:port/app/stream
            push rtmp://ip:port/app;
        }
    }
}
```
## 3. 示例
如果我们现在有4台EasyDSS服务器，分别为A、B、C、D，A用于接收推流，转发给其他3台B、C、D用于流分发，A也用于流分发。这样，我们需要在A服务进行配置即可。
我们假设A服务器的IP地址为`192.168.1.5`，B为`192.168.1.6`，C为`192.168.1.7`，D为`192.168.1.8`，他们的RTMP端口都为10085。
```
#A服务器配置
application hls {
    live on;
    hls on;
    hls_path www/hls;
    hls_fragment 4s;
    push rtmp://192.168.1.6:10085/hls;
    push rtmp://192.168.1.7:10085/hls;
    push rtmp://192.168.1.8:10085/hls;
}
#B、C、D服务器配置
application hls {
    live on;
    hls on;
    hls_path www/hls;
    hls_fragment 4s;
}
```
当A服务器接收推流为`rtmp://192.168.1.5:10085/hls/stream`时，则
```
服务的分发地址为：
A: rtmp://192.168.1.5:10085/hls/stream
   http://192.168.1.5/hls/stream/stream_live.m3u8
B: rtmp://192.168.1.6:10085/hls/stream
   http://192.168.1.6/hls/stream/stream_live.m3u8
C: rtmp://192.168.1.7:10085/hls/stream
   http://192.168.1.7/hls/stream/stream_live.m3u8
D: rtmp://192.168.1.8:10085/hls/stream
   http://192.168.1.8/hls/stream/stream_live.m3u8
```
## 获取更多信息
技术方案与商业授权咨询：**13718530929**
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
EasyDarwin开源流媒体服务器：[www.easydarwin.org](http://www.easydarwin.org)
EasyDSS商用流媒体解决方案：[www.easydss.com](http://www.easydss.com)
EasyNVR无插件直播方案：[www.easynvr.com](http://www.easynvr.com)
QQ群：587254841
Copyright © EasyDarwin.org 2012-2018
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
