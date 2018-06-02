
#include "iris.h"
#include "img/imgset.inl"
#include "iris/omkernel.inl"


#define HIMM_FEAT_VAL(A1, pw, IV) (*((A1) + (pw)))


typedef uchar himm_feat_type;
// h_feat
typedef struct HIMM_FEAT_T {
  himm_feat_type* data; // featnum * sampnum 的矩阵
  int* allfeat;         // 所有的特征数据
  uchar* flag;
  IRECT rc[IRIS_BLOCK_NUM];
  SAMPPAIR_T sp[1];
  int* ids;
  int* idx;
  int imgnum, blocknum, omnum, datastep;
}
HIMM_FEAT_T;

STATIC double HIMM_feat_val(double* A, int al, int* chi, int i, double IV)
{
  return HIMM_FEAT_VAL(A, chi[ i ], IV);
}

#define HIMM_CN 0
#define HIMM_setoffset 0
#define HIMM_integral 0

STATIC int HIMM_print(FILE* pf, const int* chi)
{
  fprintf(pf, " %6d ", chi[ 0 ]);
  return 0;
}

STATIC int HIMM_scanf(FILE* pf, int* chi)
{
  fscanf(pf, "%d", chi);
  return 0;
}

STATIC int HIMM_uninit(BOOST* hh)
{
  HIMM_FEAT_T* fs = (HIMM_FEAT_T*)(hh->h_feat);
  free(fs->allfeat);
  free(fs->data);
  free(fs->ids);
  free(fs->idx);
  free(hh->h_feat);
  hh->h_feat = 0;
  return 0;
}

STATIC int HIMM_samp_data(BOOST* hh, char** buf, int* size)
{
  HIMM_FEAT_T* fs = (HIMM_FEAT_T*)(hh->h_feat);
  *buf = (char*)(fs->allfeat);
  *size = GetFeatNum(hh) * GetFeat_elemsize(hh);
  return 0;
}

STATIC int HIMM_save_feat(BOOST* hh, int id)
{
  HIMM_FEAT_T* fs = (HIMM_FEAT_T*)(hh->h_feat);
  int es = GetFeat_elemsize(hh);
  char* dst = (char*)(hh->ca->featptr) + hh->ca->weaklen * es;

  if (id >= 0) {
    char* src = (char*)(fs->allfeat) + id * es;
    memcpy(dst, src, es);
  }
  else {
    memset(dst, 0, es);
  }

  hh->ca->weaklen++;
  return 0;
}

// 总库中第j个样本的所有特征
STATIC int HIMM_sampfeat(BOOST* hh, int j, double* samp)
{
  HIMM_FEAT_T* fs = (HIMM_FEAT_T*)(hh->h_feat);
  int k = j;
  int i, a = fs->sp->pair[k].a, b = fs->sp->pair[k].b;
  uchar* fa = fs->data + a * fs->datastep;
  uchar* fb = fs->data + b * fs->datastep;

  for (i = 0; i < hh->feat_num; ++i) {
#if 1
    int iom = i / IRIS_BLOCK_NUM;
    int iblock = i % IRIS_BLOCK_NUM;
    int off = iom * IRIS_OMLEN;
    samp[i] = himm_block_dis(fa + off, fb + off, fs->rc[iblock]);
#else
    samp[i] = poptable[fa[i] ^ fb[i]];
#endif
  }

  return 0;
}

// 选入库中所有样本的第i个特征
STATIC double* HIMM_feat(BOOST* hh, int i)
{
  HIMM_FEAT_T* fs = (HIMM_FEAT_T*)(hh->h_feat);
  int iom = i / IRIS_BLOCK_NUM;
  int iblock = i % IRIS_BLOCK_NUM;
  int ii;

  for (ii = 0; ii < hh->samp_num; ++ii) {
    int k = fs->idx[ii];
#if 1
    const uchar* fa = fs->data + fs->sp->pair[k].a * fs->datastep + iom * IRIS_OMLEN;
    const uchar* fb = fs->data + fs->sp->pair[k].b * fs->datastep + iom * IRIS_OMLEN;
    hh->fi[ii] = himm_block_dis(fa, fb, fs->rc[iblock]);
#else
    const uchar* fa = fs->data + fs->sp->pair[k].a * fs->datastep;
    const uchar* fb = fs->data + fs->sp->pair[k].b * fs->datastep;
    hh->fi[ii] = poptable[fa[i] ^ fb[i]];
#endif
  }

  return hh->fi;
}

