
// 检测内圆
int pp(buf_t* bf, const img_t* I, int* ci)
{
  int i = 0, row = 0, col = 0, Rmax_p = 0, n = 0;
  img_t polarv[1] = {0};
  IPOINT pt;
  IPOINT* pts = 0;
  int buflen = bf->len;
  //imshow(I);
  //imwrite2("ttt.bmp", I2);
  row = I->h, col = I->w;
  // 得到pupil的大致中心点
  //{utime_start(_start_time);
  pt = rough_center_7_20(bf, I);
#if 1
  //printf(" %.4f", utime_elapsed(_start_time));}
  // 半径最大值
  Rmax_p = MIN((row - pt.y), (col - pt.x));
  // 4_8 转化为极坐标下的图像
  //{utime_start(_start_time);
  pupil_to_polar_coordinate(bf, I, pt.x, pt.y, Rmax_p, polarv);
  //printf(" %.4f", utime_elapsed(_start_time));}
  //pashan(polarv);
  // 4_8 边缘检测，每行最多对应一个边缘点, 并且最后得到内边缘点及其在极坐标下的位置
  //{utime_start(_start_time);
  BFMALLOC(bf, pts, 200);

  n = edge_detection(bf, polarv, Rmax_p, pts);
  //ci[0] = n;  ci[1] = im2avg_u8(polarv);  BFFREE(bf, pts, 200);  bf_imfree(bf, polarv);  return 0;
  //printf(" %.4f", utime_elapsed(_start_time));}
  //imdraw_ipoints(polarv, pts, n, 8, CV_RGB(255, 255, 255)); imshow(polarv); cvWaitKey(-1);
  {
    int sumx, x0 = pt.x << 8, y0 = pt.y << 8;

    for (i = 0; i < n; ++i) {
      int thea = 0;
      int px = pts[i].x, py = pts[i].y;
      sumx += py;
      ASSERT(py < 360);
      thea = BOUND(py, 0, 359);
      pts[i].x = x0 + ((px * sin360[thea]) >> 8);
      pts[i].y = y0 + ((px * cos360[thea]) >> 8);
    }

    // 胡克定律
    Hooke_law_total1(bf, x0, y0, pts, n, 100, 5, ci);
  }
#endif
  //imdraw_ipoints(I, pts, n, 8, CV_RGB(255, 255, 255)); imshow(I); cvWaitKey(-1);
  //imdraw_circle(I, row/2, col/2, 1, 0, CV_RGB(255, 255, 255), 1); imdraw_circle(I, ci[0], ci[1], ci[2], 0, CV_RGB(255, 255, 255), 1);imshow(I); cvWaitKey(-1);
  //imshow(polarv); imshow(I); cvWaitKey(-1);

  BFFREE(bf, pts, 200);
  bf_imfree(bf, polarv);
  ASSERT(buflen == bf->len);
  return 0;
}
