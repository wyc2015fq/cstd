// 基于L0范数平滑的图像漫画特效生成算法
// 图像漫画特效算法，主要思路可以分两部分，第一，简化图像，即去除图像细节，仅保留图像主要信息，第二，生成合适的边缘线条，最后将两者合成即可，合成其实就是将两者简单一乘即可。在简化图像步骤，除了meanshift算法外，大家肯定会想到很多边缘保持的图像平滑算法，比如经典的双边滤波、导向滤波等等。更多内容可以查阅这里，该文作者提供了多种边缘保持平滑算法（主要包括双边滤波，域变换，WLS、导向滤波、L0范数平滑等 ）效果对比以及matlab代码，算法很多，但是本人认为L0范数平滑算法比较适合做漫画特效，平滑的结果图看着更有层次感。关于L0范数平滑算法，大家可以参考原文文献以及这篇文章 ，我这里不在赘述。L0范数平滑算法原理及公式推导比较好理解，不过实现起来比较复杂，需要用到傅里叶变换以及迭代求解方程。
// 图像经过平滑简化后，边缘检测效果也会更好。杂乱无章的零碎边缘线条将大大减少，检测到的图像边缘会更连贯，更集中。有了简化图和边缘图，那么基本就可以生成漫画特效了。传统方法为直接将两者合成，即将简化图像结果与边缘检测结果直接合成。但是，由于检测的边缘往往很生硬，缺乏平滑的笔触效果，这样直接合成的漫画特效看起来不是很理想。本算法的主要创新点就是在提取的边缘基础上，增加了线积分卷积（LIC算法）滤波，经过该算法处理后，提取的边缘将会非常圆润、平滑，视觉上更接近漫画手绘效果。
// 下面贴出算法整体调用逻辑代码，基本可以了解本文提出的漫画特效算法思路。


int ConvertToDouble(const img_t* src, double* rdata, double* gdata, double* bdata) {
  int i, j, k=0;
  for (i=0; i<src->h; ++i) {
    const uchar* src1 = img_row(uchar, src, i);
    for (j=0; j<src->w; ++j, src1 += src->c) {
      bdata[k] = src1[0];
      gdata[k] = src1[1];
      rdata[k] = src1[2];
      ++k;
    }
  }
  return 0;
}

int CartoonStylization(const img_t* src, img_t* dst, int block_count) {
  int width = src->w;
  int height=  src->h;  
  int size = width*height;  
  int mem_size = size * sizeof(double);  
  double *rdata = (double *)malloc(mem_size);  
  double *gdata = (double *)malloc(mem_size);  
  double *bdata = (double *)malloc(mem_size);  
  
  // 数据转换  
  ConvertToDouble(src, rdata, gdata, bdata);  
  
  // 简化细节  
  double *rgb_data[3] = { rdata, gdata, bdata };  
  L0ImageSmoothing(rgb_data, width, height, block_count, 0.01, 2.0);  
  
  // 抽取边缘  
  rdata = rgb_data[0];  
  gdata = rgb_data[1];  
  bdata = rgb_data[2];  
  double *gray_data = (double *)malloc(mem_size);  
  double *hor_edgedata = (double *)malloc(mem_size);  
  double *ver_edgedata = (double *)malloc(mem_size);  
  for (int i = 0; i < size; i++)  
  {  
    gray_data[i] = (rdata[i]*0.299f + gdata[i]*0.587f + bdata[i]*0.114f);  
  }  
  EdgeDetectionFilter(gray_data, width, height, hor_edgedata, ver_edgedata);  
  
  for (int i = 0; i < size; i++)  
  {  
    double edge_val = (fabs(hor_edgedata[i]) + fabs(ver_edgedata[i]))*1.2;  
    edge_val = edge_val < 1.0 ? 1.0 - edge_val : 0.0;  
    gray_data[i] = edge_val;  
  }  
  
  double *edge_data = (double *)malloc(mem_size);  
  ExtractEdge(gray_data, edge_data, width, height, 15.7, 0.017, 75.5f);  
  
  // 计算矢量场  
  double *vec_x = (double *)malloc(mem_size);  
  double *vec_y = (double *)malloc(mem_size);  
  CalcVectorField(gray_data, width, height, vec_x, vec_y, 1.0);  
  
  // 线积分卷积  
  LICFilter(edge_data, gray_data, vec_x, vec_y, width, height);  
  
  free(vec_x);  
  free(vec_y);  
  vec_x = NULL;  
  vec_y = NULL;  
  free(edge_data);  
  edge_data = NULL;  
  
  // 结果合成  
  for (int i = 0; i < size; i++)  
  {  
    rdata[i] *= gray_data[i];  
    gdata[i] *= gray_data[i];  
    bdata[i] *= gray_data[i];  
  }  
  
  // 数据转换  
  ConvertToUchar(rdata, gdata, bdata, src);  
  
  // 效果微调  
  ImageAdjust(src);  
  
  free(gray_data);  
  free(hor_edgedata);  
  free(ver_edgedata);  
  gray_data = NULL;  
  hor_edgedata = NULL;  
  ver_edgedata = NULL;  
  
  free(rdata);  
  free(gdata);  
  free(bdata);  
  rdata = NULL;  
  gdata = NULL;  
  bdata = NULL;  
  
  return NULL;  
}  

当然，本算法也是有局限性的，就像滤镜一样，有的适合人像，有的适合风景。本文算法对于背景与前景简单一些的图效果比较好，如果图像背景或者前景比较杂乱，生成的结果可能不太理想。下面是一些效果图：