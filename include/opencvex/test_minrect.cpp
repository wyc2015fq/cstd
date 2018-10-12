
#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int test_minrect()
{
  Mat srcImage(Size(600, 600), CV_8UC3, Scalar(0));

  RNG &rng = theRNG();

  char key;
  while (1)
  {
    //随机生成一些点
    //首先就是随机生成点的总数量
    int g_nPointCount = rng.uniform(3, 200);
    //接下来就是随机生成一些点的坐标
    vector<Point> points;
    for (int i = 0; i < g_nPointCount; i++)
    {
      Point midPoint;

      midPoint.x = rng.uniform(srcImage.cols / 4, srcImage.cols * 3 / 4);
      midPoint.y = rng.uniform(srcImage.rows / 4, srcImage.rows * 3 / 4);

      points.push_back(midPoint);
    }

    //显示刚刚随机生成的那些点
    for (int i = 0; i < g_nPointCount; i++)
    {
      circle(srcImage, points[i], 0, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 3);
    }

    //在生成的那些随机点中寻找最小包围矩形
    //rectPoint变量中得到了矩形的四个顶点坐标
    RotatedRect rectPoint = minAreaRect(points);
    //定义一个存储以上四个点的坐标的变量
    Point2f fourPoint2f[4];
    //将rectPoint变量中存储的坐标值放到 fourPoint的数组中
    rectPoint.points(fourPoint2f);

    //根据得到的四个点的坐标  绘制矩形
    for (int i = 0; i < 3; i++)
    {
      line(srcImage, fourPoint2f[i], fourPoint2f[i + 1]
        , Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 3);
    }
    line(srcImage, fourPoint2f[0], fourPoint2f[3]
      , Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 3);

    imshow("【绘制结束后的图像】", srcImage);

    key = waitKey();
    if (key == 27)
      break;
    else
      srcImage = Scalar::all(0);
  }

  return 0;
}


