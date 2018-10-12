
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void imCopyTo(const Mat& src, Mat& dst, Rect rect, bool keepRate) {
  int sh = src.rows;
  int sw = src.cols;
  int h = rect.height;
  int w = rect.width;
  if (sh*w*sh*w == 0) return;
  if (sh*w > sw*h) {
    rect.width = sw*h / sh;
    rect.x += (w- rect.width)/2;
  }
  else {
    rect.height = sh*w / sw;
    rect.y += (h - rect.height) / 2;
  }
  if (src.rows!=rect.height || src.cols!=rect.width) {
    Mat s2;
    s2.create(rect.height, rect.width, src.type());
    cv::resize(src, s2, Size(rect.width, rect.height), 0,0,cv::INTER_NEAREST);
    s2.copyTo(dst(rect));
  }
  else {
    src.copyTo(dst(rect));
  }
  return;
}

static Mat mergeImgs(int rc, int h, int w, const Mat** src, int n)
{
  Mat dst;
  int r = rc / 10, c = rc % 10;
  if (rc < 1) {
    int k = (int)sqrt(n);
    if (k*k < n) ++k;
    c = r = k;
  }
  const Mat* src1 = src[0];
  int gap = 5, minhw = 60;
  if (r < 1) {
    r = (n + c - 1) / c;
  }
  if (c < 1) {
    c = (n + r - 1) / r;
  }
  n = min(n, r*c);
  //if (h < src1->rows && w < src1->cols) {    h = src1->rows;    w = src1->cols;  }
  if (h < 10) {
    h = src1->rows*w/ src1->cols;
  }
  else if (w < 10) {
    w = src1->cols*h / src1->rows;
  }
  int rows = gap * (r - 1) + r*h;
  int cols = gap * (c - 1) + c*w;
  bool keep = true;
  dst.create(rows, cols, CV_8UC3);
  for (int i=0; i<n; ++i) {
    int pr = i / c;
    int pc = i % c;
    Rect rect(pc*(w + gap), pr*(h + gap), w, h);
    if (src[i]->channels() == 1) {
      Mat s2;
      cvtColor(*src[i], s2, CV_GRAY2BGR);
      imCopyTo(s2, dst, rect, keep);
    } else {
      imCopyTo(*src[i], dst, rect, keep);
    }
  }
  return dst;
}
static Mat mergeImgs(int rc, int h, int w, const vector<Mat>& src) {
  vector<Mat const *> v;
  for (int i = 0; i < src.size(); ++i) {
    v.push_back(&src[i]);
  }
  return mergeImgs(rc, h, w, &v[0], v.size());
}

static Mat mergeImgs(int rc, int h, int w, const Mat &src1, const Mat &src2) {
  const Mat* v[] = { &src1, &src2};
  return mergeImgs(rc, h, w, v, 2);
}
static Mat mergeImgs(int rc, int h, int w, const Mat &src1, const Mat &src2, const Mat &src3) {
  const Mat* v[] = { &src1, &src2, &src3 };
  return mergeImgs(rc, h, w, v, 3);
}
static Mat mergeImgs(int rc, int h, int w, const Mat &src1, const Mat &src2, const Mat &src3, const Mat &src4) {
  const Mat* v[] = { &src1, &src2, &src3, &src4 };
  return mergeImgs(rc, h, w, v, 4);
}
static Mat mergeImgs(const Mat &src1, const Mat &src2)
{
  Mat dst;
  int rows = src1.rows + 5 + src2.rows;
  int cols = src1.cols + 5 + src2.cols;
  CV_Assert(src1.type() == src2.type());
  dst.create(rows, cols, src1.type());
  src1.copyTo(dst(Rect(0, 0, src1.cols, src1.rows)));
  src2.copyTo(dst(Rect(src1.cols + 5, 0, src2.cols, src2.rows)));
  return dst;
}

int test_mergeImgs(int argc, char *argv[])
{
  Mat src1 = imread("/home/ct/Pictures/cat.jpg");
  Mat src2 = imread("/home/ct/Pictures/cat.jpg");
  Mat outImg = mergeImgs(src1, src2);
  imshow("img", outImg);
  waitKey();
  return 0;
}

bool ptInRect(int x, int y, Rect rect) {
  return (rect.x < x && x < rect.x + rect.width && rect.y < y && y < rect.y + rect.height);
}

double ptDot(Point p1, Point p2) {
  return p1.x*p2.x + p1.y*p2.y;
}

