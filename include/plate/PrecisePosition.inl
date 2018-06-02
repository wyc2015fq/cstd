

int MIN_JUMP =  14; //最小跳变次数
int MIN_ROW = 15;   //最小连续跳变行数
int color = 3;     //color=0为黄牌,color=1为蓝牌
typedef struct updown {
  int up;
  int down;
} updown;
updown Locate_up_down(const img_t* binary)
{
  updown m_locate;
  int i, j, k;
  int endrow, startrow;
  int* binaryrowcount = 0;
  binaryrowcount = MALLOC(int, binary->h);     //记录原始边缘图跳变次数

  for (k = 0; k < binary->h; k++) {
    binaryrowcount[k] = 0;
  }

  for (i = 1; i < binary->w; i++) {
    for (j = 0; j < binary->h; j++) {
      if (S(binary, i, j) != S(binary, i - 1, j)) {
        binaryrowcount[j]++;
      }
    }
  }

  endrow = binary->h - 1;
  startrow = 0;

  for (i = binary->h / 2; i < binary->h; i++) { //找出车牌终止行
    if (binaryrowcount[i] < 10) {
      endrow = i;
      break;
    }
  }

  if (endrow == binary->h / 2) {
    for (i = 0; i < binary->h; i++) { //找出车牌起始行
      if (binaryrowcount[i] > 10) {
        startrow = i;
        break;
      }
    }

    for (i = binary->h - 1; i > 0; i--) { //找出车牌终止行
      if (binaryrowcount[i] > 10) {
        endrow = i;
        break;
      }
    }
  }
  else {
    for (i = binary->h / 2; i > 0; i--) { //找出车牌起始行
      if (binaryrowcount[i] < 10) {
        startrow = i;
        break;
      }
    }
  }

  m_locate.up = startrow;
  m_locate.down = endrow;
  FREE(binaryrowcount);
  return m_locate;
}

