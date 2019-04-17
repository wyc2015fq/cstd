# nginx搭建基于http协议的视频点播服务器 - DoubleLi - 博客园






1，于由自己的服务器上已经安装好nginx(具体安装方法见我的另一篇文章，Linux中安装nginx)，所以不再安装。



2，下载nginx_mod_h264_streaming-2.2.7.tar.gz(自己在网上搜吧)。



3，安装pcre，先看有没有安装。



     [root@localhost vsftpd]# rpm -qa pcre
     pcre-7.8-7.el6.i686



     已经安装(上篇加rtmp模块的时候已经安装了)



4，解压nginx_mod_h264_streaming-2.2.7.tar.gz，添加flv，mp4模块。解压到和nginx源码同级的目录。然后./configure --add-module=../nginx_mod_h264_streaming-2.2.7 --with-http_flv_module --with-http_mp4_module

      因为flv 和http是nginx自带的所以不用下载这个模块。





5，make

      可能遇到问题1
      /root/nginx_mod_h264_streaming-2.2.7/src/ngx_http_streaming_module.c: In function ‘ngx_streaming_handler’:
     /root/nginx_mod_h264_streaming-2.2.7/src/ngx_http_streaming_module.c:158: error: ‘ngx_http_request_t’ has no member named ‘zero_in_uri’
     make[1]: *** [objs/addon/src/ngx_http_h264_streaming_module.o] Error 1
     make[1]: Leaving directory `/root/nnginx-1.5.9'
     make: *** [build] Error 2
     那么将src/ngx_http_streaming_module.c文件中以下代码删除或者是注释掉就可以了



6, make install







7,配置 nginx.conf





  #user  nobody;
worker_processes  1;



#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;



#pid        logs/nginx.pid;




events {
    worker_connections  1024;
}



#include  nginx.conf-rtmp;



http {



        include       mime.types;



        default_type  application/octet-stream;



        log_format main  '$remote_addr - $remote_user [$time_local] '



                                                '"$request" $status $bytes_sent '



                                                '"$http_referer" "$http_user_agent" '



                                                '"$gzip_ratio"';



        keepalive_timeout  60;



        server_names_hash_bucket_size  128;



        client_header_buffer_size    32k;







        large_client_header_buffers  4 32k;







        access_log off;



        gzip on;



        gzip_min_length  1100;



        gzip_buffers     4 8k;



        gzip_types       text/plain;







        output_buffers   1 32k;



        postpone_output  1460;







        client_header_timeout  3m;



        client_body_timeout    3m;



        send_timeout           3m;







        sendfile                on;



        tcp_nopush              on;



        tcp_nodelay             on;







######################################################################







    server {

            listen       8999;

            server_name  192.168.1.104;

            root    /usr/local/nginx/html/flv_file/;

            limit_rate_after 5m;   
            limit_rate 512k;        


            index   index.html;

            charset utf-8;

            location ~ \.flv {

                flv;

            }

            location ~ \.mp4$ {
                     mp4;
            }


            error_page   500 502 503 504  /50x.html;

            location = /50x.html {

                root   html;

            }
    }



}



8,启动nginx,创建/usr/local/nginx/html/flv_file/目录。







9,上传mp4视频文件CY.mp4。





10,写个html测试。看之前的文章用jwplayer来测试，但是我这边一直报jwplayer udefined。



<!DOCTYPE html>
<div id="container">Loading the player ...</div>
<video src="http://192.168.1.104:8999/CY.mp4" controls="controls" width="50%" height="50%">
</video>











