
#include "img\color.inl"
#include "img\hough.inl"
#include "cv\cvhough.inl"
int test_hough()
{
  img_t src[1] = {0};
  img_t sss[1] = {0};
  img_t dst[1] = {0};
  img_t color_dst[1] = {0};
  double lines[100];
  int i;
  int lines_total;
  const char* fn;
  int buf[1000];
  _chdir("D:/pub/pic");
  _chdir("D:/pub/bin/iris/test1pic");
  _chdir("D:/pub/bin/imgproc/pic");
  fn = "building.jpg";
  fn = "lena512.jpg";
  fn = "ttt.bmp";
  fn = "stuff.jpg";
  imread(fn, 1, 1, src);
  hough_circle(src, lines, 100);
  cvCanny(src, dst, 50, 200, 3);

  imsetsamesize(sss, src);
  //{utime_start(_start_time);
  GAUSS7(src->h, src->w, src->data, src->s, 1, sss->data, sss->s, 1, buf);
  cvCanny(sss, dst, 50, 200, 3);
  //printf("  %.4f", utime_elapsed(_start_time));}
  //imshow(dst);cvWaitKey(-1);
  //imfree(src);
  //imfree(dst);
  imcolorcvt(dst, color_dst, T_GRAY, T_RGB);

  if (0) {
    // 标准HOUGH变换
    //{utime_start(_start_time);
    lines_total = cvHoughLines2(dst, lines, 100, CC_HOUGH_STANDARD, 1, CC_PI / 180, 150, 0, 0);
    //printf("  %.4f\n", utime_elapsed(_start_time));}

    for (i = 0; i < lines_total; i++) {
      double* line = lines + i * 2;
      double rho = line[0];
      double theta = line[1];
      IPOINT pt1, pt2;
      double a = cos(theta), b = sin(theta);

      if (fabs(a) < 0.001) {
        pt1.x = pt2.x = cvRound(rho);
        pt1.y = 0;
        pt2.y = color_dst->h;
      }
      else if (fabs(b) < 0.001) {
        pt1.y = pt2.y = cvRound(rho);
        pt1.x = 0;
        pt2.x = color_dst->w;
      }
      else {
        pt1.x = 0;
        pt1.y = cvRound(rho / b);
        pt2.x = cvRound(rho / a);
        pt2.y = 0;
      }

      DrawLine(color_dst->h, color_dst->w, color_dst->data, color_dst->s, color_dst->c,
          pt1, pt2, CC_RGB(255, 0, 0), 1, 8);
    }
  }
  else {
    // 概率HOUGH变换
    lines_total = cvHoughLines2(dst, lines, 100, CC_HOUGH_PROBABILISTIC, 1, CC_PI / 180, 80, 30, 10);

    for (i = 0; i < lines_total; i++) {
      DPOINT* line = (DPOINT*)(lines + i * 4);
      IPOINT pt1 = iPOINT((int)line[0].x, (int)line[0].y), pt2 = iPOINT((int)line[1].x, (int)line[1].y);
      DrawLine(color_dst->h, color_dst->w, color_dst->data, color_dst->s, color_dst->c,
          pt1, pt2, CC_RGB(255, 0, 0), 1, 8);
    }
  }

  {
    double dp = 2;
    double min_dist = 10;
    int min_radius = 5;
    int max_radius = 150;
    //only greyimage is needed. cvHoughCircles would call cvSobel() internally.
    int circles_total;
    {
      utime_start(_start_time);
      circles_total = cvHoughCircles(sss, lines, 20, CC_HOUGH_GRADIENT,
          dp, min_dist, 100, 50, min_radius, max_radius);
      printf("  %.4f", utime_elapsed(_start_time));
    }

    //draw found curves
    for (i = 0; i < circles_total; i++) {
      double* p = lines + i * 3;
      DrawCircle(color_dst->h, color_dst->w, color_dst->data, color_dst->s, color_dst->c,
          cvRound(p[0]), cvRound(p[1]), cvRound(p[2]), CC_RGB(255, 0, 0), 1, 8);
    }
  }

  printf("\n");
  imshow(src);
  imshow(color_dst);

  cvWaitKey(-1);
  return 0;
}


