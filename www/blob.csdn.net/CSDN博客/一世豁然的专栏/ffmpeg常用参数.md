# ffmpeg常用参数 - 一世豁然的专栏 - CSDN博客





2016年08月09日 11:12:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：403








本博客转载于：[http://blog.chinaunix.net/uid-342902-id-2416123.html](http://blog.chinaunix.net/uid-342902-id-2416123.html)





具体的录音、屏幕录制、格式转换、抓取音频的例子请看[我的另一篇blog](http://blog.chinaunix.net/u1/50920/showart_418855.html)，这里记点参数备查。
|基本选项:| |
|----|----|
|-formats|输出所有可用格式|
|-f fmt|指定格式(音频或视频格式)|
|-i filename|指定输入文件名，在linux下当然也能指定:0.0(屏幕录制)或摄像头|
|-y|覆盖已有文件|
|-t duration|记录时长为t|
|-fs limit_size|设置文件大小上限|
|-ss time_off|从指定的时间(s)开始， [-]hh:mm:ss[.xxx]的格式也支持|
|-itsoffset time_off|设置时间偏移(s)，该选项影响所有后面的输入文件。该偏移被加到输入文件的时戳，定义一个正偏移意味着相应的流被延迟了 offset秒。 [-]hh:mm:ss[.xxx]的格式也支持|
|-title string|标题|
|-timestamp time|时间戳|
|-author string|作者|
|-copyright string|版权信息|
|-comment string|评论|
|-album string|album名|
|-v verbose|与log相关的|
|-target type|设置目标文件类型("vcd", "svcd", "dvd", "dv", "dv50", "pal-vcd", "ntsc-svcd", ...)|
|-dframes number|设置要记录的帧数|
| | |
|-b|指定比特率(bits/s)，似乎ffmpeg是自动VBR的，指定了就大概是平均比特率|
|-vb|指定视频比特率(bits/s)|
|-vframes number|设置转换多少桢(frame)的视频|
|-r rate|桢速率(fps)|
|-s size|分辨率|
|-aspect aspect|设置视频长宽比(4:3, 16:9 or 1.3333, 1.7777)|
|-croptop size|设置顶部切除尺寸(in pixels)|
|-cropbottom size|设置底部切除尺寸(in pixels)|
|-cropleft size|设置左切除尺寸 (in pixels)|
|-cropright size|设置右切除尺寸 (in pixels)|
|-padtop size|设置顶部补齐尺寸(in pixels)|
|-padbottom size|底补齐(in pixels)|
|-padleft size|左补齐(in pixels)|
|-padright size|右补齐(in pixels)|
|-padcolor color|补齐带颜色(000000-FFFFFF)|
|-vn|取消视频|
|-vcodec codec|强制使用codec编解码方式('copy' to copy stream)|
|-sameq|使用同样视频质量作为源（VBR）|
|-pass n|选择处理遍数（1或者2）。两遍编码非常有用。第一遍生成统计信息，第二遍生成精确的请求的码率|
|-passlogfile file|选择两遍的纪录文件名为file|
|-newvideo|在现在的视频流后面加入新的视频流|
|| |
| | |
|-pix_fmt format|set pixel format, 'list' as argument shows all the pixel formats supported|
|-intra|仅适用帧内编码|
|-qscale q|以<数值>质量为基础的VBR，取值0.01-255，约小质量越好|
|-loop_input|设置输入流的循环数(目前只对图像有效)|
|-loop_output|设置输出视频的循环数，比如输出gif时设为0表示无限循环|
|-g int|设置图像组大小|
|-cutoff int|设置截止频率|
|-qmin int|设定最小质量|
|-qmax int|设定最大质量|
|-qdiff int|量化标度间最大偏差 (VBR)|
|-bf int|使用frames B 帧，支持mpeg1,mpeg2,mpeg4|
| | |
|-ab|设置比特率(单位：bit/s，也许老版是kb/s)|
|-aframes number|设置转换多少桢(frame)的音频|
|-aq quality|设置音频质量 (指定编码)|
|-ar rate|设置音频采样率 (单位：Hz)|
|-ac channels|设置声道数|
|-an|取消音频|
|-acodec codec|指定音频编码('copy' to copy stream)|
|-vol volume|设置录制音量大小(默认为256)|
|-newaudio|在现在的音频流后面加入新的音频流|
| | |
|-sn|取消字幕|
|-scodec codec|设置字幕编码('copy' to copy stream)|
|-newsubtitle|在当前字幕后新增|
|-slang code|设置字幕所用的ISO 639编码(3个字母)|
| | |
|-vc channel|设置视频捕获通道(只对DV1394)|
|-tvstd standard|设置电视标准 NTSC PAL(SECAM)|



