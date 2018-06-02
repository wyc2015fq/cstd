
#include <stdio.h>
#include "cstd.h"
#include "img/imgio.inl"
#include "face_recog.inl"
#include "geo/vcgen.inl"

#define FACEREC_THD  0.475

//#include "inifile.h"
//#include "draw/imdraw.inl"
#include "font/font.inl"

#include "DSP_feat.h"
int strmatrix_count(const char* strs, int n, int step, const unsigned int* hash, const char* name, const time_t* ptime, int* mintime) {
  int i=0, cnt = 0;
  int len = strlen(name);
  time_t time1=0;
  const char* chash = (const char*)hash;
  const char* ctime = (const char*)ptime;
  if (hash) {
    uint h = SDBMHash(name, len);
    for (;i<n;++i) {
      if (h==*(uint*)(chash+i*step) && 0==strcmp(strs + i*step, name)) {
        ++cnt;
        if (mintime && (0==time1 || time1 > *(time_t*)(ctime+i*step)) ) {
          time1 = *(time_t*)(ctime+i*step);
          *mintime = i;
        }
      }
    }
  } else {
    for (;i<n;++i) {
      if (0==strcmp(strs + i*step, name)) {
        ++cnt;
        if (mintime && (0==time1 || time1 > *(time_t*)(ctime+i*step)) ) {
          time1 = *(time_t*)(ctime+i*step);
          *mintime = i;
        }
      }
    }
  }
  return cnt;
}

double facefeature_dist(const void* feature1, const void* feature2) {
  int dis = CPM_AbsSub(FEATURE_LEN/2, (const FEAT_T*)feature1, (const FEAT_T*)feature2);
  double ddis = dis/25000.;
  return ddis;
}
int test_face_detect_defaut(const char* pic_file_name, int is_trans, double scale, int* xywh) {
  img_t im[1] = {0};
  imread(pic_file_name, 1, 1, im);
  face_detect_defaut(im->h, im->w, im->tt.data, im->s, im->c, "gray", is_trans, scale, xywh);
  imfree(im);
  return 0;
}
int test_facefeature_dist() {
  enum {MAXFEATLEN  = 5000 };
  enum {maxpics = 10};
  char pics[maxpics][256];
  char feats[maxpics][MAXFEATLEN] = {0};
  double dis;
  int i, j;
  const char* listname = "test_libfacefeature_list.txt";
  FILE* pf = fopen(listname, "rb");
  if (!pf) {
    printf("错误: %s 文件打开失败!\n", listname);
    return 0;
  }
#define TTT(ch)   ('\n'==(ch) || '\r'==(ch) || '\t'==(ch) || ' '==(ch))
  for (i=0; i<maxpics && fgets(pics[i], 256, pf); ++i) {
    char* pic = pics[i];
    while(strlen(pic)>0 && TTT(pic[strlen(pic)-1])) {
      pic[strlen(pic)-1] = 0;
    }
    if (facefeature(pics[i], feats[i])) {
      printf("错误: 图片 %s 不是100x100的\n", pics[i]);
      return 1;
    }
    for (j=0; j<i; ++j) {
      dis = facefeature_dist(feats[i], feats[j]);
      printf("结果: 图片 %s 与图片 %s 的距离为 %f\n", pics[i], pics[j], dis);
    }
  }
#undef TTT
  return 0;
}


