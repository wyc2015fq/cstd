// CascadeLocation.cpp : Defines the entry point for the console application.
//
//取数值到img中,画框在pframe中

int rulefusion(m_Rect rect11, m_Rect rect22)
{
  int mid11 = ((rect11.x + rect11.x + rect11.w) / 2);
  int mid22 = ((rect22.x + rect22.x + rect22.w) / 2);
  int start11 = rect11.x;
  int end11 = rect11.x + rect11.w;
  int start22 = rect22.x;
  int end22 = rect22.x + rect22.w;

  if (mid11 > start22 && mid11 < end22 && mid22 > start11 && mid22 < end22) {
    return 1;
  }
  else {
    return 0;
  }
}

#define THD  60
#define ISW(_X)  ((_X)>THD)
#define ISB(_X)  ((_X)<=THD)

// 函数名称:    WriteData
// 函数功能:    把图片转换成特征向量,存储在svm_node *x中
// 输入:        img图像
// 返回值：     无
// 备注：       无
int WriteData(img_t* img)
{
  int h = img->h, w = img->w;
#if 1
  CvRect rect;
  int m, n, i, j, k;
  //投影处理
  int up = 0;
  int down = img->h;
  int left = 0;
  int right = img->w;
  int* colcount;
  int* rowcount;
  colcount = MALLOC(int, img->w);
  rowcount = MALLOC(int, img->h);

  for (m = 0; m < img->w; m++) {
    colcount[m] = 0;
  }

  for (m = 0; m < img->h; m++) {
    rowcount[m] = 0;
  }

  for (m = 0; m < img->w; m++) {
    for (n = 0; n < img->h; n++) {
      if (ISW(S(img, m, n))) {
        colcount[m]++;
        rowcount[n]++;
      }
    }
  }

  for (m = 0; m < img->w; m++) {
    if (m < img->w / 2) {
      if (colcount[m] == 0) {
        left = m;
      }
    }
    else {
      if (right == img->w && colcount[m] == 0) {
        right = m;
        break;
      }
    }
  }

  for (m = 0; m < img->h; m++) {
    if (m < img->h / 2) {
      if (rowcount[m] == 0) {
        up = m;
      }
    }
    else {
      if (down == img->h && rowcount[m] == 0) {
        down = m;
        break;
      }
    }
  }

  rect.h = down - up - 1;
  rect.w = right - left - 1;
  rect.x = left + 1;
  rect.y = up + 1;
  //投影处理结束
#endif

  if (rect.h >= 10 && rect.w >= 2 && rect.x >= 0 && rect.y >= 0) {
    img_t BinaryImg[1] = {0};
    img_t ResizeImg[1] = {0};
    img_t preciseimg[1] = {0};
    img_t im2[1] = {0};

    imsetsize(BinaryImg, h, w, 1, 1); //创建二进制图像
    imsetsize(ResizeImg, PLSTD_H, PLSTD_W, 1, 1);     //创建归一化后的图像
    cvThreshold(img, BinaryImg, 50, 255, CV_THRESH_BINARY);
    imsetsize(preciseimg, rect.h, rect.w, 1, 1);

    imrefrc(BinaryImg, rect, im2);
    imresize_bili(im2, ResizeImg);                                    //图像归一化为PLSTD_W*PLSTD_H大小
    cvThreshold(ResizeImg, ResizeImg, 50, 255, CV_THRESH_BINARY);

    //x = (struct svm_node *) malloc(max_nr_attr*sizeof(struct svm_node));//分配64个svm_node(index,value)空间大小
    k = 0;

    for (i = 0; i < ResizeImg->w; i++) {
      for (j = 0; j < ResizeImg->h; j++) {
        int data;

        if (k >= max_nr_attr - 1) { // need one more for index = -1且当内存不够时,多分配一倍的空间
          max_nr_attr *= 2;
          x = (struct svm_node*) realloc(x, max_nr_attr * sizeof(struct svm_node));
        }

        data = S(ResizeImg, i, j);

        if (0 == data) {
          x[k].index = i * PLSTD_H + j;
          x[k].value = -1;
        }
        else {
          x[k].index = i * PLSTD_H + j;
          x[k].value = 1;
        }

        ++k;
      }
    }

    x[k].index = -1;  //读完一行之后,接下来一个节点的index置为一,后面用来判断是否读取完毕
    FREE(colcount);
    FREE(rowcount);
#if 0
    //保存字符图像//
    CString char_str = "";
    char_str.Format("%d", predict_index);
    char_str = char_str + ".jpg";
    cvSaveImage(char_str, preciseimg);
    predict_index++;
    //保存结束//
#endif
    imfree(BinaryImg);
    imfree(ResizeImg);
    imfree(preciseimg);
    return 0;
  }

  return 1;
}
int replace_img_with_rect(img_t* rectimg, img_t* dst, int x, int y) //复制矩形图像到dst的X,Y位置
{
  CvRect roi_rect;
  img_t im2[1] = {0};
  int i, j;
  roi_rect.h = rectimg->h;
  roi_rect.w = rectimg->w;
  roi_rect.x = x;
  roi_rect.y = y;
  imrefrc(dst, roi_rect, im2);

  if (rectimg->c == 1) {
    img_t copy_rect_img[1] = {0};
    imsetsize(copy_rect_img, rectimg->h, rectimg->w, 3, 1);

    for (i = 0; i < rectimg->w; i++) {
      for (j = 0; j < rectimg->h; j++) {
        F(copy_rect_img, i, j, 0) = S(rectimg, i, j);
        F(copy_rect_img, i, j, 1) = S(rectimg, i, j);
        F(copy_rect_img, i, j, 2) = S(rectimg, i, j);
      }
    }

    imclone(copy_rect_img, im2);
    imfree(copy_rect_img);
  }
  else {
    imclone(rectimg, im2);
  }

  //imfree(&copy_rect_img);
  return 0;
}
int setwhite(img_t* img, int start, int end, int col) //置为白
{
  int i;

  for (i = start; i <= end; i++) {
    S(img, col, i) = 255;
  }

  return 0;
}

