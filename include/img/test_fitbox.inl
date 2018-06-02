//opencv拟合矩形代码，可以搜索图像中存在的矩形区域、模板等。

#include "cv.h"
#include "highgui.h"
#include <vector>

int otsu(const IplImage* image, int x0, int y0, int dx, int dy);

bool BoxInBox(CvBox2D box, CvBox2D box_outer);
struct ChipInfo {
  // CvRect rect;
  CvBox2D box;
  double area;
};

int main(int argc, char* argv[])
{
  IplImage* src = cvLoadImage("F:/code/cstd/src/imgproc/asdf0.bmp", 1);
  IplImage* gray = cvCreateImage(cvGetSize(src), 8, 1);
  cvCvtColor(src, gray, CC_RGB2GRAY);
  IplImage* gray_clone = cvCloneImage(gray);
  int threshold = otsu(gray_clone, 0, 0, gray->w, gray->h) * 2 / 3;
  cvThreshold(gray_clone, gray_clone, threshold, 255, CC_THRESH_BINARY);
  cvSaveImage("D:/t.bmp", gray_clone);
  IplImage* show = cvCreateImage(cvGetSize(src), 8, 3);
  cvCvtColor(gray_clone, show, CC_GRAY2RGB);


  cvErode(gray_clone, gray_clone);
  cvDilate(gray_clone , gray_clone);

  CvMemStorage* storage = cvCreateMemStorage(0);
  CvSeq* contours;
  cvFindContours(gray_clone, storage, &contours, sizeof(CvContour), CC_RETR_TREE,
      CC_CHAIN_APPROX_NONE, iPOINT(0, 0));

  IplImage* dst_tmp = cvCloneImage(gray);
  cvZero(dst_tmp);

  std::vector <ChipInfo> candiate_rect_vec;

  for (; contours; contours = contours->h_next) {
    if (contours->total < 30) {
      continue;
    }

    if (!contours->v_next) {
      continue;
    }

    CvSeq* contour_second = contours->v_next;

    for (; contour_second; contour_second = contour_second->h_next) {
      if (!(contour_second->v_next)) {
        continue;
      }

      CvSeq* contour_first = contour_second->v_next;

      for (; contour_first; contour_first = contour_first->h_next) {
        if (contour_first->total < 50) {
          continue;
        }

        CvBox2D box_outer = cvMinAreaRect2(contour_first);

        if (max(box_outer.size.height, box_outer.size.width) > 1.3 * min(box_outer.size.height, box_outer.size.width)) {
          continue;
        }

        double area = fabs(cvContourArea(contour_first));
        ChipInfo chip;
        chip.box = box_outer;
        //   chip.rect = rect;
        chip.area = area;
        candiate_rect_vec.push_back(chip);
      }
    }
  }

  //得到了候选区域后，在每个候选区域内使用otsu算法进行二值化
  //得到了一些候选的区域后，设定占空比\长宽比均接近1的候选目标为目标
  float likely_max = -1;
  int index = -1;

  for (int t = candiate_rect_vec.size() - 1; t >= 0; t--) {
    float prob = 0;
    //  prob += candiate_rect_vec[t].area/(candiate_rect_vec[t].box.size.width *candiate_rect_vec[t].box.size.width);
    prob += candiate_rect_vec[ t ].box.size.width / candiate_rect_vec[ t ].box.size.width;

    if (likely_max < prob) {
      likely_max = prob;
      index = t;
    }
  }

  if (index >= 0) {
    CvPoint2D32f pt[ 4 ];
    cvBoxPoints(candiate_rect_vec[ index ].box, pt);
    IPOINT pt0, pt1;
    pt0 = iPOINT(cvRound(pt[ 3 ].x), cvRound(pt[ 3 ].y));

    for (int i = 0; i < 4; i++) {
      pt1 = iPOINT(cvRound(pt[ i ].x), cvRound(pt[ i ].y));
      cvLine(src, pt0, pt1, CC_RGB(255, 0, 0), 1);
      pt0 = pt1;
    }

    cvLine(src, iPOINT(cvRound(candiate_rect_vec[ index ].box.center.x - 5),
        cvRound(candiate_rect_vec[ index ].box.center.y)),
        iPOINT(cvRound(candiate_rect_vec[ index ].box.center.x + 5),
            cvRound(candiate_rect_vec[ index ].box.center.y)),
        CC_RGB(255, 0, 0), 1);
    cvLine(src, iPOINT(cvRound(candiate_rect_vec[ index ].box.center.x),
        cvRound(candiate_rect_vec[ index ].box.center.y - 5)),
        iPOINT(cvRound(candiate_rect_vec[ index ].box.center.x),
            cvRound(candiate_rect_vec[ index ].box.center.y + 5)),
        CC_RGB(255, 0, 0), 1);
    char a[ 100 ];
    sprintf(a, "chip pos:(%d , %d),angle: %d", cvRound(candiate_rect_vec[ index ].box.center.x),
        cvRound(candiate_rect_vec[ index ].box.center.y),
        cvRound(candiate_rect_vec[ index ].box.angle));
    MessageBox(NULL, a, "chip information", MB_OK);

  }
  else {
    char a[ 100 ];
    sprintf(a, "No Chip");
    MessageBox(NULL, a, "chip information", MB_OK);
  }

  cvDestroyWindow("gray");
  cvNamedWindow("image", 1);
  cvShowImage("image", src);
  cvWaitKey(0);
  cvSaveImage("chip_detect.bmp", src);
  return index;
}

