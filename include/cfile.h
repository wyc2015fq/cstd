#ifndef _FILEIO_H_
#define _FILEIO_H_
#include "cstd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include "str.h"
#undef USESYS
#define USESYS
//#include "str/iconv.inl"
typedef struct stat stat_t;
#define strend(_s)  ((_s) + strlen(_s))
#define FN_IS_DOTS(_FN)      (((_FN)[0] == '.') && ((_FN)[1] == '\0' || ((_FN)[1] == '.' && (_FN)[2] == '\0')))

#define file_exist(name)       (0 == sys_access(name, AS_EXIST))
#define file_execute(name)     (0 == sys_access(name, AS_EXECUTE))
#define file_write(name)       (0 == sys_access(name, AS_WRITE))
#define file_read(name)        (0 == sys_access(name, AS_READ))
#define file_write_read(name)  (0 == sys_access(name, AS_WRITE_READ))
//#define file_subdir(name)      (0 == sys_access(name, AS_SUBDIR))
#define fileok(name)           (file_exist( name ) && filesize(name)>0)
static char* find_not_eixst_file(const char* filename, char* buf, int buflen)
{
  int i = 0;
  static char _buf[ 256 ];
  if (NULL == buf) {
    buf = _buf;
    buflen = 256;
  }
  for (; ; ++i) {
    snprintf(buf, buflen, filename, i);
    if (!file_exist(buf)) {
      return buf;
    }
  }
}
CC_INLINE int savefile(const char* fname, const void* buf, int buflen)
{
  FILE* pf = fopen(fname, "wb");
  int n = 0;
  if (NULL == pf) {
    return 0;
  }
  n = fwrite(buf, 1, buflen, pf);
  fclose(pf);
  return n;
}
CC_INLINE int loadfile(const char* fname, void* buf, int buflen, int pos)
{
  FILE* pf = fopen(fname, "rb");
  int n = 0;
  if (NULL == pf) {
    return 0;
  }
  if (pos>0) {
    fseek(pf, pos, SEEK_SET);
  }
  n = fread(buf, 1, buflen, pf);
  fclose(pf);
  return n;
}
CC_INLINE int savedata2txt_impl(FILE* pf, int h, int w, const void* A, int al,
                                 const char* szBegL, const char* szFmt, const char* szEndL)
{
  int i, j;
  const uchar* A0 = (const uchar*)A;
  
  for (i = 0; i < h; ++i, A0 += al) {
    fprintf(pf, "%s", szBegL);
    
    for (j = 0; j < w; ++j) {
      fprintf(pf, szFmt, A0[j]);
    }
    
    fprintf(pf, "%s", szEndL);
  }
  
  return 0;
}
//"%02x" "\n"
CC_INLINE int savedata2txt(const char* fn, int h, int w, const void* A, int al, const char* szBegL, const char* szFmt, const char* szEndL)
{
  FILE* pf;
  pf = fopen(fn, "wb");
  
  if (NULL == pf) {
    return 0;
  }
  
  savedata2txt_impl(pf, h, w, A, al, szBegL, szFmt, szEndL);
  fclose(pf);
  return 0;
}
static long fsize(FILE* stream)
{
  long pos, size;
  pos = ftell(stream);
  fseek(stream, 0, SEEK_END);
  size = ftell(stream);
  fseek(stream, pos, SEEK_SET);
  return size;
}
#ifndef FILESIZE
#define FILESIZE(_PF, _SZ)               { int _cur = ftell(_PF); fpos_t pos; fseek(_PF, 0, SEEK_END); fgetpos(_PF, &pos); _SZ = (int)pos; fseek(_PF, _cur, SEEK_SET); }
#endif
#ifdef _WIN
static DWORD _FILEIO_H__NumberOfBytes;
typedef HANDLE HD_FILE;
#define INVALID(_PF)                  (INVALID_HANDLE_VALUE == (_PF) || 0 == (_PF))
#define FILE_READ                     GENERIC_READ
#define FILE_WRITE                    GENERIC_WRITE
#define FILE_READWRITE                (GENERIC_READ|GENERIC_WRITE)
#define FOPEN(_NAME, _MODE)           CreateFile(_NAME, _MODE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, (_MODE & GENERIC_WRITE) ? (OPEN_ALWAYS) : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)
#define FCLOSE(_PF)                   ((!INVALID(_PF) ? CloseHandle(_PF) : 0), _PF=INVALID_HANDLE_VALUE)
#define FWRITE(_PTR, _N, _PF)         (WriteFile(_PF, _PTR, (_N)*sizeof(*(_PTR)), &_FILEIO_H__NumberOfBytes, NULL), _FILEIO_H__NumberOfBytes)
#define FREAD(_PTR, _N, _PF)          (ReadFile(_PF, _PTR, (_N)*sizeof(*(_PTR)), &_FILEIO_H__NumberOfBytes, NULL), _FILEIO_H__NumberOfBytes)
#define FFLUSH(_PF)                   FlushFileBuffers(_PF)
#define FSEEKSET(_PF, _OFF)           { LARGE_INTEGER li; li.QuadPart = (_OFF); li.LowPart=SetFilePointer(_PF,li.LowPart,&li.HighPart,FILE_BEGIN); }
#define FSEEKEND(_PF, _OFF)           { LARGE_INTEGER li; li.QuadPart = (_OFF); li.LowPart=SetFilePointer(_PF,li.LowPart,&li.HighPart,FILE_END); }
#define FSIZE(_PF, _SZ)               (_SZ)=GetFileSize(_PF,NULL)
#define FWRITEPOS(_PF, _IDX, _PTR, _N)  { FSEEKSET(_PF, ((uint64)(_IDX))*((uint64)sizeof(*(_PTR)))); FWRITE(_PTR, (_N), _PF); }
#define FREADPOS(_PF, _IDX, _PTR, _N)  { FSEEKSET(_PF, ((uint64)(_IDX))*((uint64)sizeof(*(_PTR)))); FREAD(_PTR, (_N), _PF); }
#else
typedef FILE* HD_FILE;
#define INVALID(_PF)                  (NULL == (_PF))
#define FILE_READ                     "r"
#define FILE_WRITE                    "w"
#define FILE_READWRITE                "ab+"
#define FOPEN(_NAME, _MODE)           fopen(_NAME, _MODE)
#define FCLOSE(_PF)                   ((!INVALID(_PF) ? fclose(_PF) : 0), _PF=NULL)
#define FWRITE(_PTR, _N, _PF)         fwrite(_PTR, (_N)*sizeof(*(_PTR)), 1, _PF)
#define FREAD(_PTR, _N, _PF)          fread(_PTR, (_N)*sizeof(*(_PTR)), 1, _PF)
#define FFLUSH(_PF)                   fflush(_PF)
#define FSEEKSET(_PF, _OFF)           fseek(_PF, _OFF, SEEK_SET)
#define FSEEKEND(_PF, _OFF)           fseek(_PF, _OFF, SEEK_END)
//#define FSIZE(_PF, _SZ)               FILESIZE(_PF, _SZ)
//#define FSIZE(_PF, _SZ)               (_SZ)=filelength(_PF,NULL)
#define FWRITEPOS(_PF, _IDX, _PTR, _N)  { FSEEKSET(_PF, ((size_t)(_IDX))*((size_t)sizeof(*(_PTR)))); FWRITE(_PTR, (_N), _PF); }
#define FREADPOS(_PF, _IDX, _PTR, _N)  { FSEEKSET(_PF, ((size_t)(_IDX))*((size_t)sizeof(*(_PTR)))); FREAD(_PTR, (_N), _PF); }
#endif // _WIN
#ifndef FILESIZE
#define FILESIZE(_PF, _SZ)               { int _cur = ftell(_PF); fpos_t pos; fseek(_PF, 0, SEEK_END); fgetpos(_PF, &pos); _SZ = (int)pos; fseek(_PF, _cur, SEEK_SET); }
#endif
#define DIR_SEPARATOR '\\'