int setblack(img_t* img, int start, int end, int col) //置为黑
{
  int i;

  for (i = start; i <= end; i++) {
    S(img, col, i) = 0;
  }

  return 0;
}
int setrowblack(img_t* img, int start, int end, int row) //置为黑
{
  int i;

  for (i = start; i <= end; i++) {
    S(img, i, row) = 0;
  }

  return 0;
}
int colfusionrect(img_t* rect_img)
{
  int i, j;
  int startloc = 0;
  int endloc = 0;
  int count = 0;

  for (i = 0; i < rect_img->w; i++) {
    startloc = 0;
    endloc = 0;
    count = 0;

    for (j = 0; j < rect_img->h; j++) {
      if (ISW(S(rect_img, i, j))) { //是白点
        count++;
        startloc = j;
      }

      if (ISB(S(rect_img, i, j)) && startloc != 0) { //是黑点
        endloc = j;

        if (endloc - startloc <= max_fusion_margin) {
          count++;
        }

        if (j >= rect_img->h - 2 || endloc - startloc > max_fusion_margin) {
          int value = startloc - endloc + count;

          if (value > max_fusion_height || value < min_fusion_height) {
            setblack(rect_img, endloc - count, startloc, i);
          }
          else {
            setwhite(rect_img, endloc - count, startloc, i);
          }

          count = 0;
          startloc = 0;
          endloc = 0;

          if (j >= rect_img->h - 2) {
            break;
          }

          continue;
        }
      }

    }
  }

  return 0;
}

int rowfusionrect(img_t* rect_img)
{
  int count = 0;
  int i, j;

  for (j = 0; j < rect_img->h; j++) {
    count = 0;

    for (i = 0; i < rect_img->w; i++) {
      if (ISW(S(rect_img, i, j))) {
        ++count;
      }
      else {
        if (count < min_row_width && count != 0) {
          setrowblack(rect_img, i - count, i, j);
          count = 0;
          continue;
        }

        count = 0;
      }
    }
  }

  return 0;
}

