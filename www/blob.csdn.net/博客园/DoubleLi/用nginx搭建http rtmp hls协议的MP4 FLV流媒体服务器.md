# 用nginx搭建http/rtmp/hls协议的MP4/FLV流媒体服务器 - DoubleLi - 博客园






前前后后搭建了两三个星期，终于可以告一段落，nginx实在是有点强大。写一篇笔记来记录一下这个过程中的思路和解决方案。

一.搭建nginx平台：

基本是基于http://blog.csdn.net/xiaoliouc/article/details/8363984 一步步安装nginx搭建流媒体服务器 这篇博客来搭建。

我的ubuntu是14.04LTS。各种包的版本是：

nginx-1.9.9

nginx_mod_h264_streaming-2.2.7.tar.gz

openssl-0.9.8zh.tar.gz

pcre-8.12.tar.gz

zlib-1.2.7.tar.gz

为了支持rtmp，还下载了一个模块nginx-rtmp-module-master，没有版本的区别。

在安装过程中的注意事项：

1.上述无非是./configure make make install.

2.记住如果要修改nginx的./configure的话只用make无需install因为不是覆盖安装。

3.要在sbin的目录下检查./nginx -V 才能看到nginx的配置。

4.nginx在make的时候注意把objs里面的Makefile的权限改成a+w，并且将-Werror删掉，这样就不会把warning看成error来对待。修改makefile是在configure之后make之前。

二.测试http、rtmp、hls的功能

1.配置文件（仅支持http和rtmp）

user  www www;  ##管理用户

worker_processes 30;    ##后台进程

error_log  /usr/local/nginx/logs/error.log;

##nginx错误日志存放路径

pid        /usr/local/nginx/logs/nginx.pid;

events {

        use epoll;

##轮训方式

        worker_connections      65535;

##允许的最大连接数

        }

http {

    include       mime.types;

    default_type  application/octet-stream;



    log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '

                      '$status $body_bytes_sent "$http_referer" '

                      '"$http_user_agent" "$http_x_forwarded_for"';



    access_log  /usr/local/nginx/logs/access.log;



    sendfile        on;

    tcp_nopush     on;



    #keepalive_timeout  0;

    keepalive_timeout  65;



    gzip on;
    gzip_min_length 1000;
    gzip_buffers 4 8k;
    gzip_types text/plain;



    server {

        listen       80;

        server_name  192.168.16.69;
     #root html;
        root /usr/local/nginx/html/flv_file;


        #charset koi8-r;

        limit_rate_after 5m;

        limit_rate  512k;
     index  index.html;
         charset utf-8;

     #   access_log  /usr/local/nginx/logs/host.access.log  main;



    #    location / {

        #    root   /usr/local/nginx/html/flv_file;

   #            index  index.html;

           # limit_rate_after 5m;

           # limit_rate  512k;



        #error_page  404              /404.html;

       location ~ \.flv$ {    
     #     root /var/www/flv;
                       flv;
                        }

       location ~ \.mp4$ {
     #          root /var/www/mp4;
                        mp4;
                        }
}
}
rtmp {
        server {
            listen 1935;
            chunk_size 4000;

            # video on demand
            application vod {
                play /usr/local/nginx/html/flv_file;
            }

            # HLS
            # HLS requires libavformat & should be configured as a separate
            # NGINX module in addition to nginx-rtmp-module:
            # ./configure … –add-module=/path/to/nginx-rtmp-module/hls …
            # For HLS to work please create a directory in tmpfs (/tmp/app here)
            # for the fragments. The directory contents is served via HTTP (see
            # http{} section in config)
            #
            # Incoming stream must be in H264/AAC/MP3. For iPhones use baseline #H264
            # profile (see ffmpeg example).
            # This example creates RTMP stream from movie ready for HLS:
            #
            # ffmpeg -loglevel verbose -re -i movie.avi  -vcodec libx264
            #    -vprofile baseline -acodec libmp3lame -ar 44100 -ac 1
            #    -f flv rtmp://localhost:1935/hls/movie
            #
            # If you need to transcode live stream use ‘exec’ feature.
            #
            application hls {
                hls on;
                hls_path /usr/local/nginx/html/flv_file;
                hls_fragment 10s;
            }
        }
    }
