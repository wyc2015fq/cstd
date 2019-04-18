# [ffmpeg] 音频样本 - TaigaComplex求职中 - 博客园







# [[ffmpeg] 音频样本](https://www.cnblogs.com/TaigaCon/p/9893847.html)





不仅限于ffmpeg，音频采样所得的PCM都含有三个要素：声道（channel）、采样率（sample rate）、样本格式（sample format）。



# 声道

当人听到声音时，能对声源进行定位，那么通过在不同的位置设置声源，就可以造就出更好的听觉感受，如果配合影像进行音频位置的调整，则会得到更好的视听效果。常见的声道有：
- 单声道，mono
- 双声道，stereo，最常见的类型，包含左声道以及右声道
- 2.1声道，在双声道基础上加入一个低音声道
- 5.1声道，包含一个正面声道、左前方声道、右前方声道、左环绕声道、右环绕声道、一个低音声道，最早应用于早期的电影院
- 7.1声道，在5.1声道的基础上，把左右的环绕声道拆分为左右环绕声道以及左右后置声道，主要应用于BD以及现代的电影院

如下是一个双声道的音频系统

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181102030637917-331121056.png)





# 采样率

音频采样，是把声音从模拟信号转换为数字信号。采样率，就是每秒对声音进行采集的次数，同样也是所得的数字信号的每秒样本数。在对声音进行采样时，常用的采样率有8k（电话）、44.1k（CD）、48k（视频音轨）、96k/192k（Hi-Res）。

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181102030638531-1233276738.png)





# 样本格式

#### 单个声道的样本的编码类型

区别于前文所述的样本，我们这里为其添加了前缀，特指单个声道中的样本。音频在经过采样得到样本后，还需要对该样本执行[两个步骤](https://www.cnblogs.com/TaigaCon/p/8679251.html#ADConvert)：
- 量化。音频量化的量化位数常用的有8bit、16bit、32bit、64bit。
- 
二进制编码。也就是把量化所得的结果，即单个声道的样本，以二进制的码字进行存放。其中有两种存放方式：直接以整形来存放量化结果，即Two's complement code；以浮点类型来存放量化结果，即Floating point encoding code。两者有如下关系：

$\displaystyle{Q_{FLT} = \frac{Q_{INT}}{X_{Range}}}$

其中$X_{Range}$代表该量化器的量化范围，$Q_{INT}$量化器所得出的结果，$Q_{FLT}$则是该结果的浮点表示。量化器所得出的量化结果必定在量化范围之内，因此从上面的式子可以看出，$Q_{FLT}$的绝对值必然小于等于1。




#### 帧（frame）

音频在量化得到二进制的码字后，需要进行变换，而变换（MDCT）是以块为单位（block）进行的，一个块由多个（120或128）样本组成。而一帧内会包含一个或者多个块。

帧的常见大小有960、1024、2048、4096等。

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181102030639020-925667345.png)



#### 帧当中的样本的组合方式
- 交错（interleaved）。以stereo为例，一个stereo音频的样本是由两个单声道的样本交错地进行存储得到的。
- 平面（planar）。各个声道的样本分开进行存储，。

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181105004158435-880727415.png)



#### ffmpeg中的样本格式
enum AVSampleFormat {
    AV_SAMPLE_FMT_NONE = -1,
    AV_SAMPLE_FMT_U8,          ///< unsigned 8 bits
    AV_SAMPLE_FMT_S16,         ///< signed 16 bits
    AV_SAMPLE_FMT_S32,         ///< signed 32 bits
    AV_SAMPLE_FMT_FLT,         ///< float
    AV_SAMPLE_FMT_DBL,         ///< double

    AV_SAMPLE_FMT_U8P,         ///< unsigned 8 bits, planar
    AV_SAMPLE_FMT_S16P,        ///< signed 16 bits, planar
    AV_SAMPLE_FMT_S32P,        ///< signed 32 bits, planar
    AV_SAMPLE_FMT_FLTP,        ///< float, planar
    AV_SAMPLE_FMT_DBLP,        ///< double, planar
    AV_SAMPLE_FMT_S64,         ///< signed 64 bits
    AV_SAMPLE_FMT_S64P,        ///< signed 64 bits, planar

    AV_SAMPLE_FMT_NB           ///< Number of sample formats. DO NOT USE if linking dynamically
};


可见其中有U8（无符号整型8bit）、S16（整型16bit）、S32（整型32bit）、FLT（单精度浮点类型）、DBL（双精度浮点类型）、S64（整型64bit），不以P为结尾的都是interleaved结构，以P为结尾的是planar结构。