int otsu(const IplImage* image, int x0, int y0, int dx, int dy)
{

  unsigned char* np;       // 图像指针
  int thresholdValue = 1; // 阈值
  int ihist[ 256 ];             // 图像直方图，256个点

  int i, j, k;          // various counters
  int n, n1, n2, gmin, gmax;
  double m1, m2, sum, csum, fmax, sb;

  // 对直方图置零...
  memset(ihist, 0, sizeof(ihist));

  gmin = 255;
  gmax = 0;

  // 生成直方图
  for (i = y0 + 1; i < y0 + dy - 1; i++) {
    np = (uchar*)(image->imageData + i * image->wStep);

    // np = &image[i*cols+x0+1];
    for (j = x0 + 1; j < x0 + dx - 1; j++) {
      ihist[ *np ] ++;

      if (*np > gmax) {
        gmax = *np;
      }

      if (*np < gmin) {
        gmin = *np;
      }

      np++; /* next pixel */
    }
  }

  // set up everything
  sum = csum = 0.0;
  n = 0;

  for (k = 0; k <= 255; k++) {
    sum += (double) k * (double) ihist[ k ];        /* x*f(x) 质量矩*/
    n += ihist[ k ];                                         /*  f(x)    质量    */
  }

  if (!n) {
    // if n has no value, there is problems...
    return -1;
  }

  // do the otsu global thresholding method
  fmax = -1.0;
  n1 = 0;

  for (k = 0; k < 255; k++) {
    n1 += ihist[ k ];

    if (!n1) {
      continue;
    }

    n2 = n - n1;

    if (n2 == 0) {
      break;
    }

    csum += (double) k * ihist[ k ];
    m1 = csum / n1;
    m2 = (sum - csum) / n2;
    sb = (double) n1 * (double) n2 * (m1 - m2) * (m1 - m2);

    /* bbg: note: can be optimized. */
    if (sb > fmax) {
      fmax = sb;
      thresholdValue = k;
    }
  }

  // at this point we have our thresholding value

  // debug code to display thresholding values
  return (thresholdValue);
}

bool PointInBox(CvPoint2D32f pt, CvBox2D box)
{
  CvPoint2D32f pts[ 4 ];
  cvBoxPoints(box, pts);
  //根据两点式方程，计算直线0，1和直线2，3方程
  // f(x,y) = (x1-x0)(y-y0)-(x-x0)(y1-y0)
  double fd, fp;
  float x0, y0, x1, y1, x, y, xd, yd;
  x0 = pts[ 0 ].x;
  y0 = pts[ 0 ].y;
  x1 = pts[ 1 ].x;
  y1 = pts[ 1 ].y;
  x = pt.x;
  y = pt.y;
  xd = pts[ 2 ].x;
  yd = pts[ 2 ].y;
  fp = (x1 - x0) * (y - y0) - (x - x0) * (y1 - y0);
  fd = (x1 - x0) * (yd - y0) - (xd - x0) * (y1 - y0);

  if ((fp > 0 && fd < 0) || (fp < 0 && fd > 0)) {
    return false;
  }

  x0 = pts[ 2 ].x;
  y0 = pts[ 2 ].y;
  x1 = pts[ 1 ].x;
  y1 = pts[ 1 ].y;
  x = pt.x;
  y = pt.y;
  xd = pts[ 3 ].x;
  yd = pts[ 3 ].y;
  fp = (x1 - x0) * (y - y0) - (x - x0) * (y1 - y0);
  fd = (x1 - x0) * (yd - y0) - (xd - x0) * (y1 - y0);

  if ((fp > 0 && fd < 0) || (fp < 0 && fd > 0)) {
    return false;
  }

  return true;
}

bool BoxInBox(CvBox2D box, CvBox2D box_outer)
{
  CvPoint2D32f pts[ 4 ];
  cvBoxPoints(box, pts);

  for (int i = 0; i < 4; i++) {
    if (!PointInBox(pts[ i ], box_outer)) {
      return false;
    }
  }

  return true;
}

