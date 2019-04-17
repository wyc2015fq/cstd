# 基于HTTP Live Streaming（HLS） 搭建在线点播系统 - DoubleLi - 博客园






**1、 ****为何要使用****HTTP Live Streaming**

可以参考wikipedia

HTTP Live Streaming（缩写是 HLS）是一个由苹果公司提出的基于HTTP的流媒体 网络传输协议。是苹果公司QuickTime X和iPhone软件系统的一部分。它的工作原理是把整个流分成一个个小的基于HTTP的文件来下载，每次只下载一些。当媒体流正在播放时，客户端可以选择从许多不同的备用源中以不同的速率下载同样的资源，允许流媒体会话适应不同的数据速率。在开始一个流媒体会话时，客户端会下载一个包含元数据的extended M3U (m3u8) playlist文件，用于寻找可用的媒体流。



HLS只请求基本的HTTP报文，与实时传输协议（RTP)不同，HLS可以穿过任何允许HTTP数据通过的防火墙或者代理服务器。它也很容易使用内容分发网络来传输媒体流。

**2、 ****HTTP Live Streaming****技术方案**

    HTTP服务：使用Nginx提供HTTP服务，通过使用nginx-rtmp-module [https://github.com/arut/nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module) 来增加对HLS的支持

    使用ffmpeg来完成对flv、mp4、mp3等格式的转化

    使用ffmpeg或segmenter完成对视频/音频格式文件的切割，切割为m3u8格式及ts文件

**3、 ****准备工作**

操作系统CentOS

**3.1****、准备安装****删除已安装包**
yum erase ffmpeg x264 x264-devel    
**3.2****、安装各种依赖包**

yum install  gcc make nasm pkgconfig wget curl curl-devel zlib-devel openssl-devel perl cpio expat-devel gettext-devel libtool mhash.x86_64 perl-Digest-SHA1.x86_64  pcre.i386 pcre.x86_64 pcre-devel.i386 pcre-devel.x86_64



**3.3****、安装****git**

wget -O git-devel.zip [https://github.com/msysgit/git/archive/devel.zip](https://github.com/msysgit/git/archive/devel.zip)

unzip git-devel.zip

cd git-devel/

autoconf

./configure

make

make install

**3.4****、创建安装包目录**

mkdir ~/ffmpeg-source



**4、 ****安装****ffmpeg****及其依赖包**

**4.1****、****Yasm**

cd ~/ffmpeg-source

wget [http://www.tortall.net/projects/yasm/releases/yasm-1.2.0.tar.gz](http://www.tortall.net/projects/yasm/releases/yasm-1.2.0.tar.gz)

tar xzvf yasm-1.2.0.tar.gz

cd yasm-1.2.0

./configure

make

make install

**4.2****、****x264**

cd ~/ffmpeg-source

git clone git:// [git.videolan.org/x264](http://git.videolan.org/x264)

cd x264

./configure –enable-shared 

make

make install

**4.3****、****LAME**

cd ~/ffmpeg-source

wget [http://downloads.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz](http://downloads.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz)

tar xzvf lame-3.99.5.tar.gz

cd lame-3.99.5

./configure –enable-nasm

make

make install

**4.4****、****libogg**

cd ~/ffmpeg-source

wget [http://downloads.xiph.org/releases/ogg/libogg-1.3.0.tar.gz](http://downloads.xiph.org/releases/ogg/libogg-1.3.0.tar.gz)

tar xzvf libogg-1.3.0.tar.gz

cd libogg-1.3.0

./configure

make

make install

**4.5****、****libvorbis**

cd ~/ffmpeg-source

wget [http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.3.tar.gz](http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.3.tar.gz)

tar xzvf libvorbis-1.3.3.tar.gz

cd libvorbis-1.3.3

./configure

make

make install

**4.6****、****libvpx**

cd ~/ffmpeg-source

git clone [http://git.chromium.org/webm/libvpx.git](http://git.chromium.org/webm/libvpx.git)

cd libvpx

./configure  –enable-shared

make

make install

**4.7****、****FAAD2**

cd ~/ffmpeg-source

wget [http://downloads.sourceforge.net/project/faac/faad2-src/faad2-2.7/faad2-2.7.tar.gz](http://downloads.sourceforge.net/project/faac/faad2-src/faad2-2.7/faad2-2.7.tar.gz)

tar zxvf faad2-2.7.tar.gz

cd faad2-2.7

./configure

make

make install

**4.8****、****FAAC**

cd ~/ffmpeg-source

wget [http://downloads.sourceforge.net/project/faac/faac-src/faac-1.28/faac-1.28.tar.gz](http://downloads.sourceforge.net/project/faac/faac-src/faac-1.28/faac-1.28.tar.gz)

tar zxvf faac-1.28.tar.gz

cd faac-1.28

./configure

make

make install



**4.9****、****Xvid**

cd ~/ffmpeg-source

wget [http://downloads.xvid.org/downloads/xvidcore-1.3.2.tar.gz](http://downloads.xvid.org/downloads/xvidcore-1.3.2.tar.gz)

tar zxvf xvidcore-1.3.2.tar.gz

cd xvidcore/build/generic

./configure

make

make install



**4.10****、****FFmpeg**

cd ~/ffmpeg-source

git clone git:// [source.ffmpeg.org/ffmpeg](http://source.ffmpeg.org/ffmpeg)

cd ffmpeg

./configure  –enable-version3  –enable-libvpx –enable-libfaac –enable-libmp3lame  –enable-libvorbis –enable-libx264 –enable-libxvid –enable-shared –enable-gpl –enable-postproc –enable-nonfree –enable-avfilter –enable-pthreads

make

make install

  ldconfig –v



**5、 ****安装****nginx****及****nginx-rtmp-module**

mkdir ~/nginx-source

cd  ~/nginx-source

         wget [http://nginx.org/download/nginx-1.2.4.tar.gz](http://nginx.org/download/nginx-1.2.4.tar.gz)

         tar zxvf nginx-1.2.4.tar.gz

         wget -O nginx-rtmp-module.zip  [https://github.com/arut/nginx-rtmp-module/archive/master.zip](https://github.com/arut/nginx-rtmp-module/archive/master.zip)

         unzip nginx-rtmp-module.zip

wget -O ngx_cache_purge.zip [https://github.com/FRiCKLE/ngx_cache_purge/archive/master.zip](https://github.com/FRiCKLE/ngx_cache_purge/archive/master.zip)

unzip ngx_cache_purge.zip

         cd nginx-1.2.4

         ./configure –user=daemon –group=daemon –prefix=/usr/local/nginx-1.2.4/ –add-module=../nginx-rtmp-module-master –add-module=../nginx-rtmp-module-master/hls –add-module=../ngx_cache_purge-master  –with-http_stub_status_module –with-http_ssl_module –with-http_sub_module –with-md5=/usr/lib –with-sha1=/usr/lib –with-http_gzip_static_module





在nginx.conf中增加rtmp模块的相关配置，配置例子

rtmp {



    server {



        listen 1935;



        chunk_size 4000;



        # TV mode: one publisher, many subscribers

        application mytv {



            # enable live streaming

            live on;



            # record first 1K of stream

            record all;

            record_path /tmp/av;

            record_max_size 1K;



            # append current timestamp to each flv

            record_unique on;



            # publish only from localhost

            allow publish 127.0.0.1;

            deny publish all;



            #allow play all;

        }



        # Transcoding (ffmpeg needed)

        application big {

            live on;



            # On every pusblished stream run this command (ffmpeg)

            # with substitutions: $app/${app}, $name/${name} for application & stream name.

            #

            # This ffmpeg call receives stream from this application &

            # reduces the resolution down to 32×32. The stream is the published to

            # ‘small’ application (see below) under the same name.

            #

            # ffmpeg can do anything with the stream like video/audio

            # transcoding, resizing, altering container/codec params etc

            #

            # Multiple exec lines can be specified.



            exec /usr/local/bin/ffmpeg -re -i rtmp://localhost:1935/$app/$name -vcodec flv -acodec copy -s 32×32 -f flv rtmp://localhost:1935/small/${name};

        }



        application small {

            live on;

            # Video with reduced resolution comes here from ffmpeg

        }



        application mypush {

            live on;



            # Every stream published here

            # is automatically pushed to

            # these two machines

            #push [rtmp1.example.com](http://rtmp1.example.com/);

            #push [rtmp2.example.com:1934](http://rtmp2.example.com:1934/);

        }



        application mypull {

            live on;



            # Pull all streams from remote machine

            # and play locally

            #pull rtmp:// [rtmp3.example.com](http://rtmp3.example.com/) pageUrl= [www.example.com/index.html](http://www.example.com/index.html);

        }



        # video on demand

        application vod {

            play /var/flvs;

        }



        application vod2 {

            play /var/mp4s;

        }



        # Many publishers, many subscribers

        # no checks, no recording

        application videochat {



            live on;



            # The following notifications receive all

            # the session variables as well as

            # particular call arguments in HTTP POST

            # request



            # Make HTTP request & use HTTP retcode

            # to decide whether to allow publishing

            # from this connection or not

            on_publish [http://localhost:8080/publish](http://localhost:8080/publish);



            # Same with playing

            on_play [http://localhost:8080/play](http://localhost:8080/play);



            # Publish/play end (repeats on disconnect)

            on_done [http://localhost:8080/done](http://localhost:8080/done);



            # All above mentioned notifications receive

            # standard connect() arguments as well as

            # play/publish ones. If any arguments are sent

            # with GET-style syntax to play & publish

            # these are also included.

            # Example URL:

            #   rtmp://localhost/myapp/mystream?a=b&c=d



            # record 10 video keyframes (no audio) every 2 minutes

            record keyframes;

            record_path /var/vc;

            record_max_frames 10;

            record_interval 2m;



            # Async notify about an flv recorded

            on_record_done [http://localhost:8080/record_done](http://localhost:8080/record_done);



        }





        # HLS



        # HLS requires libavformat & should be configured as a separate

        # NGINX module in addition to nginx-rtmp-module:

        # ./configure … –add-module=/path/to/nginx-rtmp-module/hls …



        # For HLS to work please create a directory in tmpfs (/tmp/app here)

        # for the fragments. The directory contents is served via HTTP (see

        # http{} section in config)

        #

        # Incoming stream must be in H264/AAC/MP3. For iPhones use baseline H264

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

            hls_path /var/app;

            hls_fragment 5s;

        }



    }

}



# HTTP can be used for accessing RTMP stats

http {



    server {



        listen      8080;



        # This URL provides RTMP statistics in XML

        location /stat {

            rtmp_stat all;



            # Use this stylesheet to view XML as web page

            # in browser

            rtmp_stat_stylesheet stat.xsl;

        }



        location /stat.xsl {

            # XML stylesheet to view RTMP stats.

            # Copy stat.xsl wherever you want

            # and put the full directory path here

            root /path/to/stat.xsl/;

        }



        location /hls {

            # Serve HLS fragments

            alias /var/app;

        }



    }

}



**6、 安装segmenter**

svn co [http://httpsegmenter.googlecode.com/svn/](http://httpsegmenter.googlecode.com/svn/)

cd  svn/trunk

gcc -Wall -g segmenter.c -o segmenter -lavformat -lavcodec -lavutil -std=c99

cp segmenter /usr/bin/





从优酷上下载一个视频文件，假定为baluobu.flv

找个mp3文件，假定为10year.mp3

mkdir /var/flvs /var/mp4s /var/vc /var/app /var/app/10year /var/app/baluobu

使用ffmpeg将测试视频和音频转为mpeg ts格式文件

         ffmpeg -i /var/flvs/baluobu.flv  -f mpegts -acodec libmp3lame -ar 48000 -ab 128k -vcodec libx264 -b 96k -flags +loop -cmp +chroma -partitions +parti4x4+partp8x8+partb8x8 -subq 5 -trellis 1 -refs 1 -coder 0 -me_range 16 -keyint_min 25 -sc_threshold 40 -i_qfactor 0.71 -bt 200k -maxrate 96k -bufsize 96k -rc_eq ‘blurCplx^(1-qComp)’ -qcomp 0.6 -qmin 10 -qmax 51 -qdiff 4 -level 30 -aspect 320:240 -g 30 -async 2 /var/app/baluobu/baluobu.ts

         ffmpeg -i /var/flvs/10year.mp3  -f mpegts -acodec libmp3lame -ar 48000 -ab 128k -vcodec libx264 -b 96k -flags +loop -cmp +chroma -partitions +parti4x4+partp8x8+partb8x8 -subq 5 -trellis 1 -refs 1 -coder 0 -me_range 16 -keyint_min 25 -sc_threshold 40 -i_qfactor 0.71 -bt 200k -maxrate 96k -bufsize 96k -rc_eq ‘blurCplx^(1-qComp)’ -qcomp 0.6 -qmin 10 -qmax 51 -qdiff 4 -level 30 -aspect 320:240 -g 30 -async 2 /var/app/10year/10year.ts

         cd /var/app/10year

         segmenter -i 10year.ts -d 5 -o 10year -x 10year.m3u8

         cd /var/app/baluobu

         segmenter -i baluobu.ts -d 5 -o buluobu -x baluobu.m3u8

**7****、测试**

简单起见使用VLC测试。

[http://192.168.1.11:8080/hls/10year/10year.m3u8](http://192.168.1.11:8080/hls/10year/10year.m3u8)

[http://192.168.1.11:8080/hls/baluobu/baluobu.m3u8](http://192.168.1.11:8080/hls/baluobu/baluobu.m3u8)

如果无问题，说明已经正常配置成功了HTTP Live Streaming（HLS）服务

可以再iOS设备上使用safari访问以上地址

或者在android机器上安装vplayer后访问以上地址

如果需要通过网页形式提供在线视频，需要在nginx的mime.types文件中添加如下MIME类型：

.m3u8 application/x-mpegURL 

.ts video/MP2T

同时在HTML5页面中使用video标签包含视频的m3u8文件



<video controls> 

<source src= [http://192.168.1.11:8080/hls/baluobu/baluobu.m3u8](http://192.168.1.11:8080/hls/baluobu/baluobu.m3u8) />

</video>

**8、开发说明**

iOS已经原生支持HTTP Live Streaming（HLS），只需要使用MPMoviePlayerController播放以上地址即可

apple文档： [http://bit.ly/Rnpsef](http://bit.ly/Rnpsef)

Android 3.x以后通过新增的NuPlayer类才支持HTTP Live streaming，而且功能也较弱，为简化处理，可以使用

Vitamio     [http://vov.io/vitamio/](http://vov.io/vitamio/) 或 [http://code.taobao.org/p/oplayer/src/](http://code.taobao.org/p/oplayer/src/) 下载

Servestream [http://sourceforge.net/projects/servestream/](http://sourceforge.net/projects/servestream/)

nginx-rtmp-module 本身也支持RTMP协议，从而也可以基于nginx来搭建在线直播系统。









