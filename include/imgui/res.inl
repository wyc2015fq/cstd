#ifndef _RES_INL_
#define _RES_INL_
#include "cstd.h"
#include "cfile.h"
#include "codec/base64.inl"
#include "img/imgio.inl"
#include "geo/packing.inl"
//#include "ui/window.inl"
#define OUTMAXSTRLEN 128
int print_string(FILE* pf, const char* s, int len, int maxlen)
{
  int j;
  char buf[256];
  maxlen = MIN(maxlen, countof(buf) - 1);
  for (j = 0; j < len;) {
    int len2 = MIN(len - j, maxlen);
    strncpy(buf, s + j, len2);
    buf[len2] = 0;
    j += len2;
    fprintf(pf, "\"%s\"", buf);
    fprintf(pf, (j < len) ? "\r\n" : ",\r\n");
  }
  return 0;
}
int base64_encode_set(const void* buf, int buflen, char** pbse64buf)
{
  char* bse64buf = *pbse64buf;
  int len = 0;
  MYREALLOC(bse64buf, buflen * 2);
  *pbse64buf = bse64buf;
  len = base64_encode(buf, buflen, bse64buf, buflen * 2);
  return len;
}
int base64_decode_set(const char* base64str, void* pbuf)
{
  char* buf = *(char**)pbuf;
  int len, len1 = strlen(base64str);
  MYREALLOC(buf, len1 * 2);
  *(char**)pbuf = buf;
  len = base64_decode(base64str, len1, buf, len1 * 2);
  return len;
}
int print_base64_string(FILE* pf, const void* buf, int buflen)
{
  char* bse64buf = NULL;
  int ret, len;
  len = base64_encode_set(buf, buflen, &bse64buf);
  ret = print_string(pf, bse64buf, len, OUTMAXSTRLEN);
  FREE(bse64buf);
  return ret;
}
int print_array(FILE* pf, const void* buf, int buflen)
{
  uchar* ubuf = (uchar*)buf;
  int i;
  for (i = 0; i < buflen; ++i) {
    fprintf(pf, "0x%02x,\r\n", ubuf[i]);
  }
  return 0;
}
int print_array_block(FILE* pf, const void* buf, int buflen)
{
  print_array(pf, &buflen, sizeof(int));
  print_array(pf, buf, buflen);
  return 0;
}
int buf_getbuf(const char* buf, int buflen, int n, buf_t* bf)
{
  int i, j = 0;
  for (i = 0; i < n; ++i) {
    bf[i].len = *(int*)(buf + j);
    j += sizeof(int);
    bf[i].data = (uchar*)(buf + j);
    j += bf[i].len;
  }
  ASSERT(j <= buflen);
  return 0;
}
int imload_base64(const char* base64str, int req_cn, img_t* im)
{
  char* buf = NULL;
  int ret, len = base64_decode_set(base64str, &buf);
  ret = imload_mem(buf, len, req_cn, im);
  FREE(buf);
  return ret;
}
int impack(int n, const img_t* ims, idrect_t* rec, img_t* im)
{
  int i, rec_num = n, sumarea = 0, width = 0, max_y = 0;
  for (i = 0; i < n; ++i) {
    sumarea += ims[i].h * ims[i].w;
    rec[i].h = ims[i].h;
    rec[i].w = ims[i].w;
    rec[i].i = i;
  }
  width = sqrti(sumarea);
  width = next_power(width);
  packing_sortw(rec_num, rec);
  packing_sorth(rec_num, rec);
  max_y = packing_FFD2(width, rec_num, rec);
  max_y = next_power(max_y);
  imsetsize(im, max_y, width, 4, 1);
  for (i = 0; i < n; ++i) {
    const img_t* im1 = ims + rec[i].i;
    memcpy2d(im->tt.data + im->s * rec[i].y + 4 * rec[i].x, im->s, im1->tt.data, im1->s, im1->h, im1->w * 4);
    //printf("%d %d %d %d %d\n", i, rec[i].x, rec[i].y, rec[i].w, rec[i].h);
  }
  //imshow(im);
  //cvWaitKey(-1);
  return 0;
}
//#include "ui/window.inl"
int res_pack_make(const char* inlist, const char* outfile)
{
  idrect_t* rec = NULL;
  img_t* ims = NULL;
  img_t* im = NULL;
  vstr_t sv[1] = {0};
  int i, n;
  FILE* pf;
  char buf[256];
  vstr_load(inlist, sv);
  n = sv->n;
  MYREALLOC(rec, n);
  MYREALLOC(ims, n + 1);
  im = ims + n;
  pf = fopen(outfile, "wb");
  for (i = 0; i < n; ++i) {
    imread(sv->v[i].s, 4, 1, ims + i);
    GetFileNameCopy(sv->v[i].s, buf, countof(buf));
    str_setstr2(sv->v + i, buf);
    //imshowmat_x4((ims+i));cvWaitKey(-1);
  }
  {
    str_t s[1] = {0};
    strv_merge(sv, s, "|");
    str_catc(s, '|');
    print_array_block(pf, s->s, s->l + 1);
    str_free(s);
  }
  impack(n, ims, rec, im);
  {
    IRECT* rcs = NULL;
    MYREALLOC(rcs, n);
    for (i = 0; i < n; ++i) {
      idrect_t* rec1 = rec + i;
      rcs[rec[i].i] = iRECT2(rec1->x, rec1->y, rec1->w, rec1->h);
    }
    print_array_block(pf, rcs, n * sizeof(*rcs));
    FREE(rcs);
  }
  {
    char* pngbuf = NULL;
    int pngbuflen = im->h * im->w * 4, pnglen;
    MYREALLOC(pngbuf, pngbuflen);
    pnglen = png_saveim_mem(pngbuf, pngbuflen, im);
    //pnglen = png_saveim_filename("C:\\aaa.png", im);
    //print_base64_string(pf, pngbuf, pnglen);
    print_array_block(pf, pngbuf, pnglen);
    FREE(pngbuf);
  }
  fclose(pf);
  imfrees(ims, n + 1);
  strv_free(sv);
  FREE(rec);
  FREE(ims);
  return 0;
}
int res_pack_getimg(const char* res, int reslen, img_t* im)
{
  buf_t bf[3] = {0};
  buf_getbuf(res, reslen, countof(bf), bf);
  return imload_mem(bf[2].data, bf[2].len, 4, im);
}
int res_pack_find(const char* res, int reslen, const char* name, IRECT* prc)
{
  buf_t bf[3] = {0};
  int k, n;
  buf_getbuf(res, reslen, countof(bf), bf);
  n = bf[1].len / sizeof(idrect_t);
  k = cstr_splitfind((char*)bf[0].data, -1, name, -1, 1, NULL);
  if (prc && k >= 0 && k < n) {
    *prc = ((IRECT*)(bf[1].data))[k];
  }
  return k;
}
int res_make_picarr(const char* inlist) {
  vstr_t sv[1] = {0};
  buf_t bf[1] = {0};
  str_t s[1] = {0};
  int i;
  char buf[256];
  char name[256];
  img_t im[1] = {0};
  vstr_load(inlist, sv);
  for (i = 0; i < sv->n; ++i) {
    imread(sv->v[i].s, 4, 1, im);
    _snprintf(buf, 256, "%s_%dx%dx%d.txt", GetFileNameCopy(sv->v[i].s, name, countof(name)), im->h, im->w, im->c);
    savedata_inl(buf, im->tt.data, im->h*im->s);
  }
  strv_free(sv);
  imfree(im);
  return 0;
}
int res_make(const char* inlist, const char* outfile)
{
  vstr_t sv[1] = {0};
  buf_t bf[1] = {0};
  str_t s[1] = {0};
  int i, len;
  FILE* pf;
  char name[256];
  vstr_load(inlist, sv);
  pf = fopen(outfile, "wb");
  for (i = 0; i < sv->n; ++i) {
    buf_load(sv->v[i].s, bf);
    str_setsize(s, bf->len * 2);
    len = base64_encode(bf->data, bf->len, s->s, s->l);
    fprintf(pf, "//%s\r\n", GetFileNameCopy(sv->v[i].s, name, countof(name)));
    fprintf(pf, "\"%s\",\r\n", GetFileNameCopy(sv->v[i].s, name, countof(name)));
    print_string(pf, s->s, len, OUTMAXSTRLEN);
  }
  fprintf(pf, "NULL,\r\n");
  fclose(pf);
  str_free(s);
  bffree(bf);
  strv_free(sv);
  return 0;
}
int test_filebase64()
{
  sys_chdir("E:/pub/bin/codec/base64");
  res_make("list.txt", "res.txt");
  return 0;
}
const char* res_find(const char** res, const char* name)
{
  int i, len = 0;
  if (res) {
    for (i = 0; res[i]; i+=2) {
      if (0 == strcmp(name, res[i])) {
        return res[i+1];
      }
    }
  }
  return NULL;
}
int loadTextureRes(render_drv_t* drv, const char** res, const char* name, texture_t* tex)
{
  char* buf = NULL;
  int ret = 0;
  img_t im[1] = {0};
  const char* base64str = res_find(res, name);
  if (base64str && imload_base64(base64str, 4, im)) {
    PixFmt fmt =  cn2PixFmt(im->c);
    drv->SetTexture(drv, tex, fmt, im->h, im->w, im->tt.data, im->s);
    imfree(im);
    ret = 1;
  }
  return ret;
}
int loadTextureFile(render_drv_t* drv, const char* picpath, texture_t* tex)
{
  img_t im[1] = {0};
  int ret = 0;
  if (imread(picpath, 4, 1, im)) {
    PixFmt fmt = cn2PixFmt(im->c);
    drv->SetTexture(drv, tex, fmt, im->h, im->w, im->tt.data, im->s);
    imfree(im);
    ret = 1;
  }
  return ret;
}

int res_set_cursor(const char* cursor) {
  sysio_t* io = sys_getio();
  int ret = 0;
  if (io && io->res) {
    if (io->cursor != cursor) {
      if (cursor) {
        const char* base64str = res_find(io->res, cursor);
        img_t im[1] = {0};
        if (base64str && imload_base64(base64str, 4, im)) {
          sys_set_cursor_image(im->h, im->w, im->tt.data, im->s, im->c, 0, 0);
          io->cursor = cursor;
          imfree(im);
          ret = 1;
        }
      } else {
        io->cursor = cursor;
        sys_set_cursor_image(0, 0, 0, 0, 0, 0, 0);
      }
    } else {
      ret = !!cursor;
    }
  }
  //printf("ret = %d\n", ret);
  return ret;
}

#endif // _RES_INL_
