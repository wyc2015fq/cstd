
#include "dir_c.h"

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
CC_INLINE int stream_puts(stream_t* fp, const char* str)
{
  int len = strlen(str);
  return stream_write(fp, str, len);
}
CC_INLINE int stream_rewind(stream_t* fp)
{
  return stream_seek(fp, 0, SEEK_SET);
}
CC_INLINE int stream_puts2(const char* str, stream_t* fp)
{
  int len = strlen(str);
  return stream_write(fp, str, len);
}
CC_INLINE char* stream_gets(char* buf, int bufsize, stream_t* fp)
{
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
  return pf != NULL;
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
  int len = s->x2 - s->x1;
  len = MIN(len, bufsize - 1);
  for (i = 0; i < len; ++i) {
    buf[i] = f[i];
    if (f[i] != '\n') {
      break;
    }
  }
  buf[i + 1] = 0;
  return buf;
}
static int memstream_unget8(int ch, stream_t* s)
{
  if (s->x1 > 0) {
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
static int stream_savefile(stream_t* s, const char* fname, int size)
{
  char buf[10240] = {0};
  int i;
  FILE* pf = NULL;
  mkdirs(fname);
  if ((pf = fopen(fname, "wb")) != NULL) {
    for (i = 0; i < size; ) {
      int sz = MIN((int)(size - i), (int)countof(buf));
      int readsz = stream_read(s, buf, sz);
      if (readsz > 0) {
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