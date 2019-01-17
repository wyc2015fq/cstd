

#ifndef _BLOBDATA_H_
#define _BLOBDATA_H_

#include "types.h"

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

struct DataInfo {
  DimType dimtype_;
  TypeFlag type_;
  DataShape shape;
};

int mnist_read_head(FILE* pf, DataInfo* info) {
  uint32_t magic, num = 1, rows = 1, cols = 1, cn = 1;
  magic = read_uint32(pf);
  switch (magic) {
  case 2053:
    info->dimtype_ = (DimType)read_uint32(pf);
    info->type_ = (TypeFlag)read_uint32(pf);
    num = read_uint32(pf);
    rows = read_uint32(pf);
    cols = read_uint32(pf);
    cn = read_uint32(pf);
    break;
  case 2051:
    info->dimtype_ = NCHW;
    info->type_ = TF_U8;
    num = read_uint32(pf);
    rows = read_uint32(pf);
    cols = read_uint32(pf);
    break;
  case 2049:
    info->dimtype_ = NCHW;
    info->type_ = TF_U8;
    num = read_uint32(pf);
    break;
  default:
    return 0;
  }
  info->shape = dataShape(num, cn, rows, cols);
  return 1;
}

#endif // _BLOBDATA_H_
