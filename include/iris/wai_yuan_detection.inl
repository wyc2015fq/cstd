
// 虹膜外圆定位
int wai_yuan_detection(buf_t* bf, int pupil_x, int pupil_y, int pupil_radius, const img_t* I_source, int* wi)
{
  img_t polarv[1] = {0};
  int i, n = 0, x0, y0, pic_xsize, pic_ysize, Rmax;
  IPOINT* p_sample = 0;
  int p_samplemaxlen = 200;
  int buflen = bf->len;
  // 极坐标原点，图像的中心点
  x0 = pupil_x;
  y0 = pupil_y;
  pic_xsize = I_source->h, pic_ysize = I_source->w;
  Rmax = MIN(MAX((pic_xsize - x0), (pic_ysize - y0)), MAX(x0, y0)) + 10;

  // 专门的外圆极坐标变换
  //{utime_start(_start_time);
  iris_to_polar_coordinate(bf, I_source, y0, x0, Rmax, pic_xsize, pic_ysize, polarv);
  //printf(" %.4f", utime_elapsed(_start_time));}
  BFMALLOC(bf, p_sample, p_samplemaxlen);
  memset(p_sample, 0, sizeof(p_sample));
  // 转化为边缘图像
  //{utime_start(_start_time);
  n = edge_detection_waiyuan(bf, polarv, pupil_radius, p_sample, p_samplemaxlen);
  //printf(" %.4f", utime_elapsed(_start_time));}

#if 1
  //imshow(I_source); imshow(polarv);  cvWaitKey(-1);
  //polarv = uint8(polarv);

  ASSERT(n < p_samplemaxlen);
  {
    IPOINT* p = p_sample;
    int x00 = x0 << 8, y00 = y0 << 8;

    //int sumx=0;
    for (i = 0; i < n; ++i) {
      int thea = 0;
      int px = p[i].x, py = p[i].y;
      //sumx += px;

      if (0 < px && py < 24) {
        thea = (py) * 2;
      }
      else if (23 < py && py < 201) {
        thea = 45 + (py - 24) / 2;
      }
      else if (200 < py && py < 247) {
        thea = 135 + (py - 201) * 2;
      }
      else if (246 < py && py < 424) {
        thea = 226 + (py - 247) / 2;
      }
      else if (423 < py && py < 447) {
        thea = 315 + (py - 424) * 2;
      }

      thea = BOUND(thea, 0, 359);
      ASSERT(thea < 360);
      p[i].x = x00 + ((px * sin360[thea]) >> 8);
      p[i].y = y00 + ((px * cos360[thea]) >> 8);
    }

    //胡克定律
    Hooke_law_total1(bf, x00, y00, p_sample, n, 100, 10, wi);
  }

#endif
  BFFREE(bf, p_sample, p_samplemaxlen);
  bf_imfree(bf, polarv);
  //imdrawcircle(I_source, wi[0], wi[1], wi[2], CV_RGB(255, 255, 255), 1, 8, 0);
  //imshow(I_source);  cvWaitKey(-1);
  ASSERT(buflen == bf->len);
  return 0;
}
int get_y(int ROI_col, double* y, int* z)
{
  int ii;

  for (ii = 0; ii < ROI_col; ++ii) {
    double x = ii + 1;
    double zz = y[0] * (x * x) + y[1] * x + y[2];
    z[ii] = (int)zz;
  }

  return 0;
}

