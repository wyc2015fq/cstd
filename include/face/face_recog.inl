
#ifndef _FACE_RECOG_INL_
#define _FACE_RECOG_INL_


#include "face.inl"
#include "cascadearr.h"
#include "DSP_feat.h"
#include "img/imgopt.inl"
#include "stdc/dir_c.h"
#include "stdc/fileio_c.h"
//#include "img/imgio.inl"

//#include "lib/Sentinel/libsentinel.inl"
#define sentinel_login() 1

#define FR_FACE_DET (FR_FACE_DETECT|FR_EYES_DETECT)
#define FR_FACE_REG (FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD|FR_FACE_FEATURE|FR_RECOG_TEST)
#define FR_FACE_REC (FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD|FR_FACE_FEATURE|FR_FACE_RECOG)

int facerecog_set(facerecog_param_t* s)
{
  memset(s, 0, sizeof(facerecog_param_t));
  s->stepxy = 1, s->mincnt = 8, s->maxoutlen = 150;
  s->casnum = 1, s->cas[ 0 ] = vis_nesting_face20110713;
  s->leyecasnum = 3, s->leyecas[ 0 ] = NIRAllEYE, s->leyecas[ 1 ] = NIREYEWithoutGlass, s->leyecas[ 2 ] = NIREYELeftWithGlass  ;
  s->reyecasnum = 3, s->reyecas[ 0 ] = NIRAllEYE, s->reyecas[ 1 ] = NIREYEWithoutGlass, s->reyecas[ 2 ] = NIREYERightWithGlass ;
  s->isc = FR_F2I(0.5f), s->iss = FR_F2I(1.1f), s->ithd = FR_F2I(0.8f);
  s->issmin = FR_F2I(1.f), s->issmax = FR_F2I(100.f);
  return 0;
}

static char face_membuf[8*1024*1024];

int xrect_trans(int height, int width, XRECT* rc, int transopt) {
  int x = rc->x, y = rc->y, w = rc->w, h = rc->h;
  switch (transopt) {
  case 1:
    rc->x = height - y - h;
    rc->y = x;
    rc->w = h;
    rc->h = w;
    break;
  case 2:
    rc->x = width - x - w;
    rc->y = height - y - h;
    rc->w = w;
    rc->h = h;
    break;
  case 3:
    rc->x = y;
    rc->y = width - x - w;
    rc->w = h;
    rc->h = w;
    break;
  }
  return 0;
}

int facerecog_run(facerecog_param_t* s, const img_t* im, int pixtype, int mode)
{
  img_t im2[1] = {0};
  int ret = 0;
  if (s->transopt) {
    int inv_trans_table[] = {0, 3, 2, 1};
    int trans = inv_trans_table[s->transopt&3];
    imtrans2(im, im2, trans, 0);
    im = im2;
  }
  s->height = im->h, s->width = im->w, s->datastep = im->s;
  s->data = im->tt.data, s->datacn = im->c, s->datacode = pixtype;
  s->mode = mode;
  s->buf = (uchar*)face_membuf;
  s->buflen = countof(face_membuf);
  facerecog_memsize(s);
  ret = facerecog_process(s);
  if (ret==FACE_RECOG_FALG_OK && s->transopt) {
    xrect_trans(s->height, s->width, &s->face, s->transopt);
    xrect_trans(s->height, s->width, &s->deye, s->transopt);
    xrect_trans(s->height, s->width, &s->leye, s->transopt);
    xrect_trans(s->height, s->width, &s->reye, s->transopt);
  }
  imfree(im2);
  return ret;
}

int load_facelib(const char* path, const char* filters, SPInfo** pfacelib) {
  dirlist_t dl[1] = {0};
  int i=0, n=0, j=0;
  dirlist(dl, path, filters, FF_FULLNAME);
  n = dl->n;
  if (pfacelib) {
    buf_t bf[1] = {0};
    SPInfo* facelib = NULL;
    MYREALLOC(facelib, n);
    *pfacelib = facelib;
    for (i=0; i<n; ++i) {
      if (buf_load(dl->v[i].name, bf)) {
        if (bf->len<sizeof(SPInfo)) {
          HUFF_Decode(bf->data, bf->len, facelib+j, sizeof(SPInfo));
        } else {
          memcpy(facelib+j, bf->data, sizeof(SPInfo));
          ++j;
        }
      }
    }
    n = j;
  }
  dirlist_free(dl);
  return n;
}
int save_feat(const char* path, const SPInfo* feat) {
  char buf[256];
  FILE* pf;
  snprintf(buf, 256, "%s/%s.dat", path, feat->m_PFeatArry->m_PID.m_ID);
  pf = fopen(buf, "wb");
  if (pf) {
    fclose(pf);
  }
  return 0;
}

int str2mode(const char* str) {
  int mode = 0;
  for (;*str; ++str) {
    char ch = tolower(*str);
    switch(ch) {
    case 'd':
      mode |= FR_FACE_DETECT|FR_EYES_DETECT;
      break;
    case 'f':
      mode |= FR_FACE_STD|FR_FACE_FEATURE;
      break;
    case 'r':
      mode |= FR_FACE_RECOG;
      break;
    }
  }
  return mode;
}

CC_INLINE int XRECT_int(const XRECT* r, int* out) {
  if (r && out) {
    out[0] = r->x;
    out[1] = r->y;
    out[2] = r->w;
    out[3] = r->h;
  }
  return 0;
}

