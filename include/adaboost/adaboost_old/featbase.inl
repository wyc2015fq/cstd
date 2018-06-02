
typedef float base_feat_type;
// h_feat
typedef struct BASE_FEAT_T {
  base_feat_type* samp; // featnum * sampnum 的矩阵
  int* allfeat;        // 所有的特征数据
}
BASE_FEAT_T;

#define BASE_FEAT_VAL(A1, pw, IV) (*((A1) + (pw)))

STATIC double BASE_feat_val(double* A, int al, int* chi, int i, double IV)
{
  return BASE_FEAT_VAL(A, chi[ i ], IV);
}

#define BASE_CN 0
#define BASE_setoffset 0
#define BASE_integral 0

STATIC int BASE_print(FILE* pf, const int* chi)
{
  fprintf(pf, " %6d ", chi[ 0 ]);
  return 0;
}
STATIC int BASE_scanf(FILE* pf, int* chi)
{
  fscanf(pf, "%d", chi);
  return 0;
}

STATIC int BASE_uninit(BOOST* hh)
{
  BASE_FEAT_T* fs = (BASE_FEAT_T*)(hh->h_feat);
  free(fs->allfeat);
  free(fs->samp);
  free(hh->h_feat);
  hh->h_feat = 0;
  return 0;
}

STATIC int BASE_samp_data(BOOST* hh, char** buf, int* size)
{
  BASE_FEAT_T* fs = (BASE_FEAT_T*)(hh->h_feat);
  *buf = (char*)(fs->allfeat);
  *size = GetFeatNum(hh) * GetFeat_elemsize(hh);
  return 0;
}

STATIC int BASE_save_feat(BOOST* hh, int id)
{
  BASE_FEAT_T* fs = (BASE_FEAT_T*)(hh->h_feat);
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
STATIC double* BASE_feat(BOOST* hh, int i)
{
  BASE_FEAT_T* fs = (BASE_FEAT_T*)(hh->h_feat);
  int nSampNum = GetSampNum(hh);
  int nFeatNum = GetFeatNum(hh);
  const base_feat_type* samp = (fs->samp) + nSampNum * i;
  COPY(nSampNum, samp, hh->fi);
  return hh->fi;
}

// HAAR 样本
// 每张图片占空间 2*4*33*33=8712 字节
STATIC int BASE_find_samp(BOOST* hh, int type)
{
  BASE_FEAT_T* fs = (BASE_FEAT_T*)(hh->h_feat);
  // 由特征计算样本
  int i, j, k, ispass, cnt;
  int nFeatNum = hh->feat_num;
  int nSampNum = GetSampNum(hh);
  int offset = (type > 0 ? 0 : GetPosNum(hh));
  const base_feat_type* Samp = (fs->samp);
  int Num = (type > 0) ? (GetPosNum(hh)) : (GetSampNum(hh) - GetPosNum(hh));
  double* wi = hh->wi + offset;
  double* vi = hh->vi + offset;
  int id = type;
  double* tmpFeat = MALLOC(double, GetFeatNum(hh));
  double score;

  for (cnt = 0, k = 0, i = offset; i < (offset + Num) && (k < Num);) {
    if (wi[ k ] >= 0.f) {
      ++k;
      continue;
    }

    for (j = 0; j < nFeatNum; ++j) {
      tmpFeat[ j ] = Samp[ nSampNum * j + i ];
    }

    // 如果通过才是样本
    score = 0;
    //CASCADE_PASS(hh->ca, tmpFeat, hh->ca->id, FEAT_VAL_ID, 0, ispass, score);
    cascade_pass(hh->ca, tmpFeat, 1, 0, &ispass, &score);

    if (ispass) {
      ++cnt;
      printf("%s样本找到%6d 个! \r", id > 0 ? "正" : "负", cnt);

      //LOGPRINTF(hh->log, "%d号%s样本通过。"ENDL, i, ff[(idx[i].id+1)&3]);
      //hh->samp_idx[k] = idx[i].id;
      wi[ k ] = 1.f;
      vi[ k ] = score;
    }
    else {
      wi[ k ] = -1.f;
    }

    ++k;
    ++i;
  }

  free(tmpFeat);
  return 0;
}

int read_matrix(int rows, int cols, void* A, int elemsize, int step, FILE* pf, int tr) {
  int i, j;
  char* m = (char*)A;
  if (tr) {
    for (j=0; j<cols; ++j) {
      for (i=0; i<rows; ++i) {
        fread(m + i*step+ j*elemsize, elemsize, 1, pf);
      }
    }
  } else {
    for (i=0; i<rows; ++i) {
      for (j=0; j<cols; ++j) {
        fread(m + i*step+ j*elemsize, elemsize, 1, pf);
      }
    }
  }
  return 0;
}

STATIC int BASE_init(BOOST* hh)
{
  int i, samp_size, nn;
  char* sampfile;    // 样本文件 txt 或 dat 文件
  base_feat_type* samp;
  BASE_FEAT_T* fs = MALLOC(BASE_FEAT_T, 1);
  sampfile = inigetstr("样本文件");
  hh->feat_num = inigetint("特征数目");
  samp_size = GetFeatNum(hh) * GetSampNum(hh);
  samp = MALLOC(base_feat_type, samp_size);
  fs->samp = samp;
  hh->h_feat = fs;
  nn = GetFeatNum(hh) * GetFeat_elemsize(hh);
  fs->allfeat = (int*) MALLOC(char, nn);

  for (i = 0; i < GetFeatNum(hh); ++i) {
    ((int*)(fs->allfeat)) [ i ] = i;
  }

  {
    char* ext = strrchr(sampfile, '.');
    FILE* pf = fopen(sampfile, "rb");

    if (pf) {
      if (memcmp(ext, ".dat", 4) == 0) {
        //fread(fs->samp, samp_size * sizeof(base_feat_type), 1, pf);
        read_matrix(hh->feat_num, hh->samp_num, fs->samp, sizeof(base_feat_type), hh->samp_num*sizeof(base_feat_type), pf, 0);
      }
      else if (memcmp(ext, ".txt", 4) == 0) {
        FSCANF_ALL(pf, "%f", samp_size, samp);
      }

      fclose(pf);
    }
  }

  return 1;
}
