
typedef double base_feat_type;
// h_feat
typedef struct BABS_FEAT_T {
  base_feat_type* samp; // featnum * sampnum 的矩阵
  int* allfeat;        // 所有的特征数据
}
BABS_FEAT_T;

static int BABS_uninit(BOOST* hh)
{
  BABS_FEAT_T* fs = (BABS_FEAT_T*)(hh->h_feat);
  free(fs->allfeat);
  free(fs->samp);
  free(hh->h_feat);
  hh->h_feat = 0;
  return 0;
}

int BABS_samp_data(BOOST* hh, char** buf, int* size)
{
  BABS_FEAT_T* fs = (BABS_FEAT_T*)(hh->h_feat);
  *buf = (char*)(fs->allfeat);
  *size = GetFeatNum(hh) * GetFeat_elemsize(hh);
  return 0;
}

int BABS_save_feat(BOOST* hh, int id)
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
static double* BABS_feat(BOOST* hh, int i)
{
  BABS_FEAT_T* fs = (BABS_FEAT_T*)(hh->h_feat);
  const base_feat_type* samp = (fs->samp);
  int nSampNum = GetSampNum(hh);
  samp += nSampNum * i;
  COPY(nSampNum, samp, hh->fi);
  return hh->fi;
}

// HAAR 样本
// 每张图片占空间 2*4*33*33=8712 字节
static int BABS_find_samp(BOOST* hh, int type)
{
  BABS_FEAT_T* fs = (BABS_FEAT_T*)(hh->h_feat);
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

    {
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
  }

  free(tmpFeat);
  return 0;
}

int BABS_init(BOOST* hh)
{
  int i, samp_size;
  char* sampfile;    // 样本文件 txt 或 dat 文件
  base_feat_type* samp;
  BABS_FEAT_T* fs = MALLOC(BABS_FEAT_T, 1);
  char buf[256];
  SETVTABLE(hh, BABS);
  ininame(INIFILENAME);
  _snprintf(buf, 256, "%d", FT_BABS);
  iniappname(buf);
  sampfile = inigetstr("样本文件");
  hh->feat_num = inigetint("特征数目");
  samp_size = GetFeatNum(hh) * GetSampNum(hh);
  samp = MALLOC(base_feat_type, samp_size);
  fs->samp = samp;
  hh->h_feat = fs;
  fs->allfeat = (int*) MALLOC(char, GetFeatNum(hh) * GetFeat_elemsize(hh));

  for (i = 0; i < GetFeatNum(hh); ++i) {
    ((int*)(fs->allfeat)) [ i ] = i;
  }

  {
    char* ext = strrchr(sampfile, '.');
    FILE* pf = fopen(sampfile, "rb");

    if (pf) {
      if (memcmp(ext, ".dat", 4) == 0) {
        fread(fs->samp, samp_size * sizeof(base_feat_type), 1, pf);
      }
      else if (memcmp(ext, ".txt", 4) == 0) {
        FSCANF_ALL(pf, "%f", samp_size, samp);
      }

      fclose(pf);
    }
  }

  return 1;
}
