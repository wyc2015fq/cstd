# 用nginx搭建基于rtmp或者http的flv、mp4流媒体服务器 - DoubleLi - 博客园






http://itindex[.NET](http://lib.csdn.net/base/dotnet)/detail/48702-nginx-rtmp-http



一、流媒体播放方式 

1、  HTTP方式 

这种方式要下载FLV视频文件到本地播放，一旦FLV视频文件下载完成，就不会消耗服务器的资源和带宽，但是拖动功能没有RTMP/RTMP流媒体方式强大，很多视频网站都是用HTTP方式实现的，如：YouTube，土豆，酷6等 

2、  RTMP/RTMP流媒体方式 

这种方式不用下载FLV视频文件到本地，可以实时的播放flv文件，可以任意拖拽播放进度条，但是比较消耗服务器的资源。 

二、使用nginx来搭建flv流媒体服务器 
1.安装git 
yum install git 

2.安装依赖包 
yum -y install gcc glibc glibc-devel make nasm pkgconfig lib-devel openssl-devel expat-devel gettext-devel libtool mhash.x86_64 perl-Digest-SHA1.x86_64   

3、安装ffmpeg及其依赖包（我的依赖包安装在/usr/local/src下，不过安装位置可以根据个人习惯而定） 
以下安装包能用yum安装的，尽量用yum来安装，没有的再wget来获得。 

#wget http://www.tortall.net/projects/yasm/releases/yasm-1.2.0.tar.gz 
#tar xzvf yasm-1.2.0.tar.gz 
#cd yasm-1.2.0 
#./configure 
#make 
#make install 
#cd .. 

#git clone git://git.videolan.org/x264 
#cd x264 
#./configure --enable-shared 
#make 
#make install 
#cd .. 

#wget http://downloads.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz 
#tar xzvf lame-3.99.5.tar.gz 
#cd lame-3.99.5 
#./configure --enable-nasm 
#make 
#make install 
#cd .. 

#yum install libogg 
注：libogg根据网上各版本教程都是用的.tar.gz包来安装,不过安装后再安装libvorbis，libvpx等会发现找不到libogg安装位置，导致安装失败。 

#wget http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.3.tar.gz 
#tar xzvf libvorbis-1.3.3.tar.gz 
#cd libvorbis-1.3.3 
#./configure 
#make 
#make install 
#cd .. 

#git clone http://git.chromium.org/webm/libvpx.git 
#cd libvpx 
#./configure  --enable-shared 
#make 
#make install 
#cd .. 

#wget http://downloads.sourceforge.net/project/faac/faad2-src/faad2-2.7/faad2-2.7.tar.gz 
#tar zxvf faad2-2.7.tar.gz 
#cd faad2-2.7 
#./configure 
#make 
#make install 
#cd .. 

#wget http://downloads.sourceforge.net/project/faac/faac-src/faac-1.28/faac-1.28.tar.gz 
#tar zxvf faac-1.28.tar.gz 
#cd faac-1.28 
#./configure 
#make 
#make install 
#cd .. 

#wget http://downloads.xvid.org/downloads/xvidcore-1.3.2.tar.gz 
#tar zxvf xvidcore-1.3.2.tar.gz 
#cd xvidcore/build/generic 
#./configure 
#make 
#make install 
#cd .. 

#git clone git://source.ffmpeg.org/ffmpeg 
#cd ffmpeg 
#./configure  --prefix=/opt/ffmpeg/ --enable-version3  --enable-libvpx --enable-libfaac --enable-libmp3lame  --enable-libvorbis --enable-libx264 --enable-libxvid --enable-shared --enable-gpl --enable-postproc --enable-nonfree  --enable-avfilter --enable-pthreads 
#make && make install 
#cd .. 

修改/etc/ld.so.conf如下: 
include ld.so.conf.d/*.conf 
/lib 
/lib64 
/usr/lib 
/usr/lib64 
/usr/local/lib 
/usr/local/lib64 
/opt/ffmpeg/lib 

三、安装nginx模块 
#wget http://h264.code-shop.com/download/nginx_mod_h264_streaming-2.2.7.tar.gz 
#tar zxvf nginx_mod_h264_streaming-2.2.7.tar.gz 

#git clone git://github.com/arut/nginx-rtmp-module.git 

#yum install pcre 
nginx没有找到地址，只能手动下载，然后上传了。 
#tar zxvf nginx-1.2.6.tar.gz 
#cd nginx-1.2.6 
#./configure --prefix=/usr/local/nginx  --add-module=../nginx_mod_h264_streaming-2.2.7 --with-http_flv_module --with-http_gzip_static_module --with-http_stub_status_module --with-http_mp4_module --add-module=../nginx-rtmp-module --add-module=../nginx-rtmp-module/hls --with-cc-opt=-I/opt/ffmpeg/include --with-ld-opt='-L/opt/ffmpeg/lib -Wl,-rpath=/opt/ffmpeg/lib' 
#make && make install 

安装yamdi 
yadmi的作用是为flv文件添加关键帧，才能实现拖动播放 
#下载yadmi 
wget http://sourceforge.net/projects/yamdi/files/yamdi/1.4/yamdi-1.4.tar.gz/download 
#安装yadmi 
tar xzvf yamdi-1.4.tar.gz 
cd yamdi-1.4 
make && make install 
使用方法：yamdi -i input.flv -o out.flv 
给input.flv文件 添加关键帧，输出为out.flv文件 

四、修改nginx主配置文件 
vi /usr/local/nginx/conf/nginx.conf 

user  www www; 

worker_processes 30; 

error_log  /usr/local/nginx/logs/error.log  crit; 

pid        /usr/local/nginx/logs/nginx.pid; 



events { 

        use epoll; 

        worker_connections    65535; 

        } 

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


server { 

        listen       80; 
        server_name  192.168.1.105; 
        root  /usr/local/nginx/html/; 
        limit_rate_after 5m;    ####在flv视频文件下载了5M以后开始限速 
         limit_rate 512k;        ####速度限制为512K 
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

五、启动nginx 
启动前检测nginx.conf文件配置 
/usr/local/nginx/sbin/nginx -t -c /usr/nginx/conf/nginx.conf 
或者 
/usr/nginx/sbin/nginx -t 
如果出错根据报错行数进行改正。 
如果报错：[error]: invalid PID number "" in "/var/run/nginx.pid" 
usr/local/nginx/sbin/nginx -c /usr/local/nginx/conf/nginx.conf 


nginx启动 
/usr/local/nginx/sbin/nginx -s reload 

测试 
http://192.168.1.105/player.swf?type=http&file=test1.flv 
（其中player.swf为播放器，test1.flv为视频文件） 
播放器要和视频文件放在一起。 
视频文件位置根据nginx.conf中配置位置对应。 



本文附件下载:
- [play.rar](http://dl.iteye.com/topics/download/710d1fa2-713f-3841-819e-32b00f84c9e6) (89 KB)
- [nginx-0.8.55.tar.gz](http://dl.iteye.com/topics/download/d2fee537-6a3f-3fb9-921f-51b353bf161f) (632.6 KB)









