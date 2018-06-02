
#include "iris/omkernel.inl"

int test_merge_feat_mask()
{
  FILE* plisttxt;
  int fram, i = 0;
  img_t im[1] = {0};
  img_t im2[1] = {0};
  char buf[256];
  char buf2[256];
  const char* flist;

  _chdir("D:/pub/bin/iris/testPic/feat");
  //_chdir("D:/pub/bin/iris/adaboost");

  flist = "list.txt";
  plisttxt = fopen(flist, "rb");

  _mkdir("ttt");

  //for ( fram = 0; fgets(buf, 256, plisttxt)>0; ++fram )
  for (fram = 0; fgets(buf, 256, plisttxt); ++fram) {
    strtrim(buf, "\n\r");
    imread(buf, 1, 1, im);
    buf[8] = 'M';
    imread(buf, 1, 1, im2);
    printf("%s\n", buf);
    {
      int i;

      for (i = 0; i < 360 * 30; ++i) {
        im->tt.data[i] = (im2->tt.data[i] ? im->tt.data[i] : 0);
      }

      //imshow(im); cvWaitKey(-1);
      _snprintf(buf2, 256, "./ttt/%s", buf);
      imwrite4(buf2, IRIS_H, IRIS_W, im->tt.data, IRIS_W, 1);
    }
  }

  imfree(im);
  fclose(plisttxt);
  return 0;
}


uchar g_ft[64 * IRIS_H* IRIS_W];
int test_omkernel()
{
  img_t im[1] = {0};
  img_t im2[1] = {0};
  int i;
  const char* fname;
  _chdir("D:/pub/bin/iris/adaboost");
  fname = "S2001L01.jpg";
  _chdir("D:/pub/bin/iris/omkernel");
  fname = "1.bmp";
  imread(fname, 1, 1, im);
  {
    utime_start(_start_time);

    for (i = 0; i < 1; ++i) {
      //imfilter_guass(im, im2, 0.8);
      makeom(im->tt.data, g_ft);
    }

    printf("%f\n", utime_elapsed(_start_time));
  }
  imshow(im);
  imshow(im2);
  cvWaitKey(-1);
  imfree(im);
  imfree(im2);
  return 0;
}

int test_omkernel_list()
{
  FILE* plisttxt;
  int fram, i = 0;
  const char* outdir = "out_270";
  img_t im[1] = {0};
  char buf[256];
  char buf2[256];
  const char* flist;

  _chdir("D:/pub/bin/iris/testPic/feat2");
  //_chdir("D:/pub/bin/iris/adaboost");

  flist = "list.txt";
  plisttxt = fopen(flist, "rb");

  _mkdir("ttt");

  //for ( fram = 0; fgets(buf, 256, plisttxt)>0; ++fram )
  for (fram = 0; fgets(buf, 256, plisttxt); ++fram) {
    strtrim(buf, "\n\r");
    imread(buf, 1, 1, im);
    printf("%s\n", buf);
    makeom(im->tt.data, g_ft);
    {
      //imshow(im); cvWaitKey(-1);
      _snprintf(buf2, 256, "./ttt/%s", buf);
      imwrite4(buf2, IRIS_H, IRIS_W, g_ft, IRIS_W, 1);
    }
  }

  imfree(im);
  fclose(plisttxt);
  return 0;
}

#include "img/imgset.inl"

#if 0
#define getfeat(a, b) bf_makeom_i1(bf, a, b, 0.8, 3)
#else
#define getfeat(a, b) makeom_bit_block_select(bf, a, b)
#endif

int test_iris_recognition_2pic()
{
  enum {N = 2};
  const char* picnamef[N] = {"S2004L04F.bmp", "S2281L08F.bmp" };
  const char* picnamem[N] = {"S2004L04M.bmp", "S2281L08M.bmp" };
  uchar feat[N * IRIS_H * IRIS_S] = {0};
  uchar mask[N * IRIS_H * IRIS_S] = {0};
  int i, j, featlen = IRIS_H * IRIS_S;
  img_t im[1] = {0};
  double dis = 0;
  buf_t bf[1]={0};
  bfsetsize(bf, 2<<24);
  _chdir("D:/pub/bin/iris/testPic/feat");

  for (i = 0; i < N; ++i) {
    imread(picnamef[i], 1, 1, im);
    getfeat(im->tt.data, feat + i * featlen);
    imread(picnamem[i], 1, 1, im);
    im2bit1(IRIS_H * IRIS_W, im->tt.data, mask + i * featlen);
  }

  for (i = 0; i < N; ++i) {
    for (j = i + 1; j < N; ++j) {
      dis = count_xor_mask(featlen, feat + i * featlen, feat + j * featlen, mask + i * featlen, mask + j * featlen);
      {
        bit12im(IRIS_H * IRIS_W, feat + i * featlen, im->tt.data);
        imshow(im);
        cvWaitKey(-1);
      }
      {
        bit12im(IRIS_H * IRIS_W, feat + j * featlen, im->tt.data);
        imshow2("ttt", im);
        cvWaitKey(-1);
      }
    }
  }

  imfree(im);
  bffree(bf);
  return 0;
}

