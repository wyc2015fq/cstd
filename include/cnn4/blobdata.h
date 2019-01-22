

#ifndef _BLOBDATA_H_
#define _BLOBDATA_H_

#include "types.h"
//#include "std/algo.h"

#define ENDIANNESS ((char)*(int*)"l??b")
#define BIGENDIANNESS ('b'==ENDIANNESS)

static uint32_t swap_endian(uint32_t val)
{
  val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
  return (val << 16) | (val >> 16);
}

uint32_t read_uint32(FILE* pf) {
  uint32_t magic;
  fread(&magic, 4, 1, pf);
  if (!BIGENDIANNESS)
    magic = swap_endian(magic);
  return magic;
}
size_t read_uint32(FILE* pf, uint32_t& x) {
  size_t ret = fread(&x, 4, 1, pf);
  if (!BIGENDIANNESS)
    x = swap_endian(x);
  return ret;
}
size_t write_uint32(FILE* pf, uint32_t x) {
  if (!BIGENDIANNESS)
    x = swap_endian(x);
  fwrite(&x, 4, 1, pf);
  return 4;
}
#define MAX_LABEL_NUM  10
struct DataInfo {
  DimType dimtype_;
  TypeFlag type_;
  DataShape shape;
  uint32_t label_num;
  uint32_t label_dim[MAX_LABEL_NUM];
};
int mnist_write_head(FILE* pf, DataInfo* info) {
  uint32_t i;
  //if (info->label_num > 0)
  {
    write_uint32(pf, 2055);
    write_uint32(pf, info->dimtype_);
    write_uint32(pf, info->type_);
    write_uint32(pf, info->label_num);
    write_uint32(pf, info->shape.n);
    write_uint32(pf, info->shape.h);
    write_uint32(pf, info->shape.w);
    write_uint32(pf, info->shape.c);
    for (i = 0; i < info->label_num; ++i) {
      write_uint32(pf, info->label_dim[i]);
    }
  }
  return (8 + info->label_num) * 4;
}
int mnist_read_head(FILE* pf, DataInfo* info) {
  uint32_t magic, i;
  info->shape.set(1, 1, 1, 1);
  read_uint32(pf, magic);
  switch (magic) {
  case 2055:
    info->dimtype_ = (DimType)read_uint32(pf);
    info->type_ = (TypeFlag)read_uint32(pf);
    info->label_num = (TypeFlag)read_uint32(pf);
    info->shape.n = read_uint32(pf);
    info->shape.h = read_uint32(pf);
    info->shape.w = read_uint32(pf);
    info->shape.c = read_uint32(pf);
    for (i = 0; i < info->label_num; ++i) {
      info->label_dim[i] = read_uint32(pf);
    }
    return (8 + info->label_num)*4;
  case 2053:
    info->dimtype_ = (DimType)read_uint32(pf);
    info->type_ = (TypeFlag)read_uint32(pf);
    info->shape.n = read_uint32(pf);
    info->shape.h = read_uint32(pf);
    info->shape.w = read_uint32(pf);
    info->shape.c = read_uint32(pf);
    return 6 * 4;
  case 2051:
    info->dimtype_ = NCHW;
    info->type_ = TF_U8;
    info->shape.n = read_uint32(pf);
    info->shape.h = read_uint32(pf);
    info->shape.w = read_uint32(pf);
    return 3 * 4;
  case 2049:
    info->dimtype_ = NCHW;
    info->type_ = TF_U8;
    info->shape.n = read_uint32(pf);
    return 4;
  default:
    return 0;
  }
  return 0;
}

#endif // _BLOBDATA_H_