static int GetLength(FILE* file)
{
  int old = ftell(file), len = 0;
  fseek(file, 0, SEEK_END);
  len = ftell(file);
  fseek(file, old, SEEK_SET);
  return len;
}
static int filesize(const char* file) {
  sys_stat st;
  return (sys_filestat(file, &st) == 0) ? st.size : 0;
}
#define DIRSP   '\\'
static int mkdirs2(const char* dir, int len)
{
  int i = 0, j = 0;
  int iRet;
  char* buf;
  int buflen = 0;
  len = len<0 ? strlen(dir) : len;
  buf = (char*)pmalloc(buflen = len * 2);
  // 锟斤拷锟斤拷目录
  for (i = 0; i < len; ++i) {
    if ('\\' == dir[i] || '/' == dir[i]) {
      ASSERT((j + 1) < buflen);
      buf[j++] = DIRSP;
      buf[j] = 0;
      iRet = sys_access(buf, AS_EXIST);
      if (iRet != 0) {
        iRet = sys_mkdir(buf);
        if (iRet != 0) {
          pfree(buf);
          return -1;
        }
      }
    }
    else {
      buf[j++] = dir[i];
      ASSERT(j < buflen);
    }
  }
  ASSERT((j + 1) < buflen);
  buf[j++] = DIRSP;
  buf[j] = 0;
  //
  iRet = sys_access(buf, 0);
  if (iRet != 0) {
    iRet = sys_mkdir(buf);
    if (iRet != 0) {
      pfree(buf);
      return -1;
    }
  }
  pfree(buf);
  return 0;
}
static int mkdirs(const char* filename)
{
  char path[ MAX_PATH ];
  char* p;
  int pos;
  sys_stat st;
  strcpy(path, filename);
  p = path;
  for (; ;) {
    int len = strlen(p);
    pos = strcspn(p, ("/\\"));
    if (pos != 0) {
      p[ pos ] = '\0';
      if (p[ pos - 1 ] != (':')) {
        if (sys_filestat(path, &st) != 0) {
          if (sys_mkdir(path) != 0) {
            return 0;
          }
        }
      }
    }
    if (pos == len) {
      break;
    }
    p[ pos ] = '/';
    p += pos + 1;
  }
  return 1;
}
static void* filemap(const char* fname, int* pflen)
{
  FILE* pf;
  int flen;
  char* buf = NULL;
  pf = fopen(fname, ("rb"));
  if (pf) {
    flen = (int)filesize(fname);
    buf = (char*) pmalloc(flen + 20);
    memset(buf, 0, flen + 20);
    fread(buf, flen, 1, pf);
    fclose(pf);
    if (pflen) {
      * pflen = flen;
    }
  }
  return buf;
}
CC_INLINE int buf_ins(buf_t* bf, int i, int n, const void* s0, int l0)
{
  int oldlen = bf->len;
  bfsetsize(bf, bf->len + l0 - n);
  memrep(bf->data, oldlen, i, n, s0, l0, bf->data, bf->len);
  return 0;
}
CC_INLINE int buf_push(buf_t* bf, const void* s0, int l0) {
  return buf_ins(bf, bf->len, 0, s0, l0);
}
static int buf_load(const char* fname, buf_t* bf)
{
  int len, readed_len;
  FILE* pf;
  pf = fname ? fopen(fname, "rb") : NULL;
  if (pf) {
    len = fsize(pf);
    bfsetsize(bf, len);
    readed_len = fread(bf->data, 1, len, pf);
    assert(readed_len == len);
    fclose(pf);
    return 1;
  }
  return 0;
}
static int buf_load_from_paths(const char* paths, const char* filename, const char* ext, buf_t* bf)
{
  vstr_t sv1[1] = {0};
  vstr_t sv2[1] = {0};
  str_t s1[1] = {0};
  int i, j, ret = 0;
  vstr_split_str(sv1, STR1(paths), "|", 0, 0);
  vstr_split_str(sv2, STR1(ext), "|", 0, 0);
  for (i = 0; i < sv1->n && !ret; ++i) {
    for (j = 0; j < sv2->n && !ret; ++j) {
      if (strchr(filename, ':')) {
        str_cat2(s1, STR1(filename), sv2->v[j]);
      }
      else {
        str_cat4(s1, sv1->v[i], STR1("/"), STR1(filename), sv2->v[j]);
      }
      if ((ret = buf_load(s1->s, bf))!=0) {
        break;
      }
    }
  }
  str_free(s1);
  vstr_free(sv2);
  vstr_free(sv1);
  return ret;
}
static int buf_save(const char* fname, const buf_t* bf)
{
  FILE* pf = fopen(fname, "wb");
  if (pf) {
    int writed_len = fwrite(bf->data, 1, bf->len, pf);
    assert(writed_len == bf->len);
    fclose(pf);
  }
  return 0;
}
static int savebufs(const char* fname, const buf_t* bf, int n) {
  if (bf && n>0) {
    FILE* pf = fopen(fname, "wb");
    if (pf) {
      int i, writed_len;
      for (i=0; i<n; ++i) {
        writed_len = fwrite(bf[i].data, 1, bf[i].len, pf);
        assert(writed_len == bf[i].len);
      }
      fclose(pf);
    }
  }
  return 0;
}
static char* loaddata0(const char* fname, int* pLen)
{
  int len, readed_len;
  FILE* pf;
  char* str;
  if (!fname || !fname[0]) {
    return 0;
  }
  pf = fopen(fname, "rb");
  if (NULL == pf) {
    return 0;
  }
  len = fsize(pf);
  str = (char*)pmalloc(len + 1);
  memset(str, 0, len + 1);
  readed_len = fread(str, 1, len, pf);
  assert(readed_len == len);
  fclose(pf);
  if (pLen) {
    *pLen = len;
  }
  return str;
}
static char* read_text(const char* fname)
{
  int len, readed_len;
  FILE* pf;
  char* str;
  pf = fopen(fname, "rb");
  if (NULL == pf) {
    return 0;
  }
  len = fsize(pf);
  str = (char*)pmalloc(len + 1);
  memset(str, 0, len + 1);
  readed_len = fread(str, 1, len, pf);
  assert(readed_len == len);
  fclose(pf);
  return str;
}
static int loaddata_1(const char* fn, void* buf, size_t n)
{
  FILE* pf = fopen(fn, "rb");
  if (pf) {
    size_t rn = fread(buf, 1, n, pf);
    //ASSERT(rn==n);
    fclose(pf);
  }
  return 0;
}
static int loaddata_11(const char* fn, void* buf, size_t n, int off)
{
  FILE* pf = fopen(fn, "rb");
  if (pf) {
    size_t rn;
    fseek(pf, off, SEEK_SET);
    rn = fread(buf, 1, n, pf);
    //ASSERT(rn == n);
    fclose(pf);
    return rn;
  }
  return 0;
}
static int savedata_inl(const char* fn, const void* buf, size_t n)
{
  FILE* pf = fopen(fn, "wb");
  if (pf) {
    size_t i;
    const unsigned char* ucbuf = (const unsigned char*)buf;
    for (i = 0; i < n; ++i) {
      fprintf(pf, "0x%02x,\n", ucbuf[i]);
    }
    fclose(pf);
  }
  return 0;
}
static int savedata_11(const char* fn, const void* buf, size_t n, int off)
{
  FILE* pf = fopen(fn, "wb");
	int ret=0;
  if (pf) {
    size_t rn;
    fseek(pf, off, SEEK_SET);
    rn = fwrite(buf, 1, n, pf);
    ret = (rn == n);
    fclose(pf);
  }
  return ret;
}
static int savedata(const char* fn, int nbuf, ...)
{
  FILE* pf = fopen(fn, "wb");
  va_list marker;
  char* buf;
  int n, rn, i, wn2=0;
  if (pf) {
    va_start(marker, nbuf);
    for (i=0; i<nbuf; ++i) {
      buf = va_arg(marker, char*);
      n = va_arg(marker, size_t);
      if (buf && n>0) {
        rn = fwrite(buf, 1, n, pf);
        wn2 += rn;
      }
    }
    va_end(marker);
    fclose(pf);
  }
  return 0;
}
static int savedata2(FILE* pf, ...)
{
  va_list marker;
  char* buf;
  size_t n;
  int ret = 0;
  if (pf) {
    va_start(marker, pf);
    while (1) {
      buf = va_arg(marker, char*);
      if (0 == buf) {
        break;
      }
      n = va_arg(marker, size_t);
      if (0 == n) {
        break;
      }
      ret = n == fwrite(buf, 1, n, pf);
      
    }
    va_end(marker);
  }
  return ret;
}
static int loaddata_2(FILE* pf, ...)
{
  va_list marker;
  char* buf;
  size_t n;
  int ret=0;
  va_start(marker, pf);
  while (1) {
    buf = va_arg(marker, char*);
    if (0 == buf) {
      break;
    }
    n = va_arg(marker, size_t);
    if (0 == n) {
      break;
    }
    ret = n == fread(buf, 1, n, pf);
  }
  va_end(marker);
  return ret;
}
static int loaddatafn(const char* fn, ...)
{
  FILE* pf = fopen(fn, "rb");
  va_list marker;
  char** buf;
  size_t n;
  int ret=0;
  if (pf) {
    va_start(marker, fn);
    while (1) {
      buf = va_arg(marker, char**);
      if (0 == buf) {
        break;
      }
      n = va_arg(marker, size_t);
      if (0 == n) {
        break;
      }
      *buf = (char*)pmalloc(n);
      ret = n == fread(*buf, 1, n, pf);
    }
    va_end(marker);
  }
  return ret;
}
static int loaddatafn2(const char* fn, ...)
{
  FILE* pf = fopen(fn, "rb");
  va_list marker;
  char** buf;
  size_t i, n, nall, rn;
  char* bufall;
  int ret=0;
  if (pf) {
    nall = 0;
    va_start(marker, fn);
    while (1) {
      buf = va_arg(marker, char**);
      if (0 == buf) {
        break;
      }
      n = va_arg(marker, size_t);
      if (0 == n) {
        break;
      }
      nall += n;
    }
    va_end(marker);
  }
  if (pf) {
    i = 0;
    bufall = (char*)pmalloc(nall);
    rn = fread(bufall, 1, nall, pf);
    va_start(marker, fn);
    while (1) {
      buf = va_arg(marker, char**);
      if (0 == buf) {
        break;
      }
      n = va_arg(marker, size_t);
      if (0 == n) {
        break;
      }
      *buf = bufall + i;
      i += n;
      ret = rn==rn;
    }
    va_end(marker);
  }
  return ret;
}
#if 0
static int vasprintf(char** pStr, const char* fmt, va_list ARGLIST)
{
  int len = 0;
  char* buf = 0;
  int sz = 256;
  for (; 1;) {
    buf = (char*)pmalloc(sz);
    len = _vsnprintf(buf, sz, fmt, ARGLIST);
    if (len > sz - 10) {
      sz *= 2;
      pfree(buf);
    }
    else {
      if (pStr) {
        *pStr = buf;
      }
      else {
        pfree(buf);
      }
      break;
    }
  }
  return len;
}
CC_INLINE int asprintf(char** pStr, const char* fmt, ...)
{
  int len;
  va_list ARGLIST;
  va_start(ARGLIST, fmt);
  len = vasprintf(pStr, fmt, ARGLIST);
  va_end(ARGLIST);
  return len;
}
#endif
CC_INLINE int str_load(const char* fname, str_t* s)
{
  size_t len, readed_len;
  FILE* pf;
  pf = fopen(fname, "rb");
  if (pf) {
    len = fsize(pf);
    str_setsize(s, len);
    readed_len = fread(s->s, 1, len, pf);
    assert(readed_len == len);
    fclose(pf);
    return 1;
  }
  return 0;
}
CC_INLINE int str_loadasni(const char* fn, str_t* s)
{
  if (str_load(fn, s)) {
    str_toasni(s, s);
    return 1;
  }
  return 0;
}
CC_INLINE int str_save(const char* fname, const str_t* s)
{
	size_t writeed_len;
  FILE* pf;
  pf = fopen(fname, "wb");
  if (pf) {
    writeed_len = fwrite(s->s, 1, s->l, pf);
    printf("%d %d", writeed_len, s->l);
    //ASSERT(writeed_len == s->l);
    fclose(pf);
    return 1;
  }
  return 0;
}
CC_INLINE int vstr_save(const char* fname, vstr_t* sv) {
  str_t s[1] = {0};
  vstr_merge(sv, s, "\n");
  str_save(fname, s);
  str_free(s);
  return 0;
}
CC_INLINE int vstr_load(const char* fname, vstr_t* sv)
{
  str_t s[1] = {0};
  if (str_load(fname, s)) {
    vstr_split_str(sv, *s, "\n", "\r\n", 1);
    str_free(s);
    return 1;
  }
  return 0;
}
CC_INLINE char* splitname(const char* path, char* name)
{
  const char* namebeg;
  const char* nameend;
  namebeg = strrchr(path, '\\');
  if (!namebeg) {
    namebeg = strrchr(path, '/');
  }
  if (!namebeg) {
    namebeg = path;
  }
  if (namebeg) {
    ++namebeg;
    nameend = strrchr(namebeg, '.');
    if (nameend) {
      int len;
      len = nameend - namebeg;
      memcpy(name, namebeg, len);
      name[ len ] = 0;
    }
    else {
      strcpy(name, namebeg);
    }
  }
  return name;
}
//***通锟斤拷锟斤拷
typedef struct stream_t_ stream_t;
struct stream_t_ {
  int (*eof)(stream_t* stream);
  int (*close)(stream_t* stream);
  int (*tell)(stream_t* stream);
  int (*get8)(stream_t* stream);
  char* (*gets)(stream_t* stream, char* buf, int bufsize);
  int (*unget8)(int ch, stream_t* stream);
  int (*put8)(stream_t* stream, int x);
  int (*seek)(stream_t* stream, int offset, int origin);
  int (*read)(stream_t* stream, void* buf, int len);
  int (*write)(stream_t* stream, const void* buf, int len);
  void* x;
  int x1, x2;
};
#define BYTEORDER(X)  X = sizeof(X)==4 ? byteorder_i4(&(X), order) : sizeof(X)==2 ? byteorder_i2(&(X), order) : (X)
#define BYTEORDER_I2(X)  {uint16 x = (uint16)X; X = BYTEORDER(x);}
#define BYTEORDER_I4(X)  {int32 x = X; X = BYTEORDER(x);}
CC_INLINE int32 byteorder_i2(void* x, int order)
{
  uchar* u = (uchar*)x;
  return order ? (u[0]) | (u[1] << 8) : (u[1]) | (u[0] << 8);
}
CC_INLINE int32 byteorder_i4(void* x, int order)
{
  uchar* u = (uchar*)x;
  return order ? (u[0]) | (u[1] << 8) | (u[2] << 16) | (u[3] << 24) : (u[3]) | (u[2] << 8) | (u[1] << 16) | (u[0] << 24);
}
#define stream_eof(stream)                    (stream)->eof(stream)
#define stream_close(stream)                  (stream)->close(stream)
#define stream_tell(stream)                   (stream)->tell(stream)
#define stream_get8(stream)                   (stream)->get8(stream)
#define stream_unget8(ch, stream)             (stream)->unget8(ch, stream)
#define stream_put8(stream, x)                (stream)->put8(stream, x)
#define stream_putc(stream, x)                (stream)->put8(stream, x)
#define stream_seek(stream, offset, origin)   (stream)->seek((stream), offset, origin)
#define stream_read(stream, buffer, len)      (stream)->read((stream), buffer, len)
#define stream_write(stream, buffer, len)     (stream)->write((stream), buffer, len)
#define stream_init_func(s, name)  (s->gets = name##stream_gets , s->put8 = name##stream_put8 , s->eof = name##stream_eof , s->close = name##stream_close , s->tell = name##stream_tell , s->get8 = name##stream_get8 , s->unget8 = name##stream_unget8 , s->seek = name##stream_seek , s->read = name##stream_read , s->write = name##stream_write )
#define stream_getu1(stream) stream_get8(stream)
#define stream_getu2(stream)  ((stream_getu1(stream))|(stream_getu1(stream)<<8))
#define stream_getu4(stream)  ((stream_getu1(stream))|(stream_getu1(stream)<<8)|(stream_getu1(stream)<<16)|(stream_getu1(stream)<<24))
static uint32 stream_filesize(stream_t* fp)
{
  uint32 pos;
  uint32 pos_cur;
  pos_cur = stream_tell(fp);
  stream_seek(fp, 0, SEEK_END);
  pos = stream_tell(fp);
  stream_seek(fp, pos_cur, SEEK_SET);
  return pos;
}
CC_INLINE int stream_puts(stream_t* fp, const char* str) {
  size_t len = strlen(str);
  return stream_write(fp, str, len);
}
CC_INLINE int stream_rewind(stream_t* fp) {
  return stream_seek(fp, 0, SEEK_SET);
}
CC_INLINE int stream_puts2(const char* str, stream_t* fp) {
	size_t len = strlen(str);
  return stream_write(fp, str, len);
}
CC_INLINE char* stream_gets(char* buf, int bufsize, stream_t* fp) {
  return fp->gets(fp, buf, bufsize);
}
static int fstream_eof(stream_t* s)
{
  return feof((FILE*)s->x);
}
static int fstream_close(stream_t* s)
{
  FILE* f = (FILE*)s->x;
  bzero(s, sizeof(stream_t));
  return fclose(f);
}
static int fstream_tell(stream_t* s)
{
  return ftell((FILE*)s->x);
}
static int fstream_get8(stream_t* s)
{
  return fgetc((FILE*)s->x);
}
static char* fstream_gets(stream_t* s, char* buf, int bufsize)
{
  return fgets(buf, bufsize, (FILE*)s->x);
}
static int fstream_unget8(int ch, stream_t* s)
{
  return ungetc(ch, (FILE*)s->x);
}
static int fstream_put8(stream_t* s, int x)
{
  return fputc(x, (FILE*)s->x);
}
static int fstream_seek(stream_t* s, int offset, int origin)
{
  return fseek((FILE*)s->x, offset, origin);
}
static int fstream_read(stream_t* s, void* buf, int len)
{
  return fread(buf, 1, len, (FILE*)s->x);
}
static int fstream_write(stream_t* s, const void* buf, int len)
{
  return fwrite(buf, 1, len, (FILE*)s->x);
}
static int fstream_init(stream_t* s, FILE* pf)
{
  if (pf) {
    stream_init_func(s, f);
    s->x = pf;
  }
  return pf!=NULL;
}
static int memstream_eof(stream_t* s)
{
  return s->x1 >= s->x2;
}
static int memstream_close(stream_t* s)
{
  bzero(s, sizeof(stream_t));
  return 0;
}
static int memstream_tell(stream_t* s)
{
  return s->x1;
}
static int memstream_seek(stream_t* s, int offset, int origin)
{
  switch (origin) {
  case SEEK_SET:
    s->x1 = offset;
    break;
  case SEEK_CUR:
    s->x1 += offset;
    s->x1 = BOUND(s->x1, 0, s->x2);
    break;
  case SEEK_END:
    s->x1 = s->x2 - offset;
    break;
  default:
    break;
  }
  s->x1 = BOUND(s->x1, 0, s->x2);
  return 0;
}
static int memstream_get8(stream_t* s)
{
  uchar* f = (uchar*)s->x;
  return f[s->x1++];
}
static char* memstream_gets(stream_t* s, char* buf, int bufsize)
{
  int i;
  uchar* f = s->x1 + (uchar*)s->x;
  int len = s->x2-s->x1;
  len = MIN(len, bufsize-1);
  for (i=0; i<len; ++i) {
    buf[i] = f[i];
    if (f[i]!='\n') {
      break;
    }
  }
  buf[i+1] = 0;
  return buf;
}
static int memstream_unget8(int ch, stream_t* s)
{
  if (s->x1>0) {
    s->x1--;
  }
  return s->x1;
}
static int memstream_put8(stream_t* s, int x)
{
  uchar* f = (uchar*)s->x;
  f[s->x1++] = x;
  return 0;
}
static int memstream_read(stream_t* s, void* buf, int len)
{
  uchar* f = (uchar*)s->x;
  int n = 0;
  if (s->x1 < s->x2) {
    n = MIN(s->x2 - s->x1, len);
    n = BOUND(n, 0, s->x2);
    memcpy(buf, f + s->x1, n);
    s->x1 += n;
    s->x1 = BOUND(s->x1, 0, s->x2);
  }
  return n;
}
static int memstream_write(stream_t* s, const void* buf, int len)
{
  uchar* f = (uchar*)s->x;
  int n;
  s->x1 = BOUND(s->x1, 0, s->x2);
  n = MIN(s->x2 - s->x1, len);
  n = BOUND(n, 0, s->x2);
  memcpy(f + s->x1, buf,  n);
  s->x1 += n;
  return n;
}
static int memstream_init(stream_t* s, void* buf, int len)
{
  stream_init_func(s, mem);
  s->x = buf, s->x1 = 0, s->x2 = len;
  return 0;
}