void Precise_Locate_with_color(img_t* img_copy, img_t* plate)
{
  int i, j, k;
  int margin = 12;
  int extend_width = 1;
  img_t img[1] = {0};

  img_t grayimg[1] = {0};
  img_t binary[1] = {0};
  img_t source[1] = {0};
  img_t edge[1] = {0};

  img_t hsv[1] = {0};
  img_t hue[1] = {0};
  img_t sue[1] = {0};
  img_t vue[1] = {0};
  img_t Iue[1] = {0};
  img_t yellowimg[1] = {0};
  img_t blueimg[1] = {0};

  img_t blue[1] = {0};
  img_t yellow[1] = {0};

  int* yellowrowcount;
  int* yellowcolcount;
  int* bluerowcount;
  int* bluecolcount;
  int* bluecount;
  int* yellowcount;
  int* binaryrowcount;
  int endrow, startrow;

  imsetsize(img, img_copy->h, img_copy->w, 1, 1);
  imsetsize(grayimg, img->h, img->w, 1, 1);
  imsetsize(binary, img->h, img->w, 1, 1);
  imsetsize(source, img->h, img->w, 1, 1);
  imsetsize(edge, img->h, img->w, 2, 1);
  //cvCvtColor(img_copy,img,CV_BGR2GRAY);
  imcolorcvt(img_copy, img, T_BGR, T_GRAY);
  cvEqualizeHist(img, grayimg);
  // SOBEL边缘提取,得到边缘图像binary
  //cvSmooth(grayimg,grayimg,CV_GAUSSIAN,3,0,0);//高斯滤波
  //cvSobel(grayimg,edge,1,0,1);
  //cvAbs(edge,edge);
  //cvConvertScaleAbs(edge,source);
  abssobel_v(grayimg, source);

  otsuThreshold(source, binary, 0);
  //cvSmooth(binary,smooth,CV_MEDIAN,3,0,0);//3*3均值滤波
  ////////////////SOBEL边缘提取,得到边缘图像binary//////////////////////

  imsetsize(hsv, img->h, img->w, 3, 1); //hsv空间图像
  imsetsize(hue, img->h, img->w, 1, 1); //h分量
  imsetsize(sue, img->h, img->w, 1, 1); //s分量
  imsetsize(vue, img->h, img->w, 1, 1); //V分量
  imsetsize(Iue, img->h, img->w, 1, 1); //I分量
  imsetsize(yellowimg, img->h, img->w, 1, 1); //黄色
  imsetsize(blueimg, img->h, img->w, 1, 1); //蓝色

  imsetsize(blue, img->h, img->w, 1, 1); //蓝色分量
  imsetsize(yellow, img->h, img->w, 1, 1); //黄色分量

  imzero(blue);
  imzero(yellow);

  imzero(blueimg);
  imzero(yellowimg);
  //cvCvtColor(img_copy, hsv,CV_BGR2HSV);
  imcolorcvt(img_copy, hsv, T_BGR, T_HSV);
  //cvSplit(hsv,hue,sue,vue,0);
  //cvNamedWindow("test",1);
  //cvShowImage("test",sue);

  yellowrowcount = MALLOC(int, img->h);  //记录跳变次数
  yellowcolcount = MALLOC(int, img->w);  //记录垂直投影值
  bluerowcount = MALLOC(int, img->h);   //记录跳变次数
  bluecolcount = MALLOC(int, img->w);    //记录垂直投影值
  bluecount = MALLOC(int, img->w);    //记录垂直投影累加值
  yellowcount = MALLOC(int, img->w);  //记录垂直投影累加值
  binaryrowcount = MALLOC(int, img->w);    //记录原始边缘图跳变次数

  for (k = 0; k < img->h; k++) {
    yellowrowcount[k] = 0;
    bluerowcount[k] = 0;
    binaryrowcount[k] = 0;
  }

  yellowcolcount[0] = 0;
  bluecolcount[0] = 0;

  for (i = 1; i < img->w; i++) { //两个for循环得到yellow和blue两幅图像的垂直投影和水平跳变次数
    yellowcolcount[i] = 0;
    bluecolcount[i] = 0;
    yellowcount[i] = 0;
    bluecount[i] = 0;

    for (j = 1; j < img->h; j++) {
#if 1

      if (S(hue, i, j) >= 85 && S(hue, i, j) <= 125 && S(sue, i, j) > 30) {
        S(blue, i, j) = 255;
      }

      if (S(hue, i, j) >= 10 && S(hue, i, j) <= 40 && S(sue, i, j) > 100) {
        S(yellow, i, j) = 255;
      }

#endif
      S(Iue, i, j) = (F(img_copy, i, j, 0) + F(img_copy, i, j, 1) + F(img_copy, i, j, 2)) / 3;

      if (S(binary, i, j) == 255) {
        if (S(hue, i, j) >= 85 && S(hue, i, j) <= 125 && S(sue, i, j) > 30) {
          S(blueimg, i, j) = 255;
          bluecolcount[i]++;
        }

        if (S(hue, i, j) >= 10 && S(hue, i, j) <= 40 && S(sue, i, j) > 100) {
          S(yellowimg, i, j) = 255;
          yellowcolcount[i]++;
        }

        if (S(blueimg, i, j) != S(blueimg, i - 1, j)) {
          bluerowcount[j]++;
        }

        if (S(yellowimg, i, j) != S(yellowimg, i - 1, j)) {
          yellowrowcount[j]++;
        }

        if (S(binary, i, j) != S(binary, i - 1, j)) {
          binaryrowcount[j]++;
        }
      }

    }
  }

#if 1
  endrow = binary->h - 1;
  startrow = 0;

  for (i = binary->h / 2; i < binary->h; i++) { //找出车牌终止行
    if (binaryrowcount[i] < 10) {
      endrow = i;
      break;
    }
  }

  if (endrow == binary->h / 2) {
    for (i = 0; i < binary->h; i++) { //找出车牌起始行
      if (binaryrowcount[i] > 10) {
        startrow = i;
        break;
      }
    }

    for (i = binary->h - 1; i > 0; i--) { //找出车牌终止行
      if (binaryrowcount[i] > 10) {
        endrow = i;
        break;
      }
    }
  }
  else {
    for (i = binary->h / 2; i > 0; i--) { //找出车牌起始行
      if (binaryrowcount[i] < 10) {
        startrow = i;
        break;
      }
    }
  }


  if (endrow - startrow > 0) {
    img_t im2[1] = {0};
    margin = (endrow - startrow) * 2 / 5;
    bluecount[0] = bluecolcount[0];
    yellowcount[0] = yellowcolcount[0];

    for (i = 1; i < img->w; i++) { //得到垂直投影的累加值
      bluecount[i] = bluecount[i - 1] + bluecolcount[i];
      yellowcount[i] = yellowcount[i - 1] + yellowcolcount[i];
    }

    if (bluecount[img->w - 1] > yellowcount[img->w - 1]) { //蓝色车牌处理
      color = 1;
      {
        int length;
        int startcol = 0;
        int endcol = img->w - 1;

        //if(img->w<=4*img->h)    //如果宽度小于4倍高度
        //{
        for (i = 0; i < img->w; i++) {
          if (bluecolcount[i] != 0) {
            startcol = i;
            break;
          }
        }

        for (i = img->w - 1; i > 0; i--) {
          if (bluecolcount[i] != 0) {
            endcol = i;
            break;
          }
        }

        length = endcol - startcol;

        if (length > 60 && bluecount[endcol] - bluecount[startcol] > 4 * length && length < 4.2 * (endrow - startrow)) {
          CvRect rect;
          rect.h = endrow - startrow + 1;
          rect.w = length + 1 + extend_width;
          rect.x = startcol;
          rect.y = startrow;

          if (rect.w + rect.x >= img_copy->w - 1) {
            rect.w = img_copy->w - rect.x - 1;
          }

          imsetsize(plate, rect.h, rect.w, 3, 1);
          imrefrc(img_copy, rect, im2);
          imclone(im2, plate);
        }
        else { //否则车牌区域过长,需要做一些处理
          int startcol[2] = {0};
          int endcol[2] = {0};
          int index = 0;
          int pre = 0;
          int pixelcount = 0;

          //int region=4.2*(endrow-startrow);
          for (i = 0; i < img->w; i++) {
            if (bluecolcount[i] != 0) {
              if (startcol[index] == 0) {
                startcol[index] = i;
              }

              endcol[index] = i;

              if (i - pre > margin) {
                if (i - startcol[index] > 60 && bluecount[i] - bluecount[startcol[index]] > 4 * (i - startcol[index])) {
                  endcol[index] = pre;
                  index++;

                }

                startcol[index] = 0;
              }

              pre = i;
            }

            if (index == 2) {
              break;
            }
          }

          i = 0;

          if (endcol[0] && endcol[1]) {
            if (bluecount[endcol[0]] - bluecount[startcol[0]] > bluecount[endcol[1]] - bluecount[startcol[1]]) {
              i = 0;
            }
            else {
              i = 1;
            }
          }

          if (endcol[i]) {
            int length = endcol[i] - startcol[i];

            if (length > 60 && bluecount[endcol[i]] - bluecount[startcol[i]] > 4 * length) {

              CvRect rect;
              rect.h = endrow - startrow + 1;
              rect.w = length + 1 + extend_width;
              rect.x = startcol[i];
              rect.y = startrow;

              if (rect.w + rect.x >= img_copy->w - 1) {
                rect.w = img_copy->w - rect.x - 1;
              }

              if (rect.w <= 3 * rect.h) {
                updown m_locate;
                imrefrc(binary, rect, im2);
                m_locate = Locate_up_down(im2);
                rect.y = m_locate.up;
                rect.h = m_locate.down - m_locate.up;
              }

              imsetsize(plate, rect.h, rect.w, 3, 1);
              imrefrc(img_copy, rect, im2);
              imclone(im2, plate);
            }
          }

        }
      }

      //}

    }
    else {     //黄色车牌处理
      color = 0;
      {
        int length;
        int startcol = 0;
        int endcol = img->w - 1;

        //if(img->w<=4*img->h)    //如果宽度小于4倍高度
        //{
        for (i = 0; i < img->w; i++) {
          if (yellowcolcount[i] != 0) {
            startcol = i;
            break;
          }
        }

        for (i = img->w - 1; i > 0; i--) {
          if (yellowcolcount[i] != 0) {
            endcol = i;
            break;
          }
        }

        length = endcol - startcol;

        if (length > 60 && yellowcount[endcol] - yellowcount[startcol] > 4 * length && length < 4 * (endrow - startrow)) {
          CvRect rect;
          rect.h = endrow - startrow + 1;
          rect.w = length + 1 + extend_width;
          rect.x = startcol;
          rect.y = startrow;

          if (rect.w + rect.x >= img_copy->w - 1) {
            rect.w = img_copy->w - rect.x - 1;
          }

          imsetsize(plate, rect.h, rect.w, 3, 1);
          imrefrc(img_copy, rect, im2);
          imclone(im2, plate);
        }
        else { //否则车牌区域过长,需要做一些处理
          int startcol[2] = {0};
          int endcol[2] = {0};
          int index = 0;
          int pre = 0;
          int pixelcount = 0;

          for (i = 0; i < img->w; i++) {
            if (yellowcolcount[i] != 0) {
              if (startcol[index] == 0) {
                startcol[index] = i;
              }

              endcol[index] = i;

              if (i - pre > margin) {
                if (i - startcol[index] > 60 && yellowcount[i] - yellowcount[startcol[index]] > 4 * (i - startcol[index])) {
                  endcol[index] = pre;
                  index++;
                }

                startcol[index] = 0;
              }

              pre = i;
            }

            if (index == 2) {
              break;
            }
          }

          i = 0;

          if (endcol[0] && endcol[1]) {
            if (yellowcount[endcol[0]] - yellowcount[startcol[0]] > yellowcount[endcol[1]] - yellowcount[startcol[1]]) {
              i = 0;
            }
            else {
              i = 1;
            }
          }

          if (endcol[i]) {
            int length = endcol[i] - startcol[i];

            if (length > 60 && yellowcount[endcol[i]] - yellowcount[startcol[i]] > 4 * length) {
              CvRect rect;
              rect.h = endrow - startrow + 1;
              rect.w = length + 1 + extend_width;
              rect.x = startcol[i];
              rect.y = startrow;

              if (rect.w + rect.x >= img_copy->w - 1) {
                rect.w = img_copy->w - rect.x - 1;
              }

              if (rect.w <= 3 * rect.h) {
                updown m_locate;
                imrefrc(binary, rect, im2);
                m_locate = Locate_up_down(im2);
                rect.y = m_locate.up;
                rect.h = m_locate.down - m_locate.up;
              }

              imsetsize(plate, rect.h, rect.w, 3, 1);
              imrefrc(img_copy, rect, im2);
              imclone(im2, plate);
            }
          }

        }
      }
    }


  }

#endif
  //cvSmooth(binary,binary,CV_MEDIAN,3,0,0);//3*3均值滤波
  //cvAnd(binary,yellowimg,yellowimg);
  //cvAnd(binary,blueimg,blueimg);
#if 0

  if (color == 1) {
    plate = blueimg;
  }
  else {
    plate = yellowimg;
  }

#endif
  //plate=yellowimg;
  FREE(yellowrowcount);
  FREE(bluerowcount);
  FREE(yellowcolcount);
  FREE(bluecolcount);
  imfree(blueimg);
  imfree(img);
  imfree(hsv);
  imfree(vue);
  imfree(hue);
  imfree(yellowimg);
  imfree(grayimg);
  imfree(sue);
  imfree(Iue);
  imfree(binary);
  imfree(source);
  imfree(edge);
}

int GetPlate(img_t* img, img_t* plate)
{
  //ColorPrewitt(img);
  //ExcuteLocate(img);
  Precise_Locate_with_color(img, plate);
  return 0;
}