double pt2Line(Point p1, Point lp1, Point lp2)
{
  // 化简两点式为一般式
  // 两点式公式为(y - y1)/(x - x1) = (y2 - y1)/ (x2 - x1)
  // 化简为一般式为(y2 - y1)x + (x1 - x2)y + (x2y1 - x1y2) = 0
  // A = y2 - y1
  // B = x1 - x2
  // C = x2y1 - x1y2
  double a = lp2.y - lp1.y;
  double b = lp1.x - lp2.x;
  double c = lp2.x * lp1.y - lp1.x * lp2.y;
  double d = a * p1.x + b * p1.y + c;
  // 距离公式为d = |A*x0 + B*y0 + C|/√(A^2 + B^2)
  double dis = fabs(d) / sqrt(a * a + b * b);
  //double dis2 = (d*d) / (a * a + b * b);
  return dis;
}
double pt2Line(Point p1,Vec4f l) {
  return pt2Line(p1, Point(l[0], l[1]), Point(l[2], l[3]));
}

void drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd, int len, int alpha,
  cv::Scalar& color, int thickness = 1, int lineType = 8)
{
  Point arrow;
  //计算 θ 角（最简单的一种情况在下面图示中已经展示，关键在于 atan2 函数，详情见下面）   
  double angle = atan2((double)(pStart.y - pEnd.y), (double)(pStart.x - pEnd.x));
  line(img, pStart, pEnd, color, thickness, lineType);
  //计算箭角边的另一端的端点位置（上面的还是下面的要看箭头的指向，也就是pStart和pEnd的位置） 
  arrow.x = pEnd.x + len * cos(angle + CV_PI * alpha / 180);
  arrow.y = pEnd.y + len * sin(angle + CV_PI * alpha / 180);
  line(img, pEnd, arrow, color, thickness, lineType);
  arrow.x = pEnd.x + len * cos(angle - CV_PI * alpha / 180);
  arrow.y = pEnd.y + len * sin(angle - CV_PI * alpha / 180);
  line(img, pEnd, arrow, color, thickness, lineType);
}

Vec4f getLine(Point2f pt, double angle, double len) {
  double a = angle * CV_PI / 180;
  Point2f end(pt.x + len*cos(a), pt.y + len*sin(a));
  Vec4f l;
  l[0] = pt.x;
  l[1] = pt.y;
  l[2] = end.x;
  l[3] = end.y;
  return l;
}

void drawRotatedRect(Mat& srcImage, RotatedRect rectPoint, Scalar color, int lw) {
  //定义一个存储以上四个点的坐标的变量
  Point2f fourPoint2f[4];
  //将rectPoint变量中存储的坐标值放到 fourPoint的数组中
  rectPoint.points(fourPoint2f);

  //根据得到的四个点的坐标  绘制矩形
  for (int i = 0; i < 3; i++)
  {
    line(srcImage, fourPoint2f[i], fourPoint2f[i + 1], color, lw);
  }
  line(srcImage, fourPoint2f[0], fourPoint2f[3], color, lw);
  double len = 0.5*max(rectPoint.size.height, rectPoint.size.width);
  double a = rectPoint.angle * CV_PI / 180;
  cv::Point end(rectPoint.center.x + len*cos(a), rectPoint.center.y + len*sin(a));
  double len2 = len *0.25;
  drawArrow(srcImage, rectPoint.center, end, len2, 15, color, 1, 4);
}


double line_len(const Vec4f& l) {
  double x = (l[2] - l[0]);
  double y = (l[3] - l[1]);
  return sqrt(x*x + y*y);
}
double ptdist(Point p1, Point p2) {
  double x = p2.x - p1.x;
  double y = p2.y - p1.y;
  return sqrt(x*x + y*y);
}

void rect2pt4(Rect r, Point* p) {
  int w = r.width, h = r.height;
  *p++ = Point(r.x, r.y);
  *p++ = Point(r.x + w, r.y);
  *p++ = Point(r.x + w, r.y + h);
  *p++ = Point(r.x, r.y + h);
}
Point rect_center(Rect r) {
  return Point(r.x+r.width/2, r.y+r.height/2);
}

double rcdist(Rect r1, Rect r2) {
  Rect rect = r1 & r2;
  double mindis = 99999;
  if (rect.width*rect.height > 0) {
    return mindis;
  }
  Point p1[4];
  Point p2[4];
  rect2pt4(r1, p1);
  rect2pt4(r2, p2);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      double dis = ptdist(p1[i], p2[j]);
      if (dis < mindis) {
        mindis = dis;
      }
    }
  }
  return (mindis);
}

