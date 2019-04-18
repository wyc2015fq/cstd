# 图像局部显著性—点特征（SURF） - wishchinYang的专栏 - CSDN博客
2017年03月16日 12:02:07[wishchin](https://me.csdn.net/wishchin)阅读数：1701
       1999年的SIFT（ICCV 1999，并改进发表于IJCV 2004，[本文描述](http://blog.csdn.net/wishchin/article/details/45146117)）；参考描述：[图像特征点描述](http://blog.csdn.net/wishchin/article/details/45146117)。
       参考原文：[SURF特征提取分析](http://blog.csdn.net/songzitea/article/details/16986423) 本文有大量删除，如有疑义，请参考原文。    
SURF对SIFT的改进：
      引用Wiki百科中对SURF描述为：“ SURF (Speeded Up Robust Features) is a robust local feature detector, first presented by Herbert Bay et al. in 2006, that can be used in computer vision tasks likeobject recognition or 3D reconstruction. It is partly inspired by the SIFT descriptor.The standard version of SURF is several times faster than SIFT and claimed by its authors to be more robust against different image transformations than SIFT. SURF is based on sums of2D Haar wavelet responses and makes an efficient use ofintegral images.It uses an integer approximation to the determinant of Hessian blob detector, which can be computed extremely quickly with an integral image (3 integer operations). For features, it uses the sum of　the Haar wavelet response around the point of interest. Again, these can be computed with the aid of the integral image".
       从上述对SURF描述，可知：第一、SURF算法是对SIFT算法加强版，同时加速的具有鲁棒性的特征。第二、标准的SURF算子比SIFT算子快好几倍，并且在多幅图片下具有更好的鲁棒性。SURF最大的特征在于采用了harr特征以及积分图像integral image的概念，这大大加快了程序的运行时间。
算法描述：
        为了实现尺度不变性的特征点检测与匹配，SURF算法则先利用Hessian矩阵确定候选点，然后进行非极大抑制，计算复杂度降低多了。整个算法由以下几个部分组成。
OpenCV源代码Nonfree包含了SURF的源代码，并添加了GPU实现：
1.使用Hessian矩阵构建，进行特征댠提取
```cpp
void SURF::operator()(InputArray _img, InputArray _mask,
                      CV_OUT vector<KeyPoint>& keypoints,
                      OutputArray _descriptors,
                      bool useProvidedKeypoints) const
{
    Mat img = _img.getMat(), mask = _mask.getMat(), mask1, sum, msum;
    bool doDescriptors = _descriptors.needed();
    CV_Assert(!img.empty() && img.depth() == CV_8U);
    if( img.channels() > 1 )
        cvtColor(img, img, COLOR_BGR2GRAY);
    CV_Assert(mask.empty() || (mask.type() == CV_8U && mask.size() == img.size()));
    CV_Assert(hessianThreshold >= 0);
    CV_Assert(nOctaves > 0);
    CV_Assert(nOctaveLayers > 0);
    integral(img, sum, CV_32S);
    // 1. 使用Hessian矩阵寻找候选点集合:
    // Compute keypoints only if we are not asked for evaluating the descriptors are some given locations:
    if( !useProvidedKeypoints )
    {
        if( !mask.empty() )
        {
            cv::min(mask, 1, mask1);
            integral(mask1, msum, CV_32S);
        }
        fastHessianDetector( sum, msum, keypoints, nOctaves, nOctaveLayers, (float)hessianThreshold );
    }
    int i, j, N = (int)keypoints.size();
    if( N > 0 )
    {
        Mat descriptors;
        bool _1d = false;
        int dcols = extended ? 128 : 64;
        size_t dsize = dcols*sizeof(float);
        if( doDescriptors )
        {
            _1d = _descriptors.kind() == _InputArray::STD_VECTOR && _descriptors.type() == CV_32F;
            if( _1d )
            {
                _descriptors.create(N*dcols, 1, CV_32F);
                descriptors = _descriptors.getMat().reshape(1, N);
            }
            else
            {
                _descriptors.create(N, dcols, CV_32F);
                descriptors = _descriptors.getMat();
            }
        }
        //2.使用SURFInvoker计算特征描述子
        // we call SURFInvoker in any case, even if we do not need descriptors,
        // since it computes orientation of each feature.
        parallel_for_(Range(0, N), SURFInvoker(img, sum, keypoints, descriptors, extended, upright) );
        // remove keypoints that were marked for deletion
        for( i = j = 0; i < N; i++ )
        {
            if( keypoints[i].size > 0 )
            {
                if( i > j )
                {
                    keypoints[j] = keypoints[i];
                    if( doDescriptors )
                        memcpy( descriptors.ptr(j), descriptors.ptr(i), dsize);
                }
                j++;
            }
        }
        if( N > j )
        {
            N = j;
            keypoints.resize(N);
            if( doDescriptors )
            {
                Mat d = descriptors.rowRange(0, N);
                if( _1d )
                    d = d.reshape(1, N*dcols);
                d.copyTo(_descriptors);
            }
        }
    }
}
```
2.寻找多尺度空间的描述子
在进行高斯模糊时，SIFT的高斯模板大小是始终不变的，只是在不同的octave之间改变图片的大小。而在SURF中，图片的大小是一直不变的，不同的octave层得到的待检测图片是改变高斯模糊尺寸大小得到的，当然了，同一个octave中个的图片用到的高斯模板尺度也不同。算法允许尺度空间多层图像同时被处理，不需对图像进行二次抽样，从而提高算法性能
       SURF算法使**原始图像保持不变而只改变滤波器大小**。SURF采用这种方法节省了降采样过程，其处理速度自然也就提上去了？
Invoke的代码：
```cpp
// Multi-threaded search of the scale-space pyramid for keypoints
// 2.多线程寻找多尺度的特征点描述子
struct SURFFindInvoker : ParallelLoopBody
{
    SURFFindInvoker( const Mat& _sum, const Mat& _mask_sum,
                     const vector<Mat>& _dets, const vector<Mat>& _traces,
                     const vector<int>& _sizes, const vector<int>& _sampleSteps,
                     const vector<int>& _middleIndices, vector<KeyPoint>& _keypoints,
                     int _nOctaveLayers, float _hessianThreshold )
    {
        sum = &_sum;
        mask_sum = &_mask_sum;
        dets = &_dets;
        traces = &_traces;
        sizes = &_sizes;
        sampleSteps = &_sampleSteps;
        middleIndices = &_middleIndices;
        keypoints = &_keypoints;
        nOctaveLayers = _nOctaveLayers;
        hessianThreshold = _hessianThreshold;
    }
    static void findMaximaInLayer( const Mat& sum, const Mat& mask_sum,
                   const vector<Mat>& dets, const vector<Mat>& traces,
                   const vector<int>& sizes, vector<KeyPoint>& keypoints,
                   int octave, int layer, float hessianThreshold, int sampleStep );
    void operator()(const Range& range) const
    {
        for( int i=range.start; i<range.end; i++ )
        {
            int layer = (*middleIndices)[i];
            int octave = i / nOctaveLayers;
            findMaximaInLayer( *sum, *mask_sum, *dets, *traces, *sizes,
                               *keypoints, octave, layer, hessianThreshold,
                               (*sampleSteps)[layer] );
        }
    }
    const Mat *sum;
    const Mat *mask_sum;
    const vector<Mat>* dets;
    const vector<Mat>* traces;
    const vector<int>* sizes;
    const vector<int>* sampleSteps;
    const vector<int>* middleIndices;
    vector<KeyPoint>* keypoints;
    int nOctaveLayers;
    float hessianThreshold;
    static Mutex findMaximaInLayer_m;
};
```
3.计算特征描述子：
      采用3维线性插值法得到亚像素级的特征点，同时也去掉那些值小于一定阈值的点，增加极值使检测到的特征点数量减少，最终只有几个特征最强点会被检测出来。
4. 选取特征点主方向确定
      为了保证旋转不变性，在SURF中，不统计其梯度直方图，而是统计特征点领域内的Harr小波特征。即以特征点为中心，计算半径为6s(S为特征点所在的尺度值)的邻域内，统计60度扇形内所有点在x(水平)和y(垂直)方向的Haar小波响应总和(Haar小波边长取4s)，并给这些响应值赋高斯权重系数，使得靠近特征点的响应贡献大，而远离特征点的响应贡献小，然后60度范围内的响应相加以形成新的矢量，遍历整个圆形区域，选择最长矢量的方向为该特征点的主方向。这样，通过特征点逐个进行计算，得到每一个特征点的主方向
5. 构造SURF特征点描述算子
      在SURF中，也是在特征点周围取一个正方形框，框的边长为20s(s是所检测到该特征点所在的尺度)。该框带方向，方向当然就是第4步检测出来的主方向了。然后把该框分为16个子区域，每个子区域统计25个像素的水平方向和垂直方向的haar小波特征，这里的x(水平)和y(垂直)方向都是相对主方向而言的。该haar小波特征为x(水平)方向值之和，水平方向绝对值之和，垂直方向之和，垂直方向绝对值之和。
```cpp
/*
 * Find the maxima in the determinant of the Hessian in a layer of the
 * scale-space pyramid
 */
void SURFFindInvoker::findMaximaInLayer( const Mat& sum, const Mat& mask_sum,
                   const vector<Mat>& dets, const vector<Mat>& traces,
                   const vector<int>& sizes, vector<KeyPoint>& keypoints,
                   int octave, int layer, float hessianThreshold, int sampleStep )
{
    // Wavelet Data
    const int NM=1;
    const int dm[NM][5] = { {0, 0, 9, 9, 1} };
    SurfHF Dm;
    int size = sizes[layer];
    // The integral image 'sum' is one pixel bigger than the source image
    int layer_rows = (sum.rows-1)/sampleStep;
    int layer_cols = (sum.cols-1)/sampleStep;
    // Ignore pixels without a 3x3x3 neighbourhood in the layer above
    int margin = (sizes[layer+1]/2)/sampleStep+1;
    if( !mask_sum.empty() )
       resizeHaarPattern( dm, &Dm, NM, 9, size, mask_sum.cols );
    int step = (int)(dets[layer].step/dets[layer].elemSize());
    for( int i = margin; i < layer_rows - margin; i++ )
    {
        const float* det_ptr = dets[layer].ptr<float>(i);
        const float* trace_ptr = traces[layer].ptr<float>(i);
        for( int j = margin; j < layer_cols-margin; j++ )
        {
            float val0 = det_ptr[j];
            if( val0 > hessianThreshold )
            {
                /* Coordinates for the start of the wavelet in the sum image. There
                   is some integer division involved, so don't try to simplify this
                   (cancel out sampleStep) without checking the result is the same */
                int sum_i = sampleStep*(i-(size/2)/sampleStep);
                int sum_j = sampleStep*(j-(size/2)/sampleStep);
                /* The 3x3x3 neighbouring samples around the maxima.
                   The maxima is included at N9[1][4] */
                const float *det1 = &dets[layer-1].at<float>(i, j);
                const float *det2 = &dets[layer].at<float>(i, j);
                const float *det3 = &dets[layer+1].at<float>(i, j);
                float N9[3][9] = { { det1[-step-1], det1[-step], det1[-step+1],
                                     det1[-1]  , det1[0] , det1[1],
                                     det1[step-1] , det1[step] , det1[step+1]  },
                                   { det2[-step-1], det2[-step], det2[-step+1],
                                     det2[-1]  , det2[0] , det2[1],
                                     det2[step-1] , det2[step] , det2[step+1]  },
                                   { det3[-step-1], det3[-step], det3[-step+1],
                                     det3[-1]  , det3[0] , det3[1],
                                     det3[step-1] , det3[step] , det3[step+1]  } };
                /* Check the mask - why not just check the mask at the center of the wavelet? */
                if( !mask_sum.empty() )
                {
                    const int* mask_ptr = &mask_sum.at<int>(sum_i, sum_j);
                    float mval = calcHaarPattern( mask_ptr, &Dm, 1 );
                    if( mval < 0.5 )
                        continue;
                }
                        //使用非极大值抑制
                /* Non-maxima suppression. val0 is at N9[1][4]*/
                if( val0 > N9[0][0] && val0 > N9[0][1] && val0 > N9[0][2] &&
                    val0 > N9[0][3] && val0 > N9[0][4] && val0 > N9[0][5] &&
                    val0 > N9[0][6] && val0 > N9[0][7] && val0 > N9[0][8] &&
                    val0 > N9[1][0] && val0 > N9[1][1] && val0 > N9[1][2] &&
                    val0 > N9[1][3]                    && val0 > N9[1][5] &&
                    val0 > N9[1][6] && val0 > N9[1][7] && val0 > N9[1][8] &&
                    val0 > N9[2][0] && val0 > N9[2][1] && val0 > N9[2][2] &&
                    val0 > N9[2][3] && val0 > N9[2][4] && val0 > N9[2][5] &&
                    val0 > N9[2][6] && val0 > N9[2][7] && val0 > N9[2][8] )
                {
                    /* Calculate the wavelet center coordinates for the maxima */
                    float center_i = sum_i + (size-1)*0.5f;
                    float center_j = sum_j + (size-1)*0.5f;
                    KeyPoint kpt( center_j, center_i, (float)sizes[layer],
                                  -1, val0, octave, CV_SIGN(trace_ptr[j]) );
                             //对最显著点位置进行3*3插值
                    /* Interpolate maxima location within the 3x3x3 neighbourhood  */
                    int ds = size - sizes[layer-1];
                    int interp_ok = interpolateKeypoint( N9, sampleStep, sampleStep, ds, kpt );
                    /* Sometimes the interpolation step gives a negative size etc. */
                    if( interp_ok  )
                    {
                        /*printf( "KeyPoint %f %f %d\n", point.pt.x, point.pt.y, point.size );*/
                        cv::AutoLock lock(findMaximaInLayer_m);
                        keypoints.push_back(kpt);
                    }
                }
            }
        }
    }
}
```
                        //使用非极大值抑制
                /* Non-maxima suppression. val0 is at N9[1][4]*/
                if( val0 > N9[0][0] && val0 > N9[0][1] && val0 > N9[0][2] &&
                    val0 > N9[0][3] && val0 > N9[0][4] && val0 > N9[0][5] &&
                    val0 > N9[0][6] && val0 > N9[0][7] && val0 > N9[0][8] &&
                    val0 > N9[1][0] && val0 > N9[1][1] && val0 > N9[1][2] &&
                    val0 > N9[1][3]                    && val0 > N9[1][5] &&
                    val0 > N9[1][6] && val0 > N9[1][7] && val0 > N9[1][8] &&
                    val0 > N9[2][0] && val0 > N9[2][1] && val0 > N9[2][2] &&
                    val0 > N9[2][3] && val0 > N9[2][4] && val0 > N9[2][5] &&
                    val0 > N9[2][6] && val0 > N9[2][7] && val0 > N9[2][8] )
                {
                    /* Calculate the wavelet center coordinates for the maxima */
                    float center_i = sum_i + (size-1)*0.5f;
                    float center_j = sum_j + (size-1)*0.5f;
                    KeyPoint kpt( center_j, center_i, (float)sizes[layer],
                                  -1, val0, octave, CV_SIGN(trace_ptr[j]) );
                             //对最显著点位置进行3*3插值
                    /* Interpolate maxima location within the 3x3x3 neighbourhood  */
                    int ds = size - sizes[layer-1];
                    int interp_ok = interpolateKeypoint( N9, sampleStep, sampleStep, ds, kpt );
                    /* Sometimes the interpolation step gives a negative size etc. */
                    if( interp_ok  )
                    {
                        /*printf( "KeyPoint %f %f %d\n", point.pt.x, point.pt.y, point.size );*/
                        cv::AutoLock lock(findMaximaInLayer_m);
                        keypoints.push_back(kpt);
                    }
                }
            }
        }
    }
}
### 改进之处的可靠性？
**1. 为什么选用高斯金字塔来作特征提取？**
       首先，采用DOG的金字塔原因是：因为它接近LOG，而LOG的极值点提供了最稳定的特征，而且DOG方便计算(只要做减法).而LOG的极值点提供的特征最稳定。其次，我们从直观理解：特征明显的点经过不同尺度的高斯滤波器进行滤波后，差别较大，所以用到的是DOG。但是直观上怎么理解呢. 如果相邻Octave的sigma不是两倍关系还好理解：如果两幅图像只是缩放的关系，那么假设第一个Octave找到了小一倍图像的极值点，那么大一倍图像的极值点会在下一个Octave找到相似的。但是现在，如果把大一倍图像进行一次下采样(这样和小的图像就完全一样了)，进行Gauss滤波时，两个图像滤波系数(sigma)是不一样的，不就找不到一样的极值点了么.
