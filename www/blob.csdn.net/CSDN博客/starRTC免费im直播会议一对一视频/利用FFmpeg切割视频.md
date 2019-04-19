# 利用FFmpeg切割视频 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月28日 13:54:50[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：482
关键词:FFmpeg，seek，ss，t，to，搜索，定位
## 介绍
如果你想要从输入文件中切割一部分，需要用到ss选项。
## 快速定位
需要将ss放在输入文件的前面（即-i的前面）
**ffmpeg**-ss 00:03:00 -i Underworld.Awakening.avi -frames:v 1 elesos1.jpg
上面的命令会将大约3分钟处的画面生成一帧， 这种基于关键帧的搜索会非常快，但缺点是只能定位到关键帧，而不一定是指定的时间00:03:00，因此不太精确。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%88%A9%E7%94%A8FFmpeg%E5%88%87%E5%89%B2%E8%A7%86%E9%A2%91&action=edit&section=3)]精确定位
需要将ss放在输入文件的后面（即-i的后面）
**ffmpeg**-i Underworld.Awakening.avi -ss 00:03:00 -frames:v 1 elesos2.jpg
上面的命令会精确地定位到3分钟，并将该处的画面生成一帧图像。
注意上面2张图片左下角的时间，第一张为"00:02:05:05" ，第2张为 "00:02:05:06".
输入文件会一帧一帧地进行解码直到到达ss所指定的位置，因此会非常慢。而且随着指定时间越长，需要等待的时间也会越长。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%88%A9%E7%94%A8FFmpeg%E5%88%87%E5%89%B2%E8%A7%86%E9%A2%91&action=edit&section=4)]快速并且精确定位
需要在输入文件前后同时指定ss参数
**ffmpeg**-ss 00:02:30-i Underworld.Awakening.avi -ss 00:00:30 -frames:v 1 elesos3.jpg
这种方法综合了上面2种方法的优点。
首先快速定位到3分钟之前的某个时间点，然后从该时间点开始一帧一帧地慢慢解码到3分钟。
FFmpeg会首先快速定位到00:02:30附近的关键帧处，然后开始慢慢往后搜索30秒。结果与“精确定位”相同（左下角也是00:02:05:06），但是速度更快。
对于上面2个ss时间点(00:02:30和00:00:30)的设置依赖于关键帧的间隔（即GOP的大小），
X264默认GOP大小为250（即如果输入帧率为25fps,则每10秒一个关键帧）。
## 切割片断
如果想要从中间切割一段视频，可以将ss和t参数联合使用，其中t参数指定时长，例如“-ss 60 -t 10”将切割一段从60秒到70秒的片断；
或者使用to选项指定结束时间点，如“ -ss 60 -to 70”也一样切割一段从60秒到70秒的片断；选项t和to不能同时使用，如果你同时指定，默认会使用参数t。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%88%A9%E7%94%A8FFmpeg%E5%88%87%E5%89%B2%E8%A7%86%E9%A2%91&action=edit&section=6)]注意
如果你仅仅在输入文件之前(-i之前)指定了ss选项，那么时间戳会被重置为0，此时选项t和选项to产生的效果一样。
**ffmpeg**-ss 00:01:00 -i video.mp4 -to 00:02:00 -c copy cut.mp4
**ffmpeg**-i video.mp4 -ss 00:01:00 -to 00:02:00 -c copy cut.mp4
上面例子中，第一个命令会得到从00:01:00 到 00:03:00的片断，而第二个命令会真正得到从00:01:00 到 00:02:00的片断。
如果你切割的时候使用了流拷贝stream copy (-c copy)，并且想用 concat demuxer来合并你切割出来的片断，你需要指定`-avoid_negative_ts 1`:
**ffmpeg**-ss 00:03:00 -i video.mp4 -t60-c copy -avoid_negative_ts 1 cut.mp4
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%88%A9%E7%94%A8FFmpeg%E5%88%87%E5%89%B2%E8%A7%86%E9%A2%91&action=edit&section=7)]时间格式
你可以使用[HH:MM:SS]格式或者以秒为单位，如00:02:30 或者 150。如果你使用了小数，如02:30.05，小数点后面的05表示1秒*5%，即50毫秒(注：1秒=1000毫秒ms =106微秒µs)而不是帧数，如02:30.5表示2分钟，30秒，500毫秒(即半秒)。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%88%A9%E7%94%A8FFmpeg%E5%88%87%E5%89%B2%E8%A7%86%E9%A2%91&action=edit&section=8)]附
如果将-c:v copy与-ss同时使用，生成的文件可能不能播放，或者出现音视频不同步的问题，因为FFmpeg是强制基于关键帧进行切割的。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%88%A9%E7%94%A8FFmpeg%E5%88%87%E5%89%B2%E8%A7%86%E9%A2%91&action=edit&section=9)]艺搜参考
[http://trac.ffmpeg.org/wiki/Seeking%20with%20FFmpeg](http://trac.ffmpeg.org/wiki/Seeking%20with%20FFmpeg)