int fitLines(vector<Point>& pts, vector<Vec4i>& lines, double thd) {
  int i, j;
  lines.clear();
  vector<char> flag(pts.size(), 0);
  for (i = 0; i < (int)pts.size(); ++i) {
    if (!flag[i]) {
      //flag[i] = 1;
      int id = -1;
      double mindis = 999999;
      for (j = 0; j < pts.size(); ++j) {
        if (!flag[j]) {
          double dis = ptdist(pts[i], pts[j]);
          if (dis < mindis) {
            mindis = dis;
            id = j;
          }
        }
      }
      if (id >= 0 && mindis < thd) {
        //flag[id] = 1;
        Vec4i v(pts[i].x, pts[i].y, pts[id].x, pts[id].y);
        lines.push_back(v);
      }
    }
  }
  return 0;
}
int fitLines(vector<Rect>& rcs, vector<Vec4i>& lines, double thd) {
  int i, j;
  lines.clear();
  vector<char> flag(rcs.size(), 0);
  for (i = 0; i < (int)rcs.size(); ++i) {
    if (1) {
      flag[i] = 1;
      int id = -1;
      double mindis = 999999;
      for (j = i + 1; j < rcs.size(); ++j) {
        if (!flag[j]) {
          double dis = rcdist(rcs[i], rcs[j]);
          if (dis < mindis) {
            mindis = dis;
            id = j;
          }
        }
      }
      if (id >= 0 && mindis < thd) {
        flag[id] = 1;
        Point c1 = rect_center(rcs[i]);
        Point c2 = rect_center(rcs[id]);
        Vec4i v(c1.x, c1.y, c2.x, c2.y);
        lines.push_back(v);
      }
    }
  }
  return 0;
}


double cal_area(Point p1, Point p2, Point p3) {
  // param
  //p : [x, y]
  // return
  //S : area of triangle
  double x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y, x3 = p3.x, y3 = p3.y;
  return 0.5 * fabs(x2 * y3 + x1 * y2 + x3 * y1 - x3 * y2 - x2 * y1 - x1 * y3);
}

struct SimilarLine2 {
  double eps_;
  double deps_;
  SimilarLine2(double eps, double deps) {
    this->eps_ = eps;
    this->deps_ = deps;
  }
  inline bool operator()(const cv::Vec4i& r1, const cv::Vec4i& r2) const
  {
    // delta为最小长宽的eps倍
    Point p1 = Point(r1[0], r1[1]);
    Point p2 = Point(r1[2], r1[3]);
    Point a1 = Point(r2[0], r2[1]);
    Point a2 = Point(r2[2], r2[3]);
    double s1 = cal_area(p1, p2, a1);
    double s2 = cal_area(p1, p2, a2);
    double s3 = max(s1, s2);
    double d1 = min(ptdist(p1, a1), ptdist(p1, a2));
    double d2 = min(ptdist(p2, a1), ptdist(p2, a2));
    double d3 = min(d1, d2);
    return s3<eps_ && d3<deps_;
  }
};

template<typename T>
vector<int> argsort(const vector<T>& v) {
  int Len = v.size();
  vector<int> idx(Len, 0);
  for (int i = 0; i < Len; i++) {
    idx[i] = i;
  }
  std::sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] > v[i2]; });
  return idx;
}
template<typename T>
vector<int> argsort_d(const vector<T>& v) {
  int Len = v.size();
  vector<int> idx(Len, 0);
  for (int i = 0; i < Len; i++) {
    idx[i] = i;
  }
  std::sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });
  return idx;
}

int lines_part(const vector<Vec4i>& lines, vector<vector<Vec4i>>& lines_class, double thd) {
  std::vector<int> labels;
  std::vector<int> cnt;
  int nclasses = cv::partition(lines, labels, SimilarLine2(thd, thd *3));
  cnt.resize(nclasses);
  for (int i = 0; i < labels.size(); ++i) {
    int j = labels[i];
    ++cnt[j];
  }
  std::vector<int> index = argsort(cnt);
  nclasses = min(nclasses,7);
  lines_class.resize(nclasses);
  for (int i = 0; i < nclasses; ++i) {
    int idx = index[i];
    int c = cnt[idx];
    for (int j = 0; j < lines.size(); ++j) {
      if (labels[j]== idx) {
        lines_class[i].push_back(lines[j]);
      }
    }
  }
  return nclasses;
}


struct SimilarRect {
  double eps_;
  double deps_;
  SimilarRect(double eps, double deps) {
    this->eps_ = eps;
    this->deps_ = deps;
  }
  inline bool operator()(const cv::Rect& r1, const cv::Rect& r2) const
  {
    // delta为最小长宽的eps倍
    Point p1 = rect_center(r1);
    Point p2 = rect_center(r2);
    double d1 = ptdist(p1, p2);
    double s1 = fabs(r1.height - r2.height);
    double s2 = fabs(r1.width - r2.width);
    double s3 = min(s1, s2);
    return s3<eps_ && d1<deps_;
  }
};

struct SimilarLine {
  double eps;
  SimilarLine(double eps_ = 0.5) {
    this->eps = eps_;
  }
  inline bool operator()(const cv::Rect& r1, const cv::Rect& r2) const
  {
    // delta为最小长宽的eps倍
    double delta = eps*((r1.height+ r2.height));
    // 如果矩形的四个顶点的位置差别都小于delta，则表示相似的矩形
    int t = MAX(r1.y, r2.y);
    int b = MIN(r1.y + r1.height, r2.y + r2.height);
    int d = b - t;
    return d>delta;
  }
};

