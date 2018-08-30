
#include "cstd.h"
#include "str.h"
#include "cfile.h"
#include "img/color.inl"

typedef struct manpic_t {
  int index;
  int cid;
  int pid;
} manpic_t;
typedef struct man_t {
  char name[64];
  int len;
  int cnt;
}man_t;

int getname1(const char* path, char* buf, int len) {
  int i, l2;
  const char* p = path;
  const char* p2;
  l2 = strlen(path);
  if (p2 = strrchr(path, '\\')) {
    l2 = (p2)-path;
  }
  l2 = MAX(l2, 0);
  l2 = MIN(len, l2);
  p = path;
  for (i=0; i<len && (p-path)<l2; ) {
    int ch = *p++;
    if (('0'<=ch && ch<='9') || '_'==ch) {
      buf[i++] = ch;
    } else if ('\\'==ch) {
      buf[i++] = '_';
    }
  }
  buf[i]=0;
  return 0;
}

int man_add(man_t* man, int n, const char* p, manpic_t* mp, trie_t* tr) {
  char p1[256];
  int i, l1;
  getname1(p, p1, 256);
  l1 = strlen(p1);
  if (tr) {
    trie_node_t* pnode = trie_insert(tr, p1, l1);
    n = tr->count;
    i = pnode->id;
    if (pnode->endcnt==1) {
      strncpy(man[i].name, p1, l1);
    }
    if (pnode->endcnt>20) {
      int asdf=0;
    }
    man[i].cnt = pnode->endcnt;
    if (mp) {
      mp->pid = pnode->cnt-1;
      mp->cid = i;
    }
  } else {
    for (i=0; i<n; ++i) {
      if (0==cstr_icmp(man[i].name, man[i].len, p1, l1, 1)) {
        if (mp) {
          mp->pid = man[i].cnt;
          mp->cid = i;
        }
        man[i].cnt++;
        return n;
      }
    }
    strncpy(man[n].name, p1, l1);
    man[n].len = l1;
    man[n].cnt = 1;
    if (mp) {
      mp->pid = man[i].cnt;
      mp->cid = 0;
    }
    ++n;
  }
  return n;
}

#include "ui/window.inl"
#include "img/imgio.inl"
#include "codec/huff.inl"
//#include "face/face.inl"
#include "face/test_face_recog.inl"

