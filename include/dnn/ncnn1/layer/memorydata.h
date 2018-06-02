

#ifndef LAYER_MEMORYDATA_H
#define LAYER_MEMORYDATA_H

#include "layer.h"

namespace ncnn
{

struct MemoryData : public Layer
{
public:
  MemoryData();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const;

public:
  int w;
  int h;
  int c;

  img_t data;
};

} // namespace ncnn

#endif // LAYER_MEMORYDATA_H
