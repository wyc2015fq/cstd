# HOG特征提取 - 勿在浮砂筑高台 - CSDN博客





2015年05月02日 21:31:40[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：935标签：[HOG																[特征提取](https://so.csdn.net/so/search/s.do?q=特征提取&t=blog)](https://so.csdn.net/so/search/s.do?q=HOG&t=blog)
个人分类：[Image Processing](https://blog.csdn.net/luoshixian099/article/category/2734445)









转载自： [点击打开链接](http://blog.csdn.net/raodotcong/article/details/6239431)


最近要做图像特征提取，可能要用下HOG特征，所以研究了下OpenCV的HOG描述子。OpenCV中的HOG特征提取功能使用了HOGDescriptor这个类来进行封装，其中也有现成的行人检测的接口。然而，无论是OpenCV官方说明文档还是各个中英文网站目前都没有这个类的使用说明，所以在这里把研究的部分心得分享一下。


      首先我们进入HOGDescriptor所在的头文件，看看它的构造函数需要哪些参数。 



**[cpp]**[view
 plain](http://blog.csdn.net/raodotcong/article/details/6239431#)[copy](http://blog.csdn.net/raodotcong/article/details/6239431#)[print](http://blog.csdn.net/raodotcong/article/details/6239431#)[?](http://blog.csdn.net/raodotcong/article/details/6239431#)



- CV_WRAP HOGDescriptor() : winSize(64,128), blockSize(16,16), blockStride(8,8),  
-     cellSize(8,8), nbins(9), derivAperture(1), winSigma(-1),  
-     histogramNormType(HOGDescriptor::L2Hys), L2HysThreshold(0.2), gammaCorrection(true),   
-     nlevels(HOGDescriptor::DEFAULT_NLEVELS)  
- {}  




**[cpp]**[view
 plain](http://blog.csdn.net/raodotcong/article/details/6239431#)[copy](http://blog.csdn.net/raodotcong/article/details/6239431#)[print](http://blog.csdn.net/raodotcong/article/details/6239431#)[?](http://blog.csdn.net/raodotcong/article/details/6239431#)



- CV_WRAP HOGDescriptor(Size _winSize, Size _blockSize, Size _blockStride,  
-               Size _cellSize, int _nbins, int _derivAperture=1, double _winSigma=-1,  
- int _histogramNormType=HOGDescriptor::L2Hys,  
- double _L2HysThreshold=0.2, bool _gammaCorrection=false,  
- int _nlevels=HOGDescriptor::DEFAULT_NLEVELS)  
- : winSize(_winSize), blockSize(_blockSize), blockStride(_blockStride), cellSize(_cellSize),  
- nbins(_nbins), derivAperture(_derivAperture), winSigma(_winSigma),  
- histogramNormType(_histogramNormType), L2HysThreshold(_L2HysThreshold),  
- gammaCorrection(_gammaCorrection), nlevels(_nlevels)  
- {}  




**[cpp]**[view
 plain](http://blog.csdn.net/raodotcong/article/details/6239431#)[copy](http://blog.csdn.net/raodotcong/article/details/6239431#)[print](http://blog.csdn.net/raodotcong/article/details/6239431#)[?](http://blog.csdn.net/raodotcong/article/details/6239431#)



- CV_WRAP HOGDescriptor(const String& filename)  
- {  
-     load(filename);  
- }  




**[cpp]**[view
 plain](http://blog.csdn.net/raodotcong/article/details/6239431#)[copy](http://blog.csdn.net/raodotcong/article/details/6239431#)[print](http://blog.csdn.net/raodotcong/article/details/6239431#)[?](http://blog.csdn.net/raodotcong/article/details/6239431#)



- HOGDescriptor(const HOGDescriptor& d)  
- {  
-     d.copyTo(*this);  
- }  



      我们看到HOGDescriptor一共有4个构造函数，前三个有CV_WRAP前缀，表示它们是从DLL里导出的函数，即我们在程序当中可以调用的函数；最后一个没有上述的前缀，所以我们暂时用不到，它其实就是一个拷贝构造函数。


      下面我们就把注意力放在前面的构造函数的参数上面吧，这里有几个重要的参数要研究下：winSize(64,128), blockSize(16,16), blockStride(8,8), cellSize(8,8), nbins(9)。上面这些都是HOGDescriptor的成员变量，括号里的数值是它们的默认值，它们反应了HOG描述子的参数。这里做了几个示意图来表示它们的含义。

**窗口大小 winSize**

![](http://hi.csdn.net/attachment/201103/11/0_129981025533pB.gif)

**      块大小 blockSize**

![](http://hi.csdn.net/attachment/201103/11/0_1299810322DuP6.gif)

**胞元大小 cellSize**

![](http://hi.csdn.net/attachment/201103/11/0_1299810389kQkY.gif)

**梯度方向数 nbins**


      nBins表示在一个胞元（cell）中统计梯度的方向数目，例如nBins=9时，在一个胞元内统计9个方向的梯度直方图，每个方向为180/9=20度。

**HOG描述子维度**

在确定了上述的参数后，我们就可以计算出一个HOG描述子的维度了。OpenCV中的HOG源代码是按照下面的式子计算出描述子的维度的。 



**[cpp]**[view
 plain](http://blog.csdn.net/raodotcong/article/details/6239431#)[copy](http://blog.csdn.net/raodotcong/article/details/6239431#)[print](http://blog.csdn.net/raodotcong/article/details/6239431#)[?](http://blog.csdn.net/raodotcong/article/details/6239431#)



- size_t HOGDescriptor::getDescriptorSize() const
- {  
-     CV_Assert(blockSize.width % cellSize.width == 0 &&  
-         blockSize.height % cellSize.height == 0);  
-     CV_Assert((winSize.width - blockSize.width) % blockStride.width == 0 &&  
-         (winSize.height - blockSize.height) % blockStride.height == 0 );  
- return (size_t)nbins*  
-         (blockSize.width/cellSize.width)*  
-         (blockSize.height/cellSize.height)*  
-         ((winSize.width - blockSize.width)/blockStride.width + 1)*  
-         ((winSize.height - blockSize.height)/blockStride.height + 1);  
- }  
- 






