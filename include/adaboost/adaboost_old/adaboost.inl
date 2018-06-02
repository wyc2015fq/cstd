
#include "matfile.h"
#include "inifile.h"
#include "cascade.h"

STATIC feat_fun_t _feat_fun[ FEATNUM ] = {0};
STATIC weak_fun_t _weak_fun[ BOOSTNUM ] = {0};

#include "samppair.inl"
#include "featbase.inl"
#include "featbabs.inl"
#include "feathaar.inl"
#include "featchi.inl"
#include "feathaar3.inl"
#include "feathimm.inl"
#include "feateoh.inl"
#include "featlobo.inl"

#include "weakgen.inl"
#include "weaktree.inl"
#include "weaklut.inl"
#include "weakgus.inl"
#include "weakcartw.inl"
#include "cascade.inl"

#define SET_FEAT_FLAT(_FEAT_NUM, _FLAG, _ID, _K) if ((_ID) >= 0 && (_ID) < _FEAT_NUM) { *(_FLAG + (_ID)) = (_K); }

int init_feat_type(BOOST* hh, int type)
{
#define FEATTYPEDEF(a, b)  if (FT_##a == type) { static featvtable vtab##a = {0}; vtab##a.name = #a,vtab##a.disc = b , \
      vtab##a.f_find_samp=a##_find_samp , vtab##a.f_get_feat=a##_feat , vtab##a.f_save_feat=a##_save_feat , \
          vtab##a.f_uninit_feat=a##_uninit , vtab##a.f_samp_data=a##_samp_data , vtab##a.f_init_feat=a##_init ; \
  hh->feat_vt=&vtab##a; hh->feat_type = FT_##a; return FT_##a ;}
#include "feattypedef.txt"
#undef FEATTYPEDEF
  return -1;
}

int init_weak_type(BOOST* hh, int type)
{
#define WEAKTYPEDEF(a, b)  if (WT_##a == type) { static weakvtable vtab##a = {0}; \
    vtab##a.f_get_error=a##_get_error, vtab##a.f_update_weight=a##_update_weight; \
    hh->weak_vt=&vtab##a; hh->weak_type = WT_##a; return WT_##a ;}

#include "weaktypedef.txt"
#undef WEAKTYPEDEF
  return -1;
}

int strto_feat_type(BOOST* hh, const char* name)
{
#define FEATTYPEDEF(a, b)  if (0==strcmp(#a , name)) { hh->feat_type = FT_##a; return FT_##a ;}
#include "feattypedef.txt"
#undef FEATTYPEDEF
  return -1;
}

int strto_weak_type(BOOST* hh, const char* name)
{
#define WEAKTYPEDEF(a, b)  if (0==strcmp(#a , name)) { hh->weak_type = WT_##a; return WT_##a ;}

#include "weaktypedef.txt"
#undef WEAKTYPEDEF
  return -1;
}

int init_boost_mem(BOOST* hh)
{
  hh->vi = MALLOCSET(double, GetSampNum(hh), 0);
  hh->fi = MALLOCSET(double, GetSampNum(hh), 0);
  hh->wi = MALLOCSET(double, GetSampNum(hh), 0);      //-1表示没有样本
  //hh->vvv = MALLOCSET( double, GetSampNum(hh), 0 );  //-1表示没有样本
  FILL(GetSampNum(hh), hh->vi, 0.f);
  FILL(GetSampNum(hh), hh->fi, 0.f);
  FILL(GetSampNum(hh), hh->wi, -1.f);

  if (init_weak_type(hh, hh->weak_type) < 0) {
    printf("未知的weak类型，检查配置文件!");
    return 0;
  }

  if (init_feat_type(hh, hh->feat_type) < 0) {
    printf("未知的feat类型，检查配置文件!");
    return 0;
  }

  ininame(INIFILENAME);
  //iniappname(hh->feat_vt->name);
  iniappname(ADABOOSTSECT);
  init_feat(hh);

  if (GetFeatNum(hh) > 0) {
    int i;
    hh->flag = MALLOCSET(unsigned char, GetFeatNum(hh), 0);

    if (hh->ca != NULL) {
      // 去掉被选过的特征
      for (i = 0; i < hh->ca->weaklen; ++i) {
        SET_FEAT_FLAT(GetFeatNum(hh), hh->flag, hh->ca->id[ i ], 1);
      }
    }
  }

  return 1;
}

