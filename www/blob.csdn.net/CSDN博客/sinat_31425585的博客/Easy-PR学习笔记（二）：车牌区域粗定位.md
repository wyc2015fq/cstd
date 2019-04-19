# Easy-PR学习笔记（二）：车牌区域粗定位 - sinat_31425585的博客 - CSDN博客
2018年04月29日 23:35:37[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：367
要实现对全景图中的车牌进行识别，首先需要对车牌进行定位，即从全景图像中对车牌进行定位，获取车牌在全景图中的矩形位置。常见思路分为两种：滑动窗口搜索和图像处理方法。
Easy-PR项目中采用的方式为图像处理的方式。核心处理函数为plate_detect.cpp文件中的plateDetect函数：
```cpp
int CPlateDetect::plateDetect(Mat src, std::vector<CPlate> &resultVec, int type,
    bool showDetectArea, int img_index) {
    std::vector<CPlate> sobel_Plates;
    sobel_Plates.reserve(16);
    std::vector<CPlate> color_Plates;
    color_Plates.reserve(16);
    std::vector<CPlate> mser_Plates;
    mser_Plates.reserve(16);
    std::vector<CPlate> all_result_Plates;
    all_result_Plates.reserve(64);
#pragma omp parallel sections
    {
#pragma omp section
      {
        if (!type || type & PR_DETECT_SOBEL) {
			// 使用Sobel边缘进行定位
          m_plateLocate->plateSobelLocate(src, sobel_Plates, img_index);
        }
      }
#pragma omp section
      {
        if (!type || type & PR_DETECT_COLOR) {
          m_plateLocate->plateColorLocate(src, color_Plates, img_index);
        }
      }
#pragma omp section
      {
        if (!type || type & PR_DETECT_CMSER) {
          m_plateLocate->plateMserLocate(src, mser_Plates, img_index);
        }
      }
    }
    for (auto plate : sobel_Plates) {
      plate.setPlateLocateType(SOBEL);
      all_result_Plates.push_back(plate);
    }
    for (auto plate : color_Plates) {
      plate.setPlateLocateType(COLOR);
      all_result_Plates.push_back(plate);
    }
    for (auto plate : mser_Plates) {
      plate.setPlateLocateType(CMSER);
      all_result_Plates.push_back(plate);
    }
    // use nms to judge plate
    PlateJudge::instance()->plateJudgeUsingNMS(all_result_Plates, resultVec, m_maxPlates);
    if (0)
      showDectectResults(src, resultVec, m_maxPlates);
    return 0;
  }
```
车牌定位方法位于plateSobelLocate和plateColorLocate两个函数中。
**1、plateSobelLocate**，即基于sobel边缘检测的车牌定位方式，而车牌上具有大量竖直方向边缘，且不会发生旋转、透视扭曲等现象，采用x方向Sobel滤波器进行边缘检测。
```cpp
int CPlateLocate::plateSobelLocate(Mat src, vector<CPlate> &candPlates,
                                   int index) {
  vector<RotatedRect> rects_sobel_all;
  rects_sobel_all.reserve(256);
  vector<CPlate> plates;
  plates.reserve(32);
  vector<Rect_<float>> bound_rects;
  bound_rects.reserve(256);
  sobelFrtSearch(src, bound_rects);
  vector<Rect_<float>> bound_rects_part;
  bound_rects_part.reserve(256);
  // enlarge area 
  for (size_t i = 0; i < bound_rects.size(); i++) {
    float fRatio = bound_rects[i].width * 1.0f / bound_rects[i].height;
    if (fRatio < 3.0 && fRatio > 1.0 && bound_rects[i].height < 120) {
      Rect_<float> itemRect = bound_rects[i];
      itemRect.x = itemRect.x - itemRect.height * (4 - fRatio);
      if (itemRect.x < 0) {
        itemRect.x = 0;
      }
      itemRect.width = itemRect.width + itemRect.height * 2 * (4 - fRatio);
      if (itemRect.width + itemRect.x >= src.cols) {
        itemRect.width = src.cols - itemRect.x;
      }
      itemRect.y = itemRect.y - itemRect.height * 0.08f;
      itemRect.height = itemRect.height * 1.16f;
      bound_rects_part.push_back(itemRect);
    }
  }
  // second processing to split one
#pragma omp parallel for
  for (int i = 0; i < (int)bound_rects_part.size(); i++) {
    Rect_<float> bound_rect = bound_rects_part[i];
    Point2f refpoint(bound_rect.x, bound_rect.y);
    float x = bound_rect.x > 0 ? bound_rect.x : 0;
    float y = bound_rect.y > 0 ? bound_rect.y : 0;
    float width =
        x + bound_rect.width < src.cols ? bound_rect.width : src.cols - x;
    float height =
        y + bound_rect.height < src.rows ? bound_rect.height : src.rows - y;
    Rect_<float> safe_bound_rect(x, y, width, height);
    Mat bound_mat = src(safe_bound_rect);
    vector<RotatedRect> rects_sobel;
    rects_sobel.reserve(128);
    sobelSecSearchPart(bound_mat, refpoint, rects_sobel);
#pragma omp critical
    {
      rects_sobel_all.insert(rects_sobel_all.end(), rects_sobel.begin(), rects_sobel.end());
    }
  }
#pragma omp parallel for
  for (int i = 0; i < (int)bound_rects.size(); i++) {
    Rect_<float> bound_rect = bound_rects[i];
    Point2f refpoint(bound_rect.x, bound_rect.y);
    float x = bound_rect.x > 0 ? bound_rect.x : 0;
    float y = bound_rect.y > 0 ? bound_rect.y : 0;
    float width =
        x + bound_rect.width < src.cols ? bound_rect.width : src.cols - x;
    float height =
        y + bound_rect.height < src.rows ? bound_rect.height : src.rows - y;
    Rect_<float> safe_bound_rect(x, y, width, height);
    Mat bound_mat = src(safe_bound_rect);
    vector<RotatedRect> rects_sobel;
    rects_sobel.reserve(128);
    sobelSecSearch(bound_mat, refpoint, rects_sobel);
#pragma omp critical
    {
      rects_sobel_all.insert(rects_sobel_all.end(), rects_sobel.begin(), rects_sobel.end());
    }
  }
  Mat src_b;
  sobelOper(src, src_b, 3, 10, 3);
  deskew(src, src_b, rects_sobel_all, plates);
  //for (size_t i = 0; i < plates.size(); i++) 
  //  candPlates.push_back(plates[i]);
  candPlates.insert(candPlates.end(), plates.begin(), plates.end());
  return 0;
}
```
可以看到，plateSobelLocate又可以分为三个步骤：sobelFrtSearch、 sobelSecSearchPart、sobelSecSearch。
sobelFrtSearch：首先，采用大小为8x8的高斯核进行高斯平滑，并转化成灰度图；然后，使用OTSU算法对二值化x方向Sobel边缘进行二值化操作；其次，采用大小为17x3的核进行形态学闭操作，八领域轮廓跟踪方法获取外轮廓；最后，获取轮廓对应最小外接矩形，并采用长宽比进行验证，过滤掉部分候选矩形区域。
Gassian（5x5）-> gray -> Sobel_x -> OTSU -> close（17x3） -> findContours -> minAreaRect -> verifySizes
sobelSecSearchPart：是对sobelFrtSearch得到的每一个候选矩形区域的进一步筛选处理，和sobelFrtSearch处理流程基本一致，增加了两个处理过程：跳变约束（清除跳变数小于7的行）和左右边界查询（修复边界）
Gassian（3x3）->gray -> Sobel_x -> OTSU -> close（6x2）-> clearLiuDingOnly -> bFindLeftRightBound  -> findContours -> minAreaRect -> verifySizes
sobelSecSearch：与sobelFrtSearch操作基本一致，只是核大小不同。
Gassian（3x3）-> gray -> Sobel_x -> OTSU -> close（10x3） -> findContours -> minAreaRect -> verifySizes
然后进行倾斜校正。
**2、plateColorLocate：**利用颜色做进一步的验证，即车牌底色一般为颜色和黄色，将车牌候选区域做一个HSV颜色空间变换，得到H分量，并对H分量是否对应蓝色和黄色区间进行验证，这样就可以得到候选车牌区域。
colorMatch -> close（10x2） -> findContours -> minAreaRect -> verifySizes
