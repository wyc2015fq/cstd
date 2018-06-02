
int otsuThreshold(const img_t* frame, img_t* dst, bool inv/*0*/)
{
  int dd;
  int width = frame->w;
  int height = frame->h;

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
      dd = S(frame, i, j);
      pixelCount[S(frame, i, j)]++;

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

  if (inv == 0) { //??
    cvThreshold(frame, dst, threshold, 255, CV_THRESH_BINARY);  //255->1
  }
  else {
    cvThreshold(frame, dst, threshold, 255, CV_THRESH_BINARY_INV);  //255->1
  }

  return 0;
}
//otsu end

// my_binarize
int OtsuAndEdgeBinarize(const img_t* plateRegion, img_t* I_final_bw, short plateColor)
{
  int i, j;
  int m, n;
  int sumWhite;
  int sumPixels;
  //otsu 二值化 黄牌反转二值化结果
  img_t I_bw[1] = {0};
  img_t plateRegionHist[1] = {0};
  img_t I_left_ver[1] = {0};
  img_t I_hor[1] = {0};
  img_t I_left_ver_bw[1] = {0};
  img_t I_hor_bw[1] = {0};

  imsetsize(I_bw, plateRegion->h, plateRegion->w, 1, 1);

  if (plateColor == 1) {
    otsuThreshold(plateRegion, I_bw, 0);
  }
  else if (plateColor == 2) {
    otsuThreshold(plateRegion, I_bw, 1);
  }
  else {
    printf("wrong plateColor");
  }

  sumWhite = 0;

  for (i = 0; i < plateRegion->w; i++) {
    for (j = 0; j < plateRegion->h; j++) {
      sumWhite += S(I_bw, i, j) / 255; //i j 的顺序？？先列后行
    }
  }

  //判断是否做拉伸或者置1处理
  sumPixels = (plateRegion->h) * (plateRegion->w);

  imsetsize(plateRegionHist, plateRegion->h, plateRegion->w, 1, 1);

  if (sumWhite > 0.75 * sumPixels) {
    //plateRegion灰度拉伸；
    cvEqualizeHist(plateRegion, plateRegionHist);

    if (plateColor == 1) {
      otsuThreshold(plateRegionHist, I_bw, 0);
    }
    else if (plateColor == 2) {
      otsuThreshold(plateRegionHist, I_bw, 1);
    }
  }
  else if (sumWhite < 0.2 * sumPixels) {
    //I_bw ={1};//可以？？
    int threshold = 0;
    cvThreshold(plateRegion, I_bw, threshold, 1, CV_THRESH_BINARY); //255->1
  }

  //边缘二值化

  imsetsize(I_left_ver, plateRegion->h, plateRegion->w, 1, 1);
  imsetsize(I_hor, plateRegion->h, plateRegion->w, 1, 1);
  imsetsize(I_left_ver_bw, plateRegion->h, plateRegion->w, 1, 1);
  imsetsize(I_hor_bw, plateRegion->h, plateRegion->w, 1, 1);

  //img_t *I_final_bw  imsetsize(, plateRegion->w, plateRegion->h, 1, 1 );

  imzero(I_left_ver);
  imzero(I_hor);

  for (m = 1; m < plateRegion->w - 1; m++) {
    for (n = 1; n < plateRegion->h - 1; n++) {
      S(I_left_ver, m, n) = (S(plateRegion, m + 1, n) - S(plateRegion, m - 1, n)) >= 0 ? S(plateRegion, m + 1, n) - S(plateRegion, m - 1, n) : 0;
      S(I_hor, m, n) = abs(S(plateRegion, m, n + 1) - S(plateRegion, m, n - 1)); //j_与matlab不同 保存这个差值的类型根据得到的结果而定
    }
  }

  otsuThreshold(I_left_ver, I_left_ver_bw, 0);
  otsuThreshold(I_hor, I_hor_bw, 0);

  for (m = 0; m < plateRegion->w; m++) {
    for (n = 0; n < plateRegion->h; n++) {
      S(I_final_bw, m, n) = (S(I_left_ver_bw, m, n) | S(I_hor_bw, m, n)) & S(I_bw, m, n); //忘了在类构造函数中给I_final_bw分配空间
    }
  }

  imfree(I_bw);
  imfree(I_left_ver);
  imfree(plateRegionHist);
  imfree(I_left_ver_bw);
  imfree(I_hor_bw);
  imfree(I_hor);
  return 0;
}
// my_binarize end


