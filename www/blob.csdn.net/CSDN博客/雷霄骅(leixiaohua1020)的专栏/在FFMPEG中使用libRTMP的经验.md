# 在FFMPEG中使用libRTMP的经验 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月06日 17:48:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：44
个人分类：[FFMPEG																[RTMP](https://blog.csdn.net/leixiaohua1020/article/category/8432890)](https://blog.csdn.net/leixiaohua1020/article/category/1360795)









FFMPEG在编译的时候可以选择支持RTMP的类库libRTMP。这样ffmpeg就可以支持rtmp://, rtmpt://, rtmpe://, rtmpte://,以及 rtmps://协议了。但是如何使用ffmpeg支持RTMP协议还是有一定的学问的。本文总结一下部分经验。

ffmpeg接受一个字符串的输入方式，比如：“rtmp://xxxx live=1 playpath=xxx ...”这种的输入形式，即第一个字符串是rtmp的url，然后加一个空格，然后再附加一些参数。附加的参数的格式形如“playpath=xxxx”这种形式。这个乍一看让人觉得有点不习惯。因为一般的输入只包含一个字符串，比如说一个流媒体的url或者是文件的路径，不会采用“url+空格+参数1+参数2+...”的形式。

例如，当需要打开一个直播流的时候，可以用如下字符串（这里连接的是中国教育电视台1频道（网络直播））：


`rtmp://pub1.guoshi.com/live/newcetv1`


当需要用ffmpeg保存RTMP直播流媒体的时候：


`ffmpeg -i "rtmp://pub1.guoshi.com/live/newcetv1 live=1" -vcodec copy -acodec copy ttt.flv`


当需要用ffplay播放RTMP直播流媒体的时候：


`ffplay "rtmp://pub1.guoshi.com/live/newcetv1 live=1"`
在使用FFMPEG类库进行编程的时候，也是一样的，只需要将字符串传递给avformat_open_input()就行了，形如（这里连接的是香港电视台频道（网络直播））：



```cpp
char url[]="rtmp://live.hkstv.hk.lxdns.com/live/hks live=1";
avformat_open_input(&pFormatCtx,url,NULL,&avdic)
```



注：librtmp支持的参数：[http://rtmpdump.mplayerhq.hu/librtmp.3.html](http://rtmpdump.mplayerhq.hu/librtmp.3.html)











