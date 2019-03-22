
#ifndef _OCR_UTILS_H_
#define _OCR_UTILS_H_


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/ml/ml.hpp"

#include <string>
using namespace std;
using namespace cv;

int LoadTextFileList(const string& testfile, std::vector<string>& imgs) {
  FILE* input = NULL;
  input = fopen(testfile.c_str(), "rb");
  if (input) {
    char buf[1024];
    char fname[256];
    for (; fgets(buf, 1024, input) > 0; ) {
      sscanf(buf, "%s", fname);
      imgs.push_back(fname);
    }
    fclose(input);
  }
  return (int)imgs.size();
}


#define V_PROJECT 'V'
#define H_PROJECT 'H'


int GetTextProjection(const Mat &src, vector<int>& pos, int mode)
{
  mode = toupper(mode);
  if (mode == V_PROJECT)
  {
    pos.resize(src.cols);
    for (int j = 0; j < src.cols; j++) {
      pos[j] = 0;
    }
    for (int i = 0; i < src.rows; i++)
    {
      const uchar* p = src.ptr<const uchar>(i);
      for (int j = 0; j < src.cols; j++)
      {
        if (p[j]>128)
        {
          pos[j]++;
        }
      }
    }

    //show_projection(pos, V_PROJECT);
  }
  else if (mode == H_PROJECT)
  {
    pos.resize(src.rows);
    for (int j = 0; j < src.rows; j++) {
      pos[j] = 0;
    }
    for (int i = 0; i < src.cols; i++)
    {

      for (int j = 0; j < src.rows; j++)
      {
        if (src.at<uchar>(j, i))
        {
          pos[j]++;
        }
      }
    }
    //show_projection(pos, H_PROJECT);

  }

  return 0;
}

void show_projection(vector<int>& pos, int mode)
{
  vector<int>::iterator max = std::max_element(std::begin(pos), std::end(pos)); //求最大值
  if (mode == H_PROJECT)
  {
    int height = (int)pos.size();
    int width = *max;
    Mat project = Mat::zeros(height, width, CV_8UC1);
    for (int i = 0; i < project.rows; i++)
    {
      for (int j = 0; j < pos[i]; j++)
      {
        project.at<uchar>(i, j) = 255;
      }
    }
    cvNamedWindow("horizational projection", 0);
    imshow("horizational projection", project);

  }
  else if (mode == V_PROJECT)
  {
    int height = *max;
    int width = (int)pos.size();
    Mat project = Mat::zeros(height, width, CV_8UC1);
    for (int i = 0; i < project.cols; i++)
    {
      for (int j = project.rows - 1; j >= project.rows - pos[i]; j--)
      {
        //std::cout << "j:" << j << "i:" << i << std::endl;
        project.at<uchar>(j, i) = 255;
      }
    }

    imshow("vertical projection", project);
  }

  //waitKey();
}


Mat getRplane(const Mat &in)
{
  vector<Mat> splitBGR(in.channels()); //容器大小为通道数3
  split(in, splitBGR);
  //return splitBGR[2];  //R分量

  return splitBGR[2];

}

Mat getRplane2(const Mat &in)
{
  Mat gray;
  Mat color_boost;
  cv::decolor(in, gray, color_boost);
  //return splitBGR[2];  //R分量
  imshow("gray", gray);
  imshow("color_boost", color_boost);
  cv::waitKey(-1);

  if (in.cols > 700 || in.cols >600)
  {
    Mat resizeR(450, 600, CV_8UC1);
    cv::resize(gray, resizeR, resizeR.size());

    return resizeR;
  }
  else
    return gray;
}

int SmoothPos121(int n, int* pos) {
  if (n > 3) {
    int pre = pos[0];
    int t0 = (pos[0] * 2 + pos[1]) / 3;
    int tn = (pos[n - 1] * 2 + pos[n - 2]) / 3;
    for (int i = 1; i < n - 1; ++i) {
      int t = pre + pos[i] + pos[i] + pos[i + 1];
      pre = pos[i];
      pos[i] = t >> 2;
    }
    pos[0] = t0;
    pos[n - 1] = tn;
  }
  return 0;
}