int RGBtoGray(const img_t* imrgb, img_t* I_gray)
{
  int i, j;

  //灰度化0.45 0.45 0.1
  for (i = 0; i < imrgb->w; i++) {
    for (j = 0; j < imrgb->h; j++) {
      S(I_gray, i, j) = (uchar)(F(imrgb, i, j, 0) * 0.1 + F(imrgb, i, j, 1) * 0.45 + F(imrgb, i, j, 2) * 0.45);
    }
  }

  return 0;
}

int ExtractRegion(const img_t* plate1, CvRect* rect, int f_t/*=16*/) //精定位
{
  int m, n;
  img_t I_ver[1] = {0};
  img_t I_ver_bw[1] = {0};
  int row[2] = {0};//精定位行坐标
  int col[2] = {0};
  int startRow = 0, freq, nn;
  bool first;
  int* startCol = 0;
  int* endCol = 0;

  imsetsize(I_ver, plate1->h, plate1->w, 1, 1);
  imsetsize(I_ver_bw, plate1->h, plate1->w, 1, 1);

  imzero(I_ver_bw);
  imzero(I_ver);

  for (m = 1; m < plate1->w - 1; m++) {
    for (n = 0; n < plate1->h; n++) {
      S(I_ver, m, n) = abs(S(plate1, m + 1, n) - S(plate1, m - 1, n));
    }
  }

  otsuThreshold(I_ver, I_ver_bw, 0);
#if 0
  cvNamedWindow("e", CV_WINDOW_AUTOSIZE);
  cvShowImage("e", I_ver_bw);
  cvWaitKey();
#endif

  //行扫描
  startRow = (plate1->h) / 2;
  startCol = MALLOC(int, plate1->h); //每行起始列标
  endCol = MALLOC(int, plate1->h);
  freq = 0;
  first = 0;

  // 下边缘
  for (n = startRow; n < plate1->h; n++) {
    freq = 0;

    for (m = 0; m < plate1->w - 1; m++) {

      if (S(I_ver_bw, m + 1, n) != S(I_ver_bw, m, n)) {
        freq += 1; //统计跳变次数
      }

      if (S(I_ver_bw, m, n) != 0) {
        if (first != 1) {
          first = 1;
          startCol[n] = m; //起始非零点列标
        }

        endCol[n] = m; //终止非零点
      }

    }

    first = 0;

    if (S(I_ver_bw, plate1->w - 1, n) != 0) {
      endCol[n] = m; //判断最后一列点是否为非零值
    }

    if (freq < f_t) {
      break;
      n -= 1;
    }
  }

  if (n == plate1->h) {
    n = plate1->h - 1;
  }

  row[1] = n;

  // 上边缘
  first = 0;

  for (n = startRow - 1; n >= 0; n--) {
    freq = 0;

    for (m = 0; m < plate1->w - 1; m++) {
      if (S(I_ver_bw, m + 1, n) != S(I_ver_bw, m, n)) {
        freq += 1; //统计跳变次数
      }

      if (S(I_ver_bw, m, n) != 0) {
        if (first != 1) {
          first = 1;
          startCol[n] = m; //起始非零点列标
        }

        endCol[n] = m; //终止非零点
      }

    }

    first = 0;

    if (S(I_ver_bw, plate1->w - 1, n) != 0) {
      endCol[n] = m; //判断最后一列点是否为非零值
    }

    if (freq < f_t) {
      break;
      n += 1;
    }
  }

  if (n == -1) {
    n = 0;
  }

  row[0] = n;

  //左右边界
  nn = 5;

  if ((row[1] - nn - row[0] - nn) < 5) {
    nn = 0;
  }

  for (m = row[0] + nn; m < row[1] - nn; m++) {
    col[0] += startCol[m];
    col[1] += endCol[m];
  }

  col[0] = col[0] / (row[1] - nn - row[0] - nn);
  col[1] = col[1] / (row[1] - nn - row[0] - nn);

  rect->x = col[0];
  rect->y = row[0];
  rect->w = col[1] - col[0] + 1;
  rect->h = row[1] - row[0] + 1;

#if 0
  int a = 1;

  if ((rect->w == 0) | (rect->h == 0)) {
    a = 0;
  }

#endif


  FREE(startCol);
  FREE(endCol);
  imfree(I_ver_bw);
  imfree(I_ver);
  return 0;
}