static int stream_get16be(stream_t* s)
{
  int z = stream_get8(s);
  return (z << 8) + stream_get8(s);
}
static uint32 stream_get32be(stream_t* s)
{
  uint32 z = stream_get16be(s);
  return (z << 16) | stream_get16be(s);
}
static int stream_get16le(stream_t* s)
{
  int z = stream_get8(s);
  return z | (stream_get8(s) << 8);
}
static uint32 stream_get32le(stream_t* s)
{
  uint32 z = stream_get16le(s);
  return z | (stream_get16le(s) << 16);
}
static int stream_put16be(stream_t* s, int val)
{
  stream_put8(s, val >> 8);
  stream_put8(s, val);
  return 2;
}
static uint32 stream_put32be(stream_t* s, int val)
{
  stream_put8(s, val >> 24);
  stream_put8(s, val >> 16);
  stream_put8(s, val >> 8);
  stream_put8(s, val);
  return 4;
}
static int stream_put16le(stream_t* s, int val)
{
  stream_put8(s, val);
  stream_put8(s, val >> 8);
  return 2;
}
static uint32 stream_put32le(stream_t* s, int val)
{
  stream_put8(s, val);
  stream_put8(s, val >> 8);
  stream_put8(s, val >> 16);
  stream_put8(s, val >> 24);
  return 4;
}
static int stream_savefile(stream_t* s, const char* fname, int size) {
  char buf[10240] = {0};
  int i;
  FILE* pf = NULL;
  mkdirs(fname);
  if ((pf = fopen(fname, "wb"))!=NULL) {
    for (i=0; i<size; ) {
      int sz = MIN((int)(size-i), (int)countof(buf));
      int readsz = stream_read(s, buf, sz);
      if (readsz>0) {
        int wrt = fwrite(buf, 1, readsz, pf);
        i += readsz;
      } else {
        break;
      }
    }
    fclose(pf);
  }
  return 0;
}
#define filefind_is_directory(s)  (FILE_ATTRIBUTE_DIRECTORY & s->info.dwFileAttributes)
#define filefind_filename(s)      (s->info->name)
#define filefind_filesize(info)   (s->info.nFileSizeLow)
// finename
typedef struct dirlist_t {
  fileinfo_t* v;
  int n;
} dirlist_t;
CC_INLINE int dirlist_free(dirlist_t* s)
{
  FREE(s->v);
  s->v = NULL;
  s->n = 0;
  return 0;
}
CC_INLINE int file_exist_in_path(const char* rootpath, const char* path, const char* fn, char* fullpath, int maxpathlen, char* pathfn) {
  if (path) {
    _snprintf(fullpath, maxpathlen, "%s/%s/%s", rootpath, path, fn);
  } else {
    _snprintf(fullpath, maxpathlen, "%s/%s", rootpath, fn);
  }
  if (file_exist(fullpath)) {
    if (pathfn) {
      if (path) {
        _snprintf(pathfn, maxpathlen, "%s/%s", path, fn);
      } else {
        strncpy(pathfn, fn, maxpathlen);
      }
    }
    return 1;
  }
  return 0;
}
CC_INLINE int file_exist_in_path_list(const vstr_t* rootpath, const char* path, const char* fn, char* fullpath, int maxpathlen, char* pathfn) {
  int i;
  for (i=0; i<rootpath->n; ++i) {
    if (file_exist_in_path(rootpath->v[i].s, path, fn, fullpath, maxpathlen, pathfn)) {
      return 1;
    }
  }
  return 0;
}
// 把连续的 delims[字符集合] 用一个ch 代替
// isnot - 对delims求反
CC_INLINE int cstr_normalization(char* s, int l, const char* delims, int ch, int isnot) {
  int i, j=0, t = 0;
  uchar* us = (uchar*)s;
  uchar256 usmap1;
  CSTRINITLEN(s, l);
  cstr_initmap(usmap1, delims, -1, isnot, !isnot);
  for (i=0; i<l; ++i) {
    if (usmap1[us[i]]) {
      if (0==t) {
        s[j++] = s[i];
      }
      t = 1;
    } else {
      s[j++] = s[i];
      t = 0;
    }
  }
  s[j] = 0;
  return j;
}
CC_INLINE int path_split(const char* s, int i, int l, str_t* ps) {
  for (; i<l && (s[i]=='/'||s[i]=='\\');++i);
  ps->s = (char*)s + i;
  for (; i<l && (s[i]!='/'&& s[i]!='\\');++i);
  ps->l = (s+i)-ps->s;
  return i;
}
CC_INLINE int path_normalization2(const char* s, int l, char* o) {
  int i=0, j=0;
  str_t ps[1] = {0};
  int slashpos[256] = {0}, nslash=0;
  CSTRINITLEN(s, l);
  for (;i<l;) {
    i = path_split(s, i, l, ps);
    if (ps->l>0) {
      int isdot = 1==ps->l && '.'==ps->s[0];
      int isdot2 = 2==ps->l && '.'==ps->s[0] && '.'==ps->s[1];
      if (isdot2) {
        if (nslash>0 && slashpos[nslash-1]>=0) {
          j = slashpos[--nslash];
        } else {
          slashpos[nslash++] = -1;
          memcpy(o+j, ps->s, ps->l);
          j+=ps->l;
          o[j++]='/';
        }
      } else if (!isdot) {
        slashpos[nslash++] = j;
        if (j>0) {
          o[j++]='/';
        }
        memcpy(o+j, ps->s, ps->l);
        j+=ps->l;
      }
    }
  }
  o[j]=0;
  return j;
}
CC_INLINE int path_normalization(char* s, int l) {
  return path_normalization2(s, l, s);
}
CC_INLINE BOOL ext_match_filters(const char* fn, const char* filters) {
  const char* ext = GetFileExt(fn);
  return *ext ? (cstr_splitfind(filters, -1, ext, -1, true, NULL)>=0) : FALSE;
}
CC_INLINE int find_next_file(findinfo_t* s, const char* path, const char* filters, fileinfo_t* f, int flag) {
  if (flag & FF_FILTERS) {
    int ret = 0;
    while((ret = sys_find_next_file(s, path, "*.*", f, flag))!=0) {
      path_normalization(f->name, -1);
      if (!S_ISDIR(f->attrib) && ext_match_filters(f->name, filters)) {
        return 1;
      }
    }
    return ret;
  }
  return sys_find_next_file(s, path, filters, f, flag);
}