2.配置文件（可支持http，rtmp，hls）

  #filename:nginx.conf


    #user  nobody;
    worker_processes  1;

    error_log  logs/error.log;
    #error_log  logs/error.log  notice;
    #error_log  logs/error.log  info;

    pid        logs/nginx.pid;

    events {
        use epoll;
        worker_connections  1024;
    }

    rtmp {
        server {
            listen 1935;
            chunk_size 4000;

            # video on demand
            application vod {
                play /usr/local/nginx/html/flv_file;
            }

            # HLS
            # HLS requires libavformat & should be configured as a separate
            # NGINX module in addition to nginx-rtmp-module:
            # ./configure … –add-module=/path/to/nginx-rtmp-module/hls …
            # For HLS to work please create a directory in tmpfs (/tmp/app here)
            # for the fragments. The directory contents is served via HTTP (see
            # http{} section in config)
            #
            # Incoming stream must be in H264/AAC/MP3. For iPhones use baseline #H264
            # profile (see ffmpeg example).
            # This example creates RTMP stream from movie ready for HLS:
            #
            # ffmpeg -loglevel verbose -re -i movie.avi  -vcodec libx264
            #    -vprofile baseline -acodec libmp3lame -ar 44100 -ac 1
            #    -f flv rtmp://localhost:1935/hls/movie
            #
            # If you need to transcode live stream use ‘exec’ feature.
            #
            application hls {
                hls on;
                hls_path /usr/local/nginx/html/flv_file;
                hls_fragment 10s;
            }
        }
    }

    http {

        include mime.types;
        default_type application/octet-stream;
        sendfile on;
        keepalive_timeout 65;
        gzip on;

         #log format

        log_format  access  ‘$remote_addr – $remote_user [$time_local] “$request” ‘
                 ‘$status $body_bytes_sent “$http_referer” ‘
                 ‘”$http_user_agent” $http_x_forwarded_for’;   

         #定义一个名为addr的limit_zone,大小10M内存来存储session
        limit_conn_zone $binary_remote_addr zone=addr:10m;    

        server {
            listen 8080;
             server_name localhost;

             # HTTP can be used for accessing RTMP stats
            # This URL provides RTMP statistics in XML
            location /stat {
                rtmp_stat all;
                rtmp_stat_stylesheet stat.xsl;
            }
            location /stat.xsl {
                root /home/nairely/Documents/nginxserver/nginx-rtmp-module-master;
            }
             location /control {
                rtmp_control all;
            }
            location / {
                root /home/nairely/Documents/nginxserver/nginx-rtmp-module-master/test/rtmp-publisher;
            }
        }

         server {
            listen 80;
             server_name localhost;

            location / {
                    root /usr/local/nginx/html/flv_file;
                    index index.html;
                  }        

             location ~ \.flv$ {
                   root /usr/local/nginx/html/flv_file;
                 flv;
                 limit_conn addr 20;
                 limit_rate 200k;
            }
            location ~ \.mp4$ {
                 root /usr/local/nginx/html/flv_file;
                 mp4;
                 limit_conn addr 20;
                 limit_rate 200k;
            }

             location /hls {
                # Serve HLS fragments
                alias /usr/local/nginx/html/flv_file;
            }

             access_log  logs/nginxflv_access.log access;
        }      
    }

3.如果视频不能播放很有可能是权限的问题，设置成644. root(read and write),group(read only)user(read only)

4.nginx作为一个后台服务器，用户从html/flv_file的文件夹中取出视频用户点播。测试的播放器用的是vlc。

http的地址是http://192.168.16.46/yequ.flv

