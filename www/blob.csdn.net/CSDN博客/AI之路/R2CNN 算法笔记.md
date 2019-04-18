# R2CNN 算法笔记 - AI之路 - CSDN博客





2018年09月01日 09:05:10[AI之路](https://me.csdn.net/u014380165)阅读数：1490








论文：R2CNN: Rotational Region CNN for Orientation Robust Scene Text Detection 

论文链接：[https://arxiv.org/abs/1706.09579](https://arxiv.org/abs/1706.09579)

这篇文章提出了R2CNN（Rotational Region CNN）算法解决旋转文本的检测。

**Figure2是R2CNN算法的整体结构**，主要是在Faster RCNN算法的基础上做了一些修改：1、ROI Pooling时的尺寸除了7*7外，还有两种长宽不一致的尺寸：3*11和11*3，这种设置的用意也非常明显，就是为了解决水平和竖直长文本的检测。然后对于提取到的ROI特征做cancat操作进行融合作为后续预测支路的输入。2、预测输出有3个支路，第一个支路是有无文本的二分类，这个和目标检测算法中的目标分类类似。第二个支路是水平框（axis-aligned box）的预测，这个和目标检测算法中的框预测一样。第三个支路时倾斜框（inclined box）的预测，这部分是这篇文章的亮点，而且该支路后面跟一个NMS进行处理得到最后结果。至于RPN网络部分输出的ROI则和常规目标检测中RPN网络输出的ROI一样，都是水平方向。另外这篇文章其实还增加了一些小尺寸的anchor提升对小文本的检测效果。**所以R2CNN算法最后既有常规的水平预测框输出，也有倾斜框输出，这两种框都是基于RPN网络输出的ROI得到的，虽然倾斜框支路也能预测水平框，但是作者认为第二个支路的存在对最后结果帮助较大。**
![这里写图片描述](https://img-blog.csdn.net/20180901090255894?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**因此R2CNN算法整体上的处理流程可以用Figure1所示**。(a)是输入图像。(b)是RPN网络得到的ROI，这些ROI区域都是常规的水平框。(c)是R2CNN算法预测得到的水平预测框（第二个支路）和倾斜预测框（第三个支路）的结果。(d)是经过倾斜框的NMS算法后得到的最终输出结果。 
![这里写图片描述](https://img-blog.csdn.net/20180901090315877?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Figure3是关于这篇文章用的倾斜框定义**。框其实表示方式非常多，比如可以用Figure3(a)所示的4个点坐标来表示一个任意形状的四边形，这种表示方式其实就覆盖了水平框和倾斜框，而且框的形状不仅限于矩形，而是延伸至四边形。但这篇文章不采用任意形状的四边形预测方式，而认为倾斜矩形框足够覆盖待检测的文本，**因此这篇文章所提到的倾斜框是指倾斜的矩形框，为了描述简单，后续都用倾斜框代替倾斜的矩形框**。倾斜框的定义也有好几种，比如可以用水平框+旋转角度来表示一个倾斜框，这种表示方式在旋转目标检测算法中也比较常见，比如east算法。但是这篇文章认为训练旋转角度时在有些角度情况下模型不是很稳定，主要是因为一些特殊的相似角度在表示上有可能差别较大，比如90度和-90度，**所以考虑采用Figure3(b)(c)这种用两个相邻点坐标和高来定义一个倾斜框。**
![这里写图片描述](https://img-blog.csdn.net/20180901090353232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Figure4是关于倾斜框的NMS算法**。(a)是R2CNN算法的水平框和倾斜框预测结果合并在一张图上的结果。(b)是采用水平框的NMS算法处理(a)中的预测框后得到的结果，可以看到有部分正确的文本框被剔除掉（红色虚线）。(c)是采用倾斜框的NMS算法处理(a)中的预测框后得到的结果，可以看到结果比较好。基于倾斜框的NMS算法和传统的基于水平矩形框的NMS算法差别不大，只不过计算对象换成两个倾斜框，这个操作还是比较重要的。 
![这里写图片描述](https://img-blog.csdn.net/20180901090410397?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
损失函数方面从Figure2的网络结构图也可以看出一共包含3个部分：1、有无文本的二分类损失Lcls。2、水平框的回归损失Lreg(vi,vi*)。3、倾斜框的回归损失Lreg(ui,ui*)。后面两部分都是采用目标检测中常用的smooth L1损失函数，所以损失函数方面没有太大的改动。 
![这里写图片描述](https://img-blog.csdn.net/20180901090422718?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实验结果：**

Table1是在IDCAR 2015数据集上不同参数配置时R2CNN测试结果。 
![这里写图片描述](https://img-blog.csdn.net/20180901090436502?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table2是R2CNN算法和其他算法在IDCAR2015数据集上的对比结果。 
![这里写图片描述](https://img-blog.csdn.net/2018090109044860?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table3是R2CNN算法和其他算法在IDCAR2013数据集上的对比结果。 
![这里写图片描述](https://img-blog.csdn.net/20180901090459919?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)










