

#include "cstd.h"
#include "libvena.h"
#include "cmath.h"
#include "rot90.inl"
#include "cvthin.inl"
#include "thin.inl"
#include "edgecrop.inl"
#include "seqpartition.h"
#include "lbp.inl"
#include "img/color.inl"


int vena_detect(buf_t* bf, const img_t* im0, uchar* im3) {
  img_t im[10] = {0};
  int mm = im0->h, nn = im0->w, i, j, ii=0, ret=1;
  int col2[4]={0};
  img_t* im00 = im+ii++;
  img_t* im01 = im+ii++;
  img_t im1[1];
  img_t* shuru = im+ii++;
  img_t* shuru1 = im+ii++;
  img_t* BW2 = im+ii++;
  int t1=0, t2=0;
  const double h[] = {0.0113, 0.0838, 0.0113,0.0838, 0.6193, 0.0838,0.0113, 0.0838, 0.0113};

  if (3==im0->c) {
    bf_imcolorcvt(bf, im0, im00, T_BGR, T_GRAY);
    im0 = im00;
  }
  if (mm<nn) {
    bf_rot90(bf, im0, 1, im01);
    im0 = im01;
  }
  imsubref(im0, iRECT(0, 80, im0->w, 480), im1);
  bf_imdouble(bf, im1, shuru);
  
  bf_imfilter2(bf, shuru, h, 3, 3, shuru1);
  
  //imshowmat_f8(shuru);
  //imshowmat_f8(shuru1);
  bf_edgecrop(bf, shuru1, 50, BW2);
  //imshow(BW2); cvWaitKey(-1);
  {
    int *col, ncol=0, nc=0, k;
    for(i=0; i<BW2->h; ++i) {
      for (j=0, k=0; k<10 && j<BW2->w; ++j) {
        if (BW2->tt.data[i*BW2->s + j]) {
          nc++;
          ++k;
        }
      }
    }
    BFMALLOC(bf, col, nc);
    ncol=0;
    for(i=0; i<BW2->h; ++i) {
      for (j=0, k=0; k<10 && j<BW2->w; ++j) {
        if (BW2->tt.data[i*BW2->s + j]) {
          col[ncol++] = j;
          ++k;
        }
      }
    }
    if (ncol<100) {
      ret=0;
      //printf("ret = %d\n", __LINE__);
    }
    if (ret) {
      int col1[100];
      MEMCPY(col1, col, 100);
      memset(col2, 0, sizeof(col2));
      i = int_partition(ncol, col, 5, 10, 1, 4, col2, bf);
      if (i<2) {
        ret=0;
      } else {
        if (col2[1]<col2[0]) {
          CV_SWAP(col2[1], col2[0], i);
        }
        if (col2[1]-col2[0]<50 || col2[0]<0 || col2[1]>=shuru->w) {
          i = int_partition(ncol, col, 5, 10, 1, 4, col2, bf);
          ret = 0;
          //printf("col2 %d %d ret = %d\n", col2[0], col2[1], __LINE__);
        }
      }
    }
    BFFREE(bf, col, ncol);
  }
  
  if (ret) {
    double maxv;
    double* row = 0;
    double* row2 = 0;
    int nr = BW2->h+10;
    BFMALLOC(bf, row, nr);
    BFMALLOC(bf, row2, nr);
    for (i=0; i<nr; ++i) {
      row[i] = row2[i] = 0;
    }
    for(i=0; i<BW2->h; ++i) {
      double* ss = (double*)shuru->tt.data + i*BW2->s;
      for (j=col2[0]; j<col2[1]; ++j) {
        row[i] += ss[j];
      }
    }
    row2[0] = 0;
    for(i=0; i<BW2->h; ++i) {
      row2[i+1] = row2[i] + row[i];
    }
    maxv = 0;
    for(i=0; i<150; ++i) {
      double v = row2[i+50]-row2[i];
      if (v>maxv) {
        maxv = v;
        t1 = i;
      }
    }
    t2=BW2->h/2;
    maxv = 0;
    for(i=BW2->h-150; i<BW2->h-51; ++i) {
      double v = row2[i+50]-row2[i];
      if (v>maxv) {
        maxv = v;
        t2 = i;
      }
    }
    if (t2-t1<30) {
      ret=0;
      //printf("ret = %d\n", __LINE__);
    }
    BFFREE(bf, row2, nr);
    BFFREE(bf, row, nr);
  }
#if 0
  if (1 || VENA_DEBUG && ret) {
    img_t imrgb[1] = {0};
    imcolorcvt(im1, imrgb, T_GRAY, T_BGR);
    imdraw_line(imrgb, 0, iPOINT(col2[0], 0), iPOINT(col2[0], imrgb->h), _RGB(255, 0, 0), 1);
    imdraw_line(imrgb, 0, iPOINT(col2[1], 0), iPOINT(col2[1], imrgb->h), _RGB(255, 0, 0), 1);
    imdraw_line(imrgb, 0, iPOINT(0, t1), iPOINT(imrgb->w, t1), _RGB(255, 0, 0), 1);
    imdraw_line(imrgb, 0, iPOINT(0, t2), iPOINT(imrgb->w, t2), _RGB(255, 255, 0), 1);
    imshow(imrgb);
    imfree(imrgb);
    cvWaitKey(-1);
  }
#endif
  if (ret && im3) {
    img_t im2[1] = {0};
    int vmin=100000, vmax=0;
    IRECT rc = iRECT(col2[0]+3, t1, col2[1]-1, t2);
    imsubref(im1, rc, im2);
    //imshow(im1);  imshow(im2);    cvWaitKey(-1);
    //bf_imresize(bf, im2, 96, 64, im3);
    bf_imresize_impl(bf, im2->h, im2->w, im2->tt.data, im2->s, im2->c, VENA_STDH, VENA_STDW, im3, VENA_STDW, 1, CV_INTER_AREA);
    im_minmax_u8(VENA_STDH, VENA_STDW, im3, VENA_STDW, &vmin, &vmax);
    if (vmax>vmin) {
      for (i=0; i<VENA_STDH; ++i) {
        for (j=0; j<VENA_STDW; ++j) {
          int t = im3[VENA_STDW * i + j];
          im3[VENA_STDW * i + j] = (t-vmin)*255/(vmax-vmin);
        }
      }
    } else {
      ret=0;
      //printf("ret = %d\n", __LINE__);
    }
  }
  bf_imfrees(bf, im, 10);
  return ret;
}