**2.Hessian矩阵为什么能用来筛选极值点？**
       SIFT先利用非极大抑制，再用到Hessian矩阵进行滤除。SURF先用Hessian矩阵，再进行非极大抑制。SURF的顺序可以加快筛选速度么(Hessian矩阵滤除的点更多?)至于SURF先用Hessian矩阵，再进行非极大抑制的原因，是不管先极大值抑制还是判断Hessian矩阵的行列式，金字塔上的点的行列式都是要计算出来的。先判断是否大于0只要进行1次判断，而判断是否是极大值点或者极小值点要与周围26个点比较，只比较1次肯定快。而在SIFT中，构建的高斯金字塔只有一座(不想SURF是有3座)，要进行非极大抑制可以直接用金字塔的结果进行比较。而如果计算Hessian矩阵的行列式，还要再计算Dxx、Dxy、Dyy。因此先进行非极大抑制。这两个步骤的先后与SIFT/SURF的实际计算情况有关的，都是当前算法下的最佳顺序，而不是说哪种先计算一定更好。
**3、为什么采用梯度特征作为局部不变特征？**
       这与人的视觉神经相关。采用梯度作为描述子的原因是，人的视觉皮层上的神经元对特定方向和空间频率的梯度相应很敏感，经过SIFT作者的一些实验验证，用梯度的方法进行匹配效果很好。
