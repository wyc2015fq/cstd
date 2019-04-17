# 搭建 Http Dynamic Streaming 点播/直播服务器 - DoubleLi - 博客园






1.        HTTP Origin Module的处理数据流：

a)         客户端发送媒体索引请求到Apache。例如：

http://www.example.com/media/ sample.f4m

b)        Apache处理请求到HTTP Origin
Module

c)         HTTP Origin Module返回 F4M文件到客户端

d)        客户端接收F4M，根据 bootstrap中的信息中的传送时间，组成一个segment#/fragment#对。例如：

http://www.example.com/media/sampleSeg1-Frag1

e)         Apache接收请求，并把请求传组HTTP
Origin Module

f)         HTTP Origin Module用这个索引文件（F4X）转换请求到分版本文件的偏移字节。例如：

http://www.example.com/media/sampleSeg1.f4f

g)        客户端接收分片，并用F4M文件中的BOOTSTRAP信息，开始进行播放



2.   准备：

a)  File Packager：[http://www.adobe.com/products/hds-dynamic-streaming.html](http://www.adobe.com/products/hds-dynamic-streaming.html)

b)   Apache2

c)   HTTP Origin Module

d)   OSMF Simple Player For Http Dynamic Streaming 下载地址： [http://www.osmf.org/downloads/OSFMPlayer_zeri2.zip](http://www.osmf.org/downloads/OSFMPlayer_zeri2.zip)



3.        配置：

需求：

a)         要apache2.2版本

b)      Windows下要安装VS2005或VS2008资源包。

c)       在linux下需要安装下列包：OpenSSL, Expat, and Netscape Portable Runtime (NSPR).

d)        安装HTTP Origin Module

把下列文件复制到 apache modules文件夹下：

·        (Windows) mod_f4fhttp.so, hds.dll (adbe_F4V.dll in earlier versions), libexpat.dll

·        (Linux) mod_f4fhttp.so, libhds.so (libF4V.so in earlier versions), libexpat.so

4.        配置 on-demand流，配置点播文件的位置

a)         打开apache配置文件httpd.conf

加载模块：

LoadModule f4fhttp_module modules/mod_f4fhttp.so

b)        配置HTTP origin module的处理对象位置：

<Location /vod>

    HttpStreamingEnabled true

    HttpStreamingContentPath  /var/www/flv

</Location>


|Directive|Description|
|----|----|
|<Location>|请求URI，注意不要包含URL前缀 Do not include an http prefix(such as “http://servername”) in the <Location> path.|
|HttpStreamingEnabled|Indicates whether HTTP streaming is enabled (true) or not (false).|
|HttpStreamingContentPath|内容的物理位置，用一个绝对路径 /var/www/flv|



c)         保存，并重启apache



5.        转换点播文件

File产生者只能对 flv或f4v/mp4文件生成分片。
|文件类型|描述|
|----|----|
|.f4f|分段文件。工具输出一个或多个f4f文件。每个文件包含源文件的一个分段(segment)。每个分段包含一个或多个分片（fragment）。一个播放器能用URL地址请求每个分片（fragment）.|
|.f4m|媒体描述文件.  包含媒体的编解码、解析、多比特率的可用性。|
|.f4x|索引文件。包含指定分片在流中的位置。|
|.bootstrap|它将告诉apache及其中的模块如何去读取./f4f文件，可以理解为引导文件，引导信息来自于.f4m文件，但是也可以额外指定其它信息来源(--external-bootstrap)|
|.drmmeta|用于保存加密的信息，需要使用(--external-bootstrap)来引用进来|



例如：

cd /var/www/flv

f4fpackager --input-file=sample.f4v --output-path=/sampleoutput



6.        播放设置：

把下载的播放器放到apache web目录下

/var/www/

解压。

打开 OSMFPlayer.html文件。

增加你的点播地址：

var links

