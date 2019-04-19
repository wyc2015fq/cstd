# 图像特征提取与匹配之SIFT算法 - 建建的博客 - CSDN博客
2018年09月17日 22:27:57[纪建](https://me.csdn.net/u013898698)阅读数：2476
推荐阅读：
**David G. Lowe, "Distinctive image features from scale-invariant keypoints," **
International Journal of Computer Vision, 60, 2 (2004), pp. 91-110
---------------------------------------------
**尺度不变特征转换(Scale-invariant feature transform** 或** SIFT)**是一种电脑视觉的算法用来侦测与描述影像中的局部性特征，它在空间尺度中寻找极值点，并提取出其位置、尺度、旋转不变量，此算法由 David Lowe 在1999年所发表，2004年完善总结。
Sift算法就是用不同尺度（标准差）的高斯函数对图像进行平滑，然后比较平滑后图像的差别，
差别大的像素就是特征明显的点。   
**一、Sift算法的步骤**
Sift（Scale Invariant Feature Transform）是一个很好的图像匹配算法，
同时能处理亮度、平移、旋转、尺度的变化，利用特征点来提取特征描述符，最后在特征描述符之间寻找
匹配。
该算法主要包括5个步骤进行匹配：
1、构建尺度空间，检测极值点，获得尺度不变性；
![](http://hi.csdn.net/attachment/201102/15/8394323_1297779319zmxx.jpg)
2、特征点过滤并进行精确定位，剔除不稳定的特征点；
![](http://hi.csdn.net/attachment/201102/15/8394323_1297779319mGLv.jpg)
3、在特征点处提取特征描述符，为特征点分配方向值；
![](http://hi.csdn.net/attachment/201102/15/8394323_1297779319w87p.jpg)
4、生成特征描述子，利用特征描述符寻找匹配点；
以特征点为中心取16*16的邻域作为采样窗口，
将采样点与特征点的相对方向通过高斯加权后归入包含8个bin的方向直方图，
最后获得4*4*8的128维特征描述子。
示意图如下：
![](http://hi.csdn.net/attachment/201102/15/8394323_1297779319K050.gif)
5、计算变换参数。
 当两幅图像的Sift特征向量生成以后，下一步就可以采用关键点特征向量的欧式距离来作为两幅图像中
关键点的相似性判定度量。
取图1的某个关键点，通过遍历找到图像2中的距离最近的两个关键点。
在这两个关键点中，如果次近距离除以最近距离小于某个阙值，则判定为一对匹配点。
最后，看下Sift 算法效果图：
下图左边部分Sift算法匹配结果，右边部分是其它算法匹配结果：
![](http://hi.csdn.net/attachment/201102/15/8394323_1297780941obTq.jpg)
**二、Sift算法的描述**
在上述的Sift算法步骤一中，提到了尺度空间，那么什么是尺度和尺度空间呢？
尺度就是受delta这个参数控制的表示。
而不同的L（x,y,delta)就构成了尺度空间，实际上，具体计算的时候，即使连续的高斯函数，都要被离
散为（一般为奇数大小）(2*k+1) *(2*k+1)矩阵，来和数字图像进行卷积运算。
David Lowe关于Sfit算法，2004年发表在Int. Journal of Computer Vision的经典论文中，
对尺度空间（scal space)是这样定义的 ：
   It has been shown by Koenderink (1984) and Lindeberg (1994) that under a variety of
reasonable assumptions the only possible scale-space kernel is the Gaussian function.
Therefore,the scale space of an image is defined as a function, L(x; y; delta) that is
produced from the convolution of a variable-scale Gaussian, G(x; y; delta), with an input
image, I(x; y):
因此 ，一个图像的尺度空间，L（x,y,delta) ,
定义为原始图像I (x,y)与一个可变尺度的2维高斯函数G(x,y,delta) 卷积运算。
即，原始影像I(x，y)在不同的尺度e下，与高斯滤波器G(x,y,e)进行卷积，得到L(x,y,e)，如下：
**L(x,y,e) = G(x,y,e)*I(x,y)**
其中G(x,y,e)是尺度可变高斯函数， 
**G(x,y,e) = [1/2*pi*e2] * exp[ -(x2 + y2)/2e2] **
（x，y）是空间坐标， e是尺度坐标。
为了更有效的在尺度空间检测到稳定的关键点，提出了高斯差分尺度空间（DOG scale-space）。
利用不同尺度的高斯差分核与原始图像I(x,y) ，卷积生成。
**D(x,y,e) = ((G(x,y,ke) - G(x,y,e)) * I(x,y)                               = L(x,y,ke) - L(x,y,e)**
DOG算子计算简单，是尺度归一化的LoG算子的近似。
Gaussian卷积是有尺寸大小的，使用同一尺寸的滤波器对两幅包含有不同尺寸的同一物体的图像求局部最值将有可能出现一方求得最值而另一方却没有的情况，但是容易知道假如物体的尺寸都一致的话它们的局部最值将会相同。
SIFT的精妙之处在于采用图像金字塔的方法解决这一问题，我们可以把两幅图像想象成是连续的，分别以它们作为底面作四棱锥，就像金字塔，那么每一个 截面与原图像相似，那么两个金字塔中必然会有包含大小一致的物体的无穷个截面，但应用只能是离散的，所以我们只能构造有限层，层数越多当然越好，但处理时 间会相应增加，层数太少不行，因为向下采样的截面中可能找不到尺寸大小一致的两个物体的图像。
有了图像金字塔就可以对每一层求出局部最值，但是这样的稳定 点数目将会十分可观，所以需要使用某种方法抑制去除一部分点，但又使得同一尺度下的稳定点得以保存
图像金字塔的构建：图像金字塔共O组，每组有S层，下一组的图像由上一组图像降采样得到。
如下图：
![](http://hi.csdn.net/attachment/201102/15/8394323_12977793994DK5.jpg)
**三、Sift算法的实现**
作为一种匹配能力较强的局部描述算子，SIFT算法的实现相当复杂，
不过David Lowe到底也还是用c++实现了它，头文件里就下述两个关键函数，下面具体阐述下。
**函数一：**
int sift_features( IplImage* img, struct feature** feat )
这个函数就是用来提取图像中的特征向量。
参数img为一个指向IplImage数据类型的指针，用来表示需要进行特征提取的图像。
IplImage是opencv库定义的图像基本类型（关于opencv是一个著名的图像处理类库，详细的介绍可以参见（[http://www.opencv.org.cn](http://www.opencv.org.cn/)）。
参数feat 是一个数组指针，用来存储图像的特征向量。
函数调用成功将返回特征向量的数目，否则返回-1.
函数，完整表述如下：
**[cpp]**[view plain](http://blog.csdn.net/v_july_v/article/details/6186942#)[copy](http://blog.csdn.net/v_july_v/article/details/6186942#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1774132/fork)
- int sift_features( IplImage* img, struct feature** feat )  
- {  
-   return _sift_features( img, feat, SIFT_INTVLS, SIFT_SIGMA, SIFT_CONTR_THR,  
-    SIFT_CURV_THR, SIFT_IMG_DBL, SIFT_DESCR_WIDTH,  
-    SIFT_DESCR_HIST_BINS );  
- }  
如你所见，函数一sift_features调用下面的函数二。
**函数二：**
int _sift_features( IplImage* img, struct feature** feat, int intvls,double sigma, double
contr_thr, int curv_thr, int img_dbl, int descr_width, int descr_hist_bins )
稍微介绍下此函数的几个参数： 
intvls: 每个尺度空间的采样间隔数，默认值为3.
sigma: 高斯平滑的数量，默认值1.6.
contr_thr:判定特征点是否稳定，取值（0，1），默认为0.04，这个值越大，被剔除的特征点就越多。
curv_thr:判定特征点是否边缘点，默认为6.
img_dbl:在建立尺度空间前如果图像被放大了1倍则取值为1，否则为0.
descr_width:计算特征描述符时邻域子块的宽度，默认为4.
descr_hist_bins:计算特征描述符时将特征点邻域进行投影的方向数，默认为8，分别是0，45，90，135
，180，215，270，315共8个方向。
**以下是此函数二的完整表述：**
**[cpp]**[view plain](http://blog.csdn.net/v_july_v/article/details/6186942#)[copy](http://blog.csdn.net/v_july_v/article/details/6186942#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1774132/fork)
- int _sift_features( IplImage* img, struct feature** feat, int intvls,  
-        double sigma, double contr_thr, int curv_thr,  
-        int img_dbl, int descr_width, int descr_hist_bins )  
- {  
-  IplImage* init_img;  
-  IplImage*** gauss_pyr, *** dog_pyr;  
-  CvMemStorage* storage;  
-  CvSeq* features;  
-  int octvs, i, n = 0;  
-  /* check arguments */  
-  if( ! img )  
-   fatal_error( "NULL pointer error, %s, line %d",  __FILE__, __LINE__ );  
-  if( ! feat )  
-   fatal_error( "NULL pointer error, %s, line %d",  __FILE__, __LINE__ );  
-  /* build scale space pyramid; smallest dimension of top level is ~4 pixels */  
-  init_img = create_init_img( img, img_dbl, sigma );  
-  octvs = log( MIN( init_img->width, init_img->height ) ) / log(2) - 2;  
-  gauss_pyr = build_gauss_pyr( init_img, octvs, intvls, sigma );  
-  dog_pyr = build_dog_pyr( gauss_pyr, octvs, intvls );  
-  storage = cvCreateMemStorage( 0 );  
-  features = scale_space_extrema( dog_pyr, octvs, intvls, contr_thr,  
-   curv_thr, storage );  
-  calc_feature_scales( features, sigma, intvls );  
-  if( img_dbl )  
-   adjust_for_img_dbl( features );  
-  calc_feature_oris( features, gauss_pyr );  
-  compute_descriptors( features, gauss_pyr, descr_width, descr_hist_bins );  
-  /* sort features by decreasing scale and move from CvSeq to array */  
-  cvSeqSort( features, (CvCmpFunc)feature_cmp, NULL );  
-  n = features->total;  
-  *feat = calloc( n, sizeof(struct feature) );  
-  *feat = cvCvtSeqToArray( features, *feat, CV_WHOLE_SEQ );  
-  for( i = 0; i < n; i++ )  
-  {  
-   free( (*feat)[i].feature_data );  
-   (*feat)[i].feature_data = NULL;  
-  }  
-  cvReleaseMemStorage( &storage );  
-  cvReleaseImage( &init_img );  
-  release_pyr( &gauss_pyr, octvs, intvls + 3 );  
-  release_pyr( &dog_pyr, octvs, intvls + 2 );  
-  return n;  
- }  
这个函数是上述函数一的重载，作用是一样的，实际上函数一只不过是使用默认参数调用了函数二，
核心的代码都是在函数二中实现的。
sift创始人David Lowe的完整代码，包括他的论文，请到此处下载：
[http://www.cs.ubc.ca/~lowe/keypoints](http://www.cs.ubc.ca/~lowe/keypoints)
日后，本BLOG内，会具体剖析下上述David Lowe的Sift算法代码。
Rob Hess维护的sift 库：
[http://blogs.oregonstate.edu/hess/code/sift/](http://blogs.oregonstate.edu/hess/code/sift/)
还可，参考这里：
sift图像特征提取与匹配算法代码(友人，onezeros博客)：
[http://blog.csdn.net/onezeros/archive/2011/01/05/6117704.aspx](http://blog.csdn.net/onezeros/archive/2011/01/05/6117704.aspx)