int test_image_dat() {
  const char* fn;
  buf_t bf[1] = {0};
  img_t im[1] = {0};
  img_t im1[10] = {0};
  int h = 120, w = 320;
  fn = "C:\\aaa.dat";
  buf_load(fn, bf);
  if (bf->len>120*320*4) {
    h = 144, w = 720;
  }
  IMINIT(im, h, w, bf->data, w*4, 4, 1);
  imcolorcvt(im, im1, T_BGRA, T_RGB);
  imshow(im1);cvWaitKey(-1);
  bffree(bf);
  return 0;
}
int test_jieya_algo() {
  vstr_t sv[1] = {0};
  int i, len;
  buf_t bf[1] = {0};
  buf_t bf2[1] = {0};
  char buf[256];
  img_t imt[10] = {0};
  img_t* img = imt+1;
  img_t* imstd = imt+2;
  img_t* imtmp = imt+3;
  int haveout=0;
#ifdef _DEBUG
  _chdir("E:/pub/bin/face/采集/3代机非抗光");
  _chdir("E:/pub/bin/face/采集/2代机");
  _chdir("E:/pub/bin/face/12");
  _chdir("E:/pub/bin/face/bin");
  _chdir("E:/pub/bin/face/bin/20151123-20151126二幼总");
  _chdir("G:/facedata/bmp");
  _chdir("G:/facedata/algo");
  _chdir("G:/facedata/dat");
  _chdir("G:/facedata/algo/20160921183143111");
#endif
  vstr_load("list.txt", sv);
  if (0) {
    trie_t tr[1] = {0};
    int n = 0;
    manpic_t* id=0;
    man_t* man=0;
    id = MALLOC(manpic_t, sv->n);
    MEMSET(id, 0, sv->n);
    trie_set(tr, 0);
    man = MALLOC(man_t, sv->n);
    for (i=0; i<sv->n; ++i) {
      n = man_add(man, n, sv->v[i].s, id+i, tr);
      id[i].index = i;
    }
#define IDLESS(A, B)   (((A).cid*100+(A).pid)<((B).cid*100+(B).pid))
    QSORT(id, id+sv->n, IDLESS, manpic_t);
#undef IDLESS
    trie_free(tr);
    logprintf("共 %d 人\n", n);
    if (0) {
      for (i=0; i<n; ++i) {
        logprintf("%05d %d %s\n", i, man[i].cnt, man[i].name);
      }
    }
    if (1) {
      for (i=0; i<n; ++i) {
        if (man[i].cnt<10) {
          logprintf("%05d %d %s\n", i, man[i].cnt, man[i].name);
        }
      }
    }
    if (0) {
      FILE* pf = fopen("out.txt", "wb");
      for (i=0; i<sv->n; ++i) {
        const char* str = sv->v[id[i].index].s;
        int cid = id[i].cid;
        fprintf(pf, "%d %d %05d_%s %s\n", i, man[cid].cnt, cid, man[cid].name, str);
      }
      fclose(pf);
    }
    FREE(man);
    free(id);
  }
  bfsetsize(bf2, 1000*1000*10);
  sys_mkdir("out");
  haveout = file_exist("out");
  for (i=0; i<sv->n; ++i) {
    const char* str = sv->v[i].s;
    //int cid = id[i].cid;
    _snprintf(buf, 256, "%s.bmp", str);
    //_snprintf(buf3, 256, "%05d_%s", cid, man[cid].name);
    // _mkdir();
    //_snprintf(buf2, 256, "..\\std\\%05d_%s.bmp", cid, man[cid].name);
    if (1) {
      int isbmp = cstr_endwith(str, -1, ".bmp", -1, 1);
      if (isbmp) {
        imread(str, 1, 1, img);
      } else {
        buf_load(str, bf);
        len = HUFF_Decode(bf->data, bf->len, bf2->data, bf2->len);
        if (len>0) {
          img_t im[1] = {0};
          int h = 576;
          int w = 720;
          const char* p1 = strrchr(buf, '\\');
          IMINIT(im, h, w, bf2->data, w, 1, 1);
          im_trans(im, imt, ISROT3);
          //imshow(imt); cvWaitKey(-1);
          im2gry(imt, img);
        } else {
          continue;
        }
      }
      if (1) {
        int save_pack_std=0;
        int save_std=!file_exist(buf);
        int len = strchr(str, '.') - str;
        memcpy(buf, str, len);
        buf[len]=0;
        strcat(buf, ".std.bmp");
        //imshow(imt); cvWaitKey(-1);
        printf("%d %d %s\n", i, sv->n, strrchr(str, '\\')+1);
        if (0 && (save_pack_std || save_std)) {
          int xywh[12] = {0};
          int have_face = 0;
          imsetsize(imtmp, 100, 100, 1, 1);
          //imshow(img); cvWaitKey(-1);
          have_face = face_recog(img->h, img->w, img->tt.data, img->s, img->c, "gray", "reg", 0, 0.15, xywh, imtmp->tt.data, 0, 0, 0, 0, 0, 0);
          if (have_face) {
            if (save_pack_std) {
              imcatv1(imstd, imtmp);
            }
            //imshow(imtmp);cvWaitKey(-1);
            if (1) {
              //_snprintf(buf, 256, "%s-std.bmp", str);
              //printf("%s\n", strrchr(buf, '\\')+1);
              imwrite(buf, imtmp);
            }
          }
        }
        //imwrite(buf, imt);
        if (haveout && !isbmp) {
          //_snprintf(buf, 256, ".\\out\\%05d_%02d.bmp", cid, id[i].pid);
          _snprintf(buf, 256, "%s.bmp", str);
          imwrite(buf, img);
          //remove(str);
        }
      }
    } else {
      int asdf0=0;
    }
  }
  imfrees(imt, 10);
  bffree(bf);
  bffree(bf2);
  strv_free(sv);
  return 0;
}