int init_boost(BOOST* hh)
{
  //char buf[ 256 ];
  char* cas_fn;
  int maxstagelen, maxweaklen;
  logset(inigetstr("日志文件"));
  hh->ha = inigetfloat("ha");
  hh->fa = inigetfloat("fa");
  hh->min_neg_rate = inigetfloat("最小负样本比例");
  maxstagelen = inigetint("最大强分类器数");
  maxweaklen = inigetint("最大弱分类器数");
  //inigetfloat( "特征挑选比例", hh->select_rate );
  hh->flag_unreselect = inigetint("不重选特征");
  //hh->curr_pos_num = inigetint( "基本正样本数" );
  //hh->pos_num_step = inigetint( "增加正样本数" );

  // 创建样本容器对象
  strto_feat_type(hh, inigetstr("特征类型"));
  strto_weak_type(hh, inigetstr("boost类型"));
  hh->pos_num = inigetint("正样本数目");
  GetSampNum(hh) = inigetint("负样本数目") + hh->pos_num;
  cas_fn = inigetstr("分类器文件");
  hh->ca = cascade_read();

  if (NULL == hh->ca) {
    hh->ca = cascade_read_txt(cas_fn);
  }
  else {
    cascade_save_txt(hh->ca, cas_fn);
  }

  if (NULL == hh->ca) {
    hh->ca = new_cascade(maxstagelen, maxweaklen, hh->feat_type, hh->weak_type);
    //hh->cas = fopen( buf, "w" );
    hh->ca->is_nesting = inigetint("使用nesting结构");
  }

  //cascade_save2(hh->ca);
  if (hh->feat_type != hh->ca->feat_type || hh->weak_type != hh->ca->weak_type) {
    printf("分类器类型不匹配！\n");
    return 0;
  }

  if (WT_LUT != hh->weak_type) {
    hh->ca->is_nesting = 0;
  }

  //cascade_printf( hh->ca, hh->cas );
  if (!hh->ca) {
    printf("打开文件错误");
    free(hh);
    return 0;
  }

  return init_boost_mem(hh);
}

int del_boost_(BOOST* hh)
{
  if (hh) {
    if (hh->ca) {
      del_cascade(hh->ca);
    }

    if ((hh) ->feat_vt) {
      uninit_feat(hh);
    }

    // 销毁样本容器对象
    FREE(hh->wi);
    FREE(hh->vi);
    FREE(hh->fi);
    //FREE(hh->Feat);
    FREE(hh->flag);
    free(hh);
  }

  return 0;
}

BOOST* new_boost()
{
  BOOST* hh;

  if (!file_exist(INIFILENAME)) {
    printf("没找到配置文件！\n");
    return 0;
  }

  ininame(INIFILENAME);
  iniappname(ADABOOSTSECT);

  if (0) {
    char buf[ 256 ];
    int i = 1;

    for (i = 1; i < FEATNUM; ++i) {
      _snprintf(buf, 256, "特征类型%d", i);
      inisetstr(buf, _feat_fun[ i ].name);
    }

    for (i = 1; i < BOOSTNUM; ++i) {
      _snprintf(buf, 256, "boost类型%d", i);
      inisetstr(buf, _weak_fun[ i ].name);
    }
  }

  //inigetstr("分类器文件");
  hh = MALLOCSET(BOOST, 1, 0);

  if (0 == init_boost(hh)) {
    del_boost(hh);
  }

  return hh;
}

int get_samp_num(const double* f, const double* l)
{
  int cnt = 0;

  for (; f != l; ++f) {
    cnt += (*f >= 0.f);   // 如果此样本的权值为负，表示此样本已经被丢弃
  }

  return cnt;
}

// pWeight: 权重数组
// m:  正样本数目
// n:  负样本数目
// c1: 正样本初始权重
// c2: 负样本初始权重
int weight_fill(double* f, double* l, double c)
{
  for (; f != l; ++f) {
    if (*f >= 0.f) {
      *f = c;
    }
  }

  return 1;
}

// 权重规一
double weight_div(double* f, double* l, double init)
{
  for (; f != l; ++f) {
    if (*f > 0.f) {
      *f /= init;
    }
  }

  return init;
}

double weight_guiyi(double* f, double* l)
{
  double init = 0.f;

  for (; f != l; ++f) {
    if (*f > 0.f) {
      init += *f;
    }
  }

  for (; f != l; ++f) {
    if (*f > 0.f) {
      *f /= init;
    }
  }

  return init;
}

