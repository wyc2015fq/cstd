
#ifndef _IMGSET_INL_
#define _IMGSET_INL_

int get_text_line_number(const char* flist, const char* trims, int minlen)
{
  int i;
  char buf[256];
  FILE* plisttxt = fopen(flist, "rb");

  for (i = 0; fgets(buf, 256, plisttxt);) {
    strtrim(buf, trims);

    if (strlen(buf) > minlen) {
      ++i;
    }
  }

  fclose(plisttxt);
  return i;
}

typedef struct imgset_t {
  int minid, maxid;
  img_t im[1]; // 多帧图片 im->f==图片数目
  int* id; // 图片id
} imgset_t;

int imgset_setsize(imgset_t* s, int h, int w, int c, int f)
{
  imsetsize(s->im, h, w, c, f);
  s->id = MALLOC(int, f);
  return 0;
}

int imgset_free(imgset_t* s)
{
  imfree(s->im);
  FREE(s->id);
  return 0;
}

int fload_buf(FILE* pf, void* buf, int len)
{
  return fread(buf, 1, len, pf);
}

int fsave_buf(FILE* pf, const void* buf, int len)
{
  return fwrite(buf, 1, len, pf);
}

#define imgset_savedatafile(s, fname)  {FILE* pf=fopen(fname, "wb"); imgset_savedata(s, pf); fclose(pf);}
#define imgset_loaddatafile(s, fname)  {FILE* pf=fopen(fname, "rb"); imgset_loaddata(s, pf); fclose(pf);}

// 从id标记的集合中每类选出k个元素
int mask_set(int n, const int* ids, int k, uchar* mask)
{
  int i, minid = MAX_INT, maxid = MIN_INT, ncnt = 0;
  int* cnt = 0;

  for (i = 0; i < n; ++i) {
    minid = MIN(minid, ids[i]);
    maxid = MAX(maxid, ids[i]);
  }

  ncnt = maxid - minid + 2;
  cnt = MALLOC(int, ncnt);
  MEMSET(cnt, 0, ncnt);
  memset(mask, 0, n);

  for (i = 0; i < n; ++i) {
    int cid = ids[i] - minid;

    if (cnt[cid] < k) {
      mask[i] = 1;
    }

    cnt[cid]++;
  }

  return 0;
}

int imgset_savedata(const imgset_t* s, FILE* pf)
{
  int h = s->im->h, w = s->im->w, c = s->im->c, f = s->im->f;
  fsave_buf(pf, &h, sizeof(int));
  fsave_buf(pf, &w, sizeof(int));
  fsave_buf(pf, &c, sizeof(int));
  fsave_buf(pf, &f, sizeof(int));
  fsave_buf(pf, &s->minid, sizeof(int));
  fsave_buf(pf, &s->maxid, sizeof(int));
  fsave_buf(pf, s->id, f * sizeof(int));
  fsave_buf(pf, s->im->tt.data, h * w * c * f);
  return 0;
}
int imgset_loaddata(imgset_t* s, FILE* pf)
{
  int h, w, c, f;
  fload_buf(pf, &h, sizeof(int));
  fload_buf(pf, &w, sizeof(int));
  fload_buf(pf, &c, sizeof(int));
  fload_buf(pf, &f, sizeof(int));
  imgset_setsize(s, h, w, c, f);
  fload_buf(pf, &s->minid, sizeof(int));
  fload_buf(pf, &s->maxid, sizeof(int));
  fload_buf(pf, s->id, f * sizeof(int));
  fload_buf(pf, s->im->tt.data, h * w * c * f);
  return 0;
}

int map_init(uchar* map, const char* str, int len)
{
  int i;
  const uchar* us = (const uchar*)str;

  for (i = 0; i < len; ++i) {
    map[us[i]] = i;
  }

  return 0;
}

int str2id(const char* str, int len, const uchar* map, int base)
{
  int i, id = 0;
  const uchar* us = (const uchar*)str;

  for (i = 0; i < len; ++i) {
    id = id * base + map[us[i]];
  }

  return id;
}

// 读取图片列表以及图片id,根据图片文件名获取id
// beg 数字id在图片文件名字符串的开始位置
// len 数字id在图片文件名字符串的长度
int imgset_read(imgset_t* s, int cn, const char* flist, int beg, int len, const uchar* map)
{
  FILE* plisttxt;
  char buf[256];
  int iframe, line_num, id;
  img_t im[1] = {0};
  s->minid = 1000000;
  s->maxid = 0;
  line_num = get_text_line_number(flist, " \n\r", 1);

  plisttxt = fopen(flist, "rb");

  //for ( iframe = 0; fgets(buf, 256, plisttxt)>0; ++iframe )
  for (iframe = 0; fgets(buf, 256, plisttxt);) {
    strtrim(buf, "\n\r");

    if (imread(buf, 1, 1, im)) {
      if (NULL == s->id) {
        imgset_setsize(s, im->h, im->w, cn, line_num);
      }

      imsetframe(s->im, iframe, im);
      ASSERT(s->id);
      s->id[iframe] = id = str2id(buf + beg, len, map, 10);
      s->minid = MIN(s->minid, id);
      s->maxid = MAX(s->maxid, id);
      //s->im->h = im->h*(iframe+1); imshow(s->im);cvWaitKey(-1); s->im->h = im->h;
      ++iframe;
    }
  }

  s->im->f = iframe;
  imfree(im);
  fclose(plisttxt);
  return iframe;
}

int imgset_readv(imgset_t* s, int cn, const vstr_t* sv, int beg, int len, const uchar* map, vstr_t* sv2)
{
  int i, iframe = 0, id;
  img_t im[1] = {0};
  s->minid = 1000000;
  s->maxid = 0;

  if (sv2) {
    strv_setsize(sv2, sv->n);
  }

  for (i = 0; i < sv->n; ++i) {
    if (imread(sv->v[i].s, cn, 1, im)) {
      if (NULL == s->id) {
        imgset_setsize(s, im->h, im->w, cn, sv->n);
      }

      imsetframe(s->im, iframe, im);
      ASSERT(s->id);
      s->id[iframe] = id = str2id(sv->v[i].s + beg, len, map, 10);
      s->minid = MIN(s->minid, id);
      s->maxid = MAX(s->maxid, id);

      if (sv2) {
        str_setstr1(sv2->v + iframe, sv->v[i]);
      }

      //s->im->h = im->h*(iframe+1); imshow(s->im);cvWaitKey(-1); s->im->h = im->h;
      ++iframe;
    }
  }

  s->im->f = iframe;
  imfree(im);
  return iframe;
}

#endif // _IMGSET_INL_
