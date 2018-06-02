
#include "cstd.h"
#include "filecache.inl"
#include "samppair.inl"
#include "libfacerecog.h"
#include "cfile.h"
#include "net/mpi.h"
//#include "../chcsock/chcsock.inl"
#include "face/face.inl"
//#include "../faceDetect/edgegradLBP.inl"
#include "img/color.inl"
#include "img/imgio.inl"

#define CHIFEAT int
#define CHI_setoffset 0
#define CHI_CN 0
#define CHI_print BASE_print
#define CHI_scanf BASE_scanf
#define CHI_feat_val BASE_feat_val
#define CHI_integral 0

#define FEATFILECOL ".\\featfilecol.dat"
#define CHI_FEAT_FILE ".\\CHI_FEAT_FILE.dat"
#define ALLIMGDATA_FILE ".\\allimgdata.dat"
#define CHI_FEAT_IDXFLAG_FILE ".\\CHI_FEAT_IDXFLAG_FILE.dat"

#define USEFILEBLOCK
#define COLFEAT // 特征安列存放


int lbp_ex_ft_ith(int srch, int srcw, const unsigned char* src, int srcstep, int cn, int ith, int fillborder);
//int lbp_ex_ft_all( int srch, int srcw, const unsigned char* src, int srcstep, int cn, FEAT_T* me, int fillborder );
int CPM_GetFeature1(int srch, int srcw, const unsigned char* src, int srcstep, int cn, int type, void* featptr);

//#pragma comment(lib, "libmonofilt.lib")
int lbp59_feat(int srch, int srcw, const unsigned char* src, int srcstep, int cn, int type, void* featptr);
int monofilt_feat2(int srch, int srcw, const unsigned char* src, int srcstep, int cn, int type, void* featptr);

int GetFeature(int srch, int srcw, const unsigned char* src, int srcstep, int cn, int type, FEAT_T* featptr)
{
  //return CPM_GetFeature1( srch, srcw, src, srcstep, cn, type, featptr ) / sizeof( FEAT_T );
  //return monofilt_feat2( srch, srcw, src, srcstep, cn, type, featptr ) / sizeof( FEAT_T );

  //return CPM_GetFeature1( srch, srcw, src, srcstep, cn, PPLBPXBIN, featptr ) / sizeof( FEAT_T );
  return CPM_GetFeature1(srch, srcw, src, srcstep, cn, PPLBPXBIN, featptr) / sizeof(FEAT_T);
  //return edgegradLBP( src, srcstep, featptr );
  //return lbp59_feat( srch, srcw, src, srcstep, cn, type, featptr ) / sizeof( FEAT_T );
}
#if 1
// 规一化图片高和宽
#define STDH  100
#define STDW  100
#ifdef USEFILEBLOCK
#define DOPP(_PTR)
#define FEATTYPE FEAT_T
#define GETFEATURE(_PTR, _OUT) (GetFeature(STDH, STDW, _PTR, STDW, 1, PPLBPXBIN, _OUT))
//#define GETFEATURE(_PTR, _OUT) (GetFeature(STDH, STDW, _PTR, STDW, 1, PPBOOSTLBPXBIN, _OUT))
#define GETFEATUREI(_PTR, _I)  (lbp_ex_ft_ith(STDH, STDW, _PTR, STDW, 1, _I, 1))
#else
#define FEATTYPE float
#define DOPP(_PTR)             CPM_PpLight(STDH, STDW, _PTR, STDW, 1, _PTR, STDW, 1)
#define GETFEATURE(_PTR, _OUT) lbp_ex_ft_all(STDH, STDW, _PTR, STDW, 1, _OUT, 1)
#define GETFEATUREI(_PTR, _I)  lbp_ex_ft_ith(STDH, STDW, _PTR, STDW, 1, _I, 1)
#endif
#else
//#include "../dfd/test_DFD_example.inl"
// 规一化图片高和宽
#define STDH  150
#define STDW  130
#define FEATTYPE float
#define GETFEATURE(_PTR, _OUT) DFD_feat1(STDH, STDW, _PTR, STDW, 1, _OUT)
#endif

