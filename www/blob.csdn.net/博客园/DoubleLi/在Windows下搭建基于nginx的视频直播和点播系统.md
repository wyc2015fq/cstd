# 在Windows下搭建基于nginx的视频直播和点播系统 - DoubleLi - 博客园






## 一、软件准备

由于nginx原生是为linux服务的，因此官方并没有编译好的windows版本可以下载，要在windows上使用nginx，要么下载源码进行编译，要么使用其他人已经编译好的文件。

而要让nginx支持视频直播和点播，还需要第三方的nginx模块：nginx-rtmp-module

所幸，已经有大神做好了nginx的编译，而且集成了很多nginx模块，其中就已经包括了nginx-rtmp-module。

下载地址：http://nginx-win.ecsds.eu/，详细说明可参见：Readme nginx-win version.txt

我下载的是nginx 1.7.11.3 Gryphon这个版本。

这个网站同时也提供了vcredist的下载（x86，x64），以避免运行nginx时出现缺少库的错误。

另外还要下载 stat.xsl 用于显示当前ngix-rtmp服务状态

另外还需要下载ffmpeg、ffplay、yamdi：

总结如下：

1. nginx 1.7.11.3 Gryphon

2. stat.xsl

3. ffmpeg、ffplay

4. yamdi

## 二、Nginx 配置

 1. nginx配置
worker_processes  1;

error_log  logs/error.log debug;

events {
    worker_connections  1024;
}

rtmp {
    server {
        listen 1935;

        application hls {
             live on;  #启用rtmp直播
                       #地址为rtmp://[server]:[rtmp_port]/[app]/[stream]
             hls on;   #启用hls直播
                       #地址为http://[server]:[http_port]/[app]/[stream].m3u8
                       #需要配合下面http段设置使用
             hls_path nginx-rtmp-module/tmp/app/;
             hls_fragment 5s;
             recorder rec {  #启用录制
               record all manual;  #手动控制录制启停
               record_suffix _rec.flv;
               record_path nginx-rtmp-module/tmp/rec/;  #录制保存地址
               record_unique on;
           }
       }
       application vod2{  #rtmp点播
       		play nginx-rtmp-module/tmp/rec/;    
       }
    }
}

http {
    server {
        listen      18080;

        location /stat {  #服务器状态
            rtmp_stat all;
            rtmp_stat_stylesheet stat.xsl;
        }

        location /stat.xsl {
            root nginx-rtmp-module/;
        }

        location /control { #控制器
            rtmp_control all;
        }
        
	location /hls/ {  #hls直播地址
           #server hls fragments
           types{
             application/vnd.apple.mpegurl m3u8;
             video/mp2t ts;
           }
           alias nginx-rtmp-module/tmp/app/;
           expires -1;
        }

        location /vod/{  #hls点播地址
            alias nginx-rtmp-module/tmp/rec/;
        }

        location / {
            root nginx-rtmp-module/test/www/;
        }
    }
}


## 三、测试

1. 启动nginx
start nginx_1.7.11.3_Gryphon\nginx


2. 推送rtmp流到nginx-rtmp
start ffmpeg\ffmpeg -re -i rtmp://live.hkstv.hk.lxdns.com/live/hks -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -acodec copy -f flv rtmp://localhost:1935/hls/mystream -loglevel quiet


3. rtmp直播
ffmpeg\ffplay "rtmp://127.0.0.1:1935/hls/mystream"


4. hls 直播
ffmpeg\ffplay "http://127.0.0.1:18080/hls/mystream.m3u8"


5. 开始录制
http://127.0.0.1:18080/control/record/start?app=hls&name=mystream&rec=rec


6. 停止录制
http://127.0.0.1:18080/control/record/stop?app=hls&name=mystream&rec=rec


7. 为rtmp点播文件添加索引，否则文件在播放时进度条不能拖动，假定刚才录制的文件名为mystream-1428384476_rec.flv
yamdi\yamdi -i nginx_1.7.11.3_Gryphon\nginx-rtmp-module\tmp\rec\mystream-1428384476_rec.flv -o nginx_1.7.11.3_Gryphon\nginx-rtmp-module\tmp\rec\mystream-1428384476_rec_idx.flv


8. rtmp点播
ffmpeg\ffplay "rtmp://127.0.0.1:1935/vod2/mystream-1428384476_rec_idx.flv"


9. 制作hls点播分片文件
ffmpeg\ffmpeg -i E:\video-nginx\nginx_1.7.11.3_Gryphon\nginx-rtmp-module\tmp\rec\mystream-1428384476_rec.flv -acodec copy -bsf:a h264_mp4toannexb -g 105 -vcodec libx264 -vprofile baseline -bf 0 -bufsize 850k -bsf:v dump_extra -map 0 -f segment -segment_format mpegts -segment_list "E:\video-nginx\nginx_1.7.11.3_Gryphon\nginx-rtmp-module\tmp\rec\mystream-1428384476_rec\mystream-1428384476_rec.m3u8" -segment_time 10 E:\video-nginx\nginx_1.7.11.3_Gryphon\nginx-rtmp-module\tmp\rec\mystream-1428384476_rec\mystream-1428384476_rec-%d.ts


10. hls 点播
ffplay "http://127.0.0.1:8080/vod/mystream-1428384476_rec/mystream-1428384476_rec.m3u8"