int test_iris_recognition_2pic2()
{
  enum {MAXN = 2};
  uchar* feat = 0;
  int i, j, featlen = 0, n;
  img_t im[1] = {0};
  int dis = 0;
  vstr_t sv[1] = {0};
  vstr_t sv2[1] = {0};
  buf_t bf[1] = {0};
  bfsetsize(bf, 2<<24);
  featlen = getfeat(0, 0);
  _chdir("D:/pub/cstd/DSP/dm6446/ttt2/raw/feat");
  _chdir("D:/pub/cstd/DSP/dm6446/ok/feat");
  
  vstr_load("list.txt", sv);
  n = sv->n;
  feat = MALLOC(uchar, featlen*n);
  for (i = 0; i < n; ++i) {
    imread(sv->v[i].s, 1, 1, im);
    getfeat(im->tt.data, feat + i * featlen);
  }
  
  for (i = 0; i < n; ++i) {
    for (j = i + 1; j < n; ++j) {
      dis = count_xor(featlen, feat + i * featlen, feat + j * featlen);
      if (0) {
        bit12im(IRIS_H * IRIS_W, feat + i * featlen, im->tt.data);
        imshow(im);
        cvWaitKey(-1);
      }
      if (0) {
        bit12im(IRIS_H * IRIS_W, feat + j * featlen, im->tt.data);
        imshow2("ttt", im);
        cvWaitKey(-1);
      }
      
      logprintf("d=%5d i=%3d j=%3d, %d\n", dis, i, j, (int)(!!memcmp(sv->v[i].s, sv->v[j].s, 4)));
    }
  }
  
  imfree(im);
  bffree(bf);
  return 0;
}


