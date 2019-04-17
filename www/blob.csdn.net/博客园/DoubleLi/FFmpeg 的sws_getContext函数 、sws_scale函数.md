# FFmpeg 的sws_getContext函数 、sws_scale函数 - DoubleLi - 博客园






FFmpeg里面的sws_scale库可以在一个函数里面同时实现：1.图像色彩空间转换；2.分辨率缩放；3.前后图像滤波处理。

其核心函数主要有三个：

// 初始化sws_scale
struct SwsContext *sws_getContext(int srcW, int srcH, enum AVPixelFormat srcFormat,
                                  int dstW, int dstH, enum AVPixelFormat dstFormat,
                                  int flags, 
                                  SwsFilter *srcFilter, SwsFilter *dstFilter, const double *param);
参数int srcW, int srcH, enum AVPixelFormat srcFormat定义输入图像信息（寬、高、颜色空间（像素格式））
参数int dstW, int dstH, enum AVPixelFormat dstFormat定义输出图像信息寬、高、颜色空间（像素格式））。
参数int flags选择缩放算法（只有当输入输出图像大小不同时有效）
参数SwsFilter *srcFilter, SwsFilter *dstFilter分别定义输入/输出图像滤波器信息，如果不做前后图像滤波，输入NULL
参数const double *param定义特定缩放算法需要的参数(?)，默认为NULL
函数返回SwsContext结构体，定义了基本变换信息。
如果是对一个序列的所有帧做相同的处理，函数sws_getContext只需要调用一次就可以了。
sws_getContext(w, h, YV12, w, h, NV12, 0, NULL, NULL, NULL);      // YV12->NV12 色彩空间转换
sws_getContext(w, h, YV12, w/2, h/2, YV12, 0, NULL, NULL, NULL);  // YV12图像缩小到原图1/4
sws_getContext(w, h, YV12, 2w, 2h, YN12, 0, NULL, NULL, NULL);    // YV12图像放大到原图4倍，并转换为NV12结构

// 做转换
int sws_scale(struct SwsContext *c, 
              const uint8_t *const srcSlice[], const int srcStride[], 
              int srcSliceY, int srcSliceH,
              uint8_t *const dst[], const int dstStride[]);
参数struct SwsContext *c，为上面sws_getContext函数返回值；
参数const uint8_t *const srcSlice[], const int srcStride[]定义输入图像信息（当前处理区域的每个通道数据指针，每个通道行字节数）
stride定义下一行的起始位置。stride和width不一定相同，这是因为：
1.由于数据帧存储的对齐，有可能会向每行后面增加一些填充字节这样 stride = width + N；
2.packet色彩空间下，每个像素几个通道数据混合在一起，例如RGB24，每个像素3字节连续存放，因此下一行的位置需要跳过3*width字节。
srcSlice和srcStride的维数相同，由srcFormat值来。
csp       维数        宽width      跨度stride      高
YUV420     3        w, w/2, w/2    s, s/2, s/2   h, h/2, h/2
YUYV       1        w, w/2, w/2   2s, 0, 0       h, h, h
NV12       2        w, w/2, w/2    s, s, 0       h, h/2
RGB24      1        w, w,   w     3s, 0, 0       h, 0, 0           
参数int srcSliceY, int srcSliceH,定义在输入图像上处理区域，srcSliceY是起始位置，srcSliceH是处理多少行。如果srcSliceY=0，srcSliceH=height，表示一次性处理完整个图像。
这种设置是为了多线程并行，例如可以创建两个线程，第一个线程处理 [0, h/2-1]行，第二个线程处理 [h/2, h-1]行。并行处理加快速度。
参数uint8_t *const dst[], const int dstStride[]定义输出图像信息（输出的每个通道数据指针，每个通道行字节数）

// 释放sws_scale

void sws_freeContext(struct SwsContext *swsContext);



在网上没有看到有关SwsFilter的讨论，看FFMpeg代码，总结下面的分析结果。


sws_scale前后图像滤波都定义为归一化的2维或者1维图像卷积处理。每个滤波器有四个分量
typedef struct SwsFilter {
    SwsVector *lumH; // 亮度水平处理
    SwsVector *lumV; // 亮度垂直处理
    SwsVector *chrH; // 色度水平处理
    SwsVector *chrV; // 色度垂直处理
} SwsFilter;
一般都是2维水平和垂直按照相同的处理系数来滤波。

每个滤波器定义为：
typedef struct SwsVector {
    double *coeff;              // 滤波器系数
    int length;                 // 滤波器长度
} SwsVector;
一般滤波器具有归一化：length个coeff之和等于1;
             对称性：length一般为奇数，coeff以中心为轴左右对称。


