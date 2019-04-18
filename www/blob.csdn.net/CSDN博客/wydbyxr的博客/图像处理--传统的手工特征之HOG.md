# 图像处理--传统的手工特征之HOG - wydbyxr的博客 - CSDN博客
2018年10月30日 10:27:24[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：302
# 方向梯度直方图（Histogram of Oriented Gradient, HOG）特征是
  一种在计算机视觉和图像处理中用来进行物体检测的特征描述子。
**至今虽然有很多行人检测算法，但基本都是以HOG+SVM的思路为主。**
  参考资料：[http://blog.csdn.net/liulina603/article/details/8291093](http://blog.csdn.net/liulina603/article/details/8291093)
## 计算过程
  HOG特征通过计算和统计图像局部区域的梯度方向直方图来构成特征。
  首先将图像分成小的连通区域，这些连通区域被叫做细胞单元。然后采集细胞单元中各像素点的梯度的或边缘的方向直方图。最后把这些直方图组合起来，就可以构成特征描述符。
  1）将图像划分成小cells（例如6*6像素/cell）；
  2）统计每个cell的梯度直方图（不同梯度的个数），即可形成每个cell的descriptor；
  例如：每个cell为6*6个像素。假设我们采用9个bin的直方图来统计这6*6个像素的梯度信息。也就是将cell的梯度方向360度分成9个方向块。例如：如果这个像素的梯度方向是20-40度，直方图第2个bin的计数就加一，这样，对cell内每个像素用梯度方向在直方图中进行加权投影（映射到固定的角度范围），就可以得到这个cell的梯度方向直方图了，就是该cell对应的9维特征向量（因为有9个bin）。
  3）将每几个cell组成一个block（例如3*3个cell/block），一个block内所有cell的特征descriptor串联起来便得到该block的HOG特征descriptor。
  4）将图像image内的所有block的HOG特征descriptor串联起来就可以得到该image（你要检测的目标）的HOG特征descriptor了。这个就是最终的可供分类使用的特征向量了。
