
#include "cfile.h"
#include "face/face_recog.inl"

enum {maxfeatnum = 20};
typedef struct face_recog_mojin_t {
  SPFeatInfo feat[maxfeatnum];
  SPFeatInfo* featlib;
  int featnum;
  int mennum;
  uchar stdface[10000];
} face_recog_mojin_t;

face_recog_mojin_t mojin[1] = {0};

int face_recog_mojin_load() {
  int i;
  sys_mkdir("feat");
  dirlist_t dl[1] = {0};
  dirlist(dl, "./feat/", "*.dat", FF_FULLNAME);
  for (i=0; i<dl->n; ++i) {
    int sz = loadfile(dl->v[i].name, mojin->feat, sizeof(mojin->feat), 0);
    int nfeat = sz/sizeof(SPFeatInfo);
    mojin->featnum+=nfeat;
    MYREALLOC(mojin->featlib, mojin->featnum);
    MEMCPY(mojin->featlib+mojin->featnum-nfeat, mojin->feat, nfeat);
    mojin->mennum++;
  }
  dirlist_free(dl);
  return 0;
}

int get_max_count(const int* ids, int nids, int* pid, int* pcnt) {
  int i, j = -1;
  if (nids>0) {
    int* menids = (int*)memdup(ids, nids*sizeof(int));
    int* cnt = MALLOC(int, nids);
    for (i=0; i<nids; ++i) {
      menids[i] = ids[i];
      cnt[i] = 0;
    }
    QSORT(menids, menids+nids, LESS, int);
    for (i=1; i<nids; ++i) {
      if (menids[i]==menids[i-1]) {
        cnt[i] = cnt[i-1]+1;
      } else {
        cnt[i] = 0;
      }
    }
    j=0;
    for (i=1; i<nids; ++i) {
      if (cnt[i]>cnt[j]) {
        j = i;
      }
    }
    if (pid) {
      *pid = menids[j];
    }
    if (pcnt) {
      *pcnt = cnt[j]+1;
    }
    FREE(cnt);
    FREE(menids);
  }
  return j;
}

int face_recog_mojin(const img_t* im, int n, int* id, int* isnew) {
  int ids[maxfeatnum];
  int ids2[maxfeatnum];
  int dis2[maxfeatnum];
  n = MIN(maxfeatnum, n);
  static int inited=0;
  if (!inited) {
    inited=1;
    face_recog_mojin_load();
  }
  int nids=0;
  int nfeat=0;
  int i, j;
  int xywh[12];
  int rets[12];
  int ret = FACE_RECOG_FALG_OK;
  int thd = 550;
  for (i=0; i<n; ++i) {
    int id=-1, dis=99999;
    const img_t* im1 = im+i;
    int ret1 = face_recog(im1->h, im1->w, im1->tt.data, im1->s, im1->c, "bgr", "dfr", 0, 0.1, xywh, mojin->stdface, mojin->feat[nfeat].m_Feature, mojin->featlib, sizeof(SPFeatInfo), mojin->featnum, &id, &dis);
    rets[i] = ret1;
    if (ret1==FACE_RECOG_FALG_OK) {
      int mid = -1;
      if (id>=0 && dis<thd) {
        mid = mojin->featlib[id].m_Flag;
      }
      mojin->feat[nfeat].m_Flag = mojin->mennum;
      ids2[nfeat] = mid;
      dis2[nfeat] = dis;
      ids[nids++] = mid;
      nfeat++;
    }
  }
  get_max_count(rets, n, &ret, 0);
  if (ret == FACE_RECOG_FALG_OK) {
    int minnfeat = MAX(1, n/2);
    if (nfeat>=minnfeat) {
      int cnt=0;
      int mincnt = MAX(1, n/2);
      j = get_max_count(ids, nids, id, &cnt);
      if (cnt>=minnfeat) {
        if (j<0 || ids[j]==-1) {
          *isnew = 1;
          *id = mojin->mennum;
          mojin->featnum+=nfeat;
          MYREALLOC(mojin->featlib, mojin->featnum);
          MEMCPY(mojin->featlib+mojin->featnum-nfeat, mojin->feat, nfeat);
          char buf[256];
          _snprintf(buf, 256, "feat/%d.dat", mojin->mennum);
          savefile(buf, mojin->feat, nfeat*sizeof(SPFeatInfo));
          ++mojin->mennum;
        } else {
          *isnew = 0;
        }
      } else {
        ret = FACE_RECOG_FALG_FACE_IS_NOT_FRONT;
        printf("cnt=%d mincnt=%d\n", cnt, mincnt);
      }
    } else {
      ret = FACE_RECOG_FALG_FACE_IS_NOT_FRONT;
      printf("nfeat=%d minnfeat=%d\n", nfeat, minnfeat);
    }
  } else if (nfeat>0) {
    // 把人脸张数不够的情况 改为 非正脸。
    ret = FACE_RECOG_FALG_FACE_IS_NOT_FRONT;
  }
  return ret;
}


