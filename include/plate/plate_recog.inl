
#include "cstd.h"
#include "svm.inl"
//#include "cascade.inl"
#include "adaboost/cascade.inl"
//#include "adaboostest.inl"
#include "cxmisc.h"
#include "cv/cxlut.inl"
//#include "cv/cxcopy.inl"
#include "cv/cvhistogram.inl"
#include "cv/cvthresh.inl"
#include "cv/cxlogic.inl"
//#include "cv/cvcontours.inl"
//#include "cv/cvderiv.inl"
//#include "cv/cvsmooth.inl"
#include "img/color.inl"
#include "img/imedge.inl"
#include "img/imgopt.inl"
#include "GlobalFunction.inl"
#include "CharacterExtraction.inl"
#include "PrecisePosition.inl"
#include "imgopt2.inl"
#include "CascadeLocation.inl"
#include "plate_detect.inl"

typedef struct plate_info_t {
  CvRect rc;
  CvRect rcs[8];
  char text[8];
} plate_info_t;

int plate_recog(const img_t* img)
{
  CvRect rc[20];
  int i, n;
  int h = img->h, w = img->w;
  char text[7] = "12345";

  n = plate_detect(img, rc, countof(rc));

  for (i = n - 1; i >= 0; i--) {   //处理每一个矩形框
    //draw_rect_to_img(m_rect[i]);  //画出矩形框
    CvRect roi_rect = rc[i];
    CvRect rect;
    img_t roi_img[1] = {0};
    rect = roi_rect;
    roi_rect = extend_rect(h, w, roi_rect, 5);

    imrefrc(img, roi_rect, roi_img);
    //把图片roi_img送去精定位和分割
    character_cut_process(roi_img, text);
    printf("%s\n", text);
  }

  return 0;
}