// 人脸识别训练样本结构
// a 第一样本, b第二样本
// 正样本a<b 负样本a>b 非样本或已经用过的样本a=b
#define XCMP1(a,b)     (((a) > (b)) - ((a) < (b)))
// 正样本1 负样本-1 非样本或已经用过的样本0
//#define SAMPPAIRID(s)  XCMP1((s).b, (s).a)
//#define LOSS(s)        ((s).b=(s).a)
//#define SETSAMPPAIR(ispos, s)  if (XCMP1(ispos, 0) != (SAMPPAIRID(s))) {int t; CV_SWAP((s).b, (s).a, t);}

#define SAMPPAIRID(s)  (s)
#define LOSS(s)        ((s)*=2)
#define SETSAMPPAIR(ispos, s)  (SAMPPAIRID(s) = ispos ? 1:-1)

// 样本结构
typedef struct CHI_FEAT_T {
  int* samp_idx;    // [samp_num] 样本索引，表示被选入的样本
  SAMPPAIR* idx;    // 样本结构
  char* idxflag;     // 标记样本结构中那些是正负样本和抛弃的样本(正=1,负=-1,抛弃=0)
  unsigned char* allimgdata;
  FEATTYPE* samp;
  //int iscolfeat; // 表示samp中一个特征向量是列表示还是行表示
  FILECACHE* fileblockcol;
  HD_FILE featfilecol;  // [samp_num×samp_step] 样本数据文件（当样本太大时的磁盘文件）

  int idx_beg;      // 开始找的id
  int img_num;      // 总图片数
  //int man_num;      // 总人数
  //int img_man;      // 每个人的图片数目
  int feat_num;
  int idxlen;       // 样本结构长度
  int dtype;        // 距离类型 (3==CPM_ABSSUB 0==CPM_CHISQUARE)
}
CHI_FEAT_T;

int CHI_samp_data(BOOST* hh, char** buf, int* size)
{
  CHI_FEAT_T* fs = (CHI_FEAT_T*)(hh->h_feat);
  *buf = (char*)(fs->samp_idx);
  *size = (hh)->samp_num * sizeof(int);
  return 0;
}

int CHI_save_feat(BOOST* hh, int id)
{
  if (id < 0) {
    hh->ca->weaklen++;
  }
  else {
    int* chi = (int*)(hh->ca->featptr);
    chi[ hh->ca->weaklen++ ] = id;
  }

  return 0;
}

int get_id_from_file_name(const char* buf, unsigned int times)
{
  int id = -1;
  const char* filename = strrchr(buf, '\\');

  if (!filename) {
    filename = strrchr(buf, '/');
  }

  for (; times-- && filename > buf;) {
    const char* p = filename - 1;

    for (; '\\' != *p && '/' != *p && p > buf; --p)
      ;

    if (buf == p) {
      break;
    }

    filename = p;
  }

  if (!filename) {
    filename = buf;
  }
  else {
    filename += 1;
  }

  filename = strpbrk(filename, "0123456789");
  sscanf(filename, "%d", &id);
  return id;
}

//#define ALLIMGDATAFILENAME "allimgdata.dat"

int loadallimg(const char* listfile, int numb, int rows, int cols, uchar* imgdata, int* img_id)
{
  char buf[ 256 ];
  int i = 0;
  FILE* pf = fopen(listfile, "rb");

  if (pf == NULL) {
    return 0;
  }

  for (; i < numb && fscanf(pf, "%s", buf) > 0;) {
    int height, width, channels = 1;
    img_t im[1] = {0};
    ;

    if (!imread(buf, 1, 1, im)) {
      continue;
    }

    height = im->h, width = im->w;
    printf("(%4d) %3d %3d %s\r", i + 1, height, width, strrchr(buf, '\\') + 1);
    assert(height == rows && width == cols);

    if (img_id) {
      img_id[ i ] = get_id_from_file_name(buf, 0);
    }

    memcpy(imgdata + i * rows * cols, im->tt.data, im->h * im->w);
    imfree(im);
    ++i;
  }

  return i;
}

