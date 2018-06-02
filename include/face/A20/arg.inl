

#include "parser/xml.inl"

#define A20_FEAT_PATH  "./feat"
#define A20MAXMEN  1000

typedef struct {
  int mode;
  char curid[ID_LEN];
  int cur_pic_count;
  int xywh[12];
  char stdface[100*100];
  short feature[2500];
  SPInfo* featlib;
  int mennum;
  SPFeatInfo curfeat;
  int id, dis, ret;
  double time;
} argA20_t;

argA20_t argA20 = {0};
//double scale = 0.1;

argA20_t* argA20_run(int h, int w, const char* data, int step, int cn, const char* s_pixtype) {
  argA20_t* arg = &argA20;
  int featnum = arg->mennum * SPFEAT_LEN;
  static int inited = 0;
  if (!inited) {
    inited = 1;
    arg->mennum = load_facelib(A20_FEAT_PATH, "*.dat", &arg->featlib);
  }
  memset(arg->xywh, 0, sizeof(arg->xywh));
  arg->time = 0;
  if (arg->mode) {
    utime_start(_start_time);
    arg->ret = face_recog(h, w, data, step, cn, s_pixtype, 1 == arg->mode ? "df" : "dfr", 0, 0.1,
      arg->xywh, arg->stdface, arg->feature, arg->featlib, sizeof(SPFeatInfo), featnum, &arg->id, &arg->dis);
    arg->time = utime_elapsed(_start_time);
  }
  if (1 == arg->mode) {
    if (arg->ret) {
      //snprintf(arg->curfeat.m_PID.m_ID, ID_LEN, "%s", curid);
      cstr_cpy(arg->curfeat.m_PID.m_ID, ID_LEN, arg->curid, ID_LEN);
      arg->featlib[arg->mennum].m_PFeatArry[arg->cur_pic_count] = arg->curfeat;
      ++arg->cur_pic_count;
    }
    if (arg->cur_pic_count >= SPFEAT_LEN) {
      arg->mennum = BOUND(arg->mennum, 0, A20MAXMEN - 1);
      arg->cur_pic_count = 0;
      arg->mode = 0;
      //savefile(DATAPATH, &facedata, sizeof(facedata));
      save_feat(A20_FEAT_PATH, &arg->featlib[arg->mennum]);
      ++arg->mennum;
    }
  }
  return arg;
}

int cmd_set(const str_t* name, const str_t* value) {
  if (0==cstr_icmp(name->s, name->l, "mode", 4, 1)) {
    argA20.mode = atoi(value->s);
  } else if (0==cstr_icmp(name->s, name->l, "curid", 5, 1)) {
    cstr_cpy(argA20.curid, ID_LEN, value->s, value->l);
  }
  return 0;
}

int parse_arg(const char* buf) {
  int len = strlen(buf);
  int i = 0;
  dstr_t ds[10];
  for (;i<len;) {
    int opos = 0;
    SKIPSPACE1;
    i = ParseNodeAttributeName(ds, &opos, buf, i, len, 0);
    if (i<len) {
      SKIPSPACE1;
      ASSERT('=' == buf[i]);
      i++;
      i = ParseNodeAttributeValue(ds, &opos, buf, i, len, 0);
      cmd_set(ds[0].s, ds[1].s);
    }
  }
  return 0;
}