int OnePass(runpass_t* s, const img_t* img, const img_t* rect_img, int hp)
{
  int i, j;
  int min_min_dist = (int)(0.6 * hp); //如果前5次跳变小于这个值就丢弃该run
  int min_dist = hp;
  int LP_width_min = (int)(3 * hp);
  int LP_width_min_min = (int)(4 * hp);
  int LP_width_max = 6 * hp;
  int LP_height_max = (int)(1.2 * hp);
  int LP_height_min = (int)(0.8 * hp);
  int LP_square = (LP_width_min + LP_width_max) * hp / 8;
  int pointlist[1000] = {0};  //记录一个run的每个点的横坐标
  int pointcount = 0;
  int min_pointcount = 10; //相当于跳变次数
  int rowrunlen = 0; //每行的run个数
  s->runlen = 0;

  for (j = 0; j < img->h; j++) {
    pointcount = 0;
    rowrunlen = 0;

    for (i = 0; i < img->w; i++) {
      if (ISW(S(img, i, j)) && ISB(S(img, i + 1, j)) || (i > img->w - 2)) {


        pointlist[pointcount] = i;

        if (pointcount <= PRE_NUM && Dist(pointlist[pointcount], pointlist[pointcount - 1]) > min_min_dist) {
          pointcount = 0;
          pointlist[pointcount] = i;
          ++pointcount;
          continue;
        }

        if (Dist(pointlist[pointcount], pointlist[0]) > LP_width_min && Dist(pointlist[pointcount], pointlist[pointcount - 1]) > min_min_dist) {
          pointcount = pointcount - 1;

          if (
              (pointcount < min_pointcount)
              || (Dist(pointlist[pointcount], pointlist[0]) > LP_width_max)
              || (Dist(pointlist[pointcount], pointlist[0]) < LP_width_min)
          ) {
            pointcount = 0;
            pointlist[pointcount] = i;
            ++pointcount;
            continue;
          }
          else {
            s->run[s->runlen].x = pointlist[0];
            s->run[s->runlen].y = j;
            s->run[s->runlen].h = 1;
            s->run[s->runlen].w = Dist(pointlist[pointcount], pointlist[0]);
            s->run[s->runlen].count = 1;
            s->run[s->runlen].flag = 0;
            ++rowrunlen;
            ++s->runlen;
            pointcount = 0;
            continue;
          }
        }

        if ((pointcount > 1 && Dist(pointlist[pointcount], pointlist[pointcount - 1]) > min_dist) || (i > img->w - 2)) {

          pointcount = pointcount - 1;

          if (
              (pointcount < min_pointcount)
              || (Dist(pointlist[pointcount], pointlist[0]) > LP_width_max)
              || (Dist(pointlist[pointcount], pointlist[0]) < LP_width_min)
          ) {
            pointcount = 0;
            pointlist[pointcount] = i;
            ++pointcount;
            continue;
          }
          else {
            s->run[s->runlen].x = pointlist[0];
            s->run[s->runlen].y = j;
            s->run[s->runlen].h = 1;
            s->run[s->runlen].w = Dist(pointlist[pointcount], pointlist[0]);
            s->run[s->runlen].count = 1;
            s->run[s->runlen].flag = 0;
            ++rowrunlen;
            ++s->runlen;
            pointcount = 0;
            continue;
          }
        }

        if ((i > img->w - 2)) {
          break;
        }

        ++pointcount;
      }

    }

#if 0
    int index = 0;

    for (k = 0; k < runlen - rowrunlen; k++) { //融合
      int label = 0;
      run[index] = run[k];

      for (l = runlen - rowrunlen; l < rowrunlen; l++) {

        if (rulefusion(run[index], run[l])) {
          run[index].x = (run[index].x * run[index].count + run[l].x * run[l].count) / (run[index].count + run[l].count);
          int m_w = (run[k].x + run[k].w) * run[k].count + (run[l].x + run[l].w) * run[l].count;
          m_w = m_w / (run[k].count + run[l].count);
          //run[index].y=(run[index].y*run[index].count+run[l].y*run[l].count)/(run[index].count+run[l].count);
          run[index].w = m_w - run[index].x;

          run[l].flag = 1;
          label = 1;

        }

      }

      if (label == 1) {
        run[index].h = run[index].h + 1;
        run[index].count = run[index].count + 1;
        index++;
      }
      else {
        lastrun[lastruncount] = run[k];
        lastruncount++;
      }
    }

    for (l = runlen - rowrunlen; l < rowrunlen; l++) {
      if (run[l].flag == 0) {
        run[index] = run[l];
        ++index;
      }
    }

    if (index) {
      runlen = index;
    }

#endif
  }

#if 0

  for (i = 0; i < lastruncount; i++) {
    IPOINT pt1, pt2;
    pt1.x = lastrun[i].x;
    pt1.y = lastrun[i].y;
    pt2.x = lastrun[i].x + lastrun[i].w;
    pt2.y = lastrun[i].y + lastrun[i].h;
    //if(lastrun[i].h>LP_height_min)
    {
      cvRectangle(frame, pt1, pt2, CV_RGB(255, 0, 0));
    }

  }

#endif
#if 0

  for (i = 0; i < runlen; i++) {
    IPOINT pt1, pt2;
    pt1.x = run[i].x;
    pt1.y = run[i].y;
    pt2.x = run[i].x + run[i].w - 1;
    pt2.y = run[i].y + run[i].h - 1;
    //if(lastrun[i].h>LP_height_min)
    {
      cvRectangle(frame, pt1, pt2, CV_RGB(255, 0, 0));
    }

  }

#endif
#if 1

  for (i = 0; i < s->runlen; i++) {
    IPOINT pt1, pt2;
    pt1.x = s->run[i].x;
    pt1.y = s->run[i].y;
    pt2.x = s->run[i].x + s->run[i].w - 1;
    pt2.y = s->run[i].y + s->run[i].h - 1;

    for (j = s->run[i].x; j < s->run[i].x + s->run[i].w; j++) {
      S(rect_img, j, s->run[i].y) = 255;
    }
  }


#endif

  return 0;
}