#ifdef _WIN32
#define S_IFMT         _S_IFMT           /* file type mask */
#define S_IFDIR        _S_IFDIR          /* directory */
#define S_IFCHR        _S_IFCHR          /* character special */
#define S_IFIFO        _S_IFIFO          /* pipe */
#define S_IFREG        _S_IFREG          /* regular */
#define S_IREAD        _S_IREAD          /* read permission, owner */
#define S_IWRITE       _S_IWRITE         /* write permission, owner */
#define S_IEXEC        _S_IEXEC          /* execute/search permission, owner */
#endif

static int is_directory(const char* path) {
  struct stat info = {0};
  int ret = stat(path, &info);
  int IsDirPath = 0;
  if (ret>=0) {
    IsDirPath = (S_IFDIR == (S_IFDIR & info.st_mode)); 
  }
  return IsDirPath;
}
static int rmdirs(const char* path) {
  if(is_directory(path)) {
    findinfo_t finfo[1] = {0};
    fileinfo_t info[1] = {0};
    int flag = FF_FULLNAME;
    const char* filters = "";
    for (; sys_find_next_file(finfo, path, "*", info, flag);) {
      BOOL isok = true;
      //printf("%s %d\n", info->name, info->attrib);
      if (info->attrib&AS_SUBDIR) {
        char* name = GetFileNameExt(info->name);
        isok = !FN_IS_DOTS(name);
        if (isok) {
          rmdirs(info->name);
        }
      } else {
        sys_rmfile(info->name);
      }
    }
    sys_rmdir(path);
  } else {
    sys_rmfile(path);
  }
  return 0;
}
// flag - FF_FULLNAME | FF_SUBDIR
static int dirlist(dirlist_t* s, const char* path, const char* filters, int flag)
{
  findinfo_t finfo[1] = {0};
  fileinfo_t info[1] = {0};
  for (; sys_find_next_file(finfo, path, "*", info, flag);) {
    BOOL isok = true;
  //printf("%s %d\n", info->name, info->attrib);
    if (info->attrib&AS_SUBDIR) {
      char* name = GetFileNameExt(info->name);
      if (!FN_IS_DOTS(name)) {
        if ((flag & FF_SUBDIR) && filters) {
          dirlist(s, info->name, filters, flag);
        }
        isok = !(flag & FF_NODIR);
      } else {
        isok = false;
      }
    }
    {
      char* ext = GetFileExt(info->name);
      if (filters && *filters) {
        isok = false;
        if (ext && *ext) {
          isok = cstr_find(filters, -1, ext, -1, 1, 1)>=0;
        }
      }
      if (isok) {
        int n = s->n++;
        MYREALLOC(s->v, s->n);
        s->v[n] = *info;
      }
    }
  }
  return 0;
}
static int dirvstr(vstr_t* sv, const char* path, const char* filters, int flag) {
  dirlist_t dl[1] = {0};
  int i;
  dirlist(dl, path, filters, flag);
  vstr_setsize(sv, dl->n);
  for (i=0; i<dl->n; ++i) {
    vstr_set_str(sv, i, STR1(dl->v[i].name));
  }
  dirlist_free(dl);
  return 0;
}
static int dirlist_c(char*** plist, const char* path, const char* filters, int flag) {
  dirlist_t s[1] = {0};
  int i;
  dirlist(s, path, filters, flag);
  vcstr_setsize(plist, s->n);
  for (i=0; i<s->n; ++i) {
    cstr_redup((*plist)+i, s->v[i].name);
  }
  dirlist_free(s);
  return 0;
}
static int dirlistW(dirlist_t* s, const char* path, const char* filters, int flag)
{
  findinfo_t finfo[1] = {0};
  fileinfo_t info[1] = {0};
  for (; sys_find_next_file(finfo, path, "*", info, flag);) {
    BOOL isok = true;
    if (S_ISDIR(info->attrib)) {
      char* name = GetFileNameExt(info->name);
      isok = !FN_IS_DOTS(name);
      if (isok && (flag & FF_SUBDIR) && filters) {
        dirlist(s, info->name, filters, flag);
      }
    }
    if (isok) {
      char* ext = strrchr(info->name, '.');
      isok = ext ? (cstr_find(filters, -1, ext, -1, 1, 0)>0) : 0;
    }
    if (isok) {
      int n = s->n++;
      MYREALLOC(s->v, s->n);
      s->v[n] = *info;
    }
  }
  return 0;
}

