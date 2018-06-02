
#include "cstd.h"

#define kMaxBlobAxes    MAX_DIM
#define MAX_BLOB_BUFF_NUM    16

#define MAXNAME 32
struct Blob {
  char name[MAXNAME];
  union {
    DataSize size;
    struct {int n, c, h, w;};
  };
#if 1
  float* data;
  float* diff;
  float* bufs[MAX_BLOB_BUFF_NUM];
#else
  udata_t data[1];
  udata_t diff[1];
  udata_t bufs[MAX_BLOB_BUFF_NUM];
#endif
};
int Blob_channels(const struct Blob* const blob) {return blob->c;}
const float* getData(const struct Blob* const blob)
{
  return (float*)blob->data;
}
const float* getDiff(const struct Blob* const blob)
{
  return (float*)blob->diff;
}
const float* getBuff(const struct Blob* const blob, int i)
{
  return (float*)blob->bufs[i];
}
float* setData(struct Blob* blob)
{
  return reset(&blob->data, blob->size);
}
float* setDiff(struct Blob* blob)
{
  return reset(&blob->diff, blob->size);
}
float* setBuff(struct Blob* blob, int i)
{
  return reset(&blob->bufs[i], blob->size);
}
float* regetData(struct Blob* blob)
{
  return blob->data;
}
float* regetDiff(struct Blob* blob)
{
  return blob->diff;
}
float* regetBuff(struct Blob* blob, int i)
{
  return blob->bufs[i];
}
float* setDataZero(struct Blob* blob)
{
  return resetZero(&blob->data, blob->size);
}
float* setDiffZero(struct Blob* blob)
{
  return resetZero(&blob->diff, blob->size);
}
float* setBuffZero(struct Blob* blob, int i)
{
  return resetZero(&blob->bufs[i], blob->size);
}
float* resetData(struct Blob* blob)
{
  return blob->data ? blob->data : setDataZero(blob);
}
float* resetDiff(struct Blob* blob)
{
  return blob->diff ? blob->diff : setDiffZero(blob);
}
float* resetBuff(struct Blob* blob, int i)
{
  return blob->bufs[i] ? blob->bufs[i] : setBuffZero(blob, i);
}
CC_INLINE void Blob_free(struct Blob* blob)
{
  int i;
  free(blob->data);
  free(blob->diff);
  for (i=0; i<MAX_BLOB_BUFF_NUM; ++i) {
    free(blob->bufs[i]);
  }
  memset(blob, 0, sizeof(struct Blob));
}
CC_INLINE void Blob_frees(struct Blob* blob, int n)
{
  for (; --n>=0; ) {
    Blob_free(blob+n);
  }
}
static void Blob_Update(struct Blob* blob) {
  blas_saxpy(Data_total(blob->size), (float)(-1), blob->diff, blob->data);
}


enum {
  PIXEL_CONVERT_SHIFT = 16,
  PIXEL_FORMAT_MASK = 0x0000ffff,
  PIXEL_CONVERT_MASK = 0xffff0000,

  PIXEL_RGB       = 1,
  PIXEL_BGR       = (1 << 1),
  PIXEL_GRAY      = (1 << 2),
  PIXEL_RGBA      = (1 << 3),

  PIXEL_RGB2BGR   = PIXEL_RGB | (PIXEL_BGR << PIXEL_CONVERT_SHIFT),
  PIXEL_RGB2GRAY  = PIXEL_RGB | (PIXEL_GRAY << PIXEL_CONVERT_SHIFT),

  PIXEL_BGR2RGB   = PIXEL_BGR | (PIXEL_RGB << PIXEL_CONVERT_SHIFT),
  PIXEL_BGR2GRAY  = PIXEL_BGR | (PIXEL_GRAY << PIXEL_CONVERT_SHIFT),

  PIXEL_GRAY2RGB  = PIXEL_GRAY | (PIXEL_RGB << PIXEL_CONVERT_SHIFT),
  PIXEL_GRAY2BGR  = PIXEL_GRAY | (PIXEL_BGR << PIXEL_CONVERT_SHIFT),

  PIXEL_RGBA2RGB  = PIXEL_RGBA | (PIXEL_RGB << PIXEL_CONVERT_SHIFT),
  PIXEL_RGBA2BGR  = PIXEL_RGBA | (PIXEL_BGR << PIXEL_CONVERT_SHIFT),
  PIXEL_RGBA2GRAY = PIXEL_RGBA | (PIXEL_GRAY << PIXEL_CONVERT_SHIFT),
};