// [f, m] 正样本权重数组
// [m, l] 负样本权重数组
// min_neg_rate 最小负样本率
// ha  通过率
int weight_inialize_impl1(double* f, double* m, double* l, double min_neg_rate, double ha, double Hr)
{
  //st_size = 0;
  int pos_num = get_samp_num(f, m);     // 正样本数目
  int neg_num = get_samp_num(m, l);     // 负样本数目
  double neg_rate = (double) neg_num / (l - m);
  logprintf("正样本数目 = %5d, 负样本数目 = %5d\n", pos_num, neg_num);

  if (neg_rate < min_neg_rate || (!neg_num)) {
    logprintf("负样本数目太少了，要添加样本!\n");
    return 0;
  }

  if (!pos_num) {
    logprintf("正样本数目太少了，要添加样本!\n");
    return 0;
  }

  {
    enum {TYPE1, TYPE2, TYPE3, TYPE4, TYPE5};
    enum {TYPE = TYPE5};
    // c0 正样本初始权重
    // c1 负样本初始权重
    double c0 = 0.f;
    double c1 = 0.f;

    if (TYPE == TYPE1) {
      c1 = 1.f / (pos_num * Hr + neg_num);
      c0 = ha * c1;
    }
    else if (TYPE == TYPE2) {
      int samp_num = pos_num + neg_num;
      c0 = ha;
      c1 = (1.f - ha);
    }
    else if (TYPE == TYPE3) {
      int samp_num = pos_num + neg_num;
      c0 = ha / pos_num;
      c1 = (1.f - ha) / neg_num;
    }
    else if (TYPE == TYPE4) {
      int samp_num = pos_num + neg_num;
      c0 = 1.f / samp_num;
      c1 = c0;
    }
    else if (TYPE == TYPE5) {
      int samp_num = pos_num + neg_num;
      c0 = 0.5f / pos_num;
      c1 = 0.5f / neg_num;
    }

    weight_fill(f, m, c0);
    weight_fill(m, l, c1);
  }

  return 1;
}

#define weight_inialize( hh )  weight_inialize_impl1( hh->wi, hh->wi + hh->pos_num, hh->wi + GetSampNum(hh), hh->min_neg_rate, hh->ha, hh->Hr )

int find_samp(BOOST* hh)
{
  int ret, samp_send_type = 0;        // 样本发送方式
  int pos_num, neg_num, nPosNum, nNegNum;
  double pos_rate, neg_rate;
  void freeblockcol(void);
  //freeblockcol();
  nPosNum = hh->pos_num;
  nNegNum = GetSampNum(hh) - hh->pos_num;
  pos_num = get_samp_num(hh->wi, hh->wi + hh->pos_num);
  neg_num = get_samp_num(hh->wi + hh->pos_num, hh->wi + GetSampNum(hh));
  pos_rate = pos_num * 1. / nPosNum;  // 正样本比例
  neg_rate = neg_num * 1. / nNegNum;  // 负样本比例

  logprintf("正样本数目 = %5d, 负样本数目 = %5d\n", pos_num, neg_num);

  // 添加正样本
  if (pos_rate < 0.01f) {
    findsamp(hh, 1);
  }

  // 添加负样本
  if (neg_rate <= hh->min_neg_rate) {
    double* wi = hh->wi + hh->pos_num;
    //FILL(nNegNum, wi, -1);
    findsamp(hh, -1);
  }

  neg_rate = neg_num * 1. / nNegNum;  // 负样本比例
  ret = weight_inialize(hh);

  if (!ret) {
    logprintf("没样本了！\n");
  }

  return ret;
}