int ColorDetect(plate_segment_t* s, const img_t* plate1)
{
  //取子块
  CvRect subrect[1] = {0};
  img_t img_temp[1] = {0};
  img_t img_hsv[1] = {0};
  img_t im2[1] = {0};
  int i, j;
  int yellow = 0;
  int blue = 0;

  subrect->x = (int)(0.25 * s->rect->w + s->rect->x);
  subrect->y = (int)(0.25 * s->rect->h + s->rect->y);
  subrect->w = (int)(0.5 * s->rect->w);
  subrect->h = (int)(0.5 * s->rect->h);

  imsetsize(img_temp, subrect->h, subrect->w, 3, 1);
  imsetsize(img_hsv, subrect->h, subrect->w, 3, 1);

  imrefrc(plate1, *subrect, im2);
  imclone(im2, img_temp);

  //HSV变换得H分量
  imcolorcvt(img_temp, img_hsv, T_BGR, T_HSV);

  //cvSplit(hsv,sue,0,0,0);
  for (j = 0; j < img_hsv->h; j++) {
    for (i = 0; i < img_hsv->w; i++) {
      //0-360 转换为0-255 黄[40-80]-[28 56] 蓝色[220 260]-[156 184]
      if (F(img_hsv, i, j, 0) >= 75 && F(img_hsv, i, j, 0) <= 125) {
        blue++;
      }

      if (F(img_hsv, i, j, 0) >= 5 && F(img_hsv, i, j, 0) <= 40) {
        yellow++;
      }
    }
  }

  if (blue > yellow) {
    s->plateColor = 1;
  }
  else {
    s->plateColor = 2;
  }

  imfree(img_temp);
  imfree(img_hsv);
  return 0;
}

int HorizontalIntegrate(plate_segment_t* s, const img_t* I_final_bw, img_t* I_final_bw_integral)
{
  int m, n, cc;
  int temp;

  for (n = 0; n < I_final_bw->h; n++) {
    for (m = 0; m < I_final_bw->w; m++) {
      if (m != 0) {
        temp = S(I_final_bw_integral, m - 1, n) + S(I_final_bw, m, n) / 255;
        cc = 0;

        if (temp != 0) {
          cc = 1;
        }

        S(I_final_bw_integral, m, n) = temp;
      }
      else {
        temp = S(I_final_bw, m, n) / 255;
        S(I_final_bw_integral, m, n) = temp;
      }

    }
  }

  return 0;
}

int preprocess(plate_segment_t* s, const img_t* I_final_bw, int* ver_p)
{
  int m, n;
  int max_ver_p = 0;
  s->min_ver_p = MIN_I;

  for (m = 0; m < s->rect->w; m++) {
    ver_p[m] = 0;

    for (n = 0; n < s->rect->h; n++) {
      ver_p[m] += S(I_final_bw, m, n) / 255;
    }

    if (ver_p[m] < s->min_ver_p) {
      s->min_ver_p = ver_p[m];
    }

    if (ver_p[m] > max_ver_p) {
      max_ver_p = ver_p[m];
    }
  }

  //lenth = rect->h;
  s->lenth = s->rect->h;//max_ver_p;
  s->connected_t = (float)0.55 * s->lenth;
  s->broken_t = (float)0.4 * s->lenth;
  s->height_t = (float)0.5 * s->lenth;
  return 0;
}

int BrokenHandle(plate_segment_t* s, int* k)
{
  int d_current = s->L_R_center[*k - 1] - s->L_R_center[*k];

  if (d_current < s->broken_t) {
    s->L[*k - 1] = s->L[*k];
    s->L_R_center[*k - 1] = (s->L[*k - 1] + s->R[*k - 1]) / 2;
    *k -= 1;
  }

  return 0;
}