// HAAR 样本
// 每张图片占空间 2*4*33*33=8712 字节
STATIC int HIMM_find_samp(BOOST* hh, int type)
{
  HIMM_FEAT_T* fs = (HIMM_FEAT_T*)(hh->h_feat);
  // 由特征计算样本
  int i, k, ispass, cnt = 0, pass_cnt = 0;
  int nFeatNum = hh->feat_num;
  int nSampNum = GetSampNum(hh);
  int offset = (type > 0 ? 0 : GetPosNum(hh));
  int Num = (type > 0) ? (GetPosNum(hh)) : (GetSampNum(hh));
  double* wi = hh->wi;
  double* vi = hh->vi;
  int id = type;
  double* tmpFeat = MALLOC(double, GetFeatNum(hh));
  double score;

  for (k = offset, i = 0; i < fs->sp->pairlen && (k < Num);) {
    int a = fs->sp->pair[i].a, b = fs->sp->pair[i].b;

    if (fs->flag[i]) {
      ++i;
      continue;
    }

    if ((fs->ids[a] == fs->ids[b]) != (type > 0)) {
      ++i;
      continue;
    }

    if (wi[ k ] >= 0.f) {
      ++k;
      continue;
    }

    {
      fs->flag[i] = 1;
      HIMM_sampfeat(hh, i, tmpFeat);
      // 如果通过才是样本
      score = 0;
      //CASCADE_PASS(hh->ca, tmpFeat, hh->ca->id, FEAT_VAL_ID, 0, ispass, score);
      cascade_pass(hh->ca, tmpFeat, 1, 0, &ispass, &score);

      ++cnt;

      if (ispass) {
        ++pass_cnt;
        printf("%s样本找到%6d/%6d 个! \r", id > 0 ? "正" : "负", pass_cnt, cnt);

        //LOGPRINTF(hh->log, "%d号%s样本通过。"ENDL, i, ff[(idx[i].id+1)&3]);
        //hh->samp_idx[k] = idx[i].id;
        wi[ k ] = 1.f;
        vi[ k ] = score;
        fs->idx[k] = i;
        ++k;
      }
      else {
        wi[ k ] = -1.f;
      }

      ++i;
    }
  }

  logprintf("%s样本找到%6d/%6d 个! \n", id > 0 ? "正" : "负", pass_cnt, cnt);

  FREE(tmpFeat);
  return 0;
}

STATIC int HIMM_init(BOOST* hh)
{
  int i, data_size;
  himm_feat_type* data;
  HIMM_FEAT_T* fs = MALLOC(HIMM_FEAT_T, 1);
  MEMSET(fs, 0, 1);
  fs->blocknum = make_block(IRIS_S, IRIS_H, 0, 0, IRIS_BLOCK_W, IRIS_BLOCK_H, IRIS_BLOCK_TX, IRIS_BLOCK_TY, fs->rc, IRIS_BLOCK_NUM);
  ASSERT(fs->blocknum == IRIS_BLOCK_NUM);
  fs->datastep = makeom_bit(0, 0);
  fs->omnum = fs->datastep / IRIS_OMLEN;
  //hh->feat_num = fs->omnum * IRIS_OMLEN;
  hh->feat_num = fs->omnum * fs->blocknum;
  hh->h_feat = fs;

  fs->allfeat = MALLOC(int, GetFeatNum(hh));

  for (i = 0; i < GetFeatNum(hh); ++i) {
    fs->allfeat[ i ] = i;
  }

  {
    uchar map[256] = {0};
    imgset_t ims[1] = {0};
    SAMPPAIR_T* sp = fs->sp;
    char* sampfile;    // 样本文件 txt 或 dat 文件
    int f;
    map_init(map, "0123456789", 10);
    map_init(map, "LR", 2);
    sampfile = inigetstr("图片txt文件");
    imgset_read(ims, 1, sampfile, 2, 4, map);
    //imgset_read(ims, 1, sampfile, 31, 4, map);
    f = ims->im->f;
    samppair_init2(sp, ims->im->f, ims->id);
    ASSERT(IRIS_H == ims->im->h && IRIS_W == ims->im->w);
    data_size = f * fs->datastep;
    data = MALLOC(himm_feat_type, data_size);
    fs->data = data;
    fs->imgnum = f;
    fs->ids = MALLOC(int, f);
    fs->idx = MALLOC(int, GetSampNum(hh));
    fs->flag = MALLOC(uchar, sp->pairlen);
    MEMSET(fs->flag, 0, sp->pairlen);
    MEMCPY(fs->ids, ims->id, f);
    MEMSET(fs->idx, 0, GetSampNum(hh));

    for (i = 0; i < f; ++i) {
      img_t im[1] = {0};
      uchar* A = ims->im->tt.data + i * IRIS_H * IRIS_W;
      makeom_bit(A, data + i * fs->datastep);
      //makeom1(im->tt.data, fts + i * featlen);
      printf("计算特征 %4d/%4d\r", i, ims->im->f);
    }

    imgset_free(ims);
  }

  return 1;
}