int chi_make_samppair(CHI_FEAT_T* fs, int* img_id)
{
  int idxsz = (fs->img_num - 1) * fs->img_num / 2;
  fs->idx = MALLOCSET(SAMPPAIR, idxsz, 0);
  fs->idxflag = MALLOCSET(char, idxsz, 0);
  {
    int i, j;
    int img_num = fs->img_num;
    fs->idxlen = 0;

    for (i = 0; i < img_num; ++i) {
      for (j = i + 1; j < img_num; ++j) {
        fs->idx[ fs->idxlen ].a = i;
        fs->idx[ fs->idxlen ].b = j;
        //fs->idx[fs->idxlen].id = ((i / img_man) == (j / img_man) ? 1 : -1);
        //fs->idx[ fs->idxlen ].id = ( img_id[ i ] == ( img_id[ j ] ) ? 1 : -1 );
        SETSAMPPAIR(img_id[ i ] == img_id[ j ], fs->idxflag[ fs->idxlen ]);
        ++(fs->idxlen);
      }
    }

    // 调试版不随机排序
    if (0) {
      int idxlen = fs->idxlen;
      srand(0);   // 随机数初始化方式
      //srand((int) time(NULL));
      RANDOM_SHUFFLE((unsigned int) idxlen, fs->idx, SAMPPAIR);
    }

#if 1

    if (1) {
      int img_man = inigetint("每个人的图片数目");
      int idxlen = fs->idxlen;
      int img_num = fs->img_num;
      int max_man_num;
      int k, t2, all_pairs;
      int sampnum = 0;
      SAMPPAIR t;
      SAMPPAIR* idx = fs->idx;
      SAMPPAIR* rr;
      sampnum = 0;
      max_man_num = img_num / img_man;

      for (i = 0; i < idxlen; ++i) {
        if (1 == SAMPPAIRID(fs->idxflag[ i ])) {
          CV_SWAP(idx[ i ], idx[ sampnum ], t);
          CV_SWAP(fs->idxflag[ i ], fs->idxflag[ sampnum ], t2);
          ++sampnum;
        }
      }

      all_pairs = (max_man_num) * (max_man_num) / 2;
      rr = MALLOCSET(SAMPPAIR, all_pairs, 0);
      assert(NULL != rr);

      for (k = 0, i = 0; i < max_man_num; ++i) {
        for (j = i + 1; j < max_man_num; ++j) {
          rr[ k ].a = i;
          rr[ k ].b = j;
          ++k;
        }
      }

      all_pairs = k;
      RANDOM_SHUFFLE(all_pairs, rr, SAMPPAIR);

      for (k = 0; k < all_pairs && sampnum < idxlen; ++k) {
        int a, b;

        if (all_pairs < 100 || k % (all_pairs / 100) == 2) {
          printf("选择样本%3d/%3d\r", k, all_pairs);
        }

        a = rr[ k ].a * (img_man);
        b = rr[ k ].b * (img_man);

        // 选择第a个人和第b个人的所有图片对压栈
        for (i = 0; i < img_man; ++i) {
          for (j = 0; j < img_man; ++j) {
            fs->idx[ sampnum ].a = a + i;
            fs->idx[ sampnum ].b = b + j;
            //fs->idx[ sampnum ].id = -1;
            SETSAMPPAIR(0, fs->idxflag[ sampnum ]);
            ++sampnum;
          }
        }
      }

#define RANDPAIR_FILENAME "randpair.txt"

      if (0 && !fileok(RANDPAIR_FILENAME)) {
        FILE* pf = fopen(RANDPAIR_FILENAME, "w");

        if (NULL != pf) {
          for (i = 0; i < all_pairs; ++i) {
            fprintf(pf, "%3d %3d\n", rr[ i ].a, rr[ i ].b);
          }

          fclose(pf);
        }
      }

      FREE(rr);
    }

#endif

  }
  return 0;
}

int calc_feat(CHI_FEAT_T* fs, const uchar* allimgdata, int iscol)
{
  int i;
  //if ( fs->iscolfeat != iscol )
  {
    if (iscol) {
      FEATTYPE* tmp = MALLOC(FEATTYPE, fs->feat_num);

      for (i = 0; i < fs->img_num; ++i) {
        printf("计算特征 (%4d/%d)\r", i + 1, fs->img_num);
        GETFEATURE(allimgdata + i * STDH * STDW, tmp);
        COPY1D(fs->feat_num, tmp, 1, fs->samp + i, fs->img_num);
        //GETFEATURE( allimgdata+i*STDH*STDW, fs->samp+i*fs->feat_num );
      }

      FREE(tmp);
    }
    else {
      for (i = 0; i < fs->img_num; ++i) {
        printf("计算特征 (%4d/%d)\r", i + 1, fs->img_num);
        GETFEATURE(allimgdata + i * STDH * STDW, fs->samp + i * fs->feat_num);
      }
    }

    //fs->iscolfeat = iscol;
  }
  return 0;
}
static int CHI_uninit(BOOST* hh)
{
  CHI_FEAT_T* fs = (CHI_FEAT_T*)(hh->h_feat);
  FREE(fs->idx);
  FREE(fs->samp_idx);
  FREE(fs->allimgdata);

  if (fs->fileblockcol) {
    fc_close(fs->fileblockcol);
    fs->fileblockcol = NULL;
  }

  if (fs->featfilecol) {
    FCLOSE(fs->featfilecol);
    fs->featfilecol = NULL;
  }

  FREE(hh->h_feat);
  return 0;
}

