
#include <stdio.h>
#include "std/stddef_c.h"
#include "std/inifile.h"
#include "std/log_c.h"
#include "std/dir_c.h"
#include "utime.h"

#define CAS_DAT "cas.dat"
#define CAS_TXT "cas.txt"

//#include "net/mpi.h"
#include "adaboost.h"
//#include "cascade.inl"

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

#define weight_inialize( hh )  weight_inialize_impl1( hh->wi, hh->wi + hh->pos_num, hh->wi + hh->samp_num, hh->min_neg_rate, hh->ha, hh->Hr )

int find_samp(BOOST* hh)
{
  int ret, samp_send_type = 0;        // 样本发送方式
  int pos_num, neg_num, nPosNum, nNegNum;
  double pos_rate, neg_rate;
  void freeblockcol(void);
  //freeblockcol();
  nPosNum = hh->pos_num;
  nNegNum = hh->samp_num - hh->pos_num;
  pos_num = get_samp_num(hh->wi, hh->wi + hh->pos_num);
  neg_num = get_samp_num(hh->wi + hh->pos_num, hh->wi + hh->samp_num);
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
  int SampNum = hh->samp_num, PosNum = hh->pos_num;
  int poscntha;
  int samp_cnt[ 2 ] = { 0 };
  int pass_cnt[ 2 ] = { 0 };
  double wisum = 0;
  double* p;
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


  if (hh->ca->id) {
    hh->ca->id[ hh->ca->weaklen ] = pwi->id;
  }

  savefeat(hh, pwi->id);

  if (hh->flag && hh->flag_unreselect) {
#define SET_FEAT_FLAT(_FEAT_NUM, _FLAG, _ID, _K) if ((_ID) >= 0 && (_ID) < _FEAT_NUM) { *(_FLAG + (_ID)) = (_K); }
    SET_FEAT_FLAT(hh->feat_num, hh->flag, pwi->id, 1);
  }

#define FFMT "%9.6g"
#define LOGFMT "%6d|"FFMT " "FFMT "| "FFMT "  "FFMT "  "FFMT "\n"
  logprintf(LOGFMT, pwi->id, pwi->Ha, pwi->Fa, pwi->Error, pwi->Threshold, pwi->Time);
  return (pwi->Ha < hh->ha || pwi->Fa > hh->fa);
}

//#define GET_ERROR get_Error_Net
int get_Error(BOOST* hh, WORKINFO* pwi)
{
  int AllFeatNum = pwi->end - pwi->beg;
  utime_start(time);
  pwi->Ha = pwi->Fa = pwi->Error = pwi->Time = 0.f;
  pwi->id = -1;
  weight_guiyi(hh->wi, hh->wi + hh->samp_num);
  //hh->get_error(hh, pwi);
  geterror(hh, pwi);

  pwi->Time = (double) utime_elapsed(time);

  return pwi->id >= 0;
}
//#include "get_Error.inl"

#define INIFILENAME "adaboost.ini"
#define ADABOOSTSECT "adaboost"

