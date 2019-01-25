

#ifndef _BLOBDATA_H_
#define _BLOBDATA_H_

//#include "types.h"
#include "std/types_c.h"
//#include "std/algo.h"

typedef void void_type;
typedef void void_float;

struct int8x4 { unsigned char v[4]; };
//DEF(U8x4, int8x4, CUDNN_DATA_INT8x4)

#define TYPEFLAGDEF_DEF(DEF) \
DEF(S8, char, CUDNN_DATA_INT8) \
DEF(U8, unsigned char, CUDNN_DATA_UINT8) \
DEF(S16, short, CUDNN_DATA_HALF) \
DEF(U16, unsigned short, CUDNN_DATA_HALF) \
DEF(S32, int, CUDNN_DATA_INT32) \
DEF(U32, unsigned int, CUDNN_DATA_INT32) \
DEF(F32, float, CUDNN_DATA_FLOAT) \
DEF(F64, double, CUDNN_DATA_DOUBLE)

enum TypeFlag //枚举消息类型
{
#define TYPEFLAGDEF(a, b, c)  TF_ ## a,
  TYPEFLAGDEF_DEF(TYPEFLAGDEF)
#undef TYPEFLAGDEF
};

static const int TypeSize[] = {
#define TYPEFLAGDEF(a, b, c)  sizeof(b),
  TYPEFLAGDEF_DEF(TYPEFLAGDEF)
#undef TYPEFLAGDEF
};

enum DimType { NCHW, NHWC };

#define ENDIANNESS ((char)*(int*)"l??b")
#define BIGENDIANNESS ('b'==ENDIANNESS)

static uint32_t swap_endian(uint32_t val)
{
  val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
  return (val << 16) | (val >> 16);
}

static uint32_t read_uint32(FILE* pf) {
  uint32_t magic;
  fread(&magic, 4, 1, pf);
  if (!BIGENDIANNESS)
    magic = swap_endian(magic);
  return magic;
}
static size_t read_uint32(FILE* pf, uint32_t& x) {
  size_t ret = fread(&x, 4, 1, pf);
  if (!BIGENDIANNESS)
    x = swap_endian(x);
  return ret;
}
static size_t write_uint32(FILE* pf, uint32_t x) {
  if (!BIGENDIANNESS)
    x = swap_endian(x);
  fwrite(&x, 4, 1, pf);
  return 4;
}

#define MAX_LABEL_NUM  10
struct DataInfo {
  DimType dimtype_;
  TypeFlag type_;
  size_t n, c, h, w;
  uint32_t label_num;
  uint32_t label_dim[MAX_LABEL_NUM];
};
static int mnist_write_head(FILE* pf, DataInfo* info) {
  uint32_t i;
  //if (info->label_num > 0)
  {
    write_uint32(pf, 2055);
    write_uint32(pf, info->dimtype_);
    write_uint32(pf, info->type_);
    write_uint32(pf, info->label_num);
    write_uint32(pf, info->n);
    write_uint32(pf, info->h);
    write_uint32(pf, info->w);
    write_uint32(pf, info->c);
    for (i = 0; i < info->label_num; ++i) {
      write_uint32(pf, info->label_dim[i]);
    }
  }
  return (8 + info->label_num) * 4;
}
static int mnist_read_head(FILE* pf, DataInfo* info) {
  uint32_t magic, i;
  info->n = 1;
  info->c = 1;
  info->h = 1;
  info->w = 1;
  read_uint32(pf, magic);
  switch (magic) {
  case 2055:
    info->dimtype_ = (DimType)read_uint32(pf);
    info->type_ = (TypeFlag)read_uint32(pf);
    info->label_num = (TypeFlag)read_uint32(pf);
    info->n = read_uint32(pf);
    info->h = read_uint32(pf);
    info->w = read_uint32(pf);
    info->c = read_uint32(pf);
    for (i = 0; i < info->label_num; ++i) {
      info->label_dim[i] = read_uint32(pf);
    }
    return (8 + info->label_num)*4;
  case 2053:
    info->dimtype_ = (DimType)read_uint32(pf);
    info->type_ = (TypeFlag)read_uint32(pf);
    info->n = read_uint32(pf);
    info->h = read_uint32(pf);
    info->w = read_uint32(pf);
    info->c = read_uint32(pf);
    return 6 * 4;
  case 2051:
    info->dimtype_ = NCHW;
    info->type_ = TF_U8;
    info->n = read_uint32(pf);
    info->h = read_uint32(pf);
    info->w = read_uint32(pf);
    return 3 * 4;
  case 2049:
    info->dimtype_ = NCHW;
    info->type_ = TF_U8;
    info->n = read_uint32(pf);
    return 4;
  default:
    return 0;
  }
  return 0;
}

#endif // _BLOBDATA_H_