int facelib_get_feat_num(const buf_t* bf)
{
  int n = 0;

  if (bf->len > 4) {
    n = *bfdata(bf, int);
  }

  return n;
}
SPInfo* facelib_get_feat(const buf_t* bf, int i)
{
  SPInfo* sp = (SPInfo*)(bf->data + 4);
  return sp + i;
}
SPFaceRegInfo* facelib_get_facereginfo(const buf_t* bf, int i)
{
  int n = facelib_get_feat_num(bf);
  SPFaceRegInfo* sp = (SPFaceRegInfo*)(bf->data + 4 + n * sizeof(SPInfo));
  return sp + i;
}
int facelib_find(buf_t* bf, const char* szId)
{
  int i, n;

  if (bf->len > 4) {
    SPInfo* sp = facelib_get_feat(bf, 0);
    n = facelib_get_feat_num(bf);

    for (i = 0; i < n; ++i) {
      if (0 == cstr_icmp(szId, -1, SPInfoID(sp + i, 0), -1, 1)) {
        return i;
      }
    }
  }

  return -1;
}
int memrcpy(void* d, const void* s, int n)
{
  const char* cs = (const char*)s;
  char* ds = (char*)d;

  for (; --n;) {
    ds[n] = cs[n];
  }

  return 0;
}
int facelib_add(buf_t* bf, const SPInfo* data, const SPFaceRegInfo* pic)
{
  const char* szId = SPInfoID(data, 0);
  int i = facelib_find(bf, szId);
  int n = facelib_get_feat_num(bf);
  SPInfo* pf;
  SPFaceRegInfo* pr;

  if (i < 0) {
    i = facelib_get_feat_num(bf);
    bfsetsize(bf, 4 + (i + 1) * (sizeof(SPInfo) + sizeof(SPFaceRegInfo)));
    n += 1;
    *bfdata(bf, int) = n;
    pf = facelib_get_feat(bf, n - 1);
    memmove(pf + 1, pf, n * sizeof(SPFaceRegInfo));
  }

  pf = facelib_get_feat(bf, i);
  //memmove(pf+1, pf, n*sizeof(SPFaceRegInfo)+(n-i-1)*sizeof(SPInfo));
  memcpy(pf, data, sizeof(SPInfo));
  pr = facelib_get_facereginfo(bf, i);
  memcpy(pr, pic, sizeof(SPFaceRegInfo));
  memcpy(pr->m_PID.m_ID, szId, ID_LEN);
  bf->len = 4 + n * (sizeof(SPFaceRegInfo) + sizeof(SPInfo));
  return 0;
}
int facelib_get_libdata(buf_t* bf, buf_t* bflib)
{
  int i, j, n = facelib_get_feat_num(bf), k = 0;
  SPInfo* pi;

  for (i = 0; i < n; ++i) {
    SPFaceRegInfo* pr = facelib_get_facereginfo(bf, i);

    if (pr->m_Flag) {
      ++k;
    }
  }

  if (0 == k) {
    return 0;
  }

  bfsetsize(bflib, sizeof(int) + k * sizeof(SPInfo));
  pi = facelib_get_feat(bf, 0);
  j = 0;
  *bfdata(bflib, int) = k;

  for (i = 0; i < n; ++i) {
    SPFaceRegInfo* pr = facelib_get_facereginfo(bf, i);

    if (pr->m_Flag) {
      memcpy(bflib->data + 4 + j * sizeof(SPInfo), pi + i, sizeof(SPInfo));
    }
  }

  return k;
}
int facelib_del_byid(buf_t* bf, int i) {
  int n = facelib_get_feat_num(bf);
  
  if (i >= 0) {
    SPInfo* pf = facelib_get_feat(bf, 0);
    SPFaceRegInfo* pr = facelib_get_facereginfo(bf, 0);
    memcpy(pf + i, pf + i + 1, (n - i - 1)*sizeof(SPInfo));
    memcpy(pr + i, pr + i + 1, (n - i - 1)*sizeof(SPFaceRegInfo));
    memcpy(pf+n-1, pr, (n - 1)*sizeof(SPFaceRegInfo));
    *bfdata(bf, int) = n - 1;
    bf->len = 4 + (n - 1) * (sizeof(SPFaceRegInfo) + sizeof(SPInfo));
  }
  return 0;
}
int facelib_del_byname(buf_t* bf, const char* szId)
{
  int i = facelib_find(bf, szId);
  facelib_del_byid(bf, i);
  return 0;
}
typedef struct faceapp_t {
  int rec_ok;
  int mode;
  int m_nsp;
  //char* m_strEdit;
  //int m_strEditLen;
  int64 pretime;
  buf_t m_fl[1];
  buf_t m_rl[1];
  SPInfo m_sp[1];   // 锟斤拷锟斤拷锟斤拷息
  SPFaceRegInfo m_pic[1]; // 注锟斤拷锟斤拷息
  uchar stdface[STDIMGH* STDIMGW];
} faceapp_t;

