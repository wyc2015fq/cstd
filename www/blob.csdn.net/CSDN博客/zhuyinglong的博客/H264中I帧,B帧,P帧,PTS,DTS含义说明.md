# H264中I帧,B帧,P帧,PTS,DTS含义说明 - zhuyinglong的博客 - CSDN博客





2017年09月22日 17:07:45[初见时的微笑](https://me.csdn.net/zhuyinglong2010)阅读数：315








声明：下文所提到的概念说明均摘自于网络，加上个人理解总结而成。


基本概念：

I frame ：帧内编码帧 又称intra picture，I 帧通常是每个 GOP（MPEG 所使用的一种视频压缩技术）的第一个帧，经过适度地压缩，做为随机访问的参考点，可以当成图象。I帧可以看成是一个图像经过压缩后的产物。

P frame: 前向预测编码帧 又称predictive-frame，通过充分将低于图像序列中前面已编码帧的时间冗余信息来压缩传输数据量的编码图像，也叫预测帧。

B frame: 双向预测内插编码帧 又称bi-directional interpolated prediction frame，既考虑与源图像序列前面已编码帧，也顾及源图像序列后面已编码帧之间的时间冗余信息来压缩传输数据量的编码图像，也叫双向预测帧。

PTS：Presentation Time Stamp。PTS主要用于度量解码后的视频帧什么时候被显示出来。

DTS：Decode Time Stamp。DTS主要是标识读入内存中的ｂｉｔ流在什么时候开始送入解码器中进行解码。

ＩＰＢ帧的不同：

I frame:自身可以通过视频解压算法解压成一张单独的完整的图片。

P frame：需要参考其前面的一个I frame 或者B frame来生成一张完整的图片。

B frame:则要参考其前一个I或者P帧及其后面的一个P帧来生成一张完整的图片。注意是及其，是必须有个P帧。

两个I frame之间形成一个GOP，在x264中同时可以通过参数来设定bf的大小，即：I 和p或者两个P之间B的数量。通过上述基本可以说明如果有B frame 存在的情况下一个GOP的最后一个frame一定是P。

DTS和PTS的不同：

DTS主要用于视频的解码,在解码阶段使用.PTS主要用于视频的同步和输出。在display的时候使用.在没有B frame的情况下.DTS和PTS的输出顺序是一样的。

例子:

下面给出一个GOP为15的例子,其解码的参照frame及其解码的顺序都在里面：

![](https://img-blog.csdn.net/20170922171007710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


如上图：I frame 的解码不依赖于任何的其它的帧。p frame的解码则依赖于其前面的I frame或者P frame。B frame的解码则依赖于其前的最近的一个I frame或者P frame及其后的最近的一个P frame。