#define is_dots(info)  ((S_ISDIR(info->attrib)) && \
    (info->name[0] == '.') && (info->name[1] == '\0' || \
        (info->name[1] == '.' && info->name[2] == '\0')) )
typedef void (*fileproc_t)(const char* path, fileinfo_t* info, void* userdata);
static char* slipfilename(const char* path)
{
  char* end;
  char* end1 = (char*)strrchr(path, ('/'));
  char* end2 = (char*)strrchr(path, ('\\'));
  end = end1 > end2 ? end1 : end2;
  if (0 == end) {
    end = (char*) path;
  }
  else {
    end += 1;
  }
  assert(NULL != end);
  return end;
}
static char* slipdir(const char* path, char* dir)
{
  char* end;
  end = slipfilename(path);
  if (dir) {
    memcpy(dir, path, end - path);
    dir[ end - path ] = 0;
  }
  return dir;
}
static char* fileext(const char* filename)
{
  char* ext;
  ext = (char*)strrchr(filename, ('\\'));
  if (NULL == ext) {
    ext = (char*)strrchr(filename, ('/'));
  }
  ext = (char*)((NULL == ext) ? (filename) : (ext + 1));
  return ext;
}
static char* mfullname(const char* path, const char* name)
{
  static char fname[ 256 ];
  strcpy(fname, path);
  strcpy(fileext(fname), name);
  return fname;
}
#define _tcscat2(_D, _S1, _S2)            strcat(_D, _S1), strcat(_D, _S2)
#define _tcscat3(_D, _S1, _S2, _S3)       strcat(_D, _S1), _tcscat2(_D, _S2, _S3)
#define _tcscat4(_D, _S1, _S2, _S3, _S4)  strcat(_D, _S1), _tcscat3(_D, _S2, _S3, _S4)
static void findfile_1(const char* path, const char* filter, fileproc_t fileproc, unsigned int flag, void* userdata)
{
  findinfo_t finfo[1] = {0};
  fileinfo_t info[1];
  for (; find_next_file(finfo, path, filter, info, flag);) {
    fileproc((path), info, userdata);
  }
  if (flag & FF_SUBDIR) {
    char* name;
    findinfo_t finfo1[1] = {0};
    for (; sys_find_next_file(finfo1, path, "*.*", info, flag);) {
      if (S_ISDIR(info->attrib) && (name = GetFileNameExt(info->name)) && !FN_IS_DOTS(name)) {
        findfile_1(info->name, filter, fileproc, flag, userdata);
      }
    }
  }
}