// 取所有样本的第i个特征
static double* CHI_featMemCol(BOOST* hh, int i)
{
  int j, k, a, b;
  CHI_FEAT_T* fs = (CHI_FEAT_T*)(hh->h_feat);
  double* tmpSamp = hh->fi;
  int feat_num = fs->feat_num;
  int img_num = fs->img_num;
  const FEATTYPE* samp = fs->samp, *p = NULL, *f = NULL;
  int pos = img_num * i;

  if (NULL != samp) {
    switch (fs->dtype) {
    case CPM_CHISQUARE:
      for (j = 0; j < hh->samp_num; ++j) {
        if (hh->wi[ j ] < 0.f) {
          continue;
        }

        k = fs->samp_idx[ j ];
        a = pos + (fs->idx[ k ].a);
        b = pos + (fs->idx[ k ].b);
        CHI_DIST(samp[ a ], samp[ b ], tmpSamp[ j ]);
      }

      break;

    case CPM_ABSSUB:
      for (j = 0; j < hh->samp_num; ++j) {
        if (hh->wi[ j ] < 0.f) {
          continue;
        }

        k = fs->samp_idx[ j ];
        a = pos + (fs->idx[ k ].a);
        b = pos + (fs->idx[ k ].b);
        ABS_DIST(samp[ a ], samp[ b ], tmpSamp[ j ]);
      }

      break;

    default:
      return 0;
      break;
    }
  }

  return tmpSamp;
}
static double* CHI_featFileCol(BOOST* hh, int i)
{
  int j, k, a, b;
  CHI_FEAT_T* fs = (CHI_FEAT_T*)(hh->h_feat);
  double* tmpSamp = hh->fi;
  int feat_num = fs->feat_num;
  int img_num = fs->img_num;
  const FEATTYPE* samp = fs->samp, *p = NULL, *f = NULL;
  int pos = img_num * i;
  ASSERT(NULL != fs->fileblockcol);
  {
    FEATTYPE* feati = NULL;
    feati = fc_get(fs->fileblockcol, i, fs->featfilecol);

    switch (fs->dtype) {
    case CPM_CHISQUARE:
      for (j = 0; j < hh->samp_num; ++j) {
        if (hh->wi[ j ] < 0.f) {
          continue;
        }

        k = fs->samp_idx[ j ];
        a = (fs->idx[ k ].a);
        b = (fs->idx[ k ].b);
        CHI_DIST(feati[ a ], feati[ b ], tmpSamp[ j ]);
      }

      break;

    case CPM_ABSSUB:
      for (j = 0; j < hh->samp_num; ++j) {
        if (hh->wi[ j ] < 0.f) {
          continue;
        }

        k = fs->samp_idx[ j ];
        a = (fs->idx[ k ].a);
        b = (fs->idx[ k ].b);
        ABS_DIST(feati[ a ], feati[ b ], tmpSamp[ j ]);
      }

      break;

    default:
      break;
    }
  }
  return tmpSamp;
}

// 去文本文件的第i行（0序）
static int get_i_line(const char* fname, int i)
{
  FILE* out = NULL;
  FILE* pf = fopen(fname, "r");
  char* pp;
  int j;
  char buf[ 1024 ];

  if (NULL == out) {
    out = fopen(".\\outline.txt", "w");
  }

  if (NULL == pf) {
    return 0;
  }

  for (j = 0; (pp = fgets(buf, 1024, pf)) && j < i; ++j) {}

  if (j == i) {
    if (NULL == out) {
      printf(buf);
    }
    else {
      fprintf(out, buf);
    }
  }

  return 1;
}

