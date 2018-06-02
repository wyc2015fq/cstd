

int graythresh(const uchar* image, int step, int x0, int y0, int dx, int dy)
{
  
  unsigned char* np;       // 图像指针
  int thresholdValue = 1; // 阈值
  int ihist[ 256 ];             // 图像直方图，256个点
  
  int i, j, k;          // various counters
  int n, n1, n2, gmin, gmax;
  double m1, m2, sum, csum, fmax, sb;
  
  // 对直方图置零...
  memset(ihist, 0, sizeof(ihist));
  
  gmin = 255;
  gmax = 0;
  
  // 生成直方图
  for (i = y0 + 1; i < y0 + dy - 1; i++) {
    np = (uchar*)(image + i * step);
    
    // np = &image[i*cols+x0+1];
    for (j = x0 + 1; j < x0 + dx - 1; j++) {
      ihist[ *np ] ++;
      
      if (*np > gmax) {
        gmax = *np;
      }
      
      if (*np < gmin) {
        gmin = *np;
      }
      
      np++; /* next pixel */
    }
  }
  
  // set up everything
  sum = csum = 0.0;
  n = 0;
  
  for (k = 0; k <= 255; k++) {
    sum += (double) k * (double) ihist[ k ];        /* x*f(x) 质量矩*/
    n += ihist[ k ];                                         /*  f(x)    质量    */
  }
  
  if (!n) {
    // if n has no value, there is problems...
    return -1;
  }
  
  // do the otsu global thresholding method
  fmax = -1.0;
  n1 = 0;
  
  for (k = 0; k < 255; k++) {
    n1 += ihist[ k ];
    
    if (!n1) {
      continue;
    }
    
    n2 = n - n1;
    
    if (n2 == 0) {
      break;
    }
    
    csum += (double) k * ihist[ k ];
    m1 = csum / n1;
    m2 = (sum - csum) / n2;
    sb = (double) n1 * (double) n2 * (m1 - m2) * (m1 - m2);
    
    /* bbg: note: can be optimized. */
    if (sb > fmax) {
      fmax = sb;
      thresholdValue = k;
    }
  }
  
  // at this point we have our thresholding value
  
  // debug code to display thresholding values
  return (thresholdValue);
}


int otsuThreshold(int height, int width, const uchar* frame, int step, int inv)
{
  int dd;
  
  int i, j, pixelSum = width * height, threshold = 0;
  int pixelCount[256] = {0};
  float pixelPro[256] = {0};
  float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
  //uchar* data = (uchar*)frame->tt.data;
  
  for (i = 0; i < 256; i++) {
    pixelCount[i] = 0;
    pixelPro[i] = 0;
  }
  
  dd = 0;
  
  //统计灰度级中每个像素在整幅图像中的个数
  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      //pixelCount[(int)data[i * width + j]]++;
      dd = frame[i*step+j];
      pixelCount[dd]++;
      
      if (dd != 255) {
        int kk = 12;
      }
    }
  }
  
  //计算每个像素在整幅图像中的比例
  for (i = 0; i < 256; i++) {
    pixelPro[i] = (float)pixelCount[i] / pixelSum;
  }
  
  //遍历灰度级[0,255]
  for (i = 0; i < 256; i++) {
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    
    for (j = 0; j < 256; j++) {
      if (j <= i) { //背景部分
        w0 += pixelPro[j];
        u0tmp += j * pixelPro[j];
      }
      else { //前景部分
        w1 += pixelPro[j];
        u1tmp += j * pixelPro[j];
      }
    }
    
    u0 = u0tmp / w0;
    u1 = u1tmp / w1;
    u = u0tmp + u1tmp;
    deltaTmp = w0 * (float)pow((u0 - u), 2) + w1 * (float)pow((u1 - u), 2);
    
    if (deltaTmp > deltaMax) {
      deltaMax = deltaTmp;
      threshold = i;
    }
  }
  
  return threshold;
}


int imgraythresh(const img_t* im) {
  int tt = graythresh(im->tt.data, im->s, 0, 0, im->w, im->h);
  //int tt1 = otsuThreshold(im->h, im->w, im->tt.data, im->s, 0);
  return tt;
}

int bf_imedge(buf_t* bf, const img_t* shuru1, img_t* BW2) {
  double y_mask[]={-1,0,1,-2,0,2,-1,0,1};//垂直方向
  double x_mask[]={-1,-2,-1,0,0,0,1,2,1};//垂直方向
  img_t im[10] = {0};
  img_t* dx = im+0;
  img_t* dy = im+1;
  img_t* grad = im+2;
  img_t* gradu8 = im+3;
  int i, n, level;
  double m1, m2;
  bf_imsetsize(bf, BW2, shuru1->h, shuru1->w, sizeof(uchar), 1);
  bf_imfilter2(bf, shuru1,x_mask, 3, 3, dx);
  bf_imfilter2(bf, shuru1,y_mask, 3, 3, dy);
  n = shuru1->h*shuru1->w;
  bf_imsetsize(bf, grad, shuru1->h, shuru1->w, sizeof(double), 1);
  bf_imsetsize(bf, gradu8, shuru1->h, shuru1->w, sizeof(uchar), 1);
  m1 = m2 = grad->tt.f8[0];
  for (i=0; i<n; ++i) {
    double x = dx->tt.f8[i];
    double y = dy->tt.f8[i];
    double t = sqrt(x*x + y*y);
    grad->tt.f8[i] = t;
    m1 = MIN(m1, t);
    m2 = MAX(m2, t);
  }
  for (i=0; i<n; ++i) {
    double t = grad->tt.f8[i];
    int tt = (int)(256*(t-m1)/(m2-m1));
    tt = BOUND(tt, 0, 255);
    gradu8->tt.data[i] = tt;
    if (254==tt) {
      int asdf=0;
    }
  }
  level = imgraythresh(gradu8);
  for (i=0; i<n; ++i) {
    int t = gradu8->tt.data[i];
    gradu8->tt.data[i] = t<level ? 0 : 1;
  }
  //cvThin(gradu8, BW2, 1);
  //imclone(gradu8, BW2);
  bf_imthin(bf, gradu8, BW2, 10);
  if (VENA_DEBUG) {
    //imshow(gradu8);
    //imshow(BW2);
    //imshowmat_f8(shuru1);
    //imshowmat_f8(dx);
    //imshow(dx);
    //imshow(dy);
    //cvWaitKey(-1);
  }
  bf_imfrees(bf, im, 10);
  return 0;
}
int bf_edgecrop(buf_t* bf, const img_t* shuru1, double kk, img_t* BW2) {
  bf_imedge(bf, shuru1, BW2);
  return 0;
}