int face_recog(int h, int w, const void* data, int step, int cn, const char* s_pixtype, const char* s_mode, int trans, double scale,
               int* xywh, void* stdface, void* feature, const void* featlib, int featstep, int feat_num, int* pid, int* pdis) {
  img_t im[1] = {0};
  facerecog_param_t s[1] = {0};
  int pixtype, mode;
  static const char* pixtypes ="|GRAY|BGR|RGB|BGRA|BGR555|BGR565|YUYV|UYVY|";
  static const int pixtypearr[] = {T_GRAY, T_BGR, T_RGB, T_BGRA, T_BGR555, T_BGR565, T_YUYV, T_UYVY};
  int ret = FACE_RECOG_FALG_OK;

#if 0
  static int inited=0;
  if (!inited) {
    inited = sentinel_login();
  }
  if (!inited) {
    return -1;
  }
#endif
  
  IMINIT(im, h, w, data, step, cn, 1);
  pixtype = splitfind_c(pixtypes, -1, s_pixtype, -1, 1, 0);
  mode = str2mode(s_mode);
  facerecog_set(s);
  s->transopt = trans;
  scale = BOUND(scale, 0.05, 5);
  s->isc = FR_F2I(scale);
  if (pixtype>=0) {
    s->stdface = (unsigned char*)stdface;
    s->feature = (FEAT_T*)feature;
    s->featlib = (FEAT_T*)featlib;
    s->featstep = featstep;
    s->featnum = feat_num;
    s->maxdist_id = -1;
    s->maxdist = MAX_INT;
    ret = facerecog_run(s, im, pixtypearr[pixtype], mode);
    if (xywh) {
      XRECT_int(&s->face, xywh);
      XRECT_int(&s->leye, xywh+4);
      XRECT_int(&s->reye, xywh+8);
    }
    if (pid) {
      *pid = s->maxdist_id;
    }
    if (pdis) {
      *pdis = (int)(s->maxdist*1000./25000);
    }
  } else {
    ret = FACE_RECOG_FALG_ARGS_ERR;
  }
  return ret;
}

//  int xywh[12] = {0};
//  h 图像高 w 图像宽 data图像数据 step图像每行字节数 cn图像通道数
// s_pixtype GRAY 灰度图 BGR|RGB|BGRA|BGR555|BGR565 彩色图 YUYV|UYVY 其他格式
// is_trans 是否翻转90度
// scale 图像预缩放
// xywh 输出 人脸[x, y, w, h] 左眼[x, y, w, h] 右眼[x, y, w, h]
//  face_detect_defaut(im->h, im->w, im->tt.data, im->s, im->c, "gray", trans, 1, xywh);
//  printf("%d %d\n", xywh[0], xywh[1]);
int face_detect_defaut(int h, int w, const void* data, int step, int cn, const char* s_pixtype, int is_trans, double scale, int* xywh) {
  int ret = face_recog(h, w, data, step, cn, s_pixtype, "d", is_trans, scale, xywh, 0, 0, 0, 0, 0, 0, 0);
  return (FACE_RECOG_FALG_OK==ret);
}

#ifdef _IMGIO_INL_
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
  pfree(data);
}
int facefeature(const char* pic_100x100_file_name, void* out_feature) {
  img_t im[1] = {0};
  imread(pic_100x100_file_name, 1, 1, im);
  if (100==im->h && 100==im->w, 1==im->c) {
    face_recog(0, 0, 0, 0, 0, "gray", "f", 0, 0, 0, im->tt.data, out_feature, 0, 0, 0, 0, 0);
  }
  imfree(im);
  return 1;
}
#endif // _IMGIO_INL_


//#include "parser/xml.inl"

#define REGH  300
#define REGW  200
#define MAXPICDATALEN  10000

typedef struct {
  //uchar stdface[100 * 100];
  CID m_PID;
  int flag;
  int picdatalen;
  uchar picdata[MAXPICDATALEN];
} men_t;

typedef struct facerecogapp_t facerecogapp_t;
typedef int (*facerecogapp_msg_t)(const facerecogapp_t* app, const char* arg);

typedef enum WorkMode {
  NotWork,
    Recog1N,
      Recog11,
    CapFace,
} WorkMode;
typedef enum MatchMode {
  MatchMode_11=1,
    MatchMode_1N=2,
  MatchMode_MIX=3,
}MatchMode;

struct facerecogapp_t {
  SPFeatInfo* feat;
  int feat_num;
  int max_feat_num;
  men_t* men;
  men_t cur_men;
  uchar cur_stdface[100 * 100];
  uchar regpic[REGH * REGW *3];
  SPFeatInfo cur_feat;
  CID curid;
  WorkMode mode;   // 工作模式 1 采集 2识别
  int cap_pause; // 暂停采集
  MatchMode match_mode; // 匹配模式 1:1 1:n mix
  int max_cap_pic; // 模板上限
  int max_men_num; // 人数上限
  int men_num; // 当前人数
  int cur_pic_count;
  int security_lev;
  int ret;
  int rec_ok;
  int xywh[12]; // 人脸 左眼 右眼 矩形框位置 [x, y, w, h]
  facerecogapp_msg_t msg;
  //facerecogapp_load_t load;
  double scale;// 检测缩放参数
  double cur_time; // 时刻
  double frame_time; // 帧耗时
  double cap_time_used; // 采集到人脸时间
  double cap_time; // 上次采集到人脸时间
  double cap_ok_time;  // 上次检测到人脸时间
  double cap_timeout; // 最大采集时间间隔
  double cap_interval; // 最小采集时间间隔
  double ident_timeout; // 识别超时
  double ident_start_time;
  int trans; // 旋转角度 trans*90度
  buf_t bf11[1]; // 1:1 数据
};