bool rect_less_line(const std::vector<Rect>& r1, const std::vector<Rect>& r2) {
  return (r1[0].y < r2[0].y);
}

int rects_part(const vector<Rect>& lines, vector<vector<Rect>>& lines_class, double thd) {
  std::vector<int> labels;
  std::vector<int> cnt;
  if (lines.size() < 1) return 0;
  int nclasses = cv::partition(lines, labels, SimilarLine(0.3));
  cnt.resize(nclasses);
  for (int i = 0; i < labels.size(); ++i) {
    int j = labels[i];
    ++cnt[j];
  }
  std::vector<int> index = argsort(cnt);
  nclasses = min(nclasses, 10);
  lines_class.resize(nclasses);
  for (int i = 0; i < nclasses; ++i) {
    int idx = index[i];
    int c = cnt[idx];
    for (int j = 0; j < lines.size(); ++j) {
      if (labels[j] == idx) {
        lines_class[i].push_back(lines[j]);
      }
    }
  }
  std::sort(lines_class.begin(), lines_class.end(), rect_less_line);
  return nclasses;
}

RotatedRect minAreaRect(const vector<Rect>& rects) {
  vector<Point> pts;
  pts.resize(rects.size()*4);
  Point* p = &pts[0];
  for (int i = 0; i < rects.size(); ++i) {
    rect2pt4(rects[i], p+i*4);
  }
  return minAreaRect(pts);
}

RotatedRect rotate(const RotatedRect& r, const Point2f& rotate_center, double angle) {
  double x1 = r.center.x;
  double y1 = r.center.y;
  double x2 = rotate_center.x;
  double y2 = rotate_center.y;
  double a = angle * CV_PI / 180.0;
  double c = cos(a), s = sin(a);
  double x = ((x1 - x2)*c - (y1 - y2)*s + x2);
  double y = ((x1 - x2)*s + (y1 - y2)*c + y2);
  return RotatedRect(Point2f(x, y), r.size, r.angle+ angle);
}

// flag 1-90 2-180 3-270
RotatedRect rect_rotate90(const RotatedRect& r, Size2f size, int flag) {
  flag &= 3;
  RotatedRect o = r;
  o.angle += flag*90;
  if (flag & 1) {
    std::swap(o.center.x, o.center.y);
    std::swap(o.size.width, o.size.height);
    std::swap(size.width, size.height);
    if (flag == 3) {
      o.center.y = size.height - o.center.y;
    }
    else {
      o.center.x = size.width - o.center.x;
    }
  }
  else if (flag == 2) {
    o.center.x = size.width - o.center.x;
    o.center.y = size.height - o.center.y;
  }
  else {
  }
  return o;
}

// flag 1-90 2-180 3-270
int rotate90(const Mat& matSrc, Mat& matDst, int flag) {
  flag &= 3;
  int width = matSrc.cols;
  int height = matSrc.rows;
  if (flag & 1) {
    cv::Mat matTranspose(width, height, matSrc.type());
    cv::transpose(matSrc, matTranspose);
    cv::flip(matTranspose, matDst, flag == 3 ? 0 : 1);
  }
  else if (flag==2) {
    cv::flip(matSrc, matDst, -1);
  }
  else {
    matDst = matSrc.clone();
  }
  return 0;
}

double angle(Point pt1, Point pt2, Point pt0) {
  double dx1 = pt1.x - pt0.x;
  double dy1 = pt1.y - pt0.y;
  double dx2 = pt2.x - pt0.x;
  double dy2 = pt2.y - pt0.y;

  double ratio;
  //矩形长和宽平方的比
  ratio = (dx1*dx1 + dy1*dy1) / (dx2*dx2 + dy2*dy2);
  if (ratio < 0.8 || 1.2 < ratio) { //根据矩形长宽平方比淘汰四边形return 1.0;
  }

  return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);//???
}

// 角度余弦
double angle(double dx1, double dy1, double dx2, double dy2) {
  double ratio;
  //矩形长和宽平方的比
  ratio = (dx1*dx1 + dy1*dy1) / (dx2*dx2 + dy2*dy2);
  //根据矩形长宽平方比淘汰四边形
  //if (ratio < 0.8 || 1.2 < ratio) {     return 1.0;  }

  return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);//???
}

struct SimilarLine3 {
  double eps_;
  SimilarLine3(double eps) {
    this->eps_ = eps;
  }
  inline bool operator()(const cv::Vec4i& r1, const cv::Vec4i& r2) const
  {
    // delta为最小长宽的eps倍
    double s3 = angle(r1[2] - r1[0], r1[3] - r1[1], r2[2] - r2[0], r2[3] - r2[1]);
    s3 = fabs(1-fabs(s3));
    double d1 = pt2Line(Point(r1[0], r1[1]), Point(r2[0], r2[1]), Point(r2[2], r2[3]));
    double d2 = pt2Line(Point(r1[2], r1[3]), Point(r2[0], r2[1]), Point(r2[2], r2[3]));
    s3 = s3*fabs(d1 - d2);
    //printf("%lf\n", s3);
    return s3<eps_;
  }
};
int near_line(const vector<Vec4i>& lines, Point pt, double thd) {
  int i, j=-1;
  for (i = 0; i < lines.size(); ++i) {
    Vec4i l = lines[i];
    double d = pt2Line(pt, Point(l[0], l[1]), Point(l[2], l[3]));
    if (d < thd) {
      thd = d;
      j = i;
    }
  }
  return j;
}

