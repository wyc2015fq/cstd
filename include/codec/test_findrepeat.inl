// 查找由文件列表(list.txt)指定的重复文件
//可配合如下批处理运行
//@echo off
//dir /b /s>list.txt
//test_findrepeat.exe
//for /f "delims=*" %%a in (replist.txt) do (del "%%a")
#include "cstd.h"
#include "cfile.h"
//#include "findfile.h"
//#include "../test/test_heap.inl"
typedef struct rp_fileinfo {
  int64 fsize;
  char* fname;
  int* hist;
  struct rp_fileinfo* next;
} rp_fileinfo;
#define HASHMAX  10000
typedef struct rp_fileset {
  rp_fileinfo* tab[HASHMAX];
} rp_fileset;
int filecmp(char* fname1, char* fname2, int64 fsize)
{
  int i = 0, ret = 0;
  FILE* pf1, *pf2;
  char buf1[1024];
  char buf2[1024];
  if (0 == strcmp(fname1, fname2)) {
    return -1;
  }
  pf1 = fopen(fname1, "rb");
  if (NULL == pf1) {
    return -1;
  }
  pf2 = fopen(fname2, "rb");
  if (NULL == pf2) {
    return 1;
  }
  for (i = 0; ret == 0 && i < fsize;) {
    int sz1, sz2;
    sz1 = fread(buf1, 1, 1024, pf1);
    sz2 = fread(buf2, 1, 1024, pf2);
    if (sz1 == sz2) {
      ret = memcmp(buf1, buf2, sz1);
      i += sz1;
    }
    else {
      ret = sz1 - sz2;
      break;
    }
  }
  fclose(pf1);
  fclose(pf2);
  return ret;
}

int filehist(const char* fname1, int64 fsize, int* hist)
{
  FILE* pf1;
  int i, j, sz1;
  unsigned char buf1[1024];
  pf1 = fopen(fname1, "rb");
  if (NULL == pf1) {
    return -1;
  }
  for (i = 0; i < 256; ++i) {
    hist[i] = 0;
  }
  for (i = 0; i < fsize;) {
    sz1 = fread(buf1, 1, 1024, pf1);
    i += sz1;
    for (j = 0; j < 1024; ++j) {
      hist[buf1[j]]++;
    }
  }
  fclose(pf1);
  return 0;
}
double filecmpsame(rp_fileinfo* finfo1, const char* fname2, int64 fsize)
{
  int i = 0, j = 0, sum = 0, ret;
  double rete = 0;
  char* fname1 = finfo1->fname;
  int* hist1;
  int hist2[256];
  if (0 == strcmp(fname1, fname2)) {
    return -1;
  }
  if (NULL == finfo1->hist) {
    finfo1->hist = (int*)malloc(sizeof(int) * 256);
    ret = filehist(finfo1->fname, finfo1->fsize, finfo1->hist);
    if (ret < 0) {
      return -1;
    }
  }
  hist1 = finfo1->hist;
  ret = filehist(fname2, fsize, hist2);
  if (ret < 0) {
    return -1;
  }
  sum = 0;
  for (i = 0; i < 256; ++i) {
    int h1 = hist1[i], h2 = hist2[i];
    sum += (h1 > h2) ? (h1 - h2) : (h2 - h1);
  }
  rete = (double)sum / fsize;
  return rete;
}
rp_fileinfo* findrepeat(rp_fileset* set, int64 fsize, char* fname, double same) {
  return NULL;
}

rp_fileinfo* findrepeat(rp_fileinfo* finfo, int fnum, int64 fsize, char* fname, double same)
{
  int i;
  if (same < 0.0000001) {
    for (i = 0; i < fnum; ++i) {
      if (finfo[i].fsize == fsize) {
        int ret = filecmp(finfo[i].fname, fname, fsize);
        if (0 == ret) {
          return finfo+i;
        }
      }
    }
  }
  else {
    for (i = 0; i < fnum; ++i) {
      double d = (double)ABSSUB(finfo[i].fsize, fsize) / (finfo[i].fsize + fsize);
      if (d < same / 10.) {
        double ret = filecmpsame(finfo + i, fname, fsize);
        if (ret < same) {
          return finfo+i;
        }
      }
    }
  }
  return NULL;
}
// 假设basetxt中已经没有重复文件
// 把listtxt中重复文件找出来
int test_findrepeat()
{
  char* listtxt = "list.txt";
  double same = 0;
  FILE* plisttxt;
  FILE* plisttxtout = NULL;
  char fname[2048];
  int result = 0, findret, basefnum = 0, listfnum = 0, fnum = 0, i = 0, finfo_sz = 0, repnum = 0;
  struct _stat bufst;
  rp_fileinfo* finfo = NULL;
  plisttxt = fopen(listtxt, "rb");
  if (!plisttxt) {
    return 0;
  }
  for (listfnum = 0; fgets(fname, 2048, plisttxt) > 0; ++listfnum) {}
  plisttxtout = fopen("replist.txt", "wb");
  finfo_sz = listfnum + basefnum;
  finfo = (rp_fileinfo*)malloc(sizeof(rp_fileinfo) * finfo_sz);
  memset(finfo, 0, sizeof(rp_fileinfo)*finfo_sz);
  fnum = 0;
  fseek(plisttxt, 0, SEEK_SET);
  {
    utime_start(_start_time);
    for (i = 0; fgets(fname, 2048, plisttxt) > 0;) {
      strtrim(fname, "\r\n");
      result = _stat(fname, &bufst);
      if (result == 0 && !(bufst.st_mode & _S_IFDIR)) {
        //printf( "%ld %c %s", buf.st_size, buf.st_dev + 'A', ctime( &buf.st_atime ) );
        //{utime_start(_start_time);
        rp_fileinfo* findret = findrepeat(finfo, fnum, bufst.st_size, fname, same);
        //printf("%f\n", utime_elapsed(_start_time));}
        if (findret==NULL) {
          findret = finfo+fnum++;
          findret->fname = strdup(fname);
          findret->fsize = bufst.st_size;
        }
        else {
          //fprintf(plisttxtout, "%s\r\n", fname);
          // 第一列为要删除的文件。第二列为保留的文件。别搞错了。
          fprintf(plisttxtout, "%s*%s\r\n", fname, findret->fname);
          fflush(plisttxtout);
          //printf("%s*%s\r\n", fname, finfo[findret].fname);
          ++repnum;
        }
      }
      {
        double t1 = utime_elapsed(_start_time);
        ++i;
        printf("scan list %4d/%6d/%d/%d [%3.1f%%] %5.fs\r", repnum, i, listfnum, basefnum, i * 100. / listfnum, t1);
      }
    }
  }
  for (i = 0; i < fnum; ++i) {
    free(finfo[i].fname);
    if (finfo[i].hist) {
      free(finfo[i].hist);
    }
  }
  free(finfo);
  if (plisttxtout != NULL) {
    fclose(plisttxtout);
  }
  fclose(plisttxt);
  return 0;
}