int test_log2htm() {
  vstr_t sv[1] = {0};
  vstr_t sv2[1] = {0};
  trie_t tr[1] = {0};
  int i, j;
  FILE* pf = NULL;
  _chdir("G:/facedata/bmp");
  vstr_load("log10.txt", sv);
  vstr_load("list_std.txt", sv2);
  pf = fopen("ttt.htm", "wb");
  fprintf(pf, "<table>\n");
  for (i=0; i<sv2->n; ++i) {
    trie_insert(tr, sv2->v[i].s, sv2->v[i].l);
  }
  for (i=0; i<sv->n; ++i) {
    str_t s, s2;
    if (cstr_ncmp("err:", 4, sv->v[i].s, -1, 4)) {
      continue;
    }
  fprintf(pf, "<tr>\n");
    s = sv->v[i];
    s2 = STRsplit(s, " ", " ", &s);
    s2 = STRsplit(s, " ", " ", &s);
    s2 = STRsplit(s, " ", " ", &s);
    fprintf(pf, "<td>\n");
    printf("%d\n", i);
    j = strv_find_instr(sv2, s2, 0, 1);
    if (j>=0) {
      fprintf(pf, "<img src=\"file://%s\">\n", sv2->v[j].s);
    }
    fprintf(pf, "</td>\n");
  }
  fprintf(pf, "</table>\n");
  strv_free(sv);
  strv_free(sv2);
  return 0;
}

// his0 类内距离直方图
// his1 类间距离直方图
// FAR，类间当作类内
// FAR，类内当作类间
double far2frr(int n, const uint* his0, const uint* his1, double farv, double* pout_i) {
  int i;
  uint64 cnt=0, ifar, cnt0=0, cnt1=0, allcnt0=0, allcnt1=0;
  double frrv;
  for (i=0; i<n; ++i) {
    allcnt0 += his0[i];
    allcnt1 += his1[i];
    cnt += his0[i] + his1[i];
  }
  ifar = (int)(farv*allcnt1);
  for (i=0; i<n; ++i) {
    cnt0 += his0[i];
    cnt1 += his1[i];
    if (cnt1>ifar) {
      break;
    }
  }
  frrv = (allcnt0-cnt0)*1./allcnt0;
  if (pout_i) {
    *pout_i = i;
  }
  return frrv;
}

int next_quality_factor(int x) {
  return 0;
}