int preciseprocess(const img_t* rect_img, CvRect r, CvRect* m_rect)
{
  int i, j;
  int rowstart[500] = {0};
  int rowend[500] = {0};
  int whitecount = 0;
  int blackcount = 0;
  int count = 0;
  double ratio;

  for (j = r.y; j < r.y + r.h; j++) {
    count = 0;

    for (i = r.x; i < r.x + r.w; i++) {
      if (ISW(S(rect_img, i, j))) {
        whitecount++;
        count++;

        if (count == 1) {
          rowstart[j - r.y] = i;
        }
      }

      if (ISB(S(rect_img, i, j))) {
        blackcount++;
      }
    }

    rowend[j - r.y] = rowstart[j - r.y] + count - 1;
  }

  ratio = (double)(blackcount) / (double)(whitecount);

  if (ratio > 0.5 && r.h > 30) {
    int leftstart = 0;
    int leftend = 0;
    int rightstart = 0;
    int rightend = 0;
    int flagstart = 0;
    int flagend = 0;
    int threshold_fusion = 40;

    for (i = 0; i < r.h; i++) {
      int start = abs(rowstart[i] - r.x);
      int end = abs(rowend[i] - r.x - r.w + 1);

      if (start <= threshold_fusion && flagstart == 0) {
        leftstart = i;
        flagstart = 1;
      }

      if (end <= threshold_fusion && flagend == 0) {
        rightstart = i;
        flagend = 1;
      }

      if (flagstart && flagend) {
        break;
      }
    }

    flagstart = 0;
    flagend = 0;

    for (i = r.h - 1; i >= 0; i--) {
      int start = abs(rowstart[i] - r.x);
      int end = abs(rowend[i] - r.x - r.w + 1);

      if (start <= threshold_fusion && flagstart == 0) {
        leftend = i;
        flagstart = 1;
      }

      if (end <= threshold_fusion && flagend == 0) {
        rightend = i;
        flagend = 1;
      }

      if (flagstart && flagend) {
        break;
      }
    }

    m_rect[0].x = r.x;
    m_rect[0].y = leftstart + r.y;
    m_rect[0].h = leftend - leftstart + 1;
    m_rect[0].w = rowend[m_rect[0].h / 2 + leftstart] - rowstart[m_rect[0].h / 2 + leftstart];


    m_rect[1].h = rightend - rightstart + 1;
    m_rect[1].w = rowend[m_rect[1].h / 2 + rightstart] - rowstart[m_rect[1].h / 2 + rightstart];
    m_rect[1].x = rowstart[m_rect[1].h / 2 + rightstart];
    m_rect[1].y = rightstart + r.y;
    return 2;
  }

  //delete [] rowstart;
  //delete [] rowend;
  return 0;
}
CvRect extend_rect(int h, int w, CvRect r, int steppixel)
{
  //int steppixel=5;
  CvRect rect;
  int rect_h = 0;
  int rect_w = 0;
  int rect_x = 0;
  int rect_y = 0;

  if (r.x - steppixel >= 0) {
    rect_x = r.x - steppixel;
  }
  else {
    rect_x = 0;
  }

  if (r.y - steppixel >= 0) {
    rect_y = r.y - steppixel;
  }
  else {
    rect_y = 0;
  }

  if (r.x + r.w + steppixel < w - 1) {
    rect_w = r.x + r.w + steppixel - rect_x;
  }
  else {
    rect_w = w - 1 - rect_x;
  }

  if (r.y + r.h + steppixel < h - 1) {
    rect_h = r.y + r.h + steppixel - rect_y;
  }
  else {
    rect_h = h - 1 - rect_y;
  }

  rect.h = rect_h;
  rect.w = rect_w;
  rect.x = rect_x;
  rect.y = rect_y;
  return rect;
}
int  rect_remove_adaboost(img_t* grayimg, CvRect r)
{
  CvRect rect = extend_rect(grayimg->h, grayimg->w, r, 5);
  const unsigned char* data = (const unsigned char*)(grayimg->tt.data + rect.y * grayimg->s + rect.x);
  int rectnum;
  //img_t im2[1]={0}; imrefrc(grayimg, rect, im2); imshow(im2); cvWaitKey(-1);
  rectnum = rectdetect(rect.h, rect.w, data, grayimg->s);
  //cvShowImage("adsfasdf", rect.h, rect.w, data, grayimg->s, 1); cvWaitKey(-1);
  return rectnum;
}
CvRect process_long_rect(img_t* binary, CvRect edge_rect)
{
  CvRect rect = edge_rect;
  int* project_count;
  int i, j, k;
  int region_width;
  int pixel_count;

  int colstart = 0;
  int colend = edge_rect.w;
  int margin_start = 0;
  int margin_end = edge_rect.w;
  int plate_width = (int)(3.5 * edge_rect.h);
  int leftcount = 0;
  int rightcount = 0;
  img_t roi_img[1] = {0};
  img_t im2[1] = {0};

  project_count = MALLOC(int, edge_rect.w);

  for (k = 0; k < edge_rect.w; k++) {
    project_count[k] = 0;
  }

  imsetsize(roi_img, edge_rect.h, edge_rect.w, 1, 1);
  imrefrc(binary, edge_rect, im2);
  imcopy(im2, roi_img);

  for (i = 0; i < roi_img->w; i++) {
    for (j = 0; j < roi_img->h; j++) {
      if (ISW(S(roi_img, i, j))) {
        project_count[i]++;
      }
    }
  }

  region_width = roi_img->w - 4 * roi_img->h;
  pixel_count = 0;

  for (i = 0; i < region_width; i++) {
    pixel_count = 0;

    for (j = i; j < 3 * roi_img->h + i; j++) {
      pixel_count = pixel_count + project_count[j];
    }

    if (pixel_count > 0.5 * 3 * roi_img->h * roi_img->h) {
      colstart = i;
      break;
    }
  }

  region_width = roi_img->w - 2 * roi_img->h;
  pixel_count = 0;

  for (i = roi_img->w - 1; i >= region_width; i--) {
    pixel_count = 0;

    for (j = i; j >= i - 3 * roi_img->h; j--) {
      pixel_count = pixel_count + project_count[j];
    }

    if (pixel_count > 0.5 * 3 * roi_img->h * roi_img->h) {
      colend = i;
      break;
    }
  }

  rect.x = rect.x + colstart;
  rect.w = colend - colstart;
#if 0

  for (i = 0; i < roi_img->w - plate_width; i++) {
    if (project_count[i] >= 0.4 * roi_img->h && colstart == 0) {
      colstart = i;
    }

    if (project_count[i] == 0) {
      leftcount++;
    }
    else {
      leftcount = 0;
    }

    if (leftcount >= roi_img->h / 2 && margin_start == 0) {
      margin_start = i;
      break;
    }
  }

  for (i = roi_img->w - 1; i >= plate_width; i--) {
    if (project_count[i] >= 0.4 * roi_img->h && colend == edge_rect.w) {
      colend = i;
    }

    if (project_count[i] == 0) {
      rightcount++;
    }
    else {
      rightcount = 0;
    }

    if (rightcount >= roi_img->h / 2 && margin_end == edge_rect.w) {
      margin_end = i;
      break;
    }
  }

  if ((colend - colstart) < 6 * roi_img->h) {
    rect.x = rect.x + colstart;
    rect.w = colend - colstart;
  }
  else if (margin_end - margin_start < 6 * roi_img->h) {
    rect.x = rect.x + margin_start;
    rect.w = margin_end - margin_start;
  }
  else {
    rect.x = rect.x + MAX_VALUE(margin_start, colstart);
    rect.w = MIN_VALUE(colend, margin_end) - MAX_VALUE(margin_start, colstart);
  }

#endif
  FREE(project_count);
  return rect;
}

