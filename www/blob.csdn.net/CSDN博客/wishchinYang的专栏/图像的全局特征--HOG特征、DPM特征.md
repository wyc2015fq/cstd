# 图像的全局特征--HOG特征、DPM特征 - wishchinYang的专栏 - CSDN博客
2017年03月16日 15:31:42[wishchin](https://me.csdn.net/wishchin)阅读数：3293
个人分类：[计算机视觉																[人脸识别																[OpenCV																[图像特征](https://blog.csdn.net/wishchin/article/category/1860001)](https://blog.csdn.net/wishchin/article/category/5778933)](https://blog.csdn.net/wishchin/article/category/1508467)](https://blog.csdn.net/wishchin/article/category/1871617)
       HOG特征：方向梯度直方图（Histogram of Oriented Gradient,）特征是一种全局图像特征描述子。
       它通过计算和统计图像局部区域的梯度方向直方图来构成特征。Hog特征结合SVM分类器已经被广泛应用于图像识别中，尤其在行人检测中获得了极大的成功。需要提醒的是，HOG+SVM进行行人检测的方法是法国研究人员Dalal在2005的CVPR上提出的，而如今虽然有很多行人检测算法不断提出，但基本都是以HOG+SVM的思路为主。
       参考原文：[目标检测之特征提取之—HOG特征](http://blog.csdn.net/zouxy09/article/details/7929348/)  如有疑义，请拜访原文。
        RGB的DPM检测特征和其级联方法是最后一个有效的传统的目标检测方法，翻译见[DPM原理详解](http://blog.csdn.net/qq_14845119/article/details/52625426)，此后，如有疑义，请拜访原文，作者的主页面：[http://www.rossgirshick.info/](http://www.rossgirshick.info/)。
        DPM方法获得cvpr2018的**PAMI Longuet-Higgins Prize。[CVPR2018](http://cvpr2018.thecvf.com/program/main_conference)；**
- 
**PAMI Longuet-Higgins Prize (Retrospective Best Paper from CVPR 2008)**
	"A Discriminatively Trained, Multiscale, Deformable Part Model" by Pedro Felzenszwalb, David McAllester, and Deva Ramanan
       DPM项目主页：[Object Detection with Discriminatively Trained Part Based Models](https://dl.dropboxusercontent.com/s/u1rvydtkt9nioyq/Object-Detection-with-Discriminatively-Trained-Part-Based-Models--Felzenszwalb-Girshick-McAllester-Ramanan.pdf?dl=0)
       See also, CACM Research Highlight:
[Visual Object Detection with Deformable Part Models](http://dl.acm.org/citation.cfm?id=2494532)
       P. Felzenszwalb, **R. Girshick**, D. McAllester, D. Ramanan   Communications of the ACM, no. 9 (2013): 97-105
**一、HOG+SVM模型**
**1、HOG描述子的计算过程**
![](https://img-blog.csdn.net/20170316151539401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
输入图像image：
1）灰度化（ 将图像看做一个x,y,z（灰度）的三维图像）；
2）采用Gamma校正法对输入图像进行颜色空间的标准化（归一化）；目的是调节图像的对比度，降低图像局部的阴影和光照变化所造成的影响，同时可以抑制噪音的干扰；
3）计算图像每个像素的梯度（包括大小和方向）；主要是为了捕获轮廓信息，同时进一步弱化光照的干扰。
4）将图像划分成小cells（例如6*6像素/cell）；
5）统计每个cell的梯度直方图（不同梯度的个数），即可形成每个cell的descriptor； 原始HOG特征划分为9个直方图区间。
**意义**：计算梯度直方图，可以降低光线明暗变化带来的影响，保持良好的光照不变性和特定的旋转不变性。
6）将每几个cell组成一个block（例如3*3个cell/block），一个block内所有cell的特征descriptor串联起来便得到该block的HOG特征descriptor。（此处cell和block，每个block可以多个cell，相邻cell之间产生特定像素的重叠，可以有效的抵挡偏移影响。行人检测的最佳参数设置是：3×3细胞/区间、6×6像素/细胞、**9个直方图通道**。则一块的特征数为：3*3*9）
**意义**：覆盖划分区域用于降低图像块偏移带来的影响；并同时保持特定区域相关性。
7）将图像image内的所有block的HOG特征descriptor串联起来就可以得到该image（你要检测的目标）的HOG特征descriptor了。这个就是最终的可供分类使用的特征向量了。 
![](https://img-blog.csdn.net/20170316152818375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       以**220X310**大小图像为例，经过缩放处理后为**216x304**，但并不直接提取整个图像的**HOG**特征，而是用一个固定大小的窗口在图像上滑动，滑动的间隔为8个像素，**opencv**中默认的窗口大小为**128x64**（高**128**，宽**64**），即有**(128÷8)x(64÷8)=16x8**个**cell**，也即有**15x7**个**block**，这样一来一幅图像就可以取到**(27-16)x(38-8)=11x30=330**个窗口。现在提取每个窗口的**HOG**特征，则可得到** 105x36=3780** 维**HOG**特征向量。
**2、HOG特征的全局性质**
      HOG特征并非固定长度约束的特征描述子。向量的维数和图像大小和Cell大小有关。每一个Cell里面统计梯度直方图，形成每个Cell的梯度特征。其中每个Cell里面提取的特征可以使用其他块特征取代。
**2.1. HOG优点：**
         与其他的特征描述方法相比，HOG有很多优点。首先，由于HOG是在图像的局部方格单元上操作，所以它对图像几何的和光学的形变都能保持很好的不变性，这两种形变只会出现在更大的空间领域上。其次，在粗的空域抽样、精细的方向抽样以及较强的局部光学归一化等条件下，只要行人大体上能够保持直立的姿势，可以容许行人有一些细微的肢体动作，这些细微的动作可以被忽略而不影响检测效果。因此HOG特征是特别适合于做图像中的人体检测的。
         全局变形性，局部不变形，在区分行人特征和左右眼特征时具有天然的优势。当然，HOG思想是一个算法族，Cell和Block思想是主要思想，重叠度直方图分区块大小可以根据实际情况进行调参，在一定的数据集上，力求达到最佳效果。
**3、使用OpenCV进行HOG特征计算**
OpenCV的ObjectDetect模块包含了HOG计算的源代码，使用例程如下
```cpp
bool CEyeClassify::GetHogFeature(const cv::Mat& imggray, std::vector<float>& vecFeature)
{
	const int cnSizeW = CEyeClassify::m_PatchWidth;
	const int cnSizeH = CEyeClassify::m_PatchHeight;
	{
		cv::Mat imggray1;
		resize(imggray, imggray1, cv::Size(cnSizeW, cnSizeH), 0, 0, CV_INTER_LINEAR);
		cv::HOGDescriptor hog(imggray1.size(), cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 9); // 48x32  540
		hog.compute(imggray1, vecFeature);
	}
	return true;
}
```
**二、DPM+Latent**S**VM模型**
**     DPM**（**Deformable Part Model**），正如其名称所述，可变形的组件模型，是一种基于组件的检测算法，其所见即其意。该模型由大神**Felzenszwalb**在2008年提出，并发表了一系列的cvpr，NIPS。并且还拿下了2010年，**PASCAL VOC**的“终身成就奖”。
**1、DPM描述子的计算过程**
输入图像image
![](https://img-blog.csdn.net/20180322164624857)
**2、DPM描述子的检测过程**
输入图像image
![](https://img-blog.csdn.net/20180322164652579)
**3、Latent SVM模型**
输入图像image
![](https://img-blog.csdn.net/20180322164730153)
参考资料：
1.HOG维基百科:[https://en.wikipedia.org/wiki/Histogram_of_oriented_gradients](https://en.wikipedia.org/wiki/Histogram_of_oriented_gradients)
2.[图像检测法三大法宝：HOG,LBP,HAAR](http://dataunion.org/20584.html)特征。