double mygettime() {
  return sys_tick_count()*0.001;
}

int facerecogapp_setcurid(facerecogapp_t* app, const str_t* value) {
  memset(app->curid.m_ID, 0, ID_LEN);
  cstr_cpy(app->curid.m_ID, ID_LEN, value->s, value->l);
  return 0;
}
//#define facerecogapp_msg(arg)  app->msg(app, arg)

CC_INLINE int facerecogapp_msg(const facerecogapp_t* app, const char* fmt, ...)
{
  if (app->msg) {
    char* buf = NULL;
    va_list arglist;
    va_start(arglist, fmt);
    buf = cstr_vsprintf_dup2(fmt, arglist);
    va_end(arglist);
    if (buf) {
      strcat(buf, "\n");
      app->msg(app, buf);
      pfree(buf);
    }
  }
  return 0;
}

#define cstr_icmp_c(s1, l1, s2)  cstr_nicmp_(s1, l1, s2, -1, -1, 0, 1)
int find_men(const facerecogapp_t* app, const char* name) {
  int i, n=0;
  for (i=0; i<app->men_num; ++i) {
    if (0==strcmp(name, app->men[i].m_PID.m_ID)) {
      return i;
    }
  }
  return -1;
}
int getmenfeatnum(const facerecogapp_t* app, const char* name) {
  int i, n=0;
  if (*name==0) {
    return 0;
  }
  for (i=0; i<app->feat_num; ++i) {
    if (0==strcmp(name, app->feat[i].m_PID.m_ID)) {
      ++n;
    }
  }
  return n;
}
int getmenfeat1(const facerecogapp_t* app, const char* name, SPFeatInfo* feat, int maxfeat) {
  int i, n=0;
  for (i=0; n<maxfeat && i<app->feat_num; ++i) {
    if (0==strcmp(name, app->feat[i].m_PID.m_ID)) {
      feat[n++] = app->feat[i];
    }
  }
  return n;
}
int getmenfeatdata(const facerecogapp_t* app, const char* name, buf_t* bf) {
  int j = find_men(app, name);
  if (j>=0) {
    int n = getmenfeatnum(app, name);
    int datalen = sizeof(SPFeatInfo)*n+sizeof(men_t);
    int len = datalen*2;
    bfsetsize(bf, datalen);
    memcpy(bf->data, app->men+j, sizeof(men_t));
    n = getmenfeat1(app, name, (SPFeatInfo*)(bf->data+sizeof(men_t)), n);
    return datalen;
  }
  return 0;
}
int facerecogapp_delmen(facerecogapp_t* app, const char* name) {
  int i, j=0;
#if 0
  int menid = find_men(app, name);
  if (menid>=0) {
    --app->men_num;
    MEMCPY(app->men+menid, app->men + menid+1, (app->men_num - menid));
  }
#endif
  for (i=j=0; i<app->men_num; ++i) {
    if (0!=strcmp(name, app->men[i].m_PID.m_ID)) {
      if (i!=j) {
        app->men[j] = app->men[i];
      }
      ++j;
    }
  }
  app->men_num = j;
  for (i=j=0; i<app->feat_num; ++i) {
    if (0!=strcmp(name, app->feat[i].m_PID.m_ID)) {
      if (i!=j) {
        app->feat[j] = app->feat[i];
      }
      ++j;
    }
  }
  if (1) {
    char buf[256];
    _snprintf(buf, 256, "%s.dat", name);
    remove(buf);
  }
  app->feat_num = j;
  return 0;
}
int facerecogapp_delall(facerecogapp_t* app) {
  buf_t bf[1] = {0};
  int i, n=0;
  dirlist_t dl[1] = {0};
  dirlist(dl, ".", "*.dat", 0);
  app->men_num = 0;
  app->feat_num = 0;
  n = dl->n;
  for(i=0; i<dl->n; ++i) {
    remove(dl->v[i].name);
  }
  dirlist_free(dl);
  bffree(bf);
  return n;
}
int facerecogapp_cap_start(facerecogapp_t* app, bool add) {
  if (!add) {
    facerecogapp_delmen(app, app->curid.m_ID);
  }
  app->cap_ok_time = app->cap_time = mygettime();
  app->mode = CapFace;
  app->cur_men.m_PID = app->curid;
  app->cur_feat.m_PID = app->curid;
  app->cur_pic_count = 0;
  app->cap_time_used = 0;
  return 0;
}

#define MAXFEATNUM  60

#include "codec/compress.inl"
#define NONSUPPORT()  printf("nonsupport\n")

int facerecogapp_savemen(const facerecogapp_t* app, const char* name) {
  int menid = find_men(app, name);
  buf_t bf[1] = {0};
  if (menid>=0) {
    int len;
    len = getmenfeatdata(app, name, bf);
    if (len>0) {
      char buf[256];
      snprintf(buf, 256, "%s.dat", name);
      compress_save(buf, bf->data, bf->len);
      printf("savemen %s ok\n", name);
    }
  }
  bffree(bf);
  return 0;
}