typedef int (*txtfileproc_t)(char* s, char* send, char* d, char** pd);
static int findtxtfile(const char* infile, const char* outfile, txtfileproc_t fileproc)
{
  int len;
  char* buf, * out, *outend;
  FILE* po;
  buf = (char*) filemap(infile, &len);
  out = (char*) pmalloc(2 * len + 10);
  fileproc(buf, buf + len, out, &outend);
  po = fopen(outfile, ("wb"));
  fwrite(buf, len, 1, po);
  fwrite(out, outend - out, 1, po);
  fclose(po);
  pfree(buf);
  pfree(out);
  return 0;
}
#define STRDUP(_STR)  (char*)memcpy(pmalloc(strlen(_STR)+1), _STR, strlen(_STR)+1)
static void __filelist(const char* path, fileinfo_t* info, void* userdata)
{
  const char* picname;
  vstr_t* sv = (vstr_t*)userdata;
  picname = info->name;
  picname = mfullname(path, picname);
  vstr_ins_str(sv, -1, STR1(picname));
}
#define get_file_list(_SPEC, _SLIST)  ((_SLIST)->strs=0, findfile( _SPEC, __filelist, _FF_SUB | _FF_FULLNAME, _SLIST ))
static int get_file_list_txt(const char* txtfile, vstr_t* sv)
{
  char buf[4096];
  FILE* txt = fopen(txtfile, "rb");
  if (txt) {
    for (; fgets(buf, sizeof(buf), txt);) {
      char* send = strend(buf);
      for (; buf < send && ('\n' == send[-1] || '\r' == send[-1] || ' ' == send[-1]); --send);
      send[0] = '\0';
      if ('\0' != buf[0]) {
        vstr_ins_str(sv, -1, STR1(buf));
      }
    }
  }
  return 0;
}
CC_INLINE time_t filettime(const char* path)
{
  //char buf[65];
  struct stat sb;
  stat(path, &sb);
  //sb.st_ctime 
  //struct tm* t = localtime(&sb.st_ctime);
  //sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
  return sb.st_ctime;
}
CC_INLINE double timetof64(time_t time1) {
  double t;
  struct tm tm1;
#ifdef _WIN32
  tm1 = *localtime(&time1);  
#else  
  localtime_r(&time1, &tm1);
#endif
  t = tm1.tm_year+1900.;
  t = t*100 + tm1.tm_mon+1;
  t = t*100 + tm1.tm_mday;
  t = t*100 + tm1.tm_hour;
  t = t*100 + tm1.tm_min;
  t = t*100 + tm1.tm_sec;
  return t;
}
// 得到几天前的时间
CC_INLINE double time_before(int year, int mon, int day, int hour, int minv, int sec) {
  time_t time1 = time(NULL);
  double t;
  struct tm tm1;
#if defined _WIN32 || defined _WIN64
  tm1 = *localtime(&time1);  
#else  
  localtime_r(&time1, &tm1);
#endif
  t = tm1.tm_year+1900. - year;
  t = t*100 + tm1.tm_mon+1-mon;
  t = t*100 + tm1.tm_mday-day;
  t = t*100 + tm1.tm_hour-hour;
  t = t*100 + tm1.tm_min-minv;
  t = t*100 + tm1.tm_sec-sec;
  return t;
}
//指定time_t类型的时间，格式化为YYYYMMDDHH24MISS型的字符串  
CC_INLINE char* timetoa(time_t time1, char* szTime)  
{  
  struct tm tm1;
#if defined _WIN32 || defined _WIN64
  tm1 = *localtime(&time1);  
#else  
  localtime_r(&time1, &tm1 );  
#endif
  sprintf( szTime, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
    tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,  
    tm1.tm_hour, tm1.tm_min,tm1.tm_sec);  
  return szTime;
}

//指定YYYYMMDDHH24MISS型的时间，格式化为time_t型的时间  
CC_INLINE time_t atotime(const char * szTime) {
  struct tm tm1;
  time_t time1;
  sscanf(szTime, "%4d%2d%2d%2d%2d%2d",
    &tm1.tm_year,
    &tm1.tm_mon,
    &tm1.tm_mday,
    &tm1.tm_hour,
    &tm1.tm_min,
    &tm1.tm_sec);
  
  tm1.tm_year -= 1900;  
  tm1.tm_mon --;
  tm1.tm_isdst=-1;
  time1 = mktime(&tm1);
  return time1;  
}  

