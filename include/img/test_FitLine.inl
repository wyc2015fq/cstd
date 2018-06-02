
#include "cv\cxjacobieigens.inl"
#include "cv\cvlinefit.inl"

//计算直线拟合度
double myLinearity(DPOINT* pt, int seq_total)
{
  int i;
  double x_bar = 0.0, y_bar = 0.0;
  double u11 = 0.0, u20 = 0.0, u02 = 0.0;
  double linearity = 0.0;

  //x_bar, y_bar
  for (i = 0; i < seq_total; i++) {
    x_bar += pt[i].x;
    y_bar += pt[i].y;
  }

  x_bar /= seq_total;
  y_bar /= seq_total;

  for (i = 0; i < seq_total; i++) {
    u11 += ((pt[i].x - x_bar) * (pt[i].y - y_bar));
    u20 += pow(pt[i].x - x_bar, 2.0f);
    u02 += pow(pt[i].y - y_bar, 2.0f);
  }

  u11 /= seq_total;
  u20 /= seq_total;
  u02 /= seq_total;

  //计算直线度
  linearity = sqrt(4 * pow(u11, 2.0f) + pow(u20 - u02, 2.0f)) / (u20 + u02);
  return linearity;
}

int test_fitline()
{
  int i;
  double fx[] = {0.0, 0.301, 0.477, 0.602, 0.699, 0.778, 0.845, 0.903, 0.954, 1.0};
  double fy[] = {3.874, 3.202, 2.781, 2.49, 2.274, 2.156, 1.934, 1.74, 1.653, 1.662};
  double line[4];
  double linearity = 0.0f;
  //三维空间的话，把CC_32FC2改为CC_32FC3、 把Point2d 改为 CvPoint3D32f
  DPOINT point_seq[100];

  for (i = 0; i < countof(fx); i++) {
    //向Sequence中追加元素
    point_seq[i] = dPOINT(fx[i], fy[i]);
  }

  linearity = myLinearity(point_seq, countof(point_seq));
  cvFitLine(&point_seq->x, countof(fx), 2, CC_DIST_L2, 0, 0.01, 0.01, line);
  printf("v=(%f,%f),vy/vx=%f\n(x,y)=(%f,%f)\nLinearity=%f\n", line[0], line[1], line[1] / line[0], line[2], line[3], linearity);
  return 0;
}