// 找样本：原始数据 -> 样本数据(或文件)
// id=-1 表示找负样本
// id=1  表示找正样本
int CHI_find_samp(BOOST* hh, int type)
{
  // 由特征计算样本
  int i, j, k, finded, findok, cnt, ispass;
  CHI_FEAT_T* fs = (CHI_FEAT_T*)(hh->h_feat);
  int nFeatNum = fs->feat_num;
  int nSampNum = hh->samp_num;
  FEATTYPE* Samp = fs->samp;
  SAMPPAIR* idx = fs->idx;
  int idxlen = fs->idxlen;
  int Num = (type > 0) ? (hh->pos_num) : (hh->samp_num - hh->pos_num);
  double* wi = hh->wi + (type > 0 ? 0 : hh->pos_num);
  double* vi = hh->vi + (type > 0 ? 0 : hh->pos_num);
  int* samp_idx = fs->samp_idx + (type > 0 ? 0 : hh->pos_num);
  int id = type, idx_beg, allfind = 0;
  double* tmpFeat = MALLOC(double, nFeatNum);
  double score;
  utime_start(time);
#ifdef USEFILEBLOCK

  FILECACHE* fileblockrow = NULL;
  fileblockrow = fc_open(0, fs->img_num, nFeatNum * sizeof(FEATTYPE));
#endif

  //if (id<0) // 保存上一级已经找过的样本，避免下次再找
  //  savedata(CHI_FEAT_IDXFLAG_FILE, fs->idxflag, fs->idxlen*sizeof(char), 0);
  assert(-1 == id || 1 == id);
  fs->idx_beg = 0;
  idx_beg = fs->idx_beg;
  cnt = 0, k = 0, i = fs->idx_beg, finded = 0, findok = 0;

  for (; i < idxlen && (k < Num); ++allfind) {
    //assert(idx[i].id >= -1 && idx[i].id <= 1 && "idx[i].id 越界！");
    if (SAMPPAIRID(fs->idxflag[ i ]) != id) {
      ++i;
      cnt += (SAMPPAIRID(fs->idxflag[ i ]) == 0);     // 加上以前被抛弃的样本
      continue;
    }

    if (wi[ k ] >= 0.f) {
      ++k; // 加上以前剩下的样本
      continue;
    }

    {
      score = 0.f;
      ispass = 1;

      if (hh->ca->stagelen > 0) {
        if (NULL != Samp) {
#ifdef COLFEAT
          int jj, img_num = fs->img_num;
          const FEATTYPE* p = Samp + (idx[ i ].a);
          const FEATTYPE* f = Samp + (idx[ i ].b);

          //assert(idx[i].a >= 0 && idx[i].a<nSampNum && "find_samp: idx[i].a 越界！");
          //assert(idx[i].b >= 0 && idx[i].b<nSampNum && "find_samp: idx[i].b 越界！");
          switch (fs->dtype) {
          case CPM_CHISQUARE:
            for (j = 0, jj = 0; j < nFeatNum; ++j, jj += img_num) {
              CHI_DIST(p[ jj ], f[ jj ], tmpFeat[ j ]);
            }

            break;

          case CPM_ABSSUB:
            for (j = 0, jj = 0; j < nFeatNum; ++j, jj += img_num) {
              ABS_DIST(p[ jj ], f[ jj ], tmpFeat[ j ]);
            }

            break;

          default:
            break;
          }

#else
          const FEATTYPE* p = Samp + nFeatNum * (idx[ i ].a);
          const FEATTYPE* f = Samp + nFeatNum * (idx[ i ].b);

          //assert(idx[i].a >= 0 && idx[i].a<nSampNum && "find_samp: idx[i].a 越界！");
          //assert(idx[i].b >= 0 && idx[i].b<nSampNum && "find_samp: idx[i].b 越界！");
          switch (fs->dtype) {
          case CPM_CHISQUARE:
            for (j = 0; j < nFeatNum; ++j) {
              CHI_DIST(p[ j ], f[ j ], tmpFeat[ j ]);
            }

            break;

          case CPM_ABSSUB:
            for (j = 0; j < nFeatNum; ++j) {
              ABS_DIST(p[ j ], f[ j ], tmpFeat[ j ]);
            }

            break;

          default:
            break;
          }

#endif

        }
        else {
          double sa, sb;
#if 1

          int isneedfree = 0;
          FEATTYPE* feata = NULL;
          FEATTYPE* featb = NULL;
          feata = fc_get(fileblockrow, idx[ i ].a, 0);

          if (0 == fileblockrow->mp[ idx[ i ].a ] ->vcnt) {
            GETFEATURE(fs->allimgdata + idx[ i ].a * STDW * STDH, feata);
            fileblockrow->mp[ idx[ i ].a ] ->vcnt = 0;
          }

          fileblockrow->mp[ idx[ i ].a ] ->vcnt += 1;
          featb = fc_get(fileblockrow, idx[ i ].b, 0);

          if (0 == fileblockrow->mp[ idx[ i ].b ] ->vcnt) {
            GETFEATURE(fs->allimgdata + idx[ i ].b * STDW * STDH, featb);
            fileblockrow->mp[ idx[ i ].b ] ->vcnt = 0;
          }

          fileblockrow->mp[ idx[ i ].b ] ->vcnt += 1;
          assert(feata != featb);
#else
          int isneedfree = 1;
          FEATTYPE* feata = NULL;
          FEATTYPE* featb = NULL;
          feata = MALLOC(FEATTYPE, nFeatNum);
          featb = MALLOC(FEATTYPE, nFeatNum);
          GETFEATURE(fs->allimgdata + idx[ i ].a * STDW * STDH, feata);
          GETFEATURE(fs->allimgdata + idx[ i ].b * STDW * STDH, featb);
#endif

          //assert(idx[i].a >= 0 && idx[i].a<nSampNum && "find_samp: idx[i].a 越界！");
          //assert(idx[i].b >= 0 && idx[i].b<nSampNum && "find_samp: idx[i].b 越界！");
          switch (fs->dtype) {
          case CPM_CHISQUARE:
            for (j = 0; j < nFeatNum; ++j) {
              sa = feata[ j ];
              sb = featb[ j ];
              CHI_DIST(sa, sb, tmpFeat[ j ]);
            }

            break;

          case CPM_ABSSUB:
            for (j = 0; j < nFeatNum; ++j) {
              sa = feata[ j ];
              sb = featb[ j ];
              ABS_DIST(sa, sb, tmpFeat[ j ]);
            }

            break;

          default:
            break;
          }

          if (isneedfree) {
            FREE(feata);
            FREE(featb);
          }
        }

        // 如果通过才是样本
        {
          score = 0.f;
          ispass = 0;
          //CASCADE_PASS(hh->ca, tmpFeat, hh->ca->id, FEAT_VAL_ID, 0, ispass, score);
          cascade_pass(hh->ca, tmpFeat, 0, 0, &ispass, &score);
        }
      }

      if (cnt == Num) {
        int aaa = 0;
        //system("pause");
        //getch();
        //printf("\n");
      }

      ++cnt;
      ++finded;

      if (ispass) {
        //LOGPRINTF(hh->log, "%d号%s样本通过。\n", i, ff[(idx[i].id+1)&3]);
        //fs->samp_idx[k] = idx[i].id;
        samp_idx[ k ] = i;
        wi[ k ] = 1.f;
        vi[ k ] = score;
        ++k;
        ++findok;
      }

      //if (fs->idxflag[ i ]<0)
      LOSS(fs->idxflag[ i ]);    // 去掉已经找过的负样本，不去正样本
      //LOGPRINTF(log, "%d号样本没通过。\n", i);

      if (-1 == id) {
        //#define INTXT "F:\\public\\bin\\adaboost\\可见光人脸识别训练2\\1000个近红外人脸库备份.txt"
        if (ispass) {
          //get_i_line(INTXT, idx[i].a);
          //get_i_line(INTXT, idx[i].b);
        }
      }

      ++i;
      //if ( k > 0 && 0 == k % ( MAX( Num / 10000, 1 ) ) )
      //if (0==i%97)
      {
        printf("%s样本找了%6d个，找到%6d个，推广率%3.1f![%3.1f%%],耗时%d秒\r",
            id > 0 ? "正" : "负", finded, findok, (cnt) * 1. / k, (k * 100.) / Num, (int) utime_elapsed(time));
      }
    }
  }

  fs->idx_beg = i;
  {
    double rate;
    int last;

    for (last = 0, i = 0; i < idxlen; ++i) {
      last += (SAMPPAIRID(fs->idxflag[ i ]) == id);
    }

    rate = (finded) * 1. / findok;
    logprintf("%s样本找了%d个，找到%d个，推广率%3.1f，还剩%d个没找!耗时%d秒\n",
        id > 0 ? "正" : "负", finded, findok, rate, last, (int) utime_elapsed(time));
  }
  FREE(tmpFeat);
#ifdef USEFILEBLOCK

  fc_close(fileblockrow);
#endif

  return 0;
}
int CHI_make_allimgdata_file(CHI_FEAT_T* fs)
{
  char* listfile = inigetstr("图片txt文件");

  if (fileok(listfile)) {
    int* img_id;
    int lodaed;
    FEATTYPE* tmp = MALLOC(FEATTYPE, fs->feat_num);
    fs->allimgdata = MALLOC(uchar, fs->img_num * STDH * STDW);
    img_id = MALLOC(int, fs->img_num);
    lodaed = loadallimg(listfile, fs->img_num, STDH, STDW, fs->allimgdata, img_id);
    //assert( lodaed == fs->img_num );
    fs->img_num = lodaed;
    chi_make_samppair(fs, img_id);
    FREE(img_id);
    savedata(ALLIMGDATA_FILE, fs->allimgdata, fs->img_num * STDH * STDW, 0);
    FREE(tmp);
  }

  return 0;
}