int adaboost_train(adaboost_t* hh)
{
  int maxstagelen, maxweaklen;
  int is_nesting;

  if (!file_exist(INIFILENAME)) {
    //printf("%s不存在", INIFILENAME);
    ASSERT(0 && INIFILENAME "不存在");
    return 0;
  }

  ininame(INIFILENAME);
  iniappname(ADABOOSTSECT);

  logset(inigetstr("日志文件"));
  hh->ha = inigetfloat("ha");
  hh->fa = inigetfloat("fa");
  hh->min_neg_rate = inigetfloat("最小负样本比例");
  maxstagelen = inigetint("最大强分类器数");
  maxweaklen = inigetint("最大弱分类器数");
  //inigetfloat( "特征挑选比例", hh->select_rate );
  hh->flag_unreselect = inigetint("不重选特征");
  is_nesting  = inigetint("使用nesting结构");

  // 创建样本容器对象
  hh->pos_num = inigetint("正样本数目");
  hh->samp_num = inigetint("负样本数目") + hh->pos_num;
  cascade_load(CAS_DAT, hh->ca);
  cascade_save_txt(CAS_TXT, hh->ca);
  cascade_setsize(hh->ca, maxstagelen, maxweaklen);

  if (hh->ca->weaklen > 0) {
    ASSERT(hh->ca->is_nesting == is_nesting);
  }

  hh->ca->is_nesting = is_nesting;

  //cascade_printf( hh->ca, hh->cas );
  if (!hh->ca) {
    printf("打开文件错误");
    free(hh);
    return 0;
  }

  init_feat(hh);

  hh->vi = MALLOCSET(double, hh->samp_num, 0);
  hh->fi = MALLOCSET(double, hh->samp_num, 0);
  hh->wi = MALLOCSET(double, hh->samp_num, 0);      //-1表示没有样本
  //hh->vvv = MALLOCSET( double, hh->samp_num, 0 );  //-1表示没有样本
  FILL(hh->samp_num, hh->vi, 0.f);
  FILL(hh->samp_num, hh->fi, 0.f);
  FILL(hh->samp_num, hh->wi, -1.f);

  if (hh->feat_num > 0) {
    int i;
    hh->flag = MALLOCSET(unsigned char, hh->feat_num, 0);

    if (hh->ca != NULL) {
      // 去掉被选过的特征
      for (i = 0; i < hh->ca->weaklen; ++i) {
        SET_FEAT_FLAT(hh->feat_num, hh->flag, hh->ca->id[ i ], 1);
#undef SET_FEAT_FLAT
      }
    }
  }

  {
    int i = 0, k, stagecnt, weakcnt;
    int SampNum = hh->samp_num, PosNum = hh->pos_num;
    int is_continue_weak;
    int continue_runing = 0;  //表示是继续训练

    logprintf("<< boost 类型: %s, 特征类型: %s, 特征总数: %6d >>\n", hh->ca->weakfun->name, hh->ca->featfun->name, hh->feat_num);

    for (stagecnt = 0; (!hh->ca || stagecnt < hh->ca->maxstagelen) && (continue_runing || find_samp(hh)); ++stagecnt) {
      WORKINFO wi = { 0 };
      int is_continue_stage = 0;
      int baseweakcnt = (hh->ca && hh->ca->is_nesting && (hh->ca->stagelen) > 0) ? 1 : 0;
      logprintf("--------------------------------- Stage %2d ------------------------------------\n",
          (hh->ca) ? hh->ca->stagelen : stagecnt);
      logprintf("  Id  |     ha      Fa    |   Error    Threshold       Time\n");

      wi.beg = 0;
      wi.end = hh->feat_num;
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

      for (weakcnt = baseweakcnt; get_Error(hh, &wi);) {
        double* fi;
        is_continue_weak = 0;

        if (hh->flag && wi.id >= 0 && wi.id < hh->feat_num) {
          hh->flag[ wi.id ] = 1;
        }

        fi = getfeat(hh, wi.id);
        COPY(SampNum, fi, hh->fi);
        is_continue_weak = update_weight1(hh, &wi, hh->wi, hh->wi);
        ++weakcnt;

        if (!is_continue_weak) {
          // 同时满足Ha Fa
          break;
        }
      }

      is_continue_stage = weakcnt;

      if (is_continue_stage) {
        hh->ca->stage[ hh->ca->stagelen ].thd = wi.Threshold;
        hh->ca->stage[ hh->ca->stagelen ].len = weakcnt;

        for (k = 0; k < hh->samp_num; ++k) {
          if (hh->vi[ k ] < wi.Threshold) {
            // 没有通过强分类器的样本的权值赋-1，表示抛弃样本
            hh->wi[ k ] = -1.f;
          }
        }

        ++(hh->ca->stagelen);
        //fseek( hh->cas, 0, SEEK_SET );
        //cascade_printf( hh->ca, hh->cas );
        cascade_save(CAS_DAT, hh->ca);
        iniappname(ADABOOSTSECT);
        cascade_save_txt(inigetstr("分类器文件"), hh->ca);
      }
      else {
        break;
      }

    }

    logprintf("训练完毕!\n");
  }

  if (hh->ca) {
    cascade_free(hh->ca);
  }

  // 销毁样本容器对象
  FREE(hh->wi);
  FREE(hh->vi);
  FREE(hh->fi);
  //FREE(hh->Feat);
  FREE(hh->flag);
  return 0;
}