int find_minwidth(int counter, Range* x_char, int minthd, int maxthd) {
  int mind = 10000000;
  int ret = -1;
  for (int i = 0; i < counter; i++) {
    if (x_char[i].size() < minthd) {
      int d1 = 10000, d2 = 10000;
      if (i > 0) {
        d1 = x_char[i].end - x_char[i - 1].start;
      }
      if (i<(counter - 1)) {
        d2 = x_char[i + 1].end - x_char[i].start;
      }
      if (d1 < maxthd || d2 < maxthd) {
        mind = min(d1, d2);
        if (d1 < d2) {
          ret = i;
        }
        else {
          ret = i + 1;
        }
      }
    }
  }
  return ret;
}

#define BOUND(x, a, b)   ((x)<(a) ? (a) : (x)>(b) ? (b) : (x))

int range_minmax(int n, const int* pos, int b, int e, int& minv, int& maxv) {
  b = BOUND(b, 0, n);
  e = BOUND(e, 0, n);
  if (b < e) {
    minv = maxv = pos[b];
    for (int i = b; i < e; ++i) {
      if (pos[i] < minv) {
        minv = pos[i];
      }
      else if (pos[i] > maxv) {
        maxv = pos[i];
      }
    }
  }
  return 0;
}

int loc_minmax(int n, const int* pos, int b, int e, std::vector<int>& minvec, std::vector<int>& maxvec) {
  for (int i = 0; i < n; ++i) {
    int minv = 0, maxv = 0;
    range_minmax(n, pos, i + b, i + e, minv, maxv);
    minvec.push_back(minv);
    maxvec.push_back(maxv);
  }
  return 0;
}


using namespace cv;
using namespace std;


void OstuBeresenThreshold(const Mat &in, Mat &out) //输入为单通道
{
  double ostu_T = threshold(in, out, 0, 255, CV_THRESH_OTSU); //otsu获得全局阈值

  double min;
  double max;
  minMaxIdx(in, &min, &max);
  const double CI = 0.12;
  double beta = CI*(max - min + 1) / 128;
  double beta_lowT = (1 - beta)*ostu_T;
  double beta_highT = (1 + beta)*ostu_T;

  Mat doubleMatIn;
  in.copyTo(doubleMatIn);
  int rows = doubleMatIn.rows;
  int cols = doubleMatIn.cols;
  double Tbn;
  for (int i = 0; i < rows; ++i)
  {
    //获取第 i行首像素指针
    uchar * p = doubleMatIn.ptr<uchar>(i);
    uchar *outPtr = out.ptr<uchar>(i);

    //对第i 行的每个像素(byte)操作
    for (int j = 0; j < cols; ++j)
    {

      if (i <2 || i>rows - 3 || j<2 || j>cols - 3)
      {

        if (p[j] <= beta_lowT)
          outPtr[j] = 0;
        else
          outPtr[j] = 255;
      }
      else
      {
        Rect rc = Rect(j - 2, i - 2, 5, 5);
        Tbn = sum(doubleMatIn(rc))[0] / 25;  //窗口大小25*25
        if (p[j] < beta_lowT || (p[j] < Tbn && (beta_lowT <= p[j] && p[j] >= beta_highT)))
          outPtr[j] = 0;
        if (p[j] > beta_highT || (p[j] >= Tbn && (beta_lowT <= p[j] && p[j] >= beta_highT)))
          outPtr[j] = 255;
      }
    }
  }

}

Rect UnionRect(Rect r1, Rect r2) {
  int l = min(r1.x, r2.x);
  int r = max(r1.x + r1.width, r2.x + r2.width);
  int t = min(r1.y, r2.y);
  int b = max(r1.y + r1.height, r2.y + r2.height);
  return cv::Rect(l, t, r - l, b - t);
}