int facerecogapp_saveall(const facerecogapp_t* app) {
  int i, j=0;
  for (i=0; i<app->men_num; ++i) {
    char* name = app->men[i].m_PID.m_ID;
    facerecogapp_savemen(app, name);
  }
  //app->men_num = j;
  return 0;
}
int facerecogapp_loadmendata(facerecogapp_t* app, const char* name1, const unsigned char* data, int len, bool add) {
  if (len>sizeof(men_t)) {
    int k = app->feat_num, j = app->men_num, n;
    const char* name = name1 ? name1 : ((const men_t*)(data))->m_PID.m_ID;
    if (!add) {
      facerecogapp_delmen(app, name);
      j = app->men_num;
    } else {
      j = find_men(app, name);
      if (j<0) {
        j = app->men_num;
      }
    }
    n = (len - sizeof(men_t))/sizeof(*app->feat);
    if ((k+n)>app->max_feat_num) {
      app->max_feat_num += n*2;
      MYREALLOC(app->feat, app->max_feat_num);
    }
    if (j>=app->max_men_num) {
      app->max_men_num = (app->max_men_num+2)*3/2;
      MYREALLOC(app->men, app->max_men_num);
    }
    memcpy(app->men+j, data, sizeof(men_t));
    memcpy(app->feat+k, data+sizeof(men_t), n*sizeof(*app->feat));
    if (name1) {
      int i;
      CID_set(app->men[j].m_PID.m_ID, name1);
      printf("CID_set\n");
      for (i=0; i<n; ++i) {
        CID_set(app->feat[k+i].m_PID.m_ID, name1);
      }
    }
    printf("load %s ok\n", app->men[j].m_PID.m_ID);
    app->feat_num = k + n;
    app->men_num = ++j;
  }
  return 0;
}
int facerecogapp_loadmen(facerecogapp_t* app, const char* name, bool add) {
  char buf[256];
  buf_t bf[1] = {0};
  _snprintf(buf, 256, "%s.dat", name);
  if (compress_load(buf, bf)) {
    facerecogapp_loadmendata(app, NULL, bf->data, bf->len, add);
  }
  bffree(bf);
  return 0;
}
int facerecogapp_loadid(facerecogapp_t* app, const char* name) {
  char buf[256];
  _snprintf(buf, 256, "%s.dat", name);
  if (compress_load(buf, app->bf11)) {
    app->mode = Recog11;
    return 1;
  }
  return 0;
}
int facerecogapp_loadall(facerecogapp_t* app) {
  buf_t bf[1] = {0};
  int i, feat_num=0, j=0, k=0;
  dirlist_t dl[1] = {0};
  dirlist(dl, ".", "*.dat", 0);
  app->max_men_num = dl->n;
  app->max_feat_num = dl->n*10;
  MYREALLOC(app->men, app->max_men_num);
  MYREALLOC(app->feat, app->max_feat_num);
  for(i=0; i<dl->n; ++i) {
    if (compress_load(dl->v[i].name, bf)) {
      if (bf->len>sizeof(men_t)) {
        int n = (bf->len - sizeof(men_t))/sizeof(*app->feat);
        if ((k+n)>app->max_feat_num) {
          app->max_feat_num += n*2;
          MYREALLOC(app->feat, app->max_feat_num);
        }
        memcpy(app->men+j, bf->data, sizeof(men_t));
        memcpy(app->feat+k, bf->data+sizeof(men_t), n*sizeof(*app->feat));
        printf("load %s ok\n", app->men[j].m_PID.m_ID);
        k+=n;
        ++j;
      }
    }
  }
  dirlist_free(dl);
  app->feat_num = k;
  app->men_num = j;
  bffree(bf);
  return 0;
}
#include "codec/base64.inl"
char* facerecogapp_getmenfeatstr(const facerecogapp_t* app, const char* name) {
  char* str = NULL;
  buf_t bf[1] = {0};
  int len = getmenfeatdata(app, name, bf);
  if (len>0) {
    int len2 = len*2;
    MYREALLOC(str, len2);
    len2 = base64_encode(bf->data, bf->len, str);
    str[len2] = 0;
  }
  bffree(bf);
  return str;
}

int facerecogapp_setmenfeatstr(facerecogapp_t* app, const char* name, const char* featstr, bool add) {
  int len = strlen(featstr);
  uchar* data = NULL;
  MYREALLOC(data, len);
  len = base64_decode(featstr, len, data);
  facerecogapp_loadmendata(app, name, data, len, add);
  return 0;
}

int facerecogapp_msg_default(const facerecogapp_t* app, const char* arg) {
  printf("%s\n", arg);
  return 0;
}

char* facerecogapp_getlist(const facerecogapp_t* app, int flag) {
  int i, slen=0;
  char* list = NULL;
  for (i=0; i<app->men_num; ++i) {
    if (!flag || (app->men[i].flag)) {
      const char* name = app->men[i].m_PID.m_ID;
      slen += strlen(name) + 10;
    }
  }
  MYREALLOC(list, slen);
  if (list) {
  list[0] = 0;
  for (i=0; i<app->men_num; ++i) {
    if (!flag || (app->men[i].flag)) {
      const char* name = app->men[i].m_PID.m_ID;
      int n = getmenfeatnum(app, name);
      int l = strlen(list);
      _snprintf(list+l, slen-l, "%s<%d>;", name, n);
    }
  }
  }
  return list;
}

