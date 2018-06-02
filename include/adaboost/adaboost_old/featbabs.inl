

typedef double babs_feat_type;
// h_feat
typedef struct BABS_FEAT_T {
  babs_feat_type* samp; // featnum * sampnum 的矩阵
  int* allfeat;        // 所有的特征数据
  int* samp_idx;        // [samp_num] 样本索引，表示被选入的样本
  SAMPPAIR_T spair[1];
  int rows;
  int cols;
}
BABS_FEAT_T;

#define BABS_setoffset 0
#define BABS_CN 0
#define BABS_integral 0
#define BABS_print BASE_print
#define BABS_scanf BASE_scanf
#define BABS_feat_val BASE_feat_val

STATIC int BABS_uninit(BOOST* hh)
{
  BABS_FEAT_T* fs = (BABS_FEAT_T*)(hh->h_feat);
  FREE(fs->allfeat);
  FREE(fs->samp);
  FREE(hh->h_feat);
  hh->h_feat = 0;
  samppair_free(fs->spair);
  return 0;
}

STATIC int BABS_samp_data(BOOST* hh, char** buf, int* size)
{
  BABS_FEAT_T* fs = (BABS_FEAT_T*)(hh->h_feat);
  *buf = (char*)(fs->allfeat);
  *size = hh->feat_num * sizeof(int);
  return 0;
}

STATIC int BABS_save_feat(BOOST* hh, int id)
{
  BABS_FEAT_T* fs = (BABS_FEAT_T*)(hh->h_feat);
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
#define BABS_feati(fs, isamp, jfeat)  fs->samp[jfeat*(fs->cols)+isamp]
STATIC double* BABS_feat(BOOST* hh, int i)
{
  BABS_FEAT_T* fs = (BABS_FEAT_T*)(hh->h_feat);
  SAMPPAIR* pair = fs->spair->pair;
  const babs_feat_type* samp = (fs->samp);
  int nSampNum = GetSampNum(hh);
  int nFeatNum = GetFeatNum(hh);
  int j;

  for (j = 0; j < nSampNum; ++j) {
    int m = fs->samp_idx[ j ];

    if (m >= 0) {
      int a = pair[m].a, b = pair[m].b;
      double da, db;
      da = BABS_feati(fs, a, i);
      db = BABS_feati(fs, b, i);
      hh->fi[ j ] = da > db ? da - db : db - da;
    }
    else {
      hh->fi[ j ] = 0;
    }
  }

  return hh->fi;
}

// HAAR 样本
// 每张图片占空间 2*4*33*33=8712 字节
STATIC int BABS_find_samp(BOOST* hh, int type)
{
  BABS_FEAT_T* fs = (BABS_FEAT_T*)(hh->h_feat);
  // 由特征计算样本
  int i, j, m, mbeg, mend, ispass, cnt;
  int nFeatNum = hh->feat_num;
  int nSampNum = GetSampNum(hh);
  int offset = (type > 0 ? 0 : GetPosNum(hh));
  const babs_feat_type* samp = (fs->samp);
  int Num = (type > 0) ? (GetPosNum(hh)) : (GetSampNum(hh) - GetPosNum(hh));
  double* wi = hh->wi;
  double* vi = hh->vi;
  int id = type;
  double* tmpFeat = MALLOC(double, hh->feat_num);
  double score;
  SAMPPAIR* pair = fs->spair->pair;

  mbeg = type > 0 ? 0 : fs->spair->pos_num;
  mend = type > 0 ? fs->spair->pos_num : fs->spair->pairlen;

  for (cnt = 0, i = offset, m = mbeg; m < mend && i < (offset + Num); ++m) {
    if (wi[ i ] >= 0.f) {
      ++i;
      continue;
    }

    if (pair[m].a >= 0) {
      int a = pair[m].a, b = pair[m].b;
      double da, db;

      for (j = 0; j < nFeatNum; ++j) {
        da = BABS_feati(fs, a, j);
        db = BABS_feati(fs, b, j);
        tmpFeat[ j ] = da > db ? da - db : db - da;
      }

      // 如果通过才是样本
      score = 0;
      //CASCADE_PASS(hh->ca, tmpFeat, hh->ca->id, FEAT_VAL_ID, 0, ispass, score);
      cascade_pass(hh->ca, tmpFeat, 1, 0, &ispass, &score);

      if (ispass) {
        ++cnt;
        printf("%s样本找到%6d 个! \r", id > 0 ? "正" : "负", cnt);

        //LOGPRINTF(hh->log, "%d号%s样本通过。"ENDL, i, ff[(pair[i].id+1)&3]);
        //hh->samp_idx[k] = pair[i].id;
        wi[ i ] = 1.f;
        vi[ i ] = score;
        fs->samp_idx[ i ] = m;
        ++i;
      }
      else {
        //wi[ i ] = -1.f;
        pair[m].a = -a - 1;
        fs->samp_idx[ i ] = -1;
      }

    }
  }

  printf("%s样本找到%6d 个! 找了%d个，剩%d个\n", id > 0 ? "正" : "负", cnt, m - mbeg, mend - m);

  FREE(tmpFeat);
  return 0;
}

int BABS_init(BOOST* hh)
{
  int i, samp_size, img_per_men, men_num;
  char* sampfile;    // 样本文件 txt 或 dat 文件
  babs_feat_type* samp;
  int nSampNum = GetSampNum(hh);
  BABS_FEAT_T* fs = MALLOC(BABS_FEAT_T, 1);
  sampfile = inigetstr("样本文件");
  hh->feat_num = inigetint("特征数");
  img_per_men = inigetint("每人图片数");
  men_num = inigetint("总人数");

  samppair_init(fs->spair, men_num, img_per_men);
  samp_size = img_per_men * men_num * hh->feat_num;
  samp = MALLOC(babs_feat_type, samp_size);
  fs->cols = img_per_men * men_num;
  fs->rows = hh->feat_num;
  fs->samp = samp;
  hh->h_feat = fs;
  fs->allfeat = MALLOC(int, hh->feat_num);
  fs->samp_idx = MALLOC(int, nSampNum);

  for (i = 0; i < nSampNum; ++i) {
    fs->samp_idx[ i ] = -1;
  }

  for (i = 0; i < hh->feat_num; ++i) {
    fs->allfeat[ i ] = i;
  }

  {
    char* ext = strrchr(sampfile, '.');
    FILE* pf = fopen(sampfile, "rb");

    if (pf) {
      int n;

      if (memcmp(ext, ".dat", 4) == 0) {
        n = fread(fs->samp, 1, samp_size * sizeof(babs_feat_type), pf);
      }
      else if (memcmp(ext, ".txt", 4) == 0) {
        FSCANF_ALL(pf, "%f", samp_size, samp);
      }

      fclose(pf);
    }
  }

  return 1;
}