Point ptFootOfPerpendicular(Point p1, Point p, double& u) {
  double d = ptDot(p, p);
  u = ptDot(p1, p) / d;
  return u*p;
}
Point ptFootOfPerpendicular(Point p1, Vec4i l, double& u) {
  Point lp1(l[0], l[1]);
  Point lp2(l[2], l[3]);
  return ptFootOfPerpendicular(p1-lp1, lp2-lp1, u)+lp1;
}
Vec4i ptResetLine(const Vec4i& l, const vector<Point>& pts, const vector<int>& labelspt, int idx) {
  double minv = 1000000000000., maxv = -minv;
  Point minpt(0,0), maxpt(0, 0);
  Point lp1(l[0], l[1]);
  Point lp2(l[2], l[3]);
  for (int i = 0; i < pts.size(); ++i) {
    if (labelspt[i] == idx) {
      Point p1 = pts[i];
      double u = 0;
      Point fpt = ptFootOfPerpendicular(p1 - lp1, lp2 - lp1, u) + lp1;
      if (u < minv) {
        minv = u;
        minpt = fpt;
      }
      else if (u > maxv) {
        maxv = u;
        maxpt = fpt;
      }
    }
  }
  return Vec4i(minpt.x, minpt.y, maxpt.x, maxpt.y);
}

int lines_part(const vector<Vec4i>& lines, const vector<Point>& pts, vector<vector<Vec4i>>& lines_class, double thd) {
  std::vector<int> labels;
  if (lines.size() < 1) return 0;
  int nclasses = cv::partition(lines, labels, SimilarLine3(0.01));
  std::vector<int> cnt(nclasses, 0);
  std::vector<int> cntline(lines.size(), 0);
  std::vector<int> labelspt(pts.size(), 0);
  for (int i = 0; i < pts.size(); ++i) {
    int j = near_line(lines, pts[i], thd);
    if (j >= 0) {
      labelspt[i] = j;
      int k = labels[j];
      cntline[j]++;
    }
  }
  for (int j = 0; j < lines.size(); ++j) {
    int c = cntline[j];
    if (c > 1) {
      int k = labels[j];
      cnt[k] += c;
    }
  }

  std::vector<int> index = argsort(cnt);
  //nclasses = min(nclasses, 10);
  lines_class.resize(nclasses);
  for (int i = 0; i < nclasses; ++i) {
    int idx = index[i];
    int c = cnt[idx];
    if (c > 5) {
      for (int j = 0; j < lines.size(); ++j) {
        if (labels[j] == idx) {
          Vec4i l = lines[j];
          l = ptResetLine(l, pts, labelspt, j);
          lines_class[i].push_back(l);
        }
      }
    }
  }
  //std::sort(lines_class.begin(), lines_class.end(), rect_less_line);
  return nclasses;
}

Vec4i ptResetLine(const Vec4i& l, const vector<RotatedRect>& pts, const vector<int>& labelspt, int idx) {
  double minv = 1000000000000., maxv = -minv;
  Point minpt(0, 0), maxpt(0, 0);
  Point lp1(l[0], l[1]);
  Point lp2(l[2], l[3]);
  for (int i = 0; i < pts.size(); ++i) {
    if (labelspt[i] == idx) {
      Point p1(pts[i].center.x, pts[i].center.y);
      double u = 0;
      Point fpt = ptFootOfPerpendicular(p1 - lp1, lp2 - lp1, u) + lp1;
      if (u < minv) {
        minv = u;
        minpt = fpt;
      }
      else if (u > maxv) {
        maxv = u;
        maxpt = fpt;
      }
    }
  }
  return Vec4i(minpt.x, minpt.y, maxpt.x, maxpt.y);
}