#include "sftime.h"

int imFaceStd(const img_t* A, img_t* B, IPOINT leye, IPOINT reye, double lx, double rx, double y)
{
  CPM_FaceStd(A->h, A->w, A->tt.data, A->s, A->c, B->h, B->w, B->tt.data, B->s, B->c, leye, reye, lx, y, rx, y);
  return 0;
}
#define facelib_path() make_module_path("facelib.dat")
#define RECPATH make_module_path("rec.dat")
#define FACELIB facelib_path()
char* make_module_path(const char* fn)
{
  static char str_facelib_path[1024]={0};
  sys_get_exe_path(str_facelib_path, 1024);
  strcpy(GetFileNameExt(str_facelib_path), fn);
  return str_facelib_path;
}

int faceapp_get_stdface(faceapp_t* g, facerecog_param_t* m_fr, const img_t* m_imcap, uchar* stdface) {
  img_t im2[1] = {0};
  IPOINT lept, rept;
  utime_restart(g->pretime);
  IMINIT(im2, STDH, STDW, stdface, STDSTEP, STDCN, 1);
  lept = iPOINT(m_fr->leye.x + m_fr->leye.w / 2, m_fr->leye.y + m_fr->leye.h / 2);
  rept = iPOINT(m_fr->reye.x + m_fr->reye.w / 2, m_fr->reye.y + m_fr->reye.h / 2);
  imFaceStd(m_imcap, im2, lept, rept, 0.3, 0.7, 0.3);
  return 0;
}
int faceapp_add_rec(faceapp_t* g, facerecog_param_t* m_fr, const img_t* m_imcap, int id, double score) {
  SPFaceRecInfo* frc = (SPFaceRecInfo*)g->m_rl->data;
  int nfrc = g->m_rl->len/sizeof(SPFaceRecInfo);
  int doadd = 0==nfrc;
  time_t time1 = time(NULL);
  int k = id / SPFEAT_LEN;
  SPInfo* sp = facelib_get_feat(g->m_fl, k);
  if (nfrc>0) {
    double tdf = difftime(time1, frc[nfrc-1].time);
    //printf("%f\n", tdf);
    doadd = (tdf>5 || (tdf>2 && 0!=memcmp(SPInfoID(sp, 0), frc[nfrc-1].m_PID.m_ID, ID_LEN)));
  }
  if (doadd) {
    bfsetsize(g->m_rl, (nfrc+1)*sizeof(SPFaceRecInfo));
    frc = (SPFaceRecInfo*)g->m_rl->data;
    faceapp_get_stdface(g, m_fr, m_imcap, frc[nfrc].m_data);
    memcpy(frc[nfrc].m_PID.m_ID, SPInfoID(sp, 0), ID_LEN);
    frc[nfrc].score = score;
    frc[nfrc].time = time1;
    buf_save(RECPATH, g->m_rl);
  }

  return 0;
}

