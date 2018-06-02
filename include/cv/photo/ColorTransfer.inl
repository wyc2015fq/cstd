
#ifndef _COLORTRANSFER_INL_
#define _COLORTRANSFER_INL_

#include "cstd.h"
#include "img/color.inl"


// Welsh等人在Reinhard等人的彩色图像间色彩迁移算法的基础上，提出了灰度图像彩色化的思想，并提出了相应的算法。该算法主要利用查找匹配像素来实现灰度图像的色彩迁移，因为灰度图像只有亮度信息，所以该算法主要通过像素的亮度值匹配，来实现灰度图像的自动彩色化。
// 
// 具体步骤如下：
// 
// (1)将参考图像和灰度图像分别由RGB空间转换到lαβ色彩空间。
// 
// (2)根据灰度图像的亮度及标准差，对参考图像进行亮度重映射。
// 
// 由于参考图像和灰度图像的直方图取值不一定在同一范围，那么如果将一幅亮度值很低的参考图像与一幅亮度值很高的目标图像的直方图进行对比将会出现较大的误差。因此，需要对参考图像进行亮度重映射：
// 
// L = (nl’ / nl)* (l – ml) + ml’
// 
// 其中，l是源图像l通道的数据，L是变换后得到新的源图像l通道的值，ml和ml’ 分别是源图像和着色图像的l通道的均值，nl和nl’表示它们的l通道标准方差。
// 
// (3)从参考图像中随机选取一批样本点，将像素点的亮度和邻域范围内亮度的标准差的线性组合值作为权值，计算公式如下：
// 
// W = l/2 + σ/2
// 
// 其中，w为权值，l为像素点的亮度值，σ为该像素点周围某个邻域内亮度值的标准差。关于邻域的大小，Welsh指出一般取5X5，对个别图像取更大的邻域效果会更好。
// 
// (4)按行扫描灰度图像，对于每个像素点，由公式计算其权值，并在参考图像的样本点中找到一个最接近的样本点，即最佳匹配点，将匹配点的仅和口通道的值赋给灰度图像的对应像素点，从而实现色彩的传递。
// 
// (5)将参考图像和灰度图像从lαβ空间转换到RGB空间。
// 

#define max_uchar(a, b)    (((a) > (b)) ? (a) : (b))  
#define min_uchar(a, b)    (((a) < (b)) ? (a) : (b))  

// 计算彩色图像均值和标准差  
void CompMeanAndVariance(const img_t* img, double* mean3f, double* variance3f)  
{
  int i, j;
  int row = img->h;
  int col = img->w;
  int total = row * col;  
  double sum[3] = { 0, 0, 0 };
  // 均值
  for (i = 0; i < row; i++) {
    const uchar *pImg = img_row(uchar, img, i);
    for (j = 0; j < col; ++j, pImg+=img->c) {
      sum[0] += pImg[0];  
      sum[1] += pImg[1];  
      sum[2] += pImg[2];
    }
  }  
  
  mean3f[0] = sum[0] / total;  
  mean3f[1] = sum[1] / total;  
  mean3f[2] = sum[2] / total;  
  
  sum[0] = sum[1] = sum[2] = 0; 
  // 标准差
  for (i = 0; i < row; i++) {
    const uchar *pImg = img_row(uchar, img, i);
    for (j = 0; j < col; ++j, pImg+=img->c) {
      sum[0] += (pImg[0] - mean3f[0]) * (pImg[0] - mean3f[0]);  
      sum[1] += (pImg[1] - mean3f[1]) * (pImg[1] - mean3f[1]);  
      sum[2] += (pImg[2] - mean3f[2]) * (pImg[2] - mean3f[2]);  
    }
  }  
  
  variance3f[0] = sqrt(sum[0] / total);
  variance3f[1] = sqrt(sum[1] / total);
  variance3f[2] = sqrt(sum[2] / total);
}  

// 颜色转换  
int ColorTransfer(const img_t* src, const img_t* tar, img_t* dst)
{
  img_t im[10] = {0};
  int i=0, j;
  img_t* srcLab = im+i++;
  img_t* tarLab = im+i++;
  double srcMean3f[4] = {0}, tarMean3f[4] = {0};// 源/目标图像均值  
  double srcVariance3f[4] = {0}, tarVariance3f[4] = {0};// 源/目标图像标准差  
  double ratioVariance3f[4] = {0};// 标准差比例  
  
  // BGR空间转Lab空间  
  imcolorcvt(src, srcLab, T_BGR, T_Lab);
  imcolorcvt(tar, tarLab, T_BGR, T_Lab);
  if(0) {
    img_t* src2 = im+i++;
    img_t* tar2 = im+i++;
    imcolorcvt(srcLab, src2, T_Lab, T_BGR);
    imcolorcvt(tarLab, tar2, T_Lab, T_BGR);
    imshow(srcLab);
    imshow(tarLab);
    imshow(src2);
    imshow(tar2);
    cvWaitKey(-1);
  }
  // 计算当前图像与目标图像均值及标准差
  CompMeanAndVariance(srcLab, srcMean3f, srcVariance3f);  
  CompMeanAndVariance(tarLab, tarMean3f, tarVariance3f);  
  
  // 标准差比  
  ratioVariance3f[0] = tarVariance3f[0] / srcVariance3f[0];  
  ratioVariance3f[1] = tarVariance3f[1] / srcVariance3f[1];  
  ratioVariance3f[2] = tarVariance3f[2] / srcVariance3f[2];  
  
  // 计算颜色转换值  
  int row = srcLab->h;  
  int col = srcLab->w;  
  for (i = 0; i < row; i++) {
    uchar *pImg = img_row(uchar, srcLab, i);
    for (j = 0; j < col; ++j, pImg+=srcLab->c) {
      double t0 = ratioVariance3f[0] * (pImg[0] - srcMean3f[0]) + tarMean3f[0];
      double t1 = ratioVariance3f[1] * (pImg[1] - srcMean3f[1]) + tarMean3f[1];
      double t2 = ratioVariance3f[2] * (pImg[2] - srcMean3f[2]) + tarMean3f[2];

      pImg[0] = (uchar)min_uchar(255, max_uchar(0, t0));
      pImg[1] = (uchar)min_uchar(255, max_uchar(0, t1));
      pImg[2] = (uchar)min_uchar(255, max_uchar(0, t2));
    }
  }  
  
  // Lab空间转BGR空间  
  imcolorcvt(srcLab, dst, T_Lab, T_BGR);
  imfrees(im, 10);
  return 0;
}


#endif // _COLORTRANSFER_INL_