int parse_name_featstr(char* buf, char** pname, char** pfeatstr) {
  char* p = buf;
  char* p1;
  char* p2;
  if (p1 = strchr(p, '<')) {
    *p1++ = 0;
    if (p2 = strchr(p1, '>')) {
      *p2 = 0;
      *pname = p;
      *pfeatstr = p1;
      return 1;
    }
  }
  return 0;
}

int facerecogapp_cmd_set(facerecogapp_t* app, const str_t* name, str_t* value) {
  char value_buf[ID_LEN] = "0";
  cstr_cpy(value_buf, ID_LEN, value->s, value->l);
  if (0==cstr_icmp_c(name->s, name->l, "ADD_ID")) {
    facerecogapp_setcurid(app, value);
    facerecogapp_cap_start(app, false);
  } else if (0==cstr_icmp_c(name->s, name->l, "ADD_ID_X")) {
    facerecogapp_setcurid(app, value);
    facerecogapp_cap_start(app, true);
  } else if (0==cstr_icmp_c(name->s, name->l, "SET_CAP_INTERVAL")) {
    app->cap_interval = atof(value->s);
    facerecogapp_msg(app, "RE_CAP_INTERVAL=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "SET_SCALE")) {
    app->scale = atof(value->s);
    facerecogapp_msg(app, "RE_SCALE=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "SET_IDENT_TIMEOUT")) {
    app->ident_timeout = atof(value->s);
    facerecogapp_msg(app, "RE_CAP_TIMEOUT=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_IDENT_TIMEOUT")) {
    facerecogapp_msg(app, "RE_IDENT_TIMEOUT=%f", app->ident_timeout);
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_CAP_INTERVAL")) {
    facerecogapp_msg(app, "RE_CAP_INTERVAL=%f", app->cap_interval);
  } else if (0==cstr_icmp_c(name->s, name->l, "SET_CAP_TIMEOUT")) {
    app->cap_timeout = atof(value->s);
    facerecogapp_msg(app, "RE_CAP_TIMEOUT=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_CAP_TIMEOUT")) {
    facerecogapp_msg(app, "RE_CAP_TIMEOUT=%f", app->cap_timeout);
  } else if (0==cstr_icmp_c(name->s, name->l, "CAP_PAUSE")) {
    app->cap_pause = atoi(value->s);
    facerecogapp_msg(app, "RE_CAP_PAUSE=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "CAP_CONTINUE")) {
    app->mode = CapFace;
    facerecogapp_msg(app, "RE_CAP_CONTINUE=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "CAP_CANCEL")) {
    app->mode = NotWork;
    app->cur_pic_count = 0;
    facerecogapp_msg(app, "RE_CAP_CANCEL=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "SET_MAX_CAP_PIC")) {
    app->max_cap_pic = atoi(value->s);
    facerecogapp_msg(app, "RE_MAX_CAP_PIC=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "SET_SECURITY_LEV")) {
    app->security_lev = atoi(value->s);
    facerecogapp_msg(app, "RE_SECURITY_LEV=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_SECURITY_LEV")) {
    facerecogapp_msg(app, "RE_SECURITY_LEV=%d", app->security_lev);
  } else if (0==cstr_icmp_c(name->s, name->l, "SET_MATCH_MODE")) {
    app->match_mode = (MatchMode)atoi(value->s);
    facerecogapp_msg(app, "RE_MATCH_MODE=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_MATCH_MODE")) {
    facerecogapp_msg(app, "RE_MATCH_MODE=%d", app->match_mode);
  } else if (0==cstr_icmp_c(name->s, name->l, "IDENT_ON")) {
    app->mode = Recog1N;
    app->cap_pause = 0;
    app->ident_start_time = mygettime();
    facerecogapp_msg(app, "RE_IDENT_ON=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "IDENT_OFF")) {
    app->mode = NotWork;
    facerecogapp_msg(app, "RE_IDENT_OFF=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "LOAD_ID")) {
    int ret = facerecogapp_loadid(app, value_buf);
    facerecogapp_msg(app, "RE_LOAD_ID=%d", ret);
  } else if (0==cstr_icmp_c(name->s, name->l, "ADD_MIX_ID")) {
    //facerecogapp_setcurid(app, value);
    //facerecogapp_msg(app, "RE_ADD_MIX_ID=OK");
    facerecogapp_msg(app, "ERROR=NONSUPPORT");
  } else if (0==cstr_icmp_c(name->s, name->l, "DEL_MIX_ID")) {
    facerecogapp_msg(app, "ERROR=NONSUPPORT");
    //facerecogapp_setcurid(app, value);
    //facerecogapp_msg(app, "RE_DEL_MIX_ID=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "DEL_ID")) {
    facerecogapp_delmen(app, value_buf);
    facerecogapp_msg(app, "RE_DEL_ID=1");
  } else if (0==cstr_icmp_c(name->s, name->l, "DEL_ID_ALL")) {
    int n = facerecogapp_delall(app);
    facerecogapp_msg(app, "RE_DEL_ID_ALL=%d", n);
  } else if (0==cstr_icmp_c(name->s, name->l, "CON_TEST")) {
    facerecogapp_msg(app, "CON_TEST=OK");
  } else if (0==cstr_icmp_c(name->s, name->l, "LOADALL")) {
    facerecogapp_loadall(app);
  } else if (0==cstr_icmp_c(name->s, name->l, "SAVEALL")) {
    facerecogapp_saveall(app);
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_ID_SUM")) {
    facerecogapp_msg(app, "RE_ID_SUM=%d", app->men_num);
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_TEMPLATE_SUM")) {
    facerecogapp_msg(app, "RE_TEMPLATE_SUM=%d", app->men_num);
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_MIX_ID_SUM")) {
    int i, n=0;
    for (i=0; i<app->men_num; ++i) {
      if (app->men[i].flag) {
        ++n;
      }
    }
    facerecogapp_msg(app, "RE_MIX_SUM=%d", n);
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_MIX_TEMPLATE_SUM")) {
    int i, n=0;
    for (i=0; i<app->feat_num; ++i) {
      if (app->feat[i].m_Flag) {
        ++n;
      } 
    }
    facerecogapp_msg(app, "RE_MIX_TEMPLATE_SUM=%d", n);
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_ID_LIST")) {
    char* list = facerecogapp_getlist(app, 0);
    if (list) {
      facerecogapp_msg(app, "RE_ID_LIST=\"%s\"", list);
      FREE(list);
    }
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_MIX_ID_LIST")) {
    char* list = facerecogapp_getlist(app, 1);
    if (list) {
      facerecogapp_msg(app, "RE_MIX_ID_LIST=\"%s\"", list);
      FREE(list);
    }
  } else if (0==cstr_icmp_c(name->s, name->l, "GET_UP_COMPRESS")) {
    facerecogapp_msg(app, "RE_UP_COMPRESS=0");
  } else if (0==cstr_icmp_c(name->s, name->l, "UP_CHAR")) {
    const char* name = value_buf;
    char* featstr = facerecogapp_getmenfeatstr(app, name);
    if (featstr) {
      facerecogapp_msg(app, "RE_UP_CHAR=%s<%s>", name, featstr);
      FREE(featstr);
    }
  } else if (0==cstr_icmp_c(name->s, name->l, "UP_CHAR_ALL")) {
    int i;
    for (i=0; i<app->men_num; ++i) {
      const char* name = app->men[i].m_PID.m_ID;
      char* featstr = facerecogapp_getmenfeatstr(app, name);
      if (featstr) {
        facerecogapp_msg(app, "RE_UP_CHAR=%s<%s>", name, featstr);
        FREE(featstr);
      }
    }
  } else if (0==cstr_icmp_c(name->s, name->l, "REBOOT")) {
    system("reboot");
  } else if (0==cstr_icmp_c(name->s, name->l, "DOWN_CHAR")) {
    char* name = value->s;
    char* featstr = NULL;
   // printf("DOWN_CHAR=%s\n", name);
    if (parse_name_featstr(value->s, &name, &featstr)) {
      //printf("DOWN_CHAR=%s ok\n", name);
      facerecogapp_setmenfeatstr(app, name, featstr, false);
      facerecogapp_savemen(app, name);
    }
  } else {
    printf("%s\n", name);
    facerecogapp_msg(app, "ERROR=NONSUPPORT");
  }
  return 0;
}
#undef cstr_icmp_c
// 13724008827
int facerecogapp_cmd(facerecogapp_t* app, const char* buf) {
  int i = 0, len;
  dstr_t ds[10];
  //if (app->load) {    app->load(app);  }
  if (NULL==app->msg) {
    app->msg = facerecogapp_msg_default;
  }
  if (NULL==buf) {
    buf = "SET_IDENT_TIMEOUT=8 SET_MATCH_MODE=2 SET_SCALE=0.2, SET_CAP_INTERVAL=0.3,SET_SECURITY_LEV=3, SET_CAP_TIMEOUT = 160, SET_MAX_CAP_PIC=2";
  }
  len = strlen(buf);
  for (;i<len;) {
    int opos = 0;
    SKIPSPACE1;
    i = ParseNodeAttributeName(ds, &opos, buf, i, len, 0);
    if (i<len) {
      SKIPSPACE1;
      ASSERT('=' == buf[i]);
      i++;
      i = ParseNodeAttributeValue(ds, &opos, buf, i, len, 0);
      facerecogapp_cmd_set(app, ds[0].s, ds[1].s);
    }
  }
  return 0;
}
#include "codec/base64.inl"
int compress_base64(const void* indata, int len, void* dst, int ndst) {
  char* buf = (char*)malloc(len*2);
  int len1 = HUFF_Encode(indata, len, buf, len*2);
  int len2 = base64_encode(buf, len1, (char*)dst);
  ((char*)dst)[len2] = 0;
  free(buf);
  return len2;
}

int uncompress_base64(const void* src, int len, void* dst, int ndst) {
  char* buf = (char*)malloc(len*2);
  int len1 = base64_decode((const char*)src, len, buf);
  int len2 = HUFF_Decode(buf, len1, dst, ndst);
  free(buf);
  return len2;
}

int test_compress_base64()
{
  enum {N=1<<12};
  char storage0[N] = {0};
  char storage1[2*N] = {0};
  char storage2[N] = {0};
  int c_len;
  int dc_len;
  int i, seed = 100;
  for (i=0; i<countof(storage0); ++i) {
    storage0[i] = i&1 ? seed : 0;
    seed = seed*13333 + i*7771;
  }
  c_len = compress_base64(storage0, N, storage1, 2*N);
  dc_len = uncompress_base64(storage1, c_len, storage2, N);
  ASSERT(dc_len == N);
  ASSERT(memcmp(storage0, storage2, dc_len) == 0);
  return c_len;
}

#if 0
//#include "codec/compress.inl"
int test_compression()
{
  enum {N=1<<10};
  char storage0[N] = {0};
  char storage1[2*N] = {0};
  char storage2[N] = {0};
  int c_len;
  int dc_len;
  int i, seed = 100;
  for (i=0; i<countof(storage0); ++i) {
    storage0[i] = i&1 ? seed : 0;
    seed = seed*13333 + i*7771;
  }
  c_len = stb_compress(storage1, storage0, countof(storage0));
  dc_len = stb_decompress_length(storage1);
  ASSERT(dc_len == countof(storage0));
  dc_len = stb_decompress(storage2, storage1, c_len);
  ASSERT(dc_len == countof(storage0));
  ASSERT(memcmp(storage0, storage2, dc_len) == 0);
  return c_len;
}
#endif
#include "img/imgio.inl"

int facerecogapp_frame(facerecogapp_t* app, int h, int w, const void* data, int step, int cn, const char* s_pixtype, const void* rgbdata, int rgbstep) {
  int* xywh = app->xywh;
  int featstep = sizeof(SPFeatInfo);
  const SPFeatInfo* feat = app->feat;
  const men_t* men = app->men;
  int feat_num = app->feat_num;
  int men_num = app->men_num;
  int pid = -1;
  int pdis = 0;
  img_t imrgb[1] = {0};
  //char buf[256];
  int ret = 0;
  double cur_time;
  if (h*w<=0) {
    return 0;
  }
  IMINIT(imrgb, h, w, (void*)rgbdata, rgbstep, 3, 1);
  app->ret = 0;
  //facerecogapp_msg(app, "facerecogapp_frame");
  if (1) {
    //app->mode = 2;
    if (app->mode) {
      int dorun = 1;
      if (app->mode==Recog11) {
        men = (men_t*)(app->bf11->data);
        feat = (SPFeatInfo*)(app->bf11->data + sizeof(men_t));
        feat_num = (app->bf11->len - sizeof(men_t))/sizeof(SPFeatInfo);
        men_num = 1;
      }
      //utime_start(_start_time);
      app->ret = ret = face_recog(h, w, data, step, cn, s_pixtype,
        CapFace == app->mode ? "df" : "dfr", app->trans, app->scale,
        xywh, app->cur_stdface, app->cur_feat.m_Feature, feat, featstep, feat_num, &pid, &pdis);
#if 0
      char buf[15000];
      //int n = stb_compress(buf, app->cur_feat.m_Feature, featstep);
      int n3 = compress_base64(app->cur_feat.m_Feature, featstep, buf, sizeof(buf));
      int n4 = base64_encode(app->cur_feat.m_Feature, featstep, buf, sizeof(buf));
      int n2 = HUFF_Encode(app->cur_feat.m_Feature, featstep, buf, sizeof(buf));
      //int n1 = stb_compress(buf, app->cur_feat.m_Feature, featstep);
      //time2 += utime_elapsed(_start_time)*1000;
#endif
    }
    if (app->ret && rgbdata) {
      IRECT rc1 = iRECT2(0, 0, w, h), rc2;
      img_t ims[1] = {0};
      rc1 = iRECT2(xywh[0], xywh[1], xywh[2], xywh[3]);
      rc1 = iRectInflate1(rc1, xywh[3]);
      iRectScaling(rc1, REGW, REGH, &rc2);
      imsubref(imrgb, rc2, ims);
      IMRESIZE((ims->h), ims->w, ims->tt.data, ims->s, ims->c, REGH, REGW, app->regpic, REGW*3, 3, INTER_BILINEARITY);
    }
  }
  cur_time = mygettime();
  app->frame_time = cur_time - app->cap_ok_time;
  app->cur_time = cur_time;
  if (ret) {
    double usetime = cur_time - app->cap_ok_time;
    double usetime2 = cur_time - app->cap_time;
    facerecogapp_msg(app, "DETECTED=1 FACERC=[%d,%d,%d,%d]", xywh[0], xywh[1], xywh[2], xywh[3]);
    if (app->cap_time>0 && usetime2>1) {
    }
    if (CapFace == app->mode) {
      if (!app->cap_pause) {
        if (usetime>app->cap_interval) {
          if (app->cur_pic_count<app->max_cap_pic) {
            int cur_feat_num = app->feat_num + app->max_cap_pic;
            if (0==app->cur_pic_count) {
              facerecogapp_msg(app, "START_CAPTURE=1");
            }
            facerecogapp_msg(app, "CAPTURED=%d ", app->cur_pic_count);
            {
              char buf[256];
              int* xywh1 = xywh+4;
              int* xywh2 = xywh+4;
              IPOINT p1 = iPOINT((xywh1[0]+xywh1[2])/2, (xywh1[1]+xywh1[3])/2);
              IPOINT p2 = iPOINT((xywh2[0]+xywh2[2])/2, (xywh2[1]+xywh2[3])/2);
              _snprintf(buf, countof(buf), "./out/%s/", app->curid.m_ID);
              mkdirs(buf);
              _snprintf(buf, countof(buf), "./out/%s/%s_%03d_%03d_%03d_%03d_%03d.jpg", app->curid.m_ID, _strtime2(), app->cur_pic_count, p1.x, p1.y, p2.x, p2.y);
              imsave(buf, h, w, (const uchar*)data, step, cn);
            }
            if (cur_feat_num>app->max_feat_num) {
              int max_feat_num = cur_feat_num*3/2+20;
              MYREALLOC(app->feat, max_feat_num);
              app->max_feat_num = max_feat_num;
            }
            app->feat[app->feat_num + app->cur_pic_count] = app->cur_feat;
            ++app->cur_pic_count;
            if (app->cur_pic_count >= app->max_cap_pic) {
              int menid = find_men(app, app->cur_men.m_PID.m_ID);
              // app->men_num = BOUND(app->men_num, 0, app->max_men_num - 1);
              if (menid<0) {
                menid = app->men_num;
                ++app->men_num;
              }
              if (menid >= app->max_men_num) {
                int max_men_num = app->men_num*3/2 + 20;
                MYREALLOC(app->men, max_men_num);
                app->max_men_num = max_men_num;
              }
              app->cur_men.picdatalen = imsave_mem(app->cur_men.picdata, MAXPICDATALEN, "jpg", REGH, REGW, app->regpic, REGW*3, 3);
              app->men[menid] = app->cur_men;
              
              app->cur_pic_count = 0;
              app->feat_num = cur_feat_num;
              app->mode = Recog1N;
              facerecogapp_msg(app, "REGISTERED=\"%s\" ", app->cur_men.m_PID.m_ID);
              facerecogapp_savemen(app, app->cur_men.m_PID.m_ID);
            }
            app->cap_ok_time = cur_time;
          } else {
            app->cur_pic_count = 0;
            app->mode = Recog1N;
          }
        } else if (app->cap_time_used>app->cap_timeout) {
          facerecogapp_msg(app, "REG_ERROR=2 ");
          app->mode = Recog1N;
          app->cur_pic_count = 0;
        }
        app->cap_time_used += app->frame_time;
      }
    }
    else if (app->mode==Recog1N || app->mode==Recog11) {
      static int lev_thd[] = {490, 480, 470, 460, 450};
      int lev = BOUND(app->security_lev, 1, countof(lev_thd)) - 1;
      int thd = lev_thd[lev];
      thd = 500;
      //printf("pid %d pdis %d  \n", pid, pdis);
      app->curid.m_ID[0] = 0;
      app->rec_ok = 0;
      if (pid >= 0 && pid < (feat_num) && (pdis<thd)) {
        //printf("time1 %f\n", time1);
        app->curid = feat[pid].m_PID;
        app->rec_ok = 1;
        facerecogapp_msg(app, "MATCHED=%s DIST=%d", feat[pid].m_PID.m_ID, pdis);
        app->cap_ok_time = cur_time;
        app->mode = Recog1N;
      }
    }
    app->cap_time = cur_time;
  }
  if (app->mode==Recog11 && (cur_time-app->ident_start_time) > app->ident_timeout) {
    //if (app->match_mode==MatchMode_1N) {    }
    app->mode = Recog1N;
  }
  return ret;
}

#include "A20/show_cnsun_logo.inl"
#include "draw/HZK16.inl"

int facerecogapp_frameui(facerecogapp_t* app, const img_t* im, const char* s_pixtype, const img_t* imrgb, img_t* out) {
  double ss = 1;
  img_t im1[1] = {0};
  app->trans = 0;
  facerecogapp_frame(app, im->h, im->w, im->tt.data, im->s, im->c, s_pixtype, imrgb->tt.data, imrgb->s);
  
  imresize(im, im->h/ss, im->w/ss, im1);
  if (app->ret) {
    int i;
    const int* xywh = app->xywh;
    COLOR clr = _rgb(1,1,1);
    char buf[256] = {0};
    for (i=0; i<3; ++i) {
      const int* xywh = app->xywh + i*4;
      //DRECT drc = dRECT2(xywh[0]/2, xywh[1]/2, xywh[2]/2, xywh[3]/2);
      //imdrawaa_rect_stroke(im, 0, NULL, drc, _rgb(0, 0, 1), 2);
      imput_rect(im1, xywh[0]/ss, xywh[1]/ss, xywh[2]/ss, xywh[3]/ss, 0, _rgb(0, 0, 1), 2);
    }
    switch (app->mode) {
    case 1:
      snprintf(buf, 256, "采集%d/%d张", app->cur_pic_count, app->max_cap_pic);
      clr =_rgb(0, 0, 1);
      break;
    case 2:
      snprintf(buf, 256, "id=%s", app->curid.m_ID);
      if (app->rec_ok) {
        clr = _rgb(0, 1, 0);
      }
      break;
    default:
      break;
    }
    imput_str(im1, xywh[0]/ss, xywh[1]/ss, buf, -1, clr);
    snprintf(buf, 256, "men=%d\nx=%d y=%d, w=%d h=%d", app->men_num, xywh[0], xywh[1], xywh[2], xywh[3]);
    imput_str(im1, 200, 0, buf, -1, _rgb(1,1,1));
  }
  //imtrans2(im, im, 2, 0);
  show_cnsun_logo(im1);
  imtrans2(im1, im1, 3, 0);
  imsetsize(out, im1->h, im1->w, im1->c, 1);
  imcopy(im1, out);
  return 0;
}
char* facerecogapp_loadfeattxt(const char* name1) {
  char* buf2 = NULL;
  int len2 = 0;
  char name[256];
  str_t str[1] = {0};
  GetFileNameCopy(name1, name, 256);
  str_load(name1, str);
  len2 = str->l+strlen(name)+20;
  MYREALLOC(buf2, len2);
  printf("%s", name1);
  _snprintf(buf2, len2, "DOWN_CHAR=%s<%s>\n", name, str->s);
  str_free(str);
  return buf2;
}
#endif // _FACE_RECOG_INL_