int faceapp_run(faceapp_t* g, facerecog_param_t* m_fr, const img_t* m_imcap, const char* m_strEdit)
{
  facerecog_set(m_fr);

  if (m_strEdit && *m_strEdit && FR_FACE_REG == g->mode) {
    char* str;
    int m_strEditLen = strlen(m_strEdit);
    m_strEditLen = MIN(m_strEditLen, ID_LEN);
    str = SPInfoID(g->m_sp, g->m_nsp);
    memcpy(str, m_strEdit, m_strEditLen);
    str[m_strEditLen] = 0;
  }

  m_fr->featlib = (FEAT_T*)facelib_get_feat(g->m_fl, 0);
  m_fr->featnum = facelib_get_feat_num(g->m_fl) * SPFEAT_LEN;
  m_fr->featstep = sizeof(SPFeatInfo);
  m_fr->stdface = g->stdface;
  m_fr->reye.w = 0;
  m_fr->feature = (FEAT_T*)g->m_sp[0].m_PFeatArry[g->m_nsp].m_Feature;
  facerecog_run(m_fr, m_imcap, T_BGR, g->mode);

  //printf("%d\n", m_fr->face.w);
  g->rec_ok = 0;

  if (FR_FACE_REG == g->mode && utime_elapsed(g->pretime) > 0.1 && m_fr->reye.w) {
    faceapp_get_stdface(g, m_fr, m_imcap, g->m_pic->m_data);
    ++g->m_nsp;

    if (g->m_nsp == SPFEAT_LEN) {
      g->m_nsp = 0;
      g->mode = FR_FACE_DET;
      g->mode = FR_FACE_REC;
      g->m_pic->m_Flag = 1;
      facelib_add(g->m_fl, g->m_sp, g->m_pic);
      buf_save(FACELIB, g->m_fl);
    }
  }
  else if (FR_FACE_REC == g->mode && m_fr->reye.w && m_fr->featnum>0) {
    int id = m_fr->maxdist_id;
    double score = m_fr->maxdist / 25000.;

    if (score < FACEREC_THD) {
      g->rec_ok = 1;
      faceapp_add_rec(g, m_fr, m_imcap, id, score);
    }
  }

  return 0;
}

int xrect2rc(IRECT imrc, int h, int w, XRECT xrc, IRECT* rc)
{
  rc->l = imrc.l + RCW(&imrc) * xrc.x / w;
  rc->r = imrc.l + RCW(&imrc) * (xrc.x + xrc.w) / w;
  rc->t = imrc.t + RCH(&imrc) * xrc.y / h;
  rc->b = imrc.t + RCH(&imrc) * (xrc.y + xrc.h) / h;
  return 0;
}

int imdraw_comprometro(img_t* im, IRECT imrc)
{
  DPOINT pt[1000];
  int lens[10];
  int i, n, pplen;
  path_point pp[200] = {0};
  double ss = RCH(&imrc) / 200.;
  const char* sss = "M23.728,31.152c-1.234,0.42-2.547,0.658-3.922,0.658c-6.724,0-12.177-5.453-12.177-12.176"
      "c0-2.552,0.794-4.912,2.136-6.867V6.215C5.686,9.274,3.04,14.142,3.04,19.634c0,9.26,7.507,16.765,16.766,16.765"
      "c2.988,0,5.792-0.785,8.222-2.153h0.003v-0.004v-5.648v-3.206V15.057l-4.303,4.577V31.152z"

      "M19.806,2.867c-3.059,0-5.927,0.824-8.396,2.256l0,0v5.706v2.94v11.22l4.284-4.462V8.182"
      "c1.287-0.459,2.667-0.726,4.111-0.726c6.727,0,12.178,5.451,12.178,12.178c0,2.569-0.808,4.938-2.169,6.9v6.544"
      "c4.1-3.057,6.76-7.936,6.76-13.444C36.574,10.373,29.067,2.867,19.806,2.867z"
      "M21.846,8.923L17.703,13.511L17.703,30.344L21.846,26.168z";
  pplen = countof(pp);
  pplen = vcgen_path_str(sss, pp, countof(pp));

  for (i = 0; i < pplen; ++i) {
    pp[i].x *= ss, pp[i].y *= ss;
  }

  for (i = 0; i < pplen; ++i) {
    pp[i].x += imrc.l, pp[i].y += imrc.t;
  }

  n = vcgen_path(pp, pplen, countof(pt), pt, lens);
  imdrawaa_poly_solid(im, 0, pt, lens, n, _rgba(1, 0, 0, 0.4));
  return 0;
}