=   [ "http://mediapm.edgesuite.net/osmf/content/test/manifest-files/dynamic_Streaming.f4m"

                , "http://mediapm.edgesuite.net/osmf/content/test/manifest-files/progressive.f4m"

                , "http://mediapm.edgesuite.net/osmf/content/test/train_1500.mp3"

                , "http://mediapm.edgesuite.net/strobe/content/test/AFaerysTale_sylviaApostol_640_500_short.flv"

                , "images/vegetation.jpg"

                , "http://192.168.0.10/vod/sample.f4m"

                ];



7.        播放

用浏览器打开 http://192.168.0.10/ OSMFPlayer.html。在上面列表中点 [http://192.168.0.10/vod/sample.f4m](http://192.168.0.10/vod/sample.f4m) 。就可以播放了。注意，可能会提示是否运行脚本，允许即可。





参考：

文件产生工具：

[http://help.adobe.com/en_US/HTTPStreaming/1.0/Using/WS9463dbe8dbe45c4c-c126f3b1260533756d-7ffc.html](http://help.adobe.com/en_US/HTTPStreaming/1.0/Using/WS9463dbe8dbe45c4c-c126f3b1260533756d-7ffc.html)

Apach配置：

[http://help.adobe.com/en_US/HTTPStreaming/1.0/Using/WS7b362c044b7dd076-735e76121260080a90e-8000.html#WS7b362c044b7dd076-735e76121260080a90e-7ffc](http://help.adobe.com/en_US/HTTPStreaming/1.0/Using/WS7b362c044b7dd076-735e76121260080a90e-8000.html#WS7b362c044b7dd076-735e76121260080a90e-7ffc)









8.        直播：

a)        下载Adobe Flash Media Server，这个是要收费的，免费的只是开发版，并有用户数据限制。[http://www.adobe.com/products/hds-dynamic-streaming.html](http://www.adobe.com/products/hds-dynamic-streaming.html)

b)       安装：



----------- Install Action Summary -----------



Installation directory         = /opt/adobe/fms



Adobe Flash Media Server Port        = 1935,80

Adobe Flash Media Admin Server Port  = 1111



Apache Install                 = Yes





Administrative username        = root

Administrative password        = (suppressed)



service owner                  = fms



service user                   = fms

service group                  = fms



Run as daemon                  = Yes

Start Adobe Flash Media Server       = Yes



Flash 媒体服务器服务端口 1935是用于接收直播rtmp流的。80端口用于客户端请求的。

它默认情况是启动了自带的Apache服务器。Fms通过80接收http请求，然后通过8314（配置文件在安装目录下conf/fms.ini）传给apache。如果我们不用它自带的apache，就可以不用配置。

例如：在安装目录下conf/fms.ini, 将其中的ADAPTOR.HOSTPORT = :1935,80 一行改为ADAPTOR.HOSTPORT =:1935; 不让FMS监听80端口；同时，将其中 HTTPPROXY.HOST = :8134一行改为：  HTTPPROXY.HOST
=

这样便去掉了以上的代理转发过程。



9.        用ffmpeg生成直播流

a)        接收组播直播流，并保存成文件：

./ffmpeg -i udp://@:30000 a.ts

b)       从文件生成直播流：

ffmpeg  -i a.flv  -f flv rtmp://IP/livepkgr/livestream?adbe-live-event=liveevent

IP是你的fms服务器的IP地址



c)        从组播地址接收组播，并把它转成rtmp协议发给 fms服务器

ffmpeg –i udp://@:port –f flv rtmp://IP/livepkgr/livestream?adbe-live-event=liveevent





10.    再按点播时的地址：