int update_weight1(BOOST* hh, WORKINFO* pwi, double* wi1, double* wi2)
{
  int i;
  static const double esp = X_ESP;        //1.0/(2*bin);
  int SampNum = GetSampNum(hh), PosNum = hh->pos_num;
  int poscntha;
  int samp_cnt[ 2 ] = { 0 };
  int pass_cnt[ 2 ] = { 0 };
  double wisum = 0;
  hh->wi = wi1;
  updateweight(hh, pwi);
  hh->wi = wi2;

  for (i = 0; i < SampNum; ++i) {
    if (wi2[ i ] > 0.f) {
      wi2[ i ] = wi1[ i ];

      if (wi1[ i ] > 0.f) {
        wisum += wi2[ i ];
      }
    }
  }

  weight_div(wi2, wi2 + SampNum, wisum);
  {
    {
      double* p;
      p = MALLOCSET(double, PosNum, 0);

      for (i = 0; i < SampNum; ++i) {
        if (hh->wi[ i ] > 0.f) {
          int aa = i < PosNum;

          if (aa) {
            p[ samp_cnt[ 1 ] ] = hh->vi[ i ];
          }

          samp_cnt[ aa ] ++;
        }
      }

      // 用ha算阀值
      if (1) {
        poscntha = (int) floor(samp_cnt[ 1 ] - hh->ha * samp_cnt[ 1 ]);
        poscntha = MIN(MAX(poscntha, 0), samp_cnt[ 1 ] - 1);
        QSORT(p, p + samp_cnt[ 1 ], LESS, double);
        pwi->Threshold = p[ poscntha ] - esp;
      }

      FREE(p);
    }

    // 算通过数目
    for (i = 0; i < SampNum; ++i) {
      if (hh->wi[ i ] > 0.f) {
        int aa = i < PosNum;
        int cc = hh->vi[ i ] > pwi->Threshold;

        if (cc) {
          // 表示通过了
          pass_cnt[ aa ] ++;
        }
      }
    }

    pwi->Ha = (double) pass_cnt[ 1 ] / samp_cnt[ 1 ];
    pwi->Fa = (double) pass_cnt[ 0 ] / samp_cnt[ 0 ];
  }

  if (hh->ca->id) {
    hh->ca->id[ hh->ca->weaklen ] = pwi->id;
  }

  savefeat(hh, pwi->id);

  if (hh->flag && hh->flag_unreselect) {
    SET_FEAT_FLAT(GetFeatNum(hh), hh->flag, pwi->id, 1);
  }

#define FFMT "%9.6g"
#define LOGFMT "%6d|"FFMT " "FFMT "| "FFMT "  "FFMT "  "FFMT "\n"
  logprintf(LOGFMT, pwi->id, pwi->Ha, pwi->Fa, pwi->Error, pwi->Threshold, pwi->Time);
  return (pwi->Ha < hh->ha || pwi->Fa > hh->fa);
}

#define GET_ERROR get_Error_Net
int get_Error(BOOST* hh, WORKINFO* pwi)
{
  int AllFeatNum = pwi->end - pwi->beg;
  utime_start(time);
  pwi->Ha = pwi->Fa = pwi->Error = pwi->Time = 0.f;
  pwi->id = -1;
  weight_guiyi(hh->wi, hh->wi + GetSampNum(hh));
  //hh->get_error(hh, pwi);
  geterror(hh, pwi);

  pwi->Time = (double) utime_elapsed(time);

  return pwi->id >= 0;
}
//#include "get_Error.inl"
#include "get_Error_mpi.inl"

int save_boost(BOOST* hh, const char* name)
{
  FILE* pf = fopen(name, "wb");

  if (pf) {
    savedata2(pf, hh, sizeof(BOOST), 0);
    cascade_save_file(hh->ca, pf);
    savedata2(pf,
        hh->vi, hh->samp_num * sizeof(hh->vi[0]),
        hh->wi, hh->samp_num * sizeof(hh->wi[0]),
        hh->fi, hh->samp_num * sizeof(hh->fi[0]),
        hh->flag, hh->feat_num * sizeof(hh->flag[0]),
        0);
    fclose(pf);
  }

  return 0;
}

int load_boost(BOOST* hh, const char* name)
{
  FILE* pf = fopen(name, "rb");

  if (pf) {
    BOOST hhh = *hh;
    loaddata_2(pf, &hhh, sizeof(BOOST), 0);
    hhh.ca = cascade_load_file(pf);
    init_boost_mem(&hhh);
    loaddata_2(pf, hh->vi, hh->samp_num * sizeof(hh->vi[0]));
    loaddata_2(pf, hh->wi, hh->samp_num * sizeof(hh->wi[0]));
    loaddata_2(pf, hh->fi, hh->samp_num * sizeof(hh->fi[0]));
    loaddata_2(pf, hh->flag, hh->feat_num * sizeof(hh->flag[0]));

    //del_boost();
    if (hh->ca) {
      del_cascade(hh->ca);
    }

    // 销毁样本容器对象
    FREE(hh->wi);
    FREE(hh->vi);
    FREE(hh->fi);
    //FREE(hh->Feat);
    FREE(hh->flag);

    fclose(pf);
  }

  return 0;
}