int faceapp_draw(img_t* im, const IRECT* pimrc, const img_t* m_imcap, const faceapp_t* g, const facerecog_param_t* m_fr, const font_t* font)
{
  COLOR _clr;
  char buf[256];
  IRECT imrc = pimrc ? *pimrc : iRECT(0, 0, im->w, im->h);
  //imdraw_comprometro(im, imrc);

  if (m_fr->face.w) {
    int i;
    IRECT rc;
    XRECT xrc[3];
    _clr = FR_FACE_REG == g->mode ? _RGB(255, 255, 255) : _RGB(255, 0, 0);
    xrc[0] = m_fr->face, xrc[1] = m_fr->leye, xrc[2] = m_fr->reye;

    for (i = 0; i < 3; ++i) {
      if (xrc[i].w) {
        xrect2rc(imrc, m_imcap->h, m_imcap->w, xrc[i], &rc);
        imdraw_rect(im, &imrc, rc, 0, _clr, 2, BDM_ALL);
      }
    }

    if (FR_FACE_REG == g->mode) {
      img_t im2[1] = {0};
      _clr = _RGBA(0, 0, 255, 128);
      rc = imrc;
      rc.b = rc.t + 30;
      imdraw_rect(im, 0, rc, _rgba(0, 0, 0, 0.4), 0, 0, BDM_ALL);
      rc.r = rc.l + RCW(&rc) * g->m_nsp / SPFEAT_LEN;
      imdraw_rect(im, 0, rc, _clr, 0, 0, BDM_ALL);
      rc = imrc;
      rc.b = rc.t + 30;
      _snprintf(buf, 256, "%d/%d", g->m_nsp, SPFEAT_LEN);
      imdraw_text(im, &imrc, rc, buf, -1, font, _rgb(1,1,1), 0, TF_VCENTER|TF_CENTER);
      IMINIT(im2, STDH, STDW, g->m_pic->m_data, STDSTEP, STDCN, 1);
      rc = iRECT(imrc.l, imrc.t, imrc.l+STDW, imrc.t+STDH);
      imdraw_image(im, &imrc, &rc, im2, 0);
    }
    else if (FR_FACE_REC == g->mode) {
      int id = m_fr->maxdist_id;
      double score = m_fr->maxdist / 25000.;

      if (id >= 0) {
        img_t im2[1] = {0};
        int k = id / SPFEAT_LEN;
        SPInfo* sp = facelib_get_feat(g->m_fl, k);
        SPFaceRegInfo* pr = facelib_get_facereginfo(g->m_fl, k);
        _clr = _RGBA(0, 0, 0, 128);
        xrect2rc(imrc, m_imcap->h, m_imcap->w, m_fr->face, &rc);
        rc.b = rc.t + 30;
        iRectInflate2(&rc, -1, -1);
        imdraw_rect(im, 0, rc, _clr, 0, 0, BDM_ALL);
        _clr = score < FACEREC_THD ? _RGB(0, 255, 0) : _RGB(255, 255, 255);
        _snprintf(buf, 256, "%d,%s,%.3f", m_fr->face.w, SPInfoID(sp, 0), score);
        //_snprintf(buf, 256, "%s:%.3f", SPInfoID(sp + k, 0), score);
        imdraw_text(im, 0, rc, buf, -1, font, _clr, 0, TF_VCENTER);
        IMINIT(im2, STDH, STDW, pr->m_data, STDSTEP, STDCN, 1);
        rc.l = rc.r - RCH(&rc)*STDW/STDH;
        imdraw_image(im, &imrc, &rc, im2, 0);
        rc = iRECT(imrc.r-STDW, imrc.t, imrc.r, imrc.t+STDH);
        imdraw_image(im, &imrc, &rc, im2, 0);
        _clr = score < FACEREC_THD ? _rgba(0, 1, 0, 1) : _rgba(1,1,1,1);
        imdraw_rect(im, &imrc, rc, score < FACEREC_THD ? 0 : _rgba(0,0,0,score), _clr, 2, BDM_ALL);
      }
    }
  }

  return 0;
}