int character_cut_process(const img_t* m_rect_img, char* text)
{
  img_t PlateChar[6] = {0};
  double* prob_estimates;
  int count_one, count_erro;
  int i, flag;
  int err;

  //imshow(m_rect_img);cvWaitKey(-1);
  flag = Segment(m_rect_img, PlateChar);

  if (flag == 1) {
    prob_estimates = MALLOC(double, model->nr_class);
    count_one = 0;
    count_erro = 0;

    for (i = 0; i < 6; i++) {
      int erro_write;
      x = (svm_node*) malloc(max_nr_attr * sizeof(svm_node)); //分配64个svm_node(index,value)空间大小
      erro_write = WriteData(PlateChar + i);

      if (erro_write) {
        text[i] = '*';
        count_erro++;
      }
      else {
        if (model->probA != NULL && model->probB != NULL) {
          double predict_label = svm_predict_probability(model, x, prob_estimates);
          double prob_value = prob_estimates[prob[(int)predict_label]];

          if (prob_value < 0.3) {
            text[i] = character[(int)predict_label];
            count_erro++;
          }
          else {
            text[i] = character[(int)predict_label];

            if (text[i] == '1') {
              count_one++;
            }
          }
        }
        else {
          double predict_label = svm_predict(model, x); //预测x属于哪一类,返回值为相应的label
          text[i] = character[(int)predict_label];
        }

        //double predict_label = svm_predict(model,x);  //预测x属于哪一类,返回值为相应的label
      }

      FREE(x);
      x = NULL;
    }

    if (count_one + count_erro >= 5 || count_erro >= 4 || count_one >= 4) {
      for (i = 0; i < 6; i++) {
        text[i] = '*';
      }
    }

    err = 0;

    for (i = 0; i < 6; i++) {
      if (text[i] == '*') {
        err = 1;
        break;
      }
    }

  }

  freeims(PlateChar, 6);
  //imshow(frame);cvWaitKey(-1);
  //imfree(rect_img);
  return 0;
}

