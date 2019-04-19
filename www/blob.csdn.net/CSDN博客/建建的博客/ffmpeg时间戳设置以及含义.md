# ffmpeg时间戳设置以及含义 - 建建的博客 - CSDN博客
2017年01月22日 19:21:35[纪建](https://me.csdn.net/u013898698)阅读数：2919
FFMPEG的很多结构中有AVRational time_base;这样的一个成员，它是AVRational结构的
typedef struct AVRational{
    int num; ///< numerator
    int den; ///< denominator
} AVRational;
AVRational这个结构标识一个分数，num为分数，den为分母。
实际上time_base的意思就是时间的刻度：
如（1,25），那么时间刻度就是1/25 （1,9000），那么时间刻度就是1/90000
那么，在刻度为1/25的体系下的time=5，转换成在刻度为1/90000体系下的时间time为(5*1/25)/(1/90000) = 3600*5=18000 
ffmpeg中做pts计算时，存在大量这种转换
在以下结构中都有
AVCodecContext：编解码上下文。
AVStream:文件或其它容器中的某一个track。
如果由某个解码器产生固定帧率的码流
AVCodecContext中的AVRational根据帧率来设定，如25帧，那么num = 1，den=25
AVStream中的time_base一般根据其采样频率设定，如（1，90000）
对于一个电影，帧是这样来显示的：I B B P。现在我们需要在显示B帧之前知道P帧中的信息。因此，帧可能会按照这样的方式来
存储：IPBB。这就是为什么我们会有一个解码时间戳和一个显示时间戳的原因。解码时间戳告诉我们什么时候需要解码，显示时间
戳告诉我们什么时候需要显示。所以，在这种情况下，我们的流可以是这样的：  
PTS: 1 4 2 3  
DTS: 1 2 3 4  
Stream: I P B B  
通常PTS和DTS只有在流中有B帧的时候会不同。  
DTS和PTS  
音频和视频流都有一些关于以多快速度和什么时间来播放它们的信息在里面。音频流有采样，视频流有每秒的帧率。然而，如果我
们只是简单的通过数帧和乘以帧率的方式来同步视频，那么就很有可能会失去同步。于是作为一种补充，在流中的包有种叫做DTS
（解码时间戳）和PTS（显示时间戳）的机制。为了这两个参数，你需要了解电影存放的方式。像MPEG等格式，使用被叫做B帧（B
表示双向bidrectional）的方式。另外两种帧被叫做I帧和P帧（I表示关键帧，P表示预测帧）。I帧包含了某个特定的完整图像。P
帧依赖于前面的I帧和P帧并且使用比较或者差分的方式来编码。B帧与P帧有点类似，但是它是依赖于前面和后面的帧的信息的。这
也就解释了为什么我们可能在调用avcodec_decode_video以后会得不到一帧图像。  
ffmpeg中的时间单位  
AV_TIME_BASE  
ffmpeg中的内部计时单位（时间基），ffmepg中的所有时间都是于它为一个单位，比如AVStream中的duration即以为着这个流的长
度为duration个AV_TIME_BASE。AV_TIME_BASE定义为：  
#define         AV_TIME_BASE   1000000  
AV_TIME_BASE_Q  
ffmpeg内部时间基的分数表示，实际上它是AV_TIME_BASE的倒数。从它的定义能很清楚的看到这点：  
#define         AV_TIME_BASE_Q   (AVRational){1, AV_TIME_BASE}  
AVRatioal的定义如下：  
typedef struct AVRational{  
int num; //numerator  
int den; //denominator  
} AVRational;  
ffmpeg提供了一个把AVRatioal结构转换成double的函数：  
复制代码  
static inline double av_q2d(AVRational a)｛  
/** 
* Convert rational to double. 
* @param a rational to convert 
**/  
    return a.num / (double) a.den;  
}  
复制代码  
现在可以根据pts来计算一桢在整个视频中的时间位置：  
timestamp(秒) = pts * av_q2d(st->time_base)  
计算视频长度的方法：  
time(秒) = st->duration * av_q2d(st->time_base)  
这里的st是一个AVStream对象指针。  
时间基转换公式  
timestamp(ffmpeg内部时间戳) = AV_TIME_BASE * time(秒)  
time(秒) = AV_TIME_BASE_Q * timestamp(ffmpeg内部时间戳)  
所以当需要把视频跳转到N秒的时候可以使用下面的方法：  
int64_t timestamp = N * AV_TIME_BASE;   
2  
av_seek_frame(fmtctx, index_of_video, timestamp, AVSEEK_FLAG_BACKWARD);  
ffmpeg同样为我们提供了不同时间基之间的转换函数：  
int64_t av_rescale_q(int64_t a, AVRational bq, AVRational cq)  
这个函数的作用是计算a * bq / cq，来把时间戳从一个时基调整到另外一个时基。在进行时基转换的时候，我们应该首选这个函
数，因为它可以避免溢出的情况发生。

