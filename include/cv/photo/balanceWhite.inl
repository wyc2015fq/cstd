
// 白平衡
//在rgb三通道上分别计算直方图，然后将1%的最大值和最小值设置为255和0，其余值映射到(0, 255)区间内，
// 这样使得每个通道的值均匀分布，以实现简单的颜色平衡。实际测试效果，对于某些图像效果还是可以的，尤其是偏色比较厉害的图像。不过该算法实现逻辑比较晦涩。
int balanceWhite(int h, int w, const uchar* src, int srcstep, uchar* dst, int dststep, int cn,
                 const float inputMin, const float inputMax, const float outputMin, const float outputMax) {
  // 在rgb三通道上分别计算直方图  
  // 将1%的最大值和最小值设置为255和0  
  // 其余值映射到(0, 255), 这样使得每个值通道的值在rgb中分布较均匀, 以实现简单的颜色平衡
  // Simple white balance
  float s1 = 1.0f;// low quantile  
  float s2 = 1.0f;// high quantile  
  int i, j, k, d, total = h*w;
  int depth = 2;// depth of histogram tree  
  int bins = 16;// number of bins at each histogram level
  int nElements = int(pow((float)bins, (float)depth));// number of elements in histogram tree
  int* hist = MALLOC(int, nElements);
  
  for (k = 0; k < cn; ++k) {
    MEMSET(hist, 0, nElements);
    // histogram filling
    for (i = 0; i < h; ++i) {
      const uchar* src1 = src + i*srcstep + k;
      for (j=0; j<w; ++j, src1+=cn) {
        int pos = 0;
        float minValue = inputMin - 0.5f;
        float maxValue = inputMax + 0.5f;  
        float interval = (float)(maxValue - minValue) / bins;  
        
        uchar val = *src1;
        for (d = 0; d < depth; ++d) {  
          int currentBin = (int)((val - minValue + 1e-4f) / interval);  
          ++hist[pos + currentBin];  
          
          pos = (pos + currentBin)*bins;  
          minValue = minValue + currentBin*interval;  
          interval /= bins;  
        }
      }
    }
    
    int p1 = 0, p2 = bins - 1;  
    int n1 = 0, n2 = total;  
    float minValue = inputMin - 0.5f;  
    float maxValue = inputMax + 0.5f;  
    float interval = float(maxValue - minValue) / bins;  
    
    // searching for s1 and s2  
    for (d = 0; d < depth; ++d) {
      while (n1 + hist[p1] < s1 * total / 100.0f)  
      {  
        n1 += hist[p1++];  
        minValue += interval;  
      }  
      p1 *= bins;  
      
      while (n2 - hist[p2] > (100.0f - s2) * total / 100.0f)  
      {  
        n2 -= hist[p2--];  
        maxValue -= interval;  
      }  
      p2 = p2*bins - 1;  
      
      interval /= bins;  
    }  
    
    for (i = 0; i < h; ++i) {
      const uchar* src1 = src + i*srcstep + k;
      uchar* dst1 = dst + i*dststep + k;
      for (j=0; j<w; ++j, src1+=cn, dst1+=cn) {
        float t = *src1;
        t = (outputMax - outputMin) * (t - minValue) / (maxValue - minValue) + outputMin;
        *dst1 = BOUND(t, 0, 255);
      }
    }
  }
  
  return 0;
}

