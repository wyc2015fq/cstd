# ffmpeg处理RTMP流媒体的命令和发送流媒体的命令（UDP，RTP，RTMP） - DoubleLi - 博客园






将文件当做直播送至live

ffmpeg -re -i localFile.mp4 -c copy -f flv rtmp://server/live/streamName  



re限制输出速率，按照帧率输出



将直播媒体保存至本地文件

ffmpeg -i rtmp://server/live/streamName -c copy dump.flv  



将其中一个直播流，视频改用h264压缩，音频不变，送至另外一个直播服务流

ffmpeg -i rtmp://server/live/originalStream -c:a copy -c:v libx264 -vpre slow -f flv rtmp://server/live/h264Stream  



将其中一个直播流，视频不变，音频改用faac压缩，送至另外一个直播服务流

ffmpeg -i rtmp://server/live/originalStream -acodec libfaac -ar 44100 -ab 48k -vcodec copy -f flv rtmp://server/live/h264_AAC_Stream



将其中一个直播流，视频改用h264压缩，音频改用faac压缩，送至另外一个直播服务流



ffmpeg -i rtmp://server/live/originalStream -c:a libfaac -ar 44100 -ab 48k -c:v libx264 -vpre slow -vpre baseline -f flv rtmp://server/live/h264Stream  



将一个高清流，复制为几个不同视频清晰度的流重新发布，其中音频不变

ffmpeg -re -i rtmp://server/live/high_FMLE_stream -acodec copy -vcodec x264lib -s 640×360 -b 500k -vpre medium -vpre baseline rtmp://server/live/baseline_500k -acodec copy -vcodec x264lib -s 480×272 -b 300k -vpre medium -vpre baseline rtmp://server/live/baseline_300k -acodec copy -vcodec x264lib -s 320×200 -b 150k -vpre medium -vpre baseline rtmp://server/live/baseline_150k -acodec libfaac -vn -ab 48k rtmp://server/live/audio_only_AAC_48k  



功能一样，只是采用-x264opts选项

ffmpeg -re -i rtmp://server/live/high_FMLE_stream -c:a copy -c:v x264lib -s 640×360 -x264opts bitrate=500:profile=baseline:preset=slow rtmp://server/live/baseline_500k -c:a copy -c:v x264lib -s 480×272 -x264opts bitrate=300:profile=baseline:preset=slow rtmp://server/live/baseline_300k -c:a copy -c:v x264lib -s 320×200 -x264opts bitrate=150:profile=baseline:preset=slow rtmp://server/live/baseline_150k -c:a libfaac -vn -b:a 48k rtmp://server/live/audio_only_AAC_48k 



将当前摄像头及音频通过DSSHOW采集，视频h264、音频faac压缩后发布

ffmpeg -r 25 -f dshow -s 640×480 -i video=”video source name”:audio=”audio source name” -vcodec libx264 -b 600k -vpre slow -acodec libfaac -ab 128k -f flv rtmp://server/application/stream_name  



将一个JPG图片经过h264压缩循环输出为mp4视频

ffmpeg -i INPUT.jpg -an -vcodec libx264 -coder 1 -flags +loop -cmp +chroma -subq 10 -qcomp 0.6 -qmin 10 -qmax 51 -qdiff 4 -flags2 +dct8x8 -trellis 2 -partitions +parti8x8+parti4x4 -crf 24 -threads 0 -r 25 -g 25 -y OUTPUT.mp4 



将普通流视频改用h264压缩，音频不变，送至高清流服务(新版本FMS live=1)

ffmpeg -i rtmp://server/live/originalStream -c:a copy -c:v libx264 -vpre slow -f flv “rtmp://server/live/h264Stream live=1″  













发送流媒体的命令（UDP，RTP，RTMP）

**1.      UDP**

**1.1. 发送H.264裸流至组播地址**

注：组播地址指的范围是224.0.0.0—239.255.255.255

下面命令实现了发送H.264裸流“chunwan.h264”至地址udp://233.233.233.223:6666


- ffmpeg -re -i chunwan.h264 -vcodec copy -f h264 udp://233.233.233.223:6666  

注1：-re一定要加，代表按照帧率发送，否则ffmpeg会一股脑地按最高的效率发送数据。

注2：-vcodec copy要加，否则ffmpeg会重新编码输入的H.264裸流。

**1.2. 播放承载H.264裸流的UDP**


- ffplay -f h264 udp://233.233.233.223:6666  

注：需要使用-f说明数据类型是H.264

播放的时候可以加一些参数，比如-max_delay，下面命令将-max_delay设置为100ms：


- ffplay -max_delay 100000 -f h264 udp://233.233.233.223:6666  

**1.3. 发送MPEG2裸流至组播地址**

下面的命令实现了读取本地摄像头的数据，编码为MPEG2，发送至地址udp://233.233.233.223:6666。


- ffmpeg -re -i chunwan.h264 -vcodec mpeg2video -f mpeg2video udp://233.233.233.223:6666  

**1.4.  播放MPEG2裸流**

指定-vcodec为mpeg2video即可。


- ffplay -vcodec mpeg2video udp://233.233.233.223:6666  

**2.      RTP**

**2.1. 发送H.264裸流至组播地址。**

下面命令实现了发送H.264裸流“chunwan.h264”至地址rtp://233.233.233.223:6666


- ffmpeg -re -i chunwan.h264 -vcodec copy -f rtp rtp://233.233.233.223:6666>test.sdp  

