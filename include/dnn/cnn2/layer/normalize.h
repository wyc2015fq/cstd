

#ifndef LAYER_NORMALIZE_H
#define LAYER_NORMALIZE_H

#include "layer.h"

namespace ncnn
{

struct Normalize : public Layer
{
public:
  Normalize();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

public:
  // param
  int across_spatial;
  int channel_shared;
  float eps;
  int scale_data_size;

  Blob scale_data;
};

} // namespace ncnn

#endif // LAYER_NORMALIZE_H