rtmp的地址是rtmp://192.168.16.46:1935/vod/yequ.flv

hls的地址是http://192.168.16.46/hls/sample.m3u8(在segment命令将output的URL设置为http://192.168.16.46/hls)将sample.m3u8文件放在html/flv_file文件夹下，当然这取决与segment语句在哪个地址下进行。



5.如何启动nginx：

cd /usr/local/nginx/sbin

./nginx -t 检查配置文件的语法问题

如果发现配置文件的路径不对

./nginx -c 路径

./nginx -s reload重新加载配置文件

killall -9 nginx 杀掉所有进程从头开始，如果遇到端口被占用的问题的话。

./nginx 启动，注意在杀掉进程之后一定要重新启动。

service nginx start





三.配置hls+ffmpeg的环境

在配置http和rtmp都比较简单。在搭建nginx平台的前提下就可以实现点播。配置hls出现的问题都比较简单。只是会花一段时间。



主要的配置还是根据这篇来的http://www.lc365.net/blog/b/31519/ 基于HTTP Live Streaming（HLS） 搭建在线点播系统

这些包实在太多了，我简直属于无聊的阶段。

1.faac的编译问题

[http://zhidao.baidu.com/link?url=ASyVwiBE-01ox_O0QascgPdqNNRlXpHCfI6cXyg71JIboOK5MTj3NLfHUPC31HH5b0FiE3tbWUetUfKL29HAzzXu4q0p75Iveu05HPp_ST3](http://zhidao.baidu.com/link?url=ASyVwiBE-01ox_O0QascgPdqNNRlXpHCfI6cXyg71JIboOK5MTj3NLfHUPC31HH5b0FiE3tbWUetUfKL29HAzzXu4q0p75Iveu05HPp_ST3)
2.在编译ffmpeg的时候发现x264notfound的解决方法

在x264的./configure --enable-static --enable-shared --enable-visualize --system-libx264 --enable-gprof --enable-strip --enable-pic --enable-debug

就是把所以的开关都打开。这简直是误打误撞。

3.**m3u8**configure的时候总是会遇到error的问题，可以看到这些错误都是什么东西过时的啥的，直接换ffmpeg。换ffmpeg版本到最新。ffmpeg-2.8.4


git clone https://github.com/johnf/m3u8-segmenter

cd m3u8-segmenter

然后configure的时候

gcc -Wall -g m3u8-segmenter.c -o segmenter -lavformat -lavcodec -lavutil

cp segmenter /usr/bin/ 

在html/flv_file路径下

ffmpeg -i /var/flvs/baluobu.flv  -f mpegts -acodec libmp3lame -ar 48000 -ab 128k -vcodec libx264 -b 96k -flags +loop -cmp +chroma -partitions +parti4x4+partp8x8+partb8x8 -subq 5 -trellis 1 -refs 1 -coder 0 -me_range 16 -keyint_min 25 -sc_threshold 40 -i_qfactor 0.71 -bt 200k -maxrate 96k -bufsize 96k -rc_eq 'blurCplx^(1-qComp)' -qcomp 0.6 -qmin 10 -qmax 51 -qdiff 4 -level 30 -aspect 320:240 -g 30 -async 2 /var/app/baluobu/baluobu.ts

上述是把flv文件转换成ts文件

下面用segmenter命令将ts转换成10秒一个ts切片，segmenter_test是ts小切片的前缀,-u制定URL地址。

segmenter -i vp5.ts -d 10 -p segmenter_test -m vp5.m3u8 -u [http://192.168.16.46/hls/](http://192.168.16.46/hls/)

在segmenter的时候有一个红色的ignore，之前一直认为是一个error，我去才发现是一个不用管的东西，然后在vlc上用http://192.168.16.46/hls/vp5.m3u8放，其实类似一个直播的功能，播完了就没有ts文件了。

再来一次测试就得再来一遍ffmpeg和segmenter.

总结到这儿。