// 虹膜 定序特征 识别率
// featlen 5376
// 黑点<3000
// (2322[8994 8995] 23980[595 609])(8044[9010 9372] 29002[2357 7155]
// 参数=  0.8 1 识别率=0.95766, 安全识别率=0.63416
// 黑点<2000
// (2434[2355 2356] 23980[595 609])(9823[1819 2219] 29002[2357 7155]
// 参数=  0.8 1 识别率=0.97332, 安全识别率=0.85636
int test_iris_recognition_rate()
{
  int i, j, f, featlen, correct_cnt = 0, all_cnt = 0, safe_correct_cnt = 0;
  const char* flist;
  imgset_t imsf[1] = {0};
  uchar map[256] = {0};
  uchar* fts = 0;
  uchar* istrain = 0;
  img_t im[1] = {0};
  int ii, jj;
  int* idx = 0;
  uchar* mask = 0;
  double* err = 0;
  vstr_t sv[1] = {0};
  vstr_t sv2[1] = {0};
  int* ids;
  buf_t bf[1] = {0};
  bfsetsize(bf, 2<24);

  _chdir("D:/pub/bin/iris/testPic/V2_normalize");
  _chdir("D:/pub/bin/iris/adaboost_om2/V2_iris");
  _chdir("D:/pub/bin/iris/testPic/feat");
  _chdir("D:/pub/bin/iris/feat");
  map_init(map, "0123456789", 10);
  map_init(map, "LR", 2);
  flist = "list.txt";

  vstr_load(flist, sv);
  imgset_readv(imsf, 1, sv, 2, 4, map, sv2);
  f = imsf->im->f;
  featlen = getfeat(0, 0);
  fts = MALLOC(uchar, f * featlen);
  idx = MALLOC(int, featlen);
  err = MALLOC(double, featlen);
  mask = MALLOC(uchar, f * featlen);
  istrain = MALLOC(uchar, f);
  mask_set(f, imsf->id, f > 4000 ? 5 : 2, istrain);
  printf("featlen %d\n", featlen);
  ids = imsf->id;
  //imshow(imsm->im); cvWaitKey(-1);
  //im2bit1(IRIS_H*IRIS_W*f, imsf->im->tt.data, mask);
  //{img_t im[1]={0}; IMINIT(im, IRIS_H, IRIS_S, mask, IRIS_S, 1, 1), imshow(im); cvWaitKey(-1);}
  {
    int nj_nn[2] = {0};

    for (i = 0; i < f; ++i) {
      for (j = 0; j < f; ++j) {
        nj_nn[ids[i] == ids[j]]++;
      }
    }

    printf("类间%d 类内%d\n", nj_nn[0], nj_nn[1]);
  }

  for (ii = 0; ii < countof(delta1); ++ii) {
    for (jj = 0; jj < countof(disl1); ++jj) {
      double mindis[2] = {INT_MAX, INT_MAX};
      double maxdis[2] = {0};
      int minid[2][2] = { -1, -1, -1, -1};
      int maxid[2][2] = { -1, -1, -1, -1};

      //printf("%g, %d\n", delta1[ii], jj);

      for (i = 0; i < f; ++i) {
        imgetframe(imsf->im, i, im);
        //makeom(im->tt.data, fts + i * featlen);
        //makeom_i1(im->tt.data, fts + i * featlen, delta1[ii], disl1[jj]);
        getfeat(im->tt.data, fts + i * featlen);
        //makeom_bit(im->tt.data, fts + i * featlen);
        //makeom_bit(im->tt.data, fts + i * featlen);
        //makeom_bit_select(im->tt.data, fts + i * featlen);
        //makeom_bit_block_select(im->tt.data, fts + i * featlen);
        //makeom1(im->tt.data, fts + i * featlen);
        printf("计算特征 %4d/%4d\r", i, f);
      }

      if (0) {
        for (i = 0; i < featlen; ++i) {
          idx[i] = i;
        }

        if (1) {
          feat_select_makeom_bit(f, featlen, fts, featlen, imsf->id, err);
#define IDLESS(a, b)  (err[a]<err[b])
          QSORT(idx, idx + featlen, IDLESS, int);
#undef IDLESS
        }
        else {
          RANDOM_SHUFFLE(featlen, idx, int);
        }
      }

      if (1) {
        // 类间最小最大值
        int t, nnok_cnt = 0;
        int nn_cnt = 0;
        double dis;

        for (i = 0; i < f; ++i) {
          for (j = i + 1; j < f; ++j) {
            dis = count_xor(featlen, fts + i * featlen, fts + j * featlen);
            //dis = count_xor_mask(featlen, fts + i * featlen, fts + j * featlen, mask + i * featlen, mask + j * featlen);
            t = ids[i] == ids[j];

            //mindis[t] = MIN(mindis[t], dis);
            //maxdis[t] = MAX(maxdis[t], dis);
            if (dis < mindis[t]) {
              mindis[t] = dis;
              minid[t][0] = i;
              minid[t][1] = j;

              if (!t && dis < 4000) {
                //logprintf("%g*%s*%s\n", dis, sv2->v[i].s, sv2->v[j].s);
              }
            }

            if (dis > maxdis[t]) {
              maxdis[t] = dis;
              maxid[t][0] = i;
              maxid[t][1] = j;
            }
          }

          //                                                            类内最小距离
          printf("%4d (%g[%d %d] %g[%d %d])(%g[%d %d] %g[%d %d])\r", i, mindis[1], minid[1][0], minid[1][1], maxdis[1], maxid[1][0], maxid[1][1], mindis[0], minid[0][0], minid[0][1], maxdis[0], maxid[0][0], maxid[0][1]);
        }

        printf("%5.1f %d (%g[%d %d] %g[%d %d])(%g[%d %d] %g[%d %d]\n", delta1[ii], disl1[jj], mindis[1], minid[1][0], minid[1][1], maxdis[1], maxid[1][0], maxid[1][1], mindis[0], minid[0][0], minid[0][1], maxdis[0], maxid[0][0], maxid[0][1]);
      }

      if (1) {
        int* ids = imsf->id;
        // 类内通过率
        int all_cnt = 0;
        int nn_pass_cnt = 0; //类内通过数

        for (i = 0; i < f; ++i) {
          for (j = 0; j < f; ++j) {
            if (ids[i] == ids[j]) {
              double dis = count_xor(featlen, fts + i * featlen, fts + j * featlen);
              ++all_cnt;

              if (dis < mindis[0]) {
                nn_pass_cnt++;
              }
            }
          }

          printf("%4d/%4d 类内通过率 %.5f\r", i, f, nn_pass_cnt * 1. / all_cnt);
        }

        printf("类内通过率 %.5f                          \n", nn_pass_cnt * 1. / all_cnt);
      }

      if (0) {
        // 识别率和安全识别率
        all_cnt = 0;
        correct_cnt = 0; //正确数
        safe_correct_cnt = 0; //取类间最小值为阀值时的正确数

        for (i = 0; i < f; ++i) {
          if (0 == istrain[i]) {
            double dis, mindis1 = MAX_INT;
            int id = 0;

            for (j = 0; j < f; ++j) {
              if (1 == istrain[j]) { // 1表示训练集
                dis = count_xor(featlen, fts + i * featlen, fts + j * featlen);
                //dis = count_xor_mask(featlen, fts + i * featlen, fts + j * featlen, mask+i*featlen, mask+j*featlen);
                //dis = count_xor_select(200, fa, fb, idx);
                //dis = count_xor_mask(featlen, fa, fb, mask + j*featlen);

                if (dis < mindis1) {
                  mindis1 = dis;
                  id = j;
                }
              }
            }

            if (imsf->id[i] == imsf->id[id]) {
              ++correct_cnt;
            }

            if (mindis1 < mindis[0]) {
              ++safe_correct_cnt;
            }

            ++all_cnt;
          }

          printf("识别 %4d/%4d 识别率%.5f, 安全识别率%.5f\r", i, f, correct_cnt * 1. / all_cnt, safe_correct_cnt * 1. / all_cnt);
        }

        printf("                                                                           \r");
        printf("参数=%5.1f %d 识别率=%.5f, 安全识别率=%.5f\n", delta1[ii], disl1[jj], correct_cnt * 1. / all_cnt, safe_correct_cnt * 1. / all_cnt);
      }
    }
  }

  FREE(fts);
  FREE(mask);
  FREE(idx);
  FREE(err);
  imgset_free(imsf);
  strv_free(sv);
  strv_free(sv2);
  bffree(bf);
  return 0;
}