struct lines_part_t {
  std::vector<int> labels;
  std::vector<int> cnt;
  std::vector<int> cntline;
  std::vector<int> labelspt;
  vector<vector<Vec4i>> lines_class;
int run(const vector<Vec4i>& lines, const vector<RotatedRect>& pts, double thd) {
  if (lines.size() < 1) return 0;
  int nclasses = cv::partition(lines, labels, SimilarLine3(0.01));
  cnt.assign(nclasses, 0);
  cntline.assign(lines.size(), 0);
  labelspt.assign(pts.size(), -1);
  for (int i = 0; i < pts.size(); ++i) {
    int j = near_line(lines, Point(pts[i].center.x, pts[i].center.y), thd);
    labelspt[i] = j;
    if (j >= 0) {
      int k = labels[j];
      cntline[j]+= pts[i].size.area();
    }
  }
  for (int j = 0; j < lines.size(); ++j) {
    int c = cntline[j];
    if (c > 1) {
      int k = labels[j];
      cnt[k] += c;
    }
  }

  std::vector<int> index = argsort(cnt);
  //nclasses = min(nclasses, 10);
  lines_class.resize(nclasses);
  for (int i = 0; i < nclasses; ++i) {
    int idx = index[i];
    int c = cnt[idx];
    if (c > 5) {
      for (int j = 0; j < lines.size(); ++j) {
        if (labels[j] == idx) {
          Vec4i l = lines[j];
          //l = ptResetLine(l, pts, labelspt, j);
          lines_class[i].push_back(l);
        }
      }
    }
  }
  //std::sort(lines_class.begin(), lines_class.end(), rect_less_line);
  return nclasses;
}
};

// 3种清晰度评价方法，分别是Tenengrad梯度方法、Laplacian梯度方法和方差方法
// Tenengrad梯度方法
// 得分越高越清晰
double clarityTenengrad(const Mat& imageGrey, int method = 0) {
  double meanValue = 0;
  Mat imageSobel;
  Mat meanValueImage;
  Mat meanStdValueImage;

  switch(method) {
    default:
      Sobel(imageGrey, imageSobel, CV_16U, 1, 1);
      meanValue = mean(imageSobel)[0];
      break;
    case 1:
      Laplacian(imageGrey, imageSobel, CV_16U);
      meanValue = mean(imageSobel)[0];
      break;
    case 2:
      meanStdDev(imageGrey, meanValueImage, meanStdValueImage);
      meanValue = meanStdValueImage.at<double>(0, 0);
      break;
  }
  //图像的平均灰度
  return meanValue;
}

Mat getSubImage(const Mat& src, RotatedRect r) {
  Point2f dstTri[4];
  Point2f srcTri[4];
  RotatedRect d(Point2f(r.size.width / 2., r.size.height / 2.), r.size, 0);
  d.points(dstTri);
  r.points(srcTri);

  Mat dst = Mat::zeros(r.size.height, r.size.width, src.type());
  Mat warp_mat = getAffineTransform(srcTri, dstTri);
  warpAffine(src, dst, warp_mat, dst.size());
  return dst;
}

Mat getSubImage(const Mat& src, RotatedRect r, Size size) {
  Point2f dstTri[4];
  Point2f srcTri[4];
  RotatedRect d(Point2f(size.width / 2., size.height / 2.), size, 0);
  d.points(dstTri);
  r.points(srcTri);
  Mat dst = Mat::zeros(size.height, size.width, src.type());
  Mat warp_mat = getAffineTransform(srcTri, dstTri);
  warpAffine(src, dst, warp_mat, dst.size());
  return dst;
}

// angle 0~360
int imRotation(const Mat& src, Mat& dst, int angle) {
  cv::Point2f center(src.cols / 2, src.rows / 2);
  cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1);
  cv::Rect bbox = cv::RotatedRect(center, src.size(), angle).boundingRect();

  rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
  rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

  cv::warpAffine(src, dst, rot, bbox.size());
  return 0;
}

struct rect_less_line_t {
  bool operator()(const std::vector<RotatedRect>& r1, const std::vector<RotatedRect>& r2) {
    return false;
  }
};


void drawRotatedRects(Mat& color_edge, const vector<vector<RotatedRect>>& lines_class, int lw)
{
  RNG rng(12345);
  for (size_t j = 0; j < lines_class.size(); j++) {
    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    for (size_t i = 0; i < lines_class[j].size(); i++) {
      RotatedRect r = lines_class[j][i];
      drawRotatedRect(color_edge, r, color, lw);
    }
  }
}

void drawRotatedRects(Mat& color_edge, const vector<RotatedRect>& lines_class, int lw)
{
  RNG rng(12345);
  for (size_t j = 0; j < lines_class.size(); j++) {
    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    RotatedRect r = lines_class[j];
    drawRotatedRect(color_edge, r, color, lw);
  }
}


RotatedRect minAreaRect(const vector<RotatedRect>& vr) {
  vector<Point> p;
  if (vr.size() <1) {
    return RotatedRect(Point2f(0,0), Size2f(0,0), 0);
  }
  for (int i = 0; i < vr.size(); ++i) {
    RotatedRect r = vr[i];
    if (r.size.area() > 100) {
      //p.push_back(Point(r.center));
      double dx = r.size.width*0.5;
      double dy = r.size.height*0.5;
      p.push_back(Point(r.center.x - dx, r.center.y - dy));
      p.push_back(Point(r.center.x - dx, r.center.y + dy));
      p.push_back(Point(r.center.x + dx, r.center.y + dy));
      p.push_back(Point(r.center.x + dx, r.center.y - dy));
    }
  }
  if (p.size() <1) {
    return RotatedRect(Point2f(0, 0), Size2f(0, 0), 0);
  }
  RotatedRect r = minAreaRect(p);
  return r;
}

