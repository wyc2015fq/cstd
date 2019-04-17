# nginx开发（四）调用ffmpeg，搭建rtmp直播流。 - DoubleLi - 博客园






1: 修改conf文件，配置rtmp直播

打开usr/local/nginx/conf/nginx.conf，添加红色内容：

rtmp {
#rtmp点播配置
    server {
        listen 1935;
        chunk_size 4000;
        application  vod {
                play /usr/local/nginx/html/multimedia/rtmp/vod;  #点播媒体文件存放目录
        }
    }

#rtmp直播配置
    server {
    listen 1936;
    chunk_size 4000;
    application  live {
                live on;
    }
}
}

2:下载ffmpeg，官网链接：http://ffmpeg.org/download.html



3: 通过ffmpeg，向nginx发送rtmp直播流

ffmpeg -re -i 1.flv -f flv rtmp://yourserverIp:1936/live/show

解释：

1）:live为application的名字，由nginx的conf中定义

2）:show为直播流的名字，由推送方定义，此处为ffmpeg



4:调用播放器播放rtmp直播流

在这我使用的是vlc播放器，播放链接为：rtmp://yourserverIp:1936/live/show



完成！！！