#include "font/font.inl"
// 0 没检测到
// -1 检测到，但没识别出来
// -2 覆盖之前特征
int face_recog_run(const img_t* im, const char* s_pixtype, int* xywh, SPFeatInfo* lib, int libsize, int* libused, const char* username, char* out_username, double* outss, double thd, img_t* rgb, const IRECT* pimrc) {
  facerecog_param_t m_fr[1] = {0};
  SPFeatInfo feature[1];
  SPFeatInfo* pfeatlib = lib;
  uchar stdface[100*100];
  int isreg = username && (*username) && lib && libused;
  int pixtype, mode2 = FR_FACE_DET, have_face;
  int featstep = sizeof(SPFeatInfo);
  int featnum = libused ? *libused : 0;
  int ret=0, m_nsp=0;
  double time1=0;
  const char* pixtypes ="|GRAY|BGR|RGB|BGRA|BGR555|BGR565|YUYV|UYVY|";
  //IMINIT(im, h, w, data, step, cn, 1);
  
  pixtype = cstr_splitfind(pixtypes, -1, s_pixtype, -1, 1, 0);
  facerecog_set(m_fr);
  m_fr->stdface = (unsigned char*)stdface;
  m_fr->feature = (FEAT_T*)feature;
  m_fr->featlib = (FEAT_T*)lib;
  m_fr->featstep = featstep;
  m_fr->featnum = featnum;
  mode2 = (isreg) ? FR_FACE_REG : out_username ? FR_FACE_REC : FR_FACE_DET;
  {
    utime_start(_start_time);
    facerecog_run(m_fr, im, pixtype, mode2);
    time1 = utime_elapsed(_start_time);
  }
  have_face = (m_fr->face.w>0) && (m_fr->leye.w>0) && (m_fr->reye.w>0);
  if (xywh) {
    XRECT_int(&m_fr->face, xywh);
    XRECT_int(&m_fr->leye, xywh+4);
    XRECT_int(&m_fr->reye, xywh+8);
  }
  ret = have_face;
  if (isreg) {
    int mintimeid=0;
    int cnt = strmatrix_count(pfeatlib->m_PID.m_ID, *libused, featstep, NULL, username, &pfeatlib->time, &mintimeid);
    feature->time = time(NULL);
    memset(feature->m_PID.m_ID, 0, ID_LEN);
    strncpy(feature->m_PID.m_ID, username, ID_LEN-1);
    if (cnt<SPFEAT_LEN) {
      pfeatlib[*libused] = *feature;
      (*libused)++;
      m_nsp = cnt+1;
      if (m_nsp==SPFEAT_LEN) {
        ret = -3;
      }
    } else {
      ASSERT(mintimeid>=0);
      pfeatlib[mintimeid] = *feature;
      ret = -2;
      m_nsp = SPFEAT_LEN;
    }
  } else if (out_username && m_fr->maxdist_id>=0) {
    double ss = m_fr->maxdist/25000;
    *out_username = 0;
    if (ss<thd) {
      strcpy(out_username, pfeatlib[m_fr->maxdist_id].m_PID.m_ID);
    } else {
      ret = -1;
    }
    if (outss) {
      *outss = ss;
    }
  }
  
  if (rgb) {
    IRECT imrc = pimrc ? *pimrc : iRECT(0, 0, im->w, im->h);
    COLOR _clr = _RGB(255, 0, 0);
    char buf[256];
    const img_t* m_imcap = im;
    img_t* im = rgb;
    font_t font[1] = {0};
    //font_t* font = NULL;
    font_init_memraster(font, "宋体", songti_12);
    
    if (m_fr->face.w) {
      int i;
      IRECT rc;
      XRECT xrc[3];
      _clr = isreg ? _RGB(255, 255, 255) : _RGB(255, 0, 0);
      xrc[0] = m_fr->face, xrc[1] = m_fr->leye, xrc[2] = m_fr->reye;
      
      for (i = 0; i < 3; ++i) {
        if (xrc[i].w) {
          xrect2rc(imrc, m_imcap->h, m_imcap->w, xrc[i], &rc);
          imdraw_rect(im, &imrc, rc, 0, _clr, 2, BDM_ALL);
        }
      }
      
      if (isreg) {
        img_t im2[1] = {0};
        _clr = _RGBA(0, 0, 255, 128);
        rc = imrc;
        rc.b = rc.t + 30;
        imdraw_rect(im, 0, rc, _rgba(0, 0, 0, 0.4), 0, 0, BDM_ALL);
        rc.r = rc.l + RCW(&rc) * m_nsp / SPFEAT_LEN;
        imdraw_rect(im, 0, rc, _clr, 0, 0, BDM_ALL);
        rc = imrc;
        rc.b = rc.t + 30;
        _snprintf(buf, 256, "%s : %d/%d", username, m_nsp, SPFEAT_LEN);
        imdraw_text(im, &imrc, rc, buf, -1, font, _rgb(1,1,1), 0, TF_VCENTER|TF_CENTER);
        //IMINIT(im2, STDH, STDW, stdface, STDSTEP, STDCN, 1);
        //rc = iRECT(imrc.l, imrc.t, imrc.l+STDW, imrc.t+STDH);
        //imdraw_image(im, &imrc, &rc, im2, 0);
      } else if (out_username) {
        int id = m_fr->maxdist_id;
        double score = m_fr->maxdist / 25000.;
        
        if (id >= 0) {
          img_t im2[1] = {0};
          _clr = _RGBA(0, 0, 0, 128);
          xrect2rc(imrc, m_imcap->h, m_imcap->w, m_fr->face, &rc);
          rc.b = rc.t + 30;
          iRectInflate2(&rc, -1, -1);
          imdraw_rect(im, 0, rc, _clr, 0, 0, BDM_ALL);
          _clr = score < FACEREC_THD ? _RGB(0, 255, 0) : _RGB(255, 255, 255);
          _snprintf(buf, 256, "%d,%s,%.3f,%.3f", m_fr->face.w, pfeatlib[id].m_PID.m_ID, score, time1);
          //_snprintf(buf, 256, "%s:%.3f", SPInfoID(sp + k, 0), score);
          imdraw_text(im, 0, rc, buf, -1, font, _clr, 0, TF_VCENTER);
#if 0
          IMINIT(im2, STDH, STDW, pr->m_data, STDSTEP, STDCN, 1);
          rc.l = rc.r - RCH(&rc)*STDW/STDH;
          imdraw_image(im, &imrc, &rc, im2, 0);
          rc = iRECT(imrc.r-STDW, imrc.t, imrc.r, imrc.t+STDH);
          imdraw_image(im, &imrc, &rc, im2, 0);
          _clr = score < FACEREC_THD ? _rgba(0, 1, 0, 1) : _rgba(1,1,1,1);
          imdraw_rect(im, &imrc, rc, score < FACEREC_THD ? 0 : _rgba(0,0,0,score), _clr, 2, BDM_ALL);
#endif
        }
      }
    }
    //imfree(rgb);
  }
  return ret;
}

