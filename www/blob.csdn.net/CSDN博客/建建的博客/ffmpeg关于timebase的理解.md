# ffmpeg关于timebase的理解 - 建建的博客 - CSDN博客
2017年03月24日 09:13:32[纪建](https://me.csdn.net/u013898698)阅读数：228
个人分类：[ffmpeg学习](https://blog.csdn.net/u013898698/article/category/6692312)
ffmpeg关于timebase的理解
一：ffmpeg的分层结构
写这篇文章的时候先把ffmpeg分层问题大致说明一下，我们按照最常见的  "转码程序" 和 "摄像头采集" 以及 "播放器" 三个模型总结。
1：转码
转码基本流程：flv格式数据-->h264/aac数据-->yuv/pcm数据-->h264/aac数据-->mp4格式数据。
a：我们把flv格式数据或者mp4格式数据这一层叫做mux/demux层或者复用层有些人习惯于叫做封装层，这里叫做mux/demux层下面同理。
b：我们把h264/aac数据这一层叫做编解码层或者codec/decode，这里叫做codec/decode层下面同理。
c：我们把yuv/pcm数据这一层叫做原始数据层或者Raw data 层，这里叫做Raw data层下面同理。
2：摄像头采集
摄像头采集基本流程：yuv/pcm数据-->h264/aac数据-->flv格式数据。
a：我们把yuv/pcm数据这一层叫做原始数据层或者Raw data 层，这里叫做Raw data层下面同理。
b：我们把h264/aac数据这一层叫做编解码层或者codec，这里叫做codec层下面同理。
c：我们把flv格式数据这一层叫做mux层或者复用层有些人习惯于叫做封装层，这里叫做mux层下面同理。
3：播放器
播放器基本流程：flv格式数据-->h264/aac数据-->yuv/pcm数据。
a：我们把flv格式数据这一层叫做demux层或者复用层有些人习惯于叫做封装层，这里叫做demux层下面同理。
b：我们把h264/aac数据这一层叫做编解码层或者decode，这里叫做decode层下面同理。
c：我们把yuv/pcm数据这一层叫做原始数据层或者Raw data 层，这里叫做Raw data层下面同理。
从上面的三个最常用模型不难看出也可以说总结出ffmpeg基本把数据或者说结构分为了 “mux/demux层”也就是ffmpeg中的AVStream 
“codec/decode层” 也就是ffmpeg中的AVCodec “Raw data 层”这个也在AVStream 中存放着（如果是自己填写的例如[iOS](http://lib.csdn.net/base/ios)或者[Android](http://lib.csdn.net/base/android)获取当前毫秒时间的可以单独放置到一个timebse的结构体中），这个。各位看官对这个分层有了大致的了解之后我们再说分层对timebase的影响。
二：ffmpeg中的timebase是什么
1：简单来讲ffmpeg的timebase是为了解决当时间戳是小数的时候转化为整数和为了更好的做分层结构而设置的一种机制。
2：ffmpeg的timebse我们可以理解为单位，比如米，毫秒，秒，千克这些都是单位，但这个timebse有时候不是我们日常中能见到的单位，举个例子ffmpeg本身是以秒为基准的，1秒= 1000毫秒，那如果timebse是1:1很好理解是秒的单位，如果timebase是1:1000也很好理解是毫秒单位，但如果timebase是1:25呢，我们的常用单位就解释不了了，这要是很多人的误区。
三：ffmpeg中分层结构和timebase的关系
1：举几个常用的ffmpeg的timebase例子
a：mux/demux层的timebase，flv，MP4等一般是1:1000，ts一般是1:90*1000 。
![](https://img-blog.csdn.net/20170322161816492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1d2VpZ2FuZ3p3Zw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170322161831179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1d2VpZ2FuZ3p3Zw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
b：codec/decode层timebase，h264随着帧率变化例如1:25 aac根据采样率变化例如1:44100。
![](https://img-blog.csdn.net/20170322162309786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1d2VpZ2FuZ3p3Zw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170322162704569?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1d2VpZ2FuZ3p3Zw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
c：Raw data 层的timebase有很多变化比如1:1000*1000 或1:1000等等
![](https://img-blog.csdn.net/20170322165106692?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1d2VpZ2FuZ3p3Zw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个截图是ffmpeg在windows平台的摄像头采集timebase，如果是ios或者android可以获取系统的毫秒时间把timebase设置成1:1000，同理。
2：ffmpeg关于分层结构timebase的转换过程
a：转码模型
首先按照分层结构获取到demux层的timebase将flv数据也就是demux层的timebase转换为codec层的timebase。
**[cpp]**[view
 plain](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)[copy](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2283909/fork)
- picture->pts = av_rescale_q_rnd(picture->pts,streams->time_base,streams->decodectext->time_base, AV_ROUND_NEAR_INF);  
然后将decode层的timebase转换成codec层（这两个同层但也需要转换）的timebase，这里有个问题Raw data 层的timebase可以略过，可以直接从decode转换为coedec，Raw data 层的timebse在摄像头采集和播放器的时候会用到。
**[cpp]**[view
 plain](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)[copy](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2283909/fork)
- picture->pts = av_rescale_q_rnd(picture->pts,streams->decodectext->time_base,streams->codectext->time_base, AV_ROUND_NEAR_INF);  
最后将codec层的timebase转换为mux层的timebase。
**[cpp]**[view
 plain](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)[copy](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2283909/fork)
- picture->pts = av_rescale_q_rnd(picture->pts,streams->codectext->time_base,streams->time_base,AV_ROUND_NEAR_INF);  
b：采集模型
首先获取到Raw data 层的timebase如果是windows的可以从streams的timebase中读取，ios和android可以自己单独写个timebase结构体用于转换，将获取到的timebase转换为codec层的timebase。
**[cpp]**[view
 plain](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)[copy](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2283909/fork)
- picture->pts = av_rescale_q_rnd(picture->pts,timebase_in,streams->codectext->time_base, AV_ROUND_NEAR_INF);  
然后将codec层的timebase转换为mux层的timebase
**[cpp]**[view
 plain](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)[copy](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2283909/fork)
- picture->pts = av_rescale_q_rnd(picture->pts,streams->codectext->time_base, streams->timebase,AV_ROUND_NEAR_INF);  
c：播放器模型
首先将demux层的timebase转换为decode层的timebase。
**[cpp]**[view
 plain](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)[copy](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2283909/fork)
- picture->pts = av_rescale_q_rnd(picture->pts,streams->time_base,streams->decodectext->time_base, AV_ROUND_NEAR_INF);  
然后将decode层的timebase转换为播放器用的Raw data 层的timebase一般设为1:1000为了将时间戳转化为毫秒。
**[cpp]**[view
 plain](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)[copy](http://blog.csdn.net/zhuweigangzwg/article/details/64919706#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2283909/fork)
- picture->pts = av_rescale_q_rnd(picture->pts,streams->decodectext->time_base,1:1000, AV_ROUND_NEAR_INF);  
这样三个模型的ffmpeg分层和timebase的关系就解释清楚了，如有问题还需交流互相学习。
ffmpeg的输出截图如下：
![](https://img-blog.csdn.net/20170322172331675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1d2VpZ2FuZ3p3Zw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[点击打开链接](http://blog.csdn.net/zhuweigangzwg/article/details/64919706)

