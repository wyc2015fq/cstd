

int plate_detect(const img_t* img, CvRect* m_rect, int max_rect)
{
  int i, ncnt = 0, j = 0;
  img_t grayimg[1] = {0};
  img_t rect_img[1] = {0};
  int h = img->h, w = img->w;
  imsetsize(grayimg, h, w, 1, 1);
  imsetsize(rect_img, h, w, 1, 1);
  imzero(rect_img);

  //cvCvtColor(img, grayimg, CV_BGR2GRAY);
  imcolorcvt(img, grayimg, T_BGR, T_GRAY);

  // 粗定位
  {
    runpass_t run[1] = {0};
    img_t binary[1] = {0};
    img_t source[1] = {0};
    img_t copybinary[1] = {0};
    img_t binaryedge[1] = {0};
    imsetsize(binary, h, w, 1, 1);
    imsetsize(binaryedge, h, w, 1, 1);
    imsetsize(source, h, w, 1, 1);
    imsetsize(copybinary, h, w, 1, 1);
    //cvSmooth(grayimg, grayimg, CV_GAUSSIAN, 3, 0, 0); //高斯滤波
    //cvEqualizeHist(grayimg,grayimg);
    abssobel_v(grayimg, source);
    otsuThreshold(source, binary, 0);
    imcopy(binary, copybinary);
    {
      img_t im1[1] = {0};
      img_t im2[1] = {0};
      CvRect rect1;
      rect1 = cvRect(0, 0, img->w - 1, img->h);
      imrefrc(binary, rect1, im1);
      rect1 = cvRect(1, 0, img->w - 1, img->h);
      imrefrc(binary, rect1, im2);
      cvXor(im1, im2, binaryedge);
    }

    for (i = 20; i <= 45; i = i + 5) {
      OnePass(run, binaryedge, rect_img, i);  //找出连通区域
    }

    //imshow(grayimg);  imshow(source); imshow(img);   imshow(binaryedge);  cvWaitKey(-1);
    //imshow(binaryedge); imshow(rect_img);  cvWaitKey(-1);
    imfree(binaryedge);
    imfree(binary);
    imfree(source);
    imfree(copybinary);
  }

  {
    int count = 0;
    int number = 0;
    int max_value[20] = {0};
    int index = 0;

    img_t copy_rect_img[1] = {0};
    imsetsize(copy_rect_img, h, w, 1, 1);
    colfusionrect(rect_img);   //按列合并矩形框
    rowfusionrect(rect_img);   //按行合并矩形框
    imcopy(rect_img, copy_rect_img);
    cvThreshold(copy_rect_img, copy_rect_img, 50, 255, CV_THRESH_BINARY);

    ncnt = juxingkuang2(IMARG(copy_rect_img), m_rect, max_rect);
    imfree(copy_rect_img);


    for (j = 0; j < ncnt && index < max_rect; ++j) {
      //int m_rect_num=0;
      CvRect r = m_rect[j];
      CvRect m_rect2[10] = {0};
      //draw_rect_to_img(r);
      int m_rect_num = preciseprocess(rect_img, r, m_rect2);

      if (m_rect_num) {
        for (i = 0; i < m_rect_num; i++) {
          number = rect_remove_adaboost(grayimg, m_rect2[i]);

          if (number) {
            count++;
            max_value[index] = number;
            m_rect[index] = m_rect2[i];
            index++;
            //draw_rect_to_img(frame, m_rect[i]);
          }
        }
      }
      else {
        number = rect_remove_adaboost(grayimg, r);

        if (number) {
          count++;
          max_value[index] = number;
          m_rect[index] = r;
          index++;
          //draw_rect_to_img(frame, r);
        }
      }
    }

    ncnt = index;
    //imshow(frame);cvWaitKey(-1);
  }

  imfree(rect_img);
  imfree(grayimg);

  return ncnt;
}