#include "ui/window.inl"
int test_kepa() {
  dirlist_t dir[1] = {0};
  buf_t bf[1] = {0};
  int i;
  char buf[256];
  dirlist(dir, "E:/pub/bin/face/kepa/img", "*.gray", FF_SUBDIR);
    cvNamedWindow("im", 0);
  for (i=0; i<dir->n; ++i) {
    img_t im[1] = {0};
    int h = 320, w = 240;
    buf_load(dir->v[i].name, bf);
    IMINIT(im, h, w, bf->data, w, 1, 1);
    //imshow(im);
    //cvWaitKey(-1);
    if (1) {
      _snprintf(buf, 256, "%s.bmp", dir->v[i].name);
      imwrite(buf, im);
    }
  }
  dirlist_free(dir);
  bffree(bf);
  return 0;
}
#include "math/popcount.inl"
//
int test_face_recog_rate_algo() {
  int i, j, k=0;
  vstr_t sv[10] = {0};
  img_t im[10] = {0};
  SPFeatInfo* featlib=0;
  const char* feat_fn = "feat.dat";
  char buf[256] = {0};
  //int nfeat = CPM_GetFeature1(100, 100, 0, 100, 1, PPLBPXBIN, 0);
  //return test_log2htm();
  
#ifdef _DEBUG
  _chdir("G:/facedata/bmp");
  _chdir("E:/pub/bin/face/ORL/ORL92112");
#endif
  vstr_load("list_std.txt", sv);
  //featlib = MALLOC(SPFeatInfo, maxfeat);
  if (1) {
    int n;
    trie_t tr[1] = {0};
    int* cid = MALLOC(int, sv->n);
    int* pid = MALLOC(int, sv->n);
    vstr_t* sv2 = sv+1;
    int nclass = 0;
    strv_setsize(sv2, sv->n);
    for (i=0; i<sv->n; ++i) {
      trie_node_t* pnode;
      getname1(sv->v[i].s, buf, 256);
      pnode = trie_insert(tr, buf, -1);
      cid[i] = pnode->id;
      pid[i] = pnode->endcnt - 1;
      strv_set_str(sv2, pnode->id, STR1(buf));
    }
    nclass = tr->count;
    if (0) {
      int* cnt = MALLOC(int, tr->count);
      MEMSET(cnt, 0, tr->count);
      for (i=0; i<sv->n; ++i) {
        cnt[cid[i]]++;
      }
      for (i=0; i<tr->count; ++i) {
        if (cnt[i]<10) {
          printf("%d %d %s\n", i, cnt[i], sv2->v[i].s);
        }
      }
      FREE(cnt);
    }
    if (0) {
      buf_t imtmp_bf[1] = {0};
      int* cnt = MALLOC(int, tr->count);
      MEMSET(cnt, 0, tr->count);
      _mkdir("..//std");
      _mkdir("..//std//train");
      _mkdir("..//std//test");
      if (1) {
        for (i=0; i<tr->count; ++i) {
          _snprintf(buf, 256, "..//std//train/%05d", i);
          _mkdir(buf);
          _snprintf(buf, 256, "..//std//test/%05d", i);
          _mkdir(buf);
        }
      }
      for (i=0; i<sv->n; ++i) {
        buf_load(sv->v[i].s, imtmp_bf);
        _snprintf(buf, 256, "..//std//%s//%05d//%04d.bmp", pid[i]<10 ? "train":"test", cid[i], cnt[cid[i]]);
        cnt[cid[i]]++;
        buf_save(buf, imtmp_bf);
        printf("%s\n", GetFileNameExt(sv->v[i].s));
      }
      FREE(cnt);
      bffree(imtmp_bf);
    }
    if (0) {
      img_t* imstd = MALLOC(img_t, nclass);
      img_t imtmp[1] = {0};
      MEMSET(imstd, 0, nclass);
      for (i=0; i<sv->n; ++i) {
        imread(sv->v[i].s, 1, 1, imtmp);
        printf("%s\n", GetFileNameExt(sv->v[i].s));
        imcatv1(imstd+cid[i], imtmp);
      }
      for (i=0; i<tr->count; ++i) {
        _snprintf(buf, 256, "..//std//05%d.bmp", i);
        imwrite(buf, imstd+i);
      }
      imfree(imtmp);
      imfrees(imstd, tr->count);
      FREE(imstd);
    }
    trie_free(tr);
    if (!file_exist(feat_fn)) {
      FILE* pf;
      int maxfeat = 1000000;
      SPFeatInfo feat[1] = {0};
      //sv->n = MIN(20, sv->n);
      pf = fopen(feat_fn, "wb");
      for (i=0; i<sv->n && k<maxfeat; ++i) {
        const char* ps = strrchr(sv->v[i].s, '\\') + 1;
        int id = atoi(ps);
        int nk = k, pid=0, dis=0;
        double ddis=0;
        imload_imagelist1(sv->v[i].s, 1, im, 0, 0, 0);
        if (i>2) {
          int asdf=0;
        }
        imresize_1(im, 100, 100, im);
        //imshow(im);cvWaitKey(-1);
        for (j=0; j<im->f && k<maxfeat; ++j) {
          uchar* stdface = im->tt.data+j*im->s*im->h;
          SPFeatInfo* feature = feat;
          face_recog(im->h, im->w, 0, im->s, im->c, "gray", "feat", 0, 0, 0, stdface, feature, NULL, sizeof(SPFeatInfo), nk, &pid, &dis);
          feature->m_Flag = id;
          fwrite(feature, sizeof(SPFeatInfo), 1, pf);
          if (0) {
            ddis = dis/25000.;
            if (pid>0 && ddis<0.47) {
              int jd = featlib[pid].m_Flag;
              const char* ps1 = strrchr(sv->v[jd].s, '\\') + 1;
              logprintf("%5d %05d %5d %05d %5d %7.5f %s %s\n", i, id, pid, jd, dis, ddis, ps, ps1);
            }
          }
          ++k;
        }
        printf("%d %d %s\n", k, sv->n, ps);
      }
      fclose(pf);
    }
    if (1) {
      int m = FEAT_SIZE2;
      buf_t bf[1] = {0};
      const int* mid = g_hashfeat2165_mid;
      n=0;
      if (1) {
        buf_load(feat_fn, bf);
        featlib = (SPFeatInfo*)bf->data;
        n = bf->len/sizeof(SPFeatInfo);
        ASSERT(n==sv->n);
      }
      if (0) {
        uint* hashv = MALLOC(uint, n);
        int err=0, right = 0, cnt=0, cnt2=0, cnt3=0, cnt4=0, k = 8;
        for (i=0; i<n; ++i) {
          int cnt[32] = {0};
          hashv[i] = hashfeat2165((const FEAT_T*)featlib[i].m_Feature);
        }
#if 0
// 测试结果
// 不用hash     识别率 96.23%
// 海明距离<10  识别率 92.42% 性能提升50.32倍
// 海明距离<9   识别率 89.60% 性能提升116.24倍
// 海明距离<8   识别率 85.26% 性能提升298.20倍
#endif
        if (0) {
          int disham;
          uint* repid = MALLOC(uint, nclass);
          MEMSET(repid, 0, nclass);
          for (i=0; i<n; ++i) {
            if (repid[cid[i]]) {
              continue;
            }
            printf("%d %d\r", i, n);
            for (j=i+1; j<n; ++j) {
              if (cid[i]!=cid[j]) {
                disham = popcnt_lookup2(hashv[i]^hashv[j]);
                if (disham<10) {
                  FEAT_T* a = (FEAT_T*)featlib[i].m_Feature;
                  FEAT_T* b = (FEAT_T*)featlib[j].m_Feature;
                  int dis = CPM_AbsSub(FEATURE_LEN/2, a, b);
                  if (dis<(int)(25000*0.47)) {
                    repid[cid[i]] = cid[j];
                    if (file_exist(sv->v[j].s)) {
                      logprintf("%f %s %s\n", dis/25000., sv2->v[cid[i]].s, sv2->v[cid[j]].s);
                    }
                    break;
                    //logprintf("err: 原图id=%d %s => 识别id=%d %s\n", cid[i], GetFileNameExt(sv->v[i].s), cid[j], GetFileNameExt(sv->v[j].s));
                  }
                }
              }
            }
          }
          for (i=0; i<tr->count; ++i) {
            if (repid[i]>0) {
              j = repid[i];
              logprintf("%s %s\n", sv2->v[i].s, sv2->v[j].s);
            }
          }
          FREE(repid);
        }
        for (i=0; i<n; ++i) {
          if (pid[i]>=k) {
            ++cnt;
            for (j=0; j<n; ++j) {
              if (pid[j]<k) {
                ++cnt2;
              }
            }
            if (0==i%10) {
              printf("进度%4.2f%%(%d/%d) 识别率%4.2f%% 误识个数%d 总个数%d 提升%4.2f倍 %d\r", i*100./n, i, n, right*100./cnt, err, cnt, cnt2*1./cnt3, cnt4);
            }
          }
        }
        printf("%d个测试样本 用前%d张做训练\n", cnt, k);
        for (i=0; i<n; ++i) {
          if (pid[i]>=k) {
            int mindis = 100000000, minid=-1;
            ++cnt;
            for (j=0; j<n; ++j) {
              if (pid[j]<k) {
                int disham = popcnt_lookup2(hashv[i]^hashv[j]);
                ++cnt2;
                if (disham<10) {
                  FEAT_T* a = (FEAT_T*)featlib[i].m_Feature;
                  FEAT_T* b = (FEAT_T*)featlib[j].m_Feature;
                  int dis = CPM_AbsSub(FEATURE_LEN/2, a, b);
                  ++cnt3;
                  if (dis<mindis) {
                    mindis = dis;
                    minid = j;
                  }
                }
              }
            }
            if (minid>=0) {
              right += cid[i]==cid[minid];
              err += cid[i]!=cid[minid];
              if (cid[i]!=cid[minid]) {
                logprintf("err: 原图id=%d %s => 识别id=%d %s\n", cid[i], GetFileNameExt(sv->v[i].s), cid[minid], GetFileNameExt(sv->v[minid].s));
              }
            } else {
              //printf("失败\n");
              cnt4++;
            }
            if (0==i%10) {
              printf("进度%4.2f%%(%d/%d) 识别率%4.2f%% 误识个数%d 总个数%d 提升%4.2f倍 %d\r", i*100./n, i, n, right*100./cnt, err, cnt, cnt2*1./cnt3, cnt4);
            }
          }
        }
        logprintf("%d个测试样本 用前%d张做训练\n", cnt, k);
        logprintf("识别率 %4.2f%% 误识率 %4.2f%% 性能提升%4.2f倍 失败%d", right*100./cnt, err*100./cnt, cnt2*1./cnt3, cnt4);
        FREE(hashv);
      }
      if (0) {
        int kk = 10000;
        int* hist = MALLOC(int, 32*kk);
        memset(hist, 0, 32*kk);
        for (i=0; i<n; ++i) {
          int cnt[32] = {0};
          FEAT_T* feat = (FEAT_T*)featlib[i].m_Feature;
          for (j=0; j<m; ++j) {
            k = j*32/m;
            cnt[k] += feat[j];
          }
          for (j=0; j<32; ++j) {
            ASSERT(cnt[j]<kk);
            hist[j*kk+cnt[j]]++;
          }
        }
        // 根据频度求中位数
        for (j=0; j<32; ++j) {
          int cc=0;
          int* hist1 = hist+j*kk;
          for (i=0; i<kk; ++i) {
            cc += hist1[i];
            if (cc>=(n/2)) {
              mid[j] = i;
              printf("%d, ", mid[j]);
              if ((j+1)%8==0) {
                printf("\n");
              }
              break;
            }
          }
        }
        FREE(hist);
      }
      if (0) {
        for (i=0; i<n; ++i) {
          FEAT_T* a = (FEAT_T*)featlib[i].m_Feature;
          for (j=0; j<i; ++j) {
            if (cid[i]!=cid[j]) {
              FEAT_T* b = (FEAT_T*)featlib[j].m_Feature;
              int dis = CPM_AbsSub(FEATURE_LEN/2, a, b);
              double ddis = dis/25000.;
              if (ddis<0.47) {
                logprintf("%7.5f %s %s\n", ddis, GetFileNameExt(sv->v[i].s), GetFileNameExt(sv->v[j].s));
              }
            }
          }
          printf("%d %d\n", i, n);
        }
      }
      if (0) {
        double dthd = 0.45;
        int err=0, right = 0, cnt=0, thd = (int)(dthd*25000), k = 5;
        for (i=0; i<n; ++i) {
          if (pid[i]>=k) {
            int mindis = 1000000000, minid=-1;
            ++cnt;
            for (j=0; j<n; ++j) {
              if (pid[j]<k) {
                FEAT_T* a = (FEAT_T*)featlib[i].m_Feature;
                FEAT_T* b = (FEAT_T*)featlib[j].m_Feature;
                int dis = CPM_AbsSub(FEATURE_LEN/2, a, b);
                if (dis<mindis) {
                  mindis = dis;
                  minid = j;
                }
              }
            }
            if (minid>=0 && mindis<thd) {
              right += cid[i]==cid[minid];
              err += cid[i]!=cid[minid];
            }
            if (0==i%10) {
              printf("进度 %4.2f%%(%d/%d) 识别率 %4.2f%% 误识个数 %d 总个数 %d 阈值=%f\r", i*100./n, i, n, right*100./cnt, err, cnt, dthd);
            }
          }
        }
        logprintf("%d个测试样本 用前%d张做训练\n", cnt, k);
        logprintf("识别率 %4.2f%% 误识率 %4.2f%% 阈值=%f\n", right*100./cnt, err*100./cnt, dthd);
      }
      if (1) {
        int err=0, right = 0, cnt=0, k = 5;
        for (i=0; i<n; ++i) {
          if (pid[i]>=k) {
            int mindis = 1000000000, minid=-1;
            ++cnt;
            for (j=0; j<n; ++j) {
              if (pid[j]<k) {
                FEAT_T* a = (FEAT_T*)featlib[i].m_Feature;
                FEAT_T* b = (FEAT_T*)featlib[j].m_Feature;
                int dis = CPM_AbsSub(FEATURE_LEN/2, a, b);
                if (dis<mindis) {
                  mindis = dis;
                  minid = j;
                }
              }
            }
            if (minid>=0) {
              right += cid[i]==cid[minid];
              err += cid[i]!=cid[minid];
              if (cid[i]!=cid[minid]) {
                logprintf("err: 原图id=%d %s => 识别id=%d %s\n", cid[i], GetFileNameExt(sv->v[i].s), cid[minid], GetFileNameExt(sv->v[minid].s));
              }
            }
            if (0==i%10) {
              printf("进度 %4.2f%%(%d/%d) 识别率 %4.2f%% 误识个数 %d 总个数 %d\r", i*100./n, i, n, right*100./cnt, err, cnt);
            }
          }
        }
        logprintf("%d个测试样本 用前%d张做训练\n", cnt, k);
        logprintf("识别率 %4.2f%% 误识率 %4.2f%%", right*100./cnt, err*100./cnt);
      }
      if (0) {
        enum {dd=1000, ttmax = 25000};
        uint dis_hist[2][dd] = {0};
        int64 k=0, nn;
        //savefile("dishist.dat", dis_hist, sizeof(dis_hist));
        //n = MIN(10000, n);
        nn = (int64)n*(n-1)/2;
        if (0) {
          for (i=0; i<n; ++i) {
            for (j=0; j<i; ++j) {
              FEAT_T* a = (FEAT_T*)featlib[i].m_Feature;
              FEAT_T* b = (FEAT_T*)featlib[j].m_Feature;
              int dis = CPM_AbsSub(FEATURE_LEN/2, a, b);
              dis = MIN(dis, 25000);
              dis = dis*dd/25001;
              dis_hist[cid[j]==cid[i]][dis]++;
              ++k;
              if (0==k%10000) {
                double ee = 0.00001, frr0, frr1;
                frr0 = far2frr(dd, dis_hist[1], dis_hist[0], 0, 0);
                frr1 = far2frr(dd, dis_hist[1], dis_hist[0], ee, 0);
                printf("进度 %4.2f%%(%I64d/%I64d) frr(0%%)=%5.3f%% frr(%5.3f%%)=%5.3f%%\r", k*100./nn, k, nn, frr0*100, ee*100, frr1*100);
              }
              if (0==k%10000000) {
                savefile("dishist.dat", dis_hist, sizeof(dis_hist));
              }
            }
          }
          savefile("dishist.dat", dis_hist, sizeof(dis_hist));
        } else {
          loadfile("dishist.dat", dis_hist, sizeof(dis_hist), 0);
        }
        {
          int ii;
          double ee[] = {0.001, 0.0001, 0.00001, 0.000001, 0.0000001, 0}, thd;
          for (ii=0; ii<countof(ee); ++ii) {
            double frr = far2frr(dd, dis_hist[1], dis_hist[0], ee[ii], &thd);
            logprintf("当FAR=%11.9f%% 时 frr=%f%% thd=%5.3f\n", ee[ii]*100, frr*100, thd/dd);
          }
          logprintf("FAR=类间当作类内 FRR=类内当作类间\n");
        }
      }
      bffree(bf);
    }
    free(cid);
    free(pid);
  }
  strv_frees(sv, 10);
  imfrees(im, 10);
  return 0;
}