**4、为什么可以采用某些特征点的局部不变特征进行整幅图像的匹配？**
       从直观的人类视觉印象来看，人类视觉对物体的描述也是局部化的，基于局部不变特征的图像识别方法十分接近于人类视觉机理，通过局部化的特征组合，形成对目标物体的整体印象，这就为局部不变特征提取方法提供了生物学上的解释，因此局部不变特征也得到了广泛应用。图像中的每个局部区域的重要性和影响范围并非同等重要，即特征不是同等显著的，其主要理论来源是Marr的计算机视觉理论和Treisman的特征整合理论，一般也称为“原子论”。该理论认为视觉的过程开始于对物体的特征性质和简单组成部分的分析，是从局部性质到大范围性质。SIFT/SURF都是对特征点的局部区域的描述，这些特征点应该是影响重要的点，对这些点的分析更加重要。所以在局部不变特征的提取和描述时也遵循与人眼视觉注意选择原理相类似的机制，所以SIFT/SURF用于匹配有效果。
**5. 为什么Hessian矩阵可以用来判断极大值/极小值?**
       在x0点上，hessian矩阵是**正定的**，且各分量的一阶偏导数为0，则x0为极小值点。在x0点上，hessian矩阵是负定的，且各分量的一阶偏导数为0，则x0为极大值点。
对于某个局部区域，若hessian矩阵是半正定的，则这个区域是凸的（反之依然成立）；若负定，则这个区域是凹的(反之依然成立)。而对于正定和负定来说，Hessian矩阵的行列式总是大于等于0的。反过来就是说：某个点若是极大值/极小值，hessian矩阵的行列式必然要大于等于0，而大于等于0如果是满足的，这个点不一定是极大值/极小值(还要判断一阶导数)。所以后面还要进行极大值抑制.
参考资料：
      1.SURF原始论文：Herbert Bay, Andreas Ess, Tinne Tuytelaars, Luc Van Gool, "[SURF: Speeded Up Robust Features](http://www.vision.ee.ethz.ch/~surf/eccv06.pdf)", Computer Vision and Image Understanding (CVIU), Vol. 110, No. 3, pp. 346--359, 2008.
      2.Wiki百科： [Speeded Up Robust Features(SURF)](http://en.wikipedia.org/wiki/SURF) ,Wikipedia, the free encyclopedia .
      3. [The Website of SURF: Speeded Up Robust Features](http://www.vision.ee.ethz.ch/~surf).