int EndDetermine(plate_segment_t* s, int* k)
{
  int i;
  int d_end = s->L_R_center[4] - s->L_R_center[5];
  int d_ave = (s->L_R_center[1] - s->L_R_center[4]) / 3;

  if (d_end > 1.18 * d_ave) {
    s->flag = 1;
  }
  else {
    //丢弃末尾字符
    for (i = 0; i < C_NUM - 1; i++) {
      s->L[i] = s->L[i + 1];
      s->R[i] = s->R[i + 1];
      s->L_R_center[i] = s->L_R_center[i + 1];

    }

    *k -= 1;
  }

  return 0;
}
int ConnectionHandle(plate_segment_t* s, const img_t* I_final_bw_integral, const int* ver_p, int leftCol, int rightCol, int* k, int min_ver_block)
{
  int m, n;
  int seg = 0;
  int temp_rcol = 0;
  int temp_lcol = 0;
  int min_ver_sub = MIN_I;
  int width_sub = 0;
  int height_sub = 0;

  for (m = rightCol; m >= leftCol; m--) {
    if ((seg == 0) & (ver_p[m] != min_ver_block)) {
      seg = 1;
      temp_rcol = m;
    }

    if ((seg == 1) & (ver_p[m] != min_ver_block) & (ver_p[m] < min_ver_sub) & (m != leftCol)) {
      min_ver_sub = ver_p[m];
      continue;
    }

    if ((seg == 1) & ((ver_p[m] == min_ver_block) | (m == leftCol))) {
      seg = 0;
      temp_lcol = m;
      //计算width_sub height_sub
      width_sub = temp_rcol - temp_lcol + 1;

      for (n = 0; n < s->rect->h; n++) {
        if (S(I_final_bw_integral, temp_rcol, n) != S(I_final_bw_integral, temp_lcol, n)) {
          height_sub++;
        }
      }

      if (height_sub < s->height_t) {
        min_ver_sub = MIN_I;
        height_sub = 0;
        width_sub = 0;
        continue;
      }

      if (width_sub > s->connected_t) {
        ConnectionHandle(s, I_final_bw_integral, ver_p, temp_lcol, temp_rcol, k, min_ver_sub);
      }
      else {
        s->L[*k] = temp_lcol;
        s->R[*k] = temp_rcol;
        s->L_R_center[*k] = (s->L[*k] + s->R[*k]) / 2;

        if (*k > 2) {
          BrokenHandle(s, k);
        }

        *k += 1;

        if (*k == C_NUM) {
          EndDetermine(s, k);
        }
      }

      if (s->flag == 1) {
        break;
      }

      min_ver_sub = MIN_I;
      height_sub = 0;
      width_sub = 0;
    }
  }

  return 0;
}

int CharacterSegment(plate_segment_t* s, const img_t* I_final_bw_integral, const int* ver_p)
{
  int m, n;
  int k = 0;
  bool seg = 0;
  int leftCol = 0;
  int rightCol = 0;
  int width_sub = 0;
  int height_sub = 0;
  int min_ver_block = MIN_I;

  //int *temp=new int[rect->w];
  //取块儿 从右往左取
  for (m = s->rect->w - 1; m >= 0; m--) {
    if ((seg == 0) & (ver_p[m] > s->min_ver_p)) {
      seg = 1;
      rightCol = m;
      //temp[m]=ver_p[m];
    }

    if ((seg == 1) & (ver_p[m] != s->min_ver_p) & (ver_p[m] < min_ver_block)) {
      min_ver_block = ver_p[m];
      continue;
    }

    if ((ver_p[m] == s->min_ver_p) & (seg == 1)) {
      seg = 0;
      leftCol = m;
      width_sub = rightCol - leftCol + 1;

      for (n = 0; n < s->rect->h; n++) {
        if (S(I_final_bw_integral, rightCol, n) != S(I_final_bw_integral, leftCol, n)) {
          height_sub++;
        }
      }

      if (height_sub < s->height_t) {
        min_ver_block = MIN_I;
        width_sub = 0;
        height_sub = 0;
        continue;
      }

      if (width_sub > s->connected_t) {
        ConnectionHandle(s, I_final_bw_integral, ver_p, leftCol, rightCol, &k, min_ver_block);
      }
      else {
        s->L[k] = leftCol;
        s->R[k] = rightCol;
        s->L_R_center[k] = (s->L[k] + s->R[k]) / 2;

        if (k > 2) {
          BrokenHandle(s, &k);
        }

        k += 1;

        if (k == C_NUM) {
          EndDetermine(s, &k);

        }

      }

      if (s->flag == 1) {
        break;
      }

      min_ver_block = MIN_I;
      width_sub = 0;
      height_sub = 0;

    }

  }

  return 0;
}