#ifndef ASSERT
#define ASSERT assert
#endif // ASSERT
static char* GetFirstFile(const char* fn, char* fltr, int isdir)
{
  static char szFirstFile[256] = {0};
  char* exts = fltr;
  char* ext;
  findinfo_t finfo[1] = {0};
  fileinfo_t info[1];
  snprintf(szFirstFile, 256, "%s", _GetFilePath(fn));
  while (sys_find_next_file(finfo, szFirstFile, "*.*", info, 0)) {
    if (isdir) {
      if ((S_ISDIR(info->attrib)) && '.' != info->name[0]) {
        break;
      }
    }
    else if (!(S_ISDIR(info->attrib))) {
      ext = GetFileExt(info->name);
      if (0 != strlen(ext) && strstr(exts, (const char*)ext) != 0) {
        break;
      }
    }
  }
  if ('.' == info->name[0]) {
    return 0;
  }
  snprintf(szFirstFile, 256, "%s\\%s", _GetFilePath(fn), info->name);
  return szFirstFile;
}
static char* GetNextFile(const char* fn, char* fltr, int isdir)
{
  static char szNextFile[256] = "";
  char* exts = fltr;
  char* name;
  char* ext;
  findinfo_t finfo[1] = {0};
  fileinfo_t info[1] = {0};
  snprintf(szNextFile, 256, "%s", _GetFilePath(fn));
  if (0 == strlen(szNextFile)) {
    return szNextFile;
  }
  name = GetFileNameExt(fn);
  if (0 == strlen(name)) {
	  szNextFile[0] = 0;
    return szNextFile;
  }
  while (sys_find_next_file(finfo, szNextFile, "*.*", info, 0)) {
    if (0 == strcmp(name, info->name)) {
      break;
    }
  }
  while (sys_find_next_file(finfo, szNextFile, "*.*", info, 0)) {
    if (isdir) {
      if (S_ISDIR(info->attrib)) {
        break;
      }
    }
    else if (!(S_ISDIR(info->attrib))) {
      ext = GetFileExt(info->name);
      if (exts || (0 != strlen(ext) && strstr(exts, ext) != 0)) {
        break;
      }
    }
  }
  if ('.' == info->name[0]) {
    return 0;
  }
  snprintf(szNextFile, 256, "%s\\%s", _GetFilePath(fn), info->name);
  return szNextFile;
}
static char* GetPrevFile(char* fn, char* fltr, int isdir)
{
  static char szPrevFile[256] = "";
  char str2[256] = {0};
  char* exts = fltr;
  char* name;
  char* ext;
  findinfo_t finfo[1] = {0};
  fileinfo_t info[1] = {0};
  snprintf(szPrevFile, 256, "%s\\", _GetFilePath(fn));
  if (0 == strlen(szPrevFile)) {
    return szPrevFile;
  }
  name = GetFileNameExt(fn);
  if (0 == strlen(name)) {
	  szPrevFile[0] = 0;
    return szPrevFile;
  }
  *szPrevFile = 0;
  while (sys_find_next_file(finfo, szPrevFile, "*.*", info, 0)) {
    if (0 == strcmp(name, info->name)) {
      break;
    }
    ext = GetFileExt(info->name);
    if (isdir) {
      if (S_ISDIR(info->attrib)) {
        strcpy(str2, info->name);
      }
    }
    else if (!(S_ISDIR(info->attrib))) {
      if (exts || (0 != strlen(ext) && strstr(exts, ext) != 0)) {
        strcpy(str2, info->name);
      }
    }
  }
  if ('.' == str2[0]) {
    return 0;
  }
  snprintf(szPrevFile, 256, "%s\\%s", _GetFilePath(fn), str2);
  return szPrevFile;
}
#define SKIP_WHITESPACE(f, l)   for (;f<l && (' '==*f ||'\t'==*f ||'\r'==*f ||'\n'==*f); ++f)
#define RSKIP_WHITESPACE(f, l)  for (;f<l && (' '==l[-1] ||'\t'==l[-1] ||'\r'==l[-1] ||'\n'==l[-1]); --l)
static int str2filename(const char* f, int len, char* buf, int buflen)
{
  static const char pp[] = ",/\\<>?/:|*\"";
  int i = 0, j = 0;
  for (; ' ' == f[i] || '\t' == f[i]; ++i);
  for (; j < (buflen - 1) && f[i] && '\n' != f[i] && '\r' != f[i]; ++i) {
    buf[j++] = (strchr(pp, f[i]) || (f[i] > 0 && f[i] < 10))  ? '_' : f[i];
  }
  buf[j] = 0;
  return j;
}

#if 0

#define BUF_SERIALIZATION_V_CASE(NAME, TYPE)  \
 case NAME : { TYPE* p = va_arg(args, TYPE*); \
 if (save) {  if (data) { d = data + ipos; va_arg(d, TYPE) = *p; }\
  } else { d = data + ipos; *p = va_arg(d, TYPE); \
  } ipos += sizeof(TYPE);\
} break

static int buf_serialization_v(char* data, int ipos, bool save, const char* fmt, va_list args) {
  char c;
  int level=0;
  char* d;
  while ((c = *fmt++)!=0) {
    switch(c) {
      BUF_SERIALIZATION_V_CASE('c', char);
      BUF_SERIALIZATION_V_CASE('s', short);
      BUF_SERIALIZATION_V_CASE('i', int);
      BUF_SERIALIZATION_V_CASE('q', int64);
      BUF_SERIALIZATION_V_CASE('f', float);
      BUF_SERIALIZATION_V_CASE('d', double);
    case 'p':
      {
        uint* pn = va_arg(args, uint*);
        char** pp = va_arg(args, char**);
        if (save) {
          if (data) {
            d = data + ipos;
            va_arg(d, uint) = *pn;
            memcpy(d, *pp, *pn);
          }
        } else {
          d = data + ipos;
          *pn = va_arg(d, uint);
          *pp = d;
        }
        ipos += sizeof(uint) + *pn;
      }
      break;
    }
  }
  return ipos;
}
static int buf_serialization(char* data, int ipos, bool save, const char* fmt, ... ) {
  va_list arglist;
  va_start(arglist, fmt);
  ipos = buf_serialization_v(data, ipos, save, fmt, arglist);
  va_end(arglist);
  return ipos;
}
#endif

typedef stream_t file_t;

#define f_seek  stream_seek
#define f_read  stream_read
#define f_write stream_write

static int df_loadstr(file_t* pf, int pos, str_t* header) {
  int ret=0;
  int len = 0;
  f_seek(pf, pos, SEEK_SET);
  if (sizeof(len)==f_read(pf, &len, sizeof(len))) {
    str_setsize(header, len);
    if (len==f_read(pf, header->s, len)) {
      ret = pos + len + sizeof(len);
    }
  }
  return ret;
}
static int df_savestr(file_t* pf, int pos, const char* header, u32 len) {
  int ret=0;
  f_seek(pf, pos, SEEK_SET);
  f_write(pf, &len, sizeof(len));
  f_write(pf, header, len);
  ret = pos + len + sizeof(len);
  return ret;
}
#define IMAGEFMTSTR "dim=[%d, %d, %d, %d]"
static int df_saveimage(file_t* pf, int pos, const img_t* im) {
  char header[128];
  int len=0;
  _snprintf(header, countof(header), IMAGEFMTSTR, im->c, im->w, im->h, im->f);
  len = strlen(header);
  pos = df_savestr(pf, pos, header, len);
  {
    int i, j;
    for (i=0; i<im->f; ++i) {
      uchar* data = im->tt.data + i*im->h*im->s;
      for (j=0; j<im->h; ++j) {
        uchar* data1 = data + j*im->s;
        pos += f_write(pf, data1, im->c*im->w);
      }
    }
  }
  return pos;
}
static int df_loadimage(file_t* pf, int pos, img_t* im) {
  str_t header[1] = {0};
  int c, w, h, f;
  c = w = h = f = 0;
  pos = df_loadstr(pf, pos, header);
  sscanf(header->s, IMAGEFMTSTR, &c, &w, &h, &f);
  imsetsize(im, h, w, c, f);
  {
    int i, j;
    for (i=0; i<im->f; ++i) {
      uchar* data = im->tt.data + i*im->h*im->s;
      for (j=0; j<im->h; ++j) {
        uchar* data1 = data + j*im->s;
        pos += f_read(pf, data1, im->c*im->w);
      }
    }
  }
  str_free(header);
  return pos;
}
#define df_saveimage_filename(fn, im)  df_saveimage_file_close(fopen(fn, "wb"), im)
static int df_saveimage_file_close(FILE* pf, const img_t* im) {
  file_t f[1] = {0};
  if (fstream_init(f, pf)) {
    df_saveimage(f, 0, im);
    fclose(pf);
    return 1;
  }
  return 0;
}
#define df_loadimage_filename(fn, im)  df_loadimage_file_close(fopen(fn, "rb"), im)
static int df_loadimage_file_close(FILE* pf, img_t* im) {
  file_t f[1] = {0};
  if (fstream_init(f, pf)) {
    df_loadimage(f, 0, im);
    fclose(pf);
    return 1;
  }
  return 0;
}


/****************************************************************************************\
*             Data structures for persistence (a.k.a serialization) functionality        *
\****************************************************************************************/

