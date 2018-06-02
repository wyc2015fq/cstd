

#ifndef LAYER_DECONVOLUTION_H
#define LAYER_DECONVOLUTION_H

#include "layer.h"

namespace ncnn
{

struct Deconvolution : public Layer
{
public:
  Deconvolution();
  virtual ~Deconvolution();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

public:
  // param
  int num_output;
  int kernel_size;
  int dilation;
  int stride;
  int pad;
  int bias_term;

  int weight_data_size;

  // model
  img_t weight_data;
  img_t bias_data;
};

} // namespace ncnn

#endif // LAYER_DECONVOLUTION_H