int test_fanhao() {
  vstr_t sv[1] = {0};
  trie_t tr[1] = {0};
  int i;
  _chdir("F:/MV/xxx");
  vstr_load("list.txt", sv);
  for (i=0; i<sv->n; ++i) {
  }
  {
    int* cid = MALLOC(int, sv->n);
    int* pid = MALLOC(int, sv->n);
    int* cnt;
    int* cntsort;
    vstr_t sv2[1] = {0};
    for (i=0; i<sv->n; ++i) {
      trie_node_t* pnode;
      char* name = GetFileNameExt(sv->v[i].s);
      pnode = trie_insert(tr, name, 4);
      cid[i] = pnode->id;
      pid[i] = pnode->endcnt - 1;
    }
    cnt = MALLOC(int, tr->count);
    cntsort = MALLOC(int, tr->count);
    MEMSET(cnt, 0, tr->count);
    strv_setsize(sv2, tr->count);
    for (i=0; i<sv->n; ++i) {
      cnt[cid[i]]++;
      if (pid[i]==0) {
        char* name = GetFileNameExt(sv->v[i].s);
        strv_set_str(sv2, cid[i], STR2(name, 4));
      }
    }
    for (i=0; i<tr->count; ++i) {
      cntsort[i] = i;
    }
#define LESSSS(a, b)   cnt[a]>cnt[b]
    QSORT(cntsort, cntsort+tr->count, LESSSS, int);
#undef LESSSS
    for (i=0; i<10; ++i) {
      int j = cntsort[i];
      printf("%s %d\n", sv2->v[j].s, cnt[j]);
    }
    trie_free(tr);
    free(cntsort);
    free(cid);
    free(pid);
    free(cnt);
  }
  strv_free(sv);
  return 0;
}

CC_INLINE uint32 next_pow_of_2(uint32 x) {
  x -= 1;
  x |= x>>1;
  x |= x>>2;
  x |= x>>4;
  x |= x>>8;
  x |= x>>16;
  return x+1;
}