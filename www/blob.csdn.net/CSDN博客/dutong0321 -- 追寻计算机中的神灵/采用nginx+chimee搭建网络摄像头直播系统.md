# 采用nginx+chimee搭建网络摄像头直播系统 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年02月27日 15:37:13[dutong0321](https://me.csdn.net/dutong0321)阅读数：247标签：[nginx																[ffmpeg																[chimee																[直播																[hls](https://so.csdn.net/so/search/s.do?q=hls&t=blog)](https://so.csdn.net/so/search/s.do?q=直播&t=blog)](https://so.csdn.net/so/search/s.do?q=chimee&t=blog)](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)
个人分类：[Server](https://blog.csdn.net/dutong0321/article/category/6222374)
#### 概述
公司最近需要一套直播系统，要求是网络摄像头进行录制，然后可以在网页上无插件进行直播，要求同时支持PC端和移动端进行直播，最后我决定采用hls协议，利用nginx+chimee搭建网络摄像头直播系统。
#### 搭建网络摄像头直播服务器
0.因为需要用到nginx-rtmp-moudle模块，所以需要下载nginx来重新编译 
1.首先要确保没有安装过nginx，如果安装过要先把nginx进行卸载
```bash
sudo apt remove nginx
```
2.下载nginx并解压，安装git,克隆nginx-rtmp-moudle
```
wget http://nginx.org/download/nginx-1.12.2.tar.gz
tar -xzvf nginx-1.12.2.tar.gz 
sudo apt-get install git
git clone https://github.com/arut/nginx-rtmp-module.git
```
3.编译并安装nginx(先安装依赖，然后记得添加nginx-rtmp-moudle)
```
sudo apt-get install gcc make
sudo apt-get install libpcre3 libpcre3-dev openssl libssl-dev zlib1g zlib1g-dev
./configure --prefix=/usr/local/nginx  --add-module=../nginx-rtmp-module  --with-http_ssl_module
make
make install
```
4.修改nginx配置文件
```
cd /usr/local/nginx/conf
mv nginx.conf nginx.conf.bak
vim nginx.conf
```
nginx.conf
```
worker_processes  1;
events {
    worker_connections  1024;
}
http {
    include       mime.types;
    default_type  application/octet-stream;
    sendfile        on;
    #tcp_nopush     on;
    #keepalive_timeout  0;
    keepalive_timeout  65;
    server {
        listen       80;
        server_name  localhost;
        location / {
            root   html;
            index  index.html index.htm;
        }
        location /hls {
            # 此处是为了跨域读取视频
            add_header 'Access-Control-Allow-Origin' '*';
            alias /tmp/app;
        }
        #error_page  404              /404.html;
        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
    }
}
```
最后，运行nginx
`/usr/local/nginx/sbin/nginx`
因为涉及到网页服务器和视频服务器不在同一域名下，所以采用的是跨域访问。 
5.安装ffmpeg（进行转码和切片）
```bash
sudo apt-get install ffmpeg
```
6.将视频源转码切片后，放到/tmp/app/下 
首先要创建该文件夹
`mkdir /tmp/app/`
然后执行以下2条语句中的任意一条。 
第一条语句是不进行转码，直接切片，对CPU的要求小些，画质也很高，但是对带宽的要求也不小。 
我采用的是第二条语句，将视频转码为846x480的分辨率然后再切片。 
我采用的摄像头是海康威视的，他的rtsp协议就是像下面这样，具体的可以咨询各个厂家。 
admin 用户名 xxxxxxxx 密码 192.168.1.64 IP地址
```
sudo ffmpeg -i rtsp://admin:xxxxxxxx@192.168.1.64:554/h264/ch1/main/av_stream -fflags flush_packets -max_delay 1 -an -flags -global_header -hls_time 1 -hls_list_size 3 -hls_wrap 3 -vcodec copy -y /tmp/app/channel101.m3u8
sudo ffmpeg -i rtsp://admin:xxxxxxxx@192.168.1.64:554/h264/ch1/main/av_stream -fflags flush_packets -max_delay 1 -an -flags -global_header -hls_time 1 -hls_list_size 3 -hls_wrap 3 -s 846x480 -y /tmp/app/channel101.m3u8
```
到目前为止，如果一切顺利的话，就可以通过VLC等播放码流的软件观看了。 
可以尝试用VLC打开[http://localhost/hls/channel101.m3u8](http://localhost/hls/channel101.m3u8)，进行观看。 
localhost要用成自己的IP。 
VLC软件还是很强大的，不仅可以播放hls协议的视频，还可以播放RTSP协议的。 
所以，如果要测试自己的摄像头是否出了问题，也可以用VLC打开摄像头的。
#### 用chimee编写播放页面
chimee是奇舞团视频云前端组研发的视频播放框架，奇舞团是360的前端团队，之前的水滴直播采用的就是chimee框架完成的。chimee支持PC端和移动端，而且还支持多种格式，很强大。 
PC端代码：
```xml
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width">
        <title>Player</title>
        <style>
            .chimee-container {
                position: relative;
            }
            .time-ad {
                background-color: transparent;
                border: 0;
            }
            .time-ad img {
                display: block;
                width: 30px;
            }
            .time-ad em {
                position: absolute;
                right: -7px;
                top: 35px;
                width: 60px;
                text-align: center;
                font-size: 12px;
                background-color: rgba(156, 85, 85, 0.66);
                padding: 0 3px;
                z-index: -1;
                border-radius: 5px;
            }
            .time-ad cm-pp-close {
                display: none;
            }
            .time-ad cm-pp-body {
                padding: 0;
            }
        </style>
    </head>
    <body>
        <div id="wrapper" style="width:80%;margin:auto" class="chimee-container">
            <video tabindex="-1"></video>
        </div>
        <script src="http://lib.baomitu.com/chimee-player/1.1.11/chimee-player.browser.js"></script>
        <script>
        new ChimeePlayer({
              wrapper: '#wrapper',  // video dom容器
              src: 'http://192.168.1.191/hls/channel101.m3u8',
              box: 'hls',
              isLive: true,
              autoplay: true,
              controls: true
            });
        </script>
    </body>
</html>
```
移动端代码需要到[https://github.com/Chimeejs/chimee-mobile-player](https://github.com/Chimeejs/chimee-mobile-player)下载lib文件夹下的代码，与该代码置于同一文件夹下。
```xml
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width">
        <title>Player</title>
        <style>
            .chimee-container {
                position: relative;
            }
            .time-ad {
                background-color: transparent;
                border: 0;
            }
            .time-ad img {
                display: block;
                width: 30px;
            }
            .time-ad em {
                position: absolute;
                right: -7px;
                top: 35px;
                width: 60px;
                text-align: center;
                font-size: 12px;
                background-color: rgba(156, 85, 85, 0.66);
                padding: 0 3px;
                z-index: -1;
                border-radius: 5px;
            }
            .time-ad cm-pp-close {
                display: none;
            }
            .time-ad cm-pp-body {
                padding: 0;
            }
        </style>
    </head>
    <body>
        <section id="wrapper" class="chimee-container">
            <video tabindex="-1"></video>
        </section>
        <script src="./lib/chimee-mobile-player.browser.js" ></script>
        <script>
        new ChimeeMobilePlayer({
            wrapper: '#wrapper',  // video dom容器
            src: 'http://192.168.1.191/hls/channel101.m3u8',
            isLive: true,
            autoplay: true,
            controls: true,
            playsInline: true,
            preload: true,
            x5VideoPlayerFullscreen: true,
            x5VideoOrientation: true,
            xWebkitAirplay: true,
            muted: true
        });
        </script>
    </body>
</html>
```
以上均为基础的代码，更加详细的功能请查看chimee的官方文档，更精美的前台界面请自行设计。 
服务器和客户端其实在搭建以后是非常好搭的，但是第一次搭建会遇到很多坑，我在写博客的时候也尽量把这些坑避开了。但是难免会遇到新坑，或者遇到问题，那么遇到问题请留言，我经常上博客，所以会回复大家。 
PS:搭建视频服务器时，尽量避免使用docker，可能是性能不够？在利用ffmpeg对视频进行转换切片时，会报错很多。