int vena_recog(buf_t* bf, const img_t* im, const buf_t* lib, uchar* imstd, uchar* feat, int* pid, int* pdis) {
  int detect_ok = vena_detect(bf, im, imstd);
  int kid=-1, i, dmin=10000000;
#define GETARG(p, T)   (T*)(p+=sizeof(T), (p-sizeof(T)))
#define GETARGS(p, T, n)   (T*)(p+=(n)*sizeof(T), (p-(n)*sizeof(T)))
  vena_feat(imstd, feat);
  if (lib && lib->len>20) {
    const char* p = (const char*)lib->data;
    int n = *GETARG(p, int);
    int step = *GETARG(p, int);
    const uchar* featlib = (const uchar*)p;
    ASSERT(step>=VENA_FEATLEN);
    for (i=0; i<n; ++i) {
      int d = abssub_u8(VENA_FEATLEN, feat, featlib+i*step);
      if (d<dmin) {
        dmin = d;
        kid = i;
      }
    }
  }
  *pid = kid;
  *pdis = dmin;
  return detect_ok;
}

__declspec(dllexport) int  __stdcall vena_recog_bf(unsigned char* buf, int buflen, int h, int w, unsigned char* img, int imgstep, int imgcn,
                                                   int n, unsigned char* featlib, int featstep, unsigned char* imstd, unsigned char* feat, int* pid, int* pdis) {
  int detect_ok;
  int kid=-1, i, dmin=10000000;
  buf_t bf[1] = {0};
  img_t im[1] = {0};
  bfinit(bf, buf, buflen);
  IMINIT(im, h, w, img, imgstep, imgcn, 1);
  detect_ok = vena_detect(bf, im, imstd);
  if (feat) {
    vena_feat(imstd, feat);
    if (featlib) {
      ASSERT(featstep>=VENA_FEATLEN);
      for (i=0; i<n; ++i) {
        int d = abssub_u8(VENA_FEATLEN, feat, featlib+i*featstep);
        if (d<dmin) {
          dmin = d;
          kid = i;
        }
      }
    }
  }
  *pid = kid;
  *pdis = dmin;
  return detect_ok;
}

int bf_pushback(buf_t* bf, int headerlen, const void* data, int len) {
  int oldlen = MAX(bf->len, headerlen);
  bfsetsize(bf, oldlen+len);
  if (oldlen==headerlen) {
    memset(bf->data, 0, headerlen);
  }
  if (data) {
    memcpy(bf->data+oldlen, data, len);
  }
  return 0;
}

#include "img/imgio.inl"


void* load_image(const char* fn, int cn_req, int* ph, int* pw, int* pstep) {
  img_t im[1] = {0};
  if (imread(fn, cn_req, 1, im)) {
    int sz = im->h * im->w * cn_req;
    *ph = im->h;
    *pw = im->w;
    *pstep = im->s;
    return im->tt.data;
  }
  return NULL;
}

int save_image(const char* fn, int h, int w, const void* data, int step, int cn) {
  return imwrite4(fn, h, w, (uchar*)data, step, cn);
}
void free_image(void* data) {
  free(data);
}

typedef struct venarecog_param_t {

} venarecog_param_t;

int venarecog_set(venarecog_param_t* s)
{
  return 0;
}


int venarecog_run(venarecog_param_t* s, const img_t* im, int pixtype, int mode)
{
  return 0;
}

int vena_recog(int h, int w, const void* data, int step, int cn, const char* s_pixtype, const char* s_mode, int is_trans, double scale,
               int* xywh, void* stdvena, void* feature, const void* featlib, int featstep, int featnum, int* pid, int* pdis) {
  return 0;
}

//  int xywh[12] = {0};
//  h 图像高 w 图像宽 data图像数据 step图像每行字节数 cn图像通道数
// s_pixtype GRAY 灰度图 BGR|RGB|BGRA|BGR555|BGR565 彩色图 YUYV|UYVY 其他格式
// is_trans 是否翻转90度
// scale 图像预缩放
// xywh 输出 人脸[x, y, w, h] 左眼[x, y, w, h] 右眼[x, y, w, h]
//  vena_detect_defaut(im->h, im->w, im->tt.data, im->s, im->c, "gray", trans, 1, xywh);
//  printf("%d %d\n", xywh[0], xywh[1]);
int vena_detect_defaut(int h, int w, const void* data, int step, int cn, const char* s_pixtype, int is_trans, double scale, int* xywh) {
  return 1;
}

int venafeature(const char* pic_100x100_file_name, void* out_feature) {
  return 1;
}

