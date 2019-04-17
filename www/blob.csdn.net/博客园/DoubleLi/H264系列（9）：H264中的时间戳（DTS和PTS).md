# H264系列（9）：H264中的时间戳（DTS和PTS) - DoubleLi - 博客园






# （1）Ffmpeg中的DTS 和 PTS

H264里有两种时间戳：DTS（Decoding Time Stamp）和PTS（Presentation Time Stamp）。 顾名思义，前者是解码的时间，后者是显示的时间。要仔细理解这两个概念，需要先了解FFmpeg中的packet和frame的概念。

FFmpeg中用AVPacket结构体来描述解码前或编码后的压缩包，用AVFrame结构体来描述解码后或编码前的信号帧。 对于视频来说，AVFrame就是视频的一帧图像。这帧图像什么时候显示给用户，就取决于它的PTS。DTS是AVPacket里的一个成员，表示这个压缩包应该什么时候被解码。 如果视频里各帧的编码是按输入顺序（也就是显示顺序）依次进行的，那么解码和显示时间应该是一致的。可事实上，在大多数编解码标准（如H.264或HEVC）中，编码顺序和输入顺序并不一致。 于是才会需要PTS和DTS这两种不同的时间戳。

这就需要深入了解I B P三种帧类型与 DTS PTS的关系

# （2）I,P，B帧和PTS，DTS的关系



基本概念：

I frame ：帧内编码帧 又称intra picture，I 帧通常是每个 GOP（MPEG 所使用的一种视频压缩技术）的第一个帧，经过适度地压缩，做为随机访问的参考点，可以当成图象。I帧可以看成是一个图像经过压缩后的产物。

P frame: 前向预测编码帧 又称predictive-frame，通过充分将低于图像序列中前面已编码帧的时间冗余信息来压缩传输数据量的编码图像，也叫预测帧；

B frame: 双向预测内插编码帧 又称bi-directional interpolated prediction frame，既考虑与源图像序列前面已编码帧，也顾及源图像序列后面已编码帧之间的时间冗余信息来压缩传输数据量的编码图像，也叫双向预测帧；

PTS：Presentation Time Stamp。PTS主要用于度量解码后的视频帧什么时候被显示出来

DTS：Decode Time Stamp。DTS主要是标识读入内存中的ｂｉｔ流在什么时候开始送入解码器中进行解码。

在没有B帧存在的情况下DTS的顺序和PTS的顺序应该是一样的。

ＩＰＢ帧的不同：

I frame:自身可以通过视频解压算法解压成一张单独的完整的图片。

P frame：需要参考其前面的一个I frame 或者P frame来生成一张完整的图片。

B frame:则要参考其前一个I或者P帧及其后面的一个P帧来生成一张完整的图片。

两个I frame之间形成一个GOP，在x264中同时可以通过参数来设定bf的大小，即：I 和p或者两个P之间B的数量。

通过上述基本可以说明如果有B frame 存在的情况下一个GOP的最后一个frame一定是P.

DTS和PTS的不同：

DTS主要用于视频的解码,在解码阶段使用.PTS主要用于视频的同步和输出.在display的时候使用.在没有B frame的情况下.DTS和PTS的输出顺序是一样的.

例子:

下面给出一个GOP为15的例子,其解码的参照frame及其解码的顺序都在里面:

![](http://img.blog.csdn.net/20170414133231112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWZseWVhZ2xlbmt1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如上图：I frame 的解码不依赖于任何的其它的帧.而p frame的解码则依赖于其前面的I frame或者P frame。B frame的解码则依赖于其前的最近的一个I frame或者P frame 及其后的最近的一个P frame。

# （3）通俗理解

这2个概念经常出现在音频视频编码和播放中，其实际意义是，PTS是真正录制和播放的时间戳，而DTS是解码的时间戳。

对于普通的无B桢视频(H264 Baseline或者VP8)，PTS/DTS应该是相等的，因为没有延迟编码。

对于有B桢的视频，I桢的PTS依然等于DTS, P桢的PTS>DTS, B桢的PTS<DTS。

可以简单地这样理解：

若视频没有B帧，则I和P都是解码后即刻显示。

若视频含有B帧，则I是解码后即刻显示，P是先解码后显示，B是后解码先显示。（B 和P的先、后是相对的）。



from:http://blog.csdn.net/aflyeaglenku/article/details/70171371









