
#include "cstd.h"
//#include "str.h"
#include "str/scanf.inl"
#include "img/imgio.inl"
#include "ui/window.inl"
#include "draw/imdraw.inl"
#include "img/imgopt.inl"

#include "lbf_face_alignment.inl"
#include "pic/pic.inl"

int lbf_draw_shape68(img_t* im, DRECT rc, const DPOINT* pt68, COLOR clr, int delay) {
  int i, npt = 68;
  imdraw_dpoint(im, 0, NULL, pt68, npt, clr);
  for (i=0; i<npt; ++i) {
    imdrawaa_circle(im, NULL, pt68[i].x, pt68[i].y, 2, _rgba(1, 0, 0, 0.5), _rgba(0, 0, 1, 0.5), 0);
  }
  imdrawaa_rect(im, 0, NULL, rc, 0, clr, 2);
  //im_trans(im2, im2, ISROT3);
  //imshow_(im2);WaitKey(delay);
  return 0;
}
int loadimglist(const char* txtlist, int maxim, img_t* im, int cn) {
  int n, i;
  vstr_t sv[1] = {0};
  vstr_load(txtlist, sv);
  n = sv->n;
  if (im) {
    n = MIN(n, maxim);
    for (i=0; i<n; ++i) {
      imread(sv->v[i].s, cn, 1, im+i);
    }
  }
  return n;
}
int loadpts(const char* txtlist, int maxim, DPOINT* pts, int npt) {
  int n, i, ipos;
  vstr_t sv[1] = {0};
  vstr_load(txtlist, sv);
  n = sv->n;
  if (pts) {
    char buf[256];
    str_t str[1] = {0};
    n = MIN(n, maxim);
    for (i=0; i<n; ++i) {
      strcpy(buf, sv->v[i].s);
      strcpy(strrchr(buf, '.'), ".pts");
      str_load(buf, str);
      ipos = str_skip_line(str->s, str->l, 0, 3);
      str_scanf_arr(str->s, str->l, ipos, "%lf%lf", npt, pts+i*npt);
    }
    str_free(str);
  }
  return n;
}
int loaddata_txt(const char* datatxt, const char* fmt, int maxdatalen, void* data) {
  int i, pos=0;
  str_t s[1] = {0};
  char* cdata = (char*)data;
  str_load(datatxt, s);
  ASSERT(s->l>0);
  for (i=0; i<maxdatalen; ++i) {
    pos = str_vscanf(s->s, s->l, pos, fmt, cdata, &cdata);
  }
  return 0;
}

#include "lbf_face_alignment.inl"
#include "test/test_asm.inl"
#include "cap.h"
#include "face/face.inl"
#include "face/cascadearr.h"

struct lbf_face_alignment_t {
  buf_t bf[2];
};

int lbf_face_alignment_init(lbf_face_alignment_t* s) {
  buf_t* bf = s->bf;
  buf_t* bf16 = bf+1;
  buf_load("train_model_new.dat", bf);
  buf_load("train_model_new_fix16.dat", bf16);
  return 0;
}

int lbf_face_alignment_free(lbf_face_alignment_t* s) {
  buf_t* bf = s->bf;
  buf_t* bf16 = bf+1;
  buf_load("train_model_new.dat", bf);
  buf_load("train_model_new_fix16.dat", bf16);
  bffrees(s->bf);
  return 0;
}

int lbf_face_alignment(lbf_face_alignment_t* s, img_t* img, DRECT* rc0, DRECT* prc, DPOINT* pt_68, int isfix16) {
  int i, ret = 0;
  static int inited = 0;
  double error_of_image;
  img_t* imgTe_gray = NULL;
  buf_t* bf = s->bf;
  buf_t* bf16 = bf+1;
  IM_BEGIN(im, 4);
  imgTe_gray = im2gry(img, im);
  XRECT out[100];
  double b[4];
  if (!rc0) {
    ret = CPM_FaceDetect(vis_nesting_face20110713, imgTe_gray->rows, imgTe_gray->cols,
      imgTe_gray->tt.data, imgTe_gray->step, NULL, 0, 0.4, 1, 100, 1.1, 1, 0.8, 5, out, 100);
    i=0;
    prc->l = b[0] = out[i].x;
    prc->r = b[1] = out[i].x+out[i].w;
    prc->t = b[2] = out[i].y;
    prc->b = b[3] = out[i].y+out[i].h;
  } else {
    ret = 1;
    *prc = *rc0;
    b[0] = prc->l;
    b[1] = prc->r;
    b[2] = prc->t;
    b[3] = prc->b;
  }
  
  if (ret) {
    // l r t b
    int flag = ISTRANS;
    enum {npt=68};
    if (flag == ISTRANS) {
      img = im_trans(img, im++, flag);
    }
    if (flag&ISTRANS) {
      double t;
      CC_SWAP(b[0], b[2], t);
      CC_SWAP(b[1], b[3], t);
    }
    imgTe_gray = im2gry(img, im++);
    ret = !!ret;
    double shape[npt*2];
    for (i=0; i<ret; ++i) {
      if (isfix16) {
        lbf_face_alignment_fix16(bf16->data, img, imgTe_gray, b, shape, flag);
      } else {
        lbf_face_alignment_64f(bf->data, img, imgTe_gray, b, shape, flag);
      }
    }
    for (i=0; i<npt; ++i) {
      pt_68[i].y = shape[npt*0 + i];
      pt_68[i].x = shape[npt*1 + i];
    }
  }
  
  IM_END(im);
  return ret;
}


