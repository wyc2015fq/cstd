# 视频支持拖动进度条播放的实现（基于nginx） - DoubleLi - 博客园






http协议下的flv/mp4流式播放支持的三个要点：

1 服务器端要支持flv/mp4流式播放，现在nginx或者lighttpd都是支持这样的应用的，还支持mp4的流式播放（默认编译版本一般都是打开了flv流式播放，而mp4要自己编译，若需要，也可以自己编译），但要专门配置。

　　参考文章《使用Nginx搭建flv流媒体服务器》

　　http://www.yanghengfei.com/archives/475/



2 播放的flv/mp4有完整的mete信息，其中包括视频分辨率、帧率等信息。有相应的软件对一般的flv上添加这样的标记信息，添加后的也支持一般的应用，而且数据量增加不多

　　参考文章《x264编码器参数完整对照表》

　　http://www.php-oa.com/2009/03/09/ffmpeg_x264.html



3 客户端允许发生请求流式播放的请求，并且能够处理获取的流式数据，从随机点开始播放。

　　如jwplayer支持播放http流媒体。

　　参考文章《JW Player使用简介》

　　http://j-coriolanus.blog.163.com/blog/static/64211038200992011745532/

　　参考文章《基于nginx+jwplayer的flv流媒体》

　　http://www.iinuu.eu/en/it-guru/flv-streaming-using-nginx-and-jw-player-5-1

　　首先配置nginx服务支持flv模块功能

　　⑴查看你的nginx是否已支持flv功能模块

　　[root@localhost ~]# /usr/local/nginx/sbin/nginx -V  //查看nginx服务支持

　　结果：

　　nginx version: nginx/0.8.24

　　built by gcc 4.4.4 20100726 (Red Hat 4.4.4-13) (GCC) 

　　configure arguments: --prefix=/usr/local/nginx --with-http_flv_module --with-http_gzip_static_module --with-http_stub_status_module

　　分析：我的nginx版本0.8.24，http_flv_module已经开启，支持flv服务。



　　⑵安装支持flv模块的nginx服务器：

　　#tar zxvf nginx-0.8.24.tar.gz

　　#cd nginx-0.8.24

　　#./configure --prefix=/usr/local/nginx  --with-http_flv_module --with-http_gzip_static_module --with-http_stub_status_module //开启flv模块

　　#make && make install

　　#/usr/local/nginx/sbin/nginx       //启Nginx



　　⑶nginx服务器配置（nginx.conf文件）

　　server

　　{

　　listen       80;

　　server_name  localhost;   

　　charset utf-8;

　　location / {

　　root   html;

　　index  index.php index.html index.htm;

　　}

　　location ~ \.flv {

　　flv;

　　limit_rate_after 10m;

　　limit_conn one 1;   

　　limit_rate 85k;

　　}

　　}

　　使用location 将 .flv的文件指向flv模块即可。

　　以上使用limit_rate 是为了限速，当 flv视频下载超过10M，则限速到85K，只允许用户开1个进程，也就是先快速缓存可以播放，后面的慢慢下载。



　　⑷安装支持flv拖放进度条的播放器

　　上面三步都是安装支持nginx服务支持flv功能的，现在需要支持flv拖放进度条的播放器。

　　你需要有一个能够播放Flv视频的播放器，很显然，它还需要能够嵌入到网页中。目前比较流行的，功能上也还比较完善的一个Flv播放器就是 JW FLV Media Player（也称为 jwplayer）。它的网址是：

[http://www.longtailvideo.com/players/jw-flv-player/](http://www.longtailvideo.com/players/jw-flv-player/)

　　这个播放器支持视频加入广告和视频节目单功能，下载包里面有很全的实例和使用方式，将包中的player.swf（如果是带有Viral插件的，就是player-viral.swf）放到你的网站内，这就是用来提供视频播放的播放器。至于包中其它的文件，都可以不要。

　　接下来，将播放器嵌入到你的视频播放网页中，直接嵌入，使用Object/Embed代码嵌入：

　　<embed  

　　type="application/x-shockwave-flash"  

　　id="player2"  

　　name="player2"  

　　src="player.swf"   

　　width="328"   

　　height="200"  

　　allowscriptaccess="always"   

　　allowfullscreen="true"     flashvars="file=http://192.168.40.135/test.flv&image=http://192.168.40.135/preview.jpg&autostart=false&type=http&streamer=start"   

　　>  </embed>

　　file:"video.flv", // FLV视频地址

　　type:"http",        // 数据类型，本文是基于http模式的，这个必须写滴

　　image:"preview.jpg",// 开始播放之前的预览图

　　autostart:"false",  // 是否自动播放

　　streamer:"start"，   // 参数为 “start”,这个参数用于传递给服务器从特定的关键帧开始播放，nginx编译了 flv 模块 所以是支持的。

　　在使用jwplayer播放视频，并进行跳转时，会赂nginx服务器发起请求，类似

　　"GET /video.flv?start=27503944 HTTP/1.1" 200 1752013"

　　"GET /video.flv?start=88336188 HTTP/1.1" 200 13564873"



　　⑸添加关键帧

　　如果一个Flv视频要能够被拖到特定点播放，该Flv需要在其metadata中有关键帧的信息。

　　使用mencoder转换成的flv视频默认每250帧加入一个关键帧。

　　如果你的Flv视频制作的时候没有这些信息，也是不能拖放播放的。可以使用yamdi来为你的视频加上关键帧信息 ：

　　安装：

　　1） [root@localhost ~]#wget http://sourceforge.net/projects/yamdi/files/yamdi/1.4/yamdi-1.4.tar.gz/download

　　2） [root@localhost ~]#tar zxvf yamdi-1.4.tar.gz

　　3 ）[root@localhost ~]#cd yamdi-1.4/

　　4） [root@localhost yamdi-1.4]#make && make install



　　使用方法：

　　yamdi -i source.flv -o dest.flv //原视频 输出视频



　　补充

　　通过使用nginx-rtmp-module [https://github.com/arut/nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module) 来增加对HLS的支持；

　　nginx-rtmp-module 本身也支持RTMP协议，从而也可以基于nginx来搭建在线直播系统。