#define CHI_feat CHI_featFileCol
int CHI_init(BOOST* hh)
{
  CHI_FEAT_T* fs = 0;

  if (1 || !fileok(CHI_FEAT_FILE)) {
    int man_num, img_man;
    fs = MALLOCSET(CHI_FEAT_T, 1, 0);
    man_num = inigetint("总人数");
    img_man = inigetint("每个人的图片数目");
    fs->img_num = img_man * man_num;
    fs->feat_num = GETFEATURE(NULL, NULL);
    logprintf("feat_num=%d\n", fs->feat_num);
    fs->dtype = inigetint("距离类型");
    fs->idx_beg = 0;
    fs->samp_idx = MALLOCSET(int, hh->samp_num, 0);
    //fs = (CHI_FEAT_T*)(hh->h_feat);
    CHI_make_allimgdata_file(fs);
    fs->samp = MALLOC(FEATTYPE, fs->feat_num * fs->img_num);
    assert(fs->samp && "内存分配失败");
    savedata(CHI_FEAT_FILE, fs, sizeof(CHI_FEAT_T),
        fs->idx, fs->idxlen * sizeof(SAMPPAIR),
        fs->idxflag, fs->idxlen * sizeof(char),
        fs->samp_idx, hh->samp_num * sizeof(int), 0);
    //savedata(CHI_FEAT_IDXFLAG_FILE, fs->idxflag, fs->idxlen*sizeof(char), 0);
  }
  else {
    FILE* pf = fopen(CHI_FEAT_FILE, "rb");

    //loaddata_1(CHI_FEAT_FILE, &ff, sizeof(ff));
    if (pf) {
      loaddata(pf, &fs, sizeof(CHI_FEAT_T), 0);
      fs->allimgdata = 0, fs->samp = 0;
      fs->fileblockcol = 0, fs->featfilecol = 0;
      fs->samp_idx = 0, fs->idx = 0, fs->idxflag = 0;
      loaddata(pf,
          &fs->idx, fs->idxlen * sizeof(SAMPPAIR),
          &fs->idxflag, fs->idxlen * sizeof(char),
          &fs->samp_idx, hh->samp_num * sizeof(int), 0);
      fclose(pf);

      if (fileok(CHI_FEAT_IDXFLAG_FILE)) {
        //loaddata_1(CHI_FEAT_IDXFLAG_FILE, fs->idxflag, fs->idxlen*sizeof(char));
      }
    }
  }

  if (!fileok(ALLIMGDATA_FILE)) {
    CHI_make_allimgdata_file(fs);
  }
  else {
    loaddatafn(ALLIMGDATA_FILE, &fs->allimgdata, fs->img_num * STDH * STDW, 0);
  }

  if (!fileok(FEATFILECOL)) {
    printf("%70s\r", "");
    calc_feat(fs, fs->allimgdata, 1);
    printf("保存特征...%70s\r", "");
    savedata(FEATFILECOL, fs->samp, fs->feat_num * fs->img_num * sizeof(FEATTYPE), 0);
  }
  else {
    loaddatafn(FEATFILECOL, &fs->samp, sizeof(FEATTYPE) * fs->img_num * fs->feat_num, 0);
  }

  fs->featfilecol = FOPEN(FEATFILECOL, FILE_READ);
  fs->fileblockcol = fc_open(0, fs->feat_num, fs->img_num * sizeof(FEATTYPE));

  if (fs) {
    hh->feat_num = fs->feat_num;
  }

  hh->h_feat = fs;
  return 1;
}
