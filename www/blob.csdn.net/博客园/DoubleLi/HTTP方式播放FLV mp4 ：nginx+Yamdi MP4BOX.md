# HTTP方式播放FLV/mp4 ：nginx+Yamdi/MP4BOX - DoubleLi - 博客园






**【导语】chrome浏览器确实很强，直接支持MP4拖动播放，对于其他播放器，可以使用以下方法来支持拖动播放。拖动的关键在于生成关键帧等元数据信息，便于服务器和播放器支持拖动。**

**另外，nginx web服务器体验起来，也是很有前途的一个web服务器，值得研究。**

**flv 视频可以采用两种方式发布：**

一、普通的 HTTP 下载方式

二、基于 Flash Media Server 或 Red5 服务器的 rtmp/rtmpt 流媒体方式。

多数知名视频网站都采用的是前一种方式。

两种方式对比如下：
|**Http**|**Rtmp/rtmpt**|
|----|----|
|生成关键帧后可拖动播放|任意拖动播放|
|下载完成后不再消耗服务器资源|无缓存，每次播放都会消耗服务器资源|


这里说一下怎么用nginx 搭建http 方式的视频点播服务器，主要分为三步：

**一、 nginx 安装及配置**

1. 下载nginx 最新win32 版本解压到任意目录

2. 打开cmd 进入nginx 目录，使用命令start nginx 启动nginx （默认使用\conf\nginx.conf 配置，可用-c 选项指定要使用的配置文件，更多参数可使用nginx –h 查看）

**二、 nginx 对flv 的支持**

1. 编辑配置文件\conf\nginx.conf ，加入如下配置：

在http 块加入启动gzip 的配置

gzip on;

gzip_min_length 1100;

gzip_buffers 4 8k;

gzip_types text/plain;

在server 块中加入启用flv 的配置：

limit_rate_after 1m; # 下载1M 后开始限速

limit_rate 100k; # 将速度限制为100k/s

location ~ \.flv {

flv;

}

2. 使用命令nginx –s reload 重新加载配置文件

**三、 客户端播放器**

1. 下载开源的flash 播放器jwplayer

2. 将文件解压到nginx 下的html 目录

3. 访问http://localhost:8080/player.swf?type=http&file=1.flv即可播放视频

注意：现在视频能播放了，但并不支持拖动功能，因为没有给flv 文件添加“关键帧”，可以的工具有“yamdi” 和“flvtool”，以yamdi win32 版为例，下载解压后进入cmd 执行:

yamdi –i src.flv –o dst.flv

意思是将src.flv 添加关键帧，并输出为dst.flv 。

OK ，这时我们再访问http://localhost:8080/player.swf?type=http&file=dst.flv即可自由拖动播放了。

=========================================================================================


This post is a comparison of the performance of different tools available to tag FLV and MP4 containers with specific metadata (e.g. title, keyframes, generator or other custom fields…). For FLV containers flvtool2, flvtool++ and yamdi are compared. For the MP4 container MP4box, AtomicParsley and ffmpeg are compared.

Here are the IMO three most important FLV taggers tested on a 125MB FLV:
|Name|Duration|Large Files|In Memory|Custom Tags|Command|
|----|----|----|----|----|----|
|[flvtool2 1.0.6](http://www.inlet-media.de/flvtool2/)|3min 11s|no|no|yes|flvtool2 -UP -band:Test -user:Test -date:1995 -genres:pop test.flv|
|[flvtool++ 1.2.1](http://mirror.facebook.net/facebook/flvtool++/)|3s|no|yes|yes|flvtool++ test.flv -tag band “Test” -tag user “Test” -tag date “1995″ -tag genres “pop” test2.flv|
|[yamdi 1.6](http://yamdi.sourceforge.net/)|1.5s|yes|no|no|yamdi -i test.flv -o test2.flv -c “Test”|

The performance of flvtool2 is horrendous. For films of 120min it will take hours to process. Therefore: **Do not use it!** Use Facebooks flvtool++ instead. I guess the bad performance results from it being built in Ruby. Also notice the “Large File” column indicating large file support which officially only yamdi support (by adding compile flag -D_FILE_OFFSET_BITS=64). Another important point is the “In Memory” column indicating that flvtool++ loads the entire file into memory when tagging, which is problematic when tagging large files. Given this results only **yamdi should be used for FLV tagging**!

Now for the MP4 tagging. Here you can select between a lot of tools from the net, but only a few of them are command line based and available for Unix. The MP4 test file used is 100MB large.
|Name|Duration|Command|
|----|----|----|
|[AtomicParsely](http://atomicparsley.sourceforge.net/)|0.6s|AtomicParsley test.mp4 –artist “Test” –genre “Test” –year “1995″|
|[mp4box](http://www.videohelp.com/tools/mp4box)|0.6s|MP4Box -itags Name=Test:Artist=Me:disk=95/100 test.mp4|
|[ffmpeg 0.6](http://ffmpeg.org/)|0.8s|ffmpeg -i test.mp4 -metadata title=”Test” -metadata artist=”Test” -metadata date=”1995″ -acodec copy -vcodec copy test2.mp4|



Given that recent ffmpeg brings the tagging for MP4 out of the box (it doesn’t for FLV though) you do not even need an external tool to add the metadata



自：http://www.aojie.biz/1030