template <typename T>
vector<T> select_index(const vector<T>& src, const vector<int>& index, int idx) {
  vector<T> dst;
  for (int i = 0; i < src.size(); ++i) {
    if (index[i] == idx) {
      dst.push_back(src[i]);
    }
  }
  return dst;
}
template <typename T>
vector<T> select_index_all(const vector<T>& src, const vector<int>& index) {
  vector<T> dst;
  for (int i = 0; i < src.size(); ++i) {
    dst.push_back(src[index[i]]);
  }
  return dst;
}
    // 计算 |p1 p2| X |p1 p|
double ptCross(Point p1, Point p2, Point p) {
  return (p2.x - p1.x) * (p.y - p1.y) - (p.x - p1.x) * (p2.y - p1.y);
}

    //判断点p是否在p1p2p3p4的正方形内
double ptInMatrix(Point p, Point p1, Point p2, Point p3, Point p4) {
  bool isPointIn = ptCross(p1, p2, p) * ptCross(p3, p4, p) >= 0 && ptCross(p2, p3, p) * ptCross(p4, p1, p) >= 0;
  return isPointIn;
}

bool DoesRectangleContainPoint(RotatedRect rectangle, Point2f point) {
  //Get the corner points.
  Point2f corners[4];
  rectangle.points(corners);

  //Convert the point array to a vector.
  //https://stackoverflow.com/a/8777619/1997617
  Point2f* lastItemPointer = (corners + sizeof corners / sizeof corners[0]);
  vector<Point2f> contour(corners, lastItemPointer);

  //Check if the point is within the rectangle.
  double indicator = pointPolygonTest(contour, point, false);
  bool rectangleContainsPoint = (indicator >= 0);
  return rectangleContainsPoint;
}

double mymod(double a, double m) {
  a = fmod(a, m);
  double k = m*0.5;
  //a = fmod(a+m, m);
  //a = fmod(a, m);
  if (a >= k) {
    a -= m;
  } else if (a < -k) {
    a += m;
  }
  return a;
}
int mymodi(int a, int m) {
  a %= m;
  int k = m/2;
  if (a >= k) {
    a -= m;
  } else if (a < -k) {
    a += m;
  }
  return a;
}

struct rrects_line_part {
  vector<vector<RotatedRect>> lines_class;
  std::vector<int> labels;
  std::vector<int> cnt;
  vector<RotatedRect> lines_bound;

  struct Similar_rrects_w_part {
    double eps_;
    Similar_rrects_w_part(double eps = 0.5) {
      this->eps_ = eps;
    }
    inline bool operator()(const cv::RotatedRect& r1, const cv::RotatedRect& r2) const
    {
      double eps1 = eps_*(r1.size.height + r2.size.height);
      double x = (r1.center.x - r2.center.x);
      double y = (r1.center.y - r2.center.y);
      double d1 = sqrt(x*x + y*y);
      return d1<eps1;
    }
  };

  int run(const vector<RotatedRect>& lines, double thd) {
    if (lines.size() < 1) return 0;
    int nclasses = cv::partition(lines, labels, Similar_rrects_w_part(thd));
    cnt.assign(nclasses, 0);
    for (int i = 0; i < labels.size(); ++i) {
      int j = labels[i];
      ++cnt[j];
    }
    std::vector<int> index = argsort(cnt);
    nclasses = min(nclasses, 15);
    lines_class.resize(nclasses);

    for (int i = 0; i < nclasses; ++i) {
      int idx = index[i];
      int c = cnt[idx];
      for (int j = 0; j < lines.size(); ++j) {
        if (labels[j] == idx) {
          lines_class[i].push_back(lines[j]);
        }
      }
    }
    if (nclasses>0) {
      lines_bound = lines_class[0];
    }
    return nclasses;
  }
};

RotatedRect& curr(RotatedRect& rr) {
  if (rr.size.height > rr.size.width) {
    std::swap(rr.size.height, rr.size.width);
    rr.angle += 90;
  }
  return rr;
}

struct rrects_part {
  vector<vector<RotatedRect>> lines_class;
  std::vector<int> labels;
  std::vector<int> cnt;
  vector<RotatedRect> lines_bound;