int Segment(const img_t* plate1, img_t* PlateChar)
{
  //精定位 并将车牌灰度图放入plateRegion
  int flag;
  int f_t = 16;
  img_t im2[1] = {0};
  plate_segment_t s[1] = {0};
  img_t plate_gray[1] = {0};
  img_t plateRegion[1] = {0};
  img_t plateRegionRGB[1] = {0};
  img_t plateRegionHist[1] = {0};
  img_t I_final_bw[1] = {0};
  img_t I_final_bw_integral[1] = {0};
  img_t PlateChar_all[1] = {0};
  int* ver_p;

  //plateRegion  imsetsize(, plate1->w, plate1->h, 1, 1 );
  s->plateColor = 3;
  //I_final_bw  imsetsize(, plateRegion->w, plateRegion->h, 1, 1 );
  imsetsize(plate_gray, plate1->h, plate1->w, 1, 1);

  s->connected_t = 0;
  s->broken_t = 0;
  s->height_t = 0;
  ver_p = 0;
  s->flag = 0;

  RGBtoGray(plate1, plate_gray);
  ExtractRegion(plate_gray, s->rect, f_t);
  //imshow(plate1); imshow(plate_gray);  cvWaitKey(-1);

  if (s->rect->w < 60 || s->rect->h < 6 || s->rect->x < 0 || s->rect->y < 0) {
    s->flag = 2;
    return 2;
  }

  imsetsize(plateRegion, s->rect->h, s->rect->w, 1, 1);
  imsetsize(plateRegionRGB, s->rect->h, s->rect->w, 3, 1);


  imrefrc(plate_gray, *s->rect, im2);
  imclone(im2, plateRegion);

  imrefrc(plate1, *s->rect, im2);
  imclone(im2, plateRegionRGB);

  //颜色判别
  ColorDetect(s, plate1);

  flag = s->flag;

  if (s->plateColor != 1 && s->plateColor != 2) {
    s->flag = 3;
    return 3;
  }

  //二值化
  imsetsize(I_final_bw, s->rect->h, s->rect->w, 1, 1);
  OtsuAndEdgeBinarize(plateRegion, I_final_bw, s->plateColor);

  //计算水平投影积分图
  imsetsize(I_final_bw_integral, s->rect->h, s->rect->w, 1, 1);
  HorizontalIntegrate(s, I_final_bw, I_final_bw_integral);
  //imshow(I_final_bw_integral);cvWaitKey(-1);

  ver_p = MALLOC(int, s->rect->w);
  //预处理
  preprocess(s, I_final_bw, ver_p);

  //分割
  CharacterSegment(s, I_final_bw_integral, ver_p);

  if (s->flag == 0) { //& (plateColor==2)
    //灰度拉伸 重新分割
    //plateRegion灰度拉伸；
    imsetsize(plateRegionHist, s->rect->h, s->rect->w, 1, 1);
    cvEqualizeHist(plateRegion, plateRegionHist);
    imclone(plateRegionHist, plateRegion);
    imfree(plateRegionHist);
    //plateRegion=plateRegionHist;

    //二值化
    OtsuAndEdgeBinarize(plateRegion, I_final_bw, s->plateColor);

    //计算水平投影积分图
    HorizontalIntegrate(s, I_final_bw, I_final_bw_integral);

    //预处理
    preprocess(s, I_final_bw, ver_p);

    //分割
    CharacterSegment(s, I_final_bw_integral, ver_p);
  }

  if (s->flag == 1) {
    //字符提取
    int i;
    img_t im2[1] = {0};
    //img_t *img=plateRegion;
    CvRect rect_all = cvRect(s->L[5], 0, s->R[0] - s->L[5] + 1, plate1->h);
    s->rect_all = rect_all;

    imrefrc(plateRegion, rect_all, im2);
    imsetsize(PlateChar_all, rect_all.h, rect_all.w, 1, 1);
    imclone(im2, PlateChar_all);

    if (s->plateColor == 1) {
      otsuThreshold(PlateChar_all, PlateChar_all, 0);
    }
    else {
      otsuThreshold(PlateChar_all, PlateChar_all, 1);
    }

    for (i = 0; i < 6; ++i) {
      int j = 5 - i;
      CvRect rect = cvRect(s->L[j] - s->L[5], 0, s->R[j] - s->L[j] + 1, plate1->h);
      imsetsize(PlateChar + i, rect.h, rect.w, 1, 1);
      imrefrc(PlateChar_all, rect, im2);
      imclone(im2, PlateChar + i);
    }
  }//分割前二值化结果及分割结果

  flag = s->flag;
  //imfree(plateRegionHist);//与plateRegion指向同一片内存空间
  //分割不成功时，没有分配内存
  FREE(ver_p);
  imfree(I_final_bw);
  imfree(I_final_bw_integral);
  imfree(PlateChar_all);
  imfree(plate_gray);
  imfree(plateRegion);
  imfree(plateRegionRGB);
  return flag;
}