int train_cascade(BOOST* hh)
{
  static const double esp = X_ESP;    //1.0/(2*bin);
  int i = 0, k, stagecnt, weakcnt;
  int SampNum = GetSampNum(hh), PosNum = hh->pos_num;
  int is_continue_weak;
  int continue_runing = 0;  //表示是继续训练
#define BOOSTDATAFILE "boost.dat"

  if (file_exist(BOOSTDATAFILE)) {
    FILE* pf = fopen(BOOSTDATAFILE, "rb");

    if (pf) {
      BOOST bb = {0};
      loaddata_2(pf, &bb, sizeof(BOOST), 0);

      if (bb.pos_num == hh->pos_num && bb.samp_num == hh->samp_num &&
          bb.weak_type == hh->weak_type && bb.feat_type == hh->feat_type) {
        char* buf;
        int len;
        samp_data(hh, buf, len);
        loaddata_2(pf, hh->wi, GetSampNum(hh) * sizeof(hh->wi[ 0 ]), 0);
        loaddata_2(pf, hh->vi, GetSampNum(hh) * sizeof(hh->vi[ 0 ]), 0);
        loaddata_2(pf, hh->fi, GetSampNum(hh) * sizeof(hh->fi[ 0 ]), 0);
        loaddata_2(pf, buf, len, 0);
        continue_runing = 1;
      }

      fclose(pf);
    }
  }

  logprintf("<< boost 类型: %s, 特征类型: %s, 特征总数: %6d >>\n", _weak_fun[ hh->weak_type ].name, _feat_fun[ hh->feat_type ].name, GetFeatNum(hh));

  for (stagecnt = 0; (!hh->ca || stagecnt < hh->ca->maxstagelen) && (continue_runing || find_samp(hh)); ++stagecnt) {
    int is_continue_stage = 0;
    int baseweakcnt = (hh->ca && hh->ca->is_nesting && (hh->ca->stagelen) > 0) ? 1 : 0;
    logprintf("--------------------------------- Stage %2d ------------------------------------\n",
        (hh->ca) ? hh->ca->stagelen : stagecnt);
    logprintf("  Id  |     ha      Fa    |   Error    Threshold       Time\n");
    {
      WORKINFO wi = { 0 };
      wi.beg = 0;
      wi.end = GetFeatNum(hh);
      wi.id = -1;

      if (!continue_runing) {
        FILL(SampNum, hh->fi, 0.f);

        if (hh->ca->is_nesting && (hh->ca->stagelen) > 0) {
          COPY(SampNum, hh->vi, hh->fi);
          FILL(SampNum, hh->vi, 0);
          is_continue_weak = update_weight1(hh, &wi, hh->wi, hh->wi);

          if (!is_continue_weak) {
            // 同时满足Ha Fa
            break;
          }
        }
        else {
          FILL(SampNum, hh->vi, 0.f);
        }
      }
      else {
        int nw = getweakcnt(hh->ca);
        baseweakcnt += hh->ca->weaklen - nw;
      }

      continue_runing = 0;

      for (weakcnt = baseweakcnt; GET_ERROR(hh, &wi);) {
        double* fi;
        is_continue_weak = 0;

        if (hh->flag && wi.id >= 0 && wi.id < GetFeatNum(hh)) {
          hh->flag[ wi.id ] = 1;
        }

        fi = getfeat(hh, wi.id);
        COPY(SampNum, fi, hh->fi);
        is_continue_weak = update_weight1(hh, &wi, hh->wi, hh->wi);
        ++weakcnt;

        if (0) {
          char* buf;
          int len;
          samp_data(hh, buf, len);
          savedata(BOOSTDATAFILE, hh, sizeof(BOOST),
              hh->wi, GetSampNum(hh) * sizeof(hh->wi[ 0 ]),
              hh->vi, GetSampNum(hh) * sizeof(hh->vi[ 0 ]),
              hh->fi, GetSampNum(hh) * sizeof(hh->fi[ 0 ]),
              buf, len, 0);
          cascade_save(hh->ca);
        }

        if (!is_continue_weak) {
          // 同时满足Ha Fa
          //remove(BOOSTDATAFILE);
#undef BOOSTDATAFILE

          break;
        }
      }

      is_continue_stage = weakcnt;

      if (is_continue_stage) {
        hh->ca->stage[ hh->ca->stagelen ].thd = wi.Threshold;
        hh->ca->stage[ hh->ca->stagelen ].len = weakcnt;

        for (k = 0; k < GetSampNum(hh); ++k) {
          if (hh->vi[ k ] < wi.Threshold) {
            // 没有通过强分类器的样本的权值赋-1，表示抛弃样本
            hh->wi[ k ] = -1.f;
          }
        }

        ++(hh->ca->stagelen);
        //fseek( hh->cas, 0, SEEK_SET );
        //cascade_printf( hh->ca, hh->cas );
        cascade_save(hh->ca);
        iniappname(ADABOOSTSECT);
        cascade_save_txt(hh->ca, inigetstr("分类器文件"));
      }
      else {
        break;
      }
    }
  }

  logprintf("训练完毕!\n");
  return stagecnt;
}
