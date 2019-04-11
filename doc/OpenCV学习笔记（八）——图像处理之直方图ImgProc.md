# OpenCV学习笔记（八）——图像处理之直方图ImgProc

2011年11月02日 18:18:24

yang_xian521

阅读数：28825

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

直方图histograms也是图像处理中经常用到的一种手段。新版本对直方图不再使用之前的histogram的形式，而是用统一的Mat或者MatND的格式来存储直方图，可见新版本Mat数据结构的优势。先介绍下其相关的函数

calcHist、calcBackProject、compareHist、EMD、equalizeHist。除了这几个常用的函数以为，还有一些c函数写的直方图类CvHistogram的相关操作，如下：cvCalcBackProjectPatch、cvCalcProbDensity、cvClearHist、cvCopyHist、cvCreateHist、cvGetHistValue_XD、cvGetMinMaxHistValue、cvMakeHistHeaderForArray、cvNormalizeHist、QueryHistValue_XD、cvReleaseHist、cvSetHistBinRanges、cvThreshHist、cvCalcPGH

calcHist函数为计算图像的直方图，使用方法如下：



```cpp
// C++: 
void calcHist(const Mat* arrays, int narrays, const int* channels, InputArray mask, OutputArray hist, int dims, const int* histSize, const ﬂoat** ranges, bool uniform=true, bool accumulate=false )
// C++: 
void calcHist(const Mat* arrays, int narrays, const int* channels, InputArray mask, SparseMat& hist, int dims, const int* histSize, const ﬂoat** ranges, bool uniform=true, bool accumulate=false )
```

arrays为输入图像指针，narrays为输入图像的个数，channels为用来计算直方图的通道列表，mask为掩膜矩阵，不为空的时候，只计算arrays中的掩膜区域的直方图，hist为输出的直方图矩阵，dims为直方图矩阵的维度，histSize为每一维直方图矩阵的大小，ranges为每一维直方图元素的取值范围，是一个2维数组的地址，uniform为直方图是否为统一模式，统一模式下会拉伸为range的大小，accumulate为累计标志，方便直方图的更新，不需要重新计算

举几个实例方便大家理解：

对于图像为灰度图，调用方式如下：



```cpp
int histSize = 255;
float ranges[] = {0, 255};
const float* histRange = {ranges};
calcHist(&img, 1, 0, Mat(), hist, 1, &histSize, &histRange);
```

直方图的归一化

已经不再适合cvNormalizeHist这个函数了，只需要用对矩阵的归一化函数

normalize

就可以实现了。 



直方图均衡化函数为equalizeHist，这个函数比较简单，这里就不详细介绍了

直方图的比较函数为compareHist，函数返回值为两矩阵的相似程度，相似度衡量的办法目前支持4种

> – CV_COMP_CORREL Correlation相关系数，相同为1，相似度范围为[ 1, 0 )
>
> – CV_COMP_CHISQR Chi-Square卡方，相同为0，相似度范围为[ 0, +inf )
>
> – CV_COMP_INTERSECT Intersection直方图交,数越大越相似，，相似度范围为[ 0, +inf )
>
> – CV_COMP_BHATTACHARYYA Bhattacharyya distance做常态分别比对的Bhattacharyya 距离，相同为0,，相似度范围为[ 0, +inf )

 计算反向投影图函数为

calcBackProject

。所谓反向投影图就是一个概率密度图。calcBackProject的输入为图像及其直方图，输出与待跟踪图像大小相同，每一个像素点表示该点为目标区域的概率。这个点越亮，该点属于物体的概率越大。关于反向直方图，可以参考一下这篇文章

http://blog.163.com/thomaskjh@126/blog/static/370829982010112810358501/

，这个函数使我们利用特征直方图寻找图片中的特征区域变得更加方便容易。这里举一个比较常用的例子：如果已经有一个肤色的特征直方图，则可以在待检测图像中利用直方图方向投影图找出图片中的肤色区域。