注1：-re一定要加，代表按照帧率发送，否则ffmpeg会一股脑地按最高的效率发送数据。

注2：-vcodec copy要加，否则ffmpeg会重新编码输入的H.264裸流。

注3：最右边的“>test.sdp”用于将ffmpeg的输出信息存储下来形成一个sdp文件。该文件用于RTP的接收。当不加“>test.sdp”的时候，ffmpeg会直接把sdp信息输出到控制台。将该信息复制出来保存成一个后缀是.sdp文本文件，也是可以用来接收该RTP流的。加上“>test.sdp”后，可以直接把这些sdp信息保存成文本。

![](file:///C:/Users/leijh/AppData/Local/YNote/data/elesos@163.com/6468b1548b07416aab6893597e7adcd4/30002527312.jpeg)



**2.2. 播放承载H.264裸流的RTP。**


- ffplay test.sdp  

**3.      RTMP**

**3.1. 发送H.264裸流至RTMP服务器（****FlashMedia Server****，Red5等）**

发送H.264裸流“chunwan.h264”至主机为localhost，Application为oflaDemo，Path为livestream的RTMP URL。


- ffmpeg -re -i chunwan.h264 -vcodec copy -f flv rtmp://localhost/oflaDemo/livestream  
- 



rtmp://10.50.8.10/socketTest/streamName



**3.2. 播放RTMP**


- 
ffplay “rtmp://localhost/oflaDemo/livestream live=1”  


注：ffplay播放的RTMP URL最好使用双引号括起来，并在后面添加live=1参数，代表实时流。实际上这个参数是传给了ffmpeg的libRTMP的。





**4.      测延时**

**4.1.测延时**


测延时有一种方式，即一路播放发送端视频，另一路播放流媒体接收下来的流。播放发送端的流有2种方式：FFmpeg和FFplay。


通过FFplay播放是一种众所周知的方法，例如：


- 
ffplay -f dshow -i video="Integrated Camera"  


即可播放本地名称为“Integrated Camera”的摄像头。


此外通过FFmpeg也可以进行播放，通过指定参数“-f sdl”即可。例如：


- 
ffmpeg -re -i chunwan.h264 -pix_fmt yuv420p –f sdl xxxx.yuv -vcodec copy -f flv rtmp://localhost/oflaDemo/livestream  


就可以一边通过SDL播放视频，一边发送视频流至RTMP服务器。


注1：sdl后面指定的xxxx.yuv并不会输出出来。


注2：FFmpeg本身是可以指定多个输出的。本命令相当于指定了两个输出。








TODO：


1，循环推流


FOR /L %% IN (0) DO 




循环播放文件示例,使用的是 concat 特性.




ffmpeg -re -f concat 
-i playlist.txt \


-vcodec libx264 -preset veryfast -maxrate 500k -bufsize 500k \


-vf "format=yuv420p:scale=720:-1" -g 48 -acodec libmp3lame -b:a 96k -ar 44100 \


-f flv "rtmp://send3.douyutv.com/live/209282rezHd16aNm?wsSecret=f759db6daa4137c184e2f16125d8af5d&wsTime=54dd5819"






golf_680_640x360.mp4:loop=0 


OR Add 


-stream_loop -1 






fmpeg -f lavfi -re -i movie=golf_680_640x360.mp4:loop=0 -acodec libfaac -b:a 64k -pix_fmt yuv420p -vcodec libx264 -x264opts level=41 -r 25 -profile:v baseline -b:v 1500k -maxrate
 2000k -force_key_frames 50 -s 640×360 -map 0 -flags -global_header -f segment -segment_list index_1500.m3u8 -segment_time 10 -segment_format mpeg_ts -segment_list_type m3u8 segment%05d.ts





ffmpeg -threads 2 -re -fflags +genpts 
-stream_loop -1 -i $F \ 


-s 640x360 -ac 2 -f flv -vcodec libx264 -profile:v baseline -b:v 600k -maxrate 600k -bufsize 600k -r 24 -ar 44100 -g 48 -c:a libfdk_aac -b:a 64k "rtmp://publish.live.metacdn.com/2050C7/dfsdfsd/lowquality_664?hello&adbe-live-event=lowquality_" \



-s 1920x1080 -ac 2 -f flv -vcodec libx264 -profile:v baseline -b:v 2000k -maxrate 2000k -bufsize 2000k -r 24 -ar 44100 -g 48 -c:a libfdk_aac -b:a 64k "rtmp://publish.live.metacdn.com/2050C7/dfsdfsd/highquality_2064?mate&adbe-live-event=highquality_"






参考

[http://www.wowza.cn/ffmpeg](http://www.wowza.cn/ffmpeg)

[https://blog.mangege.com/tech/2015/02/15/1.html](https://blog.mangege.com/tech/2015/02/15/1.html)

[https://www.v2ex.com/t/264280](https://www.v2ex.com/t/264280)

[http://video.stackexchange.com/questions/12905/repeat-loop-input-video-with-ffmpeg](http://video.stackexchange.com/questions/12905/repeat-loop-input-video-with-ffmpeg)



[http://blog.csdn.net/leixiaohua1020/article/details/38283297](http://blog.csdn.net/leixiaohua1020/article/details/38283297)

[http://blog.csdn.net/leixiaohua1020/article/details/12029543](http://blog.csdn.net/leixiaohua1020/article/details/12029543)