Rect InterRect(Rect r1, Rect r2) {
  int l = max(r1.x, r2.x);
  int r = min(r1.x + r1.width, r2.x + r2.width);
  int t = max(r1.y, r2.y);
  int b = min(r1.y + r1.height, r2.y + r2.height);
  return cv::Rect(l, t, r - l, b - t);
}

bool isEligible(const RotatedRect &candidate)
{
  double error = 0.2;
  const double aspect = 4.5 / 0.3; //长宽比
  double min = 10 * aspect * 10; //最小区域
  double max = 50 * aspect * 50;  //最大区域
  double rmin = aspect - aspect*error; //考虑误差后的最小长宽比
  double rmax = aspect + aspect*error; //考虑误差后的最大长宽比

  double area = candidate.size.height * candidate.size.width;
  double r = (double)candidate.size.width / (double)candidate.size.height;
  if (r <1)
    r = 1 / r;

  if ((area < min || area > max) || (r< rmin || r > rmax)) //满足该条件才认为该candidate为车牌区域
    return false;
  else
    return true;
}

bool isEligible2(const RotatedRect &candidate, int imgheight)
{
  double error = 0.2;
  const double aspect = 4.5 / 0.3; //长宽比
  double min = 10 * aspect * 10; //最小区域
  double max = 50 * aspect * 50;  //最大区域
  double rmin = aspect - aspect*error; //考虑误差后的最小长宽比
  double rmax = aspect + aspect*error; //考虑误差后的最大长宽比
  double hmin = 1 + imgheight / 80;
  double hmax = 1 + imgheight / 10;
  Rect rect = candidate.boundingRect();
  double h = rect.height;
  double w = rect.width;

  double area = h * w;
  double r = (w + 0.1) / (h + 0.1);
  //if (r <1)    r = 1 / r;

  if ((h< hmin || h>hmax) || (area < 100 || area > 10000) || (r< 0.2 || r > rmax)) //满足该条件才认为该candidate为车牌区域
    return false;
  else
    return true;
}

void DrawRotatedRect(Mat& out, RotatedRect& rrect) {
  Point2f vertices[4];
  rrect.points(vertices);
  for (int i = 0; i < 4; i++)
    line(out, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255));//画黑色线条
}

bool rect_less(const RotatedRect &r1, const RotatedRect &r2) {
  return r1.center.y < r2.center.y || ((r1.center.y == r2.center.y) && (r1.center.x < r2.center.x));
}

bool rect_less_row(const Rect& r1, const Rect& r2) {
  return (r1.x < r2.x);
}

bool rect_less_line(const std::vector<Rect>& r1, const std::vector<Rect>& r2) {
  return (r1[0].y < r2[0].y);
}

struct SimilarLine {
  double eps;
  SimilarLine(double eps_ = 0.5) {
    this->eps = eps_;
  }
  inline bool operator()(const cv::Rect& r1, const cv::Rect& r2) const
  {
    // delta为最小长宽的eps倍
    double delta = eps*(MIN(r1.height, r2.height))*0.5;
    // 如果矩形的四个顶点的位置差别都小于delta，则表示相似的矩形
    int t = MAX(r1.y, r2.y);
    int b = MIN(r1.y + r1.height, r2.y + r2.height);
    int d = b - t;
    return d>delta;
  }
};


int partitionLine(const std::vector<Rect>& bboxes1, std::vector<std::vector<Rect>>& bboxesvec) {
  std::vector<cv::Rect> lines;
  std::vector<int> labels;
  std::vector<int> cnt;
  if (bboxes1.size() < 1) { return 0; }
  int nclasses = cv::partition(bboxes1, labels, SimilarLine(0.3));
  bboxesvec.resize(nclasses);
  for (int i = 0; i < bboxes1.size(); ++i) {
    int j = labels[i];
    bboxesvec[j].push_back(bboxes1[i]);
  }
  for (int i = 0; i < nclasses; ++i) {
    std::sort(bboxesvec[i].begin(), bboxesvec[i].end(), rect_less_row);
  }
  std::sort(bboxesvec.begin(), bboxesvec.end(), rect_less_line);
  return 0;
}


#endif // _OCR_UTILS_H_
