# 利用Nginx搭建RTMP视频直播，点播服务器，ffmpeg推流，回看 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月03日 15:50:54[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：3649

一、环境和工具
- ubuntu 14.04 desktop
- 不用server的原因是一部分的演示用到了linux视频播放和直播软件，自己还要装桌面，麻烦。
- 不建议使用 最新的16TLS，我一开始就用它来做的，但是各种坑，因为很多包都被废弃或者必须降级
- Nginx1.8.1
- 这个是我之前装的版本，版本没什么大的影响
- 我是源代码编译安装的，不建议用 apt-get install nginx
- nginx-rtmp-module
- github地址：[https://github.com/arut/nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module)
- 直播肯定用大名鼎鼎的OBS
- OBS地址： [https://obsproject.com/](https://obsproject.com/)
- 观看目前只是用客户端观看，使用的是VLC media player
二、安装Ngnix
1、安装各种依赖包
$ apt-get  install openssl libssl-dev libpcre3 libpcre3-dev
yum install -y pcre-devel zlib-devel   *openssl openssl-devel*
2、编辑安装nginx与rtmp-module
nginx和nginx-rtmp-module是一起编译安装的。
**编译安装Nginx**
[http://nginx.org/download/nginx-1.14.0.tar.gz](http://nginx.org/download/nginx-1.14.0.tar.gz)
#注意nginx-rtmp-module-master的路径
$ ./configure --add-module=../nginx-rtmp-module-master --prefix=/opt/nginx --with-http_ssl_module
$ make
$ make install
sbin/nginx --options
三、配置直播服务器
所有的配置都基于 nginx-rtmp-module 的官方文档
github地址如下： [https://github.com/arut/nginx-rtmp-module#nginx-based-media-streaming-server](https://github.com/arut/nginx-rtmp-module#nginx-based-media-streaming-server)
因为我们这里只考虑直播，因此点播就不在记录了。
关于**视频点播服务器的配置**可以查看上面作者的文章：[[http://blog.csdn.net/kingroc/article/details/50839994](http://blog.csdn.net/kingroc/article/details/50839994)] [9]
视频点播服务器和视频直播的不同我想不需要说明，一个实时推流，一个是也是播放视频流，但是是基于文件进行的。点播无非是通过播放某服务器上的文件进行推流，从而能够观看，而直播是有专门推流客户端进行实时推流。
我把我的**nginx.conf的配置贴出来，需要注意的就是我监听的是一个域名，而不是ip，因为域名我是为了在局域网使用。**
- 我的ubuntu IP是 192.168.124.129,hosts文件将rtmp-server指向了这个ip。
- 同时windows的hosts文件我也改过，同样将rtmp-server指向了192.168.124.129，这样子好处是，很轻松在windows上进行推流和观看
- rtmp上最重要的配置就是 push rtmp://rtmp-server/live;（详细查看下面注释）
#配置RTMP，这个配置格式在github的readme上有详细说明rtmp{                    server {        listen 1935;#服务端口--默认       chunk_size 4096;#数据传输块的大小--默认
#设置直播的application名称是 live    	application live{         live on;#live on表示开启直播模式}
#设置推流的应用名称    application push{         live on;#开启直播        push rtmp://rtmp-server/live;#推流到上面的直播应用}}}
如果打算弄多个频道的直播就可以live_cctv1、live_cctv2名字任意。
#下面的server是在http的一级配置标签下的#上面的注释对懂nginx的人是废话，但是如果你不熟悉nginx，建议认真看看http{...# 这里有一些其他的配置 server {        listen       80;#端口    server_name  rtmp-server;#设置http服务器监听的域名 hosts中配置了#下面两个是加上去的，用来配置直播的http访问#具体的意思可以在github的文档中查看    location /stat {                    rtmp_stat all;               rtmp_stat_stylesheet stat.xsl;}        location /stat.xsl {#注意这里的路径不能错误，直接写绝对路径就可以        root /usr/local/nginx-rtmp-module-master/; 也可以把这个文件复制到html目录下，不然有可能因为权限打不开}        location / {            root   html;            index  index.html index.htm;}        error_page   500 502 503 504  /50x.html;        location = /50x.html {            root   html;}}}
配置完之后，需要重新启动nginx

然后在浏览器输入 [http://rtmp-server/stat](http://rtmp-ostbird/stat) ,stat就是上面server中配置的名称
如果能看到下面图片的内容，则证明配置没有问题，一般没啥问题，我是一次就成功的。如果无法访问，请关闭防火墙以及查看hosts文件等等。
![](https://note.youdao.com/src/57CC60C9408C43B487F077944ED8ED27)
点播配置
rtmp {                #RTMP服务server {        listen 1935;  #//服务端口     chunk_size 4096;   #//数据传输块的大小    application vod {        play /opt/media/vod; #//视频文件存放位置。    }    }}
可以在上面目录放一个文件，如 starRTC.mp4
则打开地址为 *rtmp://localhost/vod/starRTC.mp4*
点播不使用RTMP插件nginx自身也是可以实现点播服务的。那就是配置location部分，由于下面我们要配置直播和回看功能所以选用了RTMP服务。
四、进行直播,推流
下面是在windows上进行直播，
 在linux下直播和观看当然也没有任何的问题：
- [在linux上安装obs直播（需要ffmpeg）：http://blog.csdn.net/kingroc/article/details/50829213](http://blog.csdn.net/kingroc/article/details/50829213)
- [在linux上使用OBS：http://blog.csdn.net/kingroc/article/details/50839994#t4](http://blog.csdn.net/kingroc/article/details/50839994#t4)
1、下载OBS并进行安装
2、使用OBS直播
打开之后，在来源那里点击加号，选择一个来源，比如我选择了摄像头，
选择文件可以：
![](https://note.youdao.com/src/783BBE22A8D74483BBB7D8713B1BA9A6)
![](https://note.youdao.com/src/212A7F1A95B140C6B240B2CCE49FEA54)
3、配置串流服务
- 点击【设置】
- 选择【串流】
- 串流类型选择【自定义流媒体服务器】
- URL填写我们上面配置的push地址【rtmp://rtmp-server/push】
- 密钥没有配置，不需要填写。
- 【完成】
- 【开始串流】

FFmpeg 推流
循环推流：
ffmpeg -threads 2 -re -fflags +genpts -stream_loop -1 -i /mnt/test.mp4   -c:v copy -c:a aac -f flv rtmp://172.2.8.31:1935/live/test2
-re : 表示使用文件的原始帧率进行读取，因为ffmpeg读取视频帧的速度很快，如果不使用这个参数，ffmpeg可以在很短时间就把video.mp4中的视频帧全部读取完并进行推流，这样就无法体现出视频播放的效果了。官方文档中对这个参数的解释是：
-re (input)
Read input at native frame rate. Mainly used to simulate a grab device, or live input stream (e.g. when reading from a file). Should not be used with actual grab devices or live input streams (where it can cause packet loss). By default ffmpeg attempts to read the input(s) as fast as possible. This option will slow down the reading of the input(s) to the native frame rate of the input(s). It is useful for real-time output (e.g. live streaming).
ffmpeg -y -re -fflags +genpts -stream_loop -1 -probesize 1280 -i test.mp4 -c copy -f flv rtmp_url, 加黄色选项后重复播放时不会卡，会比较连贯一些。
五、观看直播
服务器有没有接收到呢？打开我的服务器地址“[http://localhost/stat](http://localhost/stat)”查看一下 ，
![](https://note.youdao.com/src/0A760FF0C746455E8E22075B22A029FC)
直播使用VLC进行观看，其实很简单就是填一个链接就OK。
- 打开VLC media player，
- 选择【媒体】->【网络串流】
- 在地址输入：rtmp://rtmp-server/live（之前配置的live的地址）
- 确认后即可观看
或者用： ffplay rtmp:*//localhost/videotest*
**实时回看**
如果直播服务能够把节目录制在本地，就可以直接回看先前的节目了
rtmp配置部分：
application live {        live on;        hls on; #这个参数把直播服务器改造成实时回放服务器。        wait_key on; #对视频切片进行保护，这样就不会产生马赛克了。        hls_path /opt/video/hls; #切片视频文件存放位置。        hls_fragment 10s;     #每个视频切片的时长。        hls_playlist_length 60s;  #总共可以回看的事件，这里设置的是1分钟。        hls_continuous on; #连续模式。        hls_cleanup on;    #对多余的切片进行删除。        hls_nested on;     #嵌套模式。        }
http部分
 location /stat.xsl {        root /usr/local/nginx/nginx-rtmp-module/;    }    location /live {  #这里也是需要添加的字段。        types {              application/vnd.apple.mpegurl m3u8;              video/mp2t ts;          }        alias /opt/video/hls;           expires -1;        add_header Cache-Control no-cache;      }  
或
application myapp {
           live on;
       }
       application hls {
           live on;
           hls on;
           hls_path /tmp/hls;
       }
由于这次nginx需要向服务器写切片视频文件，但nginx又没有给nginx指定用户名只能走默认的*nobody*用户和*nogroup*用户组，其实就是没有组。所以需要对写入的目录做增大权限的修改。 chmod -R 777  /opt/video/hls
播放视频，“[*http://localhost/live/test/index.m3u8*](http://localhost/live/test/index.m3u8)”。
六、问题
这个只是实现了推流然后能观看，依旧有很多问题。
- 在windows上直播观看延迟大约在5-10秒，如果非局域网这个速度没法看。
- 现在所有人只要填写推流地址，就能进行播放和查看，没有权限控制，虽然rtmp本身对ip有一个策略机制可以配置，但是并没有什么实际的用处，需要结合程序实现控制，也是后面需要实现的功能
- 目前还没有考虑网页上的观看，网页上考虑使用html5流媒体播放器，结合hls协议实现。
nginx配合ffmpeg做流媒体服务器的原理是：nginx通过rtmp模块提供rtmp服务，ffmpeg推送一个rtmp流到nginx，然后客户端通过访问nginx来收看实时视频流。HLS也是差不多的原理，只是最终客户端是通过HTTP协议来访问的,但是ffmpeg推送流仍然是rtmp的。
我的直播部分配置文件 只配置了rtmp和 http统计的部分，用的 rtmp://localhost/live/star就可以播放
![nginx.conf](https://note.youdao.com/src/856B3DFF521647E492F85BA2DF7B0CFE)