[http://IP/hds-live/livepkgr/_definst_/liveevent/livestream.f4m](http://ip/hds-live/livepkgr/_definst_/liveevent/livestream.f4m)











HTTP Dynamic Streaming介绍



一、什么是HTTP Dynamic Streaming
       使用传统的HTTP[协议](http://www.rosoo.net/a/list_46_1.html)进行在线播放叫做“渐进下载”，所有的视频内容从头到尾必须从[服务器](http://www.rosoo.net/a/list_52_1.html)传输到客户端，用户只能在传输完的视频长度中选择播放点，而不能自定义播放点及传输点，比如我们在看视频的时候是边下边看，没下载完则看不了，而且也不能绕到视频后面的片段。当视频观看完毕之后，在浏览器的缓存中会存在一个视频文件。
       而使用RTMP协议进行传输的数据包叫做“流”（如Flash Media Server）,它能够让视频内容分割成多个数据包并源源不断从服务器端传输到客户端，客户端可以在视频内容任意一个点开始请求传输，而不用关心该点之前的内容是否已经传输。这样我们看视频的时候可以在任意一个地方开始观看，点到哪里就从哪里开始下载，观看完毕之后在客户端不会有缓存文件。
       两种协议各有各的优缺点，比如http协议在第二次观看视频的时候会直接使用缓存文件进行播放，速度也比较快，而RTMP协议必须保持源源不断送出“流”，同时本地也无缓存。
       而HTTP Dynamic Streaming则是对两种协议的优点进行了一个组合，达到了两个协议取长补短的服务平台。其通过对来自RTMP端的“流”进行包装处理，转化成 HTTP“流”提供给客户端解析，用户再也不用下载整个文件，同时又能使用HTTP协议进行快速观看视频。
       架构图：



![](http://img.my.csdn.net/uploads/201209/17/1347890969_5635.jpg)

工作模式：

        HTTP Dynamic Streaming有两种工作模式，一种是On-demand模式，直接对文件进行“流”处理，把单个文件分离成N个片段，用户跳到相应的片段，则传输该 片段，用户没请求该片段，则不传输(貌似能达到节省带宽的作用)；一种是live模式，也就是所谓的直播，这里需要FMS的支持，FMS通过把直播流传递 给HTTP Dynamic Streaming，然后进行包装处理，传递给客户端，此模式可以应用在视频会议，视频聊天室，网络直播等应用中，HTTP Dynamic Streaming的主要作用也在这个模式中体现。

二、原理分析


        用过Flash Media Server(简称：FMS)的技术人员都知道FMS的工作原理，而HTTP Dynamic Streaming(简称：HDS)的实际效果则是工作在FMS计算结果上的，从架构图上不难看出，无论是On-demand模式或live模式，多多少 少都会依赖FMS，比如On-demand模式，FLV文件必须通过FILE PACKAGER进行转码得到".f4f",".f4m",".bootstrap"等文件才能够提供给“HTTP ORIGIN MODULE”处理，而一般线上的环境的视频文件何止千千万万！再说效率是否达到要求还很难说。而live模式中LIVE
PACKAGER能够把来自RTMP的“流”直接生成所需要的文件，提供给“HTTP ORIGIN MODULE”处理，但依然也是得有FMS的支持才行。


        实际的工作流程是这样的：


       On-demand:      FLV /F4V(目前只支持两种格式)------>File Packager------->(.f4f/.f4m/.f4x/..bootstrap/.drmmeta)------->Apache-------->HTTP ORIGIN MODULE-------->客户端播放器(需支持HTTP流)


        Live:      FLV /F4V(目前只支持两种格式)------->FMS(Using RTMP)------->Live Packager------->(.f4f/.f4m/.f4x/..bootstrap/.drmmeta)------->Apache-------->HTTP ORIGIN MODULE-------->客户端播放器(需支持HTTP流)


        相关模块：


        File Packager:一个命令行工具,它可以按照需求把多媒体文件形成流碎片并把碎片写进\.f4f文件。文件包装机是一种离线工具。同时也支持Flash Access验证访问的需求。


        Live Packager:该工具只针对HDS,同时集成在FMS(version 3.8以上)。它可以实时测量RTMP流(live),并将之转化成新的\.f4f文件，满足实时性要求。内置的apache服务器使用HTTP ORIGIN MODULE对生成的文件进行解析，然后提供出HTTP流。


        HTTP ORIGIN MODULE:HDS的重要组成部分，其为apache的一个modules，负责对(.f4f/.f4m/.f4x/..bootstrap/.drmmeta)等文件进行解析，然后转换成HTTP流输出。


        OSMF Player:一个开源的播放器，建立在Open Source Media Framework(OSMF)的框架上，支持HTTP流，要求Flash player 10.1或以上



转自http://blog.csdn.net/kl222/article/details/7989562