int test_lbf_face_alignment() {
  enum {MAXIM=50};
  img_t im[MAXIM] = {0};
  int i, n;
  double box[MAXIM*4] = {0};
  DPOINT pts[MAXIM*68] = {0};
  DPOINT pt_68[68];
  double error_of_image;
  int npt = 68;
  //int adf = test_get_sp();
  sys_chdir("F:/pub/bin/face/lbf_face_alignment");
  n = loadimglist("Path_Images.txt", MAXIM, im, 3);
  loadpts("./test/LFPW/Path_Images.txt", MAXIM, pts, npt);
  loaddata_txt("./test/LFPW/box.txt", "%lf", 4*n, box);
  //buf_load("D:/pub/bin/lbf_face_alignment/train_model_new.dat", bf);
  //buf_load("//192.168.1.5/d/matlabwork/lbf_face_alignment/train_model_new.dat", bf);

  lbf_face_alignment_t lbf[1] = {0};
  //int adf = test_get_sp();
  lbf_face_alignment_init(lbf);

  if (1) {
    capdev cap[1] = {0};
    
    if (capdshow_open(cap, 0, 640, 480) > 0) {
      int fram = 0;
      char ch = 0;
      img_t im[1] = {0};
      
      for (; 'q' != ch && 'Q' != ch; ++fram) {
        cap_getframe(cap, im, CAP_RGB);
        DRECT rc;

        //printf("%d w=%d h=%d\n", fram, im->w, im->h);
        int ret = 0;
        tic;
        ret = lbf_face_alignment(lbf, im, NULL, &rc, pt_68, 0);
        toc_;
        if (ret) {
          lbf_draw_shape68(im, rc, pt_68, _rgb(1,0,1), -1);
        }
        imshow_(im);
        ch = WaitKey(10);
      }
      
      cap_close(cap);
      imfree(im);
    }
  }
  for (i=0; i<n; ++i) {
    double* b = box+i*4;
    if (0) {
      //imdrawaa_rect(im+i, 0, b[0], b[2], b[1], b[3], 0, rgb(1, 0, 0), 1);
      //imshow(im+i);cvWaitKey(-1);
    }
    //imshow(im+i);cvWaitKey(-1);
    //im_trans(im+i, im+i, flag);
    DRECT rc = dRECT(b[0], b[2], b[1], b[3]);
    {
      utime_start(_start_time);
      int ret = lbf_face_alignment(lbf, im, &rc, &rc, pt_68, 0);
      error_of_image = lbf_compute_error(npt, pts+i*npt, pt_68);
      printf("%3d, %8.5f %8.5f\n", i, utime_elapsed(_start_time), error_of_image);
    }
    //im_trans(im+i, im+i, ISTRANS);
    lbf_draw_shape68(im+i, rc, pt_68, _rgb(1,0,1), -1);
    imshow_(im+i);WaitKey(-1);
  }
  freeims(im, n);

  return 0;
}

int test_lbf_face_alignment_lena() {
  enum {MAXIM=5};
  buf_t bf[1] = {0};
  DPOINT pt[68] = {0};
  int npt = 68;
  img_t imgTe_gray[1] = {0};
  lbf_face_alignment_t lbf[1] = {0};
  sys_chdir("F:/pub/bin/face/lbf_face_alignment");
  //int adf = test_get_sp();
  lbf_face_alignment_init(lbf);
  {
    int trans;
    int i;
    trans = 0;
    trans = ISTRANS;
    if (0) {
      //imdrawaa_rect(im+i, 0, b[0], b[2], b[1], b[3], 0, rgb(1, 0, 0), 1);
      //imshow(im+i);cvWaitKey(-1);
    }

    IMINIT(imgTe_gray, 144, 180, gry_180x144, 180, 1, 1), trans=1;
    IMINIT(imgTe_gray, 128, 128, lena_128x128, 128, 1, 1), trans=0;

    DRECT rc = dRECT(25, 44, 25+61, 44+61);
    //imshow(im+i);cvWaitKey(-1);
    for (i=0; i<10; ++i) {
      {utime_start(_start_time);
      lbf_face_alignment(lbf, imgTe_gray, &rc, &rc, pt, 0);
      printf("%8.5f\n", utime_elapsed(_start_time));}
    }
    lbf_draw_shape68(imgTe_gray, rc, pt, _rgb(1,0,1), -1);

  }
  bffree(bf);
  return 0;
}

