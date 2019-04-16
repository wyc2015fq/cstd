# HEVC官方软件HM源代码简单分析-解码器TAppDecoder - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年11月23日 13:49:44[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：35930标签：[HM																[HEVC																[TAppDecoder																[解码																[源代码](https://so.csdn.net/so/search/s.do?q=源代码&t=blog)
个人分类：[HM](https://blog.csdn.net/leixiaohua1020/article/category/5974711)







=====================================================

H.264/H.265 官方源代码分析文章：

[H.264官方软件JM源代码简单分析-编码器lencod](http://blog.csdn.net/leixiaohua1020/article/details/49822701)

[H.264官方软件JM源代码简单分析-解码器ldecod](http://blog.csdn.net/leixiaohua1020/article/details/49822613)

[HEVC官方软件HM源代码简单分析-编码器TAppEncoder](http://blog.csdn.net/leixiaohua1020/article/details/49912113)

[HEVC官方软件HM源代码简单分析-解码器TAppDecoder](http://blog.csdn.net/leixiaohua1020/article/details/49912013)

=====================================================


本文记录HEVC官方参考软件HM的源代码结构。HM相比于JM来说，采用了面向对象的编程方法，因而结构相对更加清晰。HM包含了视频解码器TAppDecoder和视频编码器TAppEncoder。本文记录视频解码器TAppDecoder的结构。




## 函数调用关系图

HM中的HEVC视频解码器TAppDecoder的函数调用关系图如下所示。

[![](https://img-blog.csdn.net/20151118194232337)](https://img-my.csdn.net/uploads/201511/16/1447638516_3694.png)


[单击查看更清晰的大图](https://my.csdn.net/leixiaohua1020/album/detail/1816479)


下面解释一下图中关键标记的含义。




> 
**函数背景色**
函数在图中以方框的形式表现出来。不同的背景色标志了该函数不同的作用：
白色背景的函数：普通内部函数。
粉红色背景函数：解析函数（Parser）。这些函数用于解析SPS、PPS等信息。
紫色背景的函数：熵解码函数（Entropy Decoding）。这些函数读取码流数据并且进行CABAC熵解码。
绿色背景的函数：解码函数（Decode）。这些函数通过帧内预测、帧间预测、DCT反变换等方法解码CU压缩数据。

黄色背景的函数：环路滤波函数（Loop Filter）。这些函数对解码后的数据进行滤波，去除方块效应和振铃效应。



**箭头线**
箭头线标志了函数的调用关系：
黑色箭头线：不加区别的调用关系。
粉红色箭头线：解析函数（Parser）之间的调用关系。
紫色箭头线：熵解码函数（Entropy Decoding）之间的调用关系。
绿色箭头线：解码函数（Decode）之间的调用关系。
黄色箭头线：环路滤波函数（Loop Filter）之间的调用关系。

**函数所在的文件**
每个函数标识了它所在的文件路径。




下文记录结构图中的几个关键部分。


## 普通内部函数
普通内部函数指的是TAppDecoder中还没有进行分类的函数。例如：
解码器的main()函数中调用的TAppDecTop类相关的create()、parseCfg()、decode()、destroy()等方法。
解码器最主要的解码函数：TDecTop中的decode()、xDecodeSlice()；TDecGop中的decompressSlice()；TDecSlice中的decompressSlice()等。


## 解析函数（Parser）
解析函数（Parser）用于解析HEVC码流中的一些信息（例如SPS、PPS等）。在TDecTop的decode()中都调用这些解析函数完成了解析。下面举几个解析函数的例子。

> xDecodeVPS()：解析VPS；
xDecodeSPS()：解析SPS；
xDecodePPS()：解析PPS；


## 熵解码函数（Entropy Decoding）
熵解码函数（Entropy Decoding）读取码流数据并且进行CABAC熵解码。熵解码工作是在TDecCu的decodeCtu ()函数中完成的。其中递归调用了xDecodeCU()完成了具体的熵解码工作。


## 解码函数（Decode）
解码函数（Decode）通过帧内预测、帧间预测等方法解码CU压缩数据。解码工作是在TDecCu的decompressCtu()函数中完成的。其中递归调用了xDecompressCU()完成了具体的解码工作。
xDecompressCU()调用xReconIntraQT()完成帧内预测CU的解码；xReconInter()完成帧间预测CU的解码。
xReconIntraQT()调用xIntraRecQT()，而xIntraRecQT()调用xIntraRecBlk()。xIntraRecBlk()调用了TComPrediction类的predIntraAng()完成了帧内预测的工作；调用了TComTrQuant类的invTransformNxN()完成了残差数据DCT反变换的工作。
xReconInter()调用TComPrediction的motionCompensation()完成了运动补偿的工作；调用xDecodeInterTexture()完成了残差数据的DCT反变换工作。motionCompensation()调用了xPredInterUni()完成了单向预测的运动补偿；而调用xPredInterBi()完成了双向预测的运动补偿。其中xPredInterUni()调用xPredInterBlk()完成一个分量块的运动补偿，而xPredInterBlk()调用了TComInterpolationFilter类的filterHor()和filterVer()完成了亚像素的插值工作。
xDecodeInterTexture()调用TComTrQuant类的invRecurTransformNxN()，而invRecurTransformNxN()调用了invTransformNxN()。invTransformNxN()调用xDeQuant()完成了反量化的工作，调用了xIT()完成了DCT反变换的工作。xIT()调用了xITrMxN()完成MxN维的DCT反变换，而xITrMxN()根据DCT矩阵维度的不同，分别调用了partialButterflyInverse4()、partialButterflyInverse8()、partialButterflyInverse16()、partialButterflyInverse32()几种蝶形算法。


## 环路滤波函数（Loop Filter）
环路滤波函数（Loop Filter）对解码后的数据进行滤波，去除方块效应和振铃效应。去块效应滤波是在TDecTop 的executeLoopFilters()中完成的。executeLoopFilters()调用了TDecGop 的filterPicture()。filterPicture()调用了TComLoopFilter类的loopFilterPic()完成了去块效应滤波器的工作；调用TComSampleAdaptiveOffset类的SAOProcess()完成了去除振铃效应的SAO滤波器的工作。








**雷霄骅leixiaohua1020@126.comhttp://blog.csdn.net/leixiaohua1020**](https://so.csdn.net/so/search/s.do?q=解码&t=blog)](https://so.csdn.net/so/search/s.do?q=TAppDecoder&t=blog)](https://so.csdn.net/so/search/s.do?q=HEVC&t=blog)](https://so.csdn.net/so/search/s.do?q=HM&t=blog)




