
#include "asmfit.inl"

#define POINT_NUM  101

static uint asmTrainData_all[] = {
#include "asmTrainData_all.inl"
};

#include "img/imgopt.inl"


#if 1
#include "dnn/cnn/cnn.h"
#include "dnn/cnn/mtcnn.h"
enum FaceLandmarkId {
  FLI_LEYE_CENTER,
    FLI_REYE_CENTER,
    FLI_LEYE_LCONER,
    FLI_LEYE_RCONER,
    FLI_REYE_LCONER,
    FLI_REYE_RCONER,
    FLI_LBROW_LCONER,
    FLI_LBROW_RCONER,
    FLI_RBROW_LCONER,
    FLI_RBROW_RCONER,
    FLI_MOUTH_CENTER,
    FLI_MOUTH_LCONER,
    FLI_MOUTH_RCONER,
    FLI_MOUTH_BOTTOM,
    FLI_NOSE_TIP,
    FLI_COUNT
};


static int landmark_range_101[] = {
  0,//×óÃ¼
    10,//×óÑÛ
    18,//ÓÒÃ¼
    28,//ÓÒÑÛ
    36,//±Ç×Ó
    49,//Íâ×ì
    61,//ÄÚ×ì
    73,//ÍâÂÖÀª
    101
};
static FPOINT get_center(const FPOINT* pt, int n) {
  FPOINT pc = {0};
  int i;
  if (n>0) {
    for (i=0; i<n; ++i) {
      pc.x += pt[i].x;
      pc.y += pt[i].y;
    }
    pc.x /= n;
    pc.y /= n;
  }
  return pc;
}


static int getfid_101(const FPOINT* pt101, FPOINT* fpt) {
  fpt[FLI_LEYE_CENTER] = get_center(pt101+10, 8);
  fpt[FLI_REYE_CENTER] = get_center(pt101+28, 8);
  fpt[FLI_MOUTH_CENTER] = get_center(pt101+49, 12);
  fpt[FLI_NOSE_TIP] = get_center(pt101+40, 6);
  
  fpt[FLI_LEYE_LCONER] = pt101[10];
  fpt[FLI_LEYE_RCONER] = pt101[14];
  fpt[FLI_REYE_LCONER] = pt101[28];
  fpt[FLI_REYE_RCONER] = pt101[32];

  fpt[FLI_LBROW_LCONER] = pt101[0];
  fpt[FLI_LBROW_RCONER] = pt101[5];
  fpt[FLI_RBROW_LCONER] = pt101[18];
  fpt[FLI_RBROW_RCONER] = pt101[22];

  fpt[FLI_MOUTH_LCONER] = pt101[49];
  fpt[FLI_MOUTH_RCONER] = pt101[55];
  fpt[FLI_MOUTH_BOTTOM] = pt101[58];
  return 0;
}
static IPOINT fPOINT2i(FPOINT fpt) {
  IPOINT ipt;
  ipt.x = fpt.x;
  ipt.y = fpt.y;
  return ipt;
}
static FPOINT* facelandmark(const img_t* im, FPOINT* pt101, const FPOINT* ptsrc4, CPoint2f* out) {
  asmfit_param_t p[1] = {0};
  FPOINT* ret = NULL;
  img_t gray[1] = {0};
  if (im->c==3) {
    im2gry(im, gray);
  } else {
    imclone2(im, gray);
  }
  p->height = im->h, p->width = im->w, p->data = gray->tt.data, p->datastep = gray->s;
  //p->leye = leye, p->reye = reye;
  p->asm_mode = asmTrainData_all;
  if (NULL==p->buf) {
    asmfit_memsize(&p);
    //ap->shape_result = MALLOC(CvPoint2D32f, ap->shape_result_len);
    p->buf = MALLOC(char, p->buflen);
  }
  
  int height = p->height, width = p->width;
  const unsigned char* gry = p->data;
  //imwrite("AAAA.bmp",height, width, gry, width, 1);
  if (ptsrc4) {
    p->leye = fPOINT2i(ptsrc4[0]);
    p->reye = fPOINT2i(ptsrc4[1]);
    //ret = p->shape_result = pt101;
    //asmfit_process(p);
    ret = pt101;
  } else {
    enum { BUFLEN = 100 };
    Bbox out[ BUFLEN ];
  FPOINT _flsrc[101] = {0};
    int face_cnt = mtcnn_findFace( im, out, countof(out), 60 );
    if ( face_cnt > 0 ) {
      Bbox * face = out;
      float* pt = face->ppoint;
      p->leye = iPOINT(pt[0], pt[5]);
      p->reye = iPOINT(pt[1], pt[6]);
      ret = p->shape_result = pt101;
      asmfit_process(p);
    }
  }
  if (ret && out) {
    CPoint2f _pt[FLI_COUNT] = {0};
    if (ptsrc4) {
      out[0] = ptsrc4[0];
      out[1] = ptsrc4[1];
      out[2] = ptsrc4[2];
    } else {
      getfid_101(pt101, _pt);
      out[0] = _pt[FLI_LBROW_LCONER];
      out[1] = _pt[FLI_RBROW_RCONER];
      out[2] = _pt[FLI_MOUTH_BOTTOM];
    }
  }
  FREE(p->buf);
  return ret;
}
#endif
