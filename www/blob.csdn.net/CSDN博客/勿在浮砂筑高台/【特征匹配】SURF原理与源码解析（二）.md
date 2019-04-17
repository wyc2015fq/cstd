# 【特征匹配】SURF原理与源码解析（二） - 勿在浮砂筑高台 - CSDN博客





2015年08月23日 14:56:18[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：4215标签：[surf																[opencv																[特征匹配](https://so.csdn.net/so/search/s.do?q=特征匹配&t=blog)
个人分类：[Feature Matching																[Image Processing](https://blog.csdn.net/luoshixian099/article/category/2734445)](https://blog.csdn.net/luoshixian099/article/category/5732053)

所属专栏：[特征匹配](https://blog.csdn.net/column/details/featuresdetect.html)





相关：[SURF原理与源码解析（一）](http://blog.csdn.net/luoshixian099/article/details/47807103)

[SIFT原理与C源码剖析](http://blog.csdn.net/luoshixian099/article/details/47377611)


上一篇已经详细分析了SURF的原理，本篇分析opencv中的SURF源码。

转载请注明出处：http://blog.csdn.net/luoshixian099/article/details/47905681

使用的源码是opencv2.4.3版本，SURF源码位于.../nonfree/surf.cpp中,SURF类的声明位于features2d.hpp中。

注意：如果调用了比较简单的函数，我会把函数的实现直接贴在下面。

首先看features2d.hpp中SURF类的声明：



```cpp
/*!   
 SURF implementation.
 The class implements SURF algorithm by H. Bay et al.
 */
class CV_EXPORTS_W SURF : public Feature2D
{
public:
    //! the default constructor
    CV_WRAP SURF();                              //默认构造函数
    //! the full constructor taking all the necessary parameters
    explicit CV_WRAP SURF(double hessianThreshold,                     //带参数的构造函数
                  int nOctaves=4, int nOctaveLayers=2,
                  bool extended=true, bool upright=false);

    //! returns the descriptor size in float's (64 or 128)
    CV_WRAP int descriptorSize() const;      // 返回描述子向量大小
    //! returns the descriptor type
    CV_WRAP int descriptorType() const;   // 返回描述子类型
/*
.../nonfree/surf.cpp
int SURF::descriptorSize() const { return extended ? 128 : 64; }
int SURF::descriptorType() const { return CV_32F; }
*/

//两个重载运算符函数，实际都是调用后一个函数
    //! finds the keypoints using fast hessian detector used in SURF
    void operator()(InputArray img, InputArray mask,
                    CV_OUT vector<KeyPoint>& keypoints) const;
    //! finds the keypoints and computes their descriptors. Optionally it can compute descriptors for the user-provided keypoints
    void operator()(InputArray img, InputArray mask,
                    CV_OUT vector<KeyPoint>& keypoints,
                    OutputArray descriptors,
                    bool useProvidedKeypoints=false) const;

    AlgorithmInfo* info() const;

    CV_PROP_RW double hessianThreshold;   //hessian行列式的阈值
    CV_PROP_RW int nOctaves;          //组数
    CV_PROP_RW int nOctaveLayers;     //层数
    CV_PROP_RW bool extended;        //扩展到128维
    CV_PROP_RW bool upright;         //考虑旋转不变

protected:

    void detectImpl( const Mat& image, vector<KeyPoint>& keypoints, const Mat& mask=Mat() ) const;  //重载虚函数，检测特征点
    void computeImpl( const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors ) const;        //检测特征点并计算描述子
};
```

两个构造函数

默认采用128维描述子，并使特征点旋转不变。



```cpp
SURF::SURF()
{
    hessianThreshold = 100;
    extended = true;
    upright = false;
    nOctaves = 4;
    nOctaveLayers = 2;
}

SURF::SURF(double _threshold, int _nOctaves, int _nOctaveLayers, bool _extended, bool _upright)
{
    hessianThreshold = _threshold;
    extended = _extended;
    upright = _upright;
    nOctaves = _nOctaves;
    nOctaveLayers = _nOctaveLayers;
}
```

检测特征点并生成描述子，使用的是第二个重载运算符函数：

下面的函数，根据参数的不同，可以 1.检测特征点 2.检测特征点并形成描述子 3.只做形成描述子使用

_img：输入图像

_mask:指定对某些区域检测，默认是整幅图

keypoints:存放特征点信息

_descriptors:存放描述子，如果为空，则做检测特征点使用

_useProvidedKeypoints:只做形成描述子使用，不检测特征点



```cpp
void SURF::operator()(InputArray _img, InputArray _mask,
                      CV_OUT vector<KeyPoint>& keypoints,
                      OutputArray _descriptors,
                      bool useProvidedKeypoints) const
{
    Mat img = _img.getMat(), mask = _mask.getMat(), mask1, sum, msum;
    bool doDescriptors = _descriptors.needed();

    CV_Assert(!img.empty() && img.depth() == CV_8U);
    if( img.channels() > 1 )                                                //转换成灰度图
        cvtColor(img, img, COLOR_BGR2GRAY);

    CV_Assert(mask.empty() || (mask.type() == CV_8U && mask.size() == img.size()));
    CV_Assert(hessianThreshold >= 0);
    CV_Assert(nOctaves > 0);
    CV_Assert(nOctaveLayers > 0);

    integral(img, sum, CV_32S);   //计算积分图像，积分图像比原图像宽，高都多一个像素

    // Compute keypoints only if we are not asked for evaluating the descriptors are some given locations:
    if( !useProvidedKeypoints )          //是否使用提供的特征点
    {
        if( !mask.empty() )     //指定区域寻找特征点
        {
            cv::min(mask, 1, mask1);
            integral(mask1, msum, CV_32S);
        }
        fastHessianDetector( sum, msum, keypoints, nOctaves, nOctaveLayers, (float)hessianThreshold ); //查找特征点在(1)解析
    }

    int i, j, N = (int)keypoints.size();       
    if( N > 0 )
    {
        Mat descriptors;
        bool _1d = false;
        int dcols = extended ? 128 : 64;
        size_t dsize = dcols*sizeof(float);

        if( doDescriptors )       //形成描述子
        {
            _1d = _descriptors.kind() == _InputArray::STD_VECTOR && _descriptors.type() == CV_32F;
            if( _1d )   //矩阵描述子的形式，一行或者一列表示一个特征向量
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

        // we call SURFInvoker in any case, even if we do not need descriptors,
        // since it computes orientation of each feature.
        parallel_for(BlockedRange(0, N), SURFInvoker(img, sum, keypoints, descriptors, extended, upright) );  //计算每个特征点的描述子向量(5)解析

        // remove keypoints that were marked for deletion
        for( i = j = 0; i < N; i++ )//整理特征点与描述子
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



# 1.查找特征点函数

fastHessianDetector，注意计算盒子滤波器模板的方法与原理上采用的不同

sum：积分图

mask_sum:指定区域

nOctaves:组数

nOctaveLayers:每组的层数

hessianTheshold:Hessian行列式的阈值



```cpp
static void fastHessianDetector( const Mat& sum, const Mat& mask_sum, vector<KeyPoint>& keypoints,
                                 int nOctaves, int nOctaveLayers, float hessianThreshold )
{
    /* Sampling step along image x and y axes at first octave. This is doubled
       for each additional octave. WARNING: Increasing this improves speed,
       however keypoint extraction becomes unreliable. */
    const int SAMPLE_STEP0 = 1;                           //模板初始步长

    int nTotalLayers = (nOctaveLayers+2)*nOctaves;// +2为了寻找极值点使用
    int nMiddleLayers = nOctaveLayers*nOctaves;

    vector<Mat> dets(nTotalLayers);   //存储图像行列式的值
    vector<Mat> traces(nTotalLayers);//存储图像行列式的迹
    vector<int> sizes(nTotalLayers);   //存储模板大小信息
    vector<int> sampleSteps(nTotalLayers);//存储步长信息
    vector<int> middleIndices(nMiddleLayers);//存储有效索引每组的0~nOctaveLayers层

    // Allocate space and calculate properties of each layer
    int index = 0, middleIndex = 0, step = SAMPLE_STEP0;

    for( int octave = 0; octave < nOctaves; octave++ )        //为所有图像分配空间并计算对应的属性
    {
        for( int layer = 0; layer < nOctaveLayers+2; layer++ )
        {
            /* The integral image sum is one pixel bigger than the source image*/
            dets[index].create( (sum.rows-1)/step, (sum.cols-1)/step, CV_32F );
            traces[index].create( (sum.rows-1)/step, (sum.cols-1)/step, CV_32F );
			/*
			 模板大小计算方法 (9+6*layer)<<octave；
			 第一组 9 ×9,15×15,21×21
			 第二组 18×18,30×30,42×42
			*/
            sizes[index] = (SURF_HAAR_SIZE0 + SURF_HAAR_SIZE_INC*layer) << octave;//模板大小
            sampleSteps[index] = step;

            if( 0 < layer && layer <= nOctaveLayers )
                middleIndices[middleIndex++] = index;   //有效层的索引
            index++;
        }
        step *= 2;  //每组的模板翻一倍，为了加速
    }

    // Calculate hessian determinant and trace samples in each layer
    parallel_for( BlockedRange(0, nTotalLayers), //在（2）解析并行计算所有层的黑森矩阵的行列式值和迹（调用calcLayerDetAndTrace函数）
                      SURFBuildInvoker(sum, sizes, sampleSteps, dets, traces) );

    // Find maxima in the determinant of the hessian
    parallel_for( BlockedRange(0, nMiddleLayers),                  //在（3）解析，并行计算所有层的极值点
                      SURFFindInvoker(sum, mask_sum, dets, traces, sizes,
                                      sampleSteps, middleIndices, keypoints,
                                      nOctaveLayers, hessianThreshold) );

    std::sort(keypoints.begin(), keypoints.end(), KeypointGreater());  //特征点按大小排序(模板尺寸,组序号,位置)
}
```

# 2.计算每个元素经过盒子滤波器后的值

calcLayerDetAndTrace 



```cpp
static void calcLayerDetAndTrace( const Mat& sum, int size, int sampleStep,
                                  Mat& det, Mat& trace )
{
	/*下面是初始盒子模板9×9的信息，dx,dy 3个子区域，dxy , 4个子区域*/
    const int NX=3, NY=3, NXY=4;
    const int dx_s[NX][5] = { {0, 2, 3, 7, 1}, {3, 2, 6, 7, -2}, {6, 2, 9, 7, 1} };//dxx 左上角坐标，右下角坐标，权重
    const int dy_s[NY][5] = { {2, 0, 7, 3, 1}, {2, 3, 7, 6, -2}, {2, 6, 7, 9, 1} };//dyy
    const int dxy_s[NXY][5] = { {1, 1, 4, 4, 1}, {5, 1, 8, 4, -1}, {1, 5, 4, 8, -1}, {5, 5, 8, 8, 1} };//dxy

    SurfHF Dx[NX], Dy[NY], Dxy[NXY];
/*
	struct SurfHF
	{
		int p0, p1, p2, p3;
		float w;
		SurfHF(): p0(0), p1(0), p2(0), p3(0), w(0) {}
	}; 
   
*/
    if( size > sum.rows-1 || size > sum.cols-1 )
       return;

    resizeHaarPattern( dx_s , Dx , NX , 9, size, sum.cols );  //根据size与9的比例,扩展模板大小,并计算位置坐标
    resizeHaarPattern( dy_s , Dy , NY , 9, size, sum.cols );
    resizeHaarPattern( dxy_s, Dxy, NXY, 9, size, sum.cols );
/*
	resizeHaarPattern( const int src[][5], SurfHF* dst, int n, int oldSize, int newSize, int widthStep )
	{
		float ratio = (float)newSize/oldSize;   
		for( int k = 0; k < n; k++ )
		{
			int dx1 = cvRound( ratio*src[k][0] );
			int dy1 = cvRound( ratio*src[k][1] );
			int dx2 = cvRound( ratio*src[k][2] );
			int dy2 = cvRound( ratio*src[k][3] );
			dst[k].p0 = dy1*widthStep + dx1;
			dst[k].p1 = dy2*widthStep + dx1;
			dst[k].p2 = dy1*widthStep + dx2;
			dst[k].p3 = dy2*widthStep + dx2;
			dst[k].w = src[k][4]/((float)(dx2-dx1)*(dy2-dy1));
		}
	}
*/

    /* The integral image 'sum' is one pixel bigger than the source image */
    int samples_i = 1+(sum.rows-1-size)/sampleStep;      //模板外的元素不作计算
    int samples_j = 1+(sum.cols-1-size)/sampleStep;

    /* Ignore pixels where some of the kernel is outside the image */
    int margin = (size/2)/sampleStep;  //排除模板外的元素

    for( int i = 0; i < samples_i; i++ )
    {
        const int* sum_ptr = sum.ptr<int>(i*sampleStep);
        float* det_ptr = &det.at<float>(i+margin, margin);
        float* trace_ptr = &trace.at<float>(i+margin, margin);
        for( int j = 0; j < samples_j; j++ )
        {
            float dx  = calcHaarPattern( sum_ptr, Dx , 3 );//dxx
            float dy  = calcHaarPattern( sum_ptr, Dy , 3 );//dyy
            float dxy = calcHaarPattern( sum_ptr, Dxy, 4 );//dxy
	/*
			 inline float calcHaarPattern( const int* origin, const SurfHF* f, int n )
			 {
				 double d = 0;
				 for( int k = 0; k < n; k++ )
				 d += (origin[f[k].p0] + origin[f[k].p3] - origin[f[k].p1] - origin[f[k].p2])*f[k].w;
				 return (float)d;
			 }
   */
            sum_ptr += sampleStep;
            det_ptr[j] = dx*dy - 0.81f*dxy*dxy;    //每个元素对应的黑森矩阵行列式的值
            trace_ptr[j] = dx + dy;                         //每个元素对应黑森矩阵行列式的迹
        }
    }
}
```

# 3. 并行计算所有层的极值点

调用findMaximaInLayer()函数寻找极大值点

```cpp
/*
并行计算所有层的极值点
struct SURFFindInvoker
{
...
}
调用findMaximaInLayer()函数
*/
    void operator()(const BlockedRange& range) const
    {
        for( int i=range.begin(); i<range.end(); i++ )
        {
            int layer = (*middleIndices)[i];   //有效层所在的序号
            int octave = i / nOctaveLayers;	//图像所在的组号
            findMaximaInLayer( *sum, *mask_sum, *dets, *traces, *sizes,
                               *keypoints, octave, layer, hessianThreshold,
                               (*sampleSteps)[layer] );
        }
    }
```

```cpp
<pre name="code" class="cpp">
```

```cpp
void SURFFindInvoker::findMaximaInLayer( const Mat& sum, const Mat& mask_sum,
                   const vector<Mat>& dets, const vector<Mat>& traces,
                   const vector<int>& sizes, vector<KeyPoint>& keypoints,
                   int octave, int layer, float hessianThreshold, int sampleStep )


{
    // Wavelet Data
    const int NM=1;
    const int dm[NM][5] = { {0, 0, 9, 9, 1} };  
    SurfHF Dm;          //指定了区域，使用的滤波模板

    int size = sizes[layer];

    // The integral image 'sum' is one pixel bigger than the source image
    int layer_rows = (sum.rows-1)/sampleStep;             //计算对应层的宽和高
    int layer_cols = (sum.cols-1)/sampleStep;

    // Ignore pixels without a 3x3x3 neighbourhood in the layer above
    int margin = (sizes[layer+1]/2)/sampleStep+1;       //因为与相邻层比较数据,使用最大模板的数据计算边界
                                                                               
    if( !mask_sum.empty() )
       resizeHaarPattern( dm, &Dm, NM, 9, size, mask_sum.cols );   

    int step = (int)(dets[layer].step/dets[layer].elemSize());//图像宽

    for( int i = margin; i < layer_rows - margin; i++ )
    {
        const float* det_ptr = dets[layer].ptr<float>(i);
        const float* trace_ptr = traces[layer].ptr<float>(i);
        for( int j = margin; j < layer_cols-margin; j++ )
        {
            float val0 = det_ptr[j];
            if( val0 > hessianThreshold )   //大于指定的阈值
            {
                /* Coordinates for the start of the wavelet in the sum image. There
                   is some integer division involved, so don't try to simplify this
                   (cancel out sampleStep) without checking the result is the same */
                int sum_i = sampleStep*(i-(size/2)/sampleStep);  //计算位置i,j在原积分图上的起点位置
                int sum_j = sampleStep*(j-(size/2)/sampleStep);

                /* The 3x3x3 neighbouring samples around the maxima.
                   The maxima is included at N9[1][4] */

                const float *det1 = &dets[layer-1].at<float>(i, j);
                const float *det2 = &dets[layer].at<float>(i, j);
                const float *det3 = &dets[layer+1].at<float>(i, j);
                float N9[3][9] = { { det1[-step-1], det1[-step], det1[-step+1],    // 26 个位置坐标
                                     det1[-1]  , det1[0] , det1[1],
                                     det1[step-1] , det1[step] , det1[step+1]  },
                                   { det2[-step-1], det2[-step], det2[-step+1],
                                     det2[-1]  , det2[0] , det2[1],
                                     det2[step-1] , det2[step] , det2[step+1]  },
                                   { det3[-step-1], det3[-step], det3[-step+1],
                                     det3[-1]  , det3[0] , det3[1],
                                     det3[step-1] , det3[step] , det3[step+1]  } };

                /* Check the mask - why not just check the mask at the center of the wavelet? */
                if( !mask_sum.empty() )//
                {
                    const int* mask_ptr = &mask_sum.at<int>(sum_i, sum_j);
                    float mval = calcHaarPattern( mask_ptr, &Dm, 1 );
                    if( mval < 0.5 )    //如果指定了区域且模板平均值<0.5,排除该点
                        continue;
                }

                /* Non-maxima suppression. val0 is at N9[1][4]*/
                if( val0 > N9[0][0] && val0 > N9[0][1] && val0 > N9[0][2] &&     //如果是极大值,保留
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
                    float center_i = sum_i + (size-1)*0.5f;         //原积分图模板中心位置
                    float center_j = sum_j + (size-1)*0.5f;         //原积分图模板中心位置

                    KeyPoint kpt( center_j, center_i, (float)sizes[layer],
                                  -1, val0, octave, CV_SIGN(trace_ptr[j]) );// CV_SIGN(trace_ptr[j]) 分成三类: -1, 0, 1

                    /* Interpolate maxima location within the 3x3x3 neighbourhood  */
                    int ds = size - sizes[layer-1];
                    int interp_ok = interpolateKeypoint( N9, sampleStep, sampleStep, ds, kpt );//（4）拟合插值处理,得到精确的位置

                    /* Sometimes the interpolation step gives a negative size etc. */
                    if( interp_ok  )
                    {
                        /*printf( "KeyPoint %f %f %d\n", point.pt.x, point.pt.y, point.size );*/
#ifdef HAVE_TBB
                        tbb::mutex::scoped_lock lock(findMaximaInLayer_m);
#endif
                        keypoints.push_back(kpt);
                    }
                }
            }
        }
    }
}
```

# 4.拟合插值得到亚像素点坐标

```cpp
interpolateKeypoint( float N9[3][9], int dx, int dy, int ds, KeyPoint& kpt )
{
	//三个方向的梯度值取反，-b
    Vec3f b(-(N9[1][5]-N9[1][3])/2,  // Negative 1st deriv with respect to x
            -(N9[1][7]-N9[1][1])/2,  // Negative 1st deriv with respect to y
            -(N9[2][4]-N9[0][4])/2); // Negative 1st deriv with respect to s
//三维hessian矩阵的值，A
    Matx33f A(
        N9[1][3]-2*N9[1][4]+N9[1][5],            // 2nd deriv x, x
        (N9[1][8]-N9[1][6]-N9[1][2]+N9[1][0])/4, // 2nd deriv x, y
        (N9[2][5]-N9[2][3]-N9[0][5]+N9[0][3])/4, // 2nd deriv x, s
        (N9[1][8]-N9[1][6]-N9[1][2]+N9[1][0])/4, // 2nd deriv x, y
        N9[1][1]-2*N9[1][4]+N9[1][7],            // 2nd deriv y, y
        (N9[2][7]-N9[2][1]-N9[0][7]+N9[0][1])/4, // 2nd deriv y, s
        (N9[2][5]-N9[2][3]-N9[0][5]+N9[0][3])/4, // 2nd deriv x, s
        (N9[2][7]-N9[2][1]-N9[0][7]+N9[0][1])/4, // 2nd deriv y, s
        N9[0][4]-2*N9[1][4]+N9[2][4]);           // 2nd deriv s, s

    Vec3f x = A.solve(b, DECOMP_LU);    //     Ax=b

    bool ok = (x[0] != 0 || x[1] != 0 || x[2] != 0) &&
        std::abs(x[0]) <= 1 && std::abs(x[1]) <= 1 && std::abs(x[2]) <= 1;

    if( ok )  //更新位置
    {
        kpt.pt.x += x[0]*dx;
        kpt.pt.y += x[1]*dy;
        kpt.size = (float)cvRound( kpt.size + x[2]*ds );
    }
    return ok;
}
```

# 5.形成描述子

SURFInvoker



```cpp
struct SURFInvoker
{
    enum { ORI_RADIUS = 6, ORI_WIN = 60, PATCH_SZ = 20 };

    SURFInvoker( const Mat& _img, const Mat& _sum,
                 vector<KeyPoint>& _keypoints, Mat& _descriptors,
                 bool _extended, bool _upright )
    {
        keypoints = &_keypoints;
        descriptors = &_descriptors;
        img = &_img;
        sum = &_sum;
        extended = _extended;
        upright = _upright;

        // Simple bound for number of grid points in circle of radius ORI_RADIUS
        const int nOriSampleBound = (2*ORI_RADIUS+1)*(2*ORI_RADIUS+1);

        // Allocate arrays
        apt.resize(nOriSampleBound);  //存储圆内坐标位置(6s为半径的圆)
        aptw.resize(nOriSampleBound);//圆内位置对应的权重（σ=2.5s）
        DW.resize(PATCH_SZ*PATCH_SZ);//20×20对应位置权重

        /* Coordinates and weights of samples used to calculate orientation ，SURF_ORI_SIGMA = 2.5 */
        Mat G_ori = getGaussianKernel( 2*ORI_RADIUS+1, SURF_ORI_SIGMA, CV_32F );//计算高斯核n×1，已归一化
        nOriSamples = 0;
        for( int i = -ORI_RADIUS; i <= ORI_RADIUS; i++ )    //这是半径为6的情况,下面使用时乘以尺度s
        {
            for( int j = -ORI_RADIUS; j <= ORI_RADIUS; j++ )
            {
                if( i*i + j*j <= ORI_RADIUS*ORI_RADIUS )
                {
                    apt[nOriSamples] = cvPoint(i,j);              //在圆内的点坐标
                    aptw[nOriSamples++] = G_ori.at<float>(i+ORI_RADIUS,0) * G_ori.at<float>(j+ORI_RADIUS,0);//位置对应的高斯权重
                }
            }
        }
        CV_Assert( nOriSamples <= nOriSampleBound );

        /* Gaussian used to weight descriptor samples */
        Mat G_desc = getGaussianKernel( PATCH_SZ, SURF_DESC_SIGMA, CV_32F );  
        for( int i = 0; i < PATCH_SZ; i++ )
        {
            for( int j = 0; j < PATCH_SZ; j++ )
                DW[i*PATCH_SZ+j] = G_desc.at<float>(i,0) * G_desc.at<float>(j,0); //20×20范围内坐标位置对应的权重
        }
    }

    void operator()(const BlockedRange& range) const
    {
        /* X and Y gradient wavelet data */
        const int NX=2, NY=2;
        const int dx_s[NX][5] = {{0, 0, 2, 4, -1}, {2, 0, 4, 4, 1}};    //harr小波水平方向模板，初始宽度为4
        const int dy_s[NY][5] = {{0, 0, 4, 2, 1}, {0, 2, 4, 4, -1}};    //harr小波垂直方向模板

        // Optimisation is better using nOriSampleBound than nOriSamples for
        // array lengths.  Maybe because it is a constant known at compile time
        const int nOriSampleBound =(2*ORI_RADIUS+1)*(2*ORI_RADIUS+1);

        float X[nOriSampleBound], Y[nOriSampleBound], angle[nOriSampleBound];
        uchar PATCH[PATCH_SZ+1][PATCH_SZ+1];
        float DX[PATCH_SZ][PATCH_SZ], DY[PATCH_SZ][PATCH_SZ];//存储20×20范围内的harr小波响应值
        CvMat matX = cvMat(1, nOriSampleBound, CV_32F, X);
        CvMat matY = cvMat(1, nOriSampleBound, CV_32F, Y);
        CvMat _angle = cvMat(1, nOriSampleBound, CV_32F, angle);
        Mat _patch(PATCH_SZ+1, PATCH_SZ+1, CV_8U, PATCH);

        int dsize = extended ? 128 : 64;

        int k, k1 = range.begin(), k2 = range.end();
        float maxSize = 0;
        for( k = k1; k < k2; k++ )
        {
            maxSize = std::max(maxSize, (*keypoints)[k].size);               //找出所有特征点中,最大的模板尺寸maxSize
        }
        int imaxSize = std::max(cvCeil((PATCH_SZ+1)*maxSize*1.2f/9.0f), 1);  //计算最大尺寸对应的20s , s =size*(1.2/9)
        Ptr<CvMat> winbuf = cvCreateMat( 1, imaxSize*imaxSize, CV_8U ); //存储20s×20s区域,像素位置对应积分值
        for( k = k1; k < k2; k++ )
        {
            int i, j, kk, nangle;
            float* vec;
            SurfHF dx_t[NX], dy_t[NY];
            KeyPoint& kp = (*keypoints)[k];
            float size = kp.size;
            Point2f center = kp.pt;
            /* The sampling intervals and wavelet sized for selecting an orientation
             and building the keypoint descriptor are defined relative to 's' */
            float s = size*1.2f/9.0f;                   //根据模板大小，计算对应的尺度s
            /* To find the dominant orientation, the gradients in x and y are
             sampled in a circle of radius 6s using wavelets of size 4s.
             We ensure the gradient wavelet size is even to ensure the
             wavelet pattern is balanced and symmetric around its center */
            int grad_wav_size = 2*cvRound( 2*s );    //harr 小波模板宽度为4s
            if( sum->rows < grad_wav_size || sum->cols < grad_wav_size )
            {
                /* when grad_wav_size is too big,
                 * the sampling of gradient will be meaningless
                 * mark keypoint for deletion. */
                kp.size = -1;
                continue;
            }

            float descriptor_dir = 360.f - 90.f;//不考虑旋转，默认的主方向
            if (upright == 0)        //考虑旋转不变性
            {
                resizeHaarPattern( dx_s, dx_t, NX, 4, grad_wav_size, sum->cols );
                resizeHaarPattern( dy_s, dy_t, NY, 4, grad_wav_size, sum->cols );
				/*这里使用6*6的圆域，近似6s*6s的圆域范围，降低了准确度，提高了速度*/
                for( kk = 0, nangle = 0; kk < nOriSamples; kk++ )      //计算半径为6s，圆内的点对应的harr水平方向响应与
                {                                                                                 //与垂直方向响应
                    int x = cvRound( center.x + apt[kk].x*s - (float)(grad_wav_size-1)/2 );
                    int y = cvRound( center.y + apt[kk].y*s - (float)(grad_wav_size-1)/2 );
                    if( y < 0 || y >= sum->rows - grad_wav_size ||
                        x < 0 || x >= sum->cols - grad_wav_size )
                        continue;
                    const int* ptr = &sum->at<int>(y, x);
                    float vx = calcHaarPattern( ptr, dx_t, 2 );
                    float vy = calcHaarPattern( ptr, dy_t, 2 );
                    X[nangle] = vx*aptw[kk];//权重为2.5s
                    Y[nangle] = vy*aptw[kk];
                    nangle++;
                }
                if( nangle == 0 )
                {
                    // No gradient could be sampled because the keypoint is too
                    // near too one or more of the sides of the image. As we
                    // therefore cannot find a dominant direction, we skip this
                    // keypoint and mark it for later deletion from the sequence.
                    kp.size = -1;
                    continue;
                }
                matX.cols = matY.cols = _angle.cols = nangle;
                cvCartToPolar( &matX, &matY, 0, &_angle, 1 );	      //从笛卡尔到极坐标转换，存储在_angle角度数组中

                float bestx = 0, besty = 0, descriptor_mod = 0;
                for( i = 0; i < 360; i += SURF_ORI_SEARCH_INC )//60度的滑动窗口对应的水平和垂直方向的响应值的和，保存在bestx,besty
                {
                    float sumx = 0, sumy = 0, temp_mod;
                    for( j = 0; j < nangle; j++ )
                    {
                        int d = std::abs(cvRound(angle[j]) - i);
                        if( d < ORI_WIN/2 || d > 360-ORI_WIN/2 )
                        {
                            sumx += X[j];
                            sumy += Y[j];
                        }
                    }
                    temp_mod = sumx*sumx + sumy*sumy;
                    if( temp_mod > descriptor_mod )
                    {
                        descriptor_mod = temp_mod;
                        bestx = sumx;
                        besty = sumy;
                    }
                }
                descriptor_dir = fastAtan2( -besty, bestx );// -besty ??
            }
            kp.angle = descriptor_dir;
            if( !descriptors || !descriptors->data )
                continue;

            /* Extract a window of pixels around the keypoint of size 20s */
            int win_size = (int)((PATCH_SZ+1)*s);   //沿着主方向，取20s×20s的正方形窗口
            CV_Assert( winbuf->cols >= win_size*win_size );
            Mat win(win_size, win_size, CV_8U, winbuf->data.ptr);

            if( !upright )  //旋转不变性
            {
                descriptor_dir *= (float)(CV_PI/180);   //换成弧度制
                float sin_dir = -std::sin(descriptor_dir);   
                float cos_dir =  std::cos(descriptor_dir);

                /* Subpixel interpolation version (slower). Subpixel not required since
                the pixels will all get averaged when we scale down to 20 pixels */
                /*
                float w[] = { cos_dir, sin_dir, center.x,
                -sin_dir, cos_dir , center.y };
                CvMat W = cvMat(2, 3, CV_32F, w);
                cvGetQuadrangleSubPix( img, &win, &W );
                */

                // Nearest neighbour version (faster)
                float win_offset = -(float)(win_size-1)/2;
                float start_x = center.x + win_offset*cos_dir + win_offset*sin_dir;  //旋转后的窗口左上角坐标
                float start_y = center.y - win_offset*sin_dir + win_offset*cos_dir;
                uchar* WIN = win.data;
                for( i = 0; i < win_size; i++, start_x += sin_dir, start_y += cos_dir )   //边长为20s的正方形窗口旋转后,得到区域内像素值
                {
                    float pixel_x = start_x;    //
                    float pixel_y = start_y;
                    for( j = 0; j < win_size; j++, pixel_x += cos_dir, pixel_y -= sin_dir )    
                    {
                        int x = std::min(std::max(cvRound(pixel_x), 0), img->cols-1);
                        int y = std::min(std::max(cvRound(pixel_y), 0), img->rows-1);
                        WIN[i*win_size + j] = img->at<uchar>(y, x);//保存在win矩阵里，
                    }
                }
            }
            else   //不考虑旋转
            {
                // extract rect - slightly optimized version of the code above
                // TODO: find faster code, as this is simply an extract rect operation,
                //       e.g. by using cvGetSubRect, problem is the border processing
                // descriptor_dir == 90 grad
                // sin_dir == 1
                // cos_dir == 0

                float win_offset = -(float)(win_size-1)/2;
                int start_x = cvRound(center.x + win_offset);
                int start_y = cvRound(center.y - win_offset);
                uchar* WIN = win.data;
                for( i = 0; i < win_size; i++, start_x++ )
                {
                    int pixel_x = start_x;
                    int pixel_y = start_y;
                    for( j = 0; j < win_size; j++, pixel_y-- )
                    {
                        int x = MAX( pixel_x, 0 );
                        int y = MAX( pixel_y, 0 );
                        x = MIN( x, img->cols-1 );
                        y = MIN( y, img->rows-1 );
                        WIN[i*win_size + j] = img->at<uchar>(y, x);
                    }
                }
            }
            // Scale the window to size PATCH_SZ so each pixel's size is s. This
            // makes calculating the gradients with wavelets of size 2s easy
            resize(win, _patch, _patch.size(), 0, 0, INTER_AREA);   //把20s×20s缩小到20×20,方便计算每个位置的harr小波,降低准确性

            // Calculate gradients in x and y with wavelets of size 2s
            for( i = 0; i < PATCH_SZ; i++ )                                       //采用2s的模板计算水平和垂直方向的harr响应，vx,vy
                for( j = 0; j < PATCH_SZ; j++ )
                {
                    float dw = DW[i*PATCH_SZ + j];
                    float vx = (PATCH[i][j+1] - PATCH[i][j] + PATCH[i+1][j+1] - PATCH[i+1][j])*dw;
                    float vy = (PATCH[i+1][j] - PATCH[i][j] + PATCH[i+1][j+1] - PATCH[i][j+1])*dw;
                    DX[i][j] = vx;
                    DY[i][j] = vy;
                }

            // Construct the descriptor
            vec = descriptors->ptr<float>(k);     //第k个特征点对应的描述子
            for( kk = 0; kk < dsize; kk++ )
                vec[kk] = 0;
            double square_mag = 0;
            if( extended )    //采用128维，
            {
                // 128-bin descriptor
                for( i = 0; i < 4; i++ )
                    for( j = 0; j < 4; j++ )
                    {
                        for(int y = i*5; y < i*5+5; y++ )
                        {
                            for(int x = j*5; x < j*5+5; x++ )
                            {
                                float tx = DX[y][x], ty = DY[y][x];
                                if( ty >= 0 )
                                {
                                    vec[0] += tx;
                                    vec[1] += (float)fabs(tx);
                                } else {
                                    vec[2] += tx;
                                    vec[3] += (float)fabs(tx);
                                }
                                if ( tx >= 0 )
                                {
                                    vec[4] += ty;
                                    vec[5] += (float)fabs(ty);
                                } else {
                                    vec[6] += ty;
                                    vec[7] += (float)fabs(ty);
                                }
                            }
                        }
                        for( kk = 0; kk < 8; kk++ )
                            square_mag += vec[kk]*vec[kk];
                        vec += 8;
                    }
            }
            else  //采用64维
            {
                // 64-bin descriptor
                for( i = 0; i < 4; i++ )
                    for( j = 0; j < 4; j++ )
                    {
                        for(int y = i*5; y < i*5+5; y++ )
                        {
                            for(int x = j*5; x < j*5+5; x++ )
                            {
                                float tx = DX[y][x], ty = DY[y][x];
                                vec[0] += tx; vec[1] += ty;
                                vec[2] += (float)fabs(tx); vec[3] += (float)fabs(ty);
                            }
                        }
                        for( kk = 0; kk < 4; kk++ )
                            square_mag += vec[kk]*vec[kk];
                        vec+=4;
                    }
            }

            // unit vector is essential for contrast invariance
            vec = descriptors->ptr<float>(k);
            float scale = (float)(1./(sqrt(square_mag) + DBL_EPSILON));   //描述子归一化
            for( kk = 0; kk < dsize; kk++ )
                vec[kk] *= scale;
        }
    }

    // Parameters
    const Mat* img;
    const Mat* sum;
    vector<KeyPoint>* keypoints;
    Mat* descriptors;
    bool extended;
    bool upright;

    // Pre-calculated values
    int nOriSamples;
    vector<Point> apt;  //半径为6s的范围坐标点
    vector<float> aptw;//2.5s权重
    vector<float> DW;//20×20对应的权重
};
```](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)](https://so.csdn.net/so/search/s.do?q=surf&t=blog)