  struct SimilarRotatedRect {
    double eps_;
    double angle_;
    double s;
    double c;
    SimilarRotatedRect(double angle, double eps = 0.5) {
      this->eps_ = eps;
      double a = (angle)*CV_PI / 180;
      this->s = -sin(a);
      this->c = cos(a);
    }
    inline bool operator()(const cv::RotatedRect& r1, const cv::RotatedRect& r2) const
    {
      //int k = 10;
      //if (r1.size.height < k || r1.size.width < k || r2.size.height < k || r2.size.width < k) { return false; }
      double s1 = r1.size.height*1. / r2.size.height;
      double s2 = r1.size.width*1. / r2.size.width;
      if (0.5 > s1 && s1 > 2) return false;
      if (0.5 > s2 && s2 > 2) return false;
      int h = r1.size.height + r2.size.height;
      int w = r1.size.width + r2.size.width;
      int ww = MIN(w, h);
      double dis = ptdist(r1.center, r2.center);
      if (dis > (10 * ww)) { return false; }
      double eps1 = eps_*(ww);
      double y1 = r1.center.x*s + r1.center.y*c;
      double y2 = r2.center.x*s + r2.center.y*c;
      return fabs(y1 - y2)<eps1;
    }
  };
  int run(const vector<RotatedRect>& lines, double angle, double thd, int minnum) {
    if (lines.size() < 1) return 0;
    int nclasses = cv::partition(lines, labels, SimilarRotatedRect(angle, thd));
    cnt.assign(nclasses, 0);
    for (int i = 0; i < labels.size(); ++i) {
      int j = labels[i];
      ++cnt[j];
    }
    std::vector<int> index = argsort(cnt);
    nclasses = min(nclasses, 15);
    lines_class.resize(nclasses);
    if (0) {
      for (; nclasses > 0; --nclasses) {
        int idx = index[nclasses - 1];
        int c = cnt[idx];
        if (c > minnum) {
          break;
        }
      }
    }
    for (int i = 0; i < nclasses; ++i) {
      int idx = index[i];
      int c = cnt[idx];
      for (int j = 0; j < lines.size(); ++j) {
        if (labels[j] == idx) {
          lines_class[i].push_back(lines[j]);
        }
      }
    }
    //lines_bound.resize(nclasses);
    for (int i = 0; i < nclasses; ++i) {
      vector<RotatedRect>& ls = lines_class[i];
      rrects_line_part rlp;
      if (0) {
        rlp.run(ls, 2);
        ls = rlp.lines_class[0];
      }
      RotatedRect r = minAreaRect(ls);
      double area = 0;
      for (int j = 0; j < ls.size(); ++j) {
        area += ls[j].size.area();
      }
      area /= r.size.area();
      if (r.size.height>r.size.width) {
        std::swap(r.size.height, r.size.width);
        r.angle += 90;
      }
      //r.angle = mymod(r.angle, 180);
      //lines_bound[i] = r;
      //lines_bound[i].angle = angle;
      double da = mymod(r.angle - angle, 180);
      da = fabs(da);
      if (da<10 && r.size.area()>1000 && area>0.03) {
        lines_bound.push_back(r);
      }
    }
    //std::sort(lines_class.begin(), lines_class.end(), rect_less_line);
    return lines_bound.size();
  }
};

struct SimilarRotatedRectW {
  double eps_;
  double s;
  double c;
  SimilarRotatedRectW(double angle, double eps = 0.5) {
    this->eps_ = eps;
    double a = (angle)*CV_PI / 180;
    this->s = -sin(a);
    this->c = cos(a);
  }
  inline bool operator()(const cv::RotatedRect& r1, const cv::RotatedRect& r2) const
  {
    double eps1 = eps_*(r1.size.height+ r2.size.height);
    double d1 = fabs(r1.size.height - r2.size.height);
    return d1<eps1;
  }
};

struct rrects_w_part {
  vector<vector<RotatedRect>> lines_class;
  std::vector<int> labels;
  std::vector<int> cnt;
  vector<RotatedRect> lines_bound;

  int run(const vector<RotatedRect>& lines, double angle, double thd) {
    if (lines.size() < 1) return 0;
    int nclasses = cv::partition(lines, labels, SimilarRotatedRectW(angle, thd));
    cnt.assign(nclasses, 0);
    for (int i = 0; i < labels.size(); ++i) {
      int j = labels[i];
      ++cnt[j];
    }
    std::vector<int> index = argsort(cnt);
    nclasses = min(nclasses, 15);
    lines_class.resize(nclasses);

    for (int i = 0; i < nclasses; ++i) {
      int idx = index[i];
      int c = cnt[idx];
      for (int j = 0; j < lines.size(); ++j) {
        if (labels[j] == idx) {
          lines_class[i].push_back(lines[j]);
        }
      }
    }
    if (nclasses>0) {
      lines_bound = lines_class[0];
    }
    return nclasses;
  }
};
void drawDetectLines(Mat& image, const vector<Vec4i>& lines, const Scalar & color)
{
  // 将检测到的直线在图上画出来
  vector<Vec4i>::const_iterator it = lines.begin();
  while (it != lines.end())
  {
    Point pt1((*it)[0], (*it)[1]);
    Point pt2((*it)[2], (*it)[3]);
    line(image, pt1, pt2, color, 2); //  线条宽度设置为2
    ++it;
  }
}

#include "hough.hpp"