/** "black box" file storage */
struct CFileStorage;
//typedef void (*CvParse)( struct CFileStorage* fs );
#define CFILESTRUCT_FUNDEFDEF(FUNDEF) \
FUNDEF(void, WriteStructBegin, ( struct CFileStorage* fs, const char* key, const char* type_name )) \
FUNDEF(void, WriteStructEnd, ( struct CFileStorage* fs )) \
FUNDEF(void, Write, ( struct CFileStorage* fs, const char* key, const char* data )) \
FUNDEF(void, WriteInt, ( struct CFileStorage* fs, const char* key, int value )) \
FUNDEF(void, WriteReal, ( struct CFileStorage* fs, const char* key, double value )) \
FUNDEF(void, WriteString, ( struct CFileStorage* fs, const char* key, const char* value, int quote )) \
FUNDEF(void, WriteComment, ( struct CFileStorage* fs, const char* comment, int eol_comment ))


typedef struct CFileStruct {
#define FUNDEF(ret, name, args)  ret (* name) args;
  CFILESTRUCT_FUNDEFDEF(FUNDEF)
#undef FUNDEF
} CFileStruct;


/** Storage flags: */
#define CC_STORAGE_READ          0
#define CC_STORAGE_WRITE         1
#define CC_STORAGE_WRITE_TEXT    CC_STORAGE_WRITE
#define CC_STORAGE_WRITE_BINARY  CC_STORAGE_WRITE
#define CC_STORAGE_APPEND        2
#define CC_STORAGE_MEMORY        4
#define CC_STORAGE_FORMAT_MASK   (7<<3)
#define CC_STORAGE_FORMAT_AUTO   0
#define CC_STORAGE_FORMAT_XML    8
#define CC_STORAGE_FORMAT_YAML  16
#define CC_STORAGE_FORMAT_JSON  24
#define CC_STORAGE_BASE64       64
#define CC_STORAGE_WRITE_BASE64  (CC_STORAGE_BASE64 | CC_STORAGE_WRITE)

struct CTypeInfo;
#if 0
#define CC_NODE_NONE        0
#define CC_NODE_INT         1
#define CC_NODE_INTEGER     CC_NODE_INT
#define CC_NODE_REAL        2
#define CC_NODE_FLOAT       CC_NODE_REAL
#define CC_NODE_STR         3
#define CC_NODE_STRING      CC_NODE_STR
#define CC_NODE_REF         4 /**< not used */
#define CC_NODE_SEQ         5
#define CC_NODE_MAP         6
#define CC_NODE_TYPE_MASK   7

#define CC_NODE_TYPE(flags)  ((flags) & CC_NODE_TYPE_MASK)

/** file node flags */
#define CC_NODE_FLOW        8 /**<Used only for writing structures in YAML format. */
#define CC_NODE_USER        16
#define CC_NODE_EMPTY       32
#define CC_NODE_NAMED       64

#define CC_NODE_IS_INT(flags)        (CC_NODE_TYPE(flags) == CC_NODE_INT)
#define CC_NODE_IS_REAL(flags)       (CC_NODE_TYPE(flags) == CC_NODE_REAL)
#define CC_NODE_IS_STRING(flags)     (CC_NODE_TYPE(flags) == CC_NODE_STRING)
#define CC_NODE_IS_SEQ(flags)        (CC_NODE_TYPE(flags) == CC_NODE_SEQ)
#define CC_NODE_IS_MAP(flags)        (CC_NODE_TYPE(flags) == CC_NODE_MAP)
#define CC_NODE_IS_COLLECTION(flags) (CC_NODE_TYPE(flags) >= CC_NODE_SEQ)
#define CC_NODE_IS_FLOW(flags)       (((flags) & CC_NODE_FLOW) != 0)
#define CC_NODE_IS_EMPTY(flags)      (((flags) & CC_NODE_EMPTY) != 0)
#define CC_NODE_IS_USER(flags)       (((flags) & CC_NODE_USER) != 0)
#define CC_NODE_HAS_NAME(flags)      (((flags) & CC_NODE_NAMED) != 0)

#define CC_NODE_SEQ_SIMPLE 256
#define CC_NODE_SEQ_IS_SIMPLE(seq) (((seq)->flags & CC_NODE_SEQ_SIMPLE) != 0)
#endif
/** All the keys (names) of elements in the readed file storage
   are stored in the hash to speed up the lookup operations: */
typedef struct CStringHashNode
{
    unsigned hashval;
    str_t str;
    struct CStringHashNode* next;
}
CStringHashNode;

typedef struct CGenericHash
{
    //CC_SET_FIELDS()
    mem_block_t* storage;
    int tab_size;
    void** table;
}
CGenericHash;

typedef struct CGenericHash CFileNodeHash;
typedef CGenericHash CStringHash;

/** Basic element of the file storage - scalar or collection: */
typedef struct CFileNode
{
    int tag;
    // struct CTypeInfo* info; /**< type information (only for user-defined object, for others it is 0) */
    unsigned hashval;
    str_t str;
    struct CFileNode *sub;
    struct CFileNode *next;
}
CFileNode;

typedef struct CFileMapNode
{
    CFileNode value;
    const CStringHashNode* key;
    struct CFileMapNode* next;
}
CFileMapNode;


typedef int (*CIsInstanceFunc)( const void* struct_ptr );
typedef void (*CReleaseFunc)( void** struct_dblptr );
typedef void* (*CReadFunc)( struct CFileStorage* storage, CFileNode* node );
typedef void (*CWriteFunc)( struct CFileStorage* storage, const char* name, const void* struct_ptr, vstr_t* attributes );
typedef void* (*CCloneFunc)( const void* struct_ptr );

/** @brief Type information

The structure contains information about one of the standard or user-defined types. Instances of the
type may or may not contain a pointer to the corresponding CTypeInfo structure. In any case, there
is a way to find the type info structure for a given object using the cvTypeOf function.
Alternatively, type info can be found by type name using cvFindType, which is used when an object
is read from file storage. The user can register a new type with cvRegisterType that adds the type
information structure into the beginning of the type list. Thus, it is possible to create
specialized types from generic standard types and override the basic methods.
 */
typedef struct CTypeInfo
{
    int flags; /**< not used */
    int header_size; /**< sizeof(CTypeInfo) */
    struct CTypeInfo* prev; /**< previous registered type in the list */
    struct CTypeInfo* next; /**< next registered type in the list */
    const char* type_name; /**< type name, written to file storage */
    CIsInstanceFunc is_instance; /**< checks if the passed object belongs to the type */
    CReleaseFunc release; /**< releases object (memory etc.) */
    CReadFunc read; /**< reads object from file storage */
    CWriteFunc write; /**< writes object to file storage */
    CCloneFunc clone; /**< creates a copy of the object */
}
CTypeInfo;

typedef struct CXMLStackRecord
{
    //CvMemStoragePos pos;
    str_t struct_tag;
    int struct_indent;
    int struct_flags;
}
CXMLStackRecord;

typedef struct CTextInfo {
    int lineno;
    mem_block_t* storage;
} CTextInfo;

typedef struct CFileStorage
{
    int write_mode;

    int new_line;
    CXMLStackRecord write_stack[20];
    int write_stack_top;
    int struct_indent;
    str_t struct_tag;
    stream_t file[1];
    CFileStruct* vfun;
}
CFileStorage;

static char* getDefaultObjectName(CFileStorage* fs, const char* _filename, char* name_buf)
{
	static const char* stubname = "unnamed";
	const char* filename = _filename;
	int _filename_size = strlen(_filename);
	const char* ptr2 = filename + _filename_size;
	const char* ptr = ptr2 - 1;
	//CAutoBuffer<char> name_buf(_filename.size()+1);

	while (ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':')
	{
		if (*ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0))
			ptr2 = ptr;
		ptr--;
	}
	ptr++;
	if (ptr == ptr2)
		CC_Error(CC_StsBadArg, "Invalid filename");

	char* name = name_buf;

	// name must start with letter or '_'
	if (!cc_isalpha(*ptr) && *ptr != '_') {
		*name++ = '_';
	}

	while (ptr < ptr2)
	{
		char c = *ptr++;
		if (!cc_isalnum(c) && c != '-' && c != '_')
			c = '_';
		*name++ = c;
	}
	*name = '\0';
	name = name_buf;
	if (strcmp(name, "_") == 0)
		strcpy(name, stubname);
	return (name);
}

#endif // _FILEIO_H_