//#include "test_libfacefeature.c"
#ifdef TEST
#include "ui/window.inl"

#include "pic/pic.inl"

int test_face_recog() {
  int w = 360, h=288;
  int xywh[12] = {0};
  //face_recog(h, w, gry_360x288, w, 1, "gray", "det", 1, 1, xywh);
  printf("face %d %d %d %d\n", xywh[0], xywh[1], xywh[2], xywh[3]);
  return 0;
}


int test_face_recog_pic() {
  int j;
  buf_t bf[1] = {0};
  img_t im[2] = {0};
  img_t* gry = im+1;
  vstr_t sv[1] = {0};
  facerecog_param_t fs[1] = {0};
  facerecog_set(fs);
  bfsetsize(bf, 50*_1M);
  _chdir("E:/pub/yuvs");
  vstr_load("list.txt", sv);
  for (j=0; j<sv->n; ++j) {
    if (imread(sv->v[j].s, 3, 1, im)) {
      im2gry(im, gry);
      facerecog_run(fs, im, T_RGB, FR_FACE_DET|FR_TRANSPOSE);
      
      DrawRectangle2( im, fs->face, CC_RGB( 0, 255, 0 ), 3 );
      DrawRectangle2( im, fs->leye, CC_RGB( 0, 255, 0 ), 1 );
      DrawRectangle2( im, fs->reye, CC_RGB( 0, 255, 0 ), 1 );
      if (1) {
        char buf[256];
        _mkdir("out");
        _snprintf(buf, 256, "./out/%s.jpg", sv->v[j].s);
        imwrite(buf, im);
      }
      if (0) {
        imshow(im);  cvWaitKey(-1);
      }
    }
  }
  return 0;
}
#endif
