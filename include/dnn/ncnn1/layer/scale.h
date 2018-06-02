

#ifndef LAYER_SCALE_H
#define LAYER_SCALE_H

#include "layer.h"

namespace ncnn
{

struct Scale : public Layer
{
public:
  Scale();
  virtual ~Scale();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;

public:
  // param
  int scale_data_size;
  int bias_term;

  // model
  img_t scale_data;
  img_t bias_data;
};

} // namespace ncnn

#endif // LAYER_SCALE_H