sws_scale库里定义了3种初始滤波器。
1. 高斯模糊 Gaussian Blur
   SwsVector *sws_getGaussianVec(double variance, double quality);
   variance就是σ。quality=3.0。
   const int length = (int)(variance * quality + 0.5) | 1;
   double middle  = (length - 1) * 0.5;
   for (i = 0; i < length; i++) {
       double dist = i - middle;
       vec->coeff[i] = exp(-dist * dist / (2 * variance * variance)) / sqrt(2 * variance * M_PI);
   }  
   如后在归一化vec->coeff[i]。
    // 这个公式和标准高斯公式不一样，标准高斯函数公式如下
       vec->coeff[i] = exp(-dist * dist / (2 * variance * variance)) / (variance*sqrt(2 * M_PI));

下面是一些variance值计算出来的结果。   
variance = 1.0 => length=3                 0.2741  0.4519  0.2741
variance = 1.5 => length=5         0.1201  0.2339  0.2921  0.2339  0.1201 
variance = 2.0 => length=7 0.0702  0.1311  0.1907  0.2161  0.1907  0.1311  0.0702
垂直方向滤波器length过大，不仅计算量增加，数据读取的带宽需求也增大，近似为读取length*frame_size数据。

2.锐化滤波器 Sharpen
    if (lumaSharpen != 0.0) {
        SwsVector *id = sws_getIdentityVec();
        sws_scaleVec(filter->lumH, -lumaSharpen);  // 所有点矢量乘 -lumaSharpen
        sws_addVec(filter->lumH, id);              // 矢量加   
    }
    coeff[i] = i==(length-1)/2 ? 1 - lumaSharpen*coeff[i] : - lumaSharpen*coeff[i];
    中心点设为1-lumaSharpen*coeff[i]，其他点设为 -lumaSharpen*coeff[i].
    一般情况两个矢量相加，以中心点对齐，左右两边分别相加，没有的值补0.
    {a1, a2, a3} + {b1, b2, b3, b4, b5} = {b1, a1+b1, a2+b3, a3+b4, b5}
如已经使用高斯模糊得到滤波器为：
length=5         0.1201  0.2339  0.2921  0.2339  0.1201
设lumaSharpen = 0.7; 结果为
length=5        -0.0841 -0.1637  0.7955 -0.1637 -0.0841

3.色度移动滤波器 ChromaShift
    if (chromaHShift != 0.0)
        sws_shiftVec(filter->chrH, (int)(chromaHShift + 0.5));
    函数sws_getShiftedVec(SwsVector *a, int shift) 左移矢量a;如果shift小于0,右移
    移动后矢量长度为 length = a->length + FFABS(shift) * 2;
    左移就是后面补 length - a->length个0
    右移就是前面补 length - a->length个0
例如chromaHShift = 1.3, shift = (int)(1.3+0.5) = 1; 
移位后结果增加 |1|*2 = 2个; 正数左移 后面补零
        a1, a2, a3, ... aN, 0.0000,  0.0000
例如chromaHShift = -3.1, shift = (int)(-3.1+0.5) = -2;
移位后结果增加 |-2|*2 = 4个; 负数左移 前面补零
   0.0000  0.0000  0.0000  0.0000 a1 a2 a3 ... aN
这个滤波器将色度位置移动，有什么用处？？？

4. 设置初始滤波器的流程
SwsFilter *sws_getDefaultFilter(float lumaGBlur,    float chromaGBlur,
                                float lumaSharpen,  float chromaSharpen,
                                float chromaHShift, float chromaVShift,
                                int verbose);
参数float lumaGBlur, float chromaGBlur分别设置亮度和色度的高斯模糊参数。一般亮度做模糊，色度不做。
参数float lumaSharpen, float chromaSharpen分别设置亮度和色度的锐化参数。做高斯模糊后，物体边缘也变得模糊，为了减少这种影响，调用锐化滤波。如果不做高斯模糊，没必要做锐化滤波。
参数float chromaHShift, float chromaVShift分别设置色度在水平和垂直两方向上的色彩位移，不明白是什么物理意义，还是固定为0.0的好。
参数int verbose是控制打印滤波器参数，设置为0。
在函数里面亮度滤波器的设置流程是：
a. 如果lumaGBlur不为0.0, 设置高斯滤波器；
b. 如果lumaSharpen不为0.0, 在高斯滤波器上叠加锐化滤波；
c. 归一化步骤2的滤波器，作为最终的滤波器参数。

也可以按照需要设置自己的滤波器，但是都是做1维或者2维的卷积操作，所有有些滤波器也设置不出来。
例如线性拉伸处理。 g = k*f+b. f,g分别为原始和处理后像素点值，k,b为标量参数值。

基于计算复杂度的考虑，滤波器放置在图像相对小的那一端，例如sws_scale做缩小处理，那么滤波器在后端；如果做放大处理，滤波器放前端（个人建议）